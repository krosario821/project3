/* 
Kailey Rosario 
*/
#include "Player.hpp"

Player::Player(const std::string& name, const size_t& level)
    : name_ { name }
    , level_ { level }
{}

bool Player::operator<(const Player& rhs) const
{
    return level_ < rhs.level_;
}
bool Player::operator==(const Player& rhs) const
{
    return level_ == rhs.level_;
}
bool Player::operator>(const Player& rhs) const
{
    return level_ > rhs.level_;
}
size_t Player:: getPlayerlevel() const{
    return level_;
}
const std::string& Player::getPlayerName() const {
    return name_;
}