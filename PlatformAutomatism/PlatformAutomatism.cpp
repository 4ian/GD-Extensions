/**

Game Develop - Platform Automatism Extension
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

#include <boost/shared_ptr.hpp>
#include "PlatformAutomatism.h"
#include "GDCpp/Scene.h"
#include "GDCpp/tinyxml/tinyxml.h"
#include "GDCpp/XmlMacros.h"
#include "GDCpp/RuntimeScene.h"
#include "GDCpp/RuntimeObject.h"
#include "GDCpp/CommonTools.h"
#include "ScenePlatformObjectsManager.h"
#if defined(GD_IDE_ONLY)
#include <iostream>
#include <map>
#include "GDCore/Tools/Localization.h"
#include "GDCore/IDE/Dialogs/PropgridPropertyDescriptor.h"
#endif


PlatformAutomatism::PlatformAutomatism() :
    parentScene(NULL),
    sceneManager(NULL),
    registeredInManager(false),
    platformType(NormalPlatform)
{
}

PlatformAutomatism::~PlatformAutomatism()
{
    if ( sceneManager && registeredInManager ) sceneManager->RemovePlatform(this);
}

void PlatformAutomatism::DoStepPreEvents(RuntimeScene & scene)
{
    if ( parentScene != &scene ) //Parent scene has changed
    {
        if ( sceneManager ) //Remove the object from any old scene manager.
            sceneManager->RemovePlatform(this);

        parentScene = &scene;
        sceneManager = parentScene ? &ScenePlatformObjectsManager::managers[&scene] : NULL;
        registeredInManager = false;
    }

    if (!activated && registeredInManager)
    {
        if ( sceneManager ) sceneManager->RemovePlatform(this);
        registeredInManager = false;
    }
    else if (activated && !registeredInManager)
    {
        if ( sceneManager )
        {
            sceneManager->AddPlatform(this);
            registeredInManager = true;
        }
    }
}

void PlatformAutomatism::DoStepPostEvents(RuntimeScene & scene)
{

}

void PlatformAutomatism::ChangePlatformType(const std::string & platformType_)
{
    if ( platformType_ == "Ladder" ) platformType = Ladder;
    else if ( platformType_ == "Jumpthru" ) platformType = Jumpthru;
    else platformType = NormalPlatform;
}

void PlatformAutomatism::OnActivate()
{
    if ( sceneManager )
    {
        sceneManager->AddPlatform(this);
        registeredInManager = true;
    }
}

void PlatformAutomatism::OnDeActivate()
{
    if ( sceneManager )
        sceneManager->RemovePlatform(this);

    registeredInManager = false;
}

void PlatformAutomatism::LoadFromXml(const TiXmlElement * elem)
{
    std::string platformTypeStr;
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_STRING("platformType", platformTypeStr);
    platformType = platformTypeStr == "Ladder" ?  Ladder : (platformTypeStr == "Jumpthru" ?
        Jumpthru : NormalPlatform);
}

#if defined(GD_IDE_ONLY)
void PlatformAutomatism::SaveToXml(TiXmlElement * elem) const
{
    if ( platformType == Ladder)
        GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE("platformType", "Ladder")
    else if ( platformType == Jumpthru )
        GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE("platformType", "Jumpthru")
    else
        GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE("platformType", "NormalPlatform")
}

std::map<std::string, gd::PropgridPropertyDescriptor> PlatformAutomatism::GetProperties(gd::Project & project) const
{
    std::map<std::string, gd::PropgridPropertyDescriptor> properties;

    std::string platformTypeStr = ToString(_("Platform"));
    if ( platformType == Ladder)
        platformTypeStr = ToString(_("Ladder"));
    else if ( platformType == Jumpthru )
        platformTypeStr = ToString(_("Jumpthru platform"));

    properties[ToString(_("Type"))]
        .SetValue(platformTypeStr)
        .SetType("Choice")
        .AddExtraInfo(ToString(_("Platform")))
        .AddExtraInfo(ToString(_("Jumpthru platform")))
        .AddExtraInfo(ToString(_("Ladder")));

    return properties;
}

bool PlatformAutomatism::UpdateProperty(const std::string & name, const std::string & value, gd::Project & project)
{
    if ( name == ToString(_("Type")) )
    {
        if ( value == "1" ) //1 = Position in the choice list ( See GetProperties ).
            platformType = Jumpthru;
        else if ( value == "2" )
            platformType = Ladder;
        else
            platformType = NormalPlatform;

        return true;
    }

    return false;
}

#endif