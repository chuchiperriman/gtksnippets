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

#define COL_LANGUAGE 0
#define COL_NAME 1
#define COL_SNIPPET 2

struct _GtkSnippetsManagementUIPrivate
{
	GtkWidget* window;
	GtkSnippetsLoader *loader;
	GtkTreeView *snippets_tree;
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
	g_debug("Dialog response: %i",response_id);
	switch(response_id)
	{
		case GTK_RESPONSE_CLOSE:
			gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
gmngui_load_glade(GtkSnippetsManagementUI *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "smngui_dialog");
	obj->priv->snippets_tree = GTK_TREE_VIEW(glade_xml_get_widget (gxml, "smngui_snippets_tree"));
	
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
	const GSList *lang_list;
	GtkTreeIter actual, parent;
	gint num =  0;

	GtkTreeStore *store = gtk_tree_store_new(3,
									 G_TYPE_STRING,
									 G_TYPE_STRING,
									 G_TYPE_OBJECT);
	
	lang_manager = gtk_source_languages_manager_new();
	
	lang_list = gtk_source_languages_manager_get_available_languages(lang_manager);
	
	while ((lang_list = g_slist_next(lang_list)) != NULL)
	{
		lang = GTK_SOURCE_LANGUAGE(lang_list->data);
		//Insertamos el lenguaje
		gtk_tree_store_append(store,&actual, NULL);
		gtk_tree_store_set(store,
				&actual,
				COL_LANGUAGE, gtk_source_language_get_name(lang),
				COL_NAME, NULL,
				COL_SNIPPET, NULL,
				-1);
				
		parent = actual;
		
		for (num = 0; num<5; num++)
		{
			gtk_tree_store_append(store,&actual, &parent);
			gtk_tree_store_set(store,
				&actual,
				COL_LANGUAGE, "C",
				COL_NAME, "aaaa",
				COL_SNIPPET, NULL,
				-1);	
		}
	}	
	
	return GTK_TREE_MODEL(store);
	
/*
self.model = gtk.TreeStore(str, str, object)
			self.model.set_sort_column_id(self.SORT_COLUMN, gtk.SORT_ASCENDING)
			manager = gtksourceview.SourceLanguagesManager()
			langs = manager.get_available_languages()
			
			piter = self.model.append(None, (_('Global'), '', None))
			# Add dummy node
			self.model.append(piter, ('', '', None))
			
			nm = None
			
			if current_lang:
				nm = current_lang.get_name()
		
			for lang in langs:
				name = lang.get_name()
				parent = self.model.append(None, (name, name, lang))

				# Add dummy node
				self.model.append(parent, ('', '', None))

				if (nm == name):
					expand = parent
*/
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
