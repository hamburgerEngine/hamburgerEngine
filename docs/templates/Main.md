# Main

Basically shit for main.cpp, nothing much to write here but I'ma leave this:

```cpp
#ifdef __MINGW32__
#include "engine/Engine.h"
#include "gameName/states/PlayState.h"
#include "engine/Input.h"
#elif defined(__SWITCH__)
#include "engine/Engine.h"
#include "gameName/states/PlayState.h"
#include "engine/Input.h"
#include <switch.h>
#else
#include <Engine.h>
#include <states/PlayState.h>
#include <Input.h>
#include <Discord.h>
#endif

int main(int argc, char** argv) {
    #ifdef __MINGW32__
    // nun
    #elif defined(__SWITCH__)
    // nun
    #else
    Discord::GetInstance().Initialize("1347011960088035368"); // Discord APPID  
    Discord::GetInstance().SetState("hamburger engine"); // Discord State
    Discord::GetInstance().SetDetails("Probably Testing Stuff!"); // Discord Details
    Discord::GetInstance().SetLargeImage("hamburger"); // Discord Large Image
    Discord::GetInstance().SetLargeImageText("hamburger engine by YoPhlox & MaybeKoi"); // Discord Large Image Text
    Discord::GetInstance().SetSmallImage("miku"); // Discord Small Image
    Discord::GetInstance().SetSmallImageText("HOLY SHIT IS THAT HATSUNE MIKU!?"); // Discord Small Image Text
    Discord::GetInstance().Update();
    #endif
    
    Engine engine(1280, 720, "hamburger engine", 60); // Window Width, Window Height, Window Name, Target FPS
    engine.debugMode = false; // Shows the FPS, Memory, and RAM in the top left corner if set to true
    PlayState* initialState = new PlayState(); // The initial state that you want to go to.
    engine.pushState(initialState);
    
    #ifdef __SWITCH__
    while (appletMainLoop()) {
        Input::UpdateControllerStates();
        if (Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_BACK) || 
            Input::isControllerButtonPressed(SDL_CONTROLLER_BUTTON_START)) {
            engine.quit();
            break;
        }
        engine.run();
    }
    #else
    engine.run();
    #endif
    
    #ifdef __MINGW32__
    // nun
    #elif defined(__SWITCH__)
    // nun
    #else
    Discord::GetInstance().Shutdown();
    #endif
    return 0;
}
