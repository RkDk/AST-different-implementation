//
//  AST.h
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#ifndef AST_h
#define AST_h

#include <string>

enum class NodeType {
    VARIABLE_DECLARATION,
    VARIABLE_ASSIGNMENT,
    EXPRESSION,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MUL,
    OPERATOR_DIV,
    NUMBER,
    STRING,
    UNKNOWN
};

class AST {
private:
    NodeType nodeType;
    std::string targetName;
    std::string value;
public:
    AST(NodeType);
    AST(NodeType, std::string);

    float getNumberValue();
    std::string getStringValue();
};

#endif /* AST_h */
