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
struct vec2i {
  int x, y;
  vec2i operator+(const vec2i& v) {
    return {x + v.x, y + v.y};
  }
};

static constexpr vec2i dirs[] {
  {-1,-1}, {0,-1}, {1,-1},
  {-1, 0},         {1, 0},
  {-1, 1}, {0, 1}, {1, 1}
};

bool check_bounds(const auto& map, vec2i pos) {
  return pos.x >= 0 && pos.y >= 0 &&
    pos.x < map[0].size() && pos.y < map.size();
}

bool check_dirs(const auto& map, vec2i center) {
  if (!map[center.y][center.x]) {
    return false;
  }
  int collisions = 0;
  for (vec2i dir : dirs) {
    vec2i curr = center + dir;
    if (check_bounds(map, curr) && map[curr.y][curr.x]) {
      if (++collisions >= 4) return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  std::string data;
  read("input.txt", &data);
  auto lines = split(data, "\n");
  std::vector<std::vector<bool>> map;
  for (auto& line : lines) {
    map.resize(map.size() + 1);
    for (auto& c : line) {
      map.back().emplace_back(c == '@');
    }
  }
  int count = 0, cycle_count;
  do {
    cycle_count = 0;
    for (int y = 0; y < map.size(); ++y) {
      for (int x = 0; x < map[y].size(); ++x) {
        if (check_dirs(map, {x,y})) {
          map[y][x] = false;
          ++count;
          ++cycle_count;
        }
      }
    }
  } while (cycle_count != 0);
  printf("%d\n", count);
}