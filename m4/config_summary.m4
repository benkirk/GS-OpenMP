# SYNOPSIS
#
#   Summarizes configuration settings.
#
#   AX_SUMMARIZE_CONFIG([, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
#
# DESCRIPTION
#
#   Outputs a summary of relevant configuration settings.
#
# LAST MODIFICATION
#
#   2009-11-19
#

AC_DEFUN([AX_SUMMARIZE_CONFIG],
[

######################################################################################
echo
echo '----------------------------------- SUMMARY -----------------------------------'
echo
echo Package version............... : $PACKAGE-$VERSION
echo
echo C++ compiler.................. : $CXX
echo CXXFLAGS...................... : $CXXFLAGS
echo CPPFLAGS...................... : $CPPFLAGS
echo C compiler.................... : $CC
echo CFLAGS........................ : $CFLAGS
echo Fortran compiler.............. : $FC
echo FCFLAGS....................... : $FCFLAGS
#echo Fortran 77 compiler........... : $F77
#echo libmesh dir................... : $LIBMESH_PREFIX
#echo libmesh-config................ : $LIBMESH_CONFIG
echo Install dir................... : $prefix
echo Python install dir............ : $pythondir
echo PYTHONPATH.................... : $PYTHONPATH
#echo Build user.................... : $USER
#echo Build host.................... : $BUILD_HOST
#echo Configure date................ : $BUILD_DATE
#echo Build architecture............ : $BUILD_ARCH
#echo SVN revision number........... : $BUILD_VERSION
#echo SVN status.................... : $BUILD_DEVSTATUS

######################################################################################
echo
echo Optional Dependencies:

######################################################################################
echo '  'eigen....................... : $enableeigen
if test "x$enabletbb" = "xyes"; then
  echo '      'EIGEN_INCLUDE........... : $EIGEN_INCLUDE
fi

######################################################################################
echo '  'tbb......................... : $enabletbb
if test "x$enabletbb" = "xyes"; then
  echo '      'TBB_INCLUDE............. : $TBB_INCLUDE
  echo '      'TBB_LIBRARY............. : $TBB_LIBRARY
fi

echo
echo '-------------------------------------------------------------------------------'

echo
echo Configure complete, now type \'make\' and then \'make install\'.
echo

])
