/*
 * Copyright (C) 2006-2010 by the Widelands Development Team
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

#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "wexception.h"

#include <map>
#include <stdint.h>
#include <string>

#include <lua.hpp>

#include "logic/widelands_filewrite.h"
#include "logic/widelands_fileread.h"

namespace Widelands {
	struct Editor_Game_Base;
	struct Game;
	struct Map_Map_Object_Loader;
	struct Map_Map_Object_Saver;
	struct Player;
}

struct LuaError : public _wexception {
	LuaError(const std::string & reason) : wexception("%s", reason.c_str()) {}
};
struct LuaValueError : public LuaError {
	LuaValueError(const std::string & wanted) :
		LuaError("Variable not of expected type: " + wanted)
	{}
};
struct LuaTableKeyError : public LuaError {
	LuaTableKeyError(const std::string & wanted) :
		LuaError(wanted + " is not a field in this table.")
	{}
};
struct LuaScriptNotExistingError : public LuaError {
	LuaScriptNotExistingError(std::string ns, std::string name) :
		LuaError("The script '" + ns + ":" + name + "' was not found!") {}
};

/**
 * Easy handling of LuaCoroutines
 */
struct LuaCoroutine {
	virtual ~LuaCoroutine() {}

	enum {
		DONE = 0,
		YIELDED = LUA_YIELD,
	};

	virtual int get_status() = 0;
	virtual int resume(uint32_t * = 0) = 0;

	virtual void push_arg(const Widelands::Player *) = 0;
	virtual void push_arg(const Widelands::Coords &) = 0;
};

/*
 * Easy handling of return values from Wideland's Lua configurations
 * scripts: they return a Lua table with (string,value) pairs.
 */
struct LuaTable {
	virtual ~LuaTable() {}

	virtual std::string get_string(std::string) = 0;
	virtual LuaCoroutine * get_coroutine(std::string) = 0;
};

/*
 * This is the thin class that is used to execute code
 */
typedef std::map<std::string, std::string> ScriptContainer;
struct LuaInterface {
	virtual ~LuaInterface() {}

	virtual void interpret_string(std::string) = 0;
	virtual const std::string & get_last_error() const = 0;

	virtual void register_scripts
		(FileSystem &, std::string, std::string = "scripting") = 0;
	virtual ScriptContainer & get_scripts_for(std::string) = 0;

	virtual std::unique_ptr<LuaTable> run_script(std::string, std::string) = 0;
	virtual std::unique_ptr<LuaTable> run_script
			(FileSystem &, std::string, std::string) = 0;

	virtual std::unique_ptr<LuaTable> get_hook(std::string name) = 0;
};

struct LuaGameInterface : public virtual LuaInterface {
	virtual LuaCoroutine * read_coroutine
		(Widelands::FileRead &, Widelands::Map_Map_Object_Loader &, uint32_t) = 0;
	virtual uint32_t write_coroutine
		(Widelands::FileWrite &, Widelands::Map_Map_Object_Saver &, LuaCoroutine *) = 0;

	virtual void read_global_env
		(Widelands::FileRead &, Widelands::Map_Map_Object_Loader &, uint32_t) = 0;
	virtual uint32_t write_global_env
		(Widelands::FileWrite &, Widelands::Map_Map_Object_Saver &) = 0;
};

LuaGameInterface * create_LuaGameInterface(Widelands::Game *);
LuaInterface * create_LuaEditorInterface(Widelands::Editor_Game_Base *);
LuaInterface * create_LuaInterface();

#endif
