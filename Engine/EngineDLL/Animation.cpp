#include "Animation.h"
#include "Sprite.h"

Animation::Animation(Sprite* sprite, unsigned int frames[], bool isLooping, float frameRate)
	: sprite(sprite), isLooping(isLooping), isFinished(true), frameRate(1.0f / frameRate)
{
	this->frames = new queue<unsigned int>;

	SetNewAnimation(frames);

	Play();
}
Animation::~Animation()
{
	if (frames) delete frames;
}

void Animation::Play()
{
	animationTime = 0.0f;
	isFinished = false;
}

void Animation::Update()
{
	animationTime += Defs::getInstance()->deltaTime;

	if (animationTime > frameRate)
	{
		animationTime = 0.0f;
		if (!isFinished)
		{
			actualFrame = frames->front();
			frames->pop();
			frames->push(actualFrame);
			sprite->SetNewFrame(actualFrame);

			if (actualFrame == lastFrame && !isLooping)
				isFinished = true;
		}
	}
}

void Animation::SetNewAnimation(unsigned int frames[])
{
	unsigned int size = sizeof(frames);

	while (!this->frames->empty())
		this->frames->pop();

	for (int i = 0; i <= size; i++)
		this->frames->push(frames[i]);

	actualFrame = this->frames->front();
	lastFrame = this->frames->back();

	sprite->SetNewFrame(actualFrame);
}