# creflect
一个简易的c++反射库，并基于此实现了json序列化与反序列化。

## 简介
我们知道C++对象的类型信息在编译时就已经确定了，C++对象模型将对象存储在内存中，每个对象占用一段连续的内存空间；而对象的类型信息通常存储在程序的数据段或者只读数据段中，这意味着在运行时，程序无法通过对象地址来获取对象的类型信息。此外，C++中的函数调用也是静态绑定的，这意味着在编译时就已经确定了要调用的函数，无法动态地调用函数，也就无法实现反射，这是c++的天然缺陷。  

为了实现反射，需要通过一些方法使得在类定义或初始化时将类型的基本信息注入到内存中进行存储。本项目通过RFClassBegin宏在类型定义时通过静态成员变量将类名与其反序列化函数、构造函数等进行绑定，实现在程序运行时通过类名字符串实现对象的动态创建；在类对象首次创建时，通过RFString、RFInt、RFDouble、RFArray、RFObject等成员变量声明宏将成员变量名、成员变量类型以及偏移量与对应类进行绑定，实现在程序运行时动态获取成员变量信息以及通过偏移量修改成员变量值。  

此外，本项目的所有对象成员变量均由shared_ptr进行管理，无需关注内存释放问题，仅反序列化时生成的对象需要用户手动释放，反序列化时需要确保json串中的所有类型均有对应的定义。  

本项目未实现方法的动态调用。

## 使用
以下为本项目json序列化和反序列化的基本使用用例。  

目前json序列化和反序列化支持成员变量为int(RFInt)、double(RFDouble)、bool(RFBool) 、string(RFString)、数组(RFArray)以及自定义类型(RFObject）。
```
#include "reflect.h"
#include "serialize.h"

//自定义具有反射性质的类型 videotag
RFClassBegin(videotag)
public:
    RFString(title) //string成员变量 title
    RFString(detail)
    videotag(){}
    videotag(std::string title, std::string detail) : title(title), detail(detail) {}
RFClassEnd(videotag)

RFClassBegin(user)
public:
    RFString(username)
    RFInt(age) //int成员变量 age
    RFDouble(height) //double成员变量 height
    user(){}
    user(std::string name, int age, double h) : username(name), age(age), height(h) {}
RFClassEnd(user)

RFClassBegin(comment)
public:
    RFObject(user, commentator) //自定义类型user成员变量 commentator
    RFBool(like)
    RFString(content)
    comment(){}
    comment(std::shared_ptr<user> com, bool like, const std::string& content) : commentator(com), like(like), content(content) {}
RFClassEnd(comment)

RFClassBegin(video)
public:
    RFInt(like_count)
    RFObject(user, up)
    RFString(video_url)
    RFBool(candownload)
    RFArray(videotag, tags) //videotag数组成员变量 tags
    RFArray(comment, comments)
RFClassEnd(video)

int main() {
    user u1("jackson", 17, 189.7);
    user u2("mima", 19, 165.3);
    user u3("李华", 16, 177);
    user u4("小永",22, 180);
    user u5("jimi", 15, 173);

    video v1;
    v1.candownload = true;
    v1.like_count = 1055;
    v1.up = std::make_shared<user>(u1);
    v1.video_url = "https://xxxxxxxxxxxx";
    
    //shared_ptr管理
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("综艺", "搞笑")));
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("体育", "体能")));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u2), false, "个人感觉一般"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u3), true, "好有意思"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u4), true, "非常喜欢，下饭神器"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u5), true, "里面有我最喜欢的演员，大家多多支持xx"));
    
    //序列化
    std::string json = Serializer::Serialize(&v1);
    //反序列化
    video* v2 = (video*)Serializer::Deserialize<video>(json);
    delete v2;
}
```
## 测试
连续进行100000次序列化和反序列化操作计算其耗时，并与jsoncpp进行比较。测试代码在本项目的test目录下，读者若想本地测试需要下载jsoncpp源码。以下为测试数据。  

｜耗时/微秒｜ 本项目 ｜jsoncpp｜

｜ 序列化 ｜8705877｜15867223｜

｜反序列化｜16906126｜9656936｜

仅根据该测试数据来看，本项目的序列化效率是优于jsoncpp的，但反序列化效率弱于jsoncpp，猜测是因为动态创建对象产生的消耗。  

需要注意的是，由于本项目的类型信息全部保存在map中，随着类型数增多会带来额外的查询成本。
