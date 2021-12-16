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

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "base/format/bench/boost/format_benchmark.h"
#include "base/format/bench/boost/format_functions.h"
#include "base/format/bench/boost/func_format_other.h"

RNG rng;

boost::format rnd_state_report_fmt("Random state: %08x\n");

void use_all_formats(uint32_t seed) {
	uint i;

	rng.reset();
	rng.seed(seed);

	for (i = 0; i < kNFmtStr; ++i) {
		std::cout << f_str(formats_str.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtInt; ++i) {
		std::cout << f_int(formats_int.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtUint; ++i) {
		std::cout << f_uint(formats_uint.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStr2; ++i) {
		std::cout << f_str_2(formats_str_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtInt2; ++i) {
		std::cout << f_int_2(formats_int_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtChar; ++i) {
		std::cout << f_char(formats_char.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtFloat; ++i) {
		std::cout << f_float(formats_float.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtFloatUint; ++i) {
		std::cout << f_float_uint(formats_float_uint.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtInt3; ++i) {
		std::cout << f_int_3(formats_int_3.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtInt4; ++i) {
		std::cout << f_int_4(formats_int_4.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtIntStr; ++i) {
		std::cout << f_int_str(formats_int_str.at(i)) << "\n";
	}
/*
	for (i = 0; i < kNFmtPercent; ++i) {
		std::cout << f_percent(formats_percent.at(i)) << "\n";
	}
*/
	for (i = 0; i < kNFmtPtr; ++i) {
		std::cout << f_ptr(formats_ptr.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStr3; ++i) {
		std::cout << f_str_3(formats_str_3.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStr4; ++i) {
		std::cout << f_str_4(formats_str_4.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrFloatInt2; ++i) {
		std::cout << f_str_float_int_2(formats_str_float_int_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrInt; ++i) {
		std::cout << f_str_int(formats_str_int.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrInt2; ++i) {
		std::cout << f_str_int_2(formats_str_int_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrInt3; ++i) {
		std::cout << f_str_int_3(formats_str_int_3.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrUint; ++i) {
		std::cout << f_str_uint(formats_str_uint.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrUint2; ++i) {
		std::cout << f_str_uint_2(formats_str_uint_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrUintInt; ++i) {
		std::cout << f_str_uint_int(formats_str_uint_int.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtStrUintStr; ++i) {
		std::cout << f_str_uint_str(formats_str_uint_str.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtUint2; ++i) {
		std::cout << f_uint_2(formats_uint_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtUint3; ++i) {
		std::cout << f_uint_3(formats_uint_3.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtUintStr; ++i) {
		std::cout << f_uint_str(formats_uint_str.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtUintStr2; ++i) {
		std::cout << f_uint_str_2(formats_uint_str_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtWild1; ++i) {
		std::cout << f_wild_1.at(i % kNFuncWild1)(formats_wild_1.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtWild2; ++i) {
		std::cout << f_wild_2.at(i % kNFuncWild2)(formats_wild_2.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtWild3; ++i) {
		std::cout << f_wild_3.at(i % kNFuncWild3)(formats_wild_3.at(i)) << "\n";
	}
	for (i = 0; i < kNFmtWild4; ++i) {
		std::cout << f_wild_4.at(i % kNFuncWild4)(formats_wild_4.at(i)) << "\n";
	}
	for (i = 0; i < kNFuncOther; ++i) {
		std::cout << other_format_functions.at(i)() << "\n";
	}
	std::cout << (rnd_state_report_fmt % rng.rand());
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		use_all_formats(0x12345678);
		return 0;
	}
	for (int i = 1 ; i < argc ; ++i) {
		use_all_formats(strtol(argv[i], nullptr, 16));
	}
}

