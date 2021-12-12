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
#include <string>
#include <vector>

#include "base/format/bench/boost/format_benchmark.h"

static std::vector<boost::format> other_formats = {
	boost::format(" %1s %-30s %5d(%3d%%)  %6d %6d %6d %8s %5d %5d %5d %5d\n"),
	boost::format(" %1s %-30s   %5s(perf)  %6s %6s %6s %8s %5s %5s %5s %5s\n"),
	boost::format("HP: %1$u/%2$u  AT: %3$u/%4$u  DE: %5$u/%6$u  EV: %7$u/%8$u")
};

static const std::vector<std::function<std::string()>> other_format_functions = {
[]() { return (other_formats.at(0) %
                      rnd_str() % rnd_str() % rnd_int() % rnd_int() % rnd_int() % rnd_int() %
                      rnd_int() % rnd_str() % rnd_int() % rnd_int() % rnd_int() % rnd_int()).str(); },
[]() { return (other_formats.at(1) %
                      rnd_str() % rnd_str() % rnd_str() % rnd_str() % rnd_str() % 
                      rnd_str() % rnd_str() % rnd_str() % rnd_str() % rnd_str() % 
                      rnd_str()).str(); },
[]() { return (other_formats.at(2) %
                      rnd_uint() % rnd_uint() % rnd_uint() % rnd_uint() %
                      rnd_uint() % rnd_uint() % rnd_uint() % rnd_uint()).str(); }
};

static const uint kNFuncOther = other_formats.size();

