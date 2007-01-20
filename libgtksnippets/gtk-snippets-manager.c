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

#include <gdk/gdkkeysyms.h>
#include "gtk-snippets-gsv-utils.h"
#include "gtk-snippets-manager.h"
#include "gtk-snippets-popup-dialog.h"


static GObjectClass* parent_class = NULL;

struct _GtkSnippetsManagerPrivate {
	/* Place Private Members Here */
	GtkSnippetsLoader *loader;
	GtkSnippetsPopupDialog *popup;
};

typedef struct{
	GtkSnippetsManager *manager;
	GtkWidget *editor;
	gchar* language;
} EditorData ;

static void
gtk_snippets_manager_init (GtkSnippetsManager *object)
{
	g_debug("Construido GtkSnippetsManager");
	/* TODO: Add initialization code here */
	object->priv = g_new0(GtkSnippetsManagerPrivate, 1);
	object->priv->loader = NULL;
	// TODO: Esto habrá que ponerlo la primera vez que lo usen
	object->priv->popup = gtk_snippets_popup_dialog_new();
}

static void
gtk_snippets_manager_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsManager *cobj;
	cobj = GTK_SNIPPETS_MANAGER(object);
	
	g_object_unref(cobj->priv->loader);
	
	g_object_unref(cobj->priv->popup);
	
	g_free(cobj->priv);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	
	g_debug("Destruido GtkSnippetsManager");
}

static void
gtk_snippets_manager_class_init (GtkSnippetsManagerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtk_snippets_manager_finalize;
}

GType
gtk_snippets_manager_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsManagerClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_manager_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsManager), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_manager_init, /* instance_init */
			NULL /* value_table */
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsManager",
		                                   &our_info, 0);
	}

	return our_type;
}


GtkSnippetsManager* 
gtk_snippets_manager_new (GtkSnippetsLoader *loader)
{
	GtkSnippetsManager *obj;

	obj = GTK_SNIPPETS_MANAGER(g_object_new(GTK_TYPE_SNIPPETS_MANAGER, NULL));
	
	g_object_ref(loader);
	
	obj->priv->loader = loader;
	
	gtk_snippets_popup_dialog_set_snippets(
		obj->priv->popup,
		gtk_snippets_loader_get_snippets(obj->priv->loader));
	
	return obj;	
}

static gboolean
gtk_snippet_manager_sw_key_press_event(GtkWidget *widget,
										GdkEventKey *event,
										gpointer user_data)
{

	static gboolean es_c = TRUE;
	gchar* word;
	gint x, y;
	FilterData filter_data;
	EditorData *data =(EditorData*)user_data;
	
	if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_space)
	{
		
		word = gtk_snippets_gsv_get_last_word(GTK_TEXT_VIEW(data->editor));
		
		filter_data.tag = word;
		if (es_c)
			filter_data.language = "C";
		else
			filter_data.language = "PYTHON";
		
		es_c = !es_c;
			
		gtk_snippets_popup_dialog_filter(
				data->manager->priv->popup,
				&filter_data);
		
		gtk_snippets_gsv_get_screen_pos(GTK_TEXT_VIEW(data->editor), &x, &y);
		
		gtk_snippets_popup_dialog_set_pos(data->manager->priv->popup, x, y);
		
		gtk_snippets_popup_dialog_show(data->manager->priv->popup,word);
		
	}
	
	return FALSE;
}

static void
gtk_snippet_manager_sw_destroy_event (GtkObject *object, 
										gpointer   user_data)
{
	g_free(user_data);
	g_debug("Han destruido un editor");
}

void
gtk_snippets_manager_add_support (GtkSnippetsManager *manager, gpointer editor, gchar* language)
{

	EditorData *data;
	
	data = g_new0(EditorData,1);
	
	/* TODO: Poner esto genérico, no solo para el gtksourceview */
	GtkSourceView *source_view = GTK_SOURCE_VIEW(editor);
	
	data->language = language;
	data->editor = editor;
	data->manager = manager;
	
	g_signal_connect(GTK_WIDGET(source_view), "key-press-event",
		G_CALLBACK(gtk_snippet_manager_sw_key_press_event),(gpointer) data);
	
	g_signal_connect(GTK_WIDGET(source_view), "destroy",
		G_CALLBACK(gtk_snippet_manager_sw_destroy_event),(gpointer) data);
	
}
