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

#define TM_LCTL LCTL_T(KC_LNG2)
#define LOWER   LT(LAYER_LOWER,KC_SPC)
#define MO_SYM  LT(LAYER_SYM,KC_LNG1)
#define RAISE   LT(LAYER_RAISE,KC_ENT)
#define TM_ENT  RCTL_T(KC_LNG2)
#define MO_DEV  MO(LAYER_DEVICE)
