#include "keymap.h"
#include QMK_KEYBOARD_H

static td_state_t td_state;
td_state_t cur_dance(tap_dance_state_t *state);
void altlp_finished(tap_dance_state_t *state, void *user_data);
void altlp_reset(tap_dance_state_t *state, void *user_data);

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

// region Tap Dance codes

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

// Handle the possible states for each tapdance keycode you define:

void mac_alt_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_mods(MOD_BIT(KC_LGUI));
            tap_code16(KC_SPC);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LALT));
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(KC_SPC);
            break;
        default:
            break;
    }
}

void mac_alt_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_mods(MOD_BIT(KC_LGUI));
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LALT));
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_mods(MOD_BIT(KC_LALT));
            break;
        default:
            break;
    }
}

void mac_shift_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code16(KC_RGUI);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_RSFT));
            break;
        default:
            break;
    }
}

void mac_shift_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_RSFT));
            break;
        default:
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [MAC_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mac_alt_finished, mac_alt_reset),
    [MAC_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mac_shift_finished, mac_shift_reset),
};

// endregion
