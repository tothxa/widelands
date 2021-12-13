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

#include <cstring>

#include "base/format/tree.h"
#include "base/format/string_node.h"

namespace format_impl {

char* StringNode::append(char* out, const ArgType t, const Argument arg_u, bool) const {
	if (t != ArgType::kString) {
		throw wexception("Wrong argument type: expected string, found %s", to_string(t).c_str());
	}
	const char* arg = arg_u.string_val;

	if (min_width_ == 0 || flags_ & kLeftAlign) {
		// The easy case: Just start writing.
		size_t written = 0;
		for (const char* a = arg; *a && written < precision_; ++a, ++out, ++written) {
			*out = *a;
		}
		if (written < min_width_) {
			written = min_width_ - written;
			for (; written; ++out, --written) {
				*out = ' ';
			}
		}
		return out;
	}

	// The more complex case: We want a right-aligned string with a given minimum width.
	const size_t arg_len = std::min<size_t>(precision_, strlen(arg));
	if (arg_len < min_width_) {
		for (size_t i = min_width_ - arg_len; i; ++out, --i) {
			*out = ' ';
		}
	}
	for (size_t l = arg_len; l; --l, ++out, ++arg) {
		*out = *arg;
	}
	return out;
}

}  // namespace format_impl
