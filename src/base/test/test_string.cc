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

#include <boost/test/unit_test.hpp>

#include "base/macros.h"
#include "base/string.h"

// Triggered by BOOST_AUTO_TEST_CASE
CLANG_DIAG_OFF("-Wdisabled-macro-expansion")
CLANG_DIAG_OFF("-Wused-but-marked-unused")

BOOST_AUTO_TEST_SUITE(strings)

static const std::string str1 = "Hello World";
static const std::string str2 = "hello world";
static const std::string str3 = "helloworld";
static const std::string str4 = "Hello";
static const std::string str5 = "World";
static const std::string str6 = "xyz";

BOOST_AUTO_TEST_CASE(chars) {
	BOOST_CHECK_EQUAL(as_string(70), "70");
	BOOST_CHECK_EQUAL(as_string("xyz"), str6);
	/* Boost does not allow comparing the strings directly in this one case for some reason… */
	BOOST_CHECK_EQUAL(strcmp(as_string('w').c_str(), "w"), 0);

	BOOST_CHECK_EQUAL(to_lower(str2), str2);
	BOOST_CHECK_EQUAL(to_lower(str1), str2);
	BOOST_CHECK_EQUAL(to_lower(str3) == str2, false);
}

BOOST_AUTO_TEST_CASE(equality) {
	BOOST_CHECK_EQUAL(str1 == str2, false);
	BOOST_CHECK_EQUAL(iequals(str1, str2), true);
	BOOST_CHECK_EQUAL(str1 == str3, false);
	BOOST_CHECK_EQUAL(iequals(str1, str3), false);
}

BOOST_AUTO_TEST_CASE(contain_start_end) {
	BOOST_CHECK_EQUAL(contains(str1, str4, true), true);
	BOOST_CHECK_EQUAL(contains(str2, str4, true), false);
	BOOST_CHECK_EQUAL(contains(str1, str4, false), true);
	BOOST_CHECK_EQUAL(contains(str2, str4, false), true);
	BOOST_CHECK_EQUAL(contains(str1, str6, true), false);
	BOOST_CHECK_EQUAL(contains(str1, str6, false), false);
	BOOST_CHECK_EQUAL(contains(str6, str1, true), false);
	BOOST_CHECK_EQUAL(contains(str6, str1, false), false);

	BOOST_CHECK_EQUAL(starts_with(str1, str4, true), true);
	BOOST_CHECK_EQUAL(starts_with(str1, str4, false), true);
	BOOST_CHECK_EQUAL(starts_with(str2, str4, true), false);
	BOOST_CHECK_EQUAL(starts_with(str2, str4, false), true);
	BOOST_CHECK_EQUAL(starts_with(str6, str1, true), false);
	BOOST_CHECK_EQUAL(starts_with(str6, str1, false), false);

	BOOST_CHECK_EQUAL(ends_with(str1, str5, true), true);
	BOOST_CHECK_EQUAL(ends_with(str1, str5, false), true);
	BOOST_CHECK_EQUAL(ends_with(str2, str5, true), false);
	BOOST_CHECK_EQUAL(ends_with(str2, str5, false), true);
	BOOST_CHECK_EQUAL(ends_with(str6, str1, true), false);
	BOOST_CHECK_EQUAL(ends_with(str6, str1, false), false);
}

BOOST_AUTO_TEST_CASE(join_strings) {
	BOOST_CHECK_EQUAL(join(std::vector<std::string>{"foo", "bar", "baz"}, " "), "foo bar baz");
	BOOST_CHECK_EQUAL(join(std::set<std::string>{"foo", "bar", "baz"}, "HelloWorld"),
	                  "barHelloWorldbazHelloWorldfoo");
}

BOOST_AUTO_TEST_CASE(trim_split_replace) {
	const std::string prefix = "          ";
	const std::string middle = "foo bar baz";
	const std::string suffix = "    ";

	std::string str = prefix + middle + suffix;
	trim(str, false, false);
	BOOST_CHECK_EQUAL(str, prefix + middle + suffix);
	trim(str, true, false);
	BOOST_CHECK_EQUAL(str, middle + suffix);
	trim(str, false, true);
	BOOST_CHECK_EQUAL(str, middle);
	trim(str);
	BOOST_CHECK_EQUAL(str, middle);

	std::vector<std::string> v;
	split(v, str, {' '});
	BOOST_CHECK_EQUAL(v.size(), 3);
	BOOST_CHECK_EQUAL(v[0], "foo");
	BOOST_CHECK_EQUAL(v[1], "bar");
	BOOST_CHECK_EQUAL(v[2], "baz");
	BOOST_CHECK_EQUAL(str, middle);

	split(v, str, {'a'});
	BOOST_CHECK_EQUAL(v.size(), 3);
	BOOST_CHECK_EQUAL(v[0], "foo b");
	BOOST_CHECK_EQUAL(v[1], "r b");
	BOOST_CHECK_EQUAL(v[2], "z");
	BOOST_CHECK_EQUAL(str, middle);

	replace_first(str, "bar", "word");
	BOOST_CHECK_EQUAL(str, "foo word baz");
	replace_last(str, "word", "bar");
	BOOST_CHECK_EQUAL(str, middle);
	replace_all(str, " ba", "/word");
	BOOST_CHECK_EQUAL(str, "foo/wordr/wordz");
}

BOOST_AUTO_TEST_CASE(string_formatting) {
	BOOST_CHECK_EQUAL("Hello World", bformat("%s", "Hello World"));
	BOOST_CHECK_EQUAL("Hello World", bformat("%s %s", "Hello", "World"));
	BOOST_CHECK_EQUAL("Hello World", bformat("%1$s %2%", "Hello", "World"));
	BOOST_CHECK_EQUAL("Hello World", bformat("%2% %1%", "World", "Hello"));
	BOOST_CHECK_EQUAL("   Hello World", bformat("%1$14s", "Hello World"));
	BOOST_CHECK_EQUAL("Hello World   ", bformat("%-14s", "Hello World"));
	BOOST_CHECK_EQUAL("         Hello", bformat("%14.5s", "Hello World"));
	BOOST_CHECK_EQUAL("Hello         ", bformat("%1$-14.5s", "Hello World"));

	BOOST_CHECK_EQUAL("A123X", bformat("A%dX", 123));
	BOOST_CHECK_EQUAL("AABCDEFX", bformat("A%XX", 0xABCDEF));
	BOOST_CHECK_EQUAL("A-1X", bformat("A%dX", -1));
	BOOST_CHECK_EQUAL("A     123X", bformat("A%8dX", 123));
	BOOST_CHECK_EQUAL("A00000123X", bformat("A%08dX", 123));
	BOOST_CHECK_EQUAL("A0123X", bformat("A%d%u%d%uX", 0, 1, 2, 3));

	BOOST_CHECK_EQUAL("Aw77X", bformat("A%2$c%1$iX", 77, 'w'));
	BOOST_CHECK_EQUAL("AfalsetrueX", bformat("A%b%bX", 0, 1));

	BOOST_CHECK_EQUAL("AnullptrX", bformat("A%PX", nullptr));
	BOOST_CHECK_EQUAL("A123abcX", bformat("A%pX", reinterpret_cast<int*>(0x123abc)));

	BOOST_CHECK_EQUAL("A123.456X", bformat("A%.3fX", 123.456));
	BOOST_CHECK_EQUAL("A-0.45600000X", bformat("A%2.8fX", -0.456));
	BOOST_CHECK_EQUAL("A      12.3X", bformat("A%10.1fX", 12.34567));

	format_impl::ArgsPair p1, p2;
	p1.first = p2.first = format_impl::AbstractNode::ArgType::kString;
	p1.second.string_val = "World";
	p2.second.string_val = "Hello";
	BOOST_CHECK_EQUAL("Hello World", bformat("%2% %1%", format_impl::ArgsVector{p1, p2}));
	BOOST_CHECK_EQUAL("World Hello", bformat("%2% %1%", format_impl::ArgsVector{p2, p1}));
}

BOOST_AUTO_TEST_SUITE_END()
