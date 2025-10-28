#include "piece.hpp"

Piece::Piece(sf::Texture& texture, sf::Vector2f pos):sf::Sprite(texture)
{
    this->setOrigin({ texture.getSize().x / 2.0f,texture.getSize().y / 2.0f });
    this->setPosition(pos);
    this->setScale({ 0.8f,0.8f });
}
