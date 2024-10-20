#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using uint = unsigned int;

struct Block {
  uint l = 0;
  uint r = 0;
  uint zero_count = 0;
};

class SqrtDecomposition {
public:
  SqrtDecomposition(std::vector<int>&& arr, size_t blocks_number, uint block_len)
      : arr_(std::move(arr)), blocks_(blocks_number), block_len_(block_len) {
    ;
  }

  void Init() {
    // Initialize l and r indices:
    for (uint i = 0; i < arr_.size(); i += block_len_) {
      Block& bck = blocks_[i / block_len_];
      bck.l = i;
      bck.r = std::min(i + block_len_ - 1, static_cast<uint>(arr_.size() - 1));
    }

    // Initialize zero counters in each blocks_
    for (uint i = 0; i < arr_.size(); ++i) {
      Block& bck = blocks_[i / block_len_];
      if (arr_[i] == 0) {
        ++bck.zero_count;
      }
    }
  }

  void Update(int index, int value) {
    // Update the block:
    Block& bck = blocks_[index / block_len_];
    if (arr_[index] == 0 && value != 0) {
      --bck.zero_count;
    } else if (arr_[index] != 0 && value == 0) {
      ++bck.zero_count;
    }

    // Update the array:
    arr_[index] = value;
  }

  int Get(uint left, uint right, uint key) {
    uint zero_count = 0;
    uint block_start = left / block_len_;
    uint block_end = right / block_len_;

    if (block_start ==
        block_end) {  // If the range is within a single block, fall back to element-wise counting
      for (uint i = left; i <= right; ++i) {
        if (arr_[i] == 0) {
          ++zero_count;
        }
        if (zero_count == key) {
          return static_cast<int>(i) + 1;
        }
      }
    } else {
      // Partial first block
      uint first_block_end = (block_start + 1) * block_len_ - 1;
      for (uint i = left; i <= first_block_end; ++i) {
        if (arr_[i] == 0) {
          ++zero_count;
        }
        if (zero_count == key) {
          return static_cast<int>(i) + 1;
        }
      }

      // Full blocks in between
      for (uint cur_bck = block_start + 1; cur_bck < block_end; ++cur_bck) {
        uint cur_zero_count = blocks_[cur_bck].zero_count;
        if (zero_count + cur_zero_count >=
            key) {  // If the k-th zero is within this block, fall back to element-wise search
          uint block_begin = cur_bck * block_len_;
          uint block_end = block_begin + block_len_ - 1;
          for (uint i = block_begin; i <= block_end; ++i) {
            if (arr_[i] == 0) {
              ++zero_count;
            }
            if (zero_count == key) {
              return static_cast<int>(i) + 1;
            }
          }
        }
        zero_count += cur_zero_count;
      }

      // Partial last block
      uint last_block_begin = block_end * block_len_;
      for (uint i = last_block_begin; i <= right; ++i) {
        if (arr_[i] == 0) {
          ++zero_count;
        }
        if (zero_count == key) {
          return static_cast<int>(i) + 1;
        }
      }
    }

    return -1;

    // uint zero_count = 0;
    // for (uint i = left; i <= right;) {
    //   uint cur_zero_count = zero_count + blocks_[i / block_len_].zero_count;
    //   if (i % block_len_ == 0 && i + block_len_ - 1 <= right && cur_zero_count < key) {
    //     zero_count = cur_zero_count;
    //     i += block_len_;
    //   } else {
    //     if (arr_[i] == 0) {
    //       ++zero_count;
    //     }
    //     if (zero_count == key) {
    //       return static_cast<int>(i) + 1;  // i + 1 - index appropriate for user
    //     }
    //     ++i;
    //   }
    // }
    // return -1;
  }

private:
  std::vector<int> arr_;
  std::vector<Block> blocks_;
  uint block_len_;
};

int main() {
  int size = 0;
  std::cin >> size;

  std::vector<int> arr(size);
  int temp = 0;
  for (int i = 0; i < size; ++i) {
    std::cin >> temp;
    arr[i] = temp;
  }
  uint block_len = static_cast<int>(std::sqrt(size)) + 1;
  uint blocks_amount = block_len;
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