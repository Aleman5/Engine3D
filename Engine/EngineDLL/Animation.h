#pragma once

#include <queue>
#include "Exports.h"
#include "Definitions.h"

using namespace std;

class Sprite;

class ENGINEDLL_API Animation
{
	Sprite* sprite;				// Sprite reference

	unsigned int actualFrame;	// Frame rendered
	unsigned int lastFrame;		// Las frame of the Animation

	float animationTime;		// Time left to change to the next frame
	float frameRate;			// Time per frame change

	bool isLooping;				// Is looping? Yes/No
	bool isFinished;			// Is finished? Yes/No

	queue<unsigned int>* frames;// Frames of the animation

public:
	void Play();				// Starts playing
	void Update();				// Updates the actual frame

	// Used if the sprite is animated
	void SetNewAnimation(		// Sets the Animation that will be renderered
		unsigned int frames[]	// Frames of the new Animation
	);

	void SetIsLoop(bool isLooping) { this->isLooping = isLooping; if (isLooping) isFinished = false; };
	bool IsFinished() { return isFinished; };

	Animation(
		Sprite* sprite,			// Sprite reference
		unsigned int frames[],	// Frames of the animation
		bool isLooping,			// Is looping? Yes/No
		float frameRate			// Time per frame change
	);
	~Animation();
};