/**
Game Develop - Platform Automatism Extension
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * Manage the common objects shared by objects having a 
 * platform automatism.
 */
gdjs.PlatformObjectsManager = function(runtimeScene, sharedData)
{
	this._allPlatforms = new Hashtable();
};

gdjs.PlatformObjectsManager.prototype.addPlatform = function(platformAutomatism) {
    if ( !this._allPlatforms.containsKey(platformAutomatism.owner.id) ) {
        this._allPlatforms.put(platformAutomatism.owner.id, platformAutomatism);
    }
};

gdjs.PlatformObjectsManager.prototype.removePlatform = function(platformAutomatism) {
    if ( this._allPlatforms.containsKey(platformAutomatism.owner.id) ) {
        this._allPlatforms.remove(platformAutomatism.owner.id);
    }
};

gdjs.PlatformObjectsManager.prototype.getAllPlatforms = function() {
	return this._allPlatforms;
};

/**
 * PlatformRuntimeAutomatism represents an automatism allowing objects to be
 * considered as a platform by objects having PlatformerObject Automatism.
 *
 * @class PlatformRuntimeAutomatism
 * @constructor 
 */
gdjs.PlatformRuntimeAutomatism = function(runtimeScene, automatismData, owner)
{
    gdjs.RuntimeAutomatism.call(this, runtimeScene, automatismData, owner);

    //Load the platform type
    this._platformType = automatismData.attr.platformType;
    if ( this._platformType == "Ladder" )
        this._platformType = gdjs.PlatformRuntimeAutomatism.LADDER;
    else if ( this._platformType == "Jumpthru" )
        this._platformType = gdjs.PlatformRuntimeAutomatism.JUMPTHRU;
    else
        this._platformType = gdjs.PlatformRuntimeAutomatism.NORMALPLAFTORM;

	//Create the shared manager if necessary.
	if ( !gdjs.PlatformRuntimeAutomatism.platformsObjectsManagers.containsKey(runtimeScene.getName()) ) {
		var manager = new gdjs.PlatformObjectsManager(runtimeScene);
		gdjs.PlatformRuntimeAutomatism.platformsObjectsManagers.put(runtimeScene.getName(), manager);
	}
	this._manager = gdjs.PlatformRuntimeAutomatism.platformsObjectsManagers.get(runtimeScene.getName());

	this._registeredInManager = false;

};

gdjs.PlatformRuntimeAutomatism.prototype = Object.create( gdjs.RuntimeAutomatism.prototype );
gdjs.PlatformRuntimeAutomatism.thisIsARuntimeAutomatismConstructor = "PlatformAutomatism::PlatformAutomatism";
gdjs.PlatformRuntimeAutomatism.platformsObjectsManagers = new Hashtable();

gdjs.PlatformRuntimeAutomatism.LADDER = 2;
gdjs.PlatformRuntimeAutomatism.JUMPTHRU = 1;
gdjs.PlatformRuntimeAutomatism.NORMALPLAFTORM = 0;

gdjs.PlatformRuntimeAutomatism.prototype.ownerRemovedFromScene = function() {
	if ( this._manager && this._registeredInManager ) this._manager.removePlatform(this);
};

gdjs.PlatformRuntimeAutomatism.prototype.doStepPreEvents = function(runtimeScene) {
    
    //Scene change is not supported
    /*if ( parentScene != &scene ) //Parent scene has changed
    {
        if ( sceneManager ) //Remove the object from any old scene manager.
            sceneManager->RemovePlatform(this);

        parentScene = &scene;
        sceneManager = parentScene ? &ScenePlatformObjectsManager::managers[&scene] : NULL;
        registeredInManager = false;
    }*/

    if (!this.activated() && this._registeredInManager) 
    {
        this._manager.removePlatform(this);
        this._registeredInManager = false;
    }
    else if (this.activated() && !this._registeredInManager) 
    {
        this._manager.addPlatform(this);
        this._registeredInManager = true;
    }
};

gdjs.PlatformRuntimeAutomatism.prototype.doStepPostEvents = function(runtimeScene) {

};

gdjs.PlatformRuntimeAutomatism.prototype.onActivate = function() {
    this._manager.addPlatform(this);
    this._registeredInManager = true;
};

gdjs.PlatformRuntimeAutomatism.prototype.onDeActivate = function() {
    this._manager.removePlatform(this);
    this._registeredInManager = false;
};

gdjs.PlatformRuntimeAutomatism.prototype.changePlatformType = function(platformType)
{
    if ( platformType === "Ladder" ) this._platformType = gdjs.PlatformRuntimeAutomatism.LADDER;
    else if ( platformType === "Jumpthru" ) this._platformType = gdjs.PlatformRuntimeAutomatism.JUMPTHRU;
    else this._platformType = gdjs.PlatformRuntimeAutomatism.NORMALPLAFTORM;
};

gdjs.PlatformRuntimeAutomatism.prototype.getPlatformType = function()
{
    return this._platformType;
};