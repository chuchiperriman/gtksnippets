/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-dialog.c
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
 
  
#include "gtksnippets-dialog.h"
#include <glade/glade.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>

struct _GtkSnippetsDialogPrivate
{
	/*We need one or more fields*/
	gint dummy;
	GladeXML *gxml;
};

static GtkWindowClass* parent_class = NULL;
/*Type definition*/
G_DEFINE_TYPE(GtkSnippetsDialog, gtksnippets_dialog, GTK_TYPE_WINDOW);

#define GTKSNIPPETS_DIALOG_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_DIALOG, GtkSnippetsDialogPrivate))
#define GLADE_FILE GLADE_DIR"/gtksnippets.glade"

GtkWidget*
gtksnippets_dialog_source_new(gchar *widget_name, gchar *string1, gchar
                       *string2, gint int1, gint int2)
{
	GtkWidget *source = gtk_source_view_new();
	gtk_widget_show(source);
	return source;
}

static void
_load_from_glade(GtkSnippetsDialog *self)
{
	self->priv->gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	g_assert(self->priv->gxml!=NULL);
	GtkWidget *main_box = glade_xml_get_widget (self->priv->gxml, "main_box");
	g_assert(main_box!=NULL);
	gtk_widget_show_all(main_box);
	gtk_widget_reparent(main_box,GTK_WIDGET(self));
	
	gtk_window_set_default_size(GTK_WINDOW(self),500,400);
}

static void
gtksnippets_dialog_init (GtkSnippetsDialog *self)
{
	self->priv = GTKSNIPPETS_DIALOG_GET_PRIVATE(self);
	_load_from_glade(self);
}

static void
gtksnippets_dialog_finalize (GObject *object)
{
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(object);
	
	g_object_unref(self->priv->gxml);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtksnippets_dialog_class_init (GtkSnippetsDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = GTK_WINDOW_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtksnippets_dialog_finalize;

	g_type_class_add_private (object_class, sizeof(GtkSnippetsDialogPrivate));
}


GtkSnippetsDialog*
gtksnippets_dialog_new(void)
{
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(g_object_new (GTKSNIPPETS_TYPE_DIALOG, NULL));
	/*Window type (TOPLEVEL,POPUP...)*/
	GTK_WINDOW(self)->type = GTK_WINDOW_TOPLEVEL;
	return self;
}


