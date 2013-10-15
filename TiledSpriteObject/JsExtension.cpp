/**

Game Develop - Tiled Sprite Extension
Copyright (c) 2012 Victor Levasseur (victorlevasseur01@orange.fr)
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.

*/
#if defined(GD_IDE_ONLY)
#include "GDCore/PlatformDefinition/PlatformExtension.h"
#include "GDCore/Tools/Version.h"
#include "TiledSpriteObject.h"
#include <boost/version.hpp>
#include <iostream>
#include <wx/intl.h>
//Ensure the wxWidgets macro "_" returns a std::string
#if defined(_)
    #undef _
#endif
#define _(s) std::string(wxGetTranslation((s)).mb_str())

/**
 * \brief This class declares information about the JS extension.
 */
class JsExtension : public gd::PlatformExtension
{
public:

    /**
     * Constructor of an extension declares everything the extension contains : Objects, actions, conditions and expressions.
     */
    JsExtension()
    {
        SetExtensionInformation("TiledSpriteObject",
                              _("Tiled Sprite Object"),
                              _("Extension allowing to use tiled sprite objects."),
                              "Victor Levasseur and Florian Rival",
                              "zlib/libpng License ( Open Source )");

        CloneExtension("Game Develop C++ platform", "TiledSpriteObject");

        GetObjectMetadata("TiledSpriteObject::TiledSprite").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");

        GetAllActionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Width"].codeExtraInformation
            .SetFunctionName("setWidth").SetAssociatedGetter("getWidth").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllConditionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Width"].codeExtraInformation
            .SetFunctionName("getWidth").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllActionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Height"].codeExtraInformation
            .SetFunctionName("setHeight").SetAssociatedGetter("getHeight").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllConditionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Height"].codeExtraInformation
            .SetFunctionName("getHeight").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllActionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Angle"].codeExtraInformation
            .SetFunctionName("setAngle").SetAssociatedGetter("getAngle").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllConditionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::Angle"].codeExtraInformation
            .SetFunctionName("getAngle").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllActionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::XOffset"].codeExtraInformation
            .SetFunctionName("setXOffset").SetAssociatedGetter("getXOffset").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllConditionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::XOffset"].codeExtraInformation
            .SetFunctionName("getXOffset").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllActionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::YOffset"].codeExtraInformation
            .SetFunctionName("setYOffset").SetAssociatedGetter("getYOffset").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");
        GetAllConditionsForObject("TiledSpriteObject::TiledSprite")["TiledSpriteObject::YOffset"].codeExtraInformation
            .SetFunctionName("getYOffset").SetIncludeFile("TiledSpriteObject/tiledspriteruntimeobject.js");

        StripUnimplementedInstructionsAndExpressions(); //Unimplemented things are listed here:
    };
    virtual ~JsExtension() {};
};

/**
 * Used by Game Develop to create the extension class
 * -- Do not need to be modified. --
 */
extern "C" gd::PlatformExtension * GD_EXTENSION_API CreateGDJSExtension() {
    return new JsExtension;
}

/**
 * Used by Game Develop to destroy the extension class
 * -- Do not need to be modified. --
 */
extern "C" void GD_EXTENSION_API DestroyGDJSExtension(gd::PlatformExtension * p) {
    delete p;
}
#endif
