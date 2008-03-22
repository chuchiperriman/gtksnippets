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

GSList*
gsnippets_parser_get_vars(const gchar* text);

gchar*
gsnippets_parser_replace_var(const gchar* text, const gchar* var, const gchar *var_value);

gchar*
gsnippets_parser_replace_vars(const gchar* text,
				GSList *var_list);

gchar*
gsnippets_parser_clean_vars(const gchar* text);

void
gsnippets_parser_free_var_list(GSList *list);

guint
gsnippets_parser_count_vars(const gchar* text);


