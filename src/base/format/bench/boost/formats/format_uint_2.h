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

static const std::vector<boost::format> formats_uint_2 = {
boost::format("[Client] speed: %u.%03u"),
boost::format("CmdProposeTrade: sender %u, but market owner %u\n"),
boost::format("EconomyOptionsWindow::save: No flag exists in both economies (%u & %u)"),
boost::format("expected %u but found %u"),
boost::format("Graphics: Try to set Videomode %ux%u\n"),
boost::format("header is %u, expected %u"),
boost::format("[Host]: Client %u: welcome to usernum %u"),
boost::format("          · in %u ms at %u\n"),
boost::format("MO(%u): Flag::remove_ware: ware %u not on flag"),
boost::format("MO(%u): get_building_work() for unknown worker %u"),
boost::format("MO(%u): Received a ware(%u), do not know what to do with it"),
boost::format("[sync:%08u t=%6u]"),
boost::format("Too few arguments provided: %u required but only %u passed"),
boost::format("            – %u items to %u\n"),
boost::format("  – %u items to %u\n"),
boost::format("worker %u is in the capacity wait queue of flag %u but "),
boost::format("worker %u is in the leave queue of building %u but "),
boost::format("worker %u is in the leave queue of building %u but its "),
boost::format("worker %u is in the leave queue of building %u with "),
boost::format("Wrong number of arguments: expected %u, found %u"),
boost::format("%1$u / %2$u"),
boost::format("%1$u.%2$u×"),
boost::format("%1$u×%2$u"),
boost::format("Waterway length: %1$u/%2$u")
};

static const uint kNFmtUint2 = formats_uint_2.size();

