/*
 * Copyright (C) 2006-2025 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef WL_SCRIPTING_GLOBALS_H
#define WL_SCRIPTING_GLOBALS_H

#include "logic/game.h"
#include "map_io/map_object_loader.h"
#include "map_io/map_object_saver.h"

namespace FsMenu {
class MainMenu;
}  // namespace FsMenu

bool is_main_menu(lua_State*);
FsMenu::MainMenu& get_main_menu(lua_State*);

Widelands::Game& get_game(lua_State*);
Widelands::EditorGameBase& get_egbase(lua_State*);

Widelands::MapObjectLoader* get_mol(lua_State*);
Widelands::MapObjectSaver* get_mos(lua_State*);

#endif  // end of include guard: WL_SCRIPTING_GLOBALS_H
