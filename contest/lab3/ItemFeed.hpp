#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace youndex::express {

constexpr int MaxSize = 2'000'000;

struct Item {
  std::uint64_t id;
  std::uint16_t score;
};

struct ItemComparator {
  bool operator()(const Item& lhs, const Item& rhs) const {
    if (lhs.score != rhs.score) {
      return lhs.score < rhs.score;
    }
    return lhs.id > rhs.id;
  }
};

/// NB: Is not thread safe.
class ItemFeed final {
public:
  ItemFeed() {
    items_vector_.reserve(MaxSize);
  }

  [[nodiscard]] std::vector<std::uint64_t> GetAtPosition(std::size_t position, std::size_t limit)
      const {
    assert(position < Size());
    assert(1 <= limit && limit <= 16);

    std::vector<std::uint64_t> result;
    result.reserve(limit * 2);
    for (size_t i = 0; i < limit && position + i < items_vector_.size(); ++i) {
      result.push_back(items_vector_[position + i].id);
    }
    return result;
  }

  [[nodiscard]] std::vector<std::uint64_t> GetAtItemWithId(std::uint64_t uid, std::size_t limit)
      const {
    assert(Contains(uid));
    assert(1 <= limit && limit <= 16);

    Item start_item = uid_to_item_object_map_.at(uid);
    auto set_iter = sorted_items_set_.find(start_item);

    std::vector<std::uint64_t> result;
    result.reserve(limit * 2);
    for (std::size_t i = 0; i < limit && set_iter != sorted_items_set_.end(); ++i, ++set_iter) {
      result.push_back((*set_iter).id);
    }
    return result;
  }

  void Add(Item item) {
    assert(!Contains(item.id));

    uid_to_item_object_map_[item.id] = item;
    sorted_items_set_.insert(item);
    RebuildVector();
  }

  void Update(Item item) {
    assert(Contains(item.id));

    if (uid_to_item_object_map_.at(item.id).score == item.score) {
      return;
    }

    sorted_items_set_.erase(uid_to_item_object_map_.at(item.id));
    uid_to_item_object_map_.at(item.id).score = item.score;
    sorted_items_set_.insert(uid_to_item_object_map_.at(item.id));
    RebuildVector();
  }

  void Remove(std::uint64_t uid) {
    assert(Contains(uid));

    Item item = uid_to_item_object_map_.at(uid);
    sorted_items_set_.erase(item);
    uid_to_item_object_map_.erase(uid);
    RebuildVector();
  }

  void Print() {
    for (const auto& item : sorted_items_set_) {
      std::cout << item.id << "," << item.score << " ";
    }
  }

private:
  [[nodiscard]] bool Contains(std::uint64_t uid) const {
    assert(1 <= uid);
    return uid_to_item_object_map_.count(uid) != 0;
  }

  [[nodiscard]] std::size_t Size() const {
    return items_vector_.size();
  }

  void RebuildVector() {
    items_vector_.clear();
    for (const auto& item : sorted_items_set_) {
      items_vector_.push_back(item);
    }
  }

  std::unordered_map<std::uint64_t, Item> uid_to_item_object_map_;  // for lookup by id
  std::set<Item, ItemComparator> sorted_items_set_;                 // for lookup by id
  std::vector<Item> items_vector_;                                  //  for lookup by position
};

}  // namespace youndex::express
