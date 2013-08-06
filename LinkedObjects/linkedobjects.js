/**
Game Develop - LinkedObjects Extension
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * Manages the links between objects.
 *
 * @namespace gdjs
 * @class LinksManager
 * @constructor
 */
gdjs.LinksManager = function()
{
	this.links = new Hashtable();
};

gdjs.LinksManager.prototype.getObjectsLinkedWith = function(objA) {
	if ( !this.links.containsKey(objA.id) )
		this.links.put(objA.id, []);

	return this.links.get(objA.id);
};

gdjs.LinksManager.prototype.linkObjects = function(objA, objB) {
	var objALinkedObjects = this.getObjectsLinkedWith(objA);
	if ( objALinkedObjects.indexOf(objB) === -1 )
		objALinkedObjects.push(objB);

	var objBLinkedObjects = this.getObjectsLinkedWith(objB);
	if ( objBLinkedObjects.indexOf(objA) === -1 )
		objBLinkedObjects.push(objA);
};

gdjs.LinksManager.prototype.removeAllLinksOf = function(obj) {
	var objLinkedObjects = this.getObjectsLinkedWith(obj);
	for (var i = 0; i < objLinkedObjects.length; i++) {
		if ( this.links.containsKey(objLinkedObjects[i].id) ) {
			var otherObjList = this.links.get(objLinkedObjects[i].id);
			var index = otherObjList.indexOf(obj);
			if ( index !== -1) otherObjList.remove(index);
		}
	}

	if ( !this.links.containsKey(obj.id) )
		this.links.get(obj.id).length = 0;
};

gdjs.LinksManager.prototype.removeLinkBetween = function(objA, objB) {
	var list, index;

	if ( this.links.containsKey(objA.id) ) {
		list = this.links.get(objA.id);
		index = list.indexOf(objB);
		if ( index !== -1) list.remove(index);
	}
	
	if ( this.links.containsKey(objB.id) ) {
		list = this.links.get(objB.id);
		index = list.indexOf(objA);
		if ( index !== -1) list.remove(index);
	}
};

/**
 * @namespace gdjs.evtTools
 * @class linkedObjects
 * @static
 * @private
 */
gdjs.evtTools.linkedObjects = gdjs.evtTools.linkedObjects || {managers: new Hashtable()};

gdjs.evtTools.linkedObjects.gdjsCallbackRuntimeSceneLoaded = function(runtimeScene) {
	gdjs.evtTools.linkedObjects.managers.put(runtimeScene.getName(), new gdjs.LinksManager());
};

gdjs.evtTools.linkedObjects.gdjsCallbackObjectDeletedFromScene = function(runtimeScene, obj) {
	gdjs.evtTools.linkedObjects.managers.get(runtimeScene.getName()).removeAllLinksOf(obj);
};

gdjs.evtTools.linkedObjects.linkObjects = function(runtimeScene, objA, objB) {
	gdjs.evtTools.linkedObjects.managers.get(runtimeScene.getName()).linkObjects(objA, objB);
};

gdjs.evtTools.linkedObjects.removeLinkBetween = function(runtimeScene, objA, objB) {
	gdjs.evtTools.linkedObjects.managers.get(runtimeScene.getName()).removeLinkBetween(objA, objB);
};

gdjs.evtTools.linkedObjects.removeAllLinksOf = function(runtimeScene, objA, objB) {
	gdjs.evtTools.linkedObjects.managers.get(runtimeScene.getName()).removeAllLinksOf(objA, objB);
};

gdjs.evtTools.linkedObjects.pickObjectsLinkedTo = function(runtimeScene, objectsLists, obj) {

    var isTrue = false;
    var objectsValues = objectsLists.values();

    for(var i = 0, leni = objectsValues.length;i<leni;++i) {
        var arr = objectsValues[i];
        for(var k = 0, lenk = arr.length;k<lenk;++k) {
            arr[k].pick = false;
        }
    }

    //Pick only linked objects
    var linkedObjects =
		gdjs.evtTools.linkedObjects.managers.get(runtimeScene.getName()).getObjectsLinkedWith(obj);
    for(var i = 0, leni = objectsValues.length;i<leni;++i) {
        var arr = objectsValues[i];

        for(var k = 0, lenk = arr.length;k<lenk;++k) {
            if ( linkedObjects.indexOf(arr[k]) !== -1 ) { //The current object is linked to obj.
				isTrue = true;
                arr[k].pick = true; //Pick the objects
            }
        }
    }
    
    //Trim not picked objects from arrays.
    for(var i = 0, leni = objectsValues.length;i<leni;++i) {
        var arr = objectsValues[i];
        var finalSize = 0;
        
        for(var k = 0, lenk = arr.length;k<lenk;++k) {
            var obj = arr[k];
            if ( arr[k].pick ) {
                arr[finalSize] = obj;
                finalSize++;
            }
        }
        arr.length = finalSize;
    }
};
