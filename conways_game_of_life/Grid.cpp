#include "Grid.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>


sf::Color CURSOR_COLOR = sf::Color::Green;
sf::Color OUTLINE_COLOR = sf::Color::Black;
sf::Color DEAD_COLOR = sf::Color::Red;
sf::Color ALIVE_COLOR = sf::Color::Blue;
float box_size;

/*
 * Pretty much just following:
 * http://gamecodeschool.com/sfml/coding-a-simple-pong-game-with-sfml/
 */
Grid::Grid(int windowWidth, int windowHeight, int rows, int cols)
{
    
    float start_x = windowWidth/10;
    float start_y = windowHeight/10;
	this->position.x = start_x;
	this->position.y = start_y;
    float gridWidth = (windowWidth*3)/5;
    float gridHeight = (windowHeight*3)/5;
    alive_count = 0;

    selected_x = 0;
    selected_y = 0;
    this->rows = rows;
    this->cols = cols;
        
	grid_dims.setSize(sf::Vector2f(gridWidth, gridHeight));
	grid_dims.setPosition(position);
    grid = new BoxState[rows*cols];
    draw_grid = new sf::RectangleShape [rows*cols];

    // Figure out the size boxes would need to be to fit in rows and cols.
    // Then pick the smaller one and use for both so the boxes can be square.
    float rDensity = gridWidth/cols;
    float cDensity = gridHeight/rows;

    box_size = std::min(rDensity, cDensity);
    // init grid boxes.
    for(int i = 0; i< rows; i ++)
    {
        for(int j = 0; j < cols; j++)
        { 
            sf::Vector2f cur_pos(this->position.x + j*box_size, this->position.y + i*box_size);
            draw_grid[j + i*cols].setSize(sf::Vector2f(box_size, box_size));
	        draw_grid[j + i*cols].setPosition(cur_pos);

	        draw_grid[j + i*cols].setOutlineColor(OUTLINE_COLOR);
	        draw_grid[j + i*cols].setFillColor(DEAD_COLOR);
	        draw_grid[j + i*cols].setOutlineThickness(2);
        }
    }
 
    // highlight initial selected grid
    draw_grid[selected_x + selected_y *rows].setOutlineColor(CURSOR_COLOR);

}

sf::FloatRect Grid::getPosition()
{
	return grid_dims.getGlobalBounds();
}

sf::RectangleShape Grid::getShape()
{
	return grid_dims;
}

void Grid::left(){
    if(selected_x > 0)
    {
        draw_grid[selected_x + selected_y *rows].setOutlineColor(OUTLINE_COLOR);
        selected_x--;
        draw_grid[selected_x + selected_y *rows].setOutlineColor(CURSOR_COLOR);
    }
}
void Grid::right(){
    if(selected_x < cols-1)
    {
        draw_grid[selected_x + selected_y *rows].setOutlineColor(OUTLINE_COLOR);
        this->selected_x++;
        draw_grid[selected_x + selected_y *rows].setOutlineColor(CURSOR_COLOR);
    }
}

void Grid::up(){
    if(selected_y > 0)
    {
        draw_grid[selected_x + selected_y *rows].setOutlineColor(OUTLINE_COLOR);
        selected_y--;
        draw_grid[selected_x + selected_y *rows].setOutlineColor(CURSOR_COLOR);
    }
}
void Grid::down(){
    if(selected_y < rows-1)
    {
        draw_grid[selected_x + selected_y *rows].setOutlineColor(OUTLINE_COLOR);
        selected_y++;
        draw_grid[selected_x + selected_y *rows].setOutlineColor(CURSOR_COLOR);

    }
}
void Grid::fill_box(){}
void Grid::clear_box(){}

void Grid::toggle_box(){
    if(grid[selected_x + selected_y * rows] == BoxState::DEAD) {
        grid[selected_x + selected_y * rows] = BoxState::ALIVE;
        draw_grid[selected_x + selected_y * rows].setFillColor(ALIVE_COLOR);
        alive_count++;
        
    }else {
        grid[selected_x + selected_y * rows] = BoxState::DEAD;
        draw_grid[selected_x + selected_y * rows].setFillColor(DEAD_COLOR);
        alive_count--;
    }
}

bool Grid::in_bounds(int row, int col, int rows, int cols) {
    if(col < cols && col >= 0 && row < rows && row >=0) return true;
    return false;
}

int Grid::count_neighbours(int row, int col){
    if(col > cols || row > rows) return 0;
    int neighbours = 0;
    
    
    // Right
    int cur_col = col + 1;
    int cur_row = row;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }

    // Bottom Right
    cur_col = col + 1;
    cur_row = row + 1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }

    // Bottom 
    cur_col = col;
    cur_row = row + 1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }


    // Bottom Left
    cur_col = col - 1;
    cur_row = row + 1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }

    // Left
    cur_col = col -1;
    cur_row = row;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }


    // Top Left
    cur_col = col - 1;
    cur_row = row - 1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }

    // Top
    cur_col = col;
    cur_row = row -1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }

    // Top Right
    cur_col = col + 1;
    cur_row = row -1;
    if(in_bounds(cur_row, cur_col, rows, cols))
    {
        if(grid[cur_col + cur_row *cols] == BoxState::ALIVE) neighbours++;
    }


    return neighbours;
}

// Compute and Execute the next state of the game of life.
void Grid::next_state(){
    // Rules:
    // Living cells with 2 or 3  live neighbours survive.
    
    // Dead cell with 3 or more live neighbours  becomes alive.

    // Everything else dead.
    //
   
    BoxState * temp_grid = new BoxState[rows*cols];
    sf::RectangleShape * temp_draw_grid = new sf::RectangleShape [rows*cols];

    alive_count = 0;
    for(int i = 0; i< rows; i ++)
    {
        for(int j = 0; j < cols; j++)
        {
            int live_neighbours = count_neighbours(i, j);
            bool isAlive =  grid[j + i * cols] == BoxState::ALIVE;
            // Rules:
            // Living cells with 2 or 3  live neighbours survive.
            
            // Dead cell with 3 or more live neighbours  becomes alive.

            // Everything else dead.
            //
            if(isAlive && (live_neighbours == 2 || live_neighbours ==3)){
                // Still alive.
                temp_grid[j + i * cols] = BoxState::ALIVE;
            
            }else if(!isAlive && live_neighbours == 3){
                // Born.
                temp_grid[j + i * cols] = BoxState::ALIVE;

            }else{
                // Dead.
                temp_grid[j + i * cols] = BoxState::DEAD;


            }

            // Update the box to match the boxes state.
            sf::Vector2f cur_pos(this->position.x + j*box_size, this->position.y + i*box_size);
            temp_draw_grid[j + i*cols].setSize(sf::Vector2f(box_size, box_size));
	        temp_draw_grid[j + i*cols].setPosition(cur_pos);

	        temp_draw_grid[j + i*cols].setOutlineColor(OUTLINE_COLOR);
	        temp_draw_grid[j + i*cols].setOutlineThickness(2);

            if(temp_grid[j + i * cols] == BoxState::ALIVE)
            {
    	        temp_draw_grid[j + i*cols].setFillColor(ALIVE_COLOR);
                alive_count++;
            }else {
	            temp_draw_grid[j + i*cols].setFillColor(DEAD_COLOR);
            }
        }

    }

     grid = temp_grid;
     draw_grid = temp_draw_grid;

}

// I really just used this function to print internal state when I was debugging.
void Grid::update()
{
	grid_dims.setPosition(position);

}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
    target.draw(grid_dims);

    for(int i = 0; i< rows; i ++)
    {
        for(int j = 0; j < cols; j++)
        {
            target.draw(draw_grid[j + i*cols]);
        }
    }
    // Draw the selected again so its on top.    
    target.draw(draw_grid[selected_x + selected_y*cols]);
}

int Grid::getAliveCount() 
{
    return alive_count;
}




