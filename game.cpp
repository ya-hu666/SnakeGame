#include "game.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>

Game::Game()
    : snake(width, height), wall(width, height)
{
    entities.push_back(&wall);
    entities.push_back(&snake);
    entities.push_back(&food);
    drawBorder();
    generateFood();
}

void Game::reset()
{
    snake = SnakeEntity(width, height);
    food = Food();
    score = 0;
    speed = defaultSpeed;
    isGameOver = false;
    generateFood();
    entities.clear();
    entities.push_back(&wall);
    entities.push_back(&snake);
    entities.push_back(&food);
    drawBorder();
}

Game::GameState Game::menu()
{
    int consoleWidth, consoleHeight;
    getConsoleSize(consoleWidth, consoleHeight);
    system("cls");
    std::cout << "========================================================================" << '\n';
    std::cout << "                          Snake-Console" << '\n';
    std::cout << "========================================================================" << '\n' << '\n';
    std::cout << "[Control]" << '\n';
    std::cout << "  Arrow keys  : Move" << '\n';
    std::cout << "  W A S D     : Move" << '\n';
    std::cout << "  X           : Exit" << '\n';
    std::cout << "  Space       : Pause/Resume" << '\n' << '\n';
    std::cout << "[Rules]" << '\n';
    std::cout << "  Eat F to grow. Score += current speed level." << '\n';
    std::cout << "  Hit wall(#) or yourself = Game Over" << '\n' << '\n';
    std::cout << "[Info]" << '\n';
    std::cout << "  Console size : " << consoleWidth << " x " << consoleHeight << '\n';
    std::cout << "  1. Start game" << '\n';
    std::cout << "  2. Resize map" << '\n';
    std::cout << "  3. Set speed" << '\n';
    std::cout << "  Others : Exit" << '\n';
    if (width >= consoleWidth - 3 || height >= consoleHeight - 3) {
        std::cout << "Map may exceed console. Please resize (option 2)." << '\n';
    }
    std::cout << "Choice: ";
    int option;
    std::cin >> option;
    if (option == 1) {
        reset();
        return PLAYING;
    } else if (option == 2) {
        std::cout << "Width (5-" << consoleWidth - 4 << "): ";
        std::cin >> width;
        if (width < 5) { width = 5; }
        else if (width > consoleWidth - 3) { width = consoleWidth - 4; }
        std::cout << "Height (5-" << consoleHeight - 2 << "): ";
        std::cin >> height;
        if (height < 5) { height = 5; }
        else if (height > consoleHeight - 3) { height = consoleHeight - 4; }
        std::cout << "Resized successfully." << '\n';
        system("pause");
        return MENU;
    } else if (option == 3) {
        std::cout << "Speed (1-7): ";
        std::cin >> defaultSpeed;
        if (defaultSpeed < 1) defaultSpeed = 1;
        else if (defaultSpeed > 7) defaultSpeed = 7;
        speed = defaultSpeed;
        std::cout << "Speed set to " << speed << '\n';
        system("pause");
        return MENU;
    }
    return PLAYING;
}

Game::GameState Game::playing()
{
    while (state == PLAYING) {
        input();
        logic();
        if (speed < 1) speed = 1;
        if (speed > 7) speed = 7;
        draw();
        Sleep(delays[speed - 1]);
    }
    return state;
}

Game::GameState Game::pause()
{
    gotoxy(0, height + 3);
    std::cout << "Paused. Press Space to resume.";
    while (true) {
        int c = _getch();
        if (c == 32) return PLAYING;
        else if (tolower(c) == 'x') return MENU;
    }
    return PAUSE;
}

Game::GameState Game::End()
{
    system("cls");
    gotoxy(0, height / 2);
    showGameStatus(*this);
    std::cout << '\n';
    system("pause");
    std::cout << "1. Restart" << '\n';
    std::cout << "2. Menu" << '\n';
    std::cout << "Others : Exit" << '\n';
    while (true) {
        int ch = tolower(_getch());
        if (ch == '1') { reset(); return PLAYING; }
        else if (ch == '2') return MENU;
        else return EXIT;
    }
    return EXIT;
}

void Game::logic()
{
    if (!snake.logic(width, height, score)) {
        state = GAMEOVER;
        return;
    }
    if (snake.showHead() == food.getPos()) {
        score += speed;
        generateFood();
    } else {
        snake.popTail();
    }
}

void Game::draw()
{
    if (snake.prevTail.first != 0 && snake.prevTail.second != 0) {
        gotoxy(snake.prevTail.first, snake.prevTail.second);
        std::cout << " ";
    }

    gotoxy(food.getPos().first, food.getPos().second);

    for (auto* entity : entities) {
        entity->draw();
    }

    gotoxy(width / 2 - 2, height + 2);
    std::cout << "Score: " << score << "  Speed: " << speed << "  ";
    gotoxy(0, height + 3);
    std::cout << "Playing. Space to pause.     ";
}

void Game::input()
{
    Snake::Direction dir = snake.getDir();
    Snake::Direction oldDir = dir;
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            switch (key) {
            case 72: dir = Snake::UP; break;
            case 75: dir = Snake::LEFT; break;
            case 80: dir = Snake::DOWN; break;
            case 77: dir = Snake::RIGHT; break;
            }
        } else {
            char lowerKey = tolower(key);
            switch (lowerKey) {
            case 'w': dir = Snake::UP; break;
            case 'a': dir = Snake::LEFT; break;
            case 's': dir = Snake::DOWN; break;
            case 'd': dir = Snake::RIGHT; break;
            case 'x': state = MENU; break;
            case 32:  state = PAUSE; break;
            }
        }
        if (dir == oldDir) speed++;
        else if (abs(dir - oldDir) == 2) { dir = oldDir; speed--; }
        snake.setDir(dir);
    } else {
        speed = defaultSpeed;
    }
}

void Game::getConsoleSize(int& consoleWidth, int& consoleHeight)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void Game::gotoxy(int x, int y) const
{
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Game::generateFood()
{
    if (snake.getLength() >= width * height) {
        isGameOver = true;
        return;
    }
    Food newFood(width, height, snake);
    food = newFood;
    entities[2] = &food;
}

void Game::drawBorder()
{
    system("cls");
    wall.draw();
}

void showGameStatus(const Game& g)
{
    g.gotoxy(0, g.height / 2 + 1);
    std::cout << "GAME OVER! Final Score: " << g.score;
}
