#include "experiment.hpp"
#include <chrono>

#define ENABLE_CACHE_WARMING

const uint64_t cache_line = 64;
const uint64_t elements_in_line = cache_line / 4;

int experiment(const uint64_t& array_size, const std::string& direction)
{
  int* arr = new int [array_size];
  int accessing_element;

  // warming up
#ifdef ENABLE_CACHE_WARMING
  for (uint64_t i = 0; i < array_size; i += elements_in_line)
  {
    accessing_element = arr[i];
  }
#endif

  namespace ch = std::chrono;

  float summary_time = 0;
  if (direction == "straight")
  {
    uint64_t l = 0;
    for (int i = 0; i < 1000; i++) {
      auto start = ch::high_resolution_clock::now();
      accessing_element = arr[l];
      auto end = ch::high_resolution_clock::now();
      ch::duration<uint64_t, std::nano> dur = end - start;
      float accessing_time_nano = static_cast<float>(dur.count());
      summary_time += accessing_time_nano;
      l += elements_in_line;
      l %= array_size;
    }
  } else if (direction == "reverse") {
    uint64_t l = array_size;
    for (int i = 0; i < 1000; i++) {
      auto start = ch::high_resolution_clock::now();
      accessing_element = arr[l];
      auto end = ch::high_resolution_clock::now();
      ch::duration<uint64_t, std::nano> dur = end - start;
      float accessing_time_nano = static_cast<float>(dur.count());
      summary_time += accessing_time_nano;
      l -= elements_in_line;
    }
  } else if (direction == "random") {
    uint64_t l = 0;
    for (int i = 0; i < 1000; i++) {
      auto start = ch::high_resolution_clock::now();
      accessing_element = arr[l];
      auto end = ch::high_resolution_clock::now();
      ch::duration<uint64_t, std::nano> dur = end - start;
      float accessing_time_nano = static_cast<float>(dur.count());
      summary_time += accessing_time_nano;
      unsigned int a = 1;
      l = rand_r(&a) % array_size;
    }
  } else {
    std::runtime_error ex(0);
    throw ex;
  }
  if (accessing_element)
  delete[] arr;
  double result = (summary_time / 1000);
  result = static_cast<int>((result * 100) / 100.);
  return result;
}
