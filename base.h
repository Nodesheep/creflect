//
//  base.hpp
//  reflect1
//
//  Created by yangerjun on 2023/4/29.
//

#include <vector>
#include <cxxabi.h>

#ifndef base_hpp
#define base_hpp

class RFObjectClass {
public:
    virtual const char* Clazz() const {
        return abi::__cxa_demangle(typeid(*this).name(),0,0,0);
    }
};

template <typename T>
class RFArrayClass {
public:
    std::vector<std::shared_ptr<T>> Data;
};

#endif /* base_hpp */
