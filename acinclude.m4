AC_ARG_ENABLE(pie, AC_HELP_STRING([--enable-pie],
                        [enable position independent executables flag]), [
        if (test "${enableval}" = "yes" &&
                                test "${ac_cv_prog_cc_pie}" = "yes"); then
                CFLAGS="$CFLAGS -fPIE"
                LDFLAGS="$LDFLAGS -pie"
        fi
	])

AC_DEFUN([COMPILER_FLAGS], [
        if (test "${CFLAGS}" = ""); then
                CFLAGS="-Wall -O2 -fsigned-char "
                CFLAGS+=" -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2"
        fi
        if (test "$USE_MAINTAINER_MODE" = "yes"); then
                CFLAGS+=" -Werror -Wextra"
                CFLAGS+=" -Wno-unused-parameter"
                CFLAGS+=" -Wno-missing-field-initializers"
                CFLAGS+=" -Wdeclaration-after-statement"
                CFLAGS+=" -Wmissing-declarations"
                CFLAGS+=" -Wredundant-decls"
                if ( $CC -v 2>/dev/null | grep "gcc version" ); then
                        CFLAGS+=" -Wcast-align"
                fi
        fi
])
