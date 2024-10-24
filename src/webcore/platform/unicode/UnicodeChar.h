
#ifndef _UnicodeChar_h
#define _UnicodeChar_h

#include  "UChar.h"
#include  "UnicodeGlobal.h"

#define U_GET_GC_MASK(c) U_MASK(u_charType(c))

typedef enum {
    UBLOCK_NO_BLOCK = 0, /*[none]*/ /* Special range indicating No_Block */
    UBLOCK_BASIC_LATIN = 1, /*[0000]*/ /*See note !!*/
    UBLOCK_LATIN_1_SUPPLEMENT=2, /*[0080]*/
    UBLOCK_LATIN_EXTENDED_A =3, /*[0100]*/
    UBLOCK_LATIN_EXTENDED_B =4, /*[0180]*/
    UBLOCK_IPA_EXTENSIONS =5, /*[0250]*/
    UBLOCK_SPACING_MODIFIER_LETTERS =6, /*[02B0]*/
    UBLOCK_COMBINING_DIACRITICAL_MARKS =7, /*[0300]*/
    UBLOCK_GREEK =8, /*[0370]*/
    UBLOCK_CYRILLIC =9, /*[0400]*/
    UBLOCK_ARMENIAN =10, /*[0530]*/
    UBLOCK_HEBREW =11, /*[0590]*/
    UBLOCK_ARABIC =12, /*[0600]*/
    UBLOCK_SYRIAC =13, /*[0700]*/
    UBLOCK_THAANA =14, /*[0780]*/
    UBLOCK_DEVANAGARI =15, /*[0900]*/
    UBLOCK_BENGALI =16, /*[0980]*/
    UBLOCK_GURMUKHI =17, /*[0A00]*/
    UBLOCK_GUJARATI =18, /*[0A80]*/
    UBLOCK_ORIYA =19, /*[0B00]*/
    UBLOCK_TAMIL =20, /*[0B80]*/
    UBLOCK_TELUGU =21, /*[0C00]*/
    UBLOCK_KANNADA =22, /*[0C80]*/
    UBLOCK_MALAYALAM =23, /*[0D00]*/
    UBLOCK_SINHALA =24, /*[0D80]*/
    UBLOCK_THAI =25, /*[0E00]*/
    UBLOCK_LAO =26, /*[0E80]*/
    UBLOCK_TIBETAN =27, /*[0F00]*/
    UBLOCK_MYANMAR =28, /*[1000]*/
    UBLOCK_GEORGIAN =29, /*[10A0]*/
    UBLOCK_HANGUL_JAMO =30, /*[1100]*/
    UBLOCK_ETHIOPIC =31, /*[1200]*/
    UBLOCK_CHEROKEE =32, /*[13A0]*/
    UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS =33, /*[1400]*/
    UBLOCK_OGHAM =34, /*[1680]*/
    UBLOCK_RUNIC =35, /*[16A0]*/
    UBLOCK_KHMER =36, /*[1780]*/
    UBLOCK_MONGOLIAN =37, /*[1800]*/
    UBLOCK_LATIN_EXTENDED_ADDITIONAL =38, /*[1E00]*/
    UBLOCK_GREEK_EXTENDED =39, /*[1F00]*/
    UBLOCK_GENERAL_PUNCTUATION =40, /*[2000]*/
    UBLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS =41, /*[2070]*/
    UBLOCK_CURRENCY_SYMBOLS =42, /*[20A0]*/
    UBLOCK_COMBINING_MARKS_FOR_SYMBOLS =43, /*[20D0]*/
    UBLOCK_LETTERLIKE_SYMBOLS =44, /*[2100]*/
    UBLOCK_NUMBER_FORMS =45, /*[2150]*/
    UBLOCK_ARROWS =46, /*[2190]*/
    UBLOCK_MATHEMATICAL_OPERATORS =47, /*[2200]*/
    UBLOCK_MISCELLANEOUS_TECHNICAL =48, /*[2300]*/
    UBLOCK_CONTROL_PICTURES =49, /*[2400]*/
    UBLOCK_OPTICAL_CHARACTER_RECOGNITION =50, /*[2440]*/
    UBLOCK_ENCLOSED_ALPHANUMERICS =51, /*[2460]*/
    UBLOCK_BOX_DRAWING =52, /*[2500]*/
    UBLOCK_BLOCK_ELEMENTS =53, /*[2580]*/
    UBLOCK_GEOMETRIC_SHAPES =54, /*[25A0]*/
    UBLOCK_MISCELLANEOUS_SYMBOLS =55, /*[2600]*/
    UBLOCK_DINGBATS =56, /*[2700]*/
    UBLOCK_BRAILLE_PATTERNS =57, /*[2800]*/
    UBLOCK_CJK_RADICALS_SUPPLEMENT =58, /*[2E80]*/
    UBLOCK_KANGXI_RADICALS =59, /*[2F00]*/
    UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS =60, /*[2FF0]*/
    UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION =61, /*[3000]*/
    UBLOCK_HIRAGANA =62, /*[3040]*/
    UBLOCK_KATAKANA =63, /*[30A0]*/
    UBLOCK_BOPOMOFO =64, /*[3100]*/
    UBLOCK_HANGUL_COMPATIBILITY_JAMO =65, /*[3130]*/
    UBLOCK_KANBUN =66, /*[3190]*/
    UBLOCK_BOPOMOFO_EXTENDED =67, /*[31A0]*/
    UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS =68, /*[3200]*/
    UBLOCK_CJK_COMPATIBILITY =69, /*[3300]*/
    UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A =70, /*[3400]*/
    UBLOCK_CJK_UNIFIED_IDEOGRAPHS =71, /*[4E00]*/
    UBLOCK_YI_SYLLABLES =72, /*[A000]*/
    UBLOCK_YI_RADICALS =73, /*[A490]*/
    UBLOCK_HANGUL_SYLLABLES =74, /*[AC00]*/
    UBLOCK_HIGH_SURROGATES =75, /*[D800]*/
    UBLOCK_HIGH_PRIVATE_USE_SURROGATES =76, /*[DB80]*/
    UBLOCK_LOW_SURROGATES =77, /*[DC00]*/
    UBLOCK_PRIVATE_USE = 78,
    UBLOCK_PRIVATE_USE_AREA =UBLOCK_PRIVATE_USE, /*[E000]*/
    UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS =79, /*[F900]*/
    UBLOCK_ALPHABETIC_PRESENTATION_FORMS =80, /*[FB00]*/
    UBLOCK_ARABIC_PRESENTATION_FORMS_A =81, /*[FB50]*/
    UBLOCK_COMBINING_HALF_MARKS =82, /*[FE20]*/
    UBLOCK_CJK_COMPATIBILITY_FORMS =83, /*[FE30]*/
    UBLOCK_SMALL_FORM_VARIANTS =84, /*[FE50]*/
    UBLOCK_ARABIC_PRESENTATION_FORMS_B =85, /*[FE70]*/
    UBLOCK_SPECIALS =86, /*[FFF0]*/
    UBLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS =87, /*[FF00]*/
    UBLOCK_OLD_ITALIC = 88  , /*[10300]*/
    UBLOCK_GOTHIC = 89 , /*[10330]*/
    UBLOCK_DESERET = 90 , /*[10400]*/
    UBLOCK_BYZANTINE_MUSICAL_SYMBOLS = 91 , /*[1D000]*/
    UBLOCK_MUSICAL_SYMBOLS = 92 , /*[1D100]*/
    UBLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS = 93  , /*[1D400]*/
    UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B  = 94 , /*[20000]*/
    UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT = 95 , /*[2F800]*/
    UBLOCK_TAGS = 96, /*[E0000]*/
    UBLOCK_CYRILLIC_SUPPLEMENTARY = 97, 
    UBLOCK_CYRILLIC_SUPPLEMENT = UBLOCK_CYRILLIC_SUPPLEMENTARY, /*[0500]*/
    UBLOCK_TAGALOG = 98, /*[1700]*/
    UBLOCK_HANUNOO = 99, /*[1720]*/
    UBLOCK_BUHID = 100, /*[1740]*/
    UBLOCK_TAGBANWA = 101, /*[1760]*/
    UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A = 102, /*[27C0]*/
    UBLOCK_SUPPLEMENTAL_ARROWS_A = 103, /*[27F0]*/
    UBLOCK_SUPPLEMENTAL_ARROWS_B = 104, /*[2900]*/
    UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B = 105, /*[2980]*/
    UBLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS = 106, /*[2A00]*/
    UBLOCK_KATAKANA_PHONETIC_EXTENSIONS = 107, /*[31F0]*/
    UBLOCK_VARIATION_SELECTORS = 108, /*[FE00]*/
    UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A = 109, /*[F0000]*/
    UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B = 110, /*[100000]*/
    UBLOCK_LIMBU = 111, /*[1900]*/
    UBLOCK_TAI_LE = 112, /*[1950]*/
    UBLOCK_KHMER_SYMBOLS = 113, /*[19E0]*/
    UBLOCK_PHONETIC_EXTENSIONS = 114, /*[1D00]*/
    UBLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS = 115, /*[2B00]*/
    UBLOCK_YIJING_HEXAGRAM_SYMBOLS = 116, /*[4DC0]*/
    UBLOCK_LINEAR_B_SYLLABARY = 117, /*[10000]*/
    UBLOCK_LINEAR_B_IDEOGRAMS = 118, /*[10080]*/
    UBLOCK_AEGEAN_NUMBERS = 119, /*[10100]*/
    UBLOCK_UGARITIC = 120, /*[10380]*/
    UBLOCK_SHAVIAN = 121, /*[10450]*/
    UBLOCK_OSMANYA = 122, /*[10480]*/
    UBLOCK_CYPRIOT_SYLLABARY = 123, /*[10800]*/
    UBLOCK_TAI_XUAN_JING_SYMBOLS = 124, /*[1D300]*/
    UBLOCK_VARIATION_SELECTORS_SUPPLEMENT = 125, /*[E0100]*/
    UBLOCK_COUNT = 155,
    UBLOCK_INVALID_CODE=-1
}UBlockCode;


int8_t u_charType(USChar32 c);

UBool u_isprint(USChar32 c);
UBool u_isdigit(USChar32 c);
int32_t u_charDigitValue(USChar32 c);
UBool u_ispunct(USChar32 c);
UBool u_islower(USChar32 c);
uint32_t u_getUnicodeProperties(USChar32 c, int32_t column);
UBool u_isblank(USChar32 c);
UBool u_isgraphPOSIX(USChar32 c);
UBool u_isprintPOSIX(USChar32 c);
UBool u_isxdigit(USChar32 c);
UBool u_isalnumPOSIX(USChar32 c);

UHangulSyllableType uchar_getHST(USChar32 c);

UBlockCode ublock_getCode(USChar32 c);

UScriptCode uscript_getScript(USChar32 c, UErrorCode *pErrorCode);

#endif
