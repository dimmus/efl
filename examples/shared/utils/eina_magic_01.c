//Compile with:
//gcc -g -Wall eina_magic_01.c -o eina_magic_01 `pkg-config --cflags --libs shared`

#include <Efl_Shared.h>

#define BASETYPE_MAGIC 0x12345
struct _person 
{
   EINA_MAGIC;
   char *name;
};
typedef struct _person person;

#define SUBTYPE_MAGIC 0x3333
struct _pilot
{
   person base;
   EINA_MAGIC;
   char *callsign;
};
typedef struct _pilot pilot;

static person *
person_new(const char *name)
{
   person *ptr = malloc(sizeof(person));
   EINA_MAGIC_SET(ptr, BASETYPE_MAGIC);
   ptr->name = strdup(name);

   return ptr;
}

/*
static void
person_free(person *ptr) {
   if (!EINA_MAGIC_CHECK(ptr, BASETYPE_MAGIC))
   {
      EINA_MAGIC_FAIL(ptr, BASETYPE_MAGIC);
      return;
   }
   EINA_MAGIC_SET(ptr, EINA_MAGIC_NONE);
   free(ptr->name);
   free(ptr);
}
*/

static pilot *
pilot_new(const char *name, const char *callsign)
{
   pilot *ptr = malloc(sizeof(pilot));
   EINA_MAGIC_SET(ptr, SUBTYPE_MAGIC);
   EINA_MAGIC_SET(&ptr->base, BASETYPE_MAGIC);
   ptr->base.name = strdup(name);
   ptr->callsign = strdup(callsign);

   return ptr;
}

/*
static void
pilot_free(pilot *ptr) {
   if (!EINA_MAGIC_CHECK(ptr, SUBTYPE_MAGIC))
   {
      EINA_MAGIC_FAIL(ptr, SUBTYPE_MAGIC);
      return;
   }
   EINA_MAGIC_SET(ptr, EINA_MAGIC_NONE);
   EINA_MAGIC_SET(&ptr->base, EINA_MAGIC_NONE);
   free(ptr->base.name);
   free(ptr->callsign);
   free(ptr);
}
*/

static void
print_person(person *ptr)
{
   if (!EINA_MAGIC_CHECK(ptr, BASETYPE_MAGIC))
   {
      EINA_MAGIC_FAIL(ptr, BASETYPE_MAGIC);
      return;
   }
   printf("name: %s\n", ptr->name);
}

static void
print_pilot(pilot *ptr)
{
   if (!EINA_MAGIC_CHECK(ptr, SUBTYPE_MAGIC)) 
   {
      EINA_MAGIC_FAIL(ptr, SUBTYPE_MAGIC);
      return;
   }
   print_person(&ptr->base);
   printf("callsign: %s\n", ptr->callsign);
}

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   person *base;
   pilot *sub;

   eina_init();
   
   eina_magic_string_set(BASETYPE_MAGIC, "person");
   eina_magic_string_static_set(SUBTYPE_MAGIC, "pilot");

   base = person_new("Tyrol");
   sub = pilot_new("thrace", "starbuck");

   print_person(base);
   print_person((person *)sub);

   print_pilot((pilot *)base); //BAD: fails (C cast prevents GCC warning)
   print_pilot(sub);

   eina_shutdown();
}
