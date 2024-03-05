add_custom_target(OpenGLShaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/OpenGL ${CMAKE_CURRENT_BINARY_DIR}/OpenGL
)

set(GLSLC "glslc")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "Vulkan/*.frag"
    "Vulkan/*.vert"
    )

foreach(GLSL ${GLSL_GL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
    COMMAND ${GLSLC} -O --target-env=opengl ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    VulkanShaders
    DEPENDS ${SPIRV_BINARY_FILES}
    )
