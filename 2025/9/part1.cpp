#include <string>
#include <algorithm>
#include <vector>

import fan;

struct area {
  template<typename vec_T>
  constexpr bool operator()(const vec_T& a, const vec_T& b) const {
    return a.multiply() < b.multiply();
  }
};

int main() {
  std::string data = fan::io::file::read("input.txt");
  std::vector<fan::vec2ll> positions;
  // parse 2D vectors from file
  {
    int i = 0;
    while (i < data.size()) {
      f32_t vals[2];
      for (int j = 0; j < std::size(vals); ++j) {
        auto found = data.find(',', i);
        auto found2 = data.find('\n', i);
        std::size_t end = std::min(
          found == std::string::npos ? data.size() : found,
          found2 == std::string::npos ? data.size() : found2
        );
        vals[j] = std::stof(data.substr(i, end - i));
        i = end + 1;
      }
      positions.emplace_back(fan::vec2ll(vals[0], vals[1]));
    }
  }

  fan::vec2ll distance = 0;
  for (auto [i, a] : fan::enumerate(positions)) {
    for (int64_t j = (i + 1); j < positions.size() + 1; ++j) {
      distance = std::max(distance, (positions[j % positions.size()] - a).abs(), area {});
    }
  }
  fan::print((distance + 1).multiply());
}