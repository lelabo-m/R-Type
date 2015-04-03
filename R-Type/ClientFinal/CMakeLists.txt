# Client CMake

cmake_minimum_required(VERSION 2.8)

set (PROJECT "client")

set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

project(${PROJECT} CXX)

if (WIN32)
    set (CMAKE_CXX_FLAGS_DEBUG "-Od -DEBUG -ZI -MDd")
    set (CMAKE_CXX_FLAGS_RELEASE "-O2")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W3")
else ()
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
endif ()

# SRCDIRS
set (MAINSRCDIR "./ClientFinal")
set (COMMONSRCDIR "../Common/Source")
set (NETSRCDIR "../Network/Sources")
set (SFMLSHAPESSRCDIR "../menuRtype/Sources/SFMLshapes")
set (SFMLMENUSSRCDIR "../menuRtype/Sources/SFMLmenus")
set (SFMLEVENTSSRCDIR "../menuRtype/Sources/SFMLevents")
set (AUDIOSRCDIR "../menuRtype/Sources/Audio")
set (SINGLETONSSRCDIR "../menuRtype/Sources/Singletons")
set (MENUSRCDIR "../menuRtype/Sources")

# INCDIRS
set (MAININCDIR "./ClientFinal")
set (NETINCDIR "../Network/Headers")
set (MENUINCDIR "../menuRtype/Includes")
set (SFMLSHAPESINCDIR "../menuRtype/Includes/SFMLshapes")
set (SFMLEVENTSINCDIR "../menuRtype/Includes/SFMLevents")
set (SFMLMENUSINCDIR "../menuRtype/Includes/SFMLmenus")
set (AUDIODIR "../menuRtype/Includes/Audio")
set (SINGLETONSINCDIR "../menuRtype/Includes/Singletons")
set (COMMONINCDIR "../Common/Header")

include_directories(${MAININCDIR} ${NETINCDIR} ${MENUINCDIR} ${SFMLSHAPESINCDIR} ${SFMLEVENTSINCDIR} ${SFMLMENUSINCDIR} ${AUDIODIR} ${SINGLETONSINCDIR} ${COMMONINCDIR})

set (MAINSRCS
    ${MAINSRCDIR}/BackgroundAnimation.cpp
    ${MAINSRCDIR}/Client.cpp
    ${MAINSRCDIR}/ClientManager.cpp
    ${MAINSRCDIR}/Game.cpp
    ${MAINSRCDIR}/GamePadController.cpp
    ${MAINSRCDIR}/GameSession.cpp
    ${MAINSRCDIR}/GraphicalElem.cpp
    ${MAINSRCDIR}/KeyBoardController.cpp
    ${MAINSRCDIR}/Lobby.cpp
    ${MAINSRCDIR}/MapDownloader.cpp
    ${MAINSRCDIR}/SFMLchatText.cpp
    ${MAINSRCDIR}/SFMLstartEditorPopUp.cpp
    ${MAINSRCDIR}/TickTimer.cpp
    ${MAINSRCDIR}/InputPopupValue.cpp
    ${MAINSRCDIR}/MapSelected.cpp
    ${MAINSRCDIR}/SFMLloadingGamePopUp.cpp
    ${MAINSRCDIR}/RTypeGame.cpp
    ${MAINSRCDIR}/GameEventManager.cpp
    ${MAINSRCDIR}/SFMLpausePopUp.cpp
    ${MAINSRCDIR}/SFMLscore.cpp
    ${MAINSRCDIR}/SFMLendingGamePopUp.cpp
    ${MAINSRCDIR}/main.cpp)

set (COMMONSRCS
    ${COMMONSRCDIR}/Vector2D.cpp
    ${COMMONSRCDIR}/Level.cpp
    ${COMMONSRCDIR}/XML_Level.cpp
    ${COMMONSRCDIR}/tinyxml2.cpp)

set (NETSRCS
    ${NETSRCDIR}/FileManager.cpp
    ${NETSRCDIR}/TCPFrame.cpp
    ${NETSRCDIR}/TCPHeader.cpp
    ${NETSRCDIR}/NetClient.cpp
    ${NETSRCDIR}/TCPMessageFactoryClient.cpp
    ${NETSRCDIR}/Session.cpp
    ${NETSRCDIR}/TCPMessageFactory.cpp
    ${NETSRCDIR}/NetClientManager.cpp
    ${NETSRCDIR}/UDPFrame.cpp
    ${NETSRCDIR}/NetUser.cpp
    ${NETSRCDIR}/SessionManager.cpp
    ${NETSRCDIR}/TCPManagerClient.cpp
    ${NETSRCDIR}/TCPManager.cpp
    ${NETSRCDIR}/UDPManagerClient.cpp
    ${NETSRCDIR}/UDPManager.cpp
    ${NETSRCDIR}/UDPHeader.cpp
    ${NETSRCDIR}/TCPSocket.cpp
    ${NETSRCDIR}/UDPSocket.cpp
    ${NETSRCDIR}/GameMessage.cpp
    ${NETSRCDIR}/ObjectRepresentation.cpp
    ${NETSRCDIR}/PlayerAction.cpp)

set (SFMLSHAPESSRCS
    ${SFMLSHAPESSRCDIR}/SFMLanimatedSprite.cpp
    ${SFMLSHAPESSRCDIR}/SFMLshape.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbasicText.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbigButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbuttonLauncher.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbutton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbasicSlider.cpp
    ${SFMLSHAPESSRCDIR}/SFMLinputLauncher.cpp
    ${SFMLSHAPESSRCDIR}/SFMLslider.cpp
    ${SFMLSHAPESSRCDIR}/SFMLlittleButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLmultigameButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLdropdown.cpp
    ${SFMLSHAPESSRCDIR}/SFMLinput.cpp
    ${SFMLSHAPESSRCDIR}/SFMLdropdownMultiplayer.cpp
    ${SFMLSHAPESSRCDIR}/SFMLplayermulti.cpp
    ${SFMLSHAPESSRCDIR}/SFMLunrotateDropdown.cpp
    ${SFMLSHAPESSRCDIR}/SFMLsubmenuButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLmapselectorButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLmonsterDnD.cpp
    ${SFMLSHAPESSRCDIR}/SFMLmonsterButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLeditormenuButton.cpp
    ${SFMLSHAPESSRCDIR}/SFMLcharacterInput.cpp
    ${SFMLSHAPESSRCDIR}/SFMLbasicInput.cpp
    ${SFMLSHAPESSRCDIR}/SFMLobjDragAndDrop.cpp)

set (SFMLMENUSSRCS
    ${SFMLMENUSSRCDIR}/SFMLbasePopUp.cpp
    ${SFMLMENUSSRCDIR}/SFMLoptionMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLeditor.cpp
    ${SFMLMENUSSRCDIR}/SFMLmenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLlauncherMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLplaymapSelectorMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLmultiMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLlevelscoreMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLplayMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLlengthlevelPopUp.cpp
    ${SFMLMENUSSRCDIR}/SFMLmultiplayerLobbyMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLmainMenu.cpp
    ${SFMLMENUSSRCDIR}/SFMLbaseEditor.cpp)

set (SFMLEVENTSSRCS
    ${SFMLEVENTSSRCDIR}/SFMLbuttonevents.cpp
    ${SFMLEVENTSSRCDIR}/EventManager.cpp
    ${SFMLEVENTSSRCDIR}/EditorEventManager.cpp)

set (AUDIOSRCS
    ${AUDIOSRCDIR}/Playlist.cpp
    ${AUDIOSRCDIR}/Sound.cpp)

set (SINGLETONSSRCS
    ${SINGLETONSSRCDIR}/windowSFML.cpp
    ${SINGLETONSSRCDIR}/databaseSFML.cpp
    ${SINGLETONSSRCDIR}/menuManager.cpp
    ${SINGLETONSSRCDIR}/gameOptions.cpp)

set (MENUSRCS
    ${MENUSRCDIR}/Timer.cpp)

set (${PROJECT}_SOURCES
    ${MAINSRCS}
    ${COMMONSRCS}
    ${NETSRCS}
    ${SFMLSHAPESSRCS}
    ${SINGLETONSSRCS}
    ${SFMLMENUSSRCS}
    ${AUDIOSRCS}
    ${SFMLEVENTSSRCS}
    ${MENUSRCS})

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

set (LIBS_PATH "../menuRtype/Libs")

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (SFML_LIBS_SUFFIX "-d")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set (SFML_LIBS_SUFFIX "-s-d")
    endif ()
    set (FMODNAME "fmodex_vc")
else()
    set (FMODNAME "fmodex")
endif ()

find_library(FMOD ${FMODNAME} PATHS ${LIBS_PATH} NO_DEFAULT_PATH)
find_library(SFML_SYSTEM "sfml-system${SFML_LIBS_SUFFIX}" PATHS ${LIBS_PATH} NO_DEFAULT_PATH)
find_library(SFML_GRAPHICS "sfml-graphics${SFML_LIBS_SUFFIX}" PATHS ${LIBS_PATH} NO_DEFAULT_PATH)
find_library(SFML_WINDOW "sfml-window${SFML_LIBS_SUFFIX}" PATHS ${LIBS_PATH} NO_DEFAULT_PATH)
target_link_libraries(${PROJECT} ${FMOD} ${SFML_SYSTEM} ${SFML_GRAPHICS} ${SFML_WINDOW})
