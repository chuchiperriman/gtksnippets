/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-parser.c
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

#include "gsnippets-parser.h"

/*
 * a = GSnippetsVar,b = current word
 */
static gint
_compare_vars(gconstpointer a, gconstpointer b)
{
	GSnippetsVar *vara;
	vara = (GSnippetsVar*) a;
	return g_utf8_collate(vara->name,b);
}

GSList *
gsnippets_parser_get_vars (const gchar * text)
{
  GSList *list = NULL;
  GMatchInfo *match_info = NULL;
  GSnippetsVar *var = NULL;
  GRegex *gr = g_regex_new ("\\$\\{(.*?)\\}",
			    0,
			    0,
			    NULL);

  g_regex_match (gr, text, 0, &match_info);

  while (g_match_info_matches (match_info))
    {
      gchar *word = g_match_info_fetch (match_info, 1);
      if (g_slist_find_custom (list, word, _compare_vars) == NULL)
	{
	  var = (GSnippetsVar*)g_malloc0(sizeof(GSnippetsVar));
	  var->name = word;
	  var->value = NULL;
	  
	  list = g_slist_append (list, var);
	}
      g_match_info_next (match_info, NULL);
    }

  g_match_info_free (match_info);
  g_regex_unref (gr);
  

  return list;
}

gchar *
gsnippets_parser_clean_vars (const gchar * text)
{
  GRegex *gr = g_regex_new ("\\$\\{(.*?)\\}",
			    0,
			    0,
			    NULL);
  gchar *res = g_regex_replace (gr,
				text,
				-1,
				0,
				"",
				0,
				NULL);


  g_regex_unref (gr);

  return res;
}

void
gsnippets_parser_free_var_list (GSList * list)
{
  GSList *temp = list;
  GSnippetsVar *var;
  while (temp != NULL)
    {
      var = temp->data;
      g_free (var->name);
      g_free (var->value);
      g_free (var);

      temp = g_slist_next (temp);
    }
  g_slist_free (list);
}

gchar*
gsnippets_parser_replace_var(const gchar* text, const gchar* var, const gchar *value)
{
	gchar *reg = g_strdup_printf("(\\$\\{%s\\})",var);
	GRegex *gr = g_regex_new(
						reg,
						0,
						0,
						NULL);
	gchar* res = g_regex_replace(
			gr,
			text,
			-1,
			0,
			value,
			0,
			NULL);
			
	g_free(reg);
	g_regex_unref(gr);
	
	return res;
}

gchar *
gsnippets_parser_replace_vars (const gchar * text, GSList * var_list)
{
	g_assert(var_list!=NULL);
	
	gchar *text1, *text2 = NULL;
	text1 = g_strdup(text);
	GSnippetsVar *var;
	while (var_list!=NULL)
	{
		var = var_list->data;
		text2 = gsnippets_parser_replace_var(text1,var->name,var->value);
		g_free(text1);
		text1 = text2;
		var_list = g_slist_next(var_list);
	}
	
	return text2;
}

guint
gsnippets_parser_count_vars(const gchar* text)
{
	GSList *vars = gsnippets_parser_get_vars(text);
	if (vars!=NULL)
	{
		guint num = g_slist_length(vars);
		gsnippets_parser_free_var_list(vars);
		return num;
	}
	else
	{
		return 0;
	}
}


