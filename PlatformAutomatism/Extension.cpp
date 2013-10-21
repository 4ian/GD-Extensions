/**

Game Develop - Platform Automatism Extension
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

#include "GDCpp/ExtensionBase.h"
#include "GDCore/Tools/Version.h"
#include "PlatformerObjectAutomatism.h"
#include "PlatformAutomatism.h"
#include "ScenePlatformObjectsManager.h"
#include <boost/version.hpp>

/**
 * \brief This class declares information about the extension.
 */
class Extension : public ExtensionBase
{
public:

    /**
     * Constructor of an extension declares everything the extension contains : Objects, actions, conditions and expressions.
     */
    Extension()
    {
        SetExtensionInformation("PlatformAutomatism",
                              _("Platform Automatism"),
                              _("TODO"),
                              "Florian Rival",
                              "zlib/libpng License ( Open Source )");

        {
            gd::AutomatismMetadata & aut = AddAutomatism("PlatformerObjectAutomatism",
                  _("Platformer object"),
                  _("PlatformerObject"),
                  _("Controllable object which can jump and run on platforms."),
                  "",
                  "CppPlatform/Extensions/platformerobjecticon.png",
                  "PlatformerObjectAutomatism",
                  boost::shared_ptr<gd::Automatism>(new PlatformerObjectAutomatism),
                  boost::shared_ptr<gd::AutomatismsSharedData>(new gd::AutomatismsSharedData));

            #if defined(GD_IDE_ONLY)

            aut.SetIncludeFile("PlatformAutomatism/PlatformerObjectAutomatism.h");

            aut.AddAction("SetReverseAtEnd",
                           _("De/activate rounding"),
                           _("Activate or desactivate rounding"),
                           _("Activate rounding for _PARAM0_: _PARAM2_"),
                           _("Options"),
                           "CppPlatform/Extensions/platformerobjecticon24.png",
                           "CppPlatform/Extensions/platformerobjecticon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PlatformerObjectAutomatism", false)
                .AddParameter("yesorno", _("Activate"))
                .codeExtraInformation.SetFunctionName("SetReverseAtEnd").SetIncludeFile("PlatformAutomatism/PlatformerObjectAutomatism.h");
            #endif
        }
        {
            gd::AutomatismMetadata & aut = AddAutomatism("PlatformAutomatism",
                  _("Platform"),
                  _("Platform"),
                  _("Platform on which Platformer objects can run."),
                  "",
                  "CppPlatform/Extensions/platformicon.png",
                  "PlatformAutomatism",
                  boost::shared_ptr<gd::Automatism>(new PlatformAutomatism),
                  boost::shared_ptr<gd::AutomatismsSharedData>(new gd::AutomatismsSharedData));

            #if defined(GD_IDE_ONLY)
            aut.SetIncludeFile("PlatformAutomatism/PlatformAutomatism.h");
            #endif
        }

        GD_COMPLETE_EXTENSION_COMPILATION_INFORMATION();
    };

    /**
     * \brief Initialize platforms list of the scene
     */
    virtual void SceneLoaded(RuntimeScene & scene)
    {
        ScenePlatformObjectsManager emptyManager;
        ScenePlatformObjectsManager::managers[&scene] = emptyManager;
    }

    /**
     * \brief Destroy platforms list of the scene
     */
    virtual void SceneUnloaded(RuntimeScene & scene)
    {
        ScenePlatformObjectsManager::managers.erase(&scene);
    }

    virtual ~Extension() {};
};

/**
 * Used by Game Develop to create the extension class
 * -- Do not need to be modified. --
 */
extern "C" ExtensionBase * GD_EXTENSION_API CreateGDExtension() {
    return new Extension;
}

/**
 * Used by Game Develop to destroy the extension class
 * -- Do not need to be modified. --
 */
extern "C" void GD_EXTENSION_API DestroyGDExtension(ExtensionBase * p) {
    delete p;
}

