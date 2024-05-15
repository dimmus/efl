// Compile with:
// gcc -o core_audio_to_ogg core_audio_to_ogg.c `pkg-config --libs --cflags ecore eina ecore-audio`
#ifndef EFL_BETA_API_SUPPORT
# define EFL_BETA_API_SUPPORT
#endif

#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <Efl_Eo.h>
#include <Efl_Core.h>
#include <Ecore_Audio.h>
#include <Efl_Shared.h>

double volume = 1;
Eina_List *inputs = NULL;

static void 
_play_finished(void *data EFL_UNUSED, const Efl_Event *event)
{
  const char *name;
  Eo *out;

  name = core_audio_obj_name_get(event->object);
  printf("Done: %s\n", name);

  out = core_audio_obj_in_output_get(event->object);
  efl_unref(event->object);
  efl_unref(out);

  core_main_loop_quit();
}

int
main(int argc, char *argv[])
{
   Eo *out;
   Eo *in;
   Efl_Bool ret;

   if (argc < 3)
     {
       printf("Usage: ./example <infile> <outfile>\n");
       exit(EXIT_FAILURE);
     }

   core_init();
   core_audio_init();


   in = efl_add_ref(CORE_AUDIO_IN_SNDFILE_CLASS, NULL);
   efl_name_set(in, basename(argv[1]));
   ret = core_audio_obj_source_set(in, argv[1]);
   if (!ret) {
     printf("Could not set %s as input\n", argv[1]);
     efl_unref(in);
     return 1;
   }

   efl_event_callback_add(in, CORE_AUDIO_IN_EVENT_IN_STOPPED, _play_finished, NULL);

   out = efl_add_ref(CORE_AUDIO_OUT_SNDFILE_CLASS, NULL);
   ret = core_audio_obj_source_set(out, argv[2]);
   if (!ret) {
     printf("Could not set %s as output\n", argv[2]);
     efl_unref(in);
     efl_unref(out);
     return 1;
   }

   ret = core_audio_obj_out_input_attach(out, in);
   if (!ret) {
     printf("Could not attach input\n");
     efl_unref(out);
     efl_unref(in);
     return 1;
   }

   core_main_loop_begin();

   core_audio_shutdown();
   core_shutdown();

   return 0;
}

