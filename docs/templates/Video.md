yahuhayhyah

```cpp
// state.h
private:
    VideoPlayer* videoPlayer;

// state.cpp
    void create() override {
        videoPlayer = new VideoPlayer();
        if (videoPlayer->load("assets/videos/test.mp4")) {
            videoPlayer->setWindowSize(engine->getWindowWidth(), engine->getWindowHeight());
            videoPlayer->setMaintainAspectRatio(false);
            videoPlayer->play();
            Log::getInstance().info("Video playback started");
        } else {
            Log::getInstance().error("Failed to load video");
        }
    }

    void update(float deltaTime) override {
        videoPlayer->update(deltaTime);
    }

    void render() override {
        videoPlayer->render(SDLManager::getInstance().getRenderer());
    }

    void destroy() override {
        delete videoPlayer;
    }
```