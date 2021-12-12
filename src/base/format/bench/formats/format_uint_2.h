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

static const std::vector<std::string> formats_uint_2 = {
"[Client] speed: %u.%03u",
"CmdProposeTrade: sender %u, but market owner %u\n",
"EconomyOptionsWindow::save: No flag exists in both economies (%u & %u)",
"expected %u but found %u",
"Graphics: Try to set Videomode %ux%u\n",
"header is %u, expected %u",
"[Host]: Client %u: welcome to usernum %u",
"          · in %u ms at %u\n",
"MO(%u): Flag::remove_ware: ware %u not on flag",
"MO(%u): get_building_work() for unknown worker %u",
"MO(%u): Received a ware(%u), do not know what to do with it",
"[sync:%08u t=%6u]",
"Too few arguments provided: %u required but only %u passed",
"            – %u items to %u\n",
"  – %u items to %u\n",
"worker %u is in the capacity wait queue of flag %u but ",
"worker %u is in the leave queue of building %u but ",
"worker %u is in the leave queue of building %u but its ",
"worker %u is in the leave queue of building %u with ",
"Wrong number of arguments: expected %u, found %u",
"%1$u / %2$u",
"%1$u.%2$u×",
"%1$u×%2$u",
"Waterway length: %1$u/%2$u"
};

static const uint kNFmtUint2 = formats_uint_2.size();

