/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-gtv-var.h
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
 
#ifndef __GTKSNIPPETS_GTV_VAR_H__
#define __GTKSNIPPETS_GTV_VAR_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "gsnippets-variable.h"

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_GTV_VAR		(gtksnippets_gtv_var_get_type ())
#define GTKSNIPPETS_GTV_VAR(obj)		(G_TYPE_CHECK_INSTANCE_CAST((obj), GTKSNIPPETS_TYPE_GTV_VAR, GtkSnippetsGtvVar))
#define GTKSNIPPETS_GTV_VAR_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), GTKSNIPPETS_TYPE_GTV_VAR, GtkSnippetsGtvVarClass))
#define GTKSNIPPETS_IS_GTV_VAR(obj)		(G_TYPE_CHECK_INSTANCE_TYPE((obj), GTKSNIPPETS_TYPE_GTV_VAR))
#define GTKSNIPPETS_IS_GTV_VAR_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_GTV_VAR))
#define GTKSNIPPETS_GTV_VAR_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_GTV_VAR, GtkSnippetsGtvVarClass))


typedef struct _GtkSnippetsGtvVar		GtkSnippetsGtvVar;
typedef struct _GtkSnippetsGtvVarClass	GtkSnippetsGtvVarClass;
typedef struct _GtkSnippetsGtvVarPrivate	GtkSnippetsGtvVarPrivate;

struct _GtkSnippetsGtvVar
{
	GSnippetsVariable parent_instance;

	GtkSnippetsGtvVarPrivate *priv;
};

struct _GtkSnippetsGtvVarClass
{
	GSnippetsVariableClass              parent_class;
};

GType             gtksnippets_gtv_var_get_type 		(void) G_GNUC_CONST;

GtkSnippetsGtvVar*
gtksnippets_gtv_var_new(const gchar *var_def,
			GtkTextView *view, 
			GtkTextMark *start_mark,
			GtkTextMark *end_mark,
			const gchar* tag_name,
			const gchar* error_tag_name);

void
gtksnippets_gtv_var_add_mirror(GtkSnippetsGtvVar *self, 
				GtkSnippetsGtvVar *mirror);

GList*
gtksnippets_gtv_var_get_mirrors(GtkSnippetsGtvVar *self);
				
GtkTextMark*
gtksnippets_gtv_var_get_start_mark(GtkSnippetsGtvVar *self);

GtkTextMark*
gtksnippets_gtv_var_get_end_mark(GtkSnippetsGtvVar *self);

gchar*
gtksnippets_gtv_var_get_text(GtkSnippetsGtvVar *self);

const gchar *
gtksnippets_gtv_var_get_tag_name(GtkSnippetsGtvVar *self);

void
gtksnippets_gtv_var_set_tag_name(GtkSnippetsGtvVar *self,
				 const gchar * tag_name);

const gchar *
gtksnippets_gtv_var_get_error_tag_name(GtkSnippetsGtvVar *self);

void
gtksnippets_gtv_var_set_error_tag_name(GtkSnippetsGtvVar *self,
				       const gchar * error_tag_name);

const gchar *
gtksnippets_gtv_var_get_current_tag_name(GtkSnippetsGtvVar *self);

void
gtksnippets_gtv_var_set_text(GtkSnippetsGtvVar *self, 
				const gchar* text,
				GError **error);

G_END_DECLS

#endif /* __GTKSNIPPETS_GTV_VAR_H__ */

