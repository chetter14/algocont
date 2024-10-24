#include <bitset>
#include <iostream>

constexpr int BitsSize = 1e6;

class Number {
public:
  int Add(int two_power) {
    if (bits_[two_power]) {  // bits_[S] is already set to 1
      int index = two_power;
      bits_[index] = false;  // set bits_[S] to 0 (because 1 + 1 = 0, set carry bit)
      ++index;

      // Handle bubbling of carry bit up to the first-encountered zero:
      while (bits_[index]) {   // if bits_[index] is 1
        bits_[index] = false;  // set bits_[index] to 0
        ++index;
      }
      bits_[index] = true;  // set bits_[index] from 0 to 1
      ones_count_ = ones_count_ - (index - two_power - 1);
    } else {
      bits_[two_power] = true;  // set bits_[S] to 1
      ++ones_count_;
    }
    return ones_count_;
  }

  int Subtract(int two_power) {
    if (bits_[two_power]) {      // if bits_[S] is set to 1
      bits_[two_power] = false;  // set bits_[S] to 0
      --ones_count_;
    } else {
      int index = two_power;
      bits_[index] = true;  // set bits_[S] to 1 (because 0 - 1 = 1, set borrow bit)
      ++index;

      // Handle bubbling of borrow bit up to the first-encountered one:
      while (!bits_[index]) {  // if bits_[index] is 0
        bits_[index] = true;   // set bits_[index] to 1
        ++index;
      }
      bits_[index] = false;  // set bits_[index] from 1 to 0
      ones_count_ = ones_count_ + (index - two_power - 1);
    }
    return ones_count_;
  }

private:
  std::bitset<BitsSize> bits_;
  int ones_count_ = 0;
};

int main() {
  int operations_number = 0;
  std::cin >> operations_number;

  Number my_numb;
  for (int i = 0; i < operations_number; ++i) {
    char sign = 0;
    int power = 0;
    std::cin >> sign >> power;

    if (sign == '+') {
      std::cout << my_numb.Add(power) << "\n";
    } else if (sign == '-') {
      std::cout << my_numb.Subtract(power) << "\n";
    }
  }

  return 0;
}