#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Core_Wl.h"

#include "core_wayland_internal.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "linux-dmabuf-unstable-v1-client-protocol.h"

#define MAX_BUFFERS         4
#define QUEUE_TRIM_DURATION 100

int EFL_CORE_WAYLAND_SURFACE_DMABUF = 0;

typedef struct _Efl_Core_Wayland_Dmabuf_Private
{
  Efl_Core_Wayland_Buffer *current;
  Eina_List               *buffers;
  int                      unused_duration;
} Efl_Core_Wayland_Dmabuf_Private;

static void *
_evas_dmabuf_surface_setup(Efl_Core_Wayland_Window *win)
{
  Efl_Core_Wayland_Dmabuf_Private *priv;
  Efl_Core_Wayland_Display        *ewd;
  Efl_Core_Wayland_Buffer_Type     types = 0;

  priv = calloc(1, sizeof(*priv));
  if (!priv) return NULL;

  ewd = efl_core_wayland_window_display_get(win);
  if (efl_core_wayland_display_shm_get(ewd))
    types |= EFL_CORE_WAYLAND_BUFFER_SHM;
  if (efl_core_wayland_display_dmabuf_get(ewd))
    types |= EFL_CORE_WAYLAND_BUFFER_DMABUF;

  if (!efl_core_wayland_buffer_init(ewd, types))
  {
    free(priv);
    return NULL;
  }

  return priv;
}

static void
_evas_dmabuf_surface_reconfigure(Efl_Core_Wayland_Surface *s EFL_UNUSED,
                                 void                       *priv_data,
                                 int                         w,
                                 int                         h,
                                 uint32_t flags              EFL_UNUSED,
                                 Efl_Bool alpha              EFL_UNUSED)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;
  Eina_List                       *l, *tmp;
//   Efl_Bool alpha_change;

  p = priv_data;

  if ((!w) || (!h)) return;
//   alpha_change = efl_core_wayland_surface_alpha_get(s) != alpha;
  EINA_LIST_FOREACH_SAFE(p->buffers, l, tmp, b)
  {
/*      This would be nice, but requires a partial create to follow,
        and that partial create is buffer type specific.

        if (!alpha_change && efl_core_wayland_buffer_fit(b, w, h))
          continue;
*/
    efl_core_wayland_buffer_destroy(b);
    p->buffers = eina_list_remove_list(p->buffers, l);
  }
}

static void *
_evas_dmabuf_surface_data_get(Efl_Core_Wayland_Surface *s EFL_UNUSED,
                              void                       *priv_data,
                              int                        *w,
                              int                        *h)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;
  void                            *ptr;
  int                              stride;

  p = priv_data;

  b = p->current;
  if (!b) return NULL;

  ptr = efl_core_wayland_buffer_map(b, NULL, h, &stride);
  if (!ptr) return NULL;

   /* We return stride/bpp because it may not match the allocated
    * width.  evas will figure out the clipping
    */
  if (w) *w = stride / 4;

  return ptr;
}

static Efl_Core_Wayland_Buffer *
_evas_dmabuf_surface_wait(Efl_Core_Wayland_Surface        *s,
                          Efl_Core_Wayland_Dmabuf_Private *p)
{
  Efl_Core_Wayland_Buffer *b, *best = NULL;
  Eina_List               *l;
  int                      best_age = -1;
  int                      age;
  int                      num_required = 1, num_allocated = 0;

  EINA_LIST_FOREACH(p->buffers, l, b)
  {
    num_allocated++;
    if (efl_core_wayland_buffer_busy_get(b))
    {
      num_required++;
      continue;
    }
    age = efl_core_wayland_buffer_age_get(b);
    if (age > best_age)
    {
      best     = b;
      best_age = age;
    }
  }

  if (num_required < num_allocated) p->unused_duration++;
  else p->unused_duration = 0;

   /* If we've had unused buffers for longer than QUEUE_TRIM_DURATION, then
    * destroy the oldest buffer (currently in best) and recursively call
    * ourself to get the next oldest.
    */
  if (best && (p->unused_duration > QUEUE_TRIM_DURATION))
  {
    p->unused_duration = 0;
    p->buffers         = eina_list_remove(p->buffers, best);
    efl_core_wayland_buffer_destroy(best);
    best = _evas_dmabuf_surface_wait(s, p);
  }

  if (!best && (eina_list_count(p->buffers) < MAX_BUFFERS))
  {
    best = efl_core_wayland_surface_buffer_create(s);
        /* Start at -1 so it's age is incremented to 0 for first draw */
    efl_core_wayland_buffer_age_set(best, -1);
    p->buffers = eina_list_append(p->buffers, best);
  }
  return best;
}

static int
_evas_dmabuf_surface_assign(Efl_Core_Wayland_Surface *s, void *priv_data)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;
  Eina_List                       *l;

  p          = priv_data;
  p->current = _evas_dmabuf_surface_wait(s, p);
  if (!p->current)
  {
        /* Should be unreachable and will result in graphical
         * anomalies - we should probably blow away all the
         * existing buffers and start over if we actually
         * see this happen...
         */
        //        WRN("No free DMAbuf buffers, dropping a frame");
    EINA_LIST_FOREACH(p->buffers, l, b)
      efl_core_wayland_buffer_age_set(b, 0);
    return 0;
  }
  EINA_LIST_FOREACH(p->buffers, l, b)
    efl_core_wayland_buffer_age_inc(b);

  return efl_core_wayland_buffer_age_get(p->current);
}

static void
_evas_dmabuf_surface_post(Efl_Core_Wayland_Surface *s,
                          void                     *priv_data,
                          Eina_Rectangle           *rects,
                          unsigned int              count)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;
  Efl_Core_Wayland_Window         *win;
  struct wl_buffer                *wlb;

  p = priv_data;

  b = p->current;
  if (!b) return;

  efl_core_wayland_buffer_unlock(b);

  p->current = NULL;
  efl_core_wayland_buffer_busy_set(b);
  efl_core_wayland_buffer_age_set(b, 0);

  win = efl_core_wayland_surface_window_get(s);

  wlb = efl_core_wayland_buffer_wl_buffer_get(b);
  efl_core_wayland_window_buffer_attach(win, wlb, 0, 0, EFL_FALSE);
  efl_core_wayland_window_damage(win, rects, count);

  efl_core_wayland_window_commit(win, EFL_TRUE);
}

static void
_evas_dmabuf_surface_destroy(Efl_Core_Wayland_Surface *s EFL_UNUSED,
                             void                       *priv_data)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;

  p = priv_data;

  EINA_LIST_FREE(p->buffers, b)
  efl_core_wayland_buffer_destroy(b);

  free(p);
}

static void
_evas_dmabuf_surface_flush(Efl_Core_Wayland_Surface *surface EFL_UNUSED,
                           void                             *priv_data,
                           Efl_Bool                          purge)
{
  Efl_Core_Wayland_Dmabuf_Private *p;
  Efl_Core_Wayland_Buffer         *b;

  p = priv_data;

  EINA_LIST_FREE(p->buffers, b)
  {
    if (purge || !efl_core_wayland_buffer_busy_get(b))
    {
      if (p->current == b) p->current = NULL;
      efl_core_wayland_buffer_destroy(b);
    }
  }
}

static Efl_Core_Wayland_Surface_Interface dmabuf_smanager = {
  .version     = 1,
  .setup       = _evas_dmabuf_surface_setup,
  .destroy     = _evas_dmabuf_surface_destroy,
  .reconfigure = _evas_dmabuf_surface_reconfigure,
  .data_get    = _evas_dmabuf_surface_data_get,
  .assign      = _evas_dmabuf_surface_assign,
  .post        = _evas_dmabuf_surface_post,
  .flush       = _evas_dmabuf_surface_flush
};

Efl_Bool
efl_core_wayland_surface_module_dmabuf_init(void)
{
  EFL_CORE_WAYLAND_SURFACE_DMABUF =
    efl_core_wayland_surface_manager_add(&dmabuf_smanager);

  if (EFL_CORE_WAYLAND_SURFACE_DMABUF < 1) return EFL_FALSE;

  return EFL_TRUE;
}

void
efl_core_wayland_surface_module_dmabuf_shutdown(void)
{
  efl_core_wayland_surface_manager_del(&dmabuf_smanager);
}

EINA_MODULE_INIT(efl_core_wayland_surface_module_dmabuf_init);
EINA_MODULE_SHUTDOWN(efl_core_wayland_surface_module_dmabuf_shutdown);
