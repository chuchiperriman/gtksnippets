/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-variable.h
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
 
#ifndef __GSNIPPETS_VARIABLE_H__
#define __GSNIPPETS_VARIABLE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GSNIPPETS_TYPE_VARIABLE		(gsnippets_variable_get_type ())
#define GSNIPPETS_VARIABLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), GSNIPPETS_TYPE_VARIABLE, GSnippetsVariable))
#define GSNIPPETS_VARIABLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), GSNIPPETS_TYPE_VARIABLE, GSnippetsVariableClass))
#define GSNIPPETS_IS_VARIABLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GSNIPPETS_TYPE_VARIABLE))
#define GSNIPPETS_IS_VARIABLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GSNIPPETS_TYPE_VARIABLE))
#define GSNIPPETS_VARIABLE_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), GSNIPPETS_TYPE_VARIABLE, GSnippetsVariableClass))


typedef struct _GSnippetsVariable		GSnippetsVariable;
typedef struct _GSnippetsVariableClass	GSnippetsVariableClass;
typedef struct _GSnippetsVariablePrivate	GSnippetsVariablePrivate;

struct _GSnippetsVariable
{
	GObject parent_instance;

	GSnippetsVariablePrivate *priv;
};

struct _GSnippetsVariableClass
{
	GObjectClass              parent_class;
};

GType             gsnippets_variable_get_type 		(void) G_GNUC_CONST;

void
gsnippets_variable_rebuild(GSnippetsVariable *self, const gchar *var_def);

GSnippetsVariable*
gsnippets_variable_new(const gchar *var_def);

const gchar* 
gsnippets_variable_get_name(GSnippetsVariable *self);

guint
gsnippets_variable_get_index(GSnippetsVariable *self);

const gchar*
gsnippets_variable_get_default_value(GSnippetsVariable *self);

gboolean
gsnippets_variable_is_automatic(GSnippetsVariable *self);

gchar*
gsnippets_variable_parse_value(GSnippetsVariable *self,
				const gchar* value,
				GError **error);


G_END_DECLS

#endif /* __GSNIPPETS_VARIABLE_H__ */
