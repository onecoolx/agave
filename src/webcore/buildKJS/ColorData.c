/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -CDEot -L ANSI-C -N findColor -D -s 2 platform/ColorData.gperf  */
/* Computed positions: -k'3-4,6-8,12-13' */
#include <string.h>

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "platform/ColorData.gperf"
struct NamedColor { const char *name; int RGBValue; };
/* maximum key range = 573, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static const unsigned short asso_values[] =
    {
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578,   5,   5,  75,
       65,   0, 180,   0,   5,  35,  95, 115,  30, 155,
       45,   0,  15, 160,  10,  65,  15,  10,  75, 145,
       15, 235,  60, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578, 578, 578, 578,
      578, 578, 578, 578, 578, 578, 578
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
      case 10:
      case 9:
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]+1];
      /*FALLTHROUGH*/
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval;
}

const struct NamedColor *
findColor (register const char *str, register size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 149,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 20,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 577
    };

  static const struct NamedColor wordlist[] =
    {
#line 58 "platform/ColorData.gperf"
      {"green", 0x008000},
#line 147 "platform/ColorData.gperf"
      {"wheat", 0xf5deb3},
#line 12 "platform/ColorData.gperf"
      {"blue", 0x0000ff},
#line 64 "platform/ColorData.gperf"
      {"ivory", 0xfffff0},
#line 90 "platform/ColorData.gperf"
      {"maroon", 0x800000},
#line 5 "platform/ColorData.gperf"
      {"aqua", 0x00ffff},
#line 19 "platform/ColorData.gperf"
      {"coral", 0xff7f50},
#line 89 "platform/ColorData.gperf"
      {"magenta", 0xff00ff},
#line 118 "platform/ColorData.gperf"
      {"peru", 0xcd853f},
#line 7 "platform/ColorData.gperf"
      {"azure", 0xf0ffff},
#line 75 "platform/ColorData.gperf"
      {"lightgrey", 0xd3d3d3},
#line 76 "platform/ColorData.gperf"
      {"lightgreen", 0x90ee90},
#line 74 "platform/ColorData.gperf"
      {"lightgray", 0xd3d3d3},
#line 141 "platform/ColorData.gperf"
      {"teal", 0x008080},
#line 8 "platform/ColorData.gperf"
      {"beige", 0xf5f5dc},
#line 50 "platform/ColorData.gperf"
      {"forestgreen", 0x228b22},
#line 136 "platform/ColorData.gperf"
      {"slategrey", 0x708090},
#line 145 "platform/ColorData.gperf"
      {"violet", 0xee82ee},
#line 140 "platform/ColorData.gperf"
      {"tan", 0xd2b48c},
#line 135 "platform/ColorData.gperf"
      {"slategray", 0x708090},
#line 88 "platform/ColorData.gperf"
      {"linen", 0xfaf0e6},
#line 23 "platform/ColorData.gperf"
      {"cyan", 0x00ffff},
#line 148 "platform/ColorData.gperf"
      {"white", 0xffffff},
#line 131 "platform/ColorData.gperf"
      {"sienna", 0xa0522d},
#line 146 "platform/ColorData.gperf"
      {"violetred", 0xd02090},
#line 15 "platform/ColorData.gperf"
      {"burlywood", 0xdeb887},
#line 78 "platform/ColorData.gperf"
      {"lightsalmon", 0xffa07a},
#line 123 "platform/ColorData.gperf"
      {"red", 0xff0000},
#line 73 "platform/ColorData.gperf"
      {"lightgoldenrodyellow", 0xfafad2},
#line 138 "platform/ColorData.gperf"
      {"springgreen", 0x00ff7f},
#line 6 "platform/ColorData.gperf"
      {"aquamarine", 0x7fffd4},
#line 83 "platform/ColorData.gperf"
      {"lightslategrey", 0x778899},
#line 150 "platform/ColorData.gperf"
      {"yellow", 0xffff00},
#line 79 "platform/ColorData.gperf"
      {"lightseagreen", 0x20b2aa},
#line 82 "platform/ColorData.gperf"
      {"lightslategray", 0x778899},
#line 10 "platform/ColorData.gperf"
      {"black", 0x000000},
#line 111 "platform/ColorData.gperf"
      {"orchid", 0xda70d6},
#line 84 "platform/ColorData.gperf"
      {"lightsteelblue", 0xb0c4de},
#line 71 "platform/ColorData.gperf"
      {"lightcoral", 0xf08080},
#line 66 "platform/ColorData.gperf"
      {"lavender", 0xe6e6fa},
#line 151 "platform/ColorData.gperf"
      {"yellowgreen", 0x9acd32},
#line 54 "platform/ColorData.gperf"
      {"gold", 0xffd700},
#line 17 "platform/ColorData.gperf"
      {"chartreuse", 0x7fff00},
#line 59 "platform/ColorData.gperf"
      {"greenyellow", 0xadff2f},
#line 113 "platform/ColorData.gperf"
      {"palegreen", 0x98fb98},
#line 85 "platform/ColorData.gperf"
      {"lightyellow", 0xffffe0},
#line 81 "platform/ColorData.gperf"
      {"lightslateblue", 0x8470ff},
#line 55 "platform/ColorData.gperf"
      {"goldenrod", 0xdaa520},
#line 107 "platform/ColorData.gperf"
      {"olive", 0x808000},
#line 62 "platform/ColorData.gperf"
      {"indianred", 0xcd5c5c},
#line 63 "platform/ColorData.gperf"
      {"indigo", 0x4b0082},
#line 139 "platform/ColorData.gperf"
      {"steelblue", 0x4682b4},
#line 65 "platform/ColorData.gperf"
      {"khaki", 0xf0e68c},
#line 70 "platform/ColorData.gperf"
      {"lightblue", 0xadd8e6},
#line 53 "platform/ColorData.gperf"
      {"ghostwhite", 0xf8f8ff},
#line 108 "platform/ColorData.gperf"
      {"olivedrab", 0x6b8e23},
#line 101 "platform/ColorData.gperf"
      {"mintcream", 0xf5fffa},
#line 31 "platform/ColorData.gperf"
      {"darkmagenta", 0x8b008b},
#line 134 "platform/ColorData.gperf"
      {"slateblue", 0x6a5acd},
#line 13 "platform/ColorData.gperf"
      {"blueviolet", 0x8a2be2},
#line 100 "platform/ColorData.gperf"
      {"midnightblue", 0x191970},
#line 137 "platform/ColorData.gperf"
      {"snow", 0xfffafa},
#line 14 "platform/ColorData.gperf"
      {"brown", 0xa52a2a},
#line 61 "platform/ColorData.gperf"
      {"hotpink", 0xff69b4},
#line 86 "platform/ColorData.gperf"
      {"lime", 0x00ff00},
#line 119 "platform/ColorData.gperf"
      {"pink", 0xffc0cb},
#line 106 "platform/ColorData.gperf"
      {"oldlace", 0xfdf5e6},
#line 67 "platform/ColorData.gperf"
      {"lavenderblush", 0xfff0f5},
#line 120 "platform/ColorData.gperf"
      {"plum", 0xdda0dd},
#line 52 "platform/ColorData.gperf"
      {"gainsboro", 0xdcdcdc},
#line 47 "platform/ColorData.gperf"
      {"dodgerblue", 0x1e90ff},
#line 132 "platform/ColorData.gperf"
      {"silver", 0xc0c0c0},
#line 143 "platform/ColorData.gperf"
      {"tomato", 0xff6347},
#line 94 "platform/ColorData.gperf"
      {"mediumpurple", 0x9370d8},
#line 116 "platform/ColorData.gperf"
      {"papayawhip", 0xffefd5},
#line 51 "platform/ColorData.gperf"
      {"fuchsia", 0xff00ff},
#line 16 "platform/ColorData.gperf"
      {"cadetblue", 0x5f9ea0},
#line 92 "platform/ColorData.gperf"
      {"mediumblue", 0x0000cd},
#line 127 "platform/ColorData.gperf"
      {"salmon", 0xfa8072},
#line 48 "platform/ColorData.gperf"
      {"firebrick", 0xb22222},
#line 60 "platform/ColorData.gperf"
      {"honeydew", 0xf0fff0},
#line 29 "platform/ColorData.gperf"
      {"darkgreen", 0x006400},
#line 128 "platform/ColorData.gperf"
      {"sandybrown", 0xf4a460},
#line 11 "platform/ColorData.gperf"
      {"blanchedalmond", 0xffebcd},
#line 122 "platform/ColorData.gperf"
      {"purple", 0x800080},
#line 112 "platform/ColorData.gperf"
      {"palegoldenrod", 0xeee8aa},
#line 102 "platform/ColorData.gperf"
      {"mistyrose", 0xffe4e1},
#line 98 "platform/ColorData.gperf"
      {"mediumturquoise", 0x48d1cc},
#line 95 "platform/ColorData.gperf"
      {"mediumseagreen", 0x3cb371},
#line 87 "platform/ColorData.gperf"
      {"limegreen", 0x32cd32},
#line 149 "platform/ColorData.gperf"
      {"whitesmoke", 0xf5f5f5},
#line 93 "platform/ColorData.gperf"
      {"mediumorchid", 0xba55d3},
#line 115 "platform/ColorData.gperf"
      {"palevioletred", 0xd87093},
#line 3 "platform/ColorData.gperf"
      {"aliceblue", 0xf0f8ff},
#line 109 "platform/ColorData.gperf"
      {"orange", 0xffa500},
#line 129 "platform/ColorData.gperf"
      {"seagreen", 0x2e8b57},
#line 57 "platform/ColorData.gperf"
      {"grey", 0x808080},
#line 97 "platform/ColorData.gperf"
      {"mediumspringgreen", 0x00fa9a},
#line 25 "platform/ColorData.gperf"
      {"darkcyan", 0x008b8b},
#line 56 "platform/ColorData.gperf"
      {"gray", 0x808080},
#line 110 "platform/ColorData.gperf"
      {"orangered", 0xff4500},
#line 33 "platform/ColorData.gperf"
      {"darkorange", 0xff8c00},
#line 103 "platform/ColorData.gperf"
      {"moccasin", 0xffe4b5},
#line 77 "platform/ColorData.gperf"
      {"lightpink", 0xffb6c1},
#line 104 "platform/ColorData.gperf"
      {"navajowhite", 0xffdead},
#line 22 "platform/ColorData.gperf"
      {"crimson", 0xdc143c},
#line 18 "platform/ColorData.gperf"
      {"chocolate", 0xd2691e},
#line 42 "platform/ColorData.gperf"
      {"darkviolet", 0x9400d3},
#line 142 "platform/ColorData.gperf"
      {"thistle", 0xd8bfd8},
#line 68 "platform/ColorData.gperf"
      {"lawngreen", 0x7cfc00},
#line 43 "platform/ColorData.gperf"
      {"deeppink", 0xff1493},
#line 34 "platform/ColorData.gperf"
      {"darkorchid", 0x9932cc},
#line 4 "platform/ColorData.gperf"
      {"antiquewhite", 0xfaebd7},
#line 30 "platform/ColorData.gperf"
      {"darkkhaki", 0xbdb76b},
#line 96 "platform/ColorData.gperf"
      {"mediumslateblue", 0x7b68ee},
#line 144 "platform/ColorData.gperf"
      {"turquoise", 0x40e0d0},
#line 99 "platform/ColorData.gperf"
      {"mediumvioletred", 0xc71585},
#line 44 "platform/ColorData.gperf"
      {"deepskyblue", 0x00bfff},
#line 24 "platform/ColorData.gperf"
      {"darkblue", 0x00008b},
#line 21 "platform/ColorData.gperf"
      {"cornsilk", 0xfff8dc},
#line 26 "platform/ColorData.gperf"
      {"darkgoldenrod", 0xb8860b},
#line 105 "platform/ColorData.gperf"
      {"navy", 0x000080},
#line 69 "platform/ColorData.gperf"
      {"lemonchiffon", 0xfffacd},
#line 130 "platform/ColorData.gperf"
      {"seashell", 0xfff5ee},
#line 72 "platform/ColorData.gperf"
      {"lightcyan", 0xe0ffff},
#line 121 "platform/ColorData.gperf"
      {"powderblue", 0xb0e0e6},
#line 133 "platform/ColorData.gperf"
      {"skyblue", 0x87ceeb},
#line 38 "platform/ColorData.gperf"
      {"darkslateblue", 0x483d8b},
#line 36 "platform/ColorData.gperf"
      {"darksalmon", 0xe9967a},
#line 49 "platform/ColorData.gperf"
      {"floralwhite", 0xfffaf0},
#line 126 "platform/ColorData.gperf"
      {"saddlebrown", 0x8b4513},
#line 91 "platform/ColorData.gperf"
      {"mediumaquamarine", 0x66cdaa},
#line 114 "platform/ColorData.gperf"
      {"paleturquoise", 0xafeeee},
#line 125 "platform/ColorData.gperf"
      {"royalblue", 0x4169e1},
#line 37 "platform/ColorData.gperf"
      {"darkseagreen", 0x8fbc8f},
#line 35 "platform/ColorData.gperf"
      {"darkred", 0x8b0000},
#line 80 "platform/ColorData.gperf"
      {"lightskyblue", 0x87cefa},
#line 45 "platform/ColorData.gperf"
      {"dimgray", 0x696969},
#line 32 "platform/ColorData.gperf"
      {"darkolivegreen", 0x556b2f},
#line 20 "platform/ColorData.gperf"
      {"cornflowerblue", 0x6495ed},
#line 9 "platform/ColorData.gperf"
      {"bisque", 0xffe4c4},
#line 28 "platform/ColorData.gperf"
      {"darkgrey", 0xa9a9a9},
#line 27 "platform/ColorData.gperf"
      {"darkgray", 0xa9a9a9},
#line 117 "platform/ColorData.gperf"
      {"peachpuff", 0xffdab9},
#line 41 "platform/ColorData.gperf"
      {"darkturquoise", 0x00ced1},
#line 124 "platform/ColorData.gperf"
      {"rosybrown", 0xbc8f8f},
#line 40 "platform/ColorData.gperf"
      {"darkslategrey", 0x2f4f4f},
#line 39 "platform/ColorData.gperf"
      {"darkslategray", 0x2f4f4f},
#line 46 "platform/ColorData.gperf"
      {"dimgrey", 0x696969}
    };

  static const short lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,   0,  -1,  -1,  -1,  -1,
        1,  -1,  -1,  -1,   2,   3,   4,  -1,  -1,   5,
        6,  -1,   7,  -1,   8,   9,  -1,  -1,  -1,  10,
       11,  -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,  13,
       14,  15,  -1,  -1,  16,  -1,  17,  -1,  18,  19,
       20,  -1,  -1,  -1,  21,  22,  23,  -1,  -1,  24,
       -1,  -1,  -1,  -1,  25,  -1,  26,  -1,  27,  -1,
       28,  29,  -1,  -1,  -1,  30,  -1,  -1,  -1,  31,
       -1,  32,  -1,  33,  34,  35,  36,  -1,  -1,  37,
       38,  -1,  -1,  39,  -1,  -1,  40,  -1,  -1,  41,
       42,  43,  -1,  -1,  44,  -1,  45,  -1,  -1,  46,
       -1,  -1,  -1,  -1,  47,  48,  -1,  -1,  -1,  49,
       -1,  50,  -1,  -1,  51,  52,  -1,  -1,  -1,  53,
       54,  -1,  -1,  -1,  55,  -1,  -1,  -1,  -1,  56,
       -1,  57,  -1,  -1,  58,  59,  -1,  60,  -1,  61,
       62,  -1,  63,  -1,  -1,  -1,  -1,  -1,  -1,  64,
       -1,  -1,  -1,  -1,  65,  -1,  -1,  66,  67,  68,
       -1,  -1,  -1,  -1,  69,  70,  71,  -1,  -1,  -1,
       -1,  72,  73,  -1,  -1,  74,  -1,  75,  -1,  76,
       77,  78,  -1,  -1,  79,  -1,  -1,  -1,  80,  81,
       -1,  -1,  -1,  -1,  -1,  82,  -1,  -1,  -1,  83,
       -1,  84,  -1,  -1,  -1,  -1,  -1,  -1,  85,  86,
       87,  -1,  -1,  -1,  88,  -1,  -1,  -1,  -1,  89,
       90,  -1,  91,  92,  93,  -1,  94,  -1,  95,  96,
       -1,  -1,  97,  98,  99,  -1,  -1,  -1,  -1, 100,
      101,  -1,  -1, 102, 103,  -1, 104, 105,  -1, 106,
      107,  -1, 108,  -1, 109,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 110,  -1,
      111,  -1, 112,  -1,  -1,  -1,  -1,  -1,  -1, 113,
      114,  -1,  -1,  -1, 115, 116, 117,  -1, 118,  -1,
       -1,  -1,  -1, 119,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 120, 121,  -1,  -1, 122, 123, 124,
      125,  -1, 126, 127,  -1, 128, 129,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 130,  -1,  -1,  -1,
       -1, 131,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 132,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 133,  -1,  -1, 134,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 135,  -1,  -1,
       -1,  -1, 136,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 137,  -1, 138,  -1,  -1,  -1,  -1, 139,
       -1, 140,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 141,  -1,  -1,  -1,  -1, 142, 143,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 144,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 145,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 146,  -1,
       -1,  -1,  -1, 147,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 148
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
