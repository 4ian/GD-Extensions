Official extensions for Game Develop
====================================

These are the official extensions directly bundled with Game Develop ( Free game development
software available on http://www.compilgames.net )


Download the Game Develop SDK
-----------------------------

To work on the extensions, you need to have the Game Develop SDK:
If it is not already done, just go on http://www.compilgames.net and download the SDK.

Then, extract it somewhere and put all the files from this repository in a folder 
called "Extensions", inside the Game Develop SDK folder.

Getting started
---------------

First, take a look at the Readme and the documentation provided in the Game Develop SDK.
Extensions always contains an Extension.cpp and/or a JsExtension.cpp file that are used
to expose the extension to Game Develop. 

After being compiled, extensions binaries are put in the Binaries/Output/Release/CppPlatform/Extensions
( or Binaries/Output/Release/JsPlatform/Extensions ) folder of the SDK.

Contributing
------------

Any contribution is welcome! Whether you want to submit a bug report, a feature request 
or any pull request so as to add a nice feature, do not hesitate to get in touch.

License
-------

Extensions are provided under the zlib/libpng License.
See the notice at the top of each source file of an extension for any particular information.
External libraries can be used by extensions ( Box2D, Dlib or SPARK for example ). See their
licenses in their directories.