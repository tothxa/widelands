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

static const std::vector<std::string> formats_int_str = {
"AI %2d: Building a recruitment site: %s\n",
"AI %2d: Player has achieved the basic economy at %s\n",
"AI %d detected coast building: %s",
"AI %d detected fisher: %s",
"AI %d detected hunter: %s",
"AI %d detected well: %s",
"AI %d: The mine '%s' will mine multiple resources. The AI can't handle ",
"[BufferedConnection] Error when receiving data from host (error %i: %s)\n",
"[BufferedConnection] Error when sending packet to host (error %i: %s)\n",
"Couldn't get display mode for display #%d: %s\n",
" %d: AI to be dumped to %s\n",
"glewInit returns %i\nYour OpenGL installation must be __very__ broken. %s\n",
"Highlighting item %d in dropdown '%s'\n",
"[Host]: Client %i (%s) hung",
"[LAN] Error setting options for IPv%d socket, closing socket: %s.\n",
"[LAN] Error when receiving data on IPv%d socket, closing it: %s.\n",
"[LAN] Error when trying to send something over IPv%d, closing socket: %s.\n",
"[LAN] Failed to start an IPv%d socket: %s.\n",
"MO(%i): [actObject]: bad bob type %s",
"%1$dx %2$s",
"%1$ix %2$s"
};

static const uint kNFmtIntStr = formats_int_str.size();

