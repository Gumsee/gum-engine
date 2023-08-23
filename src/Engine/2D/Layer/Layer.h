#pragma once
#include <Essentials/Settings.h>
#include <functional>
#include <Graphics/Sprite2D.h>

class Layer : public Renderable
{
private:
	
public:
	Layer();
	~Layer();

	virtual void render() override {};
};