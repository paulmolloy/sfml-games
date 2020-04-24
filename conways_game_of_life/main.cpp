#include "Grid.hpp"


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
	int windowWidth = 1800;
	int windowHeight = 1900;
	std::string windowTitle = "Game of Life";
	// Window title pong.
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle);
	
	int num_rows = 15;
	int num_cols = 15;
    Grid grid (windowWidth, windowHeight, num_rows, num_cols);
	int alive = 0; // number of alive cells.
	sf::Text hud;

	sf::Font font;
	font.loadFromFile("DS-DIGIT.TTF");

	hud.setFont(font);

	hud.setCharacterSize(75);



	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


    sf::Clock clock;
    sf::Clock sim_clock;
    window.setFramerateLimit(15);

    bool isSimulating = false;
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
			grid.right();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
		{
			grid.left();
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
		{
			grid.up();
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
		{
			grid.down();
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) 
		{
            isSimulating = !isSimulating;
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
		{
			grid.toggle_box();
		}
	
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
		{
			window.close();
		}

		/* Update the state.
		 *
		 */
	    grid.update();	

        sf::Time time = clock.getElapsedTime();
        clock.restart().asSeconds();

		std::stringstream ss;
		ss << "Alive Cells: " << grid.getAliveCount();
        ss << " FPS: " << (1.0f / time.asSeconds()) << alive << "\n";
        ss << "Press enter to ";
        if(isSimulating) ss << "pause simulation.";
        else ss << "start simulation.";
		hud.setString(ss.str());



        // Simulation should only update state if simulating and at slow rate so can see.
        // Updates once every second.
        //
        if(isSimulating) {
            sf::Time simTime = sim_clock.getElapsedTime();
            if(simTime.asSeconds() > 2){
                grid.next_state();
                sim_clock.restart().asSeconds();
                std::cout << "updating ..." << std::endl;
            }

        }
        

		/* 
		 * Drawing the frame.
		 */


		window.clear(sf::Color(26, 128, 50, 255));

		window.draw(hud);

        window.draw(grid);

		window.display();
	}

	return 0;
}

