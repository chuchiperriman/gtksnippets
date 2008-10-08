/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-func-manager.h
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
 
#ifndef __GSNIPPETS_FUNC_MANAGER_H__
#define __GSNIPPETS_FUNC_MANAGER_H__

#include <glib.h>

G_BEGIN_DECLS

typedef gchar* (*GSnippetsFunc) (GList *args,
				const gchar *value,
				gpointer user_data,
                                GError **error);

void
gsnippets_func_manager_register_func(const gchar *func_name,
				     GSnippetsFunc func,
				     gpointer user_data);

/**
 * gsnippets_func_manager_unregister_func:
 * @func_name: The function to be unregistered
 *
 * Returns: The user_data assigned when the function was registered.
 */
gpointer
gsnippets_func_manager_unregister_func(const gchar *func_name);

gchar *
gsnippets_func_manager_parse_text(const gchar *func_name,
				GList *args,
				const gchar *text,
                                GError **error);

G_END_DECLS
       
#endif /* __GSNIPPETS_FUNC_MANAGER_H__ */

