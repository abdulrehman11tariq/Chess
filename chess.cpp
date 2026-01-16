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

void display_board(RenderWindow& window, char** board , bool checkCapture[8][8]){
	
	//Circle for Move Display
	float tileSize = (window_width - 2 * boarder_width) / width;

	CircleShape moveCircle;
	float radius = tileSize * 0.28f;

	moveCircle.setRadius(radius);
	moveCircle.setOrigin(radius, radius);

	// semi transparent gray
	moveCircle.setFillColor(Color(120, 120, 120, 170));
	
	
	//Square for capture display
	CircleShape capSquare(tileSize , 4);	
	
	//clr
	capSquare.setFillColor(Color(120,120,120,170));

	
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
void clicked( RenderWindow& window ,char** board, int mouseX , int mouseY , bool& mouseClicked , bool& turn, bool checkCapture[8][8]){
	static int last_mouseY = 0 , last_mouseX = 0 ;
	static char last_clicked_piece = ' ';
	
	
	//MOVE
	//checking validity of move and then move it wuhahahahaha!!!!!!!!!!
	if(board[mouseY][mouseX] == 'O' || checkCapture[mouseY][mouseX]){
		
		
		board[mouseY][mouseX] = last_clicked_piece;
		board[last_mouseY][last_mouseX] = ' ';
		
		
		mouseClicked = false;
		//deleting old valid move
		for(int i=0 ; i<height ; i++){
			for(int j=0 ; j<width ; j++){
				if(board[i][j] == 'O')
					board[i][j] = ' ';
				}
			}
		
		turn = !turn;
	
		//Rotating the matrix for next person's turn!!!!! :yum:
		
		
		//	----------------successfull ATTEMPT TO ROTATE A MATRIX 180 DEGREE-----------------------------------
		
		
		char temp[height][width];
		
		for(int i=0 ; i<height ; i++){
			for(int j=0 ; j<width ; j++){
				temp[i][j] = board[height-1-i][width-1-j];
				}
			}
		
		for(int i=0 ; i<height ; i++){
			for(int j=0 ; j<width ; j++){
				board[i][j] = temp[i][j];
				}
			}
		
		
		
		return;
		
	}
	
	//deleting old valid move and capture
	for(int i=0 ; i<height ; i++){
		for(int j=0 ; j<width ; j++){
			if(board[i][j] == 'O')
				board[i][j] = ' ';
				checkCapture[i][j] = 0;
			}
		}
	
	if(turn == 1){
		// pawn
		if (board[mouseY][mouseX] == 'P') {
			last_clicked_piece = 'P';
			if( board[mouseY-1][mouseX] == ' ' ){
				board[mouseY-1][mouseX] = 'O';
				if(board[mouseY-2][mouseX] == ' ' && mouseY == 6 )
					board[mouseY-2][mouseX] = 'O';
			}
				

		}

		// rook
		if (board[mouseY][mouseX] == 'R') {
		last_clicked_piece = 'R';
			
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'p'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'k'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'r'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'b'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'n'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'p'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'k'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'r'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'b'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'n'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'p'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'k'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'r'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'b'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'n'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'p'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'k'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'r'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'b'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'n'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
				
		}

		// knight
		if (board[mouseY][mouseX] == 'N') {
		last_clicked_piece = 'N';
			
			
			if(mouseX-1 >= 0  &&  mouseY+2 < height){
							switch(board[mouseY+2][mouseX-1]){
							case ' ':
			 					board[mouseY+2][mouseX-1] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY+2][mouseX-1] = true;
			 					}
			 				}
		
			if(mouseX+1 < width  &&  mouseY+2 < height){
							switch(board[mouseY+2][mouseX+1]){
							case ' ':
			 					board[mouseY+2][mouseX+1] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY+2][mouseX+1] = true;
			 					}
			 				}
			
			if(mouseY-1 >= 0  &&  mouseX+2 < width){
							switch(board[mouseY-1][mouseX+2]){
							case ' ':
			 					board[mouseY-1][mouseX+2] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY-1][mouseX+2] = true;
			 					}
			 				}					
			
			if(mouseY+1 < height  &&  mouseX+2 < width){
							switch(board[mouseY+1][mouseX+2]){
							case ' ':
			 					board[mouseY+1][mouseX+2] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY+1][mouseX+2] = true;
			 					}
			 				}
			
			if(mouseY-2 >= 0  &&  mouseX-1 >= 0){
							switch(board[mouseY-2][mouseX-1]){
							case ' ':
			 					board[mouseY-2][mouseX-1] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY-2][mouseX-1] = true;
			 					}
			 				} 					
			
			if(mouseY-2 >= 0  &&  mouseX+1 < width){
							switch(board[mouseY-2][mouseX+1]){
							case ' ':
			 					board[mouseY-2][mouseX+1] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY-2][mouseX+1] = true;
			 					}
			 				}
			
			if(mouseY-1 >= 0  &&  mouseX-2 >= 0){
							switch(board[mouseY-1][mouseX-2]){
							case ' ':
			 					board[mouseY-1][mouseX-2] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY-1][mouseX-2] = true;
			 					}
			 				}
			
			if(mouseY+1 < height  &&  mouseX-2 >= 0){
							switch(board[mouseY+1][mouseX-2]){
							case ' ':
			 					board[mouseY+1][mouseX-2] = 'O';
			 					break;
			 				case 'p':
			 				case 'q':
			 				case 'b':
			 				case 'r':
			 				case 'k':
			 				case 'n':
			 					checkCapture[mouseY+1][mouseX-2] = true;
			 					}
			 				}


		}
		

		// bishop
		if (board[mouseY][mouseX] == 'B') {
		last_clicked_piece = 'B';

			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(board[mouseY + i][mouseX + i] =='p'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='q'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='n'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='r'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='k'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='b'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(board[mouseY - i][mouseX + i] =='p'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='q'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='n'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='r'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='k'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='b'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(board[mouseY - i][mouseX - i] =='p'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='q'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='n'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='r'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='k'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='b'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(board[mouseY + i][mouseX - i] =='p'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='q'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='n'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='r'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='k'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='b'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
							
					else break;
					}
									
				}
	  
		}

		// queen
		if (board[mouseY][mouseX] == 'Q') {
		last_clicked_piece = 'Q';
			
		//all rook moves
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'p'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'k'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'r'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'b'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'n'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'p'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'k'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'r'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'b'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'n'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'p'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'k'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'r'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'b'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'n'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'p'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'k'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'r'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'b'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'n'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			
			
		//all bishop movessssss
				for(int i=1 ; i<height ; i++){
					
					if( mouseY+i<height && mouseX+i<width) {
						if(board[mouseY + i][mouseX + i] ==' ')
							board[mouseY + i][mouseX + i] = 'O';
							
						else if(board[mouseY + i][mouseX + i] =='p'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
						else if(board[mouseY + i][mouseX + i] =='q'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
						else if(board[mouseY + i][mouseX + i] =='n'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
						else if(board[mouseY + i][mouseX + i] =='r'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
						else if(board[mouseY + i][mouseX + i] =='k'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
						else if(board[mouseY + i][mouseX + i] =='b'){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
								
						else break;
						}
					}
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY-i >= 0 && mouseX+i < width) {
						if(board[mouseY - i][mouseX + i] ==' ')
							board[mouseY - i][mouseX + i] = 'O';
							
						else if(board[mouseY - i][mouseX + i] =='p'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
						else if(board[mouseY - i][mouseX + i] =='q'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
						else if(board[mouseY - i][mouseX + i] =='n'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
						else if(board[mouseY - i][mouseX + i] =='r'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
						else if(board[mouseY - i][mouseX + i] =='k'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
						else if(board[mouseY - i][mouseX + i] =='b'){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
								
						else break;
						}
						
					}
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY-i >= 0 && mouseX-i >= 0) {
						if(board[mouseY - i][mouseX - i] ==' ')
							board[mouseY - i][mouseX - i] = 'O';
							
						else if(board[mouseY - i][mouseX - i] =='p'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
						else if(board[mouseY - i][mouseX - i] =='q'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
						else if(board[mouseY - i][mouseX - i] =='n'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
						else if(board[mouseY - i][mouseX - i] =='r'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
						else if(board[mouseY - i][mouseX - i] =='k'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
						else if(board[mouseY - i][mouseX - i] =='b'){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
								
						else break;
						}
					
					}	
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY+i < height && mouseX-i >= 0) {
						if(board[mouseY + i][mouseX - i] ==' ')
							board[mouseY + i][mouseX - i] = 'O';
							
						else if(board[mouseY + i][mouseX - i] =='p'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
						else if(board[mouseY + i][mouseX - i] =='q'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
						else if(board[mouseY + i][mouseX - i] =='n'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
						else if(board[mouseY + i][mouseX - i] =='r'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
						else if(board[mouseY + i][mouseX - i] =='k'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
						else if(board[mouseY + i][mouseX - i] =='b'){
							checkCapture[mouseY + i][mouseX - i] = true;
							break;}
								
						else break;
						}
										
					}

		}

		// king
		if (board[mouseY][mouseX] == 'K') {
		last_clicked_piece = 'K';
		
		
			if(mouseX-1 >= 0  &&  board[mouseY][mouseX-1] == ' ') 					board[mouseY][mouseX-1] = 'O';
			
			if(mouseX+1 < width  &&  board[mouseY][mouseX+1] == ' ') 				board[mouseY][mouseX+1] = 'O';
			
			if(mouseY-1 >= 0  &&  board[mouseY-1][mouseX] == ' ') 					board[mouseY-1][mouseX] = 'O';
			
			if(mouseY+1 < height  &&  board[mouseY+1][mouseX] == ' ') 				board[mouseY+1][mouseX] = 'O';
			
			if(mouseY+1 < height  &&  mouseX-1 >= 0  &&  board[mouseY+1][mouseX-1] == ' ') 		board[mouseY+1][mouseX-1] = 'O';
			
			if(mouseY+1 < height  &&  mouseX+1 < width  &&  board[mouseY+1][mouseX+1] == ' ') 	board[mouseY+1][mouseX+1] = 'O';
			
			if(mouseY-1 >= 0  &&  mouseX-1 >= 0  &&  board[mouseY-1][mouseX-1] == ' ') 		board[mouseY-1][mouseX-1] = 'O';
		
			if(mouseY-1 >= 0  &&  mouseX+1 < width  &&  board[mouseY-1][mouseX+1] == ' ') 		board[mouseY-1][mouseX+1] = 'O';
		
		
		}
	}

	// ------------------- KAALA -----------------
	if(turn == 0){
		// pawn
		if (board[mouseY][mouseX] == 'p') {
		last_clicked_piece = 'p';
			if( board[mouseY-1][mouseX] == ' ' ){
				
				board[mouseY-1][mouseX] = 'O';
				if(board[mouseY-2][mouseX] == ' ' && mouseY == 6 )
					board[mouseY-2][mouseX] = 'O';
			}
		
		}

		// rook
		if (board[mouseY][mouseX] == 'r') {
		last_clicked_piece = 'r';
			
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'P'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'K'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'Q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'R'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'B'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'N'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'P'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'K'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'Q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'R'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'B'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'N'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'P'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'K'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'Q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'R'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'B'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'N'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'P'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'K'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'Q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'R'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'B'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'N'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}


		}

		// knight
		if (board[mouseY][mouseX] == 'n') {
		last_clicked_piece = 'n';
		
			if(mouseX-1 >= 0  &&  mouseY+2 < height){
							switch(board[mouseY+2][mouseX-1]){
							case ' ':
			 					board[mouseY+2][mouseX-1] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY+2][mouseX-1] = true;
			 					}
			 				}
		
			if(mouseX+1 < width  &&  mouseY+2 < height){
							switch(board[mouseY+2][mouseX+1]){
							case ' ':
			 					board[mouseY+2][mouseX+1] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY+2][mouseX+1] = true;
			 					}
			 				}
			
			if(mouseY-1 >= 0  &&  mouseX+2 < width){
							switch(board[mouseY-1][mouseX+2]){
							case ' ':
			 					board[mouseY-1][mouseX+2] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY-1][mouseX+2] = true;
			 					}
			 				}					
			
			if(mouseY+1 < height  &&  mouseX+2 < width){
							switch(board[mouseY+1][mouseX+2]){
							case ' ':
			 					board[mouseY+1][mouseX+2] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY+1][mouseX+2] = true;
			 					}
			 				}
			
			if(mouseY-2 >= 0  &&  mouseX-1 >= 0){
							switch(board[mouseY-2][mouseX-1]){
							case ' ':
			 					board[mouseY-2][mouseX-1] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY-2][mouseX-1] = true;
			 					}
			 				} 					
			
			if(mouseY-2 >= 0  &&  mouseX+1 < width){
							switch(board[mouseY-2][mouseX+1]){
							case ' ':
			 					board[mouseY-2][mouseX+1] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY-2][mouseX+1] = true;
			 					}
			 				}
			
			if(mouseY-1 >= 0  &&  mouseX-2 >= 0){
							switch(board[mouseY-1][mouseX-2]){
							case ' ':
			 					board[mouseY-1][mouseX-2] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY-1][mouseX-2] = true;
			 					}
			 				}
			
			if(mouseY+1 < height  &&  mouseX-2 >= 0){
							switch(board[mouseY+1][mouseX-2]){
							case ' ':
			 					board[mouseY+1][mouseX-2] = 'O';
			 					break;
			 				case 'P':
			 				case 'Q':
			 				case 'B':
			 				case 'R':
			 				case 'K':
			 				case 'N':
			 					checkCapture[mouseY+1][mouseX-2] = true;
			 					}
			 				}


		}

		// bishop
		if (board[mouseY][mouseX] == 'b') {
		last_clicked_piece = 'b';
			
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(board[mouseY + i][mouseX + i] =='P'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='Q'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='N'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='R'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='K'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='B'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(board[mouseY - i][mouseX + i] =='P'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='Q'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='N'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='R'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='K'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='B'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(board[mouseY - i][mouseX - i] =='P'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='Q'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='N'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='R'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='K'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='B'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(board[mouseY + i][mouseX - i] =='P'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='Q'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='N'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='R'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='K'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='B'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
							
					else break;
					}
									
				}
	  
		}

		// queen
		if (board[mouseY][mouseX] == 'q') {
		last_clicked_piece = 'q';
		
		//all rook moves
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'P'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'K'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'Q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'R'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'B'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'N'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(board[mouseY][i] == 'P'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'K'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'Q'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'R'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'B'){
					checkCapture[mouseY][i] = true;
					break;}
				else if(board[mouseY][i] == 'N'){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'P'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'K'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'Q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'R'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'B'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'N'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(board[i][mouseX] == 'P'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'K'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'Q'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'R'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'B'){
					checkCapture[i][mouseX] = true;
					break;}
				else if(board[i][mouseX] == 'N'){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}


		
			
			
		//all bishop movessssss
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(board[mouseY + i][mouseX + i] =='P'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='Q'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='N'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='R'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='K'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
					else if(board[mouseY + i][mouseX + i] =='B'){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(board[mouseY - i][mouseX + i] =='P'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='Q'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='N'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='R'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='K'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
					else if(board[mouseY - i][mouseX + i] =='B'){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(board[mouseY - i][mouseX - i] =='P'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='Q'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='N'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='R'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='K'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
					else if(board[mouseY - i][mouseX - i] =='B'){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(board[mouseY + i][mouseX - i] =='P'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='Q'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='N'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='R'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='K'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
					else if(board[mouseY + i][mouseX - i] =='B'){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
							
					else break;
					}
									
				}


		}

		// king
		if (board[mouseY][mouseX] == 'k') {
		last_clicked_piece = 'k';
		
			if(mouseX-1 >= 0  &&  board[mouseY][mouseX-1] == ' ') 					board[mouseY][mouseX-1] = 'O';
			
			if(mouseX+1 < width  &&  board[mouseY][mouseX+1] == ' ') 				board[mouseY][mouseX+1] = 'O';
			
			if(mouseY-1 >= 0  &&  board[mouseY-1][mouseX] == ' ') 					board[mouseY-1][mouseX] = 'O';
			
			if(mouseY+1 < height  &&  board[mouseY+1][mouseX] == ' ') 				board[mouseY+1][mouseX] = 'O';
			
			if(mouseY+1 < height  &&  mouseX-1 >= 0  &&  board[mouseY+1][mouseX-1] == ' ') 		board[mouseY+1][mouseX-1] = 'O';
			
			if(mouseY+1 < height  &&  mouseX+1 < width  &&  board[mouseY+1][mouseX+1] == ' ') 	board[mouseY+1][mouseX+1] = 'O';
			
			if(mouseY-1 >= 0  &&  mouseX-1 >= 0  &&  board[mouseY-1][mouseX-1] == ' ') 		board[mouseY-1][mouseX-1] = 'O';
		
			if(mouseY-1 >= 0  &&  mouseX+1 < width  &&  board[mouseY-1][mouseX+1] == ' ') 		board[mouseY-1][mouseX+1] = 'O';


		}
	}
	
	last_mouseY = mouseY;
	last_mouseX = mouseX;
	mouseClicked = true;
	
	
}
	
//func end

int main(){
	
	Texture bgTexture;
	bgTexture.loadFromFile("assets/chessboard.png");
	Sprite bgSprite(bgTexture);
	bgSprite.setPosition(0,0);
	bgSprite.setScale(1.5,1.5);
	
	
	
	bool checkCapture[8][8];
	
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
				 }
				 
				else if(i == 6){
				 board[i][j] = 'P';
				}
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
	bool turn = 1; //1 for White --- 0 for Black
	
	
	
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
					
						
						
					x= (ev.mouseButton.x - boarder_width) / ((window_width - 2*boarder_width) / width);
					y= (ev.mouseButton.y - boarder_height) / ((window_height - 2*boarder_height) / height);
					
					clicked( window, board, x , y , mouseClicked , turn , checkCapture);
					
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
		display_board(window, board , checkCapture);	
		
		
		
		
		
		
		
		
		
		
		
		
		window.display();
	}
	
return 0;
}
