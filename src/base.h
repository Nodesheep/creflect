#ifndef CREFLECT_BASE_H
#define CREFLECT_BASE_H

#include <vector>
#include <cxxabi.h>

class RFObjectClass {
public:
    virtual const char* Clazz() {
        static char* result = abi::__cxa_demangle(typeid(*this).name(),0,0,0);
        return result;
    }
};

template <typename T>
class RFArrayClass : public RFObjectClass {
public:
    std::vector<std::shared_ptr<T>> Data;
    virtual const char* Clazz() {
        static char* result = abi::__cxa_demangle(typeid(*this).name(),0,0,0);
        return result;
    }
};

#endif
