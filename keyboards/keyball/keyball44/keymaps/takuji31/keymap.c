// Copyright 2024 keyball-custom contributors
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "lib/keyball/kb_hid.h"
#include "lib/keyball/td_config.h"
#include "lib/keyball/kb_settings.h"
#include "keymap.h"
#ifdef COMBO_ENABLE
#include "lib/keyball/kb_combo.h"
#endif
#ifdef OS_DETECTION_ENABLE
#include "os_detection.h"
#endif
#ifdef CONSOLE_ENABLE
#include "eeprom.h"
#endif
#ifndef LED_VERSION_BUILD
#include "lib/keyball/kb_macro.h"
#endif

#ifdef KEYBALL_AML_THRESHOLD_RUNTIME
extern uint8_t kb_aml_threshold;
#endif

#if !defined(COMBO_ENABLE) && !defined(REPEAT_KEY_ENABLE)
#    error "kb_synth_keyevent needs keyrecord_t.keycode: keep COMBO_ENABLE=yes in rules.mk"
#endif

static void kb_synth_keyevent(uint16_t kc, bool pressed) {
    if (!kc) return;
    keyrecord_t record = {
        .event   = MAKE_COMBOEVENT(pressed),
        .keycode = kc,
    };
#ifndef NO_ACTION_TAPPING
    action_tapping_process(record);
#else
    process_record(&record);
#endif
}

static void kb_fire_keycode(uint16_t kc) {
    kb_synth_keyevent(kc, true);
    kb_synth_keyevent(kc, false);
}

#ifdef GESTURE_ENABLE
static int8_t   g_gst_manual_mode = -1;
static int8_t   g_gst_layer_mode  = -1;
static int16_t  g_gesture_acc_x   = 0;
static int16_t  g_gesture_acc_y   = 0;
static bool     g_gesture_cooldown = false;
static uint16_t g_gesture_cd_timer = 0;
#define GST_COOLDOWN_MS 350

static inline int8_t gst_active_mode(void) {
    return g_gst_manual_mode >= 0 ? g_gst_manual_mode : g_gst_layer_mode;
}
#endif

static int8_t   g_shake_last_sign = 0;
static uint8_t  g_shake_reversals = 0;
static uint16_t g_shake_abs       = 0;
static uint16_t g_shake_run_start;
static uint16_t g_shake_last_move_time;
static bool     g_shake_cooldown = false;
static uint16_t g_shake_cd_timer;

static void shake_task(const report_mouse_t *mouse_report) {
    if (!kb_shake_enable_get()) return;

    const uint16_t SHAKE_GAP_MS      = 200;
    const uint16_t SHAKE_COOLDOWN_MS = 500;
    uint8_t        shake_reversals_needed = kb_shake_reversals_get();
    uint16_t       shake_run_max_ms       = kb_shake_run_max_ms_get();

    int8_t x = mouse_report->x;
    int8_t y = mouse_report->y;

    if (x == 0 && y == 0) {
        if (timer_elapsed(g_shake_last_move_time) > SHAKE_GAP_MS) {
            g_shake_reversals = 0;
            g_shake_abs       = 0;
            g_shake_last_sign = 0;
        }
    } else {
        g_shake_last_move_time = timer_read();
        int8_t ax       = x < 0 ? -x : x;
        int8_t ay       = y < 0 ? -y : y;
        int8_t dominant = (ax >= ay) ? x : y;
        int8_t sign     = dominant > 0 ? 1 : (dominant < 0 ? -1 : 0);

        if (sign != 0) {
            if (g_shake_last_sign != 0 && sign != g_shake_last_sign) {
                if (g_shake_reversals == 0) {
                    g_shake_run_start = timer_read();
                } else if (timer_elapsed(g_shake_run_start) > shake_run_max_ms) {
                    g_shake_reversals = 0;
                    g_shake_abs       = 0;
                    g_shake_run_start = timer_read();
                }
                g_shake_reversals++;
#ifdef CONSOLE_ENABLE
                dprintf("kb_debug: shake reversal=%u abs=%u key=%u threshold=%u elapsed=%u\n", g_shake_reversals, g_shake_abs, kb_shake_key_get(), kb_shake_threshold_get(), timer_elapsed(g_shake_run_start));
#endif
            }
            g_shake_last_sign = sign;
        }
        g_shake_abs += (uint16_t)(ax + ay);

        uint16_t key = kb_shake_key_get();
        if (key && !g_shake_cooldown && g_shake_reversals >= shake_reversals_needed && g_shake_abs >= kb_shake_threshold_get() && timer_elapsed(g_shake_run_start) <= shake_run_max_ms) {
#ifdef CONSOLE_ENABLE
            dprintf("kb_debug: shake FIRE key=%u\n", key);
#endif
            kb_fire_keycode(key);
            g_shake_cooldown  = true;
            g_shake_cd_timer  = timer_read();
            g_shake_reversals = 0;
            g_shake_abs       = 0;
            g_shake_last_sign = 0;
        }
    }

    if (g_shake_cooldown && timer_elapsed(g_shake_cd_timer) >= SHAKE_COOLDOWN_MS) {
        g_shake_cooldown = false;
    }
}

static bool     g_dflick_streak_active   = false;
static bool     g_dflick_stop_pending    = false;
static uint16_t g_dflick_stop_pending_at = 0;
static uint16_t g_dflick_streak_start    = 0;
static int16_t  g_dflick_sum_x           = 0;
static int16_t  g_dflick_sum_y           = 0;
static int8_t   g_dflick_last_dir        = -1;
static uint16_t g_dflick_last_time       = 0;

static void dflick_task(const report_mouse_t *mouse_report) {
    if (!kb_dflick_enable_get()) return;

    const uint16_t STOP_DEBOUNCE_MS = 30;

    if (mouse_report->x != 0 || mouse_report->y != 0) {
        if (!g_dflick_streak_active) {
            g_dflick_sum_x        = 0;
            g_dflick_sum_y        = 0;
            g_dflick_streak_start = timer_read();
        }
        g_dflick_streak_active = true;
        g_dflick_stop_pending  = false;
        g_dflick_sum_x += mouse_report->x;
        g_dflick_sum_y += mouse_report->y;
        return;
    }

    if (!g_dflick_streak_active) return;

    if (!g_dflick_stop_pending) {
        g_dflick_stop_pending    = true;
        g_dflick_stop_pending_at = timer_read();
        return;
    }
    if (timer_elapsed(g_dflick_stop_pending_at) < STOP_DEBOUNCE_MS) return;

    g_dflick_streak_active  = false;
    g_dflick_stop_pending   = false;
    uint16_t streak_elapsed = timer_elapsed(g_dflick_streak_start);
    if (streak_elapsed > kb_dflick_max_duration_ms_get()) {
#ifdef CONSOLE_ENABLE
        dprintf("kb_debug: dflick REJECT (too long) duration=%u max=%u\n", streak_elapsed, kb_dflick_max_duration_ms_get());
#endif
        return;
    }

    int16_t sx  = g_dflick_sum_x;
    int16_t sy  = g_dflick_sum_y;
    int16_t asx = sx < 0 ? -sx : sx;
    int16_t asy = sy < 0 ? -sy : sy;
    if (asx + asy < kb_dflick_flick_threshold_get()) {
#ifdef CONSOLE_ENABLE
        dprintf("kb_debug: dflick REJECT (too weak) sum=%d threshold=%u\n", asx + asy, kb_dflick_flick_threshold_get());
#endif
        return;
    }

    bool    horizontal = asx > asy;
    uint8_t dir        = horizontal
        ? (sx > 0 ? 3 : 2)
        : (sy > 0 ? 1 : 0);

#ifdef CONSOLE_ENABLE
    dprintf("kb_debug: dflick flick detected dir=%u sx=%d sy=%d last_dir=%d elapsed=%u window=%u\n", dir, (int)sx, (int)sy, g_dflick_last_dir, timer_elapsed(g_dflick_last_time), kb_dflick_window_ms_get());
#endif

    if (g_dflick_last_dir == dir && timer_elapsed(g_dflick_last_time) <= kb_dflick_window_ms_get()) {
        uint16_t dkc = kb_dflick_key_get(dir);
#ifdef CONSOLE_ENABLE
        dprintf("kb_debug: dflick FIRE dir=%u keycode=0x%04X\n", dir, dkc);
#endif
        if (dkc) kb_fire_keycode(dkc);
        g_dflick_last_dir = -1;
    } else {
        g_dflick_last_dir  = dir;
        g_dflick_last_time = timer_read();
    }
}

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef GESTURE_ENABLE
    if (keycode == GST_HOLD || keycode == GST_HOLD2 || keycode == GST_HOLD3 || keycode == GST_HOLD4 ||
        keycode == GST_HOLD5 || keycode == GST_HOLD6) {
        int8_t idx = (keycode == GST_HOLD) ? 0 : (keycode == GST_HOLD2) ? 1 : (keycode == GST_HOLD3) ? 2
                   : (keycode == GST_HOLD4) ? 3 : (keycode == GST_HOLD5) ? 4 : 5;
        if (record->event.pressed) {
            g_gst_manual_mode = idx;
        } else if (g_gst_manual_mode == idx) {
            g_gst_manual_mode = -1;
        }
        g_gesture_acc_x    = 0;
        g_gesture_acc_y    = 0;
        g_gesture_cooldown = false;
        return false;
    }
#endif
#ifndef LED_VERSION_BUILD
    if (keycode >= QK_MACRO && keycode <= QK_MACRO_MAX) {
        if (record->event.pressed) {
            kb_macro_play(keycode - QK_MACRO);
        } else {
            kb_macro_release();
        }
        return false;
    }
#endif
    return true;
}

#if defined(GESTURE_ENABLE) && defined(RGB_MATRIX_ENABLE)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return keyball_gesture_wave_overlay(led_min, led_max);
}
#endif

#ifdef RGB_MATRIX_ENABLE
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return;
    switch (keycode) {
        case QK_UNDERGLOW_TOGGLE:
        case QK_UNDERGLOW_MODE_NEXT:
        case QK_UNDERGLOW_MODE_PREVIOUS:
        case QK_UNDERGLOW_HUE_UP:
        case QK_UNDERGLOW_HUE_DOWN:
        case QK_UNDERGLOW_SATURATION_UP:
        case QK_UNDERGLOW_SATURATION_DOWN:
        case QK_UNDERGLOW_VALUE_UP:
        case QK_UNDERGLOW_VALUE_DOWN:
        case QK_UNDERGLOW_SPEED_UP:
        case QK_UNDERGLOW_SPEED_DOWN:
            kb_led_config_sync_from_rgb_matrix();
            break;
    }
}
#endif

static uint8_t keyball_dpi_curve_eval(uint8_t speed) {
    if (speed >= KB_DPI_CURVE_LUT_SIZE) speed = KB_DPI_CURVE_LUT_SIZE - 1;
    return kb_dpi_curve_lut_get()[speed];
}

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    static int32_t acc[2][2];
    static int16_t ema[2][2];

    int16_t dx = m->y;
    int16_t dy = m->x;
    if (!is_left) { dx = -dx; dy = -dy; }

    uint8_t side = is_left ? 0 : 1;
    ema[side][0] += dx - ema[side][0] / 4;
    ema[side][1] += dy - ema[side][1] / 4;
    dx = ema[side][0] / 4;
    dy = ema[side][1] / 4;

    int16_t abs_dx = dx < 0 ? -dx : dx;
    int16_t abs_dy = dy < 0 ? -dy : dy;
    int16_t speed  = abs_dx > abs_dy ? abs_dx : abs_dy;

    if (kb_dpi_curve_enable_get()) {
        acc[side][0] = 0; acc[side][1] = 0;
        if (speed == 0) {
            dx = 0; dy = 0;
        } else {
            uint8_t  out_speed = keyball_dpi_curve_eval((uint8_t)speed);
            int32_t out_x = ((int32_t)dx * out_speed) / speed;
            int32_t out_y = ((int32_t)dy * out_speed) / speed;
            dx = out_x < -127 ? -127 : out_x > 127 ? 127 : (int16_t)out_x;
            dy = out_y < -127 ? -127 : out_y > 127 ? 127 : (int16_t)out_y;
        }
    } else {
        uint8_t accel = keyball_get_accel();
        if (accel > 0) {
            if (speed == 0) {
                acc[side][0] = 0; acc[side][1] = 0;
            } else {
                uint8_t scale = 64 / accel;
                acc[side][0] += (int32_t)dx * speed;
                acc[side][1] += (int32_t)dy * speed;
                int32_t out_x = acc[side][0] / scale;
                int32_t out_y = acc[side][1] / scale;
                acc[side][0] -= out_x * (int32_t)scale;
                acc[side][1] -= out_y * (int32_t)scale;
                dx = out_x < -127 ? -127 : out_x > 127 ? 127 : (int16_t)out_x;
                dy = out_y < -127 ? -127 : out_y > 127 ? 127 : (int16_t)out_y;
            }
        } else {
            acc[side][0] = 0; acc[side][1] = 0;
        }
    }
    r->x = (int8_t)dx;
    r->y = (int8_t)dy;
    m->x = 0; m->y = 0;
}

#ifdef OS_DETECTION_ENABLE
void kb_apply_os_swap(os_variant_t os) {
    bool on                       = (kb_settings_get().flags & KB_FLAG_OS_AUTO_SWAP) != 0;
    bool mac                      = on && (os == OS_MACOS || os == OS_IOS);
    keymap_config.swap_lctl_lgui  = mac;
    keymap_config.swap_rctl_rgui  = mac;
}

bool process_detected_host_os_kb(os_variant_t os) {
    if (!process_detected_host_os_user(os)) return false;
    kb_apply_os_swap(os);
    return true;
}
#endif

static void kb_apply_layer_features(uint8_t hl);

static bool    g_ball_probe_ready = false;
static int8_t  g_hw_is_left       = -1;
extern bool is_keyboard_left_impl(void);

bool is_keyboard_left(void) {
    if (g_hw_is_left < 0) {
        g_hw_is_left = is_keyboard_left_impl();
    }
    if (!g_ball_probe_ready) {
        return (bool)g_hw_is_left;
    }
    return keyball.this_have_ball;
}

bool kb_this_side_is_physical_left(void) {
    if (g_hw_is_left < 0) {
        g_hw_is_left = is_keyboard_left_impl();
    }
    return (bool)g_hw_is_left;
}

bool keyball_motion_is_left(void) {
    if (g_hw_is_left < 0) {
        g_hw_is_left = is_keyboard_left_impl();
    }
    return !(bool)g_hw_is_left;
}

#ifdef RGB_MATRIX_ENABLE
static bool g_ball_on_hw_left = false;
bool kb_ball_on_physical_left(void) { return g_ball_on_hw_left; }

static void kb_fixup_led_matrix_rows_if_needed(void) {
    bool ball_is_on_hw_left_side = keyball.this_have_ball ? (bool)g_hw_is_left : !(bool)g_hw_is_left;
    g_ball_on_hw_left            = ball_is_on_hw_left_side;
    if (ball_is_on_hw_left_side) return;

    for (uint8_t r = 0; r < MATRIX_ROWS / 2; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            uint8_t tmp                                  = g_led_config.matrix_co[r][c];
            g_led_config.matrix_co[r][c]                 = g_led_config.matrix_co[r + MATRIX_ROWS / 2][c];
            g_led_config.matrix_co[r + MATRIX_ROWS / 2][c] = tmp;
        }
    }
}
#endif

void keyboard_post_init_user(void) {
    kb_row_order_migrate();
#ifdef CONSOLE_ENABLE
    debug_enable = true;
    uint8_t raw_scroll_layer = eeprom_read_byte((const uint8_t *)(uintptr_t)KB_SCROLL_LAYER_EEPROM);
    dprintf("kb_debug: boot raw_scroll_layer_byte=%u kb_scroll_layer_get=%u\n", raw_scroll_layer, kb_scroll_layer_get());
#endif
    kb_settings_t s = kb_settings_get();
#ifdef AUTO_SHIFT_ENABLE
    if (s.flags & KB_FLAG_AUTO_SHIFT) autoshift_enable();
    else autoshift_disable();
#endif
#ifdef COMBO_ENABLE
    kb_combo_init();
    if (s.flags & KB_FLAG_COMBO) combo_enable();
    else combo_disable();
#endif
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_layer(s.aml_layer);
    set_auto_mouse_timeout(s.aml_timeout);
    set_auto_mouse_enable(!(s.flags & KB_FLAG_AML_DISABLE));
#endif
#ifdef KEYBALL_AML_THRESHOLD_RUNTIME
    kb_aml_threshold = s.aml_threshold;
#endif
#ifdef OS_DETECTION_ENABLE
    kb_apply_os_swap(detected_host_os());
#endif
    kb_apply_layer_features(get_highest_layer(layer_state));
    (void)s;

#ifdef RGB_MATRIX_ENABLE
    kb_fixup_led_matrix_rows_if_needed();
#endif
    g_ball_probe_ready = true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // takuji31: Home Row Mods だけ長めにする（それ以外は Keyball Link の設定値）
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
    }
    uint16_t tt = kb_settings_get().tapping_term;
    return (tt >= 50 && tt <= 1000) ? tt : TAPPING_TERM;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    return (kb_settings_get().flags & KB_FLAG_PERMISSIVE_HOLD) != 0;
}

void kb_hid_layer_link_changed(void) {
    kb_apply_layer_features(get_highest_layer(layer_state));
}

static void kb_apply_layer_features(uint8_t hl) {
#if defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
    keyball_apply_layer_led(hl);
#endif

    keyball_set_scroll_mode(kb_scroll_layer_get() == hl);
    keyball_set_precision_layer(kb_precision_layer_get() == hl);
#ifdef GESTURE_ENABLE
    g_gst_layer_mode = -1;
    for (uint8_t i = 0; i < KB_GESTURE_MODE_COUNT; i++) {
        if (kb_gesture_mode_get(i).layer == hl) {
            g_gst_layer_mode = i;
            break;
        }
    }
    if (gst_active_mode() < 0) {
        g_gesture_acc_x = 0;
        g_gesture_acc_y = 0;
        g_gesture_cooldown = false;
    }
#ifdef CONSOLE_ENABLE
    dprintf("kb_debug: apply_layer_features hl=%u scroll_layer=%u scroll_mode=%u gst_layer_mode=%d gst_manual_mode=%d active_mode=%d\n",
            hl, kb_scroll_layer_get(), keyball_get_scroll_mode(), g_gst_layer_mode, g_gst_manual_mode, gst_active_mode());
#endif
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    kb_apply_layer_features(get_highest_layer(state));
    return state;
}

void matrix_scan_user(void) {
#ifdef RGBLIGHT_ENABLE
    keyball_seasonal_led_task();
#endif
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    shake_task(&mouse_report);
    dflick_task(&mouse_report);

#ifdef GESTURE_ENABLE
    int8_t gst_mode = gst_active_mode();
#ifdef CONSOLE_ENABLE
    if (mouse_report.x || mouse_report.y || mouse_report.h || mouse_report.v) {
        static uint16_t dbg_timer = 0;
        if (timer_elapsed(dbg_timer) > 300) {
            dbg_timer = timer_read();
            dprintf("kb_debug: motion x=%d y=%d h=%d v=%d gst_mode=%d scroll_mode=%u\n",
                    mouse_report.x, mouse_report.y, mouse_report.h, mouse_report.v, gst_mode, keyball_get_scroll_mode());
        }
    }
#endif
    if (gst_mode >= 0) {
        if (g_gesture_cooldown) {
            if (timer_elapsed(g_gesture_cd_timer) > GST_COOLDOWN_MS) {
                g_gesture_cooldown = false;
            } else {
                g_gesture_acc_x = 0;
                g_gesture_acc_y = 0;
                mouse_report.x = 0;
                mouse_report.y = 0;
                return mouse_report;
            }
        }
        g_gesture_acc_x += mouse_report.x;
        g_gesture_acc_y += mouse_report.y;
        uint8_t th_h = kb_gesture_th_h_get();
        uint8_t th_v = kb_gesture_th_v_get();
        if (g_gesture_acc_x > th_h || g_gesture_acc_x < -th_h ||
            g_gesture_acc_y > th_v || g_gesture_acc_y < -th_v) {
            int16_t ax = g_gesture_acc_x < 0 ? -g_gesture_acc_x : g_gesture_acc_x;
            int16_t ay = g_gesture_acc_y < 0 ? -g_gesture_acc_y : g_gesture_acc_y;
            bool    horizontal = ax > ay;
            uint8_t dir = horizontal
                ? (g_gesture_acc_x > 0 ? 3 : 2)
                : (g_gesture_acc_y > 0 ? 1 : 0);

            kb_gesture_mode_t m    = kb_gesture_mode_get(gst_mode);
            uint16_t          kc   = m.key[dir];
            bool              cont = ((m.continuous >> dir) & 1) && kc != SCRL_TO && kc != SCRL_MO;
            if (kc) kb_fire_keycode(kc);
            keyball_gesture_wave_trigger(dir, (uint8_t)gst_mode);

            if (cont) {
                if (horizontal) g_gesture_acc_x -= (g_gesture_acc_x > 0 ? th_h : -th_h);
                else            g_gesture_acc_y -= (g_gesture_acc_y > 0 ? th_v : -th_v);
            } else {
                g_gesture_acc_x    = 0;
                g_gesture_acc_y    = 0;
                g_gesture_cooldown = true;
                g_gesture_cd_timer = timer_read();
            }
        }
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }
#endif
    if (mouse_report.h != 0 || mouse_report.v != 0) {
        uint8_t flags = kb_settings_get().flags;
        if (flags & KB_FLAG_SCROLL_INV_V) mouse_report.v = -mouse_report.v;
        if (flags & KB_FLAG_SCROLL_INV_H) mouse_report.h = -mouse_report.h;
    }
    return mouse_report;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    kb_hid_receive(data, length);
}

#ifdef TAP_DANCE_ENABLE

static uint8_t td_outcome[TD_SLOT_COUNT];
#define TD_OUT_TAP  1
#define TD_OUT_HOLD 2
#define TD_OUT_DTAP 3

static void td_handle_finished(tap_dance_state_t *state, uint8_t idx) {
    td_slot_t s = td_config_get(idx);
    if (state->count == 1 && !state->pressed) {
        td_outcome[idx] = TD_OUT_TAP;
        kb_synth_keyevent(s.tap, true);
    } else if (state->count == 1 && state->pressed) {
        td_outcome[idx] = TD_OUT_HOLD;
        kb_synth_keyevent(s.hold, true);
    } else {
        td_outcome[idx] = TD_OUT_DTAP;
        kb_synth_keyevent(s.dtap ? s.dtap : s.tap, true);
    }
}

static void td_handle_reset(tap_dance_state_t *state, uint8_t idx) {
    td_slot_t s = td_config_get(idx);
    switch (td_outcome[idx]) {
        case TD_OUT_TAP:  kb_synth_keyevent(s.tap, false);  break;
        case TD_OUT_HOLD: kb_synth_keyevent(s.hold, false); break;
        case TD_OUT_DTAP: kb_synth_keyevent(s.dtap ? s.dtap : s.tap, false); break;
    }
    td_outcome[idx] = 0;
}

#define DEFINE_TD_FN(n) \
    static void td_fin_##n(tap_dance_state_t *s, void *u) { td_handle_finished(s, n); } \
    static void td_rst_##n(tap_dance_state_t *s, void *u) { td_handle_reset(s, n); }

DEFINE_TD_FN(0) DEFINE_TD_FN(1) DEFINE_TD_FN(2) DEFINE_TD_FN(3)
DEFINE_TD_FN(4) DEFINE_TD_FN(5) DEFINE_TD_FN(6) DEFINE_TD_FN(7)

tap_dance_action_t tap_dance_actions[] = {
    [0] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_0, td_rst_0),
    [1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_1, td_rst_1),
    [2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_2, td_rst_2),
    [3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_3, td_rst_3),
    [4] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_4, td_rst_4),
    [5] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_5, td_rst_5),
    [6] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_6, td_rst_6),
    [7] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_fin_7, td_rst_7),
};

#endif

#ifdef COMBO_ENABLE
combo_t key_combos[KB_COMBO_SLOT_COUNT];
#endif

#ifdef OLED_ENABLE
#    include "lib/oledkit/oledkit.h"
#    include "lib/oledkit/anim_frames.h"
void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}

#if defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_CUSTOM_USER)
static void render_seasonal_anim_oled(const kb_anim_t *anim) {
    uint16_t idx = (uint16_t)((timer_read() / KB_ANIM_FRAME_MS) % anim->frames);
    oled_set_cursor(0, 0);
    oled_write_raw((const char *)kb_anim_frame(anim, idx), KB_ANIM_FRAME_BYTES);
}

#endif

void oledkit_render_logo_user(void) {
    static bool was_handled = false;
    bool        handled     = false;
#if defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_CUSTOM_USER)
    switch (keyball.oled_anim_off ? RGB_MATRIX_NONE : rgb_matrix_get_mode()) {
        case RGB_MATRIX_CUSTOM_CHRISTMAS: render_seasonal_anim_oled(&anim_xmas); handled = true; break;
        case RGB_MATRIX_CUSTOM_HALLOWEEN: render_seasonal_anim_oled(&anim_hallow); handled = true; break;
        case RGB_MATRIX_CUSTOM_EASTER: render_seasonal_anim_oled(&anim_easter); handled = true; break;
        case RGB_MATRIX_CUSTOM_TWINKLE: render_seasonal_anim_oled(&anim_twinkle); handled = true; break;
        case RGB_MATRIX_GRADIENT_UP_DOWN: render_seasonal_anim_oled(&anim_gradient); handled = true; break;
        case RGB_MATRIX_CYCLE_ALL: render_seasonal_anim_oled(&anim_rainbow); handled = true; break;
        case RGB_MATRIX_BREATHING: render_seasonal_anim_oled(&anim_breath); handled = true; break;
        case RGB_MATRIX_CYCLE_SPIRAL: render_seasonal_anim_oled(&anim_swirl); handled = true; break;
        case RGB_MATRIX_CUSTOM_REACTIVE_KEYS: render_seasonal_anim_oled(&anim_kaleido); handled = true; break;
        case RGB_MATRIX_CUSTOM_HEATMAP: render_seasonal_anim_oled(&anim_heatmap); handled = true; break;
        case RGB_MATRIX_CUSTOM_RIPPLE: render_seasonal_anim_oled(&anim_ripple); handled = true; break;
        default: break;
    }
#endif

    if (!handled && was_handled) {
        oled_clear();
    }
    was_handled = handled;

    if (!handled) {
        const uint16_t PERIOD_MS   = 3200;
        const int8_t   AMPLITUDE   = 2;
        const uint8_t  BASE_MARGIN = 2;
        uint16_t       t           = timer_read() % PERIOD_MS;

        int16_t swing;
        if (t < PERIOD_MS / 2) {
            swing = (int16_t)(-AMPLITUDE + (int32_t)t * (2 * AMPLITUDE) / (PERIOD_MS / 2));
        } else {
            swing = (int16_t)(AMPLITUDE - (int32_t)(t - PERIOD_MS / 2) * (2 * AMPLITUDE) / (PERIOD_MS / 2));
        }
        if (keyball.oled_anim_off) swing = 0;
        uint8_t margin = (uint8_t)(BASE_MARGIN + swing);

        char ch = 0x80;
        for (int y = 0; y < 3; y++) {
            for (uint8_t i = 0; i < margin; i++) {
                oled_write_char(' ', false);
            }
            for (int x = 0; x < 16; x++) {
                oled_write_char(ch, false);
                ch++;
            }
            oled_advance_page(true);
        }
    }
}
#endif
