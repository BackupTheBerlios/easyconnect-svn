bin_PROGRAMS = scoped 
scoped_SOURCES = main.c config.c functionlist.c devicelist.c generalfunctions.c devicefunctions.c

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/tcpcliserver


# the library search path.
scoped_LDFLAGS = $(all_libraries) -l pthread -l confuse -l dl

SUBDIRS = tcpcliserver 
scoped_LDADD = $(top_builddir)/src/tcpcliserver/libtcpcliserver.a $(top_builddir)/src/tcpcliserver/cl/libcl.a
