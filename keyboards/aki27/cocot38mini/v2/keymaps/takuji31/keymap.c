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
        KC_Q,  KC_W,  KC_F,     KC_P,    KC_G,                      KC_J,    KC_L,   KC_U,    KC_Y,    KC_MINS,
        HRM_A, HRM_R, HRM_S,    HRM_T,   KC_D,                      KC_H,    HRM_N,  HRM_E,   HRM_I,   HRM_O,
        KC_Z,  KC_X,  KC_C,     KC_V,    KC_B,                      KC_K,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH,
                      SYM_LNG2, NAV_SPC, SFT_TAB, KC_ESC,  MS_BTN1, FN_BSPC, NUM_ENT, SYS_LNG1
    ),
    [_WBASE] = LAYOUT(
        KC_Q,   KC_W,   KC_F,      KC_P,     KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_MINS,
        WHRM_A, HRM_R,  WHRM_S,    HRM_T,    KC_D,                      KC_H,    HRM_N,   WHRM_E,  HRM_I,   WHRM_O,
        KC_Z,   KC_X,   KC_C,      KC_V,     KC_B,                      KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                        WSYM_LNG2, WNAV_SPC, SFT_TAB, KC_ESC, MS_BTN1, WFN_BSPC, WNUM_ENT, WSYS_LNG1
    ),
    [_MOUSE] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   MS_BTN3, MS_BTN2, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, MO(_SNIPE), XXXXXXX, _______, MS_BTN1, SCRL_MO, XXXXXXX
    ),
    [_SNIPE] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   MS_BTN3, MS_BTN2, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX, XXXXXXX, _______, XXXXXXX, _______, MS_BTN1, SCRL_MO, XXXXXXX
    ),
    // macOS layers
    [_NAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        M_UNDO,  M_CUT,   M_COPY,  M_PASTE, M_PSTM,                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
                          _______, _______, _______, _______, _______, KC_BSPC, KC_ENT,  _______
    ),
    [_NUM] = LAYOUT(
        KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,                    XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          KC_DOT,  KC_0,    _______, _______, _______, _______, _______, _______
    ),
    [_SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
                          _______, _______, _______, _______, _______, KC_DEL,  KC_ENT,  _______
    ),
    [_FN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL,
        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          _______, KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______
    ),
    [_SYS] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT,                   XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_NAV),                  AM_TOG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______
    ),
    // Windows layers
    [_WNAV] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        W_UNDO,  W_CUT,   W_COPY,  W_PASTE, W_PSTM,                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
                          _______, _______, _______, _______, _______, KC_BSPC, KC_ENT,  _______
    ),
    [_WNUM] = LAYOUT(
        KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          KC_DOT,  KC_0,    _______, _______, _______, _______, _______, _______
    ),
    [_WSYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
                          _______, _______, _______, _______, _______, KC_DEL,  KC_ENT,  _______
    ),
    [_WFN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          _______, KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______
    ),
    [_WSYS] = LAYOUT(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT,                   XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_WNAV),                 AM_TOG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   CFG_INFO, SCRL_SW, CPI_SW,  EE_CLR,  QK_BOOT,
                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______
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
    [_NUM]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_SYM]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_FN]     = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_SYS]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    // Windows layers
    [_WNAV]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WNUM]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WSYM]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WFN]    = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [_WSYS]   = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
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
        case _SYM:
        case _WSYM:
        case _FN:
        case _WFN:
            cocot_set_scroll_mode(true);
            set_auto_mouse_enable(false);
            break;
        case _NUM:
        case _WNUM:
            cocot_set_scroll_mode(false);
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

// Key category for per-key LED coloring
typedef enum {
    KC_CAT_OFF,
    KC_CAT_LAYER,
    KC_CAT_TARGET_LAYER,
    KC_CAT_ARROW,
    KC_CAT_PAGE,
    KC_CAT_MEDIA,
    KC_CAT_FKEY,
    KC_CAT_EDIT,
} key_category_t;

static HSV layer_to_hsv(uint8_t layer) {
    switch (layer) {
        case _BASE: case _WBASE:  return (HSV){128, 255, 255};  // CYAN
        case _NAV:  case _WNAV:   return (HSV){85,  255, 255};  // GREEN
        case _SYM:  case _WSYM:   return (HSV){43,  255, 255};  // YELLOW
        case _FN:   case _WFN:    return (HSV){169, 255, 255};  // BLUE
        case _SYS:  case _WSYS:   return (HSV){213, 255, 255};  // MAGENTA
        case _NUM:  case _WNUM:   return (HSV){11,  255, 255};  // CORAL
        case _MOUSE:              return (HSV){0,   255, 255};  // RED
        case _SNIPE:              return (HSV){21,  255, 255};  // ORANGE
        default:                  return (HSV){128, 255, 255};  // CYAN
    }
}

static key_category_t categorize_keycode(uint16_t keycode, uint8_t layer) {
    if (keycode == KC_NO) return KC_CAT_OFF;
    if (keycode == KC_TRNS) return KC_CAT_LAYER;

    // Standalone modifiers -> off on non-base layers
    if (keycode >= KC_LCTL && keycode <= KC_RGUI) {
        if (layer != _BASE && layer != _WBASE) {
            return KC_CAT_OFF;
        }
        return KC_CAT_LAYER;
    }

    // Numbers on NAV layers -> off
    if (keycode >= KC_1 && keycode <= KC_0) {
        if (layer == _NAV || layer == _WNAV) {
            return KC_CAT_OFF;
        }
        return KC_CAT_LAYER;
    }

    // LT(), TG(), MO() -> target layer color
    if ((keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) ||
        (keycode >= QK_MOMENTARY && keycode <= QK_MOMENTARY_MAX) ||
        (keycode >= QK_TOGGLE_LAYER && keycode <= QK_TOGGLE_LAYER_MAX)) {
        return KC_CAT_TARGET_LAYER;
    }

    // F1-F12
    if (keycode >= KC_F1 && keycode <= KC_F12) return KC_CAT_FKEY;

    // Arrow keys
    switch (keycode) {
        case KC_LEFT: case KC_DOWN: case KC_UP: case KC_RGHT:
            return KC_CAT_ARROW;
    }

    // Page navigation
    switch (keycode) {
        case KC_HOME: case KC_END: case KC_PGDN: case KC_PGUP:
            return KC_CAT_PAGE;
    }

    // Media/Volume/Brightness
    switch (keycode) {
        case KC_MUTE: case KC_VOLD: case KC_VOLU:
        case KC_MPRV: case KC_MPLY: case KC_MNXT:
        case KC_BRID: case KC_BRIU:
            return KC_CAT_MEDIA;
    }

    // Edit operations (macOS: G(), Windows: C())
    switch (keycode) {
        case G(KC_Z): case G(KC_X): case G(KC_C): case G(KC_V): case S(G(KC_V)):
        case C(KC_Z): case C(KC_X): case C(KC_C): case C(KC_V): case S(C(KC_V)):
            return KC_CAT_EDIT;
    }

    return KC_CAT_LAYER;
}

static uint8_t get_target_layer(uint16_t keycode) {
    if (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) {
        return QK_LAYER_TAP_GET_LAYER(keycode);
    }
    if (keycode >= QK_MOMENTARY && keycode <= QK_MOMENTARY_MAX) {
        return QK_MOMENTARY_GET_LAYER(keycode);
    }
    if (keycode >= QK_TOGGLE_LAYER && keycode <= QK_TOGGLE_LAYER_MAX) {
        return QK_TOGGLE_LAYER_GET_LAYER(keycode);
    }
    return 0;
}

static HSV make_hsv(uint8_t h, uint8_t s, uint8_t v) {
    HSV hsv = {h, s, v};
    return hsv;
}

static HSV make_hsv_val(uint8_t h, uint8_t s, uint8_t v, uint8_t val) {
    HSV hsv = {h, s, val};
    (void)v; // original v from constant is overridden by val
    return hsv;
}

static HSV get_category_hsv(key_category_t cat, HSV layer_hsv, uint8_t val, uint16_t keycode) {
    switch (cat) {
        case KC_CAT_OFF:
            return make_hsv(HSV_BLACK);
        case KC_CAT_TARGET_LAYER: {
            HSV target = layer_to_hsv(get_target_layer(keycode));
            target.v = val;
            return target;
        }
        case KC_CAT_ARROW:
            return make_hsv_val(HSV_WHITE, val);
        case KC_CAT_PAGE:
            return make_hsv_val(HSV_YELLOW, val);
        case KC_CAT_MEDIA:
            return make_hsv_val(HSV_GOLD, val);
        case KC_CAT_FKEY:
            return make_hsv_val(HSV_PURPLE, val);
        case KC_CAT_EDIT:
            return make_hsv_val(HSV_PINK, val);
        default:
            return make_hsv(layer_hsv.h, layer_hsv.s, val);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    uint8_t val = rgblight_get_val();
    HSV layer_hsv = layer_to_hsv(layer);

    // Set all LEDs to layer color first (covers underglow)
    RGB layer_rgb = hsv_to_rgb((HSV){layer_hsv.h, layer_hsv.s, val});
    for (uint8_t i = led_min; i <= led_max; i++) {
        rgb_matrix_set_color(i, layer_rgb.r, layer_rgb.g, layer_rgb.b);
    }

    // Override per-key colors
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index == NO_LED || led_index < led_min || led_index > led_max) {
                continue;
            }

            uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){.col = col, .row = row});
            key_category_t cat = categorize_keycode(keycode, layer);

            if (cat == KC_CAT_OFF) {
                rgb_matrix_set_color(led_index, 0, 0, 0);
            } else if (cat != KC_CAT_LAYER) {
                HSV hsv = get_category_hsv(cat, layer_hsv, val, keycode);
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
            }
        }
    }
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
