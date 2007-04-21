/* Generated by GOB (v2.0.14)   (do not edit directly) */

/* End world hunger, donate to the World Food Programme, http://www.wfp.org */

#define GOB_VERSION_MAJOR 2
#define GOB_VERSION_MINOR 0
#define GOB_VERSION_PATCHLEVEL 14

#define selfp (self->_priv)

#include <string.h> /* memset() */

#ifdef G_LIKELY
#define ___GOB_LIKELY(expr) G_LIKELY(expr)
#define ___GOB_UNLIKELY(expr) G_UNLIKELY(expr)
#else /* ! G_LIKELY */
#define ___GOB_LIKELY(expr) (expr)
#define ___GOB_UNLIKELY(expr) (expr)
#endif /* G_LIKELY */


#include <gtk/gtk.h>
#include "gtk-text-completion-data.h"

struct _GtkTextCompletionDataPrivate {
	const gchar * name;
	GdkPixbuf * icon;
	gpointer user_data;
};
/* here are local prototypes */
static void gtk_text_completion_data_init (GtkTextCompletionData * o) G_GNUC_UNUSED;
static void gtk_text_completion_data_class_init (GtkTextCompletionDataClass * c) G_GNUC_UNUSED;

/* pointer to the class of our parent */
static GObjectClass *parent_class = NULL;

/* Short form macros */
#define self_get_name gtk_text_completion_data_get_name
#define self_get_icon gtk_text_completion_data_get_icon
#define self_get_user_data gtk_text_completion_data_get_user_data
#define self_new_with_data gtk_text_completion_data_new_with_data
#define self_free gtk_text_completion_data_free
GType
gtk_text_completion_data_get_type (void)
{
	static GType type = 0;

	if ___GOB_UNLIKELY(type == 0) {
		static const GTypeInfo info = {
			sizeof (GtkTextCompletionDataClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_text_completion_data_class_init,
			(GClassFinalizeFunc) NULL,
			NULL /* class_data */,
			sizeof (GtkTextCompletionData),
			0 /* n_preallocs */,
			(GInstanceInitFunc) gtk_text_completion_data_init,
			NULL
		};

		type = g_type_register_static (G_TYPE_OBJECT, "GtkTextCompletionData", &info, (GTypeFlags)0);
	}

	return type;
}

/* a macro for creating a new object of our type */
#define GET_NEW ((GtkTextCompletionData *)g_object_new(gtk_text_completion_data_get_type(), NULL))

/* a function for creating a new object of our type */
#include <stdarg.h>
static GtkTextCompletionData * GET_NEW_VARG (const char *first, ...) G_GNUC_UNUSED;
static GtkTextCompletionData *
GET_NEW_VARG (const char *first, ...)
{
	GtkTextCompletionData *ret;
	va_list ap;
	va_start (ap, first);
	ret = (GtkTextCompletionData *)g_object_new_valist (gtk_text_completion_data_get_type (), first, ap);
	va_end (ap);
	return ret;
}


static void
___finalize(GObject *obj_self)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::finalize"
	GtkTextCompletionData *self G_GNUC_UNUSED = GTK_TEXT_COMPLETION_DATA (obj_self);
	gpointer priv G_GNUC_UNUSED = self->_priv;
	if(G_OBJECT_CLASS(parent_class)->finalize) \
		(* G_OBJECT_CLASS(parent_class)->finalize)(obj_self);
#define name (self->_priv->name)
#define VAR name
	{
	
			g_free(name);
		}
	memset(&name, 0, sizeof(name));
#undef VAR
#undef name
}
#undef __GOB_FUNCTION__

static void 
gtk_text_completion_data_init (GtkTextCompletionData * o G_GNUC_UNUSED)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::init"
	o->_priv = G_TYPE_INSTANCE_GET_PRIVATE(o,GTK_TYPE_TEXT_COMPLETION_DATA,GtkTextCompletionDataPrivate);
	o->_priv->name = NULL;
	o->_priv->icon = NULL;
	o->_priv->user_data = NULL;
}
#undef __GOB_FUNCTION__
static void 
gtk_text_completion_data_class_init (GtkTextCompletionDataClass * c G_GNUC_UNUSED)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::class_init"
	GObjectClass *g_object_class G_GNUC_UNUSED = (GObjectClass*) c;

	g_type_class_add_private(c,sizeof(GtkTextCompletionDataPrivate));

	parent_class = g_type_class_ref (G_TYPE_OBJECT);

	g_object_class->finalize = ___finalize;
}
#undef __GOB_FUNCTION__



const gchar * 
gtk_text_completion_data_get_name (GtkTextCompletionData * self)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::get_name"
	g_return_val_if_fail (self != NULL, (const gchar * )0);
	g_return_val_if_fail (GTK_IS_TEXT_COMPLETION_DATA (self), (const gchar * )0);
{
		return self->_priv->name;
	}}
#undef __GOB_FUNCTION__

GdkPixbuf * 
gtk_text_completion_data_get_icon (GtkTextCompletionData * self)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::get_icon"
	g_return_val_if_fail (self != NULL, (GdkPixbuf * )0);
	g_return_val_if_fail (GTK_IS_TEXT_COMPLETION_DATA (self), (GdkPixbuf * )0);
{
	
		return self->_priv->icon;
	}}
#undef __GOB_FUNCTION__

gpointer 
gtk_text_completion_data_get_user_data (GtkTextCompletionData * self)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::get_user_data"
	g_return_val_if_fail (self != NULL, (gpointer )0);
	g_return_val_if_fail (GTK_IS_TEXT_COMPLETION_DATA (self), (gpointer )0);
{
	
		return self->_priv->user_data;
	}}
#undef __GOB_FUNCTION__

/**
 * gtk_text_completion_data_new_with_data:
 * @name: We copy the data
 * @icon: We ref the icon
 * @user_data: We don't make anything
 *
 * Create a new completion data object
 *
 * Returns: The new oject
 **/
GtkTextCompletionData * 
gtk_text_completion_data_new_with_data (const gchar * name, GdkPixbuf * icon, gpointer user_data)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::new_with_data"
{
	
		GtkTextCompletionData *data = GTK_TEXT_COMPLETION_DATA(GET_NEW);
		
		data->_priv->name = g_strdup(name);
		if (icon!=NULL)
			data->_priv->icon = g_object_ref(icon);
		data->_priv->user_data = user_data;
		return data;
	}}
#undef __GOB_FUNCTION__

void 
gtk_text_completion_data_free (GtkTextCompletionData * data)
{
#define __GOB_FUNCTION__ "Gtk:Text:Completion:Data::free"
{
	
		if (data->_priv->name != NULL)
		{
			g_free(data->_priv->name);
			data->_priv->name = NULL;
		}
		if (data->_priv->icon != NULL)
		{
			g_object_unref(data->_priv->icon);
			data->_priv->icon = NULL;
		}
		g_object_unref(data);
	}}
#undef __GOB_FUNCTION__
