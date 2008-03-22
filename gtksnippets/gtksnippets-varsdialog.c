/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-varsdialog.c
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
 
  
#include "gtksnippets-varsdialog.h"
#include <glade/glade.h>
#include <gtksourceview/gtksourceview.h>
#include "../gsnippets/gsnippets-parser.h"

typedef struct 
{
	GSnippetsVar *var;
	GtkWidget *entry;
} VarData;

struct _GtkSnippetsVarsDialogPrivate
{
	/*We need one or more fields*/
	gchar *content;
	GSList *var_list;
	GladeXML *gxml;
	GtkWidget *source;
	GtkWidget *source2;
	GtkWidget *main_box;
	GtkWidget *vars_table;
	VarData *vars;
};

static GtkDialogClass* parent_class = NULL;
/*Type definition*/
G_DEFINE_TYPE(GtkSnippetsVarsDialog, gtksnippets_varsdialog, GTK_TYPE_DIALOG);

#define GTKSNIPPETS_VARSDIALOG_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_VARSDIALOG, GtkSnippetsVarsDialogPrivate))

#define GLADE_FILE GLADE_DIR"/gtksnippets-vars.glade"

static void
_update_vars(GtkSnippetsVarsDialog *self)
{
	VarData *vdata = self->priv->vars;
	while(vdata!=NULL && vdata->var != NULL)
	{
		g_free(vdata->var->value);
		vdata->var->value = g_strdup(gtk_entry_get_text(GTK_ENTRY(vdata->entry)));
		vdata++;
	}
}

static gboolean
_entry_key_rel_cb(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GtkSnippetsVarsDialog *self = GTKSNIPPETS_VARSDIALOG(user_data);
	_update_vars(self);
	gchar* text = gsnippets_parser_replace_vars(self->priv->content,self->priv->var_list);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source2));
	gtk_text_buffer_set_text(buffer,text,-1);
	g_free(text);
	return FALSE;
}

static void
_create_var_entries(GtkSnippetsVarsDialog *self)
{
	GSList *temp = gsnippets_parser_get_vars(self->priv->content);
	self->priv->var_list = temp;
	GSnippetsVar *var;
	guint rows =  g_slist_length(temp) + 1;
	guint pos = 1;
	gtk_table_resize(GTK_TABLE(self->priv->vars_table),rows,2);
	GtkWidget *label, *entry;
	
	/*Data objects + NULL at the end*/
	self->priv->vars = g_malloc0(sizeof(VarData) * rows);
	VarData *vdata = self->priv->vars;
	while(temp!=NULL)
	{
		var = (GSnippetsVar*)temp->data;
		label = gtk_label_new(var->name);
		entry = gtk_entry_new();
		gtk_widget_show(label);
		gtk_widget_show(entry);
		gtk_table_attach_defaults(GTK_TABLE(self->priv->vars_table),
					label,
					0,
					1,
		
					pos,
					pos+1);
		gtk_table_attach_defaults(GTK_TABLE(self->priv->vars_table),
					entry,
					1,
					2,
					pos,
					pos+1);
		g_signal_connect(
			entry,
			"key-release-event",
			G_CALLBACK(_entry_key_rel_cb),
			self);
		
		pos++;
		vdata->var = var;
		vdata->entry = entry;
		temp = g_slist_next(temp);
		vdata++;
	}
	vdata->var = NULL;
}

static void
_load_from_glade(GtkSnippetsVarsDialog *self)
{
	self->priv->gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	g_assert(self->priv->gxml!=NULL);
	self->priv->main_box = glade_xml_get_widget (self->priv->gxml, "main_box");
	GtkWidget *scroll = glade_xml_get_widget (self->priv->gxml, "source_scroll");
	GtkWidget *scroll2 = glade_xml_get_widget (self->priv->gxml, "source_scroll2");
	self->priv->vars_table = glade_xml_get_widget (self->priv->gxml, "vars_table");
	gtk_widget_reparent(self->priv->main_box,GTK_DIALOG(self)->vbox);
	self->priv->source = GTK_WIDGET(gtk_source_view_new());
	self->priv->source2 = GTK_WIDGET(gtk_source_view_new());
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll),self->priv->source);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll2),self->priv->source2);
	
	gtk_widget_show_all(self->priv->main_box);
}

static void
_response_cb(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if (response_id == GTK_RESPONSE_OK)
	{
		GtkSnippetsVarsDialog *self = GTKSNIPPETS_VARSDIALOG(user_data);
		_update_vars(self);
		gchar* text = gsnippets_parser_replace_vars(self->priv->content,self->priv->var_list);
		g_free(self->priv->content);
		self->priv->content = text;
	}
}

static void
gtksnippets_varsdialog_init (GtkSnippetsVarsDialog *self)
{
	self->priv = GTKSNIPPETS_VARSDIALOG_GET_PRIVATE(self);
	self->priv->content = NULL;
	self->priv->vars = NULL;
	_load_from_glade(self);
	gtk_window_set_default_size(GTK_WINDOW(self),500,400);
	gtk_window_set_position(GTK_WINDOW(self),GTK_WIN_POS_CENTER_ALWAYS);
	
	g_signal_connect(
		self,
		"response",
		G_CALLBACK(_response_cb),
		self);
	
}

static void
gtksnippets_varsdialog_finalize (GObject *object)
{
	GtkSnippetsVarsDialog *self = GTKSNIPPETS_VARSDIALOG(object);
	g_free(self->priv->content);
	g_object_unref(self->priv->gxml);
	gsnippets_parser_free_var_list(self->priv->var_list);
	g_free(self->priv->vars);
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtksnippets_varsdialog_class_init (GtkSnippetsVarsDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = GTK_DIALOG_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtksnippets_varsdialog_finalize;

	g_type_class_add_private (object_class, sizeof(GtkSnippetsVarsDialogPrivate));
}


GtkSnippetsVarsDialog*
gtksnippets_varsdialog_new(const gchar *snippet_content)
{
	GtkSnippetsVarsDialog *self = GTKSNIPPETS_VARSDIALOG(g_object_new (GTKSNIPPETS_TYPE_VARSDIALOG, NULL));
	self->priv->content = g_strdup(snippet_content);
	_create_var_entries(self);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source));
	gtk_text_buffer_set_text(buffer,snippet_content,-1);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source2));
	gtk_text_buffer_set_text(buffer,snippet_content,-1);
	gtk_dialog_add_button(GTK_DIALOG(self),GTK_STOCK_APPLY,GTK_RESPONSE_OK);
	gtk_dialog_add_button(GTK_DIALOG(self),GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL);
	
	return self;
}

/*New allocated text*/
gchar*
gtksnippets_varsdialog_get_content(GtkSnippetsVarsDialog *self)
{
	return g_strdup(self->priv->content);
}

gchar*
gtksnippets_varsdialog_show_and_parse(const gchar *snippet_content)
{
	gchar *content = NULL;
	if (gsnippets_parser_count_vars(snippet_content) > 0)
	{
		GtkSnippetsVarsDialog* dialog = gtksnippets_varsdialog_new(snippet_content);
	
		gint response = gtk_dialog_run(GTK_DIALOG(dialog));
		if (response == GTK_RESPONSE_OK)
		{
			content = gtksnippets_varsdialog_get_content(dialog);
		}
		else
		{
			content = g_strdup(snippet_content);
		}
		gtk_widget_destroy(GTK_WIDGET(dialog));
	}
	else
	{
		content = g_strdup(snippet_content);		
	}
	
	return content;
}




