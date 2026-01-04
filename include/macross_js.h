
#ifndef _MACROSS_JAVASCRIPT_H_
#define _MACROSS_JAVASCRIPT_H_

#include "mconfig.h"

#ifdef _EXPORT_API_
    #if defined(WIN32)
        #ifdef _EXPORT_
            #define MACROSS_API __declspec(dllexport)
        #else
            #define MACROSS_API __declspec(dllimport)
        #endif
    #else
        #if (defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))) || (defined(__clang__))
            #define MACROSS_API __attribute__((visibility("default")))
        #else
            #define MACROSS_API
        #endif
    #endif
#else
    #define MACROSS_API
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup extension Extension
 * @{
 */
#ifdef ENABLE_JSNATIVEBINDING
/**
 * \fn MC_BOOL macross_js_define_class(JSNativeClass *jclass)
 * \brief Define a javascript native binding class.
 *
 * \param jclass The description of the javascript class.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa macross_js_undefine_class
 */
MACROSS_API MC_BOOL macross_js_define_class(JSNativeClass* jclass);

/**
 * \fn MC_BOOL macross_js_undefine_class(JSNativeClass* jclass)
 * \brief Undefine a javascript native binding class.
 *
 * \param jclass The description of the javascript class.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa macross_js_define_class
 */
MACROSS_API MC_BOOL macross_js_undefine_class(JSNativeClass* jclass);

/**
 * \fn const JSClassRef macross_js_lookup_class(const char* utf8_name)
 * \brief search a javascript native binding with the class name.
 *
 * \param utf8_name The name of the native class.
 *
 * \return JSClassRef on success, NULL on error.
 */
MACROSS_API const JSClassRef macross_js_lookup_class(const char* utf8_name);

/**
 * \fn MC_BOOL macross_js_define_function(JSNativeFunction *func)
 * \brief Define a javascript native binding function.
 *
 * \param func The description of the Native Function
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa macross_js_undefine_function
 */
MACROSS_API MC_BOOL macross_js_define_function(JSNativeFunction* func);

/**
 * \fn MC_BOOL macross_js_undefine_function(JSNativeFunction *func)
 * \brief Undefine a javascript native binding function.
 *
 * \param func The description of the Native Function
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa macross_js_define_function
 */
MACROSS_API MC_BOOL macross_js_undefine_function(JSNativeFunction* func);
#endif

/** @} end of extension*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_MACROSS_JAVASCRIPT_H_*/
