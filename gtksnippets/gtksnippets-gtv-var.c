/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-gtv-var.c
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

#include "gtksnippets-gtv-var.h"
#include "../gsnippets/gsnippets-func-manager.h"

G_DEFINE_TYPE (GtkSnippetsGtvVar, gtksnippets_gtv_var, GSNIPPETS_TYPE_VARIABLE)

struct _GtkSnippetsGtvVarPrivate
{
	GtkTextView *view;
	GtkTextMark *start_mark;
	GtkTextMark *end_mark;
	const gchar* tag_name;
	const gchar* error_tag_name;
	const gchar* current_tag_name;
	GList *mirrors;
};

static void
gtksnippets_gtv_var_finalize (GObject *object)
{
	GtkSnippetsGtvVar *self;

	self = GTKSNIPPETS_GTV_VAR (object);
	GList *list = self->priv->mirrors;
	while(list !=NULL)
	{
		g_object_unref(list->data);
		list = g_list_next(list);
	}
	g_list_free(self->priv->mirrors);
	self->priv->mirrors = NULL;

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	gtk_text_buffer_delete_mark(buffer,self->priv->start_mark);
	gtk_text_buffer_delete_mark(buffer,self->priv->end_mark);

	self->priv->view = NULL;
	self->priv->start_mark = NULL;
	self->priv->end_mark = NULL;
	self->priv->tag_name = NULL;
	
	G_OBJECT_CLASS (gtksnippets_gtv_var_parent_class)->finalize (object);
}

static void
gtksnippets_gtv_var_class_init (GtkSnippetsGtvVarClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gtksnippets_gtv_var_finalize;

	g_type_class_add_private (object_class, sizeof(GtkSnippetsGtvVarPrivate));
}

static void
gtksnippets_gtv_var_init (GtkSnippetsGtvVar *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GTKSNIPPETS_TYPE_GTV_VAR,
						  GtkSnippetsGtvVarPrivate);

	self->priv->view = NULL;
	self->priv->start_mark = NULL;
	self->priv->end_mark = NULL;
	self->priv->mirrors = NULL;
	self->priv->tag_name = NULL;
}

GtkSnippetsGtvVar*
gtksnippets_gtv_var_new(const gchar *var_def,
			GtkTextView *view, 
			GtkTextMark *start_mark,
			GtkTextMark *end_mark,
			const gchar* tag_name,
			const gchar* error_tag_name)
{
	g_assert(tag_name!=NULL);
	g_assert(error_tag_name!=NULL);
	
	GtkSnippetsGtvVar *self;
	self = g_object_new (GTKSNIPPETS_TYPE_GTV_VAR, NULL);
	gsnippets_variable_rebuild(GSNIPPETS_VARIABLE(self),var_def);
	self->priv->view = view;
	self->priv->start_mark = start_mark;
	self->priv->end_mark = end_mark;
	self->priv->tag_name = tag_name;
	self->priv->error_tag_name = error_tag_name;
	return self;
}

void
gtksnippets_gtv_var_add_mirror(GtkSnippetsGtvVar *self, GtkSnippetsGtvVar *mirror)
{
	g_object_ref(mirror);
	self->priv->mirrors = g_list_prepend(self->priv->mirrors,
					     mirror);
}

GList*
gtksnippets_gtv_var_get_mirrors(GtkSnippetsGtvVar *self)
{
	return self->priv->mirrors;
}

GtkTextMark*
gtksnippets_gtv_var_get_start_mark(GtkSnippetsGtvVar *self)
{
	return self->priv->start_mark;
}

GtkTextMark*
gtksnippets_gtv_var_get_end_mark(GtkSnippetsGtvVar *self)
{
	return self->priv->end_mark;
}

gchar*
gtksnippets_gtv_var_get_text(GtkSnippetsGtvVar *self)
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextIter start_var, end_var;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_var,self->priv->start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_var,self->priv->end_mark);
	return gtk_text_buffer_get_text(buffer,&start_var,&end_var,FALSE);
}

const gchar *
gtksnippets_gtv_var_get_tag_name(GtkSnippetsGtvVar *self)
{
	return self->priv->tag_name;
}

void
gtksnippets_gtv_var_set_tag_name(GtkSnippetsGtvVar *self,
				 const gchar * tag_name)
{
	self->priv->tag_name = tag_name;
}

const gchar *
gtksnippets_gtv_var_get_error_tag_name(GtkSnippetsGtvVar *self)
{
	return self->priv->error_tag_name;
}

void
gtksnippets_gtv_var_set_error_tag_name(GtkSnippetsGtvVar *self,
				       const gchar * error_tag_name)
{
	self->priv->error_tag_name = error_tag_name;
}

const gchar *
gtksnippets_gtv_var_get_current_tag_name(GtkSnippetsGtvVar *self)
{
	return self->priv->current_tag_name;
}

void
gtksnippets_gtv_var_set_text(GtkSnippetsGtvVar *self, 
			const gchar* text,
			GError **error)
{
	GError *tmp_error = NULL;
	GtkTextIter start_var, end_var;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	gtk_text_buffer_get_iter_at_mark(buffer,&start_var,self->priv->start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_var,self->priv->end_mark);
	
	gchar *final = gsnippets_variable_parse_value(GSNIPPETS_VARIABLE(self),
							text,
							&tmp_error);
	if (tmp_error !=NULL)
	{
		g_assert(final == NULL);
		gtk_text_buffer_apply_tag_by_name (buffer, 
					   self->priv->error_tag_name,
					   &start_var,
					   &end_var);
		g_propagate_error(error,tmp_error);
		self->priv->current_tag_name = self->priv->error_tag_name;
		return;
	}
	
	gtk_text_buffer_delete(buffer,&start_var,&end_var);
	gtk_text_buffer_insert_with_tags_by_name(buffer,&start_var,final,-1,self->priv->tag_name,NULL);
	
	self->priv->current_tag_name = self->priv->tag_name;
	GList *list = self->priv->mirrors;
	while(list !=NULL)
	{
		gtksnippets_gtv_var_set_text(GTKSNIPPETS_GTV_VAR(list->data),
								final,
								&tmp_error);
		if (tmp_error !=NULL)
		{
			g_propagate_error(error,tmp_error);
			break;
		}
		list = g_list_next(list);
	}
	g_free(final);
}


