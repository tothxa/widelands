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

#include "base/string.h"
#include "base/format/bench/format_benchmark.h"

static const std::vector<std::string> other_formats = {
	" %1s %-30s %5d(%3d%%)  %6d %6d %6d %8s %5d %5d %5d %5d\n",
	" %1s %-30s   %5s(perf)  %6s %6s %6s %8s %5s %5s %5s %5s\n",
	"HP: %1$u/%2$u  AT: %3$u/%4$u  DE: %5$u/%6$u  EV: %7$u/%8$u"
};

static const std::vector<std::function<std::string()>> other_format_functions = {
[]() { return bformat(other_formats.at(0),
                      rnd_str(), rnd_str(), rnd_int(), rnd_int(), rnd_int(), rnd_int(),
                      rnd_int(), rnd_str(), rnd_int(), rnd_int(), rnd_int(), rnd_int()); },
[]() { return bformat(other_formats.at(1),
                      rnd_str(), rnd_str(), rnd_str(), rnd_str(), rnd_str(), 
                      rnd_str(), rnd_str(), rnd_str(), rnd_str(), rnd_str(), 
                      rnd_str()); },
[]() { return bformat(other_formats.at(2),
                      rnd_uint(), rnd_uint(), rnd_uint(), rnd_uint(),
                      rnd_uint(), rnd_uint(), rnd_uint(), rnd_uint()); }
};

static const uint kNFuncOther = other_formats.size();

