/**

Game Develop - Draggable Automatism Extension
Copyright (c) 2014 Florian Rival (Florian.Rival@gmail.com)

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
#include "GDCore/Tools/Localization.h"
#include "GDCore/Tools/Version.h"
#include <boost/version.hpp>
#include <iostream>

/**
 * \brief This class declares information about the JS extension.
 */
class JsExtension : public gd::PlatformExtension
{
public:

    /**
     * \brief Constructor of an extension declares everything the extension contains : Objects, actions, conditions and expressions.
     */
    JsExtension()
    {
        SetExtensionInformation("DraggableAutomatism",
                              _("Draggable Automatism"),
                              _("Automatism allowing to move objects with the mouse"),
                              "Florian Rival",
                              "zlib/libpng License ( Open Source )");

        CloneExtension("Game Develop C++ platform", "DraggableAutomatism");

        GetAutomatismMetadata("DraggableAutomatism::Draggable").SetIncludeFile("DraggableAutomatism/draggableruntimeautomatism.js");

        GetAllConditionsForAutomatism("DraggableAutomatism::Draggable")["DraggableAutomatism::Dragged"].codeExtraInformation
            .SetFunctionName("isDragged").SetIncludeFile("DraggableAutomatism/draggableruntimeautomatism.js");
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
