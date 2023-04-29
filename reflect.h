//
//  reflect.hpp
//  reflect1
//
//  Created by yangerjun on 2023/4/28.
//

#include "regist.h"
#include "base.h"
#include <memory>
#include <vector>

#ifndef reflect_h
#define reflect_h

#define RFClassBegin(clazz) class clazz : public RFObjectClass { \
public:\
static inline Register::InitialRegister<clazz> r;

#define RFClassEnd(clazz) };

#define RFInt(name) AttrRegister(int, name, 0)
#define RFDouble(name) AttrRegister(double, name, 0.0)
#define RFBool(name) AttrRegister(bool, name, false)
#define RFString(name) AttrRegister(std::string, name, "")

#define AttrRegister(type, name, default) \
type name = default;\
Register::FieldInfoRegister __##name = Register::FieldInfoRegister(this, &this->name, this->Clazz(), #type, #name);

#define RFArray(type, name) RegistArray(RFArrayClass<type>, name)

#define RegistArray(type, name) \
type name;\
Register::FieldInfoRegister __##name = Register::FieldInfoRegister(this, &this->name, this->Clazz(), #type, #name);

#define RFObject(type, name) \
std::shared_ptr<type> name; \
Register::FieldInfoRegister __##name = Register::FieldInfoRegister(this, &this->name, this->Clazz(), #type, #name);


#endif /* reflect_hpp */
