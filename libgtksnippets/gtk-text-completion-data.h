#ifndef __GTK_TEXT_COMPLETION_DATA_H__
#define __GTK_TEXT_COMPLETION_DATA_H__

#include <gtk/gtk.h>

typedef struct _GtkTextCompletionData GtkTextCompletionData;

struct _GtkTextCompletionData
{
	const gchar* name;
	GdkPixbuf* icon;
	//int priority
	gpointer user_data;
};

GtkTextCompletionData* gtk_text_completion_data_new();


#endif
