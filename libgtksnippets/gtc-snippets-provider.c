/* Advanced Vala Sample Code */
/*
 * Some code based in: http://usuarios.lycos.es/acisif/gedit/autocomplete/autocomplete.html
*/
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include <string.h>
#include "gtc-snippets-provider.h"
#include "gtk-snippets-gsv-utils.h"

struct _GtcSnippetsProviderPrivate {
	GtkSnippetsLoader *loader;
	GList *active_list;
};

#define GTC_SNIPPETS_PROVIDER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_GTC_SNIPPETS_PROVIDER, GtcSnippetsProviderPrivate))
enum  {
	GTC_SNIPPETS_PROVIDER_DUMMY_PROPERTY,
};
static GList* gtc_snippets_provider_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data);
static void gtc_snippets_provider_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* completion, GtkTextCompletionData* data);
static gpointer gtc_snippets_provider_parent_class = NULL;
static GtkTextCompletionProviderIface* gtc_snippets_provider_gtk_text_completion_provider_parent_iface = NULL;

static void
gtcsp_hash_for_each_add_snippet (gpointer key,
		gpointer value,
		gpointer user_data)
{

	GtkSnippet *snippet;
	GList *lista;
	GtcSnippetsProvider* prov = GTC_SNIPPETS_PROVIDER(user_data);
	GtkTextCompletionData *data;
	
	g_assert(user_data!=NULL);
	g_assert(key!=NULL);
	
	if (value != NULL)
	{
		lista = (GList*)value;
		do
		{
			g_assert(lista->data != NULL);
			
			snippet = GTK_SNIPPET(lista->data);
			
			data = gtk_text_completion_data_new_with_data(
				gtk_snippet_get_name(snippet),
				NULL//Icon
				,NULL);
			
			prov->priv->active_list = g_list_append(prov->priv->active_list,data);

			//Insertamos los datos
			/*gtk_list_store_append (GTK_LIST_STORE(user_data),&iter);
			
			gtk_list_store_set (GTK_LIST_STORE(user_data), 
								&iter,
								COL_LANGUAGE, gtk_snippet_get_language(snippet),
								COL_NAME, gtk_snippet_get_name(snippet),
								COL_SNIPPET, snippet,
								-1);*/
		}while((lista = g_list_next(lista))!=NULL);
	}

}

static GList* gtc_snippets_provider_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data)
{
	GHashTable *snippets;
	GtcSnippetsProvider *prov;
	
	prov = GTC_SNIPPETS_PROVIDER(base);
	
	if (strcmp(event_name,USER_REQUEST_EVENT)==0)
	{
		snippets = gtk_snippets_loader_get_snippets(prov->priv->loader);
		if (snippets!=NULL)
		{
			g_list_free(prov->priv->active_list);
			prov->priv->active_list = NULL;
			g_hash_table_foreach (snippets,gtcsp_hash_for_each_add_snippet,prov);
			return prov->priv->active_list;
		}
		
		
		
		/*for (i=0;i<500;i++)
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
			
		}*/
	}
	
	return NULL;
	//gtk_snippets_loader_get_snippets(manager->priv->loader);
}


static void gtc_snippets_provider_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* text_view, GtkTextCompletionData* data)
{
	gtk_snippets_gsv_replace_actual_word(text_view, gtk_text_completion_data_get_name(data));
}


static void gtc_snippets_provider_real_data_free (GtkTextCompletionProvider* self, GtkTextCompletionData* data)
{

}

static void gtc_snippets_provider_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{

}


static void gtc_snippets_provider_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{

}

static void gtc_snippets_provider_finalize(GObject *object)
{
	GtcSnippetsProvider *self;
	
	self = GTC_SNIPPETS_PROVIDER(object);
	
	g_object_unref(self->priv->active_list);
	g_object_unref(self->priv->loader);

	G_OBJECT_CLASS(gtc_snippets_provider_parent_class)->finalize(object);
}


static void gtc_snippets_provider_class_init (GtcSnippetsProviderClass * klass)
{
	gtc_snippets_provider_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtc_snippets_provider_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtc_snippets_provider_set_property;
	G_OBJECT_CLASS (klass)->finalize = gtc_snippets_provider_finalize;
}


static void gtc_snippets_provider_gtk_text_completion_provider_interface_init (GtkTextCompletionProviderIface * iface)
{
	gtc_snippets_provider_gtk_text_completion_provider_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_data = gtc_snippets_provider_real_get_data;
	iface->data_selected = gtc_snippets_provider_real_data_selected;
	iface->data_free = gtc_snippets_provider_real_data_free;
}


static void gtc_snippets_provider_init (GtcSnippetsProvider * self)
{
	self->priv = g_new0(GtcSnippetsProviderPrivate, 1);
	self->priv->loader=NULL;
	self->priv->active_list = NULL;
}

GType gtc_snippets_provider_get_type ()
{
	static GType g_define_type_id = 0;
	if (G_UNLIKELY (g_define_type_id == 0)) {
		static const GTypeInfo g_define_type_info = { sizeof (GtcSnippetsProviderClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gtc_snippets_provider_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GtcSnippetsProvider), 0, (GInstanceInitFunc) gtc_snippets_provider_init };
		g_define_type_id = g_type_register_static (G_TYPE_OBJECT, "GtcSnippetsProvider", &g_define_type_info, 0);
		static const GInterfaceInfo gtk_text_completion_provider_info = { (GInterfaceInitFunc) gtc_snippets_provider_gtk_text_completion_provider_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		g_type_add_interface_static (g_define_type_id, GTK_TEXT_COMPLETION_TYPE_PROVIDER, &gtk_text_completion_provider_info);
	}
	return g_define_type_id;
}


GtcSnippetsProvider*
gtc_snippets_provider_new(GtkSnippetsLoader *loader)
{
	GtcSnippetsProvider *prov = GTC_SNIPPETS_PROVIDER (g_object_new (TYPE_GTC_SNIPPETS_PROVIDER, NULL)); 
	prov->priv->loader = g_object_ref(loader);
	return prov;
}

