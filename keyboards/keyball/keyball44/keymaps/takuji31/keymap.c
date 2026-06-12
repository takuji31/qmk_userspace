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
    KC_ESC   , KC_Q     , KC_W     , KC_F     , KC_P     , KC_G     ,                                        KC_J     , KC_L     , KC_U     , KC_Y     , KC_QUOT  , KC_EQL   ,
    KC_TAB   , HRM_A    , HRM_R    , HRM_S    , HRM_T    , KC_D     ,                                        KC_H     , HRM_N    , HRM_E    , HRM_I    , HRM_O    , KC_MINS  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_K     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_SCLN  ,
                          KC_LGUI  , MOU_LNG2 , SYM_LNG1 , TM_SPC   , SFT_TAB ,                    KC_BSPC , FUN      ,                                  MO_DEV
  ),

  [LAYER_NAV] = LAYOUT_right_ball(
     KC_GRV  , KC_1     , KC_2     , KC_3    , KC_4     , KC_5     ,                                         KC_6     , KC_7     , KC_8     , KC_9     , KC_0    , KC_BSLS  ,
     XXXXXXX , KC_LGUI  , KC_LALT  , KC_LCTL , KC_LSFT  , KC_WFWD  ,                                         KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , XXXXXXX , KC_F12   ,
     XXXXXXX , XXXXXXX  , XXXXXXX  , XXXXXXX , XXXXXXX  , KC_WBAK  ,                                         KC_HOME  , KC_PGDN  , KC_PGUP  , KC_END   , XXXXXXX , XXXXXXX  ,
                          _______  , _______ , _______  , _______  , _______  ,                     KC_DEL , _______  ,                                  _______
  ),

  [LAYER_FUN] = LAYOUT_right_ball(
    KC_TILD  , XXXXXXX  , XXXXXXX  , XXXXXXX , XXXXXXX  , XXXXXXX  ,                                         XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , KC_PIPE  ,
    XXXXXXX  , KC_F1    , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                         KC_RSFT  , KC_LGUI  , KC_LALT  , KC_RCTL  , XXXXXXX  , XXXXXXX  ,
    XXXXXXX  , KC_F6    , KC_F7    , KC_F8   , KC_F9    , KC_F10   ,                                         XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
                          KC_F11   , KC_F12  , _______  , _______  , _______  ,                     KC_DEL , _______  ,                                  _______
  ),

  [LAYER_SYM] = LAYOUT_right_ball(
    _______  , KC_EXLM  , KC_AT    , KC_HASH , KC_DLR   , KC_PERC  ,                                         KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_PLUS  , KC_EQL   , _______  ,
    _______  , KC_LGUI  , KC_LALT  , KC_LCTL , KC_LSFT  , XXXXXXX  ,                                         KC_DQUO  , KC_LPRN  , KC_RPRN  , KC_COLN  , KC_QUOT  , _______  ,
    _______  , XXXXXXX  , XXXXXXX  , XXXXXXX , XXXXXXX  , XXXXXXX  ,                                         KC_SCLN  , KC_LBRC  , KC_RBRC  , KC_GRV   , KC_BSLS  , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                    KC_DEL , _______  ,                                  _______
  ),

  [LAYER_MOUSE] = LAYOUT_right_ball(
    _______  ,  _______ , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  ,  _______ , _______  , _______ , _______  , _______  ,                                         _______  , MS_BTN1  , MS_BTN3  , MS_BTN2  , _______  , _______  ,
    _______  ,  _______ , _______  , _______ , _______  , _______  ,                                         _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______ , _______  , _______  , _______  ,                   _______  , _______  ,                                  _______
),

  [LAYER_DEVICE] = LAYOUT_right_ball(
    SSNP_FRE , KC_BRID  , KC_BRIU  , _______  , KC_PSCR  , _______  ,                                        KC_MPRV  , KC_MPLY  , KC_MNXT  , KC_MUTE  , KC_VOLD  , KC_VOLU  ,
    SSNP_VRT , AML_TO   , AML_I50  , AML_D50  , _______  , SCRL_DVI ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    SSNP_HOR , _______  , _______  , _______  , _______  , SCRL_DVD ,                                        CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , _______  , KBC_SAVE ,
                          QK_BOOT  , KBC_RST  , _______  , _______  , _______  ,                   _______ , _______  ,                                  _______
  ),

};
// clang-format on

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_right_ball(
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
    '*', 'L', 'L', 'L', 'L', 'L',                            'R', 'R', 'R', 'R', 'R', '*',
                   '*', '*', '*', '*', '*',           '*',   '*',                     '*'
);

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
            return 280;
        default:
            return TAPPING_TERM;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    keyball_set_scroll_mode(layer == LAYER_SYM);
    switch (layer) {
        case LAYER_BASE:
            rgblight_sethsv(HSV_ORANGE);
            break;
        case LAYER_NAV:
            rgblight_sethsv(HSV_BLUE);
            break;
        case LAYER_FUN:
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
