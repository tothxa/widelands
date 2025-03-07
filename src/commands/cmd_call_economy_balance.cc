/*
 * Copyright (C) 2002-2025 by the Widelands Development Team
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

#include "commands/cmd_call_economy_balance.h"

#include "base/wexception.h"
#include "economy/economy.h"
#include "io/fileread.h"
#include "io/filewrite.h"
#include "logic/game.h"
#include "logic/player.h"
#include "map_io/map_object_loader.h"
#include "map_io/map_object_saver.h"

namespace Widelands {

CmdCallEconomyBalance::CmdCallEconomyBalance(const Time& starttime,
                                             Flag* representative_flag,
                                             WareWorker type,
                                             uint32_t timerid)
   : GameLogicCommand(starttime), flag_(representative_flag), type_(type), timerid_(timerid) {
	assert(representative_flag != nullptr);
	assert(representative_flag->get_economy(type) != nullptr);
}

/**
 * Called by Cmd_Queue as requested by start_request_timer().
 * Call economy functions to balance supply and request.
 */
void CmdCallEconomyBalance::execute(Game& game) {
	if (Flag* const flag = flag_.get(game)) {
		flag->get_economy(type_)->balance(timerid_);
	}
}

constexpr uint16_t kCurrentPacketVersion = 4;

/**
 * Read and write
 */
void CmdCallEconomyBalance::read(FileRead& fr, EditorGameBase& egbase, MapObjectLoader& mol) {
	try {
		uint16_t const packet_version = fr.unsigned_16();
		if (packet_version == kCurrentPacketVersion) {
			GameLogicCommand::read(fr, egbase, mol);
			uint32_t serial = fr.unsigned_32();
			if (serial != 0u) {
				flag_ = &mol.get<Flag>(serial);
			}
			timerid_ = fr.unsigned_32();
			type_ = fr.unsigned_8() != 0u ? wwWORKER : wwWARE;
		} else {
			throw UnhandledVersionError(
			   "CmdCallEconomyBalance", packet_version, kCurrentPacketVersion);
		}
	} catch (const WException& e) {
		throw wexception("call economy balance: %s", e.what());
	}
}
void CmdCallEconomyBalance::write(FileWrite& fw, EditorGameBase& egbase, MapObjectSaver& mos) {
	fw.unsigned_16(kCurrentPacketVersion);

	// Write Base Commands
	GameLogicCommand::write(fw, egbase, mos);
	if (Flag* const flag = flag_.get(egbase)) {
		fw.unsigned_32(mos.get_object_file_index(*flag));
	} else {
		fw.unsigned_32(0);
	}
	fw.unsigned_32(timerid_);
	fw.unsigned_8(type_);
}
}  // namespace Widelands
