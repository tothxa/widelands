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

#ifndef WL_WLAPPLICATION_MOUSEWHEEL_OPTIONS_H
#define WL_WLAPPLICATION_MOUSEWHEEL_OPTIONS_H

#include <SDL_keyboard.h>

#include "base/vector.h"

/*
 * Mousewheel options
 */

// Config file options
enum class MousewheelOptionID : uint16_t {
	k__Begin = 0,

	kUIChangeValueInvertX = k__Begin,
	kUIChangeValueInvertY,
	kUITabInvertX,
	kUITabInvertY,
	kMapZoomMod,
	kMapZoomX,
	kMapZoomY,
	kMapZoomInvertX,
	kMapZoomInvertY,
	kMapScrollMod,
	kMapScroll,
	kMapScrollInvert,
	kGameSpeedMod,
	kGameSpeedX,
	kGameSpeedY,
	kEditorToolsizeMod,
	kEditorToolsizeX,
	kEditorToolsizeY,

	k__End = kEditorToolsizeY,

	kEnabled,
	kDisabled,
	kNoMod,

	kUse2Ddefaults
};

void set_mousewheel_option_bool(const MousewheelOptionID, const bool);
bool get_mousewheel_option_bool(const MousewheelOptionID);

void set_mousewheel_keymod(const MousewheelOptionID, const uint16_t);
uint16_t get_mousewheel_keymod(const MousewheelOptionID);

// Map config options to handlers
enum class MousewheelHandlerConfigID : uint16_t {
	k__Begin = 0,

	kChangeValue = k__Begin,
	kTabBar,
	kZoom,
	kMapScroll,
	kGameSpeed,
	kEditorToolsize,

	k__End = kEditorToolsize
};

// Read settings from config file or write all default values to config file
void init_mousewheel_settings(bool force_defaults = false);

// Apply settings to handlers
void update_mousewheel_settings();

// Primary handler function
int32_t get_mousewheel_change(const MousewheelHandlerConfigID handler_id,
                              const int32_t x,
                              const int32_t y,
                              const uint16_t modstate);

// Handler function for 2D scrolling
Vector2i get_mousewheel_change_2D(const MousewheelHandlerConfigID handler_id,
                                  const int32_t x,
                                  const int32_t y,
                                  const uint16_t modstate);

#endif  // end of include guard: WL_WLAPPLICATION_MOUSEWHEEL_OPTIONS_H
