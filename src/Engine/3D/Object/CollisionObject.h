#pragma once

#include <functional>
class CollisionObject
{
private:
    void* internalBody;
    std::function<void()> callback;

public:
    CollisionObject();
    ~CollisionObject();

    void onCollision(std::function<void()> callback);
    void updateOnCollision();

    void setBody(void* body);
    void* getBody();
};