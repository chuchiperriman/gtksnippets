#ifndef SNIPPETS_H
#define SNIPPETS_H

#include "openldev/openldev-plugin.h"
#include "openldev/openldev.h"
#include <glade/glade.h>
#include <gtk-snippets-management-ui.h>

struct SnippetsPluginData
{
  GtkSnippetsLoader *loader;
  GtkSnippetsManagementUI *snippets_ui;
  GtkWidget *menu_manager;
};

SnippetsPluginData *snippets_plugin;

#endif

