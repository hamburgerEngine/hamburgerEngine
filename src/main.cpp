#include <Engine.h>
#include <states/PlayState.h>
#include <GL/glut.h>
#include <Discord.h>

int main(int argc, char** argv) {
    Discord::GetInstance().Initialize("1155959383625318492");
    
    Discord::GetInstance().SetState("Phlox Engine");
    Discord::GetInstance().SetDetails("Busss...");
    Discord::GetInstance().SetLargeImage("icon");
    Discord::GetInstance().SetLargeImageText("Phlox Engine by YoPhlox");
    Discord::GetInstance().SetSmallImage("tetosigma");
    Discord::GetInstance().SetSmallImageText("I feel so sigma!");    
    Discord::GetInstance().Update();
    
    Engine engine(800, 600, "Phlox Engine");
    PlayState* initialState = new PlayState();
    engine.pushState(initialState);
    
    engine.run();
    
    Discord::GetInstance().Shutdown();
    return 0;
}
