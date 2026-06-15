#pragma once
#include "snake.h"
#include "food.h"
#include "wall.h"
#include <vector>

class Game {
    friend void showGameStatus(const Game& g);

    int width = 50;
    int height = 25;
    int score = 0;
    SnakeEntity snake;
    Food food;
    Wall wall;
    bool isGameOver = false;
    int defaultSpeed = 3;
    int speed = 3;

    std::vector<Entity*> entities;

    void logic();
    void draw();
    void input();
    void getConsoleSize(int&, int&);
    void drawBorder();
    void gotoxy(int, int) const;

public:
    enum GameState { MENU, PLAYING, GAMEOVER, PAUSE, EXIT };
    GameState state = MENU;
    static constexpr int delays[] = {250, 175, 122, 85, 60, 42, 20};
    static constexpr int SPEED_LEVEL_MIN = 1;
    static constexpr int SPEED_LEVEL_MAX = 7;

    Game();
    void generateFood();
    void reset();
    GameState menu();
    GameState playing();
    GameState pause();
    GameState End();
};

void showGameStatus(const Game& g);
