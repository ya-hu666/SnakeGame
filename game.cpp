#include "game.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
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
    std::cout << "                          贪吃蛇-控制台版" << '\n';
    std::cout << "========================================================================" << '\n' << '\n';
    std::cout << "【控制】" << '\n';
    std::cout << "  方向键    : 移动" << '\n';
    std::cout << "  W A S D   : 移动" << '\n';
    std::cout << "  X         : 退出" << '\n';
    std::cout << "  空格      : 暂停/恢复" << '\n' << '\n';
    std::cout << "【规则】" << '\n';
    std::cout << "  吃F变长，得分+=当前速度等级" << '\n';
    std::cout << "  撞墙(#)或撞到自己=游戏结束" << '\n' << '\n';
    std::cout << "【信息】" << '\n';
    std::cout << "  控制台窗口大小：" << consoleWidth << " x " << consoleHeight << '\n';
    std::cout << "1. 开始游戏" << '\n';
    std::cout << "2. 调整地图大小" << '\n';
    std::cout << "3. 调整速度" << '\n';
    std::cout << "其他任意键退出" << '\n';
    if (width >= consoleWidth - 3 || height >= consoleHeight - 3) {
        std::cout << "当前窗口可能显示不全，建议调整窗口后选择2" << '\n';
    }
    std::cout << "请输入：";
    int option;
    std::cin >> option;
    if (option == 1) {
        reset();
        return PLAYING;
    } else if (option == 2) {
        std::cout << "请输入地图宽度(5-" << consoleWidth - 4 << "):";
        std::cin >> width;
        if (width < 5) { width = 5; std::cout << "输入宽度太小，已自适应为5" << '\n'; }
        else if (width > consoleWidth - 3) { width = consoleWidth - 4; std::cout << "输入宽度太大，已自适应为" << consoleWidth - 4 << '\n'; }
        std::cout << "请输入地图高度(5-" << consoleHeight - 2 << "):";
        std::cin >> height;
        if (height < 5) { height = 5; std::cout << "输入高度太小，已自适应为5" << '\n'; }
        else if (height > consoleHeight - 3) { height = consoleHeight - 4; std::cout << "输入高度太大，已自适应为" << consoleWidth - 4 << '\n'; }
        std::cout << "调整窗口成功" << '\n';
        system("pause");
        return MENU;
    } else if (option == 3) {
        std::cout << "请选择默认初始速度（1-7）：";
        std::cin >> defaultSpeed;
        if (defaultSpeed < 1) { defaultSpeed = 1; std::cout << "输入速度太小，已自适应为1" << '\n'; }
        else if (defaultSpeed > 7) { defaultSpeed = 7; std::cout << "输入速度太大，已自适应为7" << '\n'; }
        speed = defaultSpeed;
        std::cout << "设置速度成功，当前速度为：" << speed << '\n';
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
    std::cout << "游戏已暂停，请按空格恢复";
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
    std::cout << "1. 重新开始" << '\n';
    std::cout << "2. 返回菜单" << '\n';
    std::cout << "其他任意键退出" << '\n';
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

    for (auto* entity : entities) {
        entity->draw();
    }

    gotoxy(width / 2 - 2, height + 2);
    std::cout << "得分：" << score << "  当前速度为：" << speed << "  ";
    gotoxy(0, height + 3);
    std::cout << "游戏进行中，可按空格暂停    ";
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
    std::cout << "游戏结束！最终得分：" << g.score;
}
