/*
 * Copyright (C) 2008-2025 by the Widelands Development Team
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

#ifndef WL_LOGIC_MAP_OBJECTS_TRIBES_SOLDIERCONTROL_H
#define WL_LOGIC_MAP_OBJECTS_TRIBES_SOLDIERCONTROL_H

#include "logic/widelands.h"

namespace Widelands {

class EditorGameBase;
class Soldier;
class TribeDescr;

/**
 * This interface is implemented by buildings that explicitly house soldiers.
 *
 * It is used to implement queries for building UI and to implement commands
 * that change the soldiers stationed in a building.
 *
 * Soldiers are \em stationed in a building if that building is their
 * \ref Worker::location. Stationed soldiers are \em present in a building
 * if their current position is inside the building. So for a TrainingSite,
 * the two concepts are equal. However, they're different for a MilitarySite,
 * where soldiers can be outside in combat.
 */
class SoldierControl {
public:
	/**
	 * \return a list of soldiers that are currently present in the building.
	 */
	[[nodiscard]] virtual std::vector<Soldier*> present_soldiers() const = 0;

	/**
	 * \return a list of soldiers that are currently stationed in the building.
	 */
	[[nodiscard]] virtual std::vector<Soldier*> stationed_soldiers() const = 0;

	/**
	 * \return a list of soldiers that are currently stationed in or coming to the building.
	 */
	[[nodiscard]] virtual std::vector<Soldier*> associated_soldiers() const = 0;

	/**
	 * \return the minimum number of soldiers that this building can be
	 * configured to hold.
	 */
	[[nodiscard]] virtual Quantity min_soldier_capacity() const = 0;

	/**
	 * \return the maximum number of soldiers that this building can be
	 * configured to hold.
	 */
	[[nodiscard]] virtual Quantity max_soldier_capacity() const = 0;

	/**
	 * \return the number of soldiers this building is configured to hold
	 * right now.
	 */
	[[nodiscard]] virtual Quantity soldier_capacity() const = 0;

	/**
	 * \return a richtext string for the building's status line.
	 */
	[[nodiscard]] std::string get_status_string(const TribeDescr& tribe,
	                                            SoldierPreference pref) const;

	/**
	 * Sets the capacity for soldiers of this building.
	 *
	 * New soldiers will be requested and old soldiers will be evicted
	 * as necessary.
	 */
	virtual void set_soldier_capacity(Quantity capacity) = 0;

	/**
	 * Evict the given soldier from the building immediately,
	 * without changing the building's capacity.
	 *
	 * \note This has no effect if the soldier is currently involved in a battle
	 * or otherwise blocked from leaving the building.
	 */
	virtual void drop_soldier(Soldier&) = 0;

	/**
	 * Add a new soldier into this site. Returns -1 if there is no space
	 * for him, 0 on success
	 */
	virtual int incorporate_soldier(EditorGameBase&, Soldier&) = 0;

	/**
	 * Remove a soldier from the internal list. Most SoldierControls will be
	 * informed by the soldier when it is removed, but WareHouses for example
	 * will not.
	 */
	virtual int outcorporate_soldier(Soldier&) {
		return 0;
	}

	virtual ~SoldierControl() = default;
};
}  // namespace Widelands

#endif  // end of include guard: WL_LOGIC_MAP_OBJECTS_TRIBES_SOLDIERCONTROL_H
