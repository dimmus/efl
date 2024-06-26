#include <Efl_Dbus.h>
#include "efl_dbus_introspection.h"

typedef struct _Efl_Dbus_Introspection_Element_Parse_Table
  Efl_Dbus_Introspection_Element_Parse_Table;

struct _Efl_Dbus_Introspection_Element_Parse_Table
{
  const char *name;
  void *(*parse)(Eina_Simple_XML_Node_Tag *tag);
  Eina_List **list;
};

static const char *_efl_dbus_introspection_attribute_value_get(Eina_Inlist *,
                                                               const char *);
static void *_efl_dbus_introspection_parse_node(Eina_Simple_XML_Node_Tag *);
static void *
_efl_dbus_introspection_parse_interface(Eina_Simple_XML_Node_Tag *);
static void *_efl_dbus_introspection_parse_method(Eina_Simple_XML_Node_Tag *);
static void *_efl_dbus_introspection_parse_signal(Eina_Simple_XML_Node_Tag *);
static void *_efl_dbus_introspection_parse_argument(Eina_Simple_XML_Node_Tag *);
static void *_efl_dbus_introspection_parse_property(Eina_Simple_XML_Node_Tag *);
static void *
_efl_dbus_introspection_parse_annotation(Eina_Simple_XML_Node_Tag *);
static void _efl_dbus_introspection_parse_children(
  Eina_Inlist *,
  const Efl_Dbus_Introspection_Element_Parse_Table[]);
static void
_efl_dbus_introspection_interface_free(Efl_Dbus_Introspection_Interface *);
static void
_efl_dbus_introspection_method_free(Efl_Dbus_Introspection_Method *);
static void
_efl_dbus_introspection_signal_free(Efl_Dbus_Introspection_Signal *);
static void
_efl_dbus_introspection_argument_free(Efl_Dbus_Introspection_Argument *);
static void
_efl_dbus_introspection_property_free(Efl_Dbus_Introspection_Property *);
static void
_efl_dbus_introspection_annotation_free(Efl_Dbus_Introspection_Annotation *);

EAPI Efl_Dbus_Introspection_Node *
efl_dbus_introspection_parse(const char *xml)
{
  Efl_Dbus_Introspection_Node *node;
  Eina_Simple_XML_Node_Root   *xml_root;
  Eina_Simple_XML_Node        *xml_node;
  Eina_Inlist                 *last = NULL;

  EINA_SAFETY_ON_NULL_RETURN_VAL(xml, NULL);

  node     = NULL;
  xml_root = eina_simple_xml_node_load(xml, strlen(xml), EFL_TRUE);
  if (xml_root && xml_root->children) last = xml_root->children->last;
  xml_node = (Eina_Simple_XML_Node *)last;
  if (!xml_node) goto free_root;
  EINA_SAFETY_ON_FALSE_GOTO(EINA_SIMPLE_XML_NODE_TAG == xml_node->type,
                            free_root);

  node = (Efl_Dbus_Introspection_Node *)_efl_dbus_introspection_parse_node(
    (Eina_Simple_XML_Node_Tag *)xml_node);

free_root:
  eina_simple_xml_node_root_free(xml_root);

  return node;
}

static void *
_efl_dbus_introspection_parse_node(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Node               *node;
  Efl_Dbus_Introspection_Element_Parse_Table table[] = {
    {      "node",      _efl_dbus_introspection_parse_node, NULL },
    { "interface", _efl_dbus_introspection_parse_interface, NULL },
    {        NULL,                                    NULL, NULL }
  };
  const char *name;

  node = calloc(1, sizeof(Efl_Dbus_Introspection_Node));
  EINA_SAFETY_ON_NULL_RETURN_VAL(node, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  node->name    = eina_stringshare_add(name);
  table[0].list = &node->nodes;
  table[1].list = &node->interfaces;

  _efl_dbus_introspection_parse_children(tag->children, table);

  return node;
}

EAPI void
efl_dbus_introspection_node_free(Efl_Dbus_Introspection_Node *node)
{
  Efl_Dbus_Introspection_Node      *child_node;
  Efl_Dbus_Introspection_Interface *interface;

  EINA_SAFETY_ON_NULL_RETURN(node);

  EINA_LIST_FREE(node->nodes, child_node)
  efl_dbus_introspection_node_free(child_node);

  EINA_LIST_FREE(node->interfaces, interface)
  _efl_dbus_introspection_interface_free(interface);

  eina_stringshare_del(node->name);
  free(node);
}

static const char *
_efl_dbus_introspection_attribute_value_get(Eina_Inlist *attributes,
                                            const char  *key)
{
  Eina_Simple_XML_Attribute *attribute;

  EINA_INLIST_FOREACH(attributes, attribute)
  {
    if (strcmp(attribute->key, key) == 0) return attribute->value;
  }

  return NULL;
}

static void *
_efl_dbus_introspection_parse_interface(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Interface          *interface;
  Efl_Dbus_Introspection_Element_Parse_Table table[] = {
    {     "method",     _efl_dbus_introspection_parse_method, NULL },
    {     "signal",     _efl_dbus_introspection_parse_signal, NULL },
    {   "property",   _efl_dbus_introspection_parse_property, NULL },
    { "annotation", _efl_dbus_introspection_parse_annotation, NULL },
    {         NULL,                                     NULL, NULL }
  };
  const char *name;

  interface = calloc(1, sizeof(Efl_Dbus_Introspection_Interface));
  EINA_SAFETY_ON_NULL_RETURN_VAL(interface, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  if (!name) goto error;
  interface->name = eina_stringshare_add(name);
  table[0].list   = &interface->methods;
  table[1].list   = &interface->signals;
  table[2].list   = &interface->properties;
  table[3].list   = &interface->annotations;

  _efl_dbus_introspection_parse_children(tag->children, table);

  return interface;

error:
  _efl_dbus_introspection_interface_free(interface);
  return NULL;
}

static void
_efl_dbus_introspection_interface_free(
  Efl_Dbus_Introspection_Interface *interface)
{
  Efl_Dbus_Introspection_Method     *method;
  Efl_Dbus_Introspection_Signal     *signal;
  Efl_Dbus_Introspection_Property   *property;
  Efl_Dbus_Introspection_Annotation *annotation;

  EINA_SAFETY_ON_NULL_RETURN(interface);

  EINA_LIST_FREE(interface->methods, method)
  _efl_dbus_introspection_method_free(method);

  EINA_LIST_FREE(interface->signals, signal)
  _efl_dbus_introspection_signal_free(signal);

  EINA_LIST_FREE(interface->properties, property)
  _efl_dbus_introspection_property_free(property);

  EINA_LIST_FREE(interface->annotations, annotation)
  _efl_dbus_introspection_annotation_free(annotation);

  eina_stringshare_del(interface->name);
  free(interface);
}

static void *
_efl_dbus_introspection_parse_method(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Method             *method;
  Efl_Dbus_Introspection_Element_Parse_Table table[] = {
    {        "arg",   _efl_dbus_introspection_parse_argument, NULL },
    { "annotation", _efl_dbus_introspection_parse_annotation, NULL },
    {         NULL,                                     NULL, NULL }
  };
  const char *name;

  method = calloc(1, sizeof(Efl_Dbus_Introspection_Method));
  EINA_SAFETY_ON_NULL_RETURN_VAL(method, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  if (!name) goto error;
  method->name  = eina_stringshare_add(name);
  table[0].list = &method->arguments;
  table[1].list = &method->annotations;

  _efl_dbus_introspection_parse_children(tag->children, table);

  return method;

error:
  _efl_dbus_introspection_method_free(method);
  return NULL;
}

static void
_efl_dbus_introspection_method_free(Efl_Dbus_Introspection_Method *method)
{
  Efl_Dbus_Introspection_Argument   *argument;
  Efl_Dbus_Introspection_Annotation *annotation;

  EINA_SAFETY_ON_NULL_RETURN(method);

  EINA_LIST_FREE(method->arguments, argument)
  _efl_dbus_introspection_argument_free(argument);

  EINA_LIST_FREE(method->annotations, annotation)
  _efl_dbus_introspection_annotation_free(annotation);

  eina_stringshare_del(method->name);
  free(method);
}

static void *
_efl_dbus_introspection_parse_signal(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Signal             *signal;
  Efl_Dbus_Introspection_Element_Parse_Table table[] = {
    {        "arg",   _efl_dbus_introspection_parse_argument, NULL },
    { "annotation", _efl_dbus_introspection_parse_annotation, NULL },
    {         NULL,                                     NULL, NULL }
  };
  const char *name;

  signal = calloc(1, sizeof(Efl_Dbus_Introspection_Signal));
  EINA_SAFETY_ON_NULL_RETURN_VAL(signal, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  if (!name) goto error;
  signal->name  = eina_stringshare_add(name);
  table[0].list = &signal->arguments;
  table[1].list = &signal->annotations;

  _efl_dbus_introspection_parse_children(tag->children, table);

  return signal;

error:
  _efl_dbus_introspection_signal_free(signal);
  return NULL;
}

static void
_efl_dbus_introspection_signal_free(Efl_Dbus_Introspection_Signal *signal)
{
  Efl_Dbus_Introspection_Argument   *argument;
  Efl_Dbus_Introspection_Annotation *annotation;

  EINA_SAFETY_ON_NULL_RETURN(signal);

  EINA_LIST_FREE(signal->arguments, argument)
  _efl_dbus_introspection_argument_free(argument);

  EINA_LIST_FREE(signal->annotations, annotation)
  _efl_dbus_introspection_annotation_free(annotation);

  eina_stringshare_del(signal->name);
  free(signal);
}

static void *
_efl_dbus_introspection_parse_argument(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Argument *argument;
  const char                      *name;
  const char                      *type;
  const char                      *direction;

  argument = calloc(1, sizeof(Efl_Dbus_Introspection_Argument));
  EINA_SAFETY_ON_NULL_RETURN_VAL(argument, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  argument->name = eina_stringshare_add(name);

  type = _efl_dbus_introspection_attribute_value_get(tag->attributes, "type");
  if (!type) goto error;
  argument->type = eina_stringshare_add(type);

  direction =
    _efl_dbus_introspection_attribute_value_get(tag->attributes, "direction");
  if (direction)
  {
    if (strcmp(direction, "in") == 0)
      argument->direction = ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_IN;
    else if (strcmp(direction, "out") == 0)
      argument->direction = ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_OUT;
    else argument->direction = ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_NONE;
  }

  return argument;

error:
  _efl_dbus_introspection_argument_free(argument);
  return NULL;
}

static void
_efl_dbus_introspection_argument_free(Efl_Dbus_Introspection_Argument *argument)
{
  EINA_SAFETY_ON_NULL_RETURN(argument);
  eina_stringshare_del(argument->name);
  eina_stringshare_del(argument->type);
  free(argument);
}

static void *
_efl_dbus_introspection_parse_property(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Property           *property;
  Efl_Dbus_Introspection_Element_Parse_Table table[] = {
    { "annotation", _efl_dbus_introspection_parse_annotation, NULL },
    {         NULL,                                     NULL, NULL }
  };
  const char *name;
  const char *type;
  const char *access;

  property = calloc(1, sizeof(Efl_Dbus_Introspection_Property));
  EINA_SAFETY_ON_NULL_RETURN_VAL(property, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  if (!name) goto error;
  property->name = eina_stringshare_add(name);

  type = _efl_dbus_introspection_attribute_value_get(tag->attributes, "type");
  if (!type) goto error;
  property->type = eina_stringshare_add(type);

  access =
    _efl_dbus_introspection_attribute_value_get(tag->attributes, "access");
  if (!access) goto error;

  if (strcmp(access, "read") == 0)
    property->access = ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READ;
  else if (strcmp(access, "write") == 0)
    property->access = ELDBUS_INTROSPECTION_PROPERTY_ACCESS_WRITE;
  else if (strcmp(access, "readwrite") == 0)
    property->access = ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READWRITE;
  else EINA_SAFETY_ON_TRUE_GOTO(!!"Unknown property access", error);
  table[0].list = &property->annotations;

  _efl_dbus_introspection_parse_children(tag->children, table);
  return property;

error:
  _efl_dbus_introspection_property_free(property);
  return NULL;
}

static void
_efl_dbus_introspection_property_free(Efl_Dbus_Introspection_Property *property)
{
  Efl_Dbus_Introspection_Annotation *annotation;

  EINA_SAFETY_ON_NULL_RETURN(property);

  EINA_LIST_FREE(property->annotations, annotation)
  _efl_dbus_introspection_annotation_free(annotation);

  eina_stringshare_del(property->name);
  eina_stringshare_del(property->type);
  free(property);
}

static void *
_efl_dbus_introspection_parse_annotation(Eina_Simple_XML_Node_Tag *tag)
{
  Efl_Dbus_Introspection_Annotation *annotation;
  const char                        *name;
  const char                        *value;

  annotation = calloc(1, sizeof(Efl_Dbus_Introspection_Annotation));
  EINA_SAFETY_ON_NULL_RETURN_VAL(annotation, NULL);

  name = _efl_dbus_introspection_attribute_value_get(tag->attributes, "name");
  if (!name) goto error;
  annotation->name = eina_stringshare_add(name);

  value = _efl_dbus_introspection_attribute_value_get(tag->attributes, "value");
  if (!value) goto error;
  annotation->value = eina_stringshare_add(value);

  return annotation;

error:
  _efl_dbus_introspection_annotation_free(annotation);
  return NULL;
}

static void
_efl_dbus_introspection_annotation_free(
  Efl_Dbus_Introspection_Annotation *annotation)
{
  EINA_SAFETY_ON_NULL_RETURN(annotation);
  eina_stringshare_del(annotation->name);
  eina_stringshare_del(annotation->value);
  free(annotation);
}

static void
_efl_dbus_introspection_parse_children(
  Eina_Inlist                                     *children,
  const Efl_Dbus_Introspection_Element_Parse_Table table[])
{
  Eina_Simple_XML_Node *child;

  EINA_INLIST_FOREACH(children, child)
  {
    const Efl_Dbus_Introspection_Element_Parse_Table *it;
    Eina_Simple_XML_Node_Tag                         *tag;
    void                                             *item;

    if (EINA_SIMPLE_XML_NODE_TAG != child->type) continue;

    tag = (Eina_Simple_XML_Node_Tag *)child;

    for (it = table; it->name; ++it)
    {
      if (strcmp(tag->name, it->name) != 0) continue;

      item = it->parse(tag);
      if (item) *it->list = eina_list_append(*it->list, item);
    }
  }
}

EAPI Efl_Dbus_Introspection_Interface *
efl_dbus_introspection_interface_find(Eina_List *interfaces, const char *name)
{
  Eina_List                        *it;
  Efl_Dbus_Introspection_Interface *interface;

  EINA_LIST_FOREACH(interfaces, it, interface)
  {
    if (strcmp(interface->name, name) == 0) return interface;
  }

  return NULL;
}

EAPI Efl_Dbus_Introspection_Property *
efl_dbus_introspection_property_find(Eina_List *properties, const char *name)
{
  Eina_List                       *it;
  Efl_Dbus_Introspection_Property *property;

  EINA_LIST_FOREACH(properties, it, property)
  {
    if (strcmp(property->name, name) == 0) return property;
  }

  return NULL;
}

EAPI Efl_Dbus_Introspection_Argument *
efl_dbus_introspection_argument_find(Eina_List *arguments, const char *name)
{
  Eina_List                       *it;
  Efl_Dbus_Introspection_Argument *argument;

  EINA_LIST_FOREACH(arguments, it, argument)
  {
    if (strcmp(argument->name, name) == 0) return argument;
  }

  return NULL;
}
