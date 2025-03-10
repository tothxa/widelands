/*
 * Copyright (C) 2007-2025 by the Widelands Development Team
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

#ifndef WL_LOGIC_REPLAY_H
#define WL_LOGIC_REPLAY_H

/**
 * Allow players to watch previous game in a platform-independent way.
 * Also useful as a debugging aid.
 *
 * A game replay consists of a savegame plus a log-file of subsequent
 * playercommands.
 */

#include <string>

#include "base/crypto.h"
#include "base/times.h"

class StreamRead;
class StreamWrite;

namespace Widelands {
struct Command;
class Game;
class PlayerCommand;

/**
 * Read game replays from disk.
 */
class ReplayReader {
public:
	ReplayReader(Game& game, const std::string& filename);
	~ReplayReader();

	Command* get_next_command(const Time& time);
	bool end_of_replay();

private:
	StreamRead* cmdlog_;

	Time replaytime_;
};

/**
 * Write game replays to disk.
 */
class ReplayWriter {
public:
	ReplayWriter(Game&, const std::string& filename);
	~ReplayWriter();

	void send_player_command(PlayerCommand*);
	void send_sync(const crypto::MD5Checksum& hash);

private:
	Game& game_;
	StreamWrite* cmdlog_;
	std::string filename_;
};

/** Extract the savegame from a replay to a temporary file. */
struct ReplayfileSavegameExtractor {
	explicit ReplayfileSavegameExtractor(const std::string& gamefilename);
	~ReplayfileSavegameExtractor();

	[[nodiscard]] inline const std::string& file() const {
		return temp_file_.empty() ? source_file_ : temp_file_;
	}

	[[nodiscard]] bool is_replay() const {
		return is_replay_;
	}

private:
	const std::string& source_file_;
	std::string temp_file_;
	bool is_replay_{false};
};

}  // namespace Widelands

#endif  // end of include guard: WL_LOGIC_REPLAY_H
