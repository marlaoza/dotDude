#pragma once
#include <cstdint>


struct Style {
    float width;
    float height;
    uint32_t color;
};


enum AssetTag {
    IDLE,
    WALK,
    RUN,
    INTERACT,
    DANCE,
    SIT,
    SLEEP,
    HIGHLIGHT
};

struct SpriteAsset { 
    uint8_t id;
    uint8_t frameCount;
    uint8_t frameWidth;
    uint8_t frameHeight;
    uint32_t offset;
    uint32_t size;
    int8_t tags[4];
};

static inline uint8_t latestSpriteID = 0;


