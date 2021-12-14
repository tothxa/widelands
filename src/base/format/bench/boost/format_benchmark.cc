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

#include "base/format/bench/boost/format_benchmark.h"

#include <boost/format.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "base/format/bench/boost/format_functions.h"
#include "base/format/bench/boost/func_format_other.h"
#include "base/format/bench/timer.h"
#include "base/format/bench/mem_usage.h"

int main(int, char**) {
	Timer t_total;
	Timer t_unit;

	uint i;
	uint sum = 0;
	uint32_t check_random;

	boost::format time_report_fmt(("done in: %7d " + timer_unit + "\n").insert(0, 50, ' '));
	boost::format state_report_fmt("%10d characters written. Random state: %08x\n");

	memory_report();
	t_total.start();

	std::cout << "Using formats randomly " << kRounds << " times...\n";
	sum = 0;
	rng.seed(0xD0A5C7B3);
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += format_random().size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (time_report_fmt % t_unit.elapsed()).str();
	std::cout << (state_report_fmt % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using each format sequentially %u times...\n") % (kRounds / 1000)).str();
	sum = 0;
	t_unit.start();
	for (i = kRounds / 1000; i > 0; --i) {
		sum += seq_all();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%c' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_char(formats_char.at(rnd_uint(kNFmtChar))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%f' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_float(formats_float.at(rnd_uint(kNFmtFloat))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%i' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_int(formats_int.at(rnd_uint(kNFmtInt))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%p' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_ptr(formats_ptr.at(rnd_uint(kNFmtPtr))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%s' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_str(formats_str.at(rnd_uint(kNFmtStr))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%u' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_uint(formats_uint.at(rnd_uint(kNFmtUint))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	std::cout << (boost::format("Using '%%1%%' formats %u times...\n") % kRounds).str();
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_wild_1.at(rnd_uint(kNFuncWild1))(formats_wild_1.at(rnd_uint(kNFmtWild1))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << (boost::format(time_report_fmt) % t_unit.elapsed()).str();
	std::cout << (boost::format(state_report_fmt) % sum % check_random).str();
	memory_report();

	t_total.stop();
	std::cout << "Total:\n";
	std::cout << (boost::format(time_report_fmt) % t_total.elapsed()).str();

	return 0;
}

