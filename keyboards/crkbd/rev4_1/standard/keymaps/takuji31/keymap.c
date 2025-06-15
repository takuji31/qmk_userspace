#include QMK_KEYBOARD_H


#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(S(C(KC_TAB)), C(KC_TAB)), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(MS_WHLU, MS_WHLD), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [1] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [2] = { ENCODER_CCW_CW(KC_BRID, KC_BRIU), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [3] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [4] = { ENCODER_CCW_CW(MS_WHLL, MS_WHLR), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(MS_WHLU, MS_WHLD), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [5] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [6] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
  [7] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(XXXXXXX, XXXXXXX), },
};
#endif

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgblight_sethsv_noeeprom(HSV_WHITE);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, 1, 2, 3);
    state = update_tri_layer_state(state, 4, 5, 6);

    switch (get_highest_layer(state)) {
        case 0:
            rgblight_sethsv_noeeprom(HSV_WHITE);
            break;
        case 1:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_GREEN);
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_ORANGE);
            break;
        case 5:
            rgblight_sethsv_noeeprom(HSV_CHARTREUSE);
            break;
        case 6:
            rgblight_sethsv_noeeprom(HSV_CYAN);
            break;
        case 7:
            rgblight_sethsv_noeeprom(HSV_PINK);
            break;
        default:
            rgblight_sethsv_noeeprom(HSV_BLACK);
            break;
    }
    return state;
};
