#include <cstddef>
#include <iostream>
#include <string>

int main() {
  std::ios_base::sync_with_stdio(false);

  int tape_chars_count = 0;
  int strings_count = 0;
  std::cin >> tape_chars_count >> strings_count;

  std::string tape_string;
  std::cin >> tape_string;

  std::size_t cur_index = 0;
  for (int i = 0; i < strings_count; ++i) {
    std::string word;
    std::cin >> word;

    std::size_t found_word_index = tape_string.find(word, cur_index);
    if (found_word_index == std::string::npos) {  // didn't find such a word
      std::cout << "NO";
      return 0;
    }
    // else if the word is found:
    cur_index = found_word_index + word.size() + 1;
  }
  std::cout << "YES";
  return 0;
}