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
	AS_HELP_STRING([--with-boost@<:@=DIR@:>@],         specify the root directory for boost),
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

dnl @synopsis AX_BOOST_FILESYSTEM
dnl
dnl Test for Filesystem library from the Boost C++ libraries. The macro
dnl requires a preceding call to AX_BOOST_BASE. Further documentation
dnl is available at <http://randspringer.de/boost/index.html>.
dnl
dnl This macro calls:
dnl
dnl   AC_SUBST(BOOST_FILESYSTEM_LIB)
dnl
dnl And sets:
dnl
dnl   HAVE_BOOST_FILESYSTEM
dnl
dnl @category InstalledPackages
dnl @category Cxx
dnl @author Thomas Porschberg <thomas@randspringer.de>
dnl @author Michael Tindal <mtindal@paradoxpoint.com>
dnl @version 2006-06-15
dnl @license AllPermissive

AC_DEFUN([AX_BOOST_FILESYSTEM],
[
	AC_ARG_WITH([boost-filesystem],
	AS_HELP_STRING([--with-boost-filesystem@<:@=special-lib@:>@],
                   [use the Filesystem library from boost - it is possible to specify a certain library for the linker
                        e.g. --with-boost-filesystem=boost_filesystem-gcc-mt ]),
        [
        if test "$withval" = "yes"; then
            want_boost="yes"
            ax_boost_user_filesystem_lib=""
        else
		    want_boost="yes"
        	ax_boost_user_filesystem_lib="$withval"
		fi
        ],
        [want_boost="yes"]
	)

	if test "x$want_boost" = "xyes"; then
        AC_REQUIRE([AC_PROG_CC])
		CPPFLAGS_SAVED="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
		export CPPFLAGS

		LDFLAGS_SAVED="$LDFLAGS"
		LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
		export LDFLAGS

        AC_CACHE_CHECK(whether the Boost::Filesystem library is available,
					   ax_cv_boost_filesystem,
        [AC_LANG_PUSH([C++])
         AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/filesystem/path.hpp>]],
                                   [[using namespace boost::filesystem;
                                   path my_path( "foo/bar/data.txt" );
                                   return 0;]]),
            				       ax_cv_boost_filesystem=yes, ax_cv_boost_filesystem=no)
         AC_LANG_POP([C++])
		])
		if test "x$ax_cv_boost_filesystem" = "xyes"; then
			AC_DEFINE(HAVE_BOOST_FILESYSTEM,,[define if the Boost::Filesystem library is available])
			BN=boost_filesystem
            if test "x$ax_boost_user_filesystem_lib" = "x"; then
    			for ax_lib in $BN $BN-mt $BN-$CC $BN-$CC-mt $BN-$CC-mt-s $BN-$CC-s \
                              lib$BN lib$BN-mt lib$BN-$CC lib$BN-$CC-mt lib$BN-$CC-mt-s lib$BN-$CC-s \
                              $BN-mgw $BN-mgw $BN-mgw-mt $BN-mgw-mt-s $BN-mgw-s ; do
				    AC_CHECK_LIB($ax_lib, main,
                                 [BOOST_FILESYSTEM_LIB="-l$ax_lib" AC_SUBST(BOOST_FILESYSTEM_LIB) link_filesystem="yes" break],
                                 [link_filesystem="no"])
  				done
            else
               for ax_lib in $ax_boost_user_filesystem_lib $BN-$ax_boost_user_filesystem_lib; do
				      AC_CHECK_LIB($ax_lib, main,
                                   [BOOST_FILESYSTEM_LIB="-l$ax_lib" AC_SUBST(BOOST_FILESYSTEM_LIB) link_filesystem="yes" break],
                                   [link_filesystem="no"])
                  done

            fi
			if test "x$link_filesystem" = "xno"; then
				AC_MSG_ERROR(Could not link against $ax_lib !)
			fi
		fi

		CPPFLAGS="$CPPFLAGS_SAVED"
    	LDFLAGS="$LDFLAGS_SAVED"
	fi
])

