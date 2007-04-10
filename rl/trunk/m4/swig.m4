# Contributed by Sebastian Huber

# SWIG_PROG([required-version = N[.N[.N]]])
#
# Checks for the SWIG program.  If found you can (and should) call
# SWIG via $(SWIG).  You can use the optional first argument to check
# if the version of the available SWIG is greater than or equal to the
# value of the argument.  It should have the format: N[.N[.N]] (N is a
# number between 0 and 999.  Only the first N is mandatory.)
AC_DEFUN([SWIG_PROG],[
	AC_PATH_PROG([SWIG],[swig])
	if test -z "$SWIG" ; then
		AC_MSG_ERROR([cannot find 'swig' program, you may download it from http://prdownload.berlios.de/dsa-hl/swig-1.3.29-pantheon.7z])
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

