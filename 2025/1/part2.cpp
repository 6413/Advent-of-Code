#include <string>
import fan;

int main() {
  int current = 50, code = 0, i = 0;
  std::size_t found = 0;
  std::string data = fan::io::file::read("input.txt");
  while ((found = data.find('\n', i = found + (!found ? 0 : 1))) != std::string::npos) {
    int new_turn = current + std::stoi(data.substr(i + 1)) * ((data[i] - 79) / 3);
    code += std::abs(new_turn) / 100 + (current && new_turn <= 0);
    current = fan::math::modi(new_turn, 100);
  }
  fan::print(code);
}