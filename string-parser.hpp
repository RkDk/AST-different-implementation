//
//  string-parser.hpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#ifndef string_parser_hpp
#define string_parser_hpp

#include "AST.hpp"
#include <vector>
#include <string>

void parseExpression(std::vector<std::string> *, int, int *);
void buildAST(std::vector<std::string> *, AST *);
void tokenizer(std::string, std::vector<std::string> *);

#endif /* string_parser_hpp */
