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
        std::string test = "number thisisMyVar=1+2+3;";
        std::vector<std::string> tokens;
        AST *astTree = new AST(NodeType::ROOT);
        tokenizer(test, &tokens);
        /*
        for(auto it = tokens.begin(); it != tokens.end(); it++) {
            std::cout << *it << "\n";
        }*/
        getASTFromTokens(&tokens, astTree);
        astTree->printData();
    } catch( const char* e ) {
        std::cout << "Exception: " << e;
    }
    return 0;
}
