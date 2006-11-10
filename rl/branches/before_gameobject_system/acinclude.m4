# Contributed by Sebastian Huber

# SWIG_PROG([required-version = N[.N[.N]]])
#
# Checks for the SWIG program.  If found you can (and should) call
# SWIG via $(SWIG).  You can use the optional first argument to check
# if the version of the available SWIG is greater than or equal to the
# value of the argument.  It should have the format: N[.N[.N]] (N is a
# number between 0 and 999.  Only the first N is mandatory.)
AC_DEFUN([SWIG_PROG],[
	AC_PATH_PROG([SWIG],[swig-pantheon])
	if test -z "$SWIG" ; then
		AC_MSG_ERROR([cannot find 'swig-pantheon' program, you may download it from http://prdownload.berlios.de/dsa-hl/swig-1.3.29-pantheon.7z])
		SWIG='echo "error: SWIG is not installed, you may download it from http://prdownload.berlios.de/dsa-hl/swig-1.3.29-pantheon.7z" ; false'
	elif test -n "$1" ; then
		AC_MSG_CHECKING([for SWIG version])
		[swig_version=`$SWIG -version 2>&1 | grep 'SWIG Version' | sed 's/.*\([0-9]\+\.[0-9]\+\.[0-9]\+\)/\1/g'`]
		AC_MSG_RESULT([$swig_version])
		if test -n "$swig_version" ; then
			# Calculate the required version number
			[swig_tmp=( `echo $1 | sed 's/[^0-9]\+/ /g'` )]
			[swig_required_version=$(( 1000000 * ${swig_tmp[0]:-0} + 1000 * ${swig_tmp[1]:-0} + ${swig_tmp[2]:-0} ))]

			# Calculate the available version number
			[swig_tmp=( `echo $swig_version | sed 's/[^0-9]\+/ /g'` )]
			[swig_tmp=$(( 1000000 * ${swig_tmp[0]:-0} + 1000 * ${swig_tmp[1]:-0} + ${swig_tmp[2]:-0} ))]

			if test $swig_required_version -gt $swig_tmp ; then
				AC_MSG_ERROR([SWIG version $1 is required, you have $swig_version])
			fi
		else
			AC_MSG_WARN([cannot determine SWIG version])
		fi
		SWIG_RUNTIME_LIBS_DIR="${SWIG%/bin*}/lib"
		AC_MSG_NOTICE([SWIG runtime library directory is '$SWIG_RUNTIME_LIBS_DIR'])
	fi
	AC_SUBST([SWIG_RUNTIME_LIBS_DIR])
])

# SWIG_ENABLE_CXX()
#
# Enable SWIG C++ support.  This effects all invocations of $(SWIG).
AC_DEFUN([SWIG_ENABLE_CXX],[
	AC_REQUIRE([SWIG_PROG])
	AC_REQUIRE([AC_PROG_CXX])
	SWIG="$SWIG -c++ -minherit -Wall -v"
])

# SWIG_MULTI_MODULE_SUPPORT()
#
# Enable support for multiple modules.  This effects all invocations
# of $(SWIG).  You have to link all generated modules against the
# appropriate SWIG runtime library.  If you want to build Python
# modules for example, use the SWIG_PYTHON() macro and link the
# modules against $(SWIG_PYTHON_LIBS).
AC_DEFUN([SWIG_MULTI_MODULE_SUPPORT],[
	AC_REQUIRE([SWIG_PROG])
	SWIG="$SWIG -noruntime"
])


AC_DEFUN([SWIG_RUBY],[
	AC_REQUIRE([SWIG_PROG])
	AC_SUBST([SWIG_RUBY_OPT],[-ruby$swig_shadow])
	AC_SUBST([SWIG_RUBY_LIBS],["$SWIG_RUNTIME_LIBS_DIR -lswigrb"])
])

dnl @synopsis AX_BOOST([MINIMUM-VERSION])
dnl
dnl Test for the Boost C++ libraries of a particular version (or newer)
dnl
dnl If no path to the installed boost library is given the macro
dnl searchs under /usr, /usr/local, and /opt, and evaluates the
dnl $BOOST_ROOT environment variable. Further documentation is
dnl available at <http://randspringer.de/boost/index.html>.
dnl
dnl This macro calls:
dnl
dnl   AC_SUBST(BOOST_CPPFLAGS) / AC_SUBST(BOOST_LDFLAGS)
dnl
dnl And sets:
dnl
dnl   HAVE_BOOST
dnl
dnl @category InstalledPackages
dnl @category Cxx
dnl @author Thomas Porschberg <thomas@randspringer.de>
dnl @version 2006-06-15
dnl @license AllPermissive

AC_DEFUN([AX_BOOST_BASE],
[
AC_ARG_WITH([boost],
	AS_HELP_STRING([--with-boost@<:@=DIR@:>@],         specify the root directory for boost (optional)),
	[
    if test "$withval" = "yes"; then
        ac_boost_path=""
    else
        ac_boost_path="$withval"
    fi
    ],
    [want_boost="yes"])

boost_lib_version_req=ifelse([$1], ,1.20.0,$1)
boost_lib_version_req_shorten=`expr $boost_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
boost_lib_version_req_major=`expr $boost_lib_version_req : '\([[0-9]]*\)'`
boost_lib_version_req_minor=`expr $boost_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
boost_lib_version_req_sub_minor=`expr $boost_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
if test "x$boost_lib_version_req_sub_minor" = "x" ; then
    boost_lib_version_req_sub_minor="0"
fi
WANT_BOOST_VERSION=`expr $boost_lib_version_req_major \* 100000 \+  $boost_lib_version_req_minor \* 100 \+ $boost_lib_version_req_sub_minor`
AC_MSG_CHECKING(for Boost >= $boost_lib_version_req)
succeeded=no

dnl first we check the system location for boost libraries
dnl this location ist chosen if boost libraries are installed with the --layout=system option
dnl or if you install boost with RPM
if test "$ac_boost_path" != ""; then
    BOOST_LDFLAGS="-L$ac_boost_path/lib"
    BOOST_CPPFLAGS="-I$ac_boost_path/include"
else
    for ac_boost_path_tmp in /usr /usr/local /opt ; do
        if test -d "$ac_boost_path_tmp/include/boost" && test -r "$ac_boost_path_tmp/include/boost"; then
            BOOST_LDFLAGS="-L$ac_boost_path_tmp/lib"
            BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
            break;
        fi
    done
fi

CPPFLAGS_SAVED="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
export CPPFLAGS

LDFLAGS_SAVED="$LDFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
export LDFLAGS

AC_LANG_PUSH(C++)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]], [[
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
  error Boost version is too old
#endif
]])],[
AC_MSG_RESULT(yes)
succeeded=yes
found_system=yes
],[
])
AC_LANG_POP([C++])

dnl if we found no boost with system layout we search for boost libraries
dnl built and installed without the --layout=system option or for a staged(not installed) version
if test "x$succeeded" != "xyes"; then
    _version=0
    if test "$ac_boost_path" != ""; then
        BOOST_LDFLAGS="-L$ac_boost_path/lib"
        if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
            for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                V_CHECK=`expr $_version_tmp \> $_version`
                if test "$V_CHECK" = "1" ; then
                    _version=$_version_tmp
                fi
                VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
                BOOST_CPPFLAGS="-I$ac_boost_path/include/boost-$VERSION_UNDERSCORE"
            done
        fi
    else
        for ac_boost_path in /usr /usr/local /opt ; do
            if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
                for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                    _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                    V_CHECK=`expr $_version_tmp \> $_version`
                    if test "$V_CHECK" = "1" ; then
                        _version=$_version_tmp
                        best_path=$ac_boost_path
                    fi
                done
            fi
        done

        VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
        BOOST_CPPFLAGS="-I$best_path/include/boost-$VERSION_UNDERSCORE"
        BOOST_LDFLAGS="-L$best_path/lib"

        if test "x$BOOST_ROOT" != "x"; then
            if test -d "$BOOST_ROOT" && test -r "$BOOST_ROOT" && test -d "$BOOST_ROOT/stage/lib" && test -r "$BOOST_ROOT/stage/lib"; then
                version_dir=`expr //$BOOST_ROOT : '.*/\(.*\)'`
                stage_version=`echo $version_dir | sed 's/boost_//' | sed 's/_/./g'`
                stage_version_shorten=`expr $stage_version : '\([[0-9]]*\.[[0-9]]*\)'`
                V_CHECK=`expr $stage_version_shorten \>\= $_version`
                if test "$V_CHECK" = "1" ; then
                    AC_MSG_NOTICE(We will use a staged boost library from $BOOST_ROOT)
                    BOOST_CPPFLAGS="-I$BOOST_ROOT"
                    BOOST_LDFLAGS="-L$BOOST_ROOT/stage/lib"
                fi
            fi
        fi
    fi

    CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
    export CPPFLAGS
    LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
    export LDFLAGS

    AC_LANG_PUSH(C++)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    @%:@include <boost/version.hpp>
   ]], [[
   #if BOOST_VERSION >= $WANT_BOOST_VERSION
   // Everything is okay
   #else
   #  error Boost version is too old
   #endif
   ]])],[
   AC_MSG_RESULT(yes)
   succeeded=yes
   found_system=yes
   ],[
   ])
   AC_LANG_POP([C++])
fi

if test "$succeeded" != "yes" ; then
    if test "$_version" = "0" ; then
        AC_MSG_ERROR([[We could not detect the boost libraries (version $boost_lib_version_req_shorten or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
    else
        AC_MSG_NOTICE([Your boost libraries seems to old (version $_version).])
    fi
else
    AC_SUBST(BOOST_CPPFLAGS)
    AC_SUBST(BOOST_LDFLAGS)
    AC_DEFINE(HAVE_BOOST,,[define if the Boost library is available])
fi

CPPFLAGS="$CPPFLAGS_SAVED"
LDFLAGS="$LDFLAGS_SAVED"

])

dnl @synopsis AX_BOOST_THREAD
dnl
dnl Test for Thread library from the Boost C++ libraries. The macro
dnl requires a preceding call to AX_BOOST_BASE. Further documentation
dnl is available at <http://randspringer.de/boost/index.html>.
dnl
dnl This macro calls:
dnl
dnl   AC_SUBST(BOOST_THREAD_LIB)
dnl
dnl And sets:
dnl
dnl   HAVE_BOOST_THREAD
dnl
dnl @category InstalledPackages
dnl @category Cxx
dnl @author Thomas Porschberg <thomas@randspringer.de>
dnl @author Michael Tindal <mtindal@paradoxpoint.com>
dnl @version 2006-06-15
dnl @license AllPermissive

AC_DEFUN([AX_BOOST_THREAD],
[
AC_ARG_WITH([boost-thread],
            AS_HELP_STRING([--with-boost-thread@<:@=special-lib@:>@],
            [use the Thread library from boost - it is possible to specify a certain library for the linker
                        e.g. --with-boost-thread=boost_thread-gcc-mt ]),
            [
    if test "$withval" = "yes"; then
        want_boost="yes"
        ax_boost_user_thread_lib=""
    else
        want_boost="yes"
        ax_boost_user_thread_lib="$withval"
    fi
            ],
            [want_boost="yes"])

AC_REQUIRE([AC_PROG_CC])
CPPFLAGS_SAVED="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
export CPPFLAGS

LDFLAGS_SAVED="$LDFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
export LDFLAGS

AC_CACHE_CHECK(whether the Boost::Thread library is available,
               ax_cv_boost_thread,
               [AC_LANG_PUSH([C++])
CXXFLAGS_SAVE=$CXXFLAGS

if test "x$build_os" = "xsolaris" ; then
    CXXFLAGS="-pthreads $CXXFLAGS"
elif test "x$build_os" = "xming32" ; then
    CXXFLAGS="-mthreads $CXXFLAGS"
else
    CXXFLAGS="-pthread $CXXFLAGS"
fi
AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/thread/thread.hpp>]],
                  [[boost::thread_group thrds; return 0;]]),
                  ax_cv_boost_thread=yes, ax_cv_boost_thread=no)
CXXFLAGS=$CXXFLAGS_SAVE
AC_LANG_POP([C++])
               ])

if test "x$ax_cv_boost_thread" = "xyes"; then
    if test "x$build_os" = "xsolaris" ; then
        BOOST_CPPFLAGS="-pthreads $BOOST_CPPFLAGS"
    elif test "x$build_os" = "xming32" ; then
        BOOST_CPPFLAGS="-mthreads $BOOST_CPPFLAGS"
    else
        BOOST_CPPFLAGS="-pthread $BOOST_CPPFLAGS"
    fi

    AC_SUBST(BOOST_CPPFLAGS)

    AC_DEFINE(HAVE_BOOST_THREAD,,[define if the Boost::Thread library is available])
    BN=boost_thread

    if test "x$ax_boost_user_thread_lib" = "x"; then
        for ax_lib in $BN $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s \
            lib$BN lib$BN-$CC lib$BN-$CC-mt lib$BN-$CC-mt-s lib$BN-$CC-s \
            $BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s ; do
            AC_CHECK_LIB($ax_lib, main, [BOOST_THREAD_LIB="-l$ax_lib" AC_SUBST(BOOST_THREAD_LIB) link_thread="yes" break],
                         [link_thread="no"])
        done
    else
        for ax_lib in $ax_boost_user_thread_lib $BN-$ax_boost_user_thread_lib; do
             AC_CHECK_LIB($ax_lib, main,
                         [BOOST_THREAD_LIB="-l$ax_lib" AC_SUBST(BOOST_THREAD_LIB) link_thread="yes" break],
                         [link_thread="no"])
        done
    fi

    if test "x$link_thread" = "xno"; then
        AC_MSG_ERROR(Could not link against $ax_lib !)
    fi
fi

CPPFLAGS="$CPPFLAGS_SAVED"
LDFLAGS="$LDFLAGS_SAVED"
])

dnl
dnl AM_PATH_CPPUNIT(MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
AC_DEFUN([AM_PATH_CPPUNIT],
[

AC_ARG_WITH(cppunit-prefix,[  --with-cppunit-prefix=PFX   Prefix where CppUnit is installed (optional)],
            cppunit_config_prefix="$withval", cppunit_config_prefix="")
AC_ARG_WITH(cppunit-exec-prefix,[  --with-cppunit-exec-prefix=PFX  Exec prefix where CppUnit is installed (optional)],
            cppunit_config_exec_prefix="$withval", cppunit_config_exec_prefix="")

  if test x$cppunit_config_exec_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --exec-prefix=$cppunit_config_exec_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_exec_prefix/bin/cppunit-config
     fi
  fi
  if test x$cppunit_config_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --prefix=$cppunit_config_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_prefix/bin/cppunit-config
     fi
  fi

  AC_PATH_PROG(CPPUNIT_CONFIG, cppunit-config, no)
  cppunit_version_min=$1

  AC_MSG_CHECKING(for Cppunit - version >= $cppunit_version_min)
  no_cppunit=""
  if test "$CPPUNIT_CONFIG" = "no" ; then
    AC_MSG_RESULT(no)
    no_cppunit=yes
  else
    CPPUNIT_CFLAGS=`$CPPUNIT_CONFIG --cflags`
    CPPUNIT_LIBS=`$CPPUNIT_CONFIG --libs`
    cppunit_version=`$CPPUNIT_CONFIG --version`

    cppunit_major_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_major_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    if test "x${cppunit_major_min}" = "x" ; then
       cppunit_major_min=0
    fi

    cppunit_minor_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    if test "x${cppunit_minor_min}" = "x" ; then
       cppunit_minor_min=0
    fi

    cppunit_micro_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x${cppunit_micro_min}" = "x" ; then
       cppunit_micro_min=0
    fi

    cppunit_version_proper=`expr \
        $cppunit_major_version \> $cppunit_major_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \> $cppunit_minor_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \= $cppunit_minor_min \& \
        $cppunit_micro_version \>= $cppunit_micro_min `

    if test "$cppunit_version_proper" = "1" ; then
      AC_MSG_RESULT([$cppunit_major_version.$cppunit_minor_version.$cppunit_micro_version])
    else
      AC_MSG_RESULT(no)
      no_cppunit=yes
    fi
  fi

  if test "x$no_cppunit" = x ; then
     ifelse([$2], , :, [$2])
  else
     CPPUNIT_CFLAGS=""
     CPPUNIT_LIBS=""
     ifelse([$3], , :, [$3])
  fi

  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
])
