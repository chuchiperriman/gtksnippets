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
/*TODO:
	These static objects must be freed!!!. When??
*/

void
gsnippets_func_manager_register_func(const gchar *func_name,
				     GSnippetsFunc *func)
{
	if (functions==NULL)
		functions = g_hash_table_new_full(g_str_hash,
						  g_str_equal,
						  g_free,
						  NULL);
	g_hash_table_insert(functions,
			    g_strdup(func_name),
			    func);
}

gchar *
gsnippets_func_manager_parse_text(const gchar *func_name,
				GList *args,
				const gchar *text,
                                GError *error)
{

	if (functions==NULL)
	{
		g_debug("TODO: Error");
		/*TODO Devolver error porque la funcion no existe*/
		return NULL;
	}
	
	GSnippetsFunc func;
	func = g_hash_table_lookup(functions, func_name);
	if (func==NULL)
	{
		g_debug("TODO: Error");
		/*TODO Devolver error porque la funcion no existe*/
		return NULL;
	}
	
	return func(args,text,error);
}


