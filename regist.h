//
//  regist.hpp
//  reflect1
//
//  Created by yangerjun on 2023/4/28.
//

#ifndef regist_hpp
#define regist_hpp

#include <string>
#include <functional>
#include <iostream>
#include <cxxabi.h>
#include <tuple>
#include <unordered_map>
#include "metainfo.h"
#include "serialize.h"
#include "base.h"

//template<typename T,typename ...Args>
//struct Register;
#define GET_TYPE_NAME(type) abi::__cxa_demangle(typeid(type).name(),0,0,0)

class Register {
    
public:
    struct FieldInfoRegister;
    
    template<typename  T>
    struct InitialRegister;
};

template<typename  T>
struct Register::InitialRegister {
    InitialRegister() {
        std::cout << GET_TYPE_NAME(T) << std::endl;
        RFClassInfo::Deserializers[GET_TYPE_NAME(T)] = [](void *obj, const std::string& json)->void {
            Serializer::Deserialize((T*)obj, json);
        };
        
        RFClassInfo::Constructors[GET_TYPE_NAME(T)] = [](void)->void* {
            return (void*)(new T());
        };
        
        RFClassInfo::SharedConstructors[GET_TYPE_NAME(T)] = [](void* address, const std::string& value, const std::string& type)->void {
            T* obj = new T();
            (*(std::shared_ptr<T>*)address).reset(obj);
            RFClassInfo::Deserializers[type]((void*)obj, value);
        };
        
        RFClassInfo::ArrayConstructors[GET_TYPE_NAME(T)] = [](void* address, const std::vector<std::string>& elems, const std::string& type)->void {
            for(auto elem : elems) {
                std::shared_ptr<T> obj(new T());
                (*(RFArrayClass<T>*)address).Data.push_back(obj);
                RFClassInfo::Deserializers[type]((void*)obj.get(), elem);
            }
        };
    }
};

struct Register::FieldInfoRegister {
    FieldInfoRegister(void* obj, void* field_address, const char* clazz, const char* field_type, const char* field_name) {
        if(RFClassInfo::ClassMetaInfo.find(std::string(clazz)) != RFClassInfo::ClassMetaInfo.end() && RFClassInfo::ClassMetaInfo[std::string(clazz)].find(std::string(field_name)) != RFClassInfo::ClassMetaInfo[std::string(clazz)].end()) return;
        RFClassInfo::ClassMetaInfo[std::string(clazz)].insert({std::string(field_name) ,std::tuple<std::string, int>(std::string(field_type), (int)((char*)field_address - (char*)obj))});
    }
};

#endif /* regist_hpp */
