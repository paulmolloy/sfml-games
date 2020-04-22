#include "Bat.hpp"
#include "Ball.hpp"


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

/*
 * Mainly based on following:
 * http://gamecodeschool.com/sfml/coding-a-simple-pong-game-with-sfml/
 */

int main()
{
	int windowWidth = 1024;
	int windowHeight = 768;
	std::string windowTitle = "Pong";
	// Window title pong.
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle);

	int score = 0;
	int lives = 3;

	Bat bat (windowWidth / 2, windowHeight -20);
	Ball ball(windowWidth /2, windowHeight/2);

	sf::Text hud;

	sf::Font font;
	font.loadFromFile("DS-DIGIT.TTF");

	hud.setFont(font);

	hud.setCharacterSize(75);



	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
		{
			bat.moveRight();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
		{
			bat.moveLeft();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
		{
			window.close();
		}

		/* Update the state.
		 *
		 */

		if(ball.getPosition().top > windowHeight)
		{
			// Reverse ball dir.
			ball.hitBottom();
			lives--;

			if(lives <1 ) {
				score = 0;
				lives = 3;
			}
		}

		// Ball hit top.
		if(ball.getPosition().top < 0)
		{
			ball.reboundBatOrTop();
			score++;
		}

		// Ball hit sides.
		if(ball.getPosition().left < 0 || ball.getPosition().left + 10 > windowWidth)
		{
			ball.reboundSides();
		}

		// Ball bat.
		if(ball.getPosition().intersects(bat.getPosition()))
		{
			ball.reboundBatOrTop();
		}

		ball.update();
		bat.update();

		std::stringstream ss;
		ss << "Score:" << score << " 	Lives:" << lives;
		hud.setString(ss.str());

		/* 
		 * Drawing the frame.
		 */

		window.clear(sf::Color(26, 128, 59, 255));

		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.draw(hud);

		window.display();
	}

	return 0;
}
