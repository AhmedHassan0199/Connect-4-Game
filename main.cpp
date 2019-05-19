#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML\Audio.hpp>

#define speedofgame 9  // to control the speed of animations
#define speedofchip 7	// to control the speed of chips movement 
using namespace std;
using namespace sf;


struct Button                         // button range
{
	sf::Vector2f fupperLimit, flowerLimit;
	sf::Vector2i iupperLimit, ilowerLimit;
};
struct interactiveButton		     // button design  
{
	sf::RectangleShape shape;
	sf::Texture normal;
	sf::Texture hover;
	sf::Texture special;
	Button button;
};
void unlock(bool &m, bool &h, char cw, short dif)     // to pass from level to another
{
	if (cw == 'R')          // if the user is the winner
	{
		if (dif == 3)
			m = 0;         // Medium is unlocked
		else if (dif == 5)
			h = 0;         // Hard is unlocked 
	}
}
void normalize(interactiveButton &ib)  // return each button to it's original texture
{
	ib.shape.setTexture(&ib.normal);
}
void hoverize(interactiveButton &ib)  // the button texture in selection mode
{
	ib.shape.setTexture(&ib.hover);
}
void menuReset(interactiveButton &vsPlayer, interactiveButton &vsCOM, interactiveButton &menuExit, bool &movingAtoM, sf::Vector2f menuPos1)
{                           // return back the main menu as it is
	normalize(vsPlayer);
	normalize(vsCOM);
	movingAtoM = 1;
	vsPlayer.shape.setPosition(menuPos1.x, -200);
	vsCOM.shape.setPosition(menuPos1.x, -200);
	menuExit.shape.setPosition(menuPos1.x, -200);
}
void manageGridMap(bool &mapToggle, char gridMap[6][7], short &currentColumnCounter, short &i)
{
	if (mapToggle)       // switch between players
	{
		gridMap[currentColumnCounter][i] = 'R';
		mapToggle = 0;   // to let the yellow play
	}
	else
	{
		gridMap[currentColumnCounter][i] = 'Y';
		mapToggle = 1;  // to let the red play
	}
	for (short i = 5; i >= 0; i--)
	{
		for (short j = 0; j<7; ++j)
			std::cout << gridMap[i][j] << ' ';    // to appear in the console
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void pvpAnnounceWin(char currentWinner, bool &movingSub, bool &win, sf::Sprite &winShape, sf::Sprite redWinSprite, sf::Sprite yellowWinSprite, sf::Sprite tieWinSprite)
{                           // what happens at the end of the game
	movingSub = 1;         // the movement of the blue sub menu is activated 
	win = 1;
	switch (currentWinner)
	{
	case 'R':                       // If red wins
		winShape = redWinSprite;
		break;
	case 'Y':                      // if yellow wins
		winShape = yellowWinSprite;
		break;
	case 'T':                       // if it is a tie
		winShape = tieWinSprite;
		break;
	}
}
void pvpDraw(sf::RenderWindow &window, sf::RectangleShape inGamebackgroundShape, sf::RectangleShape Chip[42], sf::RectangleShape grid, interactiveButton mainMenu, bool win, sf::RectangleShape subMenuShape, sf::Sprite winShape, bool movingSub, interactiveButton playAgain, interactiveButton dontPlayAgain)
{                                           // Representing shapes in the game mode
	window.draw(inGamebackgroundShape);
	for (short i = 41; i >= 0; --i)
		window.draw(Chip[i]);
	window.draw(grid);
	window.draw(mainMenu.shape);
	if (win)
	{                               // representing the winning or the tie shapes
		window.draw(subMenuShape);   // The blue menu 
		window.draw(winShape);       // (red , yellow or tie)
		if (movingSub == 0)              // when the shapes reach it's position
		{
			window.draw(playAgain.shape);
			window.draw(dontPlayAgain.shape); // main menu
		}
	}
}
void reset(sf::RectangleShape Chip[42], short CC[7], short &Cur, char matrix[6][7], bool &win, bool &mapToggle)
{                             // after the end of the game 
	win = 0;                   // the win is deactivated
	mapToggle = 1;                 // Red's turn
	Cur = 0;
	for (short i = 0; i<42; ++i)
	{
		if (i<7)
			CC[i] = 0;
		if (i % 2 == 0)
			Chip[i].setPosition(1180, 590 - (i * 8));  // Red chips return back to it's original position
		else
			Chip[i].setPosition(1280, 590 - ((i - 1) * 8)); // yellow chips return back to it's original position
	}

	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
			matrix[i][j] = 'e';      // the 42 place are empty
}
void hang()
{                // do nothing untill the release of the left mouse button
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		continue;
}
bool LMB()
{                        // to check whether the left mouse button is pressed or not
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		return 1;
	else
		return 0;
}
void RectButtonAssign(sf::RectangleShape RShape, Button &RButton) // to assign the limits of the button to the shape
{
	RButton.flowerLimit = RShape.getPosition(); // by default get.position capture the the farthest point up to the left
	RButton.fupperLimit.x = RButton.flowerLimit.x + RShape.getSize().x;  // upper limit on x-axis
	RButton.fupperLimit.y = RButton.flowerLimit.y + RShape.getSize().y; // upper limit on y-axis
}
void RectButtonAssign2(interactiveButton &ib)        // to assign the  limits of the interactive button to the shape
{
	ib.button.flowerLimit = ib.shape.getPosition();
	ib.button.fupperLimit.x = ib.button.flowerLimit.x + ib.shape.getSize().x;  // upper limit on x-axis
	ib.button.fupperLimit.y = ib.button.flowerLimit.y + ib.shape.getSize().y;  // upper limit on y-axis
}
bool isButtonHoveredf2(Button AButton, sf::Vector2i original)
{
	if (original.x<AButton.fupperLimit.x && original.x>AButton.flowerLimit.x && original.y<AButton.fupperLimit.y && original.y>AButton.flowerLimit.y)
		return 1;
	else
		return 0;
}
bool isButtonHoveredf3(Button AButton, sf::RenderWindow &window) // to check if the button is selected or not  
{
	sf::Vector2i original = sf::Mouse::getPosition(window);  // to get the positions of the mouse 
	if (original.x<AButton.fupperLimit.x && original.x>AButton.flowerLimit.x && original.y<AButton.fupperLimit.y && original.y>AButton.flowerLimit.y)
		return 1;  // in case that the mouse is in the same range of the button .. the bool is true
	else
		return 0;
}
bool isButtonHoveredf4(interactiveButton ib, sf::RenderWindow &window)  // to check if the interactive button is selected or not  
{
	Button AButton = ib.button;
	sf::Vector2i original = sf::Mouse::getPosition(window);
	if (original.x<AButton.fupperLimit.x && original.x>AButton.flowerLimit.x && original.y<AButton.fupperLimit.y && original.y>AButton.flowerLimit.y)
		return 1;
	else
		return 0;
}
bool ibuttonAutoHover(interactiveButton &ib, sf::RenderWindow &window)
{                                             // to check if I pressed on the button or no
	if (isButtonHoveredf4(ib, window))
	{
		hoverize(ib);  // button is selected
		if (LMB())     // button is pressed
			return 1;
	}
	else
		normalize(ib);  // return it back to ot's original texture
	return 0;
}
void muteCheck(Button muteButton, sf::RenderWindow &window, bool &muted, sf::RectangleShape &muteShape, sf::Texture &mute, sf::Texture &unmute)
{ // GUI for the mute mode
	if (LMB() && isButtonHoveredf3(muteButton, window))
	{
		if (muted)
		{
			muted = 0;
			muteShape.setTexture(&unmute);
		}
		else
		{
			muted = 1;
			muteShape.setTexture(&mute);
		}
		hang();
	}
}
void muteMusicCheck(Button muteButton, sf::RenderWindow &window, bool &muted, sf::RectangleShape &muteShape, sf::Texture &mute, sf::Texture &unmute)
{                 // GUI for the mute mode
	if (LMB() && isButtonHoveredf3(muteButton, window))
	{
		if (muted)
		{
			muted = 0;
			muteShape.setTexture(&unmute);
		}
		else
		{
			muted = 1;
			muteShape.setTexture(&mute);
		}
		hang();
	}
}
bool yMovingR(sf::RectangleShape &RS, sf::Vector2f destination, char direction, float speed)
{
	if (direction == 'd')       // Moving downwards
	{
		if (RS.getPosition().y >= destination.y)
		{
			RS.setPosition(destination);  // To put the shape in the position I want
			return 0;
		}
		else
		{
			RS.move(0, speed);  // To let it move till it reach the destination
			return 1;
		}
	}
	else if (direction == 'u')  // Moving upwards
	{
		if (RS.getPosition().y <= destination.y)
		{
			RS.setPosition(destination);  // To put the shape in the position I want
			return 0;
		}
		else
		{
			RS.move(0, -1.0*speed);   // To let it move till it reach the destination
			return 1;
		}
	}
}
bool yMovingS(sf::Sprite &RS, sf::Vector2f destination, char direction, float speed)
{                                       // in case of sprite
	if (direction == 'd')
	{
		if (RS.getPosition().y >= destination.y)
		{
			RS.setPosition(destination);
			return 0;
		}
		else
		{
			RS.move(0, speed);
			return 1;
		}
	}
	else if (direction == 'u')
	{
		if (RS.getPosition().y <= destination.y)
		{
			RS.setPosition(destination);
			return 0;
		}
		else
		{
			RS.move(0, -1.0*speed);
			return 1;
		}
	}
}
char winner(char matrix[6][7])
{
	// Win By Rows
	for (short i = 0; i<6; ++i)      // loop for rows
		for (short j = 0; j<4; ++j)  // loop for columns
			if (matrix[i][j] != 'e' && matrix[i][j] == matrix[i][j + 1] && matrix[i][j] == matrix[i][j + 2] && matrix[i][j] == matrix[i][j + 3])
				return matrix[i][j];


	// Win By Diagonals
	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
		{
			if (matrix[i][j] != 'e' && i + 3<6 && j + 3<7 && matrix[i][j] == matrix[i + 1][j + 1] && matrix[i][j] == matrix[i + 2][j + 2] && matrix[i][j] == matrix[i + 3][j + 3])
				return matrix[i][j];
			else if (matrix[i][j] != 'e' && i + 3<6 && j - 3 >= 0 && matrix[i][j] == matrix[i + 1][j - 1] && matrix[i][j] == matrix[i + 2][j - 2] && matrix[i][j] == matrix[i + 3][j - 3])
				return matrix[i][j];
		}


	// Win By Columns
	for (short i = 0; i < 3; ++i)
		for (short j = 0; j < 7; ++j)
			if (matrix[i][j] != 'e' && matrix[i][j] == matrix[i + 1][j] && matrix[i][j] == matrix[i + 2][j] && matrix[i][j] == matrix[i + 3][j])
				return matrix[i][j];


	// No win yet
	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
			if (matrix[i][j] == 'e')
				return 'e';

	// Tie
	return 'T';
}
bool playTurn(char m[6][7], short col)
{                                              // red and yellow chips counter
	short RCounter = 0, YCounter = 0;

	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
		{
			if (m[i][j] == 'R')
				++RCounter;
			else if (m[i][j] == 'Y')
				++YCounter;
		}

	if (col<0 || col>6 || RCounter + YCounter == 42)
		return 0;                                    // you are unable to play
	char curTurn = (RCounter == YCounter) ? 'R' : 'Y';
	for (short i = 0; i<6; ++i)
	{
		if (m[i][col] == 'e')
		{
			m[i][col] = curTurn;
			return 1;
		}
	}

	return 0;
}
void unplayTurn(char m[6][7], short col)
{
	for (short i = 5; i >= 0; --i)
	{
		if (m[i][col] != 'e')
		{
			m[i][col] = 'e';
			break;
		}
	}
}
short evaluate(char matrix[6][7])  // USED in AI
{
	// Giving a score for each situation  
	short score = 0;


	// Win By Rows
	for (short i = 0; i<6; ++i)
		for (short j = 0; j<4; ++j)
			if (matrix[i][j] != 'e' && matrix[i][j] == matrix[i][j + 1] && matrix[i][j] == matrix[i][j + 2] && matrix[i][j] == matrix[i][j + 3])
			{
				if (matrix[i][j] == 'R')
					score -= 3;
				else
					score += 1;
			}


	// Win By Diagonals
	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
		{
			if (matrix[i][j] != 'e' && i + 3<6 && j + 3<7 && matrix[i][j] == matrix[i + 1][j + 1] && matrix[i][j] == matrix[i + 2][j + 2] && matrix[i][j] == matrix[i + 3][j + 3])
			{
				if (matrix[i][j] == 'R')
					score -= 3;
				else
					score += 1;
			}
			else if (matrix[i][j] != 'e' && i + 3<6 && j - 3 >= 0 && matrix[i][j] == matrix[i + 1][j - 1] && matrix[i][j] == matrix[i + 2][j - 2] && matrix[i][j] == matrix[i + 3][j - 3])
			{
				if (matrix[i][j] == 'R')
					score -= 3;
				else
					score += 1;
			}
		}


	// Win By Columns
	for (short i = 0; i<3; ++i)
		for (short j = 0; j<7; ++j)
			if (matrix[i][j] != 'e' && matrix[i][j] == matrix[i + 1][j] && matrix[i][j] == matrix[i + 2][j] && matrix[i][j] == matrix[i + 3][j])
			{
				if (matrix[i][j] == 'R')
					score -= 3;
				else
					score += 1;
			}
	//std::cout<<score<<' ';
	return score;
}
void MiniMax(char m[6][7], short level, short &i, long &tracker, long score[])
{                                     // Algorithm used in AI
	if (level > 0)
	{
		for (short j = 0; j<7; ++j)
		{
			if (playTurn(m, j))
			{
				score[i] += evaluate(m);
				MiniMax(m, level - 1, i, tracker, score);
				unplayTurn(m, j);
			}
		}
	}
	else if (level == 0)
	{
		score[i] += evaluate(m);
	}
}
short COM(char m[6][7], short level)
{                            // The com is considered as the main for minimax
	short maxi = 3;
	long score[7] = { 0 }, maxScore = -99999999999999999, tracker = 0;
	for (short i = 0; i<7; ++i)
	{
		if (playTurn(m, i))
		{
			score[i] += evaluate(m);
			MiniMax(m, level - 1, i, tracker, score);
			unplayTurn(m, i);
		}
	}

	for (short i = 0; i<7; ++i)
	{
		if (score[i] > maxScore && playTurn(m, i) == 1)
		{
			unplayTurn(m, i);
			maxScore = score[i];
			maxi = i;
		}
	}

	if (playTurn(m, maxi))
	{
		unplayTurn(m, maxi);
		return maxi;
	}
	else
	{
		for (short i = 0; i<7; ++i)
		{
			if (playTurn(m, i))
			{
				unplayTurn(m, i);
				return i;
			}
		}
	}
}

void play(Sound & sound1, Sound & sound4, Sound & sound3, bool & muted, bool & mutedMusic, bool & resetMusic, bool & isplay1, char & currentMode, char gridMap[6][7], char &currentWinner, char &destinationMode, bool &movingChip, bool&win, bool&movingSub, bool & mapToggle, bool &movingAtoM, bool &mediumLocked, bool&hardLocked, RectangleShape Chip[], RectangleShape & subMenuShape, RectangleShape &menuBackgroundShape, Sprite & winShape, Sprite &redWinSprite, Sprite &yellowWinSprite, Sprite &tieWinSprite, interactiveButton &playAgain, interactiveButton &dontPlayAgain, interactiveButton&vsPlayer, interactiveButton&vsCOM, interactiveButton&menuExit, RenderWindow &window, Vector2f&destination, Vector2f&subMenuDef, Vector2f&winShapeDef, Vector2f&menuPos1, short &currentChip, short columnCounter[], short&difficulty, float chipSpeed, float SubMenuSpeed) {
	if (movingChip)
	{
		if (yMovingR(Chip[currentChip], destination, 'd', chipSpeed) == 0)
		{
			movingChip = 0;
			if (muted == 0)
			{
				sound1.play();
			}
			if (currentChip < 41)
				++currentChip;
		}
		isplay1 = 1;
	}
	else if (win)
	{
		if (movingSub)
		{
			if (yMovingR(subMenuShape, subMenuDef, 'u', SubMenuSpeed) == 0 && yMovingS(winShape, winShapeDef, 'd', SubMenuSpeed) == 0)
				movingSub = 0;
		}
		else
		{
			if (ibuttonAutoHover(playAgain, window))
			{
				reset(Chip, columnCounter, currentChip, gridMap, win, mapToggle);
				resetMusic = true;
				hang();
			}
			else if (ibuttonAutoHover(dontPlayAgain, window))
			{
				currentMode = 'B';                    // Backward
				menuBackgroundShape.setPosition(0, 690);
				destinationMode = 'M';
				menuReset(vsPlayer, vsCOM, menuExit, movingAtoM, menuPos1);
				reset(Chip, columnCounter, currentChip, gridMap, win, mapToggle);
				resetMusic = true;
				hang();
			}
		}
		isplay1 = 1;
	}
	else if (currentWinner != 'e')
	{
		pvpAnnounceWin(currentWinner, movingSub, win, winShape, redWinSprite, yellowWinSprite, tieWinSprite);
		subMenuShape.setPosition(subMenuDef.x, 900);
		winShape.setPosition(winShapeDef.x, -330);
		if (mutedMusic == 0)
		{
			sound4.pause();

		}
		if (muted == 0)
		{
			sound3.play();
		}
		if (currentMode == 'C')
			unlock(mediumLocked, hardLocked, currentWinner, difficulty);
		isplay1 = 1;
	}

}

int main()
{

	sf::RenderWindow window(sf::VideoMode(1366, 690), "Connect 4");

	// gridMap and algorithm related stuff
	char currentWinner = 'e';
	bool win = 0;
	bool mapToggle = 1;        // RED STARTS
	char gridMap[6][7];
	for (short i = 0; i<6; ++i)
		for (short j = 0; j<7; ++j)
			gridMap[i][j] = 'e';              // All the grid contain empty spaces



											  // Backgrounds
	sf::Texture inGameBackgroundTexture;
	inGameBackgroundTexture.loadFromFile("c1584.png");
	sf::Texture menuBackgroundTexture;
	menuBackgroundTexture.loadFromFile("Back-Menu.png");

	sf::RectangleShape inGamebackgroundShape;
	sf::RectangleShape menuBackgroundShape;

	menuBackgroundShape.setTexture(&menuBackgroundTexture);
	menuBackgroundShape.setSize(sf::Vector2f(1366, 690));
	menuBackgroundShape.setPosition(0, 0);

	inGamebackgroundShape.setTexture(&inGameBackgroundTexture);
	inGamebackgroundShape.setSize(sf::Vector2f(1366, 690));
	inGamebackgroundShape.setPosition(0, 0);

	// Default Vectors or Positions (for movement)

	sf::Vector2f subMenuDef(233, 245);
	sf::Vector2f winShapeDef(150, 100);
	sf::Vector2f menuButtons(468, 86);   // size
	sf::Vector2f menuPos1(449, 250);
	sf::Vector2f menuPos2(449, 400);
	sf::Vector2f menuPos3(449, 550);


	// Menu and in-game buttons

	interactiveButton vsPlayer, vsCOM, menuExit, mainMenu, playAgain, dontPlayAgain, easy, medium, hard;

	sf::Texture title;
	sf::Texture redWin;
	sf::Texture yellowWin;
	sf::Texture tieWin;
	sf::Texture mute;
	sf::Texture unmute;
	sf::Texture muteMusic;
	sf::Texture unmuteMusic;
	sf::Texture subMenu;
	// Interactive buttons

	vsPlayer.normal.loadFromFile("Red_1v1.png");
	vsPlayer.hover.loadFromFile("1_v1_hover.png");
	vsCOM.normal.loadFromFile("1_v_computer.png");
	vsCOM.hover.loadFromFile("1_v_computer_hover.png");
	menuExit.normal.loadFromFile("EXIT.png");
	menuExit.hover.loadFromFile("EXIT_hover.png");
	mainMenu.normal.loadFromFile("MainMenu.png");
	mainMenu.hover.loadFromFile("MainMenu_hover.png");
	playAgain.normal.loadFromFile("Play_Again.png");
	playAgain.hover.loadFromFile("Play_Again_hover_.png");
	easy.normal.loadFromFile("Easy.png");
	easy.hover.loadFromFile("EasyHov.png");
	medium.normal.loadFromFile("Medium.png");
	medium.hover.loadFromFile("MediumHov.png");
	medium.special.loadFromFile("MediumLocked.png");
	hard.normal.loadFromFile("Hard.png");
	hard.hover.loadFromFile("HardHov.png");
	hard.special.loadFromFile("HardLocked.png");
	dontPlayAgain.normal = mainMenu.normal;
	dontPlayAgain.hover = mainMenu.hover;


	title.loadFromFile("title.png");                 // Textures
	mute.loadFromFile("Mute.png");
	unmute.loadFromFile("Unmute.png");
	muteMusic.loadFromFile("muteMusic.png");
	unmuteMusic.loadFromFile("unmuteMusic.png");
	redWin.loadFromFile("RED_WIN.png");
	yellowWin.loadFromFile("YELLOW_WIN.png");
	tieWin.loadFromFile("TIE.png");
	subMenu.loadFromFile("WinWindow.png");




	sf::Sprite redWinSprite(redWin);
	sf::Sprite yellowWinSprite(yellowWin);
	sf::Sprite tieWinSprite(tieWin);
	sf::RectangleShape titleShape;
	sf::RectangleShape muteShape;
	sf::RectangleShape muteMusicShape;
	sf::RectangleShape subMenuShape;
	// Giving sizes
	vsPlayer.shape.setSize(menuButtons);
	vsCOM.shape.setSize(menuButtons);
	menuExit.shape.setSize(menuButtons);
	mainMenu.shape.setSize(sf::Vector2f(200, 40));
	playAgain.shape.setSize(menuButtons);
	dontPlayAgain.shape.setSize(menuButtons);
	easy.shape.setSize(menuButtons);
	medium.shape.setSize(menuButtons);
	hard.shape.setSize(menuButtons);

	titleShape.setSize(menuButtons);
	muteShape.setSize(sf::Vector2f(60, 60));
	muteMusicShape.setSize(sf::Vector2f(80, 80));
	subMenuShape.setSize(sf::Vector2f(900, 400));

	normalize(vsPlayer);        // buttons are in normal mode 
	normalize(vsCOM);
	normalize(menuExit);
	normalize(mainMenu);
	normalize(playAgain);
	// putting images on the buttons
	easy.shape.setTexture(&easy.normal);
	medium.shape.setTexture(&medium.special);
	hard.shape.setTexture(&hard.special);

	titleShape.setTexture(&title);
	muteShape.setTexture(&unmute);
	muteMusicShape.setTexture(&unmuteMusic);
	subMenuShape.setTexture(&subMenu);
	//Positioning
	vsPlayer.shape.setPosition(menuPos1);
	vsCOM.shape.setPosition(menuPos2);
	menuExit.shape.setPosition(menuPos3);
	mainMenu.shape.setPosition(20, 20);
	playAgain.shape.setPosition(450, 370);
	dontPlayAgain.shape.setPosition(450, 470);
	easy.shape.setPosition(menuPos1);
	medium.shape.setPosition(menuPos2);
	hard.shape.setPosition(menuPos3);

	titleShape.setPosition(449, 100);
	muteShape.setPosition(1280, 30);
	muteMusicShape.setPosition(1270, 100);
	subMenuShape.setPosition(subMenuDef);
	redWinSprite.setPosition(winShapeDef);
	yellowWinSprite.setPosition(winShapeDef);

	Button muteButton;                               // Sound Buttons
	Button muteMusicButton;

	RectButtonAssign(muteShape, muteButton);
	RectButtonAssign(muteMusicShape, muteMusicButton);

	RectButtonAssign2(vsPlayer);
	RectButtonAssign2(vsCOM);
	RectButtonAssign2(menuExit);
	RectButtonAssign2(mainMenu);
	RectButtonAssign2(playAgain);
	RectButtonAssign2(dontPlayAgain);
	RectButtonAssign2(easy);
	RectButtonAssign2(medium);
	RectButtonAssign2(hard);


	// Button Motion
	vsPlayer.shape.setPosition(menuPos1.x, -200);
	vsCOM.shape.setPosition(menuPos1.x, -200);
	menuExit.shape.setPosition(menuPos1.x, -200);


	// Chips
	float chipRadius = 43.5;
	sf::Texture yellowChip;
	sf::Texture redChip;
	yellowChip.loadFromFile("yellownew.png");
	redChip.loadFromFile("rednew.png");
	sf::RectangleShape Chip[42];
	for (short i = 0; i<42; ++i)
	{
		Chip[i].setSize(sf::Vector2f(chipRadius * 2, chipRadius * 2));
		if (i % 2 == 0)
		{
			Chip[i].setTexture(&redChip);
			Chip[i].setPosition(1180, 590 - (i * 8));
		}
		else
		{
			Chip[i].setTexture(&yellowChip);
			Chip[i].setPosition(1280, 590 - ((i - 1) * 8));
		}
		Chip[i].setOrigin(chipRadius, chipRadius);
	}


	// Grid
	sf::RectangleShape grid;
	grid.setSize(sf::Vector2f(800, 600));
	sf::Texture gridTexture;
	gridTexture.loadFromFile("gridwoutline.png");
	grid.setTexture(&gridTexture);
	grid.setPosition(283, 90);


	// Grid Positions as Vector2f
	sf::Vector2f gridPos[6][7];
	gridPos[0][0].x = 283 + 95.7;
	gridPos[0][0].y = 690 - 75;
	for (short i = 0; i<6; ++i)
	{
		for (short j = 0; j<7; ++j)
		{
			gridPos[i][j].x = gridPos[0][0].x + (j*101.5);
			gridPos[i][j].y = gridPos[0][0].y - (i*94.7);
		}
	}

	sf::Vector2f gridHoverPos[7];
	for (short i = 0; i<7; ++i)
	{
		gridHoverPos[i] = gridPos[5][i];
		gridHoverPos[i].y -= 95;
	}
	/////////////////////////SOUNDS//////////////////
	SoundBuffer click1;
	SoundBuffer click2;
	SoundBuffer Win;
	SoundBuffer MenuMusic;
	if (!MenuMusic.loadFromFile("Trial.wav"))
	{
		cout << "DIDNT FIND MUSIC";
	}
	if (!click1.loadFromFile("click1.wav") || !click2.loadFromFile("click2.wav") || !Win.loadFromFile("WIN.wav"))
	{
		cout << "DIDNT LOAD MUSIC";
	}


	sf::Sound sound1;
	sound1.setBuffer(click1);
	sf::Sound sound2;
	sound2.setBuffer(click2);
	sf::Sound sound3;
	sound3.setBuffer(Win);

	/////////////////////////SOUNDS//////////////////
	////////PLAY MUSIC/////////
	Sound sound4;
	sound4.setBuffer(MenuMusic);
	////////PLAY MUSIC/////////

	// Column Buttons
	float columnSpacing = 102;
	Button Column[7];
	Column[0].flowerLimit = grid.getPosition();
	Column[0].flowerLimit.x += 42;
	Column[0].fupperLimit = Column[0].flowerLimit;
	Column[0].fupperLimit.y += 600;
	Column[0].fupperLimit.x += columnSpacing;
	for (short i = 1; i<7; ++i)
	{
		Column[i].flowerLimit = Column[0].flowerLimit;
		Column[i].flowerLimit.x += (i*columnSpacing);
		Column[i].fupperLimit = Column[0].fupperLimit;
		Column[i].fupperLimit.x += (i*columnSpacing);
	}




	// Variables for Runtime use and calculations on the go
	short currentChip = 0, columnCounter[7] = { 0 }, difficulty;
	char currentMode = 'M';   // Main menu mode
	char destinationMode;
	bool muted = 0, mediumLocked = 1, hardLocked = 1;
	bool mutedMusic = 0;
	bool movingChip = 0, movingSub = 0, movingMtoD = 0, movingAtoM = 1;
	sf::Vector2f destination;
	sf::Sprite winShape;

	bool resetMusic = false;
	int musicStateCounter = 0;
	sound4.setLoop(true);

	while (window.isOpen())
	{
		/////MUTE AND PLAY MUSIC////
		if (resetMusic)
		{
			musicStateCounter = 0;
			resetMusic = false;
		}

		if (musicStateCounter == 0)
		{
			sound4.play();
		}

		if (mutedMusic)
		{
			sound4.pause();
			musicStateCounter = -100000000;

		}

		if (mutedMusic == 0 && musicStateCounter<0)
		{
			resetMusic = true;
		}
		/////MUTE AND PLAY MUSIC///
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}



		muteCheck(muteButton, window, muted, muteShape, mute, unmute);
		muteMusicCheck(muteMusicButton, window, mutedMusic, muteMusicShape, muteMusic, unmuteMusic);

		currentWinner = winner(gridMap);
		bool isplay1 = 0;
		switch (currentMode)
		{
		case 'G':                          // Game .. 1vs 1
			play(sound1, sound4, sound3, muted, mutedMusic, resetMusic, isplay1, currentMode, gridMap, currentWinner, destinationMode, movingChip, win, movingSub, mapToggle, movingAtoM, mediumLocked, hardLocked, Chip, subMenuShape, menuBackgroundShape, winShape, redWinSprite, yellowWinSprite, tieWinSprite, playAgain, dontPlayAgain, vsPlayer, vsCOM, menuExit, window, destination, subMenuDef, winShapeDef, menuPos1, currentChip, columnCounter, difficulty, speedofchip, speedofgame);
			if (!isplay1)
			{
				if (currentChip != 41)
					Chip[currentChip].setPosition(1500, 0);
				for (short i = 0; i<7; ++i)
				{
					if (columnCounter[i]<6 && isButtonHoveredf3(Column[i], window))  // The column have empty spaces
					{
						Chip[currentChip].setPosition(gridHoverPos[i]); // positioning the chip over the choosen column
						if (LMB())
						{
							short currentColumnCounter = columnCounter[i]++;
							destination = gridPos[currentColumnCounter][i];
							movingChip = 1;
							manageGridMap(mapToggle, gridMap, currentColumnCounter, i); // putting what happened on the console
							hang();
						}
					}
				}
			}

			if (ibuttonAutoHover(mainMenu, window))
			{
				currentMode = 'B';
				menuBackgroundShape.setPosition(0, 690);
				destinationMode = 'M';
				menuReset(vsPlayer, vsCOM, menuExit, movingAtoM, menuPos1);
				normalize(mainMenu);
				reset(Chip, columnCounter, currentChip, gridMap, win, mapToggle);
				hang();
			}
			break;

		case 'M':             // Main menu
			if (movingAtoM)
			{
				if (yMovingR(vsPlayer.shape, menuPos1, 'd', speedofgame) == 0 && yMovingR(vsCOM.shape, menuPos2, 'd', speedofgame) == 0 && yMovingR(menuExit.shape, menuPos3, 'd', speedofgame) == 0)
				{
					movingAtoM = 0;
				}
			}
			else if (ibuttonAutoHover(vsPlayer, window))
			{
				currentMode = 'F';
				inGamebackgroundShape.setPosition(0, -690);
				destinationMode = 'G';
				hang();
			}
			else if (ibuttonAutoHover(vsCOM, window))
			{
				currentMode = 'D';
				movingMtoD = 1;
				easy.shape.setPosition(menuPos1.x, -200);
				medium.shape.setPosition(menuPos1.x, -200);
				hard.shape.setPosition(menuPos1.x, -200);
				hang();
			}
			else if (ibuttonAutoHover(menuExit, window))
			{
				window.close();
			}
			break;
		case 'D':                 //Difficulty
			if (movingMtoD)
			{
				if (yMovingR(easy.shape, menuPos1, 'd', speedofgame) == 0 && yMovingR(medium.shape, menuPos2, 'd', speedofgame) == 0 && yMovingR(hard.shape, menuPos3, 'd', speedofgame) == 0)
				{
					movingMtoD = 0;
				}
			}
			else if (ibuttonAutoHover(easy, window))
			{
				currentMode = 'F';
				inGamebackgroundShape.setPosition(0, -690);
				destinationMode = 'C';
				difficulty = 3;
				hang();
			}
			else if (mediumLocked == 0 && ibuttonAutoHover(medium, window))
			{
				currentMode = 'F';
				inGamebackgroundShape.setPosition(0, -690);
				destinationMode = 'C';
				difficulty = 5;
				hang();
			}
			else if (hardLocked == 0 && ibuttonAutoHover(hard, window))
			{
				currentMode = 'F';
				inGamebackgroundShape.setPosition(0, -690);
				destinationMode = 'C';
				difficulty = 7;
				hang();
			}
			else if (ibuttonAutoHover(mainMenu, window))
			{
				currentMode = 'M';
				menuReset(vsPlayer, vsCOM, menuExit, movingAtoM, menuPos1);
				normalize(mainMenu);
				hang();
			}
			break;

		case 'F':        // Forward
			if (yMovingR(inGamebackgroundShape, sf::Vector2f(0.0, 0.0), 'd', speedofgame) == 0 || yMovingR(menuBackgroundShape, sf::Vector2f(0.0, 690.0), 'd', speedofgame) == 0)
			{
				currentMode = destinationMode;
				menuBackgroundShape.setPosition(0, 0);
			}
			break;
		case 'B':      // Backward
			if (yMovingR(inGamebackgroundShape, sf::Vector2f(0.0, -690), 'u', speedofgame) == 0 || yMovingR(menuBackgroundShape, sf::Vector2f(0.0, 0), 'u', speedofgame) == 0)
			{
				currentMode = destinationMode;
				inGamebackgroundShape.setPosition(0, 0);
			}
			break;

		case 'C':    // Game ... palyer Vs comp

			play(sound1, sound4, sound3, muted, mutedMusic, resetMusic, isplay1, currentMode, gridMap, currentWinner, destinationMode, movingChip, win, movingSub, mapToggle, movingAtoM, mediumLocked, hardLocked, Chip, subMenuShape, menuBackgroundShape, winShape, redWinSprite, yellowWinSprite, tieWinSprite, playAgain, dontPlayAgain, vsPlayer, vsCOM, menuExit, window, destination, subMenuDef, winShapeDef, menuPos1, currentChip, columnCounter, difficulty, speedofchip, speedofgame);
			if (!isplay1)
			{
				if (currentChip != 41)
					Chip[currentChip].setPosition(1500, 0);
				for (short i = 0; i<7; ++i)
				{
					if (mapToggle == 0 || (columnCounter[i]<6 && isButtonHoveredf3(Column[i], window)))
					{
						if (mapToggle == 0)
							i = COM(gridMap, difficulty);
						Chip[currentChip].setPosition(gridHoverPos[i]);
						if (LMB() || mapToggle == 0)
						{
							short currentColumnCounter = columnCounter[i]++;
							destination = gridPos[currentColumnCounter][i];
							movingChip = 1;
							manageGridMap(mapToggle, gridMap, currentColumnCounter, i);

							hang();
							break;
						}
					}
				}
			}

			if (ibuttonAutoHover(mainMenu, window))
			{
				currentMode = 'B';
				menuBackgroundShape.setPosition(0, 690);
				destinationMode = 'M';
				menuReset(vsPlayer, vsCOM, menuExit, movingAtoM, menuPos1);
				normalize(mainMenu);
				reset(Chip, columnCounter, currentChip, gridMap, win, mapToggle);
				hang();
			}
			break;
		}







		window.clear(sf::Color::White);


		switch (currentMode)
		{
		case 'G':
			pvpDraw(window, inGamebackgroundShape, Chip, grid, mainMenu, win, subMenuShape, winShape, movingSub, playAgain, dontPlayAgain);
			break;
		case 'M':
			window.draw(menuBackgroundShape);
			window.draw(vsPlayer.shape);
			window.draw(vsCOM.shape);
			window.draw(menuExit.shape);
			window.draw(titleShape);
			break;
		case 'D':
			window.draw(menuBackgroundShape);
			window.draw(mainMenu.shape);
			window.draw(titleShape);
			window.draw(easy.shape);
			window.draw(medium.shape);
			window.draw(hard.shape);
			break;
		case 'F':
			window.draw(menuBackgroundShape);
			window.draw(inGamebackgroundShape);
			break;
		case 'B':
			window.draw(inGamebackgroundShape);
			window.draw(menuBackgroundShape);
			break;
		case 'C':
			pvpDraw(window, inGamebackgroundShape, Chip, grid, mainMenu, win, subMenuShape, winShape, movingSub, playAgain, dontPlayAgain);
			break;
		}

		window.draw(muteShape);
		window.draw(muteMusicShape);

		musicStateCounter++;
		window.display();
	}

	return 0;
}