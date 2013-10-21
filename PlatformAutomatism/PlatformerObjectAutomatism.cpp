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

#include <boost/shared_ptr.hpp>
#include "PlatformerObjectAutomatism.h"
#include "PlatformAutomatism.h"
#include "ScenePlatformObjectsManager.h"
#include "GDCpp/Scene.h"
#include "GDCpp/tinyxml/tinyxml.h"
#include "GDCpp/XmlMacros.h"
#include "GDCpp/RuntimeScene.h"
#include "GDCpp/RuntimeObject.h"
#include "GDCpp/CommonTools.h"
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#if defined(GD_IDE_ONLY)
#include <iostream>
#include <map>
#include <wx/intl.h>
#include "GDCore/IDE/Dialogs/PropgridPropertyDescriptor.h"
#endif

PlatformerObjectAutomatism::PlatformerObjectAutomatism() :
    gravity(1000),
    maxFallingSpeed(700),
    acceleration(1500),
    deceleration(1500),
    maxSpeed(250),
    jumpSpeed(600),
    parentScene(NULL),
    sceneManager(NULL),
    isOnFloor(false),
    floorPlatform(NULL),
    currentFallSpeed(0),
    currentSpeed(0),
    jumping(false),
    currentJumpSpeed(0),
    canJump(false)
{
}

PlatformerObjectAutomatism::~PlatformerObjectAutomatism()
{
}

void PlatformerObjectAutomatism::DoStepPreEvents(RuntimeScene & scene)
{
    if ( parentScene != &scene ) //Parent scene has changed
    {
        parentScene = &scene;
        sceneManager = parentScene ? &ScenePlatformObjectsManager::managers[&scene] : NULL;
        floorPlatform = NULL;
    }

    if ( !sceneManager ) return;

    double timeDelta = static_cast<double>(scene.GetElapsedTime())/1000000.0;

    //0) Get the player input:

    double requestedDeltaX = 0;
    double requestedDeltaY = 0;

    //Change the speed according to the player's input.
    bool left = sf::Keyboard::isKeyPressed( sf::Keyboard::Left );
    bool right = sf::Keyboard::isKeyPressed( sf::Keyboard::Right );
    if ( left ) 
        currentSpeed -= acceleration*timeDelta;
    if ( right ) 
        currentSpeed += acceleration*timeDelta;

    //Take deceleration into account only if no key is pressed.
    if ( left == right )
    {
        bool wasPositive = currentSpeed>0;
        currentSpeed -= deceleration*timeDelta*(wasPositive ? 1.0 : -1.0);

        //Set the speed to 0 if the speed was top low.
        if ( wasPositive && currentSpeed < 0 ) currentSpeed = 0;
        if ( !wasPositive && currentSpeed > 0 ) currentSpeed = 0;
    }

    if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
    if (currentSpeed < -maxSpeed) currentSpeed = -maxSpeed;
    requestedDeltaX += currentSpeed*timeDelta;

    std::set<PlatformAutomatism*> potentialObjects = GetPotentialCollidingObjects(std::max(requestedDeltaX, maxFallingSpeed));

    //Check that the floor object still exists and is near the object.
    if ( isOnFloor && potentialObjects.find(floorPlatform) == potentialObjects.end() ) 
    {
        isOnFloor = false;
        floorPlatform = NULL;
    }

    //1) X axis:

    //Shift the object according to the floor movement.
    if ( isOnFloor ) 
    {
        requestedDeltaX += floorPlatform->GetObject()->GetX() - floorLastX;
        requestedDeltaY += floorPlatform->GetObject()->GetY() - floorLastY;
    }

    //Move the object on x axis.
    double oldX = object->GetX();
    if ( requestedDeltaX != 0 )
    {
        object->SetX(object->GetX()+requestedDeltaX);
        while ( IsCollidingWith(potentialObjects, floorPlatform) ) //Colliding: Try to push out from the solid.
        {
            if ( (requestedDeltaX > 0 && object->GetX() <= oldX) || 
                 (requestedDeltaX < 0 && object->GetX() >= oldX) )
            {
                object->SetX(oldX); //Unable to move the object without being stuck in an obstacle.
                break;
            }

            //If on floor: try get up a bit to bypass not perfectly aligned floors.
            if ( isOnFloor )
            {
                object->SetY(object->GetY()-1);
                if ( !IsCollidingWith(potentialObjects, floorPlatform) )
                    break;
                object->SetY(object->GetY()+1);
            }

            object->SetX(floor(object->GetX())+(requestedDeltaX > 0 ? -1 : 1));
            currentSpeed = 0; //Collided with a wall
        }
    }

    //2) Y axis:

    //Fall
    if (!isOnFloor) 
    {
        currentFallSpeed += gravity*timeDelta;
        if ( currentFallSpeed > maxFallingSpeed ) currentFallSpeed = maxFallingSpeed;

        requestedDeltaY += currentFallSpeed*timeDelta;
        requestedDeltaY = std::min(requestedDeltaY, maxFallingSpeed*timeDelta);
    }

    //Jumping
    bool up = sf::Keyboard::isKeyPressed( sf::Keyboard::Up );
    if ( canJump && up )
    {
        jumping = true;
        canJump = false;
        //isOnFloor = false; If floor is a very steep slope, the object could go into it.
        currentJumpSpeed = jumpSpeed;
        object->SetY(object->GetY()-1);
    }

    if ( jumping )
    {
        requestedDeltaY -= currentJumpSpeed*timeDelta;
        currentJumpSpeed -= gravity*timeDelta;
        if ( currentJumpSpeed < 0 )
        {
            currentJumpSpeed = 0;
            jumping = false;
        }
    }

    //Follow the floor
    if ( isOnFloor )
    {
        if ( object->IsCollidingWith(floorPlatform->GetObject()) )
        {
            //Floor is getting up, as the object is colliding with it.
            double oldY = object->GetY();
            int step = 0;
            bool stillInFloor = false;
            do 
            {
                if ( step >= floor(abs(requestedDeltaX)) ) //Slope is too step ( > 50% )
                {
                    object->SetY(object->GetY()-(abs(requestedDeltaX)-(double)step)); //Try to add the decimal part.
                    if ( object->IsCollidingWith(floorPlatform->GetObject()) )
                        stillInFloor = true; //Too steep.

                    break;
                }
         
                //Try to get out of the floor.
                object->SetY(object->GetY()-1);
                step++;
            }
            while ( object->IsCollidingWith(floorPlatform->GetObject() ) );

            if ( stillInFloor ) 
            {
                object->SetY(oldY); //Unable to follow the floor ( too steep ): Go back to the original position.
                object->SetX(oldX); //And also revert the shift on X axis.
            }
        }
        else
        {
            //Floor is flat or get down.
            double oldY = object->GetY();
            object->SetY(object->GetY()+1);
            int step = 0;
            bool noMoreOnFloor = false;
            while ( !IsCollidingWith(potentialObjects) )
            {
                if ( step > abs(requestedDeltaX) ) //Slope is too step ( > 50% )
                {
                    noMoreOnFloor = true;
                    break;
                }

                //Object was on floor, but no more: Maybe a slope, try to follow it.
                object->SetY(object->GetY()+1);
                step++;
            }
            if ( noMoreOnFloor )
                object->SetY(oldY); //Unable to follow the floor: Go back to the original position. Fall will be triggered next tick.
            else
                object->SetY(object->GetY()-1); //Floor touched: Go back 1 pixel over.   
        }

    }

    //Move the object on Y axis
    if ( requestedDeltaY != 0 )
    {
        double oldY = object->GetY();
        object->SetY(object->GetY()+requestedDeltaY);
        while ( IsCollidingWith(potentialObjects) ) 
        {
            jumping = false;
            if ( (requestedDeltaY > 0 && object->GetY() <= oldY) || 
                 (requestedDeltaY < 0 && object->GetY() >= oldY) )
            {
                object->SetY(oldY); //Unable to move the object without being stuck in an obstacle.
                break;
            }

            object->SetY(floor(object->GetY())+(requestedDeltaY > 0 ? -1 : 1));
        }

    }

    //3) Update the current floor data for the next tick:

    //Check if the object is on a floor:
    //In priority, check if the last floor platform is still the floor.
    double oldY = object->GetY();
    object->SetY(object->GetY()+1);
    if ( isOnFloor && object->IsCollidingWith(floorPlatform->GetObject() ) ) 
    {
        //Still on the same floor
        floorLastX = floorPlatform->GetObject()->GetX();
        floorLastY = floorPlatform->GetObject()->GetY();
    }
    else 
    {
        std::set<PlatformAutomatism*> collidingObjects = GetPlatformsCollidingWith(potentialObjects);
        if ( !collidingObjects.empty() ) //Just landed on floor
        {
            isOnFloor = true;
            canJump = true;
            jumping = false;
            floorPlatform = *collidingObjects.begin();
            floorLastX = floorPlatform->GetObject()->GetX();
            floorLastY = floorPlatform->GetObject()->GetY();
            currentFallSpeed = 0;
        }
        else //In the air
        {
            canJump = false;
            isOnFloor = false;
            floorPlatform = NULL;
            std::cout << "AIR";
        }
    }
    object->SetY(oldY);
}

std::set<PlatformAutomatism*> PlatformerObjectAutomatism::GetPlatformsCollidingWith()
{
    return GetPlatformsCollidingWith(sceneManager->GetAllPlatforms());
}

bool PlatformerObjectAutomatism::IsCollidingWith(const std::set<PlatformAutomatism*> & candidates, PlatformAutomatism * exceptThisOne)
{
    for (std::set<PlatformAutomatism*>::iterator it = candidates.begin();
         it != candidates.end();
         ++it)
    {
        if ( *it == exceptThisOne ) continue;

        if ( object->IsCollidingWith((*it)->GetObject()) )
            return true;
    }

    return false;
}

std::set<PlatformAutomatism*> PlatformerObjectAutomatism::GetPlatformsCollidingWith(const std::set<PlatformAutomatism*> & candidates, 
    PlatformAutomatism * exceptThisOne)
{
    std::set<PlatformAutomatism*> result;
    for (std::set<PlatformAutomatism*>::iterator it = candidates.begin();
         it != candidates.end();
         ++it)
    {
        if ( *it == exceptThisOne ) continue;
        //todo//if ( /*jumptru && */ (*it)->GetObject()->GetY()  )
        if ( object->IsCollidingWith((*it)->GetObject()) )
        {
            result.insert(*it);
        }
    }

    return result;
}

std::set<PlatformAutomatism*> PlatformerObjectAutomatism::GetPotentialCollidingObjects(double maxMovementLength)
{
    //Compute the "bounding circle" radius of the object.
    float o1w = object->GetWidth();
    float o1h = object->GetHeight();
    float obj1BoundingRadius = sqrt(o1w*o1w+o1h*o1h)/2.0+maxMovementLength/2.0; //Add to it the maximum magnitude of movement.


    std::set<PlatformAutomatism*> potentialObjects;
    for (std::set<PlatformAutomatism*>::iterator it = sceneManager->GetAllPlatforms().begin();
         it != sceneManager->GetAllPlatforms().end();
         ++it)
    {
        //First check if bounding circle are too far.
        RuntimeObject * obj2 = (*it)->GetObject();
        float o2w = obj2->GetWidth();
        float o2h = obj2->GetHeight();

        float x = object->GetDrawableX()+object->GetCenterX()-(obj2->GetDrawableX()+obj2->GetCenterX());
        float y = object->GetDrawableY()+object->GetCenterY()-(obj2->GetDrawableY()+obj2->GetCenterY());
        float obj2BoundingRadius = sqrt(o2w*o2w+o2h*o2h)/2.0;

        if ( sqrt(x*x+y*y) <= obj1BoundingRadius + obj2BoundingRadius ) {
            potentialObjects.insert(*it);
        }
    }

    return potentialObjects;
}

void PlatformerObjectAutomatism::DoStepPostEvents(RuntimeScene & scene)
{
    if ( parentScene != &scene ) //Parent scene has changed
    {
        parentScene = &scene;
        sceneManager = parentScene ? &ScenePlatformObjectsManager::managers[&scene] : NULL;
        floorPlatform = NULL;
    }

}

void PlatformerObjectAutomatism::LoadFromXml(const TiXmlElement * elem)
{
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("gravity", gravity); 
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("maxFallingSpeed", maxFallingSpeed); 
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("acceleration", acceleration); 
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("deceleration", deceleration); 
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("maxSpeed", maxSpeed); 
    GD_CURRENT_ELEMENT_LOAD_ATTRIBUTE_DOUBLE("jumpSpeed", jumpSpeed); 
}

#if defined(GD_IDE_ONLY)
void PlatformerObjectAutomatism::SaveToXml(TiXmlElement * elem) const
{
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("gravity", gravity); 
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("maxFallingSpeed", maxFallingSpeed); 
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("acceleration", acceleration); 
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("deceleration", deceleration); 
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("maxSpeed", maxSpeed); 
    GD_CURRENT_ELEMENT_SAVE_ATTRIBUTE_DOUBLE("jumpSpeed", jumpSpeed); 
}

std::map<std::string, gd::PropgridPropertyDescriptor> PlatformerObjectAutomatism::GetProperties(gd::Project & project) const
{
    std::map<std::string, gd::PropgridPropertyDescriptor> properties;

    properties[ToString(_("Gravity"))].SetValue(ToString(gravity));
    properties[ToString(_("Max. falling speed"))].SetValue(ToString(maxFallingSpeed));
    properties[ToString(_("Acceleration"))].SetValue(ToString(acceleration));
    properties[ToString(_("Deceleration"))].SetValue(ToString(deceleration));
    properties[ToString(_("Max. speed"))].SetValue(ToString(maxSpeed));
    properties[ToString(_("Jump speed"))].SetValue(ToString(jumpSpeed));

    return properties;
}

bool PlatformerObjectAutomatism::UpdateProperty(const std::string & name, const std::string & value, gd::Project & project)
{
    if ( ToDouble(value) < 0 ) return false;

    if ( name == ToString(_("Gravity")) )
        gravity = ToDouble(value);
    else if ( name == ToString(_("Max. falling speed")) )
        maxFallingSpeed = ToDouble(value);
    else if ( name == ToString(_("Acceleration")) )
        acceleration = ToDouble(value);
    else if ( name == ToString(_("Deceleration")) )
        deceleration = ToDouble(value);
    else if ( name == ToString(_("Max. speed")) )
        maxSpeed = ToDouble(value);
    else if ( name == ToString(_("Jump speed")) )
        jumpSpeed = ToDouble(value);
    else 
        return false;

    return true;
}

#endif