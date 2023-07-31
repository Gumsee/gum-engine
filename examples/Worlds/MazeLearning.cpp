#include <Engine.h>
#include <Globals.h>
#include <MachineLearning/QLearning.h>
#include <MachineLearning/MachineLearningFunctions.h>
#include <unistd.h>
#include "../Examples.h"

Box *pPlayer;
int currentTile = 0;
int numRows = 0;
int numColumns = 0;
int numEpisodes = 1000;
int maxNumSteps = 100;
float boxSize = 100.0f;
std::vector<TextBox*> vMazeGrid;
GumQLearning *pQLearning;


#define WALL_REWARD "-1"
#define FREE_REWARD "0"
#define GOAL_REWARD "100"
#define START_REWARD "0"


int recalculateCurrentTile()
{
	for(int i = 0; i < vMazeGrid.size(); i++)
	{
		if(vMazeGrid[i]->collidesWith(pPlayer))
		{
			currentTile = i;
			break;
		}
	}
	return currentTile;
}

bool checkIfPositionInvalid()
{
	for(auto tile : vMazeGrid)
	{
		if(tile->collidesWith(pPlayer) && tile->getType() == WALL_REWARD)
		{
			return true;
		}
	}
	return false;
}

bool checkIfPlayerReachedGoal()
{
	for(auto tile : vMazeGrid)
	{
		if(tile->collidesWith(pPlayer) && tile->getType() == GOAL_REWARD)
		{
			return true;
		}
	}
	return false;
}

void updateArrowColors()
{
	for(auto table : *pQLearning->getQTable())
	{
		int guiChildIndex = 0;
		if(table.first == "Up") 		{ guiChildIndex = 1; }
		else if(table.first == "Down") 	{ guiChildIndex = 2; }
		else if(table.first == "Left") 	{ guiChildIndex = 3; }
		else if(table.first == "Right") { guiChildIndex = 4; }
		for(int i = 0; i < table.second.size(); i++)
		{
			Box* child = (Box*)vMazeGrid[i]->getChild(guiChildIndex);
			child->setColor(vec4(0, 1, 0, table.second[i] / GumMLFunctions::maxActionValue(*pQLearning->getQTable(), table.second[i])));
		}
	}
}


int actionMoveUp()
{
	vec2 oldpos = pPlayer->getPosition();
	pPlayer->setPosition(vec2(pPlayer->getPosition().x, pPlayer->getPosition().y - boxSize));
	if(pPlayer->getPosition().y < 0 || checkIfPositionInvalid())
		pPlayer->setPosition(oldpos);
	return recalculateCurrentTile();
}

int actionMoveDown()
{
	vec2 oldpos = pPlayer->getPosition();
	pPlayer->setPosition(vec2(pPlayer->getPosition().x, pPlayer->getPosition().y + boxSize));
	if(pPlayer->getPosition().y > numRows * boxSize || checkIfPositionInvalid())
		pPlayer->setPosition(oldpos);
	return recalculateCurrentTile();
}

int actionMoveLeft()
{
	vec2 oldpos = pPlayer->getPosition();
	pPlayer->setPosition(vec2(pPlayer->getPosition().x - boxSize, pPlayer->getPosition().y));
	if(pPlayer->getPosition().x < 0 || checkIfPositionInvalid())
		pPlayer->setPosition(oldpos);
	return recalculateCurrentTile();
}

int actionMoveRight()
{
	vec2 oldpos = pPlayer->getPosition();
	pPlayer->setPosition(vec2(pPlayer->getPosition().x + boxSize, pPlayer->getPosition().y));
	if(pPlayer->getPosition().x > numColumns * boxSize || checkIfPositionInvalid())
		pPlayer->setPosition(oldpos);
	return recalculateCurrentTile();
}

World3D* Examples::createMazeLearningExample()
{
    World3D* pWorld3D = new World3D(Gum::Window->getSize());
	pWorld3D->getObjectManager()->getSkybox()->useGradiant(true);

	GumEngine::ActiveCamera->setMode(GumEngine::ActiveCamera->STATIC);
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, 10, 0),  vec3(300), "light1"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, 10, 0),   vec3(300), "light2"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(-10, -10, 0), vec3(300), "light3"));
	pWorld3D->getLightManager()->addPointLight(new PointLight(vec3(10, -10, 0),  vec3(300), "light4"));



	int x = 0, y = 0;

	//
	// Reading maze from file
	// The GUIs type will be used as a reward storage
	//
	ivec2 directionBoxSize = ivec2(boxSize * 0.3f, boxSize * 0.05f);
	for(std::string line : Tools::readFileContentsLines(GumGlobals::ASSETS_PATH + "/maze.txt"))
	{
		x = 0;
		for(int i = 0; i < line.length(); i++)
		{
			TextBox* gridPiece = new TextBox("0.0", GumEngine::Fonts->getDefaultFont(), ivec2(x * boxSize, y * boxSize), ivec2(boxSize), 12, false, 0, 0);
			Box* arrowUp    = new Box(ivec2(boxSize * 0.5f - directionBoxSize.x * 0.5f, directionBoxSize.y), directionBoxSize, 0);
			Box* arrowDown  = new Box(ivec2(boxSize * 0.5f - directionBoxSize.x * 0.5f, boxSize - 2 * directionBoxSize.y), directionBoxSize, 0);
			Box* arrowLeft  = new Box(ivec2(directionBoxSize.y, boxSize * 0.5f - directionBoxSize.x * 0.5f), ivec2(directionBoxSize.y, directionBoxSize.x), 0);
			Box* arrowRight = new Box(ivec2(boxSize - 2 * directionBoxSize.y, boxSize * 0.5f - directionBoxSize.x * 0.5f), ivec2(directionBoxSize.y, directionBoxSize.x), 0);
			gridPiece->addGUI(arrowUp);
			gridPiece->addGUI(arrowDown);
			gridPiece->addGUI(arrowLeft);
			gridPiece->addGUI(arrowRight);

			switch (line[i])
			{
				case '0':	//Free Path
					gridPiece->setColor(vec4(1,1,1,1));
					gridPiece->setType(FREE_REWARD);
					break;

				case '1':	//Wall
					gridPiece->setColor(vec4(0,0,0,1));
					gridPiece->setType(WALL_REWARD);
					break;

				case 'S':	//Start
					gridPiece->setColor(vec4(0,0,1,1));
					gridPiece->setType(START_REWARD);
					break;

				case 'G': 	//Goal
					gridPiece->setColor(vec4(0,1,0,1));
					gridPiece->setType(GOAL_REWARD);
					break;

				default:
					delete gridPiece;
					gridPiece = nullptr;
					continue;
					break;
			}
			GumEngine::GUIS->addGUI(gridPiece);
			vMazeGrid.push_back(gridPiece);
			x++;
		}
		y++;
	}

	numColumns = x; 
	numRows = y;

	pPlayer = new Box(ivec2(10, 10), ivec2(boxSize - 20), boxSize - 20, Box::ALL_SIDES);
	pPlayer->setColor(vec4(1.0f, 0.647f, 0.0078f, 1.0f));
	GumEngine::GUIS->addGUI(pPlayer);

	;
	std::map<std::string, std::vector<float> > qtable = {
		{ "Up",    std::vector<float>(vMazeGrid.size()) },
		{ "Down",  std::vector<float>(vMazeGrid.size()) },
		{ "Left",  std::vector<float>(vMazeGrid.size()) },
		{ "Right", std::vector<float>(vMazeGrid.size()) }
	};



	pQLearning = new GumQLearning(&qtable, 0.1f);
	pQLearning->printQTable();

	pQLearning->mapAction("Up", 	actionMoveUp);
	pQLearning->mapAction("Down", 	actionMoveDown);
	pQLearning->mapAction("Left", 	actionMoveLeft);
	pQLearning->mapAction("Right", 	actionMoveRight);

	pQLearning->doInitStep();

	int currentEpisode = 1;
	
	pWorld3D->addUpdateable([currentEpisode]() mutable {
		pQLearning->doStep();
		pQLearning->giveReward(std::stof(vMazeGrid[currentTile]->getType()));
		pQLearning->updateQTable();

		//sleep(1.0f);
		pQLearning->printQTable();

		updateArrowColors();

		if(checkIfPlayerReachedGoal())
		{
			pPlayer->setPosition(ivec2(10, 10));
			pQLearning->finishedEpisode();
			currentEpisode++;
		}
	});
	
	return pWorld3D;
}