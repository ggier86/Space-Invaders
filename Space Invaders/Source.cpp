#include "SpaceInvaders.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

//Class definitions will go here

std::string wrapText(const sf::Text text, unsigned int maxWidth) {
	std::string originalString = text.getString();
	std::string wrappedText;
	std::string word;
	const sf::Font* font = text.getFont();
	unsigned int characterSize = text.getCharacterSize();
	float currentLineWidth = 0.0f;

	for (char c : originalString) {
			sf::Text tempText(word, *font, characterSize);
			float lineWidth = tempText.getLocalBounds().width; 

			if (c == ' ' || c == '\t' || c == '\n') {
				if (currentLineWidth + lineWidth > maxWidth) {
					wrappedText += "\n";
					currentLineWidth = 0.0f;
				}
				wrappedText += word + c;
				word.clear();
		} else {
			word += c;
			currentLineWidth += lineWidth;
		}
	}

	wrappedText += word;
	return wrappedText;
}

		

unsigned int maxWidth = 800;

void createInvaders(std::vector <Invader>& invaders, const sf::Texture& invaderTexture) {
	invaders.clear();
	const int rows = 4;
	const int columns = 12;
	float invaderSpacingX = 60.0f;
	float invaderSpacingY = 40.0f;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			Invader invader(invaderTexture, 50.0f + j * invaderSpacingX, 50.0f + i * invaderSpacingY);
			invader.setSpeed(25.0f);
			invader.setDirection(1);
			invaders.push_back(invader);
		}
	}
}

void updateInvaders(std::vector<Invader>& invaders, float deltaTime) {
	float leftmost = 800;
	float rightmost = 0;
	float downwardMovement = 5.0f;
	bool shouldReverse = false;

	for (auto& invader : invaders) {
		invader.update(deltaTime);
	}

	for (const auto& invader : invaders) {
		sf::Vector2f pos = invader.getPosition();
		float width = invader.getWidth();
		leftmost = std::min(leftmost, pos.x);
		rightmost = std::max(rightmost, pos.x + width);
	}
	if (leftmost <= 0 || rightmost >= 800) {
		shouldReverse = true;
	}

	for (auto& invader : invaders) {
		if (shouldReverse) {
			invader.reverseDirection();
			invader.move(0, downwardMovement);
		}
	}

	if (shouldReverse) {
		shouldReverse = false;
	}
}

int remainingInvaders = 48;

int main()
{
	enum class GameState {
		MainMenu,
		Playing,
		GameOver,
		GameWin
	};
	sf::RenderWindow window(sf::VideoMode(800, 600), "Space Invaders");

	std::vector<Invader> invaders;

	sf::Texture bulletTexture;
	if (!bulletTexture.loadFromFile("Resources/bullet.png")) {
		std::cout << "Failed to load bullet texture!" << std::endl;
	}

	sf::Texture playerShipTexture;
	if (!playerShipTexture.loadFromFile("Resources/playerShip.png")) {
	}

	sf::Texture invaderTexture;
	if (!invaderTexture.loadFromFile("Resources/invaderShip.png")) {

	}
	sf::Vector2u textureSize = invaderTexture.getSize();
	std::cout << "Invader Width: " << textureSize.x << ", Height: " << textureSize.y << std::endl;
	PlayerShip playerShip(playerShipTexture, 400.0f, 550.0f);
	std::vector<Bullet> bullets;

	
	

	sf::Font font;
	if (!font.loadFromFile("Resources/stjedise/STJEDISE.ttf")) {
		std::cout << "Failed to load font!" << std::endl;
	}
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);
	int score = 0;

	sf::Clock clock;
	float shootCooldown = 0.0f;
	float invaderShootCooldown = 0.0f;
	float invaderSpeed = 100.0f;

	GameState gameState = GameState::MainMenu;

	createInvaders(invaders, invaderTexture);

	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (gameState == GameState::Playing)
			{
				if (event.type == sf::Event::KeyPressed)
				{
					playerShip.handleInput(event.key.code, true);
					if (event.key.code == sf::Keyboard::Space && shootCooldown <= 0.0f)
					{
						playerShip.shoot(bullets, bulletTexture);
						shootCooldown = 0.2f;
					}
				}
				else if (event.type == sf::Event::KeyReleased)
				{
					playerShip.handleInput(event.key.code, false);
				}
			}
		}

		window.clear();

		if (gameState == GameState::MainMenu) {
			sf::Text menuText;
			menuText.setFont(font);
			menuText.setString("Press Enter to Start");
			menuText.setCharacterSize(24);
			menuText.setFillColor(sf::Color::White);
			menuText.setPosition(300, 300);
			window.draw(menuText);
			window.display();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameState = GameState::Playing;
			}
		
		}
		else if (gameState == GameState::Playing) {

			updateInvaders(invaders, deltaTime);

			auto invaderIt = invaders.begin();
			while (invaderIt != invaders.end()) {
				auto bulletIt = bullets.begin();
				bool collisionFound = false;

				while (bulletIt != bullets.end()) {
					if (bulletIt->getShooterType() == ShooterType::Player &&
						bulletIt->getSprite().getGlobalBounds().intersects(invaderIt->getSprite().getGlobalBounds())) {
						bulletIt = bullets.erase(bulletIt);
						collisionFound = true;
						break;
					}
					else {
						++bulletIt;
					}
				}

				if (collisionFound) {
					invaderIt = invaders.erase(invaderIt);
					score += 10;
					remainingInvaders--;
				}
				else {
					++invaderIt;
				}
			}

			auto bulletIt = bullets.begin();
			while (bulletIt != bullets.end()) {
				if (bulletIt->getShooterType() == ShooterType::Invader) {
					if (bulletIt->getSprite().getGlobalBounds().intersects(playerShip.getSprite().getGlobalBounds())) {
						playerShip.hit();

						bulletIt = bullets.erase(bulletIt);
						continue;
					}
				}
				++bulletIt;
			}



			sf::Vector2f movement(0.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				movement.x -= invaderSpeed * deltaTime;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				movement.x += invaderSpeed * deltaTime;
			}

			shootCooldown -= deltaTime;
			invaderShootCooldown -= deltaTime;

			if (!invaders.empty() && invaderShootCooldown <= 0.0f) {
				int shooterIndex = rand() % invaders.size();
				invaders[shooterIndex].shoot(bullets, bulletTexture);
				invaderShootCooldown = 2.0f;
			}


			playerShip.update(deltaTime);
			for (Invader& invader : invaders) {
			}
			for (Bullet& bullet : bullets) {
				bullet.update(deltaTime);
			}

			scoreText.setString("Score: " + std::to_string(score));
			window.draw(scoreText);

			playerShip.draw(window);
			for (const Invader& invader : invaders) {
				invader.draw(window);
			}

			for (const Bullet& bullet : bullets) {
				bullet.draw(window);

			}
			window.display();

			if (playerShip.getHitCount() >= 3) {
				gameState = GameState::GameOver;
			}
			
			if (remainingInvaders == 0) {
				gameState = GameState::GameWin;
			}
			
		}
		else if (gameState == GameState::GameOver) {
			sf::Text gameOverText;
			gameOverText.setFont(font);
			gameOverText.setString("Game over....Press Enter to return to Main Menu");
			gameOverText.setCharacterSize(24);
			gameOverText.setFillColor(sf::Color::White);
			gameOverText.setPosition(50, 300);
			window.draw(gameOverText);
			window.display();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameState = GameState::MainMenu;
				invaders.clear();
				bullets.clear();
				score = 0;
				sf::sleep(sf::milliseconds(500));
				remainingInvaders = 48;
				playerShip = PlayerShip(playerShipTexture, 400.0f, 550.0f);
				createInvaders(invaders, invaderTexture);
			}
		}
		else if (gameState == GameState::GameWin) {
			sf::Text text;
			text.setFont(font);
			text.setString("Congratulations... You've Beat The Game. :)");
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::White);
			text.setPosition(100, 200);
			window.draw(text);
			sf::Text creditsText;
			creditsText.setFont(font);
			creditsText.setString("Animations by wuhu and vergil1018 from opengameart.org and font from fonts addict.");
			creditsText.setCharacterSize(12);
			creditsText.setFillColor(sf::Color::White);
			creditsText.setPosition(50, 350);
			window.draw(creditsText);
			window.display();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				invaders.clear();
				bullets.clear();
				remainingInvaders = 48;
				score = 0;
				sf::sleep(sf::milliseconds(900));
				playerShip = PlayerShip(playerShipTexture, 400.0f, 550.0f);
				createInvaders(invaders, invaderTexture);
				gameState = GameState::MainMenu;

			}
		}
		
	}
	return 0;

}