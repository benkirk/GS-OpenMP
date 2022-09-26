#include "programming_paradigms_config.h"

#ifdef HAVE_OPENMP
#ifdef HAVE_EIGEN


#ifndef DATATYPE
#  warning("Undefined DATATYPE, defaulting to double")
#  define DATATYPE double
#endif

#include <iostream>
#include <omp.h>
#include <Eigen/Core>
#include <Eigen/Dense>


int main (int argc, char **argv)
{
  typedef Eigen::Matrix<DATATYPE, Eigen::Dynamic, Eigen::Dynamic> Matrix;

  const unsigned int N  = (argc == 1) ? 10 : std::atoi(argv[1]);
  const unsigned int MT = (argc != 3) ?  1 : std::atoi(argv[2]);

  omp_set_num_threads(MT);
  Eigen::setNbThreads(MT);

  std::cout << "max_threads: " << MT << std::endl;

  //std::cout << "N, MT=" << N << ", " << MT << std::endl;

  const double t_tot_start = omp_get_wtime();

  Matrix A = Matrix::Random(N,N);

  const double t_ops_start = omp_get_wtime();

  Matrix A2 = A*A;

  const double
    t_end = omp_get_wtime(),
    FLOP  = static_cast<double>( N*N*(2*N - 1) ),
    FLOPS = FLOP / (t_end - t_ops_start);

  std::cout << "Total time: "      << (t_end - t_tot_start) << std::endl
            << "Operations time: " << (t_end - t_ops_start) << std::endl
            << "GFLOPS: " << FLOPS / 1.e9 << std::endl;

 return 0;
}


#endif // #ifdef HAVE_EIGEN
#endif // #ifdef HAVE_OPENMP
