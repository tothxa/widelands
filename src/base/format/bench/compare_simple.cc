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
// #include "base/format/bench/format_functions.h"
// #include "base/format/bench/func_format_other.h"

RNG rng;

boost::format format;

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
	format = boost::format("%s");
	for (j = 0; j < kRepeat; ++j) {
		input_str = rnd_str();
		result_boost=(format % input_str).str();
		result_bformat=bformat("%s", input_str);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%s' -> '%s' (%u) / '%s' (%u)\n",
			                     input_str, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %i -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%i");
	for (j = 0; j < kRepeat; ++j) {
		input_int = rnd_int();
		result_boost=(format % input_int).str();
		result_bformat=bformat("%i", input_int);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%i' -> '%s' (%u) / '%s' (%u)\n",
			                     input_int, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %x -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%x");
	for (j = 0; j < kRepeat; ++j) {
		input_int = rnd_int();
		result_boost=(format % input_int).str();
		result_bformat=bformat("%x", input_int);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%i' -> '%s' (%u) / '%s' (%u)\n",
			                     input_int, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %u -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%u");
	for (j = 0; j < kRepeat; ++j) {
		input_uint = rnd_uint();
		result_boost=(format % input_uint).str();
		result_bformat=bformat("%u", input_uint);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%u' -> '%s' (%u) / '%s' (%u)\n",
			                     input_uint, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %f -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%10.3f");
	for (j = 0; j < kRepeat; ++j) {
		input_float = rnd_float(999999ul) * j;
		result_boost=(format % input_float).str();
		result_bformat=bformat("%10.3f", input_float);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%s', '%.8f' -> '%s' (%u) / '%s' (%u)\n",
			                     "%10.3f", input_float, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %p -----\n";
	count_ok = 0;
	count_diff = 0;
	format = boost::format("%p");
	for (j = 0; j < kRepeat; ++j) {
		input_uint = rnd_uint();
		result_boost=(format % input_uint).str();
		result_bformat=bformat("%p", input_uint);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%u' -> '%s' (%u) / '%s' (%u)\n",
			                     input_uint, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

//	std::cout << "\n ----- %1% -----\n";
	format = boost::format("%1%");

	std::cout << "\n ----- %1% char -----\n";
	count_ok = 0;
	count_diff = 0;
	char input_char;
	for (int c = 0; c < 256; ++c) {
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

	std::cout << "\n ----- %1% float -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kRepeat; ++i) {
		input_float = rnd_float(9999999ul) * i;
		result_boost=(format % input_float).str();
		result_bformat=bformat("%1%", input_float);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%f' -> '%s' (%u) / '%s' (%u)\n",
			                     input_float, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %1% int -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kRepeat; ++i) {
		input_int = rnd_int();
		result_boost=(format % input_int).str();
		result_bformat=bformat("%1%", input_int);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%d' -> '%s' (%u) / '%s' (%u)\n",
			                     input_int, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %1% ptr -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kRepeat; ++i) {
		uint* input_ptr = rnd_int() >= 0 ? &i + rnd_ptr() : &i - rnd_ptr();
		result_boost=(format % input_ptr).str();
		result_bformat=bformat("%1%", input_ptr);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%p' -> '%s' (%u) / '%s' (%u)\n",
			                     input_ptr, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %1% str -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kRepeat; ++i) {
		input_str = rnd_str();
		result_boost=(format % input_str).str();
		result_bformat=bformat("%1%", input_str);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%s' -> '%s' (%u) / '%s' (%u)\n",
			                     input_str, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);

	std::cout << "\n ----- %1% uint -----\n";
	count_ok = 0;
	count_diff = 0;
	for (i = 0; i < kRepeat; ++i) {
		input_uint = rnd_uint();
		result_boost=(format % input_uint).str();
		result_bformat=bformat("%1%", input_uint);
		size_boost = result_boost.size();
		size_bformat = result_bformat.size();
		if (result_boost == result_bformat && size_boost == size_bformat) {
			++count_ok;
		} else {
			++count_diff;
			std::cout << bformat("'%u' -> '%s' (%u) / '%s' (%u)\n",
			                     input_uint, result_boost, size_boost,
			                     result_bformat, size_bformat);
		}
	}
	std::cout << bformat("\nOK: %5d      Diff: %5d\n", count_ok, count_diff);
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

