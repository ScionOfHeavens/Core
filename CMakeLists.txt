cmake_minimum_required(VERSION 3.10)
project(OpenGLWindow)

# Настройка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем сборку лишних элементов GLFW (документация, тесты)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Подключаем исходный код GLFW
add_subdirectory(dependencies/glfw)

# Указываем пути к заголовочным файлам GLAD, GLFW, GLM и STB
include_directories(dependencies/glad/include)
include_directories(dependencies/glfw/include)
include_directories(dependencies/glm)
include_directories(dependencies/stb)

# Создаем исполняемый файл проекта, включая исходник GLAD
add_executable(${PROJECT_NAME} 
    main.cpp 
    stb_image.cpp
    dependencies/glad/src/glad.c
    camera.h
)

# Связываем проект с библиотеками GLFW и системным OpenGL
target_link_libraries(${PROJECT_NAME} glfw)

if(WIN32)
    # На Windows системная библиотека называется opengl32
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(NOT APPLE)
    # На Linux библиотека называется OpenGL
    target_link_libraries(${PROJECT_NAME} OpenGL)
else()
    # На macOS используется фреймворк OpenGL
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# ====================================================================
# АВТОМАТИЧЕСКОЕ КОПИРОВАНИЕ РЕСУРСОВ В ПАПКУ СБОРКИ (build)
# ====================================================================

# Копируем папку с текстурами
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/textures"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# ИСПРАВЛЕНО: Теперь копируем всю папку shaders со всем ее содержимым
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
