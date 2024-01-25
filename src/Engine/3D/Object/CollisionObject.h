#pragma once

#include <functional>
class CollisionObject
{
private:
    void* internalBody;
    void* userptr;
    std::function<void()> callback;

public:
    CollisionObject(void* body = nullptr);
    ~CollisionObject();

    void onCollision(std::function<void()> callback);
    void updateOnCollision();

    void setBody(void* body);
    void* getBody();

    void setUserPtr(void* ptr);
    void* getUserPtr();
};