#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using uint = unsigned int;

class SqrtDecomposition {
public:
  SqrtDecomposition(std::vector<int>&& arr, size_t blocks_number, uint block_len)
      : arr_(std::move(arr)), blocks_(blocks_number), block_len_(block_len) {
    ;
  }

  void Init() {
    // Initialize zero counters in each blocks_
    for (uint i = 0; i < arr_.size(); ++i) {
      int& bck_zero_count = blocks_[i / block_len_];
      if (arr_[i] == 0) {
        ++bck_zero_count;
      }
    }
  }

  void Update(int index, int value) {
    if (arr_[index] == value) {  // Nothing changes
      return;
    }

    // Update the block:
    int& bck_zero_count = blocks_[index / block_len_];
    if (arr_[index] == 0 && value != 0) {
      --bck_zero_count;
    } else if (arr_[index] != 0 && value == 0) {
      ++bck_zero_count;
    }

    // Update the array:
    arr_[index] = value;
  }

  int Get(uint left, uint right, uint key) {
    uint zero_count = 0;

    // Process the leftmost partial block
    while (left <= right && (left % block_len_ != 0) && left != 0) {
      if (arr_[left] == 0 && ++zero_count == key) {
        return static_cast<int>(left) + 1;
      }
      ++left;
    }

    // Process full blocks in the middle
    uint cur_block = left / block_len_;
    while (left + block_len_ - 1 <= right && zero_count + blocks_[cur_block] < key) {
      zero_count += blocks_[cur_block];
      left += block_len_;
      cur_block = left / block_len_;
    }

    // Process the rightmost partial block
    while (left <= right) {
      if (arr_[left] == 0 && ++zero_count == key) {
        return static_cast<int>(left) + 1;
      }
      ++left;
    }

    return -1;
  }

private:
  std::vector<int> arr_;
  std::vector<int> blocks_;
  uint block_len_;
};

uint GetClosestPowerTwo(int value) {
  enum {
    PowerTwo8 = 256,
    PowerTwo7 = 128,
    PowerTwo6 = 64,
    PowerTwo5 = 32,
    PowerTwo4 = 16,
    PowerTwo3 = 8,
    PowerTwo2 = 4,
    PowerTwo1 = 2,
    PowerTwo0 = 1
  };

  if (value >= PowerTwo8) {
    return PowerTwo8;
  }
  if (value >= PowerTwo7) {
    return PowerTwo7;
  }
  if (value >= PowerTwo6) {
    return PowerTwo6;
  }
  if (value >= PowerTwo5) {
    return PowerTwo5;
  }
  if (value >= PowerTwo4) {
    return PowerTwo4;
  }
  if (value >= PowerTwo3) {
    return PowerTwo3;
  }
  if (value >= PowerTwo2) {
    return PowerTwo2;
  }
  if (value >= PowerTwo1) {
    return PowerTwo1;
  }
  return PowerTwo0;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  uint size = 0;
  std::cin >> size;

  std::vector<int> arr(size);
  int temp = 0;
  for (uint i = 0; i < size; ++i) {
    std::cin >> temp;
    arr[i] = temp;
  }
  uint block_len = GetClosestPowerTwo(static_cast<int>(std::sqrt(size)));
  uint blocks_amount = (size + block_len - 1) / block_len;
  SqrtDecomposition my_ds{std::move(arr), blocks_amount, block_len};
  my_ds.Init();

  int queries_number = 0;
  std::cin >> queries_number;
  for (int i = 0; i < queries_number; ++i) {
    char option = 0;
    std::cin >> option;

    if (option == 'u') {
      int index = 0;
      int value = 0;
      std::cin >> index >> value;
      my_ds.Update(index - 1, value);
    } else if (option == 's') {
      int left = 0;
      int right = 0;
      int key = 0;
      std::cin >> left >> right >> key;

      int result = my_ds.Get(left - 1, right - 1, key);
      std::cout << result << "\n";
    }
  }
  return 0;
}