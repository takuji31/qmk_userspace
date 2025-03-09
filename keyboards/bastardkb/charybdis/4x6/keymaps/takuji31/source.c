/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap.h"

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_user(void) {
    #ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    set_auto_mouse_layer(LAYER_POINTER);
    set_auto_mouse_enable(true);
    #endif
}
#endif

void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(HSV_WHITE);
    #ifdef CONSOLE_ENABLE
    debug_enable=true;
    debug_matrix=true;
    #endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
    #ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    #endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER

    state = update_tri_layer_state(state, 4, 5, 6);
    state = update_tri_layer_state(state, 7, 8, 9);

    switch (get_highest_layer(state)) {
        case 0:
        case 8:
            rgb_matrix_sethsv(HSV_WHITE);
            break;
        case 1:
        case 9:
            rgb_matrix_sethsv(HSV_RED);
            break;
        case 2:
        case 10:
            rgb_matrix_sethsv(HSV_BLUE);
            break;
        case 3:
        case 11:
            rgb_matrix_sethsv(HSV_GREEN);
            break;
        case 4:
        case 12:
            rgb_matrix_sethsv(HSV_ORANGE);
            break;
        case 5:
        case 13:
            rgb_matrix_sethsv(HSV_CHARTREUSE);
            break;
        case 6:
        case 14:
            rgb_matrix_sethsv(HSV_CYAN);
            break;
        case 7:
        case 15:
            rgb_matrix_sethsv(HSV_PINK);
            break;
        default:
            rgb_matrix_sethsv(HSV_BLACK);
            break;
    }
    return state;
};
