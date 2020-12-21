//
//  utils.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "utils.hpp"

bool isInteger(std::string s) {
  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
  char* p;
  strtol(s.c_str(), &p, 10);
  return *p == 0;
} 

bool isNumber(std::string s) {
  char cleaned[s.length()];
  bool rmComma = true;
  size_t j = 0;
  for(size_t i = 0; i < s.length(); i++) {
    if(rmComma && s[i] == '.') {
      rmComma = false;
    } else {
      cleaned[j++] = s[i];
    }
  }
  return isInteger(std::string(cleaned, j));
}

bool isValidIdentifier(std::string s) {
  if(s.empty() || static_cast<int>(s[0]) < 65) {
    return false;
  }
  for(size_t i = 0; i < s.length(); i++) {
    int c = static_cast<int>(s[i]);
    if(c == 95) {
      //Ignore underlines
      continue;
    }
    if(c < 48) {
      return false;
    }
    if(c > 57 && c < 65) {
      return false;
    }
    if(c > 90 && c < 97) {
      return false;
    }
    if(c > 122) {
      return false;
    }
  }
  return true;
}

std::ostream &operator<<(std::ostream &o, std::vector<std::string> &v) {
  for(auto it = v.begin(); it != v.end(); it++) {
    o << *it << std::endl;
  }
  return o;
}
