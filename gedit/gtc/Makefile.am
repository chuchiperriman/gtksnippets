# gtc

plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 						\
	$(GEDIT_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	-I/usr/include/libgtksnippets-1	\
	-DGEDIT_GLADEDIR=\""$(datadir)/gedit-2/glade/"\"	\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libgtc.la

libgtc_la_SOURCES = \
	gtc-plugin.h	\
	gtc-plugin.c

libgtc_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) -lgtksnippets

#plugin_LDADD = /home/perriman/Projects/gtksnippets/libgtksnippets/libgtksnippets.la

# Glade files (if you use glade for your plugin, list those files here)
gladedir = $(datadir)/gedit-2/glade
glade_DATA = gtc-configuration.glade

plugin_in_files = gtc.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA) $(glade_DATA)

DISTCLEANFILES = $(plugin_DATA) $(glade_DATA)
