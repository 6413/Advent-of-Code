#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
import fan;

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
  std::vector<std::string> lines = split(data, "\n");
  std::vector<std::vector<int64_t>> table(lines.size() - 1);
  int64_t total = 0;
  int k = 0;
  fan::time::timer t {true};
  int l = 0;
  for (int i = 0; i < lines.size() - 1; ++i) {
    const std::string& line = lines[i];
    int64_t v;
    std::stringstream ss(line);
    int x = 0;
    table[i].reserve(1000);
    while (ss >> v) {
      table[i][x] = v;
      ++x;
    }
  }

  for (auto c : lines.back()) {
    if (c == ' ') {
      continue;
    }
    int64_t res = 0;
    for (int j = 0; j < table.size(); ++j) {
      switch (c) {
      case '*':
      {
        if (j == 0) res = 1;
        res *= table[j][k];
        break;
      }
      case '/':
      {
        if (j == 0) res = 1;
        res /= table[j][k];
        break;
      }
      case '+':
      {
        res += table[j][k];
        break;
      }
      case '-':
      {
        res -= table[j][k];
        break;
      }
      }
    }
    total += res;
    ++k;
  }
  //179200
  //608100
  fan::print(t.elapsed());
  std::cout << total << '\n';
}