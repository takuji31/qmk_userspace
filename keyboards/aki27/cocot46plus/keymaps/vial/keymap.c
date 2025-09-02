/*
Copyright 2022 aki27

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_number {
    _BASE = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _WBASE,
    _WLOWER,
    _WRAISE,
    _WADJUST,

    _DEVICE = 15
};
#define M_LNG2 LT(_LOWER, KC_LNG2)
#define M_LNG1 LT(_RAISE, KC_LNG1)
#define W_LNG2 LT(_WLOWER, KC_LNG2)
#define W_LNG1 LT(_WRAISE, KC_LNG1)

// Left-hand home row mods
#define GUI_A LGUI_T(KC_A)
#define ALT_S LALT_T(KC_S)
#define CTL_D LCTL_T(KC_D)
#define SFT_F LSFT_T(KC_F)

// Right-hand home row mods
#define SFT_J RSFT_T(KC_J)
#define CTL_K RCTL_T(KC_K)
#define ALT_L LALT_T(KC_L)
#define GUI_SCLN RGUI_T(KC_SCLN)

// Left-hand home row mods
#define CTL_A LCTL_T(KC_A)
#define GUI_D LGUI_T(KC_D)

// Right-hand home row mods
#define GUI_K RGUI_T(KC_K)
#define CTL_SCLN RCTL_T(KC_SCLN)

#define W_SPC LCTL_T(KC_SPC)
#define M_SPC LGUI_T(KC_SPC)

// In the buttom row, KC_MS_BTN3 binds middle-click to the encoder's button press
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                          KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT,  KC_EQL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                          KC_H,    KC_N,    KC_E,    KC_I,    KC_O, KC_MINS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_SCLN,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        KC_LALT,    TD(0),   M_SPC,  M_LNG2,  KC_MS_BTN1,             KC_MS_BTN2,   M_LNG1,  KC_ENT, KC_BSPC,  KC_GRV,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_WBASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                          KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT,  KC_EQL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                          KC_H,    KC_N,    KC_E,    KC_I,    KC_O, KC_MINS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_SCLN,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        KC_LGUI,    TD(1),   W_SPC,  W_LNG2,  KC_MS_BTN1,             KC_MS_BTN2,   W_LNG1,  KC_ENT, KC_BSPC,  KC_GRV,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_PIPE,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, _______,                                       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_LCBR, KC_RCBR,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, G(KC_Z), G(KC_X), G(KC_C), G(KC_V), _______,                                        KC_F11,  KC_F12, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,  KC_MS_BTN4,             KC_MS_BTN5,  _______, _______,  KC_DEL, _______,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_WLOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_PIPE,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, _______,                                       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_LCBR, KC_RCBR,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______,                                        KC_F11,  KC_F12, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,  KC_MS_BTN4,             KC_MS_BTN5,  _______, _______,  KC_DEL, _______,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_RAISE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),                                       S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), KC_BSLS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,                                       _______, _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______,  _______, _______,                  _______,  _______, _______, _______,  _______,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_WRAISE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),                                       S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), KC_BSLS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,                                       _______, _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______,  _______, _______,                  _______,  _______, _______, _______,  _______,
                                                                 XXXXXXX, KC_MS_BTN3,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_ADJUST] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
  MO(_DEVICE), KC_BRID, KC_BRIU, XXXXXXX,LSG(KC_6), XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX,LSG(KC_3),LSG(KC_4),LSG(KC_5),
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,  KC_MS_BTN1,             KC_MS_BTN2,  _______, _______, _______,  _______,
                                                                 XXXXXXX,   KC_MUTE,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_WADJUST] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
  MO(_DEVICE), KC_BRID, KC_BRIU, XXXXXXX,A(KC_F10),XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX,A(KC_F1), KC_PSCR, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX,XXXXXXX,LSA(KC_F10),XXXXXXX,XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,  KC_MS_BTN1,             KC_MS_BTN2,  _______, _______, _______, _______,
                                                                 XXXXXXX,   KC_MUTE,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_DEVICE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,                                       XXXXXXX,  CPI_SW, SCRL_SW, ROT_L15, ROT_R15,  EE_CLR,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, XXXXXXX, _______,  _______,   XXXXXXX,             XXXXXXX,  _______, _______, XXXXXXX,  XXXXXXX,
                                                                 XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    )
};

// Same function on all layers for now.
#if defined(ENCODER_MAP_ENABLE)
#define ENCODER_BASE { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) }
#define ENCODER_LOWER { ENCODER_CCW_CW(C(S(KC_TAB)), C(KC_TAB)) }
#define ENCODER_RAISE { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) }
#define ENCODER_ADJUST { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]   = ENCODER_BASE,
    [_WBASE]  = ENCODER_BASE,
    [_LOWER]  = ENCODER_LOWER,
    [_WLOWER] = ENCODER_LOWER,
    [_RAISE]  = ENCODER_RAISE,
    [_WRAISE] = ENCODER_RAISE,
    [_ADJUST]  = ENCODER_ADJUST,
    [_WADJUST] = ENCODER_ADJUST,
    [_DEVICE] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) }
};
#endif

void install_tap_dance_entries(void) {
    vial_tap_dance_entry_t my_tap_dance_entries[] = {
        [0] = { G(KC_SPC), KC_LALT, A(KC_SPC), XXXXXXX, TAPPING_TERM },
        [1] = { A(KC_SPC), KC_LALT,   XXXXXXX, XXXXXXX, TAPPING_TERM },
    };
    uint8_t length = sizeof(my_tap_dance_entries) / sizeof(my_tap_dance_entries[0]);
    for (uint8_t i = 0; i < length; i++) {
        dynamic_keymap_set_tap_dance(i, &my_tap_dance_entries[i]);
    }
}

void keyboard_post_init_user(void) {
    install_tap_dance_entries();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    state = update_tri_layer_state(state, _WLOWER, _WRAISE, _WADJUST);

    switch (get_highest_layer(state)) {
    case _LOWER:
    case _WLOWER:
        rgblight_sethsv_range(HSV_BLUE, 0, 2);
        cocot_set_scroll_mode(true);
        break;
    case _RAISE:
    case _WRAISE:
        rgblight_sethsv_range(HSV_RED, 0, 2);
        cocot_set_scroll_mode(true);
        break;
    case _ADJUST:
    case _WADJUST:
        rgblight_sethsv_range(HSV_GREEN, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    case _DEVICE:
        rgblight_sethsv_range(HSV_WHITE, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    default:
        rgblight_sethsv_range( 0, 0, 0, 0, 2);
        cocot_set_scroll_mode(false);
        break;
    }
    rgblight_set_effect_range( 2, 10);
    return state;
};

static os_variant_t current_os = OS_UNSURE;


void oled_write_custom_layer_state(void) {
    int cpi = cocot_get_cpi();
    int scroll_div = cocot_get_scroll_div();
    int angle = cocot_get_rotation_angle();
    int highest_layer = get_highest_layer(layer_state | default_layer_state);

    char buf_layer[3];
    char buf1[5];
    char buf2[3];
    char buf3[4];
    snprintf(buf_layer, 3, "%2d", highest_layer);
    snprintf(buf1, 5, "%4d", cpi);
    snprintf(buf2, 3, "%2d", scroll_div);
    snprintf(buf3, 4, "%3d", angle);

    // Line 1 OS and ball mode
    oled_write_P(PSTR("OS:"), true);
    oled_write_P(PSTR(" "), false);
    switch (current_os) {
        case OS_MACOS:
            oled_write_P(PSTR("macOS  "), false);
            break;
        case OS_IOS:
            oled_write_P(PSTR("iOS    "), false);
            break;
        case OS_WINDOWS:
            oled_write_P(PSTR("Windows"), false);
            break;
        case OS_LINUX:
            oled_write_P(PSTR("Linux  "), false);
            break;
        case OS_UNSURE:
            oled_write_P(PSTR("Unsure "), false);
            break;
    }
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("B:"), true);
    oled_write_P(PSTR(" "), false);

    if (cocot_get_scroll_mode()){
        oled_write_P(PSTR("Scroll"), false);
    } else{
        oled_write_P(PSTR("Cursor"), false);
    }
    // Line 2 Layer
    oled_write_P(PSTR("Layer:"), true);
    oled_write_P(PSTR(" "), false);
    oled_write(buf_layer, false);
    oled_write_P(PSTR(" "), false);
    switch (highest_layer) {
        case _BASE:
        case _WBASE:
            oled_write_P(PSTR("(Base)  "), false);
            break;
        case _LOWER:
        case _WLOWER:
            oled_write_P(PSTR("(Lower) "), false);
            break;
        case _RAISE:
        case _WRAISE:
            oled_write_P(PSTR("(Raise) "), false);
            break;
        case _ADJUST:
        case _WADJUST:
            oled_write_P(PSTR("(Adjust)"), false);
            break;
        case _DEVICE:
            oled_write_P(PSTR("(Device)"), false);
            break;
        default:
            oled_write_P(PSTR("      "), false);
            break;
    }
    oled_write_P(PSTR("   "), false);


    // Line 3 CPI/Scroll/Angle
    oled_write_P(PSTR("CPI:"), true);
    oled_write(buf1, false);
    oled_write_P(PSTR("Div:"), true);
    oled_write(buf2, false);
    oled_write_P(PSTR("Rot:"), true);
    oled_write(buf3, false);
}


#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // render_logo();
    oled_write_custom_layer_state();
    return false;
}
#endif


bool process_detected_host_os_user(os_variant_t detected_os) {
    current_os = detected_os;
    win_mode = detected_os == OS_WINDOWS;
    cocot_config.scrl_inv = win_mode ? -1 : 1;
    set_single_persistent_default_layer(win_mode ? _WBASE : _BASE);
    return false;
}
