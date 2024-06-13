
file(GLOB files LIST_DIRECTORIES false  icons/*)
foreach(f ${files})
	get_filename_component(filename  ${f} NAME)
	list(APPEND _APP_RC_FILES asset/icons/${filename})
endforeach()
set(_APP_RC_FILES ${_APP_RC_FILES} PARENT_SCOPE)
