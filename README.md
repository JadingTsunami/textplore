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


ROOMs contain ENTITIES. They also display a LONG MESSAGE. When visited again, they display a SHORT MESSAGE instead. The long message can be viewed with the "look at the room" or "look room" command.

ROOMs are navigated to/from via EXITs. EXITs are a combination of a NAME and a linked ROOM. Exits may be blocked by a CONDITION.

VIEWABLE objects are provided as a shorthand for objects that can be looked at but not interacted with. They are no different than providing an entity with no associated action, but are shorter to type.

## Input File Specification

To read this specification, apply the following rules:

* _Spec_: Lines that start with _Spec_ refer to a text description of the field. They should not be typed and serve as an illustration only.
* `#`: Lines starting with a hash (`#`) are comments and exist as explanatory detail. They don't need to be included in the level file.
* `Text`: Undecorated monospace font refer to text you should type in verbatim.
    * Example: `[LEVEL]`
* `<>`: Monospace font surrounded by angle brackets (`<>`) should be replaced by whatever value you want. Remove the angle brackets when inputting the value you want. Do not replace any other surrounding characters such as quotation marks.
    * Example: `longmessage "<your-message-here>"`
    * Could become: `longmessage "You are in a forest. You see a bear here."`
* `{}`: Monospace font surrounded by curly brackets (`{}`) containing elements separated by pipe characters (`|`) indicate you should select the desired element and replace this block with that element.
    * Example: `setting "<setting-name>" {true|false}`
    * Could become: `setting "generator powered up" false`
* `!{}`: Monospace font surrounded by curly brackets (`{}`) prepended with a bang symbol (`!`) indicate optional items.
* `_text_`: Monospace font surrounded by underlines (`_`) indicate blocks that are replaced by a separate element specification.
* `...`: Ellipses indicate a section that may be repeated to any length.

### Sections

Sections are marked in the input file with section/endsection tags.

Each section tag must start on its own line.

#### Global

Contains global settings (persist across levels).

```
[GLOBAL]
# (Optional) settings
[ENDGLOBAL]
```

#### Level

Contains level settings (exist only for a single level) and rooms.

```
[LEVEL]
# (Optional) settings

# Rooms
[ENDLEVEL]
```

#### Room

Named object containing a long and short message, entities, items and exits.

These link together to form a level.

```
[ROOM]
name "<room name>"
longmessage "<long message describing the room in detail>"
shortmessage "<short message summarizing the room>"

# Room content goes here
[ENDROOM]
```

### Content

Content elements exist one per line.

They generally contain a _name_ and an optional _alias_, giving the user two possible ways to refer to a given content item.

#### Setting

Settings exist to allow user actions in the world to affect things that happen within the world.

_Spec_: `setting name default-value`

```
setting "<setting-name>" {true|false}
```

#### viewable

An entity that can only be looked at. When looked at, the viewable produces a small message the user sees near their input box. This is used for decorations or to give hints about how the user should interact with the level. It cannot be picked up or interacted with in a meaningful way. It cannot cause actions to happen in the level or world.


_Spec_: `viewable name alias message`

```
viewable "<name>" {"<alias>"|noalias} {"<message>"|nomessage}
```

#### entity and item

Entities are the primary elements of a room. They can be interacted with to change the level, the room and even global settings.

Items are entities that can be taken. When taken, they are added to the user's inventory.

```
entity "<name>" {"<alias>"|noalias} {"<message>"|nomessage} {_action_|noaction}
```

```
item "<name>" {"<alias>"|noalias} {"<message>"|nomessage} {_action_|noaction}
```


##### Action

Actions contain an action verb that the user must type in to cause the action to happen. For instance, "listen to" could be the action for a "noise", so when the user types "listen to noise", the action triggers.

What happens when the user triggers the action depends on the _conditions_, of which there may be none (the action always triggers) or any number of conditions.

```
action "<action-verb>" {{_condition_|nocondition} ...}
```

##### Condition

Conditions allow actions to have different effects depending on the context in which they execute. For instance, you may make rooms inaccessible while locked via a level setting.

_Spec_: `condition what-to-check do-if-true do-if-false`


```
condition _check_ _continuance_
```

###### Checks

Available checks are:

* `level_setting_set`: Evaluates true if the given level setting is "true".
* `level_setting_clear`: Evaluates true if the given level setting is "false".
* `global_setting_set`: Evaluates true if the given level setting is "true".
* `global_setting_clear`: Evaluates true if the given level setting is "false".
* `inventory_has`: Evaluates true if the given item is in the user's inventory.
* `inventory_has_not`: Evaluates true if the given item is not in the user's inventory.
* `always`: Evaluates true.

Check format is:

```
{level_setting_set|level_setting_clear|global_setting_set|global_setting_clear|inventory_has|inventory_has_not|always} true _do_ false _do_
```

The _do_ section comprises two different outcomes: the first is what is done if the evaluation is true, the second is what is done if the evaluation is false. The only reason that true/false analogues are providing for checks is to ease readability; technically only one version would ever be required.

###### Do

Available "do" options are:

* `set_room_message`: Change the room's message to something else.
* `append_room_message`: Add to the room's message.
* `set_prompt`: Show a message to the user in the small window near the input area.
* `set_global`: Set a global variable to "true".
* `clear_global`: Set a global variable to "false".
* `set_level_setting`: Set a level variable to "true".
* `clear_level_setting`: Set a level variable to "false".
* `exit_room`: Go to a different room.
* `do_nothing`: Don't do anything.

Do format:

```
{set_room_message|append_room_message|set_prompt|set_global|clear_global|set_level_setting|clear_level_setting|exit_room|do_nothing} {"<target-name>"|}
```

Note: Do not specify a target if `do_nothing` is used.

#### exit

An exit forms a linkage between rooms. Exits are subject to optional conditions.

```
exit "<name>" to "<target>" {_condition_|nocondition}
```

