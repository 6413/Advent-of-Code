#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <set>

import fan;
using namespace fan::graphics;


struct connection_t {
  std::pair<fan::vec3, fan::vec3> connection;
  f32_t distance = 0;
  bool operator==(const connection_t& c) const {
    return connection.first == c.connection.second && connection.second == c.connection.first;
  }
  bool operator<(const connection_t& c) const {
    return distance < c.distance;
  }
};

struct connection_hash_t {
  static std::size_t operator()(const connection_t& c) {
    return std::hash<fan::vec3>{}(c.connection.first) ^ (std::hash<fan::vec3>{}(c.connection.second) << 1);
  }
};
//std::unordered_set<std::pair<fan::vec3, fan::vec3>> connections;

int main(int argc, char** argv) {
  engine_t engine;
  engine.clear_color = fan::colors::blue;
  std::string data;
  data = fan::io::file::read("input.txt");
  fan::time::timer t {true};
  std::vector<fan::vec3> junction_boxes;
  std::vector<circle_t> visual_circles;
  // parse 3D vectors from file
  for (int i = 0; i < data.size(); ++i) {
    auto found = data.find('\n', i);
    if (found == std::string::npos) {
      found = data.size();
    }
    junction_boxes.emplace_back(data.substr(i, found));
    visual_circles.emplace_back(circle_t{{.position = junction_boxes.back(), .radius = 4}});
    i = found;
    //fan::print(junction_boxes.back());
  }
  
  std::vector<line_t> lines;
  std::set<connection_t> unsorted_connections;

  // insert all connections, ignore duplicate
  for (int i = 0; i < junction_boxes.size(); ++i) {
    for (int j = 0; j < junction_boxes.size(); ++j) {
      if (i == j) {
        continue;
      }
      unsorted_connections.insert(connection_t{
        .connection=std::make_pair(junction_boxes[i], junction_boxes[j]),
        .distance=junction_boxes[i].distance(junction_boxes[j])
      });
    }
  }
  // sort distances
  //std::sort(unsorted_connections.begin(), unsorted_connections.end(), [](connection_t& a, connection_t& b) {
  //  return a.distance < b.distance;
  //});

  std::vector<connection_t> sorted_connections(unsorted_connections.begin(), unsorted_connections.end());
  //
  //std::sort(sorted_connections.begin(), sorted_connections.end(), [](connection_t& a, connection_t& b) {
  //  return a.distance < b.distance;
  //});

  int junction_boxes_remaining = junction_boxes.size();

  std::vector<std::vector<fan::vec3>> circuits;
  for (int i = 0; i < 1000; ++i) {
   // fan::print("");
    const auto& src = sorted_connections[i];
    {
      // junction boxes already in same circuit
      // find if src first == to any first or second in connections AND second to any first or second
      if (std::find_if(circuits.begin(), circuits.end(),
        [&](auto& a) { 
          auto has_point = [&](const fan::vec3& point) {
            return std::any_of(a.begin(), a.end(), [&](auto& b) {
              return point == b;
            });
          };
          return has_point(src.connection.first) && has_point(src.connection.second);
        }) != circuits.end()) {
     //   fan::print("boxes in came circuit, skipping...");
        continue;
      }
    }
  //  fan::print("closest:", src.connection.first, "-", src.connection.second);
    // find 'src' from existing circuits, whether we need to add new circuit or append
    // find where first and second or second and first matches src connection then continue
    auto is_destination_connected = std::find_if(circuits.begin(), circuits.end(),
      [&](auto& a) { return std::find_if(a.begin(), a.end(),
        [&](auto& b) {
          return src.connection.first == b ||
            src.connection.first == b;
        }) != a.end();
      }
    );
    auto is_src_connected = std::find_if(circuits.begin(), circuits.end(),
      [&](auto& a) { return std::find_if(a.begin(), a.end(),
        [&](auto& b) {
          return src.connection.second == b ||
            src.connection.second == b;
        }) != a.end();
      }
    );
    // append to existing circuit
    if (is_destination_connected != circuits.end()) {
      // check if src already connected, if so move src connections to dst
      // append src to dst
      if (is_src_connected != circuits.end()) {
   //     fan::print("combining two circuits of counts:", is_destination_connected->size(), "and", is_src_connected->size());
        is_destination_connected->insert(
          is_destination_connected->end(),
          is_src_connected->begin(),
          is_src_connected->end()
        );
        is_src_connected = circuits.erase(is_src_connected);
      }
      else {
        is_destination_connected->emplace_back(src.connection.second);
       // fan::print("appending to existing circuit");
        --junction_boxes_remaining;
      }
    }
    // creat new circuit
    else {
      if (is_src_connected != circuits.end()) {
      //  fan::print("appending src to new circuit");
        is_src_connected->emplace_back(
          src.connection.first
        );
        --junction_boxes_remaining;
      }
      else {
     //   fan::print("creating new circuit");
        circuits.insert(circuits.end(), std::vector<fan::vec3>{
          src.connection.first, src.connection.second
        });
        junction_boxes_remaining -= 2;
      }
    }
    //for (auto& obj : circuits) {
    //  fan::print("{");
    //  std::string circuit_str;
    //  printf("    ");
    //  for (auto [i, j] : fan::enumerate(obj)) {
    //    circuit_str += (std::string)j + ",\n";
    //    if (i != obj.size() - 1) {
    //      circuit_str += "    ";
    //    }
    //  }
    //  printf("%s", circuit_str.c_str());
    //  fan::print("}");
    //}
  }
 // fan::print("junction boxes remaining:", junction_boxes_remaining);
  //fan::print("circuits:", circuits.size());
  std::sort(circuits.begin(), circuits.end(), [](auto& a, auto& b) {return a.size() > b.size(); });
  fan::print("el:", t.elapsed());
  fan::print("multiplication:", circuits[0].size() * circuits[1].size() * circuits[2].size());
  engine.loop();
}