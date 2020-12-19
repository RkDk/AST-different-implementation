//
//  AST.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "AST.hpp"

AST::AST(NodeType _nodeType) : nodeType(_nodeType) {

}

AST::AST(NodeType _nodeType, std::string _value) : nodeType(_nodeType), value(_value) {
    
}

float AST::getNumberValue() {
    return std::stof(value);
}
std::string AST::getStringValue() {
    return value;
}
