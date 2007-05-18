/* Advanced Vala Sample Code */
/*
 * Some code based in: http://usuarios.lycos.es/acisif/gedit/autocomplete/autocomplete.html
*/
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include <glib/gprintf.h>
#include <string.h>
#include "gtc-##(PLUGIN_ID.lower)-provider.h"

#define ICON_FILE ICON_DIR"/locals.png"

struct _Gtc##(PLUGIN_ID.camel)ProviderPrivate {
	
};

#define GTC_##(PLUGIN_ID.upper)_PROVIDER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER, Gtc##(PLUGIN_ID.camel)ProviderPrivate))
enum  {
	GTC_##(PLUGIN_ID.upper)_PROVIDER_DUMMY_PROPERTY,
};
static GList* gtc_##(PLUGIN_ID.lower)_provider_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data);
static void gtc_##(PLUGIN_ID.lower)_provider_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* completion, GtkTextCompletionData* data);
static gpointer gtc_##(PLUGIN_ID.lower)_provider_parent_class = NULL;
static GtkTextCompletionProviderIface* gtc_##(PLUGIN_ID.lower)_provider_gtk_text_completion_provider_parent_iface = NULL;



static GList* gtc_##(PLUGIN_ID.lower)_provider_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data)
{
	
}


static void gtc_##(PLUGIN_ID.lower)_provider_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* text_view, GtkTextCompletionData* data)
{
	
}


static void gtc_##(PLUGIN_ID.lower)_provider_real_data_free (GtkTextCompletionProvider* self, GtkTextCompletionData* data)
{
}

static void gtc_##(PLUGIN_ID.lower)_provider_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gtc_##(PLUGIN_ID.lower)_provider_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gtc_##(PLUGIN_ID.lower)_provider_finalize(GObject *object)
{
	Gtc##(PLUGIN_ID.camel)Provider *self;
	
	self = GTC_##(PLUGIN_ID.upper)_PROVIDER(object);
	
	G_OBJECT_CLASS(gtc_##(PLUGIN_ID.lower)_provider_parent_class)->finalize(object);
}


static void gtc_##(PLUGIN_ID.lower)_provider_class_init (Gtc##(PLUGIN_ID.camel)ProviderClass * klass)
{
	gtc_##(PLUGIN_ID.lower)_provider_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtc_##(PLUGIN_ID.lower)_provider_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtc_##(PLUGIN_ID.lower)_provider_set_property;
	G_OBJECT_CLASS (klass)->finalize = gtc_##(PLUGIN_ID.lower)_provider_finalize;
}


static void gtc_##(PLUGIN_ID.lower)_provider_gtk_text_completion_provider_interface_init (GtkTextCompletionProviderIface * iface)
{
	gtc_##(PLUGIN_ID.lower)_provider_gtk_text_completion_provider_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_data = gtc_##(PLUGIN_ID.lower)_provider_real_get_data;
	iface->data_selected = gtc_##(PLUGIN_ID.lower)_provider_real_data_selected;
	iface->data_free = gtc_##(PLUGIN_ID.lower)_provider_real_data_free;
}


static void gtc_##(PLUGIN_ID.lower)_provider_init (Gtc##(PLUGIN_ID.camel)Provider * self)
{
	self->priv = g_new0(Gtc##(PLUGIN_ID.camel)ProviderPrivate, 1);
}

GType gtc_##(PLUGIN_ID.lower)_provider_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (Gtc##(PLUGIN_ID.camel)ProviderClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gtc_##(PLUGIN_ID.lower)_provider_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (Gtc##(PLUGIN_ID.camel)Provider), 0, (GInstanceInitFunc) gtc_##(PLUGIN_ID.lower)_provider_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "Gtc##(PLUGIN_ID.camel)Provider", &g_define_type_info, 0);
		static const GInterfaceInfo gtk_text_completion_provider_info = { (GInterfaceInitFunc) gtc_##(PLUGIN_ID.lower)_provider_gtk_text_completion_provider_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GTK_TEXT_COMPLETION_TYPE_PROVIDER, &gtk_text_completion_provider_info);
	}
	return g_define_type_id;
}


Gtc##(PLUGIN_ID.camel)Provider*
gtc_##(PLUGIN_ID.lower)_provider_new()
{
	return GTC_##(PLUGIN_ID.upper)_PROVIDER (g_object_new (TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER, NULL));
}

