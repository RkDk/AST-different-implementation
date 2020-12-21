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

bool isMathOpNode(NodeType nodeType) {
  return (nodeType == NodeType::OPERATOR_PLUS ||
          nodeType == NodeType::OPERATOR_MINUS ||
          nodeType == NodeType::OPERATOR_MUL ||
          nodeType == NodeType::OPERATOR_DIV);
}

bool isPlusMinusMathOpNode(NodeType nodeType) {
  return (nodeType == NodeType::OPERATOR_PLUS || nodeType == NodeType::OPERATOR_MINUS);
}

bool isValidMathOperand(NodeType nodeType) {
  return (nodeType == NodeType::NUMBER || nodeType == NodeType::IDENTIFIER);
}

NodeType getMathOpNodeType(TokenType tokenType) {
  switch(tokenType) {
    case TokenType::OPERATOR_PLUS: return NodeType::OPERATOR_PLUS;
    case TokenType::OPERATOR_MINUS: return NodeType::OPERATOR_MINUS;
    case TokenType::OPERATOR_MUL: return NodeType::OPERATOR_MUL;
    case TokenType::OPERATOR_DIV: return NodeType::OPERATOR_DIV;
    default: return NodeType::UNKNOWN;
  }
}

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
      if(active) {
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
      } else {
        throw "Unexpected assignment operator";
      }
      break;
    }
    case TokenType::OPERATOR_PLUS:
    case TokenType::OPERATOR_MINUS: {
      AST *active = context->active;
      if(active) {
        NodeType nodeType = active->getNodeType();
        if(nodeType == NodeType::EXPRESSION && active->getChildCount() == 1) {
          active->setNodeType(getMathOpNodeType(tokenType));
        }
        else if(
          (active->getChildCount() == 0 && isValidMathOperand(nodeType)) ||
          (active->getChildCount() == 2 && isMathOpNode(nodeType))) {
          AST *node = new AST(getMathOpNodeType(tokenType));
          active->addAsParent(node);
          context->active = node;
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
        if(nodeType == NodeType::EXPRESSION && active->getChildCount() == 1) {
          active->setNodeType(getMathOpNodeType(tokenType));
        }
        else if(
          (active->getChildCount() == 0 && isValidMathOperand(nodeType)) ||
          (active->getChildCount() == 2 && isMathOpNode(nodeType))) {
          AST *node = new AST(getMathOpNodeType(tokenType));
          AST *target = (active->getChildCount() == 2 && isPlusMinusMathOpNode(nodeType))? active->getChild(1) : active;
          target->addAsParent(node);
          context->active = node;
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
        NodeType nodeType = context->active->getNodeType();
        if((nodeType == NodeType::EXPRESSION && context->active->getChildCount() == 0) ||
           (isMathOpNode(nodeType) && context->active->getChildCount() == 1)) {
          AST *node = new AST(NodeType::EXPRESSION);
          context->active->addChild(node);
          context->active->setHasOpenParen(true);
          context->active = node;
        } else {
          throw "Unexpected paranthesis";
        }
      } else {
        throw "Unexpected paranthesis";
      }
      break;
    }
    case TokenType::OPERATOR_PAREN_R: {
      if(context->active) {
        NodeType nodeType = context->active->getNodeType();
        if((nodeType == NodeType::EXPRESSION && context->active->getChildCount() == 1) ||
           (isMathOpNode(nodeType) && context->active->getChildCount() == 2) ||
           isValidMathOperand(nodeType)) {
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
      } else {
        throw "Unexpected paranthesis";
      }
      break;
    }
    case TokenType::LITERAL_NUMBER: {
      if(context->active) {
        NodeType nodeType = context->active->getNodeType();
        size_t childCount = context->active->getChildCount();
        if(isMathOpNode(nodeType) && childCount == 1) {
          AST *node = new AST(NodeType::NUMBER);
          node->setValue(token);
          context->active->addChild(node);
        } else if(nodeType == NodeType::EXPRESSION && childCount == 0) {
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
      break;
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
