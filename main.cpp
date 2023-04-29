//
//  main.cpp
//  reflect1
//
//  Created by yangerjun on 2023/4/27.
//

#include <iostream>
#include "reflect.h"
#include "metainfo.h"
#include "serialize.h"

RFClassBegin(videotag)
public:
    RFString(title)
    RFString(detail)
videotag(){}
    videotag(std::string title, std::string detail) : title(title), detail(detail) {}
RFClassEnd(videotag)

RFClassBegin(user)
public:
    RFString(username)
    RFInt(age)
    RFDouble(tall)
    user(){}
    user(std::string name, int age, double tall) : username(name), age(age), tall(tall) {}
RFClassEnd(user)

RFClassBegin(comment)
public:
    RFObject(user, ur)
    RFString(content)
    comment(){}
    comment(std::shared_ptr<user> user, std::string content) : ur(user), content(content) {}
RFClassEnd(comment)

RFClassBegin(video)
public:
    RFObject(user, up)
    RFString(video_url)
    RFBool(candw)
    RFArray(videotag, tags)
    RFArray(comment, comments)
RFClassEnd(video)

//RFInformation::Register<M> r();
int main(int argc, const char * argv[]) {

    
    user u1("yang", 17, 189.7);
    user u2("mimi", 19, 165.3);
    user u3("jiji", 16, 177);
    video v1;
    v1.candw = true;
    v1.up = std::make_shared<user>(u1);
    v1.video_url = "https://xxxxxxxxxxxx";
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("综艺", "搞笑")));
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("体育", "体能")));
    //v1.tags.Data.push_back(new videotag("综艺", "搞笑"));
    //v1.tags.Data.push_back(new videotag("体育", "体能"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u2), "差点意思"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u3), "太好玩了"));
    
    std::string json = Serializer::Serialize(&v1);
    std::cout << json << std::endl;
    video* v2 = (video*)Serializer::Deserialize<video>(json);
    std::cout << "v2" << v2->up->username << std::endl;
    std::cout << "v2" << v2->video_url << std::endl;
    std::cout << "v2" << v2->comments.Data[0]->ur->username << std::endl;
    std::string json1 = Serializer::Serialize(v2);
    std::cout << json1 << std::endl;
     
    std::cout << "hello";
 
    //std::cout << Serializer::Serialize(&u1) << std::endl;
    //std::cout << Serializer::Serialize(&v1) << std::endl;
    return 0;
}
