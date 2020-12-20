//
//  string-parser.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "string-parser.hpp"

void getASTFromTokens(std::vector<std::string> *tokens, AST *astTree) {
    for(size_t i = 0; i < (*tokens).size();) {
        auto ip = i;
        ASTBuilder(tokens, astTree, &ip);
        i = ip;
    }
}

void tokenizer(std::string input, std::vector<std::string> *tokens) {
    std::string token = "";
    bool inString = false;
    char quoteType = 0;
    for(size_t i = 0; i < input.size()+1; i++) {
        char c = i == input.size() ? 0 : input[i];
        bool termQuote = false;
        bool terminate = c == 0 || c == '\n';
        bool delim = !terminate && c == ' ';
        bool quotes = !terminate && !delim && (c == '\'' || c == '"');
        bool tokenizeBoth = !terminate && !delim && !quotes && (c == ';' || c == '=' || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/');
        if(quotes) {
            if(!inString) {
                quoteType = c;
                inString = true;
            } else if(c == quoteType) {
                termQuote = true;
                inString = false;
                quoteType = 0;
            }
        }
        if(terminate || termQuote || (!inString && (delim || tokenizeBoth))) {
            if(token.size()) {
                if(termQuote) {
                    token += c;
                }
                (*tokens).push_back(token);
                token.clear();
            }
            if(tokenizeBoth) {
                (*tokens).push_back(std::string(1,c));
            }
            continue;
        }
        token += c;
    }
}
