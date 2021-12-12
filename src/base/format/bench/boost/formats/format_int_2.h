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

#include <boost/format.hpp>
#include <vector>

static const std::vector<boost::format> formats_int_2 = {
boost::format("AI %d: player has no statistics yet for player %d\n"),
boost::format("Autosave: Rolling savefiles (count): %d of %d\n"),
boost::format("Clearing for road at (%i, %i)\n"),
boost::format("Clearing for waterway at (%i, %i)\n"),
boost::format("ComputerPlayer(%d): genstats entry missing - size :%d\n"),
boost::format("Conquering territory at %3dx%3d for portdock\n"),
boost::format("%d: AI: Erasing statistics for player %d\n"),
boost::format(" %d: AI: game speed too high, jobs are too late (now %2d seconds)\n"),
boost::format("Dimensions:    %d x %d\n"),
boost::format("FAILED! No port buildspace for (%i, %i) found!\n"),
boost::format("Forcing flag at (%i, %i)\n"),
boost::format("[Host]: Client %i: Time %i"),
boost::format("[LAN] Started an IPv%d socket on UDP port %d."),
boost::format("Map origin was shifted by (%d, %d)\n"),
boost::format("Map was resized to %d×%d\n"),
boost::format("Player (%d) is not the sender of an attack (%d)\n"),
boost::format(" Portdock without a fleet created (%3dx%3d)\n"),
boost::format(" pos: %d - value %d\n"),
boost::format("    ... Primary parent: %d, secondary parent: %d\n"),
boost::format("SpinBox: Overall width %d must be bigger than %d (unit width) "),
boost::format("SUCCESS! Port buildspace set for (%i, %i) \n"),
boost::format("The dock on %3dx%3d without a fleet!\n"),
boost::format("The texture atlas must use at least %d as size (%d was given)"),
boost::format("TrainingSite::drop_stalled_soldiers: training step %d,%d "),
boost::format("Unable to create spritesheet; either the width (%d) or the height (%d) "),
boost::format("WareList: %i items of %i left.\n"),
boost::format("worker type %d does not exists (max is %d)"),
boost::format("Wrong number of players. Expected %d but read %d from packet\n"),
boost::format("Wrong player number. Expected %d but read %d from packet\n"),
boost::format("Coordinates: (%1$i, %2$i)")
};

static const uint kNFmtInt2 = formats_int_2.size();

