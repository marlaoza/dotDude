#pragma once
#include "asset.h"
#include "character.h"
#include "vector"
#include <cstdio>

#pragma pack(push, 1)

struct Header {
    char identifier[4];       
    uint8_t version;   
    uint8_t animCount; 
};                       

struct CharData {
    char name[32];       
    uint8_t baseSpeed;   
    uint8_t runSpeed;    
};                       

struct Entry {
    uint8_t frameCount;  
    uint8_t frameWidth;  
    uint8_t frameHeight; 
    uint32_t dataOffset;  
    uint32_t dataSize;    
    int8_t tags[4];      
};      

struct DudeFile {
    const char* filePath;
    Header header;
    CharData data;
    std::vector<Entry> entries;
};

#pragma pack(pop)

#define HEADER_ID "DUD3"

class IPlatform{
    protected:
        float deltaTime = 0.0f;
        const char* rootPath;
        int latestSpriteID = 0;
    public:
       
        virtual ~IPlatform() = default;

        void setRootPath(const char* path){
            rootPath = path;
        }
        void getFullPath(char* path, size_t size, const char* filePath){
            if (!path || size == 0) return;
            snprintf(path, size, "%s/%s", rootPath, filePath);
        }

        virtual void getImage(std::vector<uint8_t> &buffer, const char* file, SpriteAsset anim) = 0;

        virtual void drawFrame(const char* file, SpriteAsset* anim, int frame, int x, int y) = 0;
        virtual void drawRect(Style style, int x, int y) = 0;

        float getDeltaTime(){return deltaTime;}
        virtual void tick() = 0;
        
        virtual DudeFile unpackDude(const char* path) = 0;
};