//
//  AST.h
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#ifndef AST_hpp
#define AST_hpp

#include <string>
#include <vector>
#include <iostream>

enum class NodeType {
  ROOT,
  VARIABLE_DECLARATION,
  VARIABLE_ASSIGNMENT,
  EXPRESSION,
  OPERATOR_PLUS,
  OPERATOR_MINUS,
  OPERATOR_MUL,
  OPERATOR_DIV,
  NUMBER,
  STRING,
  IDENTIFIER,
  UNKNOWN
};

class AST {
private:
  unsigned int nodeId = 0;
  NodeType nodeType = NodeType::UNKNOWN;
  std::vector<AST*> children;
  AST *parent = NULL;
  std::string targetName;
  std::string value;
  bool hasOpenParen = false;
  void assignId();
  
public:
  AST();
  AST(NodeType);
  
  size_t getChildCount() const;
  
  bool getHasOpenParen() const;
  void setHasOpenParen(bool);
  
  AST *getTopParent();
  AST *getParent() const;
  void setParent(AST *);
  
  void setTargetName(std::string);
  std::string getTargetName() const;
  
  void clearValue();
  void setValue(std::string);
  
  void swapChild(AST *, AST *);
  void addChild(AST *);
  void addAsParent(AST *);
  AST *getChild(size_t) const;
  
  void setNodeType(NodeType);
  NodeType getNodeType() const;
  
  float getNumberValue() const;
  std::string getStringValue() const;
  
  void printData() const;
  
};

#endif /* AST_hpp */
