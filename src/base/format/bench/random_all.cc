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

static const uint kNFormats =
   kNFmtStr + kNFmtInt + kNFmtUint + kNFmtStr2 + kNFmtInt2 + kNFmtChar + kNFmtFloat +
   kNFmtFloatUint + kNFmtInt3 + kNFmtInt4 + kNFmtIntStr /* + kNFmtPercent */ + kNFmtPtr + kNFmtStr3 +
   kNFmtStr4 + kNFmtStrFloatInt2 + kNFmtStrInt + kNFmtStrInt2 + kNFmtStrInt3 + kNFmtStrUint +
   kNFmtStrUint2 + kNFmtStrUintInt + kNFmtStrUintStr + kNFmtUint2 + kNFmtUint3 + kNFmtUintStr +
   kNFmtUintStr2 + kNFmtWild1 + kNFmtWild2 + kNFmtWild3 + kNFmtWild4 + kNFuncOther;

std::string format_random() {
	uint i = rnd_uint(kNFormats);
	if (i < kNFmtStr) {
		return f_str(formats_str.at(i));
	}
	i -= kNFmtStr;
	if (i < kNFmtInt) {
		return f_int(formats_int.at(i));
	}
	i -= kNFmtInt;
	if (i < kNFmtUint) {
		return f_uint(formats_uint.at(i));
	}
	i -= kNFmtUint;
	if (i < kNFmtStr2) {
		return f_str_2(formats_str_2.at(i));
	}
	i -= kNFmtStr2;
	if (i < kNFmtInt2) {
		return f_int_2(formats_int_2.at(i));
	}
	i -= kNFmtInt2;
	if (i < kNFmtChar) {
		return f_char(formats_char.at(i));
	}
	i -= kNFmtChar;
	if (i < kNFmtFloat) {
		return f_float(formats_float.at(i));
	}
	i -= kNFmtFloat;
	if (i < kNFmtFloatUint) {
		return f_float_uint(formats_float_uint.at(i));
	}
	i -= kNFmtFloatUint;
	if (i < kNFmtInt3) {
		return f_int_3(formats_int_3.at(i));
	}
	i -= kNFmtInt3;
	if (i < kNFmtInt4) {
		return f_int_4(formats_int_4.at(i));
	}
	i -= kNFmtInt4;
	if (i < kNFmtIntStr) {
		return f_int_str(formats_int_str.at(i));
	}
	i -= kNFmtIntStr;
/*
	if (i < kNFmtPercent) {
		return f_percent(formats_percent.at(i));
	}
	i -= kNFmtPercent;
*/
	if (i < kNFmtPtr) {
		return f_ptr(formats_ptr.at(i));
	}
	i -= kNFmtPtr;
	if (i < kNFmtStr3) {
		return f_str_3(formats_str_3.at(i));
	}
	i -= kNFmtStr3;
	if (i < kNFmtStr4) {
		return f_str_4(formats_str_4.at(i));
	}
	i -= kNFmtStr4;
	if (i < kNFmtStrFloatInt2) {
		return f_str_float_int_2(formats_str_float_int_2.at(i));
	}
	i -= kNFmtStrFloatInt2;
	if (i < kNFmtStrInt) {
		return f_str_int(formats_str_int.at(i));
	}
	i -= kNFmtStrInt;
	if (i < kNFmtStrInt2) {
		return f_str_int_2(formats_str_int_2.at(i));
	}
	i -= kNFmtStrInt2;
	if (i < kNFmtStrInt3) {
		return f_str_int_3(formats_str_int_3.at(i));
	}
	i -= kNFmtStrInt3;
	if (i < kNFmtStrUint) {
		return f_str_uint(formats_str_uint.at(i));
	}
	i -= kNFmtStrUint;
	if (i < kNFmtStrUint2) {
		return f_str_uint_2(formats_str_uint_2.at(i));
	}
	i -= kNFmtStrUint2;
	if (i < kNFmtStrUintInt) {
		return f_str_uint_int(formats_str_uint_int.at(i));
	}
	i -= kNFmtStrUintInt;
	if (i < kNFmtStrUintStr) {
		return f_str_uint_str(formats_str_uint_str.at(i));
	}
	i -= kNFmtStrUintStr;
	if (i < kNFmtUint2) {
		return f_uint_2(formats_uint_2.at(i));
	}
	i -= kNFmtUint2;
	if (i < kNFmtUint3) {
		return f_uint_3(formats_uint_3.at(i));
	}
	i -= kNFmtUint3;
	if (i < kNFmtUintStr) {
		return f_uint_str(formats_uint_str.at(i));
	}
	i -= kNFmtUintStr;
	if (i < kNFmtUintStr2) {
		return f_uint_str_2(formats_uint_str_2.at(i));
	}
	i -= kNFmtUintStr2;
	if (i < kNFmtWild1) {
		return f_wild_1.at(rnd_uint(kNFuncWild1))(formats_wild_1.at(i));
	}
	i -= kNFmtWild1;
	if (i < kNFmtWild2) {
		return f_wild_2.at(rnd_uint(kNFuncWild2))(formats_wild_2.at(i));
	}
	i -= kNFmtWild2;
	if (i < kNFmtWild3) {
		return f_wild_3.at(rnd_uint(kNFuncWild3))(formats_wild_3.at(i));
	}
	i -= kNFmtWild3;
	if (i < kNFmtWild4) {
		return f_wild_4.at(rnd_uint(kNFuncWild4))(formats_wild_4.at(i));
	}
	i -= kNFmtWild4;
	return other_format_functions.at(i)();
}

