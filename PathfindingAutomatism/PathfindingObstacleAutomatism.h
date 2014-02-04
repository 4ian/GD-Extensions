/**

Game Develop - Pathfinding Automatism Extension
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

#ifndef PATHFINDINGOBSTACLEAUTOMATISM_H
#define PATHFINDINGOBSTACLEAUTOMATISM_H

#include "GDCpp/Automatism.h"
#include "GDCpp/RuntimeObject.h"
class ScenePathfindingObstaclesManager;
class RuntimeScene;
class TiXmlElement;
#if defined(GD_IDE_ONLY)
#include <map>
namespace gd { class PropgridPropertyDescriptor; }
namespace gd { class Project; }
namespace gd { class Layout; }
#endif

/**
 * \brief Automatism that mark object as being obstacles for objects using
 * pathfinding automatism.
 */
class GD_EXTENSION_API PathfindingObstacleAutomatism : public Automatism
{
public:
    PathfindingObstacleAutomatism();
    virtual ~PathfindingObstacleAutomatism();
    virtual Automatism* Clone() const { return new PathfindingObstacleAutomatism(*this); }

    /**
     * \brief Return the object owning this automatism.
     */
    RuntimeObject * GetObject() const { return object; }

    /**
     * \brief Return the cost of moving on the object (-1 for impassable obstacle)
     */
    float GetCost() const { return -1; } //TODO

    virtual void LoadFromXml(const TiXmlElement * elem);
    #if defined(GD_IDE_ONLY)
    virtual std::map<std::string, gd::PropgridPropertyDescriptor> GetProperties(gd::Project & project) const;
    //virtual bool UpdateProperty(const std::string & name, const std::string & value, gd::Project & project); //TODO: Virtual borders
    virtual void SaveToXml(TiXmlElement * elem) const;
    #endif

private:
    virtual void OnActivate();
    virtual void OnDeActivate();

    virtual void DoStepPreEvents(RuntimeScene & scene);
    virtual void DoStepPostEvents(RuntimeScene & scene);

    RuntimeScene * parentScene; ///< The scene the object belongs to.
    ScenePathfindingObstaclesManager * sceneManager; ///< The obstacles manager associated to the scene.
    bool registeredInManager; ///< True if the automatism is registered in the list of obstacles of the scene.
};

#endif // PATHFINDINGOBSTACLEAUTOMATISM_H

