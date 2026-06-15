#include "game.h"
#include "entity.h"
#include <iostream>
#include <Windows.h>

void getConsoleSize(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void hiderCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    hiderCursor();

    Game game;
    while (game.state != Game::EXIT) {
        switch (game.state) {
        case Game::MENU:
            game.state = game.menu();
            break;
        case Game::PLAYING:
            game.state = game.playing();
            break;
        case Game::PAUSE:
            game.state = game.pause();
            break;
        case Game::GAMEOVER:
            game.state = game.End();
            break;
        default: break;
        }
    }
    system("pause");
    return 0;
}
