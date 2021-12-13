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

#include "base/string.h"

#include "base/format/bench/format_benchmark.h"
#include "base/format/bench/format_functions.h"
#include "base/format/bench/func_format_other.h"

uint seq_all() {
	uint sum = 0;
	uint i;
	for (i = 0; i < kNFmtStr; ++i) {
		sum += f_str(formats_str.at(i)).size();
	}
	for (i = 0; i < kNFmtInt; ++i) {
		sum += f_int(formats_int.at(i)).size();
	}
	for (i = 0; i < kNFmtUint; ++i) {
		sum += f_uint(formats_uint.at(i)).size();
	}
	for (i = 0; i < kNFmtStr2; ++i) {
		sum += f_str_2(formats_str_2.at(i)).size();
	}
	for (i = 0; i < kNFmtInt2; ++i) {
		sum += f_int_2(formats_int_2.at(i)).size();
	}
	for (i = 0; i < kNFmtChar; ++i) {
		sum += f_char(formats_char.at(i)).size();
	}
	for (i = 0; i < kNFmtFloat; ++i) {
		sum += f_float(formats_float.at(i)).size();
	}
	for (i = 0; i < kNFmtFloatUint; ++i) {
		sum += f_float_uint(formats_float_uint.at(i)).size();
	}
	for (i = 0; i < kNFmtInt3; ++i) {
		sum += f_int_3(formats_int_3.at(i)).size();
	}
	for (i = 0; i < kNFmtInt4; ++i) {
		sum += f_int_4(formats_int_4.at(i)).size();
	}
	for (i = 0; i < kNFmtIntStr; ++i) {
		sum += f_int_str(formats_int_str.at(i)).size();
	}
/*
	for (i = 0; i < kNFmtPercent; ++i) {
		sum += f_percent(formats_percent.at(i)).size();
	}
*/
	for (i = 0; i < kNFmtPtr; ++i) {
		sum += f_ptr(formats_ptr.at(i)).size();
	}
	for (i = 0; i < kNFmtStr3; ++i) {
		sum += f_str_3(formats_str_3.at(i)).size();
	}
	for (i = 0; i < kNFmtStr4; ++i) {
		sum += f_str_4(formats_str_4.at(i)).size();
	}
	for (i = 0; i < kNFmtStrFloatInt2; ++i) {
		sum += f_str_float_int_2(formats_str_float_int_2.at(i)).size();
	}
	for (i = 0; i < kNFmtStrInt; ++i) {
		sum += f_str_int(formats_str_int.at(i)).size();
	}
	for (i = 0; i < kNFmtStrInt2; ++i) {
		sum += f_str_int_2(formats_str_int_2.at(i)).size();
	}
	for (i = 0; i < kNFmtStrInt3; ++i) {
		sum += f_str_int_3(formats_str_int_3.at(i)).size();
	}
	for (i = 0; i < kNFmtStrUint; ++i) {
		sum += f_str_uint(formats_str_uint.at(i)).size();
	}
	for (i = 0; i < kNFmtStrUint2; ++i) {
		sum += f_str_uint_2(formats_str_uint_2.at(i)).size();
	}
	for (i = 0; i < kNFmtStrUintInt; ++i) {
		sum += f_str_uint_int(formats_str_uint_int.at(i)).size();
	}
	for (i = 0; i < kNFmtStrUintStr; ++i) {
		sum += f_str_uint_str(formats_str_uint_str.at(i)).size();
	}
	for (i = 0; i < kNFmtUint2; ++i) {
		sum += f_uint_2(formats_uint_2.at(i)).size();
	}
	for (i = 0; i < kNFmtUint3; ++i) {
		sum += f_uint_3(formats_uint_3.at(i)).size();
	}
	for (i = 0; i < kNFmtUintStr; ++i) {
		sum += f_uint_str(formats_uint_str.at(i)).size();
	}
	for (i = 0; i < kNFmtUintStr2; ++i) {
		sum += f_uint_str_2(formats_uint_str_2.at(i)).size();
	}
	for (i = 0; i < kNFmtWild1; ++i) {
		sum += f_wild_1.at(i % kNFuncWild1)(formats_wild_1.at(i)).size();
	}
	for (i = 0; i < kNFmtWild2; ++i) {
		sum += f_wild_2.at(i % kNFuncWild2)(formats_wild_2.at(i)).size();
	}
	for (i = 0; i < kNFmtWild3; ++i) {
		sum += f_wild_3.at(i % kNFuncWild3)(formats_wild_3.at(i)).size();
	}
	for (i = 0; i < kNFmtWild4; ++i) {
		sum += f_wild_4.at(i % kNFuncWild4)(formats_wild_4.at(i)).size();
	}
	for (i = 0; i < kNFuncOther; ++i) {
		sum += other_format_functions.at(i)().size();
	}
	return sum;
}

