/* Advanced Vala Sample Code */
/*
 * Some code based in: http://usuarios.lycos.es/acisif/gedit/autocomplete/autocomplete.html
*/
#define VALA_FREE_CHECKED(o,f) ((o) == NULL ? NULL : ((o) = (f (o), NULL)))
#define VALA_FREE_UNCHECKED(o,f) ((o) = (f (o), NULL))

#include <string.h>
#include "gtc-snippets-provider.h"
#include "gtk-snippets-gsv-utils.h"
#include <gtksourceview/gtksourceview.h>

#define ICON_FILE ICON_DIR"/snippet.png"

struct _GtcSnippetsProviderPrivate {
	GtkSnippetsLoader *loader;
	GList *active_list;
	GdkPixbuf *icon;
	GtkTextView* temp_view;
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
add_snippet_to_active_list(GtcSnippetsProvider *prov, GtkSnippet *snippet)
{
	gchar *last_word = gtk_snippets_gsv_get_last_word(prov->priv->temp_view);
	const gchar *snippet_name = gtk_snippet_get_name(snippet);
	GtkTextCompletionData *data;
	gboolean add = TRUE; 
	if (last_word != NULL) 
	{
		if (strncmp(last_word,snippet_name,strlen(last_word))!=0)
		{
			//If not starts with, no insert
			add = FALSE;
		}
	}
	
	if (add)
	{
		data = gtk_text_completion_data_new_with_data(
				gtk_snippet_get_name(snippet),
				NULL//Icon
				,snippet);
		prov->priv->active_list = g_list_append(prov->priv->active_list,data);
	}
}

static void
gtcsp_list_for_each_add_snippet(gpointer list_data, gpointer user_data )
{
	GtkSnippet *snippet = GTK_SNIPPET(list_data);
	
	GtcSnippetsProvider* prov = GTC_SNIPPETS_PROVIDER(user_data);
	add_snippet_to_active_list(prov, snippet);		
}

static void
gtcsp_hash_for_each_add_snippet (gpointer key,
		gpointer value,
		gpointer user_data)
{

	GtkSnippet *snippet;
	GList *lista;
	GtcSnippetsProvider* prov = GTC_SNIPPETS_PROVIDER(user_data);
	
	g_assert(user_data!=NULL);
	g_assert(key!=NULL);
	
	if (value != NULL)
	{
		lista = (GList*)value;
		do
		{
			g_assert(lista->data != NULL);
			
			snippet = GTK_SNIPPET(lista->data);
			add_snippet_to_active_list(prov, snippet);
			
		}while((lista = g_list_next(lista))!=NULL);
	}
}

static GList* gtc_snippets_provider_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data)
{	
	GHashTable *snippets;
	GList *snippets_list;
	GtcSnippetsProvider *prov;
	gchar* lang_text = NULL;
	
	prov = GTC_SNIPPETS_PROVIDER(base);
	
	if (strcmp(event_name,USER_REQUEST_EVENT)==0)
	{
		g_list_free(prov->priv->active_list);
		prov->priv->active_list = NULL;
		prov->priv->temp_view = completion;
		
		if (GTK_IS_SOURCE_VIEW(completion))
		{
			//Es una lista y no un hashtable
			GtkSourceBuffer *buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(completion));
			GtkSourceLanguage* s_lang = gtk_source_buffer_get_language(buffer);
			
			if (s_lang != NULL){
				lang_text = gtk_source_language_get_name(s_lang);
			}
		}
		if (lang_text!=NULL)
		{
			snippets_list = gtk_snippets_loader_get_snippets_by_language(prov->priv->loader,lang_text);
			if (snippets_list!=NULL)
			{
				g_list_foreach(snippets_list,gtcsp_list_for_each_add_snippet,prov);
			}
		}
		else
		{
			snippets = gtk_snippets_loader_get_snippets(prov->priv->loader);
			if (snippets!=NULL)
			{
				g_hash_table_foreach (snippets,gtcsp_hash_for_each_add_snippet,prov);
			}
		}
		prov->priv->temp_view = NULL;
		return prov->priv->active_list;
	}
	
	return NULL;
}


static void gtc_snippets_provider_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* text_view, GtkTextCompletionData* data)
{
	GtkSnippet* snippet = GTK_SNIPPET(gtk_text_completion_data_get_user_data(data));
	gtk_snippets_gsv_replace_actual_word(text_view, gtk_snippet_get_text(snippet));
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
	
	g_debug("Finalize snip prov");
	
	if (self->priv->active_list != NULL)
		g_list_free(self->priv->active_list);
	
	g_object_unref(self->priv->loader);
	gdk_pixbuf_unref (self->priv->icon);

	G_OBJECT_CLASS(gtc_snippets_provider_parent_class)->finalize(object);
	g_debug("Fin Finalize snip prov");
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
	self->priv->icon = gdk_pixbuf_new_from_file(ICON_FILE,NULL);
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

