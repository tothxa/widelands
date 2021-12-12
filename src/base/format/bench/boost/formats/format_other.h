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

#include <base/macros.h>

static const std::vector<boost::format> formats_char = {
boost::format("expected numerical argument after '.', found '%c'"),
boost::format("Invalid character ‘%c’"),
boost::format("invalid format type character '%c' after '%%l'"),
boost::format("Repeated flag '%c'"),
boost::format("unrecognized format type character '%c'")
};
static const uint kNFmtChar = formats_char.size();

/******************** float, ... ********************/
static const std::vector<boost::format> formats_float = {
boost::format("%2.1f%%"),
boost::format("%.2f GB"),
boost::format("%.2f kB"),
boost::format("%.2f MB"),
boost::format("Animation sound effect priority '%.2f' without percent symbol is deprecated"),
boost::format("Fetching add-ons (%.1f%%)"),
boost::format("Floating point value too large: %f")
};
static const uint kNFmtFloat = formats_float.size();

static const std::vector<boost::format> formats_float_uint = {
boost::format("Average rating: %1$.3f (%2$u vote)"),
boost::format("Average rating: %1$.3f (%2$u votes)")
};
static const uint kNFmtFloatUint = formats_float_uint.size();

/******************** int, ... ********************/
static const std::vector<boost::format> formats_int_3 = {
boost::format("%2d: Team changed %d -> %d\n"),
boost::format("[Host]: Client %i reports time %i (networktime = %i) during hang"),
boost::format("SDL_mixer version: %d.%d.%d\n"),
boost::format("SDL version: %d.%d.%d\n")
};
static const uint kNFmtInt3 = formats_int_3.size();

static const std::vector<boost::format> formats_int_4 = {
boost::format("%2d: Team changed for player %d: %d -> %d\n")
};
static const uint kNFmtInt4 = formats_int_4.size();

/******************** % - ptr ********************/
static const std::vector<boost::format> formats_percent = {
boost::format("%%c can not have additional specifiers"),
boost::format("expected format sequence after '%%', found \\0"),
boost::format("Font renderer: Do not use width > 100%%\n"),
boost::format("%%s can not have '0' specifier"),
boost::format("%%s can not have '+' specifier")
};
static const uint kNFmtPercent = formats_percent.size();

static const std::vector<boost::format> formats_ptr = {
boost::format(" -> %p"),
boost::format("%p"),
boost::format("   queue_           %7p"),
boost::format("remove_request(%p) not in list\n"),
boost::format("     Setting        %7p"),
boost::format("   Setting          %7p"),
boost::format("   settings_        %7p")
};
static const uint kNFmtPtr = formats_ptr.size();

/******************** str, ... ********************/
static const std::vector<boost::format> formats_str_4 = {
boost::format("[Host]: Received system command %s(%s,%s,%s) from client"),
boost::format("The default shortcuts for %s and %s (%s, %s) collide"),
boost::format("%1$s, %2$s (expected version %3$s, found %4$s)"),
boost::format("Change is rapid with %1$s/%2$s and smooth with %3$s/%4$s.")
};
static const uint kNFmtStr4 = formats_str_4.size();

static const std::vector<boost::format> formats_str_float_int_2 = {
boost::format("%1$s had %2$3.0f%% of the land (%3$i of %4$i)."),
boost::format("%1$s has %2$3.0f%% of the land (%3$i of %4$i).")
};
static const uint kNFmtStrFloatInt2 = formats_str_float_int_2.size();

/******************** str, int, ... ********************/
static const std::vector<boost::format> formats_str_int = {
boost::format("[BufferedConnection] Closing network socket connected to %s:%i."),
boost::format("Graphics: %s is %d\n"),
boost::format("%1$s was the first to reach Island number %2$i."),
boost::format("%1$s was the fourth to reach Island number %2$i."),
boost::format("%1$s was the second to reach Island number %2$i."),
boost::format("%1$s was the third to reach Island number %2$i.")
};
static const uint kNFmtStrInt = formats_str_int.size();

static const std::vector<boost::format> formats_str_int_2 = {
boost::format("AI check_militarysites: Too many %s: %d, ai limit: %d\n"),
boost::format("AI check_productionsites: Too many %s: %d, ai limit: %d\n"),
boost::format("Found a stopped %s at (%i, %i) in the "),
boost::format("Saving game: %1$s (%2$d/%3$d)"),
boost::format("Saving map: %1$s (%2$d/%3$d)")
};
static const uint kNFmtStrInt2 = formats_str_int_2.size();

static const std::vector<boost::format> formats_str_int_3 = {
boost::format("Message: adding %s for player %i at (%d, %d)\n"),
boost::format("%1$s (%2$i P) x %3$i = %4$i P")
};
static const uint kNFmtStrInt3 = formats_str_int_3.size();

/******************** str, uint, ... ********************/
static const std::vector<boost::format> formats_str_uint = {
boost::format("[BufferedConnection] Trying to connect to %s:%u ... "),
boost::format("[BufferedConnection] Trying to connect to %s:%u failed!"),
boost::format("%s %u has a next_spawn time for nonexistent "),
boost::format("%s %u has a next_spawn time for worker type "),
boost::format("%1$s+%2$u"),
boost::format("%1$s.%2$u"),
boost::format("Version: %1$s+%2$u")
};
static const uint kNFmtStrUint = formats_str_uint.size();

static const std::vector<boost::format> formats_str_uint_2 = {
boost::format("Locking mutex %s took %ums (%u function calls)"),
boost::format("· %s: carrying %u items (capacity %u)\n"),
boost::format("%s (%u) has no InputQueue for %u"),
boost::format("Terrain %s: dither layer disambiguator %u exceeds maximum of %u"),
boost::format("saved on %1$s %2$u, %3$u"),
boost::format("The %1$s needs to be version %2$u.%3$u or newer.")
};
static const uint kNFmtStrUint2 = formats_str_uint_2.size();

static const std::vector<boost::format> formats_str_uint_int = {
boost::format("Loading game: %1$s (%2$u/%3$d)"),
boost::format("Loading map: %1$s (%2$u/%3$d)")
};
static const uint kNFmtStrUintInt = formats_str_uint_int.size();

static const std::vector<boost::format> formats_str_uint_str = {
boost::format("Could not resolve network name '%s:%u' to %s-address\n"),
boost::format("Resolved network name '%s:%u' to %s"),
boost::format("%s (%u) (building %s) has no WorkersQueues")
};
static const uint kNFmtStrUintStr = formats_str_uint_str.size();

/******************** uint, ... ********************/
static const std::vector<boost::format> formats_uint_3 = {
boost::format("The %1$u version needs to be version %2$u.%3$u or newer.")
};
static const uint kNFmtUint3 = formats_uint_3.size();

static const std::vector<boost::format> formats_uint_str_2 = {
boost::format("Delete comment #%" PRIuS " for %s: %s"),
boost::format("Edit comment #%" PRIuS " for %s: %s"),
boost::format("Player %1$u (%2$s): %3$s")
};
static const uint kNFmtUintStr2 = formats_uint_str_2.size();

/******************** wildcard ********************/
static const std::vector<boost::format> formats_wild_3 = {
boost::format("%1% %2%, %3%"),
boost::format("%1%(+%2%) soldier (+%3%)"),
boost::format("%1%(+%2%) soldiers (+%3%)"),
boost::format("%1% unit of %2% has been brought to your ‘%3%’ building."),
boost::format("%1% units of %2% have been brought to your ‘%3%’ building.")
};
static const uint kNFmtWild3 = formats_wild_3.size();

static const std::vector<boost::format> formats_wild_4 = {
boost::format("Loading %1%: %2% (%3%/%4%)")
};
static const uint kNFmtWild4 = formats_wild_4.size();

