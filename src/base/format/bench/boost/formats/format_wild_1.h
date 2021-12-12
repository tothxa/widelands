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

static const std::vector<boost::format> formats_wild_1 = {
boost::format("%1%:"),
boost::format("%1%d"),
boost::format("%1%G"),
boost::format("%1%h"),
boost::format("%1% hour"),
boost::format("%1% hours"),
boost::format("%1%k"),
boost::format("%1%m"),
boost::format("%1%M"),
boost::format("%1% soldier"),
boost::format("%1% soldiers"),
boost::format("Autosave: %1%"),
boost::format("Copyright 2002 - %1% by the Widelands Development Team."),
boost::format("H (%1%)"),
boost::format("Increased by %1% point for each level."),
boost::format("Increased by %1% points for each level."),
boost::format("MP (%1%)"),
boost::format("Starts at %1% points."),
boost::format("The baker needs %1% on average to bake a loaf of bread."),
boost::format("The brewer needs %1% on average to brew a vat of beer."),
boost::format("The farmer needs %1% on average to sow and harvest a sheaf of corn."),
boost::format("The farmer needs %1% on average to sow and harvest a sheaf of wheat."),
boost::format("The innkeeper needs %1% on average to prepare a ration."),
boost::format("The maximum level is %1%."),
boost::format("The miller needs %1% on average to grind wheat into a sack of flour."),
boost::format("The pig breeder needs %1% on average to raise and slaughter a pig."),
boost::format("Your second ship, \"%1%\", is ready now.")
};

static const uint kNFmtWild1 = formats_wild_1.size();

