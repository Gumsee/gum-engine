#pragma once
#include <Maths/vec.h>
#include <vector>

class Animation
{
public:
    struct Frame
    {
        vec3 position = vec3(0);
        vec3 rotation = vec3(0);
        vec3 scale = vec3(1);

        float time;
    };

    Animation();
    ~Animation();

    void addAnimationFrame(Frame *frame);
    void setRepeatAnimation(bool isRepeating);
    void updateAnimation();

    Frame *getInterpolatedFrame();
private:
    std::vector<Frame*> Frames;

    bool repeat;
    float time;

    unsigned int currentFrameIndex = 0;
    Frame *interpolatedFrame;

    float getFactorBetweenLastAndNextFrame(Frame *lastFrame, Frame *nextFrame);
};