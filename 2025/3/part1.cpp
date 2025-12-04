#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

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
  std::string data/* ="7189274928145535324\n987654321111111\n811111111111119\n234234234234278//\n818181911112111"*/;
  read("input.txt", &data);
  auto lines = split(data, "\n");
  int64_t count = 0;
  for (auto& line : lines) {
    auto max_it = std::max_element(line.begin(), line.end() - 1);
    auto max2_it = std::max_element(max_it + 1, line.end());
    count += cti(*max_it) * 10 + cti(*max2_it);
  }
	return count;
}