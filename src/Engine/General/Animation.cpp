#include "Animation.h"
#include <Essentials/FPS.h>

Animation::Animation()
{
    interpolatedFrame = new Frame();
}
Animation::~Animation()
{

}

void Animation::addAnimationFrame(Frame *frame)
{
    this->Frames.push_back(frame);
}

void Animation::setRepeatAnimation(bool isRepeating)
{
    this->repeat = isRepeating;
}

void Animation::updateAnimation()
{
    int nextFrameIndex = currentFrameIndex + 1;

    if(nextFrameIndex > Frames.size() - 1)
    {
        nextFrameIndex = Frames.size() - 1;
    }

    Frame *lastFrame = Frames[currentFrameIndex];
    Frame *nextFrame = Frames[nextFrameIndex];

    float factor = getFactorBetweenLastAndNextFrame(lastFrame, nextFrame);

    interpolatedFrame->position = vec3::mix(lastFrame->position, nextFrame->position, factor);
    
    
    time += FPS::get();
    if(time > Frames[Frames.size() - 1]->time)
    {
        time = 0;
    }
}

float Animation::getFactorBetweenLastAndNextFrame(Frame *lastFrame, Frame *nextFrame)
{
    float Distance = nextFrame->time - lastFrame->time;
    float BetweenTime = this->time - lastFrame->time;

    return BetweenTime / Distance;
}

Animation::Frame *Animation::getInterpolatedFrame()
{
    return interpolatedFrame;
}