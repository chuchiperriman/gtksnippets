/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) chuchiperriman 2007 <chuchiperriman@gmail.com>
 * 
 * main.c is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * main.c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with main.c.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <config.h>

#include <gtk/gtk.h>
#include <glade/glade.h>

#include <regex.h>

#include "libgtksnippets/gtk-snippets-loader.h"
#include "libgtksnippets/gtk-snippets-manager.h"
#include "libgtksnippets/gtk-snippets-manager-gsv.h"
#include "libgtksnippets/gtk-snippets-management-ui.h"
#include "libgtksnippets/gtk-text-completion-popup.h"
#include "libgtksnippets/gtc-provider-test.h"
#include "libgtksnippets/gtc-snippets-provider.h"

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif



#include "callbacks.h"

/* For testing propose use the local (not installed) glade file */
/* #define GLADE_FILE PACKAGE_DATA_DIR"/gtk-source-view-snippets/glade/gtk-source-view-snippets.glade" */
#define GLADE_FILE GLADE_DIR"/gtksnippets.glade"
	
static GtkWidget *source;
static GtkSnippetsLoader *loader;

static void
manager_button_clicked_cb(GtkWidget *widget, gpointer user_data)
{
	GtkSnippetsManagementUI *snippets_ui;
	snippets_ui = gtk_snippets_management_ui_new(loader);
	gtk_snippets_management_ui_show(snippets_ui);
	g_object_unref(snippets_ui);
}

GtkWidget*
create_window (void)
{
	GtkWidget *window;
	GladeXML *gxml;
	GtkWidget *scroll;
	GtkWidget *vbox;
	GtkWidget *buttons;
	GtkWidget *button;
	
	gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	/* This is important */
	glade_xml_signal_autoconnect (gxml);
	
	window = glade_xml_get_widget (gxml, "window");
	vbox = gtk_vbox_new(FALSE,5);
	buttons = gtk_hbox_new(TRUE,5);
	button = gtk_button_new_with_label("Manager");
	scroll = gtk_scrolled_window_new(NULL,NULL);
	source = gtk_source_view_new();
	gtk_widget_show(vbox);
	gtk_widget_show(button);
	gtk_widget_show(buttons);
	gtk_widget_show(source);
	gtk_widget_show(scroll);
	
	gtk_box_pack_start(buttons,button,FALSE,FALSE,5);
	gtk_box_pack_start(vbox,buttons,FALSE,FALSE,5);
	gtk_box_pack_end(vbox,scroll,TRUE,TRUE,5);
	
	gtk_container_add(window,vbox);
	gtk_container_add(scroll,source);
	
	g_signal_connect(GTK_WIDGET(button), "clicked",
		G_CALLBACK(manager_button_clicked_cb),NULL);
	
	return window;
}


static void 
prueba_regexp()
{
	regex_t compiled;
	size_t maxmatch = 5;
	gint i=0;
	regmatch_t submatch[maxmatch];
	gchar* chuchi = "123 chuchi 123 chuchi 123ch";
	if (regcomp(&compiled,"chuchi",0)!=0)
	{
		g_debug("No se compiló el regex");
		return;
	}
	
	while (regexec(&compiled,chuchi,maxmatch, submatch,0)==0)
	{
		g_debug("Encontrado");
		/*if (submatch[i].rm_so == -1)
			break;*/
		g_debug("Encontrado en %i-%i",submatch[i].rm_so ,submatch[i].rm_eo);
		chuchi += submatch[0].rm_eo;
	}
	
	
	regfree(&compiled);
}

static
gboolean funcion_timeout(gpointer data)
{
	static guint cont = 0;
	g_debug("si en timeout");
	cont++;
	if (cont>10)
		return FALSE;
	else
		return TRUE;
}

int
main (int argc, char *argv[])
{
 	GtkWidget *window;

	//prueba_regexp();

	//return;
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_set_locale ();
	gtk_init (&argc, &argv);
	
	g_debug( _("Test i18n") );

	g_debug( _("Test") );
	window = create_window ();
	
	loader = gtk_snippets_loader_new();
	
	gtk_snippets_loader_load_default(loader);

	/*GList* temp = gtk_snippets_loader_get_snippets_by_language(loader, "C");
	gtk_snippets_loader_remove_snippet(loader,GTK_SNIPPET(temp->data));
	g_debug("1");
	temp = gtk_snippets_loader_get_snippets_by_language(loader, "C");
	g_assert(temp!=NULL);
	g_assert(g_list_next(temp)!=NULL);
	g_debug("2");
	gtk_snippets_loader_remove_snippet(loader,GTK_SNIPPET(g_list_next(temp)->data));
	g_debug("3");
	
	//Add snippet
	GtkSnippet*	snippet = gtk_snippet_new(
			"Chuchi",
			"C",
			"chtag",
			_("El snippet de chuchi"),
			"Chuchi{asdfasdfasdf}chuchi");
			
	gtk_snippets_loader_add_snippet(loader, snippet);
	*/
	
	//GtkSnippetsManagerGsv *manager = gtk_snippets_manager_gsv_new(loader);
	GtkSnippetsManager *manager = gtk_snippets_manager_new(loader);
	
	//gtk_snippets_manager_gsv_add_support (manager, (gpointer)source, "C");
	//gtk_snippets_manager_add_support (manager, gtk_snippets_editor_gsv_new(source), "C");
	
	//gtk_snippets_manager_gsv_remove_support (manager, (gpointer)source);
	
	GtkSnippetsManagementUI *snippets_ui;
	snippets_ui = gtk_snippets_management_ui_new(loader);
	
	//gtk_snippets_management_ui_show(snippets_ui);
	
	gtk_widget_show (window);
	
	gtk_widget_grab_focus(source);
	
	GtkTextCompletionPopup *popup = gtk_text_completion_popup_new(GTK_TEXT_VIEW(source));
	
	GtcProviderTest* provider = gtc_provider_test_new();
	gtk_text_completion_popup_register_provider(popup, provider);
	g_object_unref(provider);

	GtcSnippetsProvider* prov_snippets = gtc_snippets_provider_new(loader);
	gtk_text_completion_popup_register_provider(popup, prov_snippets);
	g_object_unref(prov_snippets);

	//guint id = g_timeout_add(1000,funcion_timeout,NULL);
	
	//Para quitar el timeout g_source_remove(id);
	
	gtk_main ();
	
	g_object_unref(loader);
	g_object_unref(manager);
	g_object_unref(popup);
	
	g_print("Fin");
	return 0;
}
