//
//  ASTBuilder.hpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#ifndef ASTBuilder_hpp
#define ASTBuilder_hpp

#include <vector>
#include <string>
#include "AST.hpp"
#include "utils.hpp"

struct ASTBuilderContext {
  std::vector<std::string> *tokens;
  AST *ast;
  AST *active;
  size_t startIdx;
  size_t *endPtr;
  std::string getToken(size_t idx) {
    if(idx < 0 || idx >= (*tokens).size()) {
      return "";
    }
    return (*tokens)[idx];
  }
};

void ASTBuilder(std::vector<std::string> *, AST *, size_t *, int = -1);

#endif /* ASTBuilder_hpp */
