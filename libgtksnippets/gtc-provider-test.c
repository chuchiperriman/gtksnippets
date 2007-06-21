/* Advanced Vala Sample Code */
/*
 * Some code based in: http://usuarios.lycos.es/acisif/gedit/autocomplete/autocomplete.html
*/
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


/** Checks if char is in set " \t\n(){}[],.:;"
 */
static gboolean
char_in(gchar c)
{
	gchar *i=" \t\n(){}[],.:;"; /*Set of chars*/

	do{
		if(c==*i) return TRUE;
	}while( *(i++)!='\0' );

	return FALSE;
}

/** Updates word list of document
 */
static void
update_word_list(GtcProviderTest *self, GtkTextView* view)
{

	GList *wl=NULL; //Word list
	gchar *text, *i, *f;
	int number_list_elements=0;

	i = text = gtk_snippets_gsv_get_text(view);

	// Finds words in text.
	while ( char_in(*i) && *i!='\0' ) i++;
	
	f=i;

	//Keeps 2000 words in word list.
	while( *i!='\0' && number_list_elements < 2000 )
	{
		if(f==i) f++;

		if(char_in(*f))
		{
			gchar c=*f;
			GList *l=NULL, *end=NULL;
			int comparation=0;

			*f='\0';

			if ( strlen(i)>2 )
			{

				if( number_list_elements > 200 )
				{
					int n=0, jump=100;
					end=l=wl=g_list_first(wl);
					while( l!=NULL && jump>0 )
					{
						comparation=strcmp ( (gchar*)l->data, i );

						if ( 0 < comparation )
						{
							end=l;
							for( n=0; n<jump && l!=NULL; n++)
								l=l->next;
						}
						else if ( 0 > comparation )
						{
							l=end;
							break;
						}
						else
							break;
					}
					if ( l==NULL ) l=g_list_first(wl);
				}
				else
					l=g_list_first(wl);


				while( l!=NULL && ( 0 < (comparation=strcmp ( (gchar*)l->data, i ) )) )
				{
					end=l;
					l=l->next;
				}

				if ( (l==NULL || 0 != comparation )  && strlen(i)>2 )
				{ //Insert new word in word list
					if ( l==NULL ) 
						wl=g_list_insert(end,g_strdup(i),-1);
					else if ( comparation < 0 )
						wl=g_list_insert(l,g_strdup(i),0);
					else
						wl=g_list_insert(l,g_strdup(i),1);
						
					number_list_elements++;
				}
			}

			i=f;
			*i=c;

			while ( char_in(*i) && *i!='\0' ) i++;

			f=i;
		}

		if ( *f!='\0' ) f++;
		}


		/*GList *l=g_list_first(wl);
		printf("\n");
		int count=0;
		while( l!=NULL  )
		{
			printf("Palabra: %d %s\n", ++count, (gchar*)l->data);
			l=l->next;
		}
		printf("%d\n",number_list_elements);
		*/

		// Prepares g_completion GLib util. 
		g_completion_clear_items (self->completion);

		wl=g_list_first(wl);

		if(g_list_length(wl)!=0)
		{
			g_completion_add_items (self->completion, wl );
			// Frees previus word list.
			{
				GList *l=g_list_first( self->word_list );

				while (l!=NULL) {
					g_free(l->data);

					l=l->next;
				}

				g_list_free(self->word_list);
			}

			self->word_list=wl;
		}
		
	g_free(text);
	
}


static GList* gtc_provider_test_real_get_data (GtkTextCompletionProvider* base, GtkTextView* completion, const gchar* event_name, gpointer event_data)
{
	GList *list = NULL;
	GtkTextCompletionData *data;
	GtcProviderTest *test;
	gchar *word;
	
	test = GTC_PROVIDER_TEST(base);
	
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_return_val_if_fail (completion == NULL || G_IS_OBJECT (completion), NULL);
	
	if (strcmp(event_name,WORD_COMPLETION_EVENT)==0 || strcmp(event_name,USER_REQUEST_EVENT)==0)
	{
		word = gtk_snippets_gsv_get_last_word_and_iter(completion, NULL, NULL);
		if (word != NULL && strlen(word)>0)
		{
			update_word_list(test,completion);
			GList *l = g_completion_complete(test->completion,word,NULL);
			//only if there are two items (the first item is the actual word)
			if (l != NULL && l->next != NULL)
			{
				while( l!=NULL  )
				{
					data = gtk_text_completion_data_new_with_data(l->data,test->icon_test,NULL);
					list = g_list_append(list,data);
					l=l->next;
				}
				//g_list_free(l);
			}
			g_free(word);
		}
	}
	//g_debug(gtk_snippets_gsv_get_text(completion));

	return list;
}


static void gtc_provider_test_real_data_selected (GtkTextCompletionProvider* base, GtkTextView* text_view, GtkTextCompletionData* data)
{
	//GtcProviderTest * self = GTC_PROVIDER_TEST (base);
	g_return_if_fail (text_view == NULL || G_IS_OBJECT (text_view));
	//g_debug(gtk_text_completion_data_get_name(data));
	gtk_snippets_gsv_replace_actual_word(text_view, gtk_text_completion_data_get_name(data));
	
}


static void gtc_provider_test_real_data_free (GtkTextCompletionProvider* self, GtkTextCompletionData* data)
{
	gtk_text_completion_data_free(data);
}

static void gtc_provider_test_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec)
{
}


static void gtc_provider_test_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec)
{
}

static void gtc_provider_test_finalize(GObject *object)
{
	GtcProviderTest *self;
	
	self = GTC_PROVIDER_TEST(object);
	
	gdk_pixbuf_unref (self->icon_test);
	g_completion_free(self->completion);
	g_list_free(self->word_list);
	
	G_OBJECT_CLASS(gtc_provider_test_parent_class)->finalize(object);
}


static void gtc_provider_test_class_init (GtcProviderTestClass * klass)
{
	gtc_provider_test_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->get_property = gtc_provider_test_get_property;
	G_OBJECT_CLASS (klass)->set_property = gtc_provider_test_set_property;
	G_OBJECT_CLASS (klass)->finalize = gtc_provider_test_finalize;
}


static void gtc_provider_test_gtk_text_completion_provider_interface_init (GtkTextCompletionProviderIface * iface)
{
	gtc_provider_test_gtk_text_completion_provider_parent_iface = g_type_interface_peek_parent (iface);
	iface->get_data = gtc_provider_test_real_get_data;
	iface->data_selected = gtc_provider_test_real_data_selected;
	iface->data_free = gtc_provider_test_real_data_free;
}


static void gtc_provider_test_init (GtcProviderTest * self)
{
	//TODO We must free this objects
	self->icon_test = gdk_pixbuf_new_from_file(ICON_FILE,NULL);
	self->completion = g_completion_new (NULL);
	self->word_list = NULL;
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

