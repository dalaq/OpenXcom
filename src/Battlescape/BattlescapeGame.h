/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http:///www.gnu.org/licenses/>.
 */
#ifndef OPENXCOM_BATTLESCAPEGAME_H
#define OPENXCOM_BATTLESCAPEGAME_H

#include "Position.h"
#include "SDL.h"
#include <string>
#include <list>

namespace OpenXcom
{

class BattleUnit;
class SavedBattleGame;
class BattleItem;
class BattleState;
class BattlescapeState;
class Timer;
class ResourcePack;
class Map;
class TileEngine;
class Pathfinding;
class Ruleset;

enum BattleActionType { BA_NONE, BA_TURN, BA_WALK, BA_PRIME, BA_THROW, BA_AUTOSHOT, BA_SNAPSHOT, BA_AIMEDSHOT, BA_STUN, BA_HIT, BA_USE };

struct BattleAction
{
	BattleActionType type;
	BattleUnit *actor;
	BattleItem *weapon;
	Position target;
	int TU;
	bool targeting;
	int value;
	std::string result;
	BattleAction() : type(BA_NONE), actor(0), weapon(0), TU(0), targeting(false), value(0), result("") { }
};

/**
 * Battlescape game - the core game engine of the battlescape game
 */
class BattlescapeGame
{
private:
	SavedBattleGame *_save;
	BattlescapeState *_parentState;
	std::list<BattleState*> _states;
	BattleActionType _tuReserved;
	bool _debugPlay, _playerPanicHandled;
	int _AIActionCounter;
	BattleAction _currentAction;

	void selectNextPlayerUnit(bool checkReselect);
	void endTurn();
	bool handlePanickingPlayer();
	bool handlePanickingUnit(BattleUnit *unit);
	bool noActionsPending(BattleUnit *bu);
public:
	/// Creates the BattlescapeGame state.
	BattlescapeGame(SavedBattleGame *save, BattlescapeState *parentState);
	/// Cleans up the BattlescapeGame state.
	~BattlescapeGame();
	/// think.
	void think();
	void init();
	// playable unit selected?
	bool playableUnitSelected();
	/// handlestates timer.
	void handleState();
	/// Push a state at the front of the list.
	void statePushFront(BattleState *bs);
	/// Push a state at the second of the list.
	void statePushNext(BattleState *bs);
	/// Push a state at the back of the list.
	void statePushBack(BattleState *bs);
	/// Handles the result of non target actions, like priming a grenade.
	void handleNonTargetAction();
	/// Remove current state.
	void popState();
	/// Set state think interval.
	void setStateInterval(Uint32 interval);
	/// Checks for casualties in battle.
	bool checkForCasualties(BattleItem *murderweapon, BattleUnit *murderer, bool hiddenExplosion = false, bool terrainExplosion = false);
	/// Checks if a unit panics.
	void checkForPanic(BattleUnit *unit);
	/// Check reserved tu.
	bool checkReservedTU(BattleUnit *bu, int tu);
	/// Handles unit AI.
	void handleAI(BattleUnit *unit);
	/// Add item & affect with gravity.
	void dropItem(const Position &position, BattleItem *item, bool newItem = false);
	/// Check whether TUs should be spent.
	bool dontSpendTUs();
	/// Handle kneeling action.
	void kneel(BattleUnit *bu);
	/// Cancel whatever action we were going at.
	bool cancelCurrentAction();
	/// Get pointer to access action members directly.
	BattleAction *getCurrentAction();
	/// Is there currently an action going on.
	bool isBusy();
	/// Activate primary action (left click)
	void primaryAction(const Position &pos);
	/// Activate secondary action (right click)
	void secondaryAction(const Position &pos);
	/// Move a unit up or down.
	void moveUpDown(BattleUnit *unit, int dir);
	/// Request of the end of the turn (wait for explosions etc to really end the turn)
	void requestEndTurn();
	/// Set the TU reserved type.
	void setTUReserved(BattleActionType tur);
	/// Sets up the cursor taking into account the action.
	void setupCursor();
	/// Getters:
	Map *getMap();
	SavedBattleGame *getSave();
	TileEngine *getTileEngine();
	Pathfinding *getPathfinding();
	ResourcePack *getResourcePack();
	Ruleset *getRuleset();
};

}

#endif
