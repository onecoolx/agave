/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -CEot -L ANSI-C -N findDoctypeEntry -F ,PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards html/DocTypeStrings.gperf  */
/* Computed positions: -k'19,21,24-25,29,31-32,36-37' */
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

#line 1 "html/DocTypeStrings.gperf"
struct PubIDInfo {
    enum eMode { 
        eQuirks,         
        eQuirks3,       
        eAlmostStandards
    };

    const char* name;
    eMode mode_if_no_sysid;
    eMode mode_if_sysid;
};
/* maximum key range = 204, duplicates = 0 */

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
  static const unsigned char asso_values[] =
    {
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208,   5, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208,   5, 208,   0,   0,   0,  20,  45,
       10,   0,   0, 208, 208,  10, 208,   0, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208,  55, 208,  55,
        0,   0,  10, 208,  15,   5, 208, 208,   5,   0,
        0,   0,   0, 208,   0,   5,   0, 208,   0, 208,
       60, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
      208, 208, 208, 208, 208, 208
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[36]];
      /*FALLTHROUGH*/
      case 36:
        hval += asso_values[(unsigned char)str[35]];
      /*FALLTHROUGH*/
      case 35:
      case 34:
      case 33:
      case 32:
        hval += asso_values[(unsigned char)str[31]];
      /*FALLTHROUGH*/
      case 31:
        hval += asso_values[(unsigned char)str[30]];
      /*FALLTHROUGH*/
      case 30:
      case 29:
        hval += asso_values[(unsigned char)str[28]];
      /*FALLTHROUGH*/
      case 28:
      case 27:
      case 26:
      case 25:
        hval += asso_values[(unsigned char)str[24]];
      /*FALLTHROUGH*/
      case 24:
        hval += asso_values[(unsigned char)str[23]];
      /*FALLTHROUGH*/
      case 23:
      case 22:
      case 21:
        hval += asso_values[(unsigned char)str[20]];
      /*FALLTHROUGH*/
      case 20:
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
      case 18:
      case 17:
      case 16:
      case 15:
      case 14:
      case 13:
      case 12:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
        break;
    }
  return hval;
}

const struct PubIDInfo *
findDoctypeEntry (register const char *str, register size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 77,
      MIN_WORD_LENGTH = 4,
      MAX_WORD_LENGTH = 80,
      MIN_HASH_VALUE = 4,
      MAX_HASH_VALUE = 207
    };

  static const struct PubIDInfo wordlist[] =
    {
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 89 "html/DocTypeStrings.gperf"
      {"html", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 47 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 27 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 3//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 72 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 3.2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 49 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html//en//3.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 80 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd w3 html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 83 "html/DocTypeStrings.gperf"
      {"-//w3o//dtd w3 html 3.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 44 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 84 "html/DocTypeStrings.gperf"
      {"-//w3o//dtd w3 html 3.0//en//", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 26 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 3.2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 48 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 46 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict//en//3.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 34 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 3//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 68 "html/DocTypeStrings.gperf"
      {"-//w30//dtd w3 html 2.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 35 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 3//en//3.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 23 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 3.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 25 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 3.2 final//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 24 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 3.0//en//", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 45 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 32 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 42 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 3//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 88 "html/DocTypeStrings.gperf"
      {"-/w3c/dtd html 4.0 transitional/en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
#line 21 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 43 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 3//en//3.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 69 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 3 1995-03-24//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 28 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 40 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 20 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0 strict//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 64 "html/DocTypeStrings.gperf"
      {"-//spyglass//dtd html 2.0 extended//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 33 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 2//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 41 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 2//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 36 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 17 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0 level 2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 29 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 0//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 37 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 0//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 62 "html/DocTypeStrings.gperf"
      {"-//softquad software//dtd hotmetal pro 6.0::19990601::extensions to html 4.0//en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
#line 22 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.1e//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 30 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 1//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 19 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0 strict level 2//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 71 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 3.2 final//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 73 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 3.2s draft//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 74 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 4.0 frameset//en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 65 "html/DocTypeStrings.gperf"
      {"-//sq//dtd html 2.0 hotmetal + extensions//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 38 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 1//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 57 "html/DocTypeStrings.gperf"
      {"-//netscape comm. corp.//dtd html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 31 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html level 1//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 70 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 3.2 draft//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 39 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html strict level 1//en//2.0", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 87 "html/DocTypeStrings.gperf"
      {"-//webtechs//dtd mozilla html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 85 "html/DocTypeStrings.gperf"
      {"-//w3o//dtd w3 html strict 3.0//en//", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 16 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0 level 1//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 86 "html/DocTypeStrings.gperf"
      {"-//webtechs//dtd mozilla html 2.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 59 "html/DocTypeStrings.gperf"
      {"-//o'reilly and associates//dtd html 2.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 18 "html/DocTypeStrings.gperf"
      {"-//ietf//dtd html 2.0 strict level 1//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 55 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 3.0 html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 58 "html/DocTypeStrings.gperf"
      {"-//netscape comm. corp.//dtd strict html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 56 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 3.0 tables//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 60 "html/DocTypeStrings.gperf"
      {"-//o'reilly and associates//dtd html extended 1.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 13 "html/DocTypeStrings.gperf"
      {"+//silmaril//dtd html pro v0r11 19970101//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 54 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 3.0 html strict//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 52 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 2.0 html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 76 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 4.01 frameset//en", PubIDInfo::eQuirks, PubIDInfo::eAlmostStandards},
#line 53 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 2.0 tables//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
#line 61 "html/DocTypeStrings.gperf"
      {"-//o'reilly and associates//dtd html extended relaxed 1.0//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 77 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 4.01 transitional//en", PubIDInfo::eQuirks, PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 51 "html/DocTypeStrings.gperf"
      {"-//microsoft//dtd internet explorer 2.0 html strict//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 14 "html/DocTypeStrings.gperf"
      {"-//advasoft ltd//dtd html 3.0 aswedit + extensions//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 81 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd xhtml 1.0 frameset//en", PubIDInfo::eAlmostStandards, PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 75 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html 4.0 transitional//en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
#line 82 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd xhtml 1.0 transitional//en", PubIDInfo::eAlmostStandards, PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 79 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html experimental 970421//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 50 "html/DocTypeStrings.gperf"
      {"-//metrius//dtd metrius presentational//en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 63 "html/DocTypeStrings.gperf"
      {"-//softquad//dtd hotmetal pro 4.0::19971010::extensions to html 4.0//en", PubIDInfo::eQuirks, PubIDInfo::eQuirks},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 66 "html/DocTypeStrings.gperf"
      {"-//sun microsystems corp.//dtd hotjava html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 67 "html/DocTypeStrings.gperf"
      {"-//sun microsystems corp.//dtd hotjava strict html//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 15 "html/DocTypeStrings.gperf"
      {"-//as//dtd html 3.0 aswedit + extensions//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
      {"",PubIDInfo::eAlmostStandards,PubIDInfo::eAlmostStandards},
#line 78 "html/DocTypeStrings.gperf"
      {"-//w3c//dtd html experimental 19960712//en", PubIDInfo::eQuirks3, PubIDInfo::eQuirks3}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
