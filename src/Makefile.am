PSTLIB = libpst.la

common_header = common.h        \
                define.h        \
                libpst.h        \
                libstrfunc.h    \
                lzfu.h          \
                timeconv.h      \
                vbuf.h          \
		pst.h

common_source = debug.c         \
                libpst.c        \
                libstrfunc.c    \
                lzfu.c          \
                timeconv.c      \
                vbuf.c          \
		pst.c		\
		pstexport.c

lib_LTLIBRARIES       = libpst.la
libpstincludedir      = $(includedir)/libpst-@LIBPST_SO_MAJOR@/libpst
libpstinclude_HEADERS = \
        common.h    \
        libpst.h    \
        libstrfunc.h\
        lzfu.h      \
        timeconv.h  \
        vbuf.h      \
	pst.h
libpst_la_LDFLAGS     = $(NO_UNDEFINED) -version-info @LIBPST_VERSION_INFO@
libpst_la_SOURCES     = $(common_source) $(common_header)
libpst_la_LIBADD      = $(LTLIBICONV) @ZLIB_LIBS@ $(SEM_LIBS)

# set the include path found by configure
INCLUDES= -I$(srcdir)/.. $(all_includes)

# the library search path.

# pkglib_LTLIBRARIES = libpst.la
bin_PROGRAMS = example
example_LDADD = libpst.la
example_SOURCES = example.c
