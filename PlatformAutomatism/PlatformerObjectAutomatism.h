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

#ifndef PLATFORMEROBJECTAUTOMATISM_H
#define PLATFORMEROBJECTAUTOMATISM_H
#include "GDCpp/Automatism.h"
#include "GDCpp/Object.h"
#include <SFML/System/Vector2.hpp>
#include <map>
#include <set>
namespace gd { class Layout; }
class RuntimeScene;
class PlatformAutomatism;
class ScenePlatformObjectsManager;
class TiXmlElement;
class RuntimeScenePlatformData;

/**
 * \brief Allows objects to jump and stand on platforms.
 */
class GD_EXTENSION_API PlatformerObjectAutomatism : public Automatism
{
public:
    PlatformerObjectAutomatism();
    virtual ~PlatformerObjectAutomatism();
    virtual Automatism* Clone() const { return new PlatformerObjectAutomatism(*this); }

    /**
     * \brief Load the automatism from XML
     */
    virtual void LoadFromXml(const TiXmlElement * elem);

    #if defined(GD_IDE_ONLY)
    /**
     * \brief Save the automatism to XML
     */
    virtual void SaveToXml(TiXmlElement * elem) const;

    virtual std::map<std::string, gd::PropgridPropertyDescriptor> GetProperties(gd::Project & project) const;
    virtual bool UpdateProperty(const std::string & name, const std::string & value, gd::Project & project);
    #endif

private:
    virtual void DoStepPreEvents(RuntimeScene & scene);
    virtual void DoStepPostEvents(RuntimeScene & scene);

    /**
     * \brief Return a list of all the platforms that could be colliding with the object if it is moved.
     * \param maxMovementLength The maximum length of any movement that could be done by the object, in pixels.
     * \warning sceneManager must be valid and not NULL.
     */
    std::set<PlatformAutomatism*> GetPotentialCollidingObjects(double maxMovementLength);

    /**
     * \brief Return a list of all the platforms colliding with the object.
     * \warning sceneManager must be valid and not NULL.
     */
    std::set<PlatformAutomatism*> GetPlatformsCollidingWith();

    /**
     * \brief Among the platforms passed in paramater, return a list of the platforms colliding with the object..
     * \param candidates The platform to be tested for collision
     * \param exceptThisOne If not NULL, this platform won't be tested for collision.
     */
    std::set<PlatformAutomatism*> GetPlatformsCollidingWith(const std::set<PlatformAutomatism*> & candidates, PlatformAutomatism * exceptThisOne = NULL);

    /**
     * \brief Among the platforms passed in paramater, return true if there is a platform colliding with the object.
     * \param candidates The platform to be tested for collision
     * \param exceptThisOne If not NULL, this platform won't be tested for collision.
     */
    bool IsCollidingWith(const std::set<PlatformAutomatism*> & candidates, PlatformAutomatism * exceptThisOne = NULL);

    double gravity; //In pixels.seconds^-2
    double maxFallingSpeed; //In pixels.seconds^-1
    double acceleration; //In pixels.seconds^-2
    double deceleration; //In pixels.seconds^-2
    double maxSpeed; //In pixels.seconds^-1
    double jumpSpeed; //In pixels.seconds^-1

    RuntimeScene * parentScene; ///< The scene the object belongs to.
    ScenePlatformObjectsManager * sceneManager; ///< The platform objects manager associated to the scene.
    bool isOnFloor; ///< True if the object is on a floor.
    PlatformAutomatism * floorPlatform; ///< The platform the object is on, when isOnFloor == true.
    double floorLastX; ///< The last X position of the floor platform, when isOnFloor == true.
    double floorLastY; ///< The last Y position of the floor platform, when isOnFloor == true.
    double currentFallSpeed; ///< The current speed of the fall, when isOnFloor == false.
    double currentSpeed; ///< The current speed in the left/right direction.
    bool jumping; ///< True if the object is jumping.
    double currentJumpSpeed; ///< The current speed of the jump, when jumping == true.
    bool canJump; ///< True if the object can jump.
};
#endif // PLATFORMEROBJECTAUTOMATISM_H

