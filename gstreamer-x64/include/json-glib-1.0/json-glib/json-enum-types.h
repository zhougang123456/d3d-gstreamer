
/* This file is generated by glib-mkenums, do not modify it. This code is licensed under the same license as the containing project. Note that it links to GLib, so must comply with the LGPL linking clauses. */

#ifndef __JSON_ENUM_TYPES_H__
#define __JSON_ENUM_TYPES_H__

#if !defined(__JSON_GLIB_INSIDE__) && !defined(JSON_COMPILATION)
#error "Only <json-glib/json-glib.h> can be included directly."
#endif

#include <glib-object.h>
#include <json-glib/json-version-macros.h>

G_BEGIN_DECLS
/* enumerations from "json-parser.h" */
JSON_AVAILABLE_IN_1_0
GType json_parser_error_get_type (void) G_GNUC_CONST;
#define JSON_TYPE_PARSER_ERROR (json_parser_error_get_type())
/* enumerations from "json-path.h" */
JSON_AVAILABLE_IN_1_0
GType json_path_error_get_type (void) G_GNUC_CONST;
#define JSON_TYPE_PATH_ERROR (json_path_error_get_type())
/* enumerations from "json-reader.h" */
JSON_AVAILABLE_IN_1_0
GType json_reader_error_get_type (void) G_GNUC_CONST;
#define JSON_TYPE_READER_ERROR (json_reader_error_get_type())
/* enumerations from "json-types.h" */
JSON_AVAILABLE_IN_1_0
GType json_node_type_get_type (void) G_GNUC_CONST;
#define JSON_TYPE_NODE_TYPE (json_node_type_get_type())
G_END_DECLS

#endif /* !__JSON_ENUM_TYPES_H__ */

/* Generated data ends here */

