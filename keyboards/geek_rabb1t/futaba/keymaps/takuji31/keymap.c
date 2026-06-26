// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "../trackpad/gr_trackpad65_config.h"

enum layer_number {
    _BASE = 0,
    _NAV,
    _SYM,
    _FN,
    _SYS = 8,
};

// Home Row Mods (CAGS)
#define HRM_A LCTL_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LGUI_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N RSFT_T(KC_N)
#define HRM_E LGUI_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RCTL_T(KC_O)

// Thumb keys
#define SYM_LNG2 LT(_SYM, KC_LNG2)
#define NAV_SPC  LT(_NAV, KC_SPC)
#define SFT_TAB  MT(MOD_LSFT, KC_TAB)
#define FN_ENT   LT(_FN, KC_ENT)
#define SYS_LNG1 LT(_SYS, KC_LNG1)

// NAV shortcuts
#define M_UNDO  G(KC_Z)
#define M_CUT   G(KC_X)
#define M_COPY  G(KC_C)
#define M_PASTE G(KC_V)
#define M_PSTM  S(G(KC_V))

#define TRNS_LAYER LAYOUT( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______, \
             _______, _______, _______, _______, _______,    _______,    _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______ \
)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC , KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    S(C(KC_TAB)), LSG(KC_5), C(KC_TAB), KC_J,   KC_L,    KC_U,    KC_Y,    KC_QUOT, KC_EQL,
        KC_TAB , HRM_A,   HRM_R,   HRM_S,   HRM_T,   KC_D,                              KC_H,    HRM_N,   HRM_E,   HRM_I,   HRM_O,   KC_MINS,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                              KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SCLN,
                 XXXXXXX, KC_LCTL, SYM_LNG2, NAV_SPC, SFT_TAB,    LOW_SPEED,    KC_BSPC, FN_ENT, SYS_LNG1, KC_LBRC, KC_RBRC,

        MS_BTN1, KC_NO, KC_NO, KC_NO, KC_NO,
        MS_BTN2, KC_NO, KC_NO, KC_NO, KC_NO,
        MS_BTN3, G(KC_TAB), KC_BTN5, G(KC_D), KC_BTN4,
        G(KC_S), G(KC_TAB), G(C(KC_RGHT)), G(KC_D), G(C(KC_LEFT)),
        GRTL_CS, GRTL_SS, GRTL_CC, GRTL_CA, GRTL_RVS, GRTL_RHS, GRTL_IC, GRTL_IS, GRTL_SO,
        GRT_VAL_5, GRT_VAL_4, GRT_VAL_2, GRT_OFF, GRT_ON, GRT_OFF, GRT_ON, GRT_ON, GRT_OFF,
        GRTL_1T, GRTL_2T, GRTL_3T, GRTL_4T
    ),
    [_NAV] = LAYOUT(
        XXXXXXX, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______, _______, _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
        XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
        XXXXXXX, M_UNDO,  M_CUT,   M_COPY,  M_PASTE, M_PSTM,                             KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, XXXXXXX,
                 _______, _______, _______, _______, _______,    _______,    _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [_SYM] = LAYOUT(
        XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,  XXXXXXX,
        XXXXXXX, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,                            KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS, XXXXXXX,
                 _______, _______, _______, _______, _______,    _______,    KC_DEL,  KC_ENT,  _______, _______, _______,

        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [_FN] = LAYOUT(
        XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, XXXXXXX,
        XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              XXXXXXX, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL, XXXXXXX,
        XXXXXXX, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 KC_F11,  KC_F12,  _______, KC_SPC,  KC_TAB,     _______,    _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
    [4] = TRNS_LAYER,
    [5] = TRNS_LAYER,
    [6] = TRNS_LAYER,
    [7] = TRNS_LAYER,
    [_SYS] = LAYOUT(
        XXXXXXX, KC_BRID, KC_BRIU, LSG(KC_3), LSG(KC_4), LSG(KC_5), _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,   XXXXXXX,   TG(_NAV),                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,                             XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT, XXXXXXX,
                 _______, _______, _______, _______, _______,    _______,    _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______
    ),
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    '*', 'L', 'L', 'L', 'L', 'L', '*', '*', '*', 'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                'R', 'R', 'R', 'R', 'R', '*',
         '*', '*', '*', '*', '*',      '*',      '*', '*', '*', '*', '*',

    '*', '*', '*', '*', '*',
    '*', '*', '*', '*', '*',
    '*', '*', '*', '*', '*',
    '*', '*', '*', '*', '*',
    '*', '*', '*', '*', '*', '*', '*', '*', '*',
    '*', '*', '*', '*', '*', '*', '*', '*', '*',
    '*', '*', '*', '*'
);

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_NAV]  = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_SYM]  = { ENCODER_CCW_CW(G(S(KC_TAB)), G(KC_TAB)) },
    [_FN]   = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [4]     = { ENCODER_CCW_CW(_______, _______) },
    [5]     = { ENCODER_CCW_CW(_______, _______) },
    [6]     = { ENCODER_CCW_CW(_______, _______) },
    [7]     = { ENCODER_CCW_CW(_______, _______) },
    [_SYS]  = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
};
