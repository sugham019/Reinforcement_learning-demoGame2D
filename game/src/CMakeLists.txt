set(this Game)
find_package(SFML 2.6 REQUIRED graphics window system audio)

set(src 
    Game.cpp 
    EventHandler.cpp
    components/CollisionDetector.cpp
    components/MapChunkGenerator.cpp
    components/Renderer.cpp
    components/SpriteManager.cpp
    components/ErrorHandler.cpp
    
    entities/GameObject.cpp
    entities/Character.cpp
    entities/Bullet.cpp
    entities/Player.cpp
)

add_compile_options(-g)

add_library(${this} ${src})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS})
target_link_libraries(${this} sfml-graphics sfml-window sfml-system sfml-audio)

add_executable(app Application.cpp)
target_link_libraries(app Game ${TORCH_LIBRARIES})