#ifndef HW6__REGEX_PARSER_H_
#define HW6__REGEX_PARSER_H_

#include <algorithm>
#include <cstddef>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>

struct Subword {
  size_t i, j; // start and end indexes, [i, j)

  size_t Len() const { return j - i; }

  size_t Empty() const { return i == j; }

  bool Concats(const Subword& other) const;

  Subword& operator+=(const Subword& other);
  Subword operator+(const Subword& other) const;
  bool operator==(const Subword& other) const;
  bool operator<(const Subword& other) const;
};

bool Subword::operator==(const Subword& other) const {
  if (Empty() && other.Empty()) {
    return true;
  }

  return (i == other.i) && (j == other.j);
}

bool Subword::operator<(const Subword& other) const {
  if (Empty()) {
    return !other.Empty();
  }

  if (other.Empty()) {
    return false;
  }

  return (i < other.i) || ((i == other.i) && (j < other.j));
}

bool Subword::Concats(const Subword& other) const {
  if (Empty() || other.Empty()) {
    return true;
  }

  return j == other.i;
}

Subword& Subword::operator+=(const Subword& other) {
  if (!Concats(other)) {
    throw std::runtime_error("Can't concat subwords (" + std::to_string(i) +
        ", " + std::to_string(j) + ") and (" + std::to_string(other.i) + ", "
                                 + std::to_string(other.j) + ")");
  }

  if (!other.Empty()) {
    if (Empty()) {
      i = other.i;
      j = other.j;
    } else {
      j = other.j;
    }
  }

  return *this;
}

Subword Subword::operator+(const Subword& other) const {
  Subword copy = *this;
  copy += other;

  return copy;
}

class RegexPart {
 public:
  RegexPart();
  RegexPart(char c, const std::string& word);

  RegexPart& Concat(const RegexPart& other);
  RegexPart& operator+=(const RegexPart& other);
  RegexPart& Star();

  size_t MaxSubwordLen() const;

  bool operator==(const RegexPart& other) const;
  bool operator!=(const RegexPart& other) const;

  static const char eps = '1';

 private:
  Subword max_subword;
  std::set<Subword> prefix_subwords; // all subwords of u that satisfy prefix of this regex
  std::set<Subword> suffix_subwords; // all subwords of u that satisfy suffix of this regex
  std::set<Subword> full_subwords; // all subwords of u that satisfy this regex fully
};

size_t RegexPart::MaxSubwordLen() const {
  return max_subword.Len();
}

RegexPart::RegexPart() {
  max_subword = {0, 0};
  prefix_subwords.insert({0, 0});
  suffix_subwords.insert({0, 0});
  full_subwords.insert({0, 0});
}

RegexPart::RegexPart(char c, const std::string& word) {
  max_subword = {0, 0};
  prefix_subwords.insert({0, 0});
  suffix_subwords.insert({0, 0});

  if (c == eps) {
    // c == epsilon
    full_subwords.insert({0, 0});
  } else {
    for (size_t i = 0; i < word.size(); ++i) {
      if (c == word.at(i)) {
        max_subword = {i, i + 1};
        prefix_subwords.insert({i, i + 1});
        suffix_subwords.insert({i, i + 1});
        full_subwords.insert({i, i + 1});
      }
    }
  }
}

RegexPart& RegexPart::Concat(const RegexPart& other) {
  // updating max subword

  if (other.max_subword.Len() > max_subword.Len()) {
    max_subword = other.max_subword;
  }

  for (const auto& ssw : suffix_subwords) {
    for (const auto& psw2 : other.prefix_subwords) {
      if (ssw.Concats(psw2)) {
        Subword sw = ssw + psw2;

        if (sw.Len() > max_subword.Len()) {
          max_subword = sw;
        }
      }
    }
  }

  // updating prefix subwords

  for (const auto& sw : full_subwords) {
    for (const auto& psw2 : other.prefix_subwords) {
      if (sw.Concats(psw2)) {
        prefix_subwords.insert(sw + psw2);
      }
    }
  }


  // updating suffix subwords

  std::set<Subword> new_suffix = other.suffix_subwords;

  for (const auto& sw2 : other.full_subwords) {
    for (const auto& ssw : suffix_subwords) {
      if (ssw.Concats(sw2)) {
        new_suffix.insert(ssw + sw2);
      }
    }
  }

  suffix_subwords = std::move(new_suffix);

  // updating full subwords

  std::set<Subword> new_full;

  for (const auto& fsw : full_subwords) {
    for (const auto& fsw2 : other.full_subwords) {
      if (fsw.Concats(fsw2)) {
        new_full.insert(fsw + fsw2);
      }
    }
  }

  full_subwords = std::move(new_full);

  return *this;
}

RegexPart& RegexPart::operator+=(const RegexPart& other) {
  if (other.max_subword.Len() > max_subword.Len()) {
    max_subword = other.max_subword;
  }

  prefix_subwords.insert(other.prefix_subwords.begin(), other.prefix_subwords.end());
  suffix_subwords.insert(other.suffix_subwords.begin(), other.suffix_subwords.end());
  full_subwords.insert(other.full_subwords.begin(), other.full_subwords.end());

  return *this;
}

RegexPart& RegexPart::Star() {
  Subword new_max = {0, 0};
  std::set<Subword> new_prefix;
  std::set<Subword> new_suffix;
  std::set<Subword> new_full;

  RegexPart last;
  RegexPart temp;

  do {
    last = temp;

    if (temp.max_subword.Len() > new_max.Len()) {
      new_max = temp.max_subword;
    }

    new_prefix.insert(temp.prefix_subwords.begin(), temp.prefix_subwords.end());
    new_suffix.insert(temp.suffix_subwords.begin(), temp.suffix_subwords.end());
    new_full.insert(temp.full_subwords.begin(), temp.full_subwords.end());

    temp.Concat(*this);
  } while (last != temp);

  max_subword = new_max;
  prefix_subwords = std::move(new_prefix);
  suffix_subwords = std::move(new_suffix);
  full_subwords = std::move(new_full);

  return *this;
}

bool RegexPart::operator==(const RegexPart& other) const {
  return (max_subword == other.max_subword) && (prefix_subwords == other.prefix_subwords)
      && (suffix_subwords == other.suffix_subwords) && (full_subwords == other.full_subwords);
}

bool RegexPart::operator!=(const RegexPart& other) const {
  return !(*this == other);
}

size_t MaxSubwordInLanguage(const std::string& regex, const std::string& word) {
  if (regex.empty()) {
    throw std::runtime_error("Bad regular expression: it can't be empty");
  }

  if (word.empty()) {
    return 0;
  }

  std::stack<RegexPart> stack;

  for (size_t i = 0; i < regex.size(); ++i) {
    char c = regex.at(i);

    if (c == '.') {
      if (stack.size() < 2) {
        throw std::runtime_error(
            "Bad regular expression: not enough operands for operator '.' at index " + std::to_string(i));
      }

      RegexPart b = stack.top();
      stack.pop();
      stack.top().Concat(b);
    } else if (c == '+') {
      if (stack.size() < 2) {
        throw std::runtime_error(
            "Bad regular expression: not enough operands for operator '+' at index " + std::to_string(i));
      }

      RegexPart b = stack.top();
      stack.pop();
      stack.top() += b;
    } else if (c == '*') {
      if (stack.empty()) {
        throw std::runtime_error(
            "Bad regular expression: not enough operands for operator '*' at index " + std::to_string(i));
      }

      stack.top().Star();
    } else {
      stack.emplace(c, word);
    }
  }

  if (stack.size() != 1) {
    throw std::runtime_error(
        "Bad regular expression: more than one operand at the end of parsing (not enough operators)");
  }

  return stack.top().MaxSubwordLen();
}

#endif //HW6__REGEX_PARSER_H_
