/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gtksnippets
 * Copyright (C) chuchiperriman 2007 <chuchiperriman@gmail.com>
 * 
 * gtksnippets is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gtksnippets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gtksnippets.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include "gtk-snippet.h"
#include "gtk-snippets-management-ui.h"

#define GLADE_FILE GLADE_DIR"/gtk-snippets-management-ui.glade"

#define COL_LANGUAGE 0
#define COL_NAME 1
#define COL_SNIPPET 2

struct _GtkSnippetsManagementUIPrivate
{
	GtkWidget* window;
};

#define GTK_SNIPPETS_MANAGEMENT_UI_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUIPrivate))

static GObjectClass* parent_class = NULL;

static void
gtk_snippets_management_ui_init (GtkSnippetsManagementUI *popup_dialog)
{
	g_debug("Construido GtkSnippetsManagementUI");
	popup_dialog->priv = g_new0(GtkSnippetsManagementUIPrivate, 1);
}

static void
gtk_snippets_management_ui_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsManagementUI *cobj;
	cobj = GTK_SNIPPETS_MANAGEMENT_UI(object);

	//Si destruimos la ventana se destruyen los hijos
	gtk_widget_destroy(cobj->priv->window);
	
	g_free(cobj->priv);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	g_debug("Destruido GtkSnippetsManagementUI");
}

static void
gtk_snippets_management_ui_class_init (GtkSnippetsManagementUIClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	g_type_class_add_private (klass, sizeof (GtkSnippetsManagementUIPrivate));

	object_class->finalize = gtk_snippets_management_ui_finalize;
}

GType
gtk_snippets_management_ui_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsManagementUIClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_management_ui_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsManagementUI), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_management_ui_init, /* instance_init */
			NULL /* value_table */
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsManagementUI",
		                                   &our_info, 0);
	}

	return our_type;
}


static void
gspd_load_glade(GtkSnippetsManagementUI *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "snippets_popup_dialog");
	
	g_object_unref(gxml);
}

GtkSnippetsManagementUI*
gtk_snippets_management_ui_new (void)
{
	GtkSnippetsManagementUI *obj;

	obj = GTK_SNIPPETS_MANAGEMENT_UI(g_object_new(GTK_TYPE_SNIPPETS_MANAGEMENT_UI, NULL));
	
	/* TODO: Add initialization code here */
	gspd_load_glade(obj);
	
	return obj;	
}
