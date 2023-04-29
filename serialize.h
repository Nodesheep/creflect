//
//  serialize.hpp
//  reflect1
//
//  Created by yangerjun on 2023/4/28.
//

#ifndef serialize_hpp
#define serialize_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <cxxabi.h>
#include "metainfo.h"
#include "base.h"

#define GET_TYPE_NAME(type) abi::__cxa_demangle(typeid(type).name(),0,0,0)

enum SerialState {
    KEY,
    VALUE,
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_ARRAY,
    VALUE_OBJ,
    VALUE_END
};

class Serializer {
public:
    template<typename T>
    static void* obj_from_json(const std::string& json) {
        std::string clazz = "";
        T* obj = new T();
        RFClassInfo::Deserializers[clazz](obj, json);
    }
    
    static int int_from_json(const std::string& json) {
        return std::stoi(json);
    }
    
    static double double_from_json(const std::string& json) {
        return std::stod(json);
    }
    
    static bool bool_from_json(const std::string& json) {
        return std::stoi(json);
    }

    template<typename T>
    static std::string Serialize(T *obj) {
        std::string clazz = GET_TYPE_NAME(*obj);
        return Serialize(obj, clazz);
    }
    
    static std::string Serialize(void *obj, const std::string& clazz) {
        std::string json = "{";
        std::unordered_map<std::string, std::tuple<std::string, int>> map = RFClassInfo::ClassMetaInfo[clazz];
        for(std::unordered_map<std::string, std::tuple<std::string, int>>::iterator it = map.begin(); it != map.end(); ++it) {
            std::string field_name = it->first;
            std::string field_type = std::get<0>(it->second);
            void* field_address = RFClassInfo::FieldAddress(obj, clazz, field_name);
            if(field_type == "int") {
                json += "\"" + field_name + "\":" + std::to_string(*(int*)field_address) + ",";
            }else if(field_type == "double") {
                json += "\"" + field_name + "\":" + std::to_string(*(double*)field_address) + ",";
            }else if(field_type == "bool") {
                json += "\"" + field_name + "\":" + (*(bool*)field_address ? "1" : "0") + ",";
            }else if(field_type == "std::string") {
                json += "\"" + field_name + "\":" + "\"" + (*(std::string*)field_address) + "\"" + ",";
            }else if(field_type.find("RFArrayClass") == 0) {
                json += "\"" + field_name + "\":" + ArraySerialize(field_address, field_type) + ",";
            }else {
                //shared
                std::shared_ptr<const void> obj = *(std::shared_ptr<const void>*)field_address;
                json += "\"" + field_name + "\":" + Serialize((void*)obj.get(), field_type) + ",";
                
                //void*
                //json += "\"" + field_name + "\":" + Serialize(*(void**)field_address, field_type) + ",";
            }
        }
        json.pop_back();
        json += "}";
        return json;
    }
    
    static std::string ArraySerialize(void *obj, const std::string& type) {
        size_t len = type.size();
        std::string elemtype = type.substr(13, len-14);
        std::string json = "[";
        if(elemtype == "int") {
            RFArrayClass<int> array = *(RFArrayClass<int>*)(obj);
            for(auto v : array.Data) {
                json += std::to_string(*v) + ",";
            }
        }else if(elemtype == "double") {
            RFArrayClass<double> array = *(RFArrayClass<double>*)(obj);
            for(auto v : array.Data) {
                json += std::to_string(*v) + ",";
            }
        }else if(elemtype == "bool") {
            RFArrayClass<bool> array = *(RFArrayClass<bool>*)(obj);
            for(auto v : array.Data) {
                json += std::to_string((int)*v) + ",";
            }
        }else if(elemtype == "std::string") {
            RFArrayClass<std::string> array = *(RFArrayClass<std::string>*)(obj);
            for(auto v : array.Data) {
                json +=  "\"" + *v + "\"" + ",";
            }
        }else {
            RFArrayClass<const void> array = *(RFArrayClass<const void>*)(obj);
            for(auto v : array.Data) {
                json += Serialize((void*)v.get(), elemtype) + ",";
            }
        }
        
        json.pop_back();
        json += "]";
        
        return json;
    }
    
    template<typename T>
    static void* Deserialize(const std::string& json) {
        T* obj = new T();
        Deserialize(obj, json);
        return obj;
    }

    template<typename T>
    static void Deserialize(T *obj, const std::string& json) {
        std::string clazz = GET_TYPE_NAME(*obj);
        std::unordered_map<std::string, std::string> kvs;
        Decode(json, kvs);
        for(auto kv : kvs) {
            std::string field_name = kv.first;
            std::string field_value = kv.second;
            std::string field_type = RFClassInfo::FieldType(clazz, field_name);
            void* field_address = RFClassInfo::FieldAddress(obj ,clazz, field_name);
            if(field_type == "int") {
                *(int*)field_address = int_from_json(field_value);
            }else if(field_type == "double") {
                *(double*)field_address = double_from_json(field_value);
            }else if(field_type == "bool") {
                *(bool*)field_address = bool_from_json(field_value);
            }else if(field_type == "std::string") {
                *(std::string*)field_address = field_value;
            }else if(field_type.find("RFArrayClass") == 0) {
                ArrayDeserialize(field_address, field_type, field_value);
            }else {
                RFClassInfo::SharedConstructors[field_type](field_address, field_value, field_type);
            }
        }
    }
    
    static void ArrayDeserialize(void *address, const std::string& type, const std::string& json) {
        size_t len = type.size();
        std::string elemtype = type.substr(13, len-14);
        std::vector<std::string> elems;
        size_t size = json.size();
        int begin = 1, brace_count = 0;
        for(int i = 1; i < size; ++i) {
            if(json[i] == '{') ++brace_count;
            if(json[i] == '}') --brace_count;
            if(brace_count == 0) {
                if(json[i] == ',' || json[i] == ']') {
                    elems.push_back(json.substr(begin, i - begin));
                    begin = i + 1;
                }
            }
        }
        
        if(elemtype == "int") {
            for(auto elem : elems) {
                (*(RFArrayClass<int>*)address).Data.push_back(std::make_shared<int>(int_from_json(elem)));
            }
        }else if(elemtype == "double") {
            for(auto elem : elems) {
                (*(RFArrayClass<double>*)address).Data.push_back(std::make_shared<double>(double_from_json(elem)));
            }
        }else if(elemtype == "bool") {
            for(auto elem : elems) {
                (*(RFArrayClass<bool>*)address).Data.push_back(std::make_shared<bool>(bool_from_json(elem)));
            }
        }else if(elemtype == "std::string") {
            for(auto elem : elems) {
                (*(RFArrayClass<std::string>*)address).Data.push_back(std::make_shared<std::string>(elem.substr(1, elem.size() - 2)));
            }
        }else {
            RFClassInfo::ArrayConstructors[elemtype](address, elems, elemtype);
        }
    }
    
    static void Decode(const std::string& json, std::unordered_map<std::string, std::string>& kvs) {
        std::cout << "kkkkk" << json << std::endl;
        SerialState state = KEY;
        size_t len = json.size();
        std::string key = "";
        std::string value = "";
        int brace_count = 0, bracket_count = 0;
        for(int i = 0; i < len; ++i) {
            char c = json[i];
            if(c == ' ') {
                continue;
            }
            if(state == KEY) {
                if(c == ',' || c == '}' || c == '{') continue;
                if(c == ':') {
                    state = VALUE;
                }else if(c != '\"') {
                    key.push_back(c);
                }
            }else if(state == VALUE) {
                if(c == '{') {
                    value.push_back(c);
                    ++brace_count;
                    state = VALUE_OBJ;
                }else if(c == '[') {
                    value.push_back(c);
                    ++bracket_count;
                    state = VALUE_ARRAY;
                }else if(c == '\"') {
                    state = VALUE_STRING;
                }else {
                    value.push_back(c);
                    state = VALUE_NUMBER;
                }
            }else if(state == VALUE_OBJ) {
                value.push_back(c);
                if(c == '{') ++brace_count;
                if(c == '}') --brace_count;
                if(brace_count == 0) state = VALUE_END;
            }else if(state == VALUE_ARRAY) {
                value.push_back(c);
                if(c == '[') ++bracket_count;
                if(c == ']') --bracket_count;
                if(bracket_count == 0) state = VALUE_END;
            }else if(state == VALUE_STRING) {
                if(c == '\"') {
                    state = VALUE_END;
                    continue;
                }
                value.push_back(c);
            }else if(state == VALUE_NUMBER) {
                if(c == ',' || c == '}') {
                    state = VALUE_END;
                    if(c == '}') {
                        kvs[key] = value;
                        key.clear();
                        value.clear();
                    }
                }else {
                    value.push_back(c);
                }
                
            }else if(state == VALUE_END) {
                kvs[key] = value;
                std::cout << "xxx" << key << "xxx" << value << std::endl;
                key.clear();
                value.clear();
                state = KEY;
            }
        }
    }
};

#endif /* serialize_hpp */
