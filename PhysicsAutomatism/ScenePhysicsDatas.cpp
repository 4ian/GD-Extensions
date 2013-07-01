/**

Game Develop - Physic Automatism Extension
Copyright (c) 2010-2013 Florian Rival (Florian.Rival@gmail.com)

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

#include "ScenePhysicsDatas.h"
#include "GDCpp/tinyxml/tinyxml.h"
#include "GDCpp/XmlMacros.h"
#include <iostream>

#if defined(GD_IDE_ONLY)
void ScenePhysicsDatas::SaveToXml(TiXmlElement * elem) const
{
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_FLOAT("gravityX", gravityX);
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_FLOAT("gravityY", gravityY);
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_FLOAT("scaleX", scaleX);
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_FLOAT("scaleY", scaleY);
}
#endif

void ScenePhysicsDatas::LoadFromXml(const TiXmlElement * elem)
{
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_FLOAT("gravityX", gravityX);
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_FLOAT("gravityY", gravityY);
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_FLOAT("scaleX", scaleX);
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_FLOAT("scaleY", scaleY);
}

