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
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include "gtk-snippet.h"
#include "gtk-snippets-management-ui.h"

#define GLADE_FILE GLADE_DIR"/gtk-snippets-management-ui.glade"

#define COL_NAME 0
#define COL_SNIPPET 1

struct _GtkSnippetsManagementUIPrivate
{
	GtkWidget* window;
	GtkWidget* snippet_content;
	GtkWidget* snippet_tag;
	GtkWidget* remove_button;
	GtkWidget* new_button;
	GtkSnippetsLoader *loader;
	GtkTreeView *snippets_tree;
	GtkWidget* new_dialog;
	GtkWidget* new_dialog_entry;
	
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

GtkWidget*
gtk_smngui_create_source_view()
{
	GtkWidget *source = gtk_source_view_new();
	gtk_widget_show(source);
	return source;
}

void
smngui_dialog_response_cb(GtkDialog *dialog,
		gint response_id,
		gpointer user_data)
{
	GtkSnippetsManagementUI *mng;
	gboolean result;
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	
	g_debug("Dialog response: %i",response_id);
	
	switch(response_id)
	{
		case GTK_RESPONSE_CLOSE:
		{
			gtk_widget_hide(GTK_WIDGET(dialog));
			//TODO Grabar los snippets con el loader
			//TODO ver qué hacemos con la respuesta de grabar
			result = gtk_snippets_loader_save_default(mng->priv->loader);
			break;
		}
	}
}

static GtkSnippet*
smngui_get_active_snippet(GtkSnippetsManagementUI *mng)
{
	GtkTreePath *path;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;
	GtkTreeModel *model;
	GValue value = {0,};
	GtkSnippet *snippet = NULL;
	
	gtk_tree_view_get_cursor(mng->priv->snippets_tree,&path,&column);
	if(path && column) 
	{
		model = gtk_tree_view_get_model(mng->priv->snippets_tree);
		if(gtk_tree_model_get_iter(model,&iter,path))
		{
			gtk_tree_model_get_value(model, &iter, COL_SNIPPET, &value);
			snippet = GTK_SNIPPET(g_value_get_pointer (&value));
		}
	}

	if(path) gtk_tree_path_free(path);
	
	return snippet;
}

static const gchar*
smngui_get_all_content(GtkSnippetsManagementUI *mng)
{
	GtkTextBuffer* buffer;
	GtkTextIter ini,fin;
	const gchar* content;
	
	g_debug("Content key release");
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mng->priv->snippet_content));
	gtk_text_buffer_get_start_iter(buffer,&ini);
	gtk_text_buffer_get_end_iter(buffer,&fin);
	content = gtk_text_buffer_get_text(buffer,&ini,&fin,FALSE);
	
	return content;
}

static void
smngui_snippets_tree_cursor_changed_cb(GtkTreeView *tree_view, gpointer user_data)
{
	GtkSnippet *snippet;
	GtkSnippetsManagementUI *mng;
	GtkTextBuffer *buffer;
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	snippet = smngui_get_active_snippet(mng);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mng->priv->snippet_content));
	if (snippet != NULL)
	{
		gtk_text_buffer_set_text(buffer, gtk_snippet_get_text(snippet), -1);
		gtk_entry_set_text(GTK_ENTRY(mng->priv->snippet_tag), gtk_snippet_get_tag(snippet));
		//Enable remove button
		gtk_widget_set_sensitive(mng->priv->remove_button,TRUE);
	}
	else
	{
		gtk_text_buffer_set_text(buffer, "", -1);
		gtk_entry_set_text(GTK_ENTRY(mng->priv->snippet_tag), "");
		//Disable remove button
		gtk_widget_set_sensitive(mng->priv->remove_button,FALSE);
	}
}

static void
smngui_remove_button_activate_cb(GtkWidget *widget, gpointer user_data)
{
	/*
	* Remove snippet from loader
	* Remove snippet from tree_model 
	*/
	GtkSnippetsManagementUI *mng;
	GtkTreePath *path;
	GtkTreePath *path_new;
	GtkTreeViewColumn *column;
	GtkTreeModel *model;
	GtkSnippet *snippet;
	GtkTreeIter iter,parent_iter;
	
	g_debug("remove the snippet");
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	snippet = smngui_get_active_snippet(mng);
	
	if (snippet != NULL)
	{
		gtk_snippets_loader_remove_snippet(mng->priv->loader, snippet);
	
		gtk_tree_view_get_cursor(mng->priv->snippets_tree,&path,&column);
		if(path && column) 
		{
			model = gtk_tree_view_get_model(mng->priv->snippets_tree);
			if(gtk_tree_model_get_iter(model,&iter,path))
			{
				gtk_tree_model_iter_parent(model, &parent_iter, &iter);
				
				//Quitamos el snippet actual del árbol
				if (gtk_tree_store_remove(GTK_TREE_STORE(model),&iter))
				{
					path_new = gtk_tree_model_get_path(model,&iter);
					if (path_new)
					{
						gtk_tree_view_set_cursor(GTK_TREE_VIEW(mng->priv->snippets_tree), path_new, NULL, FALSE);
						gtk_tree_path_free(path_new);
					}
				}
				else
				{
					path_new = gtk_tree_model_get_path(model,&parent_iter);
					if (path_new)
					{
						gtk_tree_view_set_cursor(GTK_TREE_VIEW(mng->priv->snippets_tree), path_new, NULL, FALSE);
						gtk_tree_path_free(path_new);
					}
					else
					{
						gtk_widget_grab_focus(GTK_WIDGET(mng->priv->snippets_tree));
					}
				}
			}
		}

		if(path) gtk_tree_path_free(path);
	}
	
}

static void
smngui_new_button_activate_cb(GtkWidget *widget, gpointer user_data)
{
	GtkSnippetsManagementUI *mng;
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	
	g_debug("New button activate");
	gtk_entry_set_text(GTK_ENTRY(mng->priv->new_dialog_entry),"");
	//Controlamos la respuesta en la señal response	
	gtk_dialog_run (GTK_DIALOG (mng->priv->new_dialog));

}


/**
 * smngui_get_active_language:
 * @mng: The manager
 * @iter: An Iter to set the position of the language item. If NULL then 
 * ignore this parameter. If a snippet is selected then we return the parent, if 
 * a language is selected, we return it.
 * @path: You must pass the pointer reference.if not NULL then we set the path
 * of the actual language. You must free it if return the path
 * @Returns: language or NULL if not found
 *
 */
static const gchar*
smngui_get_active_language(GtkSnippetsManagementUI *mng,GtkTreeIter* parent_iter, GtkTreePath** path)
{
	GtkTreeModel* model;
	GtkTreePath *internal_path;
	GtkTreeViewColumn *column;
	GtkTreeIter iter,internal_parent_iter;
	GValue value = {0,};
	GValue value_language = {0,};
	GtkSnippet* snippet;
	const gchar* language = NULL;
	
	model = gtk_tree_view_get_model(mng->priv->snippets_tree);
	
	gtk_tree_view_get_cursor(mng->priv->snippets_tree,&internal_path,&column);
	if(internal_path && column)
	{
		if(gtk_tree_model_get_iter(model,&iter,internal_path))
		{
			gtk_tree_model_get_value(model, &iter, COL_SNIPPET, &value);
			snippet = GTK_SNIPPET(g_value_get_pointer (&value));
			if (snippet != NULL)
			{
				language = gtk_snippet_get_language(snippet);
				gtk_tree_model_iter_parent(model,&internal_parent_iter,&iter);
			}
			else
			{
				gtk_tree_model_get_value(model, &iter, COL_NAME ,&value_language);
				language = g_value_get_string (&value_language);
				internal_parent_iter = iter;
			}
			if (parent_iter != NULL)
				*parent_iter = internal_parent_iter;
		}
	}
		
	if(internal_path)
	{
		if (path != NULL)
			*path = internal_path;
		else
			gtk_tree_path_free(internal_path);
	}
		
	return language;
}


static void
smngui_add_snippet_to_tree_and_loader(GtkSnippetsManagementUI *mng, const gchar* snippet_name)
{
	const gchar* language;
	GtkTreeIter parent, actual;
	GtkTreeStore *store;
	GtkTreePath *path;
	GtkTreePath *parent_path = NULL;
	GtkSnippet*	snippet; 
	
	store = GTK_TREE_STORE(gtk_tree_view_get_model(mng->priv->snippets_tree));
	language = smngui_get_active_language(mng, &parent,&parent_path);
	
	
	
	g_assert(language != NULL);
	
	snippet = gtk_snippet_new(
					snippet_name,
					language,
					"",
					"New Snippet created. We need view where we can add this text",
					"");
	
	gtk_tree_store_append(store,&actual, &parent);
	
	gtk_tree_store_set(store,
		&actual,
		COL_NAME, gtk_snippet_get_name(snippet) ,
		COL_SNIPPET,snippet,
		-1);
		
	//Focus on new snippet
	path = gtk_tree_model_get_path(GTK_TREE_MODEL(store),&actual);
	if (path)
	{
		//Desplegamos el padre 
		if (!gtk_tree_view_row_expanded(GTK_TREE_VIEW(mng->priv->snippets_tree),parent_path))
			gtk_tree_view_expand_row(GTK_TREE_VIEW(mng->priv->snippets_tree),parent_path,FALSE);
			
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(mng->priv->snippets_tree), path, NULL, FALSE);
		gtk_tree_path_free(path);
	}
	
	gtk_snippets_loader_add_snippet(mng->priv->loader, snippet);
	
	if (parent_path)
		gtk_tree_path_free(parent_path);
	if (path)
		gtk_tree_path_free(path);
	
}


static void
smngui_new_dialog_response_cb(GtkDialog *dialog,
		gint response_id,
		gpointer user_data)
{
	GtkSnippetsManagementUI *mng;
	const gchar* temp;
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	
	g_debug("New Dialog response: %i",response_id);
	
	switch(response_id)
	{
		case GTK_RESPONSE_ACCEPT:
			gtk_widget_hide(GTK_WIDGET(dialog));
			temp = gtk_entry_get_text(GTK_ENTRY(mng->priv->new_dialog_entry));
			//temp = g_strstrip(temp);
			if (strcmp(temp,"")!=0)
			{
				smngui_add_snippet_to_tree_and_loader(mng,temp);
			}
			break;
		default:
			gtk_widget_hide(GTK_WIDGET(dialog));
			//Nada
			break;
	}
}

static gboolean
smngui_tag_key_release_cb(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GtkSnippetsManagementUI *mng;
	GtkSnippet* snippet;
	const gchar* tag;
	
	g_debug("tag key release");
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	
	tag = gtk_entry_get_text(GTK_ENTRY(widget));
	
	snippet = smngui_get_active_snippet(mng);
	gtk_snippet_set_tag(snippet,tag);
	
	return FALSE;
}

static gboolean
smngui_content_key_release_cb(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GtkSnippetsManagementUI *mng;
	GtkSnippet* snippet;
	const gchar* content;
	
	g_debug("Content key release");
	
	mng = GTK_SNIPPETS_MANAGEMENT_UI(user_data);
	
	content = smngui_get_all_content(mng);
	snippet = smngui_get_active_snippet(mng);
	
	gtk_snippet_set_text(snippet, content);
	
	return FALSE;
}

static void
gmngui_load_glade(GtkSnippetsManagementUI *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "smngui_dialog");
	obj->priv->snippets_tree = GTK_TREE_VIEW(glade_xml_get_widget (gxml, "smngui_snippets_tree"));
	g_assert(obj->priv->snippets_tree != NULL);
	obj->priv->snippet_content = glade_xml_get_widget (gxml, "smngui_source");
	obj->priv->snippet_tag = glade_xml_get_widget (gxml, "smngui_tag");
	obj->priv->remove_button = glade_xml_get_widget (gxml, "smngui_remove");
	obj->priv->new_button = glade_xml_get_widget (gxml, "smngui_new");
	obj->priv->new_dialog_entry = glade_xml_get_widget (gxml, "smngui_new_entry");

	//Connecting some signals
	g_signal_connect(GTK_WIDGET(obj->priv->window), "response",
		G_CALLBACK(smngui_dialog_response_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->snippet_tag), "key-release-event",
		G_CALLBACK(smngui_tag_key_release_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->snippet_content), "key-release-event",
		G_CALLBACK(smngui_content_key_release_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->snippets_tree), "cursor-changed",
		G_CALLBACK(smngui_snippets_tree_cursor_changed_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->remove_button), "clicked",
		G_CALLBACK(smngui_remove_button_activate_cb),(gpointer) obj);
		
	g_signal_connect(GTK_WIDGET(obj->priv->new_button), "clicked",
		G_CALLBACK(smngui_new_button_activate_cb),(gpointer) obj);
		
	//New Snippet dialog
	obj->priv->new_dialog = glade_xml_get_widget (gxml, "smngui_new_dialog");
	g_signal_connect(GTK_WIDGET(obj->priv->new_dialog), "response",
		G_CALLBACK(smngui_new_dialog_response_cb),(gpointer) obj);
	
	g_object_unref(gxml);
}

static void
gmngui_build_snippets_tree(GtkSnippetsManagementUI *mngui)
{
	GtkTreeView *tree = mngui->priv->snippets_tree;
	GtkTreeViewColumn* column = gtk_tree_view_column_new();
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",0,NULL);
	
	gtk_tree_view_append_column(tree, column);
	gtk_tree_view_column_set_visible(column,TRUE);
}


static GtkTreeModel*
gmngui_build_snippets_model(GtkSnippetsManagementUI *mngui)
{

	GtkSourceLanguagesManager* lang_manager;
	GtkSourceLanguage *lang;
	gchar* lang_name;
	const GSList *lang_list;
	GList* snippets;
	GtkTreeIter actual, parent;

	GtkTreeStore *store = gtk_tree_store_new(2,
									 G_TYPE_STRING,
									 G_TYPE_POINTER);
	
	lang_manager = gtk_source_languages_manager_new();
	
	lang_list = gtk_source_languages_manager_get_available_languages(lang_manager);
	
	while ((lang_list = g_slist_next(lang_list)) != NULL)
	{
		lang = GTK_SOURCE_LANGUAGE(lang_list->data);
		lang_name = gtk_source_language_get_name(lang);
		//Insertamos el lenguaje
		gtk_tree_store_append(store,&actual, NULL);
		gtk_tree_store_set(store,
				&actual,
				COL_NAME, lang_name,
				COL_SNIPPET, NULL,
				-1);
				
		parent = actual;
		
		//Preguntamos al loader por los snippets para este lenguage
		snippets = gtk_snippets_loader_get_snippets_by_language(mngui->priv->loader, lang_name);
		if (snippets!=NULL)
		{
			do
			{
				gtk_tree_store_append(store,&actual, &parent);
				gtk_tree_store_set(store,
					&actual,
					COL_NAME, gtk_snippet_get_name(GTK_SNIPPET(snippets->data)) ,
					COL_SNIPPET, (gpointer)snippets->data,
					-1);	
			}while((snippets = g_list_next(snippets)) != NULL);
		}
	}	
	
	return GTK_TREE_MODEL(store);

}

GtkSnippetsManagementUI*
gtk_snippets_management_ui_new (GtkSnippetsLoader *loader)
{
	GtkSnippetsManagementUI *obj;

	obj = GTK_SNIPPETS_MANAGEMENT_UI(g_object_new(GTK_TYPE_SNIPPETS_MANAGEMENT_UI, NULL));
	
	/* TODO: Add initialization code here */

	obj->priv->loader = loader;
	
	gmngui_load_glade(obj);
	
	gmngui_build_snippets_tree(obj);

	GtkTreeModel *model = gmngui_build_snippets_model(obj);
	
	gtk_tree_view_set_model(obj->priv->snippets_tree, model);
	
	//gtk_tree_store_clear(GTK_TREE_STORE(model));
	
	g_object_unref(model);
	
	return obj;	
}

void
gtk_snippets_management_ui_show(GtkSnippetsManagementUI* mng_ui)
{
	gtk_widget_show(mng_ui->priv->window);
}
