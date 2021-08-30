/*
 * Copyright (C) 2012-2021 by the Widelands Development Team
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

#include "wlapplication_options.h"

#include <cassert>
#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <boost/format.hpp>

#include "base/i18n.h"
#include "base/log.h"
#include "base/vector.h"
#include "graphic/text_layout.h"
#include "io/filesystem/disk_filesystem.h"
#include "logic/filesystem_constants.h"

static Profile g_options(Profile::err_log);

static std::unique_ptr<FileSystem> config_dir = nullptr;

void check_config_used() {
	g_options.check_used();
}

Section& get_config_section() {
	return g_options.pull_section("global");
}

Section& get_config_section(const std::string& section) {
	return g_options.pull_section(section.c_str());
}

Section* get_config_section_ptr(const std::string& section) {
	return g_options.get_section(section);
}

bool get_config_bool(const std::string& name, const bool dflt) {
	return g_options.pull_section("global").get_bool(name.c_str(), dflt);
}

bool get_config_bool(const std::string& section, const std::string& name, const bool dflt) {
	return g_options.pull_section(section.c_str()).get_bool(name.c_str(), dflt);
}

int32_t get_config_int(const std::string& name, const int32_t dflt) {
	return g_options.pull_section("global").get_int(name.c_str(), dflt);
}

int32_t get_config_int(const std::string& section, const std::string& name, const int32_t dflt) {
	return g_options.pull_section(section.c_str()).get_int(name.c_str(), dflt);
}

uint32_t get_config_natural(const std::string& name, const uint32_t dflt) {
	return g_options.pull_section("global").get_natural(name.c_str(), dflt);
}

uint32_t get_config_natural(const std::string& section, const std::string& name, uint32_t dflt) {
	return g_options.pull_section(section.c_str()).get_natural(name.c_str(), dflt);
}

std::string get_config_string(const std::string& name, const std::string& dflt) {
	return g_options.pull_section("global").get_string(name.c_str(), dflt.c_str());
}

std::string
get_config_string(const std::string& section, const std::string& name, const std::string& dflt) {
	return g_options.pull_section(section.c_str()).get_string(name.c_str(), dflt.c_str());
}

Section& get_config_safe_section() {
	return g_options.get_safe_section("global");
}

Section& get_config_safe_section(const std::string& section) {
	return g_options.get_safe_section(section);
}

void set_config_bool(const std::string& name, const bool value) {
	g_options.pull_section("global").set_bool(name.c_str(), value);
}

void set_config_bool(const std::string& section, const std::string& name, const bool value) {
	g_options.pull_section(section.c_str()).set_bool(name.c_str(), value);
}

void set_config_int(const std::string& name, int32_t value) {
	g_options.pull_section("global").set_int(name.c_str(), value);
}

void set_config_int(const std::string& section, const std::string& name, const int32_t value) {
	g_options.pull_section(section.c_str()).set_int(name.c_str(), value);
}

void set_config_string(const std::string& name, const std::string& value) {
	g_options.pull_section("global").set_string(name.c_str(), value.c_str());
}

void set_config_string(const std::string& section,
                       const std::string& name,
                       const std::string& value) {
	g_options.pull_section(section.c_str()).set_string(name.c_str(), value.c_str());
}

struct KeyboardShortcutInfo {
	enum class Scope {
		kGlobal,  // special value that intersects with all other scopes

		kMainMenu,
		kEditor,
		kGame,
	};

	const std::set<Scope> scopes;
	const SDL_Keysym default_shortcut;
	SDL_Keysym current_shortcut;
	const std::string internal_name;
	std::string fastplace_name;
	const std::function<std::string()> descname;

	KeyboardShortcutInfo(const std::set<Scope>& s,
	                     const SDL_Keysym& sym,
	                     const std::string& n,
	                     const std::function<std::string()>& f)
	   : scopes(s), default_shortcut(sym), current_shortcut(sym), internal_name(n), descname(f) {
	}
};

// Use Cmd instead of Ctrl for default shortcuts on MacOS
#ifdef __APPLE__
constexpr uint16_t kDefaultCtrlModifier = KMOD_GUI;
#else
constexpr uint16_t kDefaultCtrlModifier = KMOD_CTRL;
#endif

static inline SDL_Keysym keysym(const SDL_Keycode c, uint16_t mod = 0) {
	return SDL_Keysym{SDL_GetScancodeFromKey(c), c, mod, 0};
}

static std::map<KeyboardShortcut, KeyboardShortcutInfo> shortcuts_ = {
   {KeyboardShortcut::kMainMenuNew, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                         keysym(SDLK_n),
                                                         "mainmenu_new",
                                                         []() { return _("New Game"); })},
   {KeyboardShortcut::kMainMenuLoad, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                          keysym(SDLK_l),
                                                          "mainmenu_load",
                                                          []() { return _("Load Game"); })},
   {KeyboardShortcut::kMainMenuReplay,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_r),
                         "mainmenu_replay",
                         []() { return _("Watch Replay"); })},
   {KeyboardShortcut::kMainMenuRandomMatch,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_z),
                         "mainmenu_random",
                         []() { return _("New Random Game"); })},
   {KeyboardShortcut::kMainMenuTutorial,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_t),
                         "mainmenu_tutorial",
                         []() { return _("Tutorials"); })},
   {KeyboardShortcut::kMainMenuCampaign,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_h),
                         "mainmenu_campaign",
                         []() { return _("Campaigns"); })},
   {KeyboardShortcut::kMainMenuSP, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                        keysym(SDLK_s),
                                                        "mainmenu_sp",
                                                        []() { return _("Singleplayer"); })},
   {KeyboardShortcut::kMainMenuMP, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                        keysym(SDLK_m),
                                                        "mainmenu_mp",
                                                        []() { return _("Multiplayer"); })},
   {KeyboardShortcut::kMainMenuE, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                       keysym(SDLK_e),
                                                       "mainmenu_e",
                                                       []() { return _("Editor"); })},
   {KeyboardShortcut::kMainMenuEditorLoad,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_b),
                         "mainmenu_editor_load",
                         []() { return _("Editor – Load Map"); })},
   {KeyboardShortcut::kMainMenuEditorNew,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_k),
                         "mainmenu_editor_new",
                         []() { return _("Editor – New Map"); })},
   {KeyboardShortcut::kMainMenuEditorRandom,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_y),
                         "mainmenu_editor_random",
                         []() { return _("Editor – New Random Map"); })},
   {KeyboardShortcut::kMainMenuContinueEditing,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_w),
                         "mainmenu_editor_continue",
                         []() { return _("Continue Editing"); })},
   {KeyboardShortcut::kMainMenuContinuePlaying,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_c),
                         "mainmenu_continue",
                         []() { return _("Continue Playing"); })},
   {KeyboardShortcut::kMainMenuQuit, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                          keysym(SDLK_ESCAPE),
                                                          "mainmenu_quit",
                                                          []() { return _("Exit Widelands"); })},
   {KeyboardShortcut::kMainMenuAbout, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                           keysym(SDLK_F1),
                                                           "mainmenu_about",
                                                           []() { return _("About"); })},
   {KeyboardShortcut::kMainMenuAddons,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_a),
                         "mainmenu_addons",
                         []() { return _("Add-Ons"); })},
   {KeyboardShortcut::kMainMenuLAN, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                         keysym(SDLK_p),
                                                         "mainmenu_lan",
                                                         []() { return _("LAN / Direct IP"); })},
   {KeyboardShortcut::kMainMenuLobby, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                           keysym(SDLK_j),
                                                           "mainmenu_lobby",
                                                           []() { return _("Metaserver Lobby"); })},
   {KeyboardShortcut::kMainMenuLogin, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                                                           keysym(SDLK_u),
                                                           "mainmenu_login",
                                                           []() { return _("Internet Login"); })},
   {KeyboardShortcut::kMainMenuOptions,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kMainMenu},
                         keysym(SDLK_o),
                         "mainmenu_options",
                         []() { return _("Options"); })},

   {KeyboardShortcut::kCommonBuildhelp,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_SPACE),
                         "buildhelp",
                         []() { return _("Toggle Building Spaces"); })},
   {KeyboardShortcut::kCommonMinimap,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_m),
                         "minimap",
                         []() { return _("Toggle Minimap"); })},
   {KeyboardShortcut::kCommonEncyclopedia,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_F1),
                         "encyclopedia",
                         []() { return _("Encyclopedia"); })},
   {KeyboardShortcut::kCommonTextCut, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                                                           keysym(SDLK_x, kDefaultCtrlModifier),
                                                           "cut",
                                                           []() { return _("Cut Text"); })},
   {KeyboardShortcut::kCommonTextCopy, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                                                            keysym(SDLK_c, kDefaultCtrlModifier),
                                                            "copy",
                                                            []() { return _("Copy Text"); })},
   {KeyboardShortcut::kCommonTextPaste, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                                                             keysym(SDLK_v, kDefaultCtrlModifier),
                                                             "paste",
                                                             []() { return _("Paste Text"); })},
   {KeyboardShortcut::kCommonSelectAll, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                                                             keysym(SDLK_a, kDefaultCtrlModifier),
                                                             "selectall",
                                                             []() { return _("Select All"); })},
   {KeyboardShortcut::kCommonDeleteItem,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                         keysym(SDLK_DELETE),
                         "delete",
                         []() { return _("Delete Item"); })},
   {KeyboardShortcut::kCommonTooltipAccessibilityMode,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                         keysym(SDLK_F2),
                         "tt_access_mode",
                         []() { return _("Tooltip Accessibility Mode Key"); })},
   {KeyboardShortcut::kCommonFullscreen,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                         keysym(SDLK_f, kDefaultCtrlModifier),
                         "fullscreen",
                         []() { return _("Toggle Fullscreen"); })},
   {KeyboardShortcut::kCommonScreenshot,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGlobal},
                         keysym(SDLK_F11),
                         "screenshot",
                         []() { return _("Take Screenshot"); })},
   {KeyboardShortcut::kCommonZoomIn,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_PLUS, kDefaultCtrlModifier),
                         "zoom_in",
                         []() { return _("Zoom In"); })},
   {KeyboardShortcut::kCommonZoomOut,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_MINUS, kDefaultCtrlModifier),
                         "zoom_out",
                         []() { return _("Zoom Out"); })},
   {KeyboardShortcut::kCommonZoomReset,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_0, kDefaultCtrlModifier),
                         "zoom_reset",
                         []() { return _("Reset Zoom"); })},
   {KeyboardShortcut::kCommonQuicknavNext,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_PERIOD),
                         "quicknav_next",
                         []() { return _("Jump to Next Location"); })},
   {KeyboardShortcut::kCommonQuicknavPrev,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame, KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_COMMA),
                         "quicknav_prev",
                         []() { return _("Jump to Previous Location"); })},

   {KeyboardShortcut::kEditorMenu, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_h),
                                                        "editor_menu",
                                                        []() { return _("Menu"); })},
   {KeyboardShortcut::kEditorSave, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_s, kDefaultCtrlModifier),
                                                        "editor_save",
                                                        []() { return _("Save Map"); })},
   {KeyboardShortcut::kEditorLoad, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_l, kDefaultCtrlModifier),
                                                        "editor_load",
                                                        []() { return _("Load Map"); })},
   {KeyboardShortcut::kEditorUndo, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_z, kDefaultCtrlModifier),
                                                        "editor_undo",
                                                        []() { return _("Undo"); })},
   {KeyboardShortcut::kEditorRedo, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_y, kDefaultCtrlModifier),
                                                        "editor_redo",
                                                        []() { return _("Redo"); })},
   {KeyboardShortcut::kEditorTools, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                         keysym(SDLK_t),
                                                         "editor_tools",
                                                         []() { return _("Tools"); })},
   {KeyboardShortcut::kEditorInfo, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                        keysym(SDLK_i),
                                                        "editor_info",
                                                        []() { return _("Info Tool"); })},
   {KeyboardShortcut::kEditorPlayers, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                                                           keysym(SDLK_p),
                                                           "editor_players",
                                                           []() { return _("Players Menu"); })},
   {KeyboardShortcut::kEditorShowhideGrid,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_g),
                         "editor_showhide_",
                         []() { return _("Toggle Grid"); })},
   {KeyboardShortcut::kEditorShowhideImmovables,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_o),
                         "editor_showhide_immovables",
                         []() { return _("Toggle Immovables"); })},
   {KeyboardShortcut::kEditorShowhideCritters,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_a),
                         "editor_showhide_critters",
                         []() { return _("Toggle Animals"); })},
   {KeyboardShortcut::kEditorShowhideResources,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kEditor},
                         keysym(SDLK_r),
                         "editor_showhide_resources",
                         []() { return _("Toggle Resources"); })},

#define EDITOR_TOOLSIZE(radius, key)                                                               \
	{                                                                                               \
		KeyboardShortcut::kEditorToolsize##radius,                                                   \
		   KeyboardShortcutInfo(                                                                     \
		      {KeyboardShortcutInfo::Scope::kEditor}, keysym(SDLK_##key), "editor_toolsize" #radius, \
		      []() { return (boost::format(_("Set Toolsize to %d")) % radius).str(); })              \
	}
   EDITOR_TOOLSIZE(1, 1),
   EDITOR_TOOLSIZE(2, 2),
   EDITOR_TOOLSIZE(3, 3),
   EDITOR_TOOLSIZE(4, 4),
   EDITOR_TOOLSIZE(5, 5),
   EDITOR_TOOLSIZE(6, 6),
   EDITOR_TOOLSIZE(7, 7),
   EDITOR_TOOLSIZE(8, 8),
   EDITOR_TOOLSIZE(9, 9),
   EDITOR_TOOLSIZE(10, 0),
#undef EDITOR_TOOLSIZE

   {KeyboardShortcut::kInGameShowhideCensus,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_c),
                         "game_showhide_census",
                         []() { return _("Toggle Census"); })},
   {KeyboardShortcut::kInGameShowhideStats,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_s),
                         "game_showhide_stats",
                         []() { return _("Toggle Status Labels"); })},
   {KeyboardShortcut::kInGameShowhideSoldiers,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_l),
                         "game_showhide_soldiers",
                         []() { return _("Toggle Soldier Levels"); })},
   {KeyboardShortcut::kInGameShowhideBuildings,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_u),
                         "game_showhide_buildings",
                         []() { return _("Toggle Buildings Visibility"); })},
   {KeyboardShortcut::kInGameShowhideWorkareas,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_w),
                         "game_showhide_workareas",
                         []() { return _("Toggle Overlapping Workareas"); })},
   {KeyboardShortcut::kInGameStatsGeneral,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_a),
                         "game_stats_general",
                         []() { return _("General Statistics"); })},
   {KeyboardShortcut::kInGameStatsWares,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_p),
                         "game_stats_wares",
                         []() { return _("Ware Statistics"); })},
   {KeyboardShortcut::kInGameStatsBuildings,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_b),
                         "game_stats_buildings",
                         []() { return _("Building Statistics"); })},
   {KeyboardShortcut::kInGameStatsStock,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_i),
                         "game_stats_stock",
                         []() { return _("Stock Inventory"); })},
   {KeyboardShortcut::kInGameStatsSoldiers,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_x),
                         "game_stats_soldiers",
                         []() { return _("Soldier Statistics"); })},
   {KeyboardShortcut::kInGameStatsSeafaring,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_e),
                         "game_stats_seafaring",
                         []() { return _("Seafaring Statistics"); })},
   {KeyboardShortcut::kInGameObjectives, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                              keysym(SDLK_t),
                                                              "game_objectives",
                                                              []() { return _("Objectives"); })},
   {KeyboardShortcut::kInGameMessages, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                            keysym(SDLK_n),
                                                            "game_messages",
                                                            []() { return _("Messages"); })},
   {KeyboardShortcut::kInGameSpeedDown,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEDOWN),
                         "game_speed_down",
                         []() { return _("Decrease Game Speed by 1×"); })},
   {KeyboardShortcut::kInGameSpeedDownSlow,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEDOWN, KMOD_SHIFT),
                         "game_speed_down_slow",
                         []() { return _("Decrease Game Speed by 0.25×"); })},
   {KeyboardShortcut::kInGameSpeedDownFast,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEDOWN, kDefaultCtrlModifier),
                         "game_speed_down_fast",
                         []() { return _("Decrease Game Speed by 10×"); })},
   {KeyboardShortcut::kInGameSpeedUp,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEUP),
                         "game_speed_up",
                         []() { return _("Increase Game Speed by 1×"); })},
   {KeyboardShortcut::kInGameSpeedUpSlow,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEUP, KMOD_SHIFT),
                         "game_speed_up_slow",
                         []() { return _("Increase Game Speed by 0.25×"); })},
   {KeyboardShortcut::kInGameSpeedUpFast,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAGEUP, kDefaultCtrlModifier),
                         "game_speed_up_fast",
                         []() { return _("Increase Game Speed by 10×"); })},
   {KeyboardShortcut::kInGameSpeedReset,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_PAUSE, KMOD_SHIFT),
                         "game_speed_reset",
                         []() { return _("Reset Game Speed"); })},
   {KeyboardShortcut::kInGamePause, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                         keysym(SDLK_PAUSE),
                                                         "game_pause",
                                                         []() { return _("Pause"); })},
   {KeyboardShortcut::kInGameScrollToHQ,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_HOME),
                         "game_hq",
                         []() { return _("Scroll to Starting Field"); })},
   {KeyboardShortcut::kInGameChat, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                        keysym(SDLK_RETURN),
                                                        "game_chat",
                                                        []() { return _("Chat"); })},
   {KeyboardShortcut::kInGameSave, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                        keysym(SDLK_s, kDefaultCtrlModifier),
                                                        "game_save",
                                                        []() { return _("Save Game"); })},
   {KeyboardShortcut::kInGameLoad, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                                                        keysym(SDLK_l, kDefaultCtrlModifier),
                                                        "game_load",
                                                        []() { return _("Load Game"); })},
   {KeyboardShortcut::kInGameMessagesGoto,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_g),
                         "game_msg_goto",
                         []() { return _("Messages: Go to Location"); })},
   {KeyboardShortcut::kInGameMessagesFilterAll,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_0, KMOD_ALT),
                         "game_msg_filter_all",
                         []() { return _("Messages: Show "); })},
   {KeyboardShortcut::kInGameMessagesFilterGeologists,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_1, KMOD_ALT),
                         "game_msg_filter_geo",
                         []() { return _("Messages: Show Geologists’ Messages"); })},
   {KeyboardShortcut::kInGameMessagesFilterEconomy,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_2, KMOD_ALT),
                         "game_msg_filter_eco",
                         []() { return _("Messages: Show Economy Messages"); })},
   {KeyboardShortcut::kInGameMessagesFilterSeafaring,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_3, KMOD_ALT),
                         "game_msg_filter_seafaring",
                         []() { return _("Messages: Show Seafaring Messages"); })},
   {KeyboardShortcut::kInGameMessagesFilterWarfare,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_4, KMOD_ALT),
                         "game_msg_filter_warfare",
                         []() { return _("Messages: Show Military Messages"); })},
   {KeyboardShortcut::kInGameMessagesFilterScenario,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_5, KMOD_ALT),
                         "game_msg_filter_scenario",
                         []() { return _("Messages: Show Scenario Messages"); })},
   {KeyboardShortcut::kInGameSeafaringstatsGotoShip,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_j),
                         "game_sfstats_goto",
                         []() { return _("Seafaring: Go to Ship"); })},
   {KeyboardShortcut::kInGameSeafaringstatsWatchShip,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_f),
                         "game_sfstats_watch",
                         []() { return _("Seafaring: Watch Ship"); })},
   {KeyboardShortcut::kInGameSeafaringstatsOpenShipWindow,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_o),
                         "game_sfstats_open",
                         []() { return _("Seafaring: Open Ship Window"); })},
   {KeyboardShortcut::kInGameSeafaringstatsOpenShipWindowAndGoto,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_o, kDefaultCtrlModifier),
                         "game_sfstats_open_goto",
                         []() { return _("Seafaring: Open Ship Window And Go to Ship"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterAll,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_0, KMOD_SHIFT),
                         "game_sfstats_filter_all",
                         []() { return _("Seafaring: Show All Ships"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterIdle,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_1, KMOD_SHIFT),
                         "game_sfstats_filter_idle",
                         []() { return _("Seafaring: Show Idle Ships"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterShipping,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_2, KMOD_SHIFT),
                         "game_sfstats_filter_ship",
                         []() { return _("Seafaring: Show Transport Ships"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterExpWait,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_3, KMOD_SHIFT),
                         "game_sfstats_filter_wait",
                         []() { return _("Seafaring: Show Waiting Expeditions"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterExpScout,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_4, KMOD_SHIFT),
                         "game_sfstats_filter_scout",
                         []() { return _("Seafaring: Show Scouting Expeditions"); })},
   {KeyboardShortcut::kInGameSeafaringstatsFilterExpPortspace,
    KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},
                         keysym(SDLK_5, KMOD_SHIFT),
                         "game_sfstats_filter_port",
                         []() { return _("Seafaring: Show Expeditions with Port Spaces"); })},
#define QUICKNAV(i)                                                                                \
	{KeyboardShortcut::kInGameQuicknavSet##i,                                                       \
	 KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame},                                     \
	                      keysym(SDLK_##i, kDefaultCtrlModifier), "game_quicknav_set_" #i,          \
	                      []() { return (boost::format(_("Set Landmark #%d")) % i).str(); })},      \
	{                                                                                               \
		KeyboardShortcut::kInGameQuicknavGoto##i,                                                    \
		   KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame}, keysym(SDLK_##i),              \
		                        "game_quicknav_goto_" #i,                                            \
		                        []() { return (boost::format(_("Go To Landmark #%d")) % i).str(); }) \
	}
   QUICKNAV(1),
   QUICKNAV(2),
   QUICKNAV(3),
   QUICKNAV(4),
   QUICKNAV(5),
   QUICKNAV(6),
   QUICKNAV(7),
   QUICKNAV(8),
   QUICKNAV(9),
#undef QUICKNAV
};

void set_fastplace_shortcut(const KeyboardShortcut id, const std::string& building) {
	assert(id >= KeyboardShortcut::kFastplace__Begin && id <= KeyboardShortcut::kFastplace__End);
	KeyboardShortcutInfo& info = shortcuts_.at(id);
	info.fastplace_name = building;
	set_config_string("keyboard_fastplace", info.internal_name, building);
	if (building.empty()) {
		set_shortcut(id, keysym(SDLK_UNKNOWN), nullptr);
	}
}

const std::string& get_fastplace_shortcut(const KeyboardShortcut id) {
	return shortcuts_.at(id).fastplace_name;
}

std::string to_string(const KeyboardShortcut id) {
	return shortcuts_.at(id).descname();
}

SDL_Keysym get_default_shortcut(const KeyboardShortcut id) {
	return shortcuts_.at(id).default_shortcut;
}

static void write_shortcut(const KeyboardShortcut id, const SDL_Keysym code) {
	set_config_int("keyboard_sym", shortcuts_.at(id).internal_name, code.sym);
	set_config_int("keyboard_mod", shortcuts_.at(id).internal_name, code.mod);
}

static bool shared_scope(const std::set<KeyboardShortcutInfo::Scope>& scopes,
                         const KeyboardShortcutInfo& k) {
	if (scopes.count(KeyboardShortcutInfo::Scope::kGlobal) ||
	    k.scopes.count(KeyboardShortcutInfo::Scope::kGlobal)) {
		return true;
	}

	for (KeyboardShortcutInfo::Scope s : scopes) {
		if (k.scopes.count(s)) {
			return true;
		}
	}
	return false;
}

bool set_shortcut(const KeyboardShortcut id, const SDL_Keysym code, KeyboardShortcut* conflict) {
	const std::set<KeyboardShortcutInfo::Scope>& scopes = shortcuts_.at(id).scopes;

	for (auto& pair : shortcuts_) {
		if (pair.first != id && shared_scope(scopes, pair.second) &&
		    matches_shortcut(pair.first, code)) {
			if (conflict != nullptr) {
				*conflict = pair.first;
			}
			return false;
		}
	}

	shortcuts_.at(id).current_shortcut = code;
	write_shortcut(id, code);
	return true;
}

SDL_Keysym get_shortcut(const KeyboardShortcut id) {
	return shortcuts_.at(id).current_shortcut;
}

static const std::map<SDL_Keycode, SDL_Keycode> kNumpadIdentifications = {
   {SDLK_KP_9, SDLK_PAGEUP},      {SDLK_KP_8, SDLK_UP},         {SDLK_KP_7, SDLK_HOME},
   {SDLK_KP_6, SDLK_RIGHT},       {SDLK_KP_4, SDLK_LEFT},       {SDLK_KP_3, SDLK_PAGEDOWN},
   {SDLK_KP_2, SDLK_DOWN},        {SDLK_KP_1, SDLK_END},        {SDLK_KP_0, SDLK_INSERT},
   {SDLK_KP_PERIOD, SDLK_DELETE}, {SDLK_KP_ENTER, SDLK_RETURN}, {SDLK_KP_MINUS, SDLK_MINUS},
   {SDLK_KP_PLUS, SDLK_PLUS},
};

bool matches_keymod(const uint16_t mod1, const uint16_t mod2) {
	const bool ctrl1 = mod1 & KMOD_CTRL;
	const bool shift1 = mod1 & KMOD_SHIFT;
	const bool alt1 = mod1 & KMOD_ALT;
	const bool gui1 = mod1 & KMOD_GUI;
	const bool ctrl2 = mod2 & KMOD_CTRL;
	const bool shift2 = mod2 & KMOD_SHIFT;
	const bool alt2 = mod2 & KMOD_ALT;
	const bool gui2 = mod2 & KMOD_GUI;

	if (ctrl1 != ctrl2 || shift1 != shift2 || alt1 != alt2 || gui1 != gui2) {
		return false;
	}

	return true;
}

bool matches_shortcut(const KeyboardShortcut id, const SDL_Keysym code) {
	return matches_shortcut(id, code.sym, code.mod);
}
bool matches_shortcut(const KeyboardShortcut id, const SDL_Keycode code, const int mod) {
	const SDL_Keysym key = get_shortcut(id);
	if (key.sym == SDLK_UNKNOWN || code == SDLK_UNKNOWN) {
		return false;
	}

	if (!matches_keymod(key.mod, mod)) {
		return false;
	}

	if (key.sym == code) {
		return true;
	}

	// Some extra checks so we can identify keypad keys with their "normal" equivalents,
	// e.g. pressing '+' or numpad_'+' should always have the same effect

	if (mod & KMOD_NUM) {
		// If numlock is on and a number was pressed, only compare the entered number value.
		// Annoyingly, there seems to be no strict rule whether the SDLK_ constants are
		// ranged 0,1,…,9 or 1,…,9,0 so we have to treat 0 as a special case.
		if (code == SDLK_KP_0) {
			return key.sym == SDLK_0;
		}
		if (code == SDLK_0) {
			return key.sym == SDLK_KP_0;
		}
		if (code >= SDLK_1 && code <= SDLK_9) {
			return key.sym == code + SDLK_KP_1 - SDLK_1;
		}
		if (code >= SDLK_KP_1 && code <= SDLK_KP_9) {
			return key.sym == code + SDLK_1 - SDLK_KP_1;
		}
	}

	for (const auto& pair : kNumpadIdentifications) {
		if ((code == pair.first && key.sym == pair.second) ||
		    (code == pair.second && key.sym == pair.first)) {
			return true;
		}
	}

	return false;
}

std::string matching_fastplace_shortcut(const SDL_Keysym key) {
	for (int i = static_cast<int>(KeyboardShortcut::kFastplace__Begin);
	     i < static_cast<int>(KeyboardShortcut::kFastplace__End); ++i) {
		const KeyboardShortcut id = static_cast<KeyboardShortcut>(i);
		const std::string& str = shortcuts_.at(id).fastplace_name;
		if (!str.empty() && matches_shortcut(id, key)) {
			return str;
		}
	}
	return "";
}

KeyboardShortcut shortcut_from_string(const std::string& name) {
	for (const auto& pair : shortcuts_) {
		if (pair.second.internal_name == name) {
			return pair.first;
		}
	}
	throw wexception("Shortcut '%s' does not exist", name.c_str());
}

std::string shortcut_string_for(const KeyboardShortcut id, const bool rt_escape) {
	return shortcut_string_for(get_shortcut(id), rt_escape);
}

static std::string key_name(const SDL_Keycode k) {
	switch (k) {
	case SDLK_SPACE:
		return pgettext("hotkey", "Space");
	case SDLK_RETURN:
		return pgettext("hotkey", "Enter");
	case SDLK_ESCAPE:
		return pgettext("hotkey", "Escape");
	case SDLK_TAB:
		return pgettext("hotkey", "Tab");
	case SDLK_MENU:
		return pgettext("hotkey", "Menu");
	case SDLK_PAUSE:
		return pgettext("hotkey", "Pause");
	case SDLK_PAGEUP:
		return pgettext("hotkey", "Page Up");
	case SDLK_PAGEDOWN:
		return pgettext("hotkey", "Page Down");
	case SDLK_HOME:
		return pgettext("hotkey", "Home");
	case SDLK_END:
		return pgettext("hotkey", "End");
	case SDLK_LEFT:
		return pgettext("hotkey", "Left Arrow Key");
	case SDLK_RIGHT:
		return pgettext("hotkey", "Right Arrow Key");
	case SDLK_UP:
		return pgettext("hotkey", "Up Arrow Key");
	case SDLK_DOWN:
		return pgettext("hotkey", "Down Arrow Key");
	case SDLK_INSERT:
		return pgettext("hotkey", "Insert");
	case SDLK_DELETE:
		return pgettext("hotkey", "Delete");
	case SDLK_BACKSPACE:
		return pgettext("hotkey", "Backspace");
	case SDLK_CAPSLOCK:
		return pgettext("hotkey", "Caps Lock");
	case SDLK_NUMLOCKCLEAR:
		return pgettext("hotkey", "Numpad Lock");
	case SDLK_SCROLLLOCK:
		return pgettext("hotkey", "Scroll Lock");
	case SDLK_KP_1:
		return pgettext("hotkey", "Keypad 1");
	case SDLK_KP_2:
		return pgettext("hotkey", "Keypad 2");
	case SDLK_KP_3:
		return pgettext("hotkey", "Keypad 3");
	case SDLK_KP_4:
		return pgettext("hotkey", "Keypad 4");
	case SDLK_KP_5:
		return pgettext("hotkey", "Keypad 5");
	case SDLK_KP_6:
		return pgettext("hotkey", "Keypad 6");
	case SDLK_KP_7:
		return pgettext("hotkey", "Keypad 7");
	case SDLK_KP_8:
		return pgettext("hotkey", "Keypad 8");
	case SDLK_KP_9:
		return pgettext("hotkey", "Keypad 9");
	case SDLK_KP_0:
		return pgettext("hotkey", "Keypad 0");
	case SDLK_KP_PERIOD:
		return pgettext("hotkey", "Keypad .");
	case SDLK_KP_PLUS:
		return pgettext("hotkey", "Keypad +");
	case SDLK_KP_MINUS:
		return pgettext("hotkey", "Keypad -");
	case SDLK_KP_MULTIPLY:
		return pgettext("hotkey", "Keypad *");
	case SDLK_KP_DIVIDE:
		return pgettext("hotkey", "Keypad /");
	case SDLK_KP_ENTER:
		return pgettext("hotkey", "Keypad Enter");
	default:
		return SDL_GetKeyName(k);
	}
}

std::string shortcut_string_for(const SDL_Keysym sym, const bool rt_escape) {
	if (sym.sym == SDLK_UNKNOWN) {
		return "";
	}
	i18n::Textdomain textdomain("widelands");
	std::vector<std::string> mods;
	if (sym.mod & KMOD_SHIFT) {
		mods.push_back(pgettext("hotkey", "Shift"));
	}
	if (sym.mod & KMOD_ALT) {
		mods.push_back(pgettext("hotkey", "Alt"));
	}
	if (sym.mod & KMOD_GUI) {
#ifdef __APPLE__
		mods.push_back(pgettext("hotkey", "Cmd"));
#else
		mods.push_back(pgettext("hotkey", "GUI"));
#endif
	}
	if (sym.mod & KMOD_CTRL) {
		mods.push_back(pgettext("hotkey", "Ctrl"));
	}

	std::string result = key_name(sym.sym);
	for (const std::string& m : mods) {
		result = (boost::format(_("%1$s+%2$s")) % m % result).str();
	}

	return rt_escape ? richtext_escape(result) : result;
}

static void init_fastplace_shortcuts(const bool force_defaults) {
	for (KeyboardShortcut k = KeyboardShortcut::kFastplace__Begin;
	     k <= KeyboardShortcut::kFastplace__End;
	     k = static_cast<KeyboardShortcut>(static_cast<uint16_t>(k) + 1)) {
		if (force_defaults) {
			shortcuts_.erase(k);
		}
		if (shortcuts_.count(k) == 0) {
			const int off =
			   static_cast<int>(k) - static_cast<int>(KeyboardShortcut::kFastplace__Begin) + 1;
			shortcuts_.emplace(
			   k, KeyboardShortcutInfo({KeyboardShortcutInfo::Scope::kGame}, keysym(SDLK_UNKNOWN),
			                           (boost::format("fastplace_%i") % off).str(), [off]() {
				                           return (boost::format(_("Fastplace #%i")) % off).str();
			                           }));
		}
	}
}

void init_shortcuts(const bool force_defaults) {
	init_fastplace_shortcuts(force_defaults);
	for (KeyboardShortcut k = KeyboardShortcut::k__Begin; k <= KeyboardShortcut::k__End;
	     k = static_cast<KeyboardShortcut>(static_cast<uint16_t>(k) + 1)) {
		shortcuts_.at(k).current_shortcut = get_default_shortcut(k);
		if (force_defaults) {
			write_shortcut(k, shortcuts_.at(k).current_shortcut);
		}
	}

#ifndef NDEBUG
	// check that the default hotkeys don't conflict with each other
	for (auto& a : shortcuts_) {
		for (auto& b : shortcuts_) {
			if (a.first != b.first && shared_scope(a.second.scopes, b.second) &&
			    matches_shortcut(a.first, b.second.default_shortcut)) {
				log_warn("The default shortcuts for %s and %s (%s, %s) collide",
				         a.second.internal_name.c_str(), b.second.internal_name.c_str(),
				         shortcut_string_for(a.first, false).c_str(),
				         shortcut_string_for(b.first, false).c_str());
			}
		}
	}
#endif

	if (force_defaults) {
		return;
	}

	Section& ss = get_config_section("keyboard_sym");
	while (Section::Value* v = ss.get_next_val()) {
		for (auto& pair : shortcuts_) {
			if (pair.second.internal_name == v->get_name()) {
				pair.second.current_shortcut.sym = v->get_int();
				break;
			}
		}
	}
	Section& sm = get_config_section("keyboard_mod");
	while (Section::Value* v = sm.get_next_val()) {
		for (auto& pair : shortcuts_) {
			if (pair.second.internal_name == v->get_name()) {
				pair.second.current_shortcut.mod = v->get_int();
				break;
			}
		}
	}
	Section& sf = get_config_section("keyboard_fastplace");
	while (Section::Value* v = sf.get_next_val()) {
		for (auto& pair : shortcuts_) {
			if (pair.second.internal_name == v->get_name()) {
				pair.second.fastplace_name = v->get_string();
				break;
			}
		}
	}
}

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
    MousewheelOption("inputqueue_fill_x", MousewheelOptionType::kBool, false, true)},
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

void set_config_directory(const std::string& userconfigdir) {
	config_dir.reset(new RealFSImpl(userconfigdir));
	config_dir->ensure_directory_exists(".");
	log_info("Set configuration file: %s/%s\n", userconfigdir.c_str(), kConfigFile.c_str());
}

void read_config() {
	assert(config_dir != nullptr);
	g_options.read(kConfigFile.c_str(), "global", *config_dir);
	get_config_section();
}

void write_config() {
	assert(config_dir != nullptr);
	try {  //  overwrite the old config file
		g_options.write(kConfigFile.c_str(), true, *config_dir);
	} catch (const std::exception& e) {
		log_warn("could not save configuration: %s\n", e.what());
	} catch (...) {
		log_warn("could not save configuration");
	}
}
