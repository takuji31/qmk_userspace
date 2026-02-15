#include QMK_KEYBOARD_H

// Layer definitions
enum layer_number {
    _BASE = 0,
    _MOUSE,
    _NAV,
    _NUM,
    _SYM,
    _FN,
    _SYS,
};

// Home Row Mods - Windows (GACS)
#define HRM_A LGUI_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LCTL_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N RSFT_T(KC_N)
#define HRM_E RCTL_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RGUI_T(KC_O)

// Thumb keys
#define SYM_ESC  LT(_SYM, KC_ESC)
#define NAV_SPC  LT(_NAV, KC_SPC)
#define SFT_TAB  MT(MOD_LSFT, KC_TAB)
#define FN_BSPC  LT(_FN, KC_BSPC)
#define NUM_ENT  LT(_NUM, KC_ENT)


// Shortcuts for NAV layer - Windows
#define W_UNDO  C(KC_Z)
#define W_CUT   C(KC_X)
#define W_COPY  C(KC_C)
#define W_PASTE C(KC_V)
#define W_PSTM  S(C(KC_V))

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_right_ball(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_MINS,
        HRM_A,   HRM_R,   HRM_S,   HRM_T,   KC_D,                      KC_H,    HRM_N,   HRM_E,   HRM_I,   HRM_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
        KC_LNG2, KC_LGUI, KC_LNG1, SYM_ESC, NAV_SPC, SFT_TAB, FN_BSPC, NUM_ENT,                            MO(_SYS)
    ),

    [_MOUSE] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,                   _______, MS_BTN1, MS_BTN3, MS_BTN2, _______,
        _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,                            _______
    ),

    [_NAV] = LAYOUT_right_ball(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
        W_UNDO,  W_CUT,   W_COPY,  W_PASTE, W_PSTM,                    KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX,
        _______, _______, _______, _______, _______, _______, KC_BSPC, KC_ENT,                             _______
    ),

    [_NUM] = LAYOUT_right_ball(
        KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, _______, _______, KC_DOT,  KC_0,    KC_TAB,  _______, _______,                            XXXXXXX
    ),

    [_SYM] = LAYOUT_right_ball(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_PLUS, KC_EQL,
        KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                   KC_DQUO, KC_LPRN, KC_RPRN, KC_COLN, KC_QUOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_SCLN, KC_LBRC, KC_RBRC, KC_GRV,  KC_BSLS,
        _______, _______, _______, _______, _______, _______,  KC_DEL, KC_ENT,                             _______
    ),

    [_FN] = LAYOUT_right_ball(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                    XXXXXXX, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, _______, _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______,                            XXXXXXX
    ),

    [_SYS] = LAYOUT_right_ball(
        KC_BRID, KC_BRIU, KC_MPRV, KC_MPLY, KC_MNXT,                  XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU,
        XXXXXXX, XXXXXXX, KC_PSCR, XXXXXXX, TG(_NAV),                 AML_TO,  AML_I50, AML_D50, XXXXXXX, XXXXXXX,
        SSNP_FRE,SSNP_VRT,SSNP_HOR,XXXXXXX, XXXXXXX,                  CPI_D100,SCRL_DVD,SCRL_DVI,CPI_I100,KBC_SAVE,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT,                           KBC_RST
    ),
};
// clang-format on

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_right_ball(
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    '*', '*', '*', '*', '*', '*',      '*', '*',         '*'
);

bool is_mouse_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_LCTL:
        case KC_LSFT:
        case KC_LALT:
        case KC_LGUI:
            return true;
        default:
            return false;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    keyball_set_scroll_mode(layer == _SYM);

    switch (layer) {
        case _BASE:
            rgblight_sethsv(HSV_CYAN);
            break;
        case _MOUSE:
            rgblight_sethsv(HSV_RED);
            break;
        case _NAV:
            rgblight_sethsv(HSV_GREEN);
            break;
        case _NUM:
            rgblight_sethsv(HSV_CORAL);
            break;
        case _SYM:
            rgblight_sethsv(HSV_YELLOW);
            break;
        case _FN:
            rgblight_sethsv(HSV_BLUE);
            break;
        case _SYS:
            rgblight_sethsv(HSV_PURPLE);
            break;
        default:
            rgblight_sethsv(HSV_CORAL);
            break;
    }

    return state;
}

#ifdef OLED_ENABLE
#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE);
}
#endif

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
