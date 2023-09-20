AC_PREREQ(2.60)
AC_INIT(libpst,0.6.76,carl@five-ten-sg.com)
AC_CONFIG_SRCDIR([src/libpst.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CANONICAL_HOST
AC_USE_SYSTEM_EXTENSIONS

LT_INIT

AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)

#
#  1. Remember that version-info is current:revision:age, and age <= current.
#  2. If the source code has changed at all since the last public release,
#     then increment revision (`c:r:a' becomes `c:r+1:a').
#  3. If any interfaces have been added, removed, or changed since the last
#     update, increment current, and set revision to 0.
#  4. If any interfaces have been added since the last public release, then
#     increment age, since we should be backward compatible with the previous
#     version.
#  5. If any interfaces have been removed or changed since the last public
#     release, then set age to 0, since we are not backward compatible.
#  6. libtool will build libpst.so.x.y.z where the SONAME is libpst.so.x
#     and x=current-age, y=age, z=revision

libpst_version_info='5:18:1'
AC_SUBST(LIBPST_VERSION_INFO, [$libpst_version_info])
libpst_so_major='4'
AC_SUBST(LIBPST_SO_MAJOR, [$libpst_so_major])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET


# make sure we get large file support
AC_SYS_LARGEFILE
AC_CHECK_SIZEOF(off_t)


# Checks for header files.
AC_CHECK_HEADER([unistd.h],
    AM_CONDITIONAL(NEED_XGETOPT, [test yes = no]),
    AM_CONDITIONAL(NEED_XGETOPT, [test yes = yes])
    )
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([ctype.h dirent.h errno.h fcntl.h inttypes.h limits.h regex.h semaphore.h signal.h stdarg.h stdint.h stdio.h stdlib.h string.h sys/param.h sys/shm.h sys/stat.h sys/types.h time.h unistd.h wchar.h])
save_libs="$LIBS" ; LIBS=""
AC_SEARCH_LIBS([sem_init], [pthread rt], [SEM_LIBS="$LIBS"], [AC_MSG_ERROR([sem_init missing])])
AC_SUBST([SEM_LIBS])
LIBS="$save_libs"


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_HEADER_SYS_WAIT
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_PID_T
AC_STRUCT_TM


# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_STAT
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([chdir getcwd memchr memmove memset regcomp strcasecmp strncasecmp strchr strdup strerror strpbrk strrchr strstr strtol get_current_dir_name])
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.17])
AM_ICONV
if test "$am_cv_func_iconv" != "yes"; then
    AC_MSG_ERROR([libpst requires iconv which is missing])
fi


AC_CHECK_FUNCS(regexec,,[AC_CHECK_LIB(regex,regexec,
               [REGEXLIB=-lregex
                AC_DEFINE(HAVE_REGEXEC,1,[Define to 1 if you have the regexec function.])],
               [AC_MSG_ERROR([No regex library found])])])
AC_SUBST(REGEXLIB)


# The following lines adds the --enable-pst-debug option to configure:
#
# Give the user the choice to enter one of these:
# --enable-pst-debug
# --enable-pst-debug=yes
# --enable-pst-debug=no
#
AC_MSG_CHECKING([whether we are forcing debug dump file creation])
AC_ARG_ENABLE(pst-debug,
    AC_HELP_STRING([--enable-pst-debug], [force debug dump file creation]),
    [
        case "${enableval}" in
          yes) ;;
          no)  ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-pst-debug) ;;
        esac
    ],
    # default if not specified
    enable_pst_debug=no
    )
AC_MSG_RESULT([$enable_pst_debug])
if test "$enable_pst_debug" = "yes"; then
    AC_DEFINE(DEBUG_ALL, 1, Define to 1 to force debug dump file creation)
fi


# The following lines adds the --enable-libpst-shared option to configure:
#
# Give the user the choice to enter one of these:
# --enable-libpst-shared
# --enable-libpst-shared=yes
# --enable-libpst-shared=no
#
AC_MSG_CHECKING([whether we are building libpst shared object])
AC_ARG_ENABLE(libpst-shared,
    AC_HELP_STRING([--enable-libpst-shared], [build libpst shared object]),
    [
        case "${enableval}" in
          yes) ;;
          no)  ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-libpst-shared) ;;
        esac
    ],
    # default if not specified
    enable_libpst_shared=no
    )
AC_MSG_RESULT([$enable_libpst_shared])
enable_static_tools=no
enable_shared=yes

# needed by STATIC_TOOLS in src/Makefile.am
AC_SUBST(PST_OBJDIR, [$objdir])


# The following lines adds the --enable-static-tools option to configure:
#
# Give the user the choice to enter one of these:
# --enable-static-tools
# --enable-static-tools=yes
# --enable-static-tools=no
#
AC_MSG_CHECKING([whether to link command line tools with libpst statically])
AC_ARG_ENABLE([static-tools],
    AC_HELP_STRING([--enable-static-tools], [link command line tools with libpst statically]),
    [
        case "${enableval}" in
          yes) ;;
          no)  ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-static-tools) ;;
        esac
    ],
    [
        enable_static_tools=no
    ])
AC_MSG_RESULT([$enable_static_tools])
AM_CONDITIONAL(STATIC_TOOLS, [test "$enable_static_tools" = "yes"])
if test "$enable_static_tools" = "yes"; then
	enable_static="yes"
fi

# The following lines adds the --enable-profiling option to configure:
#
# Give the user the choice to enter one of these:
# --enable-profiling
# --enable-profiling=yes
# --enable-profiling=no
#
AC_MSG_CHECKING([whether to link with gprof profiling])
AC_ARG_ENABLE([profiling],
    AC_HELP_STRING([--enable-profiling], [link with gprof profiling]),
    [
        case "${enableval}" in
          yes)
              CFLAGS="$CFLAGS -pg"
              CPPFLAGS="$CPPFLAGS -pg"
              CXXFLAGS="$CXXFLAGS -pg"
              ;;
          no)
              ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --profiling) ;;
        esac
    ],
    [
        enable_profiling=no
    ])
AC_MSG_RESULT([$enable_profiling])
AM_CONDITIONAL(GPROF_PROFILING, [test "$enable_profiling" = "yes"])

PKG_CHECK_MODULES([GSF], [libgsf-1])

PKG_CHECK_MODULES([ZLIB], [zlib])

AC_OUTPUT(                  \
    Makefile                \
    html/Makefile           \
    libpst.pc               \
    libpst.spec             \
    man/Makefile            \
    src/Makefile            \
    xml/Makefile            \
    xml/libpst              \
    )

