//
//  ASTBuilder.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "ASTBuilder.hpp"

enum class TokenType {
    TERM_NUMBER,
    TERM_STRING,
    OPERATOR_EQUAL,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MUL,
    OPERATOR_DIV,
    OPERATOR_PAREN_L,
    OPERATOR_PAREN_R,
    LITERAL_NUMBER,
    LITERAL_STRING,
    IDENTIFIER,
    UNKNOWN
};

TokenType getTokenType(std::string s) {
    if(s == "number") {
        return TokenType::TERM_NUMBER;
    }
    if(s == "string") {
        return TokenType::TERM_STRING;
    }
    if(s == "=") {
        return TokenType::OPERATOR_EQUAL;
    }
    if(s == "+") {
        return TokenType::OPERATOR_PLUS;
    }
    if(s == "-") {
        return TokenType::OPERATOR_MINUS;
    }
    if(s == "*") {
        return TokenType::OPERATOR_MUL;
    }
    if(s == "/") {
        return TokenType::OPERATOR_DIV;
    }
    if(s == "(") {
        return TokenType::OPERATOR_PAREN_L;
    }
    if(s == ")") {
        return TokenType::OPERATOR_PAREN_R;
    }
    if(isNumber(s)) {
        return TokenType::LITERAL_NUMBER;
    }
    if((s[0] == '\"' || s[0] == '\'') && s[0] == s[s.length()-1]) {
        return TokenType::LITERAL_STRING;
    }
    if(isValidIdentifier(s)) {
        return TokenType::IDENTIFIER;
    }
    return TokenType::UNKNOWN;
}


void processToken(ASTBuilderContext *context, size_t *endPtr) {
    const std::string token = context->getToken(*endPtr);
    const TokenType tokenType = getTokenType(token);
    switch(tokenType) {
        case TokenType::TERM_NUMBER: {
            if(!context->active) {
                std::string nextToken = context->getToken(++(*endPtr));
                if(isValidIdentifier(nextToken)) {
                    AST *node = new AST(NodeType::VARIABLE_DECLARATION);
                    node->setTargetName(nextToken);
                    context->ast->addChild(node);
                } else {
                    throw "Invalid identifier for variable declaration";
                }
            } else {
                throw "Unexpected term: \"number\"";
            }
            break;
        }
        case TokenType::TERM_STRING: {
            break;
        }
        case TokenType::OPERATOR_EQUAL: {
            AST *active = context->active;
            if(!active) {
                AST *prev = context->ast->getPrevChild();
                if(prev && prev->getNodeType() == NodeType::VARIABLE_DECLARATION) {
                    AST *node = new AST(NodeType::VARIABLE_ASSIGNMENT);
                    node->setTargetName(prev->getTargetName());
                    context->active = node;
                } else {
                    throw "Unexpected assignment operator";
                }
            } else {
                if(active->getNodeType() == NodeType::IDENTIFIER) {
                    active->setNodeType(NodeType::VARIABLE_ASSIGNMENT);
                } else {
                    throw "Unexpected assignment operator";
                }
            }
            break;
        }
        case TokenType::OPERATOR_PLUS: {
            break;
        }
        case TokenType::OPERATOR_MINUS: {
            break;
        }
        case TokenType::OPERATOR_MUL: {
            break;
        }
        case TokenType::OPERATOR_DIV: {
            break;
        }
        case TokenType::OPERATOR_PAREN_L: {
            break;
        }
        case TokenType::OPERATOR_PAREN_R: {
            break;
        }
        case TokenType::LITERAL_NUMBER: {
            if(context->active) {
                context->active->setValue(token);
                context->ast->addChild(context->active);
                context->active = NULL;
            } else {
                throw "Unexpected number";
            }
            break;
        }
        case TokenType::LITERAL_STRING: {
            break;
        }
        case TokenType::IDENTIFIER: {
            if(!context->active) {
                AST *node = new AST(NodeType::IDENTIFIER);
                node->setTargetName(token);
                context->active = node;
            } else {
                throw "Unexpected identifier";
            }
        }
        case TokenType::UNKNOWN: {
            throw "Unknown token";
        }
    }
    (*endPtr)++;
    
    if(context->active) {
        if(*endPtr < context->tokens->size()) {
            processToken(context, endPtr);
        } else {
            delete context->active;
            context->active = NULL;
            throw "Incomplete statement";
        }
    }
}

void ASTBuilder(std::vector<std::string> *tokens, AST *ast, size_t *endPtr, int startIdx ) {
    if(!ast) {
        throw "Received null AST into builder";
    }
    ASTBuilderContext context = {
        .tokens = tokens,
        .ast = ast,
        .active = NULL,
        .startIdx = startIdx > -1? startIdx : *endPtr,
        .endPtr = endPtr
    };
    processToken(&context, endPtr);
}
