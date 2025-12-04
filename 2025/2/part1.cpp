#include <string>
import fan;
import fan.types.fstring;

bool valid(const std::string& digits) {
  std::size_t half = digits.size() / 2;
  return (digits.substr(0, half) == digits.substr(half)) * !((half * 2) & 1);
}

int main() {
  std::string data = fan::io::file::read("input.txt");
  auto ranges = fan::split(data, ",");
  int64_t count = 0;
  for (auto& range : ranges) {
    auto values = fan::split(range, "-");
    int64_t start = std::stoll(values[0]);
    int64_t end = std::stoll(values[1]);
    for (int64_t i = start; i <= end; ++i) {
      std::string digits = std::to_string(i);
      count += std::stoll(digits) * valid(digits);
    }
  }
  fan::print(count);
}