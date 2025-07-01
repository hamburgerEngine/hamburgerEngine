#pragma once

#if !defined(__MINGW32__) && !defined(__SWITCH__)

#include <discord_register.h> 
#include <discord_rpc.h> 
#ifdef _WIN32
#include <Windows.h> 
#endif

class Discord {
public:
	static Discord& GetInstance() {
		static Discord instance;
		return instance;
	}
	void Initialize(const char* applicationId);
	void Update();
	void Shutdown() {
		Discord_Shutdown();
	}
	void SetState(const char* state);
	void SetDetails(const char* details);
	void SetLargeImage(const char* largeImageKey);
	void SetLargeImageText(const char* largeImageText);
	void SetSmallImage(const char* smallImageKey);
	void SetSmallImageText(const char* smallImageText);

private:
	Discord() {} 
	Discord(const Discord&) = delete;
	Discord& operator=(const Discord&) = delete;
	
	const char* currentState = "";
	const char* currentDetails = "";
	const char* currentLargeImageKey = "";
	const char* currentLargeImageText = "";
	const char* currentSmallImageKey = "";
	const char* currentSmallImageText = "";
};

#else

// Stub implementation for non-Windows platforms
class Discord {
public:
	static Discord& GetInstance() {
		static Discord instance;
		return instance;
	}
	void Initialize(const char* applicationId) {}
	void Update() {}
	void Shutdown() {}
	void SetState(const char* state) {}
	void SetDetails(const char* details) {}
	void SetLargeImage(const char* largeImageKey) {}
	void SetLargeImageText(const char* largeImageText) {}
	void SetSmallImage(const char* smallImageKey) {}
	void SetSmallImageText(const char* smallImageText) {}

private:
	Discord() {} 
	Discord(const Discord&) = delete;
	Discord& operator=(const Discord&) = delete;
};

#endif