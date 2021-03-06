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
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "UnitDieBState.h"
#include "ExplosionBState.h"
#include "TileEngine.h"
#include "BattlescapeState.h"
#include "Map.h"
#include "Camera.h"
#include "../Engine/Game.h"
#include "../Savegame/BattleUnit.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/Tile.h"
#include "../Resource/ResourcePack.h"
#include "../Engine/SoundSet.h"
#include "../Engine/Sound.h"
#include "../Engine/RNG.h"
#include "../Ruleset/XcomRuleset.h"
#include "../Ruleset/RuleArmor.h"
#include "../Ruleset/RuleGenUnit.h"

namespace OpenXcom
{

/**
 * Sets up an UnitDieBState.
 */
UnitDieBState::UnitDieBState(BattlescapeGame *parent, BattleUnit *unit, ItemDamageType damageType, bool noSound) : BattleState(parent), _unit(unit), _damageType(damageType), _noSound(noSound)
{
	// don't show the "fall to death" animation when a unit is blasted with explosives or he is already unconscious
	if (_damageType == DT_HE || _unit->getStatus() == STATUS_UNCONSCIOUS)
	{
		_unit->startFalling();
		while (_unit->getStatus() == STATUS_FALLING)
		{
			_unit->keepFalling();
		}
	}
	else
	{
		_parent->getMap()->getCamera()->centerOnPosition(_unit->getPosition());
		_parent->setStateInterval(BattlescapeState::DEFAULT_ANIM_SPEED);
		_unit->lookAt(3);
	}	
}

/**
 * Deletes the UnitDieBState.
 */
UnitDieBState::~UnitDieBState()
{

}

void UnitDieBState::init()
{

	if (!_noSound)
	{
		Soldier *s = dynamic_cast<Soldier*>(_unit->getUnit());
		if (s)
		{
			// soldiers have screams depending on gender
			if (s->getGender() == GENDER_MALE)
			{
				_parent->getResourcePack()->getSoundSet("BATTLE.CAT")->getSound(RNG::generate(41,43))->play();
			}
			else
			{
				_parent->getResourcePack()->getSoundSet("BATTLE.CAT")->getSound(RNG::generate(44,46))->play();
			}
		}
		else
		{
			// todo get death sound from RuleGenUnit
			if (_unit->getUnit()->getArmor()->getSize() > 1)
			{
				// HWP destroy sound
				_parent->getResourcePack()->getSoundSet("BATTLE.CAT")->getSound(23)->play();
			}
			else
			{
				_parent->getResourcePack()->getSoundSet("BATTLE.CAT")->getSound(10)->play();
			}
		}
	}

	//think();
}

/*
 * Think!
 */
void UnitDieBState::think()
{
	if (_unit->getStatus() == STATUS_TURNING)
	{
		_unit->turn();
	}
	else if (_unit->getStatus() == STATUS_STANDING)
	{
		_unit->startFalling();
	}
	else if (_unit->getStatus() == STATUS_FALLING)
	{
		_unit->keepFalling();
	}

	if (_unit->isOut())
	{
		_unit->keepFalling();
		convertUnitToCorpse();
		_parent->getTileEngine()->calculateUnitLighting();
		_parent->popState();
		if (_unit->getUnit()->getSpecialAbility() == SPECAB_EXPLODEONDEATH)
		{
			Position p = Position(_unit->getPosition().x * 16, _unit->getPosition().y * 16, _unit->getPosition().z * 24);
			_parent->statePushNext(new ExplosionBState(_parent, p, 0, _unit, 0));
		}
	}

	_parent->getMap()->cacheUnit(_unit);
}

/*
 * Unit falling cannot be cancelled.
 */
void UnitDieBState::cancel()
{
}

/*
 * Convert unit to corpse(item).
 * @param unit
 * @param terrain
 */
void UnitDieBState::convertUnitToCorpse()
{
	// in case the unit was unconscious
	_parent->getSave()->removeUnconsciousBodyItem(_unit);

	int size = _unit->getUnit()->getArmor()->getSize() - 1;
	// move inventory from unit to the ground for non-large units
	if (size == 0)
	{
		for (std::vector<BattleItem*>::iterator i = _unit->getInventory()->begin(); i != _unit->getInventory()->end(); ++i)
		{
			_parent->dropItem(_unit->getPosition(), (*i));
		}
	}
	_unit->getInventory()->clear();

	// remove unit-tile link
	_unit->setTile(0);

	if (size == 0)
	{
		_parent->getSave()->getTile(_unit->getPosition())->setUnit(0);
		BattleItem *corpse = new BattleItem(_parent->getRuleset()->getItem(_unit->getUnit()->getArmor()->getCorpseItem()),_parent->getSave()->getCurrentItemId());
		corpse->setUnit(_unit);
		_parent->dropItem(_unit->getPosition(), corpse, true);
	}
	else
	{
		int i = 1;
		for (int y = 0; y <= size; y++)
		{
			for (int x = 0; x <= size; x++)
			{
				_parent->getSave()->getTile(_unit->getPosition() + Position(x,y,0))->setUnit(0);
				std::stringstream ss;
				ss << _unit->getUnit()->getArmor()->getCorpseItem() << i;
				BattleItem *corpse = new BattleItem(_parent->getRuleset()->getItem(ss.str()),_parent->getSave()->getCurrentItemId());
				//corpse->setUnit(unit); // no need for this, because large units never can be revived as they don't go unconscious
				_parent->dropItem(_unit->getPosition() + Position(x,y,0), corpse, true);
				i++;
			}
		}
	}
}

}
