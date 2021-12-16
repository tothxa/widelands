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
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "base/string.h"
#include "base/format/bench/format_benchmark.h"
#include "base/format/bench/format_functions.h"
#include "base/format/bench/func_format_other.h"

RNG rng;

constexpr uint kRepeat = 100;

void compare_formatting(uint32_t seed) {
	uint i;
	uint j;
	std::string input_str;
	int input_int;
	uint input_uint;
	double input_float;
	std::string result_boost;
	std::string result_bformat;
	uint size_boost;
	uint size_bformat;
	uint count_ok = 0;
	uint count_diff = 0;

	rng.reset();
	rng.seed(seed);

	std::cout << " ----- %s -----\n";
	for (i = 0; i < kNFmtStr; ++i) {
		boost::format format(formats_str.at(i));
		for (j = 0; j < kRepeat; ++j) {
			input_str = rnd_str();
			result_boost=(format % input_str).str();
			result_bformat=bformat(formats_str.at(i), input_str);
			size_boost = result_boost.size();
			size_bformat = result_bformat.size();
			if (result_boost == result_bformat && size_boost == size_bformat) {
				++count_ok;
			} else {
				++count_diff;
				std::cout << bformat("'%s', '%s' -> '%s' (%u) / '%s' (%u)\n",
				                     formats_str.at(i), input_str, result_boost, size_boost,
				                     result_bformat, size_bformat);
			}
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %i -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kNFmtInt; ++i) {
		boost::format format(formats_int.at(i));
		for (j = 0; j < kRepeat; ++j) {
			input_int = rnd_int();
			result_boost=(format % input_int).str();
			result_bformat=bformat(formats_int.at(i), input_int);
			size_boost = result_boost.size();
			size_bformat = result_bformat.size();
			if (result_boost == result_bformat && size_boost == size_bformat) {
				++count_ok;
			} else {
				++count_diff;
				std::cout << bformat("'%s', '%i' -> '%s' (%u) / '%s' (%u)\n",
				                     formats_int.at(i), input_int, result_boost, size_boost,
				                     result_bformat, size_bformat);
			}
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %u -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kNFmtUint; ++i) {
		boost::format format(formats_uint.at(i));
		for (j = 0; j < kRepeat; ++j) {
			input_uint = rnd_uint();
			result_boost=(format % input_uint).str();
			result_bformat=bformat(formats_uint.at(i), input_uint);
			size_boost = result_boost.size();
			size_bformat = result_bformat.size();
			if (result_boost == result_bformat && size_boost == size_bformat) {
				++count_ok;
			} else {
				++count_diff;
				std::cout << bformat("'%s', '%u' -> '%s' (%u) / '%s' (%u)\n",
				                     formats_uint.at(i), input_uint, result_boost, size_boost,
				                     result_bformat, size_bformat);
			}
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %f -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kNFmtFloat; ++i) {
		boost::format format(formats_float.at(i));
		for (j = 0; j < kRepeat; ++j) {
			input_float = rnd_float();
			result_boost=(format % input_float).str();
			result_bformat=bformat(formats_float.at(i), input_float);
			size_boost = result_boost.size();
			size_bformat = result_bformat.size();
			if (result_boost == result_bformat && size_boost == size_bformat) {
				++count_ok;
			} else {
				++count_diff;
				std::cout << bformat("'%s', '%.8f' -> '%s' (%u) / '%s' (%u)\n",
				                     formats_float.at(i), input_float, result_boost, size_boost,
				                     result_bformat, size_bformat);
			}
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %p -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kNFmtPtr; ++i) {
		boost::format format(formats_ptr.at(i));
		for (j = 0; j < kRepeat; ++j) {
			input_uint = rnd_uint();
			result_boost=(format % input_uint).str();
			result_bformat=bformat(formats_ptr.at(i), input_uint);
			size_boost = result_boost.size();
			size_bformat = result_bformat.size();
			if (result_boost == result_bformat && size_boost == size_bformat) {
				++count_ok;
			} else {
				++count_diff;
				std::cout << bformat("'%s', '%u' -> '%s' (%u) / '%s' (%u)\n",
				                     formats_uint.at(i), input_uint, result_boost, size_boost,
				                     result_bformat, size_bformat);
			}
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

/*
	std::cout << "\n ----- %1% -----\n";
	for (i = 0; i < kNFmtWild1; ++i) {
		std::cout << f_wild_1.at(i % kNFuncWild1)(formats_wild_1.at(i)) << "\n";
	}
*/
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		compare_formatting(0x12345678);
		return 0;
	}
	for (int i = 1 ; i < argc ; ++i) {
		compare_formatting(strtol(argv[i], nullptr, 16));
	}
}

