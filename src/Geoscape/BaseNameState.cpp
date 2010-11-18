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
#include "BaseNameState.h"
#include "../Engine/Game.h"
#include "../Engine/Action.h"
#include "../Resource/ResourcePack.h"
#include "../Engine/Language.h"
#include "../Resource/LangString.h"
#include "../Engine/Font.h"
#include "../Engine/Palette.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextEdit.h"
#include "../Savegame/Base.h"
#include "../Basescape/PlaceLiftState.h"

/**
 * Initializes all the elements in a Base Name window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to name.
 * @param globe Pointer to the Geoscape globe.
 * @param first Is this the first base in the game?
 */
BaseNameState::BaseNameState(Game *game, Base *base, Globe *globe, bool first) : State(game), _base(base), _globe(globe), _first(first)
{
	_screen = false;

	// Create objects
	_window = new Window(this, 192, 80, 32, 60, POPUP_BOTH);
	_txtTitle = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 182, 16, 37, 76);
	_edtName = new TextEdit(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 136, 16, 54, 108);
	
	// Set palette
	_game->setPalette(_game->getResourcePack()->getPalette("BACKPALS.DAT")->getColors(Palette::blockOffset(0)), Palette::backPos, 16);

	add(_window);
	add(_txtTitle);
	add(_edtName);

	// Set up objects
	_window->setColor(Palette::blockOffset(8)+8);
	_window->setBackground(_game->getResourcePack()->getSurface("BACK01.SCR"));

	_txtTitle->setColor(Palette::blockOffset(8)+5);
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setBig();
	_txtTitle->setText(_game->getResourcePack()->getLanguage()->getString(STR_BASE_NAME));

	_edtName->setColor(Palette::blockOffset(8)+5);
	_edtName->setBig();
	_edtName->focus();
	_edtName->onKeyboardPress((ActionHandler)&BaseNameState::edtNameKeyPress);
}

/**
 *
 */
BaseNameState::~BaseNameState()
{
	
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void BaseNameState::edtNameKeyPress(Action *action)
{
	if (action->getDetails()->key.keysym.sym == SDLK_RETURN)
	{
		_base->setName(_edtName->getText());
		_game->popState();
		_game->popState();
		if (!_first)
		{
			_game->popState();
			_game->pushState(new PlaceLiftState(_game, _base, _globe));
		}
	}
}