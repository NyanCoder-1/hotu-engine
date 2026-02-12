#include "input.em.h"
#include "input.h"
#include "stb_ds.h"
#include <stdlib.h>
#include <string.h>

typedef struct EmKeyMapPair {
	const char *key;
	uint32_t value;
} EmKeyMapPair;

typedef struct EmInput {
	EmKeyMapPair *map;
} EmInput;

static void emInputInitMap(EmKeyMapPair **map);

EmInput_t emInputNew() {
	EmInput_t input = (EmInput_t)malloc(sizeof(EmInput));
	if (!input) return 0;

	memset(input, 0, sizeof(EmInput));
	emInputInitMap(&input->map);

	return input;
}
void emInputDelete(EmInput_t *input) {
	if (!input || !*input) return;

	shfree((*input)->map);
	memset(*input, 0, sizeof(EmInput));
	free(*input);
	*input = 0;
}
uint32_t emInputParseKey(EmInput_t self, const char *key) {
	if (!self) return 0;
	EmKeyMapPair *pair = shgetp(self->map, key);
	return pair ? pair->value : KB_NONE;
}

static void emInputInitMap(EmKeyMapPair **map) {
	shput(*map, "", KB_NONE);

	// Writing system keys
	shput(*map, "Backquote", KB_GRAVE);
	shput(*map, "Backslash", KB_BACKSLASH);
	shput(*map, "BracketLeft", KB_LEFTBRACE);
	shput(*map, "BracketRight", KB_RIGHTBRACE);
	shput(*map, "Comma", KB_COMMA);
	shput(*map, "Digit0", KB_0);
	shput(*map, "Digit1", KB_1);
	shput(*map, "Digit2", KB_2);
	shput(*map, "Digit3", KB_3);
	shput(*map, "Digit4", KB_4);
	shput(*map, "Digit5", KB_5);
	shput(*map, "Digit6", KB_6);
	shput(*map, "Digit7", KB_7);
	shput(*map, "Digit8", KB_8);
	shput(*map, "Digit9", KB_9);
	shput(*map, "Equal", KB_EQUAL);
	shput(*map, "IntlBackslash", KB_102ND);
	shput(*map, "IntlRo", KB_RO);
	shput(*map, "IntlYen", KB_YEN);
	shput(*map, "KeyA", KB_A);
	shput(*map, "KeyB", KB_B);
	shput(*map, "KeyC", KB_C);
	shput(*map, "KeyD", KB_D);
	shput(*map, "KeyE", KB_E);
	shput(*map, "KeyF", KB_F);
	shput(*map, "KeyG", KB_G);
	shput(*map, "KeyH", KB_H);
	shput(*map, "KeyI", KB_I);
	shput(*map, "KeyJ", KB_J);
	shput(*map, "KeyK", KB_K);
	shput(*map, "KeyL", KB_L);
	shput(*map, "KeyM", KB_M);
	shput(*map, "KeyN", KB_N);
	shput(*map, "KeyO", KB_O);
	shput(*map, "KeyP", KB_P);
	shput(*map, "KeyQ", KB_Q);
	shput(*map, "KeyR", KB_R);
	shput(*map, "KeyS", KB_S);
	shput(*map, "KeyT", KB_T);
	shput(*map, "KeyU", KB_U);
	shput(*map, "KeyV", KB_V);
	shput(*map, "KeyW", KB_W);
	shput(*map, "KeyX", KB_X);
	shput(*map, "KeyY", KB_Y);
	shput(*map, "KeyZ", KB_Z);
	shput(*map, "Minus", KB_MINUS);
	shput(*map, "Period", KB_DOT);
	shput(*map, "Quote", KB_APOSTROPHE);
	shput(*map, "Semicolon", KB_SEMICOLON);
	shput(*map, "Slash", KB_SLASH);

	// Functional keys
	shput(*map, "AltLeft", KB_LEFTALT);
	shput(*map, "AltRight", KB_RIGHTALT);
	shput(*map, "Backspace", KB_BACKSPACE);
	shput(*map, "CapsLock", KB_CAPSLOCK);
	shput(*map, "ContextMenu", KB_COMPOSE);
	shput(*map, "ControlLeft", KB_LEFTCTRL);
	shput(*map, "ControlRight", KB_RIGHTCTRL);
	shput(*map, "Enter", KB_ENTER);
	shput(*map, "MetaLeft", KB_LEFTMETA);
	shput(*map, "MetaRight", KB_RIGHTMETA);
	shput(*map, "ShiftLeft", KB_LEFTSHIFT);
	shput(*map, "ShiftRight", KB_RIGHTSHIFT);
	shput(*map, "Space", KB_SPACE);
	shput(*map, "Tab", KB_TAB);

	// Functional keys (JP and KR)
	shput(*map, "Convert", KB_HENKAN);
	shput(*map, "KanaMode", KB_KATAKANAHIRAGANA);
	shput(*map, "Lang1", KB_HANGEUL);
	shput(*map, "Lang2", KB_HANJA);
	shput(*map, "Lang3", KB_KATAKANA);
	shput(*map, "Lang4", KB_HIRAGANA);
	shput(*map, "Lang5", KB_ZENKAKUHANKAKU);
	shput(*map, "NonConvert", KB_MUHENKAN);

	// Control pad section keys
	shput(*map, "Delete", KB_DELETE);
	shput(*map, "End", KB_END);
	shput(*map, "Help", KB_HELP);
	shput(*map, "Home", KB_HOME);
	shput(*map, "Insert", KB_INSERT);
	shput(*map, "PageDown", KB_PAGEDOWN);
	shput(*map, "PageUp", KB_PAGEUP);

	// Arrow pad section
	shput(*map, "ArrowDown", KB_DOWN);
	shput(*map, "ArrowLeft", KB_LEFT);
	shput(*map, "ArrowRight", KB_RIGHT);
	shput(*map, "ArrowUp", KB_UP);

	// Numpad
	shput(*map, "NumLock", KB_NUMLOCK);
	shput(*map, "Numpad0", KB_KP0);
	shput(*map, "Numpad1", KB_KP1);
	shput(*map, "Numpad2", KB_KP2);
	shput(*map, "Numpad3", KB_KP3);
	shput(*map, "Numpad4", KB_KP4);
	shput(*map, "Numpad5", KB_KP5);
	shput(*map, "Numpad6", KB_KP6);
	shput(*map, "Numpad7", KB_KP7);
	shput(*map, "Numpad8", KB_KP8);
	shput(*map, "Numpad9", KB_KP9);
	shput(*map, "NumpadAdd", KB_KPPLUS);
	// NumpadBackspace
	// NumpadClear
	// NumpadClearEntry
	shput(*map, "NumpadComma", KB_KPCOMMA);
	shput(*map, "NumpadDecimal", KB_KPDOT);
	shput(*map, "NumpadDivide", KB_KPSLASH);
	shput(*map, "NumpadEnter", KB_KPENTER);
	shput(*map, "NumpadEqual", KB_KPEQUAL);
	// NumpadHash
	// NumpadMemoryAdd
	// NumpadMemoryClear
	// NumpadMemoryRecall
	// NumpadMemoryStore
	// NumpadMemorySubtract
	shput(*map, "NumpadMultiply", KB_KPASTERISK);
	shput(*map, "NumpadParenLeft", KB_KPLEFTPAREN);
	shput(*map, "NumpadParenRight", KB_KPRIGHTPAREN);
	// NumpadStar
	shput(*map, "NumpadSubtract", KB_KPMINUS);

	shput(*map, "Escape", KB_ESC);
	shput(*map, "F1", KB_F1);
	shput(*map, "F2", KB_F2);
	shput(*map, "F3", KB_F3);
	shput(*map, "F4", KB_F4);
	shput(*map, "F5", KB_F5);
	shput(*map, "F6", KB_F6);
	shput(*map, "F7", KB_F7);
	shput(*map, "F8", KB_F8);
	shput(*map, "F9", KB_F9);
	shput(*map, "F10", KB_F10);
	shput(*map, "F11", KB_F11);
	shput(*map, "F12", KB_F12);
	// Fn
	// FnLock
	shput(*map, "PrintScreen", KB_SYSRQ);
	shput(*map, "ScrollLock", KB_SCROLLLOCK);
	shput(*map, "Pause", KB_PAUSE);

	// Media keys
	shput(*map, "BrowserBack", KB_MEDIA_BACK);
	// BrowserFavorites
	shput(*map, "BrowserForward", KB_MEDIA_FORWARD);
	// BrowserHome
	shput(*map, "BrowserRefresh", KB_MEDIA_REFRESH);
	// BrowserSearch
	// BrowserStop
	
	shput(*map, "Eject", KB_MEDIA_EJECTCD);
	// LaunchApp1
	shput(*map, "LaunchApp2", KB_MEDIA_CALC);
	// LaunchMail
	shput(*map, "MediaPlayPause", KB_MEDIA_PLAYPAUSE);
	// MediaSelect
	shput(*map, "MediaStop", KB_MEDIA_STOP);
	shput(*map, "MediaTrackNext", KB_MEDIA_NEXTSONG);
	shput(*map, "MediaTrackPrevious", KB_MEDIA_PREVIOUSSONG);
	shput(*map, "Power", KB_POWER);
	// Sleep
	shput(*map, "AudioVolumeDown", KB_VOLUMEDOWN);
	shput(*map, "AudioVolumeMute", KB_MUTE);
	shput(*map, "AudioVolumeUp", KB_VOLUMEUP);
	// WakeUp

	// Legacy Keys
	// Hyper
	// Super
	// Turbo
	// Abort
	// Resume
	// Suspend
	shput(*map, "Again", KB_AGAIN);
	shput(*map, "Copy", KB_COPY);
	shput(*map, "Cut", KB_CUT);
	shput(*map, "Find", KB_FIND);
	shput(*map, "Open", KB_OPEN);
	shput(*map, "Paste", KB_PASTE);
	shput(*map, "Props", KB_PROPS);
	shput(*map, "Select", KB_FRONT);
	shput(*map, "Undo", KB_UNDO);
	// International keys (duplicates of Lang4/Lang3?)
	shput(*map, "Hiragana", KB_HIRAGANA);
	shput(*map, "Katakana", KB_KATAKANA);

	// Special key
	shput(*map, "Unidentified", KB_NONE);
}

