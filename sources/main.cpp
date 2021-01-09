#include "./investigation.h"
#include <fstream>

int main()
{
  std::ofstream file("report.yml");
  print_investigation("straight", file);
  print_investigation("reverse", file);
  print_investigation("random", file);
}
