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

static const std::vector<boost::format> formats_uint_str = {
boost::format("[Host]: disconnect_player_controller(%u, %s)\n"),
boost::format("Invalid category %u for addon %s"),
boost::format("MO(%u): [actObject]: bad object type %s"),
boost::format("MO(%u): [actWalk]: bad object type %s"),
boost::format("MO(%u): update[%s] failed to act"),
boost::format("Player %u stock statistics: unknown ware name %s"),
boost::format("soldier 1 (%u): %s"),
boost::format("soldier 2 (%u): %s"),
boost::format("transfer %u: %s"),
boost::format("%u: %s"),
boost::format("worker %u: %s"),
boost::format("%1$u add-ons with wrong version: %2$s"),
boost::format("%1$u add-on with wrong version: %2$s"),
boost::format("%1$u missing add-on: %2$s"),
boost::format("%1$u missing add-ons: %2$s")
};

static const uint kNFmtUintStr = formats_uint_str.size();

