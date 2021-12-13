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

inline std::string f_char(std::string format) {
	return bformat(format, rnd_char());
}

inline std::string f_float(std::string format) {
	return bformat(format, rnd_float());
}

// only for wildcards
inline std::string f_float_2(std::string format) {
	return bformat(format, rnd_float(), rnd_float());
}

inline std::string f_float_uint(std::string format) {
	return bformat(format, rnd_float(), rnd_uint());
}

inline std::string f_int(std::string format) {
	return bformat(format, rnd_int());
}

inline std::string f_int_2(std::string format) {
	return bformat(format, rnd_int(), rnd_int());
}

inline std::string f_int_3(std::string format) {
	return bformat(format, rnd_int(), rnd_int(), rnd_int());
}

inline std::string f_int_4(std::string format) {
	return bformat(format, rnd_int(), rnd_int(), rnd_int(), rnd_int());
}

inline std::string f_int_str(std::string format) {
	return bformat(format, rnd_int(), rnd_str());
}

/*
inline std::string f_percent(std::string format) {
	return bformat(format);
}
*/

inline std::string f_ptr(std::string format) {
	return bformat(format, rnd_ptr());
}

inline std::string f_str(std::string format) {
	return bformat(format, rnd_str());
}

inline std::string f_str_2(std::string format) {
	return bformat(format, rnd_str(), rnd_str());
}

inline std::string f_str_3(std::string format) {
	return bformat(format, rnd_str(), rnd_str(), rnd_str());
}

inline std::string f_str_4(std::string format) {
	return bformat(format, rnd_str(), rnd_str(), rnd_str(), rnd_str());
}

inline std::string f_str_float_int_2(std::string format) {
	return bformat(format, rnd_str(), rnd_float(), rnd_int(), rnd_int());
}

inline std::string f_str_int(std::string format) {
	return bformat(format, rnd_str(), rnd_int());
}

inline std::string f_str_int_2(std::string format) {
	return bformat(format, rnd_str(), rnd_int(), rnd_int());
}

inline std::string f_str_int_3(std::string format) {
	return bformat(format, rnd_str(), rnd_int(), rnd_int(), rnd_int());
}

inline std::string f_str_uint(std::string format) {
	return bformat(format, rnd_str(), rnd_uint());
}

inline std::string f_str_uint_2(std::string format) {
	return bformat(format, rnd_str(), rnd_uint(), rnd_uint());
}

inline std::string f_str_uint_int(std::string format) {
	return bformat(format, rnd_str(), rnd_uint(), rnd_int());
}

inline std::string f_str_uint_str(std::string format) {
	return bformat(format, rnd_str(), rnd_uint(), rnd_str());
}

inline std::string f_uint(std::string format) {
	return bformat(format, rnd_uint());
}

inline std::string f_uint_2(std::string format) {
	return bformat(format, rnd_uint(), rnd_uint());
}

inline std::string f_uint_3(std::string format) {
	return bformat(format, rnd_uint(), rnd_uint(), rnd_uint());
}

inline std::string f_uint_str(std::string format) {
	return bformat(format, rnd_uint(), rnd_str());
}

inline std::string f_uint_str_2(std::string format) {
	return bformat(format, rnd_uint(), rnd_str(), rnd_str());
}

/***** for wildcards *****/
static const std::vector<std::function<std::string(std::string)>> f_wild_1 = {
	f_char, f_float, f_int, f_ptr, f_str, f_uint
};
static const uint kNFuncWild1 = f_wild_1.size();

static const std::vector<std::function<std::string(std::string)>> f_wild_2 = {
	f_float_2, f_float_uint, f_int_2, f_int_str, f_str_2, f_str_int, f_str_uint,
	f_uint_2, f_uint_str
};
static const uint kNFuncWild2 = f_wild_2.size();

static const std::vector<std::function<std::string(std::string)>> f_wild_3 = {
	f_int_3, f_str_3, f_str_uint_int, f_str_uint_str, f_uint_3, f_uint_str_2
};
static const uint kNFuncWild3 = f_wild_3.size();

static const std::vector<std::function<std::string(std::string)>> f_wild_4 = {
	f_int_4, f_str_4, f_str_int_3, f_str_float_int_2
};
static const uint kNFuncWild4 = f_wild_4.size();

