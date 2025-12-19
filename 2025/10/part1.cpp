#include <bitset>
#include <type_traits>

import fan;
import fan.types.fstring;

static constexpr uint8_t max_bits = sizeof(uint32_t) * 8;

struct machine_t {
  std::bitset<max_bits> goal;
  std::vector<std::bitset<max_bits>> wiring;
  std::vector<int> joltage_requirements;
};

void parse(const std::string& data, std::vector<machine_t>& machines) {
  //fan::print("MUISTA VAIHTAA <N>\n");
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
uint32_t solve(const machine_t& machine) {
  uint32_t shortest = -1;
  for (uint32_t mask = 1; mask < 1 << machine.wiring.size(); ++mask) {
    uint32_t count = 0;
    uint32_t result = 0;
    for (uint32_t bit = 0; bit < machine.wiring.size(); ++bit) {
      (mask & (1 << bit)) && (result ^= machine.wiring[bit].to_ulong(), ++count, true);
    }
    (result == machine.goal && count < shortest) && (shortest = count);
  }
  return shortest;
}

int main() {
  std::string data = fan::io::file::read("input.txt");
  std::vector<machine_t> machines;

  parse(data, machines);
  //print(machines);

  fan::time::timer t {true};
  int64_t sum = 0;
  for (const auto& machine : machines) {
    sum += solve(machine);
  }
  fan::print(t.elapsed() / 1e6);
  fan::print("total:", sum);
}