#ifdef HAVE_CONFIG_H
#  include "programming_paradigms_config.h"
#endif

// https://www.modernescpp.com/index.php/performance-of-the-parallel-stl-algorithmn
// parallelSTLPerformance.cpp
#include <execution>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr long long size = 500'000'000;

const double pi = std::acos(-1);

template <typename Func>
void getExecutionTime(const std::string& title, Func func){                   // (4)

  const auto sta = std::chrono::steady_clock::now();
  func();                                                                     // (5)
  const std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;
  std::cout << title << ": " << dur.count() << " sec. " << std::endl;

}


template <typename T>
void print_front(const std::vector<T> &v, const std::string &label="")
{
  std::cout << label << "v.size()=" << v.size() << "; ";
  for (auto i=0; i!=11 && i<v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << "...\n";
}



int main(){

  typedef unsigned int size_type;
  std::vector<size_type> randValues, workVec;
  randValues.reserve(size); /**/ workVec.reserve(size);

  auto seq_size=randValues.size();

  std::mt19937 engine;
  std::uniform_int_distribution<size_type> uniformDist(0, std::numeric_limits<size_type>::max());
  for (auto i = 0 ; i < size ; ++i) randValues.push_back(uniformDist(engine));


  getExecutionTime("std::copy() / std::sort() / std::unique() / std::execution::seq", [ &randValues, &workVec, &seq_size]() mutable {
      auto policy =  std::execution::seq;
      workVec.resize(randValues.size());
      std::copy(policy, randValues.begin(), randValues.end(), workVec.begin());
      print_front(workVec, "input: ");
      std::sort(policy, workVec.begin(), workVec.end());
      print_front(workVec, "after sort: ");
      auto new_end = std::unique(policy, workVec.begin(), workVec.end());
      workVec.erase(new_end, workVec.end());
      seq_size = workVec.size();
      print_front(workVec, "after unique: ");
    }); print_front(workVec, "final: "); std::cout << '\n';

  getExecutionTime("std::copy() / std::sort() / std::unique() / std::execution::par", [ &randValues, &workVec, seq_size]() mutable {
      auto policy =  std::execution::par;
      workVec.resize(randValues.size());
      std::copy(policy, randValues.begin(), randValues.end(), workVec.begin());
      print_front(workVec, "input: ");
      std::sort(policy, workVec.begin(), workVec.end());
      print_front(workVec, "after sort: ");
      auto new_end = std::unique(policy, workVec.begin(), workVec.end());
      workVec.erase(new_end, workVec.end());
      print_front(workVec, "after unique: ");
      if (workVec.size() != seq_size) std::cerr << "==> ERROR: size mismatch from serial algorithm!\n";
    }); print_front(workVec, "final: "); std::cout << '\n';

  getExecutionTime("std::copy() / std::sort() / std::unique() / std::execution::par_unseq", [ &randValues, &workVec, seq_size]() mutable {
      auto policy =  std::execution::par_unseq;
      workVec.resize(randValues.size());
      std::copy(policy, randValues.begin(), randValues.end(), workVec.begin());
      print_front(workVec, "input: ");
      std::sort(policy, workVec.begin(), workVec.end());
      print_front(workVec, "after sort: ");
      auto new_end = std::unique(policy, workVec.begin(), workVec.end());
      workVec.erase(new_end, workVec.end());
      print_front(workVec, "after unique: ");
      if (workVec.size() != seq_size) std::cerr << "==> ERROR: size mismatch from serial algorithm!\n";
    }); print_front(workVec, "final: "); std::cout << '\n';
}
