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
    SEMICOLON,
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
    if(s == ";") {
        return TokenType::SEMICOLON;
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
                    context->active = node;
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
        case TokenType::SEMICOLON: {
            if(context->active) {
                AST *parent = context->active;
                bool openParen = parent->getHasOpenParen();
                while(parent->getParent()) {
                    parent = parent->getParent();
                    openParen = openParen || parent->getHasOpenParen();
                }
                if(openParen) {
                    throw "Expression has unclosed paranthesis";
                }
                context->ast->addChild(parent);
                context->active = NULL;
            } else {
                throw "Unexpected semicolon";
            }
            break;
        }
        case TokenType::OPERATOR_EQUAL: {
            AST *active = context->active;
            if(!active) {
                throw "Unexpected assignment operator";
            } else {
                if(active->getNodeType() == NodeType::IDENTIFIER) {
                    active->setNodeType(NodeType::VARIABLE_ASSIGNMENT);
                    AST *node = new AST(NodeType::EXPRESSION);
                    active->addChild(node);
                    context->active = node;
                } else if(active->getNodeType() == NodeType::VARIABLE_DECLARATION) {
                    AST *node = new AST(NodeType::EXPRESSION);
                    active->addChild(node);
                    context->active = node;
                } else {
                    throw "Unexpected assignment operator";
                }
            }
            break;
        }
        case TokenType::OPERATOR_PLUS:
        case TokenType::OPERATOR_MINUS: {
            AST *active = context->active;
            if(active) {
                NodeType nodeType = active->getNodeType();
                if(nodeType == NodeType::EXPRESSION) {
                    active->setNodeType(tokenType == TokenType::OPERATOR_PLUS? NodeType::OPERATOR_PLUS : NodeType::OPERATOR_MINUS);
                }
                else if(active->getChildCount() == 0 && (nodeType == NodeType::NUMBER || nodeType == NodeType::IDENTIFIER)) {
                    active->setNodeType(tokenType == TokenType::OPERATOR_PLUS? NodeType::OPERATOR_PLUS : NodeType::OPERATOR_MINUS);
                    AST *lhs = new AST(nodeType);
                    lhs->setValue(active->getStringValue());
                    active->clearValue();
                    active->addChild(lhs);
                } else if(active->getChildCount() == 2) {
                    AST *oldParent = active->getParent();
                    AST *newParent = new AST(tokenType == TokenType::OPERATOR_PLUS? NodeType::OPERATOR_PLUS : NodeType::OPERATOR_MINUS);
                    if(oldParent) {
                        oldParent->swapChild(active, newParent);
                    }
                    newParent->addChild(active);
                    context->active = newParent;
                } else {
                    throw "Unexpected math operator";
                }
            } else {
                throw "Unexpected math operator";
            }
            break;
        }
        case TokenType::OPERATOR_MUL:
        case TokenType::OPERATOR_DIV: {
            AST *active = context->active;
            if(active) {
                NodeType nodeType = active->getNodeType();
                if(nodeType == NodeType::EXPRESSION) {
                    active->setNodeType(tokenType == TokenType::OPERATOR_MUL? NodeType::OPERATOR_MUL : NodeType::OPERATOR_DIV);
                }
                else if(active->getChildCount() == 0 && (nodeType == NodeType::NUMBER || nodeType == NodeType::IDENTIFIER)) {
                    active->setNodeType(tokenType == TokenType::OPERATOR_MUL? NodeType::OPERATOR_MUL : NodeType::OPERATOR_DIV);
                    AST *lhs = new AST(nodeType);
                    lhs->setValue(active->getStringValue());
                    active->clearValue();
                    active->addChild(lhs);
                } else if(active->getChildCount() == 2) {
                    if(active->getNodeType() == NodeType::OPERATOR_PLUS ||active->getNodeType() == NodeType::OPERATOR_MINUS) {
                        AST *child = active->getChild(1);
                        AST *oldParent = active;
                        AST *newParent = new AST(tokenType == TokenType::OPERATOR_MUL? NodeType::OPERATOR_MUL : NodeType::OPERATOR_DIV);
                        if(oldParent) {
                            oldParent->swapChild(child, newParent);
                        }
                        newParent->addChild(child);
                        context->active = newParent;
                    } else {
                        AST *oldParent = active->getParent();
                        AST *newParent = new AST(tokenType == TokenType::OPERATOR_MUL? NodeType::OPERATOR_MUL : NodeType::OPERATOR_DIV);
                        if(oldParent) {
                            oldParent->swapChild(active, newParent);
                        }
                        newParent->addChild(active);
                        context->active = newParent;
                    }
                } else {
                    throw "Unexpected math operator";
                }
            } else {
                throw "Unexpected math operator";
            }
            break;
        }
        case TokenType::OPERATOR_PAREN_L: {
            if(context->active) {
                AST *node = new AST(NodeType::EXPRESSION);
                context->active->addChild(node);
                context->active->setHasOpenParen(true);
                context->active = node;
            } else {
                throw "Unexpected paranthesis";
            }
            break;
        }
        case TokenType::OPERATOR_PAREN_R: {
            if(context->active && context->active->getParent()) {
                do {
                    if(!context->active->getParent()) {
                        throw "Unexpected closing paranthesis";
                    }
                    context->active = context->active->getParent();
                } while(!context->active->getHasOpenParen());
                context->active->setHasOpenParen(false);
            } else {
                throw "Unexpected paranthesis";
            }
            break;
        }
        case TokenType::LITERAL_NUMBER: {
            if(context->active) {
                if(context->active->getNodeType() == NodeType::OPERATOR_PLUS || context->active->getNodeType() == NodeType::OPERATOR_MINUS || context->active->getNodeType() == NodeType::OPERATOR_MUL || context->active->getNodeType() == NodeType::OPERATOR_DIV) {
                    AST *node = new AST(NodeType::NUMBER);
                    node->setValue(token);
                    context->active->addChild(node);
                } else if(context->active->getNodeType() == NodeType::EXPRESSION) {
                    context->active->setNodeType(NodeType::NUMBER);
                    context->active->setValue(token);
                } else {
                    throw "Unexpected number";
                }
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
