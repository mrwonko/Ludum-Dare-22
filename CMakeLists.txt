cmake_minimum_required(VERSION 2.6)

# name + language
project(MiniBlueBoxBoy CXX)

set(SOURCE_FILES
	"Constants.cpp"
	"EditAction_Click.cpp"
	"EditAction_MovePlayer.cpp"
	"EditAction_NewPlayer.cpp"
	"EditAction_NewStaticRect.cpp"
	"EditAction_Remove.cpp"
	"EventListener.cpp"
	"EventListenerList.cpp"
	"Helpers.cpp"
	"Level.cpp"
	"Object.cpp"
	"ParticleSystem.cpp"
	"Player.cpp"
	"StaticRect.cpp"
	"UI.cpp"
	"UIElement.cpp"
	"UIText.cpp"
	"main.cpp"
	"sfmlBox2DDebugDraw.cpp"
	)
set(HEADER_FILES
	"Constants.h"
	"EditAction.h"
	"EditAction_Click.h"
	"EditAction_MovePlayer.h"
	"EditAction_NewPlayer.h"
	"EditAction_NewStaticRect.h"
	"EditAction_Remove.h"
	"EventListener.h"
	"EventListenerList.h"
	"Helpers.h"
	"Level.h"
	"Object.h"
	"Particle.h"
	"ParticleSystem.h"
	"Player.h"
	"Sounds.h"
	"StaticRect.h"
	"Textures.h"
	"UI.h"
	"UIElement.h"
	"UIText.h"
	"sfmlBox2DDebugDraw.h"
	)

add_executable(MiniBlueBoxBoy ${SOURCE_FILES} ${HEADER_FILES})

# enable all warnings
add_definitions(-Wall)

# add make_modules directory to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# search SFML modules
find_package(SFML 2 REQUIRED system window graphics)

# find Box2D
find_path(BOX2D_INCLUDE_DIR Box2D/Box2D.h
	HINTS
		ENV BOX2D_DIR
	PATH_SUFFIXES include
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/sw
	/opt/local
	/opt/csw
	/opt
)

find_library(BOX2D_LIBRARY
  NAMES Box2D
  HINTS
    ENV BOX2D_DIR
  PATH_SUFFIXES lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
)

if    (NOT BOX2D_LIBRARY)
	message(FATAL_ERROR "Could not find Box2D Library")
endif (NOT BOX2D_LIBRARY)
if    (NOT BOX2D_INCLUDE_DIR)
	message(FATAL_ERROR "Could not find Box2D Header files")
endif (NOT BOX2D_INCLUDE_DIR)

# set include directories
include_directories("${CMAKE_SOURCE_DIR}/include" ${SFML_INCLUDE_DIR} ${BOX2D_INCLUDE_DIR})
# set link targets
target_link_libraries(MiniBlueBoxBoy ${SFML_LIBRARIES} ${BOX2D_LIBRARY})

install(TARGETS MiniBlueBoxBoy DESTINATION "${CMAKE_SOURCE_DIR}/bin")