#include "Animation.h"
#include <Essentials/Time.h>

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
    unsigned int nextFrameIndex = currentFrameIndex + 1;

    if(nextFrameIndex > Frames.size() - 1)
      nextFrameIndex = (unsigned int)Frames.size() - 1;

    Frame *lastFrame = Frames[currentFrameIndex];
    Frame *nextFrame = Frames[nextFrameIndex];

    float factor = getFactorBetweenLastAndNextFrame(lastFrame, nextFrame);

    interpolatedFrame->position = vec3::mix(lastFrame->position, nextFrame->position, factor);
    
    
    time += Time::getFrametime();
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