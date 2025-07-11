add_executable(${PROJECT_NAME} main.cpp BattleshipGame.cpp)
target_sources(${PROJECT_NAME} PRIVATE 
    main.cpp 
    BattleshipGame.cpp 
    BattleshipGame.h
    GameState.cpp
    GameState.h
    Grid.cpp
    Grid.h
    Ship.cpp
    Ship.h
    AIPlayer.cpp
    AIPlayer.h
    Renderer.cpp
    Renderer.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)