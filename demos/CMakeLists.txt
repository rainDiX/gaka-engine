file(COPY "${CMAKE_CURRENT_LIST_DIR}/../shaders/OpenGL" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


add_executable(bezier BezierSurface.cpp)
add_sanitizers(bezier)
target_include_directories(bezier PRIVATE ${gaka_include_dir})
target_link_libraries(bezier PUBLIC gakaGUIOpenGL ${CMAKE_DL_LIBS})

add_executable(anim Animation.cpp)
add_sanitizers(anim)
target_include_directories(anim PRIVATE ${gaka_include_dir})
target_link_libraries(anim PUBLIC gakaGUIOpenGL ${CMAKE_DL_LIBS})


add_executable(pbrDemo pbrDemo.cpp)
add_sanitizers(pbrDemo)
target_include_directories(pbrDemo PRIVATE ${gaka_include_dir})
target_link_libraries(pbrDemo PUBLIC gakaGUIOpenGL ${CMAKE_DL_LIBS})
