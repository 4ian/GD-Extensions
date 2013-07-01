/**

Game Develop - Text Object Extension
Copyright (c) 2008-2013 Florian Rival (Florian.Rival@gmail.com)

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
/**
 * Contributors to the extension:
 * Victor Levasseur ( Bold/Italic/Underlined styles )
 */

#include "GDCpp/ExtensionBase.h"
#include "GDCore/PlatformDefinition/PlatformExtension.h"
#include "GDCore/Tools/Version.h"
#include "TextObject.h"
#include <boost/version.hpp>

/**
 * \brief This class declares information about the C++ extension.
 */
class CppExtension : public ExtensionBase
{
public:

    /**
     * Constructor of an extension declares everything the extension contains : Objects, actions, conditions and expressions.
     */
    CppExtension()
    {
        SetExtensionInformation("TextObject",
                              _("Text object"),
                              _("Extension allowing to use an object displaying a text."),
                              "Florian Rival",
                              "zlib/libpng License ( Open Source )");


        //Declaration of all objects available
        {
            gd::ObjectMetadata & obj = AddObject("Text",
                       _("Text"),
                       _("Displays a text"),
                       "CppPlatform/Extensions/texticon.png",
                       &CreateTextObject,
                       &DestroyTextObject);

            AddRuntimeObject(obj, "RuntimeTextObject", CreateRuntimeTextObject, DestroyRuntimeTextObject);

            #if defined(GD_IDE_ONLY)

            obj.SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("String",
                           _("Modify the text"),
                           _("Modify the text of a Text object."),
                           _("Do _PARAM1__PARAM2_ to the text of _PARAM0_"),
                           _("Text"),
                           "res/actions/text24.png",
                           "res/actions/text.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("string", _("Text"))
                .codeExtraInformation.SetFunctionName("SetString").SetManipulatedType("string").SetAssociatedGetter("GetString").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("String",
                           _("Test the text"),
                           _("Test the text of a Text object."),
                           _("The text of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Text"),
                           "res/conditions/text24.png",
                           "res/conditions/text.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("string", _("Text to test"))
                .codeExtraInformation.SetFunctionName("GetString").SetManipulatedType("string").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("Font",
                           _("Font"),
                           _("Modify the font of the text."),
                           _("Change font of _PARAM0_ to _PARAM1_"),
                           _("Font"),
                           "res/actions/font24.png",
                           "res/actions/font.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("police", _("Font"))
                .codeExtraInformation.SetFunctionName("ChangeFont").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("Size",
                           _("Size"),
                           _("Modify the size of the text."),
                           _("Do _PARAM1__PARAM2_ to the size of the text of _PARAM0_"),
                           _("Size"),
                           "res/actions/characterSize24.png",
                           "res/actions/characterSize.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetCharacterSize").SetManipulatedType("number").SetAssociatedGetter("GetCharacterSize").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("Size",
                           _("Size"),
                           _("Test the size of the text"),
                           _("The size of the text of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Size"),
                           "res/conditions/characterSize24.png",
                           "res/conditions/characterSize.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Size to test"))
                .codeExtraInformation.SetFunctionName("GetCharacterSize").SetManipulatedType("number").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("ChangeColor",
                           _("Color"),
                           _("Change the color of the text. The color is white by default."),
                           _("Change color of _PARAM0_ to _PARAM1_"),
                           _("Effects"),
                           "res/actions/color24.png",
                           "res/actions/color.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("color", _("Color"))
                .codeExtraInformation.SetFunctionName("SetColor").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("Opacity",
                           _("Opacity"),
                           _("Modify the opacity of a Text object."),
                           _("Do _PARAM1__PARAM2_ to the opacity of _PARAM0_"),
                           _("Visibility"),
                           "res/actions/opacity24.png",
                           "res/actions/opacity.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetOpacity").SetManipulatedType("number").SetAssociatedGetter("GetOpacity").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("Opacity",
                           _("Opacity"),
                           _("Test the value of the opacity of a text object."),
                           _("The opacity of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Visibility"),
                           "res/conditions/opacity24.png",
                           "res/conditions/opacity.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Value to test"))
                .codeExtraInformation.SetFunctionName("GetOpacity").SetManipulatedType("number").SetIncludeFile("TextObject/TextObject.h");



            obj.AddAction("SetSmooth",
                           _("Smoothing"),
                           _("Activate or desactivate text smoothing."),
                           _("Smooth _PARAM0_: _PARAM1_"),
                           _("Visibility"),
                           "res/actions/opacity24.png",
                           "res/actions/opacity.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("yesorno", _("Smooth the text"))
                .codeExtraInformation.SetFunctionName("SetSmooth").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("Smoothed",
                           _("Smoothing"),
                           _("Test if an object is smoothed"),
                           _("_PARAM0_ is smoothed"),
                           _("Visibility"),
                           "res/conditions/opacity24.png",
                           "res/conditions/opacity.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("IsSmoothed").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("SetBold",
                           _("Bold"),
                           _("De/activate bold"),
                           _("Set bold style of _PARAM0_ : _PARAM1_"),
                           _("Style"),
                           "res/actions/bold.png",
                           "res/actions/bold16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("yesorno", _("Set bold style"))
                .codeExtraInformation.SetFunctionName("SetBold").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("IsBold",
                           _("Bold"),
                           _("Test if bold style is activated"),
                           _("_PARAM0_ bold style is set"),
                           _("Style"),
                           "res/conditions/bold.png",
                           "res/conditions/bold16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("IsBold").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("SetItalic",
                           _("Italic"),
                           _("De/activate italic."),
                           _("Set italic style for _PARAM0_ : _PARAM1_"),
                           _("Style"),
                           "res/actions/italic.png",
                           "res/actions/italic16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("yesorno", _("Set italic"))
                .codeExtraInformation.SetFunctionName("SetItalic").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("IsItalic",
                           _("Italic"),
                           _("Test if the italic style is activated"),
                           _("_PARAM0_ italic style is set"),
                           _("Style"),
                           "res/conditions/italic.png",
                           "res/conditions/italic16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("IsItalic").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("SetUnderlined",
                           _("Underlined"),
                           _("De/activate underlined style."),
                           _("Set underlined style of _PARAM0_: _PARAM1_"),
                           _("Style"),
                           "res/actions/underline.png",
                           "res/actions/underline16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("yesorno", _("Underline"))
                .codeExtraInformation.SetFunctionName("SetUnderlined").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("IsUnderlined",
                           _("Underlined"),
                           _("Test if the underlined style of an object is set."),
                           _("_PARAM0_ underlined style is activated"),
                           _("Style"),
                           "res/conditions/underline.png",
                           "res/conditions/underline16.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("IsUnderlined").SetIncludeFile("TextObject/TextObject.h");

            obj.AddAction("Angle",
                           _("Angle"),
                           _("Modify the angle of a Text object."),
                           _("Do _PARAM1__PARAM2_ to the angle of _PARAM0_"),
                           _("Rotation"),
                           "res/actions/rotate24.png",
                           "res/actions/rotate.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("operator", _("Modification's sign"))
                .AddParameter("expression", _("Value"))
                .codeExtraInformation.SetFunctionName("SetAngle").SetManipulatedType("number").SetAssociatedGetter("GetAngle").SetIncludeFile("TextObject/TextObject.h");

            obj.AddCondition("Angle",
                           _("Angle"),
                           _("Test the value of the angle of a text object."),
                           _("The angle of _PARAM0_ is _PARAM1__PARAM2_"),
                           _("Rotation"),
                           "res/conditions/rotate24.png",
                           "res/conditions/rotate.png")
                .AddParameter("object", _("Object"), "Text", false)
                .AddParameter("relationalOperator", _("Sign of the test"))
                .AddParameter("expression", _("Value to test"))
                .codeExtraInformation.SetFunctionName("GetAngle").SetManipulatedType("number").SetIncludeFile("TextObject/TextObject.h");

            obj.AddExpression("Opacity", _("Opacity"), _("Opacity"), _("Opacity"), "res/actions/opacity.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("GetOpacity").SetIncludeFile("TextObject/TextObject.h");

            obj.AddExpression("Angle", _("Angle"), _("Angle"), _("Rotation"), "res/actions/rotate.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("GetAngle").SetIncludeFile("TextObject/TextObject.h");

            obj.AddStrExpression("String", _("Text"), _("Text"), _("Text"), "res/texteicon.png")
                .AddParameter("object", _("Object"), "Text", false)
                .codeExtraInformation.SetFunctionName("GetString").SetIncludeFile("TextObject/TextObject.h");

        #endif
        }

        GD_COMPLETE_EXTENSION_COMPILATION_INFORMATION();
    };
    virtual ~CppExtension() {};
};

/**
 * Used by Game Develop to create the extension class
 * -- Do not need to be modified. --
 */
extern "C" ExtensionBase * GD_EXTENSION_API CreateGDExtension() {
    return new CppExtension;
}

/**
 * Used by Game Develop to destroy the extension class
 * -- Do not need to be modified. --
 */
extern "C" void GD_EXTENSION_API DestroyGDExtension(ExtensionBase * p) {
    delete p;
}
