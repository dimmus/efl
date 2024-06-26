#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <math.h>

#include <Efl_Core.h>
#include <Core_Audio.h>
#include <Core_File.h>

#include "efl_core_suite.h"

static void
_finished_cb(void *data EFL_UNUSED, const Efl_Event *event EFL_UNUSED)
{
  core_main_loop_quit();
}

static void
_looped_cb(void *data EFL_UNUSED, const Efl_Event *event)
{
  core_audio_obj_in_looped_set(event->object, EFL_FALSE);
}

#ifdef HAVE_PULSE

static void
_failed_cb(void *data, const Efl_Event *event EFL_UNUSED)
{
  Efl_Bool *pulse_context_failed = data;

  if (pulse_context_failed)
  {
    int pa_check          = system("pulseaudio --check");
    *pulse_context_failed = (pa_check == 0);
  }
  core_main_loop_quit();
}

static Efl_Bool
_seek_vol(void *data)
{
  double len = 0;
  Eo    *in  = data;
  Eo    *out = NULL;

  out = core_audio_obj_in_output_get(in);

  core_audio_obj_volume_set(out, 0.4);
  len = core_audio_obj_in_seek(in, -0.3, SEEK_END);
  fail_if(len < 0);

  return EFL_FALSE;
}

EFL_START_TEST(core_test_core_audio_obj_pulse)
{
  Eo      *in, *out;
  Efl_Bool ret                  = EFL_FALSE;
  Efl_Bool pulse_context_failed = EFL_FALSE;

  in = efl_add_ref(CORE_AUDIO_IN_SNDFILE_CLASS, NULL);
  fail_if(!in);

  efl_name_set(in, "sample.wav");
  ret = core_audio_obj_source_set(in, TESTS_SRC_DIR "/sample.wav");
  fail_if(!ret);

  out = efl_add_ref(CORE_AUDIO_OUT_PULSE_CLASS, NULL);
  fail_if(!out);

  core_timer_add(1.8, _seek_vol, in);

  efl_event_callback_add(in,
                         CORE_AUDIO_IN_EVENT_IN_STOPPED,
                         _finished_cb,
                         NULL);
  efl_event_callback_add(out,
                         CORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                         _failed_cb,
                         &pulse_context_failed);

  ret = core_audio_obj_out_input_attach(out, in);
  fail_if(!ret);

  core_main_loop_begin();
  fail_if(pulse_context_failed);

  efl_unref(out);
  efl_unref(in);
}

EFL_END_TEST
#endif

static Efl_Bool
_quit(void *data EFL_UNUSED)
{
  core_main_loop_quit();

  return EFL_FALSE;
}

static Efl_Bool
_idle_del(void *data)
{
  Eo *in = data;
  efl_unref(in);
  core_idler_add(_quit, NULL);

  return EFL_FALSE;
}

EFL_START_TEST(core_test_core_audio_cleanup)
{
  Eo      *in, *out;
  int      freq = 1000;
  Efl_Bool ret  = EFL_FALSE;

  in = efl_add_ref(CORE_AUDIO_IN_TONE_CLASS, NULL);
  fail_if(!in);
  efl_key_data_set(in, CORE_AUDIO_ATTR_TONE_FREQ, &freq);
  core_audio_obj_in_length_set(in, 2);

  out = efl_add_ref(CORE_AUDIO_OUT_SNDFILE_CLASS, NULL);
  fail_if(!out);
  ret = core_audio_obj_format_set(out, CORE_AUDIO_FORMAT_OGG);
  fail_if(!ret);
  ret = core_audio_obj_source_set(out, TESTS_BUILD_DIR "/tmp.ogg");
  fail_if(!ret);

  ret = core_audio_obj_out_input_attach(out, in);
  fail_if(!ret);

  core_idler_add(_idle_del, in);

  core_main_loop_begin();

  core_file_remove(TESTS_BUILD_DIR "/tmp.ogg");
}

EFL_END_TEST

EFL_START_TEST(core_test_core_audio_obj_tone)
{
  Eo      *in, *out;
  double   len;
  int      channel, rate, freq;
  Efl_Bool ret;
  char    *tmp;

  in = efl_add_ref(CORE_AUDIO_IN_TONE_CLASS, NULL);
  fail_if(!in);

  efl_name_set(in, "tone");

  channel = core_audio_obj_in_channels_get(in);
  fail_if(channel != 1);
  rate = core_audio_obj_in_samplerate_get(in);
  fail_if(rate != 44100);
  len = core_audio_obj_in_length_get(in);
  fail_if(!EINA_DBL_EQ(len, 1));

  core_audio_obj_in_length_set(in, 2.5);
  len = core_audio_obj_in_length_get(in);
  fail_if(!EINA_DBL_EQ(len, 2.5));

  core_audio_obj_in_looped_set(in, EFL_TRUE);

  len = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(len, 2.5));

  freq = (intptr_t)efl_key_data_get(in, CORE_AUDIO_ATTR_TONE_FREQ);
  fail_if(freq != 1000);

  freq = 2000;
  efl_key_data_set(in, CORE_AUDIO_ATTR_TONE_FREQ, &freq);

  freq = (intptr_t)efl_key_data_get(in, CORE_AUDIO_ATTR_TONE_FREQ);
  fail_if(freq != 2000);

  efl_key_data_set(in, "foo", "bar");
  tmp = efl_key_data_get(in, "foo");
  ck_assert_str_eq(tmp, "bar");

  len = core_audio_obj_in_seek(in, 5.0, SEEK_SET);
  fail_if(!EINA_DBL_EQ(len, -1));

  len = core_audio_obj_in_seek(in, 1.0, 42);
  fail_if(!EINA_DBL_EQ(len, -1));

  len = core_audio_obj_in_seek(in, 1.0, SEEK_SET);
  fail_if(!EINA_DBL_EQ(len, 1.0));

  len = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(len, 1.5));

  len = core_audio_obj_in_seek(in, 1.0, SEEK_CUR);
  fail_if(!EINA_DBL_EQ(len, 2.0));

  len = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(len, 0.5));

  len = core_audio_obj_in_seek(in, -1.0, SEEK_END);
  fail_if(!EINA_DBL_EQ(len, 1.5));

  len = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(len, 1.0));

  out = efl_add_ref(CORE_AUDIO_OUT_SNDFILE_CLASS, NULL);
  fail_if(!out);

  efl_name_set(out, "tmp.wav");
  ret = core_audio_obj_format_set(out, CORE_AUDIO_FORMAT_WAV);
  fail_if(!ret);
  ret = core_audio_obj_source_set(out, TESTS_BUILD_DIR "/tmp.wav");
  fail_if(!ret);

  ret = core_audio_obj_out_input_attach(out, in);
  fail_if(!ret);

  efl_event_callback_add(in, CORE_AUDIO_IN_EVENT_IN_LOOPED, _looped_cb, NULL);
  efl_event_callback_add(in,
                         CORE_AUDIO_IN_EVENT_IN_STOPPED,
                         _finished_cb,
                         NULL);

  core_main_loop_begin();

  efl_unref(in);
  efl_unref(out);

   //TODO: Compare and fail
  core_file_remove(TESTS_BUILD_DIR "/tmp.wav");
}

EFL_END_TEST

EFL_START_TEST(core_test_core_audio_obj_sndfile)
{
  Eo               *in, *out;
  double            len, rem;
  int               channel, rate;
  Efl_Bool          ret;
  Core_Audio_Format fmt;
  const char       *src;

  in = efl_add_ref(CORE_AUDIO_IN_SNDFILE_CLASS, NULL);
  fail_if(!in);

  fmt = core_audio_obj_format_get(in);
  fail_if(fmt != CORE_AUDIO_FORMAT_AUTO);

  ret = core_audio_obj_format_set(in, CORE_AUDIO_FORMAT_FLAC);
  fail_if(!ret);

  fmt = core_audio_obj_format_get(in);
  fail_if(fmt != CORE_AUDIO_FORMAT_FLAC);

  ret = core_audio_obj_format_set(in, CORE_AUDIO_FORMAT_AUTO);
  fail_if(!ret);

  efl_name_set(in, "sample.ogg");
  ret = core_audio_obj_source_set(in, TESTS_SRC_DIR "/sample.ogg");
  fail_if(!ret);

  src = core_audio_obj_source_get(in);
  ck_assert_str_eq(src, TESTS_SRC_DIR "/sample.ogg");

  fmt = core_audio_obj_format_get(in);
  fail_if(fmt != CORE_AUDIO_FORMAT_OGG);

  channel = core_audio_obj_in_channels_get(in);
  fail_if(channel != 2);
  rate = core_audio_obj_in_samplerate_get(in);
  fail_if(rate != 44100);
  len = core_audio_obj_in_length_get(in);
  fail_if(EINA_DBL_EQ(len, 0));
  rem = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(len, rem));

  fmt = core_audio_obj_format_get(in);
  fail_if(fmt != CORE_AUDIO_FORMAT_OGG);

  len = core_audio_obj_in_seek(in, 0.5, SEEK_SET);
  fail_if(!EINA_DBL_EQ(len, 0.5));

  len = core_audio_obj_in_seek(in, 1.0, SEEK_CUR);
  fail_if(!EINA_DBL_EQ(len, 1.5));

  len = core_audio_obj_in_seek(in, -1.5, SEEK_END);
  fail_if(fabs(rem - 1 - len) > 0.6);

  out = efl_add_ref(CORE_AUDIO_OUT_SNDFILE_CLASS, NULL);
  fail_if(!out);

  efl_name_set(out, "tmp.wav");
  ret = core_audio_obj_format_set(out, CORE_AUDIO_FORMAT_WAV);
  fail_if(!ret);

  fmt = core_audio_obj_format_get(out);
  fail_if(fmt != CORE_AUDIO_FORMAT_WAV);

  //   ret = core_audio_obj_source_set(out, "/tmp/file/does/not/exist/hopefully.wav");
  //   fail_if(ret);

  ret = core_audio_obj_source_set(out, TESTS_BUILD_DIR "/tmp.wav");
  fail_if(!ret);

  src = core_audio_obj_source_get(out);
  ck_assert_str_eq(src, TESTS_BUILD_DIR "/tmp.wav");

  ret = core_audio_obj_out_input_attach(out, in);
  fail_if(!ret);

  efl_event_callback_add(in,
                         CORE_AUDIO_IN_EVENT_IN_STOPPED,
                         _finished_cb,
                         NULL);

  core_main_loop_begin();

  efl_unref(in);
  efl_unref(out);

  //TODO: Compare and fail
  core_file_remove(TESTS_BUILD_DIR "/tmp.wav");
}

EFL_END_TEST

#include "core_audio_out_test.eo.h"
#include "core_audio_out_test.eo.c"

EFL_START_TEST(core_test_core_audio_obj_in_out)
{
  Eo        *out2;
  Eina_List *in3;
  Efl_Bool   attached;

  Eo *in  = efl_add_ref(CORE_AUDIO_IN_CLASS, NULL);
  Eo *in2 = efl_add_ref(CORE_AUDIO_IN_CLASS, NULL);
  Eo *out = efl_add_ref(CORE_AUDIO_OUT_TEST_CLASS, NULL);

  fail_if(!in);
  fail_if(!in2);
  fail_if(!out);

  out2 = core_audio_obj_in_output_get(in);

  fail_if(out2);

  in3 = core_audio_obj_out_inputs_get(out);

  fail_if(eina_list_count(in3) != 0);

  attached = core_audio_obj_out_input_attach(out, in);
  fail_if(!attached);

  attached = core_audio_obj_out_input_attach(out, in);
  fail_if(attached);

  out2 = core_audio_obj_in_output_get(in);

  fail_if(out2 != out);

  in3 = core_audio_obj_out_inputs_get(out);

  fail_if(eina_list_count(in3) != 1);
  fail_if(eina_list_data_get(in3) != in);

  attached = core_audio_obj_out_input_attach(out, in2);
  fail_if(!attached);

  in3 = core_audio_obj_out_inputs_get(out);

  fail_if(eina_list_count(in3) != 2);
  fail_if(eina_list_data_get(in3) != in);

  efl_unref(in2);

  in3 = core_audio_obj_out_inputs_get(out);

  fail_if(eina_list_count(in3) != 1);
  fail_if(eina_list_data_get(in3) != in);

  efl_unref(out);

  out2 = core_audio_obj_in_output_get(in);

  fail_if(out2);

  efl_unref(in);
}

EFL_END_TEST

static int
read_cb(void *data EFL_UNUSED, Eo *eo_obj EFL_UNUSED, void *buffer, int len)
{
  static int i = 0;
  int        j;
  uint8_t   *buf = buffer;

  for (j = 0; j < len; j++)
  {
    buf[j] = i++ % 256;
  }
  return len;
}

static int
write_cb(void *data  EFL_UNUSED,
         Eo *eo_obj  EFL_UNUSED,
         const void *buffer,
         int         len)
{
  static int     i = 0;
  int            j;
  const uint8_t *buf = buffer;

  for (j = 0; j < len; j++)
  {
    if (buf[j] != i % 256) fail_if(buf[j] != i % 256);
    i++;
  }

  if (i > 100000) core_main_loop_quit();

  return len;
}

Core_Audio_Vio in_vio = {
  .read = read_cb,
};

Core_Audio_Vio out_vio = {
  .write = write_cb,
};

EFL_START_TEST(core_test_core_audio_obj_vio)
{
  Eo *in, *out;

  in = efl_add_ref(CORE_AUDIO_IN_CLASS, NULL);
  fail_if(!in);

  out = efl_add_ref(CORE_AUDIO_OUT_TEST_CLASS, NULL);
  fail_if(!out);

  core_audio_obj_vio_set(in, &in_vio, NULL, NULL);
  core_audio_obj_vio_set(out, &out_vio, NULL, NULL);

  core_audio_obj_out_input_attach(out, in);

  core_main_loop_begin();

  efl_unref(out);
  efl_unref(in);
}

EFL_END_TEST

static void
_myfree(void *data)
{
  Efl_Bool *freed = data;

  *freed = EFL_TRUE;
}

EFL_START_TEST(core_test_core_audio_obj_in)
{
  int      i;
  double   speed, length;
  int      samplerate, channels;
  Efl_Bool looped;

  ssize_t read;
  uint8_t buf[10];

  Core_Audio_Vio vio;
  Efl_Bool       freed = EFL_FALSE;

  Eo *in = efl_add_ref(CORE_AUDIO_IN_CLASS, NULL);

  fail_if(!in);

  core_audio_obj_vio_set(in, &vio, &freed, _myfree);
  fail_if(freed);

  core_audio_obj_vio_set(in, NULL, NULL, NULL);
  fail_if(!freed);

  speed = core_audio_obj_in_speed_get(in);
  fail_if(!EINA_DBL_EQ(speed, 1.0));

  core_audio_obj_in_speed_set(in, 2.5);

  speed = core_audio_obj_in_speed_get(in);
  fail_if(!EINA_DBL_EQ(speed, 2.5));

  core_audio_obj_in_speed_set(in, 0);

  speed = core_audio_obj_in_speed_get(in);
  fail_if(!EINA_DBL_EQ(speed, 0.2));

  core_audio_obj_in_speed_set(in, 10);

  speed = core_audio_obj_in_speed_get(in);
  fail_if(!EINA_DBL_EQ(speed, 5.0));

  samplerate = core_audio_obj_in_samplerate_get(in);
  fail_if(samplerate != 0);

  core_audio_obj_in_samplerate_set(in, 1234);

  samplerate = core_audio_obj_in_samplerate_get(in);
  fail_if(samplerate != 1234);

  channels = core_audio_obj_in_channels_get(in);
  fail_if(channels != 0);

  core_audio_obj_in_channels_set(in, 2);

  channels = core_audio_obj_in_channels_get(in);
  fail_if(channels != 2);

  looped = core_audio_obj_in_looped_get(in);
  fail_if(looped);

  core_audio_obj_in_looped_set(in, EFL_TRUE);

  looped = core_audio_obj_in_looped_get(in);
  fail_if(!looped);

  length = core_audio_obj_in_length_get(in);
  fail_if(!EINA_DBL_EQ(length, 0));

  length = core_audio_obj_in_remaining_get(in);
  fail_if(!EINA_DBL_EQ(length, -1));

  memset(buf, 0xaa, 10);
  read = core_audio_obj_in_read(in, buf, 10);
  fail_if(read != 0);

  for (i = 0; i < 10; i++)
  {
    fail_if(buf[i] != 0xaa);
  }

  core_audio_obj_paused_set(in, EFL_TRUE);

  read = core_audio_obj_in_read(in, buf, 10);
  fail_if(read != 10);

  for (i = 0; i < 10; i++)
  {
    fail_if(buf[i] != 0x00);
  }

  efl_unref(in);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_audio_obj)
{
  int         i;
  const char *name;
  Efl_Bool    paused;
  double      volume;
  Eo         *objs[2], *obj;

  objs[0] = efl_add_ref(CORE_AUDIO_IN_CLASS, NULL);
  fail_if(!objs[0]);

  objs[1] = efl_add_ref(CORE_AUDIO_OUT_TEST_CLASS, NULL);
  fail_if(!objs[1]);

  for (i = 0; i < 2; i++)
  {
    obj = objs[i];

    fail_if(!obj);

    name = efl_name_get(obj);

    fail_if(name);

    efl_name_set(obj, "In1");
    name = efl_name_get(obj);

    ck_assert_str_eq(name, "In1");

    efl_name_get(obj);

    paused = core_audio_obj_paused_get(obj);
    fail_if(paused);

    core_audio_obj_paused_set(obj, EFL_TRUE);
    paused = core_audio_obj_paused_get(obj);
    fail_if(!paused);

    volume = core_audio_obj_volume_get(obj);
    fail_if(!EINA_DBL_EQ(volume, 1.0));

    core_audio_obj_volume_set(obj, 0.5);
    volume = core_audio_obj_volume_get(obj);
    fail_if(!EINA_DBL_EQ(volume, 0.5));

    efl_unref(obj);
  }
}

EFL_END_TEST

EFL_START_TEST(core_test_core_audio_init)
{
  int ret;

  ret = core_audio_init();
  ck_assert_int_eq(ret, 2);

  ret = core_audio_shutdown();
  ck_assert_int_eq(ret, 1);
}

EFL_END_TEST

void
setup(void)
{
  int ret;

  ret = core_audio_init();
  ck_assert_int_eq(ret, 1);
}

void
teardown(void)
{
  core_audio_shutdown();
}

void
core_test_core_audio(TCase *tc)
{
  tcase_add_checked_fixture(tc, setup, teardown);

  tcase_add_test(tc, core_test_core_audio_init);

  tcase_add_test(tc, core_test_core_audio_obj);
  tcase_add_test(tc, core_test_core_audio_obj_in);
  tcase_add_test(tc, core_test_core_audio_obj_vio);
  tcase_add_test(tc, core_test_core_audio_obj_in_out);
  tcase_add_test(tc, core_test_core_audio_obj_tone);

#ifdef HAVE_SNDFILE
  tcase_add_test(tc, core_test_core_audio_obj_sndfile);
  tcase_add_test(tc, core_test_core_audio_cleanup);
#endif
#ifdef HAVE_PULSE
  tcase_add_test(tc, core_test_core_audio_obj_pulse);
#endif
}
