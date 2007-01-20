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

#ifndef _GTK_SNIPPETS_POPUP_DIALOG_H_
#define _GTK_SNIPPETS_POPUP_DIALOG_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_POPUP_DIALOG             (gtk_snippets_popup_dialog_get_type ())
#define GTK_SNIPPETS_POPUP_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialog))
#define GTK_SNIPPETS_POPUP_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialogClass))
#define GTK_IS_SNIPPETS_POPUP_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_POPUP_DIALOG))
#define GTK_IS_SNIPPETS_POPUP_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_POPUP_DIALOG))
#define GTK_SNIPPETS_POPUP_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialogClass))

typedef struct _GtkSnippetsPopupDialogClass GtkSnippetsPopupDialogClass;
typedef struct _GtkSnippetsPopupDialog GtkSnippetsPopupDialog;
typedef struct _GtkSnippetsPopupDialogPrivate GtkSnippetsPopupDialogPrivate;
typedef struct _FilterData FilterData;

struct _GtkSnippetsPopupDialogClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsPopupDialog
{
	GObject parent_instance;
	GtkSnippetsPopupDialogPrivate *priv;
};

struct _FilterData{
	gchar* language;
	gchar* tag;
};

GType gtk_snippets_popup_dialog_get_type (void) G_GNUC_CONST;

GtkSnippetsPopupDialog* gtk_snippets_popup_dialog_new (void);

GtkWidget* gtk_snippets_popup_dialog_get_window(GtkSnippetsPopupDialog* popup_dialog);

/**
* Asigna los snippets a la lista. Limpia todo lo que había antes
* Espera un hash con el formato <nombre,snippet>
*/
void
gtk_snippets_popup_dialog_set_snippets(GtkSnippetsPopupDialog* popup_dialog, GHashTable* snippets);

void 
gtk_snippets_popup_dialog_set_pos(GtkSnippetsPopupDialog* popup_dialog, gint x, gint y);

void
gtk_snippets_popup_dialog_show(GtkSnippetsPopupDialog* popup_dialog, const gchar *word);

void
gtk_snippets_popup_dialog_filter(GtkSnippetsPopupDialog* popup_dialog, const FilterData *filter_data);

void
gtk_snippets_popup_dialog_filter_by_language(GtkSnippetsPopupDialog* popup_dialog,gchar* language);

G_END_DECLS

#endif /* _GTK_SNIPPETS_POPUP_DIALOG_H_ */
