#include "user_input.hpp"

UserInput::UserInput()
{

}

void UserInput::switchDriveMode(DriveMode newMode)
{
    m_drive_mode = newMode;
}

void UserInput::proccesInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        std::cout << "Accelerate n go forward" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        std::cout << "Go backward" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        std::cout << "Turn right" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        std::cout << "Turn left" << std::endl;
    }
}