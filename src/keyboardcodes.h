/*Scancodes Keyboard*/
/*P when key is pressed, R when its released*/
/*TODO: Las teclas comentadas son las especiales (Mandan E0 primero), que hay que hacer varios inputs dependiendo de que tecla es.
	Como por ahora no pensamos usarlas, no voy a implementarlo por ahora */
#define KBC_SPECIAL 0xE0

#define KBC_A_P 0x1E
#define KBC_A_R 0x9E

#define KBC_B_P 0x30
#define KBC_B_R 0xB0

#define KBC_C_P 0x2E
#define KBC_C_R 0xAE

#define KBC_D_P 0x20
#define KBC_D_R 0xA0

#define KBC_E_P 0x12
#define KBC_E_R 0x92

#define KBC_F_P 0x21
#define KBC_F_R 0xA1

#define KBC_G_P 0x22
#define KBC_G_R 0xA2

#define KBC_H_P 0x23
#define KBC_H_R 0xA3

#define KBC_I_P 0x17
#define KBC_I_R 0x97

#define KBC_J_P 0x24
#define KBC_J_R 0xA4

#define KBC_K_P 0x25
#define KBC_K_R 0xA5

#define KBC_L_P 0x26
#define KBC_L_R 0xA6

#define KBC_M_P 0x32
#define KBC_M_R 0xB2

#define KBC_N_P 0x31
#define KBC_N_R 0xB1

#define KBC_O_P 0x18
#define KBC_O_R 0x98

#define KBC_P_P 0x19
#define KBC_P_R 0x99

#define KBC_Q_P 0x10
#define KBC_Q_R 0x90

#define KBC_R_P 0x13
#define KBC_R_R 0x93

#define KBC_S_P 0x1F
#define KBC_S_R 0x9F

#define KBC_T_P 0x14
#define KBC_T_R 0x94

#define KBC_U_P 0x16
#define KBC_U_R 0x96

#define KBC_V_P 0x2F
#define KBC_V_R 0xAF

#define KBC_W_P 0x11
#define KBC_W_R 0x91

#define KBC_X_P 0x2D
#define KBC_X_R 0xAD

#define KBC_Y_P 0x15
#define KBC_Y_R 0x95

#define KBC_Z_P 0x2C
#define KBC_Z_R 0xAC

#define KBC_0_P 0x0B
#define KBC_0_R 0x8B

#define KBC_1_P 0x02
#define KBC_1_R 0x82

#define KBC_2_P 0x03
#define KBC_2_R 0x83

#define KBC_3_P 0x04
#define KBC_3_R 0x84

#define KBC_4_P 0x05
#define KBC_4_R 0x85

#define KBC_5_P 0x06
#define KBC_5_R 0x86

#define KBC_6_P 0x07
#define KBC_6_R 0x87

#define KBC_7_P 0x08
#define KBC_7_R 0x88

#define KBC_8_P 0x09
#define KBC_8_R 0x89

#define KBC_9_P 0x0A
#define KBC_9_R 0x8A

#define KBC_ACCENT_P 0x29
#define KBC_ACCENT_R 0x89

#define KBC_MINUS_P 0x0C
#define KBC_MINUS_R 0x8C

#define KBC_EQUAL_P 0x0D
#define KBC_EQUAL_R 0x8D

#define KBC_BSLASH_P 0x2B
#define KBC_BSLASH_R 0xAB

#define KBC_BSPACE_P 0x0E
#define KBC_BSPACE_R 0x8E

#define KBC_SPACE_P 0x39
#define KBC_SPACE_R 0xB9

#define KBC_TAB_P 0x0F
#define KBC_TAB_R 0x8F

#define KBC_CAPS_P 0x3A
#define KBC_CAPS_R 0xBA

#define KBC_LSHFT_P 0x2A
#define KBC_LSHFT_R 0xAA

#define KBC_LCTRL_P 0x1D
#define KBC_LCTRL_R 0x9D

//#define KBC_LGUI_P 0xE0,5B
//#define KBC_LGUI_R 0xE0,DB

#define KBC_LALT_P 0x38
#define KBC_LALT_R 0xB8

#define KBC_RSHFT_P 0x36
#define KBC_RSHFT_R 0xB6

//#define KBC_RCTRL_P 0xE0,1D
//#define KBC_RCTRL_R 0xE0,9D

//#define KBC_RGUI_P 0xE0,5C
//#define KBC_RGUI_R 0xE0,DC

//#define KBC_RALT_P 0xE0,38
//#define KBC_RALT_R 0xE0,B8

//#define KBC_APPS_P 0xE0,5D
//#define KBC_APPS_R 0xE0,DD

#define KBC_ENTER_P 0x1C
#define KBC_ENTER_R 0x9C

#define KBC_ESC_P 0x01
#define KBC_ESC_R 0x81

#define KBC_F1_P 0x3B
#define KBC_F1_R 0xBB

#define KBC_F2_P 0x3C
#define KBC_F2_R 0xBC

#define KBC_F3_P 0x3D
#define KBC_F3_R 0xBD

#define KBC_F4_P 0x3E
#define KBC_F4_R 0xBE

#define KBC_F5_P 0x3F
#define KBC_F5_R 0xBF

#define KBC_F6_P 0x40
#define KBC_F6_R 0xC0

#define KBC_F7_P 0x41
#define KBC_F7_R 0xC1

#define KBC_F8_P 0x42
#define KBC_F8_R 0xC2

#define KBC_F9_P 0x43
#define KBC_F9_R 0xC3

#define KBC_F10_P 0x44
#define KBC_F10_R 0xC4

#define KBC_F11_P 0x57
#define KBC_F11_R 0xD7

#define KBC_F12_P 0x58
#define KBC_F12_R 0xD8

//#define KBC_PRNTSCRN_P 0xE0,2A E0,37 
//#define KBC_PRNTSCRN_R 0xE0,B7 E0,AA

#define KBC_SCROLL_P 0x46
#define KBC_SCROLL_R 0xC6

//#define KBC_PAUSE_P 0xE1,1D,45 E1,9D,C5
//#define KBC_PAUSE_R 0x-NONE-

#define KBC_OPENBRACKET_P 0x1A
#define KBC_OPENBRACKET_R 0x9A

//#define KBC_INSERT_P 0xE0,52
//#define KBC_INSERT_R 0xE0,D2

//#define KBC_HOME_P 0xE0,47
//#define KBC_HOME_R 0xE0,97

//#define KBC_PGUP_P 0xE0,49
//#define KBC_PGUP_R 0xE0,C9

//#define KBC_DELETE_P 0xE0,53
//#define KBC_DELETE_R 0xE0,D3

//#define KBC_END_P 0xE0,4F
//#define KBC_END_R 0xE0,CF

//#define KBC_PGDN_P 0xE0,51
//#define KBC_PGDN_R 0xE0,D1

//#define KBC_UARROW_P 0xE0,48
//#define KBC_UARROW_R 0xE0,C8

//#define KBC_LARROW_P 0xE0,4B
//#define KBC_LARROW_R 0xE0,CB

//#define KBC_DARROW_P 0xE0,50
//#define KBC_DARROW_R 0xE0,D0

//#define KBC_RARROW_P 0xE0,4D
//#define KBC_RARROW_R 0xE0,CD

#define KBC_NUM_P 0x45
#define KBC_NUM_R 0xC5

//#define KBC_KP_SLASH_P 0xE0,35
//#define KBC_KP_SLASH_R 0xE0,B5

#define KBC_KP_ASTERISK_P 0x37
#define KBC_KP_ASTERISK_R 0xB7

#define KBC_KP_MINUS_P 0x4A
#define KBC_KP_MINUS_R 0xCA

#define KBC_KP_PLUS_P 0x4E
#define KBC_KP_PLUS_R 0xCE

//#define KBC_KP_EN_P 0xE0,1C
//#define KBC_KP_EN_R 0xE0,9C

#define KBC_KP_DOT_P 0x53
#define KBC_KP_DOT_R 0xD3

#define KBC_KP_0_P 0x52
#define KBC_KP_0_R 0xD2

#define KBC_KP_1_P 0x4F
#define KBC_KP_1_R 0xCF

#define KBC_KP_2_P 0x50
#define KBC_KP_2_R 0xD0

#define KBC_KP_3_P 0x51
#define KBC_KP_3_R 0xD1

#define KBC_KP_4_P 0x4B
#define KBC_KP_4_R 0xCB

#define KBC_KP_5_P 0x4C
#define KBC_KP_5_R 0xCC

#define KBC_KP_6_P 0x4D
#define KBC_KP_6_R 0xCD

#define KBC_KP_7_P 0x47
#define KBC_KP_7_R 0xC7

#define KBC_KP_8_P 0x48
#define KBC_KP_8_R 0xC8

#define KBC_KP_9_P 0x49
#define KBC_KP_9_R 0xC9

#define KBC_CLOSEBRACKET_P 0x1B
#define KBC_CLOSEBRACKET_R 0x9B

#define KBC_SEMICOLON_P 0x27
#define KBC_SEMICOLON_R 0xA7

#define KBC_QUOTE_P 0x28
#define KBC_QUOTE_R 0xA8

#define KBC_COMMA_P 0x33
#define KBC_COMMA_R 0xB3

#define KBC_DOT_P 0x34
#define KBC_DOT_R 0xB4

#define KBC_SLASH_P 0x35
#define KBC_SLASH_R 0xB5

