enum tap_dance_codes {
    MAC_ALT,
    MAC_RSFT,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;

#define TM_LALT TD(MAC_ALT)
#define TM_RSFT TD(MAC_RSFT)
