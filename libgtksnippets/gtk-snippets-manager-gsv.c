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
#include "gtk-snippet.h"
#include "gtk-snippets-gsv-utils.h"
#include "gtk-snippets-manager-gsv.h"
#include "gtk-snippets-popup-dialog.h"

static GObjectClass* parent_class = NULL;

enum _EditorDataHandlers {
	//Enumerado de las señales que hay
	HANDLER_ID_EDITOR_KEY_PRESSED,
	HANDLER_ID_EDITOR_DESTROY,
	HANDLER_ID_SNIPPET_SELECTED,
	HANDLER_ID_SNIPPET_IGNORED,
	LAST_HANDLER
};

struct _GtkSnippetsManagerGsvPrivate {
	/* Place Private Members Here */
	GtkSnippetsLoader *loader;
	GtkSnippetsPopupDialog *popup;
	GHashTable *editors_hash;
};

typedef struct{
	GtkSnippetsManagerGsv *manager;
	GtkWidget *editor;
	gchar* language;
	GtkTextIter word_start;
	GtkTextIter word_end;
	gulong signal_handles[LAST_HANDLER];
} EditorData ;

static void
gtk_snippets_manager_disconnect_editor_signals(
		EditorData *data)
{
	g_debug("Disconnecting editor signals");
	if (g_signal_handler_is_connected (data->editor, data->signal_handles[HANDLER_ID_EDITOR_KEY_PRESSED]))
		g_signal_handler_disconnect (data->editor, data->signal_handles[HANDLER_ID_EDITOR_KEY_PRESSED]);
		
	if (g_signal_handler_is_connected (data->editor, data->signal_handles[HANDLER_ID_EDITOR_DESTROY]))
		g_signal_handler_disconnect (data->editor, data->signal_handles[HANDLER_ID_EDITOR_DESTROY]);
}


static void
gtk_snippets_manager_disconnect_popup_signals(
		GtkSnippetsPopupDialog *popup, 
		EditorData *data)
{
	g_debug("Disconnecting popup signals");
	if (g_signal_handler_is_connected (popup, data->signal_handles[HANDLER_ID_SNIPPET_SELECTED]))
		g_signal_handler_disconnect (popup, data->signal_handles[HANDLER_ID_SNIPPET_SELECTED]);
		
	if (g_signal_handler_is_connected (popup, data->signal_handles[HANDLER_ID_SNIPPET_IGNORED]))
		g_signal_handler_disconnect (popup, data->signal_handles[HANDLER_ID_SNIPPET_IGNORED]);
}

static void
gtk_snippets_manager_gsv_destroy_editor_data(gpointer data)
{
	if (data!=NULL)
	{
		EditorData *ed = (EditorData*)data;
		
		gtk_snippets_manager_disconnect_editor_signals(ed);
		
		g_free(ed->language);
		g_free(ed);
		g_debug("EditorData Destroyed");
	}
}

static void
gtk_snippets_manager_gsv_init (GtkSnippetsManagerGsv *object)
{
	g_debug("Construido GtkSnippetsManagerGsv");
	/* TODO: Add initialization code here */
	object->priv = g_new0(GtkSnippetsManagerGsvPrivate, 1);
	object->priv->loader = NULL;
	// TODO: Esto habrá que ponerlo la primera vez que lo usen
	object->priv->popup = gtk_snippets_popup_dialog_new();
	//Null para que coga como clave un puntero
	object->priv->editors_hash = g_hash_table_new_full(
		NULL,
		NULL,
		NULL,
		gtk_snippets_manager_gsv_destroy_editor_data);
}

static void
gtk_snippets_manager_gsv_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsManagerGsv *cobj;
	cobj = GTK_SNIPPETS_MANAGER_GSV(object);
	
	g_object_unref(cobj->priv->loader);
	
	g_object_unref(cobj->priv->popup);
	
	g_hash_table_destroy(cobj->priv->editors_hash);
	
	g_free(cobj->priv);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	
	g_debug("Destruido GtkSnippetsManagerGsv");
}

static void
gtk_snippets_manager_gsv_class_init (GtkSnippetsManagerGsvClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtk_snippets_manager_gsv_finalize;
}

GType
gtk_snippets_manager_gsv_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsManagerGsvClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_manager_gsv_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsManagerGsv), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_manager_gsv_init, /* instance_init */
			NULL /* value_table */
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsManagerGsv",
		                                   &our_info, 0);
	}

	return our_type;
}

static void
gsm_set_snippets_to_popup(GtkSnippetsManagerGsv *manager)
{
	gtk_snippets_popup_dialog_set_snippets(
		manager->priv->popup,
		gtk_snippets_loader_get_snippets(manager->priv->loader));
}

static void
gsm_snippets_changed_cb(GtkSnippetsLoader *loader,gpointer user_data)
{
	g_debug("Cambian los snippets");
	gsm_set_snippets_to_popup(GTK_SNIPPETS_MANAGER_GSV(user_data));
	
}

/**
 * gtk_snippets_manager_gsv_new:
 * @loader: A #GtkSnippetsLoader initialized.
 *
 * Creates a new #GtkSnippetsManagerGsv .
 *
 * Returns: new allocated #GtkSnippetsLoader.
 */
GtkSnippetsManagerGsv* 
gtk_snippets_manager_gsv_new (GtkSnippetsLoader *loader)
{
	GtkSnippetsManagerGsv *obj;

	g_debug("Empezamos new");
	obj = GTK_SNIPPETS_MANAGER_GSV(g_object_new(GTK_TYPE_SNIPPETS_MANAGER_GSV, NULL));
	
	g_object_ref(loader);
	
	obj->priv->loader = loader;
	
	g_debug("Antes de poner los snippets");
	
	gsm_set_snippets_to_popup(obj);
		
	g_signal_connect(loader, "snippets-changed",
		G_CALLBACK(gsm_snippets_changed_cb),(gpointer) obj);
		
	g_debug("Puestos snippets en el dialog");
	
	return obj;	
}

static void
gtk_snippets_manager_snippet_selected_cb (GtkSnippetsPopupDialog *popup, GtkSnippet *snippet, gpointer user_data)
{
	const gchar *text;
	GtkTextBuffer *buffer;
	EditorData *data =(EditorData*)user_data;
	
	gtk_snippets_manager_disconnect_popup_signals(popup,data);
	
	text = gtk_snippet_get_text(snippet);
	
	//Hay que borrar la palabra escrita para el snippet
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->editor));
	
	gtk_text_buffer_delete(buffer,&data->word_start,&data->word_end);
	gtk_text_buffer_insert(buffer, &data->word_start, text,-1);

	g_debug("Snippet selected");
}

static void
gtk_snippets_manager_snippet_ignored_cb (GtkSnippetsPopupDialog *popup, gpointer user_data)
{
	EditorData *data =(EditorData*)user_data;
	gtk_snippets_manager_disconnect_popup_signals(popup, data);	
}

static gboolean
gtk_snippets_manager_sw_key_press_event(GtkWidget *widget,
										GdkEventKey *event,
										gpointer user_data)
{
	gchar* word;
	gint x, y;
	FilterData filter_data;
	EditorData *data =(EditorData*)user_data;
	
	if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_space)
	{
		word = gtk_snippets_gsv_get_last_word_and_iter(
					GTK_TEXT_VIEW(data->editor),
					&data->word_start,
					&data->word_end);
		
		filter_data.tag = word;
		filter_data.language = data->language;
			
		gtk_snippets_popup_dialog_filter(
				data->manager->priv->popup,
				&filter_data);
		
		gtk_snippets_gsv_get_screen_pos(GTK_TEXT_VIEW(data->editor), &x, &y);
		
		gtk_snippets_popup_dialog_set_pos(data->manager->priv->popup, x, y);
		
		gtk_snippets_popup_dialog_show(data->manager->priv->popup,word);
		
		//TODO Conectamos pero hay que desconectar. Esto está para pruebas
		data->signal_handles[HANDLER_ID_SNIPPET_SELECTED] =
			g_signal_connect(data->manager->priv->popup, "snippet-selected",
				G_CALLBACK(gtk_snippets_manager_snippet_selected_cb),(gpointer) data);
			
		data->signal_handles[HANDLER_ID_SNIPPET_IGNORED] =
			g_signal_connect(data->manager->priv->popup, "snippet-ignored",
				G_CALLBACK(gtk_snippets_manager_snippet_ignored_cb),(gpointer) data);
		
	}
	
	return FALSE;
}

static void
gtk_snippets_manager_sw_destroy_event (GtkObject *editor, 
										gpointer   data)
{
	g_debug("Han destruido un editor");
	GtkSnippetsManagerGsv *manager = GTK_SNIPPETS_MANAGER_GSV(data);
	//Se encarga de llamar a la función que destruye los datos
	g_hash_table_remove(manager->priv->editors_hash,editor);
	
}

void
gtk_snippets_manager_gsv_add_support (GtkSnippetsManagerGsv *manager, const gpointer editor, const gchar* language)
{

	EditorData *data;
	
	data = g_new0(EditorData,1);
	g_debug("EditorData created");
	
	/* TODO: Poner esto genérico, no solo para el gtksourceview */
	GtkSourceView *source_view = GTK_SOURCE_VIEW(editor);
	
	data->language = g_strdup(language);
	data->editor = editor;
	data->manager = manager;
	
	g_hash_table_insert(manager->priv->editors_hash, editor, data);
	
	data->signal_handles[HANDLER_ID_EDITOR_KEY_PRESSED] =
		g_signal_connect(GTK_WIDGET(source_view), "key-press-event",
			G_CALLBACK(gtk_snippets_manager_sw_key_press_event),(gpointer) data);
	
	data->signal_handles[HANDLER_ID_EDITOR_DESTROY] =
		g_signal_connect(GTK_WIDGET(source_view), "destroy",
			G_CALLBACK(gtk_snippets_manager_sw_destroy_event),(gpointer) manager);
	
}

void
gtk_snippets_manager_gsv_remove_support (GtkSnippetsManagerGsv *manager, gpointer editor)
{
	//Al quitar del hash llama a destruir el EditorData (y desconecta todo lo que tiene que desconectar)
	g_hash_table_remove(manager->priv->editors_hash, editor);
}

GtkSnippetsLoader*
gtk_snippets_manager_gsv_get_loader (GtkSnippetsManagerGsv *manager)
{
	return manager->priv->loader;
}

