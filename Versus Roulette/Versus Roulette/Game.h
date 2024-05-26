/// <summary>
/// @author Tymoteusz Walichnowski
/// @author Nathan Mas
/// @author Natalia Ryl
/// @author Shane Moran
/// @date April 2024
/// </summary>
#ifndef GAME_HPP
#endif // !GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Globals.h"
#include "Player.h"
#include "Enemy.h"

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupVariables();
	void setupFontAndText();
	void setupSprite();
	void setupMenu();
	void setupInstructions();
	void setupGameplay();
	void setupInventory();
	void menuSelect();
	void gameplaySelect();
	void inventorySelect();
	void setupItems();
	void displayItemDescription(int t_slot);

	void shootSelf();
	void shootOpponent();

	void enemyShootSelf();
	void enemyShootOpponent();

	void loadTaser();
	void giveItems();
	void useItem(int t_user, int t_slot);
	void displayCurrentShot();

	void setupAudio();
	void setupHUD();
	void checkHealth();
	void setupGameOver();

	void restartGame();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	bool m_exitGame; // control exiting game
	bool playerWon = false; // True if the player wins, false if the player loses
	bool enemyWon = false; // True if the enemy wins, false if the enemy loses

	Player myPlayer;
	Enemy myEnemy;

	sf::Text displayPlayerHealth; // displays the players current health
	sf::Text displayEnemyHealth; // displays the enemys current health

	// inventory screen text
	sf::Text taserContentsMessage;
	sf::Text liveRoundsMessage;
	sf::Text blankRoundsMessage;

	//B to return
	sf::Text bButtonText;

	// gameplay elements
	bool playerTurn; // is it currently the player's turn?
	sf::Text currentTurnMessage; // text depicting whos turn it currently is

	int aiTurnTimer = 0; // temporary variable to showcase AI "thinking" and taking its turn

	// main menu buttons
	sf::RectangleShape startButton; // button that starts the game
	sf::RectangleShape instructionsButton; // button that takes you to instructions screen
	sf::RectangleShape exitButton; // button that exits the game

	// gameplay buttons
	sf::RectangleShape shootSelfButton; // button that causes the player to shoot themselves
	sf::RectangleShape shootOpponentButton; // button that causes the player to shoot the opponent AI
	sf::RectangleShape inventoryButton; // button that opens the inventory screen

	// inventory screen buttons
	sf::RectangleShape useButton; // button that uses items
	sf::Sprite slot1;
	sf::Sprite slot2;
	sf::Sprite slot3;
	sf::Sprite slot4;
	sf::Texture slotTexture;


	// Buttons sprite sheet texture
	sf::Texture buttonsTexture;
	sf::Texture inventoryButtonTexture;

	// main menu texture & sprite
	sf::Texture menuScreenTexture;
	sf::Sprite menuScreenSprite;

	sf::Texture gameLogoTexture;
	sf::Sprite gameLogoSprite;

	// Declare sprites for each inventory box
	sf::Sprite inventoryItemSpriteArray[MAX_ITEMS];
	sf::Sprite enemyItemSpriteArray[MAX_ITEMS];

	// item sheet texture
	sf::Texture itemSheetTexture;

	// sprites and textures for HID elements
	sf::Texture upperBarTexture;
	sf::Sprite upperBarSprite;

	sf::Texture tableTexture;
	sf::Sprite tableSprite;

	sf::Texture playerHealthBarTexture;
	sf::Sprite playerHealthBarSprite;

	sf::Texture enemyHealthBarTexture;
	sf::Sprite enemyHealthBarSprite;

	sf::Texture liveTaserTexture;
	sf::Sprite liveTaserSprite;

	sf::Texture emptyTaserTexture;
	sf::Sprite emptyTaserSprite;

	bool upArrowPressed; // up arrow check
	bool downArrowPressed; // down arrow check
	bool leftArrowPressed; // left arrow check
	bool rightArrowPressed; // right arrow check
	bool returnKeyPressed; // return key check
	bool iKeyPressed; // i key check
	bool bKeyPressed; // b key check

	int selectedButtonIndex; // selects play button by default

	int gameScreen; // the current active game screen, what's being shown to the player

	// instructions screen texture & sprite
	sf::Texture instructionsTexture;
	sf::Sprite instructionsSprite;

	sf::Texture inventoryScreenTexture;
	sf::Sprite inventoryScreenSprite;

	// gameplay screen texture & sprite
	sf::Texture gameplayTexture;
	sf::Sprite gameplaySprite;

	//gameplay variables
	bool roundStart; // has a round started?

	int playerHealth;
	int enemyHealth;

	int currentShot; // the current shot loaded into the taser
	int endTimer; //gives couple frames of leeway before endscreen is shown
	const int endGracePeriod = 60;

	// taser variables
	static const int MAX_SHOTS = 6;
	int taserArray[MAX_SHOTS];
	int currentLoadedShots;

	int liveRounds;
	int blankRounds;

	bool liveLoaded; // has at least one live shot been loaded into the taser?
	bool blankLoaded; // has at least one blank shot been loaded into the taser?

	//inventory array
	int itemsGiven;

	//items
	bool doubleDamage; 
	bool scannerActive;
	int scannerTimer;
	bool enemyPaused;
	sf::Sprite scannedShotSprite;

	//enemy ai
	bool knowItsBlank;
	bool knowItsLive;
	bool playerPaused;

	sf::SoundBuffer robotDamageTakenBuffer;
	sf::Sound robotDamageTakenSound;
	
	sf::SoundBuffer zapBuffer;
	sf::Sound zapSound;

	sf::SoundBuffer blankBuffer;
	sf::Sound blankSound;

	sf::Texture winTexture;
	sf::Sprite winSprite;
	sf::Texture loseTexture;
	sf::Sprite loseSprite;

	sf::SoundBuffer m_menuMusicLoad;
	sf::Sound m_menuMusic;

	sf::SoundBuffer m_gameplayMusicLoad;
	sf::Sound m_gameplayMusic;

	sf::SoundBuffer m_menuSelect;
	sf::Sound m_beep;

	sf::SoundBuffer m_menuChoose;
	sf::Sound m_boop;

	sf::SoundBuffer oilDrinkBuffer;
	sf::Sound oilDrinkSound;

	sf::SoundBuffer scannerBuffer;
	sf::Sound scannerSound;

	sf::SoundBuffer pauseRemoteBuffer;
	sf::Sound pauseRemoteSound;

	sf::SoundBuffer overchargerBuffer;
	sf::Sound overchargerSound;

	sf::SoundBuffer rubbishBinBuffer;
	sf::Sound rubbishBinSound;
};