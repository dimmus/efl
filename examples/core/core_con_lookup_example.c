//Compile with:
// gcc -o core_con_lookup_example core_con_lookup_example.c `pkg-config --libs --cflags ecore ecore-con`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

static void
_lookup_done_cb(const char *canonname, const char *ip, struct sockaddr *addr, int addrlen EFL_UNUSED, void *data EFL_UNUSED)
{
   printf("canonname = %s\n", canonname);
   printf("ip = %s\n", ip);
   printf("addr = %p\n", addr);
   core_main_loop_quit();
}

int
main(int argc, const char *argv[])
{
   if (argc < 2)
     {
        printf("need one parameter: <address>\n");
        return -1;
     }

   core_con_init();

   if (!core_con_lookup(argv[1], _lookup_done_cb, NULL))
     {
        printf("error when trying to start lookup for %s\n", argv[1]);
        goto end;
     }

   core_main_loop_begin();

end:
   core_con_shutdown();

   return 0;
}

