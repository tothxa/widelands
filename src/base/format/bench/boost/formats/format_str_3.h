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

static const std::vector<boost::format> formats_str_3 = {
boost::format("Autosave: Unable to roll file %s to %s: %s\n"),
boost::format("Error downloading screenshot %s for %s: %s"),
boost::format("Map requires add-on '%s' at version %s but version %s is installed. "),
boost::format("%s + \"%s\" + \"%s\"\n"),
boost::format("%s to %s (%s EP)"),
boost::format("%s: using '%s' instead of '%s'"),
boost::format("%1$s · %2$s · %3$s"),
boost::format("%1$s and %2$s: %3$s"),
boost::format("%1$s<br>· %2$s (version %3$s)"),
boost::format("%1$s (edited by ‘%2$s’ on %3$s)"),
boost::format("%1$s (expected version %2$s, found %3$s)"),
boost::format("The smuggling route worth %1$s from %2$s to %3$s has been broken!"),
boost::format("WARNING: %1$s uses version: %2$s, while Host uses version: %3$s")
};

static const uint kNFmtStr3 = formats_str_3.size();

