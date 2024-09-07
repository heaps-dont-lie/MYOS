/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com

    Referenced from: http://www.osdever.net/bkerndev/Docs/keyboard.htm, thanks Bran!
*/

/*  
*   Modifier keys -> Normal, Control, Shift, Alt
*   2 bit representation within 8 bit storage
*   Default to Normal mode (0)
*/

uint8_t MOD_NORMAL  = 0;
uint8_t MOD_CTRL    = 1;
uint8_t MOD_SHIFT   = 2;
uint8_t MOD_ALT     = 4;


/* This is a US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard.
*/
char scancode_to_key[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char scancode_to_key_shift[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',  /* 9: '1'-'9' keys shift to '!'-'*' */
    '(', ')', '_', '+', '\b',  /* '0', '-', '=' shift to ')', '_', '+' respectively, Backspace */
    '\t',          /* Tab */
    'Q', 'W', 'E', 'R',  /* 19: 'q'-'r' shift to 'Q'-'R' */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',  /* 't'-'p' shift to uppercase and '[' to '{', ']' to '}' */
    0,          /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  /* 39: 'a'-'l' shift to 'A'-'L' and ';' to ':' */
    '"', '~',   0,    /* '\'' to '"', '`' to '~', Left shift (ignore) */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',      /* 49: '\\' to '|', 'z'-'n' to 'Z'-'N' */
    'M', '<', '>', '?',   0,        /* 'm' to 'M', ',' to '<', '.' to '>', '/' to '?' */
    '*',      /* Shift+ '*' remains '*' */
    0,  /* Alt */
    ' ',  /* Space bar (Shift+Space still outputs space) */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock */
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '_',  /* Shift+'-' becomes '_' */
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',  /* Shift+'=' becomes '+' */
    0,  /* 79 - End key */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};
