/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gtksnippets
 * Copyright (C) chuchi 2007 <chuchiperriman@gmail.com>
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

#include <glade/glade.h>
#include "gtk-text-completion-popup.h"

#define GLADE_FILE GLADE_DIR"/gtk-text-completion-popup.glade"

struct _GtkTextCompletionPopupPrivate
{
	GtkTextView *text_view;
	GtkWidget *window;
};

#define GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopupPrivate))

enum
{
	POPULATE_COMPLETION,
	LAST_SIGNAL
};

static GObjectClass* parent_class = NULL;
static guint text_completion_popup_signals[LAST_SIGNAL] = { 0 };

static void
gtcp_load_glade(GtkTextCompletionPopup *popup)
{
	g_debug("load glade");
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);

	g_assert(gxml != NULL);

	//glade_xml_signal_autoconnect (gxml);
	g_debug("despues conect load glade");
	popup->priv->window = glade_xml_get_widget (gxml, "window");
	g_debug("cogemos el window");
	
	/*g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "row-activated",
		G_CALLBACK(snippets_tree_view_row_activated_cb),(gpointer) obj);*/
	
	//Load the GtkSourceView
	/*obj->priv->source_scroll = glade_xml_get_widget(gxml,"snippets_content_scroll");
	obj->priv->source = gtk_source_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(obj->priv->source),FALSE);
	gtk_source_buffer_set_highlight(
		GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj->priv->source))),
		TRUE);
	
	gtk_container_add(GTK_CONTAINER(obj->priv->source_scroll), obj->priv->source);
	*/
	
	g_object_unref(gxml);
	g_debug("sacabo load glade");
}

static void
gtk_text_completion_popup_init (GtkTextCompletionPopup *popup)
{
	popup->priv = GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(popup);
	gtcp_load_glade(popup);
}

static void
gtk_text_completion_popup_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_text_completion_popup_populate_completion (GtkTextCompletionPopup* popup)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gtk_text_completion_popup_class_init (GtkTextCompletionPopupClass *klass)
{
	g_debug("class init");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtk_text_completion_popup_finalize;

	klass->populate_completion = gtk_text_completion_popup_populate_completion;

	/*text_completion_popup_signals[POPULATE_COMPLETION] =
		g_signal_new ("populate-completion",
		              G_OBJECT_CLASS_TYPE (klass),
		              0, 
		              G_STRUCT_OFFSET (GtkTextCompletionPopupClass, populate_completion),
		              NULL, NULL,
		              G_TYPE_NONE, 0,
		              NULL
		              );*/
	g_debug("casi fin class init");
	g_type_class_add_private (object_class, sizeof(GtkTextCompletionPopupPrivate));
	g_debug("fin class init");
}

GType
gtk_text_completion_popup_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkTextCompletionPopupClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_text_completion_popup_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkTextCompletionPopup), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_text_completion_popup_init, /* instance_init */
			NULL /* value_table */  
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkTextCompletionPopup",
		                                   &our_info, 0);
	}

	return our_type;
}

GtkTextCompletionPopup*
gtk_text_completion_popup_new (GtkTextView *view)
{
	GtkTextCompletionPopup *popup = GTK_TEXT_COMPLETION_POPUP (g_object_new (GTK_TYPE_TEXT_COMPLETION_POPUP, NULL));
	popup->priv->text_view = view;
	return popup;
}
