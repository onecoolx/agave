/*
 * Copyright (c) 2026, Zhang Ji Peng <onecoolx@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <string.h>


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "QJSSVGPathSeg.h"

#include "SVGPathSeg.h"
#include "text/String.h"

using namespace QJS;


namespace WebCore {

#define countof(x) (sizeof(x) / sizeof((x)[0]))
/* Functions table */

static JSCFunctionListEntry JSSVGPathSegAttributesFunctions[3];
static bool JSSVGPathSegAttributesFunctions_initialized = false;

static void init_JSSVGPathSegAttributesFunctions()
{
    if (JSSVGPathSegAttributesFunctions_initialized) return;
    JSSVGPathSegAttributesFunctions_initialized = true;
    memset(JSSVGPathSegAttributesFunctions, 0, sizeof(JSSVGPathSegAttributesFunctions));
    JSSVGPathSegAttributesFunctions[0].name = "constructor";
    JSSVGPathSegAttributesFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegAttributesFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegAttributesFunctions[0].magic = JSSVGPathSeg::ConstructorAttrNum;
    JSSVGPathSegAttributesFunctions[0].u.getset.get.getter_magic = JSSVGPathSeg::getValueProperty;
    JSSVGPathSegAttributesFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPathSegAttributesFunctions[1].name = "pathSegType";
    JSSVGPathSegAttributesFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegAttributesFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegAttributesFunctions[1].magic = JSSVGPathSeg::PathSegTypeAttrNum;
    JSSVGPathSegAttributesFunctions[1].u.getset.get.getter_magic = JSSVGPathSeg::getValueProperty;
    JSSVGPathSegAttributesFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPathSegAttributesFunctions[2].name = "pathSegTypeAsLetter";
    JSSVGPathSegAttributesFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegAttributesFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegAttributesFunctions[2].magic = JSSVGPathSeg::PathSegTypeAsLetterAttrNum;
    JSSVGPathSegAttributesFunctions[2].u.getset.get.getter_magic = JSSVGPathSeg::getValueProperty;
    JSSVGPathSegAttributesFunctions[2].u.getset.set.setter_magic = NULL;
}

class JSSVGPathSegConstructor {
public:
    static JSValue self(JSContext* ctx);
    static void initConstructor(JSContext * ctx, JSValue this_obj);
    static JSValue getValueProperty(JSContext*, JSValueConst this_val, int token);
};

JSValue JSSVGPathSegConstructor::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

/* Functions table for constructor */

static JSCFunctionListEntry JSSVGPathSegConstructorFunctions[20];
static bool JSSVGPathSegConstructorFunctions_initialized = false;

static void init_JSSVGPathSegConstructorFunctions()
{
    if (JSSVGPathSegConstructorFunctions_initialized) return;
    JSSVGPathSegConstructorFunctions_initialized = true;
    memset(JSSVGPathSegConstructorFunctions, 0, sizeof(JSSVGPathSegConstructorFunctions));
    JSSVGPathSegConstructorFunctions[0].name = "PATHSEG_CURVETO_CUBIC_SMOOTH_ABS";
    JSSVGPathSegConstructorFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[0].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS;
    JSSVGPathSegConstructorFunctions[0].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[1].name = "PATHSEG_MOVETO_REL";
    JSSVGPathSegConstructorFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[1].magic = SVGPathSeg::PATHSEG_MOVETO_REL;
    JSSVGPathSegConstructorFunctions[1].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[2].name = "PATHSEG_LINETO_VERTICAL_REL";
    JSSVGPathSegConstructorFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[2].magic = SVGPathSeg::PATHSEG_LINETO_VERTICAL_REL;
    JSSVGPathSegConstructorFunctions[2].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[3].name = "PATHSEG_CLOSEPATH";
    JSSVGPathSegConstructorFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[3].magic = SVGPathSeg::PATHSEG_CLOSEPATH;
    JSSVGPathSegConstructorFunctions[3].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[4].name = "PATHSEG_ARC_ABS";
    JSSVGPathSegConstructorFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[4].magic = SVGPathSeg::PATHSEG_ARC_ABS;
    JSSVGPathSegConstructorFunctions[4].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[5].name = "PATHSEG_CURVETO_CUBIC_ABS";
    JSSVGPathSegConstructorFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[5].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_ABS;
    JSSVGPathSegConstructorFunctions[5].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[6].name = "PATHSEG_LINETO_ABS";
    JSSVGPathSegConstructorFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[6].magic = SVGPathSeg::PATHSEG_LINETO_ABS;
    JSSVGPathSegConstructorFunctions[6].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[7].name = "PATHSEG_CURVETO_CUBIC_SMOOTH_REL";
    JSSVGPathSegConstructorFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[7].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL;
    JSSVGPathSegConstructorFunctions[7].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[8].name = "PATHSEG_CURVETO_CUBIC_REL";
    JSSVGPathSegConstructorFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[8].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_REL;
    JSSVGPathSegConstructorFunctions[8].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[9].name = "PATHSEG_CURVETO_QUADRATIC_ABS";
    JSSVGPathSegConstructorFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[9].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_ABS;
    JSSVGPathSegConstructorFunctions[9].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[10].name = "PATHSEG_UNKNOWN";
    JSSVGPathSegConstructorFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[10].magic = SVGPathSeg::PATHSEG_UNKNOWN;
    JSSVGPathSegConstructorFunctions[10].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[11].name = "PATHSEG_MOVETO_ABS";
    JSSVGPathSegConstructorFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[11].magic = SVGPathSeg::PATHSEG_MOVETO_ABS;
    JSSVGPathSegConstructorFunctions[11].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[12].name = "PATHSEG_LINETO_REL";
    JSSVGPathSegConstructorFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[12].magic = SVGPathSeg::PATHSEG_LINETO_REL;
    JSSVGPathSegConstructorFunctions[12].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[13].name = "PATHSEG_CURVETO_QUADRATIC_REL";
    JSSVGPathSegConstructorFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[13].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_REL;
    JSSVGPathSegConstructorFunctions[13].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[14].name = "PATHSEG_ARC_REL";
    JSSVGPathSegConstructorFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[14].magic = SVGPathSeg::PATHSEG_ARC_REL;
    JSSVGPathSegConstructorFunctions[14].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[15].name = "PATHSEG_LINETO_HORIZONTAL_ABS";
    JSSVGPathSegConstructorFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[15].magic = SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_ABS;
    JSSVGPathSegConstructorFunctions[15].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[16].name = "PATHSEG_LINETO_HORIZONTAL_REL";
    JSSVGPathSegConstructorFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[16].magic = SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_REL;
    JSSVGPathSegConstructorFunctions[16].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[16].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[17].name = "PATHSEG_LINETO_VERTICAL_ABS";
    JSSVGPathSegConstructorFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[17].magic = SVGPathSeg::PATHSEG_LINETO_VERTICAL_ABS;
    JSSVGPathSegConstructorFunctions[17].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[17].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[18].name = "PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS";
    JSSVGPathSegConstructorFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[18].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS;
    JSSVGPathSegConstructorFunctions[18].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[18].u.getset.set.setter_magic = NULL;
    JSSVGPathSegConstructorFunctions[19].name = "PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL";
    JSSVGPathSegConstructorFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegConstructorFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegConstructorFunctions[19].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL;
    JSSVGPathSegConstructorFunctions[19].u.getset.get.getter_magic = JSSVGPathSegConstructor::getValueProperty;
    JSSVGPathSegConstructorFunctions[19].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGPathSegConstructor::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[SVGPathSeg.constructor]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegConstructor::initConstructor(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[SVGPathSeg.constructor]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegConstructor::initConstructor(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegConstructorFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegConstructorFunctions, countof(JSSVGPathSegConstructorFunctions));
}

/* Functions table */

static JSCFunctionListEntry JSSVGPathSegPrototypeConstantsFunctions[20];
static bool JSSVGPathSegPrototypeConstantsFunctions_initialized = false;

static void init_JSSVGPathSegPrototypeConstantsFunctions()
{
    if (JSSVGPathSegPrototypeConstantsFunctions_initialized) return;
    JSSVGPathSegPrototypeConstantsFunctions_initialized = true;
    memset(JSSVGPathSegPrototypeConstantsFunctions, 0, sizeof(JSSVGPathSegPrototypeConstantsFunctions));
    JSSVGPathSegPrototypeConstantsFunctions[0].name = "PATHSEG_CURVETO_CUBIC_SMOOTH_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[0].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[0].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[0].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[0].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[0].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[1].name = "PATHSEG_MOVETO_REL";
    JSSVGPathSegPrototypeConstantsFunctions[1].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[1].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[1].magic = SVGPathSeg::PATHSEG_MOVETO_REL;
    JSSVGPathSegPrototypeConstantsFunctions[1].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[1].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[2].name = "PATHSEG_LINETO_VERTICAL_REL";
    JSSVGPathSegPrototypeConstantsFunctions[2].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[2].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[2].magic = SVGPathSeg::PATHSEG_LINETO_VERTICAL_REL;
    JSSVGPathSegPrototypeConstantsFunctions[2].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[2].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[3].name = "PATHSEG_CLOSEPATH";
    JSSVGPathSegPrototypeConstantsFunctions[3].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[3].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[3].magic = SVGPathSeg::PATHSEG_CLOSEPATH;
    JSSVGPathSegPrototypeConstantsFunctions[3].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[3].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[4].name = "PATHSEG_ARC_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[4].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[4].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[4].magic = SVGPathSeg::PATHSEG_ARC_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[4].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[4].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[5].name = "PATHSEG_CURVETO_CUBIC_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[5].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[5].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[5].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[5].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[5].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[6].name = "PATHSEG_LINETO_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[6].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[6].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[6].magic = SVGPathSeg::PATHSEG_LINETO_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[6].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[6].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[7].name = "PATHSEG_CURVETO_CUBIC_SMOOTH_REL";
    JSSVGPathSegPrototypeConstantsFunctions[7].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[7].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[7].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_SMOOTH_REL;
    JSSVGPathSegPrototypeConstantsFunctions[7].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[7].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[8].name = "PATHSEG_CURVETO_CUBIC_REL";
    JSSVGPathSegPrototypeConstantsFunctions[8].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[8].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[8].magic = SVGPathSeg::PATHSEG_CURVETO_CUBIC_REL;
    JSSVGPathSegPrototypeConstantsFunctions[8].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[8].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[9].name = "PATHSEG_CURVETO_QUADRATIC_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[9].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[9].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[9].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[9].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[9].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[10].name = "PATHSEG_UNKNOWN";
    JSSVGPathSegPrototypeConstantsFunctions[10].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[10].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[10].magic = SVGPathSeg::PATHSEG_UNKNOWN;
    JSSVGPathSegPrototypeConstantsFunctions[10].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[10].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[11].name = "PATHSEG_MOVETO_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[11].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[11].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[11].magic = SVGPathSeg::PATHSEG_MOVETO_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[11].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[11].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[12].name = "PATHSEG_LINETO_REL";
    JSSVGPathSegPrototypeConstantsFunctions[12].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[12].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[12].magic = SVGPathSeg::PATHSEG_LINETO_REL;
    JSSVGPathSegPrototypeConstantsFunctions[12].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[12].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[13].name = "PATHSEG_CURVETO_QUADRATIC_REL";
    JSSVGPathSegPrototypeConstantsFunctions[13].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[13].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[13].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_REL;
    JSSVGPathSegPrototypeConstantsFunctions[13].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[13].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[14].name = "PATHSEG_ARC_REL";
    JSSVGPathSegPrototypeConstantsFunctions[14].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[14].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[14].magic = SVGPathSeg::PATHSEG_ARC_REL;
    JSSVGPathSegPrototypeConstantsFunctions[14].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[14].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[15].name = "PATHSEG_LINETO_HORIZONTAL_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[15].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[15].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[15].magic = SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[15].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[15].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[16].name = "PATHSEG_LINETO_HORIZONTAL_REL";
    JSSVGPathSegPrototypeConstantsFunctions[16].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[16].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[16].magic = SVGPathSeg::PATHSEG_LINETO_HORIZONTAL_REL;
    JSSVGPathSegPrototypeConstantsFunctions[16].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[16].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[17].name = "PATHSEG_LINETO_VERTICAL_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[17].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[17].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[17].magic = SVGPathSeg::PATHSEG_LINETO_VERTICAL_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[17].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[17].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[18].name = "PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS";
    JSSVGPathSegPrototypeConstantsFunctions[18].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[18].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[18].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS;
    JSSVGPathSegPrototypeConstantsFunctions[18].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[18].u.getset.set.setter_magic = NULL;
    JSSVGPathSegPrototypeConstantsFunctions[19].name = "PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL";
    JSSVGPathSegPrototypeConstantsFunctions[19].prop_flags = JS_PROP_CONFIGURABLE;
    JSSVGPathSegPrototypeConstantsFunctions[19].def_type = JS_DEF_CGETSET_MAGIC;
    JSSVGPathSegPrototypeConstantsFunctions[19].magic = SVGPathSeg::PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL;
    JSSVGPathSegPrototypeConstantsFunctions[19].u.getset.get.getter_magic = JSSVGPathSegPrototype::getValueProperty;
    JSSVGPathSegPrototypeConstantsFunctions[19].u.getset.set.setter_magic = NULL;
}

JSValue JSSVGPathSegPrototype::self(JSContext * ctx)
{
    JSValue globalObj = JS_GetGlobalObject(ctx);
    JSValue obj = JS_GetPropertyStr(ctx, globalObj, "[[JSSVGPathSeg.prototype]]");
    if (JS_IsUndefined(obj)) {
        obj = JS_NewObject(ctx);
        JSSVGPathSegPrototype::initPrototype(ctx, obj);
        JS_SetPropertyStr(ctx, globalObj, "[[JSSVGPathSeg.prototype]]", obj);
        obj = JS_DupValue(ctx, obj);
    }
    JS_FreeValue(ctx, globalObj);
    return obj;
}

void JSSVGPathSegPrototype::initPrototype(JSContext * ctx, JSValue this_obj)
{
    init_JSSVGPathSegAttributesFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegAttributesFunctions, countof(JSSVGPathSegAttributesFunctions));
    init_JSSVGPathSegPrototypeConstantsFunctions();
    JS_SetPropertyFunctionList(ctx, this_obj, JSSVGPathSegPrototypeConstantsFunctions, countof(JSSVGPathSegPrototypeConstantsFunctions));
}

JSValue JSSVGPathSegPrototype::getValueProperty(JSContext * ctx, JSValueConst this_val, int token)
{
    // The token is the numeric value of its associated constant
    return JS_NewInt32(ctx, token);
}

static JSClassDef JSSVGPathSegClassDefine;
static bool JSSVGPathSegClassDefine_initialized = false;

static void init_JSSVGPathSegClassDefine()
{
    if (JSSVGPathSegClassDefine_initialized) return;
    JSSVGPathSegClassDefine_initialized = true;
    memset(&JSSVGPathSegClassDefine, 0, sizeof(JSSVGPathSegClassDefine));
    JSSVGPathSegClassDefine.class_name = "SVGPathSeg";
    JSSVGPathSegClassDefine.finalizer = JSSVGPathSeg::finalizer;
    JSSVGPathSegClassDefine.gc_mark = JSSVGPathSeg::mark;
}

JSClassID JSSVGPathSeg::js_class_id = 0;

void JSSVGPathSeg::init(JSContext* ctx)
{
    if (JSSVGPathSeg::js_class_id == 0) {
        init_JSSVGPathSegClassDefine();
        JS_NewClassID(&JSSVGPathSeg::js_class_id);
        JS_NewClass(JS_GetRuntime(ctx), JSSVGPathSeg::js_class_id, &JSSVGPathSegClassDefine);
        JS_SetConstructor(ctx, JSSVGPathSegConstructor::self(ctx), JSSVGPathSegPrototype::self(ctx));
        JS_SetClassProto(ctx, JSSVGPathSeg::js_class_id, JSSVGPathSegPrototype::self(ctx));
    }
}

JSValue JSSVGPathSeg::create(JSContext* ctx, SVGPathSeg* impl, SVGElement* context)
{
    JSSVGPathSeg::init(ctx);
    JSValue _proto = JSSVGPathSegPrototype::self(ctx);
    JSValue obj = JS_NewObjectProtoClass(ctx, _proto, JSSVGPathSeg::js_class_id);
    JS_FreeValue(ctx, _proto);
    if (JS_IsException(obj)) {
        return JS_EXCEPTION;
    }
    JS_SetOpaque(obj, impl);
    impl->ref();
    return obj;
}

void JSSVGPathSeg::finalizer(JSRuntime* rt, JSValue val)
{
    SVGPathSeg* impl = (SVGPathSeg*)JS_GetOpaque(val, JSSVGPathSeg::js_class_id);
    if (!impl)
        return;
    ScriptInterpreter::forgetDOMObject(impl);
    impl->deref();
}

void JSSVGPathSeg::mark(JSRuntime *rt, JSValueConst val, JS_MarkFunc *mark_func)
{
    JS_MarkValue(rt, val, mark_func);
}

JSValue JSSVGPathSeg::getValueProperty(JSContext *ctx, JSValueConst this_val, int token)
{
    switch (token) {
        case PathSegTypeAttrNum: {
            SVGPathSeg* imp = (SVGPathSeg*)JS_GetOpaque(this_val, JSSVGPathSeg::js_class_id);
            return JS_NewInt32(ctx, imp->pathSegType());
        }
        case PathSegTypeAsLetterAttrNum: {
            SVGPathSeg* imp = (SVGPathSeg*)JS_GetOpaque(this_val, JSSVGPathSeg::js_class_id);
            return JS_NewString(ctx, ((const String&)imp->pathSegTypeAsLetter()).utf8().data());
        }
        case ConstructorAttrNum:
            return getConstructor(ctx);
    }
    return JS_NULL;
}

JSValue JSSVGPathSeg::getConstructor(JSContext *ctx)
{
    return JSSVGPathSegConstructor::self(ctx);
}


SVGPathSeg* toSVGPathSeg(JSValue val)
{
    if (JS_IsObject(val)) {
        SVGPathSeg* impl = (SVGPathSeg*)JS_GetOpaque(val, JSSVGPathSeg::js_class_id);
        return impl;
    } else {
        return 0;
    }
}

}

#endif // ENABLE(SVG)
