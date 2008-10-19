/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-variable.c
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

#include "gsnippets-variable.h"
#include <stdlib.h>
#include <string.h>
#include "../gsnippets/gsnippets-func-manager.h"

#define DEFAULT_INDEX 100

G_DEFINE_TYPE (GSnippetsVariable, gsnippets_variable, G_TYPE_OBJECT)

struct _GSnippetsVariablePrivate
{
	gchar *name;
	guint index;
	gchar *default_value;
	gchar* func_name;
	GList *func_args;
};

static void free_arg(gpointer *data, gpointer *user_data)
{
	g_free(data);
}

static void
gsnippets_variable_clean(GSnippetsVariable *self)
{
	g_free(self->priv->name);
	g_free(self->priv->default_value);
	g_free(self->priv->func_name);
	self->priv->index = DEFAULT_INDEX;
	self->priv->name = NULL;
	self->priv->default_value = NULL;
	self->priv->index = DEFAULT_INDEX;
	self->priv->func_name = NULL;
	g_list_foreach(self->priv->func_args,(GFunc)free_arg,NULL);
	g_list_free(self->priv->func_args);
	self->priv->func_args = NULL;
	
}

static void
gsnippets_variable_finalize (GObject *object)
{
	GSnippetsVariable *self;
	self = GSNIPPETS_VARIABLE (object);
	
	gsnippets_variable_clean(self);
	
	G_OBJECT_CLASS (gsnippets_variable_parent_class)->finalize (object);
}

static void
gsnippets_variable_class_init (GSnippetsVariableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = gsnippets_variable_finalize;

	g_type_class_add_private (object_class, sizeof(GSnippetsVariablePrivate));
}

static void
gsnippets_variable_init (GSnippetsVariable *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GSNIPPETS_TYPE_VARIABLE,
						  GSnippetsVariablePrivate);
	self->priv->name = NULL;
	self->priv->default_value = NULL;
	self->priv->index = DEFAULT_INDEX;
	self->priv->func_name = NULL;
	self->priv->func_args = NULL;
}

static void
parse_var_func_part(GSnippetsVariable *self, gchar* func_text)
{
	gchar **parts = g_strsplit (func_text, ":", 4);
	gboolean next = TRUE;
	if (parts[0]==NULL || g_strcmp0(parts[0],"")==0)
	{
		g_warning("Empty function name");
		next = FALSE;
	}
	else
	{
		self->priv->func_name = g_strdup(parts[0]);
	}
	gint i = 1;
	while (next && parts[i]!=NULL)
	{
		self->priv->func_args = g_list_append(self->priv->func_args,
						g_strdup(parts[i]));
		i++;
	}
	g_strfreev (parts);
}

static void
parse_var_def_part(GSnippetsVariable *self, gchar* var_text)
{
	gchar **parts = g_strsplit (var_text, ":", 4);
	gboolean next = TRUE;
	if (parts[0]==NULL)
		next = FALSE;
	else
		self->priv->name = g_strdup(parts[0]);
	
	if (next && parts[1]==NULL)
		next = FALSE;
	
	if (next && g_strcmp0(parts[1],"")!=0)
	{
		if (strlen(parts[1]) != 1 || !g_ascii_isdigit(*parts[1]))
		{
			g_warning("Index is not a number or it is not one digit");
			next = FALSE;
		}
		else
		{
			self->priv->index = atoi(parts[1]);
		}
	}
	
	if (next && parts[2]==NULL)
		next = FALSE;
	
	if (next && g_strcmp0(parts[2],"")!=0)
	{
		self->priv->default_value = g_strdup(parts[2]);
	}
	
	if (next && parts[3]!=NULL)
	{
		g_warning("Too much separators...");
		next = FALSE;
	}
	g_strfreev (parts);
}

static gboolean
parse_var_definition(GSnippetsVariable *self, const gchar *var_def)
{
	gchar **parts = g_strsplit (var_def, "#", 3);
	gboolean res = FALSE;
	if (parts[0] == NULL)
	{
		g_warning("Empty variable.");
	}
	else
	{
		parse_var_def_part(self,parts[0]);
		if (parts[1]!=NULL)
			parse_var_func_part(self,parts[1]);
		
		/* if has not var name but has function, it is automatic*/
		if (self->priv->name!=NULL || self->priv->func_name!=NULL)
			res = TRUE;

	}
	
	g_strfreev (parts);
	
	return res;
}

/**
 * gsnippets_variable_new:
 * @var_def: variable definition without brakets (if ${xxxx}, only xxxx)
 *
 * Passing a variable definition, this function parses this definition and assign all 
 * data to a new GSnippetsVariable. The definition is without the brakets.
 *
 * ${varname:index:default_value#function:arg1:argn...}
 *
 * The definition goes wothout ${ and }
 *
 * Returns: A new allocated snippet variable with all the data (name, index, default value etc.)
 **/
GSnippetsVariable*
gsnippets_variable_new(const gchar *var_def)
{
	GSnippetsVariable *self;
	self = g_object_new (GSNIPPETS_TYPE_VARIABLE, NULL);
	parse_var_definition(self,var_def);
	return self;
}

void
gsnippets_variable_rebuild(GSnippetsVariable *self, const gchar *var_def)
{
	gsnippets_variable_clean(self);
	parse_var_definition(self,var_def);
}

const gchar* 
gsnippets_variable_get_name(GSnippetsVariable *self)
{
	return self->priv->name;
}

guint
gsnippets_variable_get_index(GSnippetsVariable *self)
{
	return self->priv->index;
}

const gchar*
gsnippets_variable_get_default_value(GSnippetsVariable *self)
{
	return self->priv->default_value;
}

const gchar* 
gsnippets_variable_get_func_name(GSnippetsVariable *self)
{
	return self->priv->func_name;
}

gboolean
gsnippets_variable_is_automatic(GSnippetsVariable *self)
{
	return self->priv->name==NULL && self->priv->func_name!=NULL;
}

gchar*
gsnippets_variable_parse_value(GSnippetsVariable *self,
				const gchar* value,
				GError **error)
{
	const gchar *text = value;
	gchar *final = NULL;
	if (text == NULL)
		text = (const gchar*)self->priv->default_value;
	
	if (self->priv->func_name!=NULL)
	{
		final = gsnippets_func_manager_parse_text(self->priv->func_name,
						self->priv->func_args,
						text,
						error);
	}
	else
	{
		final = g_strdup(text);
	}
	
	return final;
}



