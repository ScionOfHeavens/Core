cmake_minimum_required(VERSION 3.10)
project(OpenGLWindow)

# Настройка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем сборку лишних элементов GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Оптимизация сборки Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)  

# Подключаем исходный код зависимостей
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/assimp)

# Создаем исполняемый файл проекта
add_executable(${PROJECT_NAME} 
    main.cpp 
    stb_image.cpp
    dependencies/glad/src/glad.c
    camera.hpp
)

# Указываем пути к заголовочным файлам для нашего проекта
target_include_directories(${PROJECT_NAME} PRIVATE
    dependencies/glad/include
    dependencies/glfw/include
    dependencies/glm
    dependencies/stb
    
    # Исходные инклуды Assimp
    dependencies/assimp/include
    # ВАЖНО: сгенерированные CMake файлы Assimp (здесь лежит config.h)
    ${CMAKE_CURRENT_BINARY_DIR}/dependencies/assimp/include
    ${CMAKE_CURRENT_BINARY_DIR}/dependencies/assimp
)

# Связываем проект с библиотеками
target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif()

# ====================================================================
# АВТОМАТИЧЕСКОЕ КОПИРОВАНИЕ РЕСУРСОВ В ПАПКУ СБОРКИ (build)
# ====================================================================

# Копируем всю папку shaders
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
