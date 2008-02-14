bin_PROGRAMS = gtksnippets

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-I$(top_srcdir)/gtksnippets	\
	`pkg-config --cflags gtk-2.0`	\
	`pkg-config --cflags libglade-2.0` \
	$(PACKAGE_CFLAGS)

AM_CFLAGS = \
	-Wall	

gtksnippets_SOURCES = main.c

gtksnippets_LDFLAGS =  \
	`pkg-config --libs libglade-2.0`

gtksnippets_LDADD = 	\
	$(PACKAGE_LIBS)

 
