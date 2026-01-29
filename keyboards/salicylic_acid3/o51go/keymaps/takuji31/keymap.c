/*
Copyright 2024 takuji31

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

// Layer definitions
enum layer_number {
    _BASE = 0,
    _WBASE,
    // macOS layers
    _NAV,
    _SYM,
    _FN,
    _SYS,
    // Windows layers
    _WNAV,
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
#define FN_ENT   LT(_FN, KC_ENT)
#define SYS_LNG1 LT(_SYS, KC_LNG1)

// Thumb keys - Windows
#define WSYM_LNG2 LT(_WSYM, KC_LNG2)
#define WNAV_SPC  LT(_WNAV, KC_SPC)
#define WFN_ENT   LT(_WFN, KC_ENT)
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
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    XXXXXXX, XXXXXXX, XXXXXXX, KC_J,    KC_L,    KC_U,    KC_Y,    KC_MINS,
        HRM_A,   HRM_R,   HRM_S,   HRM_T,   KC_D,    XXXXXXX, XXXXXXX, XXXXXXX, KC_H,    HRM_N,   HRM_E,   HRM_I,   HRM_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    XXXXXXX, XXXXXXX, XXXXXXX, KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
        KC_ESC,  KC_LGUI, SYM_LNG2,NAV_SPC, SFT_TAB, XXXXXXX, XXXXXXX, KC_BSPC, FN_ENT,  SYS_LNG1,KC_RGUI, MO(_SYS)
    ),

    [_WBASE] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    XXXXXXX, XXXXXXX, XXXXXXX, KC_J,    KC_L,    KC_U,    KC_Y,    KC_MINS,
        WHRM_A,  HRM_R,   WHRM_S,  HRM_T,   KC_D,    XXXXXXX, XXXXXXX, XXXXXXX, KC_H,    HRM_N,   WHRM_E,  HRM_I,   WHRM_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    XXXXXXX, XXXXXXX, XXXXXXX, KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
        KC_ESC,  KC_LGUI, WSYM_LNG2,WNAV_SPC,SFT_TAB,XXXXXXX, XXXXXXX, KC_BSPC, WFN_ENT, WSYS_LNG1,KC_RGUI,MO(_WSYS)
    ),

    // macOS layers
    [_NAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    XXXXXXX, XXXXXXX, XXXXXXX, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, M_PSTM,  XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, KC_DEL,  _______, _______, _______, _______
    ),

    [_SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, XXXXXXX, XXXXXXX, XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______
    ),

    [_FN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL,
        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______
    ),

    [_SYS] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_NAV),XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, _______
    ),

    // Windows layers
    [_WNAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    XXXXXXX, XXXXXXX, XXXXXXX, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        W_UNDO,  W_CUT,   W_COPY,  W_PASTE, W_PSTM,  XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, KC_DEL,  _______, _______, _______, _______
    ),

    [_WSYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, XXXXXXX, XXXXXXX, XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______
    ),

    [_WFN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______
    ),

    [_WSYS] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_WNAV),XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, _______
    ),
};

// Chordal hold layout for home row mods
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    'L', 'L', 'L', 'L', 'L', '*', '*', '*', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', '*', '*', '*', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', '*', '*', '*', 'R', 'R', 'R', 'R', 'R',
    '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'
);

// Layer state management
layer_state_t layer_state_set_user(layer_state_t state) {
    return state;
}

// OS detection for automatic layer switching
bool process_detected_host_os_user(os_variant_t detected_os) {
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
