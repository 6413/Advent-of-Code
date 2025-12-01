#include <string>
#include <fan/utility.h>
import fan;

int main() {
  int current = 50, code = 0, i = -1;
  std::size_t found = 0;
  std::string data = fan::io::file::read("input.txt");
  while ((found = data.find('\n', i = found + 1)) != std::string::npos) {
    code += (current += std::stoi(data.substr(i + 1)) * ((data[i] - 79) / 3)) % 100 == 0;
  }
  fan::print(code);
}