#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <execution>
#include <unordered_set>

import fan;

void shoot(std::vector<std::string>& map, fan::vec2i p, int64_t& sc) {
  if (++p.y >= map.size() || p.x >= map[0].size()) return;
  bool s = map[p.y][p.x] == '^';
  bool add = false;
  for (int x : {-1, 2}) {
    p.x += x * s;
    add |= ((map[p.y][p.x] != '|') && (map[p.y][p.x] = '|', shoot(map, p, sc), true));
  }
  sc += add * s;
}

int main(int argc, char** argv) {
  std::string data;
  data = fan::io::file::read("input.txt");
  std::vector<std::string> map = fan::split(data);
  fan::vec2i start = {map[0].size() / 2, 0};
  int64_t split_count = 0;
  shoot(map, start, split_count);
  fan::print(split_count);
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      printf("%c", map[i][j]);
    }
    printf("\n");
  }
}