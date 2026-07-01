#pragma once
#include "asset.h"
#include "functional"
#include "character.h"
#include "optional"

struct Object {
    Body body;
    bool isForeground;
    bool isSolid;
    bool isInteractable;
    std::function<void(Character* actor)> onInteraction;
};
