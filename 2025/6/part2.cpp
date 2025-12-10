#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <execution>

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

char get_next_op(const std::string& data, int& prev_idx, int x_size, int y_size) {
  while (1) {
    if (data[prev_idx + (y_size - 1) * x_size] != ' ') {
      return data[prev_idx++ + (y_size - 1) * x_size];
    }
    ++prev_idx;
  }
}

int main(int argc, char** argv) {
  std::string data;
  read("input.txt", &data);
  int x_size = data.find('\n') + 1;
  int y_size = data.size() / (x_size - 1);
  int op_idx = 0;
  int x = 0;
  uint64_t res = 0;
  int curr_num = 0;
  uint64_t nums[4] {};
  std::vector<char> cs(y_size - 1);
  while(1) {
    for (int i = 0; i < y_size - 1; ++i) {
      cs[i] = data[i * x_size + x];
    }

    if (std::all_of(cs.begin(), cs.end(), [](char c) { return c == ' ' || c == '\n'; })) {
      char op = get_next_op(data, op_idx, x_size, y_size);
      uint64_t prev = res;
      switch (op) {
      case '*': {  res += std::accumulate(nums, nums + curr_num, 1ULL, std::multiplies<>{}); break;  }
      case '+': {  res += std::accumulate(nums, nums + curr_num, 0ULL, std::plus<>{}); break; }
      }
      if (cs[0] == '\n') {
        break;
      }
      curr_num = 0;
      std::memset(nums, 0, std::size(nums));
      ++x;
      continue;
    }
    nums[curr_num++] = std::stoull(std::string(cs.begin(), cs.end()));
    ++x;
  }
  std::cout << res << '\n';
}