# ChuchiSnippets

plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 						\
	$(GEDIT_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libchuchisnippets.la

libchuchisnippets_la_SOURCES = \
	chuchisnippets-plugin.h	\
	chuchisnippets-plugin.c

libchuchisnippets_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)	\
	-lgtksnippets

# Glade files (if you use glade for your plugin, list those files here)
gladedir = $(datadir)/gedit-2/glade
glade_DATA =

plugin_in_files = chuchisnippets.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA) $(glade_DATA)

DISTCLEANFILES = $(plugin_DATA) $(glade_DATA)
