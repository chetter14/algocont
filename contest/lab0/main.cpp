#include <iostream>
#include <string>

int main() {
  int input_strings_number = 0;
  std::cin >> input_strings_number;

  std::string temp;
  for (int i = 0; i < input_strings_number; ++i) {
    std::cin >> temp;
    if (temp.size() % 2 != 0) {
      std::cout << "NO\n";
      continue;
    }

    size_t left = 0;
    for (size_t right = temp.size() / 2; left < temp.size() / 2; ++left, ++right) {
      if (temp[left] != temp[right]) {
        std::cout << "NO\n";
        break;
      }
    }

    if (left == temp.size() / 2) {
      std::cout << "YES\n";
    }
  }

  return 0;
}