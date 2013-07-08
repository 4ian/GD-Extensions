/*
 *  Game Develop JS Platform
 *  2013 Florian Rival (Florian.Rival@gmail.com)
 */

/**
 * The textRuntimeObject displays a text on the screen.
 *
 * @class TextRuntimeObject
 * @extends RuntimeObject
 */
gdjs.TextRuntimeObject = function(runtimeScene, objectData)
{
    gdjs.RuntimeObject.call(this, runtimeScene, objectData);
    
    this._runtimeScene = runtimeScene;
    this._characterSize = parseInt(objectData.CharacterSize.attr.value);
    this._fontName = "Arial";
    this._bold = objectData.attr.bold === "true";
    this._italic = objectData.attr.italic === "true";
    this._underlined = objectData.attr.underlined === "true";
    this._color = [parseInt(objectData.Color.attr.r), 
                parseInt(objectData.Color.attr.g),
                parseInt(objectData.Color.attr.b)];
    if ( objectData.Font.attr.value !== "" ) {
        this._fontName = "\"gdjs_font_"+objectData.Font.attr.value+"\"";
    }
     
    var text = objectData.String.attr.value;
    if ( text.length === 0 ) text = " ";
    
    if ( this._text == undefined ) this._text = new PIXI.Text(text, {align:"left"});
    this._text.anchor.x = 0.5;
    this._text.anchor.y = 0.5;
    runtimeScene.getLayer("").addChildToPIXIContainer(this._text, this.zOrder);
    
    this._updateTextStyle();
    this._updateTextPosition();
}

gdjs.TextRuntimeObject.prototype = Object.create( gdjs.RuntimeObject.prototype );
gdjs.TextRuntimeObject.thisIsARuntimeObjectConstructor = "TextObject::Text";

gdjs.TextRuntimeObject.prototype.deleteFromScene = function(runtimeScene) {
    runtimeScene.markObjectForDeletion(that);
    runtimeScene.getLayer(this.layer).removePIXIContainerChild(this._text);
}

gdjs.TextRuntimeObject.prototype._updateTextStyle = function() {
    style = {align:"left"};
	style.font = "";
    if ( this._italic ) style.font += "italic ";
    if ( this._bold ) style.font += "bold ";
    //if ( this._underlined ) style.font += "underlined "; Not supported :/
    style.font += this._characterSize+"px"+" "+this._fontName;
    style.fill = "#"+gdjs.rgbToHex(this._color[0], this._color[1], this._color[2]);
    this._text.setStyle(style);
}

gdjs.TextRuntimeObject.prototype._updateTextPosition = function() {
    this._text.position.x = this.x+this._text.width/2;
    this._text.position.y = this.y+this._text.height/2;
    this.hitBoxesDirty = true;
}

gdjs.TextRuntimeObject.prototype.setX = function(x) {
    this.x = x;
    this._updateTextPosition();
}

gdjs.TextRuntimeObject.prototype.setY = function(y) {
    this.y = y;
    this._updateTextPosition();
}

gdjs.TextRuntimeObject.prototype.setAngle = function(angle) {
    this.angle = angle;
    this._text.rotation = gdjs.toRad(angle);
}

gdjs.TextRuntimeObject.prototype.setOpacity = function(opacity) {
    if ( opacity < 0 ) opacity = 0;
    if ( opacity > 255 ) opacity = 255;

    this.opacity = opacity;
    this._text.alpha = opacity/255; 
}

gdjs.TextRuntimeObject.prototype.getOpacity = function() {
    return this.opacity;
}

gdjs.TextRuntimeObject.prototype.getString = function() {
    return this._text.text;
}

gdjs.TextRuntimeObject.prototype.setString = function(str) {
    if ( str.length === 0 ) str = " ";
    this._text.setText(str);
    this._updateTextPosition();
}

gdjs.TextRuntimeObject.prototype.getCharacterSize = function() {
    return this._characterSize;
}

gdjs.TextRuntimeObject.prototype.setCharacterSize = function(newSize) {
    this._characterSize = newSize;
    this._updateTextStyle();
}

gdjs.TextRuntimeObject.prototype.isBold = function() {
    return this._bold;
}

gdjs.TextRuntimeObject.prototype.setBold = function(enable) {
    this._bold = enable;
    this._updateTextStyle();
}

gdjs.TextRuntimeObject.prototype.isItalic = function() {
    return this._italic;
}

gdjs.TextRuntimeObject.prototype.setItalic = function(enable) {
    this._italic = enable;
    this._updateTextStyle();
}

gdjs.TextRuntimeObject.prototype.hide = function(enable) {
    if ( enable == undefined ) enable = true;
    this._hidden = enable;
    this._text.visible = !enable;
}

gdjs.TextRuntimeObject.prototype.setLayer = function(name) {
    //We need to move the object from the pixi container of the layer
    this._runtimeScene.getLayer(this.layer).removePIXIContainerChild(this._text);
    this.layer = name;
    this._runtimeScene.getLayer(this.layer).addChildToPIXIContainer(this._text, this.zOrder);
}

gdjs.TextRuntimeObject.prototype.getWidth = function() {
    return this._text.width;
}

gdjs.TextRuntimeObject.prototype.getHeight = function() {
    return this._text.height;
}

gdjs.TextRuntimeObject.prototype.setColor = function(str) {
    this._color = str.split(";");
    this._updateTextStyle();
}

/**
 * Set the Z order of the object.
 *
 * @method setZOrder
 * @param z {Number} The new Z order position of the object
 */
gdjs.TextRuntimeObject.prototype.setZOrder = function(z) {
    if ( z != this.zOrder ) {
        this._runtimeScene.getLayer(this.layer).changePIXIContainerChildZOrder(this._text, z);
        this.zOrder = z;
    }
}
