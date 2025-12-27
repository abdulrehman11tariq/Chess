#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace  sf;

int height = 8; 
int width =  8;
int window_height = 1050;
int window_width  = 1050;
int boarder_width = 33*1.5;
int boarder_height = 33*1.5;

void display_board(RenderWindow& window, char** board){
	
	//SPRITES FOR WHITE GOTTI
	Texture WpawnTex;
	WpawnTex.loadFromFile("assets/White/pawn.png");
	Sprite WpawnSprite(WpawnTex);
	WpawnSprite.setScale(2,2);
	
	Texture WrookTex;
	WrookTex.loadFromFile("assets/White/rook.png");
	Sprite WrookSprite(WrookTex);
	WrookSprite.setScale(2,2);
	
	Texture WknightTex;
	WknightTex.loadFromFile("assets/White/knight.png");
	Sprite WknightSprite(WknightTex);
	WknightSprite.setScale(2,2);
	
	Texture WbishopTex;
	WbishopTex.loadFromFile("assets/White/bishop.png");
	Sprite WbishopSprite(WbishopTex);
	WbishopSprite.setScale(2,2);
	
	Texture WqueenTex;
	WqueenTex.loadFromFile("assets/White/queen.png");
	Sprite WqueenSprite(WqueenTex);
	WqueenSprite.setScale(2,2);
	
	Texture WkingTex;
	WkingTex.loadFromFile("assets/White/king.png");
	Sprite WkingSprite(WkingTex);
	WkingSprite.setScale(2,2);
	
	//SPRITES FOR BLACK GOTTI
	Texture BpawnTex;
	BpawnTex.loadFromFile("assets/Black/pawn.png");
	Sprite BpawnSprite(BpawnTex);
	BpawnSprite.setScale(2,2);
	
	Texture BrookTex;
	BrookTex.loadFromFile("assets/Black/rook.png");
	Sprite BrookSprite(BrookTex);
	BrookSprite.setScale(2,2);
	
	Texture BknightTex;
	BknightTex.loadFromFile("assets/Black/knight.png");
	Sprite BknightSprite(BknightTex);
	BknightSprite.setScale(2,2);
	
	Texture BbishopTex;
	BbishopTex.loadFromFile("assets/Black/bishop.png");
	Sprite BbishopSprite(BbishopTex);
	BbishopSprite.setScale(2,2);
	
	Texture BqueenTex;
	BqueenTex.loadFromFile("assets/Black/queen.png");
	Sprite BqueenSprite(BqueenTex);
	BqueenSprite.setScale(2,2);
	
	Texture BkingTex;
	BkingTex.loadFromFile("assets/Black/king.png");
	Sprite BkingSprite(BkingTex);
	BkingSprite.setScale(2,2);
	
	
	
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			char piece = board[row][col];
			
			if(piece == ' '){
				continue;
			}
			
			int posX = boarder_width + col * ((window_width-2*boarder_width)/width) + 15;
			int posY = boarder_height + row * ((window_height-2*boarder_height)/height) + 33;
			
			if(piece == 'P'){
				WpawnSprite.setPosition(posX, posY);
				window.draw(WpawnSprite);
			}
			else if(piece == 'R'){
				WrookSprite.setPosition(posX, posY);
				window.draw(WrookSprite);
			}
			else if(piece == 'N'){
				WknightSprite.setPosition(posX, posY);
				window.draw(WknightSprite);
			}
			else if(piece == 'B'){
				WbishopSprite.setPosition(posX, posY);
				window.draw(WbishopSprite);
			}
			else if(piece == 'Q'){
				WqueenSprite.setPosition(posX, posY);
				window.draw(WqueenSprite);
			}
			else if(piece == 'K'){
				WkingSprite.setPosition(posX, posY);
				window.draw(WkingSprite);
			}
			else if(piece == 'p'){
				BpawnSprite.setPosition(posX, posY);
				window.draw(BpawnSprite);
			}
			else if(piece == 'r'){
				BrookSprite.setPosition(posX, posY);
				window.draw(BrookSprite);
			}
			else if(piece == 'n'){
				BknightSprite.setPosition(posX, posY);
				window.draw(BknightSprite);
			}
			else if(piece == 'b'){
				BbishopSprite.setPosition(posX, posY);
				window.draw(BbishopSprite);
			}
			else if(piece == 'q'){
				BqueenSprite.setPosition(posX, posY);
				window.draw(BqueenSprite);
			}
			else if(piece == 'k'){
				BkingSprite.setPosition(posX, posY);
				window.draw(BkingSprite);
			}
		}
	}
	
}
//func end


//new func 
void clicked(char** board, int mouseX , int mouseY){
	
	//pawn
	if(board[mouseX][mouseY] == 'P'){
		
		
		
		
		
		
	}
	
	//rook
	if(board[mouseX][mouseY] == 'R'){
		
		
		
		
		
		
	}
	
	//knight
	if(board[mouseX][mouseY] == 'N'){
		
		
		
		
		
		
	}
	
	//bishop 
	if(board[mouseX][mouseY] == 'B'){
		
		
		
		
		
		
	}
	
	//queen
	if(board[mouseX][mouseY] == 'Q'){
		
		
		
		
		
		
	}
	
	//king 
	if(board[mouseX][mouseY] == 'K'){
		
		
		
		
		
		
	}
	
	//-------------------KAALA-----------------
	
		//pawn
	if(board[mouseX][mouseY] == 'p'){
		
		
		
		
		
		
	}
	
	//rook
	if(board[mouseX][mouseY] == 'r'){
		
		
		
		
		
		
	}
	
	//knight
	if(board[mouseX][mouseY] == 'n'){
		
		
		
		
		
		
	}
	
	//bishop 
	if(board[mouseX][mouseY] == 'b'){
		
		
		
		
		
		
	}
	
	//queen
	if(board[mouseX][mouseY] == 'q'){
		
		
		
		
		
		
	}
	
	//king 
	if(board[mouseX][mouseY] == 'k'){
		
		
		
		
		
		
	}
	
	
	
}
	
//func end

int main(){
	
	Texture bgTexture;
	bgTexture.loadFromFile("assets/chessboard.png");
	Sprite bgSprite(bgTexture);
	bgSprite.setPosition(0,0);
	bgSprite.setScale(1.5,1.5);
	
	
	
	
	bool WisPawnAlive[8] , BisPawnAlive[8] , WisRookAlive[2] , BisRookAlive[2] , WisKnightAlive[2] , BisKnightAlive[2] , WisBishopAlive[2] , BisBishopAlive[2] , WisQueenAlive=true , BisQueenAlive=true , WisKingAlive=true, BisKingAlive=true;
	
	for(int i=0 ; i<2 ; i++){
		WisRookAlive[i] = true;
		WisBishopAlive[i] = true;
		WisKnightAlive[i] = true;
		
		BisRookAlive[i] = true;
		BisBishopAlive[i] = true;
		BisKnightAlive[i] = true;
		
	}
	
	for(int i=0 ; i< 8 ; i++){
		
		BisPawnAlive[i] = true;
		WisPawnAlive[i] = true;
		
		}
		
	int  Wpawn_x[8] , Wpawn_y[8] , Bpawn_x[8] , Bpawn_y[8] , Wrook_x[2] , Wrook_y[2] , Brook_x[2] , Brook_y[2] , Wknight_x[2] , Wknight_y[2] , Bknight_x[2] , Bknight_y[2] , Wbishop_x[2] , Wbishop_y[2] , Bbishop_x[2] , Bbishop_y[2] , Wking_y=7 , Wking_x=4 , Bking_y=0 , Bking_x=4 , Wqueen_y=7 , Wqueen_x=3 , Bqueen_y=0 , Bqueen_x=3;
	
	
	char **board = new char* [height];
	for(int i=0 ; i<height ; i++){
		*(board+i) = new char [width];
	}
	
	for(int i=0 ; i<height ; i++){
		for(int j=0 ; j<width ; j++){
				board[i][j]=' ';
				//CAPITAL FOR WHITE
				if(i == 1){
				 board[i][j]='p';
				 Bpawn_y[j] = i;
				 Bpawn_x[j] = j;
				 }
				 
				else if(i == 6){
				 board[i][j] = 'P';
				 Wpawn_y[j] = i;
				 Wpawn_x[j] = j;
				}
			}
		}



	for(int i=0; i<2 ; i++){
		Brook_y[i]=0;
		Bbishop_y[i]=0;
		Bknight_y[i]=0;
		
		
		Wrook_y[i]=7;
		Wbishop_y[i]=7;
		Wknight_y[i]=7;
		
		if(i%2==0){
			Wrook_x[i]=0;
			Brook_x[i]=0;
			Wknight_x[i]=1;
			Bknight_x[i]=1;
			Wbishop_x[i]=2;
			Bbishop_x[i]=2;
		}
		else{
			Wrook_x[i]=7;
			Brook_x[i]=7;
			Wknight_x[i]=6;
			Bknight_x[i]=6;
			Wbishop_x[i]=5;
			Bbishop_x[i]=5;
		}
		
	}	
		
	//for black
	board[0][0] = 'r';
	board[0][7] = 'r';
	
	board[0][1] = 'n';
	board[0][6] = 'n';
	
	board[0][2] = 'b';
	board[0][5] = 'b';
	
	board[0][3] = 'q';
	board[0][4] = 'k';
	
	//for WHITE
	board[7][0] = 'R';
	board[7][7] = 'R';
	
	board[7][1] = 'N';
	board[7][6] = 'N';
	
	board[7][2] = 'B';
	board[7][5] = 'B';
	
	board[7][3] = 'Q';
	board[7][4] = 'K';
	
	
	
	RenderWindow window(VideoMode(window_width , window_height ), "!!CHESS!!" , Style::Resize);
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
		
		Event mouseButtonPressed;
		
		if(mouseButtonPressed.type == sf::Event::MouseButtonPressed){
			if(mouseButtonPressed.mouseButton.button == Mouse::Button::Left){
				
				clicked( board, mouseButtonPressed.mouseButton.x / 8 , mouseButtonPressed.mouseButton.y / 8 );
				
				}
		}
		
		window.clear();
		//background
		window.draw(bgSprite);
		
		//calling functions
		display_board(window, board);		
		
		
		
		
		
		
		
		
		
		
		
		window.display();
	}
	
return 0;
}
