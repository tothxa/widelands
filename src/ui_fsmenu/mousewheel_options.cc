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

#define DIR_COMBINE(x, y) static_cast<uint8_t>((2 * x) & y)
#define READ_DIR(option) \
	DIR_COMBINE( \
	   get_mousewheel_option_bool(MousewheelOptionID::option ## X), \
	   get_mousewheel_option_bool(MousewheelOptionID::option ## Y))

void MousewheelConfigSettings::read() {
	use_2d_defaults_ = get_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults);
	enable_map_scroll_ = get_mousewheel_option_bool(MousewheelOptionID::kMapScroll);
	zoom_mod_ = get_mousewheel_keymod(MousewheelOptionID::kMapZoomMod);
	map_scroll_mod_ = get_mousewheel_keymod(MousewheelOptionID::kMapScrollMod);
	speed_mod_ = get_mousewheel_keymod(MousewheelOptionID::kGameSpeedMod);
	toolsize_mod_ = get_mousewheel_keymod(MousewheelOptionID::kEditorToolsizeMod);
	zoom_dir_ = READ_DIR(kMapZoom);
	speed_dir_ = READ_DIR(kGameSpeed);
	toolsize_dir_ = READ_DIR(kEditorToolsize);
	value_invert_ = READ_DIR(kUIChangeValueInvert);
	tab_invert_ = READ_DIR(kUITabInvert);
	zoom_invert_ = READ_DIR(kMapZoomInvert);
}

#undef READ_DIR
#undef DIR_COMBINE

#define DIR_X(dc) static_cast<bool>(dc & 2)
#define DIR_Y(dc) static_cast<bool>(dc & 1)
#define APPLY_DIR(option, dc) \
	set_mousewheel_option_bool(MousewheelOptionID::option ## X, DIR_X(dc)); \
	set_mousewheel_option_bool(MousewheelOptionID::option ## Y, DIR_Y(dc));

void MousewheelConfigSettings::apply() {
	set_mousewheel_option_bool(MousewheelOptionID::kUse2Ddefaults, use_2d_defaults_);
	set_mousewheel_option_bool(MousewheelOptionID::kMapScroll, enable_map_scroll_);
	set_mousewheel_keymod(MousewheelOptionID::kMapZoomMod, zoom_mod_);
	set_mousewheel_keymod(MousewheelOptionID::kMapScrollMod, map_scroll_mod_);
	set_mousewheel_keymod(MousewheelOptionID::kGameSpeedMod, speed_mod_);
	set_mousewheel_keymod(MousewheelOptionID::kEditorToolsizeMod, toolsize_mod_);
	APPLY_DIR(kMapZoom, zoom_dir_)
	APPLY_DIR(kGameSpeed, speed_dir_)
	APPLY_DIR(kEditorToolsize, toolsize_dir_)
	APPLY_DIR(kUIChangeValueInvert, value_invert_)
	APPLY_DIR(kUITabInvert, tab_invert_)
	APPLY_DIR(kMapZoomInvert, zoom_invert_)

	update_mousewheel_settings();
}

#undef APPLY_DIR
#undef DIR_X
#undef DIR_Y

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
   title_area_(this, UI::PanelStyle::kFsMenu, UI::FontStyle::kFsMenuLabel, title),
   keymod_button_(this, keymod),
   dir_dropdown_(this, !has_keymod, two_d),
   end_label_(this, UI::PanelStyle::kFsMenu, UI::FontStyle::kFsMenuLabel, has_keymod ? _("scroll") : "") {
	add(&title_area_);
	if (has_keymod) {
		add(&keymod_button_);
	}
	dir_dropdown_.select(dir);
	add(&dir_dropdown_);
	if (has_keymod) {
		add(&end_label_);
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
	zoom_box_(this, _("Zoom Map:"), settings_.zoom_dir_, true, settings_.zoom_mod_),
	mapscroll_box_(this, _("Scroll Map:"), settings_.enable_map_scroll_, true, settings_.map_scroll_mod_, true),
	speed_box_(this, _("Change Game Speed:"), settings_.speed_dir_, true, settings_.speed_mod_),
	toolsize_box_(this, _("Change Editor Toolsize:"), settings_.toolsize_dir_, true, settings_.toolsize_mod_),
	zoom_invert_(this, _("Zoom Map:"), 0, settings_.zoom_invert_, false),
	tab_invert_(this, _("Switch Tab:"), 0, settings_.tab_invert_, false),
	value_invert_(this, _("Increase/Decrease:"), 0, settings_.value_invert_, false) {
	use_2d_defaults_.add(_("Desktop mouse"), false);
	use_2d_defaults_.add(_("Touchpad"), true);
	add(&use_2d_defaults_);
	add_space(8);
	add(&zoom_box_);
	add(&mapscroll_box_);
	add(&speed_box_);
	add(&toolsize_box_);
	add_space(8);
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
