
#include "gtk-text-completion-data.h"

GtkTextCompletionData*
gtk_text_completion_data_new()
{
	return g_new(GtkTextCompletionData, 1);
}


