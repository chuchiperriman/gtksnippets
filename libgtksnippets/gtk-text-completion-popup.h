/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gtksnippets
 * Copyright (C) chuchi 2007 <chuchiperriman@gmail.com>
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

#ifndef _GTK_TEXT_COMPLETION_POPUP_H_
#define _GTK_TEXT_COMPLETION_POPUP_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include "gtk-text-completion-provider.h"

G_BEGIN_DECLS

#define GTK_TYPE_TEXT_COMPLETION_POPUP             (gtk_text_completion_popup_get_type ())
#define GTK_TEXT_COMPLETION_POPUP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopup))
#define GTK_TEXT_COMPLETION_POPUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopupClass))
#define GTK_IS_TEXT_COMPLETION_POPUP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TEXT_COMPLETION_POPUP))
#define GTK_IS_TEXT_COMPLETION_POPUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_COMPLETION_POPUP))
#define GTK_TEXT_COMPLETION_POPUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopupClass))

const gchar* USER_REQUEST_EVENT = "user-request-event";

typedef struct _GtkTextCompletionPopupPrivate GtkTextCompletionPopupPrivate;

typedef struct _GtkTextCompletionPopupClass GtkTextCompletionPopupClass;
typedef struct _GtkTextCompletionPopup GtkTextCompletionPopup;

struct _GtkTextCompletionPopupClass
{
	GObjectClass parent_class;

	/* Signals */
	void(* populate_completion) (GtkTextCompletionPopup* popup);
};

struct _GtkTextCompletionPopup
{
	GObject parent_instance;
	GtkTextCompletionPopupPrivate *priv;
};

GType gtk_text_completion_popup_get_type (void) G_GNUC_CONST;

GtkTextCompletionPopup* gtk_text_completion_popup_new (GtkTextView *view);

//Añade un nuevo evento al popup
void gtk_text_completion_popup_add_event(GtkTextCompletionPopup *popup, const gchar *event_name);

//Lanzará las señales de populate y mostrara el popup.
void gtk_text_completion_popup_raise_event(GtkTextCompletionPopup *popup, const gchar *event_name);

//Registra un proveedor en el popup.
void gtk_text_completion_popup_register_provider(GtkTextCompletionPopup *popup, GtkTextCompletionProvider *provider);

GtkTextView* gtk_text_completion_popup_get_view(GtkTextCompletionPopup *popup);

G_END_DECLS

#endif /* _GTK_TEXT_COMPLETION_POPUP_H_ */
