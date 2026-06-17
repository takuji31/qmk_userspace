/* Copyright 2024 Salicylic_acid3
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

#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 10
#define VIAL_KEYBOARD_UID {0xB0, 0x32, 0x55, 0x63, 0x11, 0x63, 0x5E, 0xC2}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 12 }

// NOTE: TAPPING_TERM_PER_KEY / get_tapping_term() are owned by Vial (quantum/vial.c)
// when VIAL_ENABLE is set, so a per-key home row mod term (e.g. 280ms) is not
// possible here. We rely on a single global term plus CHORDAL_HOLD instead.
#undef TAPPING_TERM
#define TAPPING_TERM 180
#undef QUICK_TAP_TERM
#define QUICK_TAP_TERM 175
#undef FLOW_TAP_TERM
#define FLOW_TAP_TERM 150
#undef CHORDAL_HOLD
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD
