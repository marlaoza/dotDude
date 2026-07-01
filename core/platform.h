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

class IFile {
public:
    virtual ~IFile() {}
    virtual size_t read(void* ptr, size_t size) = 0;
    virtual void seek(size_t offset) = 0;
    virtual void close() = 0;
    virtual bool isOpen() = 0;
};

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

        void getImage(std::vector<uint8_t> &buffer, const char* path, SpriteAsset anim) {
            IFile* file = openFile(path);
            if (file->isOpen()) {
                file->seek(anim.offset);
                file->read(buffer.data(), anim.size);
                file->close();
            }
            delete file;
        };

        DudeFile unpackDude(const char* path){
            DudeFile dudeFile;
            dudeFile.filePath = path;
            IFile* file = openFile(path);

            if (!file->isOpen()) {
                this->log("Error - could not open .dude file");
                file->close();
                delete file;
                return dudeFile;
            }

            file->read(&dudeFile.header, sizeof(Header));
            if (strncmp(dudeFile.header.identifier, HEADER_ID, 4) != 0) {
                this->log("Error - invalid filetype");
                file->close();
                delete file;
                return dudeFile;
            }

            file->read(&dudeFile.data, sizeof(CharData));

            std::vector<Entry> entries;
            for (int i = 0; i < dudeFile.header.animCount; ++i) {
                Entry animPayload;
                file->read(&animPayload, sizeof(Entry));
                entries.push_back(animPayload);
            }
            dudeFile.entries = entries;

            file->close();
            delete file;
            return dudeFile;
        }


        virtual void log(const char* msg) = 0;

        virtual IFile* openFile(const char* path) = 0;

        virtual void drawFrame(const char* path, SpriteAsset* anim, int frame, int x, int y) = 0;
        virtual void drawRect(Style style, int x, int y) = 0;

        float getDeltaTime(){return deltaTime;}
        virtual void tick() = 0;
        
};