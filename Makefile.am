#SUBDIRS =

#AM_LDFLAGS =
AM_CPPFLAGS =
AM_CFLAGS   = -O2 $(OPENMP_CFLAGS)
AM_CXXFLAGS = -O2 $(OPENMP_CFLAGS)

bin_PROGRAMS =

bin_PROGRAMS     += gs_seq gs_openmp

gs_seq_SOURCES    = gs_seq.c
gs_openmp_SOURCES = gs_openmp.c

#################################################
# Eigen-only sources
if HAVE_EIGEN

AM_CPPFLAGS += $(EIGEN_INCLUDE)

bin_PROGRAMS += lu ldlt matprod-r4 matprod-r8

lu_SOURCES = lu.C

ldlt_SOURCES = ldlt.C

matprod_r4_SOURCES = matprod.C
matprod_r4_CPPFLAGS = -DDATATYPE=float $(EIGEN_INCLUDE)

matprod_r8_SOURCES = matprod.C
matprod_r8_CPPFLAGS = -DDATATYPE=double $(EIGEN_INCLUDE)

endif
# end Eigen-only sources
#################################################


DISTCLEANFILES = *~ speedup_*_N*.pdf
