#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

#ifdef _WIN32
#  include <evil_private.h> /* setenv */
#endif

#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Core_File.h>

#include "efl_core_suite.h"

#ifndef O_BINARY
#  define O_BINARY 0
#endif

#define MAXSIZE 256

void
_writeToFile(const char *filePath, char *text)
{
  FILE *f = fopen(filePath, "rb+");
  if (f == NULL) f = fopen(filePath, "wb");
  fail_if(f == NULL);
  fprintf(f, "%s\n", text);
  fclose(f);
}

static char *
get_tmp_dir(void)
{
  Eina_Tmpstr *tmp_dir;
  char        *realpath;

  Efl_Bool created = eina_file_mkdtemp("EcoreFileTestXXXXXX", &tmp_dir);
  if (!created) return NULL;

  realpath = core_file_realpath(tmp_dir);
  eina_tmpstr_del(tmp_dir);
  return realpath;
}

static char *
get_tmp_file(void)
{
  Eina_Tmpstr *tmp_file;
  char        *realpath;

  int fd = eina_file_mkstemp("EcoreFileTestXXXXXX", &tmp_file);
  if (fd < 0) return NULL;
  close(fd);

  realpath = core_file_realpath(tmp_file);
  eina_tmpstr_del(tmp_file);
  return realpath;
}

static void
file_monitor_cb(void *data            EFL_UNUSED,
                Core_File_Monitor *em EFL_UNUSED,
                Core_File_Event       event,
                const char           *path)
{
  switch (event)
  {
    case CORE_FILE_EVENT_NONE:
    case CORE_FILE_EVENT_CREATED_FILE:
      fprintf(stderr, "File created in %s", path);
      break;
    case CORE_FILE_EVENT_DELETED_FILE:
      fprintf(stderr, "File deleted in %s", path);
      break;
    case CORE_FILE_EVENT_MODIFIED:
      fprintf(stderr, "File modified in %s", path);
      break;
    case CORE_FILE_EVENT_CLOSED:
      fprintf(stderr, "File closed in %s", path);
      break;
    case CORE_FILE_EVENT_DELETED_DIRECTORY:
      fprintf(stderr, "Directory deleted in %s", path);
      break;
    case CORE_FILE_EVENT_CREATED_DIRECTORY:
      fprintf(stderr, "Directory created in %s", path);
      break;
    case CORE_FILE_EVENT_DELETED_SELF:
      fprintf(stderr, "Path %s deleted", path);
      break;
  }
}

void
completion_cb(void *data EFL_UNUSED, const char *file EFL_UNUSED, int status)
{
  fprintf(stderr, "Done (status: %d)\n", status);
  core_main_loop_quit();
}

void
err_completion_cb(void *data, const char *file EFL_UNUSED, int status)
{
  if (data) *((int *)data) = status;
  fail_if(status != 1);
   // NOP if called from outside main loop. Keep it here if abort fails and
   // we get called from there.
  core_main_loop_quit();
}

int
progress_cb(void *data       EFL_UNUSED,
            const char *file EFL_UNUSED,
            long int         dltotal,
            long int         dlnow,
            long int ultotal EFL_UNUSED,
            long int ulnow   EFL_UNUSED)
{
  fprintf(stderr, "Progress: %ld/%ld\n", dlnow, dltotal);
  return CORE_FILE_PROGRESS_CONTINUE;
}

EFL_START_TEST(core_test_core_file_init)
{
  int ret;

  ret = core_file_init();
  fail_if(ret != 1);

  ret = core_file_shutdown();
  fail_if(ret != 0);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_file_operations)
{
  const char  *dirs[]  = { "b", "b/c", "b/c/d", "d", 0 };
  char        *dirs2[] = { "a2", "b2", "c2", 0 };
  const char  *src_dir, *src_file, *dest_file;
  const char  *not_exist_file;
  const char  *tmpdir       = NULL;
  char        *random_text  = "This is random test String";
  char        *escaped_text = "This\\ is\\ random\\ test\\ String";
  char        *exe_cmd      = "test.sh --opt1=a --opt2=b";
  char        *exe          = "test.sh";
  char         dir[MAXSIZE] = { '\0' };
  unsigned int ret;
  int          fd;
  int          i;
  Efl_Bool     res;
  Eina_List   *list;

  tmpdir = core_file_realpath(eina_environment_tmp_get());

  ret = core_file_init();
  fail_if(ret != 1);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);

  src_file = get_tmp_file();
  fail_if(!src_file);

  res = core_file_exists(src_file);
  fail_if(res != EFL_TRUE);
  res = core_file_is_dir(src_file);
  fail_if(res != EFL_FALSE);
  res = core_file_remove(src_file);
  fail_if(res != EFL_TRUE);
  res = core_file_exists(src_file);
  fail_if(res != EFL_FALSE);

  res = core_file_is_dir(src_dir);
  fail_if(res != EFL_TRUE);
  ret = core_file_dir_is_empty(src_dir);
  fail_if(ret != 1);
  res = core_file_mkdir(src_dir);
  fail_if(res != EFL_FALSE);
  res = core_file_remove(src_dir);
  fail_if(res != EFL_TRUE);
  res = core_file_is_dir(src_dir);
  fail_if(res != EFL_FALSE);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);
  ret = core_file_mksubdirs(src_dir, dirs);
  fail_if(ret != 4);
  list = core_file_ls(src_dir);
  fail_if(!list);
  fail_if(eina_list_count(list) != 2);
  res = core_file_recursive_rm(src_dir);
  fail_if(res != EFL_TRUE);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);
  snprintf(dir, sizeof(dir), "%s/%s", src_dir, dirs[2]);
  res = core_file_mkpath(dir);
  fail_if(res != EFL_TRUE);
  res = core_file_recursive_rm(src_dir);
  fail_if(res != EFL_TRUE);

  src_file = get_tmp_file();
  fail_if(!src_file);
  _writeToFile(src_file, random_text);

  ret = core_file_size(src_file);
  fail_if(ret != strlen(random_text) + 1);
  ret = core_file_is_dir(src_file);
  fail_if(ret != EFL_FALSE);

  dest_file = get_tmp_file();
  fail_if(!dest_file);
  res = core_file_cp(src_file, dest_file);
  fail_if(res != EFL_TRUE);
  res = core_file_exists(dest_file);
  fail_if(res != EFL_TRUE);
  fail_if(core_file_size(src_file) != core_file_size(dest_file));
  res = core_file_remove(dest_file);
  fail_if(res != EFL_TRUE);

   /* On Windows, symlink/readlink are not supported */
#ifndef _WIN32
  res = core_file_symlink(src_file, dest_file);
  fail_if(res != EFL_TRUE);
  ck_assert_str_eq(core_file_readlink(dest_file), src_file);
  ck_assert_str_eq(core_file_realpath(dest_file), src_file);
  res = core_file_unlink(dest_file);
  fail_if(res != EFL_TRUE);
#endif

  dest_file = get_tmp_file();
  fail_if(!dest_file);
  res = core_file_mv(src_file, dest_file);
  fail_if(res != EFL_TRUE);
  res = core_file_exists(dest_file);
  fail_if(res != EFL_TRUE);
  res = core_file_exists(src_file);
  fail_if(res != EFL_FALSE);

  ck_assert_str_eq(core_file_dir_get(dest_file), tmpdir);
  ck_assert_str_eq(core_file_realpath(dest_file), dest_file);
  fail_if(core_file_mod_time(dest_file) == 0);
  fail_if(core_file_can_read(dest_file) != EFL_TRUE);
  fail_if(core_file_can_write(dest_file) != EFL_TRUE);
  fail_if(core_file_can_exec(dest_file) != EFL_FALSE);
  fail_if(core_file_remove(dest_file) != EFL_TRUE);

  ck_assert_str_eq(core_file_app_exe_get(exe_cmd), exe);
  ck_assert_str_eq(core_file_escape_name(random_text), escaped_text);
  ck_assert_str_eq(core_file_strip_ext(exe), "test");
  res = core_file_app_installed(random_text);
  fail_if(res != EFL_FALSE);
  src_file = get_tmp_file();
  fail_if(!src_file);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fd = open(src_file, O_RDWR | O_BINARY | O_CREAT, 0700);
  fail_if(fd < 0);
  fail_if(close(fd) != 0);
  fail_if(core_file_can_exec(src_file) != EFL_TRUE);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);
  snprintf(dir, sizeof(dir), "%s/%s", src_dir, dirs[0]);
  fail_if(core_file_mkdir(dir) != EFL_TRUE);

  fail_if(core_file_mkdirs(NULL) != -1);
  for (i = 0; i < 3; i++)
  {
    char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s/%s", src_dir, dirs2[i]);
    dirs2[i] = strdup(tmp);
  }
  fail_if(core_file_mkdirs((const char **)dirs2) != 3);
  for (i = 0; i < 3; i++)
    free(dirs2[i]);

  fail_if(core_file_mksubdirs(src_dir, NULL) != -1);
  fail_if(core_file_mksubdirs(NULL, dirs) != -1);
  fail_if(core_file_mksubdirs("", dirs) != -1);
  fail_if(core_file_mksubdirs(src_file, dirs) != 0);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fail_if(core_file_recursive_rm(src_dir) != EFL_TRUE);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);
  fail_if(core_file_rmdir(src_dir) != EFL_TRUE);
  fail_if(core_file_dir_is_empty(src_dir) != -1);
  fail_if(core_file_ls(src_dir) != NULL);

  not_exist_file = get_tmp_file();
  fail_if(!not_exist_file);
  fail_if(core_file_remove(not_exist_file) != EFL_TRUE);
  fail_if(core_file_exists(not_exist_file) != EFL_FALSE);
  fail_if(core_file_mod_time(not_exist_file) != 0);
  fail_if(core_file_size(not_exist_file) != 0);

  ck_assert_str_eq(core_file_realpath(NULL), "");
  ck_assert_str_eq(core_file_realpath(not_exist_file), "");

  src_file = get_tmp_file();
  fail_if(!src_file);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fd = open(src_file, O_RDWR | O_BINARY | O_CREAT, 0400);
  fail_if(fd < 0);
  fail_if(close(fd) != 0);
  fail_if(core_file_can_read(src_file) != EFL_TRUE);
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
  if (getuid() || geteuid())
#endif
  {
    fail_if(core_file_can_write(src_file) != EFL_FALSE);
    fail_if(core_file_can_exec(src_file) != EFL_FALSE);
  }
  fail_if(core_file_cp(src_file, src_file) != EFL_FALSE);
  fail_if(core_file_remove(src_file) != EFL_TRUE);

  src_file = get_tmp_file();
  fail_if(!src_file);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fd = open(src_file, O_RDWR | O_BINARY | O_CREAT, 0200);
  fail_if(fd < 0);
  fail_if(close(fd) != 0);
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
  if (getuid() || geteuid())
#endif
  {
    fail_if(core_file_can_read(src_file) != EFL_FALSE);
  }
  fail_if(core_file_can_exec(src_file) != EFL_FALSE);
  fail_if(core_file_can_write(src_file) != EFL_TRUE);
  fail_if(core_file_remove(src_file) != EFL_TRUE);

  src_file = get_tmp_file();
  fail_if(!src_file);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fd = open(src_file, O_RDWR | O_BINARY | O_CREAT, 0100);
  fail_if(fd < 0);
  fail_if(close(fd) != 0);
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
  if (getuid() || geteuid())
#endif
  {
    fail_if(core_file_can_read(src_file) != EFL_FALSE);
    fail_if(core_file_can_write(src_file) != EFL_FALSE);
  }
  fail_if(core_file_can_exec(src_file) != EFL_TRUE);
  fail_if(core_file_remove(src_file) != EFL_TRUE);

  fail_if(core_file_unlink(not_exist_file) != EFL_FALSE);
  fail_if(core_file_remove(not_exist_file) != EFL_FALSE);
  fail_if(core_file_cp(not_exist_file, "test_file") != EFL_FALSE);
  fail_if(core_file_mv(not_exist_file, "test_file") != EFL_FALSE);

  ck_assert_int_eq(chdir(eina_environment_tmp_get()), 0);
  fail_if(core_file_mkpath(src_dir) != EFL_TRUE);
  fail_if(core_file_rmdir(src_dir) != EFL_TRUE);
  fail_if(core_file_mkpath(NULL) != EFL_FALSE);
  fail_if(core_file_mkpaths(dirs) != 4);
  for (i = 0; dirs[i]; i++)
    if (core_file_is_dir(dirs[i]))
      fail_if(core_file_recursive_rm(dirs[i]) != EFL_TRUE);
  fail_if(core_file_mkpaths(NULL) != -1);

  fail_if(core_file_dir_get(NULL) != NULL);
  fail_if(core_file_strip_ext(NULL) != NULL);
  fail_if(core_file_escape_name(NULL) != NULL);

  ret = core_file_shutdown();
  fail_if(ret != 0);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_file_path)
{
  const char  *src_dir, *dest_file;
  char        *dup_dir, *path, *src_file;
  unsigned int ret;
  int          fd;
  Efl_Bool     res;
  Eina_List   *list, *l;

  src_file = get_tmp_file();
  fail_if(!src_file);
  fail_if(core_file_remove(src_file) != EFL_TRUE);
  fd = open(src_file, O_RDWR | O_BINARY | O_CREAT, 0700);
  fail_if(fd < 0);
  fail_if(close(fd) != 0);
  fail_if(core_file_can_exec(src_file) != EFL_TRUE);
  dup_dir = strdup(src_file);
  fail_if(!dup_dir);
  dest_file = basename(dup_dir);
  dup_dir   = strdup(src_file);

  src_dir = getenv("PATH");
  fail_if(!src_dir);
  path = malloc(strlen(src_dir) + strlen(dup_dir) + 2);
  snprintf(path,
           strlen(src_dir) + strlen(dup_dir) + 2,
           "%s:%s",
           src_dir,
           dirname(dup_dir));
  ret = setenv("PATH", path, 1);
  fail_if(ret != 0);
  free(dup_dir);
  free(path);

  ret = core_file_init();

  res = core_file_app_installed(dest_file);
  if (!res)
  {
        /* attempt to mitigate cascading failures */
    ret = setenv("PATH", src_dir, 1);
    fail_if(ret != 0);
  }
  fail_if(res != EFL_TRUE);
  res = core_file_app_installed(src_file);
  if (!res)
  {
        /* attempt to mitigate cascading failures */
    ret = setenv("PATH", src_dir, 1);
    fail_if(ret != 0);
  }
  fail_if(res != EFL_TRUE);
  list = NULL;
  list = core_file_app_list();
  ret  = setenv("PATH", src_dir, 1);
  fail_if(ret != 0);
  fd = 0;
  EINA_LIST_FOREACH(list, l, path)
  {
    if (strcmp(path, src_file) == 0)
    {
      fd = 1;
      break;
    }
  }
  fail_if(fd == 0);
  EINA_LIST_FREE(list, dup_dir)
  free(dup_dir);

  fail_if(core_file_remove(src_file) != EFL_TRUE);

  ret = core_file_shutdown();
  fail_if(ret != 0);
  free(src_file);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_file_monitor)
{
  Core_File_Monitor *mon;
  char              *src_dir;
  const char        *file               = "EcoreFileDest";
  const char        *sub_dir[]          = { "subdir1", 0 };
  char               dir_name[MAXSIZE]  = { '\0' };
  char               file_name[MAXSIZE] = { '\0' };
  char              *random_text        = "This is random test String";
  char              *realp;
  Efl_Bool           res;
  int                ret;

  ret = core_file_init();
  fail_if(ret != 1);

  src_dir = get_tmp_dir();
  fail_if(!src_dir);

  realp = core_file_realpath(src_dir);
  fail_if(!realp);
  mon = core_file_monitor_add(realp, file_monitor_cb, NULL);
  fail_if(mon == NULL);

  snprintf(file_name, sizeof(file_name), "%s/%s", src_dir, file);
  _writeToFile(file_name, random_text);
  _writeToFile(file_name, random_text);

  ck_assert_str_eq(core_file_monitor_path_get(mon), realp);
  free(realp);

  ret = core_file_mksubdirs(src_dir, sub_dir);
  fail_if(ret != 1);

  res = core_file_remove(file_name);
  fail_if(res != EFL_TRUE);

  snprintf(dir_name, sizeof(dir_name), "%s/%s", src_dir, sub_dir[0]);
  res = core_file_rmdir(dir_name);
  fail_if(res != EFL_TRUE);

  res = core_file_recursive_rm(src_dir);
  fail_if(res != EFL_TRUE);

  core_file_monitor_del(mon);

  ret = core_file_shutdown();
  fail_if(ret != 0);
  free(src_dir);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_file_download)
{
  const char             *download_dir;
  const char             *download_file;
  const char             *download_url       = "http://example.com";
  char                    dest_name[MAXSIZE] = { '\0' };
  Efl_Bool                res;
  Eina_Hash              *headers;
  Core_File_Download_Job *job = NULL;
  int                     ret;

  ret = core_file_init();
  fail_if(ret != 1);

  download_dir = get_tmp_dir();
  fail_if(!download_dir);
  download_file = core_file_file_get(download_url); //example.com
  fail_if(!download_file);
  fail_if(!core_file_download_protocol_available("http://"));
  snprintf(dest_name, sizeof(dest_name), "%s/%s", download_dir, download_file);

  res = core_file_download("xxyyzz",
                           dest_name,
                           completion_cb,
                           progress_cb,
                           NULL,
                           &job);
  fail_if(res != EFL_FALSE);

  int status = 0;
  res        = core_file_download(download_url,
                           dest_name,
                           err_completion_cb,
                           progress_cb,
                           &status,
                           &job);
  fail_if(res != EFL_TRUE);
  fail_if(!job);
  core_file_download_abort(job);
  fail_if(status != 1);
  if (!status) core_main_loop_begin();
  if (timeout_reached) goto end;
  res = core_file_remove(dest_name);
  fail_if(res != EFL_TRUE);

  headers = eina_hash_string_small_new(NULL);
  eina_hash_add(headers, "Content-type", "text/html");

  res = core_file_download_full(download_url,
                                dest_name,
                                completion_cb,
                                progress_cb,
                                NULL,
                                &job,
                                headers);
  fail_if(res != EFL_TRUE);
  fail_if(!job);
  core_main_loop_begin();
  if (timeout_reached) goto end;
  fprintf(stderr, "Downloaded %lld bytes\n", core_file_size(dest_name));
  res = core_file_exists(dest_name);
  fail_if(res != EFL_TRUE);
end:
  res = core_file_recursive_rm(download_dir);
  fail_if(res != EFL_TRUE);

  ret = core_file_shutdown();
  fail_if(ret != 0);
}

EFL_END_TEST

void
core_test_core_file(TCase *tc)
{
  tcase_add_test(tc, core_test_core_file_init);
  tcase_add_test(tc, core_test_core_file_operations);
  tcase_add_test(tc, core_test_core_file_monitor);
  tcase_add_test(tc, core_test_core_file_download);
  tcase_add_test(tc, core_test_core_file_path);
}
