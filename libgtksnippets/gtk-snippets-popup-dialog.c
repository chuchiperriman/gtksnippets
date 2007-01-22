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
#include "gtk-snippets-popup-dialog.h"

#define GLADE_FILE GLADE_DIR"/libgtksnippets.glade"

#define COL_LANGUAGE 0
#define COL_NAME 1
#define COL_SNIPPET 2

struct _GtkSnippetsPopupDialogPrivate
{
	GtkWidget* window;
	GtkWidget* entry;
	GtkWidget* tree_view;
	gint x;
	gint y;
};

#define GTK_SNIPPETS_POPUP_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialogPrivate))

static GObjectClass* parent_class = NULL;

static void
gtk_snippets_popup_dialog_init (GtkSnippetsPopupDialog *popup_dialog)
{
	g_debug("Construido GtkSnippetsPopupDialog");
	popup_dialog->priv = g_new0(GtkSnippetsPopupDialogPrivate, 1);
	popup_dialog->priv->x = 0;
	popup_dialog->priv->y = 0;
}

static void
gtk_snippets_popup_dialog_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsPopupDialog *cobj;
	cobj = GTK_SNIPPETS_POPUP_DIALOG(object);

	//Si destruimos la ventana se destruyen los hijos
	gtk_widget_destroy(cobj->priv->window);
	
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


gboolean
on_snippets_popup_dialog_focus_out_event(GtkWidget *widget,
	GdkEventFocus *event,
	gpointer user_data)
{
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
		g_debug("Selected item: %s",gtk_snippet_get_tag(snippet));
		g_debug("item text: %s",gtk_snippet_get_text(snippet));
	}
	
	g_debug("Row selected");

}

static void
gspd_load_glade(GtkSnippetsPopupDialog *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "snippets_popup_dialog");
	obj->priv->entry = glade_xml_get_widget(gxml,"snippets_text_entry");
	obj->priv->tree_view = glade_xml_get_widget(gxml,"snippets_tree_view");
	
	g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "row-activated",
		G_CALLBACK(snippets_tree_view_row_activated_cb),(gpointer) obj);
	
	g_object_unref(gxml);
}

GtkSnippetsPopupDialog*
gtk_snippets_popup_dialog_new (void)
{
	GtkSnippetsPopupDialog *obj;

	obj = GTK_SNIPPETS_POPUP_DIALOG(g_object_new(GTK_TYPE_SNIPPETS_POPUP_DIALOG, NULL));
	
	/* TODO: Add initialization code here */
	gspd_load_glade(obj);
	
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

	gtk_window_move (GTK_WINDOW (popup_dialog->priv->window),popup_dialog->priv->x,popup_dialog->priv->y);
	gtk_entry_set_text (GTK_ENTRY(popup_dialog->priv->entry),word);
	gtk_widget_show (popup_dialog->priv->window);
	gtk_widget_grab_focus(popup_dialog->priv->entry);

}

static void
gspd_hash_for_each_add_snippet (gpointer key,
		gpointer value,
		gpointer user_data)
{

	GtkTreeIter iter;
	GtkSnippet *snippet;
	
	g_assert(user_data!=NULL);
	g_assert(value!=NULL);
	g_assert(key!=NULL);
	
	snippet = GTK_SNIPPET(value);
	
	//Insertamos los datos
	gtk_list_store_append (GTK_LIST_STORE(user_data),&iter);
	
	gtk_list_store_set (GTK_LIST_STORE(user_data), 
						&iter,
						COL_LANGUAGE, gtk_snippet_get_language(snippet),
						COL_NAME, gtk_snippet_get_name(snippet),
						COL_SNIPPET, value,
						-1);

}

void
gtk_snippets_popup_dialog_set_snippets(GtkSnippetsPopupDialog* popup_dialog, GHashTable* snippets)
{

	g_assert(popup_dialog!=NULL);
	
	if (snippets != NULL)
	{
		g_debug("Asignan nuevos snippets");

		GtkCellRenderer *renderer;
		GtkTreeViewColumn *column;
		GtkListStore *list_store;
		
		
		//lenguaje,nombreSnippet,GtkSnippet
		list_store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
		
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
	GValue value = {0,};
	gchar *snippet_language;
	gchar *snippet_tag;
	FilterData *filter_data;
	gboolean show = TRUE;
	GtkSnippet *snippet;
	
	filter_data = (FilterData*)data;
	
	gtk_tree_model_get_value(model,iter,COL_SNIPPET,&value);
	snippet = GTK_SNIPPET(g_value_get_pointer(&value));
	
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
}

void
gtk_snippets_popup_dialog_filter_by_language(GtkSnippetsPopupDialog* popup_dialog,gchar* language)
{
	FilterData filter_data;
	filter_data.language = language;
	filter_data.tag = NULL;
	gtk_snippets_popup_dialog_filter(popup_dialog,&filter_data);
	
}


