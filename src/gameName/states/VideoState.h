#pragma once

#ifdef __MINGW32__ || defined(__SWITCH__)
#include "../../engine/core/State.h"
#include "../../engine/graphics/VideoPlayer.h"   
#include "../../engine/utils/Log.h"
#else
#include <core/State.h>
#include <graphics/VideoPlayer.h>
#include <utils/Log.h>
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
    bool loadVideo(const std::string& path);
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void setVolume(int volume);

private:
    VideoPlayer* videoPlayer;
    std::string currentVideoPath;
};
