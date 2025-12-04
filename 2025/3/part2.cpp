#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>

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

constexpr int cti(char v) {
  return v - '0';
}

int main(int argc, char** argv) {
//  std::string data =R"(987654321111111
//811111111111119
//234234234234278
//818181911112111)";
  std::string data;
  read("input.txt", &data);
  auto lines = split(data, "\n");
  int64_t count = 0;
  int n = 12;
  for (auto& line : lines) {
    int prev_idx = 0;
    std::string::iterator it = line.begin();
    int64_t multiplier = 1000000000000;
    for (int i = 0; i < n; ++i) {
      count += cti(*(it = std::max_element(it + !!i, line.end() - n + i + !!i))) * (multiplier *= 0.1);
    }
  }
  printf("%lld\n", count);
}