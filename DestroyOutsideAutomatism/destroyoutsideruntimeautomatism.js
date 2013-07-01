/**
Game Develop - DestroyOutside Automatism Extension
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * The destroyOutsideRuntimeAutomatism represents an automatism allowing objects to be
 * moved using the mouse.
 *
 * @class destroyOutsideRuntimeAutomatism
 * @constructor 
 */
gdjs.destroyOutsideRuntimeAutomatism = function(runtimeScene, automatismXml)
{
    var that = gdjs.runtimeAutomatism(runtimeScene, automatismXml);
    var my = {};
    
    my.extraBorder = parseFloat($(automatismXml).attr("extraBorder"));
    
    that.doStepPreEvents = function(runtimeScene) {
        
        var ow = that.owner.getWidth();
        var oh = that.owner.getWidth();
        var ocx = that.owner.getDrawableX()+that.owner.getCenterX();
        var ocy = that.owner.getDrawableY()+that.owner.getCenterY();
        var layer = runtimeScene.getLayer(that.owner.getLayer());
        
        var boundingCircleRadius = Math.sqrt(ow*ow+oh*oh)/2.0;
        if (   ocx+boundingCircleRadius+my.extraBorder < layer.getCameraX()
            || ocx-boundingCircleRadius-my.extraBorder > layer.getCameraX()+layer.getCameraWidth()
            || ocy+boundingCircleRadius+my.extraBorder < layer.getCameraY()
            || ocy-boundingCircleRadius-my.extraBorder > layer.getCameraY()+layer.getCameraHeight() ) {
            //We are outside the camera area.
            that.owner.deleteFromScene(runtimeScene);
        }
    }
    
    that.setExtraBorder = function(val) {
        my.extraBorder = val;
    }
    
    that.getExtraBorder = function() {
        return my.extraBorder;
    }
    
    return that;
}

//Notify gdjs that the destroyOutsideRuntimeAutomatism exists.
gdjs.destroyOutsideRuntimeAutomatism.thisIsARuntimeAutomatismConstructor = "DestroyOutsideAutomatism::DestroyOutside";