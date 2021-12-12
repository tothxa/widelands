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
#include "base/format/tree.h"

#include "base/format/bench/format_benchmark.h"
#include "base/format/bench/format_functions.h"
#include "base/format/bench/func_format_other.h"

void fill_cache() {
	uint i;
	for (i = 0; i < kNFmtStr; ++i) {
		format_impl::Tree::get(formats_str.at(i));
	}
	for (i = 0; i < kNFmtInt; ++i) {
		format_impl::Tree::get(formats_int.at(i));
	}
	for (i = 0; i < kNFmtUint; ++i) {
		format_impl::Tree::get(formats_uint.at(i));
	}
	for (i = 0; i < kNFmtStr2; ++i) {
		format_impl::Tree::get(formats_str_2.at(i));
	}
	for (i = 0; i < kNFmtInt2; ++i) {
		format_impl::Tree::get(formats_int_2.at(i));
	}
	for (i = 0; i < kNFmtChar; ++i) {
		format_impl::Tree::get(formats_char.at(i));
	}
	for (i = 0; i < kNFmtFloat; ++i) {
		format_impl::Tree::get(formats_float.at(i));
	}
	for (i = 0; i < kNFmtFloatUint; ++i) {
		format_impl::Tree::get(formats_float_uint.at(i));
	}
	for (i = 0; i < kNFmtInt3; ++i) {
		format_impl::Tree::get(formats_int_3.at(i));
	}
	for (i = 0; i < kNFmtInt4; ++i) {
		format_impl::Tree::get(formats_int_4.at(i));
	}
	for (i = 0; i < kNFmtIntStr; ++i) {
		format_impl::Tree::get(formats_int_str.at(i));
	}
	for (i = 0; i < kNFmtPercent; ++i) {
		format_impl::Tree::get(formats_percent.at(i));
	}
	for (i = 0; i < kNFmtPtr; ++i) {
		format_impl::Tree::get(formats_ptr.at(i));
	}
	for (i = 0; i < kNFmtStr3; ++i) {
		format_impl::Tree::get(formats_str_3.at(i));
	}
	for (i = 0; i < kNFmtStr4; ++i) {
		format_impl::Tree::get(formats_str_4.at(i));
	}
	for (i = 0; i < kNFmtStrFloatInt2; ++i) {
		format_impl::Tree::get(formats_str_float_int_2.at(i));
	}
	for (i = 0; i < kNFmtStrInt; ++i) {
		format_impl::Tree::get(formats_str_int.at(i));
	}
	for (i = 0; i < kNFmtStrInt2; ++i) {
		format_impl::Tree::get(formats_str_int_2.at(i));
	}
	for (i = 0; i < kNFmtStrInt3; ++i) {
		format_impl::Tree::get(formats_str_int_3.at(i));
	}
	for (i = 0; i < kNFmtStrUint; ++i) {
		format_impl::Tree::get(formats_str_uint.at(i));
	}
	for (i = 0; i < kNFmtStrUint2; ++i) {
		format_impl::Tree::get(formats_str_uint_2.at(i));
	}
	for (i = 0; i < kNFmtStrUintInt; ++i) {
		format_impl::Tree::get(formats_str_uint_int.at(i));
	}
	for (i = 0; i < kNFmtStrUintStr; ++i) {
		format_impl::Tree::get(formats_str_uint_str.at(i));
	}
	for (i = 0; i < kNFmtUint2; ++i) {
		format_impl::Tree::get(formats_uint_2.at(i));
	}
	for (i = 0; i < kNFmtUint3; ++i) {
		format_impl::Tree::get(formats_uint_3.at(i));
	}
	for (i = 0; i < kNFmtUintStr; ++i) {
		format_impl::Tree::get(formats_uint_str.at(i));
	}
	for (i = 0; i < kNFmtUintStr2; ++i) {
		format_impl::Tree::get(formats_uint_str_2.at(i));
	}
	for (i = 0; i < kNFmtWild1; ++i) {
		format_impl::Tree::get(formats_wild_1.at(i));
	}
	for (i = 0; i < kNFmtWild2; ++i) {
		format_impl::Tree::get(formats_wild_2.at(i));
	}
	for (i = 0; i < kNFmtWild3; ++i) {
		format_impl::Tree::get(formats_wild_3.at(i));
	}
	for (i = 0; i < kNFmtWild4; ++i) {
		format_impl::Tree::get(formats_wild_4.at(i));
	}
	for (i = 0; i < kNFuncOther; ++i) {
		format_impl::Tree::get(other_formats.at(i));
	}
}

