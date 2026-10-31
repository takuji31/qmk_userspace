RAW_ENABLE = yes

VIA_ENABLE = no

DYNAMIC_KEYMAP_ENABLE = yes

MOUSEKEY_ENABLE = no

OLED_ENABLE = yes

RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE = no
EXTRAKEY_ENABLE = yes
OPT_DEFS += -DGESTURE_ENABLE

CONSOLE_ENABLE = yes

TAP_DANCE_ENABLE = yes

AUTO_SHIFT_ENABLE = yes

COMBO_ENABLE = yes

OS_DETECTION_ENABLE = yes

SRC += lib/keyball/kb_hid.c
SRC += lib/keyball/kb_settings.c
SRC += lib/keyball/kb_macro.c
SRC += lib/keyball/kb_combo.c
SRC += lib/keyball/td_config.c

SRC += lib/oledkit/anim_frames.c
