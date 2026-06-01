#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace  sf;

const int height = 8; 
const int width =  8;
const int window_height = 1050;
const int window_width  = 1050;
const int boarder_width = 33*1.5;
const int boarder_height = 33*1.5;

void display_board(RenderWindow& window, char** board , bool checkCapture[height][width], bool wCheck, bool bCheck){
	
	//Circle for Move Display
	float tileSize = (window_width - 2 * boarder_width) / width;

	CircleShape moveCircle;
	float radius = tileSize * 0.28f;

	moveCircle.setRadius(radius);
	moveCircle.setOrigin(radius, radius);

	// semi transparent gray
	moveCircle.setFillColor(Color(120, 120, 120, 170));
	
	
	//Square for capture display
	CircleShape capSquare(tileSize*0.5f , 4);	
	
	//clr
	capSquare.setFillColor(Color::Transparent);
	capSquare.setOutlineColor(Color::Red);
	capSquare.setOutlineThickness(4.0f);
	capSquare.setRotation(45);
			
	
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
	
	
	// RED HIGHLIGHT on king tile when in check
	float tileS = (window_width - 2 * boarder_width) / width;
	RectangleShape checkHighlight(Vector2f(tileS, tileS));
	checkHighlight.setFillColor(Color(220, 50, 50, 140));
	checkHighlight.setOutlineColor(Color(255, 0, 0, 255));
	checkHighlight.setOutlineThickness(4.0f);

	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			char p = board[row][col];
			if( (wCheck && p == 'K') || (bCheck && p == 'k') ){
				float px = boarder_width + col * tileS;
				float py = boarder_height + row * tileS;
				checkHighlight.setPosition(px, py);
				window.draw(checkHighlight);
			}
		}
	}

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
			
			
			float centerX = boarder_width + col * tileSize + tileSize/2.0f;
			float centerY = boarder_height + row * tileSize ;

			capSquare.setPosition(centerX , centerY);
			if(checkCapture[row][col])
				window.draw(capSquare);
			
						
		}
	}
}

//func end


//check detection
void check_det( bool turn , char** board , bool& isCheck){
	
	char king = turn ? 'K' : 'k';
	int kingX=0 , kingY=0;
	
	for(int i=0 ; i<height ; i++){
		for(int j=0 ; j<width ;j++){
			if(board[i][j] == king){
				kingY=i;
				kingX=j;
				break;
				}
			}
		}
		
	int pcsCorrection = turn ? 'a' - 'A' : 0 ;
	
	
	//pawn check
	if(kingY-1 >=0 && kingX+1 < width){
		if(board[kingY-1][kingX+1] == 'P' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY-1 >=0 && kingX-1 >= 0){
		if(board[kingY-1][kingX-1] == 'P' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	//bishop
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height && kingX + i < width ){
			if(board[kingY+i][kingX+i] == 'B' + pcsCorrection || board[kingY+i][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX+i] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height && kingX - i >= 0 ){
			if(board[kingY+i][kingX-i] == 'B' + pcsCorrection || board[kingY+i][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX-i] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0 && kingX + i < width ){
			if(board[kingY-i][kingX+i] == 'B' + pcsCorrection || board[kingY-i][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX+i] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0 && kingX - i >= 0 ){
			if(board[kingY-i][kingX-i] == 'B' + pcsCorrection || board[kingY-i][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX-i] != ' ') break;
		}
	}
	
	//rook
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height){
			if(board[kingY+i][kingX] == 'R' + pcsCorrection || board[kingY+i][kingX] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0){
			if(board[kingY-i][kingX] == 'R' + pcsCorrection || board[kingY-i][kingX] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingX + i < height){
			if(board[kingY][kingX+i] == 'R' + pcsCorrection || board[kingY][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY][kingX+i] != ' ') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingX - i >= 0){
			if(board[kingY][kingX-i] == 'R' + pcsCorrection || board[kingY][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY][kingX-i] != ' ') break;
		}
	}
	
	//knight

	if( kingY+2 < height && kingX-1 >= 0 ){
		if(board[kingY+2][kingX-1] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
				

	if( kingY+2 < height && kingX+1 < width ){
		if(board[kingY+2][kingX+1] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY-1 >= 0  &&  kingX+2 < width){
		if(board[kingY-1][kingX+2] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}					
	
	if(kingY+1 < height  &&  kingX+2 < width){
		if(board[kingY+1][kingX+2] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY-2 >= 0  &&  kingX-1 >= 0){
		if(board[kingY-2][kingX-1] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}					
	
	if(kingY-2 >= 0  &&  kingX+1 < width){
		if(board[kingY-2][kingX+1] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY-1 >= 0  &&  kingX-2 >= 0){
		if(board[kingY-1][kingX-2] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY+1 < height  &&  kingX-2 >= 0){
		if(board[kingY+1][kingX-2] == 'N' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
		
		//--------------------------------------------------- 
	//king
	if(kingX-1 >= 0 ){
		if(board[kingY][kingX-1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}		
								
	if(kingX+1 < width){
		if(board[kingY][kingX+1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}	
			
	if(kingY-1 >= 0 ){
		if(board[kingY-1][kingX] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
			
	if(kingY+1 < height ){
		if(board[kingY+1][kingX] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
	
	if(kingY+1 < height &&  kingX-1 >= 0){
		if(board[kingY+1][kingX-1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
			
	if(kingY+1 < height && kingX+1 < width ){
		if(board[kingY+1][kingX+1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
			
	if(kingY-1 >= 0 &&  kingX-1 >= 0 ){
		if(board[kingY-1][kingX-1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
		
	if(kingY-1 >= 0 && kingX+1 < width){
		if(board[kingY-1][kingX+1] == 'K' + pcsCorrection){
			isCheck = true;
			return;
		}
	}
}

//pin detection for white piece at (pieceY, pieceX)
//temporarily removes the piece and checks if white king is in check
bool is_pinned_white(char** board, int pieceY, int pieceX){
	
	char saved = board[pieceY][pieceX];
	board[pieceY][pieceX] = ' ';
	
	bool kingInCheck = false;
	check_det(true, board, kingInCheck);
	
	board[pieceY][pieceX] = saved;
	
	return kingInCheck;
}

//pin detection for black piece at (pieceY, pieceX)
//temporarily removes the piece and checks if black king is in check
bool is_pinned_black(char** board, int pieceY, int pieceX){
	
	char saved = board[pieceY][pieceX];
	board[pieceY][pieceX] = ' ';
	
	bool kingInCheck = false;
	check_det(false, board, kingInCheck);
	
	board[pieceY][pieceX] = saved;
	
	return kingInCheck;
}

//new func 
void clicked( RenderWindow& window ,char** board, int mouseX , int mouseY , bool& mouseClicked , bool& turn, bool checkCapture[8][8]){
	if (mouseX < 0 || mouseX >= width || mouseY < 0 || mouseY >= height) return;
	
	static int last_mouseY = 0 , last_mouseX = 0 ;
	static char last_clicked_piece = ' ';
	
	//en passant tracking
	//ep_active: was the last move a double pawn push?
	//ep_col: column (after board rotation) where that pawn landed
	static bool ep_active = false;
	static int  ep_col    = -1;
	
	//-------- CASTLING TRACKING --------
	//these flags track if the king or rooks have ever moved
	//white uses W prefix, black uses B prefix
	static bool wKingMoved  = false;
	static bool wRookKMoved = false; //white kingside  rook (col 7 before rotation)
	static bool wRookQMoved = false; //white queenside rook (col 0 before rotation)
	static bool bKingMoved  = false;
	static bool bRookKMoved = false; //black kingside  rook
	static bool bRookQMoved = false;
	
	
	//MOVE
	//checking validity of move and then move it wuhahahahaha!!!!!!!!!!
	if(board[mouseY][mouseX] == 'O' || checkCapture[mouseY][mouseX]){
		
		//-------- EN PASSANT CAPTURE detection --------
		//if a pawn moved diagonally onto an empty 'O' square, it is an en passant capture
		//the captured pawn is sitting one row below (row 3) at ep_col
		bool isEnPassantCapture = false;
		if(ep_active && board[mouseY][mouseX] == 'O' && mouseY == 2 && mouseX == ep_col){
			if( (last_clicked_piece == 'P' || last_clicked_piece == 'p') && mouseX != last_mouseX ){
				isEnPassantCapture = true;
			}
		}
		
		//-------- CASTLING EXECUTION --------
		//if king moves 2 squares horizontally it must be a castling move
		//move the corresponding rook as well
		if(last_clicked_piece == 'K' && last_mouseY == 7 && last_mouseX == 4){
			if(mouseY == 7 && mouseX == 6){
				//kingside castling: move rook from col 7 to col 5
				board[7][5] = 'R';
				board[7][7] = ' ';
			}
			if(mouseY == 7 && mouseX == 2){
				//queenside castling: move rook from col 0 to col 3
				board[7][3] = 'R';
				board[7][0] = ' ';
			}
		}
		if(last_clicked_piece == 'k' && last_mouseY == 7 && last_mouseX == 4){
			if(mouseY == 7 && mouseX == 6){
				board[7][5] = 'r';
				board[7][7] = ' ';
			}
			if(mouseY == 7 && mouseX == 2){
				board[7][3] = 'r';
				board[7][0] = ' ';
			}
		}
		
		board[mouseY][mouseX] = last_clicked_piece;
		board[last_mouseY][last_mouseX] = ' ';
		
		//remove the en passant captured pawn (it sits at row 3, same column)
		if(isEnPassantCapture){
			board[3][ep_col] = ' ';
		}
		
		//-------- CASTLING FLAG UPDATES --------
		//track if king or rooks have moved so castling rights are lost
		if(turn == 1){
			if(last_clicked_piece == 'K')                             wKingMoved  = true;
			if(last_clicked_piece == 'R' && last_mouseX == 7 && last_mouseY == 7) wRookKMoved = true;
			if(last_clicked_piece == 'R' && last_mouseX == 0 && last_mouseY == 7) wRookQMoved = true;
		}
		else{
			if(last_clicked_piece == 'k')                             bKingMoved  = true;
			if(last_clicked_piece == 'r' && last_mouseX == 7 && last_mouseY == 7) bRookKMoved = true;
			if(last_clicked_piece == 'r' && last_mouseX == 0 && last_mouseY == 7) bRookQMoved = true;
		}
		
		//-------- EN PASSANT STATE UPDATE --------
		//check if a pawn just did a double push from row 6 to row 4
		//after rotation the column flips: width-1-mouseX
		if( (last_clicked_piece == 'P' || last_clicked_piece == 'p') && last_mouseY == 6 && mouseY == 4){
			ep_active = true;
			ep_col    = width - 1 - mouseX;  //column after board rotation
		}
		else{
			ep_active = false;
			ep_col    = -1;
		}
		
		mouseClicked = false;
		//deleting old valid move
		for(int i=0 ; i<height ; i++){
			for(int j=0 ; j<width ; j++){
				if(board[i][j] == 'O')		board[i][j] = ' ';
					checkCapture[i][j] = false;
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
			if(board[i][j] == 'O')		board[i][j] = ' ';
				checkCapture[i][j] = false;
			}
		}
	
	if(turn == 1){
		// pawn
		if (board[mouseY][mouseX] == 'P') {
			if(is_pinned_white(board, mouseY, mouseX)) return;
			last_clicked_piece = 'P';
			if( mouseY-1 >= 0 && board[mouseY-1][mouseX] == ' '){
				board[mouseY-1][mouseX] = 'O';
				if(mouseY == 6 && board[mouseY-2][mouseX] == ' ' )
					board[mouseY-2][mouseX] = 'O';
			}
			
			if (mouseY-1 >= 0 && mouseX-1 >= 0) {
				switch( board[mouseY-1][mouseX-1]){
					case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
						checkCapture[mouseY-1][mouseX-1] = true;
				}
			}
			
			if (mouseY-1 >= 0 && mouseX+1 < width) {
				switch( board[mouseY-1][mouseX+1]){
					case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
						checkCapture[mouseY-1][mouseX+1] = true;
				}	
			}
			
			//-------- EN PASSANT for white pawn --------
			//the white pawn must be on row 3 (same rank as the enemy pawn that double-pushed)
			//ep_col holds the column (after rotation) where the black pawn landed at row 3
			if(ep_active && mouseY == 3){
				if(mouseX - 1 == ep_col && mouseX - 1 >= 0 && board[3][ep_col] == 'p'){
					board[2][ep_col] = 'O';
				}
				if(mouseX + 1 == ep_col && mouseX + 1 < width && board[3][ep_col] == 'p'){
					board[2][ep_col] = 'O';
				}
			}

		}

		// rook
		if (board[mouseY][mouseX] == 'R') {
			if(is_pinned_white(board, mouseY, mouseX)) return;
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
			if(is_pinned_white(board, mouseY, mouseX)) return;
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
			if(is_pinned_white(board, mouseY, mouseX)) return;
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
			if(is_pinned_white(board, mouseY, mouseX)) return;
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
		
		
			if(mouseX-1 >= 0 ){
				switch(board[mouseY][mouseX-1]){
					case ' ':
	 					board[mouseY][mouseX-1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY][mouseX-1] = true;
	 					}
					}
								
			if(mouseX+1 < width){
				switch(board[mouseY][mouseX+1]){
					case ' ':
	 					board[mouseY][mouseX+1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY][mouseX+1] = true;
	 					}
					}	
			
			if(mouseY-1 >= 0 ){
				switch(board[mouseY-1][mouseX]){
					case ' ':
	 					board[mouseY-1][mouseX] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY-1][mouseX] = true;
	 					}
					}
			
			if(mouseY+1 < height ){
				switch(board[mouseY+1][mouseX]){
					case ' ':
	 					board[mouseY+1][mouseX] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY+1][mouseX] = true;
	 					}
					}
			
			if(mouseY+1 < height && mouseX-1 >=0){
				switch(board[mouseY+1][mouseX-1]){
					case ' ':
	 					board[mouseY+1][mouseX-1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY+1][mouseX-1] = true;
	 					}
					}
			
			if(mouseY+1 < height && mouseX+1 < width ){
				switch(board[mouseY+1][mouseX+1]){
					case ' ':
	 					board[mouseY+1][mouseX+1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY+1][mouseX+1] = true;
	 					}
					}
			
			if(mouseY-1 >= 0 && mouseX-1 >=0){
				switch(board[mouseY-1][mouseX-1]){
					case ' ':
	 					board[mouseY-1][mouseX-1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY-1][mouseX-1] = true;
	 					}
					}
		
			if(mouseY-1 >= 0 &&  mouseX+1 < width){
				switch(board[mouseY-1][mouseX+1]){
					case ' ':
	 					board[mouseY-1][mouseX+1] = 'O';
	 					break;
	 				case 'p':
	 				case 'q':
	 				case 'b':
	 				case 'r':
	 				case 'k':
	 				case 'n':
	 					checkCapture[mouseY-1][mouseX+1] = true;
	 					}
					}
		
			//-------- CASTLING for WHITE king --------
			//king must be on its starting square (row 7, col 4) and not have moved
			//the path squares must be empty, and the rook must not have moved
			if(mouseY == 7 && mouseX == 4 && !wKingMoved){
				
				//kingside: col 5 and col 6 must be empty, rook at col 7 must not have moved
				if(!wRookKMoved && board[7][7] == 'R' && board[7][5] == ' ' && board[7][6] == ' '){
					board[7][6] = 'O';
				}
				
				//queenside: col 3, col 2, col 1 must be empty, rook at col 0 must not have moved
				if(!wRookQMoved && board[7][0] == 'R' && board[7][3] == ' ' && board[7][2] == ' ' && board[7][1] == ' '){
					board[7][2] = 'O';
				}
			}
		
		}
	}

	// ------------------------------------------------------------------------------------ KAALA ---------------------------------------------------------------
	if(turn == 0){
		// pawn
		if (board[mouseY][mouseX] == 'p') {
			if(is_pinned_black(board, mouseY, mouseX)) return;
		last_clicked_piece = 'p';
			if( mouseY-1 >= 0 && board[mouseY-1][mouseX] == ' ' ){
				
				board[mouseY-1][mouseX] = 'O';
				if(mouseY == 6 && board[mouseY-2][mouseX] == ' ' )
					board[mouseY-2][mouseX] = 'O';
			}
		
			if (mouseY-1 >= 0 && mouseX-1 >= 0) {
				switch( board[mouseY-1][mouseX-1]){
					case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
						checkCapture[mouseY-1][mouseX-1] = true;
				}
			}
			
			if (mouseY-1 >= 0 && mouseX+1 < width) {
				switch( board[mouseY-1][mouseX+1]){
					case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
						checkCapture[mouseY-1][mouseX+1] = true;
				}
			}
			
			//-------- EN PASSANT for black pawn --------
			//same logic as white: black pawn must be on row 3, ep_col is the white pawn's column
			if(ep_active && mouseY == 3){
				if(mouseX - 1 == ep_col && mouseX - 1 >= 0 && board[3][ep_col] == 'P'){
					board[2][ep_col] = 'O';
				}
				if(mouseX + 1 == ep_col && mouseX + 1 < width && board[3][ep_col] == 'P'){
					board[2][ep_col] = 'O';
				}
			}

		}

		// rook
		if (board[mouseY][mouseX] == 'r') {
			if(is_pinned_black(board, mouseY, mouseX)) return;
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
			if(is_pinned_black(board, mouseY, mouseX)) return;
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
			if(is_pinned_black(board, mouseY, mouseX)) return;
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
			if(is_pinned_black(board, mouseY, mouseX)) return;
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
		
			if(mouseX-1 >= 0 ){
				switch(board[mouseY][mouseX-1]){
					case ' ':
	 					board[mouseY][mouseX-1] = 'O';
	 					break;
	 				case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
	 					checkCapture[mouseY][mouseX-1] = true;
	 					}
					}
								
			if(mouseX+1 < width){
				switch(board[mouseY][mouseX+1]){
					case ' ':
	 					board[mouseY][mouseX+1] = 'O';
	 					break;
	 				case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
	 					checkCapture[mouseY][mouseX+1] = true;
	 					}
					}	
			
			if(mouseY-1 >= 0 ){
				switch(board[mouseY-1][mouseX]){
					case ' ':
	 					board[mouseY-1][mouseX] = 'O';
	 					break;
	 				case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
	 					checkCapture[mouseY-1][mouseX] = true;
	 					}
					}
			
			if(mouseY+1 < height ){
				switch(board[mouseY+1][mouseX]){
					case ' ':
	 					board[mouseY+1][mouseX] = 'O';
	 					break;
	 				case 'P':
	 				case 'Q':
	 				case 'B':
	 				case 'R':
	 				case 'K':
	 				case 'N':
	 					checkCapture[mouseY+1][mouseX] = true;
	 					}
					}
			
			if(mouseY+1 < height && mouseX-1 >= 0){
				switch(board[mouseY+1][mouseX-1]){
					case ' ':
	 					board[mouseY+1][mouseX-1] = 'O';
	 					break;
					case 'P':
					case 'Q':
					case 'B':
					case 'R':
					case 'K':
					case 'N':
	 					checkCapture[mouseY+1][mouseX-1] = true;
	 					}
					}
			
			if(mouseY+1 < height && mouseX+1 < width){
				switch(board[mouseY+1][mouseX+1]){
					case ' ':
	 					board[mouseY+1][mouseX+1] = 'O';
	 					break;
					case 'P':
					case 'Q':
					case 'B':
					case 'R':
					case 'K':
					case 'N':
	 					checkCapture[mouseY+1][mouseX+1] = true;
	 					}
					}
			
			if(mouseY-1 >= 0 && mouseX-1 >= 0){
				switch(board[mouseY-1][mouseX-1]){
					case ' ':
	 					board[mouseY-1][mouseX-1] = 'O';
	 					break;
					case 'P':
					case 'Q':
					case 'B':
					case 'R':
					case 'K':
					case 'N':
	 					checkCapture[mouseY-1][mouseX-1] = true;
	 					}
					}
		
			if(mouseY-1 >= 0 && mouseX+1 < width){
				switch(board[mouseY-1][mouseX+1]){
					case ' ':
	 					board[mouseY-1][mouseX+1] = 'O';
	 					break;
					case 'P':
					case 'Q':
					case 'B':
					case 'R':
					case 'K':
					case 'N':
	 					checkCapture[mouseY-1][mouseX+1] = true;
	 					}
					}

			//-------- CASTLING for BLACK king --------
			//same logic as white but uses bKingMoved / bRookKMoved / bRookQMoved
			if(mouseY == 7 && mouseX == 4 && !bKingMoved){
				
				//kingside
				if(!bRookKMoved && board[7][7] == 'r' && board[7][5] == ' ' && board[7][6] == ' '){
					board[7][6] = 'O';
				}
				
				//queenside
				if(!bRookQMoved && board[7][0] == 'r' && board[7][3] == ' ' && board[7][2] == ' ' && board[7][1] == ' '){
					board[7][2] = 'O';
				}
			}

		}
	}
	
	last_mouseY = mouseY;
	last_mouseX = mouseX;
	mouseClicked = true;
	
	
}
	
//func end

// Returns true when the only pieces left on the board are the two kings
bool is_king_vs_king(char** board){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			char p = board[i][j];
			if(p != ' ' && p != 'K' && p != 'k' && p != 'O')
				return false;
		}
	}
	return true;
}

int main(){
	
	Texture bgTexture;
	bgTexture.loadFromFile("assets/chessboard.png");
	Sprite bgSprite(bgTexture);
	bgSprite.setPosition(0,0);
	bgSprite.setScale(1.5,1.5);
	
	
	
	bool checkCapture[8][8];
	bool whiteInCheck = false;
	bool blackInCheck = false;
	
	char **board = new char* [height];
	for(int i=0 ; i<height ; i++){
		*(board+i) = new char [width];
	}
	
	for(int i=0 ; i<height ; i++){
		for(int j=0 ; j<width ; j++){
				checkCapture[i][j] = 0;
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
	bool isDraw = false;
	
	
	
	//------------------
	RenderWindow window(VideoMode(window_width , window_height ), "!!CHESS!!" , Style::Resize);
	window.setFramerateLimit(60);
	
	
	Event ev;
	//game loop
	while(window.isOpen()){
	
		//check dettection!!! both king all the time
		whiteInCheck = false;
		blackInCheck = false;
		check_det(true,  board, whiteInCheck);
		check_det(false, board, blackInCheck);
		
		
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
		
		// King vs King draw check
		isDraw = is_king_vs_king(board);
		
		window.clear();
		//background
		window.draw(bgSprite);
		
		//calling functions
		display_board(window, board , checkCapture, whiteInCheck, blackInCheck);
		
		// -------- DRAW OVERLAY --------
		if(isDraw){
			// Dark semi-transparent overlay
			RectangleShape overlay(Vector2f(window_width, window_height));
			overlay.setFillColor(Color(0, 0, 0, 170));
			window.draw(overlay);
			
			// "DRAW" banner box
			RectangleShape banner(Vector2f(560, 180));
			banner.setOrigin(280, 90);
			banner.setPosition(window_width / 2.f, window_height / 2.f - 40);
			banner.setFillColor(Color(30, 30, 30, 230));
			banner.setOutlineColor(Color(200, 200, 200, 255));
			banner.setOutlineThickness(3.f);
			window.draw(banner);
			
			// "DRAW" text
			static Font drawFont;
			static bool fontLoaded = false;
			if(!fontLoaded){
				// Try common system fonts; fall back gracefully if none found
				if(!drawFont.loadFromFile("C:/Windows/Fonts/arialbd.ttf"))
					drawFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
				fontLoaded = true;
			}
			
			Text drawText;
			drawText.setFont(drawFont);
			drawText.setString("DRAW");
			drawText.setCharacterSize(100);
			drawText.setFillColor(Color(220, 220, 220, 255));
			drawText.setStyle(Text::Bold);
			FloatRect tb = drawText.getLocalBounds();
			drawText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
			drawText.setPosition(window_width / 2.f, window_height / 2.f - 55);
			window.draw(drawText);
			
			// subtitle
			Text subText;
			subText.setFont(drawFont);
			subText.setString("King vs King  -  Insufficient Material");
			subText.setCharacterSize(26);
			subText.setFillColor(Color(180, 180, 180, 210));
			FloatRect sb = subText.getLocalBounds();
			subText.setOrigin(sb.left + sb.width/2.f, sb.top + sb.height/2.f);
			subText.setPosition(window_width / 2.f, window_height / 2.f + 55);
			window.draw(subText);
		}
		
		window.display();
	}
	
return 0;
}
