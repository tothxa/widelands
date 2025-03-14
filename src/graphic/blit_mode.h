/*
 * Copyright (C) 2006-2025 by the Widelands Development Team
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
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef WL_GRAPHIC_BLIT_MODE_H
#define WL_GRAPHIC_BLIT_MODE_H

// The type of blit performed.
enum class BlitMode {
	// Blit texture unchanged.
	kDirect,

	// Blit texture desaturated and maybe tinted with a color.
	kMonochrome,

	// Blit texture tinted with a color everywhere where a mask is not
	// transparent,
	kBlendedWithMask,
};

#endif  // end of include guard: WL_GRAPHIC_BLIT_MODE_H
