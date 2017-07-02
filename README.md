# textplore

A simple text adventure game engine.

# Installation

Run `make` to build the project.

ncurses is required.

Run `textplore` and play a short sample level.

# Usage

Create a map via the Level Creation instructions. Play it from your terminal via:

`textplore <filename>`


# Level Creation

See the sample file, `levels.txt` for a basic map that can be used as a template.


## Basics


The game is split into LEVELs.

There are GLOBAL settings (boolean) and LEVEL settings (boolean).

The player has an inventory containing ITEMs.

ITEMs are ENTITIES which are takeable.

ENTITIES have associated ACTIONS.

ACTIONS can have CONDITIONS which evaluate settings or inventory items and can:

* Display a message.
* Set the room message.
* Set/clear a global/level setting.

CONDITIONS can be linked via STOPIFTRUE or STOPIFFALSE clauses. This allows linking conditions, breaking the chain if any are not met.

Each CONDITION in a chain will be evaluated and its true/false actions taken until the chain ends.


ROOMs contain ENTITIES. They also display a MESSAGE. When visited again, they display a SHORT MESSAGE instead. The long message can be viewed with the "look at the room" or "look room" command.

ROOMs are navigated to/from via EXITs. EXITs are a combination of a NAME and a linked ROOM. Exits may be blocked by a CONDITION.

VIEWABLE objects are provided as a shorthand for objects that can be looked at but not interacted with. They are no different than providing an entity with no associated action, but are shorter to type.

