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

int main(int argc, char** argv) {
  std::string data;
  read("input.txt", &data);
  std::vector<std::pair<int64_t, int64_t>> fresh;
  std::vector<int64_t> avail;

  { // parse
    auto found = data.find("\n\n");
    std::vector<std::string> sfresh, savail;
    sfresh = split(data.substr(0, found), "\n");
    savail = split(data.substr(found + 2), "\n");
    for (auto& i : sfresh) {
      auto found = i.find('-');
      fresh.push_back({std::stoll(i.substr(0, found)), std::stoll(i.substr(found + 1))});
    }
    for (auto& i : savail) {
      avail.emplace_back(std::stoll(i));
    }
  }

  int count = 0;
  for (auto& i : avail) {
    for (auto& j : fresh) {
      if (i >= j.first && i <= j.second) {
        ++count;
        break;
      }
    }
  }
  printf("%d\n", count);
}