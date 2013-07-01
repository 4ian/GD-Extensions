/**
Game Develop - Draggable Automatism Extension
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * The draggableRuntimeAutomatism represents an automatism allowing objects to be
 * moved using the mouse.
 *
 * @class draggableRuntimeAutomatism
 * @constructor 
 */
gdjs.draggableRuntimeAutomatism = function(runtimeScene, automatismXml)
{
    var that = gdjs.runtimeAutomatism(runtimeScene, automatismXml);
    var my = {};
    
    my.dragged = false;
    my.xOffset = 0;
    my.yOffset = 0;
    
    that.onDeActivate = function() {
        my.dragged = false;
    }
    
    that.doStepPreEvents = function(runtimeScene) {
        var mousePos = null;
            
        //Begin drag ?
        if ( !my.dragged && runtimeScene.getGame().isMouseButtonPressed(0) &&
             !gdjs.draggableRuntimeAutomatism.draggingSomething ) {
             
            mousePos = runtimeScene.getLayer(that.owner.getLayer()).convertCoords(
                runtimeScene.getGame().getMouseX(), 
                runtimeScene.getGame().getMouseY());
                
            if (that.owner.getDrawableX() <= mousePos[0] 
                && that.owner.getDrawableX() + that.owner.getWidth() >= mousePos[0] 
                && that.owner.getDrawableY() <= mousePos[1] 
                && that.owner.getDrawableY() + that.owner.getHeight() >= mousePos[1] ) {
                
                mousePos = runtimeScene.getLayer(that.owner.getLayer()).convertCoords(
                    runtimeScene.getGame().getMouseX(), 
                    runtimeScene.getGame().getMouseY());
                    
                my.dragged = true;
                gdjs.draggableRuntimeAutomatism.draggingSomething = true;
                my.xOffset = mousePos[0] - that.owner.getX();
                my.yOffset = mousePos[1] - that.owner.getY();
            }
        }
        //End dragging ?
        else if ( my.dragged && !runtimeScene.getGame().isMouseButtonPressed(0) ) {
            my.dragged = false;
            gdjs.draggableRuntimeAutomatism.draggingSomething = false;
        }
        
        //Being dragging ?
        if ( my.dragged ) {
            if ( mousePos == null ) {
                mousePos = runtimeScene.getLayer(that.owner.getLayer()).convertCoords(
                    runtimeScene.getGame().getMouseX(), 
                    runtimeScene.getGame().getMouseY());
            }
            
            that.owner.setX(mousePos[0]-my.xOffset);
            that.owner.setY(mousePos[1]-my.yOffset);
            
        }
    }
    
    that.isDragged = function(runtimeScene) {
        return my.dragged;
    }
    
    return that;
}

//Static property used to avoid start dragging an object while another is dragged.
gdjs.draggableRuntimeAutomatism.draggingSomething = false;

//Notify gdjs that the draggableRuntimeAutomatism exists.
gdjs.draggableRuntimeAutomatism.thisIsARuntimeAutomatismConstructor = "DraggableAutomatism::Draggable";