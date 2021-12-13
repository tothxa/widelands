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

#ifndef WL_BASE_FORMAT_STRING_NODE_H
#define WL_BASE_FORMAT_STRING_NODE_H

#include <cstring>

#include "base/format/abstract_node.h"

namespace format_impl {

struct StringNode : FormatNode {
	StringNode(const uint8_t f, const size_t w, const int32_t p) : FormatNode(f, w, p) {
		if (flags_ & kNumberSign) {
			throw wexception("%%s can not have '+' specifier");
		}
		if (flags_ & kPadWith0) {
			throw wexception("%%s can not have '0' specifier");
		}
	}

	char* append(char* out, const ArgType t, const Argument arg_u, bool) const override;

	static const StringNode node_;
};

inline char* append_nullptr_node(char* out, const bool localize) {
	AbstractNode::Argument a;
	a.string_val = "nullptr";
	return StringNode::node_.append(out, AbstractNode::ArgType::kString, a, localize);
}

}  // namespace format_impl

#endif  // end of include guard: WL_BASE_FORMAT_STRING_NODE_H
