//Compile with:
//gcc -Wall -o eina_simple_xml_parser_01 eina_simple_xml_parser_01.c `pkg-config --cflags --libs shared`

#include <Efl_Shared.h>
#include <stdio.h>
#include <string.h>

static Efl_Bool _xml_attr_cb(void *data, const char *key, const char *value);
static Efl_Bool _xml_tag_cb(void *data, Eina_Simple_XML_Type type,
                            const char *content, unsigned offset, unsigned length);
static Efl_Bool _print(const void *container, void *data, void *fdata);

Efl_Bool tag_login   = EFL_FALSE;
Efl_Bool tag_message = EFL_FALSE;

int
main(void)
{
   FILE *file;
   long size;
   char *buffer;
   Eina_Array *array;

   eina_init();

   if ((file = fopen("chat.xml", "rb")))
   {
      fseek(file, 0, SEEK_END);
      size = ftell(file);
      fseek(file, 0, SEEK_SET);

      if (size < 0)
      {
         EINA_LOG_ERR("Can't read chat.xml");
         return 0;
      }

      if ((buffer = malloc(size)))
      {
         if (fread(buffer, 1, size, file) != (unsigned long)size)
         {
            EINA_LOG_ERR("Can't read chat.xml");
         }

         array = eina_array_new(10);
         eina_simple_xml_parse(buffer, size, EFL_TRUE, _xml_tag_cb, array);
         eina_array_foreach(array, _print, NULL);
         eina_array_free(array);

         free(buffer);
      }
      else
      {
         EINA_LOG_ERR("Can't allocate memory!");
      }
      
      fclose(file);
   }
   else
   {
      EINA_LOG_ERR("Can't open chat.xml!");
   }
   
   eina_shutdown();

   return 0;
}

static Efl_Bool
_xml_tag_cb(void *data, Eina_Simple_XML_Type type, const char *content,
            unsigned offset EFL_UNUSED, unsigned length)
{
   char buffer[length+1];
   Eina_Array *array = data;
   char str[512] = {'\0'};

   if (type == EINA_SIMPLE_XML_OPEN)
   {
      if(!strncmp("post", content, strlen("post")))
      {
         const char *tags = eina_simple_xml_tag_attributes_find(content,
                                                               length);
         eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                          _xml_attr_cb, str);
      }
      else if (!strncmp("login>", content, strlen("login>")))
      {
         tag_login = EFL_TRUE;
      }
      else if (!strncmp("message>", content, strlen("message>")))
      {
         tag_message = EFL_TRUE;
      }
   }
   else if (type == EINA_SIMPLE_XML_DATA)
   {
      if (tag_login == EFL_TRUE)
      {
         eina_strlcpy(buffer, content, sizeof(buffer));
         eina_strlcat(str, "<", 1);
         eina_strlcat(str, buffer, sizeof(str));
         eina_strlcat(str, "> ", 2);
         tag_login = EFL_FALSE;
      }
      else if (tag_message == EFL_TRUE)
      {
         eina_strlcpy(buffer, content, sizeof(buffer));
         eina_strlcat(str, buffer, sizeof(str));
         tag_message = EFL_FALSE;
         eina_array_push(array, strdup(str));
      }
   }

   return EFL_TRUE;
}

static Efl_Bool
_xml_attr_cb(void *data, const char *key, const char *value)
{
   char *str = data;

   if(!strcmp("id", key))
   {
      snprintf(str, sizeof(value) + 3, "(%s) ", value);
   }

   return EFL_TRUE;
}

static Efl_Bool
_print(const void *container EFL_UNUSED, void *data, void *fdata EFL_UNUSED)
{
   printf("%s\n", (char *)data);

   return EFL_TRUE;
}
