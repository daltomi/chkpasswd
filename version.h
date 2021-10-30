/*
	Copyright © 2015,2019 Daniel T. Borelli <danieltborelli@gmail.com>

	This file is part of chkpasswd.

	chkpasswd is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	chkpasswd is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with chkpasswd.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define macro_string(x) #x

#define macrostr(x) macro_string(x)

#define VERSION "1.1"

#define APP "chkpasswd v" VERSION

#define INFO APP "," macrostr(__FILE__) ":"  macrostr(__LINE__) " "



