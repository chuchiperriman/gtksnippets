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
#include <string.h>
#include <glade/glade.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include "../gsnippets/gsnippets-db.h"
#include "../gsnippets/gsnippets-item.h"

#define COL_LANG_NAME 0
#define COL_LANG_ID 1
#define COL_SNIPPET 2

struct _GtkSnippetsDialogPrivate
{
	/*We need one or more fields*/
	GladeXML *gxml;
	GSnippetsDb *db;
	GtkWidget *tree;
	GtkTreeViewColumn *tree_column;
	GtkWidget *source;
	GtkWidget *new_dialog;
	GtkWidget *new_entry;
};

static GtkWindowClass* parent_class = NULL;
/*Type definition*/
G_DEFINE_TYPE(GtkSnippetsDialog, gtksnippets_dialog, GTK_TYPE_WINDOW);

#define GTKSNIPPETS_DIALOG_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_DIALOG, GtkSnippetsDialogPrivate))

#define GLADE_FILE GLADE_DIR"/gtksnippets.glade"
#define LOGO_FILE GLADE_DIR"/logo.png"

GtkWidget*
gtksnippets_dialog_source_new(gchar *widget_name, gchar *string1, gchar
                       *string2, gint int1, gint int2)
{
	GtkWidget *source = gtk_source_view_new();
	gtk_widget_show(source);
	return source;
}

static gint
_compare_lang_name(GtkSourceLanguage *lang1, GtkSourceLanguage *lang2)
{
	const gchar *lang_name1, *lang_name2;
	lang_name1 = gtk_source_language_get_name(lang1);
	lang_name2 = gtk_source_language_get_name(lang2);
	return g_utf8_collate(lang_name1,lang_name2);
}

static GSnippetsItem*
_get_active_snippet(GtkSnippetsDialog *self,GtkTreeIter *iter)
{
	GtkTreePath *path;
	GtkTreeViewColumn *column;
	GtkTreeModel *model;
	GValue value = {0,};
	GSnippetsItem *snippet = NULL;
	
	gtk_tree_view_get_cursor(GTK_TREE_VIEW(self->priv->tree),&path,&column);
	if(path && column) 
	{
		model = gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree));
		if(gtk_tree_model_get_iter(model,iter,path))
		{
			gtk_tree_model_get_value(model, iter, COL_SNIPPET, &value);
			if (GSNIPPETS_IS_ITEM(g_value_get_pointer (&value)))
				snippet = GSNIPPETS_ITEM(g_value_get_pointer (&value));
		}
	}

	if(path) gtk_tree_path_free(path);
	
	return snippet;
}

static gchar*
_get_source_content(GtkSnippetsDialog *self)
{
	gchar *content;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source));
	GtkTextIter start,end;
	gtk_text_buffer_get_start_iter(buffer,&start);
	gtk_text_buffer_get_end_iter(buffer,&end);
	content = gtk_text_buffer_get_text(buffer,
			&start,
			&end,
			FALSE
			);
	return content;
}

static gboolean
_get_current_lang_iter(GtkSnippetsDialog *self, GtkTreeIter *iter)
{
	GtkTreeModel* model;
	GtkTreePath *internal_path;
	GtkTreeViewColumn *column;
	GValue value = {0,};
	GtkTreeIter temp;
	gboolean res = FALSE;
	
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree));
	
	gtk_tree_view_get_cursor(GTK_TREE_VIEW(self->priv->tree),&internal_path,&column);
	if(internal_path && column)
	{
		if(gtk_tree_model_get_iter(model,&temp,internal_path))
		{
			gtk_tree_model_get_value(model, &temp, COL_SNIPPET, &value);
			
			if (GSNIPPETS_IS_ITEM(g_value_get_pointer (&value)))
				gtk_tree_model_iter_parent(model,iter,&temp);
			else
				gtk_tree_model_get_iter(model,iter,internal_path);
				
			res = TRUE;
		}
	}
		
	if(internal_path)
	{
		gtk_tree_path_free(internal_path);
	}
	
	return res;
}

static const gchar*
_get_current_language(GtkSnippetsDialog *self)
{
	GtkTreeIter iter;
	GtkTreeModel* model;
	GValue value = {0,};
	const gchar* language = NULL;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree));
	if (_get_current_lang_iter(self,&iter))
	{
		gtk_tree_model_get_value(model, &iter, COL_LANG_ID ,&value);
		language = g_value_get_string (&value);
	}
	return language;
	
}

static void
_select_tree_iter(GtkSnippetsDialog *self, GtkTreeIter *iter)
{
	GtkTreeStore *store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree)));
	GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(store),iter);
	gtk_tree_view_set_cursor(
		GTK_TREE_VIEW(self->priv->tree),
		path,
		GTK_TREE_VIEW_COLUMN(self->priv->tree_column),
		FALSE);
	gtk_widget_grab_focus(self->priv->tree);
	gtk_tree_path_free(path);
}

static void
_insert_snippet_to_model(GtkSnippetsDialog *self,GSnippetsItem* snippet)
{
	GtkTreeIter lang_iter, actual;
	if (_get_current_lang_iter(self, &lang_iter))
	{
		const gchar* lang_id = _get_current_language(self);
		
		GtkTreeStore *store = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree)));
		gtk_tree_store_append(store,&actual, &lang_iter);
		gtk_tree_store_set(store,
			&actual,
			COL_LANG_NAME, gsnippets_item_get_name(GSNIPPETS_ITEM(snippet)) ,
			COL_LANG_ID, lang_id,
			COL_SNIPPET, (gpointer)snippet,
			-1);
		GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(store),&lang_iter);
		gtk_tree_view_expand_row(GTK_TREE_VIEW(self->priv->tree),path,TRUE);
		gtk_tree_path_free(path);
		
		_select_tree_iter(self,&actual);
	}
	
}

static void
_build_model(GtkSnippetsDialog *self)
{
	GtkSourceLanguageManager* lm;
	GtkSourceLanguage *lang;
	const gchar* lang_id, *lang_name;
	GtkTreeIter actual, parent;
	const gchar * const *ids;
	GSList *snippet_list, *snippet_list_temp, *lang_list = NULL, *lang_list_temp;
	GSnippetsItem* snippet;
	
	GtkTreeStore *store = gtk_tree_store_new(3,
						 G_TYPE_STRING,
						 G_TYPE_STRING,
						 G_TYPE_POINTER);
	
	lm = gtk_source_language_manager_get_default();
	
	ids = gtk_source_language_manager_get_language_ids(lm);
	while (*ids != NULL)
	{
		lang = gtk_source_language_manager_get_language (lm, *ids);
		lang_list = g_slist_append(lang_list,lang);
		++ids;
	}
	
	lang_list = g_slist_sort(lang_list,(GCompareFunc)_compare_lang_name);
	
	lang_list_temp = lang_list;
	while (lang_list_temp!=NULL)
	{
		lang = GTK_SOURCE_LANGUAGE(lang_list_temp->data);
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
		snippet_list = gsnippets_db_get_by_lang_name(self->priv->db,lang_id);
		
		if (snippet_list!=NULL){
			snippet_list_temp = snippet_list;	
			do{
				snippet = GSNIPPETS_ITEM(snippet_list_temp->data);
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
		
		lang_list_temp = g_slist_next(lang_list_temp);
	}
	
	g_slist_free(lang_list);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(self->priv->tree),GTK_TREE_MODEL(store));
}

static gint
_get_lang_id(GtkSnippetsDialog *self, const gchar* lang_name)
{
	gint lang_id = gsnippets_db_lang_get_id(self->priv->db,lang_name);
	if (lang_id==-1)
	{
		/*Insert the new language into db*/
		lang_id = gsnippets_db_lang_insert(self->priv->db,lang_name);
	}
	return lang_id;
}

/*****************signals*******************/
static void
_close_clicked_cb(GtkButton *button, gpointer user_data)
{
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(user_data);
	gtk_widget_hide(GTK_WIDGET(self));
}

static void
_new_clicked_cb(GtkButton *button, gpointer user_data)
{
	GSnippetsItem *snippet = NULL;
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(user_data);
	gtk_entry_set_text(GTK_ENTRY(self->priv->new_entry),"");
	gtk_widget_grab_focus(self->priv->new_entry);
	gint result = gtk_dialog_run(GTK_DIALOG(self->priv->new_dialog));
	gint lang_id;
	const gchar* lang_name;
	
	switch (result)
	{
	case GTK_RESPONSE_ACCEPT:
		lang_id = _get_lang_id(self, _get_current_language(self));
		lang_name = gtk_entry_get_text(GTK_ENTRY(self->priv->new_entry));
		if (lang_name!=NULL && strcmp(lang_name,"")!=0)
		{
			snippet = gsnippets_item_new_full(-1,
					gtk_entry_get_text(GTK_ENTRY(self->priv->new_entry)),
					"",
					lang_id);
			gsnippets_db_save(self->priv->db,snippet);
			_insert_snippet_to_model(self,snippet);
		}
		break;
	default:
		break;
	}
	gtk_widget_hide(self->priv->new_dialog);
	gtk_widget_grab_focus(self->priv->source);
}

static void
_delete_clicked_cb(GtkButton *button, gpointer user_data)
{
	GtkTreeIter iter, set_iter;
	GtkTreeModel *model;
	GtkTreePath *path;
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(user_data);
	GSnippetsItem *snippet = _get_active_snippet(self,&iter);
	if (snippet != NULL)
	{
		gsnippets_db_delete(self->priv->db,gsnippets_item_get_id(snippet));
		model = gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->tree));
		path = gtk_tree_model_get_path(model,&iter);
		if (gtk_tree_path_prev(path))
		{
			gtk_tree_model_get_iter(model,&set_iter,path);
		}
		else
		{
			gtk_tree_model_iter_parent(model,&set_iter,&iter);
		}
		gtk_tree_path_free(path);
		
		_select_tree_iter(self,&set_iter);
		gtk_tree_store_remove(GTK_TREE_STORE(model),&iter);
	}
}

static void
_save_clicked_cb(GtkButton *button, gpointer user_data)
{
	g_debug("focus out");
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(user_data);
	GtkTreeIter iter;
	GSnippetsItem *snippet = _get_active_snippet(self,&iter);
	gchar *content;
	if(snippet!=NULL)
	{
		g_debug("snippet si ");
		content = _get_source_content(self);
		g_debug("content: %s",content);
		gsnippets_item_set_content(snippet,content);
		g_free(content);
		gsnippets_db_save(self->priv->db, snippet);
	}
}

static void
_tree_cursor_changed_cb(GtkTreeView *tree_view, gpointer user_data)
{
	g_debug("cursor changed event");
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(user_data);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source));
	GtkTreeIter iter;
	GSnippetsItem *snippet = _get_active_snippet(self,&iter);
	const gchar* lang_id = _get_current_language(self);
	if (lang_id!=NULL)
	{
		GtkSourceLanguageManager *slm = gtk_source_language_manager_get_default();
		GtkSourceLanguage *sl = gtk_source_language_manager_get_language(
						slm,
						lang_id);
		gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buffer),sl);
		if(snippet!=NULL)
		{
			gtk_text_buffer_set_text(buffer,gsnippets_item_get_content(snippet),-1);
		}
		else
		{
			gtk_text_buffer_set_text(buffer,"",-1);
		}
	}
}

/*******************************************/

static void
_load_from_glade(GtkSnippetsDialog *self)
{
	self->priv->gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	g_assert(self->priv->gxml!=NULL);
	self->priv->tree = glade_xml_get_widget (self->priv->gxml, "tree");
	self->priv->new_dialog = glade_xml_get_widget (self->priv->gxml, "new_dialog");
	self->priv->new_entry = glade_xml_get_widget (self->priv->gxml, "new_entry");
	GtkWidget *close_button = glade_xml_get_widget (self->priv->gxml, "close_button");
	GtkWidget *new_button = glade_xml_get_widget (self->priv->gxml, "new_button");
	GtkWidget *del_button = glade_xml_get_widget (self->priv->gxml, "delete_button");
	GtkWidget *main_box = glade_xml_get_widget (self->priv->gxml, "main_box");
	self->priv->source = glade_xml_get_widget (self->priv->gxml, "source_view");
	GtkWidget *save_button = glade_xml_get_widget (self->priv->gxml, "save_button");
	g_assert(self->priv->source != NULL);
	g_assert(main_box!=NULL);
	gtk_widget_show_all(main_box);
	gtk_widget_reparent(main_box,GTK_WIDGET(self));
	
	gtk_window_set_default_size(GTK_WINDOW(self),600,400);
	gtk_window_set_title(GTK_WINDOW(self),"Snippets Manager");
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->priv->source));
	gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(buffer),TRUE);
	
	/* Signals */
	g_signal_connect(
		close_button,
		"clicked",
		G_CALLBACK(_close_clicked_cb),
		self);
	g_signal_connect(
		new_button,
		"clicked",
		G_CALLBACK(_new_clicked_cb),
		self);
	g_signal_connect(
		del_button,
		"clicked",
		G_CALLBACK(_delete_clicked_cb),
		self);
	g_signal_connect(
		save_button,
		"clicked",
		G_CALLBACK(_save_clicked_cb),
		self);
	g_signal_connect(
		self->priv->tree,
		"cursor-changed",
		G_CALLBACK(_tree_cursor_changed_cb),
		self);
}

static void
_build_tree(GtkSnippetsDialog *self)
{
	GtkTreeView *tree = GTK_TREE_VIEW(self->priv->tree);
	GtkTreeViewColumn* column = gtk_tree_view_column_new();
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",0,NULL);
	
	gtk_tree_view_append_column(tree, column);
	gtk_tree_view_column_set_visible(column,TRUE);
	self->priv->tree_column = column;
}

static void
gtksnippets_dialog_init (GtkSnippetsDialog *self)
{
	self->priv = GTKSNIPPETS_DIALOG_GET_PRIVATE(self);
	gtk_window_set_position(GTK_WINDOW(self),GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_icon_from_file(GTK_WINDOW(self),LOGO_FILE,NULL);
	self->priv->db = gsnippets_db_new();
	gsnippets_db_connect(self->priv->db);
	_load_from_glade(self);
	_build_tree(self);
	_build_model(self);
}

static void
gtksnippets_dialog_finalize (GObject *object)
{
	GtkSnippetsDialog *self = GTKSNIPPETS_DIALOG(object);
	gsnippets_db_disconnect(self->priv->db);
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




