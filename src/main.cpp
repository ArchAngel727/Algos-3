#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Edge {
  std::string line;
  std::string taget;
  int weight;
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
      buffer += 1;
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

std::unordered_map<std::string, std::vector<Edge>> load_data(const char *path) {
  std::ifstream infile(path);
  std::string line;
  std::string str;
  std::vector<std::string> items;

  std::unordered_map<std::string, std::vector<Edge>> umap;
  int ttn = 0, ttp = 0;

  // Sliding window inspired
  while (std::getline(infile, str)) {
    items = split(str.c_str());

    for (size_t i = 1; i < items.size(); i += 2) {
      std::vector<Edge> edges;

      if (i + 1 < items.size()) {
        try {
          ttn = std::stoi(items[i + 1]);
        } catch (std::exception &) {
        }
      }

      if (i > 0) {
        try {
          ttp = std::stoi(items[i - 1]);
        } catch (std::exception &) {
        }
      }

      if (i + 3 <= items.size()) {
        umap[items[i]].emplace_back(
            Edge{.line = items[0], .taget = items[i + 2], .weight = ttn});
      }

      if (i > 2) {
        umap[items[i]].emplace_back(
            Edge{.line = items[0], .taget = items[i - 2], .weight = ttp});
      }
    }

    break;
  }

  return umap;
}

int main(int argc, char *argv[]) {
  std::unordered_map<std::string, std::vector<Edge>> umap;

  if (argc < 3) {
    std::cout << "Invalid arguments\n"
              << "find_path <file> <start> <end>";
  }

  umap = load_data(argv[1]);

  std::cout << "key 1: " << argv[2] << '\n';
  std::cout << "key 2: " << argv[3] << '\n';

  for (auto edge : umap) {
    std::cout << edge.first << ":\n";

    for (auto edge : edge.second) {
      std::cout << "  " << edge.line << ' ' << edge.taget << ' ' << edge.weight
                << '\n';
    }

    // std::cout << '\n';
  }

  return 0;
}
