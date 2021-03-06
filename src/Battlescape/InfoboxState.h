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
#ifndef OPENXCOM_INFOBOXSTATE_H
#define OPENXCOM_INFOBOXSTATE_H

#include "../Engine/State.h"
#include "BattlescapeState.h"

namespace OpenXcom
{

class Game;
class Text;
class Timer;

/**
 * Window that briefly shows an info like : Yasuaki Okamoto Has Panicked. It disappears after 2 seconds.
 */
class InfoboxState : public State
{
private:
	Text *_text;
	Window *_window;
	Timer *_timer;
public:
	static const int INFOBOX_DELAY = 2000;
	/// Creates the Infobox state.
	InfoboxState(Game *game, const std::wstring &msg);
	/// Cleans up the Infobox state.
	~InfoboxState();
	/// Handles the timers.
	void think();
	/// Closes the window.
	void close();
	void draw();

};

}

#endif
