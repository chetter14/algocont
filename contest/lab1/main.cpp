#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

struct Block {
  unsigned int l = 0;
  unsigned int r = 0;
  unsigned int zero_count = 0;
};

class SqrtDecomposition {
public:
  SqrtDecomposition(std::vector<int>&& arr, size_t blocks_number, unsigned int block_len) : arr_(std::move(arr)), blocks_(blocks_number), block_len_(block_len) { ; }

  void Init() {
    // Initialize l and r indices:
    for (unsigned int i = 0; i < arr_.size(); i += block_len_) {
      Block& bck = blocks_[i / block_len_];
      bck.l = i;
      bck.r = std::min(i + block_len_ - 1, static_cast<unsigned int>(arr_.size() - 1));
    }

    // Initialize zero counters in each blocks_
    for (int i = 0; i < arr_.size(); ++i) {
      Block& bck = blocks_[i / block_len_];
      if (arr_[i] == 0) {
        ++bck.zero_count;
      }
    }
  }

  void Update(int index, int value) {
    // Update the array:
    arr_[index] = value;

    // Update the block:
    Block& bck = blocks_[index / block_len_];

    unsigned int zero_count = 0;
    for (unsigned int i = bck.l; i <= bck.r; ++i) {
      if (arr_[i] == 0) {
        ++zero_count;
      }
    }
    bck.zero_count = zero_count;
  }

  unsigned int Get(int left, int right, int key) {
    unsigned int zero_count = 0;
    int iter = 0;
    for (unsigned int i = left; i < right; ++iter) {
      unsigned int cur_zero_count = zero_count + blocks_[i / block_len_].zero_count;
      if (i % block_len_ == 0 && i + block_len_ - 1 <= right && cur_zero_count < key) {
        zero_count = cur_zero_count;
        i += block_len_;
      }
      else {
        if (arr_[i] == 0) {
          ++zero_count;
        }
        if (zero_count == key) {
          return i;
        }
        ++i;
      }
    }
    return -1;
  }

private:
  std::vector<int> arr_;
  std::vector<Block> blocks_;
  unsigned int block_len_;
};

/*

API:
    1) Update(int index, int value)
    2) Get(int l, int r, int k)

Update(int index, int value):
    1) arr_[index] = value;
    2) update_block(blocks_[index / block_len_])

update_block(block* b):                 just update zero_count variable
    unsigned int zero_count = 0;
    for (int i = b->l; i <= b->r; ++i)
    {
        if (arr_[i] == 0)
            ++zero_count;
    }
    b->zero_count = zero_count;

blocks_[], block_len_, blocks_num

Get(int l, int r, int k):
    int zero_count = 0;
    for (int i = l; i <= r; )
    {
        int cur_zero_count = zero_count + blocks_[block_num]->zero_count;
        block_num = i / block_len_;
        if (i % block_len_ == 0 && i + block_len_ - 1 <= r && cur_zero_count < k ) {
            zero_count = cur_zero_count;
            i += block_len_;
        }
        else {
            if (arr_[i] == 0)
                ++zero_count;
            if (zero_count == k)
                return i;
            ++i;
        }
    }
    return -1;  *not found*

*/

int main() {
  int size = 0;
  std::cin >> size;

  std::vector<int> arr(size);
  int temp = 0;
  for (int i = 0; i < size; ++i) {
    std::cin >> temp;
    arr[i] = temp;
  }
  unsigned int block_len = static_cast<int>(std::sqrt(size)) + 1;
  unsigned int blocks_amount = block_len;
  
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
      my_ds.Update(index, value);
    }
    else if (option == 's') {
      int left = 0;
      int right = 0;
      int key = 0;
      std::cin >> left >> right >> key;

      unsigned int result = my_ds.Get(left, right, key);
      std::cout << result << "\n";
    }
  }

  return 0;
}