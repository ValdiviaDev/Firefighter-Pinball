#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
typedef unsigned int FX;

#define DEFAULT_MUSIC_FADE_TIME 2.0f


struct GameSounds {
	FX flipperUp = 0;
	FX flipperDown = 0;
	FX bigBumper = 0;
	FX cat = 0;
	FX liftUp = 0;
	FX lightBallSensor = 0;
	FX lightStarSensor = 0;
	FX sirenHose = 0;
	FX smallBumper1 = 0;
	FX smallBumper2 = 0;
	FX springDown = 0;
	FX springLaunch = 0;
	FX deathSound = 0;
};

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool Start();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	//Charge Sound Effects
	void ChargeFX();

	//Get a Sound Effect
	GameSounds GetFX();

private:

	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;

	GameSounds gameSounds;
};

#endif // __ModuleAudio_H__