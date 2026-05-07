#pragma once
enum keyball_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAV,
    LAYER_FUN,
    LAYER_MOUSE,
    LAYER_SYM,
    LAYER_DEVICE,
};

#define MOU_LNG2 LT(LAYER_MOUSE, KC_LNG2)
#define SYM_LNG1 LT(LAYER_SYM, KC_LNG1)
#define TM_SPC   LT(LAYER_NAV, KC_SPC)
#define SFT_TAB  LSFT_T(KC_TAB)
#define FUN      LT(LAYER_FUN, KC_ENT)
#define MO_DEV   MO(LAYER_DEVICE)

// Home Row Mods - Windows (GACS)
#define HRM_A LGUI_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LCTL_T(KC_S)
#define HRM_T LSFT_T(KC_T)
#define HRM_N RSFT_T(KC_N)
#define HRM_E RCTL_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RGUI_T(KC_O)
