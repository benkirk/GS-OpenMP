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

# # Eigen+MKL sources
# # /ephemeral/benkirk/BENCHMARK/openmp_playground/^Cnfigure --prefix=/ephemeral/benkirk/BENCHMARK/aerolab_workstations-intel-19.0/install CXX=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/icpc LDFLAGS=-Wl,--no-warn-search-mismatch CC=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/icc FC=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/ifort CPPFLAGS=-I/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/include LDFLAGS="-L/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/lib/intel64 -Wl,-rpath,/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/lib/intel64 -Wl,--no-warn-search-mismatch" LIBS="-lmkl_intel_lp64 -lmkl_sequential -lmkl_core"
# # /ephemeral/benkirk/BENCHMARK/openmp_playground/configure --prefix=/ephemeral/benkirk/BENCHMARK/aerolab_workstations-intel-19.0/install CXX=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/icpc LDFLAGS=-Wl,--no-warn-search-mismatch CC=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/icc FC=/software/x86_64/intel/parallel_studio_xe_2019_update2/compilers_and_libraries/linux/bin/intel64/ifort CPPFLAGS=-I/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/include LDFLAGS="-L/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/lib/intel64 -Wl,-rpath,/software/x86_64/intel/parallel_studio_xe_2019_update2/mkl/lib/intel64 -Wl,--no-warn-search-mismatch" LIBS="-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lpthread -lm -fopenmp"

# bin_PROGRAMS += matprod-mkl
# matprod_mkl_SOURCES = matprod.C
# matprod_mkl_CPPFLAGS = -DDATATYPE=double -DEIGEN_USE_MKL_ALL $(EIGEN_INCLUDE)
# # end Eigen+MKL sources

endif
# end Eigen-only sources
#################################################

EXTRA_DIST = contrib

DISTCLEANFILES = *~ speedup_*_N*.pdf results.csv

cleanup:
	rm -rf slurm-*.out out-*/
