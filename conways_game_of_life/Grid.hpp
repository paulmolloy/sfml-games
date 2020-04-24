#pragma once
#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{

    enum BoxState{DEAD, ALIVE};
private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; 
	sf::Vector2f position;
	sf::RectangleShape grid_dims;
	sf::RectangleShape cursor;
    int selected_x = 0;
    int selected_y = 0;
    enum BoxState * grid;
    int rows;
    int cols;
    int alive_count;
    sf::RectangleShape * draw_grid;
    bool in_bounds(int x, int y, int cols, int rows); 
    int count_neighbours(int x, int y);
    

public:
	Grid(int windowWidth, int windowHeight, int rows, int cols);
	sf::FloatRect getPosition();
	sf::RectangleShape getShape();
    int getAliveCount();

	void update();
	void next_state();
    // Select a box before running sim.
    void left();
    void right();
    void up();
    void down();
    // Turn on/off box in the grid.
    void fill_box();
    void clear_box();
    void toggle_box();
};
