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

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <set>
#include "PathfindingAutomatism.h"
#include "PathfindingObstacleAutomatism.h"
#include "ScenePathfindingObstaclesManager.h"
#include "GDCpp/BuiltinExtensions/MathematicalTools.h"
#include "GDCpp/Scene.h"
#include "GDCpp/tinyxml/tinyxml.h"
#include "GDCpp/XmlMacros.h"
#include "GDCpp/RuntimeScene.h"
#include "GDCpp/RuntimeObject.h"
#include "GDCpp/CommonTools.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#if defined(GD_IDE_ONLY)
#include <iostream>
#include <map>
#include <wx/intl.h>
#include "GDCore/IDE/Dialogs/PropgridPropertyDescriptor.h"
#endif

namespace
{

class NodePosition
{
public:
    NodePosition(int x_, int y_) : x(x_), y(y_) {};

    int x;
    int y;

    /**
     * \brief Tool function used to store a NodePosition as key in boost::unordered_set.
     */
    struct NodePositionHash : std::unary_function<NodePosition, std::size_t>
    {
        std::size_t operator()(NodePosition const & n) const
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, n.x);
            boost::hash_combine(seed, n.y);
            return seed;
        }
    };
};

/**
 * \brief Internal tool class representing a node when looking for a path
 */
class Node
{
public:
    Node() : pos(0, 0), cost(0), smallestCost(-1), estimateCost(-1), parent(NULL), open(true) {};
    Node(int x, int y) : pos(x, y), cost(0), smallestCost(-1), estimateCost(-1), parent(NULL), open(true) {};
    Node(const NodePosition & pos_) : pos(pos_), cost(0), smallestCost(-1), estimateCost(-1), parent(NULL), open(true) {};

    NodePosition pos;
    float cost; ///< The cost for traveling on this node
    float smallestCost; ///< the cost to go to this node (when considering the shortest path).
    float estimateCost; ///< the estimate cost total to go to the destination through this node (when considering the shortest path).
    const Node * parent; ///< The previous node to be visited to go to this node (when considering the shortest path).
    bool open; ///< true if the node is "open" (must be explored), false if "close" (already explored)

    /**
     * \brief Tool function used to store a Node in a priority_queue.
     */
    class NodeComparator
    {
    public:
        bool operator()(const Node * n1, const Node * n2)
        {
            return n1->estimateCost < n2->estimateCost;
        }
    };
};

bool operator==(Node const& n1, Node const& n2)
{
    return n1.pos.x == n2.pos.x && n1.pos.y == n2.pos.y;
};

typedef float (*DistanceFunPtr)(const NodePosition & , const NodePosition & );

/**
 * \brief Internal tool class containing the structures used by A* and members functions related
 * to them.
 */
class SearchContext
{
public:
    SearchContext(ScenePathfindingObstaclesManager & obstacles_, int startNodeX_, int startNodeY_,
        bool allowsDiagonal_ = true) :
        obstacles(obstacles_),
        finalNode(NULL),
        destination(0, 0),
        start(startNodeX_, startNodeY_),
        allowsDiagonal(allowsDiagonal_)
    {
        std::cout << "start: " << startNodeX_ << ";" << startNodeY_ << std::endl;
        distanceFunction = allowsDiagonal ? &SearchContext::EuclidianDistance : &SearchContext::ManhattanDistance;
    }

    /**
     * Compute a path to the specified target node, considering the obstacles
     * and the start node passed in the constructor.
     * \return true if computation found a path, in which case you can call GetFinalNode method
     * to construct the path.
     */
    bool ComputePathTo(int targetNodeX, int targetNodeY)
    {
        destination = NodePosition(targetNodeX, targetNodeY);
        std::cout << "Destination: " << targetNodeX << ";" << targetNodeY << std::endl;

        allNodes.clear();
        Node & startNode = GetNode(start);
        startNode.smallestCost = 0;
        startNode.estimateCost = 0 + distanceFunction(start, destination);
        openNodes.clear();
        openNodes.insert(&startNode);

        unsigned int iterationCount = 0;
        while (!openNodes.empty())
        {
            if (iterationCount++ > 500) return false; //Make sure we do not search forever.

            Node * n = *openNodes.begin(); //Get the most promising node...
            n->open = false;                 //...and flag it as explored
            openNodes.erase(openNodes.begin()); //Be sure to remove ONLY the first element!

            //Check if we reached destination?
            if ( n->pos.x == destination.x && n->pos.y == destination.y )
            {
                finalNode = n;
                return true;
            }

            //No, so add neighbors to the nodes to explore.
            InsertNeighbors(*n);
        }

        return false;
    }

    /**
     * @return The final node of the computed path.
     * Iterate on the parent member to create the path.
     */
    Node * GetFinalNode() const
    {
        return finalNode;
    }

    #if 0
    static void UnitTest()
    {
        {
            ScenePathfindingObstaclesManager obstacles;
            SearchContext ctx(obstacles, 0, 0);
            ctx.ComputePathTo(0,0);
        }
        std::cout << "---" << std::endl;
        {
            ScenePathfindingObstaclesManager obstacles;
            SearchContext ctx(obstacles, 0, 0);
            ctx.ComputePathTo(1,0);
        }
        std::cout << "---" << std::endl;
        {
            ScenePathfindingObstaclesManager obstacles;
            SearchContext ctx(obstacles, 0, 0);
            ctx.ComputePathTo(0,1);
        }
        std::cout << "---" << std::endl;
        {
            ScenePathfindingObstaclesManager obstacles;
            SearchContext ctx(obstacles, 0, 0);
            ctx.ComputePathTo(1,1);
        }
        std::cout << "---" << std::endl;
        {
            ScenePathfindingObstaclesManager obstacles;
            SearchContext ctx(obstacles, 3, 4);
            ctx.ComputePathTo(12,9);
        }
        std::cout << "---" << std::endl;
        std::cout << "End unit tests";
    }
    #endif

private:
    /**
     * Insert the neighbors of the current node in the open list
     * (Only if they are not closed, and if the cost is better than the already existing smallest cost).
     */
    void InsertNeighbors(const Node & currentNode)
    {
        AddOrUpdateNode(NodePosition(currentNode.pos.x+1, currentNode.pos.y), currentNode, 1);
        AddOrUpdateNode(NodePosition(currentNode.pos.x-1, currentNode.pos.y), currentNode, 1);
        AddOrUpdateNode(NodePosition(currentNode.pos.x, currentNode.pos.y+1), currentNode, 1);
        AddOrUpdateNode(NodePosition(currentNode.pos.x, currentNode.pos.y-1), currentNode, 1);
        if ( allowsDiagonal )
        {
            AddOrUpdateNode(NodePosition(currentNode.pos.x+1, currentNode.pos.y+1), currentNode, sqrt2);
            AddOrUpdateNode(NodePosition(currentNode.pos.x+1, currentNode.pos.y-1), currentNode, sqrt2);
            AddOrUpdateNode(NodePosition(currentNode.pos.x-1, currentNode.pos.y-1), currentNode, sqrt2);
            AddOrUpdateNode(NodePosition(currentNode.pos.x-1, currentNode.pos.y+1), currentNode, sqrt2);
        }
    }

    /**
     * Get (or construct dynamically) a node.
     * All nodes should be created using this method: The cost of the node is computed thanks
     * to the objects flagged as obstacles.
     */
    Node & GetNode(const NodePosition & pos)
    {
        if (allNodes.find(pos) != allNodes.end())
            return allNodes.find(pos)->second;

        Node newNode(pos);

        bool objectsOnCell = false;
        const std::set<PathfindingObstacleAutomatism*> & allObstacles = obstacles.GetAllObstacles();
        for (std::set<PathfindingObstacleAutomatism*>::const_iterator it = allObstacles.begin();
             it != allObstacles.end();
             ++it)
        {
            RuntimeObject * obj = (*it)->GetObject();
            int topLeftCellX = floor(obj->GetDrawableX()/obstacles.GetCellWidth());
            int topLeftCellY = floor(obj->GetDrawableY()/obstacles.GetCellHeight());
            int bottomRightCellX = ceil((obj->GetDrawableX()+obj->GetWidth())/obstacles.GetCellWidth());
            int bottomRightCellY = ceil((obj->GetDrawableY()+obj->GetHeight())/obstacles.GetCellHeight());
            if ( topLeftCellX <= pos.x && pos.x <= bottomRightCellX
                && topLeftCellY <= pos.y && pos.y <= bottomRightCellY)
            {
                objectsOnCell = true;
                if ( (*it)->GetCost() < 0 )
                {
                    newNode.cost = -1;
                    break; //The cell is impassable, stop here.
                }
                else //Superimpose obstacles
                    newNode.cost += (*it)->GetCost();
            }
        }

        if (!objectsOnCell) newNode.cost = 1; //Default cost when no objects put on the cell.

        allNodes[pos] = newNode;
        return allNodes[pos];
    }

    /**
     * Compute the euclidian distance between two positions.
     */
    static float EuclidianDistance(const NodePosition & a, const NodePosition & b)
    {
        return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    }

    /**
     * Compute the taxi distance between two positions.
     */
    static float ManhattanDistance(const NodePosition & a, const NodePosition & b)
    {
        return abs(a.x-b.x)+abs(a.y-b.y);
    }

    /**
     * Add a node to the openNodes (only if the cost to reach it is less than the existing cost, if any).
     */
    void AddOrUpdateNode(const NodePosition & newNodePosition, const Node & currentNode, float factor)
    {
        Node & neighbor = GetNode(newNodePosition);
        if (!neighbor.open || neighbor.cost < 0 ) //cost < 0 means impassable obstacle
            return;

        //Update the node costs and parent if the path coming from currentNode is better:
        if (neighbor.smallestCost == -1
            || neighbor.smallestCost > currentNode.smallestCost + (currentNode.cost+neighbor.cost)/2.0*factor)
        {
            if (neighbor.smallestCost != -1) //The node is already in the open list:
                openNodes.erase(&neighbor);   //remove it as its estimate cost will be updated.

            neighbor.smallestCost = currentNode.smallestCost + (currentNode.cost+neighbor.cost)/2.0*factor;
            neighbor.parent = &currentNode;
            neighbor.estimateCost = neighbor.smallestCost + distanceFunction(neighbor.pos, destination);

            openNodes.insert(&neighbor);
        }
    }

    boost::unordered_map< NodePosition, Node, NodePosition::NodePositionHash > allNodes; ///< All the nodes
    std::multiset<Node*, Node::NodeComparator> openNodes; ///< Only the open nodes (Such that Node::open == true)
    const ScenePathfindingObstaclesManager & obstacles; ///< A reference to all the obstacles of the scene
    Node * finalNode; //If computation succeeded, the final node is stored here.
    NodePosition destination;
    NodePosition start;
    DistanceFunPtr distanceFunction;
    bool allowsDiagonal;

    static const float sqrt2;
};

const float SearchContext::sqrt2 = 1.414213562;

}
PathfindingAutomatism::PathfindingAutomatism() :
    allowDiagonals(true),
    acceleration(400),
    maxSpeed(200),
    angularMaxSpeed(180),
    rotateObject(true),
    angleOffset(0),
    speed(0),
    angularSpeed(0),
    timeOnSegment(0),
    totalSegmentTime(0),
    currentSegment(0),
    reachedEnd(false)
{

}

void PathfindingAutomatism::MoveTo(RuntimeScene & scene, float x, float y)
{
    if ( !sceneManager ) return;
    float cellWidth = sceneManager->GetCellWidth();
    float cellHeight = sceneManager->GetCellHeight();

    int targetCellX = GDRound(x/cellWidth);
    int targetCellY = GDRound(y/cellHeight);
    int startCellX = GDRound(object->GetX()/cellWidth);// TODO: Use the center of the object?
    int startCellY = GDRound(object->GetY()/cellHeight);

    path.clear();
    if ( startCellX == targetCellX && startCellY == targetCellY ) {
        path.push_back(sf::Vector2f(object->GetX(), object->GetY()));
        path.push_back(sf::Vector2f(x, y));
        EnterSegment(0);
        return;
    }

    //Start searching for a path
    //TODO: Default cost
    //TODO: Heuristic plus agressive.
    ::SearchContext ctx(*sceneManager, startCellX, startCellY, true); //TODO: Diag
    if (ctx.ComputePathTo(targetCellX, targetCellY))
    {
        //Path found: memorize it
        const ::Node * node = ctx.GetFinalNode();
        while (node) {
            std::cout << "N: " << node->pos.x*cellWidth << ";" << node->pos.y*cellHeight << std::endl;
            path.push_back(sf::Vector2f(node->pos.x*cellWidth, node->pos.y*cellHeight));
            node = node->parent;
        }

        std::reverse(path.begin(), path.end());
        path[0] = sf::Vector2f(object->GetX(), object->GetY());
        EnterSegment(0);
        return;
    }

    //Not path found
}

void PathfindingAutomatism::EnterSegment(unsigned int segmentNumber)
{
    if ( path.empty() ) return;

    currentSegment = segmentNumber;
    if (currentSegment < path.size()-1)
    {
        sf::Vector2f newPath = (path[currentSegment + 1] - path[currentSegment]);
        totalSegmentTime = sqrtf(newPath.x*newPath.x+newPath.y*newPath.y);
        timeOnSegment = 0;
        reachedEnd = false;
    }
    else {
        reachedEnd = true;
        speed = 0;
    }
}

void PathfindingAutomatism::DoStepPreEvents(RuntimeScene & scene)
{
    if ( parentScene != &scene ) //Parent scene has changed
    {
        parentScene = &scene;
        sceneManager = parentScene ? &ScenePathfindingObstaclesManager::managers[&scene] : NULL;
    }

    if ( !sceneManager ) return;

    if (path.empty() || reachedEnd) return;

    //Update the speed of the object
    float timeDelta = static_cast<double>(scene.GetElapsedTime())/1000000.0;
    speed += acceleration*timeDelta;
    if ( speed > maxSpeed ) speed = maxSpeed;
    angularSpeed = angularMaxSpeed; //No acceleration for angular speed for now

    //Update the time on the segment and change segment if needed
    timeOnSegment += speed*timeDelta;
    if (timeOnSegment >= totalSegmentTime && currentSegment < path.size())
        EnterSegment(currentSegment + 1);

    //Position object on the segment and update its angle
    sf::Vector2f newPos;
    float pathAngle = object->GetAngle();
    if ( currentSegment < path.size()-1 ) {
        newPos = path[currentSegment] + (path[currentSegment + 1] - path[currentSegment]) * (timeOnSegment / totalSegmentTime);
        pathAngle = atan2(path[currentSegment+1].y - path[currentSegment].y,
            path[currentSegment+1].x - path[currentSegment].x)*180/3.14159;
    }
    else
        newPos = path.back();

    object->SetX(newPos.x);
    object->SetY(newPos.y);

    //Also update angle if needed
    if ( rotateObject ) {
        float angularDiff = GDpriv::MathematicalTools::angleDifference(object->GetAngle(), pathAngle);
        if ( abs(angularDiff) >= 3 ) {
            float newAngle = object->GetAngle()+(angularDiff < 0 ? 1.0 : -1.0)*angularSpeed*timeDelta;
            object->SetAngle(newAngle);

            if ( object->GetAngle() != newAngle ) //Objects like sprite in 8 directions does not handle small increments...
                object->SetAngle(pathAngle); //...so force them to be in the path angle anyway.
        }
    }
}

void PathfindingAutomatism::DoStepPostEvents(RuntimeScene & scene)
{
    if ( parentScene != &scene ) //Parent scene has changed
    {
        parentScene = &scene;
        sceneManager = parentScene ? &ScenePathfindingObstaclesManager::managers[&scene] : NULL;
    }
}

void PathfindingAutomatism::LoadFromXml(const TiXmlElement * elem)
{
    //TODO
}

#if defined(GD_IDE_ONLY)
void PathfindingAutomatism::SaveToXml(TiXmlElement * elem) const
{
    //TODO
}

std::map<std::string, gd::PropgridPropertyDescriptor> PathfindingAutomatism::GetProperties(gd::Project & project) const
{
    std::map<std::string, gd::PropgridPropertyDescriptor> properties;

    properties[ToString(_("Allows diagonals"))].SetValue(allowDiagonals ? "true" : "false").SetType("Boolean");
    properties[ToString(_("Acceleration"))].SetValue(ToString(acceleration));
    properties[ToString(_("Max. speed"))].SetValue(ToString(maxSpeed));
    properties[ToString(_("Rotate speed"))].SetValue(ToString(angularMaxSpeed));
    properties[ToString(_("Rotate object"))].SetValue(rotateObject ? "true" : "false").SetType("Boolean");
    properties[ToString(_("Angle offset"))].SetValue(ToString(angleOffset));

    return properties;
}

bool PathfindingAutomatism::UpdateProperty(const std::string & name, const std::string & value, gd::Project & project)
{
    if ( name == ToString(_("Allows diagonals")) ) {
        allowDiagonals = (value != "0");
        return true;
    }
    if ( name == ToString(_("Rotate object")) ) {
        rotateObject = (value != "0");
        return true;
    }

    if ( ToDouble(value) < 0 ) return false;

    if ( name == ToString(_("Acceleration")) )
        acceleration = ToDouble(value);
    else if ( name == ToString(_("Max. speed")) )
        maxSpeed = ToDouble(value);
    else if ( name == ToString(_("Rotate speed")) )
        angularMaxSpeed = ToDouble(value);
    else if ( name == ToString(_("Angle offset")) )
        angleOffset = ToDouble(value);
    else
        return false;

    return true;
}

#endif