/*
 * gtc-plugin.c - Type here a short description of your plugin
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

#include "gtc-plugin.h"

#include <gtk/gtkuimanager.h>
#include <glade/glade.h>
#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>
#include <gedit/gedit-window.h>
#include <gtk-text-completion-popup.h>
#include <gtk-text-completion-provider.h>
#include <gtc-provider-test.h>
#include <gtc-snippets-provider.h>
#include <gtk-snippets-loader.h>
#include <gtk-snippets-management-ui.h>

#define WINDOW_DATA_KEY	"GtcPluginWindowData"
#define GLADE_FILE GEDIT_GLADEDIR"gtc-configuration.glade"
#define MENU_PATH "/MenuBar/EditMenu/EditOps_4"

#define GTC_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), TYPE_GTC_PLUGIN, GtcPluginPrivate))

struct _GtcPluginPrivate
{
	GtkTextCompletionPopup *popup;
	GtkTextView *view;
	GtkWidget *window;
	GtkSnippetsLoader *loader;
	GtkSnippetsManagementUI *mng_ui;
};


static void snippets_manager_cb (GtkAction   *action, GtcPlugin *self);

static const GtkActionEntry action_entries[] =
{
	{ "SnippetsManager",
	  NULL,
	  N_("Manage the snippets content"),
	  NULL,
	  N_("Manage the snippets content"),
	  G_CALLBACK (snippets_manager_cb) },
};

typedef struct
{
	GtkActionGroup *action_group;
	guint           ui_id;
} WindowData;

GEDIT_PLUGIN_REGISTER_TYPE (GtcPlugin, gtc_plugin)


static void
gtc_plugin_init (GtcPlugin *plugin)
{
	plugin->priv = GTC_PLUGIN_GET_PRIVATE (plugin);
	plugin->priv->mng_ui = NULL;
	gedit_debug_message (DEBUG_PLUGINS,
			     "GtcPlugin initializing");
}

static void
gtc_plugin_finalize (GObject *object)
{
	GtcPlugin *plugin = GTC_PLUGIN(object);
	
	gedit_debug_message (DEBUG_PLUGINS,
			     "GtcPlugin finalizing");

	if (plugin->priv->mng_ui != NULL)
	{
		g_object_unref(plugin->priv->mng_ui);
	}
	G_OBJECT_CLASS (gtc_plugin_parent_class)->finalize (object);
}

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	g_object_unref (data->action_group);
	g_free (data);
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	GtcPlugin *p = GTC_PLUGIN(plugin);
	p->priv->window = NULL;
	p->priv->popup = NULL;
	p->priv->view = NULL;
	p->priv->loader = gtk_snippets_loader_new();
	gtk_snippets_loader_load_default(p->priv->loader);
	gedit_debug (DEBUG_PLUGINS);
	
	//Menu
	GtkUIManager *manager;
	WindowData *data;

	data = g_new (WindowData, 1);
	manager = gedit_window_get_ui_manager (window);

	data->action_group = gtk_action_group_new ("GeditSnippetsManagerActions");
	gtk_action_group_set_translation_domain (data->action_group, 
						 GETTEXT_PACKAGE);
	gtk_action_group_add_actions (data->action_group, 
				      action_entries,
				      G_N_ELEMENTS (action_entries), 
				      p);

	gtk_ui_manager_insert_action_group (manager, data->action_group, -1);

	data->ui_id = gtk_ui_manager_new_merge_id (manager);

	g_object_set_data_full (G_OBJECT (window), 
				WINDOW_DATA_KEY, 
				data,
				(GDestroyNotify) free_window_data);

	gtk_ui_manager_add_ui (manager, 
			       data->ui_id, 
			       MENU_PATH,
			       "SnippetsManager", 
			       "SnippetsManager",
			       GTK_UI_MANAGER_MENUITEM, 
			       TRUE);
	
	/*GeditView *view;
	GtkAction *action;

	view = gedit_window_get_active_view (window);

	action = gtk_action_group_get_action (data->action_group,
					      "SnippetsManager");
	gtk_action_set_sensitive (action,
				  (view != NULL) &&
				  gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));
	*/		       
	g_debug("Fin menu");
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	GtcPlugin *p = GTC_PLUGIN(plugin);
	if(p->priv->popup != NULL)
	{
		g_object_unref(p->priv->popup);
	}
	if (p->priv->window != NULL)
	{
		g_object_unref(p->priv->window);
	}
	g_object_unref(p->priv->loader);
	gedit_debug (DEBUG_PLUGINS);
	
	//Menu
	GtkUIManager *manager;
	WindowData *data;

	gedit_debug (DEBUG_PLUGINS);

	manager = gedit_window_get_ui_manager (window);

	data = (WindowData *) g_object_get_data (G_OBJECT (window), WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);

	gtk_ui_manager_remove_ui (manager, data->ui_id);
	gtk_ui_manager_remove_action_group (manager, data->action_group);

	g_object_set_data (G_OBJECT (window), WINDOW_DATA_KEY, NULL);	

}

static void
impl_update_ui (GeditPlugin *plugin,
		GeditWindow *window)
{
	GtcPlugin *p = GTC_PLUGIN(plugin);
	GtkTextView *view = GTK_TEXT_VIEW(gedit_window_get_active_view(window));
	if (view != NULL && p->priv->view != view)
	{
		if(p->priv->popup != NULL)
		{
			g_debug("antes destruir");
			g_object_unref(p->priv->popup);
			p->priv->popup = NULL;
		}
		g_debug("antes new");
		p->priv->popup = gtk_text_completion_popup_new (view);
		p->priv->view = view;
		GtcProviderTest* provider = gtc_provider_test_new();
		gtk_text_completion_popup_register_provider(p->priv->popup, GTK_TEXT_COMPLETION_PROVIDER(provider));
		g_object_unref(provider);
		
		g_debug("sisi");
		GtcSnippetsProvider* prov_snippets = gtc_snippets_provider_new(p->priv->loader);
		gtk_text_completion_popup_register_provider(p->priv->popup, GTK_TEXT_COMPLETION_PROVIDER(prov_snippets));
		g_object_unref(prov_snippets);
		g_debug("sisi22");
		/*if(p->priv->popup == NULL)
		{
			
			g_debug("antes new");
			p->priv->popup = gtk_text_completion_popup_new (view);
			p->priv->view = view;
			GtcProviderTest* provider = gtc_provider_test_new();
			gtk_text_completion_popup_register_provider(p->priv->popup, GTK_TEXT_COMPLETION_PROVIDER(provider));
			g_object_unref(provider);
		}*/
		
	}
	gedit_debug (DEBUG_PLUGINS);
}

static GtkWidget*
impl_create_configure_dialog(GeditPlugin *plugin)
{
	GtcPlugin *p = GTC_PLUGIN(plugin);
	if (p->priv->window == NULL)
	{
		GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);

		g_assert(gxml != NULL);

		//glade_xml_signal_autoconnect (gxml);
		p->priv->window = glade_xml_get_widget (gxml, "gtcc_main");
		//popup->priv->data_tree_view = GTK_TREE_VIEW(glade_xml_get_widget (gxml, "data_tree_view"));
		
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
	}
	return p->priv->window;
}

static gboolean
impl_is_configurable(GeditPlugin *plugin)
{
	g_debug("es configurable");
	return TRUE;
}

static void
gtc_plugin_class_init (GtcPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = gtc_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;
	plugin_class->create_configure_dialog = impl_create_configure_dialog;
	plugin_class->is_configurable = impl_is_configurable;

	g_type_class_add_private (object_class, 
				  sizeof (GtcPluginPrivate));
}

static void 
snippets_manager_cb (GtkAction   *action,
	   GtcPlugin *self)
{
	if (self->priv->mng_ui == NULL)
	{
		self->priv->mng_ui = gtk_snippets_management_ui_new(self->priv->loader);
	}
	gtk_snippets_management_ui_show(self->priv->mng_ui);
}


