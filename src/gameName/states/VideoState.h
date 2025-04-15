#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/State.h"
#include "../../engine/VideoPlayer.h"   
#include "../../engine/Log.h"
#else
#include <State.h>
#include <VideoPlayer.h>
#include <Log.h>
#endif

class VideoState : public State {
public:
    static VideoState* instance;
    
    VideoState();
    ~VideoState() override;
    
    void create() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;
    
    void openSubState(SubState* subState) override;

private:
    VideoPlayer* videoPlayer;
};
