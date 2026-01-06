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
	
	//Circle for Move Display
	float tileSize = (window_width - 2 * boarder_width) / width;

	CircleShape moveCircle;
	float radius = tileSize * 0.28f;

	moveCircle.setRadius(radius);
	moveCircle.setOrigin(radius, radius);

	// semi transparent gray
	moveCircle.setFillColor(Color(120, 120, 120, 170));

	
	//SPRITES FOR WHITE GOTTI
	static Texture WpawnTex;
	WpawnTex.loadFromFile("assets/White/pawn.png");
	static Sprite WpawnSprite(WpawnTex);
	WpawnSprite.setScale(2,2);
	
	static Texture WrookTex;
	WrookTex.loadFromFile("assets/White/rook.png");
	static Sprite WrookSprite(WrookTex);
	WrookSprite.setScale(2,2);
	
	static Texture WknightTex;
	WknightTex.loadFromFile("assets/White/knight.png");
	static Sprite WknightSprite(WknightTex);
	WknightSprite.setScale(2,2);
	
	static Texture WbishopTex;
	WbishopTex.loadFromFile("assets/White/bishop.png");
	static Sprite WbishopSprite(WbishopTex);
	WbishopSprite.setScale(2,2);
	
	static Texture WqueenTex;
	WqueenTex.loadFromFile("assets/White/queen.png");
	static Sprite WqueenSprite(WqueenTex);
	WqueenSprite.setScale(2,2);
	
	static Texture WkingTex;
	WkingTex.loadFromFile("assets/White/king.png");
	static Sprite WkingSprite(WkingTex);
	WkingSprite.setScale(2,2);
	
	//SPRITES FOR BLACK GOTTI
	static Texture BpawnTex;
	BpawnTex.loadFromFile("assets/Black/pawn.png");
	static Sprite BpawnSprite(BpawnTex);
	BpawnSprite.setScale(2,2);
	
	static Texture BrookTex;
	BrookTex.loadFromFile("assets/Black/rook.png");
	static Sprite BrookSprite(BrookTex);
	BrookSprite.setScale(2,2);
	
	static Texture BknightTex;
	BknightTex.loadFromFile("assets/Black/knight.png");
	static Sprite BknightSprite(BknightTex);
	BknightSprite.setScale(2,2);
	
	static Texture BbishopTex;
	BbishopTex.loadFromFile("assets/Black/bishop.png");
	static Sprite BbishopSprite(BbishopTex);
	BbishopSprite.setScale(2,2);
	
	static Texture BqueenTex;
	BqueenTex.loadFromFile("assets/Black/queen.png");
	static Sprite BqueenSprite(BqueenTex);
	BqueenSprite.setScale(2,2);
	
	static Texture BkingTex;
	BkingTex.loadFromFile("assets/Black/king.png");
	static Sprite BkingSprite(BkingTex);
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
			else if(piece == 'O'){
				//move
				float centerX = boarder_width + col * tileSize + tileSize / 2.f;
				float centerY = boarder_height + row * tileSize + tileSize / 2.f;

				moveCircle.setPosition(centerX, centerY);
				window.draw(moveCircle);
				
			}
						
		}
	}
	
}
//func end


//new func 
void clicked( RenderWindow& window ,char** board, int mouseX , int mouseY , bool& mouseClicked){
	
	
	
	
	// pawn
	if (board[mouseY][mouseX] == 'P') {

		if( board[mouseY-1][mouseX] == ' ' ){
			
			
			board[mouseY-1][mouseX] = 'O';
			board[mouseY-2][mouseX] = 'O';
			
		}
			

	}

	// rook
	if (board[mouseY][mouseX] == 'R') {



	}

	// knight
	if (board[mouseY][mouseX] == 'N') {



	}

	// bishop
	if (board[mouseY][mouseX] == 'B') {


  
	}

	// queen
	if (board[mouseY][mouseX] == 'Q') {



	}

	// king
	if (board[mouseY][mouseX] == 'K') {



	}

	// ------------------- KAALA -----------------

	// pawn
	if (board[mouseY][mouseX] == 'p') {



	}

	// rook
	if (board[mouseY][mouseX] == 'r') {



	}

	// knight
	if (board[mouseY][mouseX] == 'n') {



	}

	// bishop
	if (board[mouseY][mouseX] == 'b') {



	}

	// queen
	if (board[mouseY][mouseX] == 'q') {



	}

	// king
	if (board[mouseY][mouseX] == 'k') {



	}
	
	mouseClicked = true;
	
	
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	//------------------
	
	//for mouse pos X Y
	int x=0 , y=0;
	bool mouseClicked=false;
	
	
	
	
	
	//------------------
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
			
			if(ev.type == Event::MouseButtonPressed){
				if(ev.mouseButton.button == Mouse::Button::Left ){
					
					for(int i=0 ; i<height ; i++){
						for(int j=0 ; j<width ; j++){
							if(board[i][j] == 'O')
								board[i][j] == ' ';
							}
						}
						
						
					x= (ev.mouseButton.x - boarder_width) / ((window_width - 2*boarder_width) / width);
					y= (ev.mouseButton.y - boarder_height) / ((window_height - 2*boarder_height) / height);
					
					clicked( window, board, x , y , mouseClicked );
					
					}
			}
		}
		
		
		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		window.clear();
		//background
		window.draw(bgSprite);
		
		//calling functions
		display_board(window, board);	
		
		if(mouseClicked)
			clicked( window, board, x , y , mouseClicked );
		
		
		
		
		
		
		
		
		
		
		
		window.display();
	}
	
return 0;
}
