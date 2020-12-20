//
//  utils.hpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#ifndef utils_hpp
#define utils_hpp

#include <ostream>
#include <string>
#include <vector>

bool isInteger(std::string);
bool isNumber(std::string);
bool isValidIdentifier(std::string);
std::ostream &operator<<(std::ostream &, std::vector<std::string> &);

#endif /* utils_hpp */
