#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>


void randomizeHeights();
void sort();
void generateBars();
void redCycle();

bool isSorted();

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

int numberOfBars = 100;
const int maxBars = 1000;

Bar bars[maxBars];
bool sorting = false;
bool reversed = false;

bool sorted =false;

sf::Clock timer;

int main()
{

    int currentInd = 0;
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

    generateBars();

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
                currentInd = 0;
            }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                if(!sorting){
                randomizeHeights();
                sorted= false;
                }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                reversed = true;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                reversed = false;
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
               if(numberOfBars < maxBars && !sorting){
                numberOfBars += 50;
                sorted= false;
                randomizeHeights();
            }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
               if(numberOfBars > 50 && !sorting){
                numberOfBars -= 50;
                randomizeHeights();
               }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();
//add up and down controls for the user to increase the amount of bars by 50

        }

        window.clear();

    //title
    sf::Text instructions;
    sf::Text directionText;
    sf::Text direction;
    sf::Text barCount;

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

    barCount.setFont(font);
    barCount.setPosition(WIDTH-200,10);
    barCount.setString("Bar Count:" + std::to_string(numberOfBars));
    barCount.setFillColor(sf::Color::Magenta);
    barCount.setCharacterSize(30);
    window.draw(barCount);

    for (size_t i = 0; i < numberOfBars; i++)
    {
       window.draw(bars[i].render());
    }
    
        int lowInd = currentInd;    
        if(sorting){
            
                bars[currentInd].color = sf::Color::Red;
            for (size_t i = currentInd + 1; i < numberOfBars; i++)
            {
                //bars[i-1].color = sf::Color::White;
                //bars[i].color = sf::Color::Red;
                if(!reversed){
                if(bars[lowInd].getHeight() > bars[i].getHeight()){
                    lowInd = i;
                }
            }else {
                if(bars[lowInd].getHeight() < bars[i].getHeight()){
                    lowInd = i;
                }
            }
            }
            int tempHeight = bars[currentInd].getHeight();
        bars[currentInd].setHeight(bars[lowInd].getHeight());
        bars[lowInd].setHeight(tempHeight);
        currentInd++;
        // if(isSorted()){
        //     sorting = false;
        //     currentInd = 0;
        //     lowInd = currentInd;
        // }
        if(currentInd == numberOfBars+1){
            currentInd = 0;
            sorting = false;
            sorted = true;
            lowInd = currentInd;
        }
        }
        
        //if(isSorted()){
        if(sorted){
            bars[currentInd].color = sf::Color::Green;
            currentInd++;
        }

        window.display();
    }

    return 0;
}


bool isSorted(){
    for (size_t i = 0; i < numberOfBars-1; i++)
     {
       if(!reversed){
        if(bars[i].getHeight() > bars[i+1].getHeight()){
            return false;
        }
     }else{
        if(bars[i].getHeight() < bars[i+1].getHeight()){
            return false;
        } 
     }
    }
    return true;
}

void generateBars(){
    for (size_t i = 0; i < numberOfBars; i++)
    {
        //create an offset of WIDTH - (width * number of bars) then add this to the x
        int width = (WIDTH/numberOfBars)-1;
        int height = rand() % HEIGHT + 20;

        int offset = WIDTH - (width * numberOfBars);
        int x = (WIDTH/numberOfBars)*i;
        int y = HEIGHT - height;
        Bar n(x,y,width,height);
        bars[i] = n;
    }
}

void randomizeHeights(){
    for (size_t i = 0; i < numberOfBars; i++)
    {
        //calculate offset of a block only when the WIDTH % numberofbar != 0
        //width of bar has to be full integer or it will have black bars on the side of the screen


       bars[i].x = (WIDTH/numberOfBars)*i+ 1;
       bars[i].setHeight(rand() % HEIGHT + 20);
       bars[i].color = sf::Color::White;
    bars[i].width = (WIDTH/numberOfBars)-1;

    if(WIDTH % numberOfBars != 0){//non integer bar widths       
    bars[i].width = (WIDTH/numberOfBars);

    int offset = ((WIDTH - (bars[i].width*numberOfBars))/numberOfBars) + 1;
    bars[i].x = (WIDTH/numberOfBars)*i + offset;
    }   
 }
    
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

    redCycle();
}

void redCycle(){
    for (size_t i = 0; i < numberOfBars; i++)
    {
        bars[i].color = sf::Color::Red;
    }
    
}