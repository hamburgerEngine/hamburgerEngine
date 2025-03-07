#ifdef __MINGW32__
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
    #else
    Discord::GetInstance().Initialize("1155959383625318492");    
    Discord::GetInstance().SetState("Phlox Engine");
    Discord::GetInstance().SetDetails("Busss...");
    Discord::GetInstance().SetLargeImage("icon");
    Discord::GetInstance().SetLargeImageText("Phlox Engine by YoPhlox");
    Discord::GetInstance().SetSmallImage("tetosigma");
    Discord::GetInstance().SetSmallImageText("I feel so sigma!");    
    Discord::GetInstance().Update();
    #endif
    
    Engine engine(800, 600, "Phlox Engine");
    PlayState* initialState = new PlayState();
    engine.pushState(initialState);
    
    engine.run();
    
    #ifdef __MINGW32__
    // nun
    #else
    Discord::GetInstance().Shutdown();
    #endif
    return 0;
}
