
/*
 * chuchisnippets-plugin.c - Type here a short description of your plugin
 *
 * Copyright (C) 2007 - chuchi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "chuchisnippets-plugin.h"

#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include <libgtksnippets-1/gtk-snippets-loader.h>
#include <libgtksnippets-1/gtk-snippets-manager.h>
#include <libgtksnippets-1/gtk-snippets-management-ui.h>

#define WINDOW_DATA_KEY	"ChuchisnippetsPluginWindowData"
#define MENU_PATH "/MenuBar/EditMenu/EditOps_4"

#define CHUCHISNIPPETS_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), TYPE_CHUCHISNIPPETS_PLUGIN, ChuchisnippetsPluginPrivate))

struct _ChuchisnippetsPluginPrivate
{
	gpointer dummy;
	GList *lista_docs;
	GtkSnippetsManager* mng;
};

GEDIT_PLUGIN_REGISTER_TYPE (ChuchisnippetsPlugin, chuchisnippets_plugin)

typedef struct
{
	GtkActionGroup *action_group;
	guint           ui_id;
} WindowData;

typedef struct
{
	GeditWindow     *window;
	ChuchisnippetsPlugin *plugin;
} ActionData;

static void chuchisnippets_edit_snippets_cb (GtkAction *action, ActionData *data);

static const GtkActionEntry action_entries[] =
{
	{ 
		"EditSnippets",
		NULL,
		N_("Edit Snippets..."),
		NULL,
		N_("Edit, add or delete snippets"),
		G_CALLBACK (chuchisnippets_edit_snippets_cb) 
	},
};

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	g_object_unref (data->action_group);
	g_free (data);
}

static void
chuchisnippets_plugin_init (ChuchisnippetsPlugin *plugin)
{
	plugin->priv = CHUCHISNIPPETS_PLUGIN_GET_PRIVATE (plugin);

	gedit_debug_message (DEBUG_PLUGINS,
			     "ChuchisnippetsPlugin initializing");
}

static void
chuchisnippets_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS,
			     "ChuchisnippetsPlugin finalizing");

	G_OBJECT_CLASS (chuchisnippets_plugin_parent_class)->finalize (object);
}


static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	GtkUIManager *manager;
	WindowData *data;
	ActionData *action_data;
	ChuchisnippetsPlugin* plg = CHUCHISNIPPETS_PLUGIN(plugin);
	GtkSnippetsLoader *loader = gtk_snippets_loader_new();
	
	gtk_snippets_loader_load_default(loader);
	
	plg->priv->mng = gtk_snippets_manager_new(loader);
	
	g_object_unref(loader);
	
	gedit_debug (DEBUG_PLUGINS);
	
	//Menu
	data = g_new (WindowData, 1);
	action_data = g_new (ActionData, 1);
	
	action_data->plugin = CHUCHISNIPPETS_PLUGIN(plugin);
	action_data->window = window;
	
	manager = gedit_window_get_ui_manager (window);
	
	data->action_group = gtk_action_group_new ("GeditChuchiSnippetsPluginActions");
	gtk_action_group_set_translation_domain (data->action_group, 
						 GETTEXT_PACKAGE);
	gtk_action_group_add_actions_full (data->action_group, 
				      	   action_entries,
				      	   G_N_ELEMENTS (action_entries),
				      	   action_data,
				      	   (GDestroyNotify) g_free);

	gtk_ui_manager_insert_action_group (manager, data->action_group, -1);

	data->ui_id = gtk_ui_manager_new_merge_id (manager);

	g_object_set_data_full (G_OBJECT (window), 
				WINDOW_DATA_KEY, 
				data,
				(GDestroyNotify) free_window_data);

	gtk_ui_manager_add_ui (manager, 
			       data->ui_id, 
			       MENU_PATH,
			       "EditSnippets", 
			       "EditSnippets",
			       GTK_UI_MANAGER_MENUITEM, 
			       FALSE);

	//update_ui_real (window, data);
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{

	gedit_debug (DEBUG_PLUGINS);

}

static void
impl_update_ui (GeditPlugin *plugin,
		GeditWindow *window)
{
	GeditDocument* doc;
	GeditView* view;
	GtkSourceLanguage* language;
	GtkSourceLanguagesManager* manager;
	gchar* mime;
	gchar* language_name;
	GList *lista;
	ChuchisnippetsPlugin* plg = CHUCHISNIPPETS_PLUGIN(plugin);
	
	gedit_debug (DEBUG_PLUGINS);
	
	lista = plg->priv->lista_docs;
	doc = gedit_window_get_active_document(window);
	view = gedit_window_get_active_view(window);
	
	if (g_list_find(lista, view) == NULL)
	{
		mime = gedit_document_get_mime_type(doc);
		manager = gtk_source_languages_manager_new();
		language = gtk_source_languages_manager_get_language_from_mime_type(manager,mime);
		
		if (language != NULL)
		{
			language_name = gtk_source_language_get_name(language);
			g_debug("Add document with language %s",language_name);
			lista = g_list_append(lista, view);
			gtk_snippets_manager_add_support (plg->priv->mng, (gpointer)view, language_name);
		}
	}
	
	g_debug("update_ui");
}


static void
chuchisnippets_plugin_class_init (ChuchisnippetsPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = chuchisnippets_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;

	g_type_class_add_private (object_class, 
				  sizeof (ChuchisnippetsPluginPrivate));
}

static void chuchisnippets_edit_snippets_cb (GtkAction *action, ActionData *data)
{
	//TODO no crear cada vez que se llama a modificar snippets
	GtkSnippetsLoader* loader = gtk_snippets_manager_get_loader(data->plugin->priv->mng);
	
	g_debug("Edit snippets");
	
	GtkSnippetsManagementUI *snippets_ui;
	snippets_ui = gtk_snippets_management_ui_new(loader);
   gtk_snippets_management_ui_show(snippets_ui);
}

