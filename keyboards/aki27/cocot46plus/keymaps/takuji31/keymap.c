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
#include <stdio.h>
#include "quantum.h"
#include "os_detection.h"
#include "cocot46plus.h"

enum user_keycodes {
    CFG_INFO = SCRL_IN + 1,
};

static const uint16_t cpi_options[]      = COCOT_CPI_OPTIONS;
static const uint16_t scrl_div_options[] = COCOT_SCROLL_DIVIDERS;
static const int16_t  angle_options[]    = COCOT_ROTATION_ANGLE;

enum layer_number {
    _BASE = 0,
    _WBASE,
    // macOS layers
    _NAV,
    _NUM,
    _SYM,
    _FN,
    _SYS,
    // Windows layers
    _WNAV,
    _WNUM,
    _WSYM,
    _WFN,
    _WSYS,
};

// Home Row Mods - macOS (CAGS)
#define HRM_A LCTL_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LGUI_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N RSFT_T(KC_N)
#define HRM_E LGUI_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RCTL_T(KC_O)

// Home Row Mods - Windows (GACS)
#define WHRM_A LGUI_T(KC_A)
#define WHRM_S LCTL_T(KC_S)
#define WHRM_E RCTL_T(KC_E)
#define WHRM_O RGUI_T(KC_O)

// Thumb keys - macOS
#define SYM_LNG2 LT(_SYM, KC_LNG2)
#define NAV_SPC  LT(_NAV, KC_SPC)
#define SFT_TAB  MT(MOD_LSFT, KC_TAB)
#define FN_BSPC  LT(_FN, KC_BSPC)
#define NUM_ENT  LT(_NUM, KC_ENT)
#define SYS_LNG1 LT(_SYS, KC_LNG1)

// Thumb keys - Windows
#define WSYM_LNG2 LT(_WSYM, KC_LNG2)
#define WNAV_SPC  LT(_WNAV, KC_SPC)
#define WFN_BSPC  LT(_WFN, KC_BSPC)
#define WNUM_ENT  LT(_WNUM, KC_ENT)
#define WSYS_LNG1 LT(_WSYS, KC_LNG1)

// Shortcuts for NAV layer - macOS
#define M_UNDO  G(KC_Z)
#define M_CUT   G(KC_X)
#define M_COPY  G(KC_C)
#define M_PASTE G(KC_V)
#define M_PSTM  S(G(KC_V))

// Shortcuts for NAV layer - Windows
#define W_UNDO  C(KC_Z)
#define W_CUT   C(KC_X)
#define W_COPY  C(KC_C)
#define W_PASTE C(KC_V)
#define W_PSTM  S(C(KC_V))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
    //|------------------------------------------------|                                   |------------------------------------------------|
       KC_ESC,  KC_Q,  KC_W,  KC_F,  KC_P,  KC_G,                                           KC_J,  KC_L,    KC_U,    KC_Y,   KC_QUOT, KC_EQL,
       KC_TAB,  HRM_A, HRM_R, HRM_S, HRM_T, KC_D,                                           KC_H,  HRM_N,   HRM_E,   HRM_I,  HRM_O,   KC_MINS,
       KC_LSFT, KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,                                           KC_K,  KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_SCLN,
                     KC_LGUI, SYM_LNG2, NAV_SPC, SFT_TAB, MS_BTN1,            MS_BTN2,   FN_BSPC, NUM_ENT, SYS_LNG1, KC_GRV,
                                                          XXXXXXX, MS_BTN3,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WBASE] = LAYOUT(
       KC_ESC,  KC_Q,   KC_W,  KC_F,   KC_P,  KC_G,                                           KC_J,   KC_L,     KC_U,    KC_Y,   KC_QUOT, KC_EQL,
       KC_TAB,  WHRM_A, HRM_R, WHRM_S, HRM_T, KC_D,                                           KC_H,   HRM_N,    WHRM_E,  HRM_I,  WHRM_O,  KC_MINS,
       KC_LSFT, KC_Z,   KC_X,  KC_C,   KC_V,  KC_B,                                           KC_K,   KC_M,     KC_COMM, KC_DOT, KC_SLSH, KC_SCLN,
                     KC_LGUI, WSYM_LNG2, WNAV_SPC, SFT_TAB, MS_BTN1,           MS_BTN2,   WFN_BSPC, WNUM_ENT, WSYS_LNG1, KC_GRV,
                                                            XXXXXXX, MS_BTN3, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    // macOS layers
    [_NAV] = LAYOUT(
       XXXXXXX, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
       XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
       XXXXXXX, M_UNDO,  M_CUT,   M_COPY,  M_PASTE, M_PSTM,                                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, XXXXXXX,
                     XXXXXXX, _______, _______, _______, MS_BTN4,             MS_BTN5,   KC_BSPC, KC_ENT,  _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_NUM] = LAYOUT(
       XXXXXXX, KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,                                    XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL, XXXXXXX,
       XXXXXXX, KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     XXXXXXX, KC_DOT,  KC_0,    _______, _______,             _______,   _______, _______, _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_SYM] = LAYOUT(
       XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,  XXXXXXX,
       XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT, XXXXXXX,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS, XXXXXXX,
                     XXXXXXX, _______, _______, _______, _______,             _______,   KC_DEL,  KC_ENT,  _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_FN] = LAYOUT(
       XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                                    XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL, XXXXXXX,
       XXXXXXX, KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     XXXXXXX, _______, KC_SPC,  KC_TAB,  _______,             _______,   _______, _______, _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_SYS] = LAYOUT(
       XXXXXXX, KC_BRID, KC_BRIU, LSG(KC_3), LSG(KC_4), LSG(KC_5),                               KC_MPRV,  KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,
       XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,   XXXXXXX,   TG(_NAV),                                XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, ROT_L15, ROT_R15,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,                                 CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT, XXXXXXX,
                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,             _______,   XXXXXXX, XXXXXXX, _______, XXXXXXX,
                                                          XXXXXXX, KC_MUTE, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    // Windows layers
    [_WNAV] = LAYOUT(
       XXXXXXX, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
       XXXXXXX, W_UNDO,  W_CUT,   W_COPY,  W_PASTE, W_PSTM,                                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, XXXXXXX,
                     XXXXXXX, _______, _______, _______, MS_BTN4,             MS_BTN5,   KC_BSPC, KC_ENT,  _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WNUM] = LAYOUT(
       XXXXXXX, KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,                                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, XXXXXXX,
       XXXXXXX, KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     XXXXXXX, KC_DOT,  KC_0,    _______, _______,             _______,   _______, _______, _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WSYM] = LAYOUT(
       XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,  XXXXXXX,
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT, XXXXXXX,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS, XXXXXXX,
                     XXXXXXX, _______, _______, _______, _______,             _______,   KC_DEL,  KC_ENT,  _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WFN] = LAYOUT(
       XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
       XXXXXXX, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, XXXXXXX,
       XXXXXXX, KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     XXXXXXX, _______, KC_SPC,  KC_TAB,  _______,             _______,   _______, _______, _______, XXXXXXX,
                                                          XXXXXXX, _______, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WSYS] = LAYOUT(
       XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX,                                    KC_MPRV,  KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,
       XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_WNAV),                                  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, ROT_L15, ROT_R15,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                    CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT, XXXXXXX,
                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,             _______,   XXXXXXX, XXXXXXX, _______, XXXXXXX,
                                                          XXXXXXX, KC_MUTE, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX
    ),
};

// Chordal hold layout: left alphabet = 'L', right alphabet = 'R', everything else = '*'
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
                 '*', '*', '*', '*', '*',           '*',   '*', '*', '*', '*',
                                              '*', '*', '*', '*', '*', '*'
);

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]  = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_WBASE] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_NAV]   = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_NUM]   = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_SYM]   = { ENCODER_CCW_CW(G(S(KC_TAB)), G(KC_TAB)) },
    [_FN]    = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_SYS]   = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_WNAV]  = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_WNUM]  = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_WSYM]  = { ENCODER_CCW_CW(C(S(KC_TAB)), C(KC_TAB)) },
    [_WFN]   = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_WSYS]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
};
#endif

static os_variant_t current_os = OS_UNSURE;

bool process_detected_host_os_user(os_variant_t detected_os) {
    current_os = detected_os;
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            set_single_persistent_default_layer(_BASE);
            break;
        case OS_WINDOWS:
        case OS_LINUX:
        case OS_UNSURE:
            set_single_persistent_default_layer(_WBASE);
            break;
    }
    return true;
}

const char *get_os_name_user(void) {
    switch (current_os) {
        case OS_MACOS:   return "macOS  ";
        case OS_IOS:     return "iOS    ";
        case OS_WINDOWS: return "Windows";
        case OS_LINUX:   return "Linux  ";
        case OS_UNSURE:
        default:         return "Unsure ";
    }
}

const char *get_layer_name_user(uint8_t layer) {
    switch (layer) {
        case _BASE:
        case _WBASE: return "(Base)  ";
        case _NAV:
        case _WNAV:  return "(NAV)   ";
        case _NUM:
        case _WNUM:  return "(NUM)   ";
        case _SYM:
        case _WSYM:  return "(SYM)   ";
        case _FN:
        case _WFN:   return "(FN)    ";
        case _SYS:
        case _WSYS:  return "(SYS)   ";
        default:     return "        ";
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
        case _SYM:
        case _WSYM:
        case _FN:
        case _WFN:
            cocot_set_scroll_mode(true);
            break;
        default:
            cocot_set_scroll_mode(false);
            break;
    }

#ifdef RGBLIGHT_ENABLE
    switch (layer) {
        case _NAV:
        case _WNAV:
            rgblight_sethsv_range(85, 255, 255, 0, 2);   // GREEN
            break;
        case _NUM:
        case _WNUM:
            rgblight_sethsv_range(11, 255, 255, 0, 2);   // CORAL
            break;
        case _SYM:
        case _WSYM:
            rgblight_sethsv_range(43, 255, 255, 0, 2);   // YELLOW
            break;
        case _FN:
        case _WFN:
            rgblight_sethsv_range(169, 255, 255, 0, 2);  // BLUE
            break;
        case _SYS:
        case _WSYS:
            rgblight_sethsv_range(213, 255, 255, 0, 2);  // PINK
            break;
        default:
            rgblight_sethsv_range(0, 0, 0, 0, 2);
            break;
    }
    rgblight_set_effect_range(2, 10);
#endif

    return state;
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_layer_state();
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CFG_INFO:
            if (record->event.pressed) {
                tap_code(KC_LNG2);
                char buf[100];
                snprintf(buf, sizeof(buf),
                    "CPI: %u, SCRL_DIV: %u, ANGLE: %d, INV: %d, SCRL: %s",
                    cpi_options[cocot_config.cpi_idx],
                    scrl_div_options[cocot_config.scrl_div],
                    angle_options[cocot_config.rotation_angle],
                    cocot_config.scrl_inv,
                    cocot_config.scrl_mode ? "ON" : "OFF"
                );
                send_string(buf);
            }
            return false;
    }
    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_R:
        case HRM_S:
        case HRM_T:
        case HRM_N:
        case HRM_E:
        case HRM_I:
        case HRM_O:
        case WHRM_A:
        case WHRM_S:
        case WHRM_E:
        case WHRM_O:
            return 280;
        default:
            return TAPPING_TERM;
    }
}
