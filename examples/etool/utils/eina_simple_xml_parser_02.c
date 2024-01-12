//Compile with:
//gcc -Wall -o eina_simple_xml_parser_02 eina_simple_xml_parser_02.c `pkg-config --cflags --libs eina`

#include <Eina.h>
#include <stdio.h>
#include <string.h>


static Efl_Bool
_xml_attribute_parse_cb(void *data EFL_UNUSED, const char *key, const char *value)
{
   printf("attributes, key = %s, value = %s\n", key, value);
   return EFL_TRUE;
}

static Efl_Bool
_xml_tag_parse_cb(void *data EFL_UNUSED, Eina_Simple_XML_Type type, const char *content,
                  unsigned offset EFL_UNUSED, unsigned int length)
{
   if (length == 0) return EFL_FALSE;

   if (type == EINA_SIMPLE_XML_OPEN)
     {
        //Print tag
        if (!strncmp("Group", content, strlen("Group")))
          printf("tag = Group\n");
        else if (!strncmp("Label", content, strlen("Label")))
          printf("tag = Label\n");

        //Print attributes
        const char *tags = eina_simple_xml_tag_attributes_find(content, length);
        eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                         _xml_attribute_parse_cb, NULL);
     }

   return EFL_TRUE;
}

int
main(void)
{
   FILE *file;
   long size;

   eina_init();

   if ((file = fopen("exia.xml", "rb")))
   {
      fseek(file, 0, SEEK_END);
      size = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *buffer = malloc(size);
      if (!buffer)
      {
         fclose(file);
         return 0;
      }

      if (fread(buffer, 1, size, file) != (unsigned long)size)
      {
         EINA_LOG_ERR("Can't read exia.xml");
      }

      eina_simple_xml_parse(buffer, size, EFL_FALSE, _xml_tag_parse_cb, NULL);

      fclose(file);
      free(buffer);
   }
   else
   {
      EINA_LOG_ERR("Can't open exia.xml!");
   }

   return 0;
}
