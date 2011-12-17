#include <SFML/Graphics.hpp>
#include "Helpers.h"
#include "Constants.h"
#include <fstream>
#include <sstream>

void SetViewPos(sf::RenderWindow& window, const sf::Vector2f& center)
{
    //I could cache this by making it static but the viewsize might change?
    sf::Vector2f viewSize(float(window.GetWidth()) / window.GetHeight() * Constants::GAMEFIELD_HEIGHT, Constants::GAMEFIELD_HEIGHT);
    sf::View view(center, viewSize);
    window.SetView(view);
}

unsigned int GetLevelCount()
{
    unsigned int count = 0;
    while(true)
    {
        std::ifstream file(GetLevelName(count).c_str());
        if(file.fail())
        {
            return count;
        }
        ++count;
    }
}

const std::string GetLevelName(unsigned int index)
{
    std::stringstream levelname;
    levelname << Constants::LEVEL_BASE_NAME << index << Constants::LEVEL_EXTENSION;
    return levelname.str();
}
