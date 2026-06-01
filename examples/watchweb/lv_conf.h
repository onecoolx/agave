/**
 * @file lv_conf.h
 * LVGL configuration for WatchWeb (480x480 circular smartwatch browser)
 */

#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 32

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

/*====================
   HAL SETTINGS
 *====================*/
#define LV_DEF_REFR_PERIOD  33      /*[ms]*/
#define LV_DPI_DEF 160              /*[px/inch]*/

/*========================
 * RENDERING CONFIGURATION
 *========================*/
#define LV_DRAW_BUF_STRIDE_ALIGN    4
#define LV_DRAW_BUF_ALIGN           4
#define LV_USE_DRAW_SW_ASM          0  /*0=NONE, 1=NEON, 2=HELIUM*/

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/
#define LV_USE_OS   LV_OS_NONE

/*Logging*/
#define LV_USE_LOG 0

/*Asserts*/
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/*=====================
 *  FONT USAGE
 *====================*/
#define LV_FONT_MONTSERRAT_14    1
#define LV_FONT_MONTSERRAT_16    1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*==================
 * WIDGETS
 *================*/
#define LV_USE_ARC        1
#define LV_USE_BUTTON     1
#define LV_USE_CANVAS     1
#define LV_USE_IMAGE      1
#define LV_USE_LABEL      1
#define LV_USE_TEXTAREA   1
#define LV_USE_KEYBOARD   1

/*==================
 * DRIVERS
 *================*/

/*Use SDL to open window on PC*/
#define LV_USE_SDL              1
#if LV_USE_SDL
    #define LV_SDL_INCLUDE_PATH     <SDL2/SDL.h>
    #define LV_SDL_RENDER_MODE      LV_DISPLAY_RENDER_MODE_DIRECT
    #define LV_SDL_BUF_COUNT        2
    #define LV_SDL_FULLSCREEN       0
    #define LV_SDL_DIRECT_EXIT      1
#endif

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
