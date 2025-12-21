#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace  sf;

int main(){
	
	Texture bgTexture;
	bgTexture.loadFromFile("assets/chess.png");
	Sprite bgSprite(bgTexture);
	bgSprite.setPosition(0,0);
	
	
	
	
	
	
	
	
	
	
	RenderWindow window(VideoMode(612 , 612 ), "!!CHESS!!" , Style::Resize);
	window.setFramerateLimit(60);
	
	
	Event ev;
	//game loop
	while(window.isOpen()){
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed) 
			{
				window.close();
			}
		}
		
		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

	
		window.draw(bgSprite);
		window.display();
	}
	
return 0;
}
