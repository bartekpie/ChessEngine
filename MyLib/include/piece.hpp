#pragma once  
#include<SFML/Graphics.hpp>  
#include "chesstate.hpp"  

class Piece : public sf::Sprite  
{  
private:   
   
     
public:  
   Piece( sf::Texture& texture, sf::Vector2f pos);  

    
   Piece() = default;  
};
