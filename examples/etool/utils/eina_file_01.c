//Compile with:
//gcc -g -Wall eina_file_01.c -o eina_file_01 `pkg-config --cflags --libs etool`

#include <stdio.h>
#include <Eina.h>

static void
_print_cb(const char *name, const char *path, void *data EFL_UNUSED)
{
   printf("file %s in %s\n", name, path);
}

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
  Eina_Iterator *it;
  const char *f_name;
  const Eina_File_Direct_Info *f_info;
  const char *dir = "/home/";

  eina_init();

  eina_file_dir_list(dir, EFL_FALSE, _print_cb, NULL);

  it = eina_file_ls(dir);
  EINA_ITERATOR_FOREACH(it, f_name)
  {
    printf("%s\n", f_name);
    eina_stringshare_del(f_name);
  }
  eina_iterator_free(it);

  it = eina_file_stat_ls(dir);
  EINA_ITERATOR_FOREACH(it, f_info)
    printf("%s is of type %d\n", f_info->path, f_info->type);
  eina_iterator_free(it);

  it = eina_file_direct_ls(dir);
  EINA_ITERATOR_FOREACH(it, f_info)
    printf("%s is of type %d\n", f_info->path, f_info->type);
  eina_iterator_free(it);

  eina_shutdown();

  return 0;
}
