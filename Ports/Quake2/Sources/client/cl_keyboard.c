/*
 * Copyright (C) 1997-2001 Id Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * =======================================================================
 *
 * Upper layer of the keyboard implementation. This file processes all
 * keyboard events which are generated by the low level keyboard layer.
 * Remeber, that the mouse is handled by the refresher and not by the
 * client!
 *
 * =======================================================================
 */

// Hardwrired keys.
// 
// Anywhere:
// - Shift + Escape: open console
// - Alt + Return: fullscreen switch
// Menus:
// - Escape: exit menu
// - Enter: enter sub menu if the cursor is on a submenu entry
// Console:
// - Escape: cancel, exit console, exit current menu, pause game and open menu
// - Tab: autocompletion
// Game:
// - Escape: open menu


#include "client/client.h"
#include "client/refresh/r_private.h"

static cvar_t *cfg_unbindall;

/*
 * key up events are sent even if in console mode
 */

char key_lines[NUM_KEY_LINES][MAXCMDLINE];
int key_linepos;
int anykeydown;

int edit_line = 0;
int history_line = 0;

#if 0 // TODO: Implementation of key modifiers for multiple bindings per key. 
enum
{
    KeyModifierMaxNb = 2
    KeyBindingMaxNb =  1 << KeyModifierMaxNb;
};

typedef struct KeyBinding_
{
    char *bindings[KeyBindingMaxNb];
    int repeatCount; //  If > 1, it is autorepeating
    bool consoleFlag; // If true, can't be rebound while in console
    bool menuFlag; // If true, can't be rebound while in menu
    bool downFlag;
} KeyBinding;

KeyBinding keyBindings[K_LAST];

#endif

char *keybindings[K_LAST];
static qboolean consolekeys[K_LAST]; /* if true, can't be rebound while in console */
static qboolean menubound[K_LAST]; /* if true, can't be rebound while in menu */
static int key_repeats[K_LAST]; /* if > 1, it is autorepeating */
static qboolean keydown[K_LAST];

qboolean Cmd_IsComplete(char *cmd);

typedef struct
{
	char *name;
	int keynum;
} keyname_t;

// Translates internal key representations into human readable strings.
keyname_t keynames[] =
{
	{ "SELECT", K_GAMEPAD_SELECT },
	{ "START", K_GAMEPAD_START },

	{ "LEFT", K_GAMEPAD_LEFT },
	{ "RIGHT", K_GAMEPAD_RIGHT },
	{ "DOWN", K_GAMEPAD_DOWN },
	{ "UP", K_GAMEPAD_UP },

	//    {"A", K_GAMEPAD_A},
	//    {"B", K_GAMEPAD_B},
	//    {"X", K_GAMEPAD_X},
	//    {"Y", K_GAMEPAD_Y},

	//    {"L", K_GAMEPAD_L},
	//    {"R", K_GAMEPAD_R},

	//    {"LOCK", K_GAMEPAD_LOCK},
	//    {"PAUSE", K_GAMEPAD_POWER},

	{ "TAB", K_TAB },
	{ "ENTER", K_ENTER },
	{ "ESCAPE", K_ESCAPE },
	{ "SPACE", K_SPACE },
	{ "BACKSPACE", K_BACKSPACE },

	{ "COMMAND", K_COMMAND },
	{ "CAPSLOCK", K_CAPSLOCK },
	{ "POWER", K_POWER },
	{ "PAUSE", K_PAUSE },

	{ "UPARROW", K_UPARROW },
	{ "DOWNARROW", K_DOWNARROW },
	{ "LEFTARROW", K_LEFTARROW },
	{ "RIGHTARROW", K_RIGHTARROW },

	{ "ALT", K_ALT },
	{ "CTRL", K_CTRL },
	{ "SHIFT", K_SHIFT },

	{ "F1", K_F1 },
	{ "F2", K_F2 },
	{ "F3", K_F3 },
	{ "F4", K_F4 },
	{ "F5", K_F5 },
	{ "F6", K_F6 },
	{ "F7", K_F7 },
	{ "F8", K_F8 },
	{ "F9", K_F9 },
	{ "F10", K_F10 },
	{ "F11", K_F11 },
	{ "F12", K_F12 },

	{ "INS", K_INS },
	{ "DEL", K_DEL },
	{ "PGDN", K_PGDN },
	{ "PGUP", K_PGUP },
	{ "HOME", K_HOME },
	{ "END", K_END },

	{ "MOUSE1", K_MOUSE1 },
	{ "MOUSE2", K_MOUSE2 },
	{ "MOUSE3", K_MOUSE3 },
	{ "MOUSE4", K_MOUSE4 },
	{ "MOUSE5", K_MOUSE5 },

	{ "JOY1", K_JOY1 },
	{ "JOY2", K_JOY2 },
	{ "JOY3", K_JOY3 },
	{ "JOY4", K_JOY4 },
	{ "JOY5", K_JOY5 },
	{ "JOY6", K_JOY6 },
	{ "JOY7", K_JOY7 },
	{ "JOY8", K_JOY8 },
	{ "JOY9", K_JOY9 },
	{ "JOY10", K_JOY10 },
	{ "JOY11", K_JOY11 },
	{ "JOY12", K_JOY12 },
	{ "JOY13", K_JOY13 },
	{ "JOY14", K_JOY14 },
	{ "JOY15", K_JOY15 },
	{ "JOY16", K_JOY16 },
	{ "JOY17", K_JOY17 },
	{ "JOY18", K_JOY18 },
	{ "JOY19", K_JOY19 },
	{ "JOY20", K_JOY20 },
	{ "JOY21", K_JOY21 },
	{ "JOY22", K_JOY22 },
	{ "JOY23", K_JOY23 },
	{ "JOY24", K_JOY24 },
	{ "JOY25", K_JOY25 },
	{ "JOY26", K_JOY26 },
	{ "JOY27", K_JOY27 },
	{ "JOY28", K_JOY28 },
	{ "JOY29", K_JOY29 },
	{ "JOY30", K_JOY30 },
	{ "JOY31", K_JOY31 },
	{ "JOY32", K_JOY32 },

	{ "AUX1", K_AUX1 },
	{ "AUX2", K_AUX2 },
	{ "AUX3", K_AUX3 },
	{ "AUX4", K_AUX4 },
	{ "AUX5", K_AUX5 },
	{ "AUX6", K_AUX6 },
	{ "AUX7", K_AUX7 },
	{ "AUX8", K_AUX8 },
	{ "AUX9", K_AUX9 },
	{ "AUX10", K_AUX10 },
	{ "AUX11", K_AUX11 },
	{ "AUX12", K_AUX12 },
	{ "AUX13", K_AUX13 },
	{ "AUX14", K_AUX14 },
	{ "AUX15", K_AUX15 },
	{ "AUX16", K_AUX16 },
	{ "AUX17", K_AUX17 },
	{ "AUX18", K_AUX18 },
	{ "AUX19", K_AUX19 },
	{ "AUX20", K_AUX20 },
	{ "AUX21", K_AUX21 },
	{ "AUX22", K_AUX22 },
	{ "AUX23", K_AUX23 },
	{ "AUX24", K_AUX24 },
	{ "AUX25", K_AUX25 },
	{ "AUX26", K_AUX26 },
	{ "AUX27", K_AUX27 },
	{ "AUX28", K_AUX28 },
	{ "AUX29", K_AUX29 },
	{ "AUX30", K_AUX30 },
	{ "AUX31", K_AUX31 },
	{ "AUX32", K_AUX32 },

	{ "KP_HOME", K_KP_HOME },
	{ "KP_UPARROW", K_KP_UPARROW },
	{ "KP_PGUP", K_KP_PGUP },
	{ "KP_LEFTARROW", K_KP_LEFTARROW },
	{ "KP_5", K_KP_5 },
	{ "KP_RIGHTARROW", K_KP_RIGHTARROW },
	{ "KP_END", K_KP_END },
	{ "KP_DOWNARROW", K_KP_DOWNARROW },
	{ "KP_PGDN", K_KP_PGDN },
	{ "KP_ENTER", K_KP_ENTER },
	{ "KP_INS", K_KP_INS },
	{ "KP_DEL", K_KP_DEL },
	{ "KP_SLASH", K_KP_SLASH },
	{ "KP_MINUS", K_KP_MINUS },
	{ "KP_PLUS", K_KP_PLUS },

	{ "MWHEELUP", K_MWHEELUP },
	{ "MWHEELDOWN", K_MWHEELDOWN },

	{ "SEMICOLON", ';' }, // Because a raw semicolon seperates commands.

	{ NULL, 0 }
};

/* ------------------------------------------------------------------ */

void CompleteCommand()
{
	char *s = key_lines[edit_line] + 1;
	if ((*s == '\\') || (*s == '/'))
		s++;

	char *cmd = Cmd_CompleteCommand(s);
	if (cmd)
	{
		key_lines[edit_line][1] = '/';
		strcpy(key_lines[edit_line] + 2, cmd);
		key_linepos = Q_strlen(cmd) + 2;

		if (Cmd_IsComplete(cmd))
		{
			key_lines[edit_line][key_linepos] = ' ';
			key_linepos++;
			key_lines[edit_line][key_linepos] = 0;
		}
		else
		{
			key_lines[edit_line][key_linepos] = 0;
		}

		return;
	}
}

/*
 * Interactive line editing and console scrollback
 */
void Key_Console(int key)
{
	if (key == K_ENTER || key == K_KP_ENTER)
	{
		/* slash text are commands, else chat */
		if ((key_lines[edit_line][1] == '\\') || (key_lines[edit_line][1] == '/'))
		{
			Cbuf_AddText(key_lines[edit_line] + 2); /* skip the > */
		}
		else
		{
			Cbuf_AddText(key_lines[edit_line] + 1); /* valid command */
		}

		Cbuf_AddText("\n");
		Com_Printf("%s\n", key_lines[edit_line]);
		edit_line = (edit_line + 1) & (NUM_KEY_LINES - 1);
		history_line = edit_line;
		key_lines[edit_line][0] = ']';
		key_linepos = 1;

		if (cls.state == ca_disconnected)
			SCR_UpdateScreen(); /* force an update, because the command may take some time */
		return;
	}

	if (key == K_TAB)
	{
		/* command completion */
		CompleteCommand();
		return;
	}

	if (key == K_BACKSPACE || key == K_LEFTARROW)
	{
		if (key_linepos > 1)
			key_linepos--;
		return;
	}

	if (key == K_DEL)
	{
		memmove(key_lines[edit_line] + key_linepos,
			key_lines[edit_line] + key_linepos + 1,
			sizeof(key_lines[edit_line]) - key_linepos - 1);
		return;
	}

	if (key == K_UPARROW)
	{
		do
		{
			history_line = (history_line - 1) & (NUM_KEY_LINES - 1);
		}
		while (history_line != edit_line &&
		       !key_lines[history_line][1]);

		if (history_line == edit_line)
			history_line = (edit_line + 1) & (NUM_KEY_LINES - 1);

		strcpy(key_lines[edit_line], key_lines[history_line]);
		key_linepos = (int)Q_strlen(key_lines[edit_line]);
		return;
	}

	if (key == K_DOWNARROW)
	{
		if (history_line == edit_line)
			return;

		do
		{
			history_line = (history_line + 1) & (NUM_KEY_LINES - 1);
		}
		while (history_line != edit_line &&
		       !key_lines[history_line][1]);

		if (history_line == edit_line)
		{
			key_lines[edit_line][0] = ']';
			key_linepos = 1;
		}
		else
		{
			strcpy(key_lines[edit_line], key_lines[history_line]);
			key_linepos = (int)Q_strlen(key_lines[edit_line]);
		}

		return;
	}

	if (key == K_PGUP || key == K_MWHEELUP)
	{
		con.display -= 2;
		return;
	}

	if (key == K_PGDN || key == K_MWHEELDOWN)
	{
		con.display += 2;
		if (con.display > con.current)
			con.display = con.current;
		return;
	}

	if (key == K_HOME)
	{
		if (keydown[K_CTRL])
			con.display = con.current - con.totallines + 10;
		else
			key_linepos = 1;
		return;
	}

	if (key == K_END)
	{
		if (keydown[K_CTRL])
			con.display = con.current;
		else
			key_linepos = (int)Q_strlen(key_lines[edit_line]);
		return;
	}

	if ((key < 32) || (key > 127))
		return; /* non printable character */

	if (key_linepos < MAXCMDLINE - 1)
	{
		int last;
		int length;

		length = Q_strlen(key_lines[edit_line]);

		if (length >= MAXCMDLINE - 1)
			return;

		last = key_lines[edit_line][key_linepos];

		memmove(key_lines[edit_line] + key_linepos + 1,
			key_lines[edit_line] + key_linepos,
			length - key_linepos);

		key_lines[edit_line][key_linepos] = key;
		key_linepos++;

		if (!last)
		{
			key_lines[edit_line][key_linepos] = 0;
		}
	}
}

qboolean chat_team;
char chat_buffer[MAXCMDLINE];
int chat_bufferlen = 0;
int chat_cursorpos = 0;

void Key_Message(int key)
{
	char last;

	if (key == K_ENTER || key == K_KP_ENTER)
	{
		if (chat_team)
			Cbuf_AddText("say_team \"");
		else
			Cbuf_AddText("say \"");

		Cbuf_AddText(chat_buffer);
		Cbuf_AddText("\"\n");

		cls.key_dest = key_game;
		chat_bufferlen = 0;
		chat_buffer[0] = 0;
		chat_cursorpos = 0;
		return;
	}

	if (key == K_ESCAPE)
	{
		cls.key_dest = key_game;
		chat_cursorpos = 0;
		chat_bufferlen = 0;
		chat_buffer[0] = 0;
		return;
	}

	if (key == K_BACKSPACE)
	{
		if (chat_cursorpos)
		{
			memmove(chat_buffer + chat_cursorpos - 1,
				chat_buffer + chat_cursorpos,
				chat_bufferlen - chat_cursorpos + 1);
			chat_cursorpos--;
			chat_bufferlen--;
		}

		return;
	}

	if (key == K_DEL)
	{
		if (chat_bufferlen && (chat_cursorpos != chat_bufferlen))
		{
			memmove(chat_buffer + chat_cursorpos,
				chat_buffer + chat_cursorpos + 1,
				chat_bufferlen - chat_cursorpos + 1);
			chat_bufferlen--;
		}
		return;
	}

	if (key == K_LEFTARROW)
	{
		if (chat_cursorpos > 0)
			chat_cursorpos--;
		return;
	}

	if (key == K_HOME)
	{
		chat_cursorpos = 0;
		return;
	}

	if (key == K_END)
	{
		chat_cursorpos = chat_bufferlen;
		return;
	}

	if (key == K_RIGHTARROW)
	{
		if (chat_buffer[chat_cursorpos])
			chat_cursorpos++;
		return;
	}

	if ((key < 32) || (key > 127))
		return; /* non printable charcter */

	if (chat_bufferlen == sizeof(chat_buffer) - 1)
		return; /* all full, this should never happen on modern systems */

	memmove(chat_buffer + chat_cursorpos + 1,
		chat_buffer + chat_cursorpos,
		chat_bufferlen - chat_cursorpos + 1);

	last = chat_buffer[chat_cursorpos];

	chat_buffer[chat_cursorpos] = key;

	chat_bufferlen++;
	chat_cursorpos++;

	if (!last)
		chat_buffer[chat_cursorpos] = 0;
}

/*
 * Returns a key number to be used to index
 * keybindings[] by looking at the given string.
 * Single ascii characters return themselves, while
 * the K_* names are matched up.
 */
static int Key_StringToKeynum(char *str)
{
	keyname_t *kn;

	if (!str || !str[0])
		return -1;

	if (!str[1])
		return str[0];

	for (kn = keynames; kn->name; kn++)
	{
		if (!Q_stricmp(str, kn->name))
			return kn->keynum;
	}

	return -1;
}

/*
 * Returns a string (either a single ascii char,
 * or a K_* name) for the given keynum.
 */
char* Key_KeynumToString(int keynum)
{
	if (keynum == -1)
		return "<KEY NOT FOUND>";

	static char tinystr[2] = { 0 };
	if (keynum > 32 && keynum < 127)
	{
		/* printable ascii */
		tinystr[0] = keynum;
		return tinystr;
	}

	keyname_t *kn;
	for (kn = keynames; kn->name; kn++)
	{
		if (keynum == kn->keynum)
			return kn->name;
	}

	return "<UNKNOWN KEYNUM>";
}

void Key_SetBinding(int keynum, char *binding)
{
	char *new;
	int l;

	if (keynum == -1)
		return;

	/* free old bindings */
	if (keybindings[keynum])
	{
		Z_Free(keybindings[keynum]);
		keybindings[keynum] = NULL;
	}

	/* allocate memory for new binding */
	l = Q_strlen(binding);
	new = Z_Malloc(l + 1);
	strcpy(new, binding);
	new[l] = 0;
	keybindings[keynum] = new;
}

void Key_Unbind_f()
{
	if (Cmd_Argc() != 2)
	{
		Com_Printf("unbind <key> : remove commands from a key\n");
		return;
	}

	int b = Key_StringToKeynum(Cmd_Argv(1));
	if (b == -1)
	{
		Com_Printf("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}
	Key_SetBinding(b, "");
}

void Key_Unbindall_f()
{
	for (int i = 0; i < K_LAST; i++)
	{
		if (keybindings[i])
			Key_SetBinding(i, "");
	}
}

/* ugly hack, set in Cmd_ExecuteString() when platform.cfg is executed
 * (=> default.cfg is done) */
extern qboolean doneWithDefaultCfg;

void Key_Bind_f()
{
	int c = Cmd_Argc();
	if (c < 2)
	{
		Com_Printf("bind <key> [command] : attach a command to a key\n");
		return;
	}

	int b = Key_StringToKeynum(Cmd_Argv(1));
	if (b == -1)
	{
		Com_Printf("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	// Don't allow binding escape.
	if (b == K_GAMEPAD_SELECT || b == K_ESCAPE)
	{
		if (doneWithDefaultCfg)
		{
			/* don't warn about this when it's from default.cfg, we can't change that anyway */
			Com_Printf("You can't bind the special key \"%s\"!\n", Cmd_Argv(1));
		}
		return;
	}

	if (c == 2)
	{
		if (keybindings[b])
			Com_Printf("\"%s\" = \"%s\"\n", Cmd_Argv(1), keybindings[b]);
		else
			Com_Printf("\"%s\" is not bound\n", Cmd_Argv(1));
		return;
	}

	/* copy the rest of the command line */
	char cmd[1024];
	cmd[0] = 0; /* start out with a null string */
	for (int i = 2; i < c; i++)
	{
		strcat(cmd, Cmd_Argv(i));
		if (i != (c - 1))
			strcat(cmd, " ");
	}

	Key_SetBinding(b, cmd);
}

/*
 * Writes lines containing "bind key value"
 */
void Key_WriteBindings(FILE *f)
{
	if (cfg_unbindall->value)
	{
		fprintf(f, "unbindall\n");
	}

	for (int i = 0; i < K_LAST; i++)
	{
		if (keybindings[i] && keybindings[i][0])
		{
			fprintf(f, "bind %s \"%s\"\n",
				Key_KeynumToString(i), keybindings[i]);
		}
	}
}

void Key_WriteConsoleHistory()
{
	char path[MAX_OSPATH];
	Com_sprintf(path, sizeof(path), "%sconsole_history.txt", Sys_GetHomeDir());

	FILE * f = fopen(path, "w");
	if (f == NULL)
	{
		Com_Printf("Opening console history %s for writing failed!\n", path);
		return;
	}

	// save the oldest lines first by starting at edit_line
	// and going forward (and wrapping around)
	const char * lastWrittenLine = "";
	for (int i = 0; i < NUM_KEY_LINES; ++i)
	{
		int lineIdx = (edit_line + i) & (NUM_KEY_LINES - 1);
		const char * line = key_lines[lineIdx];

		if (line[1] != '\0' && strcmp(lastWrittenLine, line) != 0)
		{
			// if the line actually contains something besides the ] prompt,
			// and is not identical to the last written line, write it to the file
			fputs(line, f);
			fputc('\n', f);

			lastWrittenLine = line;
		}
	}

	fclose(f);
}

/* initializes key_lines from history file, if available */
void Key_ReadConsoleHistory()
{
	char path[MAX_OSPATH];
	Com_sprintf(path, sizeof(path), "%sconsole_history.txt", Sys_GetHomeDir());

	FILE * f = fopen(path, "r");
	if (f == NULL)
	{
		Com_DPrintf("Opening console history %s for reading failed!\n", path);
		return;
	}

	for (int i = 0; i < NUM_KEY_LINES; i++)
	{
		if (fgets(key_lines[i], MAXCMDLINE, f) == NULL)
		{
			// probably EOF.. adjust edit_line and history_line and we're done here
			edit_line = i;
			history_line = i;
			break;
		}
		// remove trailing newlines
		int lastCharIdx = Q_strlen(key_lines[i]) - 1;
		while ((key_lines[i][lastCharIdx] == '\n' || key_lines[i][lastCharIdx] == '\r') && lastCharIdx >= 0)
		{
			key_lines[i][lastCharIdx] = '\0';
			--lastCharIdx;
		}
	}

	fclose(f);
}

void Key_Bindlist_f()
{
	for (int i = 0; i < K_LAST; i++)
	{
		if (keybindings[i] && keybindings[i][0])
		{
			Com_Printf("%s \"%s\"\n", Key_KeynumToString(i), keybindings[i]);
		}
	}
}

void Key_Init()
{
	for (int i = 0; i < NUM_KEY_LINES; i++)
	{
		key_lines[i][0] = ']';
		key_lines[i][1] = 0;
	}
	// can't call Key_ReadConsoleHistory() here because FS_Gamedir() isn't set yet

	key_linepos = 1;

	/* init 128 bit ascii characters in console mode */
	for (int i = 32; i < K_LAST; i++)
		consolekeys[i] = true;
	consolekeys[K_ENTER] = true;
	consolekeys[K_TAB] = true;
	consolekeys[K_LEFTARROW] = true;
	consolekeys[K_RIGHTARROW] = true;
	consolekeys[K_UPARROW] = true;
	consolekeys[K_DOWNARROW] = true;
	consolekeys[K_BACKSPACE] = true;
	consolekeys[K_HOME] = true;
	consolekeys[K_END] = true;
	consolekeys[K_PGUP] = true;
	consolekeys[K_PGDN] = true;
	consolekeys[K_SHIFT] = true;
	consolekeys[K_INS] = true;
	consolekeys[K_KP_ENTER] = true;

	menubound[K_ESCAPE] = true;
	menubound[K_GAMEPAD_SELECT] = true;
	for (int i = 0; i < 12; i++)
		menubound[K_F1 + i] = true;

	/* register our variables */
	cfg_unbindall = Cvar_Get("cfg_unbindall", "1", CVAR_ARCHIVE);

	/* register our functions */
	Cmd_AddCommand("bind", Key_Bind_f);
	Cmd_AddCommand("unbind", Key_Unbind_f);
	Cmd_AddCommand("unbindall", Key_Unbindall_f);
	Cmd_AddCommand("bindlist", Key_Bindlist_f);
}

static bool Key_isSpecial(int key)
{
    return !(key >= ' ' && key < '~');
}

/*
 * Called every frame for every detected keypress.
 * ASCII input for the console, the menu and the
 * chat window are handled by this function.
 * Anything else is handled by Key_Event().
 */
void Char_Event(int key, bool specialOnlyFlag)
{
    #if defined(__GCW_ZERO__)
    bool specialFlag = specialOnlyFlag;
    #else
    bool specialFlag = Key_isSpecial(key);
    #endif
    
    switch (cls.key_dest)
    {
    default:
        break;
        
    /* Chat */
    case key_message:
        if (specialOnlyFlag == specialFlag)
            Key_Message(key);
        break;

    /* Menu */
    case key_menu:
        if (specialOnlyFlag)
        {
            if (specialFlag)
                M_Keydown(key, key);
            else
                M_Keydown(-1, key); // Because it will be processed by Char_event() (and so must not be processed twice) but also needed by MenuKey for binding.
        }
        else
        {
            if (!specialFlag)
                M_Keydown(key, -1);
        }
        break;

    /* Console */
    case key_console:
        if (specialOnlyFlag == specialFlag)
            Key_Console(key);
        break;

    /* Console is really open but key_dest is game anyway (not connected) */
    case key_game:
        if (cls.state == ca_disconnected || cls.state == ca_connecting)
        {
            if (specialOnlyFlag == specialFlag)
                Key_Console(key);
        }
        break;
    }
}

#if defined(__GCW_ZERO__)
static void Key_checkHarwiredCheats(int key)
{
    // Cheats for GCW Zero.
    if (keydown[K_GAMEPAD_LOCK])
    {
        extern cvar_t *gl_drawworld;
        extern cvar_t *r_lightmap_only;
        switch (key)
        {
        case K_GAMEPAD_START:
            Cbuf_AddText("cheats 1\n");
            return;

        case K_GAMEPAD_LEFT:
            Cbuf_AddText("notarget\n");
            return;

        case K_GAMEPAD_RIGHT:
            Cbuf_AddText("give all\n");
            return;

        case K_GAMEPAD_DOWN:
            Cbuf_AddText("noclip\n");
            return;

        case K_GAMEPAD_UP:
            Cbuf_AddText("god\n");
            return;

        case K_GAMEPAD_L:
            if (gl_drawworld->value == 0)
                Cbuf_AddText("gl_drawworld 1\n");
            else
                Cbuf_AddText("gl_drawworld 0\n");
            return;

        case K_GAMEPAD_R:
            if (r_lightmap_only->value == 0)
                Cbuf_AddText("r_lightmap_only 1\n");
            else
                Cbuf_AddText("r_lightmap_only 0\n");
            return;
        }
    }
}
#endif

/*
 * Called every frame for every detected keypress.
 * This is only for movement and special characters,
 * anything else is handled by Char_Event().
 */
void Key_Event(int key, bool down)
{
	char cmd[1024];
	unsigned int time = Sys_Milliseconds();

	/* Track if key is down */
	keydown[key] = down;

	if (down)
	{
		key_repeats[key]++;
        if (key_repeats[key] > 1)
        {
            if (cls.key_dest == key_game && cls.state == ca_active)
                return; // Ignore autorepeats in-game.
        }
	}
	else
		key_repeats[key] = 0;

	// Fullscreen switch through Alt + Return.
	if (down && keydown[K_ALT] && key == K_ENTER)
	{
        R_Window_toggleFullScreen();
		return;
	}

	// Toggle console though Shift + Escape.
	if (down && keydown[K_SHIFT] && key == K_ESCAPE)
	{
		Con_ToggleConsole_f();
		return;
	}

	// While in attract loop all keys besides F1 to F12 (to allow quick load and the like) are treated like escape.
	if (cl.attractloop && (cls.key_dest != key_menu) && !(key >= K_F1 && key <= K_F12))
	{
		key = K_ESCAPE;
	}

	/* Escape has a special meaning. Depending on the situation it
	   - pauses the game and breaks into the menu
	   - stops the attract loop and breaks into the menu
	   - closes the console and breaks into the menu
	   - moves one menu level up
	   - closes the menu
	   - closes the help computer
	   - closes the chat window */
	if (!cls.disable_screen)
	{
		if (key == K_ESCAPE || key == K_GAMEPAD_SELECT)
		{
			if (!down)
				return;

			/* Close the help computer */
			if (cl.frame.playerstate.stats[STAT_LAYOUTS] && (cls.key_dest == key_game))
			{
				Cbuf_AddText("cmd putaway\n");
				return;
			}

			switch (cls.key_dest)
			{
			// Close chat window.
			case key_message:
				Key_Message(key);
				break;
			// Close menu or one layer up.
			case key_menu:
				M_Keydown(key, -1);
				break;
			// Pause game and break into the menu.
			case key_game:
				MenuMain_enter();
                break;
            // Leave console.
			case key_console:
                Con_ToggleConsole_f();
				break;
			}
            
			return;
		}

		#if defined(__GCW_ZERO__)
        if (down)
            Key_checkHarwiredCheats(key);
		#endif
	}

	/* This is one of the most ugly constructs I've
	   found so far in Quake II. When the game is in
	   the intermission, the player can press any key
	   to end it and advance into the next level. It
	   should be easy to figure out at server level if
	   a button is pressed. But somehow the developers
	   decided, that they'll need special move state
	   BUTTON_ANY to solve this problem. So there's
	   this global variable anykeydown. If it's not
	   0, CL_FinishMove() encodes BUTTON_ANY into the
	   button state. The server reads this value and
	   sends it to gi->ClientThink() where it's used
	   to determine if the intermission shall end.
	   Needless to say that this is the only consumer
	   of BUTTON_ANY.

	   Since we cannot alter the network protocol nor
	   the server <-> game API, I'll leave things alone
	   and try to forget. */
	if (down)
	{
		if (key_repeats[key] == 1)
			anykeydown++;
	}
	else
	{
		anykeydown--;
		if (anykeydown < 0)
			anykeydown = 0;
	}

	/* key up events only generate commands if the game key binding
	   is a button command (leading+ sign). These will occur even in
	   console mode, to keep the character from continuing an action
	   started before a console switch. Button commands include the
	   kenum as a parameter, so multiple downs can be matched with ups */
	if (!down)
	{
        char *kb = keybindings[key];
		if (kb && (kb[0] == '+'))
		{
			Com_sprintf(cmd, sizeof(cmd), "-%s %i %i\n", kb + 1, key, time);
			Cbuf_AddText(cmd);
		}
		return;
	}
    
    if (
	        ((cls.key_dest == key_menu) && menubound[key]) ||
	        ((cls.key_dest == key_console) && !consolekeys[key]) ||
	        ((cls.key_dest == key_game) && (cls.state == ca_active || !consolekeys[key]))
	        )
	{
		char *kb = keybindings[key];
		if (kb)
		{
			if (kb[0] == '+')
			{
				/* button commands add keynum and time as a parm */
				Com_sprintf(cmd, sizeof(cmd), "%s %i %i\n", kb, key, time);
				Cbuf_AddText(cmd);
			}
			else
			{
				Cbuf_AddText(kb);
				Cbuf_AddText("\n");
			}
		}
		return;
	}

    Char_Event(key, true);
}

/*
 * Marks all keys as "up"
 */
void Key_MarkAllUp()
{
	for (int key = 0; key < K_LAST; key++)
	{
		key_repeats[key] = 0;
		keydown[key] = 0;
	}
}
