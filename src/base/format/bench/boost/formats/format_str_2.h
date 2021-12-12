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

#include <boost/format.hpp>
#include <vector>

static const std::vector<boost::format> formats_str_2 = {
boost::format("ABORTING. Unknown animation '%s' for '%s'\n"),
boost::format("[Client]: disconnect(%s, %s)"),
boost::format("[Client] SETTING_MAP '%s' '%s'"),
boost::format("Could not read font set '%s': %s\n"),
boost::format("Create new comment for %s: %s"),
boost::format("Deleting syncstream file %s failed: %s\n"),
boost::format("Directory %s is not writeable - next try: %s\n"),
boost::format("Duration '%s' without unit in %s's program is deprecated"),
boost::format("EncyclopediaWindow::load: could not find a suitable list entry for '%s' in '%s'"),
boost::format("Error loading %s: %s"),
boost::format("Error loading tips script for %s:\n%s\n"),
boost::format("Error rendering richtext: %s. Text is:\n%s\n"),
boost::format("Failed to read soldier capacity for building '%s': %s"),
boost::format("File '%s' for military_capacity_script for tribe '%s' does not exist"),
boost::format("Found deprecated parameter '%s' in place_immovable call, placing '%s'"),
boost::format("FXset: loading sound effect file \"%s\" failed: %s\n"),
boost::format("[Host]: File transfer checksum mismatch %s != %s\n"),
boost::format("Icon path '%s' for tab entry '%s' does not exist!"),
boost::format("Ignoring unreadable add-ons profile %s: %s"),
boost::format("Illegal data type %s in campaign data file, setting key %s to nil\n"),
boost::format("Immovable '%s', size of program '%s' seems to have changed.\n"),
boost::format("in \"%s\" section [%s] not found"),
boost::format("install addon %s: %s"),
boost::format("install translations for %s: %s"),
boost::format("InternetGaming: Received ips of the game to join: %s %s."),
boost::format("InternetGaming: Received ips of the relay to host: %s %s."),
boost::format("Invalid explicit datadir '%s': %s"),
boost::format("[LAN] Error when broadcasting on IPv6 socket to %s: %s.\n"),
boost::format("Launch Game: Error loading win condition: %s %s\n"),
boost::format("Launch Game: Exception: %s %s\n"),
boost::format("LaunchMPG: Error loading win condition: %s %s\n"),
boost::format("Line '%s': %s"),
boost::format("load_finish for %s: %s"),
boost::format("load_pointers for %s: %s"),
boost::format("No selector for fontset: %s in locale: %s. Falling back to default\n"),
boost::format("NOTE: Animation '%s' for '%s' has less than 2 images and doesn't need a "),
boost::format("Not loading add-on '%s' (%s)"),
boost::format("Parse error in program %s: %s"),
boost::format("RealFSImpl::load: error when loading \"%s\" (\"%s\"): file "),
boost::format("Received %s packet from %s"),
boost::format("Script path %s for entry %s does not exist!"),
boost::format("set_login as '%s': access denied (%s)"),
boost::format("%s has no program '%s'"),
boost::format("Skip add-on %s because: %s"),
boost::format("[%s]: missing boolean key '%s'"),
boost::format("[%s]: missing integer key '%s'"),
boost::format("[%s]: missing key '%s'"),
boost::format("[%s]: missing natural key '%s'"),
boost::format("[%s]: missing positive key '%s'"),
boost::format(" · '%s': %s"),
boost::format("%s %s"),
boost::format("%s: %s"),
boost::format("[%s] %s"),
boost::format("%s: '%s' is not a boolean value"),
boost::format("%s: '%s' is not an integer"),
boost::format("%s: '%s' is not a Vector2i"),
boost::format("%s: '%s' is not natural"),
boost::format("%s: '%s' is not positive"),
boost::format("%s: '%s' is out of range"),
boost::format("[%s] %s\n"),
boost::format("Terrain %s defined by add-on %s which is not enabled"),
boost::format("Unable to show window for building '%s', type '%s'.\n"),
boost::format("Unexpected registry caller identifier '%s' for offical/scenario unit %s"),
boost::format("Unknown animation '%s' for bob '%s', using main animation instead.\n"),
boost::format("Unknown animation '%s' for immovable '%s', using main animation instead.\n"),
boost::format("upload addon %s: %s"),
boost::format("WLApplication::cleanup_ai_files: File %s couldn't be deleted: %s\n"),
boost::format("WLApplication::cleanup_replays: File %s couldn't be deleted: %s\n"),
boost::format("WLApplication::cleanup_temp_backups: File %s couldn't be deleted: %s\n"),
boost::format("WLApplication::cleanup_temp_files: File %s couldn't be deleted: %s\n"),
boost::format("worker type %s needs \"%s\" to be built but that is neither "),
boost::format("Write to %s (%s) failed"),
boost::format("Wrong argument type: expected %s, found %s"),
boost::format("Wrong helptext data type for '%s', category '%s'. Expecting a table or a "),
boost::format("%1$s %2$s"),
boost::format("%1$s & %2$s"),
boost::format("%1$s (%2$s)"),
boost::format("%1$s · %2$s"),
boost::format("%1$s%2$s"),
boost::format("%1$s+%2$s"),
boost::format("%1$s, %2$s"),
boost::format("%1$s: %2$s"),
boost::format("%1$s; %2$s"),
boost::format("%1$s, %2$s (missing)"),
boost::format("%1$s and %2$s"),
boost::format("‘%1$s’ commented on version %2$s:"),
boost::format("%1$s (edited on %2$s)"),
boost::format("%1$s from: %2$s"),
boost::format("%1$s had %2$s."),
boost::format("%1$s has %2$s at the moment."),
boost::format("%1$s has been King of the Hill for %2$s!"),
boost::format("%1$s has been replaced with %2$s"),
boost::format("%1$s has left the game (%2$s)"),
boost::format("%1$s or %2$s"),
boost::format("%1$s owns %2$s."),
boost::format("· ‘%1$s’ requires ‘%2$s’ which could not be found"),
boost::format("· ‘%1$s’ requires ‘%2$s’ which is disabled"),
boost::format("· ‘%1$s’ requires ‘%2$s’ which is listed below the requiring add-on"),
boost::format("by %1$s (uploaded by %2$s)"),
boost::format("Client %1$s did not answer for more than %2$s."),
boost::format("Completed %1$s because %2$s"),
boost::format("Completed transfer of file %1$s to %2$s"),
boost::format("Did not start %1$s because %2$s"),
boost::format("Requires a Widelands version of at least %1$s and at most %2$s."),
boost::format("Skipped %1$s because %2$s"),
boost::format("Started to send file %1$s to %2$s!"),
boost::format("The winner is %1$s with %2$s."),
boost::format("Version %1$s (%2$s)")
};

static const uint kNFmtStr2 = formats_str_2.size();

