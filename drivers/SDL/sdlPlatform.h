#include "platform.h"
#include <SDL_render.h>
#include <fstream>
#include <iostream>
#include <map>

#define PITCH 2
struct CachedTexture {
    SDL_Texture* texture;
    uint64_t lastUsedTick; 
};

class SDLPlatform : public IPlatform {
    private:
        SDL_Renderer* renderer;
        std::map<int, CachedTexture> textureCache;

        uint64_t lastTime = 0;
    public:
        SDLPlatform(SDL_Renderer* renderer){
            this->renderer = renderer;
        };

        DudeFile unpackDude(const char* path) {
            DudeFile dudeFile;
            char filePath[256];
            getFullPath(filePath, sizeof(filePath), path);
            dudeFile.filePath = path;
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Error - could not open .dude file :  " << path << std::endl;
                return dudeFile;
            }

            file.read(reinterpret_cast<char*>(&dudeFile.header), sizeof(Header));

            if (std::strncmp(dudeFile.header.identifier, HEADER_ID, 4) != 0) {
                std::cerr << "Error - invalid filetype" << std::endl;
                return dudeFile;
            }

            file.read(reinterpret_cast<char*>(&dudeFile.data), sizeof(CharData));

            std::vector<Entry> entries;
            for (int i = 0; i < dudeFile.header.animCount; ++i) {
                Entry animPayload;
                file.read(reinterpret_cast<char*>(&animPayload), sizeof(Entry));
                entries.push_back(animPayload);
            }
            dudeFile.entries = entries;

            file.close();
            return dudeFile;
        };

        void getImage(std::vector<uint8_t> &buffer, const char* file, SpriteAsset anim) {
            char filePath[256];
            getFullPath(filePath, sizeof(filePath), file);
            std::ifstream f(filePath, std::ios::binary);
            
            f.seekg(anim.offset); 
            f.read(reinterpret_cast<char*>(buffer.data()), anim.size); 
            
        };

        bool updateCharacterData(Character* c) {};

        ~SDLPlatform() {
            for (auto it = textureCache.begin(); it != textureCache.end(); ) {
                SDL_DestroyTexture(it->second.texture);
                it = textureCache.erase(it);
            }
            textureCache.clear();
        };

        void drawFrame(const char* file, SpriteAsset* anim, int frame, int x, int y) {
            SDL_Texture* tex = nullptr;

            auto it = textureCache.find(anim->id);
            if (it == textureCache.end()) {
                int totalWidth = anim->frameWidth * anim->frameCount;

                std::vector<uint8_t> spriteData(anim->size);
                this->getImage(spriteData, file,*anim);
                if (spriteData.size() <= 0) return;

                SDL_Surface* surface = SDL_CreateSurfaceFrom(
                    totalWidth, 
                    anim->frameHeight, 
                    SDL_PIXELFORMAT_RGB565, 
                    spriteData.data(), 
                    totalWidth * PITCH   
                );

                const SDL_PixelFormatDetails* formatDetails = SDL_GetPixelFormatDetails(surface->format);
                Uint32 colorkey = SDL_MapRGB(formatDetails, nullptr, 255, 0, 255);
                SDL_SetSurfaceColorKey(surface, true, colorkey);

                SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_DestroySurface(surface);
                SDL_SetTextureScaleMode(newTexture, SDL_SCALEMODE_NEAREST);

                textureCache[anim->id] = {newTexture, SDL_GetTicks()};
            }

            CachedTexture& cached = textureCache[anim->id];
            cached.lastUsedTick = SDL_GetTicks();

            SDL_FRect srcRect = {
                (float)(frame * anim->frameWidth),
                0.0f,  
                (float)(anim->frameWidth), 
                (float)(anim->frameHeight)
            };

            SDL_FRect destRect = {
                (float)(x), 
                (float)(y) - (float)(anim->frameHeight), 
                (float)(anim->frameWidth), 
                (float)(anim->frameHeight)
            };

            SDL_RenderTexture(renderer, cached.texture, &srcRect, &destRect);
        };

        void drawRect(Style style, int x, int y) {
            uint8_t r = (style.color >> 16) & 0xFF;
            uint8_t g = (style.color >> 8) & 0xFF;
            uint8_t b = style.color & 0xFF;
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);    
            SDL_FRect rect = { (float)(x), (float)(y), style.width, style.height };
            SDL_RenderFillRect(renderer, &rect);
        };

        void tick(){
            uint64_t currentTime = SDL_GetTicks();
            uint64_t elapsedTime = currentTime - lastTime;

            deltaTime = static_cast<float>(elapsedTime) / 1000.0f;
            if (deltaTime > 0.1f) deltaTime = 0.1f;
            lastTime = currentTime;

            for (auto it = textureCache.begin(); it != textureCache.end(); ) {
                if (currentTime - it->second.lastUsedTick > 60000) {
                    SDL_DestroyTexture(it->second.texture);
                    it = textureCache.erase(it);
                } else {
                    ++it;
                }
            }

        }


};