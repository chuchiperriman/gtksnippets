/* Advanced Vala Sample Code */
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include <glib/gprintf.h>
#include "gtc-provider-test.h"

struct _GtcProviderTestPrivate {
};
#define GTC_PROVIDER_TEST_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GTC_PROVIDER_TEST, GtcProviderTestPrivate))
enum  {
	GTC_PROVIDER_TEST_DUMMY_PROPERTY,
};
static GList* gtc_provider_test_real_get_data (GtkTextCompletionProvider* base, GObject* completion, const gchar* event_name);
static void gtc_provider_test_real_data_selected (GtkTextCompletionProvider* base, GObject* completion, GObject* data);
static gpointer gtc_provider_test_parent_class = NULL;
static GtkTextCompletionProviderIface* gtc_provider_test_gtk_text_completion_provider_parent_iface = NULL;


static GList* gtc_provider_test_real_get_data (GtkTextCompletionProvider* base, GObject* completion, const gchar* event_name)
{
	gint i;
	GList *list = NULL;
	gchar temp[10];
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_return_val_if_fail (completion == NULL || G_IS_OBJECT (completion), NULL);
	
	for (i=0;i<5000;i++)
	{
		g_sprintf(temp,"Hola %i",i);
		list = g_list_append(list,g_strdup(temp));
	}
	return list;
}


static void gtc_provider_test_real_data_selected (GtkTextCompletionProvider* base, GObject* completion, GObject* data)
{
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_return_if_fail (completion == NULL || G_IS_OBJECT (completion));
	g_return_if_fail (data == NULL || G_IS_OBJECT (data));
}


static void gtc_provider_test_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gtc_provider_test_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}


static void gtc_provider_test_class_init (GtcProviderTestClass * klass)
{
	gtc_provider_test_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtc_provider_test_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtc_provider_test_set_property;
}


static void gtc_provider_test_gtk_text_completion_provider_interface_init (GtkTextCompletionProviderIface * iface)
{
	gtc_provider_test_gtk_text_completion_provider_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_data = gtc_provider_test_real_get_data;
	iface->data_selected = gtc_provider_test_real_data_selected;
}


static void gtc_provider_test_init (GtcProviderTest * self)
{
}


GType gtc_provider_test_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GtcProviderTestClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gtc_provider_test_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GtcProviderTest), 0, (GInstanceInitFunc) gtc_provider_test_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GtcProviderTest", &g_define_type_info, 0);
		static const GInterfaceInfo gtk_text_completion_provider_info = { (GInterfaceInitFunc) gtc_provider_test_gtk_text_completion_provider_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GTK_TEXT_COMPLETION_TYPE_PROVIDER, &gtk_text_completion_provider_info);
	}
	return g_define_type_id;
}


GtcProviderTest*
gtc_provider_test_new()
{
	return GTC_PROVIDER_TEST (g_object_new (TYPE_GTC_PROVIDER_TEST, NULL));
}

