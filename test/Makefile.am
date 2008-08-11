bin_PROGRAMS = gtksnippets-test vars-test

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-I$(top_srcdir)/gsnippets	\
	-I$(top_srcdir)/gtksnippets	\
	`pkg-config --cflags gtksourceview-2.0`	\
	`pkg-config --cflags libglade-2.0` \
	$(PACKAGE_CFLAGS)

AM_CFLAGS = \
	-Wall	

gtksnippets_test_SOURCES = 		\
	main.c

gtksnippets_test_LDFLAGS =  \
	`pkg-config --libs libglade-2.0`	\
	`pkg-config --libs gtksourceview-2.0`	

gtksnippets_test_LDADD = 	\
	$(top_builddir)/gsnippets/libgsnippets-1.0.la	\
	$(top_builddir)/gtksnippets/libgtksnippets-1.0.la	\
	$(PACKAGE_LIBS)

 
vars_test_SOURCES = 		\
	vars-test.c

vars_test_LDADD = 	\
	$(top_builddir)/gsnippets/libgsnippets-1.0.la	\
	$(top_builddir)/gtksnippets/libgtksnippets-1.0.la	\
	$(PACKAGE_LIBS)
