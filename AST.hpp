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
    NodeType nodeType = NodeType::UNKNOWN;
    std::vector<AST*> children;
    std::string targetName;
    std::string value;
public:
    AST() = default;
    AST(NodeType);
    
    void setTargetName(std::string);
    std::string getTargetName() const;
    
    void setValue(std::string);
    
    void addChild(AST *);
    AST *getPrevChild() const;
    
    void setNodeType(NodeType);
    NodeType getNodeType() const;
    
    float getNumberValue() const;
    std::string getStringValue() const;
    
    void printData() const;
    
};

#endif /* AST_hpp */
