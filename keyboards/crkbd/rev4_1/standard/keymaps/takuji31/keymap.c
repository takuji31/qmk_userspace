#include QMK_KEYBOARD_H

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    MAC_ALT = 0,
};

td_state_t cur_dance(tap_dance_state_t *state);

void mac_alt_finished(tap_dance_state_t *state, void *user_data);
void mac_alt_reset(tap_dance_state_t *state, void *user_data);

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

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicative that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currently not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustrating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t alttap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void mac_alt_finished(tap_dance_state_t *state, void *user_data) {
    alttap_state.state = cur_dance(state);
    switch (alttap_state.state) {
        case TD_SINGLE_TAP:  register_mods(MOD_BIT(KC_LGUI)); register_code(KC_SPC); break;
        case TD_SINGLE_HOLD: register_code(KC_LALT); break;
        case TD_DOUBLE_TAP: register_mods(MOD_BIT(KC_LALT)); register_code(KC_SPC); break;
        default: break;
    }
}

void mac_alt_reset(tap_dance_state_t *state, void *user_data) {
    switch (alttap_state.state) {
        case TD_SINGLE_TAP:  unregister_mods(MOD_BIT(KC_LGUI)); unregister_code(KC_SPC); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LALT); break;
        case TD_DOUBLE_TAP: unregister_mods(MOD_BIT(KC_LALT)); unregister_code(KC_SPC); break;
        default: break;
    }
    alttap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [MAC_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mac_alt_finished, mac_alt_reset),
};
