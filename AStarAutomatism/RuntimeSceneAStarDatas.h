/**

Game Develop - A Star Automatism Extension
Copyright (c) 2010-2014 Florian Rival (Florian.Rival@gmail.com)

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

#ifndef RUNTIMESCENEASTARDATAS_H
#define RUNTIMESCENEASTARDATAS_H

#include <boost/weak_ptr.hpp>
#include <vector>
#include "GDCpp/AutomatismsRuntimeSharedData.h"
class SceneAStarDatas;
class AStarAutomatism;

/**
 * Datas shared by A Star Automatism at runtime
 */
class GD_EXTENSION_API RuntimeSceneAStarDatas : public AutomatismsRuntimeSharedData
{
    public:
        RuntimeSceneAStarDatas(const SceneAStarDatas & automatismSharedDatas);
        virtual ~RuntimeSceneAStarDatas();
        virtual boost::shared_ptr<AutomatismsRuntimeSharedData> Clone() const { return boost::shared_ptr<AutomatismsRuntimeSharedData>(new RuntimeSceneAStarDatas(*this));}

        std::vector < AStarAutomatism* > objects; ///< Each object using the automatism add itself to this list, used to compute obstacles

        float gridWidth;
        float gridHeight;
        bool diagonalMove;

    private:
};

#endif // RUNTIMESCENEASTARDATAS_H

