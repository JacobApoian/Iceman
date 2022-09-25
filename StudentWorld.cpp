#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <list>
#include <cmath>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

// For info on how to implement this method consult page 19 of the specification document
int StudentWorld::move()
{
	setDisplayText();


	pIceman->doSomething();

	for (int i = 0; i < vOil.size(); i++)
	{
		if (vOil[i] != nullptr)
		{
			vOil[i]->doSomething();
		}
	}

	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr && vBoulder[i]->living())
		{
			vBoulder[i]->doSomething();
		}
	}

	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr && !vBoulder[i]->living())
		{
			vBoulder[i].reset();
		}
	}

	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr && !vSquirt[i]->living())
		{
			vSquirt[i].reset();
		}
	}


	for (int i = 0; i < vGold.size(); i++)
	{
		if (vGold[i] != nullptr)
		{
			vGold[i]->doSomething();
		}
	}


	for (int i = 0; i < vWater_pool.size(); i++)
	{
		if (vWater_pool[i] != nullptr)
		{
			vWater_pool[i]->doSomething();
		}
	}

	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr && vSquirt[i]->getX() != pIceman->getX() + 8 && vSquirt[i]->getDirection() == GraphObject::right)
		{
			vSquirt[i]->moveTo(vSquirt[i]->getX() + 4, vSquirt[i]->getY());
		}
		else if (vSquirt[i] != nullptr && vSquirt[i]->getX() == pIceman->getX() + 8)
		{
			vSquirt[i]->kill();
		}
		if (vSquirt[i] != nullptr && vSquirt[i]->getX() != pIceman->getX() - 8 && vSquirt[i]->getDirection() == GraphObject::left)
		{
			vSquirt[i]->moveTo(vSquirt[i]->getX() - 4, vSquirt[i]->getY());
		}
		else if (vSquirt[i] != nullptr && vSquirt[i]->getX() == pIceman->getX() - 8)
		{
			vSquirt[i]->kill();
		}

		if (vSquirt[i] != nullptr && vSquirt[i]->getY() != pIceman->getY() + 8 && vSquirt[i]->getDirection() == GraphObject::up)
		{
			vSquirt[i]->moveTo(vSquirt[i]->getX(), vSquirt[i]->getY() + 4);
		}
		else if (vSquirt[i] != nullptr && vSquirt[i]->getY() == pIceman->getY() + 8)
		{
			vSquirt[i]->kill();
		}

		if (vSquirt[i] != nullptr && vSquirt[i]->getY() != pIceman->getY() - 8 && vSquirt[i]->getDirection() == GraphObject::down)
		{
			vSquirt[i]->moveTo(vSquirt[i]->getX(), vSquirt[i]->getY() - 4);
		}
		else if (vSquirt[i] != nullptr && vSquirt[i]->getY() == pIceman->getY() - 8)
		{
			vSquirt[i]->kill();
		}

	}



	if (!pIceman->living())
	{
		this->playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}


	if (pIceman->getOilNeeded() == 0)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}




	pSonar_Kit->doSomething();
	int G = getLevel() * 25 + 300;
	int chance = rand() % (getLevel() * 25 + 300) + 1;
	int current_level_number = getLevel();
	int rhsTerm = 300 - 10 * current_level_number;
	int T = max(100, rhsTerm);
	int cWater = rand() % 5 + 1;
	pSonar_Kit->incTick();

	for (int i = 0; i < vWater_pool.size(); i++)
	{
		if (vWater_pool[i] != nullptr)
		{
			vWater_pool[i]->incTick();
		}
	}


	int waterpoolX = rand() % 60;
	int waterpoolY = rand() % 54;

	if (!pSonar_Kit->living() && chance == G)
	{
		if (cWater != 1)
		{
			while (SpaceIsAvailable(waterpoolX, waterpoolY) == false)
			{
				waterpoolX = rand() % 60;
				waterpoolY = rand() % 54;
			}
			auto pWater_pool = make_shared<Water_Pool>(this, waterpoolX - 1, waterpoolY);
			vWater_pool.push_back(pWater_pool);
		}
		else
		{
			pSonar_Kit = new Sonar_Kit(this, 0, 60);
			pSonar_Kit->setVisible(true);
			pSonar_Kit->resetTick();
		}
	}



	if (pSonar_Kit->living() == true && pSonar_Kit->getTick() == T)
	{
		pSonar_Kit->kill();
		pSonar_Kit->setVisible(false);
		pSonar_Kit->resetTick();
	}


	for (int i = 0; i < vWater_pool.size(); i++)
	{
		if (vWater_pool[i] != nullptr)
		{
			if (vWater_pool[i]->living() == true && vWater_pool[i]->getTick() == T)
			{
				vWater_pool[i]->kill();
				vWater_pool[i]->setVisible(false);
				vWater_pool[i]->resetTick();
			}
		}
	}


	// EVERYTHING PROTESTER RELATED IS BELOW


	int somethingProtesterRhs = 200 - getLevel();
	int somethingTargetnumProtestersrhs = 2 + getLevel() * 1.5;
	int Target_Num_Protesters = min(15, somethingTargetnumProtestersrhs);
	int TicksForProtester = max(25, somethingProtesterRhs);

	int RHSPROBHCP = getLevel() * 10 + 30;
	int ProbabilityOfHardcore = min(90, RHSPROBHCP); // At Level 0, it's a 30% chance of spawning

	ProtesterSpawnTimer.increaseTick();


	int Hardcore_or_Regular = rand() % ProbabilityOfHardcore + 1;
	int chanceHCP = rand() % 100 + 1;

	if (ProtesterSpawnTimer.gTick == TicksForProtester && vProtester.size() != Target_Num_Protesters)
	{
		ProtesterSpawnTimer.resetTick();

		if (chanceHCP <= ProbabilityOfHardcore)
		{
			auto vHardcoreProtester = make_shared<Hardcore_Protester>(this, 60, 60, IID_HARD_CORE_PROTESTER);
			vProtester.push_back(vHardcoreProtester);
			Tick MoveTimerforNewProtester;
			Tick ProtesterAnnoyedTimer;
			ProtesterMoveTimer.push_back(MoveTimerforNewProtester);
			vProtesterAnnoyedTimer.push_back(ProtesterAnnoyedTimer);

		}
		else
		{
			auto pRegProtester = make_shared<Regular_Protester>(this, 60, 60, IID_PROTESTER);
			vProtester.push_back(pRegProtester);
			Tick MoveTimerforNewProtester;
			Tick ProtesterAnnoyedTimer;
			ProtesterMoveTimer.push_back(MoveTimerforNewProtester);
			vProtesterAnnoyedTimer.push_back(ProtesterAnnoyedTimer);
		}

	}

	int somethingTTWBMrhs = 3 - (getLevel() / 4);
	int ticksToWaitBetweenMoves = max(0, somethingTTWBMrhs);

	int somethingProtesterAnnoyedRHS = 100 - (getLevel() * 10);
	int ticksUntilNoLongerAnnoyed = max(50, somethingProtesterAnnoyedRHS);


	for (int i = 0; i < ProtesterMoveTimer.size(); i++)
	{
		if (vProtester[i] != nullptr && !vProtester[i]->isAnnoyed())
		{
			ProtesterMoveTimer[i].increaseTick();
		}
		else if (vProtester[i] != nullptr && vProtester[i]->isAnnoyed())
		{
			ProtesterMoveTimer[i].resetTick();
		}
	}

	for (int i = 0; i < vProtesterAnnoyedTimer.size(); i++)
	{
		if (vProtester[i] != nullptr && vProtester[i]->isAnnoyed())
		{
			vProtesterAnnoyedTimer[i].increaseTick();
		}
	}


	for (int i = 0; i < vProtester.size(); i++)
	{

		if (vProtester[i] != nullptr && ProtesterMoveTimer[i].gTick == ticksToWaitBetweenMoves && !vProtester[i]->isAnnoyed())
		{
			vProtester[i]->doSomething();
			ProtesterMoveTimer[i].resetTick();
		}
		else if (vProtester[i]->isAnnoyed() && vProtesterAnnoyedTimer[i].gTick == ticksUntilNoLongerAnnoyed)
		{
			vProtesterAnnoyedTimer[i].resetTick();
			vProtester[i]->unAnnoy();
		}
	}

	for (int i = 0; i < vProtester.size(); i++)
	{
		if (vProtester[i] != nullptr && vProtester[i]->isAnnoyed() && ProtesterHitByWater(vProtester[i]) == true && !vProtester[i]->isGivingUp())
		{
			playSound(SOUND_PROTESTER_ANNOYED);
			vProtesterAnnoyedTimer[i].resetTick();
			vProtester[i]->decreaseHP(2);
			if (vProtester[i]->getHP() <= 0)
			{
				playSound(SOUND_PROTESTER_GIVE_UP);
				vProtester[i]->makeHimGiveUp();
				increaseScore(25);
			}
		}
	}


	for (auto x : vProtester)
	{
		if (x != nullptr)
		{
			if (!x->living())
			{
				x->setVisible(false);
				x.reset();
			}
		}
	}

	for (auto x : vDroppedGold)
	{
		if (x != nullptr && !x->living())
		{
			x->setVisible(false);
			x = nullptr;
		}
	}

	for (int i = 0; i < vDroppedGoldTimer.size(); i++)
	{
		if (vDroppedGold[i] != nullptr)
		{
			vDroppedGoldTimer[i].gTick++;
		}

		if (vDroppedGoldTimer[i].gTick == 100)
		{
			vDroppedGold[i]->setVisible(false);
			vDroppedGold[i]->kill();
		}

	}




	//for (auto x : AccessibleByAllProtesters)
	//{
	//	if(x.first.first == pIceman->getX() && x.first.second == pIceman->getY() && x.second == true)
	//	{
	//		cout << "(" << x.first.first << ", " << x.first.second << "): Is a good coord" << endl;
	//	}
	//}

	// decLives();
	//return GWSTATUS_PLAYER_DIED;
	return GWSTATUS_CONTINUE_GAME;
}







void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	double health = (pIceman->getHP() / 10.0) * 100.0;
	int squirts = pIceman->getWater();
	int gold = pIceman->getGold();
	int barrelsLeft = pIceman->getOilNeeded();;
	int sonar = pIceman->getSonarCharges();
	int score = getScore();

	ostringstream sout;
	sout << "Lvl: " << level << " Lives: " << lives << " Hlth: " << health << "% Wtr: " << squirts << " Gld: " << gold << " Oil Left: " << barrelsLeft << " Sonar: " << sonar << " Scr: " << score;

	setGameStatText(sout.str());
}



void StudentWorld::cleanUp()
{
	int count = 0;



	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			if (pIce[i][j] != nullptr) // ensures there will be no double deletion
			{
				if (i != 30 && i != 31 && i != 32 && i != 33 || j == 0 || j == 1 || j == 2 || j == 3)
				{
					delete pIce[i][j];
				}
			}
		}

	}

	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			vBoulder[i] = nullptr;
		}
	}
	vBoulder.clear();

	for (int i = 0; i < vGold.size(); i++)
	{
		if (vGold[i] != nullptr)
		{
			delete vGold[i];
		}
	}
	vGold.clear();

	for (int i = 0; i < vWater_pool.size(); i++)
	{
		if (vWater_pool[i] != nullptr)
		{
			vWater_pool[i] = nullptr;
		}
	}
	vWater_pool.clear();

	for (int i = 0; i < vOil.size(); i++)
	{
		if (vOil[i] != nullptr)
		{
			delete vOil[i];
		}
	}
	vOil.clear();

	for (int i = 0; i < vProtester.size(); i++)
	{
		if (vProtester[i] != nullptr)
		{
			vProtester[i] = nullptr;
		}
	}
	vProtester.clear();


	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr)
		{
			vSquirt[i].reset();
		}
	}
	vSquirt.clear();

	for (int i = 0; i < vDroppedGold.size(); i++)
	{
		if (vDroppedGold[i] != nullptr)
		{
			delete vDroppedGold[i];
		}
	}
	vDroppedGold.clear();

	vDroppedGoldTimer.clear();

	ProtesterMoveTimer.clear();
	ProtesterSpawnTimer.resetTick();
	vProtesterAnnoyedTimer.clear();
	delete pIceman;
	delete pSonar_Kit;


}


int StudentWorld::init()
{
	pSonar_Kit = new Sonar_Kit(this, 0, 60);
	pSonar_Kit->setVisible(false);
	pSonar_Kit->kill();


	//auto pWater_pool = make_shared<Water_Pool>(this, rand() % 60, rand() % 54);
	//vWater_pool.push_back(pWater_pool);
	//vWater_pool[0]->setVisible(false);
	//vWater_pool[0]->kill();



	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			BadCoordinates[{i, j}] = false;
		}
	}





	unsigned int something = 9; // All these somethings get the max min functions to work (there would be an issue with type otherwise)
	unsigned int something2 = 2;
	unsigned int something3 = 21;
	int numBoulders = min((getLevel() / 2) + 2, something);
	int numGold = max(5 - getLevel() / 2, something2);
	int numOil = min(2 + getLevel(), something3);


	// Get Iceman to show up
	pIceman = new Iceman(this);
	pIceman->setOilNeeded(numOil);

	//auto pRegProtester = make_shared<Regular_Protester>(this, 60, 60, IID_PROTESTER);
	//vProtester.push_back(pRegProtester);




	// Ice will populate from row 0 to 59
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)  // y < 60
		{
			if (x != 30 && x != 31 && x != 32 && x != 33 || y == 0 || y == 1 || y == 2 || y == 3)
			{
				pIce[x][y] = new Ice(x, y, this);
				AccessibleByAllProtesters[{x, y}] = false;
			}
			else
			{
				BadCoordinates[{x, y}] = true;
				BadCoordify(x, y);
				pIce[x][y] = nullptr;
			}
		}
	}




	for (int i = 0; i < numBoulders; i++)
	{
		//v1 = rand() % 100;     v1 in the range 0 to 99
		//int range = max - min + 1;
		//int num = rand() % range + min;
		//^^ just some notes on %rand()
		int Boulder_spawnX = rand() % 60;		// in range from 0-59
		int range = 56 - 20 + 1;
		int Boulder_spawnY = rand() % range + 20; //should be in the range of 20-56
		while (Boulder_spawnX > 25 && Boulder_spawnX < 37 && BadCoordinates[{Boulder_spawnX, Boulder_spawnY}] == true)
		{
			Boulder_spawnX = rand() % 60;
			Boulder_spawnY = rand() % range + 20;
		}
		BadCoordify(Boulder_spawnX, Boulder_spawnY);
		auto pBoulder = make_shared<Boulder>(this, Boulder_spawnX, Boulder_spawnY);
		vBoulder.push_back(pBoulder);


	}



	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; i < 60; i++)
		{
			if (pIce[i][j] == nullptr)
			{
				AccessibleByAllProtesters[{i, j}] = true; // Allow Protesters to Move where there is no Ice
			}
			else
			{
				AccessibleByAllProtesters[{i, j}] = false; // If there is Ice do not allow the Protester to be there
			}
		}
	}


	// delete ice where ever the boulder spawns in a 4x4 radius 
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)  // y < 60
		{
			for (int i = 0; i < vBoulder.size(); i++)
			{
				if (x >= vBoulder[i]->getX() && x <= vBoulder[i]->getX() + 3 && y >= vBoulder[i]->getY() && y <= vBoulder[i]->getY() + 3)
				{
					// cout << "The Boulder resides in (" << x << " , " << y << ")" << endl;
					delete pIce[x][y];
					pIce[x][y] = nullptr;
					AccessibleByAllProtesters[{x, y}] = false;
				}
			}
		}
	}
	// cout << "Boulder's Pixel position is (" << pBoulder->getX() << " , " << pBoulder->getY() << ")" << endl;





	int Oil_SpawnX = rand() % 60;
	int Oil_SpawnY = rand() % 56;

	for (int i = 0; i < numOil; i++)
	{
		Oil_SpawnX = rand() % 60;
		Oil_SpawnY = rand() % 56;
		while (BadCoordinates[{Oil_SpawnX, Oil_SpawnY}] == true)
		{
			Oil_SpawnX = rand() % 60;
			Oil_SpawnY = rand() % 56;
		}
		BadCoordify(Oil_SpawnX, Oil_SpawnY);
		pOil = new Oil(this, Oil_SpawnX, Oil_SpawnY);
		vOil.push_back(pOil);
	}

	int Gold_SpawnX = rand() % 60;	//-4 for each coordinate b/c each asset is 4 units and we don't want them outside of the map
	int Gold_SpawnY = rand() % 56;  // for air at the top of the game

	for (int i = 0; i < numGold; i++)
	{
		Gold_SpawnX = rand() % 60;
		Gold_SpawnY = rand() % 56;

		while (BadCoordinates[{Gold_SpawnX, Gold_SpawnY}] == true)
		{
			Gold_SpawnX = rand() % 60;
			Gold_SpawnY = rand() % 56;
		}
		BadCoordify(Gold_SpawnX, Gold_SpawnY);
		pGold = new Gold_Nugget(this, Gold_SpawnX, Gold_SpawnY);
		vGold.push_back(pGold);
	}



	// pSquirt = new Squirt(this, pIceman->getX(), pIceman->getY(), pIceman->getDirection());





	for (int i = 0; i < 60; i++)
	{
		int j = 60;
		RevealPathToProtesters(i, j);
	}
	for (int j = 0; j < 60; j++)
	{
		int i = 30;
		RevealPathToProtesters(i, j);
	}
	// The Following Code Prevents the protester from exiting the map
	for (int i = 0; i < 60; i++)
	{
		int j = 61;          // Makes the top of the map inaccessible
		AccessibleByAllProtesters[{i, j}] = false;
	}
	for (int i = 0; i < 60; i++)
	{
		int j = -1;         // Makes The Bottom of the map inaccessible
		AccessibleByAllProtesters[{i, j}] = false;
	}

	for (int j = 0; j < 60; j++)
	{
		int i = -1;         // Makes the far Left side of the map inaccessible
		AccessibleByAllProtesters[{i, j}] = false;
	}
	for (int j = 0; j < 60; j++)
	{
		int i = 61;			// Makes the far Right side of the map inaccessible
		AccessibleByAllProtesters[{i, j}] = false;
	}






	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::dig(int x, int y)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			if (i != 30 && i != 31 && i != 32 && i != 33 || j == 0 || j == 1 || j == 2 || j == 3)
			{
				if (pIce[i][j] != nullptr) // ensures there will be no double deletion on cleanup()
				{
					if ((x <= pIce[i][j]->getX() && (x + 3) >= pIce[i][j]->getX()) &&
						(y <= pIce[i][j]->getY() && (y + 3) >= pIce[i][j]->getY()))
					{
						AccessibleByAllProtesters[{i, j}] = true;
						delete pIce[i][j];
						pIce[i][j] = nullptr; // ensures there will be no double deletion on cleanup()
						return true;
					}

				}
			}
		}
	}
	return false;
}

//if ((x <= pIce[i][j]->getX() && (x + 3) >= pIce[i][j]->getX()) &&
//	(y <= pIce[i][j]->getY() && (y + 3) >= pIce[i][j]->getY()))


int StudentWorld::getIcemanXPosition()
{
	return pIceman->getX();
}

int StudentWorld::getOilXPosition()
{
	return pOil->getX();
}

int StudentWorld::getIcemanYPosition()
{
	return pIceman->getY();
}

int StudentWorld::getOilYPosition()
{
	return pOil->getX();
}


int StudentWorld::getSquirtXPosition()
{
	return pSquirt->getX();
}

int StudentWorld::getSquirtYPosition()
{
	return pSquirt->getY();
}

GraphObject::Direction StudentWorld::getIcemanDirection()
{
	return pIceman->getDirection();
}
int StudentWorld::getCurrentTickCount64()
{
	return GetTickCount64();
}



// This function returns true if there is ANY ice under the boulder
bool StudentWorld::iceisBelowBoulderinit()
{
	for (int i = 0; i < vBoulder.size(); i++) {
		if (vBoulder[i] != nullptr)
		{
			if (pIce[vBoulder[i]->getX()][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 1][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 2][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 3][vBoulder[i]->getY() - 1] != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::iceisBelowBoulder(int startX, int startY)
{

	if (pIce[startX][startY - 1] != nullptr ||
		pIce[startX + 1][startY - 1] != nullptr ||
		pIce[startX + 2][startY - 1] != nullptr ||
		pIce[startX + 3][startY - 1] != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}

}



bool StudentWorld::BoulderFalling()
{
	for (int i = 0; i < vBoulder.size(); i++) {
		if (vBoulder[i] != nullptr)
		{
			if (pIce[vBoulder[i]->getX()][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 1][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 2][vBoulder[i]->getY() - 1] != nullptr ||
				pIce[vBoulder[i]->getX() + 3][vBoulder[i]->getY() - 1] != nullptr)
			{
				return true;
			}
			/*else if (vBoulder[i]->getX() != 0)
			{
				return true;
			}*/




		}
		return false;
	}
}



bool StudentWorld::IcemanCrushed()
{
	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			if (pIceman->getX() <= vBoulder[i]->getX() + 3 && pIceman->getX() >= vBoulder[i]->getX() && pIceman->getY() <= vBoulder[i]->getY() + 3 && pIceman->getY() >= vBoulder[i]->getY())
			{
				pIceman->TakeDamage(999);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::ProtesterCrushed(Regular_Protester* p)
{
	for (int i = 0; i < vBoulder.size(); i++)
	{

		if (vBoulder[i] != nullptr && p != nullptr)
		{
			if (p->getX() <= vBoulder[i]->getX() + 3 && p->getX() >= vBoulder[i]->getX() && p->getY() <= vBoulder[i]->getY() + 3 && p->getY() >= vBoulder[i]->getY())
			{
				return true;
			}
		}
	}
	return false;
}
bool StudentWorld::isBorder(int x, int y, GraphObject::Direction dir) {
	if (dir == GraphObject::right && x >= 59) // coordinate is bottom left, asset is a size of 4
		return true;
	if (dir == GraphObject::left && x <= 0)
		return true;
	if (dir == GraphObject::up && y >= 59)
		return true;
	if (dir == GraphObject::down && y <= 4)
		return true;
	return false;

}

bool StudentWorld::isNextSpaceEmpty() {
	for (int i = 0; i < vSquirt.size(); i++) {
		if (vSquirt[i] != NULL) {
			if (isBorder(vSquirt[i]->getX(), vSquirt[i]->getY(), vSquirt[i]->getDirection()))
				return false;
			else if (vSquirt[i]->getY() > 59 && (vSquirt[i]->getDirection() == GraphObject::left || vSquirt[i]->getDirection() == GraphObject::right))
				return true;
			else {
				if (vSquirt[i]->getDirection() == GraphObject::right) {
					if (pIce[vSquirt[i]->getX() + 1][vSquirt[i]->getY()] != NULL)
						return false;
					else
						return true;
				}
				if (vSquirt[i]->getDirection() == GraphObject::left) {
					if (pIce[vSquirt[i]->getX() - 1][vSquirt[i]->getY()] != NULL)
						return false;
					else
						return true;
				}
				if (vSquirt[i]->getDirection() == GraphObject::up) {
					if (pIce[vSquirt[i]->getX()][vSquirt[i]->getY() + 1] != NULL)
						return false;
					else
						return true;
				}
				if (vSquirt[i]->getDirection() == GraphObject::down) {
					if (pIce[vSquirt[i]->getX()][vSquirt[i]->getY() - 1] != NULL)
						return false;
					else
						return true;
				}
			}
		}
	}
}

bool StudentWorld::SquirtWater()
{
	//delete pSquirt;
	if (pIceman->getDirection() == GraphObject::right) {
		auto pSquirt = make_shared<Squirt>(this, pIceman->getX() + 4, pIceman->getY(), pIceman->getDirection());
		vSquirt.push_back(pSquirt);
	}
	if (pIceman->getDirection() == GraphObject::left) {
		auto pSquirt = make_shared<Squirt>(this, pIceman->getX() - 4, pIceman->getY(), pIceman->getDirection());
		vSquirt.push_back(pSquirt);
	}
	if (pIceman->getDirection() == GraphObject::up) {
		auto pSquirt = make_shared<Squirt>(this, pIceman->getX(), pIceman->getY() + 4, pIceman->getDirection());
		vSquirt.push_back(pSquirt);
	}
	if (pIceman->getDirection() == GraphObject::down) {
		auto pSquirt = make_shared<Squirt>(this, pIceman->getX(), pIceman->getY() - 4, pIceman->getDirection());
		vSquirt.push_back(pSquirt);
	}
	//auto pSquirt = make_shared<Squirt>(this, pIceman->getX(), pIceman->getY(), pIceman->getDirection());
	playSound(SOUND_PLAYER_SQUIRT);
	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr)
		{
			vSquirt[i]->doSomething();
		}
	}
	return true;

}



bool StudentWorld::collectGold()
{
	for (int i = 0; i < vGold.size(); i++)
	{
		if (vGold[i] != nullptr)
		{

			int term1 = getIcemanXPosition() - vGold[i]->getX();
			int term2 = getIcemanYPosition() - vGold[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));

			if (d <= 3.00)
			{
				vGold[i]->kill();
				playSound(SOUND_GOT_GOODIE);
				increaseScore(10);
				pIceman->increaseGold();
				vGold[i]->setVisible(false);
				vGold[i] = nullptr;
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::collectWaterPool()
{
	for (int i = 0; i < vWater_pool.size(); i++)
	{
		if (vWater_pool[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vWater_pool[i]->getX();
			int term2 = getIcemanYPosition() - vWater_pool[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));
			if (d <= 3.00)
			{
				vWater_pool[i]->kill();
				playSound(SOUND_GOT_GOODIE);
				increaseScore(100);
				pIceman->increaseWater();
				vWater_pool[i]->setVisible(false);
				vWater_pool[i] = nullptr;
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::collectSonarCharge()
{
	pIceman->increaseSonarCharges();
}



bool StudentWorld::BoulderisNotLeftofIceman()
{
	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			if (pIceman->getX() >= vBoulder[i]->getX() && pIceman->getX() <= vBoulder[i]->getX() + 4 && pIceman->getY() >= vBoulder[i]->getY() - 4 && pIceman->getY() <= vBoulder[i]->getY() + 4)
			{
				return (pIceman->getX() != vBoulder[i]->getX() + 4 || pIceman->getY() >= vBoulder[i]->getY() + 4 || pIceman->getY() <= vBoulder[i]->getY() - 4);
			}
		}
	}

}
bool StudentWorld::BoulderisNotRightofIceman()
{
	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			if (pIceman->getX() <= vBoulder[i]->getX() && pIceman->getX() >= vBoulder[i]->getX() - 4 && pIceman->getY() >= vBoulder[i]->getY() - 4 && pIceman->getY() <= vBoulder[i]->getY() + 4)
			{
				return (pIceman->getX() != vBoulder[i]->getX() - 4 || pIceman->getY() >= vBoulder[i]->getY() + 4 || pIceman->getY() <= vBoulder[i]->getY() - 4);
			}
		}
	}
}
bool StudentWorld::BoulderisNotAboveIceman()
{
	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			if (pIceman->getX() >= vBoulder[i]->getX() - 4 && pIceman->getX() <= vBoulder[i]->getX() + 4 && pIceman->getY() >= vBoulder[i]->getY() - 4 && pIceman->getY() <= vBoulder[i]->getY() + 4)
			{
				return (pIceman->getY() != vBoulder[i]->getY() - 4 || pIceman->getX() >= vBoulder[i]->getX() + 4 || pIceman->getX() <= vBoulder[i]->getX() - 4);
			}
		}
	}
}
bool StudentWorld::BoulderisNotBelowIceman()
{
	for (int i = 0; i < vBoulder.size(); i++)
	{
		if (vBoulder[i] != nullptr)
		{
			if (pIceman->getX() >= vBoulder[i]->getX() - 4 && pIceman->getX() <= vBoulder[i]->getX() + 4 && pIceman->getY() >= vBoulder[i]->getY() - 4 && pIceman->getY() <= vBoulder[i]->getY() + 4)
			{
				return (pIceman->getY() != vBoulder[i]->getY() + 4 || pIceman->getX() >= vBoulder[i]->getX() + 4 || pIceman->getX() <= vBoulder[i]->getX() - 4);
			}
		}
	}
}

void StudentWorld::Sonar_Use()
{
	for (int i = 0; i < vOil.size(); i++) {
		if (vOil[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vOil[i]->getX();
			int term2 = getIcemanYPosition() - vOil[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));
			if (d < 12)
				vOil[i]->setVisible(true);
		}
	}
	for (int i = 0; i < vGold.size(); i++)
	{
		if (vGold[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vGold[i]->getX();
			int term2 = getIcemanYPosition() - vGold[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));
			//vGold[i]->getX() >= pIceman->getX() - 12 && vGold[i]->getX() <= pIceman->getX() + 12 && vGold[i]->getY() >= pIceman->getY() - 12 && vGold[i]->getY() <= pIceman->getY() + 12
			if (d < 12)
			{
				vGold[i]->setVisible(true);
			}
		}
	}



}

//if (getIcemanXPosition() - 12 >= vGold[i]->getX() && getIcemanXPosition() + 12 <= vGold[i]->getX() ||
//	getIcemanYPosition() - 12 >= vGold[i]->getY() && getIcemanYPosition() + 12 <= vGold[i]->getY())

bool StudentWorld::collectOil()
{
	for (int i = 0; i < vOil.size(); i++)
	{
		if (vOil[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vOil[i]->getX();
			int term2 = getIcemanYPosition() - vOil[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));

			if (d <= 3.00)
			{
				vOil[i]->kill();
				playSound(SOUND_FOUND_OIL);
				increaseScore(1000);
				pIceman->decOil();
				vOil[i]->setVisible(false);
				vOil[i] = nullptr;
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::Reveal_Gold_And_Oil()
{
	for (int i = 0; i < vGold.size(); i++)
	{
		if (vGold[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vGold[i]->getX();
			int term2 = getIcemanYPosition() - vGold[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));
			if (d <= 4.00)
			{
				vGold[i]->setVisible(true);
				return true;
			}
		}
	}

	for (int i = 0; i < vOil.size(); i++)
	{
		if (vOil[i] != nullptr)
		{
			int term1 = getIcemanXPosition() - vOil[i]->getX();
			int term2 = getIcemanYPosition() - vOil[i]->getY();
			double d = sqrt((term1 * term1) + (term2 * term2));
			if (d <= 4.00)
			{
				vOil[i]->setVisible(true);
				return true;
			}
		}
	}
	return false;

}


bool StudentWorld::SpaceIsAvailable(int startX, int startY)
{
	while (true)
	{
		return (pIce[startX][startY + 3] == nullptr && pIce[startX + 1][startY + 3] == nullptr && pIce[startX + 2][startY + 3] == nullptr && pIce[startX + 3][startY + 3] &&
			pIce[startX][startY + 2] == nullptr && pIce[startX + 1][startY + 2] == nullptr && pIce[startX + 2][startY + 2] == nullptr && pIce[startX + 3][startY + 2] &&
			pIce[startX][startY + 1] == nullptr && pIce[startX + 1][startY + 1] == nullptr && pIce[startX + 2][startY + 1] == nullptr && pIce[startX + 3][startY + 1] &&
			pIce[startX][startY] == nullptr && pIce[startX + 1][startY] == nullptr && pIce[startX + 2][startY] == nullptr && pIce[startX + 3][startY]);
	}

}

//return (pIce[startX - 1][startY + 1] == nullptr && pIce[startX][startY + 1] == nullptr && pIce[startX + 1][startY + 1] == nullptr &&
//	pIce[startX - 1][startY] == nullptr && pIce[startX][startY] == nullptr && pIce[startX + 1][startY] == nullptr &&
//	pIce[startX - ][startY - 1] == nullptr && pIce[startX][startY - 1] == nullptr && pIce[startX + 1][startY - 1] == nullptr);








//void StudentWorld::BadCoordify(int startX, int startY)
//{
//	for (int i = -6; i < 6; i++)
//	{
//		for (int j = -6; j < 6; j++)
//		{
//				BadCoordinates[{startX + i, startY + j }] = true;
//		}
//	}
//	
//
//
//}

void StudentWorld::BadCoordify(int startX, int startY)
{
	double r = 6.0;
	for (int i = startX - r; i <= startX + r; i++)
	{
		for (int j = startY - r; j <= startY + r; j++)
		{
			//if (sqrt((i - startX) * (i - startX) + (j - startY) * (j - startY)) <= r)
			BadCoordinates[{i, j }] = true;

		}
	}

}

void StudentWorld::RevealPathToProtesters(int startX, int startY)
{

	AccessibleByAllProtesters[{startX, startY }] = true;

}

void StudentWorld::FindIceman(Regular_Protester* x) // Have The Protester Roam Randomly Around the Icefield in hopes of finding Iceman
{




	int term1 = getIcemanXPosition() - x->getX();
	int term2 = getIcemanYPosition() - x->getY();
	double d = sqrt((term1 * term1) + (term2 * term2));



	if (x != nullptr && (d <= 4.0))
	{

		if (pIceman->getX() == x->getX() && pIceman->getY() <= x->getY())
		{
			ProtesterMoveDown(x);
		}
		if (pIceman->getX() == x->getX() && pIceman->getY() >= x->getY())
		{
			ProtesterMoveUp(x);
		}

		if (pIceman->getY() == x->getY() && pIceman->getX() <= x->getX())
		{
			ProtesterMoveLeft(x);
		}
		if (pIceman->getY() == x->getY() && pIceman->getX() >= x->getX())
		{
			ProtesterMoveRight(x);
		}



	}
}


void StudentWorld::ProtesterMoveRight(Regular_Protester* p)
{

	if (p != nullptr && AccessibleByAllProtesters[{p->getX() + 1, p->getY()}] == true)
	{
		p->setDirection(GraphObject::right);
		p->moveTo(p->getX() + 1, p->getY());
	}
	else if (p != nullptr && AccessibleByAllProtesters[{p->getX() + 1, p->getY()}] == false)
	{
		p->setSquarestoMove(0);
	}


}
void StudentWorld::ProtesterMoveLeft(Regular_Protester* p)
{

	if (p != nullptr && AccessibleByAllProtesters[{p->getX() - 1, p->getY()}] == true)
	{
		p->setDirection(GraphObject::left);
		p->moveTo(p->getX() - 1, p->getY());
	}
	else if (p != nullptr && AccessibleByAllProtesters[{p->getX() - 1, p->getY()}] == false)
	{
		p->setSquarestoMove(0);
	}
}
void StudentWorld::ProtesterMoveUp(Regular_Protester* p)
{

	if (p != nullptr && AccessibleByAllProtesters[{p->getX(), p->getY() + 1}] == true)
	{
		p->setDirection(GraphObject::up);
		p->moveTo(p->getX(), p->getY() + 1);
	}
	else if (p != nullptr && AccessibleByAllProtesters[{p->getX(), p->getY() + 1}] == false)
	{
		p->setSquarestoMove(0);
	}
}
void StudentWorld::ProtesterMoveDown(Regular_Protester* p)
{

	if (p != nullptr && AccessibleByAllProtesters[{p->getX(), p->getY() - 1}] == true)
	{
		p->setDirection(GraphObject::down);
		p->moveTo(p->getX(), p->getY() - 1);
	}
	else if (p != nullptr && AccessibleByAllProtesters[{p->getX(), p->getY() - 1}] == false)
	{
		p->setSquarestoMove(0);
	}
}


bool StudentWorld::ProtesterHitByWater(Regular_Protester* p)
{
	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr)
		{
			if (p != nullptr)
			{
				int term1 = vSquirt[i]->getX() - p->getX();
				int term2 = vSquirt[i]->getY() - p->getY();
				double d = sqrt((term1 * term1) + (term2 * term2));
				if (d <= 3.00) //vSquirt[i]->getX() == p->getX() && vSquirt[i]->getY() == p->getY()
				{
					return true;
				}

			}
		}
	}
	return false;
}

bool StudentWorld::ProtesterHitByWater(shared_ptr<Regular_Protester> p)
{
	for (int i = 0; i < vSquirt.size(); i++)
	{
		if (vSquirt[i] != nullptr)
		{
			if (p != nullptr)
			{
				int term1 = vSquirt[i]->getX() - p->getX();
				int term2 = vSquirt[i]->getY() - p->getY();
				double d = sqrt((term1 * term1) + (term2 * term2));
				if (d <= 3.00)//vSquirt[i]->getX() == p->getX() && vSquirt[i]->getY() == p->getY()
				{
					return true;
				}

			}
		}
	}
	return false;
}


void StudentWorld::DropGold() // TO DO - MAKE GOLD DESPAWN AFTER A CERTAIN TIME: Consult speciifcations
{
	pDroppedGold = new Gold_Nugget(this, pIceman->getX(), pIceman->getY());
	Tick DroppedGoldTimer;
	vDroppedGold.push_back(pDroppedGold);
	vDroppedGoldTimer.push_back(DroppedGoldTimer);
	for (auto x : vDroppedGold)
	{
		if (x != nullptr)
		{
			x->setVisible(true);
		}
	}
}

void StudentWorld::ProtesterPickUpGold()
{
	for (int i = 0; i < vProtester.size(); i++)
	{
		for (int j = 0; j < vDroppedGold.size(); j++)
		{
			if (vProtester[i] != nullptr && vDroppedGold[j] != nullptr && vDroppedGold[j]->living() &&
				vProtester[i]->getX() >= vDroppedGold[j]->getX() - 4 && vProtester[i]->getX() <= vDroppedGold[j]->getX() + 4 &&
				vProtester[i]->getY() >= vDroppedGold[j]->getY() - 4 && vProtester[i]->getY() <= vDroppedGold[j]->getY() + 4)
			{
				playSound(SOUND_PROTESTER_FOUND_GOLD);
				vDroppedGold[j]->kill();
				vDroppedGold[j]->setVisible(false);
				vDroppedGold[j] = nullptr;
				increaseScore(25);
				vProtester[i]->makeHimGiveUp();
			}
		}
	}


}

bool StudentWorld::LOS_Of_Protester(Regular_Protester* x)
{

	if (pIceman->getX() == x->getX() && pIceman->getY() <= x->getY() && x->getDirection() == GraphObject::down)
	{
		return true;
	}
	else if (pIceman->getX() == x->getX() && pIceman->getY() >= x->getY() && x->getDirection() == GraphObject::up)
	{
		return true;
	}

	else if (pIceman->getY() == x->getY() && pIceman->getX() <= x->getX() && x->getDirection() == GraphObject::left)
	{
		return true;
	}
	else if (pIceman->getY() == x->getY() && pIceman->getX() >= x->getX() && x->getDirection() == GraphObject::right)
	{
		return true;
	}
	else
		return false;
}



void StudentWorld::YellAtIceman()
{
	pIceman->TakeDamage(2);

}

bool StudentWorld::AbortLevel()
{
	pIceman->kill();
	return true;
}



bool StudentWorld::ForkInPath(Regular_Protester* p)
{
	if (ProtesterCanMoveRight(p) && ProtesterCanMoveUp(p))
	{
		return true;
	}

	if (ProtesterCanMoveRight(p) && ProtesterCanMoveDown(p))
	{
		return true;
	}

	if (ProtesterCanMoveLeft(p) && ProtesterCanMoveDown(p))
	{
		return true;
	}

	if (ProtesterCanMoveLeft(p) && ProtesterCanMoveUp(p))
	{
		return true;
	}
}




bool StudentWorld::ProtesterCanMoveRight(Regular_Protester* p)
{
	return AccessibleByAllProtesters[{p->getX() + 1, p->getY()}];

}
bool StudentWorld::ProtesterCanMoveLeft(Regular_Protester* p)
{
	return AccessibleByAllProtesters[{p->getX() - 1, p->getY()}];
}
bool StudentWorld::ProtesterCanMoveUp(Regular_Protester* p)
{
	return AccessibleByAllProtesters[{p->getX(), p->getY() + 1}];
}
bool StudentWorld::ProtesterCanMoveDown(Regular_Protester* p)
{
	return AccessibleByAllProtesters[{p->getX(), p->getY() - 1}];
}
