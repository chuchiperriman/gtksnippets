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

/**
* GtkSnippetsPopupDialog:
* @parent: GObject parent
* @priv: Private Data
*
* This object get the snippets and creates a TreeStore with all of them.
* Then the popup show the snippets filtering by language and the word.
* The manager creates only one popup for all the editors.
**/
typedef struct _GtkSnippetsPopupDialog GtkSnippetsPopupDialog;
typedef struct _GtkSnippetsPopupDialogClass GtkSnippetsPopupDialogClass;
typedef struct _GtkSnippetsPopupDialogPrivate GtkSnippetsPopupDialogPrivate;

/**
* FilterData:
* @language: Language name to filter by.
* @tag: Tag to filter by.
*
* This object store the data to pass to the popup dialog
* and filter by te data stored in it.
**/
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

/**
* gtk_snippets_popup_dialog_new:
* @Returns: A new GtkSnippetsPopupDialog
*
* Creates a new GtkSnippetsPopupDialog
**/
GtkSnippetsPopupDialog* 
gtk_snippets_popup_dialog_new (void);

/**
* gtk_snippets_popup_dialog_get_window:
* @popup_dialog: The popup
* @Returns: The window widget of the popup to move it at the right position
*
**/
GtkWidget* 
gtk_snippets_popup_dialog_get_window(GtkSnippetsPopupDialog* popup_dialog);

/**
* gtk_snippets_popup_dialog_set_snippets:
* @popup_dialog: The popup
* @snippets: The snippets to set
*
* Assing all the snippets to the internal TreeStore. Cleans all the 
* previous snippets in that list. We only saves the reference to the snippets
* we not copy the snippets.We need a HashTable with the format:  (nombre,snippet).
*/
void
gtk_snippets_popup_dialog_set_snippets(GtkSnippetsPopupDialog* popup_dialog, GHashTable* snippets);

/**
* gtk_snippets_popup_dialog_set_pos:
* @popup_dialog: The popup
* @x: x position on the screen
* @y: y position on the screen
*
* Sets the popup dialog position on the screen
**/
void 
gtk_snippets_popup_dialog_set_pos(GtkSnippetsPopupDialog* popup_dialog, gint x, gint y);

/**
* gtk_snippets_popup_dialog_show:
* @popup_dialog: The popup
* @word: Word to filter by.
*
* Shows the popup dialog filtered by word.
**/
void
gtk_snippets_popup_dialog_show(GtkSnippetsPopupDialog* popup_dialog, const gchar *word);

/**
* gtk_snippets_popup_dialog_filter:
* @popup_dialog: The popup
* @filter_data: Filter to apply.
*
* Shows the popup dialog filtered by the filter_data.
**/
void
gtk_snippets_popup_dialog_filter(GtkSnippetsPopupDialog* popup_dialog, const FilterData *filter_data);

/**
* gtk_snippets_popup_dialog_filter_by_language:
* @popup_dialog: The popup
* @language: Language name to filter by. 
*
* Shows the popup dialog filtered by language.
**/
/*
void
gtk_snippets_popup_dialog_filter_by_language(GtkSnippetsPopupDialog* popup_dialog,gchar* language);
*/

G_END_DECLS

#endif /* _GTK_SNIPPETS_POPUP_DIALOG_H_ */
