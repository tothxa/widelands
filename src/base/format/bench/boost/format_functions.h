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

inline std::string f_char(boost::format format) {
	return (format % rnd_char()).str();
}

inline std::string f_float(boost::format format) {
	return (format % rnd_float()).str();
}

// only for wildcards
inline std::string f_float_2(boost::format format) {
	return (format % rnd_float() % rnd_float()).str();
}

inline std::string f_float_uint(boost::format format) {
	return (format % rnd_float() % rnd_uint()).str();
}

inline std::string f_int(boost::format format) {
	return (format % rnd_int()).str();
}

inline std::string f_int_2(boost::format format) {
	return (format % rnd_int() % rnd_int()).str();
}

inline std::string f_int_3(boost::format format) {
	return (format % rnd_int() % rnd_int() % rnd_int()).str();
}

inline std::string f_int_4(boost::format format) {
	return (format % rnd_int() % rnd_int() % rnd_int() % rnd_int()).str();
}

inline std::string f_int_str(boost::format format) {
	return (format % rnd_int() % rnd_str()).str();
}

/*
inline std::string f_percent(boost::format format) {
	return (format).str();
}
*/

inline std::string f_ptr(boost::format format) {
	return (format % rnd_ptr()).str();
}

inline std::string f_str(boost::format format) {
	return (format % rnd_str()).str();
}

inline std::string f_str_2(boost::format format) {
	return (format % rnd_str() % rnd_str()).str();
}

inline std::string f_str_3(boost::format format) {
	return (format % rnd_str() % rnd_str() % rnd_str()).str();
}

inline std::string f_str_4(boost::format format) {
	return (format % rnd_str() % rnd_str() % rnd_str() % rnd_str()).str();
}

inline std::string f_str_float_int_2(boost::format format) {
	return (format % rnd_str() % rnd_float() % rnd_int() % rnd_int()).str();
}

inline std::string f_str_int(boost::format format) {
	return (format % rnd_str() % rnd_int()).str();
}

inline std::string f_str_int_2(boost::format format) {
	return (format % rnd_str() % rnd_int() % rnd_int()).str();
}

inline std::string f_str_int_3(boost::format format) {
	return (format % rnd_str() % rnd_int() % rnd_int() % rnd_int()).str();
}

inline std::string f_str_uint(boost::format format) {
	return (format % rnd_str() % rnd_uint()).str();
}

inline std::string f_str_uint_2(boost::format format) {
	return (format % rnd_str() % rnd_uint() % rnd_uint()).str();
}

inline std::string f_str_uint_int(boost::format format) {
	return (format % rnd_str() % rnd_uint() % rnd_int()).str();
}

inline std::string f_str_uint_str(boost::format format) {
	return (format % rnd_str() % rnd_uint() % rnd_str()).str();
}

inline std::string f_uint(boost::format format) {
	return (format % rnd_uint()).str();
}

inline std::string f_uint_2(boost::format format) {
	return (format % rnd_uint() % rnd_uint()).str();
}

inline std::string f_uint_3(boost::format format) {
	return (format % rnd_uint() % rnd_uint() % rnd_uint()).str();
}

inline std::string f_uint_str(boost::format format) {
	return (format % rnd_uint() % rnd_str()).str();
}

inline std::string f_uint_str_2(boost::format format) {
	return (format % rnd_uint() % rnd_str() % rnd_str()).str();
}

/***** for wildcards *****/
static const std::vector<std::function<std::string(boost::format)>> f_wild_1 = {
	f_char, f_float, f_int, f_ptr, f_str, f_uint
};
static const uint kNFuncWild1 = f_wild_1.size();

static const std::vector<std::function<std::string(boost::format)>> f_wild_2 = {
	f_float_2, f_float_uint, f_int_2, f_int_str, f_str_2, f_str_int, f_str_uint,
	f_uint_2, f_uint_str
};
static const uint kNFuncWild2 = f_wild_2.size();

static const std::vector<std::function<std::string(boost::format)>> f_wild_3 = {
	f_int_3, f_str_3, f_str_uint_int, f_str_uint_str, f_uint_3, f_uint_str_2
};
static const uint kNFuncWild3 = f_wild_3.size();

static const std::vector<std::function<std::string(boost::format)>> f_wild_4 = {
	f_int_4, f_str_4, f_str_int_3, f_str_float_int_2
};
static const uint kNFuncWild4 = f_wild_4.size();

