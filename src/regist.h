#ifndef CREFLECT_REGIST_H
#define CREFLECT_REGIST_H

#include <string>
#include <functional>
#include <cxxabi.h>
#include <tuple>
#include <unordered_map>
#include "metainfo.h"
#include "serialize.h"
#include "base.h"
#include <iostream>

class Register {
    
public:
    struct FieldInfoRegister;
    
    template<typename  T>
    struct InitialRegister;
};

template<typename  T>
struct Register::InitialRegister {
    InitialRegister() {
        const char* clazz = GET_TYPE_NAME(T);
        RFClassInfo::Deserializers[clazz] = [](void *obj, const std::string& json)->void {
            Serializer::Deserialize((T*)obj, json);
        };
        
        RFClassInfo::Constructors[clazz] = [](void)->void* {
            return (void*)(new T());
        };
        
        RFClassInfo::SharedConstructors[clazz] = [](void* address, const std::string& value, const std::string& type)->void {
            T* obj = new T();
            (*(std::shared_ptr<T>*)address).reset(obj);
            RFClassInfo::Deserializers[type]((void*)obj, value);
        };
        
        RFClassInfo::ArrayConstructors[clazz] = [](void* address, const std::vector<std::string>& elems, const std::string& type)->void {
            for(auto elem : elems) {
                std::shared_ptr<T> obj(new T());
                (*(RFArrayClass<T>*)address).Data.push_back(obj);
                RFClassInfo::Deserializers[type]((void*)obj.get(), elem);
            }
        };
        
        delete clazz;
    }
};

struct Register::FieldInfoRegister {
    FieldInfoRegister(void* obj, void* field_address,const char* clazz, const char* field_type, const char* field_name) {
        if(RFClassInfo::ClassMetaInfo.find(std::string(clazz)) != RFClassInfo::ClassMetaInfo.end() && RFClassInfo::ClassMetaInfo[std::string(clazz)].find(std::string(field_name)) != RFClassInfo::ClassMetaInfo[std::string(clazz)].end()) return;
    
        RFClassInfo::ClassMetaInfo[std::string(clazz)].insert({std::string(field_name) ,std::make_tuple<std::string, int>(std::string(field_type), (int)((char*)field_address - (char*)obj))});
    }
};

#endif
