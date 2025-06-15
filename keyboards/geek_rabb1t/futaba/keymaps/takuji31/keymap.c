// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>
#include "gr_trackpad65_driver.h"

enum layer_number {
    _BASE = 0,
    _WBASE,
    _LOWER,
    _RAISE,
    _ADJUST,
    _WLOWER,
    _WRAISE,
    _WADJUST,

    _DEVICE = 15
};

enum my_keycodes {
  HIGH_SPEED = QK_KB_0,
  LOW_SPEED,
  TGL_V_SCL,
  TGL_H_SCL,
  EN_3_TAP,
  DIS_3_TAP,
};


#define MO_DEV MO(_DEVICE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC , KC_Q,    KC_W,    KC_F,            KC_P,           KC_G, LSG(KC_5),  KC_F5,  KC_F2, KC_J,    KC_L,           KC_U,             KC_Y,    KC_QUOT,  KC_EQL,
        KC_TAB , KC_A,    KC_R,    KC_S,            KC_T,           KC_D,                            KC_H,    KC_N,           KC_E,             KC_I,    KC_O,    KC_MINS,
        KC_LSFT, KC_Z,    KC_X,    KC_C,            KC_V,           KC_B,                            KC_K,    KC_M,           KC_COMM,          KC_DOT,  KC_SLSH, KC_SCLN,
                 XXXXXXX, TD(0), LALT_T(KC_DEL), LGUI_T(KC_SPC), LT(_LOWER, KC_SPC),         LOW_SPEED,      LT(_RAISE, KC_RGUI),  KC_ENT,  KC_BSPC,   KC_LBRC, KC_RBRC
               , G(KC_TAB), KC_BTN5, G(KC_D), KC_BTN4, G(KC_TAB), G(C(KC_RGHT)), G(KC_D), G(C(KC_LEFT))
    ),
    [_WBASE] = LAYOUT(
        KC_ESC , KC_Q,    KC_W,    KC_F,            KC_P,           KC_G, KC_PSCR,  KC_F5,   KC_F2,  KC_J,    KC_L,           KC_U,             KC_Y,    KC_QUOT,  KC_EQL,
        KC_TAB , KC_A,    KC_R,    KC_S,            KC_T,           KC_D,                            KC_H,    KC_N,           KC_E,             KC_I,    KC_O,    KC_MINS,
        KC_LSFT, KC_Z,    KC_X,    KC_C,            KC_V,           KC_B,                            KC_K,    KC_M,           KC_COMM,          KC_DOT,  KC_SLSH, KC_SCLN,
                 XXXXXXX, KC_LGUI, LALT_T(KC_DEL), LCTL_T(KC_SPC), LT(_WLOWER, KC_SPC),         LOW_SPEED,      TD(1),  KC_ENT,  KC_BSPC,   KC_LBRC, KC_RBRC
                 , G(KC_TAB), KC_BTN5, G(KC_D), KC_BTN4, G(KC_TAB), G(C(KC_RGHT)), G(KC_D), G(C(KC_LEFT))
    ),

    [_LOWER] = LAYOUT(
        KC_GRV , KC_1   , KC_2   , KC_3    , KC_4   , KC_5   , _______, _______, _______,   KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI , KC_LSFT, _______,                              KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______ ,
        _______, G(KC_Z), G(KC_X), G(KC_C) , G(KC_V), S(G(KC_V)),                           KC_END , KC_PGDN, KC_PGUP, KC_HOME, _______, _______ ,
                 _______, _______, _______ , _______, _______,          _______,            _______, _______, _______, _______, _______
               , _______, _______, _______ , _______, _______, _______, _______, _______
    ),
    [_WLOWER] = LAYOUT(
        KC_GRV , KC_1   , KC_2   , KC_3    , KC_4   , KC_5   , _______, _______, _______,   KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI , KC_LSFT, _______,                              KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______ ,
        _______, G(KC_Z), G(KC_X), G(KC_C) , G(KC_V), S(G(KC_V)),                           KC_END , KC_PGDN, KC_PGUP, KC_HOME, _______, _______ ,
                 _______, _______, _______ , _______, _______,          _______,            _______, _______, _______, _______, _______
               , _______, _______, _______ , _______, _______, _______, _______, _______
    ),
    [_RAISE] = LAYOUT(
        KC_TILD, KC_EXLM, KC_AT  , KC_HASH , KC_DLR , KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
        _______, KC_F1  , KC_F2  , KC_F3   , KC_F4  , KC_F5  ,                            _______, KC_RSFT, KC_LGUI, KC_LALT, KC_RCTL, _______ ,
        _______, KC_F6  , KC_F7  , KC_F8   , KC_F9  , KC_F10 ,                            _______, _______, _______, _______, _______, _______ ,
                 KC_F11 , KC_F12 , _______ , _______, _______,          _______,          _______, _______, _______, _______, _______
               , _______, _______, _______ , _______, _______, _______, _______, _______
    ),
    [_WRAISE] = LAYOUT(
        KC_TILD, KC_EXLM, KC_AT  , KC_HASH , KC_DLR , KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
        _______, KC_F1  , KC_F2  , KC_F3   , KC_F4  , KC_F5  ,                            _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______ ,
        _______, KC_F6  , KC_F7  , KC_F8   , KC_F9  , KC_F10 ,                            _______, _______, _______, _______, _______, _______ ,
                 KC_F11 , KC_F12 , _______ , _______, _______,          _______,          _______, _______, _______, _______, _______
               , _______, _______, _______ , _______, _______, _______, _______, _______
    ),
    [_ADJUST] = LAYOUT(
        MO_DEV , KC_BRID, KC_BRIU, _______  , _______, _______, _______, _______, _______,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,
        _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
        _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
                 _______, _______, _______  , _______, _______,          _______,            _______, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______, _______, _______
    ),
    [_WADJUST] = LAYOUT(
        MO_DEV , KC_BRID, KC_BRIU, _______  , _______, _______, _______, _______, _______,   KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,
        _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
        _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
                 _______, _______, _______  , _______, _______,          _______,            _______, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______, _______, _______
    ),
    // [_LOWER] = LAYOUT(
    //     _______, _______, _______, _______  , _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,
    //     _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
    //     _______, _______, _______, _______  , _______, _______,                              _______, _______, _______, _______, _______, _______ ,
    //              _______, _______, _______  , _______, _______,          _______,            _______, _______, _______, _______, _______
    //            , _______, _______, _______  , _______, _______, _______, _______, _______
    // ),
    // [1] = LAYOUT(
    //     DF(3)  , _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______,
    //     _______, _______, _______, _______, _______, _______,                            KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______,
    //     _______, _______, _______, _______, _______, _______,                            KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, _______, _______,
    //              _______, _______, _______, _______, _______,          _______,          _______, KC_BSPC, _______, _______, _______
    //            , _______, _______, _______, _______, _______, _______, _______, _______
    // ),
    // [2] = LAYOUT(
    //     KC_TILD, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _______, _______, _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
    //     KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                            KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_EQL ,
    //     KC_F11 , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                            KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F12 ,
    //              _______, _______, _______, _______, _______,          KC_KB_MUTE,       _______, _______, _______, _______, _______
    //            , _______, _______, _______, _______, _______, _______, _______, _______
    // ),
    [_DEVICE] = LAYOUT(
        _______, _______, _______, EN_3_TAP , _______, _______, TGL_V_SCL, _______, TGL_H_SCL, _______, _______, _______, _______, _______,  QK_BOOT ,
        _______, _______, _______, DIS_3_TAP, _______, _______,                                _______, _______, _______, _______, _______, _______ ,
        _______, _______, _______, _______  , _______, _______,                                _______, _______, _______, _______, _______, _______ ,
                 _______, _______, _______  , _______, _______,            DF(0)  ,            _______, _______, _______, _______, _______
               , _______, _______, _______  , _______, _______, _______, _______, _______
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] =   { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_WBASE] =   { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [_LOWER] =   { ENCODER_CCW_CW(S(C(KC_TAB)), C(KC_TAB)) },
    [_WLOWER] =   { ENCODER_CCW_CW(S(C(KC_TAB)), C(KC_TAB)) },
    [_RAISE] =   { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_WRAISE] =   { ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_ADJUST] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    [_WADJUST] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    [_DEVICE] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [9] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [10] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [11] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [12] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [13] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [14] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
    // [15] =   { ENCODER_CCW_CW(KC_UP, KC_DOWN) },
};

void install_tap_dance_entries(void) {
    vial_tap_dance_entry_t my_tap_dance_entries[] = {
        [0] = { G(KC_SPC), KC_LCTL, A(KC_SPC), XXXXXXX, TAPPING_TERM },
        [1] = { A(KC_GRV), MO(_WRAISE), XXXXXXX, XXXXXXX, TAPPING_TERM },
    };
    uint8_t length = sizeof(my_tap_dance_entries) / sizeof(my_tap_dance_entries[0]);
    for (uint8_t i = 0; i < length; i++) {
        dynamic_keymap_set_tap_dance(i, &my_tap_dance_entries[i]);
    }
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    //debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = true;
    #ifdef TAP_DANCE_ENABLE
    install_tap_dance_entries();
    #endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    state = update_tri_layer_state(state, _WLOWER, _WRAISE, _WADJUST);
    return state;
}

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

typedef enum  {
    SPEED_MODE_LOW,
    SPEED_MODE_NORMAL,
    SPEED_MODE_HIGH
} speed_mode_t;

static speed_mode_t speed_mode = SPEED_MODE_NORMAL;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HIGH_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_HIGH;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case LOW_SPEED:
      if (record->event.pressed) {
        speed_mode = SPEED_MODE_LOW;
      } else {
        speed_mode = SPEED_MODE_NORMAL;
      }
      return true;

    case TGL_V_SCL:
      if (record->event.pressed) {
        trackpad_config.reverse_vertical_scroll = !trackpad_config.reverse_vertical_scroll;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case TGL_H_SCL:
      if (record->event.pressed) {
        trackpad_config.reverse_horizontal_scroll = !trackpad_config.reverse_horizontal_scroll;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case EN_3_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_3fingers_tap = false;
        update_trackpad_config(trackpad_config);
      }
      return false;

    case DIS_3_TAP:
      if (record->event.pressed) {
        trackpad_config.disable_3fingers_tap = true;
        update_trackpad_config(trackpad_config);
      }
      return false;

    default:
      return true;
  }
}

int LOW_SPEED_RATIO = 2;
int HIGH_SPEED_RATIO = 2;
#define CONSTRAIN_HID_XY(amt) ((amt) < XY_REPORT_MIN ? XY_REPORT_MIN : ((amt) > XY_REPORT_MAX ? XY_REPORT_MAX : (amt)))

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

    if (speed_mode == SPEED_MODE_LOW) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x / LOW_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y / LOW_SPEED_RATIO);
    }

    if (speed_mode == SPEED_MODE_HIGH) {
        mouse_report.x = CONSTRAIN_HID_XY(mouse_report.x * HIGH_SPEED_RATIO);
        mouse_report.y = CONSTRAIN_HID_XY(mouse_report.y * HIGH_SPEED_RATIO);
    }

    return mouse_report;
}
