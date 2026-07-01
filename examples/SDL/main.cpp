#include <SDL3/SDL.h>

#include "world.h"
#include "sdlPlatform.h"

SDL_Window *window;
SDL_Renderer* renderer;

int quit(int code) {
    if(window) SDL_DestroyWindow(window);
    if(renderer) SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return code;
}

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL EXAMPLE", 200, 100, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if(window == NULL){return quit(EXIT_FAILURE);}

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowMinimumSize(window, 200, 100);

    renderer = SDL_CreateRenderer(window, NULL);
    if(renderer == NULL){ return quit(EXIT_FAILURE); }

    SDL_Event windowEvent;

    SDLPlatform* platform = new SDLPlatform(renderer);
    World* world = new World(platform);

    platform->setRootPath("../../assets");
    world->addCharacter("sample.dude", 0, 100);
 
    while(true){
        while(SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_EVENT_QUIT){return quit(EXIT_SUCCESS);}
            if(windowEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if (windowEvent.button.button == SDL_BUTTON_LEFT || windowEvent.button.button == SDL_BUTTON_RIGHT){
                    int o = 0;
                    for(Character* c :  world->getCharacters()){
                        c->moveTo(windowEvent.button.x + o, windowEvent.button.y, windowEvent.button.button == SDL_BUTTON_RIGHT);
                        o++;
                    }
                }   
            }
        }
        float dt = platform->getDeltaTime();
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        world->tick(dt);
        SDL_RenderPresent(renderer);
    }
    
    return quit(EXIT_SUCCESS);
}


