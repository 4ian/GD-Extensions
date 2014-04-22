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

#ifndef PLATFORMEROBJECTAUTOMATISM_H
#define PLATFORMEROBJECTAUTOMATISM_H
#include "GDCpp/Automatism.h"
#include "GDCpp/Object.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
namespace gd { class Layout; }
class RuntimeScene;
class PlatformAutomatism;
class ScenePathfindingObstaclesManager;
class TiXmlElement;
class RuntimeScenePlatformData;

/**
 * \brief Compute path for objects avoiding obstacles
 */
class GD_EXTENSION_API PathfindingAutomatism : public Automatism
{
public:
    PathfindingAutomatism();
    virtual ~PathfindingAutomatism() {};
    virtual Automatism* Clone() const { return new PathfindingAutomatism(*this); }

    /**
     * \brief Compute and move on the path to the specified destination.
     */
    void MoveTo(RuntimeScene & scene, float x, float y);

    //Path information:
    /**
     * \brief Return true if the latest call to MoveTo succeeded.
     */
    bool PathFound() { return pathFound; }

    /**
     * \brief Return true if the object reached its destination
     */
    bool DestinationReached() { return reachedEnd; }

    float GetNodeX(unsigned int index) const;
    float GetNodeY(unsigned int index) const;
    unsigned int GetNextNodeIndex() const;
    unsigned int GetNodeCount() const { return path.size(); };
    float GetNextNodeX() const;
    float GetNextNodeY() const;
    float GetLastNodeX() const;
    float GetLastNodeY() const;
    float GetDestinationX() const;
    float GetDestinationY() const;

    //Configuration:
    bool DiagonalsAllowed() { return allowDiagonals; };
    float GetAcceleration() { return acceleration; };
    float GetMaxSpeed() { return maxSpeed; };
    float GetAngularMaxSpeed() { return angularMaxSpeed; };
    bool IsObjectRotated() { return rotateObject; }
    float GetAngleOffset() { return angleOffset; };
    unsigned int GetCellWidth() { return cellWidth; };
    unsigned int GetCellHeight() { return cellHeight; };
    float GetExtraBorder() { return extraBorder; };

    bool SetAllowDiagonals(bool allowDiagonals_) { allowDiagonals = allowDiagonals_; };
    float SetAcceleration(float acceleration_) { acceleration = acceleration_; };
    float SetMaxSpeed(float maxSpeed_) { maxSpeed = maxSpeed_; };
    float SetAngularMaxSpeed(float angularMaxSpeed_) { angularMaxSpeed = angularMaxSpeed_; };
    bool SetRotateObject(bool rotateObject_) { rotateObject = rotateObject_; };
    float SetAngleOffset(float angleOffset_) { angleOffset = angleOffset_; };
    unsigned int SetCellWidth(unsigned int cellWidth_) { cellWidth = cellWidth_; };
    unsigned int SetCellHeight(unsigned int cellHeight_) { cellHeight = cellHeight_; };
    float SetExtraBorder(float extraBorder_) { extraBorder = extraBorder_; };

    float GetSpeed() { return speed; };
    float SetSpeed(float speed_) { speed = speed_; };

    /**
     * \brief Load the automatism from XML
     */
    virtual void LoadFromXml(const TiXmlElement * elem);

    #if defined(GD_IDE_ONLY)
    /**
     * \brief Save the automatism to XML
     */
    virtual void SaveToXml(TiXmlElement * elem) const;

    virtual std::map<std::string, gd::PropertyDescriptor> GetProperties(gd::Project & project) const;
    virtual bool UpdateProperty(const std::string & name, const std::string & value, gd::Project & project);
    #endif

private:
    virtual void DoStepPreEvents(RuntimeScene & scene);
    virtual void DoStepPostEvents(RuntimeScene & scene);
    void EnterSegment(unsigned int segmentNumber);

    RuntimeScene * parentScene; ///< The scene the object belongs to.
    ScenePathfindingObstaclesManager * sceneManager; ///< The platform objects manager associated to the scene.
    std::vector<sf::Vector2f> path; ///< The computed path
    bool pathFound;

    //Automatism configuration:
    bool allowDiagonals;
    float acceleration;
    float maxSpeed;
    float angularMaxSpeed;
    bool rotateObject; ///< If true, the object is rotated according to the current segment's angle.
    float angleOffset; ///< Angle offset (added to the angle calculated with the segment)
    unsigned int cellWidth;
    unsigned int cellHeight;
    float extraBorder;

    //Attributes used for traveling on the path:
    float speed;
    float angularSpeed;
    float timeOnSegment;
    float totalSegmentTime;
    unsigned int currentSegment;
    bool reachedEnd;

};
#endif // PLATFORMEROBJECTAUTOMATISM_H

