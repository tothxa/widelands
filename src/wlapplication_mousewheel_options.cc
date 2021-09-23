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
	const std::string internal_name_;
	const MousewheelOptionType type_;
	const uint16_t default_;
	const uint16_t default_2D_;
	uint16_t current_;

	MousewheelOption(std::string n, MousewheelOptionType t, uint16_t def, uint16_t def2D)
	   : internal_name_(n), type_(t), default_(def), default_2D_(def2D), current_(def) {
	}

	void get_config() {
		const uint16_t def =
		   get_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults) ? default_2D_ : default_;
		if (!(internal_name_.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				current_ = static_cast<uint16_t>(
				   get_config_bool("mousewheel", internal_name_, static_cast<bool>(def)));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				current_ = get_config_int("mousewheel", internal_name_, def);
			}
		}
	}
	void set_bool(bool value) {
		assert(type_ == MousewheelOptionType::kBool);
		if (!(internal_name_.empty())) {
			set_config_bool("mousewheel", internal_name_, value);
			get_config();
		}
	}
	void set_keymod(uint16_t value) {
		assert(type_ == MousewheelOptionType::kKeymod);
		if (!(internal_name_.empty())) {
			set_config_int("mousewheel", internal_name_, value);
			get_config();
		}
	}
	void reset() {
		if (!(internal_name_.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				set_bool(static_cast<bool>(default_));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				set_keymod(default_);
			}
		}
	}
	void set2D() {
		if (!(internal_name_.empty())) {
			if (type_ == MousewheelOptionType::kBool) {
				set_bool(static_cast<bool>(default_2D_));
			} else {
				assert(type_ == MousewheelOptionType::kKeymod);
				set_keymod(default_2D_);
			}
		}
	}
};

struct MousewheelHandlerOptions {
	const MousewheelOptionID keymod_id_;
	uint16_t current_keymod_ = KMOD_NONE;
	const MousewheelOptionID use_x_;
	const MousewheelOptionID invert_x_;
	const int32_t default_sign_x_;
	int32_t current_sign_x_ = 0;
	const MousewheelOptionID use_y_;
	const MousewheelOptionID invert_y_;
	const int32_t default_sign_y_;
	int32_t current_sign_y_ = 0;

	void update_settings() {
		current_keymod_ = get_mousewheel_keymod(keymod_id_);
		if (get_mousewheel_option_bool(use_x_)) {
			current_sign_x_ = default_sign_x_ * (get_mousewheel_option_bool(invert_x_) ? -1 : 1);
		} else {
			current_sign_x_ = 0;
		}
		if (get_mousewheel_option_bool(use_y_)) {
			current_sign_y_ = default_sign_y_ * (get_mousewheel_option_bool(invert_y_) ? -1 : 1);
		} else {
			current_sign_y_ = 0;
		}
	}

	MousewheelHandlerOptions(MousewheelOptionID keymod_id,
	                   MousewheelOptionID use_x,
	                   MousewheelOptionID invert_x,
	                   MousewheelOptionID use_y,
	                   MousewheelOptionID invert_y,
	                   int32_t def_sign_x,
	                   int32_t def_sign_y)
	   : keymod_id_(keymod_id),
	     use_x_(use_x),
	     invert_x_(invert_x),
	     default_sign_x_(def_sign_x),
	     use_y_(use_y),
	     invert_y_(invert_y),
	     default_sign_y_(def_sign_y) {
	}

	bool can_handle(const int32_t x, const int32_t y, const uint16_t modstate) {
		return (((y && current_sign_y_) || (x && current_sign_x_)) &&
		        matches_keymod(current_keymod_, modstate));
	}
	int32_t get_change(const int32_t x, const int32_t y, const uint16_t modstate) {
		if (can_handle(x, y, modstate)) {
			return (x * current_sign_x_ + y * current_sign_y_);
		}
		return 0;
	}
	Vector2i get_change_2D(const int32_t x, const int32_t y, const uint16_t modstate) {
		if (can_handle(x, y, modstate)) {
			return Vector2i(x * current_sign_x_, y * current_sign_y_);
		}
		return Vector2i(0, 0);
	}
};

static std::map<MousewheelOptionID, MousewheelOption> mousewheel_options = {
   {MousewheelOptionID::kUIChangeValueInvertX,
    MousewheelOption("change_value_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kUIChangeValueInvertY,
    MousewheelOption("change_value_y_invert", MousewheelOptionType::kBool, false, false)},

   {MousewheelOptionID::kUITabInvertX,
    MousewheelOption("tabpanel_x_invert", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kUITabInvertY,
    MousewheelOption("tabpanel_y_invert", MousewheelOptionType::kBool, false, false)},

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

   {MousewheelOptionID::kGameSpeedMod,
    MousewheelOption("gamespeed_modifier", MousewheelOptionType::kKeymod, KMOD_ALT, KMOD_ALT)},
   {MousewheelOptionID::kGameSpeedX,
    MousewheelOption("gamespeed_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kGameSpeedY,
    MousewheelOption("gamespeed_y", MousewheelOptionType::kBool, true, true)},

   {MousewheelOptionID::kEditorToolsizeMod,
    MousewheelOption(
       "editor_toolsize_modifier", MousewheelOptionType::kKeymod, KMOD_ALT, KMOD_ALT)},
   {MousewheelOptionID::kEditorToolsizeX,
    MousewheelOption("editor_toolsize_x", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kEditorToolsizeY,
    MousewheelOption("editor_toolsize_y", MousewheelOptionType::kBool, true, true)},

   {MousewheelOptionID::kAlwaysOn, MousewheelOption("", MousewheelOptionType::kBool, true, true)},
   {MousewheelOptionID::kDisabled, MousewheelOption("", MousewheelOptionType::kBool, false, false)},
   {MousewheelOptionID::kNoMod,
    MousewheelOption("", MousewheelOptionType::kKeymod, KMOD_NONE, KMOD_NONE)},

   {MousewheelOptionID::kUse2Ddefaults,
    MousewheelOption("use_2d_defaults", MousewheelOptionType::kBool, false, true)},

};

// Default signs
#define SIGN_INCREASE_RIGHT -1
#define SIGN_INCREASE_UP 1
#define SIGN_NEXT_RIGHT -1
#define SIGN_NEXT_DOWN -1
#define SIGN_SCROLL -1

#define DEFAULT_SIGN_VALUE SIGN_INCREASE_RIGHT, SIGN_INCREASE_UP
#define DEFAULT_SIGN_MOVE SIGN_NEXT_RIGHT, SIGN_NEXT_DOWN
#define DEFAULT_SIGN_SCROLL SIGN_SCROLL, SIGN_SCROLL

static std::map<MousewheelHandlerConfigID, MousewheelHandlerOptions> mousewheel_handlers = {
   {MousewheelHandlerConfigID::kChangeValue,
    MousewheelHandlerOptions(MousewheelOptionID::kNoMod,
                       MousewheelOptionID::kAlwaysOn,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kAlwaysOn,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kTabBar,  //
    MousewheelHandlerOptions(MousewheelOptionID::kNoMod,
                       MousewheelOptionID::kAlwaysOn,
                       MousewheelOptionID::kUITabInvertX,
                       MousewheelOptionID::kAlwaysOn,
                       MousewheelOptionID::kUITabInvertY,
                       DEFAULT_SIGN_MOVE)},
   {MousewheelHandlerConfigID::kZoom,  //
    MousewheelHandlerOptions(MousewheelOptionID::kMapZoomMod,
                       MousewheelOptionID::kMapZoomX,
                       MousewheelOptionID::kMapZoomInvertX,
                       MousewheelOptionID::kMapZoomY,
                       MousewheelOptionID::kMapZoomInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kMapScroll,  //
    MousewheelHandlerOptions(MousewheelOptionID::kMapScrollMod,
                       MousewheelOptionID::kMapScroll,
                       MousewheelOptionID::kDisabled,  // always use system scroll direction
                       MousewheelOptionID::kMapScroll,
                       MousewheelOptionID::kDisabled,  // always use system scroll direction
                       DEFAULT_SIGN_SCROLL)},
   {MousewheelHandlerConfigID::kGameSpeed,  //
    MousewheelHandlerOptions(MousewheelOptionID::kGameSpeedMod,
                       MousewheelOptionID::kGameSpeedX,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kGameSpeedY,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)},
   {MousewheelHandlerConfigID::kEditorToolsize,
    MousewheelHandlerOptions(MousewheelOptionID::kEditorToolsizeMod,
                       MousewheelOptionID::kEditorToolsizeX,
                       MousewheelOptionID::kUIChangeValueInvertX,
                       MousewheelOptionID::kEditorToolsizeY,
                       MousewheelOptionID::kUIChangeValueInvertY,
                       DEFAULT_SIGN_VALUE)}

};

#undef DEFAULT_SIGN_VALUE
#undef DEFAULT_SIGN_MOVE
#undef DEFAULT_SIGN_SCROLL
#undef SIGN_INCREASE_RIGHT
#undef SIGN_INCREASE_UP
#undef SIGN_NEXT_RIGHT
#undef SIGN_NEXT_DOWN
#undef SIGN_SCROLL

void set_mousewheel_option_bool(const MousewheelOptionID opt_id, bool value) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kBool);
	mousewheel_options.at(opt_id).set_bool(value);
}
bool get_mousewheel_option_bool(const MousewheelOptionID opt_id) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kBool);
	return static_cast<bool>(mousewheel_options.at(opt_id).current_);
}

void set_mousewheel_keymod(const MousewheelOptionID opt_id, uint16_t keymod) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kKeymod);
	mousewheel_options.at(opt_id).set_keymod(keymod);
}
uint16_t get_mousewheel_keymod(const MousewheelOptionID opt_id) {
	assert(mousewheel_options.at(opt_id).type_ == MousewheelOptionType::kKeymod);
	return mousewheel_options.at(opt_id).current_;
}

int32_t get_mousewheel_change(MousewheelHandlerConfigID handler_id,
                              int32_t x,
                              int32_t y,
                              uint16_t modstate) {
	return mousewheel_handlers.at(handler_id).get_change(x, y, modstate);
}

Vector2i get_mousewheel_change_2D(MousewheelHandlerConfigID handler_id,
                                  int32_t x,
                                  int32_t y,
                                  uint16_t modstate) {
	return mousewheel_handlers.at(handler_id).get_change_2D(x, y, modstate);
}

void update_mousewheel_settings() {
	for (MousewheelHandlerConfigID i = MousewheelHandlerConfigID::k__Begin;
	     i <= MousewheelHandlerConfigID::k__End;
	     i = static_cast<MousewheelHandlerConfigID>(static_cast<uint16_t>(i) + 1)) {
		mousewheel_handlers.at(i).update_settings();
	}
}

void init_mousewheel_settings(const bool force_defaults) {
	mousewheel_options.at(MousewheelOptionID::kUse2Ddefaults).get_config();
	const bool use_2D_defaults = get_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults);

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
