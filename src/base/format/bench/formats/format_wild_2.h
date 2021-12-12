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

static const std::vector<std::string> formats_wild_2 = {
"%1% %2%",
"%1% (%2%)",
"%1% / %2%",
"%1% × %2%",
"%1%/%2%",
"%1%: %2%",
"%1%× %2%",
"%1%(+%2%) soldier",
"%1%(+%2%) soldiers",
"%1% and %2%",
"%1% hour and %2%",
"%1% hours and %2%",
"%1% soldier (+%2%)",
"%1% soldiers (+%2%)",
"%1%x %2%",
"An unexpected error message has been received about command %1%: %2%",
"A random value between %1% and %2% points is added to each attack.",
"Autosave %1%: %2%",
"Press %1% to store important locations, then press %2% to recall them.",
"saved today at %1%:%2%",
"saved yesterday at %1%:%2%",
"Selected %1% directories and %2%:",
"Selected %1% directory and %2%:",
"Size: %1% × %2%",
"Size: %1% x %2%",
"The translation into %1% is %2%%% complete.",
"Today, %1%:%2%",
"Trains ‘Attack’ from %1% up to %2%",
"Trains ‘Defense’ from %1% up to %2%",
"Trains ‘Evade’ from %1% up to %2%",
"Trains ‘Health’ from %1% up to %2%",
"Yesterday, %1%:%2%"
};

static const uint kNFmtWild2 = formats_wild_2.size();

