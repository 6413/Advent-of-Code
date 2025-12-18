#include <bitset>
#include <type_traits>

import fan;
import fan.types.fstring;

static constexpr uint8_t max_bits = 4;

struct machine_t {
  std::bitset<max_bits> goal;
  std::vector<std::bitset<max_bits>> wiring;
  std::vector<int> joltage_requirements;
};

void parse(const std::string& data, std::vector<machine_t>& machines) {
  fan::print("MUISTA VAIHTAA <N>\n");
  auto lines = fan::split(data);
  for (const auto& line : lines) {
    std::size_t begin = line.find('[');
    std::size_t end;
    if (begin == std::string::npos) {
      break;
    }
    ++begin; // skip [
    end = line.find(']');
    machines.resize(machines.size() + 1);
    machine_t& machine = machines.back();
    for (int i = begin; i < end; ++i) {
      machine.goal[i - begin] = line[i] == '#';
    }

    std::size_t start = begin;

    while (1) {
      begin = line.find('(', start); // skip (
      if (begin == std::string::npos) {
        break;
      }
      ++begin;
      end = line.find(')', start);

      auto& wiring = machine.wiring;
      wiring.resize(wiring.size() + 1);
      for (int i = begin; i < end; ++i) {
        if (line[i] == ',') continue;
        wiring.back()[line[i] - '0'] = 1;
      }
      start = end + 1;
    }

    {
      begin = line.find('{');
      ++begin;
      end = line.find('}', begin);
      auto& jr = machine.joltage_requirements;
      std::size_t start = begin;
      while(1) {
        std::size_t till = line.find(',', start);
        if (till == std::string::npos) {
          till = end + 1;
        }
        jr.emplace_back(std::stoi(line.substr(start, till - start)));
        if (till == line.size()) {
          break;
        }
        start = till + 1;
      }
    }
  }
}

void print(const std::vector<machine_t>& machines) {
  for (const auto& machine : machines) {
    fan::print(machine.goal);
    fan::print("-----");
    for (auto& mask : machine.wiring) {
      fan::printr();
      fan::printr(mask, "\n");
    }
    fan::print("-----");
    fan::print(typename decltype(machine_t::wiring)::value_type(
      0b11111
    ));
    fan::printr("{ ");
    for (auto& joltage : machine.joltage_requirements) {
      fan::printr(joltage, " ");
    }
    fan::print("}\n");
  }
}

std::vector<std::bitset<max_bits>> solve(machine_t& machine) {
  uint32_t combinations = 1 << machine.wiring.size();
  std::vector<std::bitset<max_bits>> shortest_combo;
  uint32_t shortest_combo_counter = -1;
  for (uint32_t mask = 1; mask < combinations; ++mask) {
    std::vector<std::bitset<max_bits>> possible_combinations;
    for (int bit = 0; bit < machine.wiring.size(); ++bit) {
      if (mask & (1 << bit)) {
        possible_combinations.emplace_back(machine.wiring[bit]);
      }
    }
    std::bitset<max_bits> result;
    for (const auto& bits : possible_combinations) {
      result ^= bits;
    }
    if (result == machine.goal) {
      if (possible_combinations.size() < shortest_combo_counter) {
        shortest_combo = possible_combinations;
        shortest_combo_counter = possible_combinations.size();
      }
    }
    //for (auto& mask : possible_combinations) {
    //  fan::print(mask);
    //}
   /* if (possible_combinations.size() == 2 && possible_combinations[1] == 0b1100) {
      fan::print("\n");
      fan::print(possible_combinations[0] ^ possible_combinations[1], machine.goal);
      fan::print((result & machine.goal) == machine.goal, (result & machine.goal), possible_combinations.size());
      fan::print("\n");
    }*/
    //fan::print("-----");
    //fan::print(machine.goal);
    fan::print("\n");
  }
  return shortest_combo;
}


int main() {
  std::string data = fan::io::file::read("input.txt");
  std::vector<machine_t> machines;
 
  parse(data, machines);

  //print(machines);

  int64_t sum = 0;

  for (auto [i, machine] : fan::enumerate(machines)) {
    auto solved = solve(machine);
    for (auto& bset : solved) {
      fan::print(bset);
    }
    fan::print("-----");
    fan::print(machine.goal);
    sum += solved.size();
    fan::print("presses:", solved.size());
    fan::print("\n");
    fan::print("---------------------------", i, "---------------------------");
  }


  fan::print("total:", sum);
}