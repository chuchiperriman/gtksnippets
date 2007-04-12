/* Advanced Vala Sample Code */

#ifndef __GTK_TEXT_COMPLETION_PROVIDER_H__
#define __GTK_TEXT_COMPLETION_PROVIDER_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "gtk-text-completion-data.h"

G_BEGIN_DECLS


#define GTK_TEXT_COMPLETION_TYPE_PROVIDER (gtk_text_completion_provider_get_type ())
#define GTK_TEXT_COMPLETION_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TEXT_COMPLETION_TYPE_PROVIDER, GtkTextCompletionProvider))
#define GTK_TEXT_COMPLETION_IS_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TEXT_COMPLETION_TYPE_PROVIDER))
#define GTK_TEXT_COMPLETION_PROVIDER_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TEXT_COMPLETION_TYPE_PROVIDER, GtkTextCompletionProviderIface))

typedef struct _GtkTextCompletionProvider GtkTextCompletionProvider;
typedef struct _GtkTextCompletionProviderIface GtkTextCompletionProviderIface;

struct _GtkTextCompletionProviderIface {
	GTypeInterface parent;
	GList* (*get_data) (GtkTextCompletionProvider* self, GtkTextView* completion, const gchar* event_name);
	void (*data_selected) (GtkTextCompletionProvider* self, GtkTextView* completion, GtkTextCompletionData* data);
};


GList* gtk_text_completion_provider_get_data (GtkTextCompletionProvider* self, GtkTextView* completion, const gchar* event_name);
void gtk_text_completion_provider_data_selected (GtkTextCompletionProvider* self, GtkTextView* completion, GtkTextCompletionData* data);
GType gtk_text_completion_provider_get_type ();

G_END_DECLS

#endif
