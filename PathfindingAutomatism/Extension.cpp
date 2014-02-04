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

#include "GDCpp/ExtensionBase.h"
#include "GDCpp/AutomatismsSharedData.h"
#include "GDCore/Tools/Version.h"
#include "PathfindingAutomatism.h"
#include "PathfindingObstacleAutomatism.h"
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
        SetExtensionInformation("PathfindingAutomatism",
                              _("Pathfinding automatism"),
                              _("Compute paths for objects avoiding obstacles."),
                              "Florian Rival",
                              "zlib/libpng License ( Open Source )");

        {
            gd::AutomatismMetadata & aut = AddAutomatism("PathfindingAutomatism",
                  _("Pathfinding"),
                  _("AStar"),
                  _("Automatism which move objects and avoid objects flagged as obstacles."),
                  "",
                  "CppPlatform/Extensions/AStaricon.png",
                  "PathfindingAutomatism",
                  boost::shared_ptr<gd::Automatism>(new PathfindingAutomatism),
                  boost::shared_ptr<gd::AutomatismsSharedData>(new gd::AutomatismsSharedData));

            #if defined(GD_IDE_ONLY)

            aut.SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddAction("SetDestination",
                           _("Move to a position"),
                           _("Move the object to a position"),
                           _("Move _PARAM0_ to _PARAM3_;_PARAM4_"),
                           _("Displacement"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("expression", _("Destination X position"))
                .AddParameter("expression", _("Destination Y position"))
                .codeExtraInformation.SetFunctionName("MoveTo").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            /*
            aut.AddAction("SetSpeed",
                           _("Speed"),
                           _("Change the moving speed on the path"),
                           _("Do _PARAM1__PARAM2_ to speed of of _PARAM0_ on the path"),
                           _("Displacement"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Speed"))
                .codeExtraInformation.SetFunctionName("SetSpeed").SetAssociatedGetter("GetSpeed").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("Speed",
                           _("Speed"),
                           _("Test the moving speed on the path"),
                           _("Moving speed of _PARAM0_ is _PARAM2__PARAM3_"),
                           _("Displacement"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Speed"))
                .codeExtraInformation.SetFunctionName("GetSpeed").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddAction("SetCost",
                           _("Cost"),
                           _("Change the cost of going through the object. 9 means the object is impassable."),
                           _("Do _PARAM1__PARAM2_ to cost of _PARAM0_"),
                           _("Obstacles"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Difficulty ( from 0 to 9 )"))
                .codeExtraInformation.SetFunctionName("SetCost").SetAssociatedGetter("GetCost").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("Cost",
                           _("Cost"),
                           _("Test the cost of going through the object"),
                           _("Cost of _PARAM0_ is _PARAM2__PARAM3_"),
                           _("Obstacles"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Difficulty"))
                .codeExtraInformation.SetFunctionName("GetCost").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("PathFound",
                           _("Path found"),
                           _("Return true if a path has been found"),
                           _("A path has been found for _PARAM0_"),
                           _("Displacement"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("PathFound").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("DestinationReached",
                           _("Destination reached"),
                           _("Return true if destination was reached"),
                           _("_PARAM0_ reached its destination"),
                           _("Displacement"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("DestinationReached").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddAction("SetGridWidth",
                           _("Width of the virtual grid"),
                           _("Change the width of cells of virtual grid."),
                           _("Do _PARAM3__PARAM4_ to the width of cells of the virtual grid"),
                           _("Global Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Width ( pixels )"))
                .codeExtraInformation.SetFunctionName("SetGridWidth").SetAssociatedGetter("GetGridWidth").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("GridWidth",
                           _("Width of the virtual grid"),
                           _("Test the width of cells of the virtual grid."),
                           _("Width of cells of the virtual grid is _PARAM3__PARAM4_"),
                           _("Global Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Width ( pixels )"))
                .codeExtraInformation.SetFunctionName("GetGridWidth").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");
            aut.AddAction("SetGridHeight",
                           _("Height of the virtual grid"),
                           _("Change the height of cells of the virtual grid"),
                           _("Do _PARAM3__PARAM4_ to the height of cells of the virtual grid"),
                           _("Global Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Height ( pixels )"))
                .codeExtraInformation.SetFunctionName("SetGridHeight").SetAssociatedGetter("GetGridHeight").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddCondition("GridHeight",
                           _("Height of the virtual grid"),
                           _("Test the height of cells of the virtual grid"),
                           _("Height of cells of the virtual grid is _PARAM3__PARAM4_"),
                           _("Global Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Height ( pixels )"))
                .codeExtraInformation.SetFunctionName("GetGridHeight").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddAction("EnterSegment",
                           _("Change the position on the path"),
                           _("Change state of the object on the path"),
                           _("Do _PARAM__PARAM2_ to the state of the object on the path"),
                           _("Advanced"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("EnterSegment").SetAssociatedGetter("GetCurrentSegment").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("Speed", _("Moving speed"), _("Moving speed on the path"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetSpeed").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("Cost", _("Cost"), _("Obstacle cost"), _("Obstacles"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetCost").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");
*/
            //TODO:
            /*aut.AddExpression("GetNodeX", _("Get waypoint X position"), _("Get a waypoint X position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetNodeX").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("GetNodeY", _("Get waypoint Y position"), _("Get a waypoint Y position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetNodeY").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");*/
/*
            aut.AddExpression("LastNodeX", _("Last waypoint X position"), _("Last waypoint X position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetLastNodeX").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("LastNodeY", _("Last waypoint Y position"), _("Last waypoint Y position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetLastNodeY").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("NextNodeX", _("Next waypoint X position"), _("Next waypoint X position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetNextNodeX").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("NextNodeY", _("Next waypoint Y position"), _("Next waypoint Y position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetNextNodeY").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("DestinationX", _("Destination X position"), _("Destination X position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetDestinationX").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("DestinationY", _("Destination Y position"), _("Destination Y position"), _("Displacement"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetDestinationY").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("GridWidth", _("Width of the virtual grid"), _("Width of the virtual grid"), _("Global Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetGridWidth").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");

            aut.AddExpression("GridHeight", _("Height of the virtual grid"), _("Height of the virtual grid"), _("Global Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddCodeOnlyParameter("currentScene", "")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetGridHeight").SetIncludeFile("PathfindingAutomatism/PathfindingAutomatism.h");
*/
            #endif

        }
        {
            gd::AutomatismMetadata & aut = AddAutomatism("PathfindingObstacleAutomatism",
                  _("Obstacle for pathfinding"),
                  _("PathfindingObstacle"),
                  _("Flag the object as being an obstacle for pathfinding."),
                  "",
                  "CppPlatform/Extensions/platformicon.png",
                  "PathfindingObstacleAutomatism",
                  boost::shared_ptr<gd::Automatism>(new PathfindingObstacleAutomatism),
                  boost::shared_ptr<gd::AutomatismsSharedData>(new gd::AutomatismsSharedData));

            #if defined(GD_IDE_ONLY)
            aut.SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddCondition("LeftBorder",
                           _("Left border"),
                           _("Teste the size, in virtual cells, of the left border of the object."),
                           _("Test the size, in virtual cells, of the left border of the object."),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Width ( virtual cells )"))
                .codeExtraInformation.SetFunctionName("GetLeftBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddCondition("RightBorder",
                           _("Right border"),
                           _("Test the size, in virtual cells, of the right border of the object."),
                           _("Right border of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Width ( virtual cells )"))
                .codeExtraInformation.SetFunctionName("GetRightBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddCondition("TopBorder",
                           _("Top border"),
                           _("Test the size, in virtual cells, of the top border of the object."),
                           _("Top border of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Width ( virtual cells )"))
                .codeExtraInformation.SetFunctionName("GetTopBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddCondition("BottomBorder",
                           _("Bottom border"),
                           _("Test the size, in virtual cells, of the bottom border of the object."),
                           _("Bottom border of _PARAM0_ is _PARAM2_ _PARAM1_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Width ( virtual cells )"))
                .codeExtraInformation.SetFunctionName("GetBottomBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddAction("SetLeftBorder",
                           _("Left border"),
                           _("Change the size, in virtual cells, of the left border of the object."),
                           _("Do _PARAM1__PARAM2_ to the left border of _PARAM0_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Width ( virtual cases )"))
                .codeExtraInformation.SetFunctionName("SetLeftBorder").SetAssociatedGetter("GetLeftBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddAction("SetRightBorder",
                           _("Right border"),
                           _("Change the size, in virtual cells, of the right border of the object."),
                           _("Do _PARAM1__PARAM2_ to the right border of _PARAM0_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Width ( virtual cases )"))
                .codeExtraInformation.SetFunctionName("SetRightBorder").SetAssociatedGetter("GetRightBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddAction("SetTopBorder",
                           _("Top border"),
                           _("Change the size, in virtual cells, of the top border of the object."),
                           _("Do _PARAM1__PARAM2_ to  the top border of _PARAM0_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Width ( virtual cases )"))
                .codeExtraInformation.SetFunctionName("SetTopBorder").SetAssociatedGetter("GetTopBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddAction("SetBottomBorder",
                           _("Bottom border"),
                           _("Change size, in virtual cells, of the bottom border of the object."),
                           _("Do _PARAM1__PARAM2_ to the  bottom border of _PARAM0_"),
                           _("Setup"),
                           "CppPlatform/Extensions/AStaricon24.png",
                           "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Width ( virtual cases )"))
                .codeExtraInformation.SetFunctionName("SetBottomBorder").SetAssociatedGetter("GetBottomBorder").SetManipulatedType("number").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");

            aut.AddExpression("LeftBorder", _("Left border"), _("Left border"), _("Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetLeftBorder").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");
            aut.AddExpression("RightBorder", _("Right border"), _("Right border"), _("Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetRightBorder").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");
            aut.AddExpression("TopBorder", _("Top border"), _("Top border"), _("Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetTopBorder").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");
            aut.AddExpression("BottomBorder", _("Bottom border"), _("Bottom border"), _("Setup"), "CppPlatform/Extensions/AStaricon16.png")
                .AddParameter("object", _("Object"))
                .AddParameter("automatism", _("Automatism"), "PathfindingAutomatism", false)
                .codeExtraInformation.SetFunctionName("GetBottomBorder").SetIncludeFile("PathfindingAutomatism/PathfindingObstacleAutomatism.h");
            #endif
        }

        GD_COMPLETE_EXTENSION_COMPILATION_INFORMATION();
    };
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

