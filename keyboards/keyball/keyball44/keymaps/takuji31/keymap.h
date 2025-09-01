#pragma once
enum keyball_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_ADJUST,
    LAYER_MOUSE,
    LAYER_SYM,
    LAYER_DEVICE,
};

#define TM_LNG1 LT(LAYER_LOWER, KC_LNG1)
#define TM_LNG2 LT(LAYER_SYM, KC_LNG2)
#define TM_SPC  LGUI_T(KC_SPC)
#define RAISE   LT(LAYER_RAISE,KC_ENT)
#define TM_ENT  RCTL_T(KC_LNG2)
#define MO_DEV  MO(LAYER_DEVICE)
