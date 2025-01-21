#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int WINDOW_SIZE = 600;
const int GRID_SIZE = 3;
const float CELL_SIZE = WINDOW_SIZE / GRID_SIZE;

enum class Player { None, X, O };

class Game {
private:
    sf::RenderWindow window;
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    bool gameOver;

public:
    Game() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic-Tac-Toe"), board(GRID_SIZE, std::vector<Player>(GRID_SIZE, Player::None)), currentPlayer(Player::X), gameOver(false) {}

    void run() {
        while (window.isOpen()) {
            handleEvents();
            window.clear(sf::Color::White);
            drawGrid();
            drawBoard();
            window.display();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (!gameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }

    void handleClick(int x, int y) {
        int row = y / CELL_SIZE;
        int col = x / CELL_SIZE;

        if (board[row][col] == Player::None) {
            board[row][col] = currentPlayer;
            if (checkWin(row, col)) {
                gameOver = true;
                std::cout << "Player " << (currentPlayer == Player::X ? "X" : "O") << " wins!" << std::endl;
            } else if (checkDraw()) {
                gameOver = true;
                std::cout << "It's a draw!" << std::endl;
            } else {
                currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
            }
        }
    }

    bool checkWin(int row, int col) {
        // Check row
        if (std::all_of(board[row].begin(), board[row].end(), [&](Player p) { return p == currentPlayer; }))
            return true;

        // Check column
        if (std::all_of(board.begin(), board.end(), [&](const std::vector<Player>& r) { return r[col] == currentPlayer; }))
            return true;

        // Check diagonal
        if (row == col && std::all_of(board.begin(), board.end(), [&, i = 0](const std::vector<Player>& r) mutable { return r[i++] == currentPlayer; }))
            return true;

        // Check anti-diagonal
        if (row + col == GRID_SIZE - 1 && std::all_of(board.begin(), board.end(), [&, i = GRID_SIZE - 1](const std::vector<Player>& r) mutable { return r[i--] == currentPlayer; }))
            return true;

        return false;
    }

    bool checkDraw() {
        return std::all_of(board.begin(), board.end(), [](const std::vector<Player>& row) {
            return std::all_of(row.begin(), row.end(), [](Player p) { return p != Player::None; });
        });
    }

    void drawGrid() {
        sf::RectangleShape line(sf::Vector2f(WINDOW_SIZE, 2));
        line.setFillColor(sf::Color::Black);

        for (int i = 1; i < GRID_SIZE; ++i) {
            line.setPosition(0, i * CELL_SIZE);
            window.draw(line);
            line.setSize(sf::Vector2f(2, WINDOW_SIZE));
            line.setPosition(i * CELL_SIZE, 0);
            window.draw(line);
            line.setSize(sf::Vector2f(WINDOW_SIZE, 2));
        }
    }

    void drawBoard() {
        sf::Font font;
        if (!font.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/ARIAL.TTF")) {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(CELL_SIZE / 2);
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);

        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                if (board[row][col] != Player::None) {
                    text.setString(board[row][col] == Player::X ? "X" : "O");
                    text.setPosition(col * CELL_SIZE + CELL_SIZE / 4, row * CELL_SIZE);
                    window.draw(text);
                }
            }
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}