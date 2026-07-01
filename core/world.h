#pragma once
#include "character.h"
#include "object.h"
#include "platform.h"

class World {
    private:
        std::vector<Character*> characters;

        std::vector<Object*> foregroundObjects;
        std::vector<Object*> objects;

        IPlatform* platform;
    public:
        World(IPlatform* platform) : platform(platform) {}
        ~World(){
            for(Character* c : characters) delete c;
            characters.clear();
            for(Object* o : foregroundObjects) delete o;
            foregroundObjects.clear();
            for(Object* o : objects) delete o;
            objects.clear();
        }
        void addObject(Object* object){
            if(object->isForeground){foregroundObjects.push_back(object);}
            else{objects.push_back(object);}
        }

        void addCharacter(Character* character, float x, float y) {
            character->body.setPosition(x, y);
            characters.push_back(character);
        }
        void addCharacter(const char* filePath, float x, float y){
            Character* character = new Character(platform->unpackDude(filePath), x, y);
            characters.push_back(character);
        }

        std::vector<Character*> getCharacters() {return this->characters;}
     
        bool checkCollision(float x, float y, int width, int height){
            for (Object* obj : objects) {
                int objWidth, objHeight;
                obj->body.getSize(objWidth, objHeight);
                float objX, objY;
                obj->body.getPosition(objX, objY);
                if (x < objX + objWidth &&
                    x + width > objX &&
                    y < objY + objHeight &&
                    y + height >objY) {
                    if (obj->isInteractable){obj->body.setAnimation(HIGHLIGHT);}
                    if (obj->isSolid) {return true;}
                }
            }
            return false;
        }

        Object* checkInteraction(float x, float y, int width, int height){
            for (Object* obj : objects) {
                if (obj->isInteractable) {
                    int objWidth, objHeight;
                    obj->body.getSize(objWidth, objHeight);
                    float objX, objY;
                    obj->body.getPosition(objX, objY);
                    
                     if (x < (objX - 3) + (objWidth + 6) &&
                        x + width > (objX - 3) &&
                        y < (objY - 3) + (objHeight + 6) &&
                        y + height > (objY - 3)) {
                        return obj;
                    }
                }
            }
            return nullptr;
        }

        void interact(Character* c){
            float x, y;
            c->body.getPosition(x, y);
            int width, height;
            c->body.getSize(width, height);

            Object* o = checkInteraction(x, y, width, height);
            if(o != nullptr &&  o->onInteraction){
                o->body.setAnimation(INTERACT);
                c->body.setAnimation(INTERACT);
                o->onInteraction(c); 
            }
            
        }
        void draw(Body& b, float deltaTime){
            float x, y;
            b.getPosition(x,y);
            platform->drawFrame(b.file, b.getCurrentSprite(), b.getCurrentFrame(), (int)x, (int)y);
            b.passFrame(deltaTime);
        }
        void tick(float deltaTime){
            for(Object* o : objects){draw(o->body, deltaTime);}

            for (Character* c : characters){
                if(c->getState() == STATE_MOVING){
                    float curX, curY; c->body.getPosition(curX, curY);
                    float targetX, targetY; c->getTargetPosition(targetX, targetY);
                    float deltaX, deltaY;
                    if(curX > targetX) {deltaX = curX - targetX; } else {deltaX = targetX - curX;}
                    if(curY > targetY) {deltaY = curY - targetY; } else {deltaY = targetY - curY;}

                    if(deltaX < 0.1f && deltaY < 0.1f){c->setState(STATE_IDLE);}
                    else{
                        int width, height;
                        c->body.getSize(width, height);
                        float dx = targetX - curX;
                        float dy = targetY - curY;

                        int dirX = (dx > 0) - (dx < 0); 
                        int dirY = (dy > 0) - (dy < 0);

                        float stepSize = c->getSpeed() * deltaTime;
                        float moveX = (std::abs(dx) < stepSize) ? std::abs(dx) * dirX : stepSize * dirX;
                        float moveY = (std::abs(dy) < stepSize) ? std::abs(dy) * dirY : stepSize * dirY;
                        
                        bool canMoveX = false; bool canMoveY = false;
                        if (dirX != 0) {
                            float nextX = curX + moveX;
                            if (!checkCollision(nextX, curY, width, height)) {
                                c->body.setPosition(nextX, curY);
                                curX = nextX; 
                                canMoveX = true;
                            }
                        }
                        if (dirY != 0) {
                            float nextY = curY + moveY;
                            if (!checkCollision(curX, nextY, width, height)) {
                                c->body.setPosition(curX, nextY);
                                canMoveY = true;
                            }
                        }
                       
                        if(!canMoveX && !canMoveY){c->setState(STATE_IDLE);}
                    }
                    
                }
                draw(c->body, deltaTime);                
            }

            for(Object* o : foregroundObjects){draw(o->body, deltaTime);}

            platform->tick();
        }
};