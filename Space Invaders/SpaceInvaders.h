#pragma once
#include <SFML/Graphics.hpp>
enum class ShooterType {
	Player,
	Invader
};

class Bullet {
public:
	Bullet(const sf::Texture& texture, float x, float y, float speed, ShooterType shooterType);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window) const;
	const sf::Sprite& getSprite() const;
	ShooterType getShooterType() const;

private:
	sf::Sprite sprite;
	float speed;
	ShooterType shooterType;
};

class Ship {
public:
	Ship(const sf::Texture& texture, float x, float y);
	virtual void update(float deltaTime);
	void draw(sf::RenderWindow& window) const;
	const sf::Sprite& getSprite() const {
		return sprite;
	}

protected:
	mutable sf::Sprite sprite;

};

class PlayerShip : public Ship {
public:
	PlayerShip(const sf::Texture& texture, float x, float y);
	void update(float deltaTime) override;
	void handleInput(const sf::Keyboard::Key& key, bool isPressed);
	void shoot(std::vector<Bullet>& bullets, const sf::Texture& bulletTexture);
	void hit() { ++hitCount; }
	int getHitCount() const { return hitCount; }

private:
	float speed;
	bool movingLeft;
	bool movingRight;
	int hitCount = 0;
};

class Invader : public Ship {
public:
	Invader(const sf::Texture& texture, float x, float y);
	void update(float deltaTime) override;
	void shoot(std::vector<Bullet>& bullets, const sf::Texture& bulletTexture) const;
	void setSpeed(float newSpeed);
	void setDirection(int newDirection);
	inline void move(float dx, float dy) {
		sprite.move(dx, dy);
	}
	void reverseDirection() {
		direction = -direction;
	}
	
	inline float getWidth() const {
		return sprite.getGlobalBounds().width;
	}
	inline sf::Vector2f getPosition() const {
		return sprite.getPosition();
	}

	inline bool isReversed() const {
		return reversed;
	}
private:
	float speed;
	int direction;
	bool reversed;
};