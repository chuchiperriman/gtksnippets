/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-manager.c
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

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h> 
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gsnippets/gsnippets-db.h>
#include <gsnippets/gsnippets-item.h>
  
#include "gtksnippets-manager.h"

#define COL_LANG_NAME 0
#define COL_LANG_ID 1
#define COL_SNIPPET 2

struct _GtkSnippetsManagerPrivate
{
	GtkWidget *tree;
	GtkWidget *name;
	GtkWidget *content;
	
	gint dummy;
	
};

static GObjectClass* parent_class = NULL;

#define GTKSNIPPETS_MANAGER_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_MANAGER, GtkSnippetsManagerPrivate))

/*Type definition*/
G_DEFINE_TYPE(GtkSnippetsManager, gtksnippets_manager, GTK_TYPE_WINDOW);


static void
_close_cb(GtkWidget *button, gpointer user_data)
{
	/*GtkSnippetsManager *self = GTKSNIPPETS_MANAGER(user_data);*/
	/* We must raise the destroy event or g_object_unref(self)*/
	gtk_main_quit();
}

static void
_new_cb(GtkWidget *button, gpointer user_data)
{
	g_debug("New clicked");
}

static void
_delete_cb(GtkWidget *button, gpointer user_data)
{
	g_debug("Delete clicked");
}

static void
_save_cb(GtkWidget *button, gpointer user_data)
{
	g_debug("Save clicked");
}

/*
static void
_add_languages(GtkSnippetsManager *self)
{
	GtkSourceLanguageManager *lm = gtk_source_language_manager_get_default();
	const gchar * const *ids;
	ids = gtk_source_language_manager_get_language_ids(lm);
	while (*ids != NULL)
	{
		GtkSourceLanguage *lang;

		lang = gtk_source_language_manager_get_language (lm, *ids);
		g_debug("Language: %s-%s", 
			gtk_source_language_get_id (lang),
			gtk_source_language_get_name(lang));
		++ids;
	}
}*/

static void
_build_tree(GtkSnippetsManager *self)
{
	GtkTreeView *tree = GTK_TREE_VIEW(self->priv->tree);
	GtkTreeViewColumn* column = gtk_tree_view_column_new();
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",0,NULL);
	
	gtk_tree_view_append_column(tree, column);
	gtk_tree_view_column_set_visible(column,TRUE);
}

static void
_build_model(GtkSnippetsManager *self)
{
	GtkSourceLanguageManager* lm;
	GtkSourceLanguage *lang;
	const gchar* lang_id, *lang_name;
	GtkTreeIter actual, parent;
	const gchar * const *ids;
	GSList *snippet_list, *snippet_list_temp;
	GSnippetsItem* snippet;
	
	GSnippetsDb *db = gsnippets_db_new();
	gsnippets_db_connect(db);
	
	GtkTreeStore *store = gtk_tree_store_new(3,
						 G_TYPE_STRING,
						 G_TYPE_STRING,
						 G_TYPE_POINTER);
	
	lm = gtk_source_language_manager_get_default();
	
	ids = gtk_source_language_manager_get_language_ids(lm);
	while (*ids != NULL)
	{
		lang = gtk_source_language_manager_get_language (lm, *ids);
		lang_id = gtk_source_language_get_id (lang);
		lang_name = gtk_source_language_get_name(lang);
		++ids;
		/* Inserting language*/
		gtk_tree_store_append(store,&actual, NULL);
		gtk_tree_store_set(store,
				&actual,
				COL_LANG_NAME, lang_name,
				COL_LANG_ID, lang_id,
				COL_SNIPPET, NULL,
				-1);
		parent = actual;
		
		/* Snippets for this language */
		snippet_list = gsnippets_db_get_by_lang_name(db,lang_id);
		
		if (snippet_list!=NULL){
			snippet_list_temp = snippet_list;	
			do{
				snippet = (GSnippetsItem*)snippet_list_temp->data;
				gtk_tree_store_append(store,&actual, &parent);
				gtk_tree_store_set(store,
						&actual,
						COL_LANG_NAME, gsnippets_item_get_name(GSNIPPETS_ITEM(snippet)) ,
						COL_LANG_ID, lang_id,
						COL_SNIPPET, (gpointer)snippet,
						-1);
				/*g_object_unref(snippet);*/
			}while((snippet_list_temp = g_slist_next(snippet_list_temp))!= NULL);
		
			g_slist_free(snippet_list);
		}
	}
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(self->priv->tree),GTK_TREE_MODEL(store));
	gsnippets_db_disconnect(db);
}

static void
gtksnippets_manager_init (GtkSnippetsManager *self)
{
	self->priv = GTKSNIPPETS_MANAGER_GET_PRIVATE(self);
	GtkWidget *vbox, *hpaned, *hbox, 
		*close_button, *vbox_list,
		*label, *vbox_data, *scroll, *tree,
		*hbox_temp, *name, *source,
		*new_button, *delete_button, *save_button;
	
	hpaned = gtk_hpaned_new();
	//gtk_paned_add2(GTK_PANED(hpaned),NULL);
	vbox = gtk_vbox_new(FALSE,0);
	
	//Snippet list pane 
	vbox_list = gtk_vbox_new(FALSE,0);
	label = gtk_label_new("Snippets");
	gtk_box_pack_start(GTK_BOX(vbox_list),label,FALSE,FALSE,0);
	scroll = gtk_scrolled_window_new(NULL,NULL);
	tree = gtk_tree_view_new();
	gtk_container_add(GTK_CONTAINER(scroll),tree);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree),FALSE);
	gtk_box_pack_start(GTK_BOX(vbox_list),scroll,TRUE,TRUE,0);
	hbox_temp = gtk_hbox_new(FALSE,0);
	new_button = gtk_button_new_from_stock(GTK_STOCK_NEW);
	delete_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
	gtk_box_pack_start(GTK_BOX(hbox_temp),new_button,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(hbox_temp),delete_button,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox_list),hbox_temp,FALSE,FALSE,0);
	
	
	gtk_paned_add1(GTK_PANED(hpaned),vbox_list);
	
	//Snippet data pane
	vbox_data = gtk_vbox_new(FALSE,0);
	label = gtk_label_new("Snippet information");
	gtk_box_pack_start(GTK_BOX(vbox_data),label,FALSE,FALSE,0);
	hbox_temp = gtk_hbox_new(FALSE,0);
	label = gtk_label_new("Name: ");
	gtk_box_pack_start(GTK_BOX(hbox_temp),label,FALSE,FALSE,0);
	name = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox_temp),name,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(vbox_data),hbox_temp,FALSE,FALSE,0);
	label = gtk_label_new("Content: ");
	gtk_box_pack_start(GTK_BOX(vbox_data),label,FALSE,FALSE,0);
	scroll = gtk_scrolled_window_new(NULL,NULL);
	source = gtk_source_view_new();
	gtk_container_add(GTK_CONTAINER(scroll),source);
	gtk_box_pack_start(GTK_BOX(vbox_data),scroll,TRUE,TRUE,0);
	hbox_temp = gtk_hbox_new(FALSE,0);
	save_button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_box_pack_end(GTK_BOX(hbox_temp),save_button,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox_data),hbox_temp,FALSE,FALSE,0);
	
	gtk_paned_add2(GTK_PANED(hpaned),vbox_data);
	
	
	hbox = gtk_hbox_new(FALSE,0);
	close_button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_box_pack_end(GTK_BOX(hbox),close_button,FALSE,FALSE,5);
	
	gtk_box_pack_start(GTK_BOX(vbox),hpaned,TRUE,TRUE,5);
	gtk_box_pack_end(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
	gtk_widget_show_all(vbox);
	
	gtk_container_add(GTK_CONTAINER(self),vbox);
	
	gtk_window_set_default_size(GTK_WINDOW(self),500,400);
	
	/* Signals */
	g_signal_connect(
		close_button,
		"clicked",
		G_CALLBACK(_close_cb),
		self);
	g_signal_connect(
		new_button,
		"clicked",
		G_CALLBACK(_new_cb),
		self);
	g_signal_connect(
		delete_button,
		"clicked",
		G_CALLBACK(_delete_cb),
		self);
	g_signal_connect(
		save_button,
		"clicked",
		G_CALLBACK(_save_cb),
		self);
	self->priv->tree = tree;
	self->priv->name = name;
	self->priv->content = source;
	
	_build_tree(self);
	_build_model(self);
	/* _add_languages(self); */
	
}

static void
gtksnippets_manager_finalize (GObject *object)
{
	/*GtkSnippetsManager *self = GTKSNIPPETS_MANAGER(object);*/
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtksnippets_manager_class_init (GtkSnippetsManagerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtksnippets_manager_finalize;

	g_type_class_add_private (klass, sizeof(GtkSnippetsManagerPrivate));
}


GtkSnippetsManager*
gtksnippets_manager_new(void)
{
	GtkSnippetsManager *self = GTKSNIPPETS_MANAGER(g_object_new (GTKSNIPPETS_TYPE_MANAGER, NULL));
	/*Window type (TOPLEVEL,POPUP...)*/
	GTK_WINDOW(self)->type = GTK_WINDOW_TOPLEVEL;
	return self;
}


