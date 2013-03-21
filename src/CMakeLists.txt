set ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )

Find_Package ( SDL REQUIRED )
Find_Package ( SDL_image REQUIRED )
Find_Package ( SDL_mixer REQUIRED )
Find_Package ( SDL_ttf REQUIRED )

if ( NOT SDL_FOUND )
	message ( FATAL_ERROR "SDL not found!" )
	endif ( NOT SDL_FOUND )

add_executable (../aquarium main.c)

target_link_libraries (
	../aquarium
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLMIXER_LIBRARY}
	${SDLTTF_LIBRARY}
	SDLmain
)
