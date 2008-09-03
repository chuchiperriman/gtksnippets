/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-variable.h
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
 
#ifndef __GTKSNIPPETS_VARIABLE_H__
#define __GTKSNIPPETS_VARIABLE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_VARIABLE		(gtksnippets_variable_get_type ())
#define GTKSNIPPETS_VARIABLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), GTKSNIPPETS_TYPE_VARIABLE, GtkSnippetsVariable))
#define GTKSNIPPETS_VARIABLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), GTKSNIPPETS_TYPE_VARIABLE, GtkSnippetsVariableClass))
#define GTKSNIPPETS_IS_VARIABLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GTKSNIPPETS_TYPE_VARIABLE))
#define GTKSNIPPETS_IS_VARIABLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_VARIABLE))
#define GTKSNIPPETS_VARIABLE_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_VARIABLE, GtkSnippetsVariableClass))


typedef struct _GtkSnippetsVariable		GtkSnippetsVariable;
typedef struct _GtkSnippetsVariableClass	GtkSnippetsVariableClass;
typedef struct _GtkSnippetsVariablePrivate	GtkSnippetsVariablePrivate;

struct _GtkSnippetsVariable
{
	GObject parent_instance;

	GtkSnippetsVariablePrivate *priv;
};

struct _GtkSnippetsVariableClass
{
	GObjectClass              parent_class;
};

GType             gtksnippets_variable_get_type 		(void) G_GNUC_CONST;

void
gtksnippets_variable_rebuild(GtkSnippetsVariable *self, const gchar *var_def);

GtkSnippetsVariable*
gtksnippets_variable_new(const gchar *var_def);

const gchar* 
gtksnippets_variable_get_name(GtkSnippetsVariable *self);

guint
gtksnippets_variable_get_index(GtkSnippetsVariable *self);

const gchar*
gtksnippets_variable_get_default_value(GtkSnippetsVariable *self);


G_END_DECLS

#endif /* __GTKSNIPPETS_VARIABLE_H__ */
