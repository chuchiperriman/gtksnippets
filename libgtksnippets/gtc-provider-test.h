/* Advanced Vala Sample Code */

#ifndef __GTC_PROVIDER_TEST_H__
#define __GTC_PROVIDER_TEST_H__

#include <glib.h>
#include <glib-object.h>
#include "gtk-text-completion-provider.h"
#include "gtk-text-completion-popup.h"

G_BEGIN_DECLS


#define TYPE_GTC_PROVIDER_TEST (gtc_provider_test_get_type ())
#define GTC_PROVIDER_TEST(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_GTC_PROVIDER_TEST, GtcProviderTest))
#define GTC_PROVIDER_TEST_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_GTC_PROVIDER_TEST, GtcProviderTestClass))
#define IS_GTC_PROVIDER_TEST(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_GTC_PROVIDER_TEST))
#define IS_GTC_PROVIDER_TEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_GTC_PROVIDER_TEST))
#define GTC_PROVIDER_TEST_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_GTC_PROVIDER_TEST, GtcProviderTestClass))

typedef struct _GtcProviderTest GtcProviderTest;
typedef struct _GtcProviderTestClass GtcProviderTestClass;
typedef struct _GtcProviderTestPrivate GtcProviderTestPrivate;

struct _GtcProviderTest {
	GObject parent;
	GtcProviderTestPrivate *priv;	
	GdkPixbuf *icon_test;
	GCompletion *completion;
	GList *word_list;
};
struct _GtcProviderTestClass {
	GObjectClass parent;
};

GType gtc_provider_test_get_type ();

GtcProviderTest* gtc_provider_test_new();

G_END_DECLS

#endif
