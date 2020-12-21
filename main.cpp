//
//  main.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include <iostream>
#include "string-parser.hpp"

int main(int argc, const char * argv[]) {
  try {
    std::string test = "number thisisMyVar=1+(2*3+5+(1+3/5)*3+((3)-1)+5)/7 + (2+1) * 3 + 5 / 7 / 7 + (8); number myOtherVar = 10 * 2; myOtherVar = 5 * 25; ";
    std::vector<std::string> tokens;
    AST *astTree = new AST(NodeType::ROOT);
    tokenizer(test, &tokens);
    /*
     for(auto it = tokens.begin(); it != tokens.end(); it++) {
     std::cout << *it << "\n";
     }*/
    getASTFromTokens(&tokens, astTree);
    runAST(astTree);
    //astTree->printData();
  } catch( const char* e ) {
    std::cout << "Exception: " << e << "\n";
  }
  return 0;
}
