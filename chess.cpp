#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace  sf;

//-------- SELECTION & LAST MOVE HIGHLIGHT TRACKING --------
//these track which tile is currently selected and what the last move was
static int  selectedRow = -1, selectedCol = -1; //tile of the piece the player clicked
static int  lastMoveFromRow = -1, lastMoveFromCol = -1; //where the last piece came from
static int  lastMoveToRow   = -1, lastMoveToCol   = -1; //where the last piece went to

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
	float radius = tileSize * 0.15f;

	moveCircle.setRadius(radius);
	moveCircle.setOrigin(radius, radius);

	// semi transparent dark gray
	moveCircle.setFillColor(Color(0, 0, 0, 80));
	
	
	//Donut ring for capture display (outer circle minus inner = ring)
	float capRadius = tileSize * 0.48f;
	CircleShape capRing(capRadius, 40);
	capRing.setFillColor(Color::Transparent);
	capRing.setOutlineColor(Color(0, 0, 0, 80));
	capRing.setOutlineThickness(-tileSize * 0.12f);
			
	
	//SPRITES FOR WHITE & BLACK PIECES (load textures only once)
	static Texture WpawnTex, WrookTex, WknightTex, WbishopTex, WqueenTex, WkingTex;
	static Texture BpawnTex, BrookTex, BknightTex, BbishopTex, BqueenTex, BkingTex;
	static Sprite WpawnSprite, WrookSprite, WknightSprite, WbishopSprite, WqueenSprite, WkingSprite;
	static Sprite BpawnSprite, BrookSprite, BknightSprite, BbishopSprite, BqueenSprite, BkingSprite;
	static bool piecesTexLoaded = false;
	if(!piecesTexLoaded){
		WpawnTex.loadFromFile("assets/White/pawn.png");
		WrookTex.loadFromFile("assets/White/rook.png");
		WknightTex.loadFromFile("assets/White/knight.png");
		WbishopTex.loadFromFile("assets/White/bishop.png");
		WqueenTex.loadFromFile("assets/White/queen.png");
		WkingTex.loadFromFile("assets/White/king.png");
		BpawnTex.loadFromFile("assets/Black/pawn.png");
		BrookTex.loadFromFile("assets/Black/rook.png");
		BknightTex.loadFromFile("assets/Black/knight.png");
		BbishopTex.loadFromFile("assets/Black/bishop.png");
		BqueenTex.loadFromFile("assets/Black/queen.png");
		BkingTex.loadFromFile("assets/Black/king.png");
		WpawnSprite.setTexture(WpawnTex);     WpawnSprite.setScale(2,2);
		WrookSprite.setTexture(WrookTex);     WrookSprite.setScale(2,2);
		WknightSprite.setTexture(WknightTex); WknightSprite.setScale(2,2);
		WbishopSprite.setTexture(WbishopTex); WbishopSprite.setScale(2,2);
		WqueenSprite.setTexture(WqueenTex);   WqueenSprite.setScale(2,2);
		WkingSprite.setTexture(WkingTex);     WkingSprite.setScale(2,2);
		BpawnSprite.setTexture(BpawnTex);     BpawnSprite.setScale(2,2);
		BrookSprite.setTexture(BrookTex);     BrookSprite.setScale(2,2);
		BknightSprite.setTexture(BknightTex); BknightSprite.setScale(2,2);
		BbishopSprite.setTexture(BbishopTex); BbishopSprite.setScale(2,2);
		BqueenSprite.setTexture(BqueenTex);   BqueenSprite.setScale(2,2);
		BkingSprite.setTexture(BkingTex);     BkingSprite.setScale(2,2);
		piecesTexLoaded = true;
	}
	
	
	// HIGHLIGHT on last move from/to tiles (soft green like lichess)
	RectangleShape lastMoveHL(Vector2f(tileSize, tileSize));
	lastMoveHL.setFillColor(Color(155, 199, 0, 105));
	
	if(lastMoveFromRow >= 0 && lastMoveFromCol >= 0){
		lastMoveHL.setPosition(boarder_width + lastMoveFromCol * tileSize, boarder_height + lastMoveFromRow * tileSize);
		window.draw(lastMoveHL);
	}
	if(lastMoveToRow >= 0 && lastMoveToCol >= 0){
		lastMoveHL.setPosition(boarder_width + lastMoveToCol * tileSize, boarder_height + lastMoveToRow * tileSize);
		window.draw(lastMoveHL);
	}
	
	// HIGHLIGHT on selected piece tile (soft blue)
	if(selectedRow >= 0 && selectedCol >= 0){
		RectangleShape selHL(Vector2f(tileSize, tileSize));
		selHL.setFillColor(Color(100, 150, 255, 100));
		selHL.setPosition(boarder_width + selectedCol * tileSize, boarder_height + selectedRow * tileSize);
		window.draw(selHL);
	}
	
	// RED HIGHLIGHT on king tile when in check
	RectangleShape checkHighlight(Vector2f(tileSize, tileSize));
	checkHighlight.setFillColor(Color(220, 50, 50, 140));
	checkHighlight.setOutlineColor(Color(255, 0, 0, 255));
	checkHighlight.setOutlineThickness(4.0f);

	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			char p = board[row][col];
			if( (wCheck && p == 'K') || (bCheck && p == 'k') ){
				float px = boarder_width + col * tileSize;
				float py = boarder_height + row * tileSize;
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
			
			
			float capCX = boarder_width + col * tileSize + tileSize/2.0f;
			float capCY = boarder_height + row * tileSize + tileSize/2.0f;

			capRing.setOrigin(capRadius, capRadius);
			capRing.setPosition(capCX , capCY);
			if(checkCapture[row][col])
				window.draw(capRing);
			
						
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
		if(kingX + i < width){
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

bool is_pinned(bool turn, char** board, int pieceY, int pieceX){
	
	char saved = board[pieceY][pieceX];
	board[pieceY][pieceX] = ' ';
	
	bool kingInCheck = false;
	check_det(turn, board, kingInCheck);
	
	board[pieceY][pieceX] = saved;
	
	return kingInCheck;
}

//-------- PAWN PROMOTION STATE --------
//these track whether a promotion choice is pending
static bool promotionPending = false;
static int  promotionRow = -1;
static int  promotionCol = -1;
static bool promotionTurn = true; //which side is promoting

//draws the promotion selection popup on screen
void draw_promotion_menu(RenderWindow& window, bool turn){
	
	float tileSize = (window_width - 2 * boarder_width) / width;
	
	//dark overlay behind the popup
	RectangleShape overlay(Vector2f(window_width, window_height));
	overlay.setFillColor(Color(0, 0, 0, 140));
	window.draw(overlay);
	
	//popup background box (4 tiles wide, 1 tile tall)
	float boxW = tileSize * 4 + 40;
	float boxH = tileSize + 40;
	RectangleShape popupBg(Vector2f(boxW, boxH));
	popupBg.setOrigin(boxW / 2.f, boxH / 2.f);
	popupBg.setPosition(window_width / 2.f, window_height / 2.f);
	popupBg.setFillColor(Color(40, 40, 40, 240));
	popupBg.setOutlineColor(Color(200, 200, 200, 255));
	popupBg.setOutlineThickness(3.f);
	window.draw(popupBg);
	
	//the 4 piece options: Queen, Rook, Bishop, Knight
	//load textures (static so only once)
	static Texture promoWQ, promoWR, promoWB, promoWN;
	static Texture promoBQ, promoBR, promoBB, promoBN;
	static bool promoTexLoaded = false;
	if(!promoTexLoaded){
		promoWQ.loadFromFile("assets/White/queen.png");
		promoWR.loadFromFile("assets/White/rook.png");
		promoWB.loadFromFile("assets/White/bishop.png");
		promoWN.loadFromFile("assets/White/knight.png");
		promoBQ.loadFromFile("assets/Black/queen.png");
		promoBR.loadFromFile("assets/Black/rook.png");
		promoBB.loadFromFile("assets/Black/bishop.png");
		promoBN.loadFromFile("assets/Black/knight.png");
		promoTexLoaded = true;
	}
	
	Sprite pieces[4];
	if(turn == 1){
		pieces[0].setTexture(promoWQ);
		pieces[1].setTexture(promoWR);
		pieces[2].setTexture(promoWB);
		pieces[3].setTexture(promoWN);
	}
	else{
		pieces[0].setTexture(promoBQ);
		pieces[1].setTexture(promoBR);
		pieces[2].setTexture(promoBB);
		pieces[3].setTexture(promoBN);
	}
	
	float startX = window_width / 2.f - (tileSize * 4) / 2.f;
	float startY = window_height / 2.f - tileSize / 2.f;
	
	for(int i = 0; i < 4; i++){
		//tile background for each piece option
		RectangleShape tileBg(Vector2f(tileSize, tileSize));
		tileBg.setPosition(startX + i * tileSize, startY);
		if(i % 2 == 0)
			tileBg.setFillColor(Color(180, 140, 100, 255));
		else
			tileBg.setFillColor(Color(240, 220, 180, 255));
		tileBg.setOutlineColor(Color(100, 100, 100, 200));
		tileBg.setOutlineThickness(2.f);
		window.draw(tileBg);
		
		pieces[i].setScale(2, 2);
		pieces[i].setPosition(startX + i * tileSize + 15, startY + 33);
		window.draw(pieces[i]);
	}
}

//handles the click during promotion: returns the chosen piece char, or ' ' if no valid click
char handle_promotion_click(int pixelX, int pixelY, bool turn){
	
	float tileSize = (window_width - 2 * boarder_width) / width;
	
	float startX = window_width / 2.f - (tileSize * 4) / 2.f;
	float startY = window_height / 2.f - tileSize / 2.f;
	
	//check if click is inside the popup area
	if(pixelY < startY || pixelY > startY + tileSize) return ' ';
	if(pixelX < startX || pixelX > startX + tileSize * 4) return ' ';
	
	int slot = (int)((pixelX - startX) / tileSize);
	if(slot < 0 || slot > 3) return ' ';
	
	//slot 0=Queen, 1=Rook, 2=Bishop, 3=Knight
	if(turn == 1){
		if(slot == 0) return 'Q';
		else if(slot == 1) return 'R';
		else if(slot == 2) return 'B';
		else if(slot == 3) return 'N';
	}
	else{
		if(slot == 0) return 'q';
		else if(slot == 1) return 'r';
		else if(slot == 2) return 'b';
		else if(slot == 3) return 'n';
	}
	
	return ' ';
}

//helper: returns true if the char is a white piece (uppercase)
bool isWhitePiece(char c){
	if(c == 'P' || c == 'R' || c == 'N' || c == 'B' || c == 'Q' || c == 'K') return true;
	return false;
}

//helper: returns true if the char is a black piece (lowercase)
bool isBlackPiece(char c){
	if(c == 'p' || c == 'r' || c == 'n' || c == 'b' || c == 'q' || c == 'k') return true;
	return false;
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
		
		//-------- LAST MOVE HIGHLIGHT update --------
		//store where the piece came from and where it went
		//these get rotated with the board below, so store the rotated coords
		lastMoveFromRow = height - 1 - last_mouseY;
		lastMoveFromCol = width  - 1 - last_mouseX;
		lastMoveToRow   = height - 1 - mouseY;
		lastMoveToCol   = width  - 1 - mouseX;
		
		//clear selection since the move is done
		selectedRow = -1;
		selectedCol = -1;
		
		mouseClicked = false;
		//deleting old valid move
		for(int i=0 ; i<height ; i++){
			for(int j=0 ; j<width ; j++){
				if(board[i][j] == 'O') board[i][j] = ' ';
				checkCapture[i][j] = false;
			}
		}
		
		//-------- PAWN PROMOTION CHECK --------
		//if a pawn just landed on row 0, it reached the end -> promotion needed!
		if( (last_clicked_piece == 'P' || last_clicked_piece == 'p') && mouseY == 0){
			promotionPending = true;
			promotionRow = mouseY;
			promotionCol = mouseX;
			promotionTurn = turn;
			//dont rotate or switch turn yet, wait for player to pick a piece
			return;
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
	
	//clear selection highlight (moving to new piece or clicking empty)
	selectedRow = -1;
	selectedCol = -1;
	
	//deleting old valid move and capture
	for(int i=0 ; i<height ; i++){
		for(int j=0 ; j<width ; j++){
			if(board[i][j] == 'O') board[i][j] = ' ';
			checkCapture[i][j] = false;
		}
	}
	
	if(turn == 1){
		// pawn
		if (board[mouseY][mouseX] == 'P') {
			if(is_pinned(true, board, mouseY, mouseX)) return;
			last_clicked_piece = 'P';
			if( mouseY-1 >= 0 && board[mouseY-1][mouseX] == ' '){
				board[mouseY-1][mouseX] = 'O';
				if(mouseY == 6 && board[mouseY-2][mouseX] == ' ' )
					board[mouseY-2][mouseX] = 'O';
			}
			
			if (mouseY-1 >= 0 && mouseX-1 >= 0) {
				if(isBlackPiece(board[mouseY-1][mouseX-1]))
					checkCapture[mouseY-1][mouseX-1] = true;
			}
			
			if (mouseY-1 >= 0 && mouseX+1 < width) {
				if(isBlackPiece(board[mouseY-1][mouseX+1]))
					checkCapture[mouseY-1][mouseX+1] = true;
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
			if(is_pinned(true, board, mouseY, mouseX)) return;
		last_clicked_piece = 'R';
			
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isBlackPiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isBlackPiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isBlackPiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isBlackPiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
				
		}

		// knight
		if (board[mouseY][mouseX] == 'N') {
			if(is_pinned(true, board, mouseY, mouseX)) return;
			last_clicked_piece = 'N';
			
			if(mouseX-1 >= 0  &&  mouseY+2 < height){
				if(board[mouseY+2][mouseX-1] == ' ')
					board[mouseY+2][mouseX-1] = 'O';
				else if(isBlackPiece(board[mouseY+2][mouseX-1]))
					checkCapture[mouseY+2][mouseX-1] = true;
			}
		
			if(mouseX+1 < width  &&  mouseY+2 < height){
				if(board[mouseY+2][mouseX+1] == ' ')
					board[mouseY+2][mouseX+1] = 'O';
				else if(isBlackPiece(board[mouseY+2][mouseX+1]))
					checkCapture[mouseY+2][mouseX+1] = true;
			}
			
			if(mouseY-1 >= 0  &&  mouseX+2 < width){
				if(board[mouseY-1][mouseX+2] == ' ')
					board[mouseY-1][mouseX+2] = 'O';
				else if(isBlackPiece(board[mouseY-1][mouseX+2]))
					checkCapture[mouseY-1][mouseX+2] = true;
			}
			
			if(mouseY+1 < height  &&  mouseX+2 < width){
				if(board[mouseY+1][mouseX+2] == ' ')
					board[mouseY+1][mouseX+2] = 'O';
				else if(isBlackPiece(board[mouseY+1][mouseX+2]))
					checkCapture[mouseY+1][mouseX+2] = true;
			}
			
			if(mouseY-2 >= 0  &&  mouseX-1 >= 0){
				if(board[mouseY-2][mouseX-1] == ' ')
					board[mouseY-2][mouseX-1] = 'O';
				else if(isBlackPiece(board[mouseY-2][mouseX-1]))
					checkCapture[mouseY-2][mouseX-1] = true;
			}
			
			if(mouseY-2 >= 0  &&  mouseX+1 < width){
				if(board[mouseY-2][mouseX+1] == ' ')
					board[mouseY-2][mouseX+1] = 'O';
				else if(isBlackPiece(board[mouseY-2][mouseX+1]))
					checkCapture[mouseY-2][mouseX+1] = true;
			}
			
			if(mouseY-1 >= 0  &&  mouseX-2 >= 0){
				if(board[mouseY-1][mouseX-2] == ' ')
					board[mouseY-1][mouseX-2] = 'O';
				else if(isBlackPiece(board[mouseY-1][mouseX-2]))
					checkCapture[mouseY-1][mouseX-2] = true;
			}
			
			if(mouseY+1 < height  &&  mouseX-2 >= 0){
				if(board[mouseY+1][mouseX-2] == ' ')
					board[mouseY+1][mouseX-2] = 'O';
				else if(isBlackPiece(board[mouseY+1][mouseX-2]))
					checkCapture[mouseY+1][mouseX-2] = true;
			}
		}
		

		// bishop
		if (board[mouseY][mouseX] == 'B') {
			if(is_pinned(true, board, mouseY, mouseX)) return;
		last_clicked_piece = 'B';

			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(isBlackPiece(board[mouseY + i][mouseX + i])){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(isBlackPiece(board[mouseY - i][mouseX + i])){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(isBlackPiece(board[mouseY - i][mouseX - i])){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(isBlackPiece(board[mouseY + i][mouseX - i])){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
							
					else break;
					}
									
				}
	  
		}

		// queen
		if (board[mouseY][mouseX] == 'Q') {
			if(is_pinned(true, board, mouseY, mouseX)) return;
		last_clicked_piece = 'Q';
			
		//all rook moves
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isBlackPiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isBlackPiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isBlackPiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isBlackPiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			
			
		//all bishop movessssss
				for(int i=1 ; i<height ; i++){
					
					if( mouseY+i<height && mouseX+i<width) {
						if(board[mouseY + i][mouseX + i] ==' ')
							board[mouseY + i][mouseX + i] = 'O';
							
						else if(isBlackPiece(board[mouseY + i][mouseX + i])){
							checkCapture[mouseY + i][mouseX + i] = true;
							break;}
								
						else break;
						}
					}
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY-i >= 0 && mouseX+i < width) {
						if(board[mouseY - i][mouseX + i] ==' ')
							board[mouseY - i][mouseX + i] = 'O';
							
						else if(isBlackPiece(board[mouseY - i][mouseX + i])){
							checkCapture[mouseY - i][mouseX + i] = true;
							break;}
								
						else break;
						}
						
					}
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY-i >= 0 && mouseX-i >= 0) {
						if(board[mouseY - i][mouseX - i] ==' ')
							board[mouseY - i][mouseX - i] = 'O';
							
						else if(isBlackPiece(board[mouseY - i][mouseX - i])){
							checkCapture[mouseY - i][mouseX - i] = true;
							break;}
								
						else break;
						}
					
					}	
					
				for(int i=1 ; i<height ; i++){
					
					if( mouseY+i < height && mouseX-i >= 0) {
						if(board[mouseY + i][mouseX - i] ==' ')
							board[mouseY + i][mouseX - i] = 'O';
							
						else if(isBlackPiece(board[mouseY + i][mouseX - i])){
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
		
			//-------- FILTER OUT KING MOVES THAT LAND IN CHECK --------
			//for each of the 8 surrounding squares, simulate the king moving there
			//and check if the king would be in check; if so, remove that move
			{
				int dirs[8][2] = {{0,-1},{0,1},{-1,0},{1,0},{1,-1},{1,1},{-1,-1},{-1,1}};
				for(int d=0; d<8; d++){
					int ty = mouseY + dirs[d][0];
					int tx = mouseX + dirs[d][1];
					if(ty < 0 || ty >= height || tx < 0 || tx >= width) continue;
					
					//only check squares where we placed 'O' or marked as capture
					if(board[ty][tx] == 'O' || checkCapture[ty][tx]){
						//simulate: remove king from old pos, place king at new pos
						char savedDst = board[ty][tx];
						bool wasCap = checkCapture[ty][tx];
						char savedSrc = board[mouseY][mouseX];
						
						board[mouseY][mouseX] = ' ';
						board[ty][tx] = 'K';
						
						bool wouldBeInCheck = false;
						check_det(true, board, wouldBeInCheck);
						
						//undo simulation
						board[mouseY][mouseX] = savedSrc;
						board[ty][tx] = savedDst;
						
						//if moving there puts king in check, remove that move option
						if(wouldBeInCheck){
							if(wasCap){
								checkCapture[ty][tx] = false;
							} else {
								board[ty][tx] = ' '; //remove the 'O'
							}
						}
					}
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
			if(is_pinned(false, board, mouseY, mouseX)) return;
		last_clicked_piece = 'p';
			if( mouseY-1 >= 0 && board[mouseY-1][mouseX] == ' ' ){
				
				board[mouseY-1][mouseX] = 'O';
				if(mouseY == 6 && board[mouseY-2][mouseX] == ' ' )
					board[mouseY-2][mouseX] = 'O';
			}
		
			if (mouseY-1 >= 0 && mouseX-1 >= 0) {
				if(isWhitePiece(board[mouseY-1][mouseX-1]))
					checkCapture[mouseY-1][mouseX-1] = true;
			}
			
			if (mouseY-1 >= 0 && mouseX+1 < width) {
				if(isWhitePiece(board[mouseY-1][mouseX+1]))
					checkCapture[mouseY-1][mouseX+1] = true;
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
			if(is_pinned(false, board, mouseY, mouseX)) return;
		last_clicked_piece = 'r';
			
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isWhitePiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isWhitePiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isWhitePiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isWhitePiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}


		}

		// knight
		if (board[mouseY][mouseX] == 'n') {
			if(is_pinned(false, board, mouseY, mouseX)) return;
			last_clicked_piece = 'n';
		
			if(mouseX-1 >= 0  &&  mouseY+2 < height){
				if(board[mouseY+2][mouseX-1] == ' ')
					board[mouseY+2][mouseX-1] = 'O';
				else if(isWhitePiece(board[mouseY+2][mouseX-1]))
					checkCapture[mouseY+2][mouseX-1] = true;
			}
		
			if(mouseX+1 < width  &&  mouseY+2 < height){
				if(board[mouseY+2][mouseX+1] == ' ')
					board[mouseY+2][mouseX+1] = 'O';
				else if(isWhitePiece(board[mouseY+2][mouseX+1]))
					checkCapture[mouseY+2][mouseX+1] = true;
			}
			
			if(mouseY-1 >= 0  &&  mouseX+2 < width){
				if(board[mouseY-1][mouseX+2] == ' ')
					board[mouseY-1][mouseX+2] = 'O';
				else if(isWhitePiece(board[mouseY-1][mouseX+2]))
					checkCapture[mouseY-1][mouseX+2] = true;
			}
			
			if(mouseY+1 < height  &&  mouseX+2 < width){
				if(board[mouseY+1][mouseX+2] == ' ')
					board[mouseY+1][mouseX+2] = 'O';
				else if(isWhitePiece(board[mouseY+1][mouseX+2]))
					checkCapture[mouseY+1][mouseX+2] = true;
			}
			
			if(mouseY-2 >= 0  &&  mouseX-1 >= 0){
				if(board[mouseY-2][mouseX-1] == ' ')
					board[mouseY-2][mouseX-1] = 'O';
				else if(isWhitePiece(board[mouseY-2][mouseX-1]))
					checkCapture[mouseY-2][mouseX-1] = true;
			}
			
			if(mouseY-2 >= 0  &&  mouseX+1 < width){
				if(board[mouseY-2][mouseX+1] == ' ')
					board[mouseY-2][mouseX+1] = 'O';
				else if(isWhitePiece(board[mouseY-2][mouseX+1]))
					checkCapture[mouseY-2][mouseX+1] = true;
			}
			
			if(mouseY-1 >= 0  &&  mouseX-2 >= 0){
				if(board[mouseY-1][mouseX-2] == ' ')
					board[mouseY-1][mouseX-2] = 'O';
				else if(isWhitePiece(board[mouseY-1][mouseX-2]))
					checkCapture[mouseY-1][mouseX-2] = true;
			}
			
			if(mouseY+1 < height  &&  mouseX-2 >= 0){
				if(board[mouseY+1][mouseX-2] == ' ')
					board[mouseY+1][mouseX-2] = 'O';
				else if(isWhitePiece(board[mouseY+1][mouseX-2]))
					checkCapture[mouseY+1][mouseX-2] = true;
			}

		}

		// bishop
		if (board[mouseY][mouseX] == 'b') {
			if(is_pinned(false, board, mouseY, mouseX)) return;
		last_clicked_piece = 'b';
			
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(isWhitePiece(board[mouseY + i][mouseX + i])){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(isWhitePiece(board[mouseY - i][mouseX + i])){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(isWhitePiece(board[mouseY - i][mouseX - i])){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(isWhitePiece(board[mouseY + i][mouseX - i])){
						checkCapture[mouseY + i][mouseX - i] = true;
						break;}
							
					else break;
					}
									
				}
	  
		}

		// queen
		if (board[mouseY][mouseX] == 'q') {
			if(is_pinned(false, board, mouseY, mouseX)) return;
		last_clicked_piece = 'q';
		
		//all rook moves
			//moves after that peice
			for(int i=mouseX+1 ; i<width ; i++){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isWhitePiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseX-1 ; i>=0 ; i--){
				
				if(board[mouseY][i] == ' ')
					board[mouseY][i] = 'O';
				else if(isWhitePiece(board[mouseY][i])){
					checkCapture[mouseY][i] = true;
					break;}
				else break;
				
				}
			
			//moves after that peice
			for(int i =mouseY+1 ; i<height ;  i++){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isWhitePiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}
			
			//moves before that
			for(int i=mouseY-1 ; i>=0 ;  i--){
				
				if(board[i][mouseX] == ' ')
					board[i][mouseX] = 'O';
				else if(isWhitePiece(board[i][mouseX])){
					checkCapture[i][mouseX] = true;
					break;}
				else break;
				
				}


		
			
			
		//all bishop movessssss
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i<height && mouseX+i<width) {
					if(board[mouseY + i][mouseX + i] ==' ')
						board[mouseY + i][mouseX + i] = 'O';
						
					else if(isWhitePiece(board[mouseY + i][mouseX + i])){
						checkCapture[mouseY + i][mouseX + i] = true;
						break;}
							
					else break;
					}
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX+i < width) {
					if(board[mouseY - i][mouseX + i] ==' ')
						board[mouseY - i][mouseX + i] = 'O';
						
					else if(isWhitePiece(board[mouseY - i][mouseX + i])){
						checkCapture[mouseY - i][mouseX + i] = true;
						break;}
							
					else break;
					}
					
				}
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY-i >= 0 && mouseX-i >= 0) {
					if(board[mouseY - i][mouseX - i] ==' ')
						board[mouseY - i][mouseX - i] = 'O';
						
					else if(isWhitePiece(board[mouseY - i][mouseX - i])){
						checkCapture[mouseY - i][mouseX - i] = true;
						break;}
							
					else break;
					}
				
				}	
				
			for(int i=1 ; i<height ; i++){
				
				if( mouseY+i < height && mouseX-i >= 0) {
					if(board[mouseY + i][mouseX - i] ==' ')
						board[mouseY + i][mouseX - i] = 'O';
						
					else if(isWhitePiece(board[mouseY + i][mouseX - i])){
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

			//-------- FILTER OUT KING MOVES THAT LAND IN CHECK --------
			//for each of the 8 surrounding squares, simulate the king moving there
			//and check if the king would be in check; if so, remove that move
			{
				int dirs[8][2] = {{0,-1},{0,1},{-1,0},{1,0},{1,-1},{1,1},{-1,-1},{-1,1}};
				for(int d=0; d<8; d++){
					int ty = mouseY + dirs[d][0];
					int tx = mouseX + dirs[d][1];
					if(ty < 0 || ty >= height || tx < 0 || tx >= width) continue;
					
					//only check squares where we placed 'O' or marked as capture
					if(board[ty][tx] == 'O' || checkCapture[ty][tx]){
						//simulate: remove king from old pos, place king at new pos
						char savedDst = board[ty][tx];
						bool wasCap = checkCapture[ty][tx];
						char savedSrc = board[mouseY][mouseX];
						
						board[mouseY][mouseX] = ' ';
						board[ty][tx] = 'k';
						
						bool wouldBeInCheck = false;
						check_det(false, board, wouldBeInCheck);
						
						//undo simulation
						board[mouseY][mouseX] = savedSrc;
						board[ty][tx] = savedDst;
						
						//if moving there puts king in check, remove that move option
						if(wouldBeInCheck){
							if(wasCap){
								checkCapture[ty][tx] = false;
							} else {
								board[ty][tx] = ' '; //remove the 'O'
							}
						}
					}
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
	
	//-------- SELECTION HIGHLIGHT update --------
	selectedRow = mouseY;
	selectedCol = mouseX;
	
	
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
					
					//-------- PROMOTION CLICK HANDLING --------
					//if promotion is pending, handle the popup click instead of normal move
					if(promotionPending){
						char chosen = handle_promotion_click(ev.mouseButton.x, ev.mouseButton.y, promotionTurn);
						if(chosen != ' '){
							//replace the pawn with chosen piece
							board[promotionRow][promotionCol] = chosen;
							promotionPending = false;
							
							//now do the turn switch and rotation that was delayed
							turn = !turn;
							
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
						}
					}
					else{
						
					x= (ev.mouseButton.x - boarder_width) / ((window_width - 2*boarder_width) / width);
					y= (ev.mouseButton.y - boarder_height) / ((window_height - 2*boarder_height) / height);
					
					if(!isDraw)
					clicked( window, board, x , y , mouseClicked , turn , checkCapture);
					
					}
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
		
		//-------- TURN INDICATOR --------
		//show whose turn it is above the board
		{
			static Font turnFont;
			static bool turnFontLoaded = false;
			if(!turnFontLoaded){
				if(!turnFont.loadFromFile("C:/Windows/Fonts/arialbd.ttf"))
					turnFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
				turnFontLoaded = true;
			}
			
			Text turnText;
			turnText.setFont(turnFont);
			turnText.setString(turn ? "White's Turn" : "Black's Turn");
			turnText.setCharacterSize(22);
			turnText.setFillColor(Color(230, 230, 230, 220));
			turnText.setStyle(Text::Bold);
			FloatRect tb = turnText.getLocalBounds();
			turnText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
			turnText.setPosition(window_width / 2.f, boarder_height / 2.f);
			window.draw(turnText);
		}
		
		//-------- PROMOTION POPUP --------
		//draw the piece selection menu when promotion is pending
		if(promotionPending){
			draw_promotion_menu(window, promotionTurn);
		}
		
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
