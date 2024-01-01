add_custom_target(OpenGLShaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/OpenGL ${CMAKE_CURRENT_BINARY_DIR}/OpenGL
)
