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

#include <string>
#include <vector>

static const std::vector<std::string> formats_str_2 = {
"ABORTING. Unknown animation '%s' for '%s'\n",
"[Client]: disconnect(%s, %s)",
"[Client] SETTING_MAP '%s' '%s'",
"Could not read font set '%s': %s\n",
"Create new comment for %s: %s",
"Deleting syncstream file %s failed: %s\n",
"Directory %s is not writeable - next try: %s\n",
"Duration '%s' without unit in %s's program is deprecated",
"EncyclopediaWindow::load: could not find a suitable list entry for '%s' in '%s'",
"Error loading %s: %s",
"Error loading tips script for %s:\n%s\n",
"Error rendering richtext: %s. Text is:\n%s\n",
"Failed to read soldier capacity for building '%s': %s",
"File '%s' for military_capacity_script for tribe '%s' does not exist",
"Found deprecated parameter '%s' in place_immovable call, placing '%s'",
"FXset: loading sound effect file \"%s\" failed: %s\n",
"[Host]: File transfer checksum mismatch %s != %s\n",
"Icon path '%s' for tab entry '%s' does not exist!",
"Ignoring unreadable add-ons profile %s: %s",
"Illegal data type %s in campaign data file, setting key %s to nil\n",
"Immovable '%s', size of program '%s' seems to have changed.\n",
"in \"%s\" section [%s] not found",
"install addon %s: %s",
"install translations for %s: %s",
"InternetGaming: Received ips of the game to join: %s %s.",
"InternetGaming: Received ips of the relay to host: %s %s.",
"Invalid explicit datadir '%s': %s",
"[LAN] Error when broadcasting on IPv6 socket to %s: %s.\n",
"Launch Game: Error loading win condition: %s %s\n",
"Launch Game: Exception: %s %s\n",
"LaunchMPG: Error loading win condition: %s %s\n",
"Line '%s': %s",
"load_finish for %s: %s",
"load_pointers for %s: %s",
"No selector for fontset: %s in locale: %s. Falling back to default\n",
"NOTE: Animation '%s' for '%s' has less than 2 images and doesn't need a ",
"Not loading add-on '%s' (%s)",
"Parse error in program %s: %s",
"RealFSImpl::load: error when loading \"%s\" (\"%s\"): file ",
"Received %s packet from %s",
"Script path %s for entry %s does not exist!",
"set_login as '%s': access denied (%s)",
"%s has no program '%s'",
"Skip add-on %s because: %s",
"[%s]: missing boolean key '%s'",
"[%s]: missing integer key '%s'",
"[%s]: missing key '%s'",
"[%s]: missing natural key '%s'",
"[%s]: missing positive key '%s'",
" · '%s': %s",
"%s %s",
"%s: %s",
"[%s] %s",
"%s: '%s' is not a boolean value",
"%s: '%s' is not an integer",
"%s: '%s' is not a Vector2i",
"%s: '%s' is not natural",
"%s: '%s' is not positive",
"%s: '%s' is out of range",
"[%s] %s\n",
"Terrain %s defined by add-on %s which is not enabled",
"Unable to show window for building '%s', type '%s'.\n",
"Unexpected registry caller identifier '%s' for offical/scenario unit %s",
"Unknown animation '%s' for bob '%s', using main animation instead.\n",
"Unknown animation '%s' for immovable '%s', using main animation instead.\n",
"upload addon %s: %s",
"WLApplication::cleanup_ai_files: File %s couldn't be deleted: %s\n",
"WLApplication::cleanup_replays: File %s couldn't be deleted: %s\n",
"WLApplication::cleanup_temp_backups: File %s couldn't be deleted: %s\n",
"WLApplication::cleanup_temp_files: File %s couldn't be deleted: %s\n",
"worker type %s needs \"%s\" to be built but that is neither ",
"Write to %s (%s) failed",
"Wrong argument type: expected %s, found %s",
"Wrong helptext data type for '%s', category '%s'. Expecting a table or a ",
"%1$s %2$s",
"%1$s & %2$s",
"%1$s (%2$s)",
"%1$s · %2$s",
"%1$s%2$s",
"%1$s+%2$s",
"%1$s, %2$s",
"%1$s: %2$s",
"%1$s; %2$s",
"%1$s, %2$s (missing)",
"%1$s and %2$s",
"‘%1$s’ commented on version %2$s:",
"%1$s (edited on %2$s)",
"%1$s from: %2$s",
"%1$s had %2$s.",
"%1$s has %2$s at the moment.",
"%1$s has been King of the Hill for %2$s!",
"%1$s has been replaced with %2$s",
"%1$s has left the game (%2$s)",
"%1$s or %2$s",
"%1$s owns %2$s.",
"· ‘%1$s’ requires ‘%2$s’ which could not be found",
"· ‘%1$s’ requires ‘%2$s’ which is disabled",
"· ‘%1$s’ requires ‘%2$s’ which is listed below the requiring add-on",
"by %1$s (uploaded by %2$s)",
"Client %1$s did not answer for more than %2$s.",
"Completed %1$s because %2$s",
"Completed transfer of file %1$s to %2$s",
"Did not start %1$s because %2$s",
"Requires a Widelands version of at least %1$s and at most %2$s.",
"Skipped %1$s because %2$s",
"Started to send file %1$s to %2$s!",
"The winner is %1$s with %2$s.",
"Version %1$s (%2$s)"
};

static const uint kNFmtStr2 = formats_str_2.size();

