/* Advanced Vala Sample Code */
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include "gtk-text-completion-provider.h"



GList* gtk_text_completion_provider_get_data (GtkTextCompletionProvider* self, GtkTextView* completion, GString* event_name)
{
	return GTK_TEXT_COMPLETION_PROVIDER_GET_INTERFACE (self)->get_data (self, completion, event_name);
}


void gtk_text_completion_provider_data_selected (GtkTextCompletionProvider* self, GtkTextView* completion, gpointer data)
{
	GTK_TEXT_COMPLETION_PROVIDER_GET_INTERFACE (self)->data_selected (self, completion, data);
}


static void gtk_text_completion_provider_base_init (GtkTextCompletionProviderIface * iface)
{
	static gboolean initialized = FALSE;
	if (!initialized) {
		initialized = TRUE;
	}
}


GType gtk_text_completion_provider_get_type ()
{
	static GType gtk_text_completion_provider_type_id = 0;
	if (G_UNLIKELY (gtk_text_completion_provider_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GtkTextCompletionProviderIface), (GBaseInitFunc) gtk_text_completion_provider_base_init, (GBaseFinalizeFunc) NULL, (GClassInitFunc) NULL, (GClassFinalizeFunc) NULL, NULL, 0, 0, (GInstanceInitFunc) NULL };
		gtk_text_completion_provider_type_id = g_type_register_static (G_TYPE_INTERFACE, "GtkTextCompletionProvider", &g_define_type_info, 0);
	}
	return gtk_text_completion_provider_type_id;
}




