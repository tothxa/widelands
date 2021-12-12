/*
 * Copyright (C) 2021 by the Widelands Development Team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <string>
#include <vector>

static const std::vector<std::string> formats_int_2 = {
"AI %d: player has no statistics yet for player %d\n",
"Autosave: Rolling savefiles (count): %d of %d\n",
"Clearing for road at (%i, %i)\n",
"Clearing for waterway at (%i, %i)\n",
"ComputerPlayer(%d): genstats entry missing - size :%d\n",
"Conquering territory at %3dx%3d for portdock\n",
"%d: AI: Erasing statistics for player %d\n",
" %d: AI: game speed too high, jobs are too late (now %2d seconds)\n",
"Dimensions:    %d x %d\n",
"FAILED! No port buildspace for (%i, %i) found!\n",
"Forcing flag at (%i, %i)\n",
"[Host]: Client %i: Time %i",
"[LAN] Started an IPv%d socket on UDP port %d.",
"Map origin was shifted by (%d, %d)\n",
"Map was resized to %d×%d\n",
"Player (%d) is not the sender of an attack (%d)\n",
" Portdock without a fleet created (%3dx%3d)\n",
" pos: %d - value %d\n",
"    ... Primary parent: %d, secondary parent: %d\n",
"SpinBox: Overall width %d must be bigger than %d (unit width) ",
"SUCCESS! Port buildspace set for (%i, %i) \n",
"The dock on %3dx%3d without a fleet!\n",
"The texture atlas must use at least %d as size (%d was given)",
"TrainingSite::drop_stalled_soldiers: training step %d,%d ",
"Unable to create spritesheet; either the width (%d) or the height (%d) ",
"WareList: %i items of %i left.\n",
"worker type %d does not exists (max is %d)",
"Wrong number of players. Expected %d but read %d from packet\n",
"Wrong player number. Expected %d but read %d from packet\n",
"Coordinates: (%1$i, %2$i)"
};

static const uint kNFmtInt2 = formats_int_2.size();

