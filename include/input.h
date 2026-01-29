#ifndef __INPUT_HID_KEYS_H__
#define __INPUT_HID_KEYS_H__

#define MB_LEFT 0x01
#define MB_RIGHT 0x02
#define MB_MIDDLE 0x03
#define MB_X1 0x04
#define MB_X2 0x05

/**
 * The next list is copied and adapted from [this gist](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2)
 */

/**
 * Modifier masks - used for the first byte in the HID report.
 * NOTE: The second byte in the report is reserved, 0x00
 */
/**
 * Modifier masks - used for the first byte in the HID report.
 * NOTE: The second byte in the report is reserved, 0x00
 */
#define KB_MASK_LCTRL  0x02
#define KB_MASK_LSHIFT 0x02
#define KB_MASK_LALT   0x04
#define KB_MASK_LMETA  0x08
#define KB_MASK_RCTRL  0x10
#define KB_MASK_RSHIFT 0x20
#define KB_MASK_RALT   0x40
#define KB_MASK_RMETA  0x80

/**
 * Scan codes - last N slots in the HID report (usually 6).
 * 0x00 if no key pressed.
 * 
 * If more than N keys are pressed, the HID reports 
 * KB_ERR_OVF in all slots to indicate this condition.
 */

#define KB_NONE 0x00 // No key pressed
#define KB_ERR_OVF 0x01 //  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
// 0x02 //  Keyboard POST Fail
// 0x03 //  Keyboard Error Undefined
#define KB_A 0x04 // Keyboard a and A
#define KB_B 0x05 // Keyboard b and B
#define KB_C 0x06 // Keyboard c and C
#define KB_D 0x07 // Keyboard d and D
#define KB_E 0x08 // Keyboard e and E
#define KB_F 0x09 // Keyboard f and F
#define KB_G 0x0a // Keyboard g and G
#define KB_H 0x0b // Keyboard h and H
#define KB_I 0x0c // Keyboard i and I
#define KB_J 0x0d // Keyboard j and J
#define KB_K 0x0e // Keyboard k and K
#define KB_L 0x0f // Keyboard l and L
#define KB_M 0x10 // Keyboard m and M
#define KB_N 0x11 // Keyboard n and N
#define KB_O 0x12 // Keyboard o and O
#define KB_P 0x13 // Keyboard p and P
#define KB_Q 0x14 // Keyboard q and Q
#define KB_R 0x15 // Keyboard r and R
#define KB_S 0x16 // Keyboard s and S
#define KB_T 0x17 // Keyboard t and T
#define KB_U 0x18 // Keyboard u and U
#define KB_V 0x19 // Keyboard v and V
#define KB_W 0x1a // Keyboard w and W
#define KB_X 0x1b // Keyboard x and X
#define KB_Y 0x1c // Keyboard y and Y
#define KB_Z 0x1d // Keyboard z and Z

#define KB_1 0x1e // Keyboard 1 and !
#define KB_2 0x1f // Keyboard 2 and @
#define KB_3 0x20 // Keyboard 3 and #
#define KB_4 0x21 // Keyboard 4 and $
#define KB_5 0x22 // Keyboard 5 and %
#define KB_6 0x23 // Keyboard 6 and ^
#define KB_7 0x24 // Keyboard 7 and &
#define KB_8 0x25 // Keyboard 8 and *
#define KB_9 0x26 // Keyboard 9 and (
#define KB_0 0x27 // Keyboard 0 and )

#define KB_ENTER 0x28 // Keyboard Return (ENTER)
#define KB_RETURN KB_ENTER
#define KB_ESC 0x29 // Keyboard ESCAPE
#define KB_ESCAPE KB_ESC
#define KB_BACKSPACE 0x2a // Keyboard DELETE (Backspace)
#define KB_TAB 0x2b // Keyboard Tab
#define KB_SPACE 0x2c // Keyboard Spacebar
#define KB_MINUS 0x2d // Keyboard - and _
#define KB_EQUAL 0x2e // Keyboard = and +
#define KB_LEFTBRACE 0x2f // Keyboard [ and {
#define KB_BRACKETBLEFT KB_LEFTBRACE
#define KB_RIGHTBRACE 0x30 // Keyboard ] and }
#define KB_BRACKETRIGHT KB_RIGHTBRACE
#define KB_BACKSLASH 0x31 // Keyboard \ and |
#define KB_HASHTILDE 0x32 // Keyboard Non-US # and ~
#define KB_SEMICOLON 0x33 // Keyboard ; and :
#define KB_APOSTROPHE 0x34 // Keyboard ' and "
#define KB_QUOTE KB_APOSTROPHE
#define KB_GRAVE 0x35 // Keyboard ` and ~
#define KB_BACKQUOTE KB_GRAVE
#define KB_COMMA 0x36 // Keyboard , and <
#define KB_DOT 0x37 // Keyboard . and >
#define KB_PERIOD KB_DOT
#define KB_SLASH 0x38 // Keyboard / and ?
#define KB_CAPSLOCK 0x39 // Keyboard Caps Lock

#define KB_F1 0x3a // Keyboard F1
#define KB_F2 0x3b // Keyboard F2
#define KB_F3 0x3c // Keyboard F3
#define KB_F4 0x3d // Keyboard F4
#define KB_F5 0x3e // Keyboard F5
#define KB_F6 0x3f // Keyboard F6
#define KB_F7 0x40 // Keyboard F7
#define KB_F8 0x41 // Keyboard F8
#define KB_F9 0x42 // Keyboard F9
#define KB_F10 0x43 // Keyboard F10
#define KB_F11 0x44 // Keyboard F11
#define KB_F12 0x45 // Keyboard F12

#define KB_SYSRQ 0x46 // Keyboard Print Screen
#define KB_SCROLLLOCK 0x47 // Keyboard Scroll Lock
#define KB_PAUSE 0x48 // Keyboard Pause
#define KB_INSERT 0x49 // Keyboard Insert
#define KB_HOME 0x4a // Keyboard Home
#define KB_PAGEUP 0x4b // Keyboard Page Up
#define KB_DELETE 0x4c // Keyboard Delete Forward
#define KB_END 0x4d // Keyboard End
#define KB_PAGEDOWN 0x4e // Keyboard Page Down
#define KB_RIGHT 0x4f // Keyboard Right Arrow
#define KB_LEFT 0x50 // Keyboard Left Arrow
#define KB_DOWN 0x51 // Keyboard Down Arrow
#define KB_UP 0x52 // Keyboard Up Arrow

#define KB_NUMLOCK 0x53 // Keyboard Num Lock and Clear
#define KB_KPSLASH 0x54 // Keypad /
#define KB_KPASTERISK 0x55 // Keypad *
#define KB_KPMINUS 0x56 // Keypad -
#define KB_KPPLUS 0x57 // Keypad +
#define KB_KPENTER 0x58 // Keypad ENTER
#define KB_KP1 0x59 // Keypad 1 and End
#define KB_KP2 0x5a // Keypad 2 and Down Arrow
#define KB_KP3 0x5b // Keypad 3 and PageDn
#define KB_KP4 0x5c // Keypad 4 and Left Arrow
#define KB_KP5 0x5d // Keypad 5
#define KB_KP6 0x5e // Keypad 6 and Right Arrow
#define KB_KP7 0x5f // Keypad 7 and Home
#define KB_KP8 0x60 // Keypad 8 and Up Arrow
#define KB_KP9 0x61 // Keypad 9 and Page Up
#define KB_KP0 0x62 // Keypad 0 and Insert
#define KB_KPDOT 0x63 // Keypad . and Delete

#define KB_102ND 0x64 // Keyboard Non-US \ and |
#define KB_COMPOSE 0x65 // Keyboard Application
#define KB_POWER 0x66 // Keyboard Power
#define KB_KPEQUAL 0x67 // Keypad =

#define KB_F13 0x68 // Keyboard F13
#define KB_F14 0x69 // Keyboard F14
#define KB_F15 0x6a // Keyboard F15
#define KB_F16 0x6b // Keyboard F16
#define KB_F17 0x6c // Keyboard F17
#define KB_F18 0x6d // Keyboard F18
#define KB_F19 0x6e // Keyboard F19
#define KB_F20 0x6f // Keyboard F20
#define KB_F21 0x70 // Keyboard F21
#define KB_F22 0x71 // Keyboard F22
#define KB_F23 0x72 // Keyboard F23
#define KB_F24 0x73 // Keyboard F24

#define KB_OPEN 0x74 // Keyboard Execute
#define KB_HELP 0x75 // Keyboard Help
#define KB_PROPS 0x76 // Keyboard Menu
#define KB_FRONT 0x77 // Keyboard Select
#define KB_SELECT KB_FRONT
#define KB_STOP 0x78 // Keyboard Stop
#define KB_AGAIN 0x79 // Keyboard Again
#define KB_UNDO 0x7a // Keyboard Undo
#define KB_CUT 0x7b // Keyboard Cut
#define KB_COPY 0x7c // Keyboard Copy
#define KB_PASTE 0x7d // Keyboard Paste
#define KB_FIND 0x7e // Keyboard Find
#define KB_MUTE 0x7f // Keyboard Mute
#define KB_VOLUMEUP 0x80 // Keyboard Volume Up
#define KB_VOLUMEDOWN 0x81 // Keyboard Volume Down
// 0x82  Keyboard Locking Caps Lock
// 0x83  Keyboard Locking Num Lock
// 0x84  Keyboard Locking Scroll Lock
#define KB_KPCOMMA 0x85 // Keypad Comma
#define KB_AS400KPEQUAL 0x86 // Keypad Equal Sign
#define KB_RO 0x87 // Keyboard International1
#define KB_KATAKANAHIRAGANA 0x88 // Keyboard International2
#define KB_KANAMODE KB_KATAKANAHIRAGANA
#define KB_KATAKANAHIRAGANAKANA KB_KATAKANAHIRAGANA
#define KB_YEN 0x89 // Keyboard International3
#define KB_HENKAN 0x8a // Keyboard International4
#define KB_CONVERT KB_HENKAN
#define KB_MUHENKAN 0x8b // Keyboard International5
#define KB_NONCONVERT KB_MUHENKAN
#define KB_KPJPCOMMA 0x8c // Keyboard International6
// 0x8d  Keyboard International7
// 0x8e  Keyboard International8
// 0x8f  Keyboard International9
#define KB_HANGEUL 0x90 // Keyboard LANG1
#define KB_LANG1 KB_HANGEUL
#define KB_HANJA 0x91 // Keyboard LANG2
#define KB_LANG2 KB_HANJA
#define KB_EISU KB_HANJA
#define KB_KATAKANA 0x92 // Keyboard LANG3
#define KB_LANG3 KB_KATAKANA
#define KB_HIRAGANA 0x93 // Keyboard LANG4
#define KB_LANG4 KB_HIRAGANA
#define KB_ZENKAKUHANKAKU 0x94 // Keyboard LANG5
#define KB_LANG5 KB_ZENKAKUHANKAKU
#define KB_LANG6 0x95 // Keyboard LANG6
#define KB_LANG7 0x96 // Keyboard LANG7
#define KB_LANG8 0x97 // Keyboard LANG8
#define KB_LANG9 0x98 // Keyboard LANG9
// 0x99  Keyboard Alternate Erase
// 0x9a  Keyboard SysReq/Attention
// 0x9b  Keyboard Cancel
// 0x9c  Keyboard Clear
// 0x9d  Keyboard Prior
// 0x9e  Keyboard Return
// 0x9f  Keyboard Separator
// 0xa0  Keyboard Out
// 0xa1  Keyboard Oper
// 0xa2  Keyboard Clear/Again
// 0xa3  Keyboard CrSel/Props
// 0xa4  Keyboard ExSel

// 0xb0  Keypad 00
// 0xb1  Keypad 000
// 0xb2  Thousands Separator
// 0xb3  Decimal Separator
// 0xb4  Currency Unit
// 0xb5  Currency Sub-unit
#define KB_KPLEFTPAREN 0xb6 // Keypad (
#define KB_KPRIGHTPAREN 0xb7 // Keypad )
// 0xb8  Keypad {
// 0xb9  Keypad }
// 0xba  Keypad Tab
// 0xbb  Keypad Backspace
// 0xbc  Keypad A
// 0xbd  Keypad B
// 0xbe  Keypad C
// 0xbf  Keypad D
// 0xc0  Keypad E
// 0xc1  Keypad F
// 0xc2  Keypad XOR
// 0xc3  Keypad ^
// 0xc4  Keypad %
// 0xc5  Keypad <
// 0xc6  Keypad >
// 0xc7  Keypad &
// 0xc8  Keypad &&
// 0xc9  Keypad |
// 0xca  Keypad ||
// 0xcb  Keypad :
// 0xcc  Keypad #
// 0xcd  Keypad Space
// 0xce  Keypad @
// 0xcf  Keypad !
// 0xd0  Keypad Memory Store
// 0xd1  Keypad Memory Recall
// 0xd2  Keypad Memory Clear
// 0xd3  Keypad Memory Add
// 0xd4  Keypad Memory Subtract
// 0xd5  Keypad Memory Multiply
// 0xd6  Keypad Memory Divide
// 0xd7  Keypad +/-
// 0xd8  Keypad Clear
// 0xd9  Keypad Clear Entry
// 0xda  Keypad Binary
// 0xdb  Keypad Octal
// 0xdc  Keypad Decimal
// 0xdd  Keypad Hexadecimal

#define KB_LEFTCTRL 0xe0 // Keyboard Left Control
#define KB_LEFTCONTROL KB_LEFTCTRL
#define KB_CONTROLLEFT KB_LEFTCTRL
#define KB_CTRLLEFT KB_LEFTCTRL
#define KB_LEFTSHIFT 0xe1 // Keyboard Left Shift
#define KB_SHIFTLEFT KB_LEFTSHIFT
#define KB_LEFTALT 0xe2 // Keyboard Left Alt
#define KB_ALTLEFT KB_LEFTALT
#define KB_LEFTMETA 0xe3 // Keyboard Left GUI
#define KB_METALEFT KB_LEFTMETA
#define KB_RIGHTCTRL 0xe4 // Keyboard Right Control
#define KB_RIGHTCONTROL KB_RIGHTCTRL
#define KB_CONTROLRIGHT KB_RIGHTCTRL
#define KB_CTRLRIGHT KB_RIGHTCTRL
#define KB_RIGHTSHIFT 0xe5 // Keyboard Right Shift
#define KB_SHIFTRIGHT KB_RIGHTSHIFT
#define KB_RIGHTALT 0xe6 // Keyboard Right Alt
#define KB_ALTRIGHT KB_RIGHTALT
#define KB_RIGHTMETA 0xe7 // Keyboard Right GUI
#define KB_METARIGHT KB_RIGHTMETA

#define KB_MEDIA_PLAYPAUSE 0xe8
#define KB_MEDIA_STOPCD 0xe9
#define KB_MEDIA_PREVIOUSSONG 0xea
#define KB_MEDIA_NEXTSONG 0xeb
#define KB_MEDIA_EJECTCD 0xec
#define KB_MEDIA_VOLUMEUP 0xed
#define KB_MEDIA_VOLUMEDOWN 0xee
#define KB_MEDIA_MUTE 0xef
#define KB_MEDIA_WWW 0xf0
#define KB_MEDIA_BACK 0xf1
#define KB_MEDIA_FORWARD 0xf2
#define KB_MEDIA_STOP 0xf3
#define KB_MEDIA_FIND 0xf4
#define KB_MEDIA_SCROLLUP 0xf5
#define KB_MEDIA_SCROLLDOWN 0xf6
#define KB_MEDIA_EDIT 0xf7
#define KB_MEDIA_SLEEP 0xf8
#define KB_MEDIA_COFFEE 0xf9
#define KB_MEDIA_REFRESH 0xfa
#define KB_MEDIA_CALC 0xfb

#endif

