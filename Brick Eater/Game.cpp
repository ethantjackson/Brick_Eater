#include "Game.h"

void Game::initVariables() {
    this->window = nullptr;

    //Game logic
    this->points = 0;
    this->health = 100;
    this->speed = 5.0;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 2;
    this->powerUpSpawnTimerMax = 50.f;
    this->powerUpSpawnTimer = this->powerUpSpawnTimerMax;
    this->gameOver = false;
    this->playerReady = false;
    this->healExists = false;
    this->slowExists = false;
    this->slowAmount = 0;

    if (!this->readyBuffer.loadFromFile("res/ready_sfx.wav")) {
        std::cout << "error loading file (ready_sfx) \n";
        system("pause");
    }
    this->ready_sfx.setBuffer(this->readyBuffer);
    this->ready_sfx.setVolume(75.f);
    if (!this->gameOverBuffer.loadFromFile("res/game_over_sfx.wav")) {
        std::cout << "error loading file (game_over_sfx) \n";
        system("pause");
    }
    this->game_over_sfx.setBuffer(this->gameOverBuffer);
    this->game_over_sfx.setVolume(70.f);
}

void Game::initWindow() {
    this->videoMode.height = 1000;
    this->videoMode.width = 1500;

    this->window = new sf::RenderWindow(this->videoMode, "Block Eater", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);

    this->window->setMouseCursorVisible(false);
}

void Game::initEnemies() {
    this->enemy.setPosition(25.f, 25.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    if (!this->enemyKillBuffer.loadFromFile("res/enemy_kill_sfx.wav")) {
        std::cout << "error loading file (enemy_kill_sfx) \n";
        system("pause");
    }
    this->enemy_kill_sfx.setBuffer(this->enemyKillBuffer);
    this->enemy_kill_sfx.setVolume(35.f);
    if (!this->enemyMissedBuffer.loadFromFile("res/enemy_missed_sfx.wav")) {
        std::cout << "error loading file (enemy_missed_sfx) \n";
        system("pause");
    }
    this->enemy_missed_sfx.setBuffer(this->enemyMissedBuffer);
    this->enemy_missed_sfx.setVolume(30.f);
    //    this->enemy.setOutlineColor(sf::Color::White);
    //    this->enemy.setOutlineThickness(1.f);
}

void Game::initPlayer() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    //takes window pixels to coords(int to floats)
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
    this->player.setRadius(25.f);
    this->player.setPosition(mousePosView.x - player.getRadius() / 2, mousePosView.y - player.getRadius() / 2);
    this->player.setFillColor(sf::Color::Black);
    this->player.setOutlineColor(sf::Color::White);
    this->player.setOutlineThickness(15.0f);
}

void Game::initPowerUps() {
    if (!this->healTexture.loadFromFile("res/heal.jpg")) {
        std::cout << "error loading file (heal) \n";
        system("pause");
    }
    this->heal.setTexture(this->healTexture);
    this->heal.setPosition(25.f, 25.f);
    this->heal.setScale(sf::Vector2f(2.0f, 2.0f));

    if (!this->slowTexture.loadFromFile("res/slow.jpg")) {
        std::cout << "error loading file (slow) \n";
        system("pause");
    }
    this->slow.setTexture(this->slowTexture);
    this->slow.setPosition(25.f, 25.f);
    this->slow.setScale(sf::Vector2f(2.0f, 2.0f));

    //load power up sounds
    if (!this->healBuffer.loadFromFile("res/heal_sfx.wav")) {
        std::cout << "error loading file (heal_sfx) \n";
        system("pause");
    }
    this->heal_sfx.setBuffer(this->healBuffer);
    if (!this->slowBuffer.loadFromFile("res/slow_sfz.wav")) {
        std::cout << "error loading file (slow_sfz) \n";
        system("pause");
    }
    this->slow_sfx.setBuffer(this->slowBuffer);
}

void Game::initFont() {
    //font
    if (!this->font.loadFromFile("res/sansation.ttf")) {
        std::cout << "error loading file \n";
        system("pause");
    }
}

void Game::initText() {
    this->text.setFont(this->font);
    this->text.setOrigin(0.f, 0.f);
    this->text.setPosition(sf::Vector2f(25.f, 10.f));
    this->text.setCharacterSize(50);
    this->text.setFillColor(sf::Color::White);
    this->text.setStyle(sf::Text::Bold);
    this->text.setString("NONE");
}

//Constructor
Game::Game() {
    this->initVariables();
    this->initWindow();
    this->initPlayer();
    this->initEnemies();
    this->initPowerUps();
    this->initFont();
    this->initText();
}

//Deconstructor
Game::~Game() {
    delete this->window;
}

//Accessors
const bool Game::running() const {
    return this->window->isOpen();
}

//Functions
void Game::spawnEnemy() {
    /*
        @return voide

        Spawns enemies and sets their colors and positions
     - sets a random pos
     - sets a random color
     - adds enemy to vector
     */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    //Randomize enemy type
    int type = rand() % 4;

    switch (type) {
    case 0:
        this->enemy.setSize(sf::Vector2f(50.f, 50.f));
        break;
    case 1:
        this->enemy.setSize(sf::Vector2f(125.f, 125.f));
        break;
    case 2:
        this->enemy.setSize(sf::Vector2f(200.f, 200.f));
        break;
    case 3:
        this->enemy.setSize(sf::Vector2f(275.f, 275.f));
    }

    this->enemy.setFillColor(sf::Color(rand() % 201 + 55, rand() % 201 + 55, rand() % 201 + 55, 255));

    //Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::spawnPowerUps() {
    int type = rand() % 3;
    if (type == 1) {
        this->heal.setPosition(
            static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 100)),
            0.f
        );
        this->healExists = true;
    }
    else if (type == 2 && speed > 5.5) {
        this->slow.setPosition(
            static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 100)),
            0.f
        );
        this->slowExists = true;
    }
}

void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateMousePositions() {
    /*
     @return void
     updates mouse positions:
        - mouse position relative to window (Vector 2i)
     */
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    //takes window pixels to coords(int to floats)
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies() {
    /*
        @return void

        Updates the enemy spawn timer and spawns enemies when total amount
        of enemies is smaller than the maximum.
        Moves enemies downwards
        Removes enemies at edge of the screen. //TODO
     */
     //Updating timer for enemy spawning
    int numEnemies = this->maxEnemies + points / 500.0 - slowAmount / 5.0;
    if (numEnemies < 2) numEnemies = 2;
    //std::cout << numEnemies << " ";

    if (this->enemies.size() < numEnemies) {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            //spawn enemy and reset timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    //Move and updating the enemies
    this->speed = 5.f + points / 150.0 - slowAmount;
    if (speed < 5.f) speed = 5.f;
    if (speed > 10) speed = speed * 0.8;
    //std::cout << speed << std::endl;
    for (int i = 0; i < this->enemies.size(); i++) {
        bool deleted = false;

        this->enemies[i].move(0.f, speed);

        //Check if hovered over
        if (this->enemies[i].getGlobalBounds().intersects(this->player.getGlobalBounds()) && !gameOver) {
            deleted = true;
            enemy_kill_sfx.play();

            //Gain points
            if (this->enemies[i].getSize().x == 50)
                this->points += 50;
            else if (this->enemies[i].getSize().x == 125)
                this->points += 25;
            else if (this->enemies[i].getSize().x == 200)
                this->points += 10;
            else if (this->enemies[i].getSize().x == 275)
                this->points += 5;
        }

        //If enemy is below bottom of window
        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            deleted = true;
            if (!gameOver)
                enemy_missed_sfx.play();

            if (this->enemies[i].getSize().x == 50)
                this->health -= 5;
            else if (this->enemies[i].getSize().x == 125)
                this->health -= 10;
            else if (this->enemies[i].getSize().x == 200)
                this->health -= 15;
            else if (this->enemies[i].getSize().x == 275)
                this->health -= 20;
            if (this->health <= 0 && !gameOver) {
                this->gameOver = true;
                this->game_over_sfx.play();
            }
        }

        //Final Delete
        if (deleted) {
            this->enemies.erase(this->enemies.begin() + i);
        }
    }
}

void Game::updatePowerUps() {
    if (!this->healExists && !this->slowExists && this->powerUpSpawnTimer >= this->powerUpSpawnTimerMax) {
        this->spawnPowerUps();
        this->powerUpSpawnTimer = 0.f;
    }
    else
        this->powerUpSpawnTimer += 1.f;

    if (this->healExists) {
        this->heal.move(0.f, 5.f);
        if (this->heal.getGlobalBounds().intersects(this->player.getGlobalBounds()) && !gameOver) {
            this->healExists = false;
            this->heal_sfx.play();
            this->health += 25;
        }
    }
    if (this->slowExists) {
        this->slow.move(0.f, 5.f);
        if (this->slow.getGlobalBounds().intersects(this->player.getGlobalBounds()) && !gameOver) {
            this->slowExists = false;
            this->slow_sfx.play();
            ++this->slowAmount;
        }
    }
    if (this->heal.getPosition().y > this->window->getSize().y)
        healExists = false;
    if (this->slow.getPosition().y > this->window->getSize().y)
        slowExists = false;
}

void Game::updatePlayer() {
    this->player.setPosition(mousePosView.x - player.getRadius() / 2, mousePosView.y - player.getRadius() / 2);
}

void Game::updateText() {
    std::stringstream ss;
    if (!gameOver) {
        if (!playerReady) {
            this->text.setCharacterSize(150);
            this->text.setFillColor(sf::Color::Red);
            this->text.setOutlineColor(sf::Color::White);
            this->text.setOutlineThickness(2.0f);
            ss << "READY?";
            this->text.setString(ss.str());
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            text.setPosition(sf::Vector2f(this->window->getSize().x / 2.0f, this->window->getSize().y / 2.0f));
            if (text.getGlobalBounds().intersects(this->player.getGlobalBounds())) {
                this->playerReady = true;
                this->ready_sfx.play();
                this->initText();
            }
        }
        else {
            ss << "Points: " << this->points << "\n"
                << "Health: " << this->health << "\n";
            this->text.setString(ss.str());
        }
    }
    else {
        this->text.setCharacterSize(150);
        this->text.setFillColor(sf::Color::Red);
        this->text.setOutlineColor(sf::Color::White);
        this->text.setOutlineThickness(2.0f);
        ss << "GAME OVER " << "\n"
            << "   SCORE: " << this->points << "\n";
        this->text.setString(ss.str());
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(this->window->getSize().x / 2.0f, this->window->getSize().y / 2.0f));
    }
}

void Game::update() {
    this->pollEvents();

    this->updateMousePositions();

    this->updatePlayer();

    if (this->playerReady) {
        this->updatePowerUps();

        this->updateEnemies();
    }


    this->updateText();
}

void Game::renderEnemies(sf::RenderTarget& target) {
    for (auto& e : this->enemies) {
        target.draw(e);
    }
}

void Game::renderPowerUps(sf::RenderTarget& target) {
    if (healExists)
        target.draw(heal);
    if (slowExists)
        target.draw(slow);
}

void Game::renderPlayer(sf::RenderTarget& target) {
    target.draw(this->player);
}

void Game::renderText(sf::RenderTarget& target) {
    target.draw(this->text);
}

void Game::render() {
    /*
        @return void

        -clear old frame
        - render objects
        - display frame in window

        Renders game objects
     */

    this->window->clear();

    //Draw game objects
    this->renderEnemies(*this->window);
    this->renderPowerUps(*this->window);

    if (!gameOver) {
        this->renderPlayer(*this->window);
    }
    //Draw font
    this->renderText(*this->window);

    this->window->display();
}
