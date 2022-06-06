#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

void randomizeHeights();
void sort();
bool isSorted();

const int WIDTH = 1000;
const int HEIGHT = 800;
int maxHeight = 700;

class Bar
{

public:
    int x;
    int y;
    int width;
    int height;
    sf::Color color = sf::Color::White;
    Bar()
    {
    }

    Bar(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
    { }

    sf::RectangleShape render()
    {
        sf::RectangleShape newBar(sf::Vector2f(width, height));
        newBar.setFillColor(this->color);
        newBar.setPosition(x, y);

        return newBar;
    }

    void setHeight(int height)
    {
        this->height = height;
        this->y = HEIGHT - this->height;
    }

    int getHeight() const
    {
        return this->height;
    }
};

int numberOfBars = 50;
const int maxBars = 500;

Bar bars[maxBars];
bool sorting = false;
bool sorted = false;

int main()
{

    int currentInd = 0;
    int endPos = 0;

    sf::Image icon;
    if (!icon.loadFromFile("icon.png"))
    {
        std::cout << "Error loading Icon" << std::endl;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Sorting Visualizer");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Couldn't load font" << std::endl;
    }

    //generate starting array of heights
    for (size_t i = 0; i < numberOfBars; i++)
    {
        Bar& b = bars[i]; 
        b.width = (WIDTH / numberOfBars) - 1;
        b.height = rand() % maxHeight + 20;
        b.x = (WIDTH / numberOfBars) * i;
        b.y = HEIGHT - b.height;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            { // initialize sorting algorithm
                if (!sorting && !sorted)
                {
                    sorting = true;
                    currentInd = 0;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                if (!sorting)
                {
                    randomizeHeights();
                    sorted = false;
                }
            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (numberOfBars < maxBars && !sorting)
                {
                    numberOfBars += 50;
                    while (WIDTH % numberOfBars != 0) numberOfBars += 50;
                    
                    randomizeHeights();
                    sorted = false;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (numberOfBars > 50 && !sorting)
                {
                    numberOfBars -= 50;
                    while (WIDTH % numberOfBars != 0)
                    {
                        numberOfBars -= 50;
                    }
                    sorted = false;
                    randomizeHeights();
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add)){
                if(maxHeight < 750 && !sorting){
                maxHeight += 10;
                sorted= false;
                randomizeHeights();
            }
            }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)){
                if(maxHeight > 100 && !sorting){
                    maxHeight -= 10;
                    sorted = false;
                    randomizeHeights();
            }
            }
        }

        window.clear();

        // title
        sf::Text instructions;
        sf::Text barCount;
        sf::Text maxHeightText;

        instructions.setFont(font);
        instructions.setPosition(sf::Vector2f(10, 10));
        instructions.setString("('r' = randomize data, Space = start sorting)");
        instructions.setCharacterSize(17);
        instructions.setFillColor(sf::Color::Magenta);
        window.draw(instructions);

        barCount.setFont(font);
        barCount.setPosition(WIDTH - 200, 10);
        barCount.setString("Bar Count:" + std::to_string(numberOfBars));
        barCount.setFillColor(sf::Color::Magenta);
        barCount.setCharacterSize(30);
        window.draw(barCount);

        maxHeightText.setFont(font);
        maxHeightText.setPosition(WIDTH/2, HEIGHT - maxHeight -50);
        maxHeightText.setString("Size: " + std::to_string(maxHeight));
        maxHeightText.setFillColor(sf::Color::Magenta);
        maxHeightText.setCharacterSize(30);
        window.draw(maxHeightText);
        //draw a dotted line for the max height
        sf::RectangleShape line(sf::Vector2f(WIDTH,1));
        line.setPosition(0, HEIGHT-(maxHeight + 20));
        line.setFillColor(sf::Color::Blue);
        window.draw(line);
        
        for (size_t i = 0; i < numberOfBars; i++)
        {
            window.draw(bars[i].render());
        }

        // bubble sort algorithm
        if (sorting)
        {
            // reset previous bars colors
            bars[currentInd].color = sf::Color::White;
            bars[currentInd - 1].color = sf::Color::White;
            if (currentInd < numberOfBars - endPos - 1)
            {
                bars[currentInd].color = sf::Color::Red;
                bars[currentInd + 1].color = sf::Color::Red;

                if (bars[currentInd].getHeight() > bars[currentInd + 1].getHeight())
                {
                    int temp = bars[currentInd].getHeight();
                    bars[currentInd].setHeight(bars[currentInd + 1].getHeight());
                    bars[currentInd + 1].setHeight(temp);
                }
                currentInd++;
            }
            else
            {
                currentInd = 0;
                endPos++;
            }

            if (isSorted())
            {
                sorting = false;
                sorted = true;
                currentInd = 0;
                endPos = 0;
            }
        }

        if (sorted && bars[numberOfBars-1].color != sf::Color::Green && currentInd < numberOfBars)// 
        {
            bars[currentInd].color = sf::Color::Green;
            currentInd++;
        }

        window.display();
    }

    return 0;
}

bool isSorted()
{
    for (size_t i = 0; i < numberOfBars - 1; i++) if (bars[i].getHeight() > bars[i + 1].getHeight())  return false;     
    return true;
}


//randomize the current set of data
void randomizeHeights()
{
    for (size_t i = 0; i < numberOfBars; i++)
    {
        Bar& b = bars[i];
        b.x = (WIDTH / numberOfBars) * i;
        b.setHeight(rand() % maxHeight + 20);
        b.color = sf::Color::White;
        b.width = (WIDTH / numberOfBars)-1;
    }
}
