#This is the CMake file used to build the extensions.
#For more information, see the Readme.txt file.

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0011 NEW)

project(GD-Extensions)

#Sanity checks
IF ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  message( "CMAKE_BUILD_TYPE is empty, assuming build type is Release" )
	set(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(AES)
ADD_SUBDIRECTORY(AStarAutomatism)
ADD_SUBDIRECTORY(Box3DObject)
ADD_SUBDIRECTORY("Common Dialogs")
ADD_SUBDIRECTORY(DestroyOutsideAutomatism)
ADD_SUBDIRECTORY(DraggableAutomatism)
ADD_SUBDIRECTORY(Function)
ADD_SUBDIRECTORY(Light)
ADD_SUBDIRECTORY(LinkedObjects)
ADD_SUBDIRECTORY(Network)
ADD_SUBDIRECTORY(ParticleSystem)
ADD_SUBDIRECTORY(PanelSpriteObject)
ADD_SUBDIRECTORY(PathAutomatism)
ADD_SUBDIRECTORY(PhysicsAutomatism)
ADD_SUBDIRECTORY(PrimitiveDrawing)
ADD_SUBDIRECTORY(SoundObject)
ADD_SUBDIRECTORY(TextEntryObject)
ADD_SUBDIRECTORY(TextObject)
ADD_SUBDIRECTORY(TiledSpriteObject)
ADD_SUBDIRECTORY(TimedEvent)