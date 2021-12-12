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

static const std::vector<boost::format> formats_uint = {
boost::format("AI %u: default starting position already taken!\n"),
boost::format("Are you certain that you want to upgrade these %u add-ons?"),
boost::format("Are you certain that you want to upgrade this %u add-on?"),
boost::format("Attack: %u"),
boost::format("Attempted to change to out-of-range initialization index %u "),
boost::format("Browse (%u)"),
boost::format("   Building ID      %7u"),
boost::format("Checking starting position for all %u players:\n"),
boost::format("Comments (%u)"),
boost::format("Computer %u"),
boost::format("Could not find a suitable place for player %u\n"),
boost::format("Create custom mutex #%u."),
boost::format("Defense: %u"),
boost::format("       Desired fill %7u"),
boost::format("     Desired fill   %7u"),
boost::format("Do you really want to discard all changes to the following %u add-on?"),
boost::format("duplicate use of index %u"),
boost::format("ERROR: The message queue for player %u contains a message "),
boost::format("Evade: %u"),
boost::format("expected 1 but found %u"),
boost::format("Graphics: OpenGL: Max texture size: %u\n"),
boost::format("Health: %u"),
boost::format("[Host]: Client %u: got pong"),
boost::format("[Host]: Unexpected command %u while in game\n"),
boost::format("   icons_.size()    %7u"),
boost::format("       Index        %7u"),
boost::format("index %u is unused"),
boost::format("Installed (%u)"),
boost::format("InternetGaming: Received a client list update with %u items."),
boost::format("InternetGaming: Received a game list update with %u items."),
boost::format("It is not recommended to permit waterway lengths greater than %u"),
boost::format("       Max fill     %7u"),
boost::format("     Max fill       %7u"),
boost::format("MO(%u): Carrier::find_closest_flag: not on road, not on "),
boost::format("MO(%u): [dropoff]: not on building on return"),
boost::format("MO(%u): [dropoff]: not on building or on flag - fishy"),
boost::format("MO(%u): init_auto_task() failed to schedule something"),
boost::format("MO(%u): init_auto_task() failed to set a task"),
boost::format("MO(%u): start_task_movepath(index): not on path"),
boost::format("MO(%u): start_task_return(): not owned by building or flag"),
boost::format("MO(%u): [waitleavebuilding]: buildings do not match"),
boost::format("MO(%u): wakeup_flag_capacity: Flags do not match."),
boost::format("[NetHost] Opening a listening IPv4 socket on TCP port %u"),
boost::format("[NetHost] Opening a listening IPv6 socket on TCP port %u"),
boost::format("   nr_icons_        %7u"),
boost::format("Omitting screenshot because diskspace is lower than %lluMiB\n"),
boost::format("Owned by: Player %u"),
boost::format("Player::bulldoze(%u): bad immovable type"),
boost::format("Players: %u"),
boost::format("-> Player %u: "),
boost::format("Player %u"),
boost::format("Player %u: –"),
boost::format("       Priority     %7u"),
boost::format("     Priority       %7u"),
boost::format("Random number: %u\n"),
boost::format("REPLAY: End of replay (gametime: %u)\n"),
boost::format("Road length: %u"),
boost::format("Screenshots (%u)"),
boost::format("Shared in Player %u"),
boost::format("Team %u won!"),
boost::format("Too many arguments provided (expected only %u)"),
boost::format("Total size: %u byte"),
boost::format("Total size: %u bytes"),
boost::format("transfer target %u is neither ware nor worker"),
boost::format("×%u"),
boost::format("%u add-on mismatch detected:\n"),
boost::format("%u add-on mismatches detected:\n"),
boost::format("%u add-ons with wrong version"),
boost::format("%u add-on with wrong version"),
boost::format("%u bytes"),
boost::format("%u comment"),
boost::format("%u comment:"),
boost::format("%u comments"),
boost::format("%u comments:"),
boost::format("%u Dependency Error"),
boost::format("%u Dependency Errors"),
boost::format("%u download"),
boost::format("%u downloads"),
boost::format("%u missing add-on"),
boost::format("%u missing add-ons"),
boost::format("%u new message"),
boost::format("%u new messages"),
boost::format("Unknown packet %u"),
boost::format("Unknown Queue_Cmd_Id in file: %u"),
boost::format("unnamed %u"),
boost::format("Upgrade all (%u)"),
boost::format("Upgrade the following %u add-on:"),
boost::format("Upgrade the following %u add-ons:"),
boost::format("%u Player"),
boost::format("%u Players"),
boost::format("%u screenshot"),
boost::format("%u screenshots"),
boost::format("%u soldier"),
boost::format("%u soldiers"),
boost::format("Votes (%u)"),
boost::format("WarehouseSupply::~WarehouseSupply: Warehouse %u still belongs to "),
boost::format("WareInstance delivered to bad location %u"),
boost::format("   #wares           %7u"),
boost::format("WARE(%u): WareInstance::update has no location\n"),
boost::format("   #workers       %7u"),
boost::format("%u%% dismantled"),
boost::format("cargo for %" PRIuS " destinations\n"),
boost::format("Expected 3 entries for RGB color, but got %" PRIuS "."),
boost::format("Expected 4 entries for RGBA color, but got %" PRIuS "."),
boost::format("expected format sequence after '%%%u', found \\0"),
boost::format("PlayerImmovable::~PlayerImmovable: %" PRIuS " workers left!\n"),
boost::format("%" PRIuS " found ... "),
boost::format("  SCHEDULE: %" PRIuS " stations\n")
};

static const uint kNFmtUint = formats_uint.size();

