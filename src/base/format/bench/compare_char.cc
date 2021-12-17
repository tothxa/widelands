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
// #include <cstddef>
// #include <cstdlib>
// #include <cstring>
#include <iostream>
#include <string>

#include "base/string.h"
#include "base/format/bench/format_benchmark.h"

boost::format format;

int main(int, char**) {
	int c;
	char input_char;
	std::string result_boost;
	std::string result_bformat;
	uint size_boost;
	uint size_bformat;
	uint count_ok = 0;
	uint count_diff = 0;

	std::cout << "\n ----- '%c' -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%c");
	for (c = 0; c < 256; ++c) {
		input_char = c;
		result_boost=(format % input_char).str();
		result_bformat=bformat("%c", input_char);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%p' (%u) -> " /* '%s' */ " (%u) / " /* '%s' */ " (%u)\n",
			                     c, c,  /* result_boost, */  size_boost,
			                     /* result_bformat, */  size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- '-%c-' -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("-%c-");
	for (c = 0; c < 256; ++c) {
		input_char = c;
		result_boost=(format % input_char).str();
		result_bformat=bformat("-%c-", input_char);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%p' (%u) -> " /* '%s' */ " (%u) / " /* '%s' */ " (%u)\n",
			                     c, c,  /* result_boost, */  size_boost,
			                     /* result_bformat, */  size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- '%1%' char -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%1%");
	for (c = 0; c < 256; ++c) {
		input_char = c;
		result_boost=(format % input_char).str();
		result_bformat=bformat("%1%", input_char);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%p' (%u) -> (%u) / (%u)\n",
			                     c, c, size_boost, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- '-%1%-' char -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("-%1%-");
	for (c = 0; c < 256; ++c) {
		input_char = c;
		result_boost=(format % input_char).str();
		result_bformat=bformat("-%1%-", input_char);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%p' (%u) -> (%u) / (%u)\n",
			                     c, c, size_boost, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

}

