//
//  information.hpp
//  reflect1
//
//  Created by yangerjun on 2023/4/28.
//

#include <unordered_map>
#include <string>
#include <functional>
#include <tuple>

#ifndef information_hpp
#define information_hpp

class RFClassInfo {
public:
    static inline std::unordered_map<std::string, std::function<void(void *, const std::string&)>> Deserializers;
    static inline std::unordered_map<std::string, std::function<void*(void)>> Constructors;
    static inline std::unordered_map<std::string, std::unordered_map<std::string, std::tuple<std::string, int>>> ClassMetaInfo;
    
    static inline std::unordered_map<std::string, std::function<void(void*, const std::string&, const std::string&)>> SharedConstructors;
    
    static inline std::unordered_map<std::string, std::function<void(void*, const std::vector<std::string>&, const std::string&)>> ArrayConstructors;
    
    static std::string FieldType(const std::string& clazz, const std::string field_name) {
        return std::get<0>(ClassMetaInfo[clazz][field_name]);
    }
    static void* FieldAddress(void* obj, const std::string& clazz, const std::string field_name) {
        return (void*)((char*)obj + std::get<1>(ClassMetaInfo[clazz][field_name]));
    }
};

#endif /* information_hpp */
