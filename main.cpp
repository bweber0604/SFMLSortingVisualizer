#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>


void randomizeHeights();
void sort();

const int WIDTH = 1000;
const int HEIGHT = 800;


class Bar{

    public:
        int x;
        int y;
        int width;
        int height;
        sf::Color color = sf::Color::White;
        Bar(){

        }

        Bar(int x, int y, int width, int height){
            this->x = x;
            this->y = y;
            this->height = height;
            this->width = width;
        }

        sf::RectangleShape render(){
            sf::RectangleShape newBar(sf::Vector2f(width,height));
            newBar.setFillColor(this->color);
            newBar.setPosition(x,y);
            
            return newBar;
        }

        void setHeight(int height){
            this->height = height;
            this->y = HEIGHT - this->height;
        }

        int getHeight(){
            return this->height;
        }

        

};

const int numberOfBars = 100;

Bar bars[numberOfBars];
bool sorting = false;
bool reversed = false;

sf::Clock timer;

int main()
{
    sf::Image icon;
    if(!icon.loadFromFile("icon.png")){
        std::cout << "Error loading Icon" << std::endl;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "SFML Sorting Visualizer");
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    sf::Font font;
    if(!font.loadFromFile("arial.ttf")){
        std::cout << "Couldn't load font" << std::endl;
    }

    //genererate bars with random heights and constant width across the screen
    for (size_t i = 0; i < numberOfBars; i++)
    {
        int width = (WIDTH/numberOfBars)-1;
        int height = rand() % (HEIGHT-70) + 20;
        int x = (WIDTH/numberOfBars)*i;
        int y = HEIGHT - height;
        Bar n(x,y,width,height);
        bars[i] = n;
    }
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){//initialize sorting algorithm
            if(!sorting){
                sorting = true;
                sort();
            }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                if(!sorting){
                randomizeHeights();
                }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                reversed = true;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                reversed = false;
            }
        }

        window.clear();

    //title
    sf::Text instructions;
    sf::Text directionText;
    sf::Text direction;

    instructions.setFont(font);
    instructions.setPosition(sf::Vector2f(10,10));
    instructions.setString("('r' = randomize data, Space = start sorting)");
    instructions.setCharacterSize(17);
    instructions.setFillColor(sf::Color::Magenta);
    window.draw(instructions);

    directionText.setFont(font);
    directionText.setPosition(sf::Vector2f(10,30));
    directionText.setString("left arrow = sort from high to low, right arrow = sort from low to high");
    directionText.setFillColor(sf::Color::Magenta);
    directionText.setCharacterSize(17);
    window.draw(directionText);

    direction.setFont(font);
    direction.setPosition(200,50);
    if(reversed){
        direction.setString("<--");
    }else
    direction.setString("-->");
    direction.setFillColor(sf::Color::Green);
    direction.setCharacterSize(25);
    window.draw(direction);

    for (size_t i = 0; i < numberOfBars; i++)
    {
       window.draw(bars[i].render());
    }
        window.display();
    }

    return 0;
}

void sort(){
    //selection sort algoirthm
    for (size_t i = 0; i < numberOfBars; i++)
    {
        int lowestBarInd = i;
        bars[i].color = sf::Color::Red;
        for (size_t j = i+1; j < numberOfBars; j++)
        {
            bars[j].color = sf::Color::Red;

        if(reversed){
            if(bars[j].getHeight() > bars[lowestBarInd].getHeight()){
                lowestBarInd = j;
            }
        }else{//sort low to high
            if(bars[j].getHeight() < bars[lowestBarInd].getHeight()){
                lowestBarInd = j;
            }
        }

            
            bars[j].color = sf::Color::White;
        }

        int tempHeight = bars[i].getHeight();
        bars[i].setHeight(bars[lowestBarInd].getHeight());
        bars[lowestBarInd].setHeight(tempHeight);
        
        bars[i].color = sf::Color::White;
        
    }
    sorting = false;
    for (size_t i = 0; i < numberOfBars; i++) bars[i].color = sf::Color::Green;

    
}

void randomizeHeights(){
    for (size_t i = 0; i < numberOfBars; i++)
    {
       bars[i].setHeight(rand() % (HEIGHT-70) + 20);
       bars[i].color = sf::Color::White;
    }
    
}