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

#include "wlapplication_mousewheel_options.h"

#include <cassert>
#include <cstdlib>
#include <map>

#include "wlapplication_options.h"

enum class MousewheelOptionType { kBool, kKeymod };

struct MousewheelOption {
	const std::string internal_name;
	const MousewheelOptionType type_;
	const uint16_t default_;
	const uint16_t default_2D;
	uint16_t current;

	MousewheelOption(std::string n, MousewheelOptionType t, uint16_t def, uint16_t def2D)
	   : internal_name(n), type_(t), default_(def), default_2D(def2D), current(def) {
	}

	void get_config() {
		if (!(internal_name.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				current = static_cast<uint16_t>(
				   get_config_bool("mousewheel", internal_name, static_cast<bool>(default_)));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				current = get_config_int("mousewheel", internal_name, default_);
			}
		}
	}
	void set_bool(bool value) {
		assert(type_ == MousewheelOptionType::kBool);
		if (!(internal_name.empty())) {
			set_config_bool("mousewheel", internal_name, value);
			get_config();
		}
	}
	void set_keymod(uint16_t value) {
		assert(type_ == MousewheelOptionType::kKeymod);
		if (!(internal_name.empty())) {
			set_config_int("mousewheel", internal_name, value);
			get_config();
		}
	}
	void reset() {
		if (!(internal_name.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				set_bool(static_cast<bool>(default_));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				set_keymod(default_);
			}
		}
	}
	void set2D() {
		if (!(internal_name.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				set_bool(static_cast<bool>(default_2D));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				set_keymod(default_2D);
			}
		}
	}
};

struct MousewheelSettings {
	const MousewheelOptionID keymod_id;
	uint16_t current_keymod = KMOD_NONE;
	const MousewheelOptionID use_x;
	const MousewheelOptionID invert_x;
	const int32_t default_sign_x;
	int32_t current_sign_x = 0;
	const MousewheelOptionID use_y;
	const MousewheelOptionID invert_y;
	const int32_t default_sign_y;
	int32_t current_sign_y = 0;

	void update_settings() {
		current_keymod = get_mousewheel_keymod(keymod_id);
		if (get_mousewheel_option_bool(use_x)) {
			current_sign_x = default_sign_x * (get_mousewheel_option_bool(invert_x) ? -1 : 1);
		} else {
			current_sign_x = 0;
		}
		if (get_mousewheel_option_bool(use_y)) {
			current_sign_y = default_sign_y * (get_mousewheel_option_bool(invert_y) ? -1 : 1);
		} else {
			current_sign_y = 0;
		}
	}

	MousewheelSettings(MousewheelOptionID _keymod_id,
	                   MousewheelOptionID _use_x,
	                   MousewheelOptionID _invert_x,
	                   MousewheelOptionID _use_y,
	                   MousewheelOptionID _invert_y,
	                   int32_t def_sign_x,
	                   int32_t def_sign_y)
	   : keymod_id(_keymod_id),
	     use_x(_use_x),
	     invert_x(_invert_x),
	     default_sign_x(def_sign_x),
	     use_y(_use_y),
	     invert_y(_invert_y),
	     default_sign_y(def_sign_y) {
	}

	bool can_handle(const int32_t x, const int32_t y, const uint16_t modstate) {
		return (((y && current_sign_y) || (x && current_sign_x)) &&
		        matches_keymod(current_keymod, modstate));
	}
	int32_t get_change(const int32_t x, const int32_t y, const uint16_t modstate) {
		if (can_handle(x, y, modstate)) {
			return (x * current_sign_x + y * current_sign_y);
		}
		return 0;
	}
	Vector2i get_change_2D(const int32_t x, const int32_t y, const uint16_t modstate) {
		if (can_handle(x, y, modstate)) {
			return Vector2i(x * current_sign_x, y * current_sign_y);
		}
		return Vector2i(0, 0);
	}
};

static std::map<MousewheelOptionID, MousewheelOption> mousewheel_options = {
   {MousewheelOptionID::kUIChangeValueMod,
    MousewheelOption("change_value_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},
   {MousewheelOptionID::kUIChangeValueX,
    MousewheelOption("change_value_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kUIChangeValueY,
    MousewheelOption("change_value_y", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kUIChangeValueInvertX,
    MousewheelOption("change_value_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kUIChangeValueInvertY,
    MousewheelOption("change_value_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kUITabMod,
    MousewheelOption("tabpanel_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},
   {MousewheelOptionID::kUITabX,
    MousewheelOption("tabpanel_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kUITabY,
    MousewheelOption("tabpanel_y", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kUITabInvertX,
    MousewheelOption("tabpanel_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kUITabInvertY,
    MousewheelOption("tabpanel_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kWUIUnifiedInputQueue,
    MousewheelOption("inputqueue_unified", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kWUIInputFillMod,
    MousewheelOption(
       "inputqueue_fill_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},
   {MousewheelOptionID::kWUIInputFillX,
    MousewheelOption("inputqueue_fill_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kWUIInputFillY,
    MousewheelOption("inputqueue_fill_y", MousewheelOptionType::kBool, true, false)},

   {MousewheelOptionID::kWUIInputPriorityMod,
    MousewheelOption(
       "inputqueue_priority_modifier", MousewheelOptionType::kKeymod, KMOD_ALT, KMOD_NONE)},
   {MousewheelOptionID::kWUIInputPriorityX,
    MousewheelOption("inputqueue_priority_x", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kWUIInputPriorityY,
    MousewheelOption("inputqueue_priority_y", MousewheelOptionType::kBool, true, true)},

   {MousewheelOptionID::kMapZoomMod,
    MousewheelOption("zoom_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_CTRL)},
   {MousewheelOptionID::kMapZoomX,
    MousewheelOption("zoom_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kMapZoomY,
    MousewheelOption("zoom_y", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kMapZoomInvertX,
    MousewheelOption("zoom_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kMapZoomInvertY,
    MousewheelOption("zoom_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kMapScrollMod,
    MousewheelOption("move_map_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},
   {MousewheelOptionID::kMapScroll,
    MousewheelOption("move_map", MousewheelOptionType::kBool, false, true)},
   {MousewheelOptionID::kMapScrollInvert,
    MousewheelOption("move_map_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kMapScrollHorizYMod,
    MousewheelOption(
       "move_map_horizontal_y_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},
   {MousewheelOptionID::kMapScrollHorizYInvert,
    MousewheelOption("move_map_horizontal_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kGameSpeedMod,
    MousewheelOption("gamespeed_modifier", MousewheelOptionType::kKeymod, KMOD_ALT, KMOD_ALT)},
   {MousewheelOptionID::kGameSpeedX,
    MousewheelOption("gamespeed_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kGameSpeedY,
    MousewheelOption("gamespeed_y", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kGameSpeedInvertX,
    MousewheelOption("gamespeed_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kGameSpeedInvertY,
    MousewheelOption("gamespeed_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kEditorToolsizeMod,
    MousewheelOption(
       "editor_toolsize_modifier", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_CTRL)},
   {MousewheelOptionID::kEditorToolsizeX,
    MousewheelOption("editor_toolsize_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kEditorToolsizeY,
    MousewheelOption("editor_toolsize_y", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kEditorToolsizeInvertX,
    MousewheelOption("editor_toolsize_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kEditorToolsizeInvertY,
    MousewheelOption("editor_toolsize_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kDisabled, MousewheelOption("", MousewheelOptionType::kBool, false, false)}

};

// Default signs
#define kSignIncreaseRight -1
#define kSignIncreaseUp 1
#define kSignNextRight -1
#define kSignNextDown -1
#define kSignScroll -1

#define DEFAULT_SIGN_VALUE kSignIncreaseRight, kSignIncreaseUp
#define DEFAULT_SIGN_MOVE kSignNextRight, kSignNextDown
#define DEFAULT_SIGN_SCROLL kSignScroll, kSignScroll

static std::map<MousewheelHandlerConfigID, MousewheelSettings> mousewheel_handlers = {
   {MousewheelHandlerConfigID::kChangeValue,
    MousewheelSettings(MousewheelOptionID::kUIChangeValueMod,
                       MousewheelOptionID::kUIChangeValueX,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kUIChangeValueY,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kTabBar,  //
    MousewheelSettings(MousewheelOptionID::kUITabMod,
                       MousewheelOptionID::kUITabX,
                       MousewheelOptionID::kUITabInvertX,
                       MousewheelOptionID::kUITabY,
                       MousewheelOptionID::kUITabInvertY,
                       DEFAULT_SIGN_MOVE)},
   {MousewheelHandlerConfigID::kWUIInputFill,
    MousewheelSettings(MousewheelOptionID::kWUIInputFillMod,
                       MousewheelOptionID::kWUIInputFillX,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kWUIInputFillY,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kWUIInputPriority,
    MousewheelSettings(MousewheelOptionID::kWUIInputPriorityMod,
                       MousewheelOptionID::kWUIInputPriorityX,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kWUIInputPriorityY,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kZoom,  //
    MousewheelSettings(MousewheelOptionID::kMapZoomMod,
                       MousewheelOptionID::kMapZoomX,
                       MousewheelOptionID::kMapZoomInvertX,
                       MousewheelOptionID::kMapZoomY,
                       MousewheelOptionID::kMapZoomInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kMapScroll,  //
    MousewheelSettings(MousewheelOptionID::kMapScrollMod,
                       MousewheelOptionID::kMapScroll,
                       MousewheelOptionID::kMapScrollInvert,
                       MousewheelOptionID::kMapScroll,
                       MousewheelOptionID::kMapScrollInvert,
                       DEFAULT_SIGN_SCROLL)},
   {MousewheelHandlerConfigID::kMapScrollHorizY,
    MousewheelSettings(MousewheelOptionID::kMapScrollHorizYMod,
                       MousewheelOptionID::kDisabled,
                       MousewheelOptionID::kDisabled,
                       MousewheelOptionID::kMapScroll,
                       MousewheelOptionID::kMapScrollHorizYInvert,
                       DEFAULT_SIGN_SCROLL)},
   {MousewheelHandlerConfigID::kGameSpeed,  //
    MousewheelSettings(MousewheelOptionID::kGameSpeedMod,
                       MousewheelOptionID::kGameSpeedX,
                       MousewheelOptionID::kGameSpeedInvertX,
                       MousewheelOptionID::kGameSpeedY,
                       MousewheelOptionID::kGameSpeedInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kEditorToolsize,
    MousewheelSettings(MousewheelOptionID::kEditorToolsizeMod,
                       MousewheelOptionID::kEditorToolsizeX,
                       MousewheelOptionID::kEditorToolsizeInvertX,
                       MousewheelOptionID::kEditorToolsizeY,
                       MousewheelOptionID::kEditorToolsizeInvertY,
                       DEFAULT_SIGN_VALUE)}

};

#undef DEFAULT_SIGN_VALUE
#undef DEFAULT_SIGN_MOVE
#undef DEFAULT_SIGN_SCROLL
#undef kSignIncreaseRight
#undef kSignIncreaseUp
#undef kSignNextRight
#undef kSignNextDown
#undef kSignScroll

void set_mousewheel_option_bool(const MousewheelOptionID opt_id, bool value) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kBool);
	mousewheel_options.at(opt_id).set_bool(value);
}
bool get_mousewheel_option_bool(const MousewheelOptionID opt_id) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kBool);
	return static_cast<bool>(mousewheel_options.at(opt_id).current);
}

void set_mousewheel_keymod(const MousewheelOptionID opt_id, uint16_t keymod) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kKeymod);
	mousewheel_options.at(opt_id).set_keymod(keymod);
}
uint16_t get_mousewheel_keymod(const MousewheelOptionID opt_id) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kKeymod);
	return mousewheel_options.at(opt_id).current;
}

int32_t get_mousewheel_change(const MousewheelHandlerConfigID handler_id,
                              const int32_t x,
                              const int32_t y,
                              const uint16_t modstate) {
	return mousewheel_handlers.at(handler_id).get_change(x, y, modstate);
}

Vector2i get_mousewheel_change_2D(const MousewheelHandlerConfigID handler_id,
                                  const int32_t x,
                                  const int32_t y,
                                  const uint16_t modstate) {
	return mousewheel_handlers.at(handler_id).get_change_2D(x, y, modstate);
}

void update_mousewheel_settings() {
	for (MousewheelHandlerConfigID i = MousewheelHandlerConfigID::k__Begin;
	     i <= MousewheelHandlerConfigID::k__End;
	     i = static_cast<MousewheelHandlerConfigID>(static_cast<uint16_t>(i) + 1)) {
		mousewheel_handlers.at(i).update_settings();
	}
}

void init_mousewheel_settings(const bool force_defaults, const bool use_2D_defaults) {
	for (MousewheelOptionID i = MousewheelOptionID::k__Begin; i <= MousewheelOptionID::k__End;
	     i = static_cast<MousewheelOptionID>(static_cast<uint16_t>(i) + 1)) {
		if (force_defaults) {
			if (use_2D_defaults) {
				mousewheel_options.at(i).set2D();
			} else {
				mousewheel_options.at(i).reset();
			}
		} else {
			mousewheel_options.at(i).get_config();
		}
	}
	update_mousewheel_settings();
}
