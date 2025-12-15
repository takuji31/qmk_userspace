/*
Copyright 2024 aki27

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
#include "cocot38mini.h"
#include "qmk_settings.h"

enum user_keycodes {
    CFG_INFO = AM_TOG + 1,
};

static const uint16_t cpi_options[] = COCOT_CPI_OPTIONS;
static const uint16_t scrl_div_options[] = COCOT_SCROLL_DIVIDERS;
static const int16_t angle_options[] = COCOT_ROTATION_ANGLE;

static uint8_t saved_cpi_idx = 1;
static bool snipe_mode_active = false;

// Layer definitions
enum layer_number {
    _BASE = 0,
    _WBASE,
    _MOUSE,
    _SNIPE,
    // macOS layers
    _NAV,
    _SYM,
    _FN,
    _VIAL1,
    _VIAL2,
    _MEDIA,
    // Windows layers
    _WNAV,
    _WSYM,
    _WFN,
    _VIAL3,
    _VIAL4,
    _WMEDIA,
};

// Mouse button shortcuts
#define MS_BTN1 KC_MS_BTN1
#define MS_BTN2 KC_MS_BTN2
#define MS_BTN3 KC_MS_BTN3

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

// Thumb keys - Windows
#define WSYM_LNG2 LT(_WSYM, KC_LNG2)
#define WNAV_SPC  LT(_WNAV, KC_SPC)
#define WFN_ENT   LT(_WFN, KC_ENT)

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
        KC_Q,  KC_W,  KC_F,     KC_P,    KC_G,                      KC_J,    KC_L,   KC_U,    KC_Y,    KC_MINS,
        HRM_A, HRM_R, HRM_S,    HRM_T,   KC_D,                      KC_H,    HRM_N,  HRM_E,   HRM_I,   HRM_O,
        KC_Z,  KC_X,  KC_C,     KC_V,    KC_B,                      KC_K,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH,
                      SYM_LNG2, NAV_SPC, SFT_TAB, KC_ESC,  MS_BTN1, KC_BSPC, FN_ENT, KC_LNG1
    ),
    [_WBASE] = LAYOUT(
        KC_Q,   KC_W,   KC_F,      KC_P,     KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_MINS,
        WHRM_A, HRM_R,  WHRM_S,    HRM_T,    KC_D,                      KC_H,    HRM_N,   WHRM_E,  HRM_I,   WHRM_O,
        KC_Z,   KC_X,   KC_C,      KC_V,     KC_B,                      KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                        WSYM_LNG2, WNAV_SPC, SFT_TAB, KC_ESC, TG(_WNAV), KC_BSPC, WFN_ENT, KC_LNG1
    ),
    [_MOUSE] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   MS_BTN3, MS_BTN2, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, MO(_SNIPE), XXXXXXX, MS_BTN1, MS_BTN1, SCRL_MO, XXXXXXX
    ),
    [_SNIPE] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   MS_BTN3, MS_BTN2, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, _______, XXXXXXX, MS_BTN1, MS_BTN1, SCRL_MO, XXXXXXX
    ),
    // macOS layers
    [_NAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, M_PSTM,                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
                          _______, _______, _______, _______, _______, KC_DEL,  _______, _______
    ),
    [_SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_FN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     _______, _______, _______, _______, _______,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    _______, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL,
        KC_F11,  KC_F12,  _______, _______, _______,                   _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_VIAL1] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_VIAL2] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_MEDIA] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT,                   XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
    // Windows layers
    [_WNAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        W_UNDO,  W_CUT,   W_COPY,  W_PASTE, W_PSTM,                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
                          _______, _______, _______, _______, _______, KC_DEL,  _______, _______
    ),
    [_WSYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_WFN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     _______, _______, _______, _______, _______,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_F11,  KC_F12,  _______, _______, _______,                   _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_VIAL3] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_VIAL4] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_WMEDIA] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT,                   XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT,
                          _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

// Chordal hold layout for home row mods
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
              '*', '*', '*', '*', '*', '*', '*', '*'
);

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WBASE]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_MOUSE]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_SNIPE]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    // macOS layers
    [_NAV]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_SYM]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_FN]     = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_VIAL1]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_VIAL2]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_MEDIA]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    // Windows layers
    [_WNAV]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WSYM]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WFN]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_VIAL3]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_VIAL4]  = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WMEDIA] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
};
#endif

// Keep mouse layer active for these keycodes
bool is_mouse_record_kb(uint16_t keycode, keyrecord_t* record) {
    switch(keycode) {
        case KC_LCTL:
        case KC_LSFT:
        case KC_LALT:
        case KC_LGUI:
        case SCRL_MO:
            return true;
        default:
            return false;
    }
    return is_mouse_record_user(keycode, record);
}

// Layer state management
layer_state_t layer_state_set_user(layer_state_t state) {
    // Tri-layer
    state = update_tri_layer_state(state, _NAV, _FN, _MEDIA);
    state = update_tri_layer_state(state, _WNAV, _WFN, _WMEDIA);

    uint8_t layer = get_highest_layer(state);

    if (layer != _SNIPE && snipe_mode_active) {
        snipe_mode_active = false;
        pointing_device_set_cpi(cpi_options[saved_cpi_idx]);
    }

    switch (layer) {
        case _BASE:
        case _WBASE:
            cocot_set_scroll_mode(false);
            set_auto_mouse_enable(cocot_config.auto_mouse);
            break;
        case _MOUSE:
            cocot_set_scroll_mode(false);
            break;
        case _SNIPE:
            cocot_set_scroll_mode(false);
            set_auto_mouse_enable(false);
            if (!snipe_mode_active) {
                snipe_mode_active = true;
                saved_cpi_idx = cocot_config.cpi_idx;
                uint16_t snipe_cpi = cpi_options[saved_cpi_idx] / 2;
                pointing_device_set_cpi(snipe_cpi);
            }
            break;
        case _NAV:
        case _WNAV:
        case _FN:
        case _WFN:
            cocot_set_scroll_mode(true);
            set_auto_mouse_enable(false);
            break;
        default:
            cocot_set_scroll_mode(false);
            set_auto_mouse_enable(false);
            break;
    }

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

#ifdef RGB_MATRIX_ENABLE

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    int is_layer = get_highest_layer(layer_state | default_layer_state);
    HSV hsv = {0, 255, rgblight_get_val()};

    switch (is_layer) {
        case _BASE:
        case _WBASE:
            hsv.h = 128;  // CYAN - Base layer
            break;
        case _NAV:
        case _WNAV:
            hsv.h = 85;   // GREEN - Navigation
            break;
        case _SYM:
        case _WSYM:
            hsv.h = 43;   // YELLOW - Symbols
            break;
        case _FN:
        case _WFN:
            hsv.h = 169;  // BLUE - Function
            break;
        case _MEDIA:
        case _WMEDIA:
            hsv.h = 191;  // PURPLE - Media
            break;
        case _MOUSE:
            hsv.h = 0;    // RED - Mouse
            break;
        case _SNIPE:
            hsv.h = 21;   // ORANGE - Snipe
            break;
        default:
            hsv.h = 128;  // CYAN - Default
            break;
    }

    RGB rgb = hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i <= led_max; i++) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return false;
}

#endif

// Set default tap-hold settings on EEPROM reset
void eeconfig_init_user(void) {
    uint16_t flow_tap = FLOW_TAP_TERM;
    uint8_t enabled = 1;
    qmk_settings_set(27, &flow_tap, sizeof(flow_tap));  // flow_tap_term
    qmk_settings_set(26, &enabled, sizeof(enabled));    // chordal_hold
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CFG_INFO:
            if (record->event.pressed) {
                tap_code(KC_LNG2);
                char buf[100];
                snprintf(buf, sizeof(buf),
                    "CPI: %u, SCRL_DIV: %u, ANGLE: %d, INV: %s, AUTO: %s, SCRL: %s",
                    cpi_options[cocot_config.cpi_idx],
                    scrl_div_options[cocot_config.scrl_div],
                    angle_options[cocot_config.rotation_angle],
                    cocot_config.scrl_inv ? "ON" : "OFF",
                    cocot_config.auto_mouse ? "ON" : "OFF",
                    cocot_config.scrl_mode ? "ON" : "OFF"
                );
                send_string(buf);
            }
            return false;
    }
    return true;
}
