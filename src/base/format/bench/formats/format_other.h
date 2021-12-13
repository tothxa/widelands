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

#include <base/macros.h>

static const std::vector<std::string> formats_char = {
"expected numerical argument after '.', found '%c'",
"Invalid character ‘%c’",
"invalid format type character '%c' after '%%l'",
"Repeated flag '%c'",
"unrecognized format type character '%c'"
};
static const uint kNFmtChar = formats_char.size();

/******************** float, ... ********************/
static const std::vector<std::string> formats_float = {
"%2.1f%%",
"%.2f GB",
"%.2f kB",
"%.2f MB",
"Animation sound effect priority '%.2f' without percent symbol is deprecated",
"Fetching add-ons (%.1f%%)",
"Floating point value too large: %f"
};
static const uint kNFmtFloat = formats_float.size();

static const std::vector<std::string> formats_float_uint = {
"Average rating: %1$.3f (%2$u vote)",
"Average rating: %1$.3f (%2$u votes)"
};
static const uint kNFmtFloatUint = formats_float_uint.size();

/******************** int, ... ********************/
static const std::vector<std::string> formats_int_3 = {
"%2d: Team changed %d -> %d\n",
"[Host]: Client %i reports time %i (networktime = %i) during hang",
"SDL_mixer version: %d.%d.%d\n",
"SDL version: %d.%d.%d\n"
};
static const uint kNFmtInt3 = formats_int_3.size();

static const std::vector<std::string> formats_int_4 = {
"%2d: Team changed for player %d: %d -> %d\n"
};
static const uint kNFmtInt4 = formats_int_4.size();

/******************** % - ptr ********************/
static const std::vector<std::string> formats_percent = {
"%%c can not have additional specifiers",
"expected format sequence after '%%', found \\0",
"Font renderer: Do not use width > 100%%\n",
"%%s can not have '0' specifier",
"%%s can not have '+' specifier"
};
static const uint kNFmtPercent = formats_percent.size();

static const std::vector<std::string> formats_ptr = {
" -> %p",
"%p",
"   queue_           %7p",
"remove_request(%p) not in list\n",
"     Setting        %7p",
"   Setting          %7p",
"   settings_        %7p"
};
static const uint kNFmtPtr = formats_ptr.size();

/******************** str, ... ********************/
static const std::vector<std::string> formats_str_4 = {
"[Host]: Received system command %s(%s,%s,%s) from client",
"The default shortcuts for %s and %s (%s, %s) collide",
"%1$s, %2$s (expected version %3$s, found %4$s)",
"Change is rapid with %1$s/%2$s and smooth with %3$s/%4$s."
};
static const uint kNFmtStr4 = formats_str_4.size();

static const std::vector<std::string> formats_str_float_int_2 = {
"%1$s had %2$3.0f%% of the land (%3$i of %4$i).",
"%1$s has %2$3.0f%% of the land (%3$i of %4$i)."
};
static const uint kNFmtStrFloatInt2 = formats_str_float_int_2.size();

/******************** str, int, ... ********************/
static const std::vector<std::string> formats_str_int = {
"[BufferedConnection] Closing network socket connected to %s:%i.",
"Graphics: %s is %d\n",
"%1$s was the first to reach Island number %2$i.",
"%1$s was the fourth to reach Island number %2$i.",
"%1$s was the second to reach Island number %2$i.",
"%1$s was the third to reach Island number %2$i."
};
static const uint kNFmtStrInt = formats_str_int.size();

static const std::vector<std::string> formats_str_int_2 = {
"AI check_militarysites: Too many %s: %d, ai limit: %d\n",
"AI check_productionsites: Too many %s: %d, ai limit: %d\n",
"Found a stopped %s at (%i, %i) in the ",
"Saving game: %1$s (%2$d/%3$d)",
"Saving map: %1$s (%2$d/%3$d)"
};
static const uint kNFmtStrInt2 = formats_str_int_2.size();

static const std::vector<std::string> formats_str_int_3 = {
"Message: adding %s for player %i at (%d, %d)\n",
"%1$s (%2$i P) x %3$i = %4$i P"
};
static const uint kNFmtStrInt3 = formats_str_int_3.size();

/******************** str, uint, ... ********************/
static const std::vector<std::string> formats_str_uint = {
"[BufferedConnection] Trying to connect to %s:%u ... ",
"[BufferedConnection] Trying to connect to %s:%u failed!",
"%s %u has a next_spawn time for nonexistent ",
"%s %u has a next_spawn time for worker type ",
"%1$s+%2$u",
"%1$s.%2$u",
"Version: %1$s+%2$u"
};
static const uint kNFmtStrUint = formats_str_uint.size();

static const std::vector<std::string> formats_str_uint_2 = {
"Locking mutex %s took %ums (%u function calls)",
"· %s: carrying %u items (capacity %u)\n",
"%s (%u) has no InputQueue for %u",
"Terrain %s: dither layer disambiguator %u exceeds maximum of %u",
"saved on %1$s %2$u, %3$u",
"The %1$s needs to be version %2$u.%3$u or newer."
};
static const uint kNFmtStrUint2 = formats_str_uint_2.size();

static const std::vector<std::string> formats_str_uint_int = {
"Loading game: %1$s (%2$u/%3$d)",
"Loading map: %1$s (%2$u/%3$d)"
};
static const uint kNFmtStrUintInt = formats_str_uint_int.size();

static const std::vector<std::string> formats_str_uint_str = {
"Could not resolve network name '%s:%u' to %s-address\n",
"Resolved network name '%s:%u' to %s",
"%s (%u) (building %s) has no WorkersQueues"
};
static const uint kNFmtStrUintStr = formats_str_uint_str.size();

/******************** uint, ... ********************/
static const std::vector<std::string> formats_uint_3 = {
"The %1$u version needs to be version %2$u.%3$u or newer."
};
static const uint kNFmtUint3 = formats_uint_3.size();

static const std::vector<std::string> formats_uint_str_2 = {
"Delete comment #%" PRIuS " for %s: %s",
"Edit comment #%" PRIuS " for %s: %s",
"Player %1$u (%2$s): %3$s"
};
static const uint kNFmtUintStr2 = formats_uint_str_2.size();

/******************** wildcard ********************/
static const std::vector<std::string> formats_wild_3 = {
"%1% %2%, %3%",
"%1%(+%2%) soldier (+%3%)",
"%1%(+%2%) soldiers (+%3%)",
"%1% unit of %2% has been brought to your ‘%3%’ building.",
"%1% units of %2% have been brought to your ‘%3%’ building."
};
static const uint kNFmtWild3 = formats_wild_3.size();

static const std::vector<std::string> formats_wild_4 = {
"Loading %1%: %2% (%3%/%4%)"
};
static const uint kNFmtWild4 = formats_wild_4.size();

