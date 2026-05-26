#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Station {
  std::string name;
  int time_to_next;
};

struct Line {
  std::string name;
  std::vector<Station> stations;
};

std::vector<std::string> split(const char *buffer) {
  std::string accumulator;
  std::vector<std::string> vec;
  int counter = 0;

  while (buffer != nullptr) {
    if (*buffer == '\0') {
      if (accumulator.size() != 0) {
        vec.push_back(accumulator);
        accumulator.erase();
      }

      break;
    }

    if (*buffer == ':') {
      buffer += 1;
      continue;
    }

    if (*buffer == '"') {
      counter += 1;
    }

    if (*buffer == ' ') {
      if (counter != 1) {
        vec.push_back(accumulator);
        accumulator.erase();
      } else {
        accumulator.push_back(' ');
      }

      if (counter > 1) {
        counter = 0;
      }

      buffer += 1;
      continue;
    }

    accumulator.push_back(*buffer);
    buffer += 1;
  }

  return vec;
}

std::vector<Line> load_data(const char *path) {
  std::ifstream infile(path);
  std::string line;
  std::string str;
  std::vector<std::string> items;

  std::vector<Line> lines;
  std::vector<Station> stations;
  std::string line_name;
  std::string station_name;
  int ttn = 0;

  while (std::getline(infile, str)) {
    items = split(str.c_str());
    line_name = items[0];

    for (int i = 1; i < items.size(); i++) {
      station_name = items[i];

      if (i + 1 == items.size()) {
        ttn = 0;
      } else {
        try {
          ttn = std::stoi(items[++i]);
        } catch (std::exception &) {
        }
      }

      stations.emplace_back(Station{.name = station_name, .time_to_next = ttn});
    }

    lines.emplace_back(Line{.name = line_name, .stations = stations});
  }

  return lines;
}

int main(int argc, char *argv[]) {
  auto lines = load_data(argv[1]);

  std::cout << lines.size();

  for (auto line : lines) {
    for (auto station : line.stations) {
      std::cout << station.name << ' ' << station.time_to_next << '\n';
    }
    std::cout << '\n';
  }

  return 0;
}
