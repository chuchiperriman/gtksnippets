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

#include "libgtksnippets/gtk-snippets-loader.h"
#include "libgtksnippets/gtk-snippets-manager.h"



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

GtkWidget*
create_window (void)
{
	GtkWidget *window;
	GladeXML *gxml;
	
	gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	/* This is important */
	glade_xml_signal_autoconnect (gxml);
	
	GtkWidget *vbox = gtk_vbox_new(FALSE,0);
	window = glade_xml_get_widget (gxml, "window");
	
	source = gtk_source_view_new();
	gtk_widget_show(source);
	gtk_container_add(window,source);
	
	return window;
}


int
main (int argc, char *argv[])
{
 	GtkWidget *window;


#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	
	gtk_set_locale ();
	gtk_init (&argc, &argv);

	window = create_window ();
	
	GtkSnippetsLoader *loader = gtk_snippets_loader_new();
	
	gtk_snippets_loader_load_default(loader);
	
	GtkSnippetsManager *manager = gtk_snippets_manager_new(loader);
	
	g_object_unref(loader);
	
	gtk_snippets_manager_add_support (manager, (gpointer)source, "C");
	
	gtk_snippets_manager_remove_support (manager, (gpointer)source);
	
	gtk_widget_show (window);
	
	gtk_widget_grab_focus(source);

	gtk_main ();
	
	
	g_object_unref(manager);
	
	g_print("Fin");
	return 0;
}
