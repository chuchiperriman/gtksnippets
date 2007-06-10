INCLUDES = \
	$(GTK_CFLAGS)\
	$(GTKSOURCEVIEW_CFLAGS)\
	$(GNOMEVFS_CFLAGS)\
	$(VTE_CFLAGS)\
	$(LIBGLADE_CFLAGS)\
	$(LIBXML_CFLAGS)\
	$(GNOMEPRINT_CFLAGS)\
	$(GNOMEPRINTUI_CFLAGS)\
	$(GCONF_CFLAGS)\
	$(GNOMEUI_CFLAGS)\
	$(OPENLDEV_CFLAGS) \
	-I/usr/include/libgtksnippets-1
	
AM_CPPFLAGS = -DINSTALL_PREFIX=\"$(prefix)\"

plugindir = $(libdir)/openldev

plugin_LTLIBRARIES = libsnippets.la

libsnippets_la_SOURCES = snippets.cc
libsnippets_la_LIBADD = $(OPENLDEV_LIBS) /home/perriman/Projects/gtksnippets/libgtksnippets/libgtksnippets.la
libsnippets_la_LDFLAGS = $(PLUGIN_FLAGS) -lgtksnippets

plugin_DATA = snippets.plugin
EXTRA_DIST = $(plugin_DATA)
