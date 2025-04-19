#if !defined(__MINGW32__) && !defined(__SWITCH__)

#include "Discord.h"
#include <time.h>
#include <chrono>
#include <iostream> 

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void Discord::Initialize(const char* applicationId)
{
	std::cout << "Initializing Discord RPC with App ID: " << applicationId << std::endl;
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize(applicationId, &Handle, 1, NULL);
	std::cout << "Discord RPC initialized" << std::endl;
}

void Discord::SetState(const char* state) {
	std::cout << "Setting Discord state: " << state << std::endl;
	currentState = state;
}

void Discord::SetDetails(const char* details) {
	std::cout << "Setting Discord details: " << details << std::endl;
	currentDetails = details;
}

void Discord::SetLargeImage(const char* largeImageKey) {
	std::cout << "Setting Discord large image: " << largeImageKey << std::endl;
	currentLargeImageKey = largeImageKey;
}

void Discord::SetSmallImage(const char* smallImageKey) {
	std::cout << "Setting Discord small image: " << smallImageKey << std::endl;
	currentSmallImageKey = smallImageKey;
}

void Discord::SetSmallImageText(const char* smallImageText) {
	std::cout << "Setting Discord small image text: " << smallImageText << std::endl;
	currentSmallImageText = smallImageText;
}

void Discord::SetLargeImageText(const char* largeImageText) {
	std::cout << "Setting Discord large image text: " << largeImageText << std::endl;
	currentLargeImageText = largeImageText;
}

void Discord::Update()
{
	std::cout << "Updating Discord presence..." << std::endl;
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = currentState;
	discordPresence.details = currentDetails;
	discordPresence.startTimestamp = eptime;
	discordPresence.largeImageKey = currentLargeImageKey;
	discordPresence.largeImageText = currentLargeImageText;
	discordPresence.smallImageKey = currentSmallImageKey;
	discordPresence.smallImageText = currentSmallImageText;
	Discord_UpdatePresence(&discordPresence);
	Discord_RunCallbacks();
}

#endif // __MINGW32__