file(GLOB HEADERS "include/graphene/wren/*.hpp")
file(GLOB HEADERS_WREN "wren/src/include/*.hpp")


add_library( graphene_wren
        wren_plugin.cpp
           )

target_link_libraries( graphene_wren graphene_chain graphene_app wren)
target_include_directories( graphene_wren
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
  set_source_files_properties(wren_plugin.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   graphene_wren

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/graphene/wren" )

