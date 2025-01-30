#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


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
    bool gameOver, gameStarted;
    // bool gameStarted;
    sf::Texture xTexture, oTexture;
    // sf::Texture oTexture;
    sf::Sprite xSprite, oSprite;
    // sf::Sprite oSprite;
    sf::RectangleShape restartButton;
    sf::Text restartButtonText;


public:
     Game()
        : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic-Tac-Toe"),
          board(GRID_SIZE, std::vector<Player>(GRID_SIZE, Player::None)),
          currentPlayer(Player::X),
          gameOver(false),
          gameStarted(false)
    {
        // Load textures and font in the constructor
        if (!xTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/xIcon.png")) {
            std::cerr << "Failed to load X image!" << std::endl;
            exit(1);
        }
        if (!oTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/oIcon.png")) {
            std::cerr << "Failed to load O image!" << std::endl;
            exit(1);
        }

        sf::Font font;
        if (!font.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/ARIAL.TTF")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(1);
        }

        xSprite.setTexture(xTexture);
        oSprite.setTexture(oTexture);

        // Adjust the scaling to make the images smaller
        float scaleFactor = 0.7f; // Adjust this value to shrink the images
        xSprite.setScale(scaleFactor * NEW_CELL_SIZE / xTexture.getSize().x,
                         scaleFactor * NEW_CELL_SIZE / xTexture.getSize().y);
        oSprite.setScale(scaleFactor * NEW_CELL_SIZE / oTexture.getSize().x,
                         scaleFactor * NEW_CELL_SIZE / oTexture.getSize().y);

        // Initialize restart button and text
        restartButton.setSize(sf::Vector2f(150, 50));
        restartButton.setFillColor(sf::Color::Green);
        restartButton.setPosition(WINDOW_SIZE / 2 - 75, WINDOW_SIZE - 80);

        restartButtonText.setFont(font);
        restartButtonText.setString("Restart");
        restartButtonText.setCharacterSize(20);
        restartButtonText.setFillColor(sf::Color::Black);
        restartButtonText.setPosition(WINDOW_SIZE / 2 - 50, WINDOW_SIZE - 70);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            window.clear(sf::Color::White);
            if (gameStarted) {
                drawGrid();
                drawBoard();
            } else {
                drawStartScreen();
            }
            window.display();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // You can keep this for when the user manually closes the window.
            }else if (gameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    resetGame();
                }
            }

            else if (!gameStarted && event.type == sf::Event::MouseButtonPressed) {
                if (isStartButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                    gameStarted = true;
                }
            }else if (gameStarted && !gameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleClick(event.mouseButton.x, event.mouseButton.y);
            }

        }
    }

    bool isStartButtonClicked(int x, int y) {
        sf::FloatRect startButtonBounds(WINDOW_SIZE / 2 - 100, WINDOW_SIZE / 2 - 50, 200, 100);
        return startButtonBounds.contains(x, y);
    }

    void resetGame() {
        std::cout << "Game Reset!" << std::endl;
        board = std::vector<std::vector<Player>>(GRID_SIZE, std::vector<Player>(GRID_SIZE, Player::None));
        currentPlayer = Player::X;
        gameOver = false;
    }

    void handleClick(int x, int y) {
        if (gameOver) return;  // Prevent any further moves if the game is over.

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

    const float GRID_OFFSET = 100; // Space from the top of the window to center the grid
    const float NEW_CELL_SIZE = CELL_SIZE / 1.5f; // Reduce the grid size
    void drawGrid() {
        sf::RectangleShape line(sf::Vector2f(GRID_SIZE * NEW_CELL_SIZE, 2));
        line.setFillColor(sf::Color::Black);

        for (int i = 1; i < GRID_SIZE; ++i) {
            // Horizontal lines
            line.setPosition(GRID_OFFSET, GRID_OFFSET + i * NEW_CELL_SIZE);
            window.draw(line);

            // Vertical lines
            line.setSize(sf::Vector2f(2, GRID_SIZE * NEW_CELL_SIZE));
            line.setPosition(GRID_OFFSET + i * NEW_CELL_SIZE, GRID_OFFSET);
            window.draw(line);

            line.setSize(sf::Vector2f(GRID_SIZE * NEW_CELL_SIZE, 2)); // Reset line size
        }
    }

    void drawBoard() {
        sf::Font font;
        if (!font.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/ARIAL.TTF")) {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }

        sf::Text statusText;
        statusText.setFont(font);
        statusText.setCharacterSize(30);
        statusText.setFillColor(sf::Color::Blue);
        statusText.setPosition(20, 10);

        if (gameOver) {
            if (checkDraw()) {
                statusText.setString("It's a Draw!");
            } else {
                statusText.setString("Player " + std::string(currentPlayer == Player::X ? "One" : "Two") + " Wins!");
            }
            window.draw(restartButton);
            window.draw(restartButtonText);
        } else {
            statusText.setString(currentPlayer == Player::X ? "Player One's Turn (X)" : "Player Two's Turn (O)");
        }

        window.draw(statusText);

        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                if (board[row][col] == Player::X) {
                    xSprite.setPosition(GRID_OFFSET + col * NEW_CELL_SIZE + NEW_CELL_SIZE * 0.15f,
                                        GRID_OFFSET + row * NEW_CELL_SIZE + NEW_CELL_SIZE * 0.15f);
                    window.draw(xSprite);
                } else if (board[row][col] == Player::O) {
                    oSprite.setPosition(GRID_OFFSET + col * NEW_CELL_SIZE + NEW_CELL_SIZE * 0.15f,
                                        GRID_OFFSET + row * NEW_CELL_SIZE + NEW_CELL_SIZE * 0.15f);
                    window.draw(oSprite);
                }
            }
        }
    }




    void drawStartScreen() {

        sf::Texture backgroundImg;
        if (!backgroundImg.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/spaceBackground.jpeg")) {
            std::cerr << "Failed to load background" << std::endl;
            return;
        }

        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundImg);
        window.draw(backgroundSprite);

        sf::Font font;
        if (!font.loadFromFile("/Users/muhammadbenoun/CLionProjects/XOXO/Recourse/ARIAL.TTF")) {
            std::cerr << "Failed to load font" << std::endl;
            return;
        }

        sf::Text title;
        title.setFont(font);
        title.setString("Tic-Tac-Toe");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Black);
        title.setPosition(WINDOW_SIZE / 2 - 130, WINDOW_SIZE / 4);

        sf::RectangleShape button(sf::Vector2f(200, 100));
        button.setFillColor(sf::Color::Green);
        button.setPosition(WINDOW_SIZE / 2 - 100, WINDOW_SIZE / 2 - 50);

        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString("Start Game");
        buttonText.setCharacterSize(30);
        buttonText.setFillColor(sf::Color::Black);
        buttonText.setPosition(WINDOW_SIZE / 2 - 75, WINDOW_SIZE / 2 - 30);

        window.draw(title);
        window.draw(button);
        window.draw(buttonText);
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
