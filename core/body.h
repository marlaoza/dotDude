#pragma once
#include "asset.h"
#include "platform.h"
#include <cstdlib>
#include <cstring>
#include <string>

class Body {
    private:
        
        float x;
        float y;
        SpriteAsset* currentSprite;

        std::vector<SpriteAsset> spriteList;

        int currentFrame;
        float frameTimer = 0.0f;
    public:
        const char* file;
        Body(
            const char* file,
            float x, float y, 
            std::vector<SpriteAsset> spriteList) : 
            x(x), y(y), 
            spriteList(spriteList),
            currentFrame(0)
        {
            this->file = file;
            setAnimation(IDLE);
            if (!spriteList.empty()) {currentSprite = &spriteList[0]; currentFrame = 0;}};

        void setSpriteList(std::vector<SpriteAsset>& list){
            spriteList = list;
            if (!spriteList.empty()) {currentSprite = &spriteList[0]; currentFrame = 0;}
        }

        SpriteAsset* getCurrentSprite(){return this->currentSprite;}

        int getCurrentFrame(){return currentFrame;}
        void passFrame(float deltaTime, float targetFPS = 12.0f){
            if(!currentSprite || currentSprite->frameCount <= 1) return;
            frameTimer += deltaTime;
            float timePerFrame = 1.0f / targetFPS;
            while (frameTimer >= timePerFrame) {
                currentFrame = (currentFrame + 1) % currentSprite->frameCount;
                frameTimer -= timePerFrame;
            }
        }
        
        void getSize(int& width, int& height){
            width = 1;
            height = 1;
            if(currentSprite){
                width = currentSprite->frameWidth;
                height = currentSprite->frameHeight;
                return;
            }
            return;
            
        }

        void setPosition(float x, float y) {
            this->x = x; 
            this->y = y;
        }
        void getPosition(float& x, float&y) {x = this->x; y = this->y;}

        void setAnimation(int tag){
            if(spriteList.size() <= 0){return;}
            SpriteAsset* candidates[8];
            int c = 0;
            for(SpriteAsset& s : this->spriteList){
                for(short  t : s.tags){if(tag != -1) {if(tag == t){candidates[c] = &s; c++;}}}
            }

            if(c > 0){
                int randomIndex = std::rand() % c;
                if (currentSprite != candidates[randomIndex]) {
                    currentSprite = candidates[randomIndex];
                    currentFrame = 0;
                }
            }
            
            else{
                if(tag == (short)RUN){setAnimation(WALK);}
                else if(tag != (short)IDLE){setAnimation(IDLE);}
                else{
                    currentSprite = &spriteList[0]; 
                    currentFrame = 0;
                }
            }
        }
};