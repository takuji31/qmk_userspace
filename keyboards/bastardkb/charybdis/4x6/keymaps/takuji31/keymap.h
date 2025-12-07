enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER = 4,
    LAYER_RAISE = 5,
    LAYER_ADJUST = 6,
    LAYER_MODE = 14,
    LAYER_POINTER = 15,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
//#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define WLOWER MO(LAYER_LOWER)
#define WRAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE
