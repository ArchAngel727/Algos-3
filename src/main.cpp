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
  int ttn = 0;
  size_t window_size = 3;

  // Sliding window inspired
  while (std::getline(infile, str)) {
    items = split(str.c_str());
    std::vector<Edge> edges;

    for (size_t i = 1; i < items.size() - window_size; i += 2) {
      if (i + 1 == items.size()) {
        ttn = 0;
      } else {
        try {
          ttn = std::stoi(items[i + 1]);
        } catch (std::exception &) {
        }
      }

      edges.emplace_back(
          Edge{.line = items[0], .taget = items[i], .weight = ttn});
    }

    umap.insert({items[0], edges});
    break;
  }

  return umap;
}

int main(int argc, char *argv[]) {
  std::unordered_map<std::string, std::vector<Edge>> umap;

  if (argc >= 1) {
    umap = load_data(argv[1]);
  }

  for (auto idk : umap) {
    std::cout << idk.first << ":\n";

    for (auto edge : idk.second) {
      std::cout << edge.line << ' ' << edge.taget << ' ' << edge.weight << '\n';
    }
  }

  return 0;
}
