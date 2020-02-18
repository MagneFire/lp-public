#include "Client/console.h"
#include "Client/input.h"
#include "Client/keys.h"
#include "Common/quakedef.h"
#include "Rendering/r_private.h"

#include "SDL/SDLWrapper.h"

#if defined(__RASPBERRY_PI__)
#define GL_WINDOWED_MOUSE_DEFAULT_STRING "0"
#elif defined(__GCW_ZERO__)
#define GL_WINDOWED_MOUSE_DEFAULT_STRING "0"
#else
#define GL_WINDOWED_MOUSE_DEFAULT_STRING "1"
#endif

#define MOUSE_MAX 3000
#define MOUSE_MIN 40

cvar_t input_grab = { "input_grab", "2" };
cvar_t input_freelook = { "input_freelook", "1", true };
cvar_t input_lookstrafe = { "input_lookstrafe", "0", true };
cvar_t input_lookspring = { "input_lookspring", "0", true };

cvar_t mouse_windowed = { "mouse_windowed", GL_WINDOWED_MOUSE_DEFAULT_STRING, true };
cvar_t mouse_pitch_inverted = { "mouse_pitch_inverted", "0", true };
cvar_t mouse_filter = { "mouse_filter", "0", true };
cvar_t mouse_exponential_sensitivity = { "mouse_exponential_sensitivity", "0", true };
cvar_t mouse_linear_sensitivity = { "mouse_linear_sensitivity", "3", true };
cvar_t mouse_speed_forward = { "mouse_speed_forward", "1", true };
cvar_t mouse_speed_side = { "mouse_speed_side", "0.8", true };
cvar_t mouse_speed_pitch = { "mouse_speed_pitch", "0.022", true };
cvar_t mouse_speed_yaw = { "mouse_speed_yaw", "0.022", true };

#if defined(__unix__) && !defined(__GCW_ZERO__)
// There is some issues with the l_joystick under Linux. So disable it by default (it works for GCW Zero and it is needed for this platform).
cvar_t stick_enabled = { "stick_enabled", "0", true };
#else
cvar_t stick_enabled = { "stick_enabled", "1", true };
#endif
cvar_t stick_pitch_inverted = { "stick_pitch_inverted", "0", true };
cvar_t stick_sensitivity = { "stick_sensitivity", "1", true };
cvar_t stick_curve = { "stick_curve", "1", true };
cvar_t stick_deadzone = { "stick_deadzone", "0.2", true };

static int l_mouseX, l_mouseY;
static int l_mouseOldX, l_mouseOldY;
static SDL_Joystick *l_joystick = NULL;
static SDL_GameController *l_controller = NULL;

static int IN_TranslateSDLtoQuakeKey(Sint32 keysym)
{
	int key;
    
    switch (keysym)
    {
    default:
        key = (keysym >= SDLK_SPACE && keysym < SDLK_DELETE) ? keysym : 0;
        break;
        
		#if defined(__GCW_ZERO__)

	case SDLK_RETURN:
		key = K_GAMEPAD_START;
		break;
	case SDLK_ESCAPE:
		key = K_GAMEPAD_SELECT;
		break;

	case SDLK_LCTRL:
		key = K_GAMEPAD_A;
		break;
	case SDLK_LALT:
		key = K_GAMEPAD_B;
		break;
	case SDLK_LSHIFT:
		key = K_GAMEPAD_X;
		break;
	case SDLK_SPACE:
		key = K_GAMEPAD_Y;
		break;

	case SDLK_TAB:
		key = K_GAMEPAD_L;
		break;
	case SDLK_BACKSPACE:
		key = K_GAMEPAD_R;
		break;

	case SDLK_LEFT:
		key = K_GAMEPAD_LEFT;
		break;
	case SDLK_RIGHT:
		key = K_GAMEPAD_RIGHT;
		break;
	case SDLK_DOWN:
		key = K_GAMEPAD_DOWN;
		break;
	case SDLK_UP:
		key = K_GAMEPAD_UP;
		break;

	case SDLK_PAUSE:
		key = K_GAMEPAD_LOCK;
		break;
	case SDLK_HOME:
		key = K_GAMEPAD_POWER;
		break;

		#else

	case SDLK_PAGEUP:
		key = K_PGUP;
		break;
	case SDLK_KP_9:
		key = K_KP_PGUP;
		break;
	case SDLK_PAGEDOWN:
		key = K_PGDN;
		break;
	case SDLK_KP_3:
		key = K_KP_PGDN;
		break;
	case SDLK_KP_7:
		key = K_KP_HOME;
		break;
	case SDLK_HOME:
		key = K_HOME;
		break;
	case SDLK_KP_1:
		key = K_KP_END;
		break;
	case SDLK_END:
		key = K_END;
		break;
	case SDLK_KP_4:
		key = K_KP_LEFTARROW;
		break;
	case SDLK_LEFT:
		key = K_LEFTARROW;
		break;
	case SDLK_KP_6:
		key = K_KP_RIGHTARROW;
		break;
	case SDLK_RIGHT:
		key = K_RIGHTARROW;
		break;
	case SDLK_KP_2:
		key = K_KP_DOWNARROW;
		break;
	case SDLK_DOWN:
		key = K_DOWNARROW;
		break;
	case SDLK_KP_8:
		key = K_KP_UPARROW;
		break;
	case SDLK_UP:
		key = K_UPARROW;
		break;
	case SDLK_ESCAPE:
		key = K_ESCAPE;
		break;
	case SDLK_KP_ENTER:
		key = K_KP_ENTER;
		break;
	case SDLK_RETURN:
		key = K_ENTER;
		break;
	case SDLK_TAB:
		key = K_TAB;
		break;
	case SDLK_F1:
		key = K_F1;
		break;
	case SDLK_F2:
		key = K_F2;
		break;
	case SDLK_F3:
		key = K_F3;
		break;
	case SDLK_F4:
		key = K_F4;
		break;
	case SDLK_F5:
		key = K_F5;
		break;
	case SDLK_F6:
		key = K_F6;
		break;
	case SDLK_F7:
		key = K_F7;
		break;
	case SDLK_F8:
		key = K_F8;
		break;
	case SDLK_F9:
		key = K_F9;
		break;
	case SDLK_F10:
		key = K_F10;
		break;
	case SDLK_F11:
		key = K_F11;
		break;
	case SDLK_F12:
		key = K_F12;
		break;
	case SDLK_F13:
		key = K_F13;
		break;
	case SDLK_F14:
		key = K_F14;
		break;
	case SDLK_F15:
		key = K_F15;
		break;
	case SDLK_BACKSPACE:
		key = K_BACKSPACE;
		break;
	case SDLK_KP_PERIOD:
		key = K_KP_DEL;
		break;
	case SDLK_DELETE:
		key = K_DEL;
		break;
	case SDLK_PAUSE:
		key = K_PAUSE;
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		key = K_SHIFT;
		break;
	case SDLK_LCTRL:
	case SDLK_RCTRL:
		key = K_CTRL;
		break;
	case SDLK_RGUI:
	case SDLK_LGUI:
		key = K_COMMAND;
		break;
	case SDLK_RALT:
	case SDLK_LALT:
		key = K_ALT;
		break;
	case SDLK_KP_5:
		key = K_KP_5;
		break;
	case SDLK_INSERT:
		key = K_INS;
		break;
	case SDLK_KP_0:
		key = K_KP_INS;
		break;
	case SDLK_KP_MULTIPLY:
		key = K_KP_STAR;
		break;
	case SDLK_KP_PLUS:
		key = K_KP_PLUS;
		break;
	case SDLK_KP_MINUS:
		key = K_KP_MINUS;
		break;
	case SDLK_KP_DIVIDE:
		key = K_KP_SLASH;
		break;
	case SDLK_MODE:
		key = K_MODE;
		break;
	case SDLK_APPLICATION:
		key = K_COMPOSE;
		break;
	case SDLK_HELP:
		key = K_HELP;
		break;
	case SDLK_PRINTSCREEN:
		key = K_PRINT;
		break;
	case SDLK_SYSREQ:
		key = K_SYSREQ;
		break;
	case SDLK_MENU:
		key = K_MENU;
		break;
	case SDLK_POWER:
		key = K_POWER;
		break;
	case SDLK_UNDO:
		key = K_UNDO;
		break;
	case SDLK_SCROLLLOCK:
		key = K_SCROLLOCK;
		break;
	case SDLK_NUMLOCKCLEAR:
		key = K_KP_NUMLOCK;
		break;
	case SDLK_CAPSLOCK:
		key = K_CAPSLOCK;
		break;
    
    #endif
    }
          
    return key;
}

bool IN_processEvent(SDL_Event *event)
{   
	printf("IN_processEvent\r\n");
    switch (event->type)
    {
    default:
        break;

	case SDL_QUIT:
		printf("Exit requested by the system.");
		sdlwRequestExit(true);
        Host_Exit();
		break;

	case SDL_WINDOWEVENT:
		switch (event->window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
			printf("Exit requested by the user (by closing the window).");
			sdlwRequestExit(true);
            Host_Exit();
			break;
		case SDL_WINDOWEVENT_RESIZED:
			//sdlwResize(event->window.data1, event->window.data2);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
            Key_ClearStates();
			break;
		}
		break;

	case SDL_MOUSEWHEEL:
		Key_Event((event->wheel.y > 0 ? K_MWHEELUP : K_MWHEELDOWN), true);
		Key_Event((event->wheel.y > 0 ? K_MWHEELUP : K_MWHEELDOWN), false);
		break;
	case SDL_MOUSEBUTTONDOWN:
	// fall-through
	case SDL_MOUSEBUTTONUP:
        {
            int key = -1;
            switch (event->button.button)
            {
            default:
                break;
            case SDL_BUTTON_LEFT:
                key = K_MOUSE1;
                break;
            case SDL_BUTTON_MIDDLE:
                key = K_MOUSE3;
                break;
            case SDL_BUTTON_RIGHT:
                key = K_MOUSE2;
                break;
            case SDL_BUTTON_X1:
                key = K_MOUSE4;
                break;
            case SDL_BUTTON_X2:
                key = K_MOUSE5;
                break;
            }
            if (key >= 0)
                Key_Event(key, (event->type == SDL_MOUSEBUTTONDOWN));
        }
		break;
    case SDL_MOUSEMOTION:
		if (key_dest == key_game && !cl.paused)
        {
            l_mouseX += event->motion.xrel;
            l_mouseY += event->motion.yrel;
        }
        break;

		#if !defined(__GCW_ZERO__)
	case SDL_TEXTINPUT:
		Char_Event(event->text.text[0], false);
		break;
		#endif

    case SDL_KEYDOWN:
    case SDL_KEYUP:
        {
            int key = IN_TranslateSDLtoQuakeKey(event->key.keysym.sym);
            if (key > 0)
            {
                bool down = (event->type == SDL_KEYDOWN);
                Key_Event(key, down);
            }
        }
        break;

	case SDL_JOYAXISMOTION:
		break;
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	{
		bool down = (event->type == SDL_JOYBUTTONDOWN);
		Key_Event(K_JOY1 + event->jbutton.button, down);
	}
	break;
	case SDL_JOYHATMOTION:
	{
		int v = event->jhat.value;
		bool left = (v == SDL_HAT_LEFTDOWN || v == SDL_HAT_LEFT || v == SDL_HAT_LEFTUP);
		bool right = (v == SDL_HAT_RIGHTDOWN || v == SDL_HAT_RIGHT || v == SDL_HAT_RIGHTUP);
		bool down = (v == SDL_HAT_LEFTDOWN || v == SDL_HAT_DOWN || v == SDL_HAT_RIGHTDOWN);
		bool up = (v == SDL_HAT_LEFTUP || v == SDL_HAT_UP || v == SDL_HAT_RIGHTUP);
		Key_Event(K_GAMEPAD_LEFT, left);
		Key_Event(K_GAMEPAD_RIGHT, right);
		Key_Event(K_GAMEPAD_DOWN, down);
		Key_Event(K_GAMEPAD_UP, up);
	}
	break;

	case SDL_CONTROLLERAXISMOTION:
		break;
	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
	{
		bool down = (event->type == SDL_CONTROLLERBUTTONDOWN);
		int key;
		switch (event->cbutton.button)
		{
		default: key = -1; break;
		case SDL_CONTROLLER_BUTTON_A: key = K_GAMEPAD_A; break;
		case SDL_CONTROLLER_BUTTON_B: key = K_GAMEPAD_B; break;
		case SDL_CONTROLLER_BUTTON_X: key = K_GAMEPAD_X; break;
		case SDL_CONTROLLER_BUTTON_Y: key = K_GAMEPAD_Y; break;
		//case SDL_CONTROLLER_BUTTON_BACK: key = K_GAMEPAD_; break;
		case SDL_CONTROLLER_BUTTON_GUIDE: key = K_GAMEPAD_SELECT; break;
		case SDL_CONTROLLER_BUTTON_START: key = K_GAMEPAD_START; break;
		//case SDL_CONTROLLER_BUTTON_LEFTSTICK: key = K_GAMEPAD_; break;
		//case SDL_CONTROLLER_BUTTON_RIGHTSTICK: key = K_GAMEPAD_; break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: key = K_GAMEPAD_L; break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: key = K_GAMEPAD_R; break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: key = K_GAMEPAD_UP; break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: key = K_GAMEPAD_DOWN; break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: key = K_GAMEPAD_LEFT; break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: key = K_GAMEPAD_RIGHT; break;
		}
		if (key >= 0)
			Key_Event(key, down);
	}
	break;
    }
    
    return true;
}

void Sys_SendKeyEvents()
{
	sdlwCheckEvents();

	// Grab and ungrab the mouse if the console or the menu is opened.
	qboolean want_grab = (r_fullscreen.value || input_grab.value == 1 || (input_grab.value == 2 && mouse_windowed.value));
	IN_Grab(want_grab);
}

void IN_Commands()
{
}

static float ComputeStickValue(float stickValue)
{
	float deadzone = stick_deadzone.value * (1.0f / 2.0f);
	if (stickValue < 0.0f)
	{
		if (stickValue > -deadzone)
			stickValue = 0.0f;
		else
			stickValue = (stickValue + deadzone) / (1.0f - deadzone); // Normalize.
	}
	else
	{
		if (stickValue < deadzone)
			stickValue = 0.0f;
		else
			stickValue = (stickValue - deadzone) / (1.0f - deadzone); // Normalize.
	}

	// for stick_curve, 0.5 gives quick response, 1.0 is linear, 2.0 gives slow responses.
	if (stickValue < 0.0f)
		stickValue = -powf(-stickValue, stick_curve.value);
	else
		stickValue = +powf(+stickValue, stick_curve.value);

	return stickValue * stick_sensitivity.value;
}

void IN_Move(usercmd_t *cmd)
{
    int mouseX = l_mouseX, mouseY = l_mouseY;
	l_mouseX = l_mouseY = 0;

	if (mouse_filter.value)
	{
		if ((mouseX > 1) || (mouseX < -1))
			mouseX = (mouseX + l_mouseOldX) * 0.5f;

		if ((mouseY > 1) || (mouseY < -1))
			mouseY = (mouseY + l_mouseOldY) * 0.5f;
	}
	l_mouseOldX = mouseX;
	l_mouseOldY = mouseY;

    float exponential_sensitivity = mouse_exponential_sensitivity.value;
    if (!exponential_sensitivity)
    {
        float linear_sensitivity = mouse_linear_sensitivity.value;
        mouseX *= linear_sensitivity;
        mouseY *= linear_sensitivity;
    }
    else
    {
        if (mouseX > MOUSE_MIN || mouseY > MOUSE_MIN || mouseX < -MOUSE_MIN || mouseY < -MOUSE_MIN)
        {
            mouseX = exponential_sensitivity * (mouseX * mouseX * mouseX) / 4;
            mouseY = exponential_sensitivity * (mouseY * mouseY * mouseY) / 4;

            if (mouseX > MOUSE_MAX)
                mouseX = MOUSE_MAX;
            else if (mouseX < -MOUSE_MAX)
                mouseX = -MOUSE_MAX;

            if (mouseY > MOUSE_MAX)
                mouseY = MOUSE_MAX;
            else if (mouseY < -MOUSE_MAX)
                mouseY = -MOUSE_MAX;
        }
    }

    float speed;
    float frametime = host_frametime;
    if (in_speed.state & 1)
        speed = frametime * cl_anglespeedkey.value;
    else
        speed = frametime;

    float running;
    if ((in_speed.state & 1) ^ (int)(cl_run.value))
        running = 2.0f;
    else
        running = 1.0f;

    float joyXFloat = 0.0f, joyYFloat = 0.0f;

    if (l_joystick != NULL && stick_enabled.value)
    {
        float joyX, joyY;
        joyX = (float)SDL_JoystickGetAxis(l_joystick, 0) / 32768.0f;
        joyY = (float)SDL_JoystickGetAxis(l_joystick, 1) / 32768.0f;
        joyXFloat = ComputeStickValue(joyX);
        joyYFloat = ComputeStickValue(joyY);
    }

    if (l_controller != NULL)
    {
        float joyX, joyY;

        joyX = (float)SDL_GameControllerGetAxis(l_controller, SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;;
        joyY = (float)SDL_GameControllerGetAxis(l_controller, SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;;
        joyXFloat += ComputeStickValue(joyX);
        joyYFloat += ComputeStickValue(joyY);

        joyX = (float)SDL_GameControllerGetAxis(l_controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f;;
        joyY = (float)SDL_GameControllerGetAxis(l_controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f;;
        joyX = ComputeStickValue(joyX);
        joyY = ComputeStickValue(joyY);
        cmd->sidemove += cl_sidespeed.value * joyX * running;
        cmd->forwardmove += cl_forwardspeed.value * joyY * running;
    }

    /* add mouse X/Y movement to cmd */
    if ((in_strafe.state & 1) || (input_lookstrafe.value && (in_mlook.state & 1)))
    {
        float sideDelta;
        sideDelta  = mouse_speed_side.value * mouseX;
        sideDelta += cl_sidespeed.value * joyXFloat * running;
        cmd->sidemove += sideDelta;
    }
    else
    {
        float yawDelta;
        yawDelta  = mouse_speed_yaw.value * mouseX;
        yawDelta += speed * cl_yawspeed.value * joyXFloat;
        cl.viewangles[YAW] -= yawDelta;
    }

    if (((in_mlook.state & 1) || input_freelook.value) && !(in_strafe.state & 1))
    {
        float pitchDeltaMouse = mouse_speed_pitch.value * mouseY;
        if (mouse_pitch_inverted.value)
            pitchDeltaMouse = -pitchDeltaMouse;
        float pitchDeltaStick = speed * cl_pitchspeed.value * joyYFloat;
        if (stick_pitch_inverted.value)
            pitchDeltaStick = -pitchDeltaStick;
        cl.viewangles[PITCH] += pitchDeltaMouse + pitchDeltaStick;
    }
    else
    {
        float forwardDelta;
        forwardDelta  = -mouse_speed_forward.value * mouseY;
        forwardDelta += cl_forwardspeed.value * joyYFloat * running;
        cmd->forwardmove += forwardDelta;
    }

    if (in_mlook.state != 0 || input_freelook.value != 0)
		V_StopPitchDrift();
    
    #if 0
    bool strafe = in_strafe.state != 0;
    bool mlook = in_mlook.state != 0 || input_freelook.value != 0;

	if (strafe || (input_lookstrafe.value && mlook))
		cmd->sidemove += mouse_speed_side.value * mouseX;
	else
		cl.viewangles[YAW] -= mouse_speed_yaw.value * mouseX;
	if (mlook)
		V_StopPitchDrift();

	if (mlook && !strafe)
	{
		cl.viewangles[PITCH] += mouse_speed_pitch.value * mouseY;
		if (cl.viewangles[PITCH] > 80)
			cl.viewangles[PITCH] = 80;
		if (cl.viewangles[PITCH] < -70)
			cl.viewangles[PITCH] = -70;
	}
	else
	{
		if (strafe && noclip_anglehack)
			cmd->upmove -= mouse_speed_forward.value * mouseY;
		else
			cmd->forwardmove -= mouse_speed_forward.value * mouseY;
	}
    #endif
}

void IN_Init()
{
	l_mouseX = l_mouseY = 0;

   	Cvar_RegisterVariable(&input_grab);
	Cvar_RegisterVariable(&input_freelook);
	Cvar_RegisterVariable(&input_lookspring);
	Cvar_RegisterVariable(&input_lookstrafe);

   	Cvar_RegisterVariable(&stick_enabled);
   	Cvar_RegisterVariable(&stick_pitch_inverted);
   	Cvar_RegisterVariable(&stick_sensitivity);
   	Cvar_RegisterVariable(&stick_curve);
   	Cvar_RegisterVariable(&stick_deadzone);

   	Cvar_RegisterVariable(&mouse_windowed);
   	Cvar_RegisterVariable(&mouse_pitch_inverted);
   	Cvar_RegisterVariable(&mouse_filter);
	Cvar_RegisterVariable(&mouse_exponential_sensitivity);
	Cvar_RegisterVariable(&mouse_linear_sensitivity);
	Cvar_RegisterVariable(&mouse_speed_forward);
	Cvar_RegisterVariable(&mouse_speed_side);
	Cvar_RegisterVariable(&mouse_speed_pitch);
	Cvar_RegisterVariable(&mouse_speed_yaw);

	Con_Printf("IN_Init\n");

	/*if (!SDL_WasInit(SDL_INIT_JOYSTICK))
	{
		Con_Printf("SDL_WasInit(SDL_INIT_JOYSTICK)\n");
		if (SDL_Init(SDL_INIT_JOYSTICK) == -1)
		{
			Con_Printf("Couldn't init SDL l_joystick: %s.\n", SDL_GetError());
		}
		else
		{*/
			Con_Printf("SDL_WasInit(SDL_INIT_JOYSTICK) %d\n", SDL_NumJoysticks());
			if (SDL_NumJoysticks() > 0)
			{
				Con_Printf("SDL_NumJoysticks() %i\n", SDL_NumJoysticks());
				int n = SDL_NumJoysticks();
				for (int i = 0; i < n; i++)
				{
					if (SDL_IsGameController(i))
					{
						if (!l_controller)
						{
							l_controller = SDL_GameControllerOpen(i);
							if (!l_controller)
							{
								Con_Printf("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
							} else {
								Con_Printf("Opened gamecontroller %i: %s\n", i);
							}
						}
					}
					else
					{
						if (!l_joystick)
						{
							l_joystick = SDL_JoystickOpen(i);
							if (!l_joystick)
							{
								Con_Printf("Could not open l_joystick %i: %s\n", i, SDL_GetError());
							}
						}
					}
				}
			}
		/*}
	}*/
}

void IN_Shutdown()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK))
	{
		if (SDL_JoystickGetAttached(l_joystick))
		{
			SDL_JoystickClose(l_joystick);
			l_joystick = NULL;
		}
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	}
}

void IN_Grab(qboolean grab)
{
	if (sdlwContext->window != NULL)
		SDL_SetWindowGrab(sdlwContext->window, grab ? SDL_TRUE : SDL_FALSE);
	if (SDL_SetRelativeMouseMode(grab ? SDL_TRUE : SDL_FALSE) < 0)
		Con_Printf("Setting relative mouse mode failed, reason: %s\nYou should probably update to SDL 2.0.3 or newer\n", SDL_GetError());
}
