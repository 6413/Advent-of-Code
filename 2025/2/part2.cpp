#include <string>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

bool is_repeating(const std::string& digits, const std::string& chunk) {
  if (chunk.size() / 2 == 0) {
    return 0;
  }
  for (auto x : std::views::chunk(digits, chunk.size() / 2)) {
    if (!std::ranges::equal(x, chunk)) {
      return false;
    }
  }
  return true;
}

bool valid(const std::string& digits) {
  for (int i = 1; i < digits.size(); ++i) {
    auto chunk = digits.substr(0, i);
    if (is_repeating(digits, {chunk.begin(), chunk.end()}) && chunk.size() != digits.size()) {
      return true;
    }
  }
  return false;
}

bool read(const std::string& path, std::string* str) {
  std::ifstream file(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  if (file.fail()) {
    return 1;
  }
  str->resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(&(*str)[0], str->size());
  file.close();
  return 0;
}

std::vector<std::string> split(const std::string& str, std::string_view token) {
  std::vector<std::string> result;
  std::size_t start = 0;
  std::size_t pos = 0;

  while ((pos = str.find(token, start)) != std::string::npos) {
    result.push_back(str.substr(start, pos - start));
    start = pos + token.size();
  }
  result.push_back(str.substr(start));

  return result;
}

int main(int argc, char** argv) {

  std::string data;
  read(argv[1], &data);
  auto ranges = split(data, ",");
  int64_t count = 0;
  for (auto& range : ranges) {
    auto values = split(range, "-");
    int64_t start = std::stoll(values[0]);
    int64_t end = std::stoll(values[1]);
    for (int64_t i = start; i <= end; ++i) {
      std::string digits = std::to_string(i);
      count += std::stoll(digits) * valid(digits);
    }
  }
  printf("%lld\n", count);
}