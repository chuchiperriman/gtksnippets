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
#include <gdk/gdkkeysyms.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include "gtk-snippet.h"
#include "gtk-snippets-popup-dialog.h"

#define GLADE_FILE GLADE_DIR"/gtk-snippets-popup-dialog.glade"

#define COL_LANGUAGE 0
#define COL_NAME 1
#define COL_SNIPPET 2

struct _GtkSnippetsPopupDialogPrivate
{
	GtkWidget* window;
	GtkWidget* tree_view;
	GtkWidget* source;
	GtkWidget* source_scroll;
	gboolean source_visible;
	GtkSnippet *selected_snippet;
	GHashTable* hash_languages;
	gint x;
	gint y;
};

#define GTK_SNIPPETS_POPUP_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialogPrivate))

enum _GtkSnippetsPopupDialogSignalType {
	//Enumerado de las señales que hay
	SIGNAL_TYPE_SNIPPET_SELECTED,
	SIGNAL_TYPE_SNIPPET_IGNORED,
	LAST_SIGNAL
};

static GObjectClass* parent_class = NULL;
static guint gtk_snippets_popup_dialog_signals[LAST_SIGNAL] = { 0 };



static void
gtk_snippets_popup_dialog_init (GtkSnippetsPopupDialog *popup_dialog)
{
	g_debug("Construido GtkSnippetsPopupDialog");
	popup_dialog->priv = g_new0(GtkSnippetsPopupDialogPrivate, 1);
	popup_dialog->priv->x = 0;
	popup_dialog->priv->y = 0;
	popup_dialog->priv->selected_snippet = NULL;
	popup_dialog->priv->source_visible = FALSE;
	popup_dialog->priv->hash_languages = g_hash_table_new(g_str_hash,g_str_equal);
}

static void
gtk_snippets_popup_dialog_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsPopupDialog *cobj;
	cobj = GTK_SNIPPETS_POPUP_DIALOG(object);

	//Si destruimos la ventana se destruyen los hijos
	gtk_widget_destroy(cobj->priv->window);
	g_hash_table_destroy(cobj->priv->hash_languages);
	
	g_free(cobj->priv);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	g_debug("Destruido GtkSnippetsPopupDialog");
}

static void
gtk_snippets_popup_dialog_class_init (GtkSnippetsPopupDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	g_type_class_add_private (klass, sizeof (GtkSnippetsPopupDialogPrivate));

	object_class->finalize = gtk_snippets_popup_dialog_finalize;
	
	//Señales
	
	//Se lanza cuando se selecciona un snippet. EL parámetro 1 es el snippet seleccionado
	gtk_snippets_popup_dialog_signals[SIGNAL_TYPE_SNIPPET_SELECTED] = 
			g_signal_new("snippet-selected",
					G_TYPE_FROM_CLASS(klass),
					G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
					0,
					NULL,
					NULL,
					g_cclosure_marshal_VOID__POINTER,
					G_TYPE_NONE,
					1,
					G_TYPE_POINTER);
	
	//Se lanza cuando se cierra el diálogo de snippets sin seleccionar nada				
	gtk_snippets_popup_dialog_signals[SIGNAL_TYPE_SNIPPET_IGNORED] = 
			g_signal_new("snippet-ignored",
					G_TYPE_FROM_CLASS(klass),
					G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
					0,
					NULL,
					NULL,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE,
					0);
}

GType
gtk_snippets_popup_dialog_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsPopupDialogClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_popup_dialog_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsPopupDialog), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_popup_dialog_init, /* instance_init */
			NULL /* value_table */
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsPopupDialog",
		                                   &our_info, 0);
	}

	return our_type;
}


static gboolean
snippets_popup_dialog_focus_out_event_cb(GtkWidget *widget,
	GdkEventFocus *event,
	gpointer user_data)
{

	g_debug("focus out");

	GtkSnippetsPopupDialog *popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
	if (popup->priv->selected_snippet == NULL)
	{
		g_signal_emit(
				popup,
				gtk_snippets_popup_dialog_signals[SIGNAL_TYPE_SNIPPET_IGNORED],
				0);
	}
	
	gtk_widget_hide(widget);
	return FALSE;
}

static void
snippets_tree_view_row_activated_cb(GtkTreeView *tree_view,
		GtkTreePath *path,
		GtkTreeViewColumn *column,
		gpointer user_data)
{

	GtkSnippetsPopupDialog *popup;
	GtkTreeModel *tree_model;
	GtkTreeIter iter;
	GValue value = {0,};
	GtkSnippet *snippet;
	
	popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
	gtk_widget_hide(popup->priv->window);
	
	tree_model = gtk_tree_view_get_model(tree_view);
	
	if (gtk_tree_model_get_iter(tree_model, &iter, path))
	{
		gtk_tree_model_get_value(tree_model, &iter, COL_SNIPPET, &value);
		snippet = GTK_SNIPPET(g_value_get_pointer(&value));

		popup->priv->selected_snippet = snippet;
		
		g_debug("Selected item: %s",gtk_snippet_get_tag(snippet));
		g_debug("item text: %s",gtk_snippet_get_text(snippet));
		
		//Lanzamos una señal de que se ha seleccionado un snippet
		g_signal_emit(
				popup,
				gtk_snippets_popup_dialog_signals[SIGNAL_TYPE_SNIPPET_SELECTED],
				0,
				snippet);
	}
	g_debug("Row selected");
}

static gboolean
gspd_key_release_esc_hide_cb(GtkWidget *widget,
										GdkEventKey *event,
										gpointer user_data)
{
	GtkSnippetsPopupDialog *popup;
	
	if (event->keyval == GDK_Escape)
	{
		popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
		gtk_widget_hide(popup->priv->window);
	}
	
	return FALSE;
}

static GtkSnippet*
gspd_get_active_snippet(GtkTreeView *tree_view)
{
	GtkTreePath *path;
	GtkTreeIter iter;
	GValue value = {0,};
	GtkTreeModel *tree_model;
	GtkSnippet *snippet = NULL;
	
	tree_model = gtk_tree_view_get_model(tree_view);
	
	gtk_tree_view_get_cursor(tree_view,&path,NULL);
	if (path)
	{
		if (gtk_tree_model_get_iter(tree_model, &iter, path))
		{
			gtk_tree_model_get_value(tree_model, &iter, COL_SNIPPET, &value);
			snippet = GTK_SNIPPET(g_value_get_pointer(&value));
		}
		gtk_tree_path_free(path);
	}
	
	return snippet;
}
static void
gspd_cursor_changed_tree_cb(GtkTreeView *tree_view, gpointer user_data)
{
	GtkSnippetsPopupDialog *popup;
	GtkSnippet *snippet;
	GtkTextBuffer *text_buffer;
	
	popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(popup->priv->source));
	
	if (popup->priv->source_visible)
	{
		snippet = gspd_get_active_snippet(tree_view);
		if (snippet != NULL)
			gtk_text_buffer_set_text(text_buffer,gtk_snippet_get_text(snippet),-1);
		else
			gtk_text_buffer_set_text(text_buffer,"",-1);
	}
}

static gboolean
gspd_key_release_tree_cb(GtkWidget *widget,
								GdkEventKey *event,
								gpointer user_data)
{
	GtkSnippetsPopupDialog *popup;
	
	if (event->keyval == GDK_Right)
	{
		popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
		gtk_widget_show(popup->priv->source_scroll);
		gtk_widget_show(popup->priv->source);
		popup->priv->source_visible = TRUE;
		//To load the snippet content on the source
		gspd_cursor_changed_tree_cb(GTK_TREE_VIEW(widget),user_data);
	}
	else if (event->keyval == GDK_Left)
	{
		popup = GTK_SNIPPETS_POPUP_DIALOG(user_data);
		gtk_widget_hide(popup->priv->source_scroll);
		gtk_widget_hide(popup->priv->source);
		popup->priv->source_visible = FALSE;
	}
	return FALSE;
}

static void
gspd_load_glade(GtkSnippetsPopupDialog *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "snippets_popup_dialog");
	obj->priv->tree_view = glade_xml_get_widget(gxml,"snippets_tree_view");
	
	g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "row-activated",
		G_CALLBACK(snippets_tree_view_row_activated_cb),(gpointer) obj);
	
	g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "key-release-event",
		G_CALLBACK(gspd_key_release_tree_cb),(gpointer) obj);
	
	g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "cursor-changed",
		G_CALLBACK(gspd_cursor_changed_tree_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->window), "focus-out-event",
		G_CALLBACK(snippets_popup_dialog_focus_out_event_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->window), "key-release-event",
		G_CALLBACK(gspd_key_release_esc_hide_cb),(gpointer) obj);
		
	//Load the GtkSourceView
	obj->priv->source_scroll = glade_xml_get_widget(gxml,"snippets_content_scroll");
	obj->priv->source = gtk_source_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(obj->priv->source),FALSE);
	gtk_source_buffer_set_highlight(
		GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj->priv->source))),
		TRUE);
	
	gtk_container_add(GTK_CONTAINER(obj->priv->source_scroll), obj->priv->source);
	
	g_object_unref(gxml);
}

/**
* Crea el arbol pero no hace nada con el modelo
*/
static void
gspd_load_tree(GtkSnippetsPopupDialog *popup_dialog)
{
	g_assert(popup_dialog!=NULL);
	
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	/* crea una columna */
	column = gtk_tree_view_column_new();
	/* coloca el nombre a la columna */
	//gtk_tree_view_column_set_title(column, "Snippet");
	/* crea un render tipo texto */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",1,NULL);

	/* agrega la columna al arbol */
	gtk_tree_view_append_column (GTK_TREE_VIEW(popup_dialog->priv->tree_view), column);
}

static void
gspd_load_languages(GtkSnippetsPopupDialog *popup_dialog)
{
	GtkSourceLanguagesManager* lang_manager;
	GtkSourceLanguage *lang;
	const GSList *lang_list;
	
	lang_manager = gtk_source_languages_manager_new();
	
	lang_list = gtk_source_languages_manager_get_available_languages(lang_manager);

	while(lang_list)
	{
		lang = GTK_SOURCE_LANGUAGE(lang_list->data);
		g_hash_table_insert(
			popup_dialog->priv->hash_languages,
			gtk_source_language_get_name(lang),
			lang);

		lang_list = g_slist_next(lang_list);
	}
	
}

GtkSnippetsPopupDialog*
gtk_snippets_popup_dialog_new (void)
{
	GtkSnippetsPopupDialog *obj;

	obj = GTK_SNIPPETS_POPUP_DIALOG(g_object_new(GTK_TYPE_SNIPPETS_POPUP_DIALOG, NULL));
	
	/* TODO: Add initialization code here */
	gspd_load_languages(obj);
	gspd_load_glade(obj);
	gspd_load_tree(obj);
	
	return obj;	
}

GtkWidget*
gtk_snippets_popup_dialog_get_window(GtkSnippetsPopupDialog* popup_dialog)
{
	return popup_dialog->priv->window;
}

void 
gtk_snippets_popup_dialog_set_pos(GtkSnippetsPopupDialog* popup_dialog, gint x, gint y)
{
	popup_dialog->priv->x = x;
	popup_dialog->priv->y = y;
}

void
gtk_snippets_popup_dialog_show(GtkSnippetsPopupDialog* popup_dialog,
								const gchar *word)
{

	popup_dialog->priv->selected_snippet = NULL;
	gspd_cursor_changed_tree_cb(GTK_TREE_VIEW(popup_dialog->priv->tree_view),popup_dialog);
	
	gtk_window_move (GTK_WINDOW (popup_dialog->priv->window),popup_dialog->priv->x,popup_dialog->priv->y);
	gtk_widget_show (popup_dialog->priv->window);
	gtk_widget_grab_focus(popup_dialog->priv->tree_view);

}

static void
gspd_hash_for_each_add_snippet (gpointer key,
		gpointer value,
		gpointer user_data)
{

	GtkTreeIter iter;
	GtkSnippet *snippet;
	GList *lista;
	
	g_assert(user_data!=NULL);
	g_assert(key!=NULL);
	
	if (value != NULL)
	{
		lista = (GList*)value;
		do
		{
			g_assert(lista->data != NULL);
			
			snippet = GTK_SNIPPET(lista->data);
			//Insertamos los datos
			gtk_list_store_append (GTK_LIST_STORE(user_data),&iter);
			
			gtk_list_store_set (GTK_LIST_STORE(user_data), 
								&iter,
								COL_LANGUAGE, gtk_snippet_get_language(snippet),
								COL_NAME, gtk_snippet_get_name(snippet),
								COL_SNIPPET, snippet,
								-1);
		}while((lista = g_list_next(lista))!=NULL);
	}

}

void
gtk_snippets_popup_dialog_set_snippets(GtkSnippetsPopupDialog* popup_dialog, GHashTable* snippets)
{

	g_assert(popup_dialog!=NULL);
	
	if (snippets != NULL)
	{
		g_debug("Asignan nuevos snippets");

		GtkListStore *list_store;
		
		
		//lenguaje,nombreSnippet,GtkSnippet
		list_store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
		
		g_hash_table_foreach (snippets,gspd_hash_for_each_add_snippet,list_store);
		
		g_debug("Asignamos el modelo");
		gtk_tree_view_set_model(GTK_TREE_VIEW(popup_dialog->priv->tree_view),GTK_TREE_MODEL(list_store));
	}
	else
	{
		g_debug("Se quisieron añadir snippets pero son NULL");
	}	
}

static gboolean
gspd_filter_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	g_debug("Filter function");
	GValue value = {0,};
	const gchar *snippet_language;
	const gchar *snippet_tag;
	FilterData *filter_data;
	gboolean show = TRUE;
	GtkSnippet *snippet;
	
	filter_data = (FilterData*)data;
	
	gtk_tree_model_get_value(model,iter,COL_SNIPPET,&value);
	g_debug("antes value");
	snippet = GTK_SNIPPET(g_value_get_pointer(&value));
	g_debug("des value");
	
	//Language filter
	if (filter_data->language != NULL)
	{
		snippet_language = gtk_snippet_get_language(snippet);
		if (strcmp(filter_data->language, snippet_language) != 0)
		{
			show = FALSE;
		}
	}
	
	//Tag filter
	if (show == TRUE && filter_data->tag != NULL)
	{
		snippet_tag = gtk_snippet_get_tag(snippet);
		if (strncmp(filter_data->tag, snippet_tag, strlen(filter_data->tag)) != 0)
		{
			show = FALSE;
		}
	}	
	return show;
}	
	
void
gtk_snippets_popup_dialog_filter(GtkSnippetsPopupDialog* popup_dialog, const FilterData *filter_data)
{
	g_debug("Iniciamos filtrado");
	GtkTreeModel *model_filter;
	GtkTreeModel *model_actual;
	GtkTextBuffer *buffer;
		
	model_actual = gtk_tree_view_get_model(GTK_TREE_VIEW(popup_dialog->priv->tree_view));
	//TODO comprobar si el lenguaje que mandan es por el que ya está filtrado, entonces no hacemos nada
	if (GTK_IS_TREE_MODEL_FILTER(model_actual))
	{
		//Cogemos el modelo hijo (el que tiene todos los datos)
		model_filter = gtk_tree_model_filter_new (
				gtk_tree_model_filter_get_model(GTK_TREE_MODEL_FILTER(model_actual)), 
				NULL);
	}
	else
	{
		model_filter = gtk_tree_model_filter_new (model_actual, NULL);
	}
	
	gtk_tree_model_filter_set_visible_func(
		GTK_TREE_MODEL_FILTER(model_filter),
		gspd_filter_func,
		(gpointer)filter_data,
		NULL);
		
	gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(model_filter));
			
	gtk_tree_view_set_model(GTK_TREE_VIEW(popup_dialog->priv->tree_view),model_filter);

	//Setting source language	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(popup_dialog->priv->source));
	gtk_source_buffer_set_language(
		GTK_SOURCE_BUFFER(buffer),
		g_hash_table_lookup(popup_dialog->priv->hash_languages,filter_data->language));
		
}

/*
void
gtk_snippets_popup_dialog_filter_by_language(GtkSnippetsPopupDialog* popup_dialog,gchar* language)
{
	FilterData filter_data;
	filter_data.language = language;
	filter_data.tag = NULL;
	gtk_snippets_popup_dialog_filter(popup_dialog,&filter_data);
	
}
*/

