/**

Game Develop - A Star Automatism Extension
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

#ifndef SCENEASTARDATAS_H
#define SCENEASTARDATAS_H

#include "GDCpp/AutomatismsSharedData.h"
#include "RuntimeSceneAStarDatas.h"

/**
 * \brief Data common to all A star automatisms of a scene.
 */
class GD_EXTENSION_API SceneAStarDatas : public gd::AutomatismsSharedData
{
public:
    SceneAStarDatas() : gridWidth(20), gridHeight(20), diagonalMove(true) {};
    virtual ~SceneAStarDatas() {};
    virtual boost::shared_ptr<gd::AutomatismsSharedData> Clone() const { return boost::shared_ptr<gd::AutomatismsSharedData>(new SceneAStarDatas(*this));}

    virtual boost::shared_ptr<AutomatismsRuntimeSharedData> CreateRuntimeSharedDatas()
    {
        return boost::shared_ptr<AutomatismsRuntimeSharedData>(new RuntimeSceneAStarDatas(*this));
    }

    #if defined(GD_IDE_ONLY)
    virtual void SaveToXml(TiXmlElement * eventElem) const;
    #endif

    virtual void LoadFromXml(const TiXmlElement * eventElem);

    float gridWidth;
    float gridHeight;
    bool diagonalMove;
};

#endif // SCENEASTARDATAS_H

