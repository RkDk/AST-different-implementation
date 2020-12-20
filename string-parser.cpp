//
//  string-parser.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "string-parser.hpp"

float evaluateExpression(const AST *ast) {
    if(ast->getChildCount() == 0) {
        if(ast->getNodeType() == NodeType::NUMBER) {
            return ast->getNumberValue();
        }
        return 0.0f;
    }
    if(ast->getNodeType() == NodeType::EXPRESSION) {
        return evaluateExpression(ast->getChild(0));
    }
    if(ast->getNodeType() == NodeType::OPERATOR_PLUS) {
        float lhs = evaluateExpression(ast->getChild(0));
        float rhs = evaluateExpression(ast->getChild(1));
        return lhs+rhs;
    }
    if(ast->getNodeType() == NodeType::OPERATOR_MINUS) {
        float lhs = evaluateExpression(ast->getChild(0));
        float rhs = evaluateExpression(ast->getChild(1));
        return lhs-rhs;
    }
    if(ast->getNodeType() == NodeType::OPERATOR_MUL) {
        float lhs = evaluateExpression(ast->getChild(0));
        float rhs = evaluateExpression(ast->getChild(1));
        return lhs*rhs;
    }
    if(ast->getNodeType() == NodeType::OPERATOR_DIV) {
        float lhs = evaluateExpression(ast->getChild(0));
        float rhs = evaluateExpression(ast->getChild(1));
        return lhs/rhs;
    }
    return 0.0f;
}

void runAST(AST *ast) {
    const AST *cur = ast;
    const NodeType nodeType = cur->getNodeType();
    switch(nodeType) {
        case NodeType::ROOT: {
            std::cout << "Entering root document\n";
            break;
        }
        case NodeType::VARIABLE_DECLARATION: {
            const std::string targetName = cur->getTargetName();
            const AST *child = cur->getChild(0);
            if(child) {
                float value = evaluateExpression(child);
                std::cout << "Declaring variable \"" << targetName << "\" with initial value: " << value << "\n";
            } else {
                std::cout << "Declaring variable \"" << targetName << "\"\n";
            }
            break;
        }
        case NodeType::VARIABLE_ASSIGNMENT: {
            const std::string targetName = cur->getTargetName();
            const AST *child = cur->getChild(0);
            if(child) {
                float value = evaluateExpression(child);
                std::cout << "Assigning variable \"" << targetName << "\" value: " << value << "\n";
            }
            break;
        }
        default: {
            return;
        }
    }
    for(size_t i = 0; i < cur->getChildCount(); i++) {
        runAST(cur->getChild(i));
    }
}

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
