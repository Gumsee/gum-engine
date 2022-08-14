#pragma once
#include <string>
#include <Maths/vec.h>

//Here are two convenience functions, they will become very convenient later on.
//IMPORTANT!!!! The 0.041666666667 float in these functions is equal to
//    1.0 / 24.0, which is how many frames there are in a second, as the software
//    I am using (Blender 3D), plays SkeletalAnimations at 24 frames per second.

//This first function converts from a time passed in (in seconds) to the number
//of frames to which it corresponds.
/*static float TimeToFrame(float time)
{
    float frame = 0.041666666667;
    return time / frame;
}*/

//This next function converts from a start frame and an end frame passed in to
//a start time and an end time, both of which are stored in a vec2.
static vec2 FramesToTime(vec2 frames)
{
    float frame = 0.041666666667;
    return frames * frame;
}

class SkeletalAnimation
{
public:
    std::string name;
    float speed, YOffset;
    float start_time;  //The start time of the SkeletalAnimation (in seconds)
    float end_time;    //The end time of the SkeletalAnimation (in seconds)
    int priority;      //The priority of this SkeletalAnimation, if another
                    //SkeletalAnimation is being played and we try to play
                    //this one, this one will only play if it has
                    //a lower priority than the one being played,
                    //this way, the more important SkeletalAnimations are
                    //always the ones selected to be played.

    SkeletalAnimation()
    {
        start_time = end_time = priority = 0; //Set everything to zero...
    }

    SkeletalAnimation(std::string in_name, vec2 times, int in_priority, float speed, float YOffset)
    {
        this->speed = speed;
        name = in_name;

        vec2 frametimes = FramesToTime(times);
        start_time = frametimes.x;
        end_time = frametimes.y;
        priority = in_priority;
        this->YOffset = YOffset;
    }
};