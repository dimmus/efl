#include <stdio.h>
#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Ecore_Evas.h>
#include <Ecore_Buffer.h>
#include <Ecore_Buffer_Queue.h>
#include <Evas.h>

#ifdef DEBUG
#define LOG(f, x...) printf("[PROVIDER|%30.30s|%04d] " f "\n", __func__, __LINE__, ##x)
#else
#define LOG(f, x...)
#endif

typedef struct _Provider_Data
{
   Ecore_Buffer_Provider *provider;
   Ecore_Buffer *buffer;
   Eina_List *buffer_list;
   Ecore_Job *render_job;
   Core_Idle_Enterer *post_render;
   unsigned int w, h;
} Provider_Data;

const char *name = "core_buffer_queue_test";

static void _provider_render_queue(Provider_Data *pd);

static void
shutdown_all(void)
{
   core_buffer_queue_shutdown();
   core_buffer_shutdown();
   core_evas_shutdown();
   core_shutdown();
   eina_shutdown();
}

static Efl_Bool
init_all(void)
{
   if (!eina_init()) goto err;
   if (!core_init()) goto err;
   if (!core_evas_init()) goto err;
   if (!core_buffer_init()) goto err;
   if (!core_buffer_queue_init()) goto err;

   return EFL_TRUE;
err:
   shutdown_all();
   return EFL_FALSE;
}

Ecore_Buffer *
_provider_buffer_get(Provider_Data *pd, unsigned int w, unsigned int h, unsigned int format)
{
   Ecore_Buffer *buffer = NULL;
   Ecore_Buffer_Return ret;
   unsigned int res_w, res_h, res_format;

   LOG("Dequeue");
   ret = core_buffer_provider_buffer_acquire(pd->provider, &buffer);

   if (ret == CORE_BUFFER_RETURN_NEED_ALLOC)
     {
        buffer = core_buffer_new("shm", w, h, format, 0);
        pd->buffer_list = eina_list_append(pd->buffer_list, buffer);
        LOG("No buffer in Queue, Create Buffer");
     }
   else if (ret == CORE_BUFFER_RETURN_SUCCESS)
     {
        core_buffer_size_get(buffer, &res_w, &res_h);
        res_format = core_buffer_format_get(buffer);
        if ((res_w != w) || (res_h != h) || (res_format != format))
          {
             LOG("Need to Reallocate Buffer, Free it First: %p", buffer);
             pd->buffer_list = eina_list_remove(pd->buffer_list, buffer);
             core_buffer_free(buffer);

             buffer = core_buffer_new("shm", w, h, format, 0);
             pd->buffer_list = eina_list_append(pd->buffer_list, buffer);
             LOG("Create Buffer: %p", buffer);
          }
     }

   return buffer;
}

static void
paint_pixels(void *image, int padding, int width, int height, uint32_t time)
{
   const int halfh = padding + (height - padding * 2) / 2;
   const int halfw = padding + (width  - padding * 2) / 2;
   int ir, or;
   uint32_t *pixel = image;
   int y;

   /* squared radii thresholds */
   or = (halfw < halfh ? halfw : halfh) - 8;
   ir = or - 32;
   or *= or;
   ir *= ir;

   pixel += padding * width;
   for (y = padding; y < height - padding; y++) {
        int x;
        int y2 = (y - halfh) * (y - halfh);

        pixel += padding;
        for (x = padding; x < width - padding; x++) {
             uint32_t v;

             /* squared distance from center */
             int r2 = (x - halfw) * (x - halfw) + y2;

             if (r2 < ir)
               v = (r2 / 32 + time / 64) * 0x0080401;
             else if (r2 < or)
               v = (y + time / 32) * 0x0080401;
             else
               v = (x + time / 16) * 0x0080401;
             v &= 0x00ffffff;
             v |= 0xff000000;

             *pixel++ = v;
        }

        pixel += padding;
   }
}

static Efl_Bool
_cb_render_post(void *data)
{
   Provider_Data *pd = (Provider_Data *)data;
   Ecore_Buffer *next_buffer = NULL;

   LOG("Startup - Post Render");

   LOG("Submit Buffer - buffer: %p", pd->buffer);
   core_buffer_provider_buffer_enqueue(pd->provider, pd->buffer);
   pd->buffer = NULL;

   next_buffer = _provider_buffer_get(pd, pd->w, pd->h, CORE_BUFFER_FORMAT_XRGB8888);
   if (next_buffer)
     {
        LOG("Drawable Buffer is Existed, ADD Render job again - buffer:%p", next_buffer);
        pd->buffer = next_buffer;
        _provider_render_queue(pd);
     }

   core_idle_enterer_del(pd->post_render);
   pd->post_render = NULL;

   LOG("Done - Post Render");

   return CORE_CALLBACK_RENEW;
}

static void
_provider_cb_render_job(void *data)
{
   Provider_Data *pd = (Provider_Data *)data;

   LOG("Startup - Render");

   if (!pd->buffer)
     {
        pd->buffer = _provider_buffer_get(pd,
                                          pd->w, pd->h, CORE_BUFFER_FORMAT_XRGB8888);
     }

   if (pd->buffer)
     {
        void *data;

        LOG("Success to get Drawable Buffer, Drawing now... - buffer:%p", pd->buffer);
        // Drawing...
        data = core_buffer_data_get(pd->buffer);
        paint_pixels(data, 0, pd->w, pd->h, core_loop_time_get() * 1000);

        if (!pd->post_render)
          {
             pd->post_render =
                core_idle_enterer_before_add(_cb_render_post, pd);
          }
     }

   core_job_del(pd->render_job);
   pd->render_job = NULL;
}

static void
_provider_render_queue(Provider_Data *pd)
{
   if (!pd) return;

   LOG("Render Queue");

   if (!pd->render_job)
     pd->render_job = core_job_add(_provider_cb_render_job, pd);
}

static void
_cb_consumer_add(Ecore_Buffer_Provider *provider EFL_UNUSED, int queue_size EFL_UNUSED, int w, int h, void *data)
{
   Provider_Data *pd = (Provider_Data *)data;

   LOG("Connected with Consumer, Now We can use Ecore_Buffer_Queue - queue_size:%d, geo(%dx%d)",
       queue_size, w, h);

   pd->w = w;
   pd->h = h;

   _provider_render_queue(pd);
}

static void
_cb_consumer_del(Ecore_Buffer_Provider *provider EFL_UNUSED, void *data EFL_UNUSED)
{
   LOG("Disconnected with Consumer, Shutdown Provider now.");

   core_main_loop_quit();
}

static void
_cb_buffer_released(Ecore_Buffer_Provider *provider EFL_UNUSED, void *data)
{
   Provider_Data *pd = (Provider_Data *)data;

   LOG("Buffer Enqueued");

   _provider_render_queue(pd);
}

int
main(void)
{
   Provider_Data *pd;

   if (!init_all())
     {
        LOG("Initializing failed");
        return -1;
     }

   pd = (Provider_Data *)calloc(1, sizeof(Provider_Data));

   if (!pd || !(pd->provider = core_buffer_provider_new(name)))
     {
        LOG("Failed to create provider");
        goto shutdown;
     }

   core_buffer_provider_consumer_add_cb_set(pd->provider, _cb_consumer_add, pd);
   core_buffer_provider_consumer_del_cb_set(pd->provider, _cb_consumer_del, pd);
   core_buffer_provider_buffer_released_cb_set(pd->provider, _cb_buffer_released, pd);

   core_main_loop_begin();

shutdown:
   if (pd->buffer_list)
     {
        Ecore_Buffer *b;

        EINA_LIST_FREE(pd->buffer_list, b)
           core_buffer_free(b);
     }
   if (pd->provider) core_buffer_provider_free(pd->provider);
   free(pd);

   shutdown_all();
   return 0;
}
