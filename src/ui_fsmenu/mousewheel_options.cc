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

#include "base/i18n.h"
#include "ui_fsmenu/mousewheel_options.h"
#include "wlapplication_mousewheel_options.h"
#include "wlapplication_options.h"

namespace FsMenu {

#define dir_combine(x, y) static_cast<uint8_t>((2 * x) & y)
#define read_dir(option) \
	dir_combine( \
	   get_mousewheel_option_bool(MousewheelOptionID::option ## X), \
	   get_mousewheel_option_bool(MousewheelOptionID::option ## Y))

void MousewheelConfigSettings::read() {
	Use2Ddefaults = get_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults);
	EnableMapScroll = get_mousewheel_option_bool(MousewheelOptionID::kMapScroll);
	ZoomMod = get_mousewheel_keymod(MousewheelOptionID::kMapZoomMod);
	MapScrollMod = get_mousewheel_keymod(MousewheelOptionID::kMapScrollMod);
	SpeedMod = get_mousewheel_keymod(MousewheelOptionID::kGameSpeedMod);
	ToolsizeMod = get_mousewheel_keymod(MousewheelOptionID::kEditorToolsizeMod);
	ZoomDir = read_dir(kMapZoom);
	SpeedDir = read_dir(kGameSpeed);
	ToolsizeDir = read_dir(kEditorToolsize);
	ValueInvert = read_dir(kUIChangeValueInvert);
	TabInvert = read_dir(kUITabInvert);
	ZoomInvert = read_dir(kMapZoomInvert);
}

#undef read_dir
#undef dir_combine

#define dir_x(dc) static_cast<bool>(dc & 2)
#define dir_y(dc) static_cast<bool>(dc & 1)
#define apply_dir(option, dc) \
	set_mousewheel_option_bool(MousewheelOptionID::option ## X, dir_x(dc)); \
	set_mousewheel_option_bool(MousewheelOptionID::option ## Y, dir_y(dc));

void MousewheelConfigSettings::apply() {
	set_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults, Use2Ddefaults);
	set_mousewheel_option_bool(MousewheelOptionID::kMapScroll, EnableMapScroll);
	set_mousewheel_keymod(MousewheelOptionID::kMapZoomMod, ZoomMod);
	set_mousewheel_keymod(MousewheelOptionID::kMapScrollMod, MapScrollMod);
	set_mousewheel_keymod(MousewheelOptionID::kGameSpeedMod, SpeedMod);
	set_mousewheel_keymod(MousewheelOptionID::kEditorToolsizeMod, ToolsizeMod);
	apply_dir(kMapZoom, ZoomDir)
	apply_dir(kGameSpeed, SpeedDir)
	apply_dir(kEditorToolsize, ToolsizeDir)
	apply_dir(kUIChangeValueInvert, ValueInvert)
	apply_dir(kUITabInvert, TabInvert)
	apply_dir(kMapZoomInvert, ZoomInvert)

	update_mousewheel_settings();
}

#undef apply_dir
#undef dir_x
#undef dir_y

// void MousewheelConfigSettings::save() {}


KeymodButton::KeymodButton(UI::Panel* parent, uint16_t keymod) :
   UI::Button(parent, std::string(), 0, 0, 150, 24, UI::ButtonStyle::kFsMenuSecondary, std::string()) {
	update_text(keymod);
}

void KeymodButton::update_text(uint16_t keymod) {
	std::string t = keymod_string_for(keymod);
	set_title(t.empty() ? _("(plain)") : t);
}

DirDropdown::DirDropdown(Panel* parent, bool invert, bool two_d) :
   UI::Dropdown<uint8_t>(parent,
                         std::string(),
                         0,
                         0,
                         150,
                         4,
                         24,
                         std::string(),
                         UI::DropdownType::kTextual,
                         UI::PanelStyle::kFsMenu,
                         UI::ButtonStyle::kFsMenuMenu) {
	add(_("Disabled"), 0);
	if (two_d) {
		add(_("Any"), 1);
	} else {
		add(_("Vertical"), 1);
		add(_("Horizontal"), 2);
		add(invert ? _("Both") : _("Any"), 1 & 2);
	}
}

KeymodAndDirBox::KeymodAndDirBox(
	UI::Panel* parent, const std::string& title, uint8_t dir, bool has_keymod, uint16_t keymod, bool two_d) :
   UI::Box(parent, UI::PanelStyle::kFsMenu, 0, 0, UI::Box::Horizontal, 500, 36, kPadding),
   title_area(this, UI::PanelStyle::kFsMenu, UI::FontStyle::kFsMenuLabel, title),
   keymod_button(this, keymod),
   dir_dropdown(this, !has_keymod, two_d),
   end_label(this, UI::PanelStyle::kFsMenu, UI::FontStyle::kFsMenuLabel, has_keymod ? _("scroll") : "") {
	add(&title_area);
	if (has_keymod) {
		add(&keymod_button);
	}
	dir_dropdown.select(dir);
	add(&dir_dropdown);
	if (has_keymod) {
		add(&end_label);
	}
}


MousewheelOptionsDialog::MousewheelOptionsDialog(
   UI::Panel* parent) :
	UI::Box(parent, UI::PanelStyle::kFsMenu, 0, 0, UI::Box::Vertical, 0, 0, kPadding),
	settings_(),
	use_2d_defaults_(this,
                         std::string(),
                         0,
                         0,
                         250,
                         4,
                         24,
                         std::string(),
                         UI::DropdownType::kTextual,
                         UI::PanelStyle::kFsMenu,
                         UI::ButtonStyle::kFsMenuMenu),
	zoom_box(this, _("Zoom Map:"), settings_.ZoomDir, true, settings_.ZoomMod),
	mapscroll_box(this, _("Scroll Map:"), settings_.EnableMapScroll, true, settings_.MapScrollMod, true),
	speed_box(this, _("Change Game Speed:"), settings_.SpeedDir, true, settings_.SpeedMod),
	toolsize_box(this, _("Change Editor Toolsize:"), settings_.ToolsizeDir, true, settings_.ToolsizeMod),
	zoom_invert_(this, _("Zoom Map:"), 0, settings_.ZoomInvert, false),
	tab_invert_(this, _("Switch Tab:"), 0, settings_.TabInvert, false),
	value_invert_(this, _("Increase/Decrease:"), 0, settings_.ValueInvert, false) {
	use_2d_defaults_.add(_("Desktop mouse"), 0);
	use_2d_defaults_.add(_("Touchpad"), 1);
	add(&use_2d_defaults_);
	add(&zoom_box);
	add(&mapscroll_box);
	add(&speed_box);
	add(&toolsize_box);
	// add textarea "Invert scroll direction for:"
	add(&zoom_invert_);
	add(&tab_invert_);
	add(&value_invert_);
}


// Saves the options and reloads the active tab
void MousewheelOptionsDialog::clicked_apply() {

}
// Restores old options when canceled
void MousewheelOptionsDialog::clicked_cancel() {

}

}  // namespace FsMenu
