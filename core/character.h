#pragma once
#include "body.h"
#include <cstdlib>
#include "vector"
#include <iostream>

enum CharacterState {
    STATE_IDLE,
    STATE_MOVING
};

struct CharacterData {
    char name[32];
    uint8_t baseSpeed;
    uint8_t runSpeed;
    uint8_t speed;
};

class Character {
    private:
        const char* filePath;

        CharacterData data;

        CharacterState state;
        float targetX;
        float targetY;

    public:
        Character(const char* filePath, CharacterData data, std::vector<SpriteAsset> spriteList, float x, float y) 
            : body(filePath, x, y, spriteList)
        {
            this->filePath = filePath;
            this->data = data;
            targetX = 0;
            targetY = 0;
            setState(STATE_IDLE);
        }

        Character(DudeFile file, float x, float y) : body(file.filePath, 0, 0, {}){
            std::vector<SpriteAsset> spriteList;
            this->filePath = file.filePath;
            std::strncpy(data.name, file.data.name, 32);
            data.name[31] = '\0'; 
            data.baseSpeed = file.data.baseSpeed;
            data.runSpeed = file.data.runSpeed;
            data.speed = data.baseSpeed;
            std::cout << "name: " << data.name << " | base: " << (int)data.baseSpeed << " | run: " << (int)data.runSpeed << std::endl;
            std::cout << "sprites" << std::endl;
            for(Entry e : file.entries){
                SpriteAsset asset;
                asset.id = latestSpriteID++; 
                asset.frameCount = e.frameCount;
                asset.frameWidth = e.frameWidth;
                asset.frameHeight = e.frameHeight;
                asset.offset = e.dataOffset;
                asset.size = e.dataSize;
                
                std::cout << "frameCount: " << (int)asset.frameCount << " | frameSize: " << (int)asset.frameWidth << "x" << (int)asset.frameHeight << std::endl;            
                for (int t = 0; t < 4; ++t) {asset.tags[t] = e.tags[t];}
                std::cout << "tags: " << (int)asset.tags[0] << "|" << (int)asset.tags[1] << "|" << (int)asset.tags[2] << "|" << (int)asset.tags[3] << std::endl;

                spriteList.push_back(asset);
            }

            this->body.setSpriteList(spriteList);

            targetX = 0;
            targetY = 0;
            setState(STATE_IDLE);
            body.setPosition(x, y);
        }

        ~Character();

        Body body;

        CharacterState getState() {return this->state;}
        void setState(CharacterState state){
            this->state = state;
            if(state == CharacterState::STATE_MOVING){
                if(data.speed == data.runSpeed) body.setAnimation(RUN);
                else body.setAnimation(WALK);
            }else{
                body.setAnimation(IDLE);
            }
        }
        
        void moveTo(float x, float y, bool run){
            this->data.speed = run ? data.runSpeed : data.baseSpeed;
            setState(STATE_MOVING);
            
            this->targetX = x;
            this->targetY = y;
        }
       
        int getSpeed(){return this->data.speed;}
        void getTargetPosition(float& x, float&y) {x = this->targetX; y = this->targetY;}
};
