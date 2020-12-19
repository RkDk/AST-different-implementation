//
//  utils.cpp
//  AST-different-implementation
//
//  Created by Rick Chou on 12/19/20.
//

#include "utils.hpp"

std::ostream &operator<<(std::ostream &o, std::vector<std::string> &v) {
    for(auto i = v.begin(); i != v.end(); i++) {
        o << *i << std::endl;
    }
    return o;
}
