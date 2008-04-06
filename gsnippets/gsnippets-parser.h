/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-parser.h
 *
 *  Copyright (C) 2008 - ChuchiPerriman <chuchiperriman@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.

 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <glib.h>

typedef struct
{
	gchar *name;
	gchar *value;
} GSnippetsVar;

/**
 * gsnippets_parser_get_vars:
 * @text: The text to be parsed
 *
 * This function search all the variables into the text. The variables look 
 * like: ${var.name}
 *
 * Returns a new allocated list of #GSnippetsVar objects
 */
GSList*
gsnippets_parser_get_vars(const gchar* text);

/**
 * gsnippets_parser_replace_var:
 * @text: The text to be parsed
 * @var: The variable name to be replaced
 * @var_value: The new value to be inserted in the variable place.
 *
 * Search all occurences of a variable (like ${@var}) and replace 
 * it with the var_value
 *
 * Returns A new allocated text with the variables replaced
 */
gchar*
gsnippets_parser_replace_var(const gchar* text, const gchar* var, const gchar *var_value);

/**
 * gsnippets_parser_replace_vars:
 * @text: The text to be parsed
 * @var_list: A list of #GSnippetsVar objects.
 *
 * Search all occurences of all variables and replace 
 * it with the new values.
 *
 * Returns A new allocated text with the variables replaced
 */
gchar*
gsnippets_parser_replace_vars(const gchar* text,
				GSList *var_list);

/**
 * gsnippets_parser_clean_vars:
 * @text: The text to be parsed
 *
 * Remove all the variables from the text. The variables like ${var.name} will
 * be removed.
 *
 * Returns A new allocated text with the variables replaced
 */
gchar*
gsnippets_parser_clean_vars(const gchar* text);

/**
 * gsnippets_parser_free_var_list:
 * @list: The list of #GSnippetsVar objects
 *
 * Frees a list of #GSnippetsVar objects. Frees the variables and the list.
 *
 */
void
gsnippets_parser_free_var_list(GSList *list);

/**
 * gsnippets_parser_count_vars:
 * @text: The text to be parsed
 *
 * Search all variables and return the number of different variables. If there 
 * are three ${var.name} and two ${var2.name} this function returns 2 because 
 * there are only two variables with a different name.
 *
 * Returns The number of variables into the text. 
 */
guint
gsnippets_parser_count_vars(const gchar* text);


