#ifndef Game_hpp
#define Game_hpp\

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

/*
    This class acts as the game engine.
*/

class Game {
private:
    //Variables
    //Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    //Mouse positions (int vs. float)
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Game logic
    int points;
    int health;
    float speed;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    float powerUpSpawnTimer;
    float powerUpSpawnTimerMax;
    bool healExists;
    bool slowExists;
    int slowAmount;
    bool gameOver;
    bool playerReady;

    //Game Objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;
    sf::SoundBuffer enemyKillBuffer;
    sf::Sound enemy_kill_sfx;
    sf::SoundBuffer enemyMissedBuffer;
    sf::Sound enemy_missed_sfx;

    sf::CircleShape player;

    sf::Texture healTexture;
    sf::SoundBuffer healBuffer;
    sf::Sound heal_sfx;
    sf::Sprite heal;
    sf::Texture slowTexture;
    sf::SoundBuffer slowBuffer;
    sf::Sound slow_sfx;
    sf::Sprite slow;

    sf::SoundBuffer readyBuffer;
    sf::Sound ready_sfx;
    sf::SoundBuffer gameOverBuffer;
    sf::Sound game_over_sfx;

    //font and text
    sf::Font font;
    sf::Text text;

    //Private functions
    void initVariables();
    void initWindow();
    void initEnemies();
    void initPlayer();
    void initPowerUps();
    void initFont();
    void initText();

public:
    //Constructors / Destructors
    Game();
    virtual ~Game();

    //Accessors
    const bool running() const;

    //Functions
    void spawnEnemy();
    void spawnPowerUps();

    void pollEvents();
    void updateMousePositions();
    void updateEnemies();
    void updatePlayer();
    void updatePowerUps();
    void updateText();
    void update();

    void renderEnemies(sf::RenderTarget& target);
    void renderPlayer(sf::RenderTarget& target);
    void renderPowerUps(sf::RenderTarget& target);
    void renderText(sf::RenderTarget& target);
    void render();
};

#endif /* Game_hpp */

