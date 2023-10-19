#include "SpaceInvaders.h"

Ship::Ship(const sf::Texture& texture, float x, float y) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}


void Ship::update(float deltaTime) {

}

void Ship::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

PlayerShip::PlayerShip(const sf::Texture& texture, float x, float y)
    : Ship(texture, x, y), speed(200.0f), movingLeft(false), movingRight(false) {}

void PlayerShip::update(float deltaTime) {
    float dx = 0.0f;
    if (movingLeft) {
        dx -= speed * deltaTime;
    }
    if (movingRight) {
        dx += speed * deltaTime;
    }
    sprite.move(dx, 0.0f);
    if (sprite.getPosition().x < 0) {
        sprite.setPosition(0, sprite.getPosition().y);
    }
    if (sprite.getPosition().x + sprite.getGlobalBounds().width > 800) {
        sprite.setPosition(800 - sprite.getGlobalBounds().width, sprite.getPosition().y);
    }
}

void PlayerShip::handleInput(const sf::Keyboard::Key& key, bool isPressed) {
    if (key == sf::Keyboard::Left) {
        movingLeft = isPressed;
    }
    else if (key == sf::Keyboard::Right) {
        movingRight = isPressed;
    }
}

Invader::Invader(const sf::Texture& texture, float x, float y)
    : Ship(texture, x, y), speed(50.0f) {}

bool reversed;

void Invader::update(float deltaTime) {
    sprite.move(speed * direction * deltaTime, 0);
}
void Invader::setSpeed(float newSpeed) {
    speed = newSpeed;
}
void Invader::setDirection(int newDirection) {
    direction = newDirection;
}

void Invader::shoot(std::vector<Bullet>& bullets, const sf::Texture& bulletTexture) const {
    float x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2;
    float y = sprite.getPosition().y + sprite.getGlobalBounds().height;
    bullets.emplace_back(bulletTexture, x, y, 300.0f, ShooterType::Invader);
}

void PlayerShip::shoot(std::vector<Bullet>& bullets, const sf::Texture& bulletTexture) {
    float x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2;
    float y = sprite.getPosition().y;
    bullets.emplace_back(bulletTexture, x, y, -600.0f, ShooterType::Player);
}
