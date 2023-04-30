/*
#include <iostream>
#include "reflect.h"
#include "metainfo.h"
#include "serialize.h"
#include <unistd.h>
#include <sys/time.h>
#include "json.h"

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
    RFDouble(height)
    user(){}
    user(std::string name, int age, double h) : username(name), age(age), height(h) {}
RFClassEnd(user)

RFClassBegin(comment)
public:
    RFObject(user, commentator)
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
    RFArray(videotag, tags)
    RFArray(comment, comments)
RFClassEnd(video)

std::string jsoncpp_serialize() {
    Json::Value u1;
    u1["name"] = "jackson";
    u1["age"] = 17;
    u1["height"] = 188;
    
    Json::Value u2;
    u2["name"] = "jackson";
    u2["age"] = 17;
    u2["height"] = 188;
    
    Json::Value u3;
    u3["name"] = "jackson";
    u3["age"] = 17;
    u3["height"] = 188;
    
    Json::Value u4;
    u4["name"] = "jackson";
    u4["age"] = 17;
    u4["height"] = 188;
    
    Json::Value u5;
    u5["name"] = "jackson";
    u5["age"] = 17;
    u5["height"] = 188;
    
    Json::Value tag1;
    tag1["title"] = "综艺";
    tag1["detail"] = "搞笑";
    
    Json::Value tag;
    tag["title"] = "综艺";
    tag["detail"] = "搞笑";
    
    Json::Value c1;
    c1["commentor"] = u2;
    c1["like"] = false;
    c1["content"] = "非常喜欢，下饭神器";
    
    Json::Value c2;
    c2["commentor"] = u3;
    c2["like"] = false;
    c2["content"] = "非常喜欢，下饭神器";
    
    Json::Value c3;
    c3["commentor"] = u4;
    c3["like"] = false;
    c3["content"] = "非常喜欢，下饭神器";
    
    Json::Value c4;
    c4["commentor"] = u5;
    c4["like"] = false;
    c4["content"] = "非常喜欢，下饭神器";
    
    Json::Value v1;
    v1["candown"] = true;
    v1["like_count"] = 1055;
    v1["up"] = u1;
    v1["video_url"] = "https://xxxxxxxxxxxx";
    v1["tags"].append(tag);
    v1["tags"].append(tag1);
    
    v1["comments"].append(c1);
    v1["comments"].append(c2);
    v1["comments"].append(c3);
    v1["comments"].append(c4);
    
    std::ostringstream jsonos;
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(v1, &jsonos);
    return jsonos.str();
}

void jsoncpp_deserialize(const std::string& json) {
    Json::CharReaderBuilder crb;
    Json::Value root;
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
    cr->parse(&json[0],&json[0]+json.size(),&root,nullptr);
    
    /*
    std::ostringstream jsonos;
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &jsonos);
    std::cout << jsonos.str() << std::endl;
}

std::string creflect_serialize() {
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
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("综艺", "搞笑")));
    v1.tags.Data.push_back(std::shared_ptr<videotag>(new videotag("体育", "体能")));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u2), false, "个人感觉一般"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u3), true, "好有意思"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u4), true, "非常喜欢，下饭神器"));
    v1.comments.Data.push_back(std::make_shared<comment>(std::make_shared<user>(u5), true, "里面有我最喜欢的演员，大家多多支持xx"));
    

    std::string json = Serializer::Serialize(&v1);
    return json;
}

void creflect_deserialize(const std::string& json) {
    video* v = (video*)Serializer::Deserialize<video>(json);
    std::cout << Serializer::Serialize(v) << std::endl;
    delete v;
}

int main(int argc, const char * argv[]) {
    

    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    
    std::cout << seconds * 1000 * 1000 + tv.tv_usec << std::endl;
    
    //std::string json = jsoncpp_serialize();
    std::string json = creflect_serialize();
    for(int i = 0 ;i < 1; ++i) {
        //jsoncpp_deserialize(json);
        creflect_deserialize(json);
    }
    
    gettimeofday(&tv, NULL);
    seconds = tv.tv_sec;
    
    std::cout << seconds * 1000 * 1000 + tv.tv_usec << std::endl;
    while(1) {
        sleep(100000);
    }
    return 0;
}*/
