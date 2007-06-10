
#include "snippets.h"
#include <gtk-text-completion-popup.h>
#include <gtc-provider-test.h>
#include <gtc-snippets-provider.h>

static void 
open_manager_menu (GtkMenuItem *menuitem, gpointer data)
{
	gtk_snippets_management_ui_show(snippets_plugin->snippets_ui);
}

static void
insert_menu_items (OpenLDev *openldev){

	MenuHandle *menus = openldev->menus;
	GtkWidget *menu_edit = gtk_menu_item_get_submenu (GTK_MENU_ITEM (gtk_ui_manager_get_widget 
                                               (menus->uimanager, "/MenuBar/Edit")));

	snippets_plugin->menu_manager = gtk_menu_item_new_with_label("Snippets Manager");
	
	g_signal_connect (G_OBJECT (snippets_plugin->menu_manager), "activate",
                    G_CALLBACK (open_manager_menu), (gpointer) openldev);
                    
	//gtk_menu_attach (GTK_MENU (menu_edit), snippets_plugin->menu_manager, 0, 1, 13, 14);
	gtk_menu_shell_insert(GTK_MENU_SHELL(menu_edit),snippets_plugin->menu_manager,10);
	
	gtk_widget_show(snippets_plugin->menu_manager);
	
}

static void
file_new_cb(OpenLDev *openldev,gpointer user_data)
{
	g_debug("file-new");
	FileManager *files = FILE_MANAGER (openldev->files);
	GtkTextView *view;
	
	if (file_manager_get_current_tab (files) != -1)
	{
		view = GTK_TEXT_VIEW(file_manager_get_current_view(files));
	
		GtkTextCompletionPopup *popup = gtk_text_completion_popup_new(GTK_TEXT_VIEW(view));
	
		GtcProviderTest* provider = gtc_provider_test_new();
		gtk_text_completion_popup_register_provider(popup, GTK_TEXT_COMPLETION_PROVIDER(provider));
		g_object_unref(provider);

		GtcSnippetsProvider* prov_snippets = gtc_snippets_provider_new( snippets_plugin->loader );
		gtk_text_completion_popup_register_provider(popup, GTK_TEXT_COMPLETION_PROVIDER(prov_snippets));
		g_object_unref(prov_snippets);	
	}
}

static void
connect_signals(OpenLDev *openldev)
{
	g_signal_connect (G_OBJECT (openldev), "file-new",
                    G_CALLBACK (file_new_cb), (gpointer) openldev);
	g_signal_connect (G_OBJECT (openldev), "file-open",
                    G_CALLBACK (file_new_cb), (gpointer) openldev);
}

extern "C" 
{
  // Initialize the plugin
  G_MODULE_EXPORT PluginState init (gpointer data)
  {
    OpenLDev *openldev = (OpenLDev*) data;
    // Insert your code here
	snippets_plugin = new SnippetsPluginData;

	snippets_plugin->loader = gtk_snippets_loader_new();
	gtk_snippets_loader_load_default(snippets_plugin->loader);
	
	snippets_plugin->snippets_ui = gtk_snippets_management_ui_new(snippets_plugin->loader);
	insert_menu_items(openldev);

	connect_signals(openldev);
		
    return PLUGIN_OK;
  } 

  // Hide the plugin for later use
  G_MODULE_EXPORT PluginState destroy (gpointer data)
  {
    // Hide any widgets you created
    gtk_widget_hide(snippets_plugin->menu_manager);
    return PLUGIN_OK;
  }

  // Show the plugin
  G_MODULE_EXPORT PluginState reload (gpointer data)
  {
    // Show any widgets you created
    return PLUGIN_OK;
  }
}

