SUBDIRS =

AM_LDFLAGS =


bin_PROGRAMS =

bin_PROGRAMS     += gs_seq gs_openmp

gs_seq_SOURCES  = gs_seq.c
gs_seq_CFLAGS   = -O2

gs_openmp_SOURCES = gs_openmp.c
gs_openmp_CFLAGS  = -O2 $(OPENMP_CFLAGS)


# eigen-only sources
if HAVE_EIGEN

bin_PROGRAMS += lu ldlt matprod

lu_SOURCES = lu.C
lu_CXXFLAGS = $(EIGEN_INCLUDE) $(OPENMP_CXXFLAGS)

ldlt_SOURCES = ldlt.C
ldlt_CXXFLAGS = $(EIGEN_INCLUDE) $(OPENMP_CXXFLAGS)

matprod_SOURCES = matprod.C
matprod_CXXFLAGS = $(EIGEN_INCLUDE) $(OPENMP_CXXFLAGS)

endif


DISTCLEANFILES = *~ speedup_N*.pdf



# #
# # special things to do when running 'make clean/distclean'
# clean-local:
# 	rm -rf `find $(builddir) -type d -name "*.dSYM"`

# ouput_files = test.*.xdr test.*.bin test.*.h5 test.*.fbin out_?.e
# log_files   = slurm-*.out MPI-*.log

# MOSTLYCLEANFILES = $(ouput_files) $(logfiles)

# cleandata:
# 	rm -f $(ouput_files)

# cleanlogs:
# 	rm -f $(log_files)
