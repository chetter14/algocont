#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using uint = unsigned int;

class SqrtDecomposition {
public:
  SqrtDecomposition(std::vector<int>&& arr, size_t blocks_number, uint block_len)
      : arr_(std::move(arr)), blocks_sum_(blocks_number), block_len_(block_len) {
    ;
  }

  void Init() {
    // Initialize total sum of numbers in each block
    for (uint i = 0; i < arr_.size(); ++i) {
      blocks_sum_[i / block_len_] += arr_[i];
    }
  }

  void SwapNumbersInRange(uint left, uint right) {
    while (left < right) {
      uint next = left + 1;
      if (left / block_len_ != next / block_len_) {  // in different blocks
        blocks_sum_[left / block_len_] += (arr_[next] - arr_[left]);
        blocks_sum_[next / block_len_] += (arr_[left] - arr_[next]);
      }
      std::swap(arr_[left], arr_[next]);
      left += 2;
    }
  }

  int GetRangeSum(uint left, uint right) {
    if (left == right) {
      return arr_[left];
    }

    int range_sum = 0;

    // Process the leftmost partial block
    while (left <= right && (left % block_len_ != 0) && left != 0) {
      range_sum += arr_[left];
      ++left;
    }

    // Process full blocks in the middle
    uint cur_block = left / block_len_;
    while (left + block_len_ - 1 <= right) {
      range_sum += blocks_sum_[cur_block];
      left += block_len_;
      cur_block = left / block_len_;
    }

    // Process the rightmost partial block
    while (left <= right) {
      range_sum += arr_[left];
      ++left;
    }

    return range_sum;
  }

private:
  std::vector<int> arr_;
  std::vector<int> blocks_sum_;
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

  uint suite_number = 1;

  while (true) {
    uint size = 0;
    uint requests_number = 0;
    std::cin >> size >> requests_number;

    if (size == 0 && requests_number == 0) {  // two 0s in the end of input
      return 0;
    }

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

    std::cout << "Suite " << suite_number << ":\n";
    ++suite_number;

    for (uint i = 0; i < requests_number; ++i) {
      int option = 0;
      int left_boundary = 0;
      int right_boundary = 0;
      std::cin >> option >> left_boundary >> right_boundary;

      if (option == 1) {
        my_ds.SwapNumbersInRange(left_boundary - 1, right_boundary - 1);
      } else if (option == 2) {
        int sum = my_ds.GetRangeSum(left_boundary - 1, right_boundary - 1);
        std::cout << sum << "\n";
      }
    }
    std::cout << "\n";
  }

  return 0;
}