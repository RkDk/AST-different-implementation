//
//  AST.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "AST.hpp"

std::string getNodeTypeName(NodeType nodeType) {
    switch(nodeType) {
        case NodeType::ROOT: return "ROOT";
        case NodeType::VARIABLE_DECLARATION: return "VARIABLE_DECLARATION";
        case NodeType::VARIABLE_ASSIGNMENT: return "VARIABLE_ASSIGNMENT";
        case NodeType::EXPRESSION: return "EXPRESSION";
        case NodeType::OPERATOR_PLUS: return "OPERATOR_PLUS";
        case NodeType::OPERATOR_MINUS: return "OPERATOR_MINUS";
        case NodeType::OPERATOR_MUL: return "OPERATOR_MUL";
        case NodeType::OPERATOR_DIV: return "OPERATOR_DIV";
        case NodeType::NUMBER: return "NUMBER";
        case NodeType::STRING: return "STRING";
        case NodeType::IDENTIFIER: return "IDENTIFIER";
        default: return "UNKNOWN";
    }
}

AST::AST() {
    assignId();
}

AST::AST(NodeType _nodeType) : nodeType(_nodeType) {
    assignId();
}

void AST::assignId() {
    static unsigned int globalId = 1;
    if(nodeId == 0) {
        nodeId = globalId++;
    }
}

size_t AST::getChildCount() const {
    return children.size();
}

AST *AST::getTopParent() {
    if(!parent) {
        return this;
    }
    return parent->getTopParent();
}

void AST::setParent(AST *node) {
    parent = node;
}

AST *AST::getParent() {
    return parent;
}

void AST::setTargetName(std::string s) {
    targetName = s;
}

std::string AST::getTargetName() const {
    return targetName;
}

void AST::clearValue() {
    value.clear();
}

void AST::setValue(std::string s) {
    value = s;
}

void AST::swapChild(AST *oldChild, AST *newChild) {
    for(size_t i = 0; i < children.size(); i++) {
        if(children[i]->nodeId == oldChild->nodeId) {
            newChild->parent = this;
            oldChild->parent = NULL;
            children[i] = newChild;
            break;
        }
    }
}

void AST::addChild(AST *node) {
    node->setParent(this);
    children.push_back(node);
}

AST* AST::getPrevChild() const {
    if(children.size() == 0) {
        return NULL;
    }
    return children[children.size()-1];
}

void AST::setNodeType(NodeType t) {
    nodeType = t;
}

NodeType AST::getNodeType() const {
    return nodeType;
}

float AST::getNumberValue() const {
    return std::stof(value);
}
std::string AST::getStringValue() const {
    return value;
}

void AST::printData() const {
    std::cout << "=== AST Node ===\n";
    std::cout << "ID: " << nodeId << "\n";
    if(parent) {
        std::cout << "Parent ID: " << parent->nodeId << "\n";
    }
    std::cout << "Type: " << getNodeTypeName(nodeType) << "\n";
    if(!targetName.empty()) {
        std::cout << "Target name: " << targetName << "\n";
    }
    if(!value.empty()) {
        std::cout << "Value: " << value << "\n";
    }
    std::cout << "=== End ===\n";
    for(auto it = children.begin(); it != children.end(); it++) {
        (*it)->printData();
    }
}
