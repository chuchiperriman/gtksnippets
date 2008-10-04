/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-func-manager.c
 *
 *  Copyright (C) 2008 - ChuchiPerriman <chuchiperriman@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gsnippets-func-manager.h"

static GHashTable *functions = NULL;
static gboolean initilized = FALSE;
/*TODO:
	These static objects must be freed!!!. When??
*/

#define GSNIPPETS_FUNC_MANAGER_ERROR gsnippets_func_manager_quark ()

typedef enum {
	GSNIPPETS_FUNC_MANAGER_ERROR_NO_FUNC = 0,
	GSNIPPETS_FUNC_MANAGER_ERROR_NO_ARGS,
	GSNIPPETS_FUNC_MANAGER_ERROR_INVALID_ARGS
} GSnippetsFuncManagerError;


GQuark
gsnippets_func_manager_quark (void)
{
  return g_quark_from_static_string ("gsnippets-func-manager-quark");
}

/* *********** Default functions ********** */
static gchar*
gsnippets_func_upper (GList *args,
			const gchar *value,
			GError **error)
{
	if (value==NULL)
		return NULL;
	
	return g_utf8_strup(value,-1);
}

static gchar*
gsnippets_func_lower (GList *args,
			const gchar *value,
			GError **error)
{
	if (value==NULL)
		return NULL;
		
	return g_utf8_strdown(value,-1);
}

static gchar*
gsnippets_func_camel (GList *args,
			const gchar *value,
			GError **error)
{
	GError *tmp_error = NULL;
	gchar *res = NULL;
	GRegex *gr = g_regex_new ("( +|-+|_+|\\b)(.)",
				    0,
				    0,
				    &tmp_error);
	if (tmp_error!=NULL)
	{
		g_propagate_error(error,tmp_error);
		return NULL;
	}
	res = g_regex_replace(gr,
			value,
			-1,
			0,
			"\\u\\2",
			0,
			&tmp_error);
	g_regex_unref (gr);

	if (tmp_error!=NULL)
		g_propagate_error(error,tmp_error);
	
	return res;
}

static gchar*
gsnippets_func_regexp_rep (GList *args,
			const gchar *value,
			GError **error)
{
	gchar *res = NULL;
	GError *tmp_error = NULL;
	if (g_list_length(args)!=2)
	{
		g_set_error(error,
			    GSNIPPETS_FUNC_MANAGER_ERROR,
			    GSNIPPETS_FUNC_MANAGER_ERROR_NO_ARGS,
			    "regexp_rep needs 2 arguments: the regexp and the replacement. See GRegex API."
			    );
		return NULL;
	}
	GRegex *gr = g_regex_new ((const gchar*)g_list_nth(args,0)->data,
				    0,
				    0,
				    &tmp_error);
	if (tmp_error!=NULL)
	{
		g_propagate_error(error,tmp_error);
		return NULL;
	}
	res = g_regex_replace(gr,
			value,
			-1,
			0,
			(const gchar*)g_list_nth(args,1)->data,
			0,
			&tmp_error);
	
	g_regex_unref (gr);
	
	if (tmp_error!=NULL)
	{
		g_propagate_error(error,tmp_error);
		return NULL;
	}
	
	return res;
}
/* **************************************** */

static void
gsnippets_func_manager_init()
{
	functions = g_hash_table_new_full(g_str_hash,
					  g_str_equal,
					  g_free,
					  NULL);
	initilized = TRUE;
	gsnippets_func_manager_register_func("upper",
					(GSnippetsFunc*)gsnippets_func_upper);
	gsnippets_func_manager_register_func("lower",
					(GSnippetsFunc*)gsnippets_func_lower);
	gsnippets_func_manager_register_func("camel",
					(GSnippetsFunc*)gsnippets_func_camel);
	gsnippets_func_manager_register_func("regexp_rep",
					(GSnippetsFunc*)gsnippets_func_regexp_rep);
					
}

void
gsnippets_func_manager_register_func(const gchar *func_name,
				     GSnippetsFunc *func)
{
	if (!initilized) gsnippets_func_manager_init();
		
	g_hash_table_insert(functions,
			    g_strdup(func_name),
			    func);
}

gchar *
gsnippets_func_manager_parse_text(const gchar *func_name,
				GList *args,
				const gchar *text,
                                GError **error)
{

	if (!initilized) gsnippets_func_manager_init();
	
	GSnippetsFunc func;
	func = g_hash_table_lookup(functions, func_name);
	if (func==NULL)
	{
		g_set_error(error,
			    GSNIPPETS_FUNC_MANAGER_ERROR,
			    GSNIPPETS_FUNC_MANAGER_ERROR_NO_FUNC,
			    "The function %s has not been found",
			    func_name
			    );
		return NULL;
	}
	
	return func(args,text,error);
}


