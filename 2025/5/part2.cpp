#include <string>
#include <iostream>
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
  auto found = data.find("\n\n");
  std::vector<std::string> sfresh, savail;
  sfresh = split(data.substr(0, found), "\n");
  savail = split(data.substr(found + 2), "\n");
  std::vector<int64_t> digits_left;
  std::vector<int64_t> digits_right;
  std::vector<int64_t> avail;
  for (auto& i : sfresh) {
    auto found = i.find('-');
    digits_left.push_back(std::stoll(i.substr(0, found)));
    digits_right.push_back(std::stoll(i.substr(found + 1)));
  }
  int64_t count = 0;
  std::sort(digits_left.begin(), digits_left.end());
  std::sort(digits_right.begin(), digits_right.end());
  for (int left = 0; left < digits_left.size(); ++left) {
    int right_digit_seeker = left;
    int prev_left = left;
    int64_t end = digits_right[right_digit_seeker];
    while (end >= digits_left[right_digit_seeker + 1]) {
      if (right_digit_seeker + 1 >= digits_left.size()) {
        end = digits_right[right_digit_seeker];
        left = right_digit_seeker;
        break;
      }
      end = digits_right[right_digit_seeker + 1];
      ++right_digit_seeker;
      left = right_digit_seeker;
    }
    count += end - digits_left[prev_left] + 1;
  }
  std::cout << count << '\n';
}