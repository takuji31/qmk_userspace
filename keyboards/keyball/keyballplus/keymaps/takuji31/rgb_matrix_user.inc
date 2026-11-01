RGB_MATRIX_EFFECT(LED_TEST)

RGB_MATRIX_EFFECT(REACTIVE_KEYS)

RGB_MATRIX_EFFECT(SNAKE)
RGB_MATRIX_EFFECT(KNIGHT)
RGB_MATRIX_EFFECT(CHRISTMAS)
RGB_MATRIX_EFFECT(ALTERNATING)

RGB_MATRIX_EFFECT(HALLOWEEN)
RGB_MATRIX_EFFECT(EASTER)

RGB_MATRIX_EFFECT(TWINKLE)

RGB_MATRIX_EFFECT(TRACKBALL)

RGB_MATRIX_EFFECT(RIPPLE)

RGB_MATRIX_EFFECT(HEATMAP)

#ifdef GESTURE_ENABLE
RGB_MATRIX_EFFECT(GESTURE_WAVE)

RGB_MATRIX_EFFECT(TRACKBALL_BREATH)
#endif

RGB_MATRIX_EFFECT(BOOT_WAIT)
RGB_MATRIX_EFFECT(BOOT_SPREAD)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool kb_ball_on_physical_left(void);

#define KB_WAVE_VAL_CAPPED \
    ((uint32_t)(keyball.gesture_wave_val > RGB_MATRIX_MAXIMUM_BRIGHTNESS ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : keyball.gesture_wave_val))

#include "report.h"
#include "lib/keyball/keyball.h"
#include "lib/keyball/kb_settings.h"

static bool LED_TEST(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t idx = rgb_matrix_config.hsv.h;
    for (uint8_t i = led_min; i < led_max; i++) {
        if (i == idx) {
            rgb_matrix_set_color(i, 255, 100, 0);
        } else {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool REACTIVE_KEYS(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    static uint16_t hit_start[RGB_MATRIX_LED_COUNT];
    static bool     hit_active[RGB_MATRIX_LED_COUNT];

    uint8_t cnt = g_last_hit_tracker.count;
    if (cnt > LED_HITS_TO_REMEMBER) cnt = LED_HITS_TO_REMEMBER;
    for (uint8_t j = 0; j < cnt; j++) {
        if (g_last_hit_tracker.tick[j] > 30) continue;
        uint8_t idx = g_last_hit_tracker.index[j];
        if (idx < RGB_MATRIX_LED_COUNT) {
            hit_start[idx]  = timer_read();
            hit_active[idx] = true;
        }
    }

    const uint16_t FADE_MS = 250;

    for (uint8_t i = led_min; i < led_max; i++) {
        HSV hsv = rgb_matrix_config.hsv;
        if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
        } else {
            uint8_t peak = hsv.v;
            hsv.v        = 0;
            if (hit_active[i]) {
                uint16_t elapsed = timer_elapsed(hit_start[i]);
                if (elapsed >= FADE_MS) {
                    hit_active[i] = false;
                } else {
                    hsv.v = peak - (uint8_t)(((uint32_t)elapsed * peak) / FADE_MS);
                }
            }
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool SNAKE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t count = led_max - led_min;
    if (count == 0) return rgb_matrix_check_finished_leds(led_max);

    uint16_t interval = 400 - ((uint16_t)rgb_matrix_config.speed * 350 / 255);
    static uint16_t last_tick = 0;
    static uint8_t  head      = 0;
    if (timer_elapsed(last_tick) >= interval) {
        last_tick = timer_read();
        head      = (head + 1) % count;
    }

    HSV hsv = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        uint8_t pos = i - led_min;
        HSV     out = hsv;
        if (pos != head) out.v = 0;
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool KNIGHT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t count = led_max - led_min;
    if (count == 0) return rgb_matrix_check_finished_leds(led_max);

    uint16_t interval = 300 - ((uint16_t)rgb_matrix_config.speed * 250 / 255);
    static uint16_t last_tick = 0;
    static int8_t   pos       = 0;
    static int8_t   dir       = 1;
    if (timer_elapsed(last_tick) >= interval) {
        last_tick = timer_read();
        pos += dir;
        if (pos >= (int8_t)count - 1) { pos = count - 1; dir = -1; }
        if (pos <= 0) { pos = 0; dir = 1; }
    }

    const uint8_t TAIL_LEN = 4;
    HSV           hsv      = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        int16_t dist = pos - (int16_t)(i - led_min);
        if (dist < 0) dist = -dist;
        HSV out = hsv;
        out.v   = (dist < TAIL_LEN) ? (uint8_t)((uint16_t)hsv.v * (TAIL_LEN - dist) / TAIL_LEN) : 0;
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static void render_checkerboard(effect_params_t* params, const uint8_t* hues, uint8_t count, uint16_t* last_tick, uint8_t* phase) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t interval = 1500 - ((uint16_t)rgb_matrix_config.speed * 1400 / 255);
    if (timer_elapsed(*last_tick) >= interval) {
        *last_tick = timer_read();
        *phase     = (uint8_t)((*phase + 1) % count);
    }

    HSV base = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out;
        if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
            out = (HSV){hues[0], base.s, base.v};
        } else {
            uint8_t slot = (uint8_t)(((i - led_min) + *phase) % count);
            out          = (HSV){hues[slot], base.s, base.v};
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
}

static bool CHRISTMAS(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    static const uint8_t hues[] = {0, 85};
    static uint16_t      last_tick = 0;
    static uint8_t       phase     = 0;
    render_checkerboard(params, hues, 2, &last_tick, &phase);
    return rgb_matrix_check_finished_leds(led_max);
}

static bool HALLOWEEN(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    static const uint8_t hues[] = {21, 191};
    static uint16_t      last_tick = 0;
    static uint8_t       phase     = 0;
    render_checkerboard(params, hues, 2, &last_tick, &phase);
    return rgb_matrix_check_finished_leds(led_max);
}

static bool EASTER(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    static const uint8_t hues[] = {228, 90, 42};
    static uint16_t      last_tick = 0;
    static uint8_t       phase     = 0;
    render_checkerboard(params, hues, 3, &last_tick, &phase);
    return rgb_matrix_check_finished_leds(led_max);
}

static bool TWINKLE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    static uint16_t twinkle_start[RGB_MATRIX_LED_COUNT];
    static bool     twinkle_active[RGB_MATRIX_LED_COUNT];
    static uint16_t spawn_last_tick = 0;

    uint8_t half_min   = is_keyboard_left() ? 0 : 26;
    uint8_t half_max   = is_keyboard_left() ? 26 : RGB_MATRIX_LED_COUNT;
    uint8_t half_count = half_max - half_min;

    if (led_max > led_min) {
        uint16_t spawn_interval = 3000 - ((uint16_t)rgb_matrix_config.speed * 2200 / 255);
        if (timer_elapsed(spawn_last_tick) >= spawn_interval) {
            spawn_last_tick     = timer_read();
            uint8_t idx         = half_min + random8_max(half_count);
            twinkle_start[idx]  = timer_read();
            twinkle_active[idx] = true;
        }
    }

    const uint16_t TWINKLE_MS = 1500;
    for (uint8_t i = led_min; i < led_max; i++) {
        HSV hsv = rgb_matrix_config.hsv;
        if (twinkle_active[i]) {
            uint16_t elapsed = timer_elapsed(twinkle_start[i]);
            if (elapsed >= TWINKLE_MS) {
                twinkle_active[i] = false;
            } else {
                uint8_t  t     = (uint8_t)(((uint32_t)elapsed * 255) / TWINKLE_MS);
                uint8_t  boost = t < 128 ? (uint8_t)(t * 2) : (uint8_t)((255 - t) * 2);
                hsv.s          = (uint8_t)((uint16_t)hsv.s * (255 - boost) / 255);
                uint16_t v     = (uint16_t)hsv.v + ((uint16_t)(255 - hsv.v) * boost / 255);
                hsv.v          = v > 255 ? 255 : (uint8_t)v;
            }
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool TRACKBALL(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    int16_t dx = keyball.this_have_ball ? keyball.this_motion.x : keyball.that_motion.x;
    int16_t dy = keyball.this_have_ball ? keyball.this_motion.y : keyball.that_motion.y;
    uint8_t adx = (uint8_t)(dx < 0 ? -dx : dx);
    uint8_t ady = (uint8_t)(dy < 0 ? -dy : dy);

    static uint16_t last_move_tick = 0;
    static uint8_t  hold_hue       = 0;
    static uint8_t  hold_speed     = 0;

    if (adx > 0 || ady > 0) {
        last_move_tick = timer_read();

        uint8_t hue;
        if (adx > (uint8_t)(ady * 2)) {
            hue = dx > 0 ? 0 : 128;
        } else if (ady > (uint8_t)(adx * 2)) {
            hue = dy > 0 ? 192 : 64;
        } else if (dx > 0) {
            hue = dy > 0 ? 224 : 32;
        } else {
            hue = dy > 0 ? 160 : 96;
        }
        hold_hue = hue;

        uint16_t mag = (uint16_t)adx + (uint16_t)ady;
        hold_speed   = mag > 255 ? 255 : (uint8_t)mag;
    }

    const uint16_t DECAY_MS = 3000 - ((uint16_t)rgb_matrix_config.speed * 2500 / 255);
    uint16_t       elapsed  = timer_elapsed(last_move_tick);
    uint8_t        glow     = (elapsed >= DECAY_MS) ? 0 : (uint8_t)((uint32_t)hold_speed * (DECAY_MS - elapsed) / DECAY_MS);

    HSV base = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out = base;
        if (glow > 0) {
            out.h          = hold_hue;
            uint16_t val_v = (uint16_t)base.v + glow;
            out.v          = val_v > 255 ? 255 : (uint8_t)val_v;
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#define RIPPLE_SLOT_COUNT 6
static bool RIPPLE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    static uint16_t ripple_start[RIPPLE_SLOT_COUNT];
    static uint8_t  ripple_x[RIPPLE_SLOT_COUNT];
    static uint8_t  ripple_y[RIPPLE_SLOT_COUNT];
    static bool     ripple_active[RIPPLE_SLOT_COUNT];

    const uint16_t RIPPLE_MS    = 1200 - ((uint16_t)rgb_matrix_config.speed * 1000 / 255);
    const uint8_t  RIPPLE_MAX_R = 100;
    const uint8_t  RIPPLE_WIDTH = 30;

    uint8_t half_min = is_keyboard_left() ? 0 : 26;
    uint8_t half_max = is_keyboard_left() ? 26 : RGB_MATRIX_LED_COUNT;

    uint8_t cnt = g_last_hit_tracker.count;
    if (cnt > LED_HITS_TO_REMEMBER) cnt = LED_HITS_TO_REMEMBER;
    for (uint8_t j = 0; j < cnt; j++) {
        if (g_last_hit_tracker.tick[j] > 30) continue;
        uint8_t idx = g_last_hit_tracker.index[j];
        if (idx >= RGB_MATRIX_LED_COUNT) continue;
        if (idx < half_min || idx >= half_max) continue;
        if (g_led_config.flags[idx] & LED_FLAG_UNDERGLOW) continue;

        uint8_t slot = RIPPLE_SLOT_COUNT;
        for (uint8_t s = 0; s < RIPPLE_SLOT_COUNT; s++) {
            if (!ripple_active[s]) { slot = s; break; }
        }
        if (slot == RIPPLE_SLOT_COUNT) {
            uint16_t oldest = 0;
            for (uint8_t s = 0; s < RIPPLE_SLOT_COUNT; s++) {
                uint16_t e = timer_elapsed(ripple_start[s]);
                if (e >= oldest) { oldest = e; slot = s; }
            }
        }
        ripple_start[slot]  = timer_read();
        ripple_x[slot]      = g_led_config.point[idx].x;
        ripple_y[slot]      = g_led_config.point[idx].y;
        ripple_active[slot] = true;
    }

    HSV base = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out = base;
        if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
        } else {
            out.v = 0;
            for (uint8_t s = 0; s < RIPPLE_SLOT_COUNT; s++) {
                if (!ripple_active[s]) continue;
                uint16_t elapsed = timer_elapsed(ripple_start[s]);
                if (elapsed >= RIPPLE_MS) continue;

                uint16_t radius = (uint16_t)((uint32_t)elapsed * RIPPLE_MAX_R / RIPPLE_MS);
                int16_t  dx     = (int16_t)g_led_config.point[i].x - (int16_t)ripple_x[s];
                int16_t  dy     = (int16_t)g_led_config.point[i].y - (int16_t)ripple_y[s];
                uint8_t  dist   = sqrt16((uint32_t)dx * dx + (uint32_t)dy * dy);
                uint16_t ring_d = (dist > radius) ? (dist - radius) : (radius - dist);

                if (ring_d < RIPPLE_WIDTH) {
                    uint8_t fade        = (uint8_t)(255 - ((uint32_t)elapsed * 255 / RIPPLE_MS));
                    uint8_t ring_bright = (uint8_t)(255 - ((uint32_t)ring_d * 255 / RIPPLE_WIDTH));
                    uint8_t v = (uint8_t)(((uint32_t)fade * ring_bright / 255) * base.v / 255);
                    if (v > out.v) out.v = v;
                }
            }
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    for (uint8_t s = 0; s < RIPPLE_SLOT_COUNT; s++) {
        if (ripple_active[s] && timer_elapsed(ripple_start[s]) >= RIPPLE_MS) {
            ripple_active[s] = false;
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

static bool ALTERNATING(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t count = led_max - led_min;
    if (count == 0) return rgb_matrix_check_finished_leds(led_max);

    uint16_t interval = 1500 - ((uint16_t)rgb_matrix_config.speed * 1400 / 255);
    static uint16_t last_tick = 0;
    static bool     phase     = false;
    if (timer_elapsed(last_tick) >= interval) {
        last_tick = timer_read();
        phase     = !phase;
    }

    uint8_t half = count / 2;
    HSV     hsv  = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        bool first_group = (i - led_min) < half;
        HSV  out         = hsv;
        out.v            = (first_group == phase) ? hsv.v : 0;
        RGB rgb          = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#define HEATMAP_INCREASE_MIN 4
#define HEATMAP_INCREASE_MAX 128
#define HEATMAP_DECAY_INTERVAL_MS 100
static bool HEATMAP(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    static uint8_t  heat[RGB_MATRIX_LED_COUNT];
    static uint16_t heat_last_tick[RGB_MATRIX_LED_COUNT];
    static bool     heat_init = false;
    if (!heat_init) {
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) heat_last_tick[i] = 0xFFFF;
        heat_init = true;
    }

    uint8_t increase_step = HEATMAP_INCREASE_MIN + (uint8_t)(((uint16_t)rgb_matrix_config.speed * (HEATMAP_INCREASE_MAX - HEATMAP_INCREASE_MIN)) / 255);

    uint8_t cnt = g_last_hit_tracker.count;
    if (cnt > LED_HITS_TO_REMEMBER) cnt = LED_HITS_TO_REMEMBER;
    for (uint8_t j = 0; j < cnt; j++) {
        if (g_last_hit_tracker.tick[j] > 30) continue;
        uint8_t idx = g_last_hit_tracker.index[j];
        if (idx >= RGB_MATRIX_LED_COUNT) continue;
        if (g_last_hit_tracker.tick[j] < heat_last_tick[idx]) {
            heat[idx] = qadd8(heat[idx], increase_step);
        }
        heat_last_tick[idx] = g_last_hit_tracker.tick[j];
    }

    static uint16_t decay_last_tick = 0;
    static bool     do_decay        = false;
    if (params->iter == 0) {
        do_decay = timer_elapsed(decay_last_tick) >= HEATMAP_DECAY_INTERVAL_MS;
        if (do_decay) decay_last_tick = timer_read();
    }

    HSV base = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out = base;
        if (!(g_led_config.flags[i] & LED_FLAG_UNDERGLOW)) {
            if (heat[i] == 0) {
                out.v = 0;
            } else {
                out.h = (uint8_t)(170 - ((uint16_t)heat[i] * 170 / 255));
                out.v = base.v;
            }
            if (do_decay && heat[i] > 0) heat[i]--;
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#ifdef GESTURE_ENABLE
static void gesture_wave_render_sharp(uint8_t led_min, uint8_t led_max, bool overlay) {
    if (led_max <= led_min) return;

    uint8_t half_min = is_keyboard_left() ? 0 : 26;
    uint8_t half_max = is_keyboard_left() ? 26 : RGB_MATRIX_LED_COUNT;

    static bool    bounds_ready = false;
    static uint8_t x_min, x_max, y_min, y_max;
    if (!bounds_ready) {
        x_min = y_min = 255;
        x_max = y_max = 0;
        for (uint8_t i = half_min; i < half_max; i++) {
            uint8_t x = g_led_config.point[i].x;
            uint8_t y = g_led_config.point[i].y;
            if (x < x_min) x_min = x;
            if (x > x_max) x_max = x;
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
        }
        bounds_ready = true;
    }

    const uint16_t WAVE_MS    = 900 - ((uint16_t)keyball.gesture_wave_speed * 700 / 255);
    const uint8_t WAVE_WIDTH_V = 20;
    const uint8_t WAVE_WIDTH_H = 28;

    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out;
        if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
            if (overlay) continue;
            out.h = 0;
            out.s = 0;
            out.v = 0;
        } else {
            out.h = 0;
            out.s = 0;
            out.v = 0;
            for (uint8_t s = 0; s < KEYBALL_GESTURE_WAVE_SLOT_COUNT; s++) {
                if (!keyball.gesture_wave_active[s]) continue;
                uint16_t elapsed = timer_elapsed(keyball.gesture_wave_start[s]);
                if (elapsed >= WAVE_MS) continue;

                uint8_t dir        = keyball.gesture_wave_dir[s];
                bool    horizontal = (dir == 2 || dir == 3);
                uint8_t WAVE_WIDTH = horizontal ? WAVE_WIDTH_H : WAVE_WIDTH_V;
                uint8_t coord      = horizontal ? g_led_config.point[i].x : g_led_config.point[i].y;
                uint8_t range_min  = horizontal ? x_min : y_min;
                uint8_t range_max  = horizontal ? x_max : y_max;
                uint16_t range     = (range_max > range_min) ? (uint16_t)(range_max - range_min) : 1;

                bool     from_min        = horizontal ? ((dir == 2) != kb_ball_on_physical_left()) : (dir == 1);
                uint16_t wave_pos_scaled = (uint16_t)((uint32_t)elapsed * range / WAVE_MS);
                uint8_t  wave_coord      = from_min ? (uint8_t)(range_min + wave_pos_scaled) : (uint8_t)(range_max - wave_pos_scaled);

                int16_t d = (int16_t)coord - (int16_t)wave_coord;
                if (d < 0) d = -d;
                if (d < WAVE_WIDTH) {
                    uint8_t bright = (uint8_t)(255 - ((uint32_t)d * 255 / WAVE_WIDTH));
                    uint8_t v      = (uint8_t)(((uint32_t)bright * KB_WAVE_VAL_CAPPED) / 255);
                    if (v > out.v) {
                        out.h = keyball.gesture_wave_hue;
                        out.s = keyball.gesture_wave_sat;
                        out.v = v;
                    }
                }
            }
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

}

static bool GESTURE_WAVE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    gesture_wave_render_sharp(led_min, led_max, false);
    return rgb_matrix_check_finished_leds(led_max);
}

static void gesture_wave_render_breath(uint8_t led_min, uint8_t led_max, bool overlay) {
    if (led_max <= led_min) return;

    uint8_t half_min = is_keyboard_left() ? 0 : 26;
    uint8_t half_max = is_keyboard_left() ? 26 : RGB_MATRIX_LED_COUNT;

    static bool    bounds_ready = false;
    static uint8_t x_min, x_max, y_min, y_max;
    if (!bounds_ready) {
        x_min = y_min = 255;
        x_max = y_max = 0;
        for (uint8_t i = half_min; i < half_max; i++) {
            uint8_t x = g_led_config.point[i].x;
            uint8_t y = g_led_config.point[i].y;
            if (x < x_min) x_min = x;
            if (x > x_max) x_max = x;
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
        }
        bounds_ready = true;
    }

    const uint16_t WAVE_MS      = 900 - ((uint16_t)keyball.gesture_wave_speed * 800 / 255);
    const uint8_t  WAVE_WIDTH_V = 20;
    const uint8_t  WAVE_WIDTH_H = 28;
    const uint16_t FADE_IN_MS   = 250;
    const uint16_t FADE_OUT_MS  = 400;
    const uint16_t ALL_LIT_MS   = WAVE_MS + FADE_IN_MS;

    for (uint8_t i = led_min; i < led_max; i++) {
        HSV out;
        if (g_led_config.flags[i] & LED_FLAG_UNDERGLOW) {
            if (overlay) continue;
            out.h = 0;
            out.s = 0;
            out.v = 0;
        } else {
            out.h = 0;
            out.s = 0;
            out.v = 0;

            uint8_t env = 0;
            for (uint8_t s = 0; s < KEYBALL_GESTURE_WAVE_SLOT_COUNT; s++) {
                if (!keyball.gesture_wave_active[s]) continue;
                uint16_t elapsed = timer_elapsed(keyball.gesture_wave_start[s]);
                if (elapsed >= ALL_LIT_MS + FADE_OUT_MS) continue;

                uint8_t  dir        = keyball.gesture_wave_dir[s];
                bool     horizontal = (dir == 2 || dir == 3);
                uint8_t  WAVE_WIDTH = horizontal ? WAVE_WIDTH_H : WAVE_WIDTH_V;
                uint8_t  coord      = horizontal ? g_led_config.point[i].x : g_led_config.point[i].y;
                uint8_t  range_min  = horizontal ? x_min : y_min;
                uint8_t  range_max  = horizontal ? x_max : y_max;
                uint16_t range      = (range_max > range_min) ? (uint16_t)(range_max - range_min) : 1;

                bool    from_min = horizontal ? ((dir == 2) != kb_ball_on_physical_left()) : (dir == 1);
                int16_t dist     = from_min ? (int16_t)coord - range_min : (int16_t)range_max - coord;
                int16_t  reach_d = dist - WAVE_WIDTH;
                uint16_t reach   = reach_d <= 0 ? 0 : (uint16_t)((uint32_t)reach_d * WAVE_MS / range);
                if (elapsed < reach) continue;

                uint16_t t_in  = elapsed - reach;
                uint8_t  e_in  = 255;
                if (t_in < FADE_IN_MS) {
                    uint32_t x = (uint32_t)t_in * 255 / FADE_IN_MS;
                    e_in       = (uint8_t)(x * x * (3 * 255 - 2 * x) / (255 * 255));
                }
                uint8_t  e_out = 255;
                if (elapsed > ALL_LIT_MS) {
                    uint16_t t_out = elapsed - ALL_LIT_MS;
                    e_out          = (t_out >= FADE_OUT_MS) ? 0 : (uint8_t)(((uint32_t)(FADE_OUT_MS - t_out) * 255) / FADE_OUT_MS);
                }
                uint8_t e = (uint8_t)(((uint16_t)e_in * e_out) / 255);
                if (e > env) env = e;
            }

            if (env > 0) {
                out.h = keyball.gesture_wave_hue;
                out.s = keyball.gesture_wave_sat;
                out.v = (uint8_t)(((uint32_t)env * KB_WAVE_VAL_CAPPED) / 255);
            }
        }
        RGB rgb = rgb_matrix_hsv_to_rgb(out);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

}

static bool TRACKBALL_BREATH(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    gesture_wave_render_breath(led_min, led_max, false);
    return rgb_matrix_check_finished_leds(led_max);
}
bool keyball_gesture_wave_overlay(uint8_t led_min, uint8_t led_max) {
    if (!keyball_gesture_wave_drawing() || keyball_gesture_wave_overriding()) return false;
    if (keyball.gesture_wave_style == KB_GESTURE_WAVE_STYLE_BREATH) {
        gesture_wave_render_breath(led_min, led_max, true);
    } else {
        gesture_wave_render_sharp(led_min, led_max, true);
    }
    return false;
}
#endif

static bool BOOT_WAIT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool BOOT_SPREAD(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    static uint32_t start = 0;
    if (params->init) start = g_rgb_timer;
    const uint32_t SPREAD_MS = 900;
    const int16_t  CENTER    = 99;
    const int16_t  EDGE      = 40;
    uint32_t       t         = g_rgb_timer - start;
    int16_t        radius    = (t >= SPREAD_MS) ? 32767 : (int16_t)((uint32_t)(CENTER + EDGE) * t / SPREAD_MS);
    HSV            base      = rgb_matrix_config.hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        int16_t d = (int16_t)g_led_config.point[i].x - CENTER;
        if (d < 0) d = -d;
        uint8_t v;
        if (d <= radius - EDGE) {
            v = base.v;
        } else if (d < radius) {
            v = (uint8_t)((uint16_t)base.v * (uint16_t)(radius - d) / EDGE);
        } else {
            v = 0;
        }
        RGB rgb = rgb_matrix_hsv_to_rgb((HSV){base.h, base.s, v});
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
