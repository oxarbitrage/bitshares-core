file(GLOB HEADERS "include/graphene/lua/*.hpp")

add_library( graphene_lua
        lua.cpp
           )

target_link_libraries( graphene_lua graphene_chain graphene_app lua5.2 )
target_include_directories( graphene_lua
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if(MSVC)
  set_source_files_properties(lua.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   graphene_lua

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/graphene/lua" )

