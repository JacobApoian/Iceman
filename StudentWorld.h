#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init();
	virtual int move(); // All comments below were what was in move from the beginning of the skeleton
	//{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	/*decLives();
	return GWSTATUS_CONTINUE_GAME;*/
	//return GWSTATUS_PLAYER_DIED;

	//}

	bool dig(int x, int y);
	bool iceisBelowBoulderinit();
	bool iceisBelowBoulder(int startX, int startY);
	bool ForkInPath(Regular_Protester* p);

	bool ProtesterCanMoveRight(Regular_Protester* p);
	bool ProtesterCanMoveLeft(Regular_Protester* p);
	bool ProtesterCanMoveUp(Regular_Protester* p);
	bool ProtesterCanMoveDown(Regular_Protester* p);

	bool ProtesterCrushed(Regular_Protester* p);
	bool IcemanCrushed();
	bool BoulderFalling();


	int getIcemanXPosition();
	int getIcemanYPosition();
	GraphObject::Direction getIcemanDirection();

	int getOilXPosition();
	int getOilYPosition();
	int getSquirtXPosition();
	int getSquirtYPosition();
	int getCurrentTickCount64();
	bool collectGold();
	bool collectOil();
	bool SquirtWater();
	bool collectWaterPool();
	bool Reveal_Gold_And_Oil();
	bool ProtesterHitByWater(Regular_Protester* p);
	bool ProtesterHitByWater(shared_ptr<Regular_Protester> p);
	bool LOS_Of_Protester(Regular_Protester* x);




	bool BoulderisNotLeftofIceman();
	bool BoulderisNotRightofIceman();
	bool BoulderisNotAboveIceman();
	bool isBorder(int x, int y, GraphObject::Direction dir);
	bool isNextSpaceEmpty();
	bool BoulderisNotBelowIceman();

	void ProtesterMoveRight(Regular_Protester* p);
	void ProtesterMoveLeft(Regular_Protester* p);
	void ProtesterMoveUp(Regular_Protester* p);
	void ProtesterMoveDown(Regular_Protester* p);

	bool AbortLevel();
	bool SpaceIsAvailable(int startX, int startY);
	void BadCoordify(int startX, int startY);
	void RevealPathToProtesters(int startX, int startY);
	void FindIceman(Regular_Protester* p);
	void Sonar_Use();
	void collectSonarCharge();
	void setDisplayText();
	void I_Give_Up();
	void DropGold();
	void ProtesterPickUpGold();
	void YellAtIceman();

	virtual void cleanUp();

private:
	Ice* pIce[64][60];
	Ice* pIceDug[64][60];


	map<pair<int, int>, bool> BadCoordinates;
	map<pair<int, int>, bool> AccessibleByAllProtesters;


	Iceman* pIceman;
	shared_ptr<Actor> pBoulder;
	Actor* pOil;
	Actor* pGold;
	Actor* pDroppedGold;
	Sonar_Kit* pSonar_Kit;
	Water_Pool* pWater_pool;
	Actor* pSquirt;
	shared_ptr<Regular_Protester> pRegProtester;

	vector<shared_ptr<Boulder>> vBoulder;
	vector<Actor*> vGold;
	vector<shared_ptr<Water_Pool>> vWater_pool;
	vector<Actor*> vOil;
	vector<shared_ptr<Regular_Protester>> vProtester;
	vector<shared_ptr<Squirt>> vSquirt;
	vector<Actor*> vDroppedGold;


	Tick ProtesterSpawnTimer;
	vector<Tick> ProtesterMoveTimer;
	vector<Tick> vProtesterAnnoyedTimer;
	vector<Tick> vDroppedGoldTimer;


	vector<string> Choices;

};



#endif // STUDENTWORLD_H_
