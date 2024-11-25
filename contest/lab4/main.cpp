#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using uint = unsigned int;

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

void SwapNumbersInRange(
    uint left, uint right, uint block_len, std::vector<int>& blocks_sum, std::vector<int>& arr
) {
  while (left < right) {
    uint next = left + 1;
    if (left / block_len != next / block_len) {  // in different blocks
      blocks_sum[left / block_len] += (arr[next] - arr[left]);
      blocks_sum[next / block_len] += (arr[left] - arr[next]);
    }
    std::swap(arr[left], arr[next]);
    left += 2;
  }
}

int GetRangeSum(
    uint left,
    uint right,
    uint block_len,
    const std::vector<int>& blocks_sum,
    const std::vector<int>& arr
) {
  if (left == right) {
    return arr[left];
  }

  int range_sum = 0;

  // Process the leftmost partial block
  while (left <= right && (left % block_len != 0) && left != 0) {
    range_sum += arr[left];
    ++left;
  }

  // Process full blocks in the middle
  uint cur_block = left / block_len;
  while (left + block_len - 1 <= right) {
    range_sum += blocks_sum[cur_block];
    left += block_len;
    cur_block = left / block_len;
  }

  // Process the rightmost partial block
  while (left <= right) {
    range_sum += arr[left];
    ++left;
  }

  return range_sum;
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
    std::vector<int> blocks_sum(blocks_amount);

    // Initialize total sum of numbers in each block
    for (uint i = 0; i < arr.size(); ++i) {
      blocks_sum[i / block_len] += arr[i];
    }

    std::cout << "Suite " << suite_number << ":\n";
    ++suite_number;

    for (uint i = 0; i < requests_number; ++i) {
      int option = 0;
      int left_boundary = 0;
      int right_boundary = 0;
      std::cin >> option >> left_boundary >> right_boundary;

      if (option == 1) {
        SwapNumbersInRange(left_boundary - 1, right_boundary - 1, block_len, blocks_sum, arr);
      } else if (option == 2) {
        int sum = GetRangeSum(left_boundary - 1, right_boundary - 1, block_len, blocks_sum, arr);
        std::cout << sum << "\n";
      }
    }
    std::cout << "\n";
  }

  return 0;
}