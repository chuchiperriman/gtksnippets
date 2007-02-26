## Process this file with automake to produce Makefile.in
## Created by Anjuta

DISTCHECK_CONFIGURE_FLAGS=--enable-gtk-doc

SUBDIRS = libgtksnippets src po  

gtksnippetsdocdir = ${prefix}/doc/gtksnippets
gtksnippetsdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(gtksnippetsdoc_DATA) docs/reference/libgtksnippets

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done
