/// <summary>
/// @author Tymoteusz Walichnowski
/// @author Nathan Mas
/// @author Natalia Ryl
/// @author Shane Moran
/// @date April 2024
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Globals.h"
#include "Game.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	srand(time(NULL)); // randomize seed
	setupVariables(); //sets up game logic variables
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupMenu(); // setup main menu
	setupInstructions(); // setup instructions screen
	setupGameplay(); // setup gameplay screen
	setupItems(); // setup inventory items
	setupInventory(); // setup inventory screen
	setupGameOver();
	setupAudio();
	setupHUD(); // Call the setupHUD function
	
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}

/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}

/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Up == t_event.key.code)
	{
		upArrowPressed = true;
		m_boop.play();
		switch (gameScreen)
		{
		case MAIN_MENU:
			menuSelect();
			break;
		case GAMEPLAY:
			gameplaySelect();
			break;
		case INSTRUCTIONS:
			break;
		case INVENTORY:
			inventorySelect();
			break;
		case GAME_OVER:
			break;
		}
	}
	if (sf::Keyboard::Down == t_event.key.code)
	{
		downArrowPressed = true;
		m_boop.play();
		switch (gameScreen)
		{
		case MAIN_MENU:
			menuSelect();
			break;
		case GAMEPLAY:
			gameplaySelect();
			break;
		case INSTRUCTIONS:
			break;
		case INVENTORY:
			inventorySelect();
			break;
		case GAME_OVER:
			break;
		}
	}

	if (sf::Keyboard::Left == t_event.key.code)
	{
		leftArrowPressed = true;
		m_boop.play();
		switch (gameScreen)
		{
		case INVENTORY:
			inventorySelect();
			break;
		}
	}

	if (sf::Keyboard::Right == t_event.key.code)
	{
		rightArrowPressed = true;
		m_boop.play();
		switch (gameScreen)
		{
		case INVENTORY:
			inventorySelect();
			break;
		}
	}

	if (sf::Keyboard::Return == t_event.key.code)
	{
		returnKeyPressed = true;
		m_beep.play();
		switch (gameScreen)
		{
		case MAIN_MENU:
			menuSelect();
			break;
		case GAMEPLAY:
			gameplaySelect();
			break;
		case INSTRUCTIONS:
			break;
		case INVENTORY:
			inventorySelect();
			break;
		case GAME_OVER:
			break;
		}
	}

	if (sf::Keyboard::B == t_event.key.code)
	{
		bKeyPressed = true;
		switch (gameScreen)
		{
		case INSTRUCTIONS:
			gameScreen = MAIN_MENU; // If 'B' is pressed on the instructions screen, go back to the main menu.
			break;
		case INVENTORY:
 			inventorySelect();
			gameScreen = GAMEPLAY;
			break;
		case GAME_OVER:
			gameScreen = MAIN_MENU;
			break;
			// Add similar cases for other screens as needed.
		}
	}

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	// main menu screen code
	if (gameScreen == MAIN_MENU)
	{
		restartGame();
	}
	// gameplay screen code
	else if (gameScreen == GAMEPLAY)
	{
		displayPlayerHealth.setString("Player Health: " + std::to_string(myPlayer.getHealth()));
		displayEnemyHealth.setString("Enemy Health: " + std::to_string(myEnemy.getHealth()));

		//new round once taser is out of ammo
		if (currentLoadedShots == 0)
		{
			roundStart = true;
		}

		// Actions if round has just started
		if (roundStart == true)
		{
			//loads taser, gives items and round starts on player's turn
			loadTaser();
			giveItems();
			playerTurn = true;
			roundStart = false;
		}

		// Player's turn
		if (playerTurn == true)
		{
			currentTurnMessage.setString("Player Turn");
		}

		// AI's turn
		else
		{
			// ai runs a timer so it doesn't all just happen in one frame.
			aiTurnTimer++;
			currentTurnMessage.setString("Opponent Turn");
			
			//USING ITEMS
			if (aiTurnTimer > 30)
			{
				for (int index = 0; index < MAX_ITEMS; index++) //checks for oil drinks and uses them
				{
					if (myEnemy.getInventoryArray(index) == OIL_DRINK)
					{
						useItem(1, index);
					}
				}
			}
			if (aiTurnTimer > 60)
			{
				for (int index = 0; index < MAX_ITEMS; index++) //checks for rubbish bins and uses them if more live shots than blanks
				{
					if (myEnemy.getInventoryArray(index) == RUBBISH_BIN && liveRounds > blankRounds)
					{
						useItem(1, index);
					}
				}
			}
			if (aiTurnTimer > 90)
			{
				for (int index = 0; index < MAX_ITEMS; index++) //checks for pause remotes and uses them
				{
					if (myEnemy.getInventoryArray(index) == PAUSE_REMOTE)
					{
						useItem(1, index);
					}
				}
			}
			if (aiTurnTimer > 120)
			{
				for (int index = 0; index < MAX_ITEMS; index++) //checks for scanners and uses them
				{
					if (myEnemy.getInventoryArray(index) == SCANNER)
					{
						useItem(1, index);
					}
				}
			}
			if (aiTurnTimer > 150)
			{
				for (int index = 0; index < MAX_ITEMS; index++) //checks for overchargers and uses them if it knows it has a live shot
				{
					if (myEnemy.getInventoryArray(index) == OVERCHARGER && knowItsLive == true)
					{
						useItem(1, index);
					}
				}
			}
			if (aiTurnTimer > 180)
			{
				if (playerTurn == false) // wont do anything if using rubbish bin caused it to be player turn
				{
					if (blankRounds >= liveRounds || knowItsBlank == true) //enemy only shoots itself if it has more or equal blank shots than lives, or it knows its a blank
					{
						enemyShootSelf();
					}
					if (blankRounds < liveRounds || knowItsLive) // shoots opponent if it knows its current shot is a blank
					{
						enemyShootOpponent();
						if (playerPaused == true) //pause remote item
						{
							playerTurn = false;
							playerPaused = false;
						}
						else
						{
							playerTurn = true;
						}
					}
					aiTurnTimer = 0;
					knowItsBlank = false;
					knowItsLive = false;
				}
			}
		}

		//playing animations
		//playing current player animation
		if (myPlayer.getAnimationPlaying() == true)
		{
			myPlayer.playAnimation();
		}
		//playing current enemy animation
		if (myEnemy.getAnimationPlaying() == true)
		{
			myEnemy.playAnimation();
		}

		if (myPlayer.getHealth() <= 0) 
		{
			enemyWon = true;
			playerWon = false;
			endTimer++;
			if (endTimer > endGracePeriod) // couple frames before screen is displayed
			{
				gameScreen = GAME_OVER;
				endTimer = 0;
			}
		}
		else if (myEnemy.getHealth() <= 0) 
		{
			playerWon = true;
			enemyWon = false;
			endTimer++;
			if (endTimer > endGracePeriod) // couple frames before screen is displayed
			{
				gameScreen = GAME_OVER;
				endTimer = 0;
			}
		}

		checkHealth();
	}

	//instructions screen code
	else if (gameScreen == INSTRUCTIONS)
	{

	}
	//inventory screen code
	else if (gameScreen == INVENTORY)
	{
		inventorySelect();

		displayItemDescription(selectedButtonIndex);

		if (scannerActive == true) //scanner item
		{
			displayCurrentShot();
			scannerTimer--;
			if (scannerTimer <= 0)
			{
				scannerActive = false;
			}
		}
	}

	else if (gameScreen == GAME_OVER)
	{
		
	}

	// Handle music for Main Menu and Instructions
	if (gameScreen == MAIN_MENU || gameScreen == INSTRUCTIONS)
	{
		if (m_menuMusic.getStatus() != sf::Music::Playing)
		{
			std::cout << "Starting menu music." << std::endl;
			m_menuMusic.play();
		}
		if (m_gameplayMusic.getStatus() == sf::Music::Playing)
		{
			std::cout << "Stopping gameplay music." << std::endl;
			m_gameplayMusic.stop();
		}
	}
	else if (gameScreen == GAMEPLAY || gameScreen == INVENTORY) // Handle music for Gameplay and Inventory
	{
		if (m_gameplayMusic.getStatus() != sf::Music::Playing)
		{
			std::cout << "Starting gameplay music." << std::endl;
			m_gameplayMusic.play();
		}
		if (m_menuMusic.getStatus() == sf::Music::Playing)
		{
			std::cout << "Stopping menu music." << std::endl;
			m_menuMusic.stop();
		}
	}
	else // For other screens where no music should play
	{
		if (m_menuMusic.getStatus() == sf::Music::Playing)
		{
			std::cout << "Stopping menu music (other screen)." << std::endl;
			m_menuMusic.stop();
		}
		if (m_gameplayMusic.getStatus() == sf::Music::Playing)
		{
			std::cout << "Stopping gameplay music (other screen)." << std::endl;
			m_gameplayMusic.stop();
		}
	}

	if (m_exitGame)
	{
		m_window.close();
	}

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	//drawing main menu elements
	if (gameScreen == MAIN_MENU)
	{
		m_window.draw(menuScreenSprite);
		m_window.draw(startButton);
		m_window.draw(instructionsButton);
		m_window.draw(exitButton);
		m_window.draw(gameLogoSprite);

		startButton.setSize(sf::Vector2f(256, 128));
		startButton.setPosition(500, 200);

		instructionsButton.setSize(sf::Vector2f(256, 128));
		instructionsButton.setPosition(500, 300);

		exitButton.setSize(sf::Vector2f(256, 128));
		exitButton.setPosition(500, 400);

		gameLogoSprite.setPosition(SCREEN_WIDTH / 2 - 75, 10);
	}

	//drawing gameplay screen elements
	else if (gameScreen == GAMEPLAY)
	{
		m_window.draw(gameplaySprite);
		m_window.draw(upperBarSprite);
		m_window.draw(tableSprite);
		m_window.draw(playerHealthBarSprite);
		m_window.draw(enemyHealthBarSprite);

		m_window.draw(myPlayer.getBody());
		m_window.draw(myEnemy.getBody());

		m_window.draw(shootSelfButton);
		m_window.draw(shootOpponentButton);
		m_window.draw(inventoryButton);

		m_window.draw(liveTaserSprite);
		m_window.draw(emptyTaserSprite);

		shootSelfButton.setSize(sf::Vector2f(256, 128));
		shootSelfButton.setPosition(275, 70);

		shootOpponentButton.setSize(sf::Vector2f(256, 128));
		shootOpponentButton.setPosition(275, 170);

		inventoryButton.setSize(sf::Vector2f(256, 128));
		inventoryButton.setPosition(275, 270);

		m_window.draw(taserContentsMessage);

		m_window.draw(liveRoundsMessage);
		m_window.draw(blankRoundsMessage);
		m_window.draw(displayPlayerHealth);
		m_window.draw(displayEnemyHealth);
		m_window.draw(currentTurnMessage);

		sf::Vector2f playerItemPositions[MAX_ITEMS] = 
		{
			{292, 470}, // Coordinates for the first item
			{280,505}, // Coordinates for the second item
			{217, 470},  // Coordinates for the third item
			{205, 505}        // Coordinates for the fourth item
		};

		// Display player's inventory items
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			if (myPlayer.getInventoryArray(i) != 0) // Check if the slot is not empty
			{
				sf::Sprite itemSprite = inventoryItemSpriteArray[i];
				itemSprite.setScale(0.5f, 0.5f); // Scale down by half
				itemSprite.setPosition(playerItemPositions[i]); 
				m_window.draw(itemSprite);
			}
		}

		sf::Vector2f enemyItemPositions[MAX_ITEMS] = 
		{
			{477, 470}, // Coordinates for the first item
			{494,505}, // Coordinates for the second item
			{552, 470},  // Coordinates for the third item
			{567, 505} // Coordinates for the fourth item
		};

		// Display enemy's inventory items
		for (int i = 0; i < MAX_ITEMS; i++)
		{
			if (myEnemy.getInventoryArray(i) != 0) // Check if the slot is not empty
			{
				sf::Sprite itemSprite = enemyItemSpriteArray[i];
				itemSprite.setScale(0.5f, 0.5f); // Scale down by half
				itemSprite.setPosition(enemyItemPositions[i]); // Use predefined position
				m_window.draw(itemSprite);
			}
		}
	}

	//drawing instructions screen elements
	else if (gameScreen == INSTRUCTIONS)
	{
		m_window.draw(instructionsSprite);
		m_window.draw(bButtonText);
	}

	//drawing inventory screen elements
	else if (gameScreen == INVENTORY)
	{
		m_window.draw(inventoryScreenSprite);
		m_window.draw(upperBarSprite);


		m_window.draw(taserContentsMessage);
		m_window.draw(liveRoundsMessage);
		m_window.draw(blankRoundsMessage);

		m_window.draw(slot1);
		m_window.draw(slot2);
		m_window.draw(slot3);
		m_window.draw(slot4);

		// Drawing the sprites for inventory box items
		for (int index = 0; index < MAX_ITEMS; index++)
		{
			m_window.draw(inventoryItemSpriteArray[index]);
		}

		// Drawing the sprites for inventory boxes
		m_window.draw(bButtonText);
		m_window.draw(liveTaserSprite);
		m_window.draw(emptyTaserSprite);
	}

	else if (gameScreen == GAME_OVER) 
	{	
		if (playerWon) 
		{
			m_window.draw(winSprite);
		}
		else if (enemyWon) 
		{
			m_window.draw(loseSprite);
		}
		m_window.draw(bButtonText);
	}

	if (scannerActive && gameScreen == INVENTORY) // draw sprite of currently scanned shot when on inventory screen
	{
		m_window.draw(scannedShotSprite);
	}
	m_window.display();
}

/// <summary>
/// sets up the variables to be used within the game
/// </summary>
void Game::setupVariables()
{
	//menu and selection
	upArrowPressed = false;
	downArrowPressed = false;
	leftArrowPressed = false;
	rightArrowPressed = false;
	returnKeyPressed = false;
	iKeyPressed = false;
	selectedButtonIndex = 0;

	//game screens
	gameScreen = MAIN_MENU;

	//gameplay variables
	playerTurn = true;
	roundStart = false;

	playerHealth = 5;
	enemyHealth = 5;

	currentShot = 0;
	currentLoadedShots = 0;

	liveRounds = 0;
	blankRounds = 0;

	liveLoaded = false;
	blankLoaded = false;

	itemsGiven = 0;

	doubleDamage = false;
	scannerActive = false;
	scannerTimer = 0;
	enemyPaused = false;

	endTimer = 0;
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	//current turn text
	currentTurnMessage.setFont(m_ArialBlackfont);
	currentTurnMessage.setString("Player Turn");
	currentTurnMessage.setCharacterSize(30U);
	currentTurnMessage.setStyle(sf::Text::Italic | sf::Text::Bold);
	currentTurnMessage.setFillColor(sf::Color::White);
	currentTurnMessage.setPosition(180.0f, 45.0f);

	//taser contents text
	taserContentsMessage.setFont(m_ArialBlackfont);
	taserContentsMessage.setString("Taser Contents: ");
	taserContentsMessage.setCharacterSize(25U);
	taserContentsMessage.setStyle(sf::Text::Italic | sf::Text::Bold);
	taserContentsMessage.setFillColor(sf::Color::White);
	taserContentsMessage.setPosition(15.0f, 15.0f);

	//live rounds text
	liveRoundsMessage.setFont(m_ArialBlackfont);
	liveRoundsMessage.setCharacterSize(25U);
	liveRoundsMessage.setStyle(sf::Text::Italic | sf::Text::Bold);
	liveRoundsMessage.setFillColor(sf::Color::White);
	liveRoundsMessage.setPosition(470.0f, 15.0f);
	liveRoundsMessage.setString(std::to_string(liveRounds));


	//blank rounds text
	blankRoundsMessage.setFont(m_ArialBlackfont);
	blankRoundsMessage.setCharacterSize(25U);
	blankRoundsMessage.setStyle(sf::Text::Italic | sf::Text::Bold);
	blankRoundsMessage.setFillColor(sf::Color::White);
	blankRoundsMessage.setPosition(600.0f, 15.0f);
	blankRoundsMessage.setString(std::to_string(blankRounds));

	//B button inventory text
	bButtonText.setFont(m_ArialBlackfont);
	bButtonText.setString("Press B to return");
	bButtonText.setCharacterSize(25U);
	bButtonText.setStyle(sf::Text::Italic | sf::Text::Bold);
	bButtonText.setFillColor(sf::Color::Black);
	bButtonText.setPosition(10.0f, 560.0f);
}


/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!gameLogoTexture.loadFromFile("ASSETS\\IMAGES\\versusRouletteLogo.png"))
	{
		std::cout << "Failed to load game logo image!" << std::endl;
	}

	gameLogoSprite.setTexture(gameLogoTexture);
}

/// <summary>
/// sets up menu elements
/// </summary>
void Game::setupMenu()
{
	if (!menuScreenTexture.loadFromFile("ASSETS\\IMAGES\\main screen.png"))
	{
		std::cout << "Failed to menu screen image!" << std::endl;
	}
	menuScreenSprite.setTexture(menuScreenTexture);

	if (!buttonsTexture.loadFromFile("ASSETS\\IMAGES\\Button-Sheet.png"))
	{
		std::cout << "Failed to load button image!" << std::endl;
	}

	sf::IntRect playButtonRect(1408, 0, 128, 64);
	sf::IntRect instructionsButtonRect(512, 0, 128, 64);
	sf::IntRect exitButtonRect(256, 0, 128, 64);

	startButton.setTexture(&buttonsTexture);
	startButton.setTextureRect(playButtonRect);

	instructionsButton.setTexture(&buttonsTexture);
	instructionsButton.setTextureRect(instructionsButtonRect);

	exitButton.setTexture(&buttonsTexture);
	exitButton.setTextureRect(exitButtonRect);
}

void Game::setupInstructions()
{
	if (!instructionsTexture.loadFromFile("ASSETS\\IMAGES\\instructions screen.png"))
	{
		std::cout << "Failed to load instructions screen!" << std::endl;
	}
	instructionsSprite.setTexture(instructionsTexture);
}

/// <summary>
/// sets up elements on gameplay screen
/// </summary>
void Game::setupGameplay()
{
	if (!gameplayTexture.loadFromFile("ASSETS\\IMAGES\\gameplay screen.png"))
	{
		std::cout << "Failed to gameplay screen image!" << std::endl;
	}
	gameplaySprite.setTexture(gameplayTexture);

	if (!buttonsTexture.loadFromFile("ASSETS\\IMAGES\\Button-Sheet.png"))
	{
		std::cout << "Failed to load button image!" << std::endl;
	}

	sf::IntRect shootSelfButtonRect(1920, 0, 128, 64);
	sf::IntRect shootOpponentButtonRect(1536, 0, 128, 64);
	sf::IntRect inventoryButtonRect(1024, 0, 128, 64);

	shootSelfButton.setTexture(&buttonsTexture);
	shootSelfButton.setTextureRect(shootSelfButtonRect);

	shootOpponentButton.setTexture(&buttonsTexture);
	shootOpponentButton.setTextureRect(shootOpponentButtonRect);

	inventoryButton.setTexture(&buttonsTexture);
	inventoryButton.setTextureRect(inventoryButtonRect);
}

/// <summary>
/// sets up elements on inventory screen
/// </summary>
void Game::setupInventory()
{
	//setting up inventories
	for (int index = 0; index < MAX_ITEMS; index++)
	{
		myPlayer.setInventoryArray(index, 0);
		myEnemy.setInventoryArray(index, 0);
	}

	if (!inventoryButtonTexture.loadFromFile("ASSETS\\IMAGES\\Item-Sheet.png"))
	{
		std::cout << "Error loading inventory buttons";
	}

	sf::IntRect OilDrinkButtonRect(0, 0, 64, 64);
	sf::IntRect PauseRemoteButtonRect(256, 0, 64, 64);
	sf::IntRect BatteryButtonRect(384, 0, 64, 64);
	sf::IntRect TaserButtonRect(128, 0, 64, 64);

	if (!slotTexture.loadFromFile("ASSETS\\IMAGES\\slots.png"))
	{
		std::cout << "Error loading inventory slots";
	}

	sf::IntRect firstItemSlot(0, 0, 64, 64); // displays item in first slot
	sf::IntRect secondItemSlot(0, 0, 64, 64); // displays item in second slot
	sf::IntRect thirdItemSlot(0, 0, 64, 64); // displays item in third slot
	sf::IntRect fourthItemSlot(0, 0, 64, 64); // displays item in fourth slot

	// first slot 
	slot1.setTexture(slotTexture);
	slot1.setTextureRect(firstItemSlot);
	slot1.setPosition(460.0f, 200.0f);
	slot1.setScale(1.5f, 1.5f);

	// second slot 
	slot2.setTexture(slotTexture);
	slot2.setTextureRect(secondItemSlot);
	slot2.setPosition(620.0f, 200.0f);
	slot2.setScale(1.5f, 1.5f);

	// third slot 
	slot3.setTexture(slotTexture);
	slot3.setTextureRect(thirdItemSlot);
	slot3.setPosition(460.0f, 360.0f);
	slot3.setScale(1.5f, 1.5f);

	// fourth slot 
	slot4.setTexture(slotTexture);
	slot4.setTextureRect(fourthItemSlot);
	slot4.setPosition(620.0f, 360.0f);
	slot4.setScale(1.5f, 1.5f);

	if (!inventoryScreenTexture.loadFromFile("ASSETS\\IMAGES\\inventory screen.png"))
	{
		std::cout << "Error loading inventory screen";
	}
	inventoryScreenSprite.setTexture(inventoryScreenTexture);

}

/// <summary>
/// controls selection of main menu buttons
/// </summary>
void Game::menuSelect()
{
	if (upArrowPressed)
	{
		selectedButtonIndex = (selectedButtonIndex - 1 + 3) % 3; // moves to the last button
		upArrowPressed = false;
	}

	if (downArrowPressed)
	{
		selectedButtonIndex = (selectedButtonIndex + 1) % 3; // moves to the next button
		downArrowPressed = false;
	}

	// Load button texture
	if (!buttonsTexture.loadFromFile("ASSETS\\IMAGES\\Button-Sheet.png"))
	{
		std::cout << "Failed to load button image!" << std::endl;
	}

	// Initialize button rectangles
	sf::IntRect originalPlayButtonRect(1280, 0, 128, 64);
	sf::IntRect originalInstructionsButtonRect(512, 0, 128, 64);
	sf::IntRect originalExitButtonRect(256, 0, 128, 64);

	sf::IntRect playButtonRect = originalPlayButtonRect;
	sf::IntRect instructionsButtonRect = originalInstructionsButtonRect;
	sf::IntRect exitButtonRect = originalExitButtonRect;

	// changes button colours depending which one is selected
	switch (selectedButtonIndex)
	{
	case 0:
		playButtonRect.left = 1408; // Adjust the left coordinate

		startButton.setTextureRect(playButtonRect);

		startButton.setSize(sf::Vector2f(256, 128));
		startButton.setPosition(500, 200);

		// Reset other buttons to their original state
		instructionsButton.setTextureRect(originalInstructionsButtonRect);
		exitButton.setTextureRect(originalExitButtonRect);

		break;
	case 1:
		instructionsButtonRect.left = 640;

		instructionsButton.setTextureRect(instructionsButtonRect);

		instructionsButton.setSize(sf::Vector2f(256, 128));
		instructionsButton.setPosition(500, 300);

		// Reset other buttons to their original state
		startButton.setTextureRect(originalPlayButtonRect);
		exitButton.setTextureRect(originalExitButtonRect);
		break;
	case 2:
		exitButtonRect.left = 384;

		exitButton.setTextureRect(exitButtonRect);

		exitButton.setSize(sf::Vector2f(256, 128));
		exitButton.setPosition(500, 400);

		// Reset other buttons to their original state
		startButton.setTextureRect(originalPlayButtonRect);
		instructionsButton.setTextureRect(originalInstructionsButtonRect);
		break;
	}

	if (returnKeyPressed)
	{

		switch (selectedButtonIndex)
		{
		case 0: // the play button
			gameScreen = GAMEPLAY;  // will begin gameplay
			roundStart = true;
			break;
		case 1: // the instructions button
			gameScreen = INSTRUCTIONS;// will display image for instructions
			break;
		case 2: // the exit button
			m_exitGame = true; // will exit the game
			break;
		}

		returnKeyPressed = false;
	}
}

/// <summary>
/// controls selection of gameplay menu buttons
/// </summary>
void Game::gameplaySelect()
{
	if (upArrowPressed)
	{
		selectedButtonIndex = (selectedButtonIndex - 1 + 3) % 3; // moves to the last button
		upArrowPressed = false;
	}

	if (downArrowPressed)
	{
		selectedButtonIndex = (selectedButtonIndex + 1) % 3; // moves to the next button
		downArrowPressed = false;
	}

	if (!buttonsTexture.loadFromFile("ASSETS\\IMAGES\\Button-Sheet.png"))
	{
		std::cout << "Failed to load button image!" << std::endl;
	}

	// Initialize button rectangles
	sf::IntRect originalShootSelfButtonRect(1792, 0, 128, 64);
	sf::IntRect originalShootOpponentButtonRect(1536, 0, 128, 64);
	sf::IntRect originalInventoryButtonRect(1024, 0, 128, 64);

	sf::IntRect shootSelfButtonRect = originalShootSelfButtonRect;
	sf::IntRect shootOpponentButtonRect = originalShootOpponentButtonRect;
	sf::IntRect inventoryButtonRect = originalInventoryButtonRect;

	// changes button colours depending which one is selected
	switch (selectedButtonIndex)
	{
	case 0:
		shootSelfButtonRect.left = 1920; // Adjust the left coordinate

		shootSelfButton.setTextureRect(shootSelfButtonRect);

		shootSelfButton.setSize(sf::Vector2f(256, 128));
		shootSelfButton.setPosition(275, 200);

		// Reset other buttons to their original state
		shootOpponentButton.setTextureRect(originalShootOpponentButtonRect);
		inventoryButton.setTextureRect(originalInventoryButtonRect);
		break;
	case 1:
		shootOpponentButtonRect.left = 1664; // Adjust the left coordinate

		shootOpponentButton.setTextureRect(shootOpponentButtonRect);

		shootOpponentButton.setSize(sf::Vector2f(256, 128));
		shootOpponentButton.setPosition(275, 300);

		// Reset other buttons to their original state
		shootSelfButton.setTextureRect(originalShootSelfButtonRect);
		inventoryButton.setTextureRect(originalInventoryButtonRect);
		break;
	case 2:
		inventoryButtonRect.left = 1152; // Adjust the left coordinate

		inventoryButton.setTextureRect(inventoryButtonRect);

		inventoryButton.setSize(sf::Vector2f(256, 128));
		inventoryButton.setPosition(275, 400);

		// Reset other buttons to their original state
		shootSelfButton.setTextureRect(originalShootSelfButtonRect);
		shootOpponentButton.setTextureRect(originalShootOpponentButtonRect);
		break;
	}

	// player selects one of their gameplay options, makes sure it's the player's turn
	if (returnKeyPressed && playerTurn == true)
	{
		switch (selectedButtonIndex)
		{
		case 0: // the shoot self button
			shootSelf();
			break;
		case 1: // the shoot opponent button
			shootOpponent();
			if (enemyPaused == true) //pause remote
			{
				enemyPaused = false;
			}
			else
			{
				playerTurn = false; // ends player turn
			}
			
			break;
		case 2: // the inventory button
			gameScreen = INVENTORY;
			break;
		}


	}

	returnKeyPressed = false;
}

/// <summary>
/// controls selection of inventory boxes and buttons
/// </summary>
void Game::inventorySelect() 
{
	if (upArrowPressed) 
	{
		if (selectedButtonIndex == 0 || selectedButtonIndex == 1) 
		{
			// Do nothing if already in the top row
		}
		else {
			selectedButtonIndex -= 2;
		}
		upArrowPressed = false;
	}
	else if (downArrowPressed) 
	{
		if (selectedButtonIndex == 2 || selectedButtonIndex == 3) {
			// Do nothing if already in the bottom row
		}
		else 
		{
			selectedButtonIndex += 2;
		}
		downArrowPressed = false;
	}
	else if (leftArrowPressed) 
	{
		if (selectedButtonIndex == 0 || selectedButtonIndex == 2) 
		{
			// Do nothing if already in the leftmost column
		}
		else 
		{
			selectedButtonIndex--;
		}
		leftArrowPressed = false;
	}
	else if (rightArrowPressed) 
	{
		if (selectedButtonIndex == 1 || selectedButtonIndex == 3) 
		{
			// Do nothing if already in the rightmost column
		}
		else {
			selectedButtonIndex++;
		}
		rightArrowPressed = false;
	}

	// Set colors for items and buttons
	slot1.setTextureRect(selectedButtonIndex == 0 ? sf::IntRect(0,0,64,64) : sf::IntRect(65, 0, 64, 64));
	slot2.setTextureRect(selectedButtonIndex == 1 ? sf::IntRect(0, 0, 64, 64) : sf::IntRect(65, 0, 64, 64));
	slot3.setTextureRect(selectedButtonIndex == 2 ? sf::IntRect(0, 0, 64, 64) : sf::IntRect(65, 0, 64, 64));
	slot4.setTextureRect(selectedButtonIndex == 3 ? sf::IntRect(0, 0, 64, 64) : sf::IntRect(65, 0, 64, 64));

	// Handle action when return key is pressed
	if (returnKeyPressed) 
	{
		switch (selectedButtonIndex) 
		{
		case 0:
			// Handle action for first item slot
			useItem(0, 0);
			break;
		case 1:
			// Handle action for second item slot
			useItem(0, 1);
			break;
		case 2:
			// Handle action for third item slot
			useItem(0, 2);
			break;
		case 3:
			// Handle action for fourth item slot
			useItem(0, 3);
			break;
		}
		returnKeyPressed = false;
	}

	if (bKeyPressed == true)
	{
		gameScreen = GAMEPLAY;
		bKeyPressed = false;
	}

}

/// <summary>
/// player robot shoots themselves with taser
/// </summary>
void Game::shootSelf()
{
	if (taserArray[currentShot] == 1) // player takes damage and turn ends if live shot, otherwise turn will continue
	{
		myPlayer.setHealth(-1);
		liveRounds--;
		liveRoundsMessage.setString(std::to_string(liveRounds));
		myPlayer.setAnimationPlaying(true, SHOOT_SELF_LIVE);
		robotDamageTakenSound.play();
		zapSound.play();

		if (doubleDamage == true) //overcharger item
		{
			myPlayer.setHealth(-1);
		}

		if (enemyPaused == true) // pause remote item
		{
			playerTurn = true;
			enemyPaused = false;
		}
		else
		{
			playerTurn = false;
		}
		
	}
	else
	{
		blankRounds--;
		blankRoundsMessage.setString(std::to_string(blankRounds));
		myPlayer.setAnimationPlaying(true, SHOOT_SELF_BLANK);
		blankSound.play();
	}
	currentLoadedShots--;
	currentShot--;
	doubleDamage = false;
}

/// <summary>
/// player robot shoots opponent with taser
/// </summary>
void Game::shootOpponent()
{
	if (taserArray[currentShot] == 1) // opponent takes damage if live shot
	{
		myEnemy.setHealth(-1);
		liveRounds--;
		liveRoundsMessage.setString(std::to_string(liveRounds));
		myPlayer.setAnimationPlaying(true, SHOOT_OPPONENT_LIVE);
		myEnemy.setAnimationPlaying(true, GETTING_HIT);
		robotDamageTakenSound.play();
		zapSound.play();

		if (doubleDamage == true) //overcharger item
		{
			myEnemy.setHealth(-1);
		}
	}
	else
	{
		blankRounds--;
		blankRoundsMessage.setString(std::to_string(blankRounds));
		myPlayer.setAnimationPlaying(true, SHOOT_OPPONENT_BLANK);
		blankSound.play();
	}

	currentLoadedShots--;
	currentShot--;
	doubleDamage = false;
}

/// <summary>
/// enemy robot shoots themselves with taser
/// </summary>
void Game::enemyShootSelf()
{
	if (taserArray[currentShot] == 1) // opponent takes damage if live shot
	{
		myEnemy.setHealth(-1);
		liveRounds--;
		liveRoundsMessage.setString(std::to_string(liveRounds));
		myEnemy.setAnimationPlaying(true, SHOOT_SELF_LIVE);
		if (playerPaused == true) //pause remote item
		{
			playerPaused = false;
		}
		else
		{
			playerTurn = true;
		}
		robotDamageTakenSound.play();
		zapSound.play();

		if (doubleDamage == true) //overcharger item
		{
			myEnemy.setHealth(-1);
		}
	}
	else
	{
		blankRounds--;
		blankRoundsMessage.setString(std::to_string(blankRounds));
		myEnemy.setAnimationPlaying(true, SHOOT_SELF_BLANK);
		playerTurn = false;
		blankSound.play();
	}
	currentLoadedShots--;
	currentShot--;
	doubleDamage = false;
}

/// <summary>
/// enemy robot shoots player with taser
/// </summary>
void Game::enemyShootOpponent()
{
	if (taserArray[currentShot] == 1) // player takes damage if live shot
	{
		myPlayer.setHealth(-1);
		liveRounds--;
		liveRoundsMessage.setString(std::to_string(liveRounds));
		playerTurn = true;
		myEnemy.setAnimationPlaying(true, SHOOT_OPPONENT_LIVE);
		myPlayer.setAnimationPlaying(true, GETTING_HIT);
		robotDamageTakenSound.play();
		zapSound.play();

		if (doubleDamage == true) 	//overcharger item
		{
			myPlayer.setHealth(-1);
		}
	}
	else
	{
		blankRounds--;
		blankRoundsMessage.setString(std::to_string(blankRounds));
		playerTurn = true;
		myEnemy.setAnimationPlaying(true, SHOOT_OPPONENT_BLANK);
		blankSound.play();
	}
	currentLoadedShots--;
	currentShot--;

	doubleDamage = false;
}

/// <summary>
/// randomly loads taser contents
/// </summary>
void Game::loadTaser()
{
	// blank and live shots not loaded yet
	blankLoaded = false;
	liveLoaded = false;
	blankRounds = 0;
	liveRounds = 0;

	for (int index = 0; index < 6; index++)
	{
		int numberGen = (rand() % 2); //randomly generates number 0-1
		taserArray[index] = numberGen; //loads random blank or live into taser

		if (numberGen == 0) // loaded blank
		{
			blankRounds++;
			blankRoundsMessage.setString(std::to_string(blankRounds));
			blankLoaded = true;
		}
		else if (numberGen == 1) // loaded live
		{
			liveRounds++;
			liveRoundsMessage.setString(std::to_string(liveRounds));
			liveLoaded = true;
		}

		currentLoadedShots = 6; //there are 6 shots in the taser
		currentShot = 5; //current shot is 6th shot (goes 5,4,3,2,1,0)
	}

	// checking if gun was loaded with at least one live and one blank shot within, reloads the code if not
	if (blankLoaded == false || liveLoaded == false)
	{
		loadTaser();
	}
}

/// <summary>
/// gives player and enemy items at start of round
/// </summary>
void Game::giveItems()
{
	//giving player items
	itemsGiven = 0;
	for (int index = 0; index < 4; index++)
	{
		int numberGen = (rand() % 5 + 1); //randomly generates number 1-5

		if (myPlayer.getInventoryArray(index) == 0 && itemsGiven < 2) //slot not currently being used and two items not been given yet
		{
			myPlayer.setInventoryArray(index, numberGen);
			itemsGiven++;
			//displaying item sprites
			switch (numberGen)
			{
			case 1:
				inventoryItemSpriteArray[index].setTextureRect(OIL_DRINK_RECT);
				break;
			case 2:
				inventoryItemSpriteArray[index].setTextureRect(SCANNER_RECT);
				break;
			case 3:
				inventoryItemSpriteArray[index].setTextureRect(PAUSE_REMOTE_RECT);
				break;
			case 4:
				inventoryItemSpriteArray[index].setTextureRect(OVERCHARGER_RECT);
				break;
			case 5:
				inventoryItemSpriteArray[index].setTextureRect(RUBBISH_BIN_RECT);
				break;
			}
		}
	}

	//giving enemy items
	itemsGiven = 0;
	for (int index = 0; index < 4; index++)
	{
		int numberGen = (rand() % 5 + 1); //randomly generates number 1-5

		if (myEnemy.getInventoryArray(index) == 0 && itemsGiven < 2) //slot not currently being used and two items not been given yet
		{
			myEnemy.setInventoryArray(index, numberGen);
			itemsGiven++;
			//displaying item sprites
			switch (numberGen)
			{
			case 1:
				enemyItemSpriteArray[index].setTextureRect(OIL_DRINK_RECT);
				break;
			case 2:
				enemyItemSpriteArray[index].setTextureRect(SCANNER_RECT);
				break;
			case 3:
				enemyItemSpriteArray[index].setTextureRect(PAUSE_REMOTE_RECT);
				break;
			case 4:
				enemyItemSpriteArray[index].setTextureRect(OVERCHARGER_RECT);
				break;
			case 5:
				enemyItemSpriteArray[index].setTextureRect(RUBBISH_BIN_RECT);
				break;
			}
		}
	}
}

/// <summary>
/// uses the item
/// </summary>
void Game::useItem(int t_user, int t_slot)
{
	if (t_user == 0) //player using items
	{
		//getting what item to use and then setting slot to be empty
		int itemToUse = myPlayer.getInventoryArray(t_slot); 
		myPlayer.setInventoryArray(t_slot, 0); 
		inventoryItemSpriteArray[t_slot].setTextureRect(NULL_RECT);

		switch (itemToUse)
		{
		case OIL_DRINK:
			oilDrinkSound.play();
			myPlayer.setHealth(1);
			break;
		case SCANNER:
			scannerSound.play();
			scannerActive = true;
			scannerTimer = 100;
			break;
		case PAUSE_REMOTE:
			pauseRemoteSound.play();
			enemyPaused = true;
			break;
		case OVERCHARGER:
			overchargerSound.play();
			doubleDamage = true;
			break;
		case RUBBISH_BIN:
			rubbishBinSound.play();
			if (taserArray[currentShot] == 1) // live shot discarded
			{
				liveRounds--;
				liveRoundsMessage.setString(std::to_string(liveRounds));
			}
			else // blank shot discarded
			{
				blankRounds--;
				blankRoundsMessage.setString(std::to_string(liveRounds));
			}
			currentLoadedShots--;
			currentShot--;
			break;
		}
	}

	else //enemy using items
	{
		//getting what item to use and then setting slot to be empty
		int itemToUse = myEnemy.getInventoryArray(t_slot);
		myEnemy.setInventoryArray(t_slot, 0);
		enemyItemSpriteArray[t_slot].setTextureRect(NULL_RECT);
		switch (itemToUse)
		{
		case OIL_DRINK:
			oilDrinkSound.play();
			myEnemy.setHealth(1);
			break;
		case SCANNER:
			scannerSound.play();
			if (taserArray[currentShot] == 1)
			{
				knowItsLive = true;
				knowItsBlank = false;
			}
			else
			{
				knowItsBlank = true;
				knowItsBlank = false;
			}
			break;
		case PAUSE_REMOTE:
			pauseRemoteSound.play();
			playerPaused = true;
			break;
		case OVERCHARGER:
				overchargerSound.play();
			doubleDamage = true;
			break;
		case RUBBISH_BIN:
			rubbishBinSound.play();
			if (taserArray[currentShot] == 1) // live shot discarded
			{
				liveRounds--;
				liveRoundsMessage.setString(std::to_string(liveRounds));
			}
			else // blank shot discarded
			{
				blankRounds--;
				blankRoundsMessage.setString(std::to_string(liveRounds));
			}
			currentLoadedShots--;
			currentShot--;
			break;
		}
	}
}

/// <summary>
/// shows current shot when player uses scanner
/// </summary>
void Game::displayCurrentShot()
{
	scannedShotSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	scannedShotSprite.setScale(2, 2);
	if (taserArray[currentShot] == 1)
	{
		scannedShotSprite.setTexture(liveTaserTexture);
	}
	else if (taserArray[currentShot] == 0)
	{
		scannedShotSprite.setTexture(emptyTaserTexture);
	}

}

/// <summary>
/// sets up audio to be used in game
/// </summary>
void Game::setupAudio()
{
	if (!robotDamageTakenBuffer.loadFromFile("ASSETS\\AUDIO\\robotOuch.wav"))
	{
		std::cout << "robot damage taken audio not loading" << std::endl;
	}
	robotDamageTakenSound.setBuffer(robotDamageTakenBuffer);
	robotDamageTakenSound.setVolume(40);

	if (!zapBuffer.loadFromFile("ASSETS\\AUDIO\\zap.wav"))
	{
		std::cout << "zap audio not loading" << std::endl;
	}
	zapSound.setBuffer(zapBuffer);
	zapSound.setVolume(20);

	if (!blankBuffer.loadFromFile("ASSETS\\AUDIO\\blank.wav"))
	{
		std::cout << "blank round audio not loading" << std::endl;
	}
	blankSound.setBuffer(blankBuffer); 
	blankSound.setVolume(50);

	// Loading and setting up menu music
	if (!m_menuMusicLoad.loadFromFile("ASSETS\\AUDIO\\Black Soul.wav"))
	{
		std::cout << "Menu music not loading" << std::endl;
	}
	else
	{
		m_menuMusic.setBuffer(m_menuMusicLoad);
		m_menuMusic.setLoop(true);
		m_menuMusic.setVolume(10);
	}

	// Loading and setting up gameplay music
	if (!m_gameplayMusicLoad.loadFromFile("ASSETS\\AUDIO\\CertifiedBanger.wav"))
	{
		std::cout << "Gameplay music not loading" << std::endl;
	}
	else
	{
		m_gameplayMusic.setBuffer(m_gameplayMusicLoad);
		m_gameplayMusic.setLoop(true);
		m_gameplayMusic.setVolume(10);
	}

	if (!m_menuChoose.loadFromFile("ASSETS\\AUDIO\\MenuChoose.wav"))
	{
		std::cout << "Menu choose sound not loading" << std::endl;
	}
	else
	{
		m_boop.setBuffer(m_menuChoose);
		m_boop.setVolume(10);
	}

	if (!m_menuSelect.loadFromFile("ASSETS\\AUDIO\\MenuSelect.wav"))
	{
		std::cout << "Menu choose sound not loading" << std::endl;
	}
	else
	{
		m_beep.setBuffer(m_menuSelect);
		m_beep.setVolume(10);
	}
	if (!oilDrinkBuffer.loadFromFile("ASSETS\\AUDIO\\oilDrink.wav")) {
		std::cout << "Failed to load oil drink sound." << std::endl;
	}
	else {
		oilDrinkSound.setBuffer(oilDrinkBuffer);
		oilDrinkSound.setVolume(50);
	}

	if (!scannerBuffer.loadFromFile("ASSETS\\AUDIO\\scan.wav")) {
		std::cout << "Failed to load scanner sound." << std::endl;
	}
	else {
		scannerSound.setBuffer(scannerBuffer);
		scannerSound.setVolume(50);
	}

	if (!pauseRemoteBuffer.loadFromFile("ASSETS\\AUDIO\\pauseRemote.wav")) {
		std::cout << "Failed to load pause remote sound." << std::endl;
	}
	else {
		pauseRemoteSound.setBuffer(pauseRemoteBuffer);
		pauseRemoteSound.setVolume(50);
	}

	if (!overchargerBuffer.loadFromFile("ASSETS\\AUDIO\\battery.wav")) {
		std::cout << "Failed to load overcharger sound." << std::endl;
	}
	else {
		overchargerSound.setBuffer(overchargerBuffer);
		overchargerSound.setVolume(50);
	}

	if (!rubbishBinBuffer.loadFromFile("ASSETS\\AUDIO\\rubbishBin.wav")) {
		std::cout << "Failed to load rubbish bin sound." << std::endl;
	}
	else {
		rubbishBinSound.setBuffer(rubbishBinBuffer);
		rubbishBinSound.setVolume(50);
	}

}

/// <summary>
/// sets up hud to be used in game
/// </summary>
void Game::setupHUD()
{
	if (!upperBarTexture.loadFromFile("ASSETS\\IMAGES\\upperBar.png"))
	{
		std::cout << "Failed to load upper bar image!" << std::endl;
	}
	upperBarSprite.setTexture(upperBarTexture);
	upperBarSprite.setPosition(0, 0);

	if (!tableTexture.loadFromFile("ASSETS\\IMAGES\\table.png"))
	{
		std::cout << "Failed to load table image!" << std::endl;
	}
	tableSprite.setTexture(tableTexture);
	tableSprite.setPosition(0, 50);

	if (!playerHealthBarTexture.loadFromFile("ASSETS\\IMAGES\\playerHealth.png"))
	{
		std::cout << "Failed to load player health bar image!" << std::endl;
	}
	playerHealthBarSprite.setTexture(playerHealthBarTexture);
	playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_5_RECT);
	playerHealthBarSprite.setPosition(150, 250);
	playerHealthBarSprite.setScale(-1, 1);

	if (!enemyHealthBarTexture.loadFromFile("ASSETS\\IMAGES\\enemyHealth.png"))
	{
		std::cout << "Failed to load upper bar image!" << std::endl;
	}
	enemyHealthBarSprite.setTexture(enemyHealthBarTexture);
	enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_5_RECT);
	enemyHealthBarSprite.setPosition(720, 250);
	enemyHealthBarSprite.setScale(-1, 1);

	if (!liveTaserTexture.loadFromFile("ASSETS\\IMAGES\\liveTaserCharge.png"))
	{
		std::cout << "Failed to load upper bar image!" << std::endl;
	}
	liveTaserSprite.setTexture(liveTaserTexture);
	liveTaserSprite.setPosition(500, 0);

	if (!emptyTaserTexture.loadFromFile("ASSETS\\IMAGES\\emptyTaserCharge.png"))
	{
		std::cout << "Failed to load upper bar image!" << std::endl;
	}
	emptyTaserSprite.setTexture(emptyTaserTexture);
	emptyTaserSprite.setPosition(630, 0);

	
}

/// <summary>
/// checks health indicators of player/enemy and adjusts them
/// </summary>
void Game::checkHealth()
{
	switch (myPlayer.getHealth())
	{
	case 5:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_5_RECT);
		break;
	case 4:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_4_RECT);
		break;
	case 3:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_3_RECT);
		break;
	case 2:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_2_RECT);
		break;
	case 1:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_1_RECT);
		break;
	case 0:
		playerHealthBarSprite.setTextureRect(PLAYER_BATTERY_0_RECT);
		break;
	}

	switch (myEnemy.getHealth())
	{
	case 5:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_5_RECT);
		break;
	case 4:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_4_RECT);
		break;
	case 3:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_3_RECT);
		break;
	case 2:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_2_RECT);
		break;
	case 1:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_1_RECT);
		break;
	case 0:
		enemyHealthBarSprite.setTextureRect(ENEMY_BATTERY_0_RECT);
		break;
	}
}

/// <summary>
/// sets up item sheet
/// </summary>
void Game::setupItems()
{
	if (!itemSheetTexture.loadFromFile("ASSETS\\IMAGES\\Item-Sheet.png"))
	{
		std::cout << "Failed to load item sheet image!" << std::endl;
	}

	// Inventory box sprites
	inventoryItemSpriteArray[0].setTexture(itemSheetTexture);
	inventoryItemSpriteArray[0].setPosition(475.0f, 215.0f);
	inventoryItemSpriteArray[0].setTextureRect(NULL_RECT);
	inventoryItemSpriteArray[1].setTexture(itemSheetTexture);
	inventoryItemSpriteArray[1].setPosition(635.0f, 215.0f);
	inventoryItemSpriteArray[1].setTextureRect(NULL_RECT);
	inventoryItemSpriteArray[2].setTexture(itemSheetTexture);
	inventoryItemSpriteArray[2].setPosition(475.0f, 375.0f);
	inventoryItemSpriteArray[2].setTextureRect(NULL_RECT);
	inventoryItemSpriteArray[3].setTexture(itemSheetTexture);
	inventoryItemSpriteArray[3].setPosition(635.0f, 375.0f);
	inventoryItemSpriteArray[3].setTextureRect(NULL_RECT);

	enemyItemSpriteArray[0].setTexture(itemSheetTexture);
	enemyItemSpriteArray[0].setPosition(475.0f, 215.0f);
	enemyItemSpriteArray[0].setTextureRect(NULL_RECT);
	enemyItemSpriteArray[1].setTexture(itemSheetTexture);
	enemyItemSpriteArray[1].setPosition(635.0f, 215.0f);
	enemyItemSpriteArray[1].setTextureRect(NULL_RECT);
	enemyItemSpriteArray[2].setTexture(itemSheetTexture);
	enemyItemSpriteArray[2].setPosition(475.0f, 375.0f);
	enemyItemSpriteArray[2].setTextureRect(NULL_RECT);
	enemyItemSpriteArray[3].setTexture(itemSheetTexture);
	enemyItemSpriteArray[3].setPosition(635.0f, 375.0f);
	enemyItemSpriteArray[3].setTextureRect(NULL_RECT);
}

/// <summary>
/// displays background showing item description
/// </summary>
/// <param name="t_slot"></param>
void Game::displayItemDescription(int t_slot)
{
	int backgroundToDisplay = myPlayer.getInventoryArray(t_slot);

	switch (backgroundToDisplay)
	{
	case 0:
		inventoryScreenSprite.setTextureRect(sf::IntRect(0, 0, 800, 600));
		break;
	case OIL_DRINK:
		inventoryScreenSprite.setTextureRect(sf::IntRect(800, 0, 800, 600));
		break;
	case SCANNER:
		inventoryScreenSprite.setTextureRect(sf::IntRect(1600, 0, 800, 600));
		break;
	case PAUSE_REMOTE:
		inventoryScreenSprite.setTextureRect(sf::IntRect(2400, 0, 800, 600));
		break;
	case OVERCHARGER:
		inventoryScreenSprite.setTextureRect(sf::IntRect(3200, 0, 800, 600));
		break;
	case RUBBISH_BIN:
		inventoryScreenSprite.setTextureRect(sf::IntRect(4000, 0, 800, 600));
		break;
	}
}

/// <summary>
/// sets up game over screen
/// </summary>
void Game::setupGameOver() {
	// Load the win image
	if (!winTexture.loadFromFile("ASSETS\\IMAGES\\youWin.png")) {
		std::cerr << "Failed to load win image!" << std::endl;
	}
	else {
		winSprite.setTexture(winTexture);
		winSprite.setPosition((m_window.getSize().x - winSprite.getLocalBounds().width) / 2,
			(m_window.getSize().y - winSprite.getLocalBounds().height) / 2);
	}

	// Load the lose image
	if (!loseTexture.loadFromFile("ASSETS\\IMAGES\\gameOver.png")) {
		std::cerr << "Failed to load lose image!" << std::endl;
	}
	else {
		loseSprite.setTexture(loseTexture);	
		loseSprite.setPosition((m_window.getSize().x - loseSprite.getLocalBounds().width) / 2,
			(m_window.getSize().y - loseSprite.getLocalBounds().height) / 2);
	}
}

/// <summary>
/// restarts game variables and inventory sprite array
/// </summary>
void Game::restartGame()
{
	myPlayer.reset();
	myEnemy.reset();
	for (int index = 0; index < MAX_ITEMS; index++)
	{
		inventoryItemSpriteArray[index].setTextureRect(NULL_RECT);
	}
}
