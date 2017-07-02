"
"  Textplore: A text adventure game engine.
"
"  Copyright (C) 2014-2017  Jading Tsunami
"
"  This program is free software: you can redistribute it and/or modify
"  it under the terms of the GNU General Public License as published by
"  the Free Software Foundation, either version 3 of the License, or
"  (at your option) any later version.
"
"  This program is distributed in the hope that it will be useful,
"  but WITHOUT ANY WARRANTY; without even the implied warranty of
"  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
"  GNU General Public License for more details.
"
"  You should have received a copy of the GNU General Public License
"  along with this program.  If not, see <http://www.gnu.org/licenses/>.

syn keyword statement longmessage shortmessage name
syn keyword storageclass noalias nomessage nocondition
syn keyword storageclass stopiftrue
syn keyword storageclass stopiffalse

syntax region String   start=+"+  skip=+\\"+  end=+"+
syntax match Comment /^#.*/

syn keyword identifier setting
syn keyword identifier true
syn keyword identifier false
syn keyword identifier name
syn keyword identifier longmessage
syn keyword identifier shortmessage
syn keyword identifier item
syn keyword identifier entity
syn keyword identifier exit
syn keyword identifier to
syn keyword identifier action
syn keyword identifier condition
syn keyword identifier viewable

syn keyword tag level_setting_set
syn keyword tag level_setting_clear
syn keyword tag global_setting_set
syn keyword tag global_setting_clear
syn keyword tag inventory_has
syn keyword tag inventory_has_not
syn keyword tag always

syn keyword label display
syn keyword label set_room_message
syn keyword label append_room_message
syn keyword label set_prompt
syn keyword label set_global
syn keyword label clear_global
syn keyword label set_level_setting
syn keyword label clear_level_setting
syn keyword label exit_room
syn keyword label do_nothing


