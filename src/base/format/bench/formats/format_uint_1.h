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

static const std::vector<std::string> formats_uint = {
"AI %u: default starting position already taken!\n",
"Are you certain that you want to upgrade these %u add-ons?",
"Are you certain that you want to upgrade this %u add-on?",
"Attack: %u",
"Attempted to change to out-of-range initialization index %u ",
"Browse (%u)",
"   Building ID      %7u",
"Checking starting position for all %u players:\n",
"Comments (%u)",
"Computer %u",
"Could not find a suitable place for player %u\n",
"Create custom mutex #%u.",
"Defense: %u",
"       Desired fill %7u",
"     Desired fill   %7u",
"Do you really want to discard all changes to the following %u add-on?",
"duplicate use of index %u",
"ERROR: The message queue for player %u contains a message ",
"Evade: %u",
"expected 1 but found %u",
"Graphics: OpenGL: Max texture size: %u\n",
"Health: %u",
"[Host]: Client %u: got pong",
"[Host]: Unexpected command %u while in game\n",
"   icons_.size()    %7u",
"       Index        %7u",
"index %u is unused",
"Installed (%u)",
"InternetGaming: Received a client list update with %u items.",
"InternetGaming: Received a game list update with %u items.",
"It is not recommended to permit waterway lengths greater than %u",
"       Max fill     %7u",
"     Max fill       %7u",
"MO(%u): Carrier::find_closest_flag: not on road, not on ",
"MO(%u): [dropoff]: not on building on return",
"MO(%u): [dropoff]: not on building or on flag - fishy",
"MO(%u): init_auto_task() failed to schedule something",
"MO(%u): init_auto_task() failed to set a task",
"MO(%u): start_task_movepath(index): not on path",
"MO(%u): start_task_return(): not owned by building or flag",
"MO(%u): [waitleavebuilding]: buildings do not match",
"MO(%u): wakeup_flag_capacity: Flags do not match.",
"[NetHost] Opening a listening IPv4 socket on TCP port %u",
"[NetHost] Opening a listening IPv6 socket on TCP port %u",
"   nr_icons_        %7u",
"Omitting screenshot because diskspace is lower than %lluMiB\n",
"Owned by: Player %u",
"Player::bulldoze(%u): bad immovable type",
"Players: %u",
"-> Player %u: ",
"Player %u",
"Player %u: –",
"       Priority     %7u",
"     Priority       %7u",
"Random number: %u\n",
"REPLAY: End of replay (gametime: %u)\n",
"Road length: %u",
"Screenshots (%u)",
"Shared in Player %u",
"Team %u won!",
"Too many arguments provided (expected only %u)",
"Total size: %u byte",
"Total size: %u bytes",
"transfer target %u is neither ware nor worker",
"×%u",
"%u add-on mismatch detected:\n",
"%u add-on mismatches detected:\n",
"%u add-ons with wrong version",
"%u add-on with wrong version",
"%u bytes",
"%u comment",
"%u comment:",
"%u comments",
"%u comments:",
"%u Dependency Error",
"%u Dependency Errors",
"%u download",
"%u downloads",
"%u missing add-on",
"%u missing add-ons",
"%u new message",
"%u new messages",
"Unknown packet %u",
"Unknown Queue_Cmd_Id in file: %u",
"unnamed %u",
"Upgrade all (%u)",
"Upgrade the following %u add-on:",
"Upgrade the following %u add-ons:",
"%u Player",
"%u Players",
"%u screenshot",
"%u screenshots",
"%u soldier",
"%u soldiers",
"Votes (%u)",
"WarehouseSupply::~WarehouseSupply: Warehouse %u still belongs to ",
"WareInstance delivered to bad location %u",
"   #wares           %7u",
"WARE(%u): WareInstance::update has no location\n",
"   #workers       %7u",
"%u%% dismantled",
"cargo for %" PRIuS " destinations\n",
"Expected 3 entries for RGB color, but got %" PRIuS ".",
"Expected 4 entries for RGBA color, but got %" PRIuS ".",
"expected format sequence after '%%%u', found \\0",
"PlayerImmovable::~PlayerImmovable: %" PRIuS " workers left!\n",
"%" PRIuS " found ... ",
"  SCHEDULE: %" PRIuS " stations\n"
};

static const uint kNFmtUint = formats_uint.size();

