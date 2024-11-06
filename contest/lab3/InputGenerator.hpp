#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <random>
#include <unordered_map>
#include <vector>

#include "ItemFeed.hpp"

constexpr int MaxId = 10, MaxScore = 10, MaxLimit = 16, MaxCommands = 5;

void CustomTestInOut() {
  youndex::express::ItemFeed feed;

  std::unordered_map<unsigned long, char> command_map{
      {1, 'i'},
      {2, 'u'},
      {3, 'r'},
      {4, 'p'},
      {5, 'g'}
  };

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<std::mt19937::result_type> command_gen(1, MaxCommands);
  std::uniform_int_distribution<std::mt19937::result_type> uid_gen(1, MaxId);
  std::uniform_int_distribution<std::mt19937::result_type> score_gen(1, MaxScore);
  std::uniform_int_distribution<std::mt19937::result_type> limit_gen(1, MaxLimit);

  using Item = youndex::express::Item;

  std::vector<Item> ideal_vector;
  auto sort_comparator = [&](const Item& lhs, const Item& rhs) {
    if (lhs.score != rhs.score) {
      return lhs.score < rhs.score;
    }
    return lhs.id > rhs.id;
  };

  constexpr int ActionsCount = 15;
  std::cout << "Started" << "\n";

  std::unordered_map<std::uint64_t, int> uid_counter;

  char command = 'i';
  for (std::size_t action = 0; action < ActionsCount; ++action) {
    switch (command) {
      case 'i': {  // i <id> <score>
        for (int i = 0; i < 2; ++i) {
          std::uint64_t uid = 0;
          do {
            uid = uid_gen(gen);
          } while (uid_counter[uid] != 0);
          uid_counter[uid] = 1;
          std::uint16_t score = score_gen(gen);

          feed.Add({.id = uid, .score = score});

          ideal_vector.emplace_back(uid, score);
          std::sort(ideal_vector.begin(), ideal_vector.end(), sort_comparator);

          std::cout << "\nAdded: " << uid << " " << score << "\n";
          feed.Print();
          std::cout << "\n";
          for (const auto& elem : ideal_vector) {
            std::cout << elem.id << "," << elem.score << " ";
          }
        }
      } break;
      case 'u': {
        for (int i = 0; i < 2; ++i) {
          std::uint64_t uid = 0;
          do {
            uid = uid_gen(gen);
          } while (uid_counter[uid] == 0);
          std::uint16_t score = score_gen(gen);

          feed.Update({.id = uid, .score = score});

          auto item = std::find_if(ideal_vector.begin(), ideal_vector.end(), [&](const Item& item) {
            return item.id == uid;
          });
          item->score = score;
          std::sort(ideal_vector.begin(), ideal_vector.end(), sort_comparator);

          std::cout << "\nUpdated: " << uid << " " << score << "\n";
          feed.Print();
          std::cout << "\n";
          for (const auto& elem : ideal_vector) {
            std::cout << elem.id << "," << elem.score << " ";
          }
        }
      } break;
      case 'r': {  // r <id>
        std::uint64_t uid = 0;
        do {
          uid = uid_gen(gen);
        } while (uid_counter[uid] == 0);
        uid_counter[uid] = 0;

        feed.Remove(uid);
        ideal_vector.erase(
            std::remove_if(
                ideal_vector.begin(),
                ideal_vector.end(),
                [&](const Item& item) { return item.id == uid; }
            ),
            ideal_vector.end()
        );

        std::cout << "\nRemoved: " << uid << "\n";
        feed.Print();
        std::cout << "\n";
        for (const auto& elem : ideal_vector) {
          std::cout << elem.id << "," << elem.score << " ";
        }
      } break;
      case 'p': {  // p <pos> <limit>
        std::uint64_t pos = (31 * action) % ideal_vector.size();
        std::size_t limit = limit_gen(gen);

        std::cout << "\nGetPos: " << pos << " " << limit << "\n";
        auto res = feed.GetAtPosition(pos, limit);
        for (auto& iter : res) {
          std::cout << iter << " ";
        }
        std::cout << "\n";
        for (std::uint16_t i = pos; i < limit && i < ideal_vector.size(); ++i) {
          std::cout << ideal_vector[i].id << " ";
        }
      } break;
      case 'g': {  // g <id> <limit>
        std::uint64_t uid = 0;
        do {
          uid = uid_gen(gen);
        } while (uid_counter[uid] == 0);
        std::size_t limit = limit_gen(gen);

        std::cout << "\nGetById: " << uid << " " << limit << "\n";
        auto res = feed.GetAtItemWithId(uid, limit);
        for (auto& iter : res) {
          std::cout << iter << " ";
        }
        std::cout << "\n";

        // Find index by uid in vector:
        unsigned int index = 0;
        for (index = 0; index < ideal_vector.size(); ++index) {
          if (ideal_vector[index].id == uid) {
            break;
          }
        }
        std::cout << "Id index - " << index << "\n";
        for (std::uint16_t i = index; i < limit && i < ideal_vector.size(); ++i) {
          std::cout << ideal_vector[i].id << " ";
        }
      } break;
      default: {
        assert(false);
      } break;
    }
    command = command_map[command_gen(gen)];
    if (ideal_vector.size() == 0 && command != 'i') {  // must be no remove or update
      command = 'i';
    }
    std::cout << std::endl;
    std::flush(std::cout);
  }
}