// Bullet implementation
#include "SpaceInvaders.h"

Bullet::Bullet(const sf::Texture& texture, float x, float y, float speed, ShooterType shooterType)
	: sprite(), speed(speed), shooterType(shooterType) {
	sprite.setTexture(texture);
	sprite.setPosition(x, y);

	if (shooterType == ShooterType::Invader) {
		sprite.setRotation(180);
	}
}

ShooterType Bullet::getShooterType() const {
	return shooterType;
}

void Bullet::update(float deltaTime)
{
	sprite.move(0, speed * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}

const sf::Sprite& Bullet::getSprite() const
{
	return sprite;
}