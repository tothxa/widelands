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

#ifndef WL_BASE_FORMAT_FLOAT_NODE_H
#define WL_BASE_FORMAT_FLOAT_NODE_H

#include <cstring>

#include "base/format/abstract_node.h"

namespace format_impl {

struct FloatNode : FormatNode {
	FloatNode(const uint8_t f, const size_t w, const int32_t p) : FormatNode(f, w, p) {
	}

	char* append(char* out, const ArgType t, Argument arg_u, const bool localize) const override;

	static const FloatNode node_;
};

}  // namespace format_impl

#endif  // end of include guard: WL_BASE_FORMAT_FLOAT_NODE_H
