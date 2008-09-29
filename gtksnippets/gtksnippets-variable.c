/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-variable.c
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

#include "gtksnippets-variable.h"
#include <stdlib.h>
#include <string.h>
#include "../gsnippets/gsnippets-func-manager.h"

#define DEFAULT_INDEX 100

G_DEFINE_TYPE (GtkSnippetsVariable, gtksnippets_variable, G_TYPE_OBJECT)

struct _GtkSnippetsVariablePrivate
{
	gchar *name;
	guint index;
	gchar *default_value;
	gchar *func_name;
};

static void
gtksnippets_variable_clean(GtkSnippetsVariable *self)
{
	g_free(self->priv->name);
	g_free(self->priv->default_value);
	g_free(self->priv->func_name);
	self->priv->index = DEFAULT_INDEX;
	self->priv->name = NULL;
	self->priv->default_value = NULL;
	self->priv->index = DEFAULT_INDEX;
	self->priv->func_name = NULL;
}

static void
gtksnippets_variable_finalize (GObject *object)
{
	GtkSnippetsVariable *self;
	self = GTKSNIPPETS_VARIABLE (object);
	
	gtksnippets_variable_clean(self);
	
	G_OBJECT_CLASS (gtksnippets_variable_parent_class)->finalize (object);
}

static void
gtksnippets_variable_class_init (GtkSnippetsVariableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = gtksnippets_variable_finalize;

	g_type_class_add_private (object_class, sizeof(GtkSnippetsVariablePrivate));
}

static void
gtksnippets_variable_init (GtkSnippetsVariable *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GTKSNIPPETS_TYPE_VARIABLE,
						  GtkSnippetsVariablePrivate);
	self->priv->name = NULL;
	self->priv->default_value = NULL;
	self->priv->index = DEFAULT_INDEX;
	self->priv->func_name = NULL;
}

static gboolean
parse_var_func_part(GtkSnippetsVariable *self, gchar* func_text)
{
	/*TODO Parse function args too*/
	self->priv->func_name = g_strdup(func_text);
	return TRUE;
}

static gboolean
parse_var_def_part(GtkSnippetsVariable *self, gchar* var_text)
{
	gchar **parts = g_strsplit (var_text, ":", 4);
	gboolean ok = TRUE, next = TRUE;
	if (parts[0]==NULL || g_strcmp0(parts[0],"")==0)
	{
		g_warning("Empty variable name");
		ok = FALSE;
		next = FALSE;
	}
	else
	{
		self->priv->name = g_strdup(parts[0]);
	}
	
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
	return ok;
}

static gboolean
parse_var_definition(GtkSnippetsVariable *self, const gchar *var_def)
{
	gchar **parts = g_strsplit (var_def, "#", 3);
	gboolean res = FALSE;
	if (parts[0] == NULL || strlen(parts[0]) < 1)
	{
		g_warning("Empty variable.");
	}
	else
	{
		res = parse_var_def_part(self,parts[0]);
		
		if (res && parts[1]!=NULL)
			res = parse_var_func_part(self,parts[1]);
	}
	
	g_strfreev (parts);
	
	return res;
}

/**
 * gtksnippets_variable_new:
 * @var_def: variable definition without brakets (if ${xxxx}, only xxxx)
 *
 * Passing a variable definition, this function parses this definition and assign all 
 * data to a new GtkSnippetsVariable. The definition is without the brakets.
 *
 * ${varname:index:default_value#function:arg1:argn...}
 *
 * The definition goes wothout ${ and }
 *
 * Returns: A new allocated snippet variable with all the data (name, index, default value etc.)
 **/
GtkSnippetsVariable*
gtksnippets_variable_new(const gchar *var_def)
{
	GtkSnippetsVariable *self;
	self = g_object_new (GTKSNIPPETS_TYPE_VARIABLE, NULL);
	parse_var_definition(self,var_def);
	return self;
}

void
gtksnippets_variable_rebuild(GtkSnippetsVariable *self, const gchar *var_def)
{
	gtksnippets_variable_clean(self);
	parse_var_definition(self,var_def);
}

const gchar* 
gtksnippets_variable_get_name(GtkSnippetsVariable *self)
{
	return self->priv->name;
}

guint
gtksnippets_variable_get_index(GtkSnippetsVariable *self)
{
	return self->priv->index;
}

const gchar*
gtksnippets_variable_get_default_value(GtkSnippetsVariable *self)
{
	return self->priv->default_value;
}

gchar*
gtksnippets_variable_parse_value(GtkSnippetsVariable *self,
				const gchar* value)
{
	const gchar *text = value;
	gchar *final;
	if (text == NULL)
	{
		text = (const gchar*)self->priv->default_value;
		if (text == NULL)
			return NULL;
	}
	
	if (self->priv->func_name!=NULL)
	{
		/*TODO Check error and see how to notify it*/
		final = gsnippets_func_manager_parse_text(self->priv->func_name,
						NULL,
						text,
						NULL);
	}
	else
	{
		final = g_strdup(text);
	}
	//TODO Apply the function to the variable value
	return final;
}



