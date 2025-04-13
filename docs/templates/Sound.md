# Sound System

## Usage

```cpp
// Basic sound playback
SoundManager& audio = SoundManager::getInstance();

// Play background music
audio.playMusic("assets/music/song.ogg", 0.8f);  // Play at 80% volume
audio.pauseMusic();   // Pause current music
audio.resumeMusic();  // Resume paused music
audio.stopMusic();    // Stop music completely

// Play sound effects
audio.playSound("assets/sounds/jump.ogg", 1.0f);  // Play at full volume

// Manual sound control
Sound* sound = audio.loadSound("assets/sounds/explosion.ogg");
if (sound) {
    sound->setVolume(0.5f);    // Set to 50% volume
    sound->setLoop(true);      // Make the sound loop
    sound->play();             // Start playback
    sound->pause();            // Pause playback
    sound->resume();           // Resume playback
    sound->stop();             // Stop playback
    
    if (sound->isPlaying()) {
        // Sound is currently playing
    }
}
```

## Setting Up Sound

The SoundManager automatically initializes OpenAL when first accessed. All sounds are loaded and cached for reuse:

```cpp
// Get the sound manager instance
SoundManager& audio = SoundManager::getInstance();

// Load and cache a sound for repeated use
Sound* hitSound = audio.loadSound("assets/sounds/hit.ogg");
Sound* jumpSound = audio.loadSound("assets/sounds/jump.ogg");

// Play the cached sounds
hitSound->play();
jumpSound->play();
```

## Available Properties

The Sound class provides several methods to control playback:

- `play()`: Start playing the sound
- `pause()`: Pause playback
- `resume()`: Resume paused playback
- `stop()`: Stop playback completely
- `setVolume(float)`: Set volume (0.0f to 1.0f)
- `setLoop(bool)`: Enable/disable looping
- `loopMusic(string)`: Enable/disable looping for music
- `isPlaying()`: Check if sound is currently playing

```cpp
Sound* music = audio.loadSound("assets/music/background.ogg");
music->setVolume(0.7f);     // Set to 70% volume
music->setLoop(true);       // Enable looping
music->play();              // Start playback

if (music->isPlaying()) {
    music->setVolume(0.5f); // Lower volume while playing
}
```

The SoundManager provides high-level control for music and sound effects:

- `playMusic(path, volume)`: Play background music
- `pauseMusic()`: Pause current music
- `resumeMusic()`: Resume paused music
- `stopMusic()`: Stop current music
- `setMusicVolume(float)`: Adjust music volume
- `loadSound(path)`: Load and cache a sound
- `playSound(path, volume)`: Play a sound effect