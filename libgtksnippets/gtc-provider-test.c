/* Advanced Vala Sample Code */
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include <glib/gprintf.h>
#include <string.h>
#include "gtc-provider-test.h"
#include "gtk-snippets-gsv-utils.h"

#define ICON_FILE ICON_DIR"/locals.png"

struct _GtcProviderTestPrivate {
	
};

#define GTC_PROVIDER_TEST_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GTC_PROVIDER_TEST, GtcProviderTestPrivate))
enum  {
	GTC_PROVIDER_TEST_DUMMY_PROPERTY,
};
static GList* gtc_provider_test_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data);
static void gtc_provider_test_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* completion, GtkTextCompletionData* data);
static gpointer gtc_provider_test_parent_class = NULL;
static GtkTextCompletionProviderIface* gtc_provider_test_gtk_text_completion_provider_parent_iface = NULL;


static GList* gtc_provider_test_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data)
{
	gint i;
	GList *list = NULL;
	GtkTextCompletionData *data;
	GtcProviderTest *test;
	gchar *word;
	gchar *final_word;
	
	test = GTC_PROVIDER_TEST(base);
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_return_val_if_fail (completion == NULL || G_IS_OBJECT (completion), NULL);
	
	if (strcmp(event_name,USER_REQUEST_EVENT)==0)
	{
		for (i=0;i<500;i++)
		{
			word = gtk_snippets_gsv_get_last_word_and_iter(completion, NULL, NULL);
			if (strlen(word)>0)
			{
				final_word = g_strdup_printf("%s%i",word,i);
				data = gtk_text_completion_data_new_with_data(final_word,test->icon_test,NULL);
				list = g_list_append(list,data);
				g_free(final_word);
			}
			g_free(word);
			
		}
	}
	else if (strcmp(event_name,WORD_COMPLETION_EVENT)==0)
	{
		for (i=0;i<50;i++)
		{
			g_assert(event_data != NULL);
			
			guint keyval = ((GdkEventKey*)event_data)->keyval;
			word = gtk_snippets_gsv_get_last_word_and_iter(completion, NULL, NULL);
			final_word = g_strdup_printf("%s%c%i",word,keyval,i);
			data = gtk_text_completion_data_new_with_data(final_word,test->icon_test,NULL);
			list = g_list_append(list,data);
			g_free(word);
			g_free(final_word);
		}
	}

	return list;
}


static void gtc_provider_test_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* completion, GtkTextCompletionData* data)
{
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_debug("000");
	g_return_if_fail (completion == NULL || G_IS_OBJECT (completion));
	g_debug("000");
	g_debug((gchar*)data);
	
	gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(completion), (const gchar*)data,-1);	
	
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
	self->icon_test = gdk_pixbuf_new_from_file(ICON_FILE,NULL);
	g_debug(ICON_FILE);
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

