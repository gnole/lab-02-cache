#include "yaml-cpp/yaml.h"
#include "experiment.hpp"
#include <iostream>
#include <string>

const uint64_t bytes_l1_line = 65536;
const uint64_t bytes_l3_line = 9437184;

std::ostream& print_investigation(const std::string& direction,
                                  std::ostream& out_func)
{
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "investigation";
  out << YAML::Value << YAML::BeginMap;
  out << YAML::Key << "travel_order";
  out << YAML::Value << direction;
  out << YAML::Key << "experiments";
  out << YAML::BeginSeq;
  uint64_t memory_size = bytes_l1_line / 2;
  int i = 1;
  while (memory_size < 3 * bytes_l3_line / 2) {
    out << YAML::BeginMap;
    out << YAML::Key << "experiment";
    out << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "number" << YAML::Value << i;
    out << YAML::Key << "input_data" << YAML::Key << YAML::BeginMap
        << YAML::Key << "buffer_size" << YAML::Value;
    std::string string_mem_size = std::to_string(memory_size / 1024);
    string_mem_size += " kb";
    out << string_mem_size << YAML::EndMap;
    out << YAML::Key << "results" << YAML::Key << YAML::BeginMap
        << YAML::Key << "duration" << YAML::Value;
    std::string durability = std::to_string(experiment(memory_size, direction));
    durability += " ns";
    out << durability << YAML::EndMap;
    out << YAML::EndMap;
    out << YAML::EndMap;
    memory_size *= 2;
    i++;
  }
  out_func << out.c_str() << std::endl;
  return out_func;
}
