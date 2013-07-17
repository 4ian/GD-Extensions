/**
Game Develop - Physics Automatism Extension
Copyright (c) 2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * Manage the common objects shared by objects having a 
 * physics automatism.
 */
gdjs.PhysicsSharedData = function(runtimeScene, sharedData)
{
	this.stepped = false;

	this.totalTime = 0;
	this.fixedTimeStep = 1/60;
	this.scaleX = parseFloat(sharedData.attr.scaleX);
	this.scaleY = parseFloat(sharedData.attr.scaleY);
	this.invScaleX = 1/this.scaleX;
	this.invScaleY = 1/this.scaleY;

	//Setup world and contact listener
	var b2World = Box2D.Dynamics.b2World;
	var b2Vec2 = Box2D.Common.Math.b2Vec2;
	this.world = new b2World(new b2Vec2(parseFloat(sharedData.attr.gravityX), 
		-parseFloat(sharedData.attr.gravityY)), true);
	//this.world.SetAutoClearForces(false);
 
 	this.contactListener = new Box2D.Dynamics.b2ContactListener();
	this.contactListener.BeginContact = function (contact) {

    	if ( contact.GetFixtureA().GetBody() == null || 
    		 contact.GetFixtureB().GetBody() == null ) 
    		return;

		var automatismA = contact.GetFixtureA().GetBody().GetUserData(),
		automatismB = contact.GetFixtureB().GetBody().GetUserData();
		 
		automatismA.currentContacts.push(automatismB);
		automatismB.currentContacts.push(automatismA);
	};
	this.contactListener.EndContact = function (contact) {

    	if ( contact.GetFixtureA().GetBody() == null || 
    		 contact.GetFixtureB().GetBody() == null ) 
    		return;

		var automatismA = contact.GetFixtureA().GetBody().GetUserData(),
		automatismB = contact.GetFixtureB().GetBody().GetUserData();
		
		var i = automatismA.currentContacts.indexOf(automatismB);
		if ( i !== -1 ) automatismA.currentContacts.remove(i);
		
		i = automatismB.currentContacts.indexOf(automatismA);
		if ( i !== -1 ) automatismB.currentContacts.remove(i);
	};

	this.world.SetContactListener(this.contactListener);
}

gdjs.PhysicsSharedData.prototype.step = function(dt) {
	this.totalTime += dt;

	if(this.totalTime > this.fixedTimeStep) {
	    var numberOfSteps = Math.floor(this.totalTime / this.fixedTimeStep);
	    this.totalTime -= numberOfSteps * this.fixedTimeStep;

        if ( numberOfSteps > 5 ) numberOfSteps = 5; //Process 5 steps at max.

	    for(var a = 0; a < numberOfSteps; a++) {
            this.world.Step(this.fixedTimeStep, 1, 1);
            this.world.ClearForces();
	    }
	}
	
    this.stepped = true;
}

/**
 * PhysicsRuntimeAutomatism represents an automatism allowing objects to be
 * moved in a realistic way thanks to a physics engine.
 *
 * @class PhysicsRuntimeAutomatism
 * @constructor 
 */
gdjs.PhysicsRuntimeAutomatism = function(runtimeScene, automatismData, owner)
{
    gdjs.RuntimeAutomatism.call(this, runtimeScene, automatismData, owner);
    
    this._box2DBody = null;
    this._dynamic = automatismData.attr.dynamic === "true";
    this._objectOldWidth = 0;
    this._objectOldHeight = 0;
    this._objectOldX = 0;
    this._objectOldY = 0;
    this._objectOldAngle = 0;
    this._angularDamping = parseFloat(automatismData.attr.angularDamping);
    this._linearDamping = parseFloat(automatismData.attr.linearDamping);
    this._isBullet = automatismData.attr.isBullet === "true";
    this._fixedRotation = automatismData.attr.fixedRotation === "true";
	this._massDensity = parseFloat(automatismData.attr.massDensity);
	this._averageFriction = parseFloat(automatismData.attr.averageFriction);
	this._averageRestitution = parseFloat(automatismData.attr.averageRestitution);
    this._shapeType = automatismData.attr.shapeType;
	if ( this.currentContacts !== undefined )
		this.currentContacts.length = 0;
	else
		this.currentContacts = [];

	//Create the shared data if necessary.
	if ( !gdjs.PhysicsRuntimeAutomatism.scenesSharedData.containsKey(runtimeScene.getName()) ) {
		var initialData = runtimeScene.getInitialSharedDataForAutomatism(automatismData.attr.Name);
		var data = new gdjs.PhysicsSharedData(runtimeScene, initialData);
		gdjs.PhysicsRuntimeAutomatism.scenesSharedData.put(runtimeScene.getName(), data);
	}	
	this._sharedData = gdjs.PhysicsRuntimeAutomatism.scenesSharedData.get(runtimeScene.getName());

	//Do not create body now: the object is not fully created.
};

gdjs.PhysicsRuntimeAutomatism.prototype = Object.create( gdjs.RuntimeAutomatism.prototype );
gdjs.PhysicsRuntimeAutomatism.thisIsARuntimeAutomatismConstructor = "PhysicsAutomatism::PhysicsAutomatism";
gdjs.PhysicsRuntimeAutomatism.scenesSharedData = new Hashtable();
    
gdjs.PhysicsRuntimeAutomatism.prototype.onDeActivate = function() {
	if ( this._box2DBody !== null ) {
		this._sharedData.world.DestroyBody(this._box2DBody);
		this._box2DBody = null;
	}
};

gdjs.PhysicsRuntimeAutomatism.prototype.ownerRemovedFromScene = function() {
	this.onDeActivate();
};

gdjs.PhysicsRuntimeAutomatism.prototype.createBody = function() {
	var b2Vec2 = Box2D.Common.Math.b2Vec2;
	var b2BodyDef = Box2D.Dynamics.b2BodyDef;
	var b2Body = Box2D.Dynamics.b2Body;
	var b2FixtureDef = Box2D.Dynamics.b2FixtureDef;
	var b2World = Box2D.Dynamics.b2World;
	var b2PolygonShape = Box2D.Collision.Shapes.b2PolygonShape;
	var b2CircleShape = Box2D.Collision.Shapes.b2CircleShape;

    //Create body from object
    var bodyDef = new b2BodyDef();
	bodyDef.type = this._dynamic ? b2Body.b2_dynamicBody : b2Body.b2_staticBody;
	bodyDef.position.Set(
		(this.owner.getDrawableX()+this.owner.getWidth()/2)*this._sharedData.invScaleX,
		-(this.owner.getDrawableY()+this.owner.getHeight()/2)*this._sharedData.invScaleY);
    bodyDef.angle = -gdjs.toRad(this.owner.getAngle());
    bodyDef.angularDamping = this._angularDamping > 0 ? this._angularDamping : 0;
    bodyDef.linearDamping = this._linearDamping > 0 ? this._linearDamping : 0;
    bodyDef.bullet = this._isBullet;
    bodyDef.fixedRotation = this._fixedRotation;

	this._box2DBody = this._sharedData.world.CreateBody(bodyDef);
	this._box2DBody.SetUserData(this);

	//Setup the fixture
    var fixDef = null;
	if ( this._shapeType === "Circle" ) {
		var circle = new b2CircleShape(((this.owner.getWidth()*this._sharedData.invScaleX)+
            (this.owner.getHeight()*this._sharedData.invScaleY))/4);

		if ( circle.GetRadius() <= 0 ) circle.SetRadius(1);

		fixDef = new b2FixtureDef();
		fixDef.shape = circle;
	}
	/*else if ( this._shapeType === "CustomPolygon" ) {
		//TODO
	}*/
	else { //Box
		var box = new b2PolygonShape();
		box.SetAsBox(
			((this.owner.getWidth() > 0 ? this.owner.getWidth() : 1)*
				this._sharedData.invScaleX)/2, 
			((this.owner.getHeight() > 0 ? this.owner.getHeight() : 1)*
				this._sharedData.invScaleY)/2);

		fixDef = new b2FixtureDef();
		fixDef.shape = box;
	}

	fixDef.density = this._massDensity;
	fixDef.friction = this._averageFriction;
	fixDef.restitution = this._averageRestitution;
	this._box2DBody.CreateFixture(fixDef);

    this._objectOldWidth = this.owner.getWidth();
    this._objectOldHeight = this.owner.getHeight();
};

gdjs.PhysicsRuntimeAutomatism.prototype.doStepPreEvents = function(runtimeScene) {
	if ( this._box2DBody === null ) this.createBody();

	//Simulate the world
	if ( !this._sharedData.stepped ) 
		this._sharedData.step(runtimeScene.getElapsedTime()/1000);

    //Update object position according to Box2D body
	this.owner.setX(this._box2DBody.GetPosition().x*this._sharedData.scaleX-
		this.owner.getWidth()/2+this.owner.getX()-this.owner.getDrawableX());
	this.owner.setY(-this._box2DBody.GetPosition().y*this._sharedData.scaleY-
		this.owner.getHeight()/2+this.owner.getY()-this.owner.getDrawableY());
	this.owner.setAngle(-gdjs.toDegrees(this._box2DBody.GetAngle()));

    this._objectOldX = this.owner.getX();
    this._objectOldY = this.owner.getY();
    this._objectOldAngle = this.owner.getAngle();
};

gdjs.PhysicsRuntimeAutomatism.prototype.getBox2DBody = function() {
	if ( this._box2DBody === null ) this.createBody();
	return this._box2DBody;
};

gdjs.PhysicsRuntimeAutomatism.prototype.doStepPostEvents = function(runtimeScene) {
	if ( this._box2DBody === null ) this.createBody();

	//Ensure the Box2D body width and height are correct.
	if ( this._objectOldWidth !== this.owner.getWidth() || 
		this._objectOldHeight !== this.owner.getHeight() ) {
		this._sharedData.world.DestroyBody(this._box2DBody);
		this.createBody();
	}

	this._sharedData.stepped = false;

	//Ensure that the Box2D body position is correct
    if ( this._objectOldX == this.owner.getX() && 
    	this._objectOldY == this.owner.getY() && 
    	this._objectOldAngle == this.owner.getAngle())
        return;

    var pos = new Box2D.Common.Math.b2Vec2(
        (this.owner.getDrawableX()+this.owner.getWidth()/2)*this._sharedData.invScaleX, -
        (this.owner.getDrawableY()+this.owner.getHeight()/2)*this._sharedData.invScaleY);
    this._box2DBody.SetPositionAndAngle(pos, -gdjs.toRad(this.owner.getAngle()));
    this._box2DBody.SetAwake(true);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setStatic = function() {
    this._dynamic = false;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetType(Box2D.Dynamics.b2Body.b2_staticBody);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setDynamic = function()
{
    this._dynamic = true;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetType(Box2D.Dynamics.b2Body.b2_dynamicBody);
    this._box2DBody.SetAwake(true);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setFixedRotation = function()
{
    this._fixedRotation = true;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetFixedRotation(true);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setFreeRotation = function()
{
    this._fixedRotation = false;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetFixedRotation(false);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setAsBullet = function()
{
    this._isBullet = true;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetBullet(true);
};

gdjs.PhysicsRuntimeAutomatism.prototype.dontSetAsBullet = function()
{
    this._isBullet = false;

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetBullet(false);
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyImpulse = function(xCoordinate, yCoordinate)
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.ApplyImpulse(new Box2D.Common.Math.b2Vec2(xCoordinate, -yCoordinate),
        this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyImpulseUsingPolarCoordinates = function( angle, length )
{
	angle = gdjs.toRad(angle);

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.ApplyImpulse(new Box2D.Common.Math.b2Vec2(Math.cos(angle)*length,
        -Math.sin(angle)*length), this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyImpulseTowardPosition = function(xPosition, yPosition, length )
{
    if ( this._box2DBody === null ) this.createBody();

    var angle = Math.atan2(yPosition*this._sharedData.invScaleY+this._box2DBody.GetPosition().y,
                        xPosition*this._sharedData.invScaleX-this._box2DBody.GetPosition().x);

    this._box2DBody.ApplyImpulse(new Box2D.Common.Math.b2Vec2(Math.cos(angle)*length, 
        -Math.sin(angle)*length), this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyForce = function( xCoordinate, yCoordinate )
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.ApplyForce(new Box2D.Common.Math.b2Vec2(xCoordinate, -yCoordinate),
        this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyForceUsingPolarCoordinates = function( angle, length )
{
	angle = gdjs.toRad(angle);

    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.ApplyForce(new Box2D.Common.Math.b2Vec2(Math.cos(angle)*length,
    -Math.sin(angle)*length), this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyForceTowardPosition = function(xPosition, yPosition, length )
{
    if ( this._box2DBody === null ) this.createBody();

    var angle = Math.atan2(yPosition*this._sharedData.invScaleY+this._box2DBody.GetPosition().y,
                        xPosition*this._sharedData.invScaleX-this._box2DBody.GetPosition().x);

    this._box2DBody.ApplyForce(new Box2D.Common.Math.b2Vec2(Math.cos(angle)*length, 
        -Math.sin(angle)*length), this._box2DBody.GetPosition());
};

gdjs.PhysicsRuntimeAutomatism.prototype.applyTorque = function( torque )
{
    if ( this._box2DBody === null ) this.createBody();

    this._box2DBody.ApplyTorque(torque);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setLinearVelocity = function( xVelocity, yVelocity )
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetLinearVelocity(new Box2D.Common.Math.b2Vec2(xVelocity,-yVelocity));
};

gdjs.PhysicsRuntimeAutomatism.prototype.setAngularVelocity = function( angularVelocity )
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetAngularVelocity(angularVelocity);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setLinearDamping = function( linearDamping )
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetLinearDamping(linearDamping);
};

gdjs.PhysicsRuntimeAutomatism.prototype.setAngularDamping = function( angularDamping )
{
    if ( this._box2DBody === null ) this.createBody();
    this._box2DBody.SetAngularDamping(angularDamping);
};

gdjs.PhysicsRuntimeAutomatism.prototype.addRevoluteJointBetweenObjects = function( object, scene, xPosRelativeToMassCenter, yPosRelativeToMassCenter )
{
    if ( this._box2DBody === null ) this.createBody();

    if ( object == null || !object.hasAutomatism(this.name) ) return;
    var otherBody = object.getAutomatism(this.name).getBox2DBody();

    if ( this._box2DBody == otherBody ) return;

    var jointDef = new Box2D.Dynamics.Joints.b2RevoluteJointDef();
    jointDef.Initialize(this._box2DBody,
                        otherBody,
                        new Box2D.Common.Math.b2Vec2(xPosRelativeToMassCenter*this._sharedData.invScaleX+this._box2DBody.GetWorldCenter().x,
                                                     yPosRelativeToMassCenter*this._sharedData.invScaleY+this._box2DBody.GetWorldCenter().y));
    this._sharedData.world.CreateJoint(jointDef);
}

gdjs.PhysicsRuntimeAutomatism.prototype.addRevoluteJoint = function( xPosition, yPosition )
{
    if ( this._box2DBody === null ) this.createBody();

    var jointDef = new Box2D.Dynamics.Joints.b2RevoluteJointDef();
    jointDef.Initialize(this._box2DBody, this._sharedData.world.GetGroundBody(),
                        new Box2D.Common.Math.b2Vec2( xPosition*this._sharedData.invScaleX, 
                        	-yPosition*this._sharedData.invScaleY));

    this._sharedData.world.CreateJoint(jointDef);
}

gdjs.PhysicsRuntimeAutomatism.prototype.setGravity = function( xGravity, yGravity )
{
    if ( this._box2DBody === null ) this.createBody();

    this._sharedData.world.SetGravity(new Box2D.Common.Math.b2Vec2( 
    	xGravity*this._sharedData.invScaleX, -yGravity*this._sharedData.invScaleY));
}

gdjs.PhysicsRuntimeAutomatism.prototype.addGearJointBetweenObjects = function( object, ratio )
{
    if ( this._box2DBody === null ) this.createBody();

    if ( object == null || !object.hasAutomatism(this.name) ) return;
    var otherBody = object.getAutomatism(this.name).getBox2DBody();

    if ( this._box2DBody == otherBody ) return;

    //Gear joint need a revolute joint to the ground for the two objects
    var jointDef1 = new Box2D.Dynamics.Joints.b2RevoluteJointDef();
    jointDef1.Initialize(this._sharedData.world.GetGroundBody(), this._box2DBody, 
    	this._box2DBody.GetWorldCenter());

    var jointDef2 = new Box2D.Dynamics.Joints.b2RevoluteJointDef();
    jointDef2.Initialize(this._sharedData.world.GetGroundBody(), otherBody, 
    	otherBody.GetWorldCenter());

    var jointDef = new Box2D.Dynamics.Joints.b2GearJointDef();
    jointDef.bodyA = this._box2DBody;
    jointDef.bodyB = otherBody;
    jointDef.joint1 = this._sharedData.world.CreateJoint(jointDef1);
    jointDef.joint2 = this._sharedData.world.CreateJoint(jointDef2);
    jointDef.ratio = ratio * 3.14159;

    this._sharedData.world.CreateJoint(jointDef);
}

gdjs.PhysicsRuntimeAutomatism.prototype.setLinearVelocityX = function( xVelocity )
{
    if ( this._box2DBody === null ) this.createBody();

    this._box2DBody.SetLinearVelocity(new Box2D.Common.Math.b2Vec2(xVelocity, 
    	this._box2DBody.GetLinearVelocity().y));

}
gdjs.PhysicsRuntimeAutomatism.prototype.setLinearVelocityY = function( yVelocity )
{
    if ( this._box2DBody === null ) this.createBody();

    this._box2DBody.SetLinearVelocity(new Box2D.Common.Math.b2Vec2(
    	this._box2DBody.GetLinearVelocity().x, -yVelocity));
}
gdjs.PhysicsRuntimeAutomatism.prototype.getLinearVelocityX = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return this._box2DBody.GetLinearVelocity().x;
}
gdjs.PhysicsRuntimeAutomatism.prototype.getLinearVelocityY = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return -this._box2DBody.GetLinearVelocity().y;
}

gdjs.PhysicsRuntimeAutomatism.prototype.getLinearVelocity = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return Math.sqrt(this._box2DBody.GetLinearVelocity().x*this._box2DBody.GetLinearVelocity().x
    	+this._box2DBody.GetLinearVelocity().y*this._box2DBody.GetLinearVelocity().y);
}
gdjs.PhysicsRuntimeAutomatism.prototype.getAngularVelocity = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return this._box2DBody.GetAngularVelocity();
}
gdjs.PhysicsRuntimeAutomatism.prototype.getLinearDamping = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return this._box2DBody.GetLinearDamping();
}
gdjs.PhysicsRuntimeAutomatism.prototype.getAngularDamping = function()
{
    if ( this._box2DBody === null ) this.createBody();

    return this._box2DBody.GetAngularDamping();
}

gdjs.PhysicsRuntimeAutomatism.prototype.collisionWith = function( otherObjectsTable )
{
    if ( this._box2DBody === null ) this.createBody();

    //Getting a list of all objects which are tested
    var objects = [];
    var objectsLists = otherObjectsTable.values();

    for(var i = 0, len = objectsLists.length;i<len;++i) {
        objects.push.apply(objects, objectsLists[i]);
    }

    //Test if an object of the list is in collision with our object.
    for(var i = 0, len = objects.length;i<len;++i) {
        for (var j = 0, lenj = this.currentContacts.length;j<lenj;++j) {
            if ( this.currentContacts[j].owner.getNameId() === objects[i].getNameId() )
                return true;
        }
    }

    return false;
}

gdjs.PhysicsRuntimeAutomatism.prototype.isStatic = function() {
    return !this._dynamic;
}

gdjs.PhysicsRuntimeAutomatism.prototype.IsDynamic = function() {
    return this._dynamic;
}

/*
gdjs.PhysicsRuntimeAutomatism.prototype.setPolygonCoords(const std::vector<sf::Vector2f> &vec)
{
    polygonCoords = vec;
}

const std::vector<sf::Vector2f>& PhysicsAutomatism::GetPolygonCoords() const
{
    return polygonCoords;
}

bool PhysicsAutomatism::HasAutomaticResizing() const
{
    return automaticResizing;
}

gdjs.PhysicsRuntimeAutomatism.prototype.setAutomaticResizing(bool b)
{
    automaticResizing = b;
}

gdjs.PhysicsRuntimeAutomatism.prototype.getPolygonScaleX() const
{
    if(automaticResizing)
        return object->GetWidth() / polygonWidth;
    else
        return polygonScaleX;
}

gdjs.PhysicsRuntimeAutomatism.prototype.setPolygonScaleX(float scX, RuntimeScene &scene)
{
    polygonScaleX = scX;

    runtimeScenesPhysicsDatas->world->DestroyBody(body);
    CreateBody(scene);
}

gdjs.PhysicsRuntimeAutomatism.prototype.getPolygonScaleY() const
{
    if(automaticResizing)
        return object->GetHeight() / polygonHeight;
    else
        return polygonScaleY;
}

gdjs.PhysicsRuntimeAutomatism.prototype.setPolygonScaleY(float scY, RuntimeScene &scene)
{
    polygonScaleY = scY;

    runtimeScenesPhysicsDatas->world->DestroyBody(body);
    CreateBody(scene);
}*/