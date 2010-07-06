/*
 * Copyright 2010 Daniel Albano
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
#ifndef OPENXCOM__PLACELIFTSTATE_H
#define OPENXCOM__PLACELIFTSTATE_H

#include <sstream>
#include "State_Interactive.h"
#include "LangString.h"
#include "Palette.h"
#include "TextButton.h"
#include "Window.h"
#include "Text.h"
#include "BaseView.h"
#include "Base.h"
#include "BaseFacility.h"
#include "RuleBaseFacility.h"
#include "BasescapeState.h"

/**
 * Window shown when the player has to
 * place the access lift of a base.
 */
class PlaceLiftState : public State
{
private:
	Base *_base;

	BaseView *_view;
	Text *_txtTitle;
public:
	/// Creates the Place Lift state.
	PlaceLiftState(Game *game, Base *base);
	/// Cleans up the Place Lift state.
	~PlaceLiftState();
	/// Handler for clicking the base view.
	void viewClick(SDL_Event *ev, int scale);
};

#endif