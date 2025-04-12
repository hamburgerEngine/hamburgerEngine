#ifdef __MINGW32__
#include "engine/Engine.h"
#include "gameName/states/PlayState.h"
#elif defined(__SWITCH__)
#include "engine/Engine.h"
#include "gameName/states/PlayState.h"
#else
#include <Engine.h>
#include <states/PlayState.h>
#include <Discord.h>
#endif

#include <GL/glut.h>

int main(int argc, char** argv) {
    #ifdef __MINGW32__
    // nun
    #elif defined(__SWITCH__)
    // nun
    #else
    Discord::GetInstance().Initialize("1347011960088035368");    
    Discord::GetInstance().SetState("hamburger engine");
    Discord::GetInstance().SetDetails("Probably Testing Stuff!");
    Discord::GetInstance().SetLargeImage("hamburger");
    Discord::GetInstance().SetLargeImageText("hamburger engine by YoPhlox & MaybeKoi");
    Discord::GetInstance().SetSmallImage("miku");
    Discord::GetInstance().SetSmallImageText("HOLY SHIT IS THAT HATSUNE MIKU!?");    
    Discord::GetInstance().Update();
    #endif
    
    Engine engine(800, 600, "hamburger engine");
    PlayState* initialState = new PlayState();
    engine.pushState(initialState);
    
    engine.run();
    
    #ifdef __MINGW32__
    // nun
    #elif defined(__SWITCH__)
    // nun
    #else
    Discord::GetInstance().Shutdown();
    #endif
    return 0;
}
