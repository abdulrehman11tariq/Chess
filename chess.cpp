// integrating an already made AI engine to evaluate the game(with an option of suggesting moves)
// sounds and haptics, also an option to change sounds and haptics into memes
 
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>


using namespace std;
using namespace  sf;


static bool ROTATE_BOARD = true;

static bool musicEnabled = true;	
static bool soundsEnabled = true;   
static bool undoEnabled   = true;   

static bool pendingMoveSound = false;
static bool pendingCaptureSound = false;

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
	isCheck = false;
	
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
	//with ROTATE_BOARD on, both sides always face each other from the top (kingY-1)
	//with ROTATE_BOARD off, white pawns attack upward (row-1) and black pawns attack downward (row+1)
	//when turn==true we check for enemy (black) pawns; when turn==false we check for enemy (white) pawns
	{
		int pawnDir;
		if(ROTATE_BOARD){
			pawnDir = -1; //enemy pawns always above with rotation
		} else {
			//turn==true means checking white king for black pawn attacks (black pawns move down, attack from kingY-1 perspective... no)
			//black pawns are at the top and attack downward, so they threaten squares BELOW them = kingY-1 means pawn at kingY-1 attacks kingY
			//wait: a pawn at row R attacks row R+dir. White pawns attack upward (R-1). Black pawns without rotation attack downward (R+1).
			//so an enemy pawn threatens the king if the pawn is at kingY+dir where dir is the pawn's attack direction inverted.
			//White checking for black pawns: black pawns attack DOWN (row+1), so pawn at kingY-1 attacks king. dir = -1
			//Black checking for white pawns: white pawns attack UP (row-1), so pawn at kingY+1 attacks king. dir = +1
			pawnDir = turn ? -1 : 1;
		}
		int pawnRow = kingY + pawnDir;
		if(pawnRow >= 0 && pawnRow < height && kingX+1 < width){
			if(board[pawnRow][kingX+1] == 'P' + pcsCorrection){
				isCheck = true;
				return;
			}
		}
		
		if(pawnRow >= 0 && pawnRow < height && kingX-1 >= 0){
			if(board[pawnRow][kingX-1] == 'P' + pcsCorrection){
				isCheck = true;
				return;
			}
		}
	}
	
	//bishop
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height && kingX + i < width ){
			if(board[kingY+i][kingX+i] == 'B' + pcsCorrection || board[kingY+i][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX+i] != ' ' && board[kingY+i][kingX+i] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height && kingX - i >= 0 ){
			if(board[kingY+i][kingX-i] == 'B' + pcsCorrection || board[kingY+i][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX-i] != ' ' && board[kingY+i][kingX-i] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0 && kingX + i < width ){
			if(board[kingY-i][kingX+i] == 'B' + pcsCorrection || board[kingY-i][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX+i] != ' ' && board[kingY-i][kingX+i] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0 && kingX - i >= 0 ){
			if(board[kingY-i][kingX-i] == 'B' + pcsCorrection || board[kingY-i][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX-i] != ' ' && board[kingY-i][kingX-i] != 'O') break;
		}
	}
	
	//rook
	for(int i=1 ; i<height ; i++){
		if(kingY + i < height){
			if(board[kingY+i][kingX] == 'R' + pcsCorrection || board[kingY+i][kingX] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY+i][kingX] != ' ' && board[kingY+i][kingX] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingY - i >= 0){
			if(board[kingY-i][kingX] == 'R' + pcsCorrection || board[kingY-i][kingX] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY-i][kingX] != ' ' && board[kingY-i][kingX] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingX + i < width){
			if(board[kingY][kingX+i] == 'R' + pcsCorrection || board[kingY][kingX+i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY][kingX+i] != ' ' && board[kingY][kingX+i] != 'O') break;
		}
	}
	
	for(int i=1 ; i<height ; i++){
		if(kingX - i >= 0){
			if(board[kingY][kingX-i] == 'R' + pcsCorrection || board[kingY][kingX-i] == 'Q' + pcsCorrection){
				isCheck = true;
				return;
			}
			
			else if(board[kingY][kingX-i] != ' ' && board[kingY][kingX-i] != 'O') break;
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

//-------- SQUARE ATTACK CHECK (for castling) --------
//temporarily places a king of the given side on (row,col) and checks if
//that square is under attack. Restores the board afterward.
bool is_square_attacked(bool side, char** board, int row, int col){
	char saved = board[row][col];
	board[row][col] = side ? 'K' : 'k';
	
	bool attacked = false;
	check_det(side, board, attacked);
	
	board[row][col] = saved;
	return attacked;
}

// absolute pins
//  single check 
//  double check 
// discovered checks
void filter_illegal_moves(bool turn, char** board, bool checkCapture[8][8], int pieceY, int pieceX){
	char piece = board[pieceY][pieceX];
	
	for(int r = 0; r < height; r++){
		for(int c = 0; c < width; c++){
			if(board[r][c] != 'O' && !checkCapture[r][c]) continue;
			
			//save state
			char savedDst = board[r][c];
			bool savedCap = checkCapture[r][c];
			
			//simulate the move
			board[r][c] = piece;
			board[pieceY][pieceX] = ' ';
			
			//EN PASSANTTTTT: pawn moved diagonally to an empty 'O' square
			//the captured pawn sits at (pieceY, c) - same row as source, dest column
			char savedEP = ' ';
			bool isEP = false;
			if((piece == 'P' || piece == 'p') && c != pieceX && savedDst == 'O'){
				isEP = true;
				savedEP = board[pieceY][c];
				board[pieceY][c] = ' ';
			}
			
			//check if own king is still in check
			bool stillInCheck = false;
			check_det(turn, board, stillInCheck);
			
			//undo simulation
			board[pieceY][pieceX] = piece;
			board[r][c] = savedDst;
			if(isEP) board[pieceY][c] = savedEP;
			
			//remove move if king is still in check
			if(stillInCheck){
				if(savedCap)
					checkCapture[r][c] = false;
				else
					board[r][c] = ' ';
			}
		}
	}
}

//-------- PAWN PROMOTION STATE --------
static bool promotionPending = false;
static int  promotionRow = -1;
static int  promotionCol = -1;
static bool promotionTurn = true; //which side is promoting	B/W

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

//-------- EN PASSANT TRACKING (file-scope) --------
static bool ep_active = false;
static int  ep_col    = -1;

//-------- CASTLING TRACKING (file-scope) --------
static bool wKingMoved  = false;
static bool wRookKMoved = false;
static bool wRookQMoved = false;
static bool bKingMoved  = false;
static bool bRookKMoved = false;
static bool bRookQMoved = false;

//-------- FORWARD DECLARATION (needed for notation) --------
bool has_any_legal_move(bool turn, char** board);

//-------- NOTATION TRACKING (file-scope) --------
//stores all move notations in order: index 0 = white move 1, index 1 = black move 1, etc.
static vector<string> notationLog;
static int moveNumber = 1; //increments after every black move (full move counter)

//helper: converts a board column (0-7) to a file letter (a-h)
//when the board is rotated, the absolute column is flipped
char colToFile(int col, bool isRotated){
	int absCol = isRotated ? (7 - col) : col;
	return 'a' + absCol;
}

//helper: converts a board row (0-7) to a rank number (1-8)
//row 0 is rank 8 normally, but when rotated we flip
int rowToRank(int row, bool isRotated){
	int absRow = isRotated ? (7 - row) : row;
	return 8 - absRow;
}

//helper: returns the piece letter for notation (K, Q, R, B, N)
//pawns return empty string
string pieceToLetter(char piece){
	char upper = piece;
	if(piece >= 'a' && piece <= 'z') upper = piece - 32; //to uppercase
	
	if(upper == 'K') return "K";
	else if(upper == 'Q') return "Q";
	else if(upper == 'R') return "R";
	else if(upper == 'B') return "B";
	else if(upper == 'N') return "N";
	return ""; //pawn has no letter
}

//builds the notation string for a move
//called BEFORE the board is rotated, right after the move is executed on the board
//destPiece is what was on the destination before the move (for capture detection)
string build_notation(char piece, int fromRow, int fromCol, int toRow, int toCol,
					   char destPiece, bool isEP, bool isCastleK, bool isCastleQ,
					   bool isPromo, char promoChoice, bool turn, char** board){
	
	//castling notation (still need to check for +/# at the end)
	string notation = "";
	if(isCastleK){ notation = "O-O"; }
	else if(isCastleQ){ notation = "O-O-O"; }
	else{
		//when ROTATE_BOARD is on, white plays on standard board, black plays on flipped board
		//so we only flip coords when its blacks turn with rotation enabled
		bool isFlipped = ROTATE_BOARD && !turn;
		
		//piece letter (pawns have none)
		notation += pieceToLetter(piece);
		
		//for pawns: if capturing, add the source file before 'x'
		bool isCapture = (destPiece != ' ' && destPiece != 'O') || isEP;
		if(notation == "" && isCapture){
			//pawn capture: add source file
			notation += colToFile(fromCol, isFlipped);
		}
		
		//capture symbol
		if(isCapture) notation += "x";
		
		//destination square (file + rank)
		notation += colToFile(toCol, isFlipped);
		notation += to_string(rowToRank(toRow, isFlipped));
		
		//promotion suffix
		if(isPromo){
			notation += "=";
			notation += pieceToLetter(promoChoice);
		}
	}
	
	//check and checkmate detection on the board AFTER the move
	//the move has already been executed, so we check the opponent's king
	//check_det scans the whole board so orientation doesnt matter
	bool oppInCheck = false;
	check_det(!turn, board, oppInCheck);
	
	if(oppInCheck){
		//has_any_legal_move expects the checked side at the bottom
		//since the board hasnt been rotated yet, temporarily flip it
		if(ROTATE_BOARD){
			char tempB[8][8];
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  tempB[r][c] = board[7-r][7-c];
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  board[r][c] = tempB[r][c];
		}
		
		bool oppHasMoves = has_any_legal_move(!turn, board);
		
		//rotate back
		if(ROTATE_BOARD){
			char tempB[8][8];
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  tempB[r][c] = board[7-r][7-c];
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  board[r][c] = tempB[r][c];
		}
		
		if(!oppHasMoves)
			notation += "#";
		else
			notation += "+";
	}
	
	return notation;
}

//-------- UNDO HISTORY (file-scope) --------
struct MoveRecord {
	char boardSnap[8][8];
	bool turnSnap;
	
	bool wKM, wRKM, wRQM;
	bool bKM, bRKM, bRQM;
	
	bool epActive;
	int epCol;
	
	int lmFR, lmFC, lmTR, lmTC;
	int selR, selC;
	int savedMoveNumber; //for undo
};
static vector<MoveRecord> undoHistory;

//new func 
void clicked( RenderWindow& window ,char** board, int mouseX , int mouseY , bool& mouseClicked , bool& turn, bool checkCapture[8][8]){
	if (mouseX < 0 || mouseX >= width || mouseY < 0 || mouseY >= height) return;
	
	static int last_mouseY = 0 , last_mouseX = 0 ;
	static char last_clicked_piece = ' ';
	
	//MOVE
	//checking validity of move and then move it wuhahahahaha!!!!!!!!!!
	if(board[mouseY][mouseX] == 'O' || checkCapture[mouseY][mouseX]){
		
		//-------- SAVE STATE FOR UNDO before executing the move --------
		{
			MoveRecord rec;
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  rec.boardSnap[r][c] = board[r][c];
			
			rec.turnSnap = turn;
			rec.wKM = wKingMoved;  rec.wRKM = wRookKMoved;  rec.wRQM = wRookQMoved;
			rec.bKM = bKingMoved;  rec.bRKM = bRookKMoved;  rec.bRQM = bRookQMoved;
			rec.epActive = ep_active;  rec.epCol = ep_col;
			
			rec.lmFR = lastMoveFromRow;  rec.lmFC = lastMoveFromCol;
			rec.lmTR = lastMoveToRow;    rec.lmTC = lastMoveToCol;
			rec.selR = selectedRow;  rec.selC = selectedCol;
			rec.savedMoveNumber = moveNumber;
			undoHistory.push_back(rec);
		}
		
		//save what was on the destination BEFORE the move (for capture detection in notation)
		char destBeforeMove = board[mouseY][mouseX];
		
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
		
		//-------- ROOK CAPTURE invalidates castling rights --------
		//if a move CAPTURES a rook sitting on its starting square, revoke that side's castling
		//with ROTATE_BOARD, after rotation the opponent's back rank is row 0
		//but at this point the board hasn't rotated yet, so opponent's back rank is row 0
		if(turn == 1){
			//white just moved, may have captured a black rook at row 0
			if(mouseY == 0 && mouseX == 7) bRookKMoved = true;
			if(mouseY == 0 && mouseX == 0) bRookQMoved = true;
		} else {
			//black just moved, may have captured a white rook at row 0
			if(mouseY == 0 && mouseX == 7) wRookKMoved = true;
			if(mouseY == 0 && mouseX == 0) wRookQMoved = true;
		}
		
		//-------- EN PASSANT STATE UPDATE --------
		//check if a pawn just did a double push from row 6 to row 4
		//after rotation the column flips: width-1-mouseX
		if( (last_clicked_piece == 'P' || last_clicked_piece == 'p') && last_mouseY == 6 && mouseY == 4){
			ep_active = true;
			ep_col    = ROTATE_BOARD ? (width - 1 - mouseX) : mouseX;
		}
		else{
			ep_active = false;
			ep_col    = -1;
		}
		
		//-------- LAST MOVE HIGHLIGHT update --------
		//store where the piece came from and where it went
		if(ROTATE_BOARD){
			//these get rotated with the board below, so store the rotated coords
			lastMoveFromRow = height - 1 - last_mouseY;
			lastMoveFromCol = width  - 1 - last_mouseX;
			lastMoveToRow   = height - 1 - mouseY;
			lastMoveToCol   = width  - 1 - mouseX;
		}
		else{
			//no rotation: highlight stays at the actual board coords
			lastMoveFromRow = last_mouseY;
			lastMoveFromCol = last_mouseX;
			lastMoveToRow   = mouseY;
			lastMoveToCol   = mouseX;
		}
		
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
		//with rotation both sides promote at row 0; without rotation white promotes
		//at row 0 and black promotes at row 7
		{
			bool isPromotion = false;
			if(ROTATE_BOARD){
				isPromotion = (last_clicked_piece == 'P' || last_clicked_piece == 'p') && mouseY == 0;
			} else {
				isPromotion = (last_clicked_piece == 'P' && mouseY == 0) || (last_clicked_piece == 'p' && mouseY == 7);
			}
			if(isPromotion){
				promotionPending = true;
				promotionRow = mouseY;
				promotionCol = mouseX;
				promotionTurn = turn;
				//dont rotate or switch turn yet, wait for player to pick a piece
				//notation will be built after the player picks a piece (in main event loop)
				return;
			}
		}
		
		//-------- BUILD NOTATION for this move --------
		{
			//detect castling from the king move
			bool castleK = false, castleQ = false;
			if((last_clicked_piece == 'K' || last_clicked_piece == 'k') && last_mouseX == 4 && last_mouseY == 7){
				if(mouseX == 6 && mouseY == 7) castleK = true;
				if(mouseX == 2 && mouseY == 7) castleQ = true;
			}
			
			string moveNotation = build_notation(
				last_clicked_piece, last_mouseY, last_mouseX, mouseY, mouseX,
				destBeforeMove, isEnPassantCapture, castleK, castleQ,
				false, ' ', turn, board
			);
			
			notationLog.push_back(moveNotation);
			
			//print to console: "1. e4" for white, "1... e5" for black
			if(turn){
				cout << moveNumber << ". " << moveNotation;
			} else {
				cout << "  " << moveNotation << endl;
				moveNumber++;
			}
		}
		
		turn = !turn;
		pendingMoveSound = true;
		//flag capture for sound if destination was occupied
		if(destBeforeMove != ' ' && destBeforeMove != 'O') pendingCaptureSound = true;
		if(isEnPassantCapture) pendingCaptureSound = true;
	
		//Rotating the matrix for next person's turn!!!!! :yum:
		if(ROTATE_BOARD){
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
		
		return;
		
	}
	
	//clear selection highlight (moving to new piece or clicking empty)
	selectedRow = -1;
	selectedCol = -1;
	
	//reset last clicked piece so stale data cant cause issues
	last_clicked_piece = ' ';
	
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
			//king cannot castle out of check, through check, or into check
			if(mouseY == 7 && mouseX == 4 && !wKingMoved && !is_square_attacked(true, board, 7, 4)){
				
				//kingside: col 5 and col 6 must be empty, rook at col 7 must not have moved
				//king passes through col 5 and lands on col 6 - both must not be attacked
				if(!wRookKMoved && board[7][7] == 'R' && board[7][5] == ' ' && board[7][6] == ' '){
					if(!is_square_attacked(true, board, 7, 5) && !is_square_attacked(true, board, 7, 6)){
						board[7][6] = 'O';
					}
				}
				
				//queenside: col 3, col 2, col 1 must be empty, rook at col 0 must not have moved
				//king passes through col 3 and lands on col 2 - both must not be attacked
				if(!wRookQMoved && board[7][0] == 'R' && board[7][3] == ' ' && board[7][2] == ' ' && board[7][1] == ' '){
					if(!is_square_attacked(true, board, 7, 3) && !is_square_attacked(true, board, 7, 2)){
						board[7][2] = 'O';
					}
				}
			}
		
		}
	}

	// ------------------------------------------------------------------------------------ KAALA ---------------------------------------------------------------
	if(turn == 0){
		// pawn
		if (board[mouseY][mouseX] == 'p') {
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
			//king cannot castle out of check, through check, or into check
			if(mouseY == 7 && mouseX == 4 && !bKingMoved && !is_square_attacked(false, board, 7, 4)){
				
				//kingside
				if(!bRookKMoved && board[7][7] == 'r' && board[7][5] == ' ' && board[7][6] == ' '){
					if(!is_square_attacked(false, board, 7, 5) && !is_square_attacked(false, board, 7, 6)){
						board[7][6] = 'O';
					}
				}
				
				//queenside
				if(!bRookQMoved && board[7][0] == 'r' && board[7][3] == ' ' && board[7][2] == ' ' && board[7][1] == ' '){
					if(!is_square_attacked(false, board, 7, 3) && !is_square_attacked(false, board, 7, 2)){
						board[7][2] = 'O';
					}
				}
			}

		}
	}
	
	//-------- FILTER ILLEGAL MOVES for non-king pieces --------
	//simulate every generated move; remove any that leave own king in check
	{
		char pc = board[mouseY][mouseX];
		if(pc != 'K' && pc != 'k' && pc != ' ' && pc != 'O'){
			filter_illegal_moves(turn, board, checkCapture, mouseY, mouseX);
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

//-------- GAME OVER STATE --------
static bool gameOver = false;
static string gameOverMsg = "";
static string gameOverSub = "";

//-------- HAS ANY LEGAL MOVE --------
//checks if the side whose turn it is (turn=true->white, turn=false->black)
//has at least one legal move on the board. Works by copying the board,
//generating moves for each friendly piece, filtering illegal ones via
//simulation, and returning true as soon as one legal move is found.
bool has_any_legal_move(bool turn, char** board){
	
	//make a working copy so move generation markers dont pollute the real board
	char copy[8][8];
	for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  copy[r][c] = board[r][c];
	
	//also need a char** wrapper for the copy so existing funcs work
	char* rows[8];
	for(int r=0 ; r<8 ; r++)  rows[r] = copy[r];
	char** bd = rows;
	
	
	for(int pr=0 ; pr<8 ; pr++){
		for(int pc=0 ; pc<8 ; pc++){
			
			char piece = bd[pr][pc];
			if(piece == ' ' || piece == 'O') continue;
			
			//skip pieces that dont belong to current side
			if(turn  && !isWhitePiece(piece)) continue;
			if(!turn && !isBlackPiece(piece)) continue;
			
			//--- generate moves for this piece on the copy ---
			//reset copy from original each time
			for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  copy[r][c] = board[r][c];
			
			bool capArr[8][8] = {};
			int mY = pr , mX = pc;
			
			
			//--- PAWN ---
			if(piece == 'P'){
				
				if(mY-1 >= 0  &&  bd[mY-1][mX] == ' '){
					bd[mY-1][mX] = 'O';
					if(mY == 6  &&  bd[mY-2][mX] == ' ')  bd[mY-2][mX] = 'O';
				}
				
				if(mY-1 >= 0  &&  mX-1 >= 0  &&  isBlackPiece(bd[mY-1][mX-1]))  capArr[mY-1][mX-1] = true;
				if(mY-1 >= 0  &&  mX+1 < 8   &&  isBlackPiece(bd[mY-1][mX+1]))  capArr[mY-1][mX+1] = true;
				
				if(ep_active  &&  mY == 3){
					if(mX-1 == ep_col  &&  mX-1 >= 0  &&  bd[3][ep_col] == 'p')  bd[2][ep_col] = 'O';
					if(mX+1 == ep_col  &&  mX+1 < 8   &&  bd[3][ep_col] == 'p')  bd[2][ep_col] = 'O';
				}
			}
			
			else if(piece == 'p'){
				
				if(mY-1 >= 0  &&  bd[mY-1][mX] == ' '){
					bd[mY-1][mX] = 'O';
					if(mY == 6  &&  bd[mY-2][mX] == ' ')  bd[mY-2][mX] = 'O';
				}
				
				if(mY-1 >= 0  &&  mX-1 >= 0  &&  isWhitePiece(bd[mY-1][mX-1]))  capArr[mY-1][mX-1] = true;
				if(mY-1 >= 0  &&  mX+1 < 8   &&  isWhitePiece(bd[mY-1][mX+1]))  capArr[mY-1][mX+1] = true;
				
				if(ep_active  &&  mY == 3){
					if(mX-1 == ep_col  &&  mX-1 >= 0  &&  bd[3][ep_col] == 'P')  bd[2][ep_col] = 'O';
					if(mX+1 == ep_col  &&  mX+1 < 8   &&  bd[3][ep_col] == 'P')  bd[2][ep_col] = 'O';
				}
			}
			
			//--- ROOK / QUEEN (straight) ---
			else if(piece == 'R' || piece == 'r' || piece == 'Q' || piece == 'q'){
				
				bool isW = isWhitePiece(piece);
				int dx[] = {1,-1,0,0} , dy[] = {0,0,1,-1};
				
				for(int d=0 ; d<4 ; d++){
					for(int s=1 ; s<8 ; s++){
						
						int nr = mY + dy[d]*s , nc = mX + dx[d]*s;
						if(nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
						
						if(bd[nr][nc] == ' ')  bd[nr][nc] = 'O';
						else if((isW && isBlackPiece(bd[nr][nc])) || (!isW && isWhitePiece(bd[nr][nc]))){
							capArr[nr][nc] = true; break;
						} else break;
					}
				}
				
				//if queen, also do diagonals
				if(piece == 'Q' || piece == 'q'){
					
					int ddx[] = {1,-1,1,-1} , ddy[] = {1,-1,-1,1};
					for(int d=0 ; d<4 ; d++){
						for(int s=1 ; s<8 ; s++){
							
							int nr = mY + ddy[d]*s , nc = mX + ddx[d]*s;
							if(nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
							
							if(bd[nr][nc] == ' ')  bd[nr][nc] = 'O';
							else if((isW && isBlackPiece(bd[nr][nc])) || (!isW && isWhitePiece(bd[nr][nc]))){
								capArr[nr][nc] = true; break;
							} else break;
						}
					}
				}
			}
			
			//--- BISHOP ---
			else if(piece == 'B' || piece == 'b'){
				
				bool isW = isWhitePiece(piece);
				int ddx[] = {1,-1,1,-1} , ddy[] = {1,-1,-1,1};
				
				for(int d=0 ; d<4 ; d++){
					for(int s=1 ; s<8 ; s++){
						
						int nr = mY + ddy[d]*s , nc = mX + ddx[d]*s;
						if(nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
						
						if(bd[nr][nc] == ' ')  bd[nr][nc] = 'O';
						else if((isW && isBlackPiece(bd[nr][nc])) || (!isW && isWhitePiece(bd[nr][nc]))){
							capArr[nr][nc] = true; break;
						} else break;
					}
				}
			}
			
			//--- KNIGHT ---
			else if(piece == 'N' || piece == 'n'){
				
				bool isW = isWhitePiece(piece);
				int km[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
				
				for(int m=0 ; m<8 ; m++){
					
					int nr = mY + km[m][0] , nc = mX + km[m][1];
					if(nr < 0 || nr >= 8 || nc < 0 || nc >= 8) continue;
					
					if(bd[nr][nc] == ' ')  bd[nr][nc] = 'O';
					else if((isW && isBlackPiece(bd[nr][nc])) || (!isW && isWhitePiece(bd[nr][nc])))
						capArr[nr][nc] = true;
				}
			}
			
			//--- KING ---
			else if(piece == 'K' || piece == 'k'){
				
				bool isW = isWhitePiece(piece);
				int kd[8][2] = {{0,-1},{0,1},{-1,0},{1,0},{1,-1},{1,1},{-1,-1},{-1,1}};
				
				for(int d=0 ; d<8 ; d++){
					
					int nr = mY + kd[d][0] , nc = mX + kd[d][1];
					if(nr < 0 || nr >= 8 || nc < 0 || nc >= 8) continue;
					
					if(bd[nr][nc] == ' ')  bd[nr][nc] = 'O';
					else if((isW && isBlackPiece(bd[nr][nc])) || (!isW && isWhitePiece(bd[nr][nc])))
						capArr[nr][nc] = true;
				}
				
				//filter king moves that land in check
				for(int d=0 ; d<8 ; d++){
					
					int ty = mY + kd[d][0] , tx = mX + kd[d][1];
					if(ty < 0 || ty >= 8 || tx < 0 || tx >= 8) continue;
					
					if(bd[ty][tx] == 'O' || capArr[ty][tx]){
						
						char sDst = bd[ty][tx];  bool wCap = capArr[ty][tx];
						bd[mY][mX] = ' ';  bd[ty][tx] = piece;
						
						bool chk = false;
						check_det(isW, bd, chk);
						
						bd[mY][mX] = piece;  bd[ty][tx] = sDst;
						if(chk){ if(wCap) capArr[ty][tx] = false; else bd[ty][tx] = ' '; }
					}
				}
				
				//castling moves for legality check
				if(isW  &&  mY == 7  &&  mX == 4  &&  !wKingMoved  &&  !is_square_attacked(true, bd, 7, 4)){
					
					if(!wRookKMoved  &&  bd[7][7] == 'R'  &&  bd[7][5] == ' '  &&  bd[7][6] == ' ')
						if(!is_square_attacked(true, bd, 7, 5) && !is_square_attacked(true, bd, 7, 6))  bd[7][6] = 'O';
					
					if(!wRookQMoved  &&  bd[7][0] == 'R'  &&  bd[7][3] == ' '  &&  bd[7][2] == ' '  &&  bd[7][1] == ' ')
						if(!is_square_attacked(true, bd, 7, 3) && !is_square_attacked(true, bd, 7, 2))  bd[7][2] = 'O';
				}
				
				if(!isW  &&  mY == 7  &&  mX == 4  &&  !bKingMoved  &&  !is_square_attacked(false, bd, 7, 4)){
					
					if(!bRookKMoved  &&  bd[7][7] == 'r'  &&  bd[7][5] == ' '  &&  bd[7][6] == ' ')
						if(!is_square_attacked(false, bd, 7, 5) && !is_square_attacked(false, bd, 7, 6))  bd[7][6] = 'O';
					
					if(!bRookQMoved  &&  bd[7][0] == 'r'  &&  bd[7][3] == ' '  &&  bd[7][2] == ' '  &&  bd[7][1] == ' ')
						if(!is_square_attacked(false, bd, 7, 3) && !is_square_attacked(false, bd, 7, 2))  bd[7][2] = 'O';
				}
			}
			
			
			//--- filter illegal moves for non-king pieces ---
			if(piece != 'K'  &&  piece != 'k'){
				filter_illegal_moves(turn, bd, capArr, mY, mX);
			}
			
			//--- check if any move survived ---
			for(int r=0 ; r<8 ; r++){
				for(int c=0 ; c<8 ; c++){
					if(bd[r][c] == 'O' || capArr[r][c]) return true;
					}
				}
		}
	}
	
	return false;
}

int main(){
	
	Texture bgTexture;
	bgTexture.loadFromFile("assets/chessboard.png");
	Sprite bgSprite(bgTexture);
	bgSprite.setPosition(0, 0);
	bgSprite.setScale(1.5, 1.5);
	
	
	
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
				board[i][j] = ' ';
				
				//CAPITAL FOR WHITE
				if(i == 1){
				 board[i][j] = 'p';
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
	bool mouseClicked = false;
	bool turn = 1; //1 for White --- 0 for Black
	bool isDraw = false;
	
	
	
	//------------------
	RenderWindow window(VideoMode(window_width , window_height ), "!!CHESS!!" , Style::Resize);
	window.setFramerateLimit(60);
	
	//-------- MENU IMAGE SETUP --------
	Texture menuTexture;
	menuTexture.loadFromFile("assets/Static_menu.png");
	Sprite menuSprite(menuTexture);
	//scale from original 2048x2048 image to the 1050x1050 window
	float menuScaleX = (float)window_width  / (float)menuTexture.getSize().x;
	float menuScaleY = (float)window_height / (float)menuTexture.getSize().y;
	menuSprite.setScale(menuScaleX, menuScaleY);
	
	//-------- OPTIONS MENU IMAGE SETUP --------
	Texture optionsTexture;
	optionsTexture.loadFromFile("assets/menu_option.png");
	Sprite optionsSprite(optionsTexture);
	//scale from original 2048x2048 options image to the 1050x1050 window
	float optScaleX = (float)window_width  / (float)optionsTexture.getSize().x;
	float optScaleY = (float)window_height / (float)optionsTexture.getSize().y;
	optionsSprite.setScale(optScaleX, optScaleY);
	
	//button hit regions (original image pixel coords scaled to window)
	//PLAY button: original (713,745) to (1335,934)
	float playLeft   = 713.f  * menuScaleX;
	float playTop    = 745.f  * menuScaleY;
	float playRight  = 1335.f * menuScaleX;
	float playBottom = 934.f  * menuScaleY;
	//OPTIONS button: original (713,978) to (1335,1168)
	float optLeft   = 713.f  * menuScaleX;
	float optTop    = 978.f  * menuScaleY;
	float optRight  = 1335.f * menuScaleX;
	float optBottom = 1168.f * menuScaleY;
	//EXIT button: original (713,1213) to (1335,1403)
	float exitLeft   = 713.f  * menuScaleX;
	float exitTop    = 1213.f * menuScaleY;
	float exitRight  = 1335.f * menuScaleX;
	float exitBottom = 1403.f * menuScaleY;
	
	//-------- OPTIONS MENU BUTTON HIT REGIONS (original 2048x2048 coords) --------
	//MUSIC button: original ~(713, 700) to (1335, 840)
	float musicLeft   = 713.f  * optScaleX;
	float musicTop    = 700.f  * optScaleY;
	float musicRight  = 1335.f * optScaleX;
	float musicBottom = 840.f  * optScaleY;
	//SOUNDS button: original ~(713, 870) to (1335, 1010)
	float soundsLeft   = 713.f  * optScaleX;
	float soundsTop    = 870.f  * optScaleY;
	float soundsRight  = 1335.f * optScaleX;
	float soundsBottom = 1010.f * optScaleY;
	//BOARD ROTATION button: original ~(713, 1040) to (1335, 1180)
	float rotLeft   = 713.f  * optScaleX;
	float rotTop    = 1040.f * optScaleY;
	float rotRight  = 1335.f * optScaleX;
	float rotBottom = 1180.f * optScaleY;
	//UNDO button: original ~(713, 1210) to (1335, 1350)
	float undoBtnLeft   = 713.f  * optScaleX;
	float undoBtnTop    = 1210.f * optScaleY;
	float undoBtnRight  = 1335.f * optScaleX;
	float undoBtnBottom = 1350.f * optScaleY;
	//BACK button on the options screen: original (713,1390) to (1335,1530)
	float backLeft   = 713.f  * optScaleX;
	float backTop    = 1390.f * optScaleY;
	float backRight  = 1335.f * optScaleX;
	float backBottom = 1530.f * optScaleY;
	
	bool inMenu = true;
	bool inOptions = false;
	
	//-------- SOUND EFFECTS SETUP --------
	sf::SoundBuffer moveBuf, checkBuf, checkmateBuf, drawBuf;
	
	moveBuf.loadFromFile("assets/Sounds/move.mp3");
	checkBuf.loadFromFile("assets/Sounds/check.mp3");
	checkmateBuf.loadFromFile("assets/Sounds/checkmate.mp3");
	drawBuf.loadFromFile("assets/Sounds/draw.wav");
	
	sf::Sound moveSound(moveBuf);
	sf::Sound checkSound(checkBuf);
	sf::Sound checkmateSound(checkmateBuf);
	sf::Sound drawSound(drawBuf);
	
	moveSound.setVolume(100.f);
	checkSound.setVolume(100.f);
	checkmateSound.setVolume(45.f);
	drawSound.setVolume(45.f);
	
	//-------- BACKGROUND MUSIC SETUP --------
	sf::Music bgMusic;
	if(bgMusic.openFromFile("assets/Sounds/bg_music.mp3")){
		bgMusic.setLoop(true);
		bgMusic.setVolume(30.f);
		if(musicEnabled) bgMusic.play();
	}
	
	//track previous check/gameOver state to trigger sounds once
	bool prevWhiteCheck = false;
	bool prevBlackCheck = false;
	bool prevGameOver = false;
	bool prevDraw = false;
	
	Event ev;
	
	//game loop
	while(window.isOpen()){
	
		//-------- OPTIONS SCREEN --------
		if(inOptions){
			while(window.pollEvent(ev)){
				if(ev.type == Event::Closed){
					window.close();
				}
				if(ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Button::Left){
					float mx = (float)ev.mouseButton.x;
					float my = (float)ev.mouseButton.y;
					
					//MUSIC button clicked -> toggle background music
					if(mx >= musicLeft && mx <= musicRight && my >= musicTop && my <= musicBottom){
						musicEnabled = !musicEnabled;
						if(musicEnabled){
							if(bgMusic.getStatus() != sf::Music::Playing)
								bgMusic.play();
						} else {
							bgMusic.pause();
						}
						cout << "Music: " << (musicEnabled ? "ON" : "OFF") << endl;
					}
					
					//SOUNDS button clicked -> toggle game sound effects
					if(mx >= soundsLeft && mx <= soundsRight && my >= soundsTop && my <= soundsBottom){
						soundsEnabled = !soundsEnabled;
						cout << "Sounds: " << (soundsEnabled ? "ON" : "OFF") << endl;
					}
					
					//BOARD ROTATION button clicked -> toggle board rotation
					if(mx >= rotLeft && mx <= rotRight && my >= rotTop && my <= rotBottom){
						ROTATE_BOARD = !ROTATE_BOARD;
						cout << "Board Rotation: " << (ROTATE_BOARD ? "ON" : "OFF") << endl;
					}
					
					//UNDO button clicked -> toggle undo functionality
					if(mx >= undoBtnLeft && mx <= undoBtnRight && my >= undoBtnTop && my <= undoBtnBottom){
						undoEnabled = !undoEnabled;
						cout << "Undo: " << (undoEnabled ? "ON" : "OFF") << endl;
					}
					
					//BACK button clicked -> return to main menu
					if(mx >= backLeft && mx <= backRight && my >= backTop && my <= backBottom){
						inOptions = false;
						inMenu = true;
					}
				}
			}
			if(Keyboard::isKeyPressed(Keyboard::Escape)){
				inOptions = false;
				inMenu = true;
			}
			window.clear();
			window.draw(optionsSprite);
			
			//-------- DRAW GREEN HIGHLIGHT on enabled options --------
			{
				auto drawEnabledHighlight = [&](float left, float top, float right, float bottom, bool enabled){
					if(!enabled) return;
					RectangleShape highlight(Vector2f(right - left, bottom - top));
					highlight.setPosition(left, top);
					highlight.setFillColor(Color(50, 200, 50, 65));
					highlight.setOutlineColor(Color(80, 255, 80, 120));
					highlight.setOutlineThickness(2.f);
					window.draw(highlight);
				};
				
				drawEnabledHighlight(musicLeft, musicTop, musicRight, musicBottom, musicEnabled);
				drawEnabledHighlight(soundsLeft, soundsTop, soundsRight, soundsBottom, soundsEnabled);
				drawEnabledHighlight(rotLeft, rotTop, rotRight, rotBottom, ROTATE_BOARD);
				drawEnabledHighlight(undoBtnLeft, undoBtnTop, undoBtnRight, undoBtnBottom, undoEnabled);
			}
			
			window.display();
			continue; //skip the rest of the game loop while in options
		}
	
		//-------- MENU SCREEN --------
		if(inMenu){
			while(window.pollEvent(ev)){
				if(ev.type == Event::Closed){
					window.close();
				}
				if(ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Button::Left){
					float mx = (float)ev.mouseButton.x;
					float my = (float)ev.mouseButton.y;
					//PLAY button clicked
					if(mx >= playLeft && mx <= playRight && my >= playTop && my <= playBottom){
						inMenu = false;
					}
					//OPTIONS button clicked -> go to options screen
					if(mx >= optLeft && mx <= optRight && my >= optTop && my <= optBottom){
						inMenu = false;
						inOptions = true;
					}
					//EXIT button clicked
					if(mx >= exitLeft && mx <= exitRight && my >= exitTop && my <= exitBottom){
						window.close();
					}
				}
			}
			if(Keyboard::isKeyPressed(Keyboard::Escape)){
				window.close();
			}
			window.clear();
			window.draw(menuSprite);
			window.display();
			continue; //skip the rest of the game loop while in menu
		}
		
		//check dettection!!! both king all the time
		whiteInCheck = false;
		blackInCheck = false;
		
		check_det( true,  board, whiteInCheck );
		check_det( false, board, blackInCheck );
		
		
		//-------- CHECKMATE / STALEMATE DETECTION --------
		if(!gameOver && !promotionPending && !isDraw){
			bool currentInCheck = turn ? whiteInCheck : blackInCheck;
			if(!has_any_legal_move(turn, board)){
				gameOver = true;
				if(currentInCheck){
					gameOverMsg = "CHECKMATE";
					gameOverSub = turn ? "Black Wins!" : "White Wins!";
				} else {
					gameOverMsg = "STALEMATE";
					gameOverSub = "Draw  -  No Legal Moves";
					isDraw = true;
				}
			}
		}
		
		
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
							
							//-------- BUILD PROMOTION NOTATION --------
							//the pawn move info was saved in the undo record; we use lastMove coords
							//since the move already happened, we reconstruct from saved state
							{
								MoveRecord& lastRec = undoHistory.back();
								//find where the pawn was (from the saved board snapshot vs current)
								int fromR = -1, fromC = -1;
								char pawnChar = promotionTurn ? 'P' : 'p';
								for(int r=0 ; r<8  &&  fromR<0 ; r++){
									for(int c=0 ; c<8 ; c++){
										if(lastRec.boardSnap[r][c] == pawnChar  &&  board[r][c] != pawnChar){
											fromR = r;  fromC = c;  break;
										}
									}
								}
								
								//detect if it was a capture
								char destBefore = lastRec.boardSnap[promotionRow][promotionCol];
								
								string promoNotation = build_notation(
									pawnChar, fromR >= 0 ? fromR : 0, fromC >= 0 ? fromC : 0,
									promotionRow, promotionCol,
									destBefore, false, false, false,
									true, chosen, promotionTurn, board
								);
								
								notationLog.push_back(promoNotation);
								
								if(promotionTurn){
									cout << moveNumber << ". " << promoNotation;
								} else {
									cout << "  " << promoNotation << endl;
									moveNumber++;
								}
							}
							
							//now do the turn switch and rotation that was delayed
							turn = !turn;
							pendingMoveSound = true;
							
							char temp[height][width];
							if(ROTATE_BOARD){
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
					}
					else{
						
					x= (ev.mouseButton.x - boarder_width) / ((window_width - 2*boarder_width) / width);
					y= (ev.mouseButton.y - boarder_height) / ((window_height - 2*boarder_height) / height);
					
					if(!isDraw && !gameOver)
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
		
		//-------- UNDO (press Z) --------
		//restores the game to the state before the last move (only if undo is enabled)
		{
			static bool zWasPressed = false;
			bool zNow = Keyboard::isKeyPressed(Keyboard::Z);
			if(zNow  &&  !zWasPressed  &&  undoEnabled  &&  !undoHistory.empty()  &&  !promotionPending){
				MoveRecord& rec = undoHistory.back();
				
				//restore board
				for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++)  board[r][c] = rec.boardSnap[r][c];
				
				//restore turn and flags
				turn = rec.turnSnap;
				wKingMoved = rec.wKM;  wRookKMoved = rec.wRKM;  wRookQMoved = rec.wRQM;
				bKingMoved = rec.bKM;  bRookKMoved = rec.bRKM;  bRookQMoved = rec.bRQM;
				ep_active = rec.epActive;  ep_col = rec.epCol;
				
				//restore highlights
				lastMoveFromRow = rec.lmFR;  lastMoveFromCol = rec.lmFC;
				lastMoveToRow   = rec.lmTR;  lastMoveToCol   = rec.lmTC;
				selectedRow = rec.selR;  selectedCol = rec.selC;
				
				//clear any displayed move indicators
				for(int r=0 ; r<8 ; r++)  for(int c=0 ; c<8 ; c++){
					if(board[r][c] == 'O') board[r][c] = ' ';
					checkCapture[r][c] = false;
				}
				
				//restore move number and remove last notation entry
				moveNumber = rec.savedMoveNumber;
				if(!notationLog.empty()) notationLog.pop_back();
				
				undoHistory.pop_back();
				isDraw = false;
				gameOver = false;
				gameOverMsg = "";
				gameOverSub = "";
			}
			zWasPressed = zNow;
		}
		
		// King vs King draw check (only when game is not already over)
		if(!gameOver)
			isDraw = is_king_vs_king(board);
		
		//-------- SOUND EFFECTS TRIGGERS --------
		if(soundsEnabled){
			//move / capture sound
			if(pendingMoveSound){
				moveSound.play();
				pendingMoveSound = false;
				pendingCaptureSound = false; //move sound covers capture too
			}
			//checkmate / stalemate sound (play once)
			if(gameOver && !prevGameOver){
				if(gameOverMsg == "CHECKMATE")
					checkmateSound.play();
			}
			//draw sound (play once)
			if(isDraw && !prevDraw){
				drawSound.play();
			}
			//check sound (play once per new check state)
			if(!gameOver && !isDraw){
				if(whiteInCheck && !prevWhiteCheck)
					checkSound.play();
				if(blackInCheck && !prevBlackCheck)
					checkSound.play();
			}
		} else {
			//sounds disabled: just clear pending flags
			pendingMoveSound = false;
			pendingCaptureSound = false;
		}
		prevWhiteCheck = whiteInCheck;
		prevBlackCheck = blackInCheck;
		prevGameOver   = gameOver;
		prevDraw       = isDraw;
		
		
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
		
		// -------- GAME OVER / DRAW OVERLAY --------
		if(isDraw || gameOver){
			// Dark semi-transparent overlay
			RectangleShape overlay(Vector2f(window_width, window_height));
			overlay.setFillColor(Color(0, 0, 0, 170));
			window.draw(overlay);
			
			// banner box
			RectangleShape banner(Vector2f(620, 180));
			banner.setOrigin(310, 90);
			banner.setPosition(window_width / 2.f, window_height / 2.f - 40);
			banner.setFillColor(Color(30, 30, 30, 230));
			banner.setOutlineColor(Color(200, 200, 200, 255));
			banner.setOutlineThickness(3.f);
			window.draw(banner);
			
			static Font endFont;
			static bool endFontLoaded = false;
			if(!endFontLoaded){
				if(!endFont.loadFromFile("C:/Windows/Fonts/arialbd.ttf"))
					endFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
				endFontLoaded = true;
			}
			
			// main text (CHECKMATE / STALEMATE / DRAW)
			string mainMsg = gameOver ? gameOverMsg : "DRAW";
			string subMsg  = gameOver ? gameOverSub : "King vs King  -  Insufficient Material";
			
			Text mainText;
			mainText.setFont(endFont);
			mainText.setString(mainMsg);
			mainText.setCharacterSize(gameOver && gameOverMsg == "CHECKMATE" ? 80 : 100);
			mainText.setFillColor(Color(220, 220, 220, 255));
			mainText.setStyle(Text::Bold);
			FloatRect tb = mainText.getLocalBounds();
			mainText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
			mainText.setPosition(window_width / 2.f, window_height / 2.f - 55);
			window.draw(mainText);
			
			// subtitle
			Text subText;
			subText.setFont(endFont);
			subText.setString(subMsg);
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
