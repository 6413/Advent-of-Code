#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unordered_set>
#include <set>
#include <deque>
#include <map>

import fan;
using namespace fan::graphics;

struct area {
  template<typename vec_T>
  constexpr bool operator()(const vec_T& a, const vec_T& b) const {
    return a.multiply() < b.multiply();
  }
};

fan::vec2ll translate(const fan::vec2ll& p) {
  return p / 200 + fan::vec2(100, 0);
//  return p * 100 + fan::vec2(200, 0);
}

int main() {
  engine_t engine;
  interactive_camera_t ic;
  std::string data = fan::io::file::read("input.txt");
  fan::time::timer timer {true};
  std::vector<fan::vec2ll> positions;
  std::vector<circle_t> circles;
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
      circles.emplace_back(circle_t(fan::vec3(translate(positions.back()), 0), 1.f));
    }
  }

  fan::vec2ll world_min = *std::min_element(positions.begin(), positions.end(), fan::vec_lexi_comp{});

  fan::vec2ll world_max = *std::max_element(positions.begin(), positions.end(), fan::vec_lexi_comp{});

  // contains src to dst, src and dst itself
  std::unordered_set<fan::vec2ll> all_corner_coordinates;

  for (int64_t i = 0; i < positions.size(); ++i) {
    fan::vec2ll current = positions[i];
    fan::vec2ll next = positions[(i + 1) % positions.size()];
    bool iterate_y = (next.y - current.y) != 0;
    for (int64_t j = current[iterate_y]; j != next[iterate_y]; (next[iterate_y] < current[iterate_y] ? --j : ++j)) {
      fan::vec2 insert = current;
      insert[iterate_y] = j;
      all_corner_coordinates.insert(insert);
      //circles.emplace_back(circle_t(fan::vec3(translate(insert), 5.f), 5.f, fan::colors::red.set_alpha(0.3)));
    }
  }

  std::vector<fan::physics::aabb_t> valid_aabbs;

  for (int64_t k = 0; k < 3; ++k) // 
  for (int64_t i = k; i < positions.size(); i += 2) {

    fan::vec2 current = positions[i];
    fan::vec2 next = positions[(i + 2) % positions.size()];

    {
      fan::vec2ll c0 = positions[i];
      fan::vec2ll c1 = positions[(i + 1) % positions.size()];
      fan::vec2ll c2 = positions[(i + 2) % positions.size()];

      if (all_corner_coordinates.contains(c0) && all_corner_coordinates.contains(c2)) {
        fan::vec2ll check {c0.x ^ c1.x ^ c2.x, c0.y ^ c1.y ^ c2.y};

        if (!all_corner_coordinates.contains(check)) {
        //  for (auto& i : )
        //  if ()
          continue;
        }
      }
    }


    fan::physics::aabb_t aabb(current, next);
    aabb.normalize();
    bool point_inside = false;
    for (int64_t j = (i + 1); j < positions.size(); ++j) {
      if (!all_corner_coordinates.contains(positions[j]) && aabb.contains_point(positions[j])) {
        point_inside = true;
        break;
      }
    }
    if (point_inside) {
      continue;
    }
    valid_aabbs.push_back(aabb);
  }
  
  fan::vec2ll distance = 0;
  fan::physics::aabb_t biggest_area;

  std::vector<rectangle_t> valid_corners;
  std::unordered_set<fan::vec2ll> invalid_corner;

  for (auto [i, a] : fan::enumerate(positions)) {
    for (int64_t j = (i + 1); j < positions.size() + 1; ++j) {
      fan::vec2ll c0 = positions[i];
      fan::vec2ll c1 = positions[(i + 1) % positions.size()];
      fan::vec2ll c2 = positions[(i + 2) % positions.size()];
      fan::vec2ll c3 {c0.x ^ c1.x ^ c2.x, c0.y ^ c1.y ^ c2.y};

      bool is_valid = true;

      int corner_valid_count = 0;

      for (fan::vec2ll corner : {c0, c1, c2, c3}) {
        if (all_corner_coordinates.contains(corner)) {
          ++corner_valid_count;
        }
      }
      //fan::print(corner_valid_count);
      if (corner_valid_count < 3) {
        continue;
      }
      for (fan::vec2ll corner : {c0, c1, c2, c3}) {
        
        
        /*if (!all_corner_coordinates.contains(corner)) {
          for (auto& aabb : valid_aabbs) {
            if (aabb.contains_point(corner)) {
              is_valid = false;
              break;
            }
          }
        }*/
        bool f2 = 0;
        for (auto& aabb : valid_aabbs) {
          if (aabb.contains_point(corner)) {
            f2 = 1;
          }
        }
        if (!f2 && !all_corner_coordinates.contains(corner)) {
          circles.emplace_back(circle_t(fan::vec3(translate(corner), 5.f), 0.3f, fan::colors::red.set_alpha(0.3)));
          invalid_corner.insert(corner);
        }
      }
    }
  }



  for (auto [i, a] : fan::enumerate(positions)) {
    for (int64_t j = (i + 1); j < positions.size() + 1; ++j) {

      fan::vec2ll min = a;
      fan::vec2ll max = positions[j % positions.size()];

      int64_t xmin = std::min(min.x, max.x);
      int64_t xmax = std::max(min.x, max.x);
      int64_t ymin = std::min(min.y, max.y);
      int64_t ymax = std::max(min.y, max.y);

      fan::vec2ll c0{xmin, ymin};
      fan::vec2ll c1{xmin, ymax};
      fan::vec2ll c2{xmax, ymin};
      fan::vec2ll c3{xmax, ymax};
      bool is_valid = true;

      //fan::vec2ll c0 = positions[j];
      //fan::vec2ll c1 = positions[(j + 1) % positions.size()];
      //fan::vec2ll c2 = positions[(j + 2) % positions.size()];
      //fan::vec2ll c3 {c0.x ^ c1.x ^ c2.x, c0.y ^ c1.y ^ c2.y};

      for (fan::vec2ll corner : {c0, c1, c2, c3}) {
        if (invalid_corner.contains(corner)) {
          is_valid = false;
          break;
        }
      }

      if (!is_valid) {
        continue;
      }

      for (fan::vec2ll corner : {c0, c1, c2, c3}) {

        //valid_corners.push_back(rectangle_t(translate(corner), 10, fan::colors::green.set_alpha(0.2)));
      }
      fan::vec2ll prev = distance;
      distance = std::max(distance, (max - min).abs(), area {});
      if (prev != distance) {
        biggest_area = {a, positions[j % positions.size()]};
      }
    }
  }


  fan::print((distance + 1).multiply());

  int draw_index = 0;

  std::vector<fan::color> v;

  for (auto [idx, i] : fan::enumerate(valid_aabbs)) {
    v.emplace_back(fan::random::bright_color());
  }


  engine.loop([&] {
    for (auto [idx, i] : fan::enumerate(valid_aabbs)) {
      fan::vec2ll min = translate(i.min);
      fan::vec2ll max = translate(i.max);
      fan::color c = fan::random::bright_color().set_alpha(0.3);

      fan::vec2 s2w_min = world_to_screen(min);
      fan::vec2 s2w_max = world_to_screen(max);

      /*fan::graphics::gui::text_at(i.min.to_string(i.min, 0), s2w_min);
      fan::graphics::gui::text_at(i.max.to_string(i.max, 0), s2w_max);*/

      int64_t xmin = std::min(min.x, max.x);
      int64_t xmax = std::max(min.x, max.x);
      int64_t ymin = std::min(min.y, max.y);
      int64_t ymax = std::max(min.y, max.y);

      fan::vec2ll c0{xmin, ymin};
      fan::vec2ll c1{xmin, ymax};
      fan::vec2ll c2{xmax, ymin};
      fan::vec2ll c3{xmax, ymax};

      if (draw_index / 100 % valid_aabbs.size()  == idx) {
        //rectangle(c0, 20, v[idx]);
        //rectangle(c1, 20, v[idx]);
        //rectangle(c2, 20, v[idx]);
        //rectangle(c3, 20, v[idx]);
      }


     // rectangle(min, 20, c);
     // rectangle(max, 20, c);
      aabb({min,max}, 55000, v[idx], 0.1);
      ////if ((draw_index % valid_aabbs.size())== idx) {
      //  aabb(translate(valid_aabbs[(draw_index) % valid_aabbs.size()].min), translate(valid_aabbs[(draw_index + valid_aabbs.size() / 4) % valid_aabbs.size()].min), 55600, fan::colors::red);
      //}
    }


    ++draw_index;

    for (int64_t i = 0; i < positions.size(); ++i) {
    //  line(translate(positions[i]), translate(positions[(i + 2) % positions.size()]), fan::colors::green);
      //line(translate(positions[(i)]), translate(positions[((i + 1)) % positions.size()]), fan::colors::red.set_alpha(1.0), 0.1);
    }

    aabb({translate(biggest_area.min), translate(biggest_area.max)}, 56000, fan::colors::blue.set_alpha(0.4));
  });
}