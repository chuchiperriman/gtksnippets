/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  common.c
 *
 *  Copyright (C) 2008 - Chuchiperriman <chuchiperriman@gmail.com>
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
#include <sys/stat.h>

static gchar* gsnippets_user_path = NULL;
static gchar* gsnippets_db_path = NULL;


static void gsnippets_common_check_dir(gchar *path) {

	if(!g_file_test(path, G_FILE_TEST_IS_DIR)) {
		if(0 != g_mkdir_with_parents(path, S_IRUSR | S_IWUSR | S_IXUSR)) {
			/*TODO i18n*/
			g_error("Cannot create cache directory \"%s\"!", path);
		}
	}
	g_free(path);
}

static void
gsnippets_common_load_gsnippets_user_path(void)
{
	gsnippets_user_path = g_strdup_printf("%s" G_DIR_SEPARATOR_S ".gsnippets", g_get_home_dir());
	gsnippets_common_check_dir(g_strdup(gsnippets_user_path));
	gsnippets_db_path = g_strdup_printf("%s" G_DIR_SEPARATOR_S "gsnippets.db", gsnippets_user_path);
}

const gchar*
gsnippets_common_get_gsnippets_user_path(void)
{
	if(!gsnippets_user_path)
		gsnippets_common_load_gsnippets_user_path();
		
	return gsnippets_user_path;
}

const gchar*
gsnippets_common_get_gsnippets_db_path(void)
{
	if(!gsnippets_user_path)
		gsnippets_common_load_gsnippets_user_path();
		
	return gsnippets_db_path;
}



