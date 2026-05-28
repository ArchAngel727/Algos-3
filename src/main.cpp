#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Edge {
  std::string line;
  std::string target;
  int weight;
};

struct Previous {
  std::string station;
  std::string line;
};

std::vector<std::string> parse(const char *buffer) {
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
      continue;
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
    items = parse(str.c_str());

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
            Edge{.line = items[0], .target = items[i + 2], .weight = ttn});
      }

      if (i > 2) {
        umap[items[i]].emplace_back(
            Edge{.line = items[0], .target = items[i - 2], .weight = ttp});
      }
    }
  }

  return umap;
}

std::pair<std::unordered_map<std::string, Previous>,
          std::unordered_map<std::string, int>>
djikstar(const std::string &start, const std::string &end,
         const std::unordered_map<std::string, std::vector<Edge>> &graph) {
  std::unordered_map<std::string, int> distance;
  std::unordered_map<std::string, Previous> previous;
  std::priority_queue<std::pair<int, std::string>,
                      std::vector<std::pair<int, std::string>>, std::greater<>>
      pq;

  for (auto pair : graph) {
    distance[pair.first] = INT_MAX;
  }
  distance[start] = 0;

  pq.push({0, start});

  while (!pq.empty()) {
    auto [dist, station] = pq.top();
    pq.pop();

    if (dist > distance[station]) {
      continue;
    }

    if (station == end) {
      break;
    }

    for (const Edge &edge : graph.at(station)) {
      int new_dist = dist + edge.weight;

      if (new_dist < distance[edge.target]) {
        distance[edge.target] = new_dist;
        previous[edge.target] = {station, edge.line};
        pq.push({new_dist, edge.target});
      }
    }
  }

  return {previous, distance};
}

void print_path(const std::string &start, const std::string &end,
                const std::unordered_map<std::string, Previous> &previous,
                const std::unordered_map<std::string, int> &distance) {
  std::vector<std::pair<std::string, std::string>> path;
  std::string current = end;
  std::string current_line = "";

  while (current != start) {
    const auto &info = previous.at(current);
    path.push_back({current, info.line});
    current = previous.at(current).station;
  }
  path.push_back({start, ""});

  std::reverse(path.begin(), path.end());

  if (path.size() < 2) {
    std::cout << path[0].first;
    return;
  }

  for (size_t i = 0; i < path.size(); ++i) {
    const auto &[station, line] = path[i];

    if (i == 0) {
      continue;
    } else if (i == 1) {
      current_line = line;
      std::cout << line << ": " << path[0].first << " -> " << station;
    } else if (line != current_line) {
      std::cout << "\nUmsteigen in " << line << '\n';
      std::cout << line << ": " << path[i - 1].first << " -> " << station;
      current_line = line;
    } else {
      std::cout << " -> " << station;
    }
  }

  std::cout << "\nTotal: " << distance.at(end);
}

int main(int argc, char *argv[]) {
  std::unordered_map<std::string, std::vector<Edge>> graph;

  if (argc < 3) {
    std::cout << "Invalid arguments\n"
              << "find_path <file> <start> <end>";
  }

  graph = load_data(argv[1]);
  std::string start(argv[2]);
  std::string end(argv[3]);

  if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
    std::cerr << "Invalid stations";
    return -1;
  }

  auto [prev, dist] = djikstar(start, end, graph);

  print_path(start, end, prev, dist);

  return 0;
}
