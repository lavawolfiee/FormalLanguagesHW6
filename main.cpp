#include <iostream>
#include "regex_parser.h"

int main() {
  std::string regex;
  std::string word;

  std::cout << "Enter regex: " << std::flush;
  std::cin >> regex;

  std::cout << "Enter word u: " << std::flush;
  std::cin >> word;

  std::cout << "Answer: " << MaxSubwordInLanguage(regex, word) << std::endl;

  return 0;
}