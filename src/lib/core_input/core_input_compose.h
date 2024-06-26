/* a magic compose table stuffed into a big string. it is actually a tree
 * of strings with a string plus a number (number is encoded as binary
 * values with upper bit 0 if value < 0x80, upper bit 0x80 if < 0x4000 or
 * upper bit 0xc0 if < 0x200000 - and in each case being 1 byte, 2 bytes or
 * 3 bytes in length for a number sequence. most are 1 byte for compactness.
 * being a const string means this string is SHARED between all processes so
 * it's loaded into memory only once at most. it's about as compact as you
 * get (a bit less than 64k) UNLESS you get into some exotic compression
 * algorithms.
 */

static const char *const comp = "dead_breve\x00"
                                "\x83\x07" /* 'dead_breve' 775 */
                                "dead_breve\x00"
                                "\x10" /* 'dead_breve' 16 */
                                "˘\x00"
                                "\x04" /* '˘' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ă\x00"
                                "\x04" /* 'ă' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῐ\x00"
                                "\x05" /* 'Ῐ' 5 */
                                "dead_grave\x00"
                                "\x1c" /* 'dead_grave' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῐ\x00"
                                "\x05" /* 'ῐ' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ĕ\x00"
                                "\x04" /* 'ĕ' 4 */
                                "agrave\x00"
                                "\x0d" /* 'agrave' 13 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ŏ\x00"
                                "\x04" /* 'ŏ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῠ\x00"
                                "\x05" /* 'ῠ' 5 */
                                "ahook\x00"
                                "\x0c" /* 'ahook' 12 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "dead_belowdot\x00"
                                "\x1f" /* 'dead_belowdot' 31 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "˘\x00"
                                "\x04" /* '˘' 4 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Й\x00"
                                "\x04" /* 'Й' 4 */
                                "Multi_key\x00"
                                "\x53" /* 'Multi_key' 83 */
                                "exclam\x00"
                                "\x18" /* 'exclam' 24 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "comma\x00"
                                "\x17" /* 'comma' 23 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ĭ\x00"
                                "\x04" /* 'ĭ' 4 */
                                "dead_tilde\x00"
                                "\x1c" /* 'dead_tilde' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "ӑ\x00"
                                "\x04" /* 'ӑ' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ў\x00"
                                "\x04" /* 'Ў' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̆\x00"
                                "\x04" /* '̆' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ŭ\x00"
                                "\x04" /* 'ŭ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾰ\x00"
                                "\x05" /* 'Ᾰ' 5 */
                                "atilde\x00"
                                "\x0d" /* 'atilde' 13 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ӗ\x00"
                                "\x04" /* 'ӗ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ĕ\x00"
                                "\x04" /* 'Ĕ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "й\x00"
                                "\x04" /* 'й' 4 */
                                "Atilde\x00"
                                "\x0d" /* 'Atilde' 13 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "Cyrillic_zhe\x00"
                                "\x12" /* 'Cyrillic_zhe' 18 */
                                "ӂ\x00"
                                "\x04" /* 'ӂ' 4 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾰ\x00"
                                "\x05" /* 'ᾰ' 5 */
                                "Ahook\x00"
                                "\x0c" /* 'Ahook' 12 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ŏ\x00"
                                "\x04" /* 'Ŏ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ă\x00"
                                "\x04" /* 'Ă' 4 */
                                "Cyrillic_A\x00"
                                "\x10" /* 'Cyrillic_A' 16 */
                                "Ӑ\x00"
                                "\x04" /* 'Ӑ' 4 */
                                "dead_hook\x00"
                                "\x1b" /* 'dead_hook' 27 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "Cyrillic_ZHE\x00"
                                "\x12" /* 'Cyrillic_ZHE' 18 */
                                "Ӂ\x00"
                                "\x04" /* 'Ӂ' 4 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ӗ\x00"
                                "\x04" /* 'Ӗ' 4 */
                                "Aacute\x00"
                                "\x0d" /* 'Aacute' 13 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "aacute\x00"
                                "\x0d" /* 'aacute' 13 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "dead_acute\x00"
                                "\x1c" /* 'dead_acute' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "Agrave\x00"
                                "\x0d" /* 'Agrave' 13 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ĭ\x00"
                                "\x04" /* 'Ĭ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ŭ\x00"
                                "\x04" /* 'Ŭ' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ў\x00"
                                "\x04" /* 'ў' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῠ\x00"
                                "\x05" /* 'Ῠ' 5 */
                                "dead_grave\x00"
                                "\x8a\x04" /* 'dead_grave' 2564 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẁ\x00"
                                "\x05" /* 'Ẁ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "à\x00"
                                "\x04" /* 'à' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ὶ\x00"
                                "\x05" /* 'Ὶ' 5 */
                                "dead_grave\x00"
                                "\x0f" /* 'dead_grave' 15 */
                                "`\x00"
                                "\x03" /* '`' 3 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ὶ\x00"
                                "\x05" /* 'ὶ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὸ\x00"
                                "\x05" /* 'Ὸ' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̀\x00"
                                "\x06" /* 'р̀' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "è\x00"
                                "\x04" /* 'è' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ò\x00"
                                "\x04" /* 'ò' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὺ\x00"
                                "\x05" /* 'ὺ' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "`\x00"
                                "\x03" /* '`' 3 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ѝ\x00"
                                "\x04" /* 'Ѝ' 4 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỳ\x00"
                                "\x05" /* 'ỳ' 5 */
                                "Multi_key\x00"
                                "\x83\x2f" /* 'Multi_key' 815 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "parenright\x00"
                                "\x80\xfb" /* 'parenright' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἲ\x00"
                                "\x05" /* 'Ἲ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἲ\x00"
                                "\x05" /* 'ἲ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὂ\x00"
                                "\x05" /* 'Ὂ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὒ\x00"
                                "\x05" /* 'ὒ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἒ\x00"
                                "\x05" /* 'ἒ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἂ\x00"
                                "\x05" /* 'Ἂ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὂ\x00"
                                "\x05" /* 'ὂ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἢ\x00"
                                "\x05" /* 'ἢ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἂ\x00"
                                "\x05" /* 'ἂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἢ\x00"
                                "\x05" /* 'Ἢ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἒ\x00"
                                "\x05" /* 'Ἒ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὢ\x00"
                                "\x05" /* 'ὢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὢ\x00"
                                "\x05" /* 'Ὢ' 5 */
                                "quotedbl\x00"
                                "\x3d" /* 'quotedbl' 61 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "underscore\x00"
                                "\x2c" /* 'underscore' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "macron\x00"
                                "\x28" /* 'macron' 40 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "parenleft\x00"
                                "\x81\x0e" /* 'parenleft' 270 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἳ\x00"
                                "\x05" /* 'Ἳ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἳ\x00"
                                "\x05" /* 'ἳ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὃ\x00"
                                "\x05" /* 'Ὃ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὓ\x00"
                                "\x05" /* 'ὓ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἓ\x00"
                                "\x05" /* 'ἓ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἃ\x00"
                                "\x05" /* 'Ἃ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὃ\x00"
                                "\x05" /* 'ὃ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἣ\x00"
                                "\x05" /* 'ἣ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἃ\x00"
                                "\x05" /* 'ἃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἣ\x00"
                                "\x05" /* 'Ἣ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἓ\x00"
                                "\x05" /* 'Ἓ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὣ\x00"
                                "\x05" /* 'ὣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὣ\x00"
                                "\x05" /* 'Ὣ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὓ\x00"
                                "\x05" /* 'Ὓ' 5 */
                                "U\x00"
                                "\x13" /* 'U' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̀\x00"
                                "\x06" /* 'О̀' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ì\x00"
                                "\x04" /* 'ì' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ǹ\x00"
                                "\x04" /* 'ǹ' 4 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̀\x00"
                                "\x06" /* 'а̀' 6 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̀\x00"
                                "\x06" /* 'Р̀' 6 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ὲ\x00"
                                "\x05" /* 'ὲ' 5 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̀\x00"
                                "\x06" /* 'У̀' 6 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̀\x00"
                                "\x04" /* '̀' 4 */
                                "V\x00"
                                "\x07" /* 'V' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "omacron\x00"
                                "\x0e" /* 'omacron' 14 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ù\x00"
                                "\x04" /* 'ù' 4 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ὰ\x00"
                                "\x05" /* 'Ὰ' 5 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ѐ\x00"
                                "\x04" /* 'ѐ' 4 */
                                "emacron\x00"
                                "\x0e" /* 'emacron' 14 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "È\x00"
                                "\x04" /* 'È' 4 */
                                "Greek_iotadieresis\x00"
                                "\x19" /* 'Greek_iotadieresis' 25 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỳ\x00"
                                "\x05" /* 'Ỳ' 5 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ѝ\x00"
                                "\x04" /* 'ѝ' 4 */
                                "dead_dasia\x00"
                                "\x81\x0f" /* 'dead_dasia' 271 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἳ\x00"
                                "\x05" /* 'Ἳ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἳ\x00"
                                "\x05" /* 'ἳ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὃ\x00"
                                "\x05" /* 'Ὃ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὓ\x00"
                                "\x05" /* 'ὓ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἓ\x00"
                                "\x05" /* 'ἓ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἃ\x00"
                                "\x05" /* 'Ἃ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὃ\x00"
                                "\x05" /* 'ὃ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἣ\x00"
                                "\x05" /* 'ἣ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἃ\x00"
                                "\x05" /* 'ἃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἣ\x00"
                                "\x05" /* 'Ἣ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἓ\x00"
                                "\x05" /* 'Ἓ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὣ\x00"
                                "\x05" /* 'ὣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὣ\x00"
                                "\x05" /* 'Ὣ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὓ\x00"
                                "\x05" /* 'Ὓ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1c" /* 'Greek_upsilondieresis' 28 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὸ\x00"
                                "\x05" /* 'ὸ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ὴ\x00"
                                "\x05" /* 'ὴ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "dead_psili\x00"
                                "\x80\xfb" /* 'dead_psili' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἲ\x00"
                                "\x05" /* 'Ἲ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἲ\x00"
                                "\x05" /* 'ἲ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὂ\x00"
                                "\x05" /* 'Ὂ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὒ\x00"
                                "\x05" /* 'ὒ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἒ\x00"
                                "\x05" /* 'ἒ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἂ\x00"
                                "\x05" /* 'Ἂ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὂ\x00"
                                "\x05" /* 'ὂ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἢ\x00"
                                "\x05" /* 'ἢ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἂ\x00"
                                "\x05" /* 'ἂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἢ\x00"
                                "\x05" /* 'Ἢ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἒ\x00"
                                "\x05" /* 'Ἒ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὢ\x00"
                                "\x05" /* 'ὢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὢ\x00"
                                "\x05" /* 'Ὢ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ὰ\x00"
                                "\x05" /* 'ὰ' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẁ\x00"
                                "\x05" /* 'ẁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ὴ\x00"
                                "\x05" /* 'Ὴ' 5 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̀\x00"
                                "\x06" /* 'о̀' 6 */
                                "Emacron\x00"
                                "\x0e" /* 'Emacron' 14 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "v\x00"
                                "\x07" /* 'v' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ò\x00"
                                "\x04" /* 'Ò' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "À\x00"
                                "\x04" /* 'À' 4 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ὲ\x00"
                                "\x05" /* 'Ὲ' 5 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̀\x00"
                                "\x06" /* 'А̀' 6 */
                                "Omacron\x00"
                                "\x0e" /* 'Omacron' 14 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ѐ\x00"
                                "\x04" /* 'Ѐ' 4 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὼ\x00"
                                "\x05" /* 'ὼ' 5 */
                                "dead_diaeresis\x00"
                                "\x43" /* 'dead_diaeresis' 67 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὼ\x00"
                                "\x05" /* 'Ὼ' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ì\x00"
                                "\x04" /* 'Ì' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ǹ\x00"
                                "\x04" /* 'Ǹ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ù\x00"
                                "\x04" /* 'Ù' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̀\x00"
                                "\x06" /* 'у̀' 6 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὺ\x00"
                                "\x05" /* 'Ὺ' 5 */
                                "dead_horn\x00"
                                "\x82\x3e" /* 'dead_horn' 574 */
                                "Uhook\x00"
                                "\x0c" /* 'Uhook' 12 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "Obelowdot\x00"
                                "\x10" /* 'Obelowdot' 16 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "Ograve\x00"
                                "\x0d" /* 'Ograve' 13 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "dead_grave\x00"
                                "\x2c" /* 'dead_grave' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "dead_horn\x00"
                                "\x0f" /* 'dead_horn' 15 */
                                "̛\x00"
                                "\x04" /* '̛' 4 */
                                "Oacute\x00"
                                "\x0d" /* 'Oacute' 13 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "ohook\x00"
                                "\x0c" /* 'ohook' 12 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ơ\x00"
                                "\x04" /* 'ơ' 4 */
                                "Utilde\x00"
                                "\x0d" /* 'Utilde' 13 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "dead_belowdot\x00"
                                "\x2f" /* 'dead_belowdot' 47 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "̛\x00"
                                "\x04" /* '̛' 4 */
                                "ubelowdot\x00"
                                "\x10" /* 'ubelowdot' 16 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "oacute\x00"
                                "\x0d" /* 'oacute' 13 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "uhook\x00"
                                "\x0c" /* 'uhook' 12 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "dead_tilde\x00"
                                "\x2c" /* 'dead_tilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "Uacute\x00"
                                "\x0d" /* 'Uacute' 13 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "Ugrave\x00"
                                "\x0d" /* 'Ugrave' 13 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̛\x00"
                                "\x04" /* '̛' 4 */
                                "uacute\x00"
                                "\x0d" /* 'uacute' 13 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ư\x00"
                                "\x04" /* 'ư' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "utilde\x00"
                                "\x0d" /* 'utilde' 13 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "ograve\x00"
                                "\x0d" /* 'ograve' 13 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "Ohook\x00"
                                "\x0c" /* 'Ohook' 12 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ơ\x00"
                                "\x04" /* 'Ơ' 4 */
                                "Ubelowdot\x00"
                                "\x10" /* 'Ubelowdot' 16 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "dead_hook\x00"
                                "\x2b" /* 'dead_hook' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "ugrave\x00"
                                "\x0d" /* 'ugrave' 13 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "obelowdot\x00"
                                "\x10" /* 'obelowdot' 16 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "dead_acute\x00"
                                "\x2c" /* 'dead_acute' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ư\x00"
                                "\x04" /* 'Ư' 4 */
                                "dead_circumflex\x00"
                                "\x86\xae" /* 'dead_circumflex' 1710 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "⁻\x00"
                                "\x05" /* '⁻' 5 */
                                "W\x00"
                                "\x07" /* 'W' 7 */
                                "Ŵ\x00"
                                "\x04" /* 'Ŵ' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ĝ\x00"
                                "\x04" /* 'ĝ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "â\x00"
                                "\x04" /* 'â' 4 */
                                "Ograve\x00"
                                "\x0d" /* 'Ograve' 13 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "dead_circumflex\x00"
                                "\x14" /* 'dead_circumflex' 20 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "dead_grave\x00"
                                "\x3c" /* 'dead_grave' 60 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Ehook\x00"
                                "\x0c" /* 'Ehook' 12 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "1\x00"
                                "\x07" /* '1' 7 */
                                "¹\x00"
                                "\x04" /* '¹' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ĉ\x00"
                                "\x04" /* 'Ĉ' 4 */
                                "KP_4\x00"
                                "\x0b" /* 'KP_4' 11 */
                                "⁴\x00"
                                "\x05" /* '⁴' 5 */
                                "Oacute\x00"
                                "\x0d" /* 'Oacute' 13 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̂\x00"
                                "\x06" /* 'р̂' 6 */
                                "ohook\x00"
                                "\x0c" /* 'ohook' 12 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ê\x00"
                                "\x04" /* 'ê' 4 */
                                "agrave\x00"
                                "\x0d" /* 'agrave' 13 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "KP_6\x00"
                                "\x0b" /* 'KP_6' 11 */
                                "⁶\x00"
                                "\x05" /* '⁶' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ô\x00"
                                "\x04" /* 'ô' 4 */
                                "ahook\x00"
                                "\x0c" /* 'ahook' 12 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "dead_belowdot\x00"
                                "\x3f" /* 'dead_belowdot' 63 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "KP_8\x00"
                                "\x0b" /* 'KP_8' 11 */
                                "⁸\x00"
                                "\x05" /* '⁸' 5 */
                                "Etilde\x00"
                                "\x0d" /* 'Etilde' 13 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И̂\x00"
                                "\x06" /* 'И̂' 6 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ŷ\x00"
                                "\x04" /* 'ŷ' 4 */
                                "Multi_key\x00"
                                "\x81\x52" /* 'Multi_key' 338 */
                                "exclam\x00"
                                "\x38" /* 'exclam' 56 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "t\x00"
                                "\x13" /* 't' 19 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "underbar\x00"
                                "\x60" /* 'underbar' 96 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ª\x00"
                                "\x04" /* 'ª' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "º\x00"
                                "\x04" /* 'º' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ˡ\x00"
                                "\x04" /* 'ˡ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ʸ\x00"
                                "\x04" /* 'ʸ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ⁱ\x00"
                                "\x05" /* 'ⁱ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ⁿ\x00"
                                "\x05" /* 'ⁿ' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ʲ\x00"
                                "\x04" /* 'ʲ' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "ˣ\x00"
                                "\x04" /* 'ˣ' 4 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ʷ\x00"
                                "\x04" /* 'ʷ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ʳ\x00"
                                "\x04" /* 'ʳ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ˢ\x00"
                                "\x04" /* 'ˢ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ʰ\x00"
                                "\x04" /* 'ʰ' 4 */
                                "S\x00"
                                "\x13" /* 'S' 19 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "underscore\x00"
                                "\x62" /* 'underscore' 98 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ª\x00"
                                "\x04" /* 'ª' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "º\x00"
                                "\x04" /* 'º' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ˡ\x00"
                                "\x04" /* 'ˡ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ʸ\x00"
                                "\x04" /* 'ʸ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ⁱ\x00"
                                "\x05" /* 'ⁱ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ⁿ\x00"
                                "\x05" /* 'ⁿ' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ʲ\x00"
                                "\x04" /* 'ʲ' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "ˣ\x00"
                                "\x04" /* 'ˣ' 4 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ʷ\x00"
                                "\x04" /* 'ʷ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ʳ\x00"
                                "\x04" /* 'ʳ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ˢ\x00"
                                "\x04" /* 'ˢ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ʰ\x00"
                                "\x04" /* 'ʰ' 4 */
                                "s\x00"
                                "\x13" /* 's' 19 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "T\x00"
                                "\x13" /* 'T' 19 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "oacute\x00"
                                "\x0d" /* 'oacute' 13 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̂\x00"
                                "\x06" /* 'О̂' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "î\x00"
                                "\x04" /* 'î' 4 */
                                "KP_9\x00"
                                "\x0b" /* 'KP_9' 11 */
                                "⁹\x00"
                                "\x05" /* '⁹' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "⁼\x00"
                                "\x05" /* '⁼' 5 */
                                "KP_Space\x00"
                                "\x0e" /* 'KP_Space' 14 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "dead_tilde\x00"
                                "\x3c" /* 'dead_tilde' 60 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "⁷\x00"
                                "\x05" /* '⁷' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̂\x00"
                                "\x06" /* 'а̂' 6 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ĵ\x00"
                                "\x04" /* 'ĵ' 4 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⁾\x00"
                                "\x05" /* '⁾' 5 */
                                "Eacute\x00"
                                "\x0d" /* 'Eacute' 13 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̂\x00"
                                "\x06" /* 'Р̂' 6 */
                                "KP_7\x00"
                                "\x0b" /* 'KP_7' 11 */
                                "⁷\x00"
                                "\x05" /* '⁷' 5 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̂\x00"
                                "\x06" /* 'У̂' 6 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̂\x00"
                                "\x04" /* '̂' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "û\x00"
                                "\x04" /* 'û' 4 */
                                "z\x00"
                                "\x08" /* 'z' 8 */
                                "ẑ\x00"
                                "\x05" /* 'ẑ' 5 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ĝ\x00"
                                "\x04" /* 'Ĝ' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ĥ\x00"
                                "\x04" /* 'Ĥ' 4 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⁸\x00"
                                "\x05" /* '⁸' 5 */
                                "KP_1\x00"
                                "\x0a" /* 'KP_1' 10 */
                                "¹\x00"
                                "\x04" /* '¹' 4 */
                                "atilde\x00"
                                "\x0d" /* 'atilde' 13 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "3\x00"
                                "\x07" /* '3' 7 */
                                "³\x00"
                                "\x04" /* '³' 4 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̂\x00"
                                "\x06" /* 'е̂' 6 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ê\x00"
                                "\x04" /* 'Ê' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ŝ\x00"
                                "\x04" /* 'Ŝ' 4 */
                                "2\x00"
                                "\x07" /* '2' 7 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ŷ\x00"
                                "\x04" /* 'Ŷ' 4 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и̂\x00"
                                "\x06" /* 'и̂' 6 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "Atilde\x00"
                                "\x0d" /* 'Atilde' 13 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "egrave\x00"
                                "\x0d" /* 'egrave' 13 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "ograve\x00"
                                "\x0d" /* 'ograve' 13 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "⁺\x00"
                                "\x05" /* '⁺' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "⁶\x00"
                                "\x05" /* '⁶' 5 */
                                "Ahook\x00"
                                "\x0c" /* 'Ahook' 12 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ŵ\x00"
                                "\x04" /* 'ŵ' 4 */
                                "Ohook\x00"
                                "\x0c" /* 'Ohook' 12 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̂\x00"
                                "\x06" /* 'о̂' 6 */
                                "4\x00"
                                "\x08" /* '4' 8 */
                                "⁴\x00"
                                "\x05" /* '⁴' 5 */
                                "KP_3\x00"
                                "\x0a" /* 'KP_3' 10 */
                                "³\x00"
                                "\x04" /* '³' 4 */
                                "eacute\x00"
                                "\x0d" /* 'eacute' 13 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "J\x00"
                                "\x07" /* 'J' 7 */
                                "Ĵ\x00"
                                "\x04" /* 'Ĵ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ô\x00"
                                "\x04" /* 'Ô' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ŝ\x00"
                                "\x04" /* 'ŝ' 4 */
                                "Z\x00"
                                "\x08" /* 'Z' 8 */
                                "Ẑ\x00"
                                "\x05" /* 'Ẑ' 5 */
                                "KP_0\x00"
                                "\x0b" /* 'KP_0' 11 */
                                "⁰\x00"
                                "\x05" /* '⁰' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Â\x00"
                                "\x04" /* 'Â' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ĉ\x00"
                                "\x04" /* 'ĉ' 4 */
                                "KP_Add\x00"
                                "\x0d" /* 'KP_Add' 13 */
                                "⁺\x00"
                                "\x05" /* '⁺' 5 */
                                "KP_2\x00"
                                "\x0a" /* 'KP_2' 10 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̂\x00"
                                "\x06" /* 'А̂' 6 */
                                "dead_hook\x00"
                                "\x3b" /* 'dead_hook' 59 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⁵\x00"
                                "\x05" /* '⁵' 5 */
                                "KP_5\x00"
                                "\x0b" /* 'KP_5' 11 */
                                "⁵\x00"
                                "\x05" /* '⁵' 5 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "⁹\x00"
                                "\x05" /* '⁹' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̂\x00"
                                "\x06" /* 'Е̂' 6 */
                                "Egrave\x00"
                                "\x0d" /* 'Egrave' 13 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "⁰\x00"
                                "\x05" /* '⁰' 5 */
                                "Aacute\x00"
                                "\x0d" /* 'Aacute' 13 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "etilde\x00"
                                "\x0d" /* 'etilde' 13 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "aacute\x00"
                                "\x0d" /* 'aacute' 13 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "dead_acute\x00"
                                "\x3c" /* 'dead_acute' 60 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Agrave\x00"
                                "\x0d" /* 'Agrave' 13 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "⁽\x00"
                                "\x05" /* '⁽' 5 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ĥ\x00"
                                "\x04" /* 'ĥ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Î\x00"
                                "\x04" /* 'Î' 4 */
                                "ehook\x00"
                                "\x0c" /* 'ehook' 12 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Û\x00"
                                "\x04" /* 'Û' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̂\x00"
                                "\x06" /* 'у̂' 6 */
                                "KP_Equal\x00"
                                "\x0f" /* 'KP_Equal' 15 */
                                "⁼\x00"
                                "\x05" /* '⁼' 5 */
                                "dead_currency\x00"
                                "\x81\xdd" /* 'dead_currency' 477 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "₩\x00"
                                "\x05" /* '₩' 5 */
                                "g\x00"
                                "\x08" /* 'g' 8 */
                                "₲\x00"
                                "\x05" /* '₲' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "؋\x00"
                                "\x04" /* '؋' 4 */
                                "dead_currency\x00"
                                "\x13" /* 'dead_currency' 19 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "₡\x00"
                                "\x05" /* '₡' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "F\x00"
                                "\x08" /* 'F' 8 */
                                "₣\x00"
                                "\x05" /* '₣' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "௹\x00"
                                "\x05" /* '௹' 5 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "£\x00"
                                "\x04" /* '£' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "৳\x00"
                                "\x05" /* '৳' 5 */
                                "thorn\x00"
                                "\x0c" /* 'thorn' 12 */
                                "৲\x00"
                                "\x05" /* '৲' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "฿\x00"
                                "\x05" /* '฿' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "﷼\x00"
                                "\x05" /* '﷼' 5 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "₭\x00"
                                "\x05" /* '₭' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "₦\x00"
                                "\x05" /* '₦' 5 */
                                "ccedilla\x00"
                                "\x0f" /* 'ccedilla' 15 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "元\x00"
                                "\x05" /* '元' 5 */
                                "G\x00"
                                "\x08" /* 'G' 8 */
                                "₲\x00"
                                "\x05" /* '₲' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "₴\x00"
                                "\x05" /* '₴' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "₠\x00"
                                "\x05" /* '₠' 5 */
                                "S\x00"
                                "\x06" /* 'S' 6 */
                                "$\x00"
                                "\x03" /* '$' 3 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "円\x00"
                                "\x05" /* '円' 5 */
                                "f\x00"
                                "\x07" /* 'f' 7 */
                                "ƒ\x00"
                                "\x04" /* 'ƒ' 4 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "₫\x00"
                                "\x05" /* '₫' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "₯\x00"
                                "\x05" /* '₯' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "₩\x00"
                                "\x05" /* '₩' 5 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "₰\x00"
                                "\x05" /* '₰' 5 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "₧\x00"
                                "\x05" /* '₧' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "ℳ\x00"
                                "\x05" /* 'ℳ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "૱\x00"
                                "\x05" /* '૱' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "₥\x00"
                                "\x05" /* '₥' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "₢\x00"
                                "\x05" /* '₢' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "₪\x00"
                                "\x05" /* '₪' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "₳\x00"
                                "\x05" /* '₳' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "₨\x00"
                                "\x05" /* '₨' 5 */
                                "THORN\x00"
                                "\x0c" /* 'THORN' 12 */
                                "৲\x00"
                                "\x05" /* '৲' 5 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "₤\x00"
                                "\x05" /* '₤' 5 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "₮\x00"
                                "\x05" /* '₮' 5 */
                                "Ccedilla\x00"
                                "\x0f" /* 'Ccedilla' 15 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "₭\x00"
                                "\x05" /* '₭' 5 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "₱\x00"
                                "\x05" /* '₱' 5 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "₴\x00"
                                "\x05" /* '₴' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "៛\x00"
                                "\x05" /* '៛' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "₦\x00"
                                "\x05" /* '₦' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "圓\x00"
                                "\x05" /* '圓' 5 */
                                "dead_belowdiaeresis\x00"
                                "\x41" /* 'dead_belowdiaeresis' 65 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṳ\x00"
                                "\x05" /* 'ṳ' 5 */
                                "dead_diaeresis\x00"
                                "\x1c" /* 'dead_diaeresis' 28 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "⩷\x00"
                                "\x05" /* '⩷' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṳ\x00"
                                "\x05" /* 'Ṳ' 5 */
                                "dead_belowdot\x00"
                                "\x83\x6e" /* 'dead_belowdot' 878 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "⨪\x00"
                                "\x05" /* '⨪' 5 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẉ\x00"
                                "\x05" /* 'Ẉ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ạ\x00"
                                "\x05" /* 'ạ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẹ\x00"
                                "\x05" /* 'ẹ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ọ\x00"
                                "\x05" /* 'ọ' 5 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḷ\x00"
                                "\x05" /* 'ḷ' 5 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṭ\x00"
                                "\x05" /* 'ṭ' 5 */
                                "dead_belowdot\x00"
                                "\x13" /* 'dead_belowdot' 19 */
                                "̣\x00"
                                "\x04" /* '̣' 4 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "̣\x00"
                                "\x04" /* '̣' 4 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỵ\x00"
                                "\x05" /* 'ỵ' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "ḅ\x00"
                                "\x05" /* 'ḅ' 5 */
                                "Multi_key\x00"
                                "\x31" /* 'Multi_key' 49 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ị\x00"
                                "\x05" /* 'ị' 5 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḳ\x00"
                                "\x05" /* 'ḳ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṇ\x00"
                                "\x05" /* 'ṇ' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "⩦\x00"
                                "\x05" /* '⩦' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "sabovedot\x00"
                                "\x10" /* 'sabovedot' 16 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̣\x00"
                                "\x04" /* '̣' 4 */
                                "V\x00"
                                "\x08" /* 'V' 8 */
                                "Ṿ\x00"
                                "\x05" /* 'Ṿ' 5 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ụ\x00"
                                "\x05" /* 'ụ' 5 */
                                "z\x00"
                                "\x08" /* 'z' 8 */
                                "ẓ\x00"
                                "\x05" /* 'ẓ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḥ\x00"
                                "\x05" /* 'Ḥ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẹ\x00"
                                "\x05" /* 'Ẹ' 5 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṣ\x00"
                                "\x05" /* 'Ṣ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỵ\x00"
                                "\x05" /* 'Ỵ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḍ\x00"
                                "\x05" /* 'ḍ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḍ\x00"
                                "\x05" /* 'Ḍ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "⨥\x00"
                                "\x05" /* '⨥' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "dead_abovedot\x00"
                                "\x1f" /* 'dead_abovedot' 31 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẉ\x00"
                                "\x05" /* 'ẉ' 5 */
                                "v\x00"
                                "\x08" /* 'v' 8 */
                                "ṿ\x00"
                                "\x05" /* 'ṿ' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ṃ\x00"
                                "\x05" /* 'Ṃ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ọ\x00"
                                "\x05" /* 'Ọ' 5 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ṃ\x00"
                                "\x05" /* 'ṃ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṛ\x00"
                                "\x05" /* 'ṛ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṣ\x00"
                                "\x05" /* 'ṣ' 5 */
                                "Z\x00"
                                "\x08" /* 'Z' 8 */
                                "Ẓ\x00"
                                "\x05" /* 'Ẓ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ạ\x00"
                                "\x05" /* 'Ạ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṛ\x00"
                                "\x05" /* 'Ṛ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḷ\x00"
                                "\x05" /* 'Ḷ' 5 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṭ\x00"
                                "\x05" /* 'Ṭ' 5 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḳ\x00"
                                "\x05" /* 'Ḳ' 5 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "Ḅ\x00"
                                "\x05" /* 'Ḅ' 5 */
                                "Sabovedot\x00"
                                "\x10" /* 'Sabovedot' 16 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḥ\x00"
                                "\x05" /* 'ḥ' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ị\x00"
                                "\x05" /* 'Ị' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṇ\x00"
                                "\x05" /* 'Ṇ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ụ\x00"
                                "\x05" /* 'Ụ' 5 */
                                "dead_macron\x00"
                                "\x84\xfc" /* 'dead_macron' 1276 */
                                "adiaeresis\x00"
                                "\x10" /* 'adiaeresis' 16 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "g\x00"
                                "\x08" /* 'g' 8 */
                                "ḡ\x00"
                                "\x05" /* 'ḡ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῑ\x00"
                                "\x05" /* 'Ῑ' 5 */
                                "Ograve\x00"
                                "\x0d" /* 'Ograve' 13 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "dead_grave\x00"
                                "\x2c" /* 'dead_grave' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῑ\x00"
                                "\x05" /* 'ῑ' 5 */
                                "Oacute\x00"
                                "\x0d" /* 'Oacute' 13 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̄\x00"
                                "\x06" /* 'р̄' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῡ\x00"
                                "\x05" /* 'ῡ' 5 */
                                "dead_belowdot\x00"
                                "\x2f" /* 'dead_belowdot' 47 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "dead_macron\x00"
                                "\x11" /* 'dead_macron' 17 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ӣ\x00"
                                "\x04" /* 'Ӣ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ȳ\x00"
                                "\x04" /* 'ȳ' 4 */
                                "Multi_key\x00"
                                "\x80\xbf" /* 'Multi_key' 191 */
                                "period\x00"
                                "\x24" /* 'period' 36 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "exclam\x00"
                                "\x28" /* 'exclam' 40 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "quotedbl\x00"
                                "\x34" /* 'quotedbl' 52 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "asciitilde\x00"
                                "\x1a" /* 'asciitilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "semicolon\x00"
                                "\x19" /* 'semicolon' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "oacute\x00"
                                "\x0d" /* 'oacute' 13 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̄\x00"
                                "\x06" /* 'О̄' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "dead_tilde\x00"
                                "\x1a" /* 'dead_tilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̄\x00"
                                "\x06" /* 'а̄' 6 */
                                "Eacute\x00"
                                "\x0d" /* 'Eacute' 13 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̄\x00"
                                "\x06" /* 'Р̄' 6 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӯ\x00"
                                "\x04" /* 'Ӯ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̄\x00"
                                "\x04" /* '̄' 4 */
                                "V\x00"
                                "\x07" /* 'V' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǣ\x00"
                                "\x04" /* 'Ǣ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "G\x00"
                                "\x08" /* 'G' 8 */
                                "Ḡ\x00"
                                "\x05" /* 'Ḡ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾱ\x00"
                                "\x05" /* 'Ᾱ' 5 */
                                "otilde\x00"
                                "\x0c" /* 'otilde' 12 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̄\x00"
                                "\x06" /* 'е̄' 6 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ȳ\x00"
                                "\x04" /* 'Ȳ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ӣ\x00"
                                "\x04" /* 'ӣ' 4 */
                                "dead_ogonek\x00"
                                "\x1b" /* 'dead_ogonek' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "odiaeresis\x00"
                                "\x10" /* 'odiaeresis' 16 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "Otilde\x00"
                                "\x0c" /* 'Otilde' 12 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "egrave\x00"
                                "\x0d" /* 'egrave' 13 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "dead_greek\x00"
                                "\x3c" /* 'dead_greek' 60 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ᾱ\x00"
                                "\x05" /* 'ᾱ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ῑ\x00"
                                "\x05" /* 'ῑ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ῡ\x00"
                                "\x05" /* 'ῡ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ᾱ\x00"
                                "\x05" /* 'Ᾱ' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ῑ\x00"
                                "\x05" /* 'Ῑ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ῡ\x00"
                                "\x05" /* 'Ῡ' 5 */
                                "ograve\x00"
                                "\x0d" /* 'ograve' 13 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾱ\x00"
                                "\x05" /* 'ᾱ' 5 */
                                "dead_abovedot\x00"
                                "\x2b" /* 'dead_abovedot' 43 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̄\x00"
                                "\x06" /* 'о̄' 6 */
                                "eacute\x00"
                                "\x0d" /* 'eacute' 13 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "v\x00"
                                "\x07" /* 'v' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "Odiaeresis\x00"
                                "\x10" /* 'Odiaeresis' 16 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̄\x00"
                                "\x06" /* 'А̄' 6 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̄\x00"
                                "\x06" /* 'Е̄' 6 */
                                "Egrave\x00"
                                "\x0d" /* 'Egrave' 13 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "dead_diaeresis\x00"
                                "\x3a" /* 'dead_diaeresis' 58 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Adiaeresis\x00"
                                "\x10" /* 'Adiaeresis' 16 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "dead_acute\x00"
                                "\x2c" /* 'dead_acute' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӯ\x00"
                                "\x04" /* 'ӯ' 4 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǣ\x00"
                                "\x04" /* 'ǣ' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῡ\x00"
                                "\x05" /* 'Ῡ' 5 */
                                "dead_doublegrave\x00"
                                "\x80\xef" /* 'dead_doublegrave' 239 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̏\x00"
                                "\x06" /* 'р̏' 6 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И̏\x00"
                                "\x06" /* 'И̏' 6 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̏\x00"
                                "\x06" /* 'О̏' 6 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̏\x00"
                                "\x06" /* 'а̏' 6 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̏\x00"
                                "\x06" /* 'Р̏' 6 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̏\x00"
                                "\x06" /* 'У̏' 6 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̏\x00"
                                "\x06" /* 'е̏' 6 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и̏\x00"
                                "\x06" /* 'и̏' 6 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̏\x00"
                                "\x06" /* 'о̏' 6 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̏\x00"
                                "\x06" /* 'А̏' 6 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̏\x00"
                                "\x06" /* 'Е̏' 6 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̏\x00"
                                "\x06" /* 'у̏' 6 */
                                "Multi_key\x00"
                                "\xc0\x92\xd5" /* 'Multi_key' 37589 */
                                "backslash\x00"
                                "\x27" /* 'backslash' 39 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "⍀\x00"
                                "\x05" /* '⍀' 5 */
                                "o\x00"
                                "\x10" /* 'o' 16 */
                                "slash\x00"
                                "\x0d" /* 'slash' 13 */
                                "🙌\x00"
                                "\x06" /* '🙌' 6 */
                                "minus\x00"
                                "\x81\x25" /* 'minus' 293 */
                                "backslash\x00"
                                "\x10" /* 'backslash' 16 */
                                "⍀\x00"
                                "\x05" /* '⍀' 5 */
                                "minus\x00"
                                "\x2b" /* 'minus' 43 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "—\x00"
                                "\x05" /* '—' 5 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "–\x00"
                                "\x05" /* '–' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "­\x00"
                                "\x04" /* '­' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "£\x00"
                                "\x04" /* '£' 4 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "parenright\x00"
                                "\x0f" /* 'parenright' 15 */
                                "}\x00"
                                "\x03" /* '}' 3 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "đ\x00"
                                "\x04" /* 'đ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Đ\x00"
                                "\x04" /* 'Đ' 4 */
                                "plus\x00"
                                "\x0a" /* 'plus' 10 */
                                "±\x00"
                                "\x04" /* '±' 4 */
                                "colon\x00"
                                "\x0b" /* 'colon' 11 */
                                "÷\x00"
                                "\x04" /* '÷' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "£\x00"
                                "\x04" /* '£' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "¬\x00"
                                "\x04" /* '¬' 4 */
                                "slash\x00"
                                "\x0c" /* 'slash' 12 */
                                "⌿\x00"
                                "\x05" /* '⌿' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "→\x00"
                                "\x05" /* '→' 5 */
                                "parenleft\x00"
                                "\x0e" /* 'parenleft' 14 */
                                "{\x00"
                                "\x03" /* '{' 3 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "period\x00"
                                "\x82\x5f" /* 'period' 607 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "·\x00"
                                "\x04" /* '·' 4 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "…\x00"
                                "\x05" /* '…' 5 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẇ\x00"
                                "\x05" /* 'Ẇ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ġ\x00"
                                "\x04" /* 'ġ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ȧ\x00"
                                "\x04" /* 'ȧ' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ċ\x00"
                                "\x04" /* 'Ċ' 4 */
                                "exclam\x00"
                                "\x18" /* 'exclam' 24 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "‹\x00"
                                "\x05" /* '‹' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ė\x00"
                                "\x04" /* 'ė' 4 */
                                "F\x00"
                                "\x08" /* 'F' 8 */
                                "Ḟ\x00"
                                "\x05" /* 'Ḟ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȯ\x00"
                                "\x04" /* 'ȯ' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṫ\x00"
                                "\x05" /* 'ṫ' 5 */
                                "dead_belowdot\x00"
                                "\x1f" /* 'dead_belowdot' 31 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ẏ\x00"
                                "\x05" /* 'ẏ' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "ḃ\x00"
                                "\x05" /* 'ḃ' 5 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ı\x00"
                                "\x04" /* 'ı' 4 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṅ\x00"
                                "\x05" /* 'ṅ' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "•\x00"
                                "\x05" /* '•' 5 */
                                "dead_caron\x00"
                                "\x1c" /* 'dead_caron' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "x\x00"
                                "\x08" /* 'x' 8 */
                                "ẋ\x00"
                                "\x05" /* 'ẋ' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ż\x00"
                                "\x04" /* 'ż' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ġ\x00"
                                "\x04" /* 'Ġ' 4 */
                                "Sacute\x00"
                                "\x0d" /* 'Sacute' 13 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḣ\x00"
                                "\x05" /* 'Ḣ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ė\x00"
                                "\x04" /* 'Ė' 4 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṡ\x00"
                                "\x05" /* 'Ṡ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ẏ\x00"
                                "\x05" /* 'Ẏ' 5 */
                                "scaron\x00"
                                "\x0d" /* 'scaron' 13 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "f\x00"
                                "\x08" /* 'f' 8 */
                                "ḟ\x00"
                                "\x05" /* 'ḟ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḋ\x00"
                                "\x05" /* 'ḋ' 5 */
                                "Scaron\x00"
                                "\x0d" /* 'Scaron' 13 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḋ\x00"
                                "\x05" /* 'Ḋ' 5 */
                                "acute\x00"
                                "\x17" /* 'acute' 23 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẇ\x00"
                                "\x05" /* 'ẇ' 5 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ṗ\x00"
                                "\x05" /* 'ṗ' 5 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ṗ\x00"
                                "\x05" /* 'Ṗ' 5 */
                                "apostrophe\x00"
                                "\x1c" /* 'apostrophe' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ṁ\x00"
                                "\x05" /* 'Ṁ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȯ\x00"
                                "\x04" /* 'Ȯ' 4 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ṁ\x00"
                                "\x05" /* 'ṁ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṙ\x00"
                                "\x05" /* 'ṙ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṡ\x00"
                                "\x05" /* 'ṡ' 5 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ż\x00"
                                "\x04" /* 'Ż' 4 */
                                "sacute\x00"
                                "\x0d" /* 'sacute' 13 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ȧ\x00"
                                "\x04" /* 'Ȧ' 4 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṙ\x00"
                                "\x05" /* 'Ṙ' 5 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ċ\x00"
                                "\x04" /* 'ċ' 4 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṫ\x00"
                                "\x05" /* 'Ṫ' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "›\x00"
                                "\x05" /* '›' 5 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "Ḃ\x00"
                                "\x05" /* 'Ḃ' 5 */
                                "dead_acute\x00"
                                "\x1c" /* 'dead_acute' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "X\x00"
                                "\x08" /* 'X' 8 */
                                "Ẋ\x00"
                                "\x05" /* 'Ẋ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḣ\x00"
                                "\x05" /* 'ḣ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "İ\x00"
                                "\x04" /* 'İ' 4 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṅ\x00"
                                "\x05" /* 'Ṅ' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "·\x00"
                                "\x04" /* '·' 4 */
                                "W\x00"
                                "\x20" /* 'W' 32 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₩\x00"
                                "\x05" /* '₩' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Ŵ\x00"
                                "\x04" /* 'Ŵ' 4 */
                                "g\x00"
                                "\x3b" /* 'g' 59 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "ġ\x00"
                                "\x04" /* 'ġ' 4 */
                                "breve\x00"
                                "\x0b" /* 'breve' 11 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ģ\x00"
                                "\x04" /* 'ģ' 4 */
                                "parenleft\x00"
                                "\x0f" /* 'parenleft' 15 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "a\x00"
                                "\x80\xc6" /* 'a' 198 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "å\x00"
                                "\x04" /* 'å' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "æ\x00"
                                "\x04" /* 'æ' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ä\x00"
                                "\x04" /* 'ä' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ä\x00"
                                "\x04" /* 'ä' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "á\x00"
                                "\x04" /* 'á' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "á\x00"
                                "\x04" /* 'á' 4 */
                                "asterisk\x00"
                                "\x0e" /* 'asterisk' 14 */
                                "å\x00"
                                "\x04" /* 'å' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ą\x00"
                                "\x04" /* 'ą' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "ã\x00"
                                "\x04" /* 'ã' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "â\x00"
                                "\x04" /* 'â' 4 */
                                "parenleft\x00"
                                "\x0f" /* 'parenleft' 15 */
                                "ă\x00"
                                "\x04" /* 'ă' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "à\x00"
                                "\x04" /* 'à' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "â\x00"
                                "\x04" /* 'â' 4 */
                                "Greek_IOTA\x00"
                                "\x2a" /* 'Greek_IOTA' 42 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ϊ\x00"
                                "\x04" /* 'Ϊ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ί\x00"
                                "\x04" /* 'Ί' 4 */
                                "Greek_iota\x00"
                                "\x90\xfe" /* 'Greek_iota' 4350 */
                                "dead_grave\x00"
                                "\x82\x10" /* 'dead_grave' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῂ\x00"
                                "\x05" /* 'ῂ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾲ\x00"
                                "\x05" /* 'ᾲ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῲ\x00"
                                "\x05" /* 'ῲ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "dead_tilde\x00"
                                "\x82\x10" /* 'dead_tilde' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῇ\x00"
                                "\x05" /* 'ῇ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾷ\x00"
                                "\x05" /* 'ᾷ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῷ\x00"
                                "\x05" /* 'ῷ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾈ\x00"
                                "\x05" /* 'ᾈ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾐ\x00"
                                "\x05" /* 'ᾐ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾀ\x00"
                                "\x05" /* 'ᾀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾘ\x00"
                                "\x05" /* 'ᾘ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾠ\x00"
                                "\x05" /* 'ᾠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾨ\x00"
                                "\x05" /* 'ᾨ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾼ\x00"
                                "\x05" /* 'ᾼ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾉ\x00"
                                "\x05" /* 'ᾉ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾑ\x00"
                                "\x05" /* 'ᾑ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾁ\x00"
                                "\x05" /* 'ᾁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾙ\x00"
                                "\x05" /* 'ᾙ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾡ\x00"
                                "\x05" /* 'ᾡ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾩ\x00"
                                "\x05" /* 'ᾩ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῃ\x00"
                                "\x05" /* 'ῃ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾈ\x00"
                                "\x05" /* 'ᾈ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾐ\x00"
                                "\x05" /* 'ᾐ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾀ\x00"
                                "\x05" /* 'ᾀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾘ\x00"
                                "\x05" /* 'ᾘ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾠ\x00"
                                "\x05" /* 'ᾠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾨ\x00"
                                "\x05" /* 'ᾨ' 5 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ϊ\x00"
                                "\x04" /* 'ϊ' 4 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾳ\x00"
                                "\x05" /* 'ᾳ' 5 */
                                "acute\x00"
                                "\x82\x0b" /* 'acute' 523 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ῌ\x00"
                                "\x05" /* 'ῌ' 5 */
                                "apostrophe\x00"
                                "\x82\x10" /* 'apostrophe' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_omegaaccent\x00"
                                "\x18" /* 'Greek_omegaaccent' 24 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "asciitilde\x00"
                                "\x82\x10" /* 'asciitilde' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῇ\x00"
                                "\x05" /* 'ῇ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾷ\x00"
                                "\x05" /* 'ᾷ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῷ\x00"
                                "\x05" /* 'ῷ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῳ\x00"
                                "\x05" /* 'ῳ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ῼ\x00"
                                "\x05" /* 'ῼ' 5 */
                                "dead_acute\x00"
                                "\x82\x10" /* 'dead_acute' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_alphaaccent\x00"
                                "\x18" /* 'Greek_alphaaccent' 24 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾉ\x00"
                                "\x05" /* 'ᾉ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾑ\x00"
                                "\x05" /* 'ᾑ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾁ\x00"
                                "\x05" /* 'ᾁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾙ\x00"
                                "\x05" /* 'ᾙ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾡ\x00"
                                "\x05" /* 'ᾡ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾩ\x00"
                                "\x05" /* 'ᾩ' 5 */
                                "grave\x00"
                                "\x82\x0b" /* 'grave' 523 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῂ\x00"
                                "\x05" /* 'ῂ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾲ\x00"
                                "\x05" /* 'ᾲ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῲ\x00"
                                "\x05" /* 'ῲ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "Greek_etaaccent\x00"
                                "\x16" /* 'Greek_etaaccent' 22 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "1\x00"
                                "\x5d" /* '1' 93 */
                                "1\x00"
                                "\x0b" /* '1' 11 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "⅒\x00"
                                "\x05" /* '⅒' 5 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "⅐\x00"
                                "\x05" /* '⅐' 5 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⅛\x00"
                                "\x05" /* '⅛' 5 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "⅓\x00"
                                "\x05" /* '⅓' 5 */
                                "2\x00"
                                "\x07" /* '2' 7 */
                                "½\x00"
                                "\x04" /* '½' 4 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "⅙\x00"
                                "\x05" /* '⅙' 5 */
                                "4\x00"
                                "\x07" /* '4' 7 */
                                "¼\x00"
                                "\x04" /* '¼' 4 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⅕\x00"
                                "\x05" /* '⅕' 5 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "⅑\x00"
                                "\x05" /* '⅑' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "¹\x00"
                                "\x04" /* '¹' 4 */
                                "Greek_OMICRON\x00"
                                "\x1f" /* 'Greek_OMICRON' 31 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ό\x00"
                                "\x04" /* 'Ό' 4 */
                                "C\x00"
                                "\x80\x82" /* 'C' 130 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "Ċ\x00"
                                "\x04" /* 'Ċ' 4 */
                                "C\x00"
                                "\x0e" /* 'C' 14 */
                                "C\x00"
                                "\x0b" /* 'C' 11 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "☭\x00"
                                "\x05" /* '☭' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Č\x00"
                                "\x04" /* 'Č' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "₠\x00"
                                "\x05" /* '₠' 5 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ć\x00"
                                "\x04" /* 'Ć' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "₢\x00"
                                "\x05" /* '₢' 5 */
                                "bar\x00"
                                "\x09" /* 'bar' 9 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ç\x00"
                                "\x04" /* 'Ç' 4 */
                                "slash\x00"
                                "\x0c" /* 'slash' 12 */
                                "₡\x00"
                                "\x05" /* '₡' 5 */
                                "exclam\x00"
                                "\x81\xe6" /* 'exclam' 486 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẉ\x00"
                                "\x05" /* 'Ẉ' 5 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ạ\x00"
                                "\x05" /* 'ạ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "exclam\x00"
                                "\x0c" /* 'exclam' 12 */
                                "¡\x00"
                                "\x04" /* '¡' 4 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẹ\x00"
                                "\x05" /* 'ẹ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ọ\x00"
                                "\x05" /* 'ọ' 5 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḷ\x00"
                                "\x05" /* 'ḷ' 5 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṭ\x00"
                                "\x05" /* 'ṭ' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỵ\x00"
                                "\x05" /* 'ỵ' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "ḅ\x00"
                                "\x05" /* 'ḅ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ị\x00"
                                "\x05" /* 'ị' 5 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḳ\x00"
                                "\x05" /* 'ḳ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṇ\x00"
                                "\x05" /* 'ṇ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "V\x00"
                                "\x08" /* 'V' 8 */
                                "Ṿ\x00"
                                "\x05" /* 'Ṿ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ụ\x00"
                                "\x05" /* 'ụ' 5 */
                                "z\x00"
                                "\x08" /* 'z' 8 */
                                "ẓ\x00"
                                "\x05" /* 'ẓ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḥ\x00"
                                "\x05" /* 'Ḥ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẹ\x00"
                                "\x05" /* 'Ẹ' 5 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṣ\x00"
                                "\x05" /* 'Ṣ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỵ\x00"
                                "\x05" /* 'Ỵ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḍ\x00"
                                "\x05" /* 'ḍ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḍ\x00"
                                "\x05" /* 'Ḍ' 5 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ợ\x00"
                                "\x05" /* 'ợ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ự\x00"
                                "\x05" /* 'ự' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ợ\x00"
                                "\x05" /* 'Ợ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẉ\x00"
                                "\x05" /* 'ẉ' 5 */
                                "v\x00"
                                "\x08" /* 'v' 8 */
                                "ṿ\x00"
                                "\x05" /* 'ṿ' 5 */
                                "question\x00"
                                "\x0f" /* 'question' 15 */
                                "‽\x00"
                                "\x05" /* '‽' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ṃ\x00"
                                "\x05" /* 'Ṃ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ọ\x00"
                                "\x05" /* 'Ọ' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ṃ\x00"
                                "\x05" /* 'ṃ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṛ\x00"
                                "\x05" /* 'ṛ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṣ\x00"
                                "\x05" /* 'ṣ' 5 */
                                "Z\x00"
                                "\x08" /* 'Z' 8 */
                                "Ẓ\x00"
                                "\x05" /* 'Ẓ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ạ\x00"
                                "\x05" /* 'Ạ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṛ\x00"
                                "\x05" /* 'Ṛ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḷ\x00"
                                "\x05" /* 'Ḷ' 5 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṭ\x00"
                                "\x05" /* 'Ṭ' 5 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḳ\x00"
                                "\x05" /* 'Ḳ' 5 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "Ḅ\x00"
                                "\x05" /* 'Ḅ' 5 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ự\x00"
                                "\x05" /* 'Ự' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḥ\x00"
                                "\x05" /* 'ḥ' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ị\x00"
                                "\x05" /* 'Ị' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṇ\x00"
                                "\x05" /* 'Ṇ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ụ\x00"
                                "\x05" /* 'Ụ' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "¦\x00"
                                "\x04" /* '¦' 4 */
                                "less\x00"
                                "\x81\x03" /* 'less' 259 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "←\x00"
                                "\x05" /* '←' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Č\x00"
                                "\x04" /* 'Č' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "«\x00"
                                "\x04" /* '«' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ě\x00"
                                "\x04" /* 'ě' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ľ\x00"
                                "\x04" /* 'ľ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ť\x00"
                                "\x04" /* 'ť' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "ˇ\x00"
                                "\x04" /* 'ˇ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ň\x00"
                                "\x04" /* 'ň' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "≤\x00"
                                "\x05" /* '≤' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ž\x00"
                                "\x04" /* 'ž' 4 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "♥\x00"
                                "\x05" /* '♥' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ě\x00"
                                "\x04" /* 'Ě' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Š\x00"
                                "\x04" /* 'Š' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "ď\x00"
                                "\x04" /* 'ď' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Ď\x00"
                                "\x04" /* 'Ď' 4 */
                                "quotedbl\x00"
                                "\x0f" /* 'quotedbl' 15 */
                                "“\x00"
                                "\x05" /* '“' 5 */
                                "underscore\x00"
                                "\x11" /* 'underscore' 17 */
                                "≤\x00"
                                "\x05" /* '≤' 5 */
                                "apostrophe\x00"
                                "\x11" /* 'apostrophe' 17 */
                                "‘\x00"
                                "\x05" /* '‘' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ř\x00"
                                "\x04" /* 'ř' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "š\x00"
                                "\x04" /* 'š' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ž\x00"
                                "\x04" /* 'Ž' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ř\x00"
                                "\x04" /* 'Ř' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "č\x00"
                                "\x04" /* 'č' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ľ\x00"
                                "\x04" /* 'Ľ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ť\x00"
                                "\x04" /* 'Ť' 4 */
                                "slash\x00"
                                "\x0a" /* 'slash' 10 */
                                "\\\x00"
                                "\x03" /* '\' 3 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "⋄\x00"
                                "\x05" /* '⋄' 5 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ň\x00"
                                "\x04" /* 'Ň' 4 */
                                "KP_Divide\x00"
                                "\x80\xea" /* 'KP_Divide' 234 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǥ\x00"
                                "\x04" /* 'ǥ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ø\x00"
                                "\x04" /* 'ø' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ł\x00"
                                "\x04" /* 'ł' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ŧ\x00"
                                "\x04" /* 'ŧ' 4 */
                                "b\x00"
                                "\x07" /* 'b' 7 */
                                "ƀ\x00"
                                "\x04" /* 'ƀ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ɨ\x00"
                                "\x04" /* 'ɨ' 4 */
                                "Cyrillic_GHE\x00"
                                "\x12" /* 'Cyrillic_GHE' 18 */
                                "Ғ\x00"
                                "\x04" /* 'Ғ' 4 */
                                "leftarrow\x00"
                                "\x10" /* 'leftarrow' 16 */
                                "↚\x00"
                                "\x05" /* '↚' 5 */
                                "Cyrillic_KA\x00"
                                "\x11" /* 'Cyrillic_KA' 17 */
                                "Ҟ\x00"
                                "\x04" /* 'Ҟ' 4 */
                                "rightarrow\x00"
                                "\x11" /* 'rightarrow' 17 */
                                "↛\x00"
                                "\x05" /* '↛' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ƶ\x00"
                                "\x04" /* 'ƶ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǥ\x00"
                                "\x04" /* 'Ǥ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ħ\x00"
                                "\x04" /* 'Ħ' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "đ\x00"
                                "\x04" /* 'đ' 4 */
                                "Cyrillic_ka\x00"
                                "\x11" /* 'Cyrillic_ka' 17 */
                                "ҟ\x00"
                                "\x04" /* 'ҟ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Đ\x00"
                                "\x04" /* 'Đ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ø\x00"
                                "\x04" /* 'Ø' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ƶ\x00"
                                "\x04" /* 'Ƶ' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ł\x00"
                                "\x04" /* 'Ł' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ŧ\x00"
                                "\x04" /* 'Ŧ' 4 */
                                "Cyrillic_ghe\x00"
                                "\x12" /* 'Cyrillic_ghe' 18 */
                                "ғ\x00"
                                "\x04" /* 'ғ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ħ\x00"
                                "\x04" /* 'ħ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ɨ\x00"
                                "\x04" /* 'Ɨ' 4 */
                                "F\x00"
                                "\x28" /* 'F' 40 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ḟ\x00"
                                "\x05" /* 'Ḟ' 5 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ﬄ\x00"
                                "\x05" /* 'ﬄ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ﬃ\x00"
                                "\x05" /* 'ﬃ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "₣\x00"
                                "\x05" /* '₣' 5 */
                                "e\x00"
                                "\x80\xb4" /* 'e' 180 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "ė\x00"
                                "\x04" /* 'ė' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ě\x00"
                                "\x04" /* 'ě' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ə\x00"
                                "\x04" /* 'ə' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ë\x00"
                                "\x04" /* 'ë' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ë\x00"
                                "\x04" /* 'ë' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "é\x00"
                                "\x04" /* 'é' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "é\x00"
                                "\x04" /* 'é' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ę\x00"
                                "\x04" /* 'ę' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "ê\x00"
                                "\x04" /* 'ê' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "è\x00"
                                "\x04" /* 'è' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "ê\x00"
                                "\x04" /* 'ê' 4 */
                                "o\x00"
                                "\x81\x06" /* 'o' 262 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "å\x00"
                                "\x04" /* 'å' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "œ\x00"
                                "\x04" /* 'œ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "°\x00"
                                "\x04" /* '°' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ö\x00"
                                "\x04" /* 'ö' 4 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ẙ\x00"
                                "\x05" /* 'ẙ' 5 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ů\x00"
                                "\x04" /* 'ů' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ö\x00"
                                "\x04" /* 'ö' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "ó\x00"
                                "\x04" /* 'ó' 4 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẘ\x00"
                                "\x05" /* 'ẘ' 5 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ó\x00"
                                "\x04" /* 'ó' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "®\x00"
                                "\x04" /* '®' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Å\x00"
                                "\x04" /* 'Å' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "®\x00"
                                "\x04" /* '®' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "õ\x00"
                                "\x04" /* 'õ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "ø\x00"
                                "\x04" /* 'ø' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "ô\x00"
                                "\x04" /* 'ô' 4 */
                                "X\x00"
                                "\x07" /* 'X' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "ò\x00"
                                "\x04" /* 'ò' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ů\x00"
                                "\x04" /* 'Ů' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "ô\x00"
                                "\x04" /* 'ô' 4 */
                                "l\x00"
                                "\x44" /* 'l' 68 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "£\x00"
                                "\x04" /* '£' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ľ\x00"
                                "\x04" /* 'ľ' 4 */
                                "v\x00"
                                "\x06" /* 'v' 6 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ĺ\x00"
                                "\x04" /* 'ĺ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ļ\x00"
                                "\x04" /* 'ļ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "ł\x00"
                                "\x04" /* 'ł' 4 */
                                "Greek_upsilon\x00"
                                "\x2d" /* 'Greek_upsilon' 45 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ϋ\x00"
                                "\x04" /* 'ϋ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ύ\x00"
                                "\x04" /* 'ύ' 4 */
                                "t\x00"
                                "\x52" /* 't' 82 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ŧ\x00"
                                "\x04" /* 'ŧ' 4 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ṫ\x00"
                                "\x05" /* 'ṫ' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ť\x00"
                                "\x04" /* 'ť' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ţ\x00"
                                "\x04" /* 'ţ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "ŧ\x00"
                                "\x04" /* 'ŧ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "þ\x00"
                                "\x04" /* 'þ' 4 */
                                "diaeresis\x00"
                                "\x80\xe7" /* 'diaeresis' 231 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ä\x00"
                                "\x04" /* 'ä' 4 */
                                "dead_grave\x00"
                                "\x11" /* 'dead_grave' 17 */
                                "῭\x00"
                                "\x05" /* '῭' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ë\x00"
                                "\x04" /* 'ë' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ö\x00"
                                "\x04" /* 'ö' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ÿ\x00"
                                "\x04" /* 'ÿ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ï\x00"
                                "\x04" /* 'ï' 4 */
                                "dead_tilde\x00"
                                "\x11" /* 'dead_tilde' 17 */
                                "῁\x00"
                                "\x05" /* '῁' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ü\x00"
                                "\x04" /* 'ü' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ë\x00"
                                "\x04" /* 'Ë' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ÿ\x00"
                                "\x04" /* 'Ÿ' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ö\x00"
                                "\x04" /* 'Ö' 4 */
                                "asterisk\x00"
                                "\x0f" /* 'asterisk' 15 */
                                "⍣\x00"
                                "\x05" /* '⍣' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ä\x00"
                                "\x04" /* 'Ä' 4 */
                                "asciitilde\x00"
                                "\x11" /* 'asciitilde' 17 */
                                "῁\x00"
                                "\x05" /* '῁' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "⍩\x00"
                                "\x05" /* '⍩' 5 */
                                "dead_acute\x00"
                                "\x10" /* 'dead_acute' 16 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ï\x00"
                                "\x04" /* 'Ï' 4 */
                                "grave\x00"
                                "\x0c" /* 'grave' 12 */
                                "῭\x00"
                                "\x05" /* '῭' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ü\x00"
                                "\x04" /* 'Ü' 4 */
                                "space\x00"
                                "\x80\x92" /* 'space' 146 */
                                "minus\x00"
                                "\x0a" /* 'minus' 10 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                " \x00"
                                "\x05" /* ' ' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ˇ\x00"
                                "\x04" /* 'ˇ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                " \x00"
                                "\x04" /* ' ' 4 */
                                "apostrophe\x00"
                                "\x0f" /* 'apostrophe' 15 */
                                "'\x00"
                                "\x03" /* ''' 3 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "¸\x00"
                                "\x04" /* '¸' 4 */
                                "asciitilde\x00"
                                "\x0f" /* 'asciitilde' 15 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "greater\x00"
                                "\x0c" /* 'greater' 12 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "parenleft\x00"
                                "\x0f" /* 'parenleft' 15 */
                                "˘\x00"
                                "\x04" /* '˘' 4 */
                                "grave\x00"
                                "\x0a" /* 'grave' 10 */
                                "`\x00"
                                "\x03" /* '`' 3 */
                                "asciicircum\x00"
                                "\x10" /* 'asciicircum' 16 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "percent\x00"
                                "\x11" /* 'percent' 17 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "‰\x00"
                                "\x05" /* '‰' 5 */
                                "y\x00"
                                "\x62" /* 'y' 98 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ÿ\x00"
                                "\x04" /* 'ÿ' 4 */
                                "equal\x00"
                                "\x0b" /* 'equal' 11 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ÿ\x00"
                                "\x04" /* 'ÿ' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "ý\x00"
                                "\x04" /* 'ý' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ý\x00"
                                "\x04" /* 'ý' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "ŷ\x00"
                                "\x04" /* 'ŷ' 4 */
                                "b\x00"
                                "\x81\xfe" /* 'b' 510 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ḃ\x00"
                                "\x05" /* 'ḃ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ă\x00"
                                "\x04" /* 'ă' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῐ\x00"
                                "\x05" /* 'Ῐ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῐ\x00"
                                "\x05" /* 'ῐ' 5 */
                                "exclam\x00"
                                "\x18" /* 'exclam' 24 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ĕ\x00"
                                "\x04" /* 'ĕ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ŏ\x00"
                                "\x04" /* 'ŏ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῠ\x00"
                                "\x05" /* 'ῠ' 5 */
                                "dead_belowdot\x00"
                                "\x1f" /* 'dead_belowdot' 31 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Й\x00"
                                "\x04" /* 'Й' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ĭ\x00"
                                "\x04" /* 'ĭ' 4 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "ӑ\x00"
                                "\x04" /* 'ӑ' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ў\x00"
                                "\x04" /* 'Ў' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ŭ\x00"
                                "\x04" /* 'ŭ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾰ\x00"
                                "\x05" /* 'Ᾰ' 5 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ӗ\x00"
                                "\x04" /* 'ӗ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ĕ\x00"
                                "\x04" /* 'Ĕ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "й\x00"
                                "\x04" /* 'й' 4 */
                                "Cyrillic_zhe\x00"
                                "\x12" /* 'Cyrillic_zhe' 18 */
                                "ӂ\x00"
                                "\x04" /* 'ӂ' 4 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾰ\x00"
                                "\x05" /* 'ᾰ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ŏ\x00"
                                "\x04" /* 'Ŏ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ă\x00"
                                "\x04" /* 'Ă' 4 */
                                "Cyrillic_A\x00"
                                "\x10" /* 'Cyrillic_A' 16 */
                                "Ӑ\x00"
                                "\x04" /* 'Ӑ' 4 */
                                "comma\x00"
                                "\x17" /* 'comma' 23 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "Cyrillic_ZHE\x00"
                                "\x12" /* 'Cyrillic_ZHE' 18 */
                                "Ӂ\x00"
                                "\x04" /* 'Ӂ' 4 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ӗ\x00"
                                "\x04" /* 'Ӗ' 4 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ĭ\x00"
                                "\x04" /* 'Ĭ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ŭ\x00"
                                "\x04" /* 'Ŭ' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ў\x00"
                                "\x04" /* 'ў' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῠ\x00"
                                "\x05" /* 'Ῠ' 5 */
                                "i\x00"
                                "\x80\xbd" /* 'i' 189 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "ı\x00"
                                "\x04" /* 'ı' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ï\x00"
                                "\x04" /* 'ï' 4 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ĳ\x00"
                                "\x04" /* 'ĳ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ï\x00"
                                "\x04" /* 'ï' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "í\x00"
                                "\x04" /* 'í' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "í\x00"
                                "\x04" /* 'í' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "į\x00"
                                "\x04" /* 'į' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "ĩ\x00"
                                "\x04" /* 'ĩ' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "î\x00"
                                "\x04" /* 'î' 4 */
                                "semicolon\x00"
                                "\x0f" /* 'semicolon' 15 */
                                "į\x00"
                                "\x04" /* 'į' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "ì\x00"
                                "\x04" /* 'ì' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "î\x00"
                                "\x04" /* 'î' 4 */
                                "k\x00"
                                "\x15" /* 'k' 21 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ĸ\x00"
                                "\x04" /* 'ĸ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ķ\x00"
                                "\x04" /* 'ķ' 4 */
                                "n\x00"
                                "\x3f" /* 'n' 63 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ŋ\x00"
                                "\x04" /* 'ŋ' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ň\x00"
                                "\x04" /* 'ň' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ń\x00"
                                "\x04" /* 'ń' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ņ\x00"
                                "\x04" /* 'ņ' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "ñ\x00"
                                "\x04" /* 'ñ' 4 */
                                "equal\x00"
                                "\x80\xd3" /* 'equal' 211 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "₩\x00"
                                "\x05" /* '₩' 5 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ő\x00"
                                "\x04" /* 'ő' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӳ\x00"
                                "\x04" /* 'Ӳ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ű\x00"
                                "\x04" /* 'ű' 4 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "₫\x00"
                                "\x05" /* '₫' 5 */
                                "underscore\x00"
                                "\x11" /* 'underscore' 17 */
                                "≡\x00"
                                "\x05" /* '≡' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ő\x00"
                                "\x04" /* 'Ő' 4 */
                                "Cyrillic_ES\x00"
                                "\x12" /* 'Cyrillic_ES' 18 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "₤\x00"
                                "\x05" /* '₤' 5 */
                                "slash\x00"
                                "\x0c" /* 'slash' 12 */
                                "≠\x00"
                                "\x05" /* '≠' 5 */
                                "Cyrillic_IE\x00"
                                "\x12" /* 'Cyrillic_IE' 18 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "₦\x00"
                                "\x05" /* '₦' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ű\x00"
                                "\x04" /* 'Ű' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӳ\x00"
                                "\x04" /* 'ӳ' 4 */
                                "7\x00"
                                "\x0b" /* '7' 11 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⅞\x00"
                                "\x05" /* '⅞' 5 */
                                "parenright\x00"
                                "\x81\x24" /* 'parenright' 292 */
                                "minus\x00"
                                "\x0a" /* 'minus' 10 */
                                "}\x00"
                                "\x03" /* '}' 3 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἰ\x00"
                                "\x05" /* 'Ἰ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἰ\x00"
                                "\x05" /* 'ἰ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὀ\x00"
                                "\x05" /* 'Ὀ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὐ\x00"
                                "\x05" /* 'ὐ' 5 */
                                "parenright\x00"
                                "\x0f" /* 'parenright' 15 */
                                "]\x00"
                                "\x03" /* ']' 3 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἐ\x00"
                                "\x05" /* 'ἐ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἀ\x00"
                                "\x05" /* 'Ἀ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὀ\x00"
                                "\x05" /* 'ὀ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἠ\x00"
                                "\x05" /* 'ἠ' 5 */
                                "Greek_rho\x00"
                                "\x10" /* 'Greek_rho' 16 */
                                "ῤ\x00"
                                "\x05" /* 'ῤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἀ\x00"
                                "\x05" /* 'ἀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἠ\x00"
                                "\x05" /* 'Ἠ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἐ\x00"
                                "\x05" /* 'Ἐ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὠ\x00"
                                "\x05" /* 'ὠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὠ\x00"
                                "\x05" /* 'Ὠ' 5 */
                                "x\x00"
                                "\x18" /* 'x' 24 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "×\x00"
                                "\x04" /* '×' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "Greek_epsilon\x00"
                                "\x1f" /* 'Greek_epsilon' 31 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "έ\x00"
                                "\x04" /* 'έ' 4 */
                                "braceleft\x00"
                                "\x1c" /* 'braceleft' 28 */
                                "braceright\x00"
                                "\x11" /* 'braceright' 17 */
                                "∅\x00"
                                "\x05" /* '∅' 5 */
                                "underbar\x00"
                                "\x81\x2c" /* 'underbar' 300 */
                                "1\x00"
                                "\x08" /* '1' 8 */
                                "₁\x00"
                                "\x05" /* '₁' 5 */
                                "KP_4\x00"
                                "\x0b" /* 'KP_4' 11 */
                                "₄\x00"
                                "\x05" /* '₄' 5 */
                                "KP_6\x00"
                                "\x0b" /* 'KP_6' 11 */
                                "₆\x00"
                                "\x05" /* '₆' 5 */
                                "KP_8\x00"
                                "\x0b" /* 'KP_8' 11 */
                                "₈\x00"
                                "\x05" /* '₈' 5 */
                                "KP_9\x00"
                                "\x0b" /* 'KP_9' 11 */
                                "₉\x00"
                                "\x05" /* '₉' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₌\x00"
                                "\x05" /* '₌' 5 */
                                "KP_Space\x00"
                                "\x0f" /* 'KP_Space' 15 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "₇\x00"
                                "\x05" /* '₇' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "₎\x00"
                                "\x05" /* '₎' 5 */
                                "KP_7\x00"
                                "\x0b" /* 'KP_7' 11 */
                                "₇\x00"
                                "\x05" /* '₇' 5 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "₈\x00"
                                "\x05" /* '₈' 5 */
                                "KP_1\x00"
                                "\x0b" /* 'KP_1' 11 */
                                "₁\x00"
                                "\x05" /* '₁' 5 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "₃\x00"
                                "\x05" /* '₃' 5 */
                                "2\x00"
                                "\x08" /* '2' 8 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "₊\x00"
                                "\x05" /* '₊' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "₆\x00"
                                "\x05" /* '₆' 5 */
                                "4\x00"
                                "\x08" /* '4' 8 */
                                "₄\x00"
                                "\x05" /* '₄' 5 */
                                "KP_3\x00"
                                "\x0b" /* 'KP_3' 11 */
                                "₃\x00"
                                "\x05" /* '₃' 5 */
                                "KP_0\x00"
                                "\x0b" /* 'KP_0' 11 */
                                "₀\x00"
                                "\x05" /* '₀' 5 */
                                "KP_Add\x00"
                                "\x0d" /* 'KP_Add' 13 */
                                "₊\x00"
                                "\x05" /* '₊' 5 */
                                "KP_2\x00"
                                "\x0b" /* 'KP_2' 11 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "₅\x00"
                                "\x05" /* '₅' 5 */
                                "KP_5\x00"
                                "\x0b" /* 'KP_5' 11 */
                                "₅\x00"
                                "\x05" /* '₅' 5 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "₉\x00"
                                "\x05" /* '₉' 5 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "₀\x00"
                                "\x05" /* '₀' 5 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "₍\x00"
                                "\x05" /* '₍' 5 */
                                "KP_Equal\x00"
                                "\x0f" /* 'KP_Equal' 15 */
                                "₌\x00"
                                "\x05" /* '₌' 5 */
                                "V\x00"
                                "\x09" /* 'V' 9 */
                                "L\x00"
                                "\x06" /* 'L' 6 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "u\x00"
                                "\x80\xbb" /* 'u' 187 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "ü\x00"
                                "\x04" /* 'ü' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ŭ\x00"
                                "\x04" /* 'ŭ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "ü\x00"
                                "\x04" /* 'ü' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "ú\x00"
                                "\x04" /* 'ú' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ú\x00"
                                "\x04" /* 'ú' 4 */
                                "asterisk\x00"
                                "\x0e" /* 'asterisk' 14 */
                                "ů\x00"
                                "\x04" /* 'ů' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ų\x00"
                                "\x04" /* 'ų' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "ũ\x00"
                                "\x04" /* 'ũ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "û\x00"
                                "\x04" /* 'û' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "ù\x00"
                                "\x04" /* 'ù' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "û\x00"
                                "\x04" /* 'û' 4 */
                                "breve\x00"
                                "\x15" /* 'breve' 21 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "z\x00"
                                "\x29" /* 'z' 41 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "ż\x00"
                                "\x04" /* 'ż' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ž\x00"
                                "\x04" /* 'ž' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ź\x00"
                                "\x04" /* 'ź' 4 */
                                "G\x00"
                                "\x3b" /* 'G' 59 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "Ġ\x00"
                                "\x04" /* 'Ġ' 4 */
                                "breve\x00"
                                "\x0b" /* 'breve' 11 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ģ\x00"
                                "\x04" /* 'Ģ' 4 */
                                "parenleft\x00"
                                "\x0f" /* 'parenleft' 15 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "Greek_ALPHA\x00"
                                "\x1d" /* 'Greek_ALPHA' 29 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ά\x00"
                                "\x04" /* 'Ά' 4 */
                                "bracketleft\x00"
                                "\x20" /* 'bracketleft' 32 */
                                "bracketright\x00"
                                "\x13" /* 'bracketright' 19 */
                                "⌷\x00"
                                "\x05" /* '⌷' 5 */
                                "H\x00"
                                "\x0f" /* 'H' 15 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "Ḩ\x00"
                                "\x05" /* 'Ḩ' 5 */
                                "8\x00"
                                "\x0b" /* '8' 11 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "∞\x00"
                                "\x05" /* '∞' 5 */
                                "3\x00"
                                "\x2b" /* '3' 43 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⅜\x00"
                                "\x05" /* '⅜' 5 */
                                "4\x00"
                                "\x07" /* '4' 7 */
                                "¾\x00"
                                "\x04" /* '¾' 4 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⅗\x00"
                                "\x05" /* '⅗' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "³\x00"
                                "\x04" /* '³' 4 */
                                "E\x00"
                                "\x80\xad" /* 'E' 173 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "Ė\x00"
                                "\x04" /* 'Ė' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ě\x00"
                                "\x04" /* 'Ě' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ë\x00"
                                "\x04" /* 'Ë' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ë\x00"
                                "\x04" /* 'Ë' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "É\x00"
                                "\x04" /* 'É' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "É\x00"
                                "\x04" /* 'É' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ę\x00"
                                "\x04" /* 'Ę' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "Ê\x00"
                                "\x04" /* 'Ê' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "È\x00"
                                "\x04" /* 'È' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Ê\x00"
                                "\x04" /* 'Ê' 4 */
                                "S\x00"
                                "\x60" /* 'S' 96 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ṡ\x00"
                                "\x05" /* 'Ṡ' 5 */
                                "exclam\x00"
                                "\x0c" /* 'exclam' 12 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Š\x00"
                                "\x04" /* 'Š' 4 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "ẞ\x00"
                                "\x05" /* 'ẞ' 5 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ś\x00"
                                "\x04" /* 'Ś' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ş\x00"
                                "\x04" /* 'Ş' 4 */
                                "2\x00"
                                "\x24" /* '2' 36 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "⅔\x00"
                                "\x05" /* '⅔' 5 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⅖\x00"
                                "\x05" /* '⅖' 5 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "Y\x00"
                                "\x62" /* 'Y' 98 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ÿ\x00"
                                "\x04" /* 'Ÿ' 4 */
                                "equal\x00"
                                "\x0b" /* 'equal' 11 */
                                "¥\x00"
                                "\x04" /* '¥' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ÿ\x00"
                                "\x04" /* 'Ÿ' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "Ý\x00"
                                "\x04" /* 'Ý' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ý\x00"
                                "\x04" /* 'Ý' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Ŷ\x00"
                                "\x04" /* 'Ŷ' 4 */
                                "f\x00"
                                "\x36" /* 'f' 54 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ḟ\x00"
                                "\x05" /* 'ḟ' 5 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ﬂ\x00"
                                "\x05" /* 'ﬂ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ﬁ\x00"
                                "\x05" /* 'ﬁ' 5 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "ſ\x00"
                                "\x04" /* 'ſ' 4 */
                                "f\x00"
                                "\x08" /* 'f' 8 */
                                "ﬀ\x00"
                                "\x05" /* 'ﬀ' 5 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ſ\x00"
                                "\x04" /* 'ſ' 4 */
                                "Greek_omicron\x00"
                                "\x1f" /* 'Greek_omicron' 31 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ό\x00"
                                "\x04" /* 'ό' 4 */
                                "Greek_eta\x00"
                                "\x1b" /* 'Greek_eta' 27 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ή\x00"
                                "\x04" /* 'ή' 4 */
                                "d\x00"
                                "\x4c" /* 'd' 76 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "đ\x00"
                                "\x04" /* 'đ' 4 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ḋ\x00"
                                "\x05" /* 'ḋ' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ď\x00"
                                "\x04" /* 'ď' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "⌀\x00"
                                "\x05" /* '⌀' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₫\x00"
                                "\x05" /* '₫' 5 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "ḑ\x00"
                                "\x05" /* 'ḑ' 5 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ð\x00"
                                "\x04" /* 'ð' 4 */
                                "D\x00"
                                "\x38" /* 'D' 56 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Đ\x00"
                                "\x04" /* 'Đ' 4 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ḋ\x00"
                                "\x05" /* 'Ḋ' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ď\x00"
                                "\x04" /* 'Ď' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ð\x00"
                                "\x04" /* 'Ð' 4 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "Ḑ\x00"
                                "\x05" /* 'Ḑ' 5 */
                                "quotedbl\x00"
                                "\x83\x6e" /* 'quotedbl' 878 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẅ\x00"
                                "\x05" /* 'Ẅ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ä\x00"
                                "\x04" /* 'ä' 4 */
                                "Greek_IOTA\x00"
                                "\x10" /* 'Greek_IOTA' 16 */
                                "Ϊ\x00"
                                "\x04" /* 'Ϊ' 4 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ϊ\x00"
                                "\x04" /* 'ϊ' 4 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "“\x00"
                                "\x05" /* '“' 5 */
                                "Umacron\x00"
                                "\x0e" /* 'Umacron' 14 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "Cyrillic_ZE\x00"
                                "\x11" /* 'Cyrillic_ZE' 17 */
                                "Ӟ\x00"
                                "\x04" /* 'Ӟ' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ë\x00"
                                "\x04" /* 'ë' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ö\x00"
                                "\x04" /* 'ö' 4 */
                                "Cyrillic_ze\x00"
                                "\x11" /* 'Cyrillic_ze' 17 */
                                "ӟ\x00"
                                "\x04" /* 'ӟ' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ẗ\x00"
                                "\x05" /* 'ẗ' 5 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ϋ\x00"
                                "\x04" /* 'ϋ' 4 */
                                "dead_macron\x00"
                                "\x1d" /* 'dead_macron' 29 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ӥ\x00"
                                "\x04" /* 'Ӥ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ÿ\x00"
                                "\x04" /* 'ÿ' 4 */
                                "Cyrillic_O\x00"
                                "\x10" /* 'Cyrillic_O' 16 */
                                "Ӧ\x00"
                                "\x04" /* 'Ӧ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ï\x00"
                                "\x04" /* 'ï' 4 */
                                "Ukrainian_I\x00"
                                "\x11" /* 'Ukrainian_I' 17 */
                                "Ї\x00"
                                "\x04" /* 'Ї' 4 */
                                "dead_tilde\x00"
                                "\x1c" /* 'dead_tilde' 28 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Cyrillic_che\x00"
                                "\x12" /* 'Cyrillic_che' 18 */
                                "ӵ\x00"
                                "\x04" /* 'ӵ' 4 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "ӓ\x00"
                                "\x04" /* 'ӓ' 4 */
                                "x\x00"
                                "\x08" /* 'x' 8 */
                                "ẍ\x00"
                                "\x05" /* 'ẍ' 5 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӱ\x00"
                                "\x04" /* 'Ӱ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ü\x00"
                                "\x04" /* 'ü' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḧ\x00"
                                "\x05" /* 'Ḧ' 5 */
                                "Cyrillic_YERU\x00"
                                "\x13" /* 'Cyrillic_YERU' 19 */
                                "Ӹ\x00"
                                "\x04" /* 'Ӹ' 4 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ё\x00"
                                "\x04" /* 'ё' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ë\x00"
                                "\x04" /* 'Ë' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ÿ\x00"
                                "\x04" /* 'Ÿ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ӥ\x00"
                                "\x04" /* 'ӥ' 4 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Cyrillic_zhe\x00"
                                "\x12" /* 'Cyrillic_zhe' 18 */
                                "ӝ\x00"
                                "\x04" /* 'ӝ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "¨\x00"
                                "\x04" /* '¨' 4 */
                                "umacron\x00"
                                "\x0e" /* 'umacron' 14 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "Cyrillic_yeru\x00"
                                "\x13" /* 'Cyrillic_yeru' 19 */
                                "ӹ\x00"
                                "\x04" /* 'ӹ' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "̈́\x00"
                                "\x04" /* '̈́' 4 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẅ\x00"
                                "\x05" /* 'ẅ' 5 */
                                "Cyrillic_CHE\x00"
                                "\x12" /* 'Cyrillic_CHE' 18 */
                                "Ӵ\x00"
                                "\x04" /* 'Ӵ' 4 */
                                "Cyrillic_o\x00"
                                "\x10" /* 'Cyrillic_o' 16 */
                                "ӧ\x00"
                                "\x04" /* 'ӧ' 4 */
                                "Ukrainian_i\x00"
                                "\x11" /* 'Ukrainian_i' 17 */
                                "ї\x00"
                                "\x04" /* 'ї' 4 */
                                "Cyrillic_E\x00"
                                "\x10" /* 'Cyrillic_E' 16 */
                                "Ӭ\x00"
                                "\x04" /* 'Ӭ' 4 */
                                "underscore\x00"
                                "\x1c" /* 'underscore' 28 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "̈́\x00"
                                "\x04" /* '̈́' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ö\x00"
                                "\x04" /* 'Ö' 4 */
                                "macron\x00"
                                "\x18" /* 'macron' 24 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ä\x00"
                                "\x04" /* 'Ä' 4 */
                                "Cyrillic_A\x00"
                                "\x10" /* 'Cyrillic_A' 16 */
                                "Ӓ\x00"
                                "\x04" /* 'Ӓ' 4 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "„\x00"
                                "\x05" /* '„' 5 */
                                "asciitilde\x00"
                                "\x1c" /* 'asciitilde' 28 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "”\x00"
                                "\x05" /* '”' 5 */
                                "Cyrillic_ZHE\x00"
                                "\x12" /* 'Cyrillic_ZHE' 18 */
                                "Ӝ\x00"
                                "\x04" /* 'Ӝ' 4 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ё\x00"
                                "\x04" /* 'Ё' 4 */
                                "Cyrillic_e\x00"
                                "\x10" /* 'Cyrillic_e' 16 */
                                "ӭ\x00"
                                "\x04" /* 'ӭ' 4 */
                                "dead_acute\x00"
                                "\x10" /* 'dead_acute' 16 */
                                "̈́\x00"
                                "\x04" /* '̈́' 4 */
                                "X\x00"
                                "\x08" /* 'X' 8 */
                                "Ẍ\x00"
                                "\x05" /* 'Ẍ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḧ\x00"
                                "\x05" /* 'ḧ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ï\x00"
                                "\x04" /* 'Ï' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ü\x00"
                                "\x04" /* 'Ü' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӱ\x00"
                                "\x04" /* 'ӱ' 4 */
                                "Greek_UPSILON\x00"
                                "\x13" /* 'Greek_UPSILON' 19 */
                                "Ϋ\x00"
                                "\x04" /* 'Ϋ' 4 */
                                "plus\x00"
                                "\x36" /* 'plus' 54 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "±\x00"
                                "\x04" /* '±' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ơ\x00"
                                "\x04" /* 'ơ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ư\x00"
                                "\x04" /* 'ư' 4 */
                                "plus\x00"
                                "\x09" /* 'plus' 9 */
                                "#\x00"
                                "\x03" /* '#' 3 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ơ\x00"
                                "\x04" /* 'Ơ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ư\x00"
                                "\x04" /* 'Ư' 4 */
                                "cedilla\x00"
                                "\x80\xa8" /* 'cedilla' 168 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ģ\x00"
                                "\x04" /* 'ģ' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ç\x00"
                                "\x04" /* 'Ç' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ȩ\x00"
                                "\x04" /* 'ȩ' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ļ\x00"
                                "\x04" /* 'ļ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ţ\x00"
                                "\x04" /* 'ţ' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ķ\x00"
                                "\x04" /* 'ķ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ņ\x00"
                                "\x04" /* 'ņ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ģ\x00"
                                "\x04" /* 'Ģ' 4 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḩ\x00"
                                "\x05" /* 'Ḩ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ȩ\x00"
                                "\x04" /* 'Ȩ' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ş\x00"
                                "\x04" /* 'Ş' 4 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḑ\x00"
                                "\x05" /* 'ḑ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḑ\x00"
                                "\x05" /* 'Ḑ' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŗ\x00"
                                "\x04" /* 'ŗ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ş\x00"
                                "\x04" /* 'ş' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŗ\x00"
                                "\x04" /* 'Ŗ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ç\x00"
                                "\x04" /* 'ç' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ļ\x00"
                                "\x04" /* 'Ļ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ţ\x00"
                                "\x04" /* 'Ţ' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ķ\x00"
                                "\x04" /* 'Ķ' 4 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḩ\x00"
                                "\x05" /* 'ḩ' 5 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ņ\x00"
                                "\x04" /* 'Ņ' 4 */
                                "Greek_alpha\x00"
                                "\x1d" /* 'Greek_alpha' 29 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ά\x00"
                                "\x04" /* 'ά' 4 */
                                "dead_abovedot\x00"
                                "\x1a" /* 'dead_abovedot' 26 */
                                "f\x00"
                                "\x0b" /* 'f' 11 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ẛ\x00"
                                "\x05" /* 'ẛ' 5 */
                                "acute\x00"
                                "\x8c\x29" /* 'acute' 3113 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẃ\x00"
                                "\x05" /* 'Ẃ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǵ\x00"
                                "\x04" /* 'ǵ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "á\x00"
                                "\x04" /* 'á' 4 */
                                "Greek_IOTA\x00"
                                "\x10" /* 'Greek_IOTA' 16 */
                                "Ί\x00"
                                "\x04" /* 'Ί' 4 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ί\x00"
                                "\x04" /* 'ί' 4 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Greek_OMICRON\x00"
                                "\x13" /* 'Greek_OMICRON' 19 */
                                "Ό\x00"
                                "\x04" /* 'Ό' 4 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ć\x00"
                                "\x04" /* 'Ć' 4 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р́\x00"
                                "\x06" /* 'р́' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "é\x00"
                                "\x04" /* 'é' 4 */
                                "KP_Divide\x00"
                                "\x19" /* 'KP_Divide' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "Utilde\x00"
                                "\x0d" /* 'Utilde' 13 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ó\x00"
                                "\x04" /* 'ó' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ĺ\x00"
                                "\x04" /* 'ĺ' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ύ\x00"
                                "\x04" /* 'ύ' 4 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И́\x00"
                                "\x06" /* 'И́' 6 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ý\x00"
                                "\x04" /* 'ý' 4 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "idiaeresis\x00"
                                "\x11" /* 'idiaeresis' 17 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О́\x00"
                                "\x06" /* 'О́' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "í\x00"
                                "\x04" /* 'í' 4 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḱ\x00"
                                "\x05" /* 'ḱ' 5 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ń\x00"
                                "\x04" /* 'ń' 4 */
                                "ccedilla\x00"
                                "\x0f" /* 'ccedilla' 15 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Cyrillic_GHE\x00"
                                "\x12" /* 'Cyrillic_GHE' 18 */
                                "Ѓ\x00"
                                "\x04" /* 'Ѓ' 4 */
                                "dead_tilde\x00"
                                "\x2c" /* 'dead_tilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а́\x00"
                                "\x06" /* 'а́' 6 */
                                "parenright\x00"
                                "\x80\xfb" /* 'parenright' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р́\x00"
                                "\x06" /* 'Р́' 6 */
                                "Greek_epsilon\x00"
                                "\x13" /* 'Greek_epsilon' 19 */
                                "έ\x00"
                                "\x04" /* 'έ' 4 */
                                "Cyrillic_KA\x00"
                                "\x11" /* 'Cyrillic_KA' 17 */
                                "Ќ\x00"
                                "\x04" /* 'Ќ' 4 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У́\x00"
                                "\x06" /* 'У́' 6 */
                                "dead_abovering\x00"
                                "\x1e" /* 'dead_abovering' 30 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǽ\x00"
                                "\x04" /* 'Ǽ' 4 */
                                "omacron\x00"
                                "\x0e" /* 'omacron' 14 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ú\x00"
                                "\x04" /* 'ú' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ź\x00"
                                "\x04" /* 'ź' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǵ\x00"
                                "\x04" /* 'Ǵ' 4 */
                                "Greek_ALPHA\x00"
                                "\x11" /* 'Greek_ALPHA' 17 */
                                "Ά\x00"
                                "\x04" /* 'Ά' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "utilde\x00"
                                "\x0d" /* 'utilde' 13 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е́\x00"
                                "\x06" /* 'е́' 6 */
                                "emacron\x00"
                                "\x0e" /* 'emacron' 14 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "É\x00"
                                "\x04" /* 'É' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ś\x00"
                                "\x04" /* 'Ś' 4 */
                                "Greek_iotadieresis\x00"
                                "\x18" /* 'Greek_iotadieresis' 24 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ý\x00"
                                "\x04" /* 'Ý' 4 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и́\x00"
                                "\x06" /* 'и́' 6 */
                                "dead_dasia\x00"
                                "\x81\x0f" /* 'dead_dasia' 271 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1b" /* 'Greek_upsilondieresis' 27 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "Greek_omicron\x00"
                                "\x13" /* 'Greek_omicron' 19 */
                                "ό\x00"
                                "\x04" /* 'ό' 4 */
                                "Greek_eta\x00"
                                "\x0f" /* 'Greek_eta' 15 */
                                "ή\x00"
                                "\x04" /* 'ή' 4 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "Cyrillic_ka\x00"
                                "\x11" /* 'Cyrillic_ka' 17 */
                                "ќ\x00"
                                "\x04" /* 'ќ' 4 */
                                "Aring\x00"
                                "\x0b" /* 'Aring' 11 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "dead_psili\x00"
                                "\x80\xfb" /* 'dead_psili' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "quotedbl\x00"
                                "\x4b" /* 'quotedbl' 75 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Greek_alpha\x00"
                                "\x11" /* 'Greek_alpha' 17 */
                                "ά\x00"
                                "\x04" /* 'ά' 4 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẃ\x00"
                                "\x05" /* 'ẃ' 5 */
                                "Greek_ETA\x00"
                                "\x0f" /* 'Greek_ETA' 15 */
                                "Ή\x00"
                                "\x04" /* 'Ή' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о́\x00"
                                "\x06" /* 'о́' 6 */
                                "Emacron\x00"
                                "\x0e" /* 'Emacron' 14 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "Ooblique\x00"
                                "\x0e" /* 'Ooblique' 14 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ṕ\x00"
                                "\x05" /* 'ṕ' 5 */
                                "underscore\x00"
                                "\x2c" /* 'underscore' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ṕ\x00"
                                "\x05" /* 'Ṕ' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ḿ\x00"
                                "\x05" /* 'Ḿ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ó\x00"
                                "\x04" /* 'Ó' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ḿ\x00"
                                "\x05" /* 'ḿ' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŕ\x00"
                                "\x04" /* 'ŕ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ś\x00"
                                "\x04" /* 'ś' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ź\x00"
                                "\x04" /* 'Ź' 4 */
                                "macron\x00"
                                "\x28" /* 'macron' 40 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Á\x00"
                                "\x04" /* 'Á' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŕ\x00"
                                "\x04" /* 'Ŕ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ć\x00"
                                "\x04" /* 'ć' 4 */
                                "Idiaeresis\x00"
                                "\x11" /* 'Idiaeresis' 17 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ĺ\x00"
                                "\x04" /* 'Ĺ' 4 */
                                "Greek_EPSILON\x00"
                                "\x13" /* 'Greek_EPSILON' 19 */
                                "Έ\x00"
                                "\x04" /* 'Έ' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А́\x00"
                                "\x06" /* 'А́' 6 */
                                "comma\x00"
                                "\x17" /* 'comma' 23 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "asciitilde\x00"
                                "\x2c" /* 'asciitilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Ccedilla\x00"
                                "\x0f" /* 'Ccedilla' 15 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "slash\x00"
                                "\x15" /* 'slash' 21 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "aring\x00"
                                "\x0b" /* 'aring' 11 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḱ\x00"
                                "\x05" /* 'Ḱ' 5 */
                                "Omacron\x00"
                                "\x0e" /* 'Omacron' 14 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е́\x00"
                                "\x06" /* 'Е́' 6 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Greek_omega\x00"
                                "\x11" /* 'Greek_omega' 17 */
                                "ώ\x00"
                                "\x04" /* 'ώ' 4 */
                                "dead_diaeresis\x00"
                                "\x51" /* 'dead_diaeresis' 81 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "Greek_OMEGA\x00"
                                "\x11" /* 'Greek_OMEGA' 17 */
                                "Ώ\x00"
                                "\x04" /* 'Ώ' 4 */
                                "oslash\x00"
                                "\x0c" /* 'oslash' 12 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "Cyrillic_ghe\x00"
                                "\x12" /* 'Cyrillic_ghe' 18 */
                                "ѓ\x00"
                                "\x04" /* 'ѓ' 4 */
                                "parenleft\x00"
                                "\x81\x0e" /* 'parenleft' 270 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Í\x00"
                                "\x04" /* 'Í' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ń\x00"
                                "\x04" /* 'Ń' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ú\x00"
                                "\x04" /* 'Ú' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у́\x00"
                                "\x06" /* 'у́' 6 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǽ\x00"
                                "\x04" /* 'ǽ' 4 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Greek_UPSILON\x00"
                                "\x13" /* 'Greek_UPSILON' 19 */
                                "Ύ\x00"
                                "\x04" /* 'Ύ' 4 */
                                "Cyrillic_pe\x00"
                                "\x1d" /* 'Cyrillic_pe' 29 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "w\x00"
                                "\x14" /* 'w' 20 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "ŵ\x00"
                                "\x04" /* 'ŵ' 4 */
                                "Greek_ETA\x00"
                                "\x1b" /* 'Greek_ETA' 27 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ή\x00"
                                "\x04" /* 'Ή' 4 */
                                "4\x00"
                                "\x0b" /* '4' 11 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⅘\x00"
                                "\x05" /* '⅘' 5 */
                                "bracketright\x00"
                                "\x20" /* 'bracketright' 32 */
                                "bracketleft\x00"
                                "\x12" /* 'bracketleft' 18 */
                                "⌷\x00"
                                "\x05" /* '⌷' 5 */
                                "colon\x00"
                                "\x33" /* 'colon' 51 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "÷\x00"
                                "\x04" /* '÷' 4 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "☺\x00"
                                "\x05" /* '☺' 5 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "☹\x00"
                                "\x05" /* '☹' 5 */
                                "p\x00"
                                "\x1c" /* 'p' 28 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ṗ\x00"
                                "\x05" /* 'ṗ' 5 */
                                "exclam\x00"
                                "\x0c" /* 'exclam' 12 */
                                "¶\x00"
                                "\x04" /* '¶' 4 */
                                "underscore\x00"
                                "\x85\x23" /* 'underscore' 1315 */
                                "adiaeresis\x00"
                                "\x10" /* 'adiaeresis' 16 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "period\x00"
                                "\x24" /* 'period' 36 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "g\x00"
                                "\x08" /* 'g' 8 */
                                "ḡ\x00"
                                "\x05" /* 'ḡ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῑ\x00"
                                "\x05" /* 'Ῑ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῑ\x00"
                                "\x05" /* 'ῑ' 5 */
                                "1\x00"
                                "\x08" /* '1' 8 */
                                "₁\x00"
                                "\x05" /* '₁' 5 */
                                "exclam\x00"
                                "\x28" /* 'exclam' 40 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "KP_4\x00"
                                "\x0b" /* 'KP_4' 11 */
                                "₄\x00"
                                "\x05" /* '₄' 5 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "≤\x00"
                                "\x05" /* '≤' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̄\x00"
                                "\x06" /* 'р̄' 6 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "KP_6\x00"
                                "\x0b" /* 'KP_6' 11 */
                                "₆\x00"
                                "\x05" /* '₆' 5 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῡ\x00"
                                "\x05" /* 'ῡ' 5 */
                                "dead_belowdot\x00"
                                "\x2f" /* 'dead_belowdot' 47 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "KP_8\x00"
                                "\x0b" /* 'KP_8' 11 */
                                "₈\x00"
                                "\x05" /* '₈' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ӣ\x00"
                                "\x04" /* 'Ӣ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ȳ\x00"
                                "\x04" /* 'ȳ' 4 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̄\x00"
                                "\x06" /* 'О̄' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "KP_9\x00"
                                "\x0b" /* 'KP_9' 11 */
                                "₉\x00"
                                "\x05" /* '₉' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₌\x00"
                                "\x05" /* '₌' 5 */
                                "KP_Space\x00"
                                "\x0f" /* 'KP_Space' 15 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "dead_tilde\x00"
                                "\x1a" /* 'dead_tilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "₇\x00"
                                "\x05" /* '₇' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̄\x00"
                                "\x06" /* 'а̄' 6 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "₎\x00"
                                "\x05" /* '₎' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̄\x00"
                                "\x06" /* 'Р̄' 6 */
                                "KP_7\x00"
                                "\x0b" /* 'KP_7' 11 */
                                "₇\x00"
                                "\x05" /* '₇' 5 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӯ\x00"
                                "\x04" /* 'Ӯ' 4 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǣ\x00"
                                "\x04" /* 'Ǣ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "G\x00"
                                "\x08" /* 'G' 8 */
                                "Ḡ\x00"
                                "\x05" /* 'Ḡ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾱ\x00"
                                "\x05" /* 'Ᾱ' 5 */
                                "otilde\x00"
                                "\x0c" /* 'otilde' 12 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "₈\x00"
                                "\x05" /* '₈' 5 */
                                "KP_1\x00"
                                "\x0b" /* 'KP_1' 11 */
                                "₁\x00"
                                "\x05" /* '₁' 5 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "₃\x00"
                                "\x05" /* '₃' 5 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̄\x00"
                                "\x06" /* 'е̄' 6 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "2\x00"
                                "\x08" /* '2' 8 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ȳ\x00"
                                "\x04" /* 'Ȳ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ӣ\x00"
                                "\x04" /* 'ӣ' 4 */
                                "dead_ogonek\x00"
                                "\x1b" /* 'dead_ogonek' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "odiaeresis\x00"
                                "\x10" /* 'odiaeresis' 16 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "Otilde\x00"
                                "\x0c" /* 'Otilde' 12 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "quotedbl\x00"
                                "\x34" /* 'quotedbl' 52 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "₊\x00"
                                "\x05" /* '₊' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "₆\x00"
                                "\x05" /* '₆' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾱ\x00"
                                "\x05" /* 'ᾱ' 5 */
                                "dead_abovedot\x00"
                                "\x2b" /* 'dead_abovedot' 43 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̄\x00"
                                "\x06" /* 'о̄' 6 */
                                "4\x00"
                                "\x08" /* '4' 8 */
                                "₄\x00"
                                "\x05" /* '₄' 5 */
                                "KP_3\x00"
                                "\x0b" /* 'KP_3' 11 */
                                "₃\x00"
                                "\x05" /* '₃' 5 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "apostrophe\x00"
                                "\x11" /* 'apostrophe' 17 */
                                "⍘\x00"
                                "\x05" /* '⍘' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "KP_0\x00"
                                "\x0b" /* 'KP_0' 11 */
                                "₀\x00"
                                "\x05" /* '₀' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "KP_Add\x00"
                                "\x0d" /* 'KP_Add' 13 */
                                "₊\x00"
                                "\x05" /* '₊' 5 */
                                "Odiaeresis\x00"
                                "\x10" /* 'Odiaeresis' 16 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "KP_2\x00"
                                "\x0b" /* 'KP_2' 11 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̄\x00"
                                "\x06" /* 'А̄' 6 */
                                "asciitilde\x00"
                                "\x1a" /* 'asciitilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "₅\x00"
                                "\x05" /* '₅' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "≥\x00"
                                "\x05" /* '≥' 5 */
                                "semicolon\x00"
                                "\x19" /* 'semicolon' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "KP_5\x00"
                                "\x0b" /* 'KP_5' 11 */
                                "₅\x00"
                                "\x05" /* '₅' 5 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "₉\x00"
                                "\x05" /* '₉' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̄\x00"
                                "\x06" /* 'Е̄' 6 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "₀\x00"
                                "\x05" /* '₀' 5 */
                                "dead_diaeresis\x00"
                                "\x3a" /* 'dead_diaeresis' 58 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Adiaeresis\x00"
                                "\x10" /* 'Adiaeresis' 16 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "₍\x00"
                                "\x05" /* '₍' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӯ\x00"
                                "\x04" /* 'ӯ' 4 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǣ\x00"
                                "\x04" /* 'ǣ' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῡ\x00"
                                "\x05" /* 'Ῡ' 5 */
                                "KP_Equal\x00"
                                "\x0f" /* 'KP_Equal' 15 */
                                "₌\x00"
                                "\x05" /* '₌' 5 */
                                "v\x00"
                                "\x23" /* 'v' 35 */
                                "l\x00"
                                "\x06" /* 'l' 6 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ž\x00"
                                "\x04" /* 'ž' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ž\x00"
                                "\x04" /* 'Ž' 4 */
                                "slash\x00"
                                "\x0c" /* 'slash' 12 */
                                "√\x00"
                                "\x05" /* '√' 5 */
                                "P\x00"
                                "\x2b" /* 'P' 43 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ṗ\x00"
                                "\x05" /* 'Ṗ' 5 */
                                "exclam\x00"
                                "\x0c" /* 'exclam' 12 */
                                "¶\x00"
                                "\x04" /* '¶' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "₧\x00"
                                "\x05" /* '₧' 5 */
                                "P\x00"
                                "\x07" /* 'P' 7 */
                                "¶\x00"
                                "\x04" /* '¶' 4 */
                                "question\x00"
                                "\x82\x3a" /* 'question' 570 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ả\x00"
                                "\x05" /* 'ả' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "exclam\x00"
                                "\x0d" /* 'exclam' 13 */
                                "⸘\x00"
                                "\x05" /* '⸘' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẻ\x00"
                                "\x05" /* 'ẻ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỏ\x00"
                                "\x05" /* 'ỏ' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỷ\x00"
                                "\x05" /* 'ỷ' 5 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ỉ\x00"
                                "\x05" /* 'ỉ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ủ\x00"
                                "\x05" /* 'ủ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẻ\x00"
                                "\x05" /* 'Ẻ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỷ\x00"
                                "\x05" /* 'Ỷ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "question\x00"
                                "\x0e" /* 'question' 14 */
                                "¿\x00"
                                "\x04" /* '¿' 4 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỏ\x00"
                                "\x05" /* 'Ỏ' 5 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ả\x00"
                                "\x05" /* 'Ả' 5 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ỉ\x00"
                                "\x05" /* 'Ỉ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ủ\x00"
                                "\x05" /* 'Ủ' 5 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "apostrophe\x00"
                                "\x8c\x61" /* 'apostrophe' 3169 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẃ\x00"
                                "\x05" /* 'Ẃ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǵ\x00"
                                "\x04" /* 'ǵ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "á\x00"
                                "\x04" /* 'á' 4 */
                                "Greek_IOTA\x00"
                                "\x10" /* 'Greek_IOTA' 16 */
                                "Ί\x00"
                                "\x04" /* 'Ί' 4 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ί\x00"
                                "\x04" /* 'ί' 4 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Greek_OMICRON\x00"
                                "\x13" /* 'Greek_OMICRON' 19 */
                                "Ό\x00"
                                "\x04" /* 'Ό' 4 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ć\x00"
                                "\x04" /* 'Ć' 4 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "‘\x00"
                                "\x05" /* '‘' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р́\x00"
                                "\x06" /* 'р́' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "é\x00"
                                "\x04" /* 'é' 4 */
                                "KP_Divide\x00"
                                "\x19" /* 'KP_Divide' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "Utilde\x00"
                                "\x0d" /* 'Utilde' 13 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ó\x00"
                                "\x04" /* 'ó' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ĺ\x00"
                                "\x04" /* 'ĺ' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ύ\x00"
                                "\x04" /* 'ύ' 4 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "'\x00"
                                "\x03" /* ''' 3 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И́\x00"
                                "\x06" /* 'И́' 6 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ý\x00"
                                "\x04" /* 'ý' 4 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "idiaeresis\x00"
                                "\x11" /* 'idiaeresis' 17 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О́\x00"
                                "\x06" /* 'О́' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "í\x00"
                                "\x04" /* 'í' 4 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḱ\x00"
                                "\x05" /* 'ḱ' 5 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ń\x00"
                                "\x04" /* 'ń' 4 */
                                "ccedilla\x00"
                                "\x0f" /* 'ccedilla' 15 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Cyrillic_GHE\x00"
                                "\x12" /* 'Cyrillic_GHE' 18 */
                                "Ѓ\x00"
                                "\x04" /* 'Ѓ' 4 */
                                "dead_tilde\x00"
                                "\x2c" /* 'dead_tilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а́\x00"
                                "\x06" /* 'а́' 6 */
                                "parenright\x00"
                                "\x80\xfb" /* 'parenright' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р́\x00"
                                "\x06" /* 'Р́' 6 */
                                "Greek_epsilon\x00"
                                "\x13" /* 'Greek_epsilon' 19 */
                                "έ\x00"
                                "\x04" /* 'έ' 4 */
                                "Cyrillic_KA\x00"
                                "\x11" /* 'Cyrillic_KA' 17 */
                                "Ќ\x00"
                                "\x04" /* 'Ќ' 4 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У́\x00"
                                "\x06" /* 'У́' 6 */
                                "dead_abovering\x00"
                                "\x1e" /* 'dead_abovering' 30 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǽ\x00"
                                "\x04" /* 'Ǽ' 4 */
                                "omacron\x00"
                                "\x0e" /* 'omacron' 14 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ú\x00"
                                "\x04" /* 'ú' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ź\x00"
                                "\x04" /* 'ź' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǵ\x00"
                                "\x04" /* 'Ǵ' 4 */
                                "Greek_ALPHA\x00"
                                "\x11" /* 'Greek_ALPHA' 17 */
                                "Ά\x00"
                                "\x04" /* 'Ά' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "utilde\x00"
                                "\x0d" /* 'utilde' 13 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е́\x00"
                                "\x06" /* 'е́' 6 */
                                "emacron\x00"
                                "\x0e" /* 'emacron' 14 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "É\x00"
                                "\x04" /* 'É' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ś\x00"
                                "\x04" /* 'Ś' 4 */
                                "Greek_iotadieresis\x00"
                                "\x18" /* 'Greek_iotadieresis' 24 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ý\x00"
                                "\x04" /* 'Ý' 4 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и́\x00"
                                "\x06" /* 'и́' 6 */
                                "dead_dasia\x00"
                                "\x81\x0f" /* 'dead_dasia' 271 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1b" /* 'Greek_upsilondieresis' 27 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "Greek_omicron\x00"
                                "\x13" /* 'Greek_omicron' 19 */
                                "ό\x00"
                                "\x04" /* 'ό' 4 */
                                "Greek_eta\x00"
                                "\x0f" /* 'Greek_eta' 15 */
                                "ή\x00"
                                "\x04" /* 'ή' 4 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "Cyrillic_ka\x00"
                                "\x11" /* 'Cyrillic_ka' 17 */
                                "ќ\x00"
                                "\x04" /* 'ќ' 4 */
                                "Aring\x00"
                                "\x0b" /* 'Aring' 11 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "dead_psili\x00"
                                "\x80\xfb" /* 'dead_psili' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "quotedbl\x00"
                                "\x56" /* 'quotedbl' 86 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Greek_alpha\x00"
                                "\x11" /* 'Greek_alpha' 17 */
                                "ά\x00"
                                "\x04" /* 'ά' 4 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẃ\x00"
                                "\x05" /* 'ẃ' 5 */
                                "Greek_ETA\x00"
                                "\x0f" /* 'Greek_ETA' 15 */
                                "Ή\x00"
                                "\x04" /* 'Ή' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о́\x00"
                                "\x06" /* 'о́' 6 */
                                "Emacron\x00"
                                "\x0e" /* 'Emacron' 14 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "Ooblique\x00"
                                "\x0e" /* 'Ooblique' 14 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ṕ\x00"
                                "\x05" /* 'ṕ' 5 */
                                "underscore\x00"
                                "\x2c" /* 'underscore' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ṕ\x00"
                                "\x05" /* 'Ṕ' 5 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "´\x00"
                                "\x04" /* '´' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ḿ\x00"
                                "\x05" /* 'Ḿ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ó\x00"
                                "\x04" /* 'Ó' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ḿ\x00"
                                "\x05" /* 'ḿ' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŕ\x00"
                                "\x04" /* 'ŕ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ś\x00"
                                "\x04" /* 'ś' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ź\x00"
                                "\x04" /* 'Ź' 4 */
                                "macron\x00"
                                "\x28" /* 'macron' 40 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Á\x00"
                                "\x04" /* 'Á' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŕ\x00"
                                "\x04" /* 'Ŕ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ć\x00"
                                "\x04" /* 'ć' 4 */
                                "Idiaeresis\x00"
                                "\x11" /* 'Idiaeresis' 17 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ĺ\x00"
                                "\x04" /* 'Ĺ' 4 */
                                "Greek_EPSILON\x00"
                                "\x13" /* 'Greek_EPSILON' 19 */
                                "Έ\x00"
                                "\x04" /* 'Έ' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А́\x00"
                                "\x06" /* 'А́' 6 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "‚\x00"
                                "\x05" /* '‚' 5 */
                                "asciitilde\x00"
                                "\x2c" /* 'asciitilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Ccedilla\x00"
                                "\x0f" /* 'Ccedilla' 15 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "slash\x00"
                                "\x15" /* 'slash' 21 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "aring\x00"
                                "\x0b" /* 'aring' 11 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "’\x00"
                                "\x05" /* '’' 5 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḱ\x00"
                                "\x05" /* 'Ḱ' 5 */
                                "Omacron\x00"
                                "\x0e" /* 'Omacron' 14 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е́\x00"
                                "\x06" /* 'Е́' 6 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Greek_omega\x00"
                                "\x11" /* 'Greek_omega' 17 */
                                "ώ\x00"
                                "\x04" /* 'ώ' 4 */
                                "dead_diaeresis\x00"
                                "\x51" /* 'dead_diaeresis' 81 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "Greek_OMEGA\x00"
                                "\x11" /* 'Greek_OMEGA' 17 */
                                "Ώ\x00"
                                "\x04" /* 'Ώ' 4 */
                                "oslash\x00"
                                "\x0c" /* 'oslash' 12 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "Cyrillic_ghe\x00"
                                "\x12" /* 'Cyrillic_ghe' 18 */
                                "ѓ\x00"
                                "\x04" /* 'ѓ' 4 */
                                "parenleft\x00"
                                "\x81\x0e" /* 'parenleft' 270 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Í\x00"
                                "\x04" /* 'Í' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ń\x00"
                                "\x04" /* 'Ń' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ú\x00"
                                "\x04" /* 'Ú' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у́\x00"
                                "\x06" /* 'у́' 6 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǽ\x00"
                                "\x04" /* 'ǽ' 4 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Greek_UPSILON\x00"
                                "\x13" /* 'Greek_UPSILON' 19 */
                                "Ύ\x00"
                                "\x04" /* 'Ύ' 4 */
                                "M\x00"
                                "\x10" /* 'M' 16 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ṁ\x00"
                                "\x05" /* 'Ṁ' 5 */
                                "O\x00"
                                "\x80\xdb" /* 'O' 219 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ö\x00"
                                "\x04" /* 'Ö' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Œ\x00"
                                "\x04" /* 'Œ' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ö\x00"
                                "\x04" /* 'Ö' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "Ó\x00"
                                "\x04" /* 'Ó' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ó\x00"
                                "\x04" /* 'Ó' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "®\x00"
                                "\x04" /* '®' 4 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ⓐ\x00"
                                "\x05" /* 'Ⓐ' 5 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "®\x00"
                                "\x04" /* '®' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "©\x00"
                                "\x04" /* '©' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "Õ\x00"
                                "\x04" /* 'Õ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "Ø\x00"
                                "\x04" /* 'Ø' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "Ô\x00"
                                "\x04" /* 'Ô' 4 */
                                "X\x00"
                                "\x07" /* 'X' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "Ò\x00"
                                "\x04" /* 'Ò' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Ô\x00"
                                "\x04" /* 'Ô' 4 */
                                "m\x00"
                                "\x23" /* 'm' 35 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ṁ\x00"
                                "\x05" /* 'ṁ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "slash\x00"
                                "\x0c" /* 'slash' 12 */
                                "₥\x00"
                                "\x05" /* '₥' 5 */
                                "r\x00"
                                "\x28" /* 'r' 40 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "ř\x00"
                                "\x04" /* 'ř' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ŕ\x00"
                                "\x04" /* 'ŕ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ŗ\x00"
                                "\x04" /* 'ŗ' 4 */
                                "s\x00"
                                "\x6c" /* 's' 108 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "ṡ\x00"
                                "\x05" /* 'ṡ' 5 */
                                "exclam\x00"
                                "\x0c" /* 'exclam' 12 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "š\x00"
                                "\x04" /* 'š' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "§\x00"
                                "\x04" /* '§' 4 */
                                "cedilla\x00"
                                "\x0d" /* 'cedilla' 13 */
                                "ş\x00"
                                "\x04" /* 'ş' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ś\x00"
                                "\x04" /* 'ś' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "℠\x00"
                                "\x05" /* '℠' 5 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ß\x00"
                                "\x04" /* 'ß' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ş\x00"
                                "\x04" /* 'ş' 4 */
                                "asterisk\x00"
                                "\x57" /* 'asterisk' 87 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "å\x00"
                                "\x04" /* 'å' 4 */
                                "diaeresis\x00"
                                "\x10" /* 'diaeresis' 16 */
                                "⍣\x00"
                                "\x05" /* '⍣' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ů\x00"
                                "\x04" /* 'ů' 4 */
                                "apostrophe\x00"
                                "\x1a" /* 'apostrophe' 26 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Å\x00"
                                "\x04" /* 'Å' 4 */
                                "0\x00"
                                "\x07" /* '0' 7 */
                                "°\x00"
                                "\x04" /* '°' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ů\x00"
                                "\x04" /* 'Ů' 4 */
                                "Z\x00"
                                "\x29" /* 'Z' 41 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "Ż\x00"
                                "\x04" /* 'Ż' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ž\x00"
                                "\x04" /* 'Ž' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ź\x00"
                                "\x04" /* 'Ź' 4 */
                                "bar\x00"
                                "\x24" /* 'bar' 36 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "asciitilde\x00"
                                "\x11" /* 'asciitilde' 17 */
                                "⍭\x00"
                                "\x05" /* '⍭' 5 */
                                "macron\x00"
                                "\x83\xb3" /* 'macron' 947 */
                                "adiaeresis\x00"
                                "\x10" /* 'adiaeresis' 16 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "period\x00"
                                "\x24" /* 'period' 36 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "g\x00"
                                "\x08" /* 'g' 8 */
                                "ḡ\x00"
                                "\x05" /* 'ḡ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ā\x00"
                                "\x04" /* 'ā' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῑ\x00"
                                "\x05" /* 'Ῑ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῑ\x00"
                                "\x05" /* 'ῑ' 5 */
                                "exclam\x00"
                                "\x28" /* 'exclam' 40 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̄\x00"
                                "\x06" /* 'р̄' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ē\x00"
                                "\x04" /* 'ē' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ō\x00"
                                "\x04" /* 'ō' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῡ\x00"
                                "\x05" /* 'ῡ' 5 */
                                "dead_belowdot\x00"
                                "\x2f" /* 'dead_belowdot' 47 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḹ\x00"
                                "\x05" /* 'ḹ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṝ\x00"
                                "\x05" /* 'ṝ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṝ\x00"
                                "\x05" /* 'Ṝ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḹ\x00"
                                "\x05" /* 'Ḹ' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ӣ\x00"
                                "\x04" /* 'Ӣ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ȳ\x00"
                                "\x04" /* 'ȳ' 4 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̄\x00"
                                "\x06" /* 'О̄' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ī\x00"
                                "\x04" /* 'ī' 4 */
                                "dead_tilde\x00"
                                "\x1a" /* 'dead_tilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̄\x00"
                                "\x06" /* 'а̄' 6 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̄\x00"
                                "\x06" /* 'Р̄' 6 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӯ\x00"
                                "\x04" /* 'Ӯ' 4 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǣ\x00"
                                "\x04" /* 'Ǣ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ū\x00"
                                "\x04" /* 'ū' 4 */
                                "G\x00"
                                "\x08" /* 'G' 8 */
                                "Ḡ\x00"
                                "\x05" /* 'Ḡ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾱ\x00"
                                "\x05" /* 'Ᾱ' 5 */
                                "otilde\x00"
                                "\x0c" /* 'otilde' 12 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̄\x00"
                                "\x06" /* 'е̄' 6 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ē\x00"
                                "\x04" /* 'Ē' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ȳ\x00"
                                "\x04" /* 'Ȳ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ӣ\x00"
                                "\x04" /* 'ӣ' 4 */
                                "dead_ogonek\x00"
                                "\x1b" /* 'dead_ogonek' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "odiaeresis\x00"
                                "\x10" /* 'odiaeresis' 16 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "Otilde\x00"
                                "\x0c" /* 'Otilde' 12 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "quotedbl\x00"
                                "\x34" /* 'quotedbl' 52 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾱ\x00"
                                "\x05" /* 'ᾱ' 5 */
                                "dead_abovedot\x00"
                                "\x2b" /* 'dead_abovedot' 43 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̄\x00"
                                "\x06" /* 'о̄' 6 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ō\x00"
                                "\x04" /* 'Ō' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "Odiaeresis\x00"
                                "\x10" /* 'Odiaeresis' 16 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̄\x00"
                                "\x06" /* 'А̄' 6 */
                                "asciitilde\x00"
                                "\x1a" /* 'asciitilde' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "semicolon\x00"
                                "\x19" /* 'semicolon' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̄\x00"
                                "\x06" /* 'Е̄' 6 */
                                "dead_diaeresis\x00"
                                "\x3a" /* 'dead_diaeresis' 58 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǖ\x00"
                                "\x04" /* 'Ǖ' 4 */
                                "Adiaeresis\x00"
                                "\x10" /* 'Adiaeresis' 16 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǖ\x00"
                                "\x04" /* 'ǖ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӯ\x00"
                                "\x04" /* 'ӯ' 4 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǣ\x00"
                                "\x04" /* 'ǣ' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῡ\x00"
                                "\x05" /* 'Ῡ' 5 */
                                "A\x00"
                                "\x80\xcc" /* 'A' 204 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ä\x00"
                                "\x04" /* 'Ä' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Æ\x00"
                                "\x04" /* 'Æ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ä\x00"
                                "\x04" /* 'Ä' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "Á\x00"
                                "\x04" /* 'Á' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "Ā\x00"
                                "\x04" /* 'Ā' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Á\x00"
                                "\x04" /* 'Á' 4 */
                                "asterisk\x00"
                                "\x0e" /* 'asterisk' 14 */
                                "Å\x00"
                                "\x04" /* 'Å' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Å\x00"
                                "\x04" /* 'Å' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ą\x00"
                                "\x04" /* 'Ą' 4 */
                                "T\x00"
                                "\x06" /* 'T' 6 */
                                "@\x00"
                                "\x03" /* '@' 3 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "Ã\x00"
                                "\x04" /* 'Ã' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "Â\x00"
                                "\x04" /* 'Â' 4 */
                                "parenleft\x00"
                                "\x0f" /* 'parenleft' 15 */
                                "Ă\x00"
                                "\x04" /* 'Ă' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "À\x00"
                                "\x04" /* 'À' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Â\x00"
                                "\x04" /* 'Â' 4 */
                                "R\x00"
                                "\x37" /* 'R' 55 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ř\x00"
                                "\x04" /* 'Ř' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ŕ\x00"
                                "\x04" /* 'Ŕ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "®\x00"
                                "\x04" /* '®' 4 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "₨\x00"
                                "\x05" /* '₨' 5 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ŗ\x00"
                                "\x04" /* 'Ŗ' 4 */
                                "Cyrillic_ES\x00"
                                "\x19" /* 'Cyrillic_ES' 25 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "c\x00"
                                "\x81\xa4" /* 'c' 420 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "ċ\x00"
                                "\x04" /* 'ċ' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǧ\x00"
                                "\x04" /* 'ǧ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǎ\x00"
                                "\x04" /* 'ǎ' 4 */
                                "ezh\x00"
                                "\x09" /* 'ezh' 9 */
                                "ǯ\x00"
                                "\x04" /* 'ǯ' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Č\x00"
                                "\x04" /* 'Č' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "č\x00"
                                "\x04" /* 'č' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ě\x00"
                                "\x04" /* 'ě' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǒ\x00"
                                "\x04" /* 'ǒ' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ľ\x00"
                                "\x04" /* 'ľ' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ť\x00"
                                "\x04" /* 'ť' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ǐ\x00"
                                "\x04" /* 'ǐ' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ǩ\x00"
                                "\x04" /* 'ǩ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ň\x00"
                                "\x04" /* 'ň' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ǰ\x00"
                                "\x04" /* 'ǰ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǔ\x00"
                                "\x04" /* 'ǔ' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ž\x00"
                                "\x04" /* 'ž' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǧ\x00"
                                "\x04" /* 'Ǧ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ȟ\x00"
                                "\x04" /* 'Ȟ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ě\x00"
                                "\x04" /* 'Ě' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Š\x00"
                                "\x04" /* 'Š' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "ď\x00"
                                "\x04" /* 'ď' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Ď\x00"
                                "\x04" /* 'Ď' 4 */
                                "quotedbl\x00"
                                "\x18" /* 'quotedbl' 24 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ć\x00"
                                "\x04" /* 'ć' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǒ\x00"
                                "\x04" /* 'Ǒ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ř\x00"
                                "\x04" /* 'ř' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "š\x00"
                                "\x04" /* 'š' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ž\x00"
                                "\x04" /* 'Ž' 4 */
                                "bar\x00"
                                "\x09" /* 'bar' 9 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "EZH\x00"
                                "\x09" /* 'EZH' 9 */
                                "Ǯ\x00"
                                "\x04" /* 'Ǯ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǎ\x00"
                                "\x04" /* 'Ǎ' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ř\x00"
                                "\x04" /* 'Ř' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "č\x00"
                                "\x04" /* 'č' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ľ\x00"
                                "\x04" /* 'Ľ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "ç\x00"
                                "\x04" /* 'ç' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ť\x00"
                                "\x04" /* 'Ť' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ǩ\x00"
                                "\x04" /* 'Ǩ' 4 */
                                "dead_diaeresis\x00"
                                "\x1e" /* 'dead_diaeresis' 30 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ȟ\x00"
                                "\x04" /* 'ȟ' 4 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ǐ\x00"
                                "\x04" /* 'Ǐ' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ň\x00"
                                "\x04" /* 'Ň' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǔ\x00"
                                "\x04" /* 'Ǔ' 4 */
                                "numbersign\x00"
                                "\x4d" /* 'numbersign' 77 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "♪\x00"
                                "\x05" /* '♪' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "♭\x00"
                                "\x05" /* '♭' 5 */
                                "q\x00"
                                "\x08" /* 'q' 8 */
                                "♩\x00"
                                "\x05" /* '♩' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "♫\x00"
                                "\x05" /* '♫' 5 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "♬\x00"
                                "\x05" /* '♬' 5 */
                                "f\x00"
                                "\x08" /* 'f' 8 */
                                "♮\x00"
                                "\x05" /* '♮' 5 */
                                "numbersign\x00"
                                "\x11" /* 'numbersign' 17 */
                                "♯\x00"
                                "\x05" /* '♯' 5 */
                                "L\x00"
                                "\x50" /* 'L' 80 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "£\x00"
                                "\x04" /* '£' 4 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ľ\x00"
                                "\x04" /* 'Ľ' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₤\x00"
                                "\x05" /* '₤' 5 */
                                "V\x00"
                                "\x06" /* 'V' 6 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ĺ\x00"
                                "\x04" /* 'Ĺ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ļ\x00"
                                "\x04" /* 'Ļ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "Ł\x00"
                                "\x04" /* 'Ł' 4 */
                                "Greek_EPSILON\x00"
                                "\x1f" /* 'Greek_EPSILON' 31 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Έ\x00"
                                "\x04" /* 'Έ' 4 */
                                "comma\x00"
                                "\x81\x11" /* 'comma' 273 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "¬\x00"
                                "\x04" /* '¬' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ģ\x00"
                                "\x04" /* 'ģ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ą\x00"
                                "\x04" /* 'ą' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ç\x00"
                                "\x04" /* 'Ç' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ę\x00"
                                "\x04" /* 'ę' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ļ\x00"
                                "\x04" /* 'ļ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ţ\x00"
                                "\x04" /* 'ţ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "¸\x00"
                                "\x04" /* '¸' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "į\x00"
                                "\x04" /* 'į' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ķ\x00"
                                "\x04" /* 'ķ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ņ\x00"
                                "\x04" /* 'ņ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ų\x00"
                                "\x04" /* 'ų' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ģ\x00"
                                "\x04" /* 'Ģ' 4 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḩ\x00"
                                "\x05" /* 'Ḩ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ę\x00"
                                "\x04" /* 'Ę' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ş\x00"
                                "\x04" /* 'Ş' 4 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḑ\x00"
                                "\x05" /* 'ḑ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḑ\x00"
                                "\x05" /* 'Ḑ' 5 */
                                "quotedbl\x00"
                                "\x0f" /* 'quotedbl' 15 */
                                "„\x00"
                                "\x05" /* '„' 5 */
                                "apostrophe\x00"
                                "\x11" /* 'apostrophe' 17 */
                                "‚\x00"
                                "\x05" /* '‚' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŗ\x00"
                                "\x04" /* 'ŗ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ş\x00"
                                "\x04" /* 'ş' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ą\x00"
                                "\x04" /* 'Ą' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŗ\x00"
                                "\x04" /* 'Ŗ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ç\x00"
                                "\x04" /* 'ç' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ļ\x00"
                                "\x04" /* 'Ļ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "¸\x00"
                                "\x04" /* '¸' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ţ\x00"
                                "\x04" /* 'Ţ' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ķ\x00"
                                "\x04" /* 'Ķ' 4 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḩ\x00"
                                "\x05" /* 'ḩ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Į\x00"
                                "\x04" /* 'Į' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ņ\x00"
                                "\x04" /* 'Ņ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ų\x00"
                                "\x04" /* 'Ų' 4 */
                                "T\x00"
                                "\x52" /* 'T' 82 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ŧ\x00"
                                "\x04" /* 'Ŧ' 4 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ṫ\x00"
                                "\x05" /* 'Ṫ' 5 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ť\x00"
                                "\x04" /* 'Ť' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Þ\x00"
                                "\x04" /* 'Þ' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "™\x00"
                                "\x05" /* '™' 5 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ţ\x00"
                                "\x04" /* 'Ţ' 4 */
                                "slash\x00"
                                "\x0b" /* 'slash' 11 */
                                "Ŧ\x00"
                                "\x04" /* 'Ŧ' 4 */
                                "asciitilde\x00"
                                "\x86\x28" /* 'asciitilde' 1576 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ã\x00"
                                "\x04" /* 'ã' 4 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῖ\x00"
                                "\x05" /* 'ῖ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẽ\x00"
                                "\x05" /* 'ẽ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "õ\x00"
                                "\x04" /* 'õ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῦ\x00"
                                "\x05" /* 'ῦ' 5 */
                                "diaeresis\x00"
                                "\x10" /* 'diaeresis' 16 */
                                "⍨\x00"
                                "\x05" /* '⍨' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỹ\x00"
                                "\x05" /* 'ỹ' 5 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ĩ\x00"
                                "\x04" /* 'ĩ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ñ\x00"
                                "\x04" /* 'ñ' 4 */
                                "parenright\x00"
                                "\x80\xab" /* 'parenright' 171 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἶ\x00"
                                "\x05" /* 'Ἶ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἶ\x00"
                                "\x05" /* 'ἶ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὖ\x00"
                                "\x05" /* 'ὖ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἆ\x00"
                                "\x05" /* 'Ἆ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἦ\x00"
                                "\x05" /* 'ἦ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἆ\x00"
                                "\x05" /* 'ἆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἦ\x00"
                                "\x05" /* 'Ἦ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὦ\x00"
                                "\x05" /* 'ὦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὦ\x00"
                                "\x05" /* 'Ὦ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "V\x00"
                                "\x08" /* 'V' 8 */
                                "Ṽ\x00"
                                "\x05" /* 'Ṽ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ũ\x00"
                                "\x04" /* 'ũ' 4 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẽ\x00"
                                "\x05" /* 'Ẽ' 5 */
                                "Greek_iotadieresis\x00"
                                "\x19" /* 'Greek_iotadieresis' 25 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỹ\x00"
                                "\x05" /* 'Ỹ' 5 */
                                "dead_dasia\x00"
                                "\x80\xbf" /* 'dead_dasia' 191 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἷ\x00"
                                "\x05" /* 'Ἷ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἷ\x00"
                                "\x05" /* 'ἷ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὗ\x00"
                                "\x05" /* 'ὗ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἇ\x00"
                                "\x05" /* 'Ἇ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἧ\x00"
                                "\x05" /* 'ἧ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἇ\x00"
                                "\x05" /* 'ἇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἧ\x00"
                                "\x05" /* 'Ἧ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὧ\x00"
                                "\x05" /* 'ὧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὧ\x00"
                                "\x05" /* 'Ὧ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὗ\x00"
                                "\x05" /* 'Ὗ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1c" /* 'Greek_upsilondieresis' 28 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῆ\x00"
                                "\x05" /* 'ῆ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "dead_psili\x00"
                                "\x80\xab" /* 'dead_psili' 171 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἶ\x00"
                                "\x05" /* 'Ἶ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἶ\x00"
                                "\x05" /* 'ἶ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὖ\x00"
                                "\x05" /* 'ὖ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἆ\x00"
                                "\x05" /* 'Ἆ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἦ\x00"
                                "\x05" /* 'ἦ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἆ\x00"
                                "\x05" /* 'ἆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἦ\x00"
                                "\x05" /* 'Ἦ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὦ\x00"
                                "\x05" /* 'ὦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὦ\x00"
                                "\x05" /* 'Ὦ' 5 */
                                "quotedbl\x00"
                                "\x2f" /* 'quotedbl' 47 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾶ\x00"
                                "\x05" /* 'ᾶ' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "v\x00"
                                "\x08" /* 'v' 8 */
                                "ṽ\x00"
                                "\x05" /* 'ṽ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Õ\x00"
                                "\x04" /* 'Õ' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "bar\x00"
                                "\x0a" /* 'bar' 10 */
                                "⍭\x00"
                                "\x05" /* '⍭' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ã\x00"
                                "\x04" /* 'Ã' 4 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "⍬\x00"
                                "\x05" /* '⍬' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῶ\x00"
                                "\x05" /* 'ῶ' 5 */
                                "dead_diaeresis\x00"
                                "\x35" /* 'dead_diaeresis' 53 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "parenleft\x00"
                                "\x80\xbe" /* 'parenleft' 190 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἷ\x00"
                                "\x05" /* 'Ἷ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἷ\x00"
                                "\x05" /* 'ἷ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὗ\x00"
                                "\x05" /* 'ὗ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἇ\x00"
                                "\x05" /* 'Ἇ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἧ\x00"
                                "\x05" /* 'ἧ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἇ\x00"
                                "\x05" /* 'ἇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἧ\x00"
                                "\x05" /* 'Ἧ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὧ\x00"
                                "\x05" /* 'ὧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὧ\x00"
                                "\x05" /* 'Ὧ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὗ\x00"
                                "\x05" /* 'Ὗ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ĩ\x00"
                                "\x04" /* 'Ĩ' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ñ\x00"
                                "\x04" /* 'Ñ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ũ\x00"
                                "\x04" /* 'Ũ' 4 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "slash\x00"
                                "\x81\x47" /* 'slash' 327 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "⌿\x00"
                                "\x05" /* '⌿' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǥ\x00"
                                "\x04" /* 'ǥ' 4 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "₡\x00"
                                "\x05" /* '₡' 5 */
                                "less\x00"
                                "\x09" /* 'less' 9 */
                                "\\\x00"
                                "\x03" /* '\' 3 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ø\x00"
                                "\x04" /* 'ø' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ł\x00"
                                "\x04" /* 'ł' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ŧ\x00"
                                "\x04" /* 'ŧ' 4 */
                                "b\x00"
                                "\x07" /* 'b' 7 */
                                "ƀ\x00"
                                "\x04" /* 'ƀ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ɨ\x00"
                                "\x04" /* 'ɨ' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "≠\x00"
                                "\x05" /* '≠' 5 */
                                "Cyrillic_GHE\x00"
                                "\x12" /* 'Cyrillic_GHE' 18 */
                                "Ғ\x00"
                                "\x04" /* 'Ғ' 4 */
                                "leftarrow\x00"
                                "\x10" /* 'leftarrow' 16 */
                                "↚\x00"
                                "\x05" /* '↚' 5 */
                                "Cyrillic_KA\x00"
                                "\x11" /* 'Cyrillic_KA' 17 */
                                "Ҟ\x00"
                                "\x04" /* 'Ҟ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "rightarrow\x00"
                                "\x11" /* 'rightarrow' 17 */
                                "↛\x00"
                                "\x05" /* '↛' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ƶ\x00"
                                "\x04" /* 'ƶ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǥ\x00"
                                "\x04" /* 'Ǥ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ħ\x00"
                                "\x04" /* 'Ħ' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "đ\x00"
                                "\x04" /* 'đ' 4 */
                                "Cyrillic_ka\x00"
                                "\x11" /* 'Cyrillic_ka' 17 */
                                "ҟ\x00"
                                "\x04" /* 'ҟ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Đ\x00"
                                "\x04" /* 'Đ' 4 */
                                "v\x00"
                                "\x08" /* 'v' 8 */
                                "√\x00"
                                "\x05" /* '√' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ø\x00"
                                "\x04" /* 'Ø' 4 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "₥\x00"
                                "\x05" /* '₥' 5 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ƶ\x00"
                                "\x04" /* 'Ƶ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "¢\x00"
                                "\x04" /* '¢' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ł\x00"
                                "\x04" /* 'Ł' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ŧ\x00"
                                "\x04" /* 'Ŧ' 4 */
                                "slash\x00"
                                "\x0a" /* 'slash' 10 */
                                "\\\x00"
                                "\x03" /* '\' 3 */
                                "Cyrillic_ghe\x00"
                                "\x12" /* 'Cyrillic_ghe' 18 */
                                "ғ\x00"
                                "\x04" /* 'ғ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ħ\x00"
                                "\x04" /* 'ħ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ɨ\x00"
                                "\x04" /* 'Ɨ' 4 */
                                "asciicircum\x00"
                                "\x10" /* 'asciicircum' 16 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "5\x00"
                                "\x13" /* '5' 19 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⅝\x00"
                                "\x05" /* '⅝' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "⅚\x00"
                                "\x05" /* '⅚' 5 */
                                "Cyrillic_EN\x00"
                                "\x2f" /* 'Cyrillic_EN' 47 */
                                "Cyrillic_O\x00"
                                "\x11" /* 'Cyrillic_O' 17 */
                                "№\x00"
                                "\x05" /* '№' 5 */
                                "Cyrillic_o\x00"
                                "\x11" /* 'Cyrillic_o' 17 */
                                "№\x00"
                                "\x05" /* '№' 5 */
                                "greater\x00"
                                "\x80\xbf" /* 'greater' 191 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "â\x00"
                                "\x04" /* 'â' 4 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "⋄\x00"
                                "\x05" /* '⋄' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ê\x00"
                                "\x04" /* 'ê' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ô\x00"
                                "\x04" /* 'ô' 4 */
                                "diaeresis\x00"
                                "\x10" /* 'diaeresis' 16 */
                                "⍩\x00"
                                "\x05" /* '⍩' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "î\x00"
                                "\x04" /* 'î' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "≥\x00"
                                "\x05" /* '≥' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "û\x00"
                                "\x04" /* 'û' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ê\x00"
                                "\x04" /* 'Ê' 4 */
                                "quotedbl\x00"
                                "\x0f" /* 'quotedbl' 15 */
                                "”\x00"
                                "\x05" /* '”' 5 */
                                "underscore\x00"
                                "\x11" /* 'underscore' 17 */
                                "≥\x00"
                                "\x05" /* '≥' 5 */
                                "apostrophe\x00"
                                "\x11" /* 'apostrophe' 17 */
                                "’\x00"
                                "\x05" /* '’' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ô\x00"
                                "\x04" /* 'Ô' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Â\x00"
                                "\x04" /* 'Â' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "»\x00"
                                "\x04" /* '»' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Î\x00"
                                "\x04" /* 'Î' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Û\x00"
                                "\x04" /* 'Û' 4 */
                                "semicolon\x00"
                                "\x62" /* 'semicolon' 98 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ą\x00"
                                "\x04" /* 'ą' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ę\x00"
                                "\x04" /* 'ę' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǫ\x00"
                                "\x04" /* 'ǫ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "į\x00"
                                "\x04" /* 'į' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ų\x00"
                                "\x04" /* 'ų' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ę\x00"
                                "\x04" /* 'Ę' 4 */
                                "underscore\x00"
                                "\x11" /* 'underscore' 17 */
                                "⍮\x00"
                                "\x05" /* '⍮' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǫ\x00"
                                "\x04" /* 'Ǫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ą\x00"
                                "\x04" /* 'Ą' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Į\x00"
                                "\x04" /* 'Į' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ų\x00"
                                "\x04" /* 'Ų' 4 */
                                "K\x00"
                                "\x0e" /* 'K' 14 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ķ\x00"
                                "\x04" /* 'Ķ' 4 */
                                "Cyrillic_IE\x00"
                                "\x19" /* 'Cyrillic_IE' 25 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "€\x00"
                                "\x05" /* '€' 5 */
                                "B\x00"
                                "\x10" /* 'B' 16 */
                                "period\x00"
                                "\x0d" /* 'period' 13 */
                                "Ḃ\x00"
                                "\x05" /* 'Ḃ' 5 */
                                "0\x00"
                                "\x2a" /* '0' 42 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "↉\x00"
                                "\x05" /* '↉' 5 */
                                "asterisk\x00"
                                "\x0e" /* 'asterisk' 14 */
                                "°\x00"
                                "\x04" /* '°' 4 */
                                "asciitilde\x00"
                                "\x11" /* 'asciitilde' 17 */
                                "⍬\x00"
                                "\x05" /* '⍬' 5 */
                                "Greek_omega\x00"
                                "\x1d" /* 'Greek_omega' 29 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "ώ\x00"
                                "\x04" /* 'ώ' 4 */
                                "Greek_OMEGA\x00"
                                "\x1d" /* 'Greek_OMEGA' 29 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ώ\x00"
                                "\x04" /* 'Ώ' 4 */
                                "X\x00"
                                "\x11" /* 'X' 17 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "¤\x00"
                                "\x04" /* '¤' 4 */
                                "parenleft\x00"
                                "\x9c\x2a" /* 'parenleft' 7210 */
                                "minus\x00"
                                "\x0a" /* 'minus' 10 */
                                "{\x00"
                                "\x03" /* '{' 3 */
                                "W\x00"
                                "\x14" /* 'W' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓦ\x00"
                                "\x05" /* 'Ⓦ' 5 */
                                "g\x00"
                                "\x14" /* 'g' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓖ\x00"
                                "\x05" /* 'ⓖ' 5 */
                                "kana_KE\x00"
                                "\x1a" /* 'kana_KE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋘\x00"
                                "\x05" /* '㋘' 5 */
                                "a\x00"
                                "\x14" /* 'a' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓐ\x00"
                                "\x05" /* 'ⓐ' 5 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἱ\x00"
                                "\x05" /* 'Ἱ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἱ\x00"
                                "\x05" /* 'ἱ' 5 */
                                "1\x00"
                                "\x81\xde" /* '1' 478 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑪\x00"
                                "\x05" /* '⑪' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑭\x00"
                                "\x05" /* '⑭' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑯\x00"
                                "\x05" /* '⑯' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑱\x00"
                                "\x05" /* '⑱' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑲\x00"
                                "\x05" /* '⑲' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑰\x00"
                                "\x05" /* '⑰' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "①\x00"
                                "\x05" /* '①' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑰\x00"
                                "\x05" /* '⑰' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑱\x00"
                                "\x05" /* '⑱' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑪\x00"
                                "\x05" /* '⑪' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑬\x00"
                                "\x05" /* '⑬' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑯\x00"
                                "\x05" /* '⑯' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑭\x00"
                                "\x05" /* '⑭' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑬\x00"
                                "\x05" /* '⑬' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑩\x00"
                                "\x05" /* '⑩' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑮\x00"
                                "\x05" /* '⑮' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑮\x00"
                                "\x05" /* '⑮' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑲\x00"
                                "\x05" /* '⑲' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑩\x00"
                                "\x05" /* '⑩' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὁ\x00"
                                "\x05" /* 'Ὁ' 5 */
                                "C\x00"
                                "\x14" /* 'C' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓒ\x00"
                                "\x05" /* 'Ⓒ' 5 */
                                "KP_4\x00"
                                "\x81\xe1" /* 'KP_4' 481 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊶\x00"
                                "\x05" /* '㊶' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊹\x00"
                                "\x05" /* '㊹' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊻\x00"
                                "\x05" /* '㊻' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊽\x00"
                                "\x05" /* '㊽' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊾\x00"
                                "\x05" /* '㊾' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊼\x00"
                                "\x05" /* '㊼' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "④\x00"
                                "\x05" /* '④' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊼\x00"
                                "\x05" /* '㊼' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊽\x00"
                                "\x05" /* '㊽' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊶\x00"
                                "\x05" /* '㊶' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊸\x00"
                                "\x05" /* '㊸' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊻\x00"
                                "\x05" /* '㊻' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊹\x00"
                                "\x05" /* '㊹' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊸\x00"
                                "\x05" /* '㊸' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊵\x00"
                                "\x05" /* '㊵' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊺\x00"
                                "\x05" /* '㊺' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊺\x00"
                                "\x05" /* '㊺' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊾\x00"
                                "\x05" /* '㊾' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊵\x00"
                                "\x05" /* '㊵' 5 */
                                "kana_SA\x00"
                                "\x1a" /* 'kana_SA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋚\x00"
                                "\x05" /* '㋚' 5 */
                                "e\x00"
                                "\x14" /* 'e' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓔ\x00"
                                "\x05" /* 'ⓔ' 5 */
                                "F\x00"
                                "\x14" /* 'F' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓕ\x00"
                                "\x05" /* 'Ⓕ' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑥\x00"
                                "\x05" /* '⑥' 5 */
                                "o\x00"
                                "\x14" /* 'o' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓞ\x00"
                                "\x05" /* 'ⓞ' 5 */
                                "l\x00"
                                "\x14" /* 'l' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓛ\x00"
                                "\x05" /* 'ⓛ' 5 */
                                "kana_SE\x00"
                                "\x1a" /* 'kana_SE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋝\x00"
                                "\x05" /* '㋝' 5 */
                                "kana_SU\x00"
                                "\x1a" /* 'kana_SU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋜\x00"
                                "\x05" /* '㋜' 5 */
                                "t\x00"
                                "\x14" /* 't' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓣ\x00"
                                "\x05" /* 'ⓣ' 5 */
                                "kana_ME\x00"
                                "\x1a" /* 'kana_ME' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋱\x00"
                                "\x05" /* '㋱' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὑ\x00"
                                "\x05" /* 'ὑ' 5 */
                                "kana_WO\x00"
                                "\x1a" /* 'kana_WO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋾\x00"
                                "\x05" /* '㋾' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "˘\x00"
                                "\x04" /* '˘' 4 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑧\x00"
                                "\x05" /* '⑧' 5 */
                                "Greek_RHO\x00"
                                "\x10" /* 'Greek_RHO' 16 */
                                "Ῥ\x00"
                                "\x05" /* 'Ῥ' 5 */
                                "Q\x00"
                                "\x14" /* 'Q' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓠ\x00"
                                "\x05" /* 'Ⓠ' 5 */
                                "y\x00"
                                "\x14" /* 'y' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓨ\x00"
                                "\x05" /* 'ⓨ' 5 */
                                "b\x00"
                                "\x14" /* 'b' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓑ\x00"
                                "\x05" /* 'ⓑ' 5 */
                                "kana_YO\x00"
                                "\x1a" /* 'kana_YO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋵\x00"
                                "\x05" /* '㋵' 5 */
                                "i\x00"
                                "\x14" /* 'i' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓘ\x00"
                                "\x05" /* 'ⓘ' 5 */
                                "kana_MA\x00"
                                "\x1a" /* 'kana_MA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋮\x00"
                                "\x05" /* '㋮' 5 */
                                "k\x00"
                                "\x14" /* 'k' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓚ\x00"
                                "\x05" /* 'ⓚ' 5 */
                                "n\x00"
                                "\x14" /* 'n' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓝ\x00"
                                "\x05" /* 'ⓝ' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑨\x00"
                                "\x05" /* '⑨' 5 */
                                "KP_Space\x00"
                                "\x81\xe5" /* 'KP_Space' 485 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "②\x00"
                                "\x05" /* '②' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "kana_YU\x00"
                                "\x1a" /* 'kana_YU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋴\x00"
                                "\x05" /* '㋴' 5 */
                                "kana_TE\x00"
                                "\x1a" /* 'kana_TE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋢\x00"
                                "\x05" /* '㋢' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑦\x00"
                                "\x05" /* '⑦' 5 */
                                "kana_NU\x00"
                                "\x1a" /* 'kana_NU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋦\x00"
                                "\x05" /* '㋦' 5 */
                                "kana_HO\x00"
                                "\x1a" /* 'kana_HO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋭\x00"
                                "\x05" /* '㋭' 5 */
                                "kana_HI\x00"
                                "\x1a" /* 'kana_HI' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋪\x00"
                                "\x05" /* '㋪' 5 */
                                "j\x00"
                                "\x14" /* 'j' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓙ\x00"
                                "\x05" /* 'ⓙ' 5 */
                                "kana_E\x00"
                                "\x19" /* 'kana_E' 25 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋓\x00"
                                "\x05" /* '㋓' 5 */
                                "x\x00"
                                "\x14" /* 'x' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓧ\x00"
                                "\x05" /* 'ⓧ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἑ\x00"
                                "\x05" /* 'ἑ' 5 */
                                "q\x00"
                                "\x14" /* 'q' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓠ\x00"
                                "\x05" /* 'ⓠ' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑦\x00"
                                "\x05" /* '⑦' 5 */
                                "kana_I\x00"
                                "\x19" /* 'kana_I' 25 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋑\x00"
                                "\x05" /* '㋑' 5 */
                                "kana_WA\x00"
                                "\x1a" /* 'kana_WA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋻\x00"
                                "\x05" /* '㋻' 5 */
                                "kana_RU\x00"
                                "\x1a" /* 'kana_RU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋸\x00"
                                "\x05" /* '㋸' 5 */
                                "V\x00"
                                "\x14" /* 'V' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓥ\x00"
                                "\x05" /* 'Ⓥ' 5 */
                                "u\x00"
                                "\x14" /* 'u' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓤ\x00"
                                "\x05" /* 'ⓤ' 5 */
                                "kana_NI\x00"
                                "\x1a" /* 'kana_NI' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋥\x00"
                                "\x05" /* '㋥' 5 */
                                "kana_MU\x00"
                                "\x1a" /* 'kana_MU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋰\x00"
                                "\x05" /* '㋰' 5 */
                                "kana_CHI\x00"
                                "\x1b" /* 'kana_CHI' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋠\x00"
                                "\x05" /* '㋠' 5 */
                                "kana_HA\x00"
                                "\x1a" /* 'kana_HA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋩\x00"
                                "\x05" /* '㋩' 5 */
                                "z\x00"
                                "\x14" /* 'z' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓩ\x00"
                                "\x05" /* 'ⓩ' 5 */
                                "G\x00"
                                "\x14" /* 'G' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓖ\x00"
                                "\x05" /* 'Ⓖ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἁ\x00"
                                "\x05" /* 'Ἁ' 5 */
                                "H\x00"
                                "\x14" /* 'H' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓗ\x00"
                                "\x05" /* 'Ⓗ' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑧\x00"
                                "\x05" /* '⑧' 5 */
                                "KP_1\x00"
                                "\x81\xe1" /* 'KP_1' 481 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑪\x00"
                                "\x05" /* '⑪' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑭\x00"
                                "\x05" /* '⑭' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑯\x00"
                                "\x05" /* '⑯' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑱\x00"
                                "\x05" /* '⑱' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑲\x00"
                                "\x05" /* '⑲' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑰\x00"
                                "\x05" /* '⑰' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "①\x00"
                                "\x05" /* '①' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑰\x00"
                                "\x05" /* '⑰' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑱\x00"
                                "\x05" /* '⑱' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑪\x00"
                                "\x05" /* '⑪' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑬\x00"
                                "\x05" /* '⑬' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑯\x00"
                                "\x05" /* '⑯' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑭\x00"
                                "\x05" /* '⑭' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑬\x00"
                                "\x05" /* '⑬' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑩\x00"
                                "\x05" /* '⑩' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑫\x00"
                                "\x05" /* '⑫' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑮\x00"
                                "\x05" /* '⑮' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑮\x00"
                                "\x05" /* '⑮' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑲\x00"
                                "\x05" /* '⑲' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑩\x00"
                                "\x05" /* '⑩' 5 */
                                "3\x00"
                                "\x81\xde" /* '3' 478 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉛\x00"
                                "\x05" /* '㉛' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉞\x00"
                                "\x05" /* '㉞' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊱\x00"
                                "\x05" /* '㊱' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊳\x00"
                                "\x05" /* '㊳' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊴\x00"
                                "\x05" /* '㊴' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊲\x00"
                                "\x05" /* '㊲' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "③\x00"
                                "\x05" /* '③' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊲\x00"
                                "\x05" /* '㊲' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊳\x00"
                                "\x05" /* '㊳' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉛\x00"
                                "\x05" /* '㉛' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉝\x00"
                                "\x05" /* '㉝' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊱\x00"
                                "\x05" /* '㊱' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉞\x00"
                                "\x05" /* '㉞' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉝\x00"
                                "\x05" /* '㉝' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉚\x00"
                                "\x05" /* '㉚' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉟\x00"
                                "\x05" /* '㉟' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉟\x00"
                                "\x05" /* '㉟' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊴\x00"
                                "\x05" /* '㊴' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉚\x00"
                                "\x05" /* '㉚' 5 */
                                "E\x00"
                                "\x14" /* 'E' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓔ\x00"
                                "\x05" /* 'Ⓔ' 5 */
                                "S\x00"
                                "\x14" /* 'S' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓢ\x00"
                                "\x05" /* 'Ⓢ' 5 */
                                "2\x00"
                                "\x81\xde" /* '2' 478 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "②\x00"
                                "\x05" /* '②' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "Y\x00"
                                "\x14" /* 'Y' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓨ\x00"
                                "\x05" /* 'Ⓨ' 5 */
                                "kana_RA\x00"
                                "\x1a" /* 'kana_RA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋶\x00"
                                "\x05" /* '㋶' 5 */
                                "f\x00"
                                "\x14" /* 'f' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓕ\x00"
                                "\x05" /* 'ⓕ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὁ\x00"
                                "\x05" /* 'ὁ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἡ\x00"
                                "\x05" /* 'ἡ' 5 */
                                "kana_HE\x00"
                                "\x1a" /* 'kana_HE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋬\x00"
                                "\x05" /* '㋬' 5 */
                                "Greek_rho\x00"
                                "\x10" /* 'Greek_rho' 16 */
                                "ῥ\x00"
                                "\x05" /* 'ῥ' 5 */
                                "kana_KO\x00"
                                "\x1a" /* 'kana_KO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋙\x00"
                                "\x05" /* '㋙' 5 */
                                "d\x00"
                                "\x14" /* 'd' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓓ\x00"
                                "\x05" /* 'ⓓ' 5 */
                                "kana_NE\x00"
                                "\x1a" /* 'kana_NE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋧\x00"
                                "\x05" /* '㋧' 5 */
                                "D\x00"
                                "\x14" /* 'D' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓓ\x00"
                                "\x05" /* 'Ⓓ' 5 */
                                "kana_FU\x00"
                                "\x1a" /* 'kana_FU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋫\x00"
                                "\x05" /* '㋫' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑥\x00"
                                "\x05" /* '⑥' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἁ\x00"
                                "\x05" /* 'ἁ' 5 */
                                "kana_A\x00"
                                "\x19" /* 'kana_A' 25 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋐\x00"
                                "\x05" /* '㋐' 5 */
                                "w\x00"
                                "\x14" /* 'w' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓦ\x00"
                                "\x05" /* 'ⓦ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἡ\x00"
                                "\x05" /* 'Ἡ' 5 */
                                "4\x00"
                                "\x81\xde" /* '4' 478 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊶\x00"
                                "\x05" /* '㊶' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊹\x00"
                                "\x05" /* '㊹' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊻\x00"
                                "\x05" /* '㊻' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊽\x00"
                                "\x05" /* '㊽' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊾\x00"
                                "\x05" /* '㊾' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊼\x00"
                                "\x05" /* '㊼' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "④\x00"
                                "\x05" /* '④' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊼\x00"
                                "\x05" /* '㊼' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊽\x00"
                                "\x05" /* '㊽' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊶\x00"
                                "\x05" /* '㊶' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊸\x00"
                                "\x05" /* '㊸' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊻\x00"
                                "\x05" /* '㊻' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊹\x00"
                                "\x05" /* '㊹' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊸\x00"
                                "\x05" /* '㊸' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊵\x00"
                                "\x05" /* '㊵' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊷\x00"
                                "\x05" /* '㊷' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊺\x00"
                                "\x05" /* '㊺' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊺\x00"
                                "\x05" /* '㊺' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊾\x00"
                                "\x05" /* '㊾' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊵\x00"
                                "\x05" /* '㊵' 5 */
                                "kana_KU\x00"
                                "\x1a" /* 'kana_KU' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋗\x00"
                                "\x05" /* '㋗' 5 */
                                "KP_3\x00"
                                "\x81\xe1" /* 'KP_3' 481 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉛\x00"
                                "\x05" /* '㉛' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉞\x00"
                                "\x05" /* '㉞' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊱\x00"
                                "\x05" /* '㊱' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊳\x00"
                                "\x05" /* '㊳' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊴\x00"
                                "\x05" /* '㊴' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊲\x00"
                                "\x05" /* '㊲' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "③\x00"
                                "\x05" /* '③' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊲\x00"
                                "\x05" /* '㊲' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊳\x00"
                                "\x05" /* '㊳' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉛\x00"
                                "\x05" /* '㉛' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉝\x00"
                                "\x05" /* '㉝' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊱\x00"
                                "\x05" /* '㊱' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉞\x00"
                                "\x05" /* '㉞' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉝\x00"
                                "\x05" /* '㉝' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉚\x00"
                                "\x05" /* '㉚' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉜\x00"
                                "\x05" /* '㉜' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉟\x00"
                                "\x05" /* '㉟' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉟\x00"
                                "\x05" /* '㉟' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊴\x00"
                                "\x05" /* '㊴' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉚\x00"
                                "\x05" /* '㉚' 5 */
                                "p\x00"
                                "\x14" /* 'p' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓟ\x00"
                                "\x05" /* 'ⓟ' 5 */
                                "J\x00"
                                "\x14" /* 'J' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓙ\x00"
                                "\x05" /* 'Ⓙ' 5 */
                                "kana_YA\x00"
                                "\x1a" /* 'kana_YA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋳\x00"
                                "\x05" /* '㋳' 5 */
                                "v\x00"
                                "\x14" /* 'v' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓥ\x00"
                                "\x05" /* 'ⓥ' 5 */
                                "P\x00"
                                "\x14" /* 'P' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓟ\x00"
                                "\x05" /* 'Ⓟ' 5 */
                                "M\x00"
                                "\x14" /* 'M' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓜ\x00"
                                "\x05" /* 'Ⓜ' 5 */
                                "O\x00"
                                "\x14" /* 'O' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓞ\x00"
                                "\x05" /* 'Ⓞ' 5 */
                                "m\x00"
                                "\x14" /* 'm' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓜ\x00"
                                "\x05" /* 'ⓜ' 5 */
                                "r\x00"
                                "\x14" /* 'r' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓡ\x00"
                                "\x05" /* 'ⓡ' 5 */
                                "s\x00"
                                "\x14" /* 's' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓢ\x00"
                                "\x05" /* 'ⓢ' 5 */
                                "Z\x00"
                                "\x14" /* 'Z' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓩ\x00"
                                "\x05" /* 'Ⓩ' 5 */
                                "kana_U\x00"
                                "\x19" /* 'kana_U' 25 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋒\x00"
                                "\x05" /* '㋒' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⓪\x00"
                                "\x05" /* '⓪' 5 */
                                "A\x00"
                                "\x14" /* 'A' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓐ\x00"
                                "\x05" /* 'Ⓐ' 5 */
                                "R\x00"
                                "\x14" /* 'R' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓡ\x00"
                                "\x05" /* 'Ⓡ' 5 */
                                "kana_TO\x00"
                                "\x1a" /* 'kana_TO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋣\x00"
                                "\x05" /* '㋣' 5 */
                                "kana_TA\x00"
                                "\x1a" /* 'kana_TA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋟\x00"
                                "\x05" /* '㋟' 5 */
                                "c\x00"
                                "\x14" /* 'c' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓒ\x00"
                                "\x05" /* 'ⓒ' 5 */
                                "kana_RO\x00"
                                "\x1a" /* 'kana_RO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋺\x00"
                                "\x05" /* '㋺' 5 */
                                "L\x00"
                                "\x14" /* 'L' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓛ\x00"
                                "\x05" /* 'Ⓛ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἑ\x00"
                                "\x05" /* 'Ἑ' 5 */
                                "KP_2\x00"
                                "\x81\xe1" /* 'KP_2' 481 */
                                "1\x00"
                                "\x14" /* '1' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "KP_4\x00"
                                "\x17" /* 'KP_4' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_6\x00"
                                "\x17" /* 'KP_6' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "KP_8\x00"
                                "\x17" /* 'KP_8' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_9\x00"
                                "\x17" /* 'KP_9' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "KP_Space\x00"
                                "\x1b" /* 'KP_Space' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "7\x00"
                                "\x14" /* '7' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "②\x00"
                                "\x05" /* '②' 5 */
                                "KP_7\x00"
                                "\x17" /* 'KP_7' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉗\x00"
                                "\x05" /* '㉗' 5 */
                                "8\x00"
                                "\x14" /* '8' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉘\x00"
                                "\x05" /* '㉘' 5 */
                                "KP_1\x00"
                                "\x17" /* 'KP_1' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉑\x00"
                                "\x05" /* '㉑' 5 */
                                "3\x00"
                                "\x14" /* '3' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "2\x00"
                                "\x14" /* '2' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "6\x00"
                                "\x14" /* '6' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉖\x00"
                                "\x05" /* '㉖' 5 */
                                "4\x00"
                                "\x14" /* '4' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉔\x00"
                                "\x05" /* '㉔' 5 */
                                "KP_3\x00"
                                "\x17" /* 'KP_3' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉓\x00"
                                "\x05" /* '㉓' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "KP_2\x00"
                                "\x17" /* 'KP_2' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉒\x00"
                                "\x05" /* '㉒' 5 */
                                "5\x00"
                                "\x14" /* '5' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "KP_5\x00"
                                "\x17" /* 'KP_5' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉕\x00"
                                "\x05" /* '㉕' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㉙\x00"
                                "\x05" /* '㉙' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑳\x00"
                                "\x05" /* '⑳' 5 */
                                "kana_O\x00"
                                "\x19" /* 'kana_O' 25 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋔\x00"
                                "\x05" /* '㋔' 5 */
                                "kana_RI\x00"
                                "\x1a" /* 'kana_RI' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋷\x00"
                                "\x05" /* '㋷' 5 */
                                "T\x00"
                                "\x14" /* 'T' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓣ\x00"
                                "\x05" /* 'Ⓣ' 5 */
                                "kana_KA\x00"
                                "\x1a" /* 'kana_KA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋕\x00"
                                "\x05" /* '㋕' 5 */
                                "kana_MI\x00"
                                "\x1a" /* 'kana_MI' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋯\x00"
                                "\x05" /* '㋯' 5 */
                                "5\x00"
                                "\x3f" /* '5' 63 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑤\x00"
                                "\x05" /* '⑤' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊿\x00"
                                "\x05" /* '㊿' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊿\x00"
                                "\x05" /* '㊿' 5 */
                                "kana_KI\x00"
                                "\x1a" /* 'kana_KI' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋖\x00"
                                "\x05" /* '㋖' 5 */
                                "KP_5\x00"
                                "\x42" /* 'KP_5' 66 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑤\x00"
                                "\x05" /* '⑤' 5 */
                                "KP_0\x00"
                                "\x17" /* 'KP_0' 23 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊿\x00"
                                "\x05" /* '㊿' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㊿\x00"
                                "\x05" /* '㊿' 5 */
                                "K\x00"
                                "\x14" /* 'K' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓚ\x00"
                                "\x05" /* 'Ⓚ' 5 */
                                "9\x00"
                                "\x14" /* '9' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⑨\x00"
                                "\x05" /* '⑨' 5 */
                                "kana_SO\x00"
                                "\x1a" /* 'kana_SO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋞\x00"
                                "\x05" /* '㋞' 5 */
                                "B\x00"
                                "\x14" /* 'B' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓑ\x00"
                                "\x05" /* 'Ⓑ' 5 */
                                "kana_TSU\x00"
                                "\x1b" /* 'kana_TSU' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋡\x00"
                                "\x05" /* '㋡' 5 */
                                "0\x00"
                                "\x14" /* '0' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⓪\x00"
                                "\x05" /* '⓪' 5 */
                                "kana_MO\x00"
                                "\x1a" /* 'kana_MO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋲\x00"
                                "\x05" /* '㋲' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὡ\x00"
                                "\x05" /* 'ὡ' 5 */
                                "kana_NO\x00"
                                "\x1a" /* 'kana_NO' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋨\x00"
                                "\x05" /* '㋨' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὡ\x00"
                                "\x05" /* 'Ὡ' 5 */
                                "kana_NA\x00"
                                "\x1a" /* 'kana_NA' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋤\x00"
                                "\x05" /* '㋤' 5 */
                                "X\x00"
                                "\x14" /* 'X' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓧ\x00"
                                "\x05" /* 'Ⓧ' 5 */
                                "parenleft\x00"
                                "\x0e" /* 'parenleft' 14 */
                                "[\x00"
                                "\x03" /* '[' 3 */
                                "h\x00"
                                "\x14" /* 'h' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "ⓗ\x00"
                                "\x05" /* 'ⓗ' 5 */
                                "I\x00"
                                "\x14" /* 'I' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓘ\x00"
                                "\x05" /* 'Ⓘ' 5 */
                                "N\x00"
                                "\x14" /* 'N' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓝ\x00"
                                "\x05" /* 'Ⓝ' 5 */
                                "kana_SHI\x00"
                                "\x1b" /* 'kana_SHI' 27 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋛\x00"
                                "\x05" /* '㋛' 5 */
                                "U\x00"
                                "\x14" /* 'U' 20 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "Ⓤ\x00"
                                "\x05" /* 'Ⓤ' 5 */
                                "kana_RE\x00"
                                "\x1a" /* 'kana_RE' 26 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "㋹\x00"
                                "\x05" /* '㋹' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὑ\x00"
                                "\x05" /* 'Ὑ' 5 */
                                "h\x00"
                                "\x0f" /* 'h' 15 */
                                "comma\x00"
                                "\x0c" /* 'comma' 12 */
                                "ḩ\x00"
                                "\x05" /* 'ḩ' 5 */
                                "I\x00"
                                "\x80\xb5" /* 'I' 181 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "İ\x00"
                                "\x04" /* 'İ' 4 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ï\x00"
                                "\x04" /* 'Ï' 4 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "Ĳ\x00"
                                "\x04" /* 'Ĳ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ï\x00"
                                "\x04" /* 'Ï' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "Í\x00"
                                "\x04" /* 'Í' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "Ī\x00"
                                "\x04" /* 'Ī' 4 */
                                "J\x00"
                                "\x07" /* 'J' 7 */
                                "Ĳ\x00"
                                "\x04" /* 'Ĳ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Í\x00"
                                "\x04" /* 'Í' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Į\x00"
                                "\x04" /* 'Į' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "Ĩ\x00"
                                "\x04" /* 'Ĩ' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "Î\x00"
                                "\x04" /* 'Î' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "Ì\x00"
                                "\x04" /* 'Ì' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Î\x00"
                                "\x04" /* 'Î' 4 */
                                "N\x00"
                                "\x5b" /* 'N' 91 */
                                "less\x00"
                                "\x0a" /* 'less' 10 */
                                "Ň\x00"
                                "\x04" /* 'Ň' 4 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "№\x00"
                                "\x05" /* '№' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₦\x00"
                                "\x05" /* '₦' 5 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ŋ\x00"
                                "\x04" /* 'Ŋ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ń\x00"
                                "\x04" /* 'Ń' 4 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "№\x00"
                                "\x05" /* '№' 5 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ņ\x00"
                                "\x04" /* 'Ņ' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "Ñ\x00"
                                "\x04" /* 'Ñ' 4 */
                                "grave\x00"
                                "\x8a\x95" /* 'grave' 2709 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẁ\x00"
                                "\x05" /* 'Ẁ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "à\x00"
                                "\x04" /* 'à' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ὶ\x00"
                                "\x05" /* 'Ὶ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ὶ\x00"
                                "\x05" /* 'ὶ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὸ\x00"
                                "\x05" /* 'Ὸ' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̀\x00"
                                "\x06" /* 'р̀' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "è\x00"
                                "\x04" /* 'è' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ò\x00"
                                "\x04" /* 'ò' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὺ\x00"
                                "\x05" /* 'ὺ' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "`\x00"
                                "\x03" /* '`' 3 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ѝ\x00"
                                "\x04" /* 'Ѝ' 4 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỳ\x00"
                                "\x05" /* 'ỳ' 5 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̀\x00"
                                "\x06" /* 'О̀' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ì\x00"
                                "\x04" /* 'ì' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ǹ\x00"
                                "\x04" /* 'ǹ' 4 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̀\x00"
                                "\x06" /* 'а̀' 6 */
                                "parenright\x00"
                                "\x80\xfb" /* 'parenright' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἲ\x00"
                                "\x05" /* 'Ἲ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἲ\x00"
                                "\x05" /* 'ἲ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὂ\x00"
                                "\x05" /* 'Ὂ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὒ\x00"
                                "\x05" /* 'ὒ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἒ\x00"
                                "\x05" /* 'ἒ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἂ\x00"
                                "\x05" /* 'Ἂ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὂ\x00"
                                "\x05" /* 'ὂ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἢ\x00"
                                "\x05" /* 'ἢ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἂ\x00"
                                "\x05" /* 'ἂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἢ\x00"
                                "\x05" /* 'Ἢ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἒ\x00"
                                "\x05" /* 'Ἒ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὢ\x00"
                                "\x05" /* 'ὢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὢ\x00"
                                "\x05" /* 'Ὢ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̀\x00"
                                "\x06" /* 'Р̀' 6 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ὲ\x00"
                                "\x05" /* 'ὲ' 5 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̀\x00"
                                "\x06" /* 'У̀' 6 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "omacron\x00"
                                "\x0e" /* 'omacron' 14 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ù\x00"
                                "\x04" /* 'ù' 4 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ὰ\x00"
                                "\x05" /* 'Ὰ' 5 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ѐ\x00"
                                "\x04" /* 'ѐ' 4 */
                                "emacron\x00"
                                "\x0e" /* 'emacron' 14 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "È\x00"
                                "\x04" /* 'È' 4 */
                                "Greek_iotadieresis\x00"
                                "\x19" /* 'Greek_iotadieresis' 25 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỳ\x00"
                                "\x05" /* 'Ỳ' 5 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ѝ\x00"
                                "\x04" /* 'ѝ' 4 */
                                "dead_dasia\x00"
                                "\x81\x0f" /* 'dead_dasia' 271 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἳ\x00"
                                "\x05" /* 'Ἳ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἳ\x00"
                                "\x05" /* 'ἳ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὃ\x00"
                                "\x05" /* 'Ὃ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὓ\x00"
                                "\x05" /* 'ὓ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἓ\x00"
                                "\x05" /* 'ἓ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἃ\x00"
                                "\x05" /* 'Ἃ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὃ\x00"
                                "\x05" /* 'ὃ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἣ\x00"
                                "\x05" /* 'ἣ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἃ\x00"
                                "\x05" /* 'ἃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἣ\x00"
                                "\x05" /* 'Ἣ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἓ\x00"
                                "\x05" /* 'Ἓ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὣ\x00"
                                "\x05" /* 'ὣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὣ\x00"
                                "\x05" /* 'Ὣ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὓ\x00"
                                "\x05" /* 'Ὓ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1c" /* 'Greek_upsilondieresis' 28 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὸ\x00"
                                "\x05" /* 'ὸ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ὴ\x00"
                                "\x05" /* 'ὴ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ằ\x00"
                                "\x05" /* 'Ằ' 5 */
                                "dead_psili\x00"
                                "\x80\xfb" /* 'dead_psili' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἲ\x00"
                                "\x05" /* 'Ἲ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἲ\x00"
                                "\x05" /* 'ἲ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὂ\x00"
                                "\x05" /* 'Ὂ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὒ\x00"
                                "\x05" /* 'ὒ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἒ\x00"
                                "\x05" /* 'ἒ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἂ\x00"
                                "\x05" /* 'Ἂ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὂ\x00"
                                "\x05" /* 'ὂ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἢ\x00"
                                "\x05" /* 'ἢ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἂ\x00"
                                "\x05" /* 'ἂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἢ\x00"
                                "\x05" /* 'Ἢ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἒ\x00"
                                "\x05" /* 'Ἒ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὢ\x00"
                                "\x05" /* 'ὢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὢ\x00"
                                "\x05" /* 'Ὢ' 5 */
                                "quotedbl\x00"
                                "\x3d" /* 'quotedbl' 61 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ờ\x00"
                                "\x05" /* 'ờ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ừ\x00"
                                "\x05" /* 'ừ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ờ\x00"
                                "\x05" /* 'Ờ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ὰ\x00"
                                "\x05" /* 'ὰ' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẁ\x00"
                                "\x05" /* 'ẁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ὴ\x00"
                                "\x05" /* 'Ὴ' 5 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̀\x00"
                                "\x06" /* 'о̀' 6 */
                                "Emacron\x00"
                                "\x0e" /* 'Emacron' 14 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "underscore\x00"
                                "\x2c" /* 'underscore' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ò\x00"
                                "\x04" /* 'Ò' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ằ\x00"
                                "\x05" /* 'ằ' 5 */
                                "macron\x00"
                                "\x28" /* 'macron' 40 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḕ\x00"
                                "\x05" /* 'ḕ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṑ\x00"
                                "\x05" /* 'ṑ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḕ\x00"
                                "\x05" /* 'Ḕ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "À\x00"
                                "\x04" /* 'À' 4 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ὲ\x00"
                                "\x05" /* 'Ὲ' 5 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̀\x00"
                                "\x06" /* 'А̀' 6 */
                                "Omacron\x00"
                                "\x0e" /* 'Omacron' 14 */
                                "Ṑ\x00"
                                "\x05" /* 'Ṑ' 5 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ѐ\x00"
                                "\x04" /* 'Ѐ' 4 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὼ\x00"
                                "\x05" /* 'ὼ' 5 */
                                "dead_diaeresis\x00"
                                "\x43" /* 'dead_diaeresis' 67 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῒ\x00"
                                "\x05" /* 'ῒ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῢ\x00"
                                "\x05" /* 'ῢ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ừ\x00"
                                "\x05" /* 'Ừ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὼ\x00"
                                "\x05" /* 'Ὼ' 5 */
                                "parenleft\x00"
                                "\x81\x0e" /* 'parenleft' 270 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἳ\x00"
                                "\x05" /* 'Ἳ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἳ\x00"
                                "\x05" /* 'ἳ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὃ\x00"
                                "\x05" /* 'Ὃ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὓ\x00"
                                "\x05" /* 'ὓ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἓ\x00"
                                "\x05" /* 'ἓ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἃ\x00"
                                "\x05" /* 'Ἃ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὃ\x00"
                                "\x05" /* 'ὃ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἣ\x00"
                                "\x05" /* 'ἣ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἃ\x00"
                                "\x05" /* 'ἃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἣ\x00"
                                "\x05" /* 'Ἣ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἓ\x00"
                                "\x05" /* 'Ἓ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὣ\x00"
                                "\x05" /* 'ὣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὣ\x00"
                                "\x05" /* 'Ὣ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὓ\x00"
                                "\x05" /* 'Ὓ' 5 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ì\x00"
                                "\x04" /* 'Ì' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ǹ\x00"
                                "\x04" /* 'Ǹ' 4 */
                                "grave\x00"
                                "\x80\xe4" /* 'grave' 228 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̏\x00"
                                "\x06" /* 'р̏' 6 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И̏\x00"
                                "\x06" /* 'И̏' 6 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̏\x00"
                                "\x06" /* 'О̏' 6 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̏\x00"
                                "\x06" /* 'а̏' 6 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̏\x00"
                                "\x06" /* 'Р̏' 6 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̏\x00"
                                "\x06" /* 'У̏' 6 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̏\x00"
                                "\x06" /* 'е̏' 6 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и̏\x00"
                                "\x06" /* 'и̏' 6 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̏\x00"
                                "\x06" /* 'о̏' 6 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̏\x00"
                                "\x06" /* 'А̏' 6 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̏\x00"
                                "\x06" /* 'Е̏' 6 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̏\x00"
                                "\x06" /* 'у̏' 6 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ù\x00"
                                "\x04" /* 'Ù' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̀\x00"
                                "\x06" /* 'у̀' 6 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ầ\x00"
                                "\x05" /* 'ầ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ề\x00"
                                "\x05" /* 'ề' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ồ\x00"
                                "\x05" /* 'ồ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ề\x00"
                                "\x05" /* 'Ề' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ồ\x00"
                                "\x05" /* 'Ồ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ầ\x00"
                                "\x05" /* 'Ầ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὺ\x00"
                                "\x05" /* 'Ὺ' 5 */
                                "U\x00"
                                "\x82\x9d" /* 'U' 669 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ğ\x00"
                                "\x04" /* 'ğ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ă\x00"
                                "\x04" /* 'ă' 4 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ῐ\x00"
                                "\x05" /* 'Ῐ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῐ\x00"
                                "\x05" /* 'ῐ' 5 */
                                "exclam\x00"
                                "\x18" /* 'exclam' 24 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ĕ\x00"
                                "\x04" /* 'ĕ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ŏ\x00"
                                "\x04" /* 'ŏ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῠ\x00"
                                "\x05" /* 'ῠ' 5 */
                                "diaeresis\x00"
                                "\x0f" /* 'diaeresis' 15 */
                                "Ü\x00"
                                "\x04" /* 'Ü' 4 */
                                "dead_belowdot\x00"
                                "\x1f" /* 'dead_belowdot' 31 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ặ\x00"
                                "\x05" /* 'ặ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ặ\x00"
                                "\x05" /* 'Ặ' 5 */
                                "space\x00"
                                "\x1e" /* 'space' 30 */
                                "comma\x00"
                                "\x17" /* 'comma' 23 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Й\x00"
                                "\x04" /* 'Й' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ĭ\x00"
                                "\x04" /* 'ĭ' 4 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "ӑ\x00"
                                "\x04" /* 'ӑ' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ў\x00"
                                "\x04" /* 'Ў' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ŭ\x00"
                                "\x04" /* 'ŭ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ğ\x00"
                                "\x04" /* 'Ğ' 4 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ᾰ\x00"
                                "\x05" /* 'Ᾰ' 5 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ӗ\x00"
                                "\x04" /* 'ӗ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ĕ\x00"
                                "\x04" /* 'Ĕ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "й\x00"
                                "\x04" /* 'й' 4 */
                                "Cyrillic_zhe\x00"
                                "\x12" /* 'Cyrillic_zhe' 18 */
                                "ӂ\x00"
                                "\x04" /* 'ӂ' 4 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ü\x00"
                                "\x04" /* 'Ü' 4 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾰ\x00"
                                "\x05" /* 'ᾰ' 5 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "Ú\x00"
                                "\x04" /* 'Ú' 4 */
                                "underscore\x00"
                                "\x10" /* 'underscore' 16 */
                                "Ū\x00"
                                "\x04" /* 'Ū' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ú\x00"
                                "\x04" /* 'Ú' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ŏ\x00"
                                "\x04" /* 'Ŏ' 4 */
                                "asterisk\x00"
                                "\x0e" /* 'asterisk' 14 */
                                "Ů\x00"
                                "\x04" /* 'Ů' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ă\x00"
                                "\x04" /* 'Ă' 4 */
                                "Cyrillic_A\x00"
                                "\x10" /* 'Cyrillic_A' 16 */
                                "Ӑ\x00"
                                "\x04" /* 'Ӑ' 4 */
                                "comma\x00"
                                "\x0b" /* 'comma' 11 */
                                "Ų\x00"
                                "\x04" /* 'Ų' 4 */
                                "asciitilde\x00"
                                "\x10" /* 'asciitilde' 16 */
                                "Ũ\x00"
                                "\x04" /* 'Ũ' 4 */
                                "greater\x00"
                                "\x0d" /* 'greater' 13 */
                                "Û\x00"
                                "\x04" /* 'Û' 4 */
                                "Cyrillic_ZHE\x00"
                                "\x12" /* 'Cyrillic_ZHE' 18 */
                                "Ӂ\x00"
                                "\x04" /* 'Ӂ' 4 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ӗ\x00"
                                "\x04" /* 'Ӗ' 4 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ĭ\x00"
                                "\x04" /* 'Ĭ' 4 */
                                "grave\x00"
                                "\x0b" /* 'grave' 11 */
                                "Ù\x00"
                                "\x04" /* 'Ù' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ŭ\x00"
                                "\x04" /* 'Ŭ' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ў\x00"
                                "\x04" /* 'ў' 4 */
                                "asciicircum\x00"
                                "\x11" /* 'asciicircum' 17 */
                                "Û\x00"
                                "\x04" /* 'Û' 4 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ῠ\x00"
                                "\x05" /* 'Ῠ' 5 */
                                "asciicircum\x00"
                                "\x84\x20" /* 'asciicircum' 1056 */
                                "minus\x00"
                                "\x0b" /* 'minus' 11 */
                                "¯\x00"
                                "\x04" /* '¯' 4 */
                                "period\x00"
                                "\x0c" /* 'period' 12 */
                                "·\x00"
                                "\x04" /* '·' 4 */
                                "W\x00"
                                "\x07" /* 'W' 7 */
                                "Ŵ\x00"
                                "\x04" /* 'Ŵ' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ĝ\x00"
                                "\x04" /* 'ĝ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "â\x00"
                                "\x04" /* 'â' 4 */
                                "1\x00"
                                "\x07" /* '1' 7 */
                                "¹\x00"
                                "\x04" /* '¹' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ĉ\x00"
                                "\x04" /* 'Ĉ' 4 */
                                "KP_4\x00"
                                "\x0b" /* 'KP_4' 11 */
                                "⁴\x00"
                                "\x05" /* '⁴' 5 */
                                "exclam\x00"
                                "\x38" /* 'exclam' 56 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̂\x00"
                                "\x06" /* 'р̂' 6 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ô\x00"
                                "\x04" /* 'ô' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ê\x00"
                                "\x04" /* 'ê' 4 */
                                "KP_6\x00"
                                "\x0b" /* 'KP_6' 11 */
                                "⁶\x00"
                                "\x05" /* '⁶' 5 */
                                "dead_belowdot\x00"
                                "\x3f" /* 'dead_belowdot' 63 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ậ\x00"
                                "\x05" /* 'ậ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ệ\x00"
                                "\x05" /* 'ệ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ộ\x00"
                                "\x05" /* 'ộ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ệ\x00"
                                "\x05" /* 'Ệ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ộ\x00"
                                "\x05" /* 'Ộ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ậ\x00"
                                "\x05" /* 'Ậ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "^\x00"
                                "\x03" /* '^' 3 */
                                "KP_8\x00"
                                "\x0b" /* 'KP_8' 11 */
                                "⁸\x00"
                                "\x05" /* '⁸' 5 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И̂\x00"
                                "\x06" /* 'И̂' 6 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ŷ\x00"
                                "\x04" /* 'ŷ' 4 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̂\x00"
                                "\x06" /* 'О̂' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "î\x00"
                                "\x04" /* 'î' 4 */
                                "KP_9\x00"
                                "\x0b" /* 'KP_9' 11 */
                                "⁹\x00"
                                "\x05" /* '⁹' 5 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "⁼\x00"
                                "\x05" /* '⁼' 5 */
                                "KP_Space\x00"
                                "\x0e" /* 'KP_Space' 14 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "⁷\x00"
                                "\x05" /* '⁷' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̂\x00"
                                "\x06" /* 'а̂' 6 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ĵ\x00"
                                "\x04" /* 'ĵ' 4 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "⁾\x00"
                                "\x05" /* '⁾' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̂\x00"
                                "\x06" /* 'Р̂' 6 */
                                "KP_7\x00"
                                "\x0b" /* 'KP_7' 11 */
                                "⁷\x00"
                                "\x05" /* '⁷' 5 */
                                "underbar\x00"
                                "\x60" /* 'underbar' 96 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ª\x00"
                                "\x04" /* 'ª' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "º\x00"
                                "\x04" /* 'º' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ˡ\x00"
                                "\x04" /* 'ˡ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ʸ\x00"
                                "\x04" /* 'ʸ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ⁱ\x00"
                                "\x05" /* 'ⁱ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ⁿ\x00"
                                "\x05" /* 'ⁿ' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ʲ\x00"
                                "\x04" /* 'ʲ' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "ˣ\x00"
                                "\x04" /* 'ˣ' 4 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ʷ\x00"
                                "\x04" /* 'ʷ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ʳ\x00"
                                "\x04" /* 'ʳ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ˢ\x00"
                                "\x04" /* 'ˢ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ʰ\x00"
                                "\x04" /* 'ʰ' 4 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̂\x00"
                                "\x06" /* 'У̂' 6 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "û\x00"
                                "\x04" /* 'û' 4 */
                                "z\x00"
                                "\x08" /* 'z' 8 */
                                "ẑ\x00"
                                "\x05" /* 'ẑ' 5 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ĝ\x00"
                                "\x04" /* 'Ĝ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ĥ\x00"
                                "\x04" /* 'Ĥ' 4 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "⁸\x00"
                                "\x05" /* '⁸' 5 */
                                "KP_1\x00"
                                "\x0a" /* 'KP_1' 10 */
                                "¹\x00"
                                "\x04" /* '¹' 4 */
                                "3\x00"
                                "\x07" /* '3' 7 */
                                "³\x00"
                                "\x04" /* '³' 4 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̂\x00"
                                "\x06" /* 'е̂' 6 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ê\x00"
                                "\x04" /* 'Ê' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ŝ\x00"
                                "\x04" /* 'Ŝ' 4 */
                                "2\x00"
                                "\x07" /* '2' 7 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ŷ\x00"
                                "\x04" /* 'Ŷ' 4 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и̂\x00"
                                "\x06" /* 'и̂' 6 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "⁺\x00"
                                "\x05" /* '⁺' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "⁶\x00"
                                "\x05" /* '⁶' 5 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ŵ\x00"
                                "\x04" /* 'ŵ' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̂\x00"
                                "\x06" /* 'о̂' 6 */
                                "4\x00"
                                "\x08" /* '4' 8 */
                                "⁴\x00"
                                "\x05" /* '⁴' 5 */
                                "KP_3\x00"
                                "\x0a" /* 'KP_3' 10 */
                                "³\x00"
                                "\x04" /* '³' 4 */
                                "underscore\x00"
                                "\x62" /* 'underscore' 98 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ª\x00"
                                "\x04" /* 'ª' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "º\x00"
                                "\x04" /* 'º' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ˡ\x00"
                                "\x04" /* 'ˡ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ʸ\x00"
                                "\x04" /* 'ʸ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ⁱ\x00"
                                "\x05" /* 'ⁱ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ⁿ\x00"
                                "\x05" /* 'ⁿ' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ʲ\x00"
                                "\x04" /* 'ʲ' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "ˣ\x00"
                                "\x04" /* 'ˣ' 4 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ʷ\x00"
                                "\x04" /* 'ʷ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ʳ\x00"
                                "\x04" /* 'ʳ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ˢ\x00"
                                "\x04" /* 'ˢ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ʰ\x00"
                                "\x04" /* 'ʰ' 4 */
                                "J\x00"
                                "\x07" /* 'J' 7 */
                                "Ĵ\x00"
                                "\x04" /* 'Ĵ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ô\x00"
                                "\x04" /* 'Ô' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ŝ\x00"
                                "\x04" /* 'ŝ' 4 */
                                "Z\x00"
                                "\x08" /* 'Z' 8 */
                                "Ẑ\x00"
                                "\x05" /* 'Ẑ' 5 */
                                "KP_0\x00"
                                "\x0b" /* 'KP_0' 11 */
                                "⁰\x00"
                                "\x05" /* '⁰' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Â\x00"
                                "\x04" /* 'Â' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ĉ\x00"
                                "\x04" /* 'ĉ' 4 */
                                "KP_Add\x00"
                                "\x0d" /* 'KP_Add' 13 */
                                "⁺\x00"
                                "\x05" /* '⁺' 5 */
                                "KP_2\x00"
                                "\x0a" /* 'KP_2' 10 */
                                "²\x00"
                                "\x04" /* '²' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̂\x00"
                                "\x06" /* 'А̂' 6 */
                                "slash\x00"
                                "\x0a" /* 'slash' 10 */
                                "|\x00"
                                "\x03" /* '|' 3 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "⁵\x00"
                                "\x05" /* '⁵' 5 */
                                "KP_5\x00"
                                "\x0b" /* 'KP_5' 11 */
                                "⁵\x00"
                                "\x05" /* '⁵' 5 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "⁹\x00"
                                "\x05" /* '⁹' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̂\x00"
                                "\x06" /* 'Е̂' 6 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "⁰\x00"
                                "\x05" /* '⁰' 5 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "⁽\x00"
                                "\x05" /* '⁽' 5 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ĥ\x00"
                                "\x04" /* 'ĥ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Î\x00"
                                "\x04" /* 'Î' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Û\x00"
                                "\x04" /* 'Û' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̂\x00"
                                "\x06" /* 'у̂' 6 */
                                "KP_Equal\x00"
                                "\x0f" /* 'KP_Equal' 15 */
                                "⁼\x00"
                                "\x05" /* '⁼' 5 */
                                "Greek_UPSILON\x00"
                                "\x2d" /* 'Greek_UPSILON' 45 */
                                "quotedbl\x00"
                                "\x0e" /* 'quotedbl' 14 */
                                "Ϋ\x00"
                                "\x04" /* 'Ϋ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "Ύ\x00"
                                "\x04" /* 'Ύ' 4 */
                                "dead_belowcircumflex\x00"
                                "\x76" /* 'dead_belowcircumflex' 118 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḙ\x00"
                                "\x05" /* 'ḙ' 5 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḽ\x00"
                                "\x05" /* 'ḽ' 5 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṱ\x00"
                                "\x05" /* 'ṱ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṋ\x00"
                                "\x05" /* 'ṋ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṷ\x00"
                                "\x05" /* 'ṷ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḙ\x00"
                                "\x05" /* 'Ḙ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḓ\x00"
                                "\x05" /* 'ḓ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḓ\x00"
                                "\x05" /* 'Ḓ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḽ\x00"
                                "\x05" /* 'Ḽ' 5 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṱ\x00"
                                "\x05" /* 'Ṱ' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṋ\x00"
                                "\x05" /* 'Ṋ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṷ\x00"
                                "\x05" /* 'Ṷ' 5 */
                                "dead_caron\x00"
                                "\x82\x75" /* 'dead_caron' 629 */
                                "minus\x00"
                                "\x0c" /* 'minus' 12 */
                                "₋\x00"
                                "\x05" /* '₋' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǧ\x00"
                                "\x04" /* 'ǧ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǎ\x00"
                                "\x04" /* 'ǎ' 4 */
                                "1\x00"
                                "\x08" /* '1' 8 */
                                "₁\x00"
                                "\x05" /* '₁' 5 */
                                "ezh\x00"
                                "\x09" /* 'ezh' 9 */
                                "ǯ\x00"
                                "\x04" /* 'ǯ' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Č\x00"
                                "\x04" /* 'Č' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ě\x00"
                                "\x04" /* 'ě' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǒ\x00"
                                "\x04" /* 'ǒ' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ľ\x00"
                                "\x04" /* 'ľ' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ť\x00"
                                "\x04" /* 'ť' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "ˇ\x00"
                                "\x04" /* 'ˇ' 4 */
                                "Multi_key\x00"
                                "\x23" /* 'Multi_key' 35 */
                                "quotedbl\x00"
                                "\x18" /* 'quotedbl' 24 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ǐ\x00"
                                "\x04" /* 'ǐ' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ǩ\x00"
                                "\x04" /* 'ǩ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ň\x00"
                                "\x04" /* 'ň' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "₌\x00"
                                "\x05" /* '₌' 5 */
                                "dead_caron\x00"
                                "\x10" /* 'dead_caron' 16 */
                                "ˇ\x00"
                                "\x04" /* 'ˇ' 4 */
                                "7\x00"
                                "\x08" /* '7' 8 */
                                "₇\x00"
                                "\x05" /* '₇' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ǰ\x00"
                                "\x04" /* 'ǰ' 4 */
                                "parenright\x00"
                                "\x11" /* 'parenright' 17 */
                                "₎\x00"
                                "\x05" /* '₎' 5 */
                                "sabovedot\x00"
                                "\x10" /* 'sabovedot' 16 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̌\x00"
                                "\x04" /* '̌' 4 */
                                "V\x00"
                                "\x07" /* 'V' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǔ\x00"
                                "\x04" /* 'ǔ' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ž\x00"
                                "\x04" /* 'ž' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǧ\x00"
                                "\x04" /* 'Ǧ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ȟ\x00"
                                "\x04" /* 'Ȟ' 4 */
                                "8\x00"
                                "\x08" /* '8' 8 */
                                "₈\x00"
                                "\x05" /* '₈' 5 */
                                "3\x00"
                                "\x08" /* '3' 8 */
                                "₃\x00"
                                "\x05" /* '₃' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ě\x00"
                                "\x04" /* 'Ě' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Š\x00"
                                "\x04" /* 'Š' 4 */
                                "2\x00"
                                "\x08" /* '2' 8 */
                                "₂\x00"
                                "\x05" /* '₂' 5 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "ď\x00"
                                "\x04" /* 'ď' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Ď\x00"
                                "\x04" /* 'Ď' 4 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "₊\x00"
                                "\x05" /* '₊' 5 */
                                "6\x00"
                                "\x08" /* '6' 8 */
                                "₆\x00"
                                "\x05" /* '₆' 5 */
                                "dead_abovedot\x00"
                                "\x1f" /* 'dead_abovedot' 31 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "4\x00"
                                "\x08" /* '4' 8 */
                                "₄\x00"
                                "\x05" /* '₄' 5 */
                                "v\x00"
                                "\x07" /* 'v' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǒ\x00"
                                "\x04" /* 'Ǒ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ř\x00"
                                "\x04" /* 'ř' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "š\x00"
                                "\x04" /* 'š' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ž\x00"
                                "\x04" /* 'Ž' 4 */
                                "EZH\x00"
                                "\x09" /* 'EZH' 9 */
                                "Ǯ\x00"
                                "\x04" /* 'Ǯ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǎ\x00"
                                "\x04" /* 'Ǎ' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ř\x00"
                                "\x04" /* 'Ř' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "č\x00"
                                "\x04" /* 'č' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ľ\x00"
                                "\x04" /* 'Ľ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ť\x00"
                                "\x04" /* 'Ť' 4 */
                                "5\x00"
                                "\x08" /* '5' 8 */
                                "₅\x00"
                                "\x05" /* '₅' 5 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ǩ\x00"
                                "\x04" /* 'Ǩ' 4 */
                                "9\x00"
                                "\x08" /* '9' 8 */
                                "₉\x00"
                                "\x05" /* '₉' 5 */
                                "0\x00"
                                "\x08" /* '0' 8 */
                                "₀\x00"
                                "\x05" /* '₀' 5 */
                                "Sabovedot\x00"
                                "\x10" /* 'Sabovedot' 16 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "dead_diaeresis\x00"
                                "\x1e" /* 'dead_diaeresis' 30 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "parenleft\x00"
                                "\x10" /* 'parenleft' 16 */
                                "₍\x00"
                                "\x05" /* '₍' 5 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ȟ\x00"
                                "\x04" /* 'ȟ' 4 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ǐ\x00"
                                "\x04" /* 'Ǐ' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ň\x00"
                                "\x04" /* 'Ň' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǔ\x00"
                                "\x04" /* 'Ǔ' 4 */
                                "dead_tilde\x00"
                                "\x87\x32" /* 'dead_tilde' 1842 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ã\x00"
                                "\x04" /* 'ã' 4 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῖ\x00"
                                "\x05" /* 'ῖ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "≲\x00"
                                "\x05" /* '≲' 5 */
                                "Oacute\x00"
                                "\x0d" /* 'Oacute' 13 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẽ\x00"
                                "\x05" /* 'ẽ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "õ\x00"
                                "\x04" /* 'õ' 4 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῦ\x00"
                                "\x05" /* 'ῦ' 5 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "dead_macron\x00"
                                "\x1b" /* 'dead_macron' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỹ\x00"
                                "\x05" /* 'ỹ' 5 */
                                "Multi_key\x00"
                                "\x82\x2d" /* 'Multi_key' 557 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "parenright\x00"
                                "\x80\xab" /* 'parenright' 171 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἶ\x00"
                                "\x05" /* 'Ἶ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἶ\x00"
                                "\x05" /* 'ἶ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὖ\x00"
                                "\x05" /* 'ὖ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἆ\x00"
                                "\x05" /* 'Ἆ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἦ\x00"
                                "\x05" /* 'ἦ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἆ\x00"
                                "\x05" /* 'ἆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἦ\x00"
                                "\x05" /* 'Ἦ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὦ\x00"
                                "\x05" /* 'ὦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὦ\x00"
                                "\x05" /* 'Ὦ' 5 */
                                "quotedbl\x00"
                                "\x2f" /* 'quotedbl' 47 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ữ\x00"
                                "\x05" /* 'ữ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "parenleft\x00"
                                "\x80\xbe" /* 'parenleft' 190 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἷ\x00"
                                "\x05" /* 'Ἷ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἷ\x00"
                                "\x05" /* 'ἷ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὗ\x00"
                                "\x05" /* 'ὗ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἇ\x00"
                                "\x05" /* 'Ἇ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἧ\x00"
                                "\x05" /* 'ἧ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἇ\x00"
                                "\x05" /* 'ἇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἧ\x00"
                                "\x05" /* 'Ἧ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὧ\x00"
                                "\x05" /* 'ὧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὧ\x00"
                                "\x05" /* 'Ὧ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὗ\x00"
                                "\x05" /* 'Ὗ' 5 */
                                "U\x00"
                                "\x13" /* 'U' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẫ\x00"
                                "\x05" /* 'ẫ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ễ\x00"
                                "\x05" /* 'Ễ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẫ\x00"
                                "\x05" /* 'Ẫ' 5 */
                                "oacute\x00"
                                "\x0d" /* 'oacute' 13 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ĩ\x00"
                                "\x04" /* 'ĩ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ñ\x00"
                                "\x04" /* 'ñ' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "≃\x00"
                                "\x05" /* '≃' 5 */
                                "dead_tilde\x00"
                                "\x0f" /* 'dead_tilde' 15 */
                                "~\x00"
                                "\x03" /* '~' 3 */
                                "Uacute\x00"
                                "\x0d" /* 'Uacute' 13 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ỡ\x00"
                                "\x05" /* 'Ỡ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ỡ\x00"
                                "\x05" /* 'ỡ' 5 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̃\x00"
                                "\x04" /* '̃' 4 */
                                "V\x00"
                                "\x08" /* 'V' 8 */
                                "Ṽ\x00"
                                "\x05" /* 'Ṽ' 5 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ỗ\x00"
                                "\x05" /* 'Ỗ' 5 */
                                "omacron\x00"
                                "\x0d" /* 'omacron' 13 */
                                "ȭ\x00"
                                "\x04" /* 'ȭ' 4 */
                                "uacute\x00"
                                "\x0d" /* 'uacute' 13 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ỗ\x00"
                                "\x05" /* 'ỗ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ũ\x00"
                                "\x04" /* 'ũ' 4 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẽ\x00"
                                "\x05" /* 'Ẽ' 5 */
                                "Greek_iotadieresis\x00"
                                "\x19" /* 'Greek_iotadieresis' 25 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỹ\x00"
                                "\x05" /* 'Ỹ' 5 */
                                "dead_dasia\x00"
                                "\x80\xbf" /* 'dead_dasia' 191 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἷ\x00"
                                "\x05" /* 'Ἷ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἷ\x00"
                                "\x05" /* 'ἷ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὗ\x00"
                                "\x05" /* 'ὗ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἇ\x00"
                                "\x05" /* 'Ἇ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἧ\x00"
                                "\x05" /* 'ἧ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἇ\x00"
                                "\x05" /* 'ἇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἧ\x00"
                                "\x05" /* 'Ἧ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὧ\x00"
                                "\x05" /* 'ὧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὧ\x00"
                                "\x05" /* 'Ὧ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὗ\x00"
                                "\x05" /* 'Ὗ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1c" /* 'Greek_upsilondieresis' 28 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "odiaeresis\x00"
                                "\x11" /* 'odiaeresis' 17 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῆ\x00"
                                "\x05" /* 'ῆ' 5 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ẵ\x00"
                                "\x05" /* 'Ẵ' 5 */
                                "dead_psili\x00"
                                "\x80\xab" /* 'dead_psili' 171 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἶ\x00"
                                "\x05" /* 'Ἶ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἶ\x00"
                                "\x05" /* 'ἶ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὖ\x00"
                                "\x05" /* 'ὖ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἆ\x00"
                                "\x05" /* 'Ἆ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἦ\x00"
                                "\x05" /* 'ἦ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἆ\x00"
                                "\x05" /* 'ἆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἦ\x00"
                                "\x05" /* 'Ἦ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὦ\x00"
                                "\x05" /* 'ὦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὦ\x00"
                                "\x05" /* 'Ὦ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾶ\x00"
                                "\x05" /* 'ᾶ' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ễ\x00"
                                "\x05" /* 'ễ' 5 */
                                "v\x00"
                                "\x08" /* 'v' 8 */
                                "ṽ\x00"
                                "\x05" /* 'ṽ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Õ\x00"
                                "\x04" /* 'Õ' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ẵ\x00"
                                "\x05" /* 'ẵ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ã\x00"
                                "\x04" /* 'Ã' 4 */
                                "Odiaeresis\x00"
                                "\x11" /* 'Odiaeresis' 17 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "≳\x00"
                                "\x05" /* '≳' 5 */
                                "Omacron\x00"
                                "\x0d" /* 'Omacron' 13 */
                                "Ȭ\x00"
                                "\x04" /* 'Ȭ' 4 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῶ\x00"
                                "\x05" /* 'ῶ' 5 */
                                "dead_diaeresis\x00"
                                "\x45" /* 'dead_diaeresis' 69 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ῗ\x00"
                                "\x05" /* 'ῗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ῧ\x00"
                                "\x05" /* 'ῧ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ữ\x00"
                                "\x05" /* 'Ữ' 5 */
                                "dead_acute\x00"
                                "\x2c" /* 'dead_acute' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ĩ\x00"
                                "\x04" /* 'Ĩ' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ñ\x00"
                                "\x04" /* 'Ñ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ũ\x00"
                                "\x04" /* 'Ũ' 4 */
                                "dead_belowcomma\x00"
                                "\x5d" /* 'dead_belowcomma' 93 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ț\x00"
                                "\x04" /* 'ț' 4 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                ",\x00"
                                "\x03" /* ',' 3 */
                                "dead_belowcomma\x00"
                                "\x14" /* 'dead_belowcomma' 20 */
                                ",\x00"
                                "\x03" /* ',' 3 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̦\x00"
                                "\x04" /* '̦' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ș\x00"
                                "\x04" /* 'Ș' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ș\x00"
                                "\x04" /* 'ș' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ț\x00"
                                "\x04" /* 'Ț' 4 */
                                "dead_doubleacute\x00"
                                "\x80\x82" /* 'dead_doubleacute' 130 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ő\x00"
                                "\x04" /* 'ő' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "˝\x00"
                                "\x04" /* '˝' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӳ\x00"
                                "\x04" /* 'Ӳ' 4 */
                                "dead_doubleacute\x00"
                                "\x16" /* 'dead_doubleacute' 22 */
                                "˝\x00"
                                "\x04" /* '˝' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̋\x00"
                                "\x04" /* '̋' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ű\x00"
                                "\x04" /* 'ű' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ő\x00"
                                "\x04" /* 'Ő' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ű\x00"
                                "\x04" /* 'Ű' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӳ\x00"
                                "\x04" /* 'ӳ' 4 */
                                "dead_abovering\x00"
                                "\x80\xa0" /* 'dead_abovering' 160 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "å\x00"
                                "\x04" /* 'å' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "°\x00"
                                "\x04" /* '°' 4 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ẙ\x00"
                                "\x05" /* 'ẙ' 5 */
                                "dead_abovering\x00"
                                "\x14" /* 'dead_abovering' 20 */
                                "°\x00"
                                "\x04" /* '°' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̊\x00"
                                "\x04" /* '̊' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ů\x00"
                                "\x04" /* 'ů' 4 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẘ\x00"
                                "\x05" /* 'ẘ' 5 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Å\x00"
                                "\x04" /* 'Å' 4 */
                                "Aacute\x00"
                                "\x0c" /* 'Aacute' 12 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "aacute\x00"
                                "\x0c" /* 'aacute' 12 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "dead_acute\x00"
                                "\x1a" /* 'dead_acute' 26 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ů\x00"
                                "\x04" /* 'Ů' 4 */
                                "Greek_accentdieresis\x00"
                                "\x39" /* 'Greek_accentdieresis' 57 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "dead_voiced_sound\x00"
                                "\x81\x58" /* 'dead_voiced_sound' 344 */
                                "kana_KE\x00"
                                "\x0e" /* 'kana_KE' 14 */
                                "ゲ\x00"
                                "\x05" /* 'ゲ' 5 */
                                "kana_SA\x00"
                                "\x0e" /* 'kana_SA' 14 */
                                "ザ\x00"
                                "\x05" /* 'ザ' 5 */
                                "kana_SE\x00"
                                "\x0e" /* 'kana_SE' 14 */
                                "ゼ\x00"
                                "\x05" /* 'ゼ' 5 */
                                "kana_SU\x00"
                                "\x0e" /* 'kana_SU' 14 */
                                "ズ\x00"
                                "\x05" /* 'ズ' 5 */
                                "kana_WO\x00"
                                "\x0e" /* 'kana_WO' 14 */
                                "ヺ\x00"
                                "\x05" /* 'ヺ' 5 */
                                "kana_TE\x00"
                                "\x0e" /* 'kana_TE' 14 */
                                "デ\x00"
                                "\x05" /* 'デ' 5 */
                                "kana_HO\x00"
                                "\x0e" /* 'kana_HO' 14 */
                                "ボ\x00"
                                "\x05" /* 'ボ' 5 */
                                "kana_HI\x00"
                                "\x0e" /* 'kana_HI' 14 */
                                "ビ\x00"
                                "\x05" /* 'ビ' 5 */
                                "kana_WA\x00"
                                "\x0e" /* 'kana_WA' 14 */
                                "ヷ\x00"
                                "\x05" /* 'ヷ' 5 */
                                "kana_CHI\x00"
                                "\x0f" /* 'kana_CHI' 15 */
                                "ヂ\x00"
                                "\x05" /* 'ヂ' 5 */
                                "kana_HA\x00"
                                "\x0e" /* 'kana_HA' 14 */
                                "バ\x00"
                                "\x05" /* 'バ' 5 */
                                "kana_HE\x00"
                                "\x0e" /* 'kana_HE' 14 */
                                "ベ\x00"
                                "\x05" /* 'ベ' 5 */
                                "kana_KO\x00"
                                "\x0e" /* 'kana_KO' 14 */
                                "ゴ\x00"
                                "\x05" /* 'ゴ' 5 */
                                "kana_FU\x00"
                                "\x0e" /* 'kana_FU' 14 */
                                "ブ\x00"
                                "\x05" /* 'ブ' 5 */
                                "kana_KU\x00"
                                "\x0e" /* 'kana_KU' 14 */
                                "グ\x00"
                                "\x05" /* 'グ' 5 */
                                "kana_U\x00"
                                "\x0d" /* 'kana_U' 13 */
                                "ヴ\x00"
                                "\x05" /* 'ヴ' 5 */
                                "kana_TO\x00"
                                "\x0e" /* 'kana_TO' 14 */
                                "ド\x00"
                                "\x05" /* 'ド' 5 */
                                "kana_TA\x00"
                                "\x0e" /* 'kana_TA' 14 */
                                "ダ\x00"
                                "\x05" /* 'ダ' 5 */
                                "kana_KA\x00"
                                "\x0e" /* 'kana_KA' 14 */
                                "ガ\x00"
                                "\x05" /* 'ガ' 5 */
                                "kana_KI\x00"
                                "\x0e" /* 'kana_KI' 14 */
                                "ギ\x00"
                                "\x05" /* 'ギ' 5 */
                                "kana_SO\x00"
                                "\x0e" /* 'kana_SO' 14 */
                                "ゾ\x00"
                                "\x05" /* 'ゾ' 5 */
                                "kana_TSU\x00"
                                "\x0f" /* 'kana_TSU' 15 */
                                "ヅ\x00"
                                "\x05" /* 'ヅ' 5 */
                                "kana_SHI\x00"
                                "\x0f" /* 'kana_SHI' 15 */
                                "ジ\x00"
                                "\x05" /* 'ジ' 5 */
                                "dead_belowtilde\x00"
                                "\x4c" /* 'dead_belowtilde' 76 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḛ\x00"
                                "\x05" /* 'ḛ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḭ\x00"
                                "\x05" /* 'ḭ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṵ\x00"
                                "\x05" /* 'ṵ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḛ\x00"
                                "\x05" /* 'Ḛ' 5 */
                                "plus\x00"
                                "\x0b" /* 'plus' 11 */
                                "⨦\x00"
                                "\x05" /* '⨦' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḭ\x00"
                                "\x05" /* 'Ḭ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṵ\x00"
                                "\x05" /* 'Ṵ' 5 */
                                "dead_ogonek\x00"
                                "\x80\xb7" /* 'dead_ogonek' 183 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ą\x00"
                                "\x04" /* 'ą' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ę\x00"
                                "\x04" /* 'ę' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǫ\x00"
                                "\x04" /* 'ǫ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "˛\x00"
                                "\x04" /* '˛' 4 */
                                "dead_macron\x00"
                                "\x1b" /* 'dead_macron' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "į\x00"
                                "\x04" /* 'į' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̨\x00"
                                "\x04" /* '̨' 4 */
                                "omacron\x00"
                                "\x0d" /* 'omacron' 13 */
                                "ǭ\x00"
                                "\x04" /* 'ǭ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ų\x00"
                                "\x04" /* 'ų' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ę\x00"
                                "\x04" /* 'Ę' 4 */
                                "dead_ogonek\x00"
                                "\x11" /* 'dead_ogonek' 17 */
                                "˛\x00"
                                "\x04" /* '˛' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǫ\x00"
                                "\x04" /* 'Ǫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ą\x00"
                                "\x04" /* 'Ą' 4 */
                                "Omacron\x00"
                                "\x0d" /* 'Omacron' 13 */
                                "Ǭ\x00"
                                "\x04" /* 'Ǭ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Į\x00"
                                "\x04" /* 'Į' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ų\x00"
                                "\x04" /* 'Ų' 4 */
                                "dead_dasia\x00"
                                "\x81\x2f" /* 'dead_dasia' 303 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἱ\x00"
                                "\x05" /* 'Ἱ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἱ\x00"
                                "\x05" /* 'ἱ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὁ\x00"
                                "\x05" /* 'Ὁ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὑ\x00"
                                "\x05" /* 'ὑ' 5 */
                                "Greek_RHO\x00"
                                "\x10" /* 'Greek_RHO' 16 */
                                "Ῥ\x00"
                                "\x05" /* 'Ῥ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἑ\x00"
                                "\x05" /* 'ἑ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἁ\x00"
                                "\x05" /* 'Ἁ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὁ\x00"
                                "\x05" /* 'ὁ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἡ\x00"
                                "\x05" /* 'ἡ' 5 */
                                "Greek_rho\x00"
                                "\x10" /* 'Greek_rho' 16 */
                                "ῥ\x00"
                                "\x05" /* 'ῥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἁ\x00"
                                "\x05" /* 'ἁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἡ\x00"
                                "\x05" /* 'Ἡ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἑ\x00"
                                "\x05" /* 'Ἑ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὡ\x00"
                                "\x05" /* 'ὡ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὡ\x00"
                                "\x05" /* 'Ὡ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὑ\x00"
                                "\x05" /* 'Ὑ' 5 */
                                "dead_iota\x00"
                                "\x91\x39" /* 'dead_iota' 4409 */
                                "dead_grave\x00"
                                "\x82\x1c" /* 'dead_grave' 540 */
                                "Multi_key\x00"
                                "\x80\xf3" /* 'Multi_key' 243 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῂ\x00"
                                "\x05" /* 'ῂ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾲ\x00"
                                "\x05" /* 'ᾲ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῲ\x00"
                                "\x05" /* 'ῲ' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "ͺ\x00"
                                "\x04" /* 'ͺ' 4 */
                                "Multi_key\x00"
                                "\x89\x29" /* 'Multi_key' 2345 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾈ\x00"
                                "\x05" /* 'ᾈ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾐ\x00"
                                "\x05" /* 'ᾐ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾀ\x00"
                                "\x05" /* 'ᾀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾘ\x00"
                                "\x05" /* 'ᾘ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾠ\x00"
                                "\x05" /* 'ᾠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾨ\x00"
                                "\x05" /* 'ᾨ' 5 */
                                "acute\x00"
                                "\x82\x0b" /* 'acute' 523 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "apostrophe\x00"
                                "\x82\x10" /* 'apostrophe' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "asciitilde\x00"
                                "\x82\x10" /* 'asciitilde' 528 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῇ\x00"
                                "\x05" /* 'ῇ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾷ\x00"
                                "\x05" /* 'ᾷ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῷ\x00"
                                "\x05" /* 'ῷ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾉ\x00"
                                "\x05" /* 'ᾉ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾑ\x00"
                                "\x05" /* 'ᾑ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾁ\x00"
                                "\x05" /* 'ᾁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾙ\x00"
                                "\x05" /* 'ᾙ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾡ\x00"
                                "\x05" /* 'ᾡ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾩ\x00"
                                "\x05" /* 'ᾩ' 5 */
                                "grave\x00"
                                "\x82\x0b" /* 'grave' 523 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῂ\x00"
                                "\x05" /* 'ῂ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾊ\x00"
                                "\x05" /* 'ᾊ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾒ\x00"
                                "\x05" /* 'ᾒ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾂ\x00"
                                "\x05" /* 'ᾂ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾚ\x00"
                                "\x05" /* 'ᾚ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾢ\x00"
                                "\x05" /* 'ᾢ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾪ\x00"
                                "\x05" /* 'ᾪ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾲ\x00"
                                "\x05" /* 'ᾲ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῲ\x00"
                                "\x05" /* 'ῲ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾋ\x00"
                                "\x05" /* 'ᾋ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾓ\x00"
                                "\x05" /* 'ᾓ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾃ\x00"
                                "\x05" /* 'ᾃ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾛ\x00"
                                "\x05" /* 'ᾛ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾣ\x00"
                                "\x05" /* 'ᾣ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾫ\x00"
                                "\x05" /* 'ᾫ' 5 */
                                "dead_tilde\x00"
                                "\x82\x1c" /* 'dead_tilde' 540 */
                                "Multi_key\x00"
                                "\x80\xf3" /* 'Multi_key' 243 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾏ\x00"
                                "\x05" /* 'ᾏ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾗ\x00"
                                "\x05" /* 'ᾗ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾇ\x00"
                                "\x05" /* 'ᾇ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾟ\x00"
                                "\x05" /* 'ᾟ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾧ\x00"
                                "\x05" /* 'ᾧ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾯ\x00"
                                "\x05" /* 'ᾯ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῇ\x00"
                                "\x05" /* 'ῇ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾎ\x00"
                                "\x05" /* 'ᾎ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾖ\x00"
                                "\x05" /* 'ᾖ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾆ\x00"
                                "\x05" /* 'ᾆ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾞ\x00"
                                "\x05" /* 'ᾞ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾦ\x00"
                                "\x05" /* 'ᾦ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾮ\x00"
                                "\x05" /* 'ᾮ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾷ\x00"
                                "\x05" /* 'ᾷ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῷ\x00"
                                "\x05" /* 'ῷ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾼ\x00"
                                "\x05" /* 'ᾼ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾉ\x00"
                                "\x05" /* 'ᾉ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾑ\x00"
                                "\x05" /* 'ᾑ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾁ\x00"
                                "\x05" /* 'ᾁ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾙ\x00"
                                "\x05" /* 'ᾙ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾡ\x00"
                                "\x05" /* 'ᾡ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾩ\x00"
                                "\x05" /* 'ᾩ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῃ\x00"
                                "\x05" /* 'ῃ' 5 */
                                "dead_iota\x00"
                                "\x0f" /* 'dead_iota' 15 */
                                "ͺ\x00"
                                "\x04" /* 'ͺ' 4 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾈ\x00"
                                "\x05" /* 'ᾈ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾐ\x00"
                                "\x05" /* 'ᾐ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾀ\x00"
                                "\x05" /* 'ᾀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾘ\x00"
                                "\x05" /* 'ᾘ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾠ\x00"
                                "\x05" /* 'ᾠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾨ\x00"
                                "\x05" /* 'ᾨ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾳ\x00"
                                "\x05" /* 'ᾳ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ῌ\x00"
                                "\x05" /* 'ῌ' 5 */
                                "Greek_omegaaccent\x00"
                                "\x18" /* 'Greek_omegaaccent' 24 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῳ\x00"
                                "\x05" /* 'ῳ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ῼ\x00"
                                "\x05" /* 'ῼ' 5 */
                                "dead_acute\x00"
                                "\x82\x1c" /* 'dead_acute' 540 */
                                "Multi_key\x00"
                                "\x80\xf3" /* 'Multi_key' 243 */
                                "parenright\x00"
                                "\x74" /* 'parenright' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "parenleft\x00"
                                "\x73" /* 'parenleft' 115 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "dead_dasia\x00"
                                "\x74" /* 'dead_dasia' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾍ\x00"
                                "\x05" /* 'ᾍ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾕ\x00"
                                "\x05" /* 'ᾕ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾅ\x00"
                                "\x05" /* 'ᾅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾝ\x00"
                                "\x05" /* 'ᾝ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾥ\x00"
                                "\x05" /* 'ᾥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾭ\x00"
                                "\x05" /* 'ᾭ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_psili\x00"
                                "\x74" /* 'dead_psili' 116 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "ᾌ\x00"
                                "\x05" /* 'ᾌ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ᾔ\x00"
                                "\x05" /* 'ᾔ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾄ\x00"
                                "\x05" /* 'ᾄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "ᾜ\x00"
                                "\x05" /* 'ᾜ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ᾤ\x00"
                                "\x05" /* 'ᾤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "ᾬ\x00"
                                "\x05" /* 'ᾬ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ῴ\x00"
                                "\x05" /* 'ῴ' 5 */
                                "Greek_alphaaccent\x00"
                                "\x18" /* 'Greek_alphaaccent' 24 */
                                "ᾴ\x00"
                                "\x05" /* 'ᾴ' 5 */
                                "Greek_etaaccent\x00"
                                "\x16" /* 'Greek_etaaccent' 22 */
                                "ῄ\x00"
                                "\x05" /* 'ῄ' 5 */
                                "dead_greek\x00"
                                "\x81\xed" /* 'dead_greek' 493 */
                                "W\x00"
                                "\x07" /* 'W' 7 */
                                "Ω\x00"
                                "\x04" /* 'Ω' 4 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "γ\x00"
                                "\x04" /* 'γ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "α\x00"
                                "\x04" /* 'α' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ε\x00"
                                "\x04" /* 'ε' 4 */
                                "F\x00"
                                "\x07" /* 'F' 7 */
                                "Φ\x00"
                                "\x04" /* 'Φ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ο\x00"
                                "\x04" /* 'ο' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "λ\x00"
                                "\x04" /* 'λ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "τ\x00"
                                "\x04" /* 'τ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "dead_macron\x00"
                                "\x3d" /* 'dead_macron' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ᾱ\x00"
                                "\x05" /* 'ᾱ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ῑ\x00"
                                "\x05" /* 'ῑ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ῡ\x00"
                                "\x05" /* 'ῡ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ᾱ\x00"
                                "\x05" /* 'Ᾱ' 5 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ῑ\x00"
                                "\x05" /* 'Ῑ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ῡ\x00"
                                "\x05" /* 'Ῡ' 5 */
                                "Q\x00"
                                "\x07" /* 'Q' 7 */
                                "Χ\x00"
                                "\x04" /* 'Χ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ψ\x00"
                                "\x04" /* 'ψ' 4 */
                                "b\x00"
                                "\x07" /* 'b' 7 */
                                "β\x00"
                                "\x04" /* 'β' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ι\x00"
                                "\x04" /* 'ι' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "κ\x00"
                                "\x04" /* 'κ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ν\x00"
                                "\x04" /* 'ν' 4 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "θ\x00"
                                "\x04" /* 'θ' 4 */
                                "x\x00"
                                "\x07" /* 'x' 7 */
                                "ξ\x00"
                                "\x04" /* 'ξ' 4 */
                                "q\x00"
                                "\x07" /* 'q' 7 */
                                "χ\x00"
                                "\x04" /* 'χ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "υ\x00"
                                "\x04" /* 'υ' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ζ\x00"
                                "\x04" /* 'ζ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Γ\x00"
                                "\x04" /* 'Γ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Η\x00"
                                "\x04" /* 'Η' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ε\x00"
                                "\x04" /* 'Ε' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Σ\x00"
                                "\x04" /* 'Σ' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ψ\x00"
                                "\x04" /* 'Ψ' 4 */
                                "f\x00"
                                "\x07" /* 'f' 7 */
                                "φ\x00"
                                "\x04" /* 'φ' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "δ\x00"
                                "\x04" /* 'δ' 4 */
                                "dead_greek\x00"
                                "\x10" /* 'dead_greek' 16 */
                                "µ\x00"
                                "\x04" /* 'µ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Δ\x00"
                                "\x04" /* 'Δ' 4 */
                                "w\x00"
                                "\x07" /* 'w' 7 */
                                "ω\x00"
                                "\x04" /* 'ω' 4 */
                                "p\x00"
                                "\x07" /* 'p' 7 */
                                "π\x00"
                                "\x04" /* 'π' 4 */
                                "J\x00"
                                "\x07" /* 'J' 7 */
                                "Θ\x00"
                                "\x04" /* 'Θ' 4 */
                                "P\x00"
                                "\x07" /* 'P' 7 */
                                "Π\x00"
                                "\x04" /* 'Π' 4 */
                                "M\x00"
                                "\x07" /* 'M' 7 */
                                "Μ\x00"
                                "\x04" /* 'Μ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ο\x00"
                                "\x04" /* 'Ο' 4 */
                                "m\x00"
                                "\x07" /* 'm' 7 */
                                "μ\x00"
                                "\x04" /* 'μ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ρ\x00"
                                "\x04" /* 'ρ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "σ\x00"
                                "\x04" /* 'σ' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ζ\x00"
                                "\x04" /* 'Ζ' 4 */
                                "dead_stroke\x00"
                                "\x14" /* 'dead_stroke' 20 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ϼ\x00"
                                "\x04" /* 'ϼ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Α\x00"
                                "\x04" /* 'Α' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ρ\x00"
                                "\x04" /* 'Ρ' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Λ\x00"
                                "\x04" /* 'Λ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Τ\x00"
                                "\x04" /* 'Τ' 4 */
                                "dead_hook\x00"
                                "\x12" /* 'dead_hook' 18 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "ϒ\x00"
                                "\x04" /* 'ϒ' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Κ\x00"
                                "\x04" /* 'Κ' 4 */
                                "B\x00"
                                "\x07" /* 'B' 7 */
                                "Β\x00"
                                "\x04" /* 'Β' 4 */
                                "X\x00"
                                "\x07" /* 'X' 7 */
                                "Ξ\x00"
                                "\x04" /* 'Ξ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "η\x00"
                                "\x04" /* 'η' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ι\x00"
                                "\x04" /* 'Ι' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ν\x00"
                                "\x04" /* 'Ν' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Υ\x00"
                                "\x04" /* 'Υ' 4 */
                                "dead_invertedbreve\x00"
                                "\x80\xf1" /* 'dead_invertedbreve' 241 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р̑\x00"
                                "\x06" /* 'р̑' 6 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И̑\x00"
                                "\x06" /* 'И̑' 6 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О̑\x00"
                                "\x06" /* 'О̑' 6 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а̑\x00"
                                "\x06" /* 'а̑' 6 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р̑\x00"
                                "\x06" /* 'Р̑' 6 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У̑\x00"
                                "\x06" /* 'У̑' 6 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е̑\x00"
                                "\x06" /* 'е̑' 6 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и̑\x00"
                                "\x06" /* 'и̑' 6 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о̑\x00"
                                "\x06" /* 'о̑' 6 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А̑\x00"
                                "\x06" /* 'А̑' 6 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е̑\x00"
                                "\x06" /* 'Е̑' 6 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у̑\x00"
                                "\x06" /* 'у̑' 6 */
                                "dead_psili\x00"
                                "\x81\x0b" /* 'dead_psili' 267 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἰ\x00"
                                "\x05" /* 'Ἰ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἰ\x00"
                                "\x05" /* 'ἰ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὀ\x00"
                                "\x05" /* 'Ὀ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὐ\x00"
                                "\x05" /* 'ὐ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἐ\x00"
                                "\x05" /* 'ἐ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἀ\x00"
                                "\x05" /* 'Ἀ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὀ\x00"
                                "\x05" /* 'ὀ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἠ\x00"
                                "\x05" /* 'ἠ' 5 */
                                "Greek_rho\x00"
                                "\x10" /* 'Greek_rho' 16 */
                                "ῤ\x00"
                                "\x05" /* 'ῤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἀ\x00"
                                "\x05" /* 'ἀ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἠ\x00"
                                "\x05" /* 'Ἠ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἐ\x00"
                                "\x05" /* 'Ἐ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὠ\x00"
                                "\x05" /* 'ὠ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὠ\x00"
                                "\x05" /* 'Ὠ' 5 */
                                "dead_abovedot\x00"
                                "\x82\xf7" /* 'dead_abovedot' 759 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẇ\x00"
                                "\x05" /* 'Ẇ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ġ\x00"
                                "\x04" /* 'ġ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ȧ\x00"
                                "\x04" /* 'ȧ' 4 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ċ\x00"
                                "\x04" /* 'Ċ' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ė\x00"
                                "\x04" /* 'ė' 4 */
                                "F\x00"
                                "\x08" /* 'F' 8 */
                                "Ḟ\x00"
                                "\x05" /* 'Ḟ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȯ\x00"
                                "\x04" /* 'ȯ' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ŀ\x00"
                                "\x04" /* 'ŀ' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṫ\x00"
                                "\x05" /* 'ṫ' 5 */
                                "dead_belowdot\x00"
                                "\x1f" /* 'dead_belowdot' 31 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "˙\x00"
                                "\x04" /* '˙' 4 */
                                "dead_macron\x00"
                                "\x29" /* 'dead_macron' 41 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ẏ\x00"
                                "\x05" /* 'ẏ' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "ḃ\x00"
                                "\x05" /* 'ḃ' 5 */
                                "Multi_key\x00"
                                "\x74" /* 'Multi_key' 116 */
                                "exclam\x00"
                                "\x18" /* 'exclam' 24 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṩ\x00"
                                "\x05" /* 'Ṩ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṩ\x00"
                                "\x05" /* 'ṩ' 5 */
                                "f\x00"
                                "\x0b" /* 'f' 11 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ẛ\x00"
                                "\x05" /* 'ẛ' 5 */
                                "acute\x00"
                                "\x17" /* 'acute' 23 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "apostrophe\x00"
                                "\x1c" /* 'apostrophe' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "c\x00"
                                "\x13" /* 'c' 19 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ı\x00"
                                "\x04" /* 'ı' 4 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṅ\x00"
                                "\x05" /* 'ṅ' 5 */
                                "dead_caron\x00"
                                "\x1c" /* 'dead_caron' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ȷ\x00"
                                "\x04" /* 'ȷ' 4 */
                                "x\x00"
                                "\x08" /* 'x' 8 */
                                "ẋ\x00"
                                "\x05" /* 'ẋ' 5 */
                                "amacron\x00"
                                "\x0d" /* 'amacron' 13 */
                                "ǡ\x00"
                                "\x04" /* 'ǡ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̇\x00"
                                "\x04" /* '̇' 4 */
                                "omacron\x00"
                                "\x0d" /* 'omacron' 13 */
                                "ȱ\x00"
                                "\x04" /* 'ȱ' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ż\x00"
                                "\x04" /* 'ż' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ġ\x00"
                                "\x04" /* 'Ġ' 4 */
                                "Sacute\x00"
                                "\x0d" /* 'Sacute' 13 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḣ\x00"
                                "\x05" /* 'Ḣ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ė\x00"
                                "\x04" /* 'Ė' 4 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṡ\x00"
                                "\x05" /* 'Ṡ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ẏ\x00"
                                "\x05" /* 'Ẏ' 5 */
                                "scaron\x00"
                                "\x0d" /* 'scaron' 13 */
                                "ṧ\x00"
                                "\x05" /* 'ṧ' 5 */
                                "f\x00"
                                "\x08" /* 'f' 8 */
                                "ḟ\x00"
                                "\x05" /* 'ḟ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḋ\x00"
                                "\x05" /* 'ḋ' 5 */
                                "Scaron\x00"
                                "\x0d" /* 'Scaron' 13 */
                                "Ṧ\x00"
                                "\x05" /* 'Ṧ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḋ\x00"
                                "\x05" /* 'Ḋ' 5 */
                                "dead_abovedot\x00"
                                "\x13" /* 'dead_abovedot' 19 */
                                "˙\x00"
                                "\x04" /* '˙' 4 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẇ\x00"
                                "\x05" /* 'ẇ' 5 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ṗ\x00"
                                "\x05" /* 'ṗ' 5 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ṗ\x00"
                                "\x05" /* 'Ṗ' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ṁ\x00"
                                "\x05" /* 'Ṁ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȯ\x00"
                                "\x04" /* 'Ȯ' 4 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ṁ\x00"
                                "\x05" /* 'ṁ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṙ\x00"
                                "\x05" /* 'ṙ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṡ\x00"
                                "\x05" /* 'ṡ' 5 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ż\x00"
                                "\x04" /* 'Ż' 4 */
                                "sacute\x00"
                                "\x0d" /* 'sacute' 13 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "dead_stroke\x00"
                                "\x14" /* 'dead_stroke' 20 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ɟ\x00"
                                "\x04" /* 'ɟ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ȧ\x00"
                                "\x04" /* 'Ȧ' 4 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṙ\x00"
                                "\x05" /* 'Ṙ' 5 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ċ\x00"
                                "\x04" /* 'ċ' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ŀ\x00"
                                "\x04" /* 'Ŀ' 4 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṫ\x00"
                                "\x05" /* 'Ṫ' 5 */
                                "Omacron\x00"
                                "\x0d" /* 'Omacron' 13 */
                                "Ȱ\x00"
                                "\x04" /* 'Ȱ' 4 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "Ḃ\x00"
                                "\x05" /* 'Ḃ' 5 */
                                "Amacron\x00"
                                "\x0d" /* 'Amacron' 13 */
                                "Ǡ\x00"
                                "\x04" /* 'Ǡ' 4 */
                                "dead_acute\x00"
                                "\x1c" /* 'dead_acute' 28 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "X\x00"
                                "\x08" /* 'X' 8 */
                                "Ẋ\x00"
                                "\x05" /* 'Ẋ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḣ\x00"
                                "\x05" /* 'ḣ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "İ\x00"
                                "\x04" /* 'İ' 4 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṅ\x00"
                                "\x05" /* 'Ṅ' 5 */
                                "dead_double_grave\x00"
                                "\x67" /* 'dead_double_grave' 103 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ȁ\x00"
                                "\x04" /* 'ȁ' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ȅ\x00"
                                "\x04" /* 'ȅ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȍ\x00"
                                "\x04" /* 'ȍ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ȉ\x00"
                                "\x04" /* 'ȉ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ȕ\x00"
                                "\x04" /* 'ȕ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ȅ\x00"
                                "\x04" /* 'Ȅ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȍ\x00"
                                "\x04" /* 'Ȍ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ȑ\x00"
                                "\x04" /* 'ȑ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ȁ\x00"
                                "\x04" /* 'Ȁ' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ȑ\x00"
                                "\x04" /* 'Ȑ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ȉ\x00"
                                "\x04" /* 'Ȉ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ȕ\x00"
                                "\x04" /* 'Ȕ' 4 */
                                "dead_semivoiced_sound\x00"
                                "\x5d" /* 'dead_semivoiced_sound' 93 */
                                "kana_HO\x00"
                                "\x0e" /* 'kana_HO' 14 */
                                "ポ\x00"
                                "\x05" /* 'ポ' 5 */
                                "kana_HI\x00"
                                "\x0e" /* 'kana_HI' 14 */
                                "ピ\x00"
                                "\x05" /* 'ピ' 5 */
                                "kana_HA\x00"
                                "\x0e" /* 'kana_HA' 14 */
                                "パ\x00"
                                "\x05" /* 'パ' 5 */
                                "kana_HE\x00"
                                "\x0e" /* 'kana_HE' 14 */
                                "ペ\x00"
                                "\x05" /* 'ペ' 5 */
                                "kana_FU\x00"
                                "\x0e" /* 'kana_FU' 14 */
                                "プ\x00"
                                "\x05" /* 'プ' 5 */
                                "dead_stroke\x00"
                                "\x81\xdd" /* 'dead_stroke' 477 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǥ\x00"
                                "\x04" /* 'ǥ' 4 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ⱥ\x00"
                                "\x05" /* 'ⱥ' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ȼ\x00"
                                "\x04" /* 'Ȼ' 4 */
                                "less\x00"
                                "\x0b" /* 'less' 11 */
                                "≮\x00"
                                "\x05" /* '≮' 5 */
                                "Oacute\x00"
                                "\x0c" /* 'Oacute' 12 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ɇ\x00"
                                "\x04" /* 'ɇ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ø\x00"
                                "\x04" /* 'ø' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ł\x00"
                                "\x04" /* 'ł' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ŧ\x00"
                                "\x04" /* 'ŧ' 4 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "/\x00"
                                "\x03" /* '/' 3 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ɏ\x00"
                                "\x04" /* 'ɏ' 4 */
                                "b\x00"
                                "\x07" /* 'b' 7 */
                                "ƀ\x00"
                                "\x04" /* 'ƀ' 4 */
                                "oacute\x00"
                                "\x0c" /* 'oacute' 12 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ɨ\x00"
                                "\x04" /* 'ɨ' 4 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "≠\x00"
                                "\x05" /* '≠' 5 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ɉ\x00"
                                "\x04" /* 'ɉ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̸\x00"
                                "\x04" /* '̸' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ʉ\x00"
                                "\x04" /* 'ʉ' 4 */
                                "greaterthanequal\x00"
                                "\x17" /* 'greaterthanequal' 23 */
                                "≱\x00"
                                "\x05" /* '≱' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ƶ\x00"
                                "\x04" /* 'ƶ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǥ\x00"
                                "\x04" /* 'Ǥ' 4 */
                                "H\x00"
                                "\x07" /* 'H' 7 */
                                "Ħ\x00"
                                "\x04" /* 'Ħ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ɇ\x00"
                                "\x04" /* 'Ɇ' 4 */
                                "2\x00"
                                "\x07" /* '2' 7 */
                                "ƻ\x00"
                                "\x04" /* 'ƻ' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ɏ\x00"
                                "\x04" /* 'Ɏ' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "đ\x00"
                                "\x04" /* 'đ' 4 */
                                "dead_greek\x00"
                                "\x13" /* 'dead_greek' 19 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ϼ\x00"
                                "\x04" /* 'ϼ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Đ\x00"
                                "\x04" /* 'Đ' 4 */
                                "dead_abovedot\x00"
                                "\x16" /* 'dead_abovedot' 22 */
                                "j\x00"
                                "\x07" /* 'j' 7 */
                                "ɟ\x00"
                                "\x04" /* 'ɟ' 4 */
                                "lessthanequal\x00"
                                "\x14" /* 'lessthanequal' 20 */
                                "≰\x00"
                                "\x05" /* '≰' 5 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ᵽ\x00"
                                "\x05" /* 'ᵽ' 5 */
                                "J\x00"
                                "\x07" /* 'J' 7 */
                                "Ɉ\x00"
                                "\x04" /* 'Ɉ' 4 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ᵽ\x00"
                                "\x05" /* 'Ᵽ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ø\x00"
                                "\x04" /* 'Ø' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ɍ\x00"
                                "\x04" /* 'ɍ' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ƶ\x00"
                                "\x04" /* 'Ƶ' 4 */
                                "dead_stroke\x00"
                                "\x10" /* 'dead_stroke' 16 */
                                "/\x00"
                                "\x03" /* '/' 3 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ⱥ\x00"
                                "\x04" /* 'Ⱥ' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ɍ\x00"
                                "\x04" /* 'Ɍ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ȼ\x00"
                                "\x04" /* 'ȼ' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ł\x00"
                                "\x04" /* 'Ł' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ŧ\x00"
                                "\x04" /* 'Ŧ' 4 */
                                "greater\x00"
                                "\x0e" /* 'greater' 14 */
                                "≯\x00"
                                "\x05" /* '≯' 5 */
                                "B\x00"
                                "\x07" /* 'B' 7 */
                                "Ƀ\x00"
                                "\x04" /* 'Ƀ' 4 */
                                "dead_acute\x00"
                                "\x1a" /* 'dead_acute' 26 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ħ\x00"
                                "\x04" /* 'ħ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ɨ\x00"
                                "\x04" /* 'Ɨ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ʉ\x00"
                                "\x04" /* 'Ʉ' 4 */
                                "dead_hook\x00"
                                "\x83\x5e" /* 'dead_hook' 862 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ⱳ\x00"
                                "\x05" /* 'Ⱳ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ɠ\x00"
                                "\x04" /* 'ɠ' 4 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ả\x00"
                                "\x05" /* 'ả' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ƈ\x00"
                                "\x04" /* 'Ƈ' 4 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ẻ\x00"
                                "\x05" /* 'ẻ' 5 */
                                "F\x00"
                                "\x07" /* 'F' 7 */
                                "Ƒ\x00"
                                "\x04" /* 'Ƒ' 4 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ỏ\x00"
                                "\x05" /* 'ỏ' 5 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ƭ\x00"
                                "\x04" /* 'ƭ' 4 */
                                "schwa\x00"
                                "\x0b" /* 'schwa' 11 */
                                "ɚ\x00"
                                "\x04" /* 'ɚ' 4 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "̉\x00"
                                "\x04" /* '̉' 4 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "y\x00"
                                "\x08" /* 'y' 8 */
                                "ỷ\x00"
                                "\x05" /* 'ỷ' 5 */
                                "b\x00"
                                "\x07" /* 'b' 7 */
                                "ɓ\x00"
                                "\x04" /* 'ɓ' 4 */
                                "Multi_key\x00"
                                "\x80\x95" /* 'Multi_key' 149 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ử\x00"
                                "\x05" /* 'ử' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "U\x00"
                                "\x13" /* 'U' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ẩ\x00"
                                "\x05" /* 'ẩ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ể\x00"
                                "\x05" /* 'Ể' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ẩ\x00"
                                "\x05" /* 'Ẩ' 5 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ỉ\x00"
                                "\x05" /* 'ỉ' 5 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ƙ\x00"
                                "\x04" /* 'ƙ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ɲ\x00"
                                "\x04" /* 'ɲ' 4 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ở\x00"
                                "\x05" /* 'Ở' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ở\x00"
                                "\x05" /* 'ở' 5 */
                                "q\x00"
                                "\x07" /* 'q' 7 */
                                "ʠ\x00"
                                "\x04" /* 'ʠ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̉\x00"
                                "\x04" /* '̉' 4 */
                                "V\x00"
                                "\x07" /* 'V' 7 */
                                "Ʋ\x00"
                                "\x04" /* 'Ʋ' 4 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ổ\x00"
                                "\x05" /* 'Ổ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ổ\x00"
                                "\x05" /* 'ổ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ủ\x00"
                                "\x05" /* 'ủ' 5 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ȥ\x00"
                                "\x04" /* 'ȥ' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ɠ\x00"
                                "\x04" /* 'Ɠ' 4 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ẻ\x00"
                                "\x05" /* 'Ẻ' 5 */
                                "Y\x00"
                                "\x08" /* 'Y' 8 */
                                "Ỷ\x00"
                                "\x05" /* 'Ỷ' 5 */
                                "f\x00"
                                "\x07" /* 'f' 7 */
                                "ƒ\x00"
                                "\x04" /* 'ƒ' 4 */
                                "d\x00"
                                "\x07" /* 'd' 7 */
                                "ɗ\x00"
                                "\x04" /* 'ɗ' 4 */
                                "dead_greek\x00"
                                "\x13" /* 'dead_greek' 19 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "ϒ\x00"
                                "\x04" /* 'ϒ' 4 */
                                "D\x00"
                                "\x07" /* 'D' 7 */
                                "Ɗ\x00"
                                "\x04" /* 'Ɗ' 4 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ẳ\x00"
                                "\x05" /* 'Ẳ' 5 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ể\x00"
                                "\x05" /* 'ể' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ⱳ\x00"
                                "\x05" /* 'ⱳ' 5 */
                                "p\x00"
                                "\x07" /* 'p' 7 */
                                "ƥ\x00"
                                "\x04" /* 'ƥ' 4 */
                                "v\x00"
                                "\x07" /* 'v' 7 */
                                "ʋ\x00"
                                "\x04" /* 'ʋ' 4 */
                                "P\x00"
                                "\x07" /* 'P' 7 */
                                "Ƥ\x00"
                                "\x04" /* 'Ƥ' 4 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ɱ\x00"
                                "\x05" /* 'Ɱ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ỏ\x00"
                                "\x05" /* 'Ỏ' 5 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ẳ\x00"
                                "\x05" /* 'ẳ' 5 */
                                "m\x00"
                                "\x07" /* 'm' 7 */
                                "ɱ\x00"
                                "\x04" /* 'ɱ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ɼ\x00"
                                "\x04" /* 'ɼ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ʂ\x00"
                                "\x04" /* 'ʂ' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ȥ\x00"
                                "\x04" /* 'Ȥ' 4 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ả\x00"
                                "\x05" /* 'Ả' 5 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ƈ\x00"
                                "\x04" /* 'ƈ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ƭ\x00"
                                "\x04" /* 'Ƭ' 4 */
                                "dead_hook\x00"
                                "\x0f" /* 'dead_hook' 15 */
                                "̉\x00"
                                "\x04" /* '̉' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ƙ\x00"
                                "\x04" /* 'Ƙ' 4 */
                                "B\x00"
                                "\x07" /* 'B' 7 */
                                "Ɓ\x00"
                                "\x04" /* 'Ɓ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ử\x00"
                                "\x05" /* 'Ử' 5 */
                                "h\x00"
                                "\x07" /* 'h' 7 */
                                "ɦ\x00"
                                "\x04" /* 'ɦ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ỉ\x00"
                                "\x05" /* 'Ỉ' 5 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ɲ\x00"
                                "\x04" /* 'Ɲ' 4 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ủ\x00"
                                "\x05" /* 'Ủ' 5 */
                                "dead_belowbreve\x00"
                                "\x21" /* 'dead_belowbreve' 33 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḫ\x00"
                                "\x05" /* 'Ḫ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḫ\x00"
                                "\x05" /* 'ḫ' 5 */
                                "dead_cedilla\x00"
                                "\x81\x68" /* 'dead_cedilla' 360 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḝ\x00"
                                "\x05" /* 'ḝ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḝ\x00"
                                "\x05" /* 'Ḝ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ģ\x00"
                                "\x04" /* 'ģ' 4 */
                                "dead_currency\x00"
                                "\x1f" /* 'dead_currency' 31 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ç\x00"
                                "\x04" /* 'Ç' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ȩ\x00"
                                "\x04" /* 'ȩ' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ļ\x00"
                                "\x04" /* 'ļ' 4 */
                                "t\x00"
                                "\x07" /* 't' 7 */
                                "ţ\x00"
                                "\x04" /* 'ţ' 4 */
                                "ColonSign\x00"
                                "\x10" /* 'ColonSign' 16 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "¸\x00"
                                "\x04" /* '¸' 4 */
                                "k\x00"
                                "\x07" /* 'k' 7 */
                                "ķ\x00"
                                "\x04" /* 'ķ' 4 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ņ\x00"
                                "\x04" /* 'ņ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̧\x00"
                                "\x04" /* '̧' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ģ\x00"
                                "\x04" /* 'Ģ' 4 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḩ\x00"
                                "\x05" /* 'Ḩ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ȩ\x00"
                                "\x04" /* 'Ȩ' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ş\x00"
                                "\x04" /* 'Ş' 4 */
                                "Cacute\x00"
                                "\x0d" /* 'Cacute' 13 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḑ\x00"
                                "\x05" /* 'ḑ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḑ\x00"
                                "\x05" /* 'Ḑ' 5 */
                                "cent\x00"
                                "\x0b" /* 'cent' 11 */
                                "₵\x00"
                                "\x05" /* '₵' 5 */
                                "cacute\x00"
                                "\x0d" /* 'cacute' 13 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŗ\x00"
                                "\x04" /* 'ŗ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ş\x00"
                                "\x04" /* 'ş' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŗ\x00"
                                "\x04" /* 'Ŗ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ç\x00"
                                "\x04" /* 'ç' 4 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ļ\x00"
                                "\x04" /* 'Ļ' 4 */
                                "T\x00"
                                "\x07" /* 'T' 7 */
                                "Ţ\x00"
                                "\x04" /* 'Ţ' 4 */
                                "K\x00"
                                "\x07" /* 'K' 7 */
                                "Ķ\x00"
                                "\x04" /* 'Ķ' 4 */
                                "dead_cedilla\x00"
                                "\x12" /* 'dead_cedilla' 18 */
                                "¸\x00"
                                "\x04" /* '¸' 4 */
                                "dead_acute\x00"
                                "\x1c" /* 'dead_acute' 28 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḩ\x00"
                                "\x05" /* 'ḩ' 5 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ņ\x00"
                                "\x04" /* 'Ņ' 4 */
                                "dead_inverted_breve\x00"
                                "\x69" /* 'dead_inverted_breve' 105 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ȃ\x00"
                                "\x04" /* 'ȃ' 4 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ȇ\x00"
                                "\x04" /* 'ȇ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȏ\x00"
                                "\x04" /* 'ȏ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ȋ\x00"
                                "\x04" /* 'ȋ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ȗ\x00"
                                "\x04" /* 'ȗ' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ȇ\x00"
                                "\x04" /* 'Ȇ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȏ\x00"
                                "\x04" /* 'Ȏ' 4 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ȓ\x00"
                                "\x04" /* 'ȓ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ȃ\x00"
                                "\x04" /* 'Ȃ' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ȓ\x00"
                                "\x04" /* 'Ȓ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ȋ\x00"
                                "\x04" /* 'Ȋ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ȗ\x00"
                                "\x04" /* 'Ȗ' 4 */
                                "dead_diaeresis\x00"
                                "\x84\xb3" /* 'dead_diaeresis' 1203 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẅ\x00"
                                "\x05" /* 'Ẅ' 5 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ä\x00"
                                "\x04" /* 'ä' 4 */
                                "Greek_IOTA\x00"
                                "\x10" /* 'Greek_IOTA' 16 */
                                "Ϊ\x00"
                                "\x04" /* 'Ϊ' 4 */
                                "dead_grave\x00"
                                "\x1a" /* 'dead_grave' 26 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ϊ\x00"
                                "\x04" /* 'ϊ' 4 */
                                "Umacron\x00"
                                "\x0e" /* 'Umacron' 14 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "Cyrillic_ZE\x00"
                                "\x11" /* 'Cyrillic_ZE' 17 */
                                "Ӟ\x00"
                                "\x04" /* 'Ӟ' 4 */
                                "dead_belowdiaeresis\x00"
                                "\x21" /* 'dead_belowdiaeresis' 33 */
                                "equal\x00"
                                "\x0c" /* 'equal' 12 */
                                "⩷\x00"
                                "\x05" /* '⩷' 5 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "ë\x00"
                                "\x04" /* 'ë' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ö\x00"
                                "\x04" /* 'ö' 4 */
                                "iacute\x00"
                                "\x0d" /* 'iacute' 13 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "Cyrillic_ze\x00"
                                "\x11" /* 'Cyrillic_ze' 17 */
                                "ӟ\x00"
                                "\x04" /* 'ӟ' 4 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ẗ\x00"
                                "\x05" /* 'ẗ' 5 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ϋ\x00"
                                "\x04" /* 'ϋ' 4 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "\"\x00"
                                "\x03" /* '"' 3 */
                                "dead_macron\x00"
                                "\x39" /* 'dead_macron' 57 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "Cyrillic_I\x00"
                                "\x10" /* 'Cyrillic_I' 16 */
                                "Ӥ\x00"
                                "\x04" /* 'Ӥ' 4 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ÿ\x00"
                                "\x04" /* 'ÿ' 4 */
                                "Multi_key\x00"
                                "\x5b" /* 'Multi_key' 91 */
                                "underscore\x00"
                                "\x1c" /* 'underscore' 28 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "macron\x00"
                                "\x18" /* 'macron' 24 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṻ\x00"
                                "\x05" /* 'Ṻ' 5 */
                                "asciitilde\x00"
                                "\x1c" /* 'asciitilde' 28 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Cyrillic_O\x00"
                                "\x10" /* 'Cyrillic_O' 16 */
                                "Ӧ\x00"
                                "\x04" /* 'Ӧ' 4 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "ï\x00"
                                "\x04" /* 'ï' 4 */
                                "Ukrainian_I\x00"
                                "\x11" /* 'Ukrainian_I' 17 */
                                "Ї\x00"
                                "\x04" /* 'Ї' 4 */
                                "dead_caron\x00"
                                "\x1a" /* 'dead_caron' 26 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǚ\x00"
                                "\x04" /* 'ǚ' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǚ\x00"
                                "\x04" /* 'Ǚ' 4 */
                                "dead_tilde\x00"
                                "\x1c" /* 'dead_tilde' 28 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Cyrillic_che\x00"
                                "\x12" /* 'Cyrillic_che' 18 */
                                "ӵ\x00"
                                "\x04" /* 'ӵ' 4 */
                                "Uacute\x00"
                                "\x0c" /* 'Uacute' 12 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Cyrillic_a\x00"
                                "\x10" /* 'Cyrillic_a' 16 */
                                "ӓ\x00"
                                "\x04" /* 'ӓ' 4 */
                                "Ugrave\x00"
                                "\x0c" /* 'Ugrave' 12 */
                                "Ǜ\x00"
                                "\x04" /* 'Ǜ' 4 */
                                "x\x00"
                                "\x08" /* 'x' 8 */
                                "ẍ\x00"
                                "\x05" /* 'ẍ' 5 */
                                "amacron\x00"
                                "\x0d" /* 'amacron' 13 */
                                "ǟ\x00"
                                "\x04" /* 'ǟ' 4 */
                                "Cyrillic_U\x00"
                                "\x10" /* 'Cyrillic_U' 16 */
                                "Ӱ\x00"
                                "\x04" /* 'Ӱ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "̈\x00"
                                "\x04" /* '̈' 4 */
                                "omacron\x00"
                                "\x0d" /* 'omacron' 13 */
                                "ȫ\x00"
                                "\x04" /* 'ȫ' 4 */
                                "uacute\x00"
                                "\x0c" /* 'uacute' 12 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ü\x00"
                                "\x04" /* 'ü' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ṏ\x00"
                                "\x05" /* 'ṏ' 5 */
                                "Iacute\x00"
                                "\x0d" /* 'Iacute' 13 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "H\x00"
                                "\x08" /* 'H' 8 */
                                "Ḧ\x00"
                                "\x05" /* 'Ḧ' 5 */
                                "Cyrillic_YERU\x00"
                                "\x13" /* 'Cyrillic_YERU' 19 */
                                "Ӹ\x00"
                                "\x04" /* 'Ӹ' 4 */
                                "Cyrillic_ie\x00"
                                "\x11" /* 'Cyrillic_ie' 17 */
                                "ё\x00"
                                "\x04" /* 'ё' 4 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "Ë\x00"
                                "\x04" /* 'Ë' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ÿ\x00"
                                "\x04" /* 'Ÿ' 4 */
                                "Cyrillic_i\x00"
                                "\x10" /* 'Cyrillic_i' 16 */
                                "ӥ\x00"
                                "\x04" /* 'ӥ' 4 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ṏ\x00"
                                "\x05" /* 'Ṏ' 5 */
                                "Cyrillic_zhe\x00"
                                "\x12" /* 'Cyrillic_zhe' 18 */
                                "ӝ\x00"
                                "\x04" /* 'ӝ' 4 */
                                "umacron\x00"
                                "\x0e" /* 'umacron' 14 */
                                "ṻ\x00"
                                "\x05" /* 'ṻ' 5 */
                                "Cyrillic_yeru\x00"
                                "\x13" /* 'Cyrillic_yeru' 19 */
                                "ӹ\x00"
                                "\x04" /* 'ӹ' 4 */
                                "acute\x00"
                                "\x0b" /* 'acute' 11 */
                                "̈́\x00"
                                "\x04" /* '̈́' 4 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẅ\x00"
                                "\x05" /* 'ẅ' 5 */
                                "Cyrillic_CHE\x00"
                                "\x12" /* 'Cyrillic_CHE' 18 */
                                "Ӵ\x00"
                                "\x04" /* 'Ӵ' 4 */
                                "Cyrillic_o\x00"
                                "\x10" /* 'Cyrillic_o' 16 */
                                "ӧ\x00"
                                "\x04" /* 'ӧ' 4 */
                                "Ukrainian_i\x00"
                                "\x11" /* 'Ukrainian_i' 17 */
                                "ї\x00"
                                "\x04" /* 'ї' 4 */
                                "Cyrillic_E\x00"
                                "\x10" /* 'Cyrillic_E' 16 */
                                "Ӭ\x00"
                                "\x04" /* 'Ӭ' 4 */
                                "apostrophe\x00"
                                "\x10" /* 'apostrophe' 16 */
                                "̈́\x00"
                                "\x04" /* '̈́' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ö\x00"
                                "\x04" /* 'Ö' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ä\x00"
                                "\x04" /* 'Ä' 4 */
                                "Cyrillic_A\x00"
                                "\x10" /* 'Cyrillic_A' 16 */
                                "Ӓ\x00"
                                "\x04" /* 'Ӓ' 4 */
                                "ugrave\x00"
                                "\x0c" /* 'ugrave' 12 */
                                "ǜ\x00"
                                "\x04" /* 'ǜ' 4 */
                                "Omacron\x00"
                                "\x0d" /* 'Omacron' 13 */
                                "Ȫ\x00"
                                "\x04" /* 'Ȫ' 4 */
                                "Cyrillic_ZHE\x00"
                                "\x12" /* 'Cyrillic_ZHE' 18 */
                                "Ӝ\x00"
                                "\x04" /* 'Ӝ' 4 */
                                "Cyrillic_IE\x00"
                                "\x11" /* 'Cyrillic_IE' 17 */
                                "Ё\x00"
                                "\x04" /* 'Ё' 4 */
                                "dead_diaeresis\x00"
                                "\x14" /* 'dead_diaeresis' 20 */
                                "¨\x00"
                                "\x04" /* '¨' 4 */
                                "Amacron\x00"
                                "\x0d" /* 'Amacron' 13 */
                                "Ǟ\x00"
                                "\x04" /* 'Ǟ' 4 */
                                "Cyrillic_e\x00"
                                "\x10" /* 'Cyrillic_e' 16 */
                                "ӭ\x00"
                                "\x04" /* 'ӭ' 4 */
                                "dead_acute\x00"
                                "\x58" /* 'dead_acute' 88 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "X\x00"
                                "\x08" /* 'X' 8 */
                                "Ẍ\x00"
                                "\x05" /* 'Ẍ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ḧ\x00"
                                "\x05" /* 'ḧ' 5 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Ï\x00"
                                "\x04" /* 'Ï' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ü\x00"
                                "\x04" /* 'Ü' 4 */
                                "Cyrillic_u\x00"
                                "\x10" /* 'Cyrillic_u' 16 */
                                "ӱ\x00"
                                "\x04" /* 'ӱ' 4 */
                                "Greek_UPSILON\x00"
                                "\x13" /* 'Greek_UPSILON' 19 */
                                "Ϋ\x00"
                                "\x04" /* 'Ϋ' 4 */
                                "dead_acute\x00"
                                "\x8c\xfd" /* 'dead_acute' 3325 */
                                "W\x00"
                                "\x08" /* 'W' 8 */
                                "Ẃ\x00"
                                "\x05" /* 'Ẃ' 5 */
                                "dead_breve\x00"
                                "\x1c" /* 'dead_breve' 28 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "g\x00"
                                "\x07" /* 'g' 7 */
                                "ǵ\x00"
                                "\x04" /* 'ǵ' 4 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "á\x00"
                                "\x04" /* 'á' 4 */
                                "Greek_IOTA\x00"
                                "\x10" /* 'Greek_IOTA' 16 */
                                "Ί\x00"
                                "\x04" /* 'Ί' 4 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ί\x00"
                                "\x04" /* 'ί' 4 */
                                "dead_horn\x00"
                                "\x2b" /* 'dead_horn' 43 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "dead_circumflex\x00"
                                "\x41" /* 'dead_circumflex' 65 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "Greek_OMICRON\x00"
                                "\x13" /* 'Greek_OMICRON' 19 */
                                "Ό\x00"
                                "\x04" /* 'Ό' 4 */
                                "Acircumflex\x00"
                                "\x12" /* 'Acircumflex' 18 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "C\x00"
                                "\x07" /* 'C' 7 */
                                "Ć\x00"
                                "\x04" /* 'Ć' 4 */
                                "Cyrillic_er\x00"
                                "\x13" /* 'Cyrillic_er' 19 */
                                "р́\x00"
                                "\x06" /* 'р́' 6 */
                                "e\x00"
                                "\x07" /* 'e' 7 */
                                "é\x00"
                                "\x04" /* 'é' 4 */
                                "Utilde\x00"
                                "\x0d" /* 'Utilde' 13 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ó\x00"
                                "\x04" /* 'ó' 4 */
                                "l\x00"
                                "\x07" /* 'l' 7 */
                                "ĺ\x00"
                                "\x04" /* 'ĺ' 4 */
                                "Udiaeresis\x00"
                                "\x10" /* 'Udiaeresis' 16 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ύ\x00"
                                "\x04" /* 'ύ' 4 */
                                "uhorn\x00"
                                "\x0c" /* 'uhorn' 12 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "space\x00"
                                "\x0a" /* 'space' 10 */
                                "'\x00"
                                "\x03" /* ''' 3 */
                                "dead_macron\x00"
                                "\x2d" /* 'dead_macron' 45 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "acircumflex\x00"
                                "\x12" /* 'acircumflex' 18 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "Ecircumflex\x00"
                                "\x12" /* 'Ecircumflex' 18 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "Cyrillic_I\x00"
                                "\x12" /* 'Cyrillic_I' 18 */
                                "И́\x00"
                                "\x06" /* 'И́' 6 */
                                "y\x00"
                                "\x07" /* 'y' 7 */
                                "ý\x00"
                                "\x04" /* 'ý' 4 */
                                "Multi_key\x00"
                                "\x83\xd8" /* 'Multi_key' 984 */
                                "KP_Divide\x00"
                                "\x19" /* 'KP_Divide' 25 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "o\x00"
                                "\x11" /* 'o' 17 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "b\x00"
                                "\x13" /* 'b' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "parenright\x00"
                                "\x80\xfb" /* 'parenright' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "quotedbl\x00"
                                "\x4b" /* 'quotedbl' 75 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "plus\x00"
                                "\x26" /* 'plus' 38 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ứ\x00"
                                "\x05" /* 'ứ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "cedilla\x00"
                                "\x19" /* 'cedilla' 25 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "underscore\x00"
                                "\x2c" /* 'underscore' 44 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "macron\x00"
                                "\x28" /* 'macron' 40 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "comma\x00"
                                "\x17" /* 'comma' 23 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "asciitilde\x00"
                                "\x2c" /* 'asciitilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "slash\x00"
                                "\x15" /* 'slash' 21 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "parenleft\x00"
                                "\x81\x0e" /* 'parenleft' 270 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "U\x00"
                                "\x13" /* 'U' 19 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "asciicircum\x00"
                                "\x3d" /* 'asciicircum' 61 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ấ\x00"
                                "\x05" /* 'ấ' 5 */
                                "e\x00"
                                "\x08" /* 'e' 8 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "E\x00"
                                "\x08" /* 'E' 8 */
                                "Ế\x00"
                                "\x05" /* 'Ế' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ấ\x00"
                                "\x05" /* 'Ấ' 5 */
                                "idiaeresis\x00"
                                "\x11" /* 'idiaeresis' 17 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "Cyrillic_O\x00"
                                "\x12" /* 'Cyrillic_O' 18 */
                                "О́\x00"
                                "\x06" /* 'О́' 6 */
                                "i\x00"
                                "\x07" /* 'i' 7 */
                                "í\x00"
                                "\x04" /* 'í' 4 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḱ\x00"
                                "\x05" /* 'ḱ' 5 */
                                "n\x00"
                                "\x07" /* 'n' 7 */
                                "ń\x00"
                                "\x04" /* 'ń' 4 */
                                "ccedilla\x00"
                                "\x0f" /* 'ccedilla' 15 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Cyrillic_GHE\x00"
                                "\x12" /* 'Cyrillic_GHE' 18 */
                                "Ѓ\x00"
                                "\x04" /* 'Ѓ' 4 */
                                "dead_tilde\x00"
                                "\x2c" /* 'dead_tilde' 44 */
                                "o\x00"
                                "\x08" /* 'o' 8 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "u\x00"
                                "\x08" /* 'u' 8 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "O\x00"
                                "\x08" /* 'O' 8 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "U\x00"
                                "\x08" /* 'U' 8 */
                                "Ṹ\x00"
                                "\x05" /* 'Ṹ' 5 */
                                "Cyrillic_a\x00"
                                "\x12" /* 'Cyrillic_a' 18 */
                                "а́\x00"
                                "\x06" /* 'а́' 6 */
                                "Ohorn\x00"
                                "\x0c" /* 'Ohorn' 12 */
                                "Ớ\x00"
                                "\x05" /* 'Ớ' 5 */
                                "ohorn\x00"
                                "\x0c" /* 'ohorn' 12 */
                                "ớ\x00"
                                "\x05" /* 'ớ' 5 */
                                "sabovedot\x00"
                                "\x10" /* 'sabovedot' 16 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "Cyrillic_ER\x00"
                                "\x13" /* 'Cyrillic_ER' 19 */
                                "Р́\x00"
                                "\x06" /* 'Р́' 6 */
                                "Greek_epsilon\x00"
                                "\x13" /* 'Greek_epsilon' 19 */
                                "έ\x00"
                                "\x04" /* 'έ' 4 */
                                "Cyrillic_KA\x00"
                                "\x11" /* 'Cyrillic_KA' 17 */
                                "Ќ\x00"
                                "\x04" /* 'Ќ' 4 */
                                "Cyrillic_U\x00"
                                "\x12" /* 'Cyrillic_U' 18 */
                                "У́\x00"
                                "\x06" /* 'У́' 6 */
                                "dead_abovering\x00"
                                "\x1e" /* 'dead_abovering' 30 */
                                "a\x00"
                                "\x07" /* 'a' 7 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "nobreakspace\x00"
                                "\x12" /* 'nobreakspace' 18 */
                                "́\x00"
                                "\x04" /* '́' 4 */
                                "V\x00"
                                "\x07" /* 'V' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Ocircumflex\x00"
                                "\x12" /* 'Ocircumflex' 18 */
                                "Ố\x00"
                                "\x05" /* 'Ố' 5 */
                                "AE\x00"
                                "\x08" /* 'AE' 8 */
                                "Ǽ\x00"
                                "\x04" /* 'Ǽ' 4 */
                                "omacron\x00"
                                "\x0e" /* 'omacron' 14 */
                                "ṓ\x00"
                                "\x05" /* 'ṓ' 5 */
                                "ocircumflex\x00"
                                "\x12" /* 'ocircumflex' 18 */
                                "ố\x00"
                                "\x05" /* 'ố' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ú\x00"
                                "\x04" /* 'ú' 4 */
                                "z\x00"
                                "\x07" /* 'z' 7 */
                                "ź\x00"
                                "\x04" /* 'ź' 4 */
                                "G\x00"
                                "\x07" /* 'G' 7 */
                                "Ǵ\x00"
                                "\x04" /* 'Ǵ' 4 */
                                "Greek_ALPHA\x00"
                                "\x11" /* 'Greek_ALPHA' 17 */
                                "Ά\x00"
                                "\x04" /* 'Ά' 4 */
                                "otilde\x00"
                                "\x0d" /* 'otilde' 13 */
                                "ṍ\x00"
                                "\x05" /* 'ṍ' 5 */
                                "utilde\x00"
                                "\x0d" /* 'utilde' 13 */
                                "ṹ\x00"
                                "\x05" /* 'ṹ' 5 */
                                "Cyrillic_ie\x00"
                                "\x13" /* 'Cyrillic_ie' 19 */
                                "е́\x00"
                                "\x06" /* 'е́' 6 */
                                "emacron\x00"
                                "\x0e" /* 'emacron' 14 */
                                "ḗ\x00"
                                "\x05" /* 'ḗ' 5 */
                                "E\x00"
                                "\x07" /* 'E' 7 */
                                "É\x00"
                                "\x04" /* 'É' 4 */
                                "S\x00"
                                "\x07" /* 'S' 7 */
                                "Ś\x00"
                                "\x04" /* 'Ś' 4 */
                                "Greek_iotadieresis\x00"
                                "\x18" /* 'Greek_iotadieresis' 24 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Y\x00"
                                "\x07" /* 'Y' 7 */
                                "Ý\x00"
                                "\x04" /* 'Ý' 4 */
                                "Cyrillic_i\x00"
                                "\x12" /* 'Cyrillic_i' 18 */
                                "и́\x00"
                                "\x06" /* 'и́' 6 */
                                "dead_dasia\x00"
                                "\x81\x0f" /* 'dead_dasia' 271 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἵ\x00"
                                "\x05" /* 'Ἵ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἵ\x00"
                                "\x05" /* 'ἵ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὅ\x00"
                                "\x05" /* 'Ὅ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὕ\x00"
                                "\x05" /* 'ὕ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἕ\x00"
                                "\x05" /* 'ἕ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἅ\x00"
                                "\x05" /* 'Ἅ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὅ\x00"
                                "\x05" /* 'ὅ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἥ\x00"
                                "\x05" /* 'ἥ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἅ\x00"
                                "\x05" /* 'ἅ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἥ\x00"
                                "\x05" /* 'Ἥ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἕ\x00"
                                "\x05" /* 'Ἕ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὥ\x00"
                                "\x05" /* 'ὥ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὥ\x00"
                                "\x05" /* 'Ὥ' 5 */
                                "Greek_UPSILON\x00"
                                "\x14" /* 'Greek_UPSILON' 20 */
                                "Ὕ\x00"
                                "\x05" /* 'Ὕ' 5 */
                                "Greek_upsilondieresis\x00"
                                "\x1b" /* 'Greek_upsilondieresis' 27 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "Greek_omicron\x00"
                                "\x13" /* 'Greek_omicron' 19 */
                                "ό\x00"
                                "\x04" /* 'ό' 4 */
                                "Greek_eta\x00"
                                "\x0f" /* 'Greek_eta' 15 */
                                "ή\x00"
                                "\x04" /* 'ή' 4 */
                                "Otilde\x00"
                                "\x0d" /* 'Otilde' 13 */
                                "Ṍ\x00"
                                "\x05" /* 'Ṍ' 5 */
                                "Cyrillic_ka\x00"
                                "\x11" /* 'Cyrillic_ka' 17 */
                                "ќ\x00"
                                "\x04" /* 'ќ' 4 */
                                "Aring\x00"
                                "\x0b" /* 'Aring' 11 */
                                "Ǻ\x00"
                                "\x04" /* 'Ǻ' 4 */
                                "Abreve\x00"
                                "\x0d" /* 'Abreve' 13 */
                                "Ắ\x00"
                                "\x05" /* 'Ắ' 5 */
                                "dead_psili\x00"
                                "\x80\xfb" /* 'dead_psili' 251 */
                                "Greek_IOTA\x00"
                                "\x11" /* 'Greek_IOTA' 17 */
                                "Ἴ\x00"
                                "\x05" /* 'Ἴ' 5 */
                                "Greek_iota\x00"
                                "\x11" /* 'Greek_iota' 17 */
                                "ἴ\x00"
                                "\x05" /* 'ἴ' 5 */
                                "Greek_OMICRON\x00"
                                "\x14" /* 'Greek_OMICRON' 20 */
                                "Ὄ\x00"
                                "\x05" /* 'Ὄ' 5 */
                                "Greek_upsilon\x00"
                                "\x14" /* 'Greek_upsilon' 20 */
                                "ὔ\x00"
                                "\x05" /* 'ὔ' 5 */
                                "Greek_epsilon\x00"
                                "\x14" /* 'Greek_epsilon' 20 */
                                "ἔ\x00"
                                "\x05" /* 'ἔ' 5 */
                                "Greek_ALPHA\x00"
                                "\x12" /* 'Greek_ALPHA' 18 */
                                "Ἄ\x00"
                                "\x05" /* 'Ἄ' 5 */
                                "Greek_omicron\x00"
                                "\x14" /* 'Greek_omicron' 20 */
                                "ὄ\x00"
                                "\x05" /* 'ὄ' 5 */
                                "Greek_eta\x00"
                                "\x10" /* 'Greek_eta' 16 */
                                "ἤ\x00"
                                "\x05" /* 'ἤ' 5 */
                                "Greek_alpha\x00"
                                "\x12" /* 'Greek_alpha' 18 */
                                "ἄ\x00"
                                "\x05" /* 'ἄ' 5 */
                                "Greek_ETA\x00"
                                "\x10" /* 'Greek_ETA' 16 */
                                "Ἤ\x00"
                                "\x05" /* 'Ἤ' 5 */
                                "Greek_EPSILON\x00"
                                "\x14" /* 'Greek_EPSILON' 20 */
                                "Ἔ\x00"
                                "\x05" /* 'Ἔ' 5 */
                                "Greek_omega\x00"
                                "\x12" /* 'Greek_omega' 18 */
                                "ὤ\x00"
                                "\x05" /* 'ὤ' 5 */
                                "Greek_OMEGA\x00"
                                "\x12" /* 'Greek_OMEGA' 18 */
                                "Ὤ\x00"
                                "\x05" /* 'Ὤ' 5 */
                                "Greek_alpha\x00"
                                "\x11" /* 'Greek_alpha' 17 */
                                "ά\x00"
                                "\x04" /* 'ά' 4 */
                                "ecircumflex\x00"
                                "\x12" /* 'ecircumflex' 18 */
                                "ế\x00"
                                "\x05" /* 'ế' 5 */
                                "dead_abovedot\x00"
                                "\x1f" /* 'dead_abovedot' 31 */
                                "S\x00"
                                "\x08" /* 'S' 8 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "s\x00"
                                "\x08" /* 's' 8 */
                                "ṥ\x00"
                                "\x05" /* 'ṥ' 5 */
                                "w\x00"
                                "\x08" /* 'w' 8 */
                                "ẃ\x00"
                                "\x05" /* 'ẃ' 5 */
                                "Greek_ETA\x00"
                                "\x0f" /* 'Greek_ETA' 15 */
                                "Ή\x00"
                                "\x04" /* 'Ή' 4 */
                                "Cyrillic_o\x00"
                                "\x12" /* 'Cyrillic_o' 18 */
                                "о́\x00"
                                "\x06" /* 'о́' 6 */
                                "Emacron\x00"
                                "\x0e" /* 'Emacron' 14 */
                                "Ḗ\x00"
                                "\x05" /* 'Ḗ' 5 */
                                "Ooblique\x00"
                                "\x0e" /* 'Ooblique' 14 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "p\x00"
                                "\x08" /* 'p' 8 */
                                "ṕ\x00"
                                "\x05" /* 'ṕ' 5 */
                                "v\x00"
                                "\x07" /* 'v' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "P\x00"
                                "\x08" /* 'P' 8 */
                                "Ṕ\x00"
                                "\x05" /* 'Ṕ' 5 */
                                "M\x00"
                                "\x08" /* 'M' 8 */
                                "Ḿ\x00"
                                "\x05" /* 'Ḿ' 5 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ó\x00"
                                "\x04" /* 'Ó' 4 */
                                "abreve\x00"
                                "\x0d" /* 'abreve' 13 */
                                "ắ\x00"
                                "\x05" /* 'ắ' 5 */
                                "m\x00"
                                "\x08" /* 'm' 8 */
                                "ḿ\x00"
                                "\x05" /* 'ḿ' 5 */
                                "r\x00"
                                "\x07" /* 'r' 7 */
                                "ŕ\x00"
                                "\x04" /* 'ŕ' 4 */
                                "s\x00"
                                "\x07" /* 's' 7 */
                                "ś\x00"
                                "\x04" /* 'ś' 4 */
                                "Z\x00"
                                "\x07" /* 'Z' 7 */
                                "Ź\x00"
                                "\x04" /* 'Ź' 4 */
                                "dead_stroke\x00"
                                "\x1b" /* 'dead_stroke' 27 */
                                "o\x00"
                                "\x07" /* 'o' 7 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "O\x00"
                                "\x07" /* 'O' 7 */
                                "Ǿ\x00"
                                "\x04" /* 'Ǿ' 4 */
                                "A\x00"
                                "\x07" /* 'A' 7 */
                                "Á\x00"
                                "\x04" /* 'Á' 4 */
                                "R\x00"
                                "\x07" /* 'R' 7 */
                                "Ŕ\x00"
                                "\x04" /* 'Ŕ' 4 */
                                "c\x00"
                                "\x07" /* 'c' 7 */
                                "ć\x00"
                                "\x04" /* 'ć' 4 */
                                "Idiaeresis\x00"
                                "\x11" /* 'Idiaeresis' 17 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "L\x00"
                                "\x07" /* 'L' 7 */
                                "Ĺ\x00"
                                "\x04" /* 'Ĺ' 4 */
                                "Greek_EPSILON\x00"
                                "\x13" /* 'Greek_EPSILON' 19 */
                                "Έ\x00"
                                "\x04" /* 'Έ' 4 */
                                "Cyrillic_A\x00"
                                "\x12" /* 'Cyrillic_A' 18 */
                                "А́\x00"
                                "\x06" /* 'А́' 6 */
                                "Ccedilla\x00"
                                "\x0f" /* 'Ccedilla' 15 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "aring\x00"
                                "\x0b" /* 'aring' 11 */
                                "ǻ\x00"
                                "\x04" /* 'ǻ' 4 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḱ\x00"
                                "\x05" /* 'Ḱ' 5 */
                                "Omacron\x00"
                                "\x0e" /* 'Omacron' 14 */
                                "Ṓ\x00"
                                "\x05" /* 'Ṓ' 5 */
                                "Cyrillic_IE\x00"
                                "\x13" /* 'Cyrillic_IE' 19 */
                                "Е́\x00"
                                "\x06" /* 'Е́' 6 */
                                "Sabovedot\x00"
                                "\x10" /* 'Sabovedot' 16 */
                                "Ṥ\x00"
                                "\x05" /* 'Ṥ' 5 */
                                "dead_cedilla\x00"
                                "\x1e" /* 'dead_cedilla' 30 */
                                "C\x00"
                                "\x08" /* 'C' 8 */
                                "Ḉ\x00"
                                "\x05" /* 'Ḉ' 5 */
                                "c\x00"
                                "\x08" /* 'c' 8 */
                                "ḉ\x00"
                                "\x05" /* 'ḉ' 5 */
                                "Greek_omega\x00"
                                "\x11" /* 'Greek_omega' 17 */
                                "ώ\x00"
                                "\x04" /* 'ώ' 4 */
                                "dead_diaeresis\x00"
                                "\x5c" /* 'dead_diaeresis' 92 */
                                "Greek_iota\x00"
                                "\x10" /* 'Greek_iota' 16 */
                                "ΐ\x00"
                                "\x04" /* 'ΐ' 4 */
                                "Greek_upsilon\x00"
                                "\x13" /* 'Greek_upsilon' 19 */
                                "ΰ\x00"
                                "\x04" /* 'ΰ' 4 */
                                "space\x00"
                                "\x0b" /* 'space' 11 */
                                "΅\x00"
                                "\x04" /* '΅' 4 */
                                "i\x00"
                                "\x08" /* 'i' 8 */
                                "ḯ\x00"
                                "\x05" /* 'ḯ' 5 */
                                "u\x00"
                                "\x07" /* 'u' 7 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x08" /* 'I' 8 */
                                "Ḯ\x00"
                                "\x05" /* 'Ḯ' 5 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ǘ\x00"
                                "\x04" /* 'Ǘ' 4 */
                                "Uhorn\x00"
                                "\x0c" /* 'Uhorn' 12 */
                                "Ứ\x00"
                                "\x05" /* 'Ứ' 5 */
                                "Greek_OMEGA\x00"
                                "\x11" /* 'Greek_OMEGA' 17 */
                                "Ώ\x00"
                                "\x04" /* 'Ώ' 4 */
                                "dead_acute\x00"
                                "\x10" /* 'dead_acute' 16 */
                                "´\x00"
                                "\x04" /* '´' 4 */
                                "oslash\x00"
                                "\x0c" /* 'oslash' 12 */
                                "ǿ\x00"
                                "\x04" /* 'ǿ' 4 */
                                "Cyrillic_ghe\x00"
                                "\x12" /* 'Cyrillic_ghe' 18 */
                                "ѓ\x00"
                                "\x04" /* 'ѓ' 4 */
                                "udiaeresis\x00"
                                "\x10" /* 'udiaeresis' 16 */
                                "ǘ\x00"
                                "\x04" /* 'ǘ' 4 */
                                "I\x00"
                                "\x07" /* 'I' 7 */
                                "Í\x00"
                                "\x04" /* 'Í' 4 */
                                "N\x00"
                                "\x07" /* 'N' 7 */
                                "Ń\x00"
                                "\x04" /* 'Ń' 4 */
                                "U\x00"
                                "\x07" /* 'U' 7 */
                                "Ú\x00"
                                "\x04" /* 'Ú' 4 */
                                "Cyrillic_u\x00"
                                "\x12" /* 'Cyrillic_u' 18 */
                                "у́\x00"
                                "\x06" /* 'у́' 6 */
                                "ae\x00"
                                "\x08" /* 'ae' 8 */
                                "ǽ\x00"
                                "\x04" /* 'ǽ' 4 */
                                "Greek_UPSILON\x00"
                                "\x13" /* 'Greek_UPSILON' 19 */
                                "Ύ\x00"
                                "\x04" /* 'Ύ' 4 */
                                "dead_belowmacron\x00"
                                "\x80\x9b" /* 'dead_belowmacron' 155 */
                                "l\x00"
                                "\x08" /* 'l' 8 */
                                "ḻ\x00"
                                "\x05" /* 'ḻ' 5 */
                                "t\x00"
                                "\x08" /* 't' 8 */
                                "ṯ\x00"
                                "\x05" /* 'ṯ' 5 */
                                "b\x00"
                                "\x08" /* 'b' 8 */
                                "ḇ\x00"
                                "\x05" /* 'ḇ' 5 */
                                "k\x00"
                                "\x08" /* 'k' 8 */
                                "ḵ\x00"
                                "\x05" /* 'ḵ' 5 */
                                "n\x00"
                                "\x08" /* 'n' 8 */
                                "ṉ\x00"
                                "\x05" /* 'ṉ' 5 */
                                "z\x00"
                                "\x08" /* 'z' 8 */
                                "ẕ\x00"
                                "\x05" /* 'ẕ' 5 */
                                "d\x00"
                                "\x08" /* 'd' 8 */
                                "ḏ\x00"
                                "\x05" /* 'ḏ' 5 */
                                "D\x00"
                                "\x08" /* 'D' 8 */
                                "Ḏ\x00"
                                "\x05" /* 'Ḏ' 5 */
                                "r\x00"
                                "\x08" /* 'r' 8 */
                                "ṟ\x00"
                                "\x05" /* 'ṟ' 5 */
                                "Z\x00"
                                "\x08" /* 'Z' 8 */
                                "Ẕ\x00"
                                "\x05" /* 'Ẕ' 5 */
                                "R\x00"
                                "\x08" /* 'R' 8 */
                                "Ṟ\x00"
                                "\x05" /* 'Ṟ' 5 */
                                "L\x00"
                                "\x08" /* 'L' 8 */
                                "Ḻ\x00"
                                "\x05" /* 'Ḻ' 5 */
                                "T\x00"
                                "\x08" /* 'T' 8 */
                                "Ṯ\x00"
                                "\x05" /* 'Ṯ' 5 */
                                "K\x00"
                                "\x08" /* 'K' 8 */
                                "Ḵ\x00"
                                "\x05" /* 'Ḵ' 5 */
                                "B\x00"
                                "\x08" /* 'B' 8 */
                                "Ḇ\x00"
                                "\x05" /* 'Ḇ' 5 */
                                "h\x00"
                                "\x08" /* 'h' 8 */
                                "ẖ\x00"
                                "\x05" /* 'ẖ' 5 */
                                "N\x00"
                                "\x08" /* 'N' 8 */
                                "Ṉ\x00"
                                "\x05" /* 'Ṉ' 5 */
                                "dead_belowring\x00"
                                "\x2a" /* 'dead_belowring' 42 */
                                "a\x00"
                                "\x08" /* 'a' 8 */
                                "ḁ\x00"
                                "\x05" /* 'ḁ' 5 */
                                "bar\x00"
                                "\x0a" /* 'bar' 10 */
                                "⫰\x00"
                                "\x05" /* '⫰' 5 */
                                "A\x00"
                                "\x08" /* 'A' 8 */
                                "Ḁ\x00"
                                "\x05" /* 'Ḁ' 5 */
/* end marker - 4 0 bytes */
                                "\x00\x00\x00\x00";
