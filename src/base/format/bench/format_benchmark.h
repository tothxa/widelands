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

#ifndef WL_BASE_FORMAT_BENCHMARK
#define WL_BASE_FORMAT_BENCHMARK

#include <string>

#include "base/random.h"

#include "base/format/bench/const_strings.h"
#include "base/format/bench/formats/format_int_1.h"
#include "base/format/bench/formats/format_int_2.h"
#include "base/format/bench/formats/format_int_str.h"
#include "base/format/bench/formats/format_other.h"
#include "base/format/bench/formats/format_str_1.h"
#include "base/format/bench/formats/format_str_2.h"
#include "base/format/bench/formats/format_str_3.h"
#include "base/format/bench/formats/format_uint_1.h"
#include "base/format/bench/formats/format_uint_2.h"
#include "base/format/bench/formats/format_uint_str.h"
#include "base/format/bench/formats/format_wild_1.h"
#include "base/format/bench/formats/format_wild_2.h"

static constexpr uint kRounds = 100000;

extern RNG rng;

static constexpr uint kRndIntMax = 2047;
static constexpr uint kRndFloatMax = 99999;
static constexpr uint kRndFloatMaxDiv = 9999;

inline uint rnd_uint(uint max = kRndIntMax) {
	return rng.rand() % max;
}

inline int rnd_int(uint max = kRndIntMax) {
	return static_cast<int>(rng.rand() % (2 * max - 1)) - max;
}

inline char rnd_char() {
	return rnd_uint(95) + 32;  // 7 bit ASCII non-control
}

inline double rnd_float(uint max = kRndFloatMax, uint div = kRndFloatMaxDiv) {
	return (static_cast<double>(rng.rand() % (2 * max)) - max) / (rnd_uint(div) + 1);
}

inline uint32_t rnd_ptr() {
	return rng.rand();
}

inline std::string rnd_str() {
	return const_strings.at(rnd_uint(kNStr));
}

inline std::string rnd_fmt(std::vector<std::string>& formats) {
	return formats.at(rnd_uint(formats.size()));
}

std::string format_random();
void fill_cache();
uint seq_all();

#endif
