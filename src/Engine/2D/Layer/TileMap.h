#pragma once
#include "Layer.h"
#include <Essentials/Settings.h>
#include <functional>
#include <Graphics/Sprite2D.h>

typedef std::function<void(Sprite2D*)> AddSpriteCallback;

class TileMap : public Layer
{
private:
    ShaderProgram* pSpriteShader;
	std::vector<Sprite2D*> vSprites;
    AddSpriteCallback pAddSpriteCallback;

    void initShader();
	
public:
	TileMap();
	~TileMap();


	void render() override;
 
	Sprite2D* addSprite(Sprite2D* obj);
	void removeSprite(Sprite2D *obj);

    //Setter
    void onAddSprite(AddSpriteCallback callback);

    //Getter
	Sprite2D* getSprite(const std::string& name);
	Sprite2DInstance* getInstanceByID(const unsigned int& id);
	bool hasSprite(const std::string& name);
	unsigned int numSprites();
};