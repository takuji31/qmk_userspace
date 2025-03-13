/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include "quantum.h"
#include "keymap.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default
  [LAYER_BASE] = LAYOUT_right_ball(
    KC_ESC   , KC_Q     , KC_W     , KC_P     , KC_F     , KC_B     ,                                        KC_J     , KC_L     , KC_U     , KC_Y     , KC_QUOT  , KC_EQL   ,
    KC_TAB   , KC_A     , KC_R     , KC_S     , KC_T     , KC_G     ,                                        KC_M     , KC_N     , KC_E     , KC_I     , KC_O     , KC_MINS  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_D     , KC_V     ,                                        KC_K     , KC_H     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_SCLN  ,
                          KC_LALT  , KC_LGUI  , TM_LCTL  , LOWER    , MO_SYM  ,                    KC_BSPC , RAISE    ,                                  MO_DEV
  ),

  [LAYER_LOWER] = LAYOUT_right_ball(
     _______ , KC_F1    , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10  , KC_F11   ,
     _______ , KC_LGUI  , KC_LALT  , KC_LCTL , KC_LSFT  , KC_WFWD  ,                                         KC_PGUP  , KC_LEFT  , KC_UP    , KC_RGHT  , _______ , KC_F12   ,
     _______ , C(KC_Z)  , C(KC_X)  , C(KC_C) , C(KC_V)  , KC_WBAK  ,                                         KC_PGDN  , KC_HOME  , KC_DOWN  , KC_END   , _______ , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                     KC_DEL , _______  ,                                  _______
  ),

  [LAYER_RAISE] = LAYOUT_right_ball(
    _______  , _______  , KC_7     , KC_8    , KC_9     , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , KC_4     , KC_5    , KC_6     , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , KC_1     , KC_2    , KC_3     , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
                          KC_0     , KC_DOT  , _______  , _______  , _______  ,                     KC_DEL , _______  ,                                  _______
  ),

  [LAYER_ADJUST] = LAYOUT_right_ball(
    _______  , KC_BRID  , KC_BRIU  , _______ , _______  , _______  ,                                         KC_MPRV  , KC_MPLY  , KC_MNXT  , KC_MUTE  , KC_VOLD  , KC_VOLU  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                   _______  , _______  ,                                  _______
  ),

  [LAYER_SYM] = LAYOUT_right_ball(
    KC_GRV   , KC_QUOT  , KC_LT    , KC_GT   , KC_DQUO  , KC_DOT   ,                                         KC_AMPR  , _______  , KC_LBRC  , KC_RBRC  , KC_PERC  , _______  ,
    _______  , KC_EXLM  , KC_MINS  , KC_PLUS , KC_EQL   , KC_HASH  ,                                         KC_PIPE  , KC_COLN  , KC_LPRN  , KC_RPRN  , KC_QUES  , _______  ,
    _______  , KC_CIRC  , KC_SLSH  , KC_ASTR , KC_BSLS  , _______  ,                                         KC_TILD  , KC_DLR   , KC_LCBR  , KC_RCBR  , KC_AT    , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                   _______  , _______  ,                                  _______
  ),

  [LAYER_MOUSE] = LAYOUT_right_ball(
    SSNP_FRE ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    SSNP_VRT ,  _______ , _______  , KC_UP   , KC_ENT   , KC_DEL   ,                                         KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    SSNP_HOR ,  _______ , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BSPC  ,                                         KC_PGDN  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                   _______  , _______  ,                                  _______
),

  [LAYER_DEVICE] = LAYOUT_right_ball(
    _______  , AML_TO   , AML_I50  , AML_D50  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , KC_PSCR  , _______  ,
    _______  , _______  , _______  , _______  , _______  , SCRL_DVI ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
                          QK_BOOT  , KBC_RST  , _______  , _______  , _______  ,                   _______ , _______  ,                                  _______
  ),

};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, LAYER_LOWER, LAYER_RAISE, LAYER_ADJUST);


    uint8_t layer = get_highest_layer(state);
    keyball_set_scroll_mode(layer == LAYER_SYM);
    switch (layer) {
        case LAYER_BASE:
            rgblight_sethsv(HSV_BLACK);
            break;
        case LAYER_LOWER:
            rgblight_sethsv(HSV_BLUE);
            break;
        case LAYER_RAISE:
            rgblight_sethsv(HSV_RED);
            break;
        case LAYER_MOUSE:
            rgblight_sethsv(HSV_GREEN);
            break;
        default:
            rgblight_sethsv(HSV_CORAL);
            break;
    }
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef TAP_DANCE_ENABLE
tap_dance_action_t tap_dance_actions[] = {
};
#endif

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_layer(LAYER_MOUSE);
}
#endif
