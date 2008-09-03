## @start 1
## Process this file with automake to produce Makefile.in

gladedir = $(datadir)/gtksnippets

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DGLADE_DIR=\""$(gladedir)"\" \
	`pkg-config --cflags gtksourceview-2.0`	\
	`pkg-config --cflags libglade-2.0` \
	-I$(top_srcdir)/gsnippets	\
	$(PACKAGE_CFLAGS)
	
AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES=libgtksnippets-1.0.la

libgtksnippets_headers = 	\
	gtksnippets-dialog.h		\
	gtksnippets-varsdialog.h	\
	gtksnippets-variable.h		\
	gtksnippets-gtv-var.h		\
	gtksnippets-inplaceparser.h
	
libgtksnippets_1_0_la_SOURCES	= \
	gtksnippets-dialog.c			\
	gtksnippets-varsdialog.c	\
	gtksnippets-variable.c		\
	gtksnippets-gtv-var.c		\
	gtksnippets-inplaceparser.c	\
	$(libgtksnippets_headers)


libgtksnippets_1_0_la_LDFLAGS =	\
	-version-info 1:0:0 
	
libgtksnippets_1_0_includedir = $(includedir)/gtksnippets-1.0/gtksnippets

libgtksnippets_1_0_include_HEADERS =          \
	$(libgtksnippets_headers)             
	
## @end 1
