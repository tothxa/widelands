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

#ifndef WL_UI_FSMENU_MOUSEWHEEL_OPTIONS_H
#define WL_UI_FSMENU_MOUSEWHEEL_OPTIONS_H

#include <map>
#include <memory>

#include "ui_basic/box.h"
#include "ui_basic/button.h"
#include "ui_basic/checkbox.h"
#include "ui_basic/dropdown.h"
#include "ui_basic/multilinetextarea.h"
#include "ui_basic/textarea.h"
#include "ui_basic/window.h"
#include "ui_fsmenu/main.h"

namespace FsMenu {

struct MousewheelConfigSettings {
	bool Use2Ddefaults, EnableMapScroll;
	uint16_t ZoomMod, MapScrollMod, SpeedMod, ToolsizeMod;
	uint8_t ZoomDir, SpeedDir, ToolsizeDir;
	uint8_t ValueInvert, TabInvert, ZoomInvert;

	void read();
	void apply();
//	void save();

	MousewheelConfigSettings() {
		read();
	};
};

struct KeymodButton : public UI::Button {
	KeymodButton(UI::Panel* parent, uint16_t keymod);
	void update_text(uint16_t keymod);
};

struct KeymodChooser : public UI::Window {
	KeymodChooser(UI::Panel* parent, const std::string& title);
};

struct DirDropdown : public UI::Dropdown<uint8_t> {
	DirDropdown(Panel* parent, bool invert = false, bool two_d = false);
};

// Box for options of a single handler
struct KeymodAndDirBox : public UI::Box {
	KeymodAndDirBox(
	   UI::Panel* parent,
	   const std::string& title,
	   uint8_t dir,
	   bool has_keymod = false,
	   uint16_t keymod = 0,
	   bool two_d = false );

	uint16_t get_keymod();
	uint8_t get_dir();
	void set(uint16_t keymod, uint8_t dir);

private:
	UI::Textarea title_area;
	KeymodButton keymod_button;
	DirDropdown dir_dropdown;
	UI::Textarea end_label;
};

/**
 * Box for setting mousewheel options
 */
class MousewheelOptionsDialog : public UI::Box {
public:
	MousewheelOptionsDialog(UI::Panel* parent);

	// Saves the options and reloads the active tab
	void clicked_apply();
	// Restores old options when canceled
	void clicked_cancel();

private:
	MousewheelConfigSettings settings_;

	// Option controls
	UI::Dropdown<bool> use_2d_defaults_;
	KeymodAndDirBox zoom_box;
	KeymodAndDirBox mapscroll_box;
	KeymodAndDirBox speed_box;
	KeymodAndDirBox toolsize_box;

	KeymodAndDirBox zoom_invert_;
	KeymodAndDirBox tab_invert_;
	KeymodAndDirBox value_invert_;
};

}  // namespace FsMenu

#endif  // end of include guard: WL_UI_FSMENU_MOUSEWHEEL_OPTIONS_H
