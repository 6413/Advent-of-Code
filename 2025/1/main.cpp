#include <string>

import fan;

static constexpr uint8_t mod = 100;
static constexpr char L = 'L';
static constexpr char R = 'R';

int main() {
  std::string data;
  fan::io::file::read("input.txt", &data);

  std::vector<std::string> lines = fan::io::file::read_line("input.txt");
  int current = 50;
  int code = 0;
  for (auto& line : lines) {
    int dir = line[0] == L ? -1 : 1;
    line.erase(0, 1);
    int v = std::stoi(line);
    v *= dir;
    current += v;
    current %= mod;
    if (current == 0) {
      ++code;
    }
  }
  fan::print(code);
}