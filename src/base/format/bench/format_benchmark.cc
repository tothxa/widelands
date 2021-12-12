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

#include "base/format/bench/format_benchmark.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "base/string.h"

#include "base/format/bench/format_functions.h"
#include "base/format/bench/func_format_other.h"
#include "base/format/bench/timer.h"
#include "base/format/bench/mem_usage.h"

inline void print_usage() {
	std::cout << "Use maximum one of [random|seq|init] as argument. Default is 'random'.\n";
}

int main(int argc, char** argv) {
	if (argc > 2) {
		print_usage();
		return 1;
	}

	Timer t_total;
	Timer t_unit;

	uint i;
	uint sum = 0;
	uint32_t check_random;

	std::string time_report_fmt = ("done in: %7d " + timer_unit + "\n").insert(0, 50, ' ');
	std::string state_report_fmt = "%10d characters written. Random state: %08x\n";

	memory_report();
	t_total.start();

	bool arg_ok = false;
	if (argc == 2) {
		if (strcmp(argv[1], "init") == 0) {
			std::cout << "Initialising cache...\n";
			arg_ok = true;
			t_unit.start();
			fill_cache();
			t_unit.stop();
			std::cout << bformat(time_report_fmt, t_unit.elapsed());
			memory_report();
		}
		if (arg_ok || strcmp(argv[1], "seq") == 0) {
			arg_ok = true;
			std::cout << "Using each format once sequentially...\n";
			rng.seed(0x12345678);
			t_unit.start();
			sum = seq_all();
			t_unit.stop();
			check_random = rng.rand();
			std::cout << bformat(time_report_fmt, t_unit.elapsed());
			std::cout << bformat(state_report_fmt, sum, check_random);
			memory_report();
			rng.reset();
		} else if (strcmp(argv[1], "random") != 0) {
			print_usage();
			return 1;
		}
	}

	// We do this first to allow filling the cache in pseudo-random order
	// when no argument or 'random' is given.
	std::cout << "Using formats randomly " << kRounds << " times...\n";
	sum = 0;
	rng.seed(0xD0A5C7B3);
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += format_random().size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using each format sequentially %u times...\n", kRounds / 1000);
	sum = 0;
	t_unit.start();
	for (i = kRounds / 1000; i > 0; --i) {
		sum += seq_all();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%c' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_char(formats_char.at(rnd_uint(kNFmtChar))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%f' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_float(formats_float.at(rnd_uint(kNFmtFloat))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%i' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_int(formats_int.at(rnd_uint(kNFmtInt))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%p' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_ptr(formats_ptr.at(rnd_uint(kNFmtPtr))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%s' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_str(formats_str.at(rnd_uint(kNFmtStr))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%u' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_uint(formats_uint.at(rnd_uint(kNFmtUint))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	std::cout << bformat("Using '%%1%%' formats %u times...\n", kRounds);
	sum = 0;
	t_unit.start();
	for (i = 0; i < kRounds; ++i) {
		sum += f_wild_1.at(rnd_uint(kNFuncWild1))(formats_wild_1.at(rnd_uint(kNFmtWild1))).size();
	}
	t_unit.stop();
	check_random = rng.rand();
	std::cout << bformat(time_report_fmt, t_unit.elapsed());
	std::cout << bformat(state_report_fmt, sum, check_random);
	memory_report();

	t_total.stop();
	std::cout << "Total:\n";
	std::cout << bformat(time_report_fmt, t_total.elapsed());

	return 0;
}

