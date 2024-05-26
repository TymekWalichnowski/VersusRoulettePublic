/// <summary>
/// @author Natalia Ryl
/// @date April 2024
/// </summary>

#include "Player.h"   
#include <SFML/Graphics/Transformable.hpp>
#include "Game.h"
#include "Globals.h"

Player::Player() //default constructor
{
	setupSprite();
	initializeInventoryArray();

	animationPlaying = false;
	currentAnimation = 0;
	playerFrame = -1;
	frameCounter = 0.0f;
	frameIncrement = 0.2f;

	itemsHeld = 0;
	health = 5;
}

/// <summary>
/// sets up the player sprite
/// </summary>
void Player::setupSprite()
{
	//loads blank and live spritesheet textures
	if (!textureBlank.loadFromFile("ASSETS\\IMAGES\\player shoot blank sheet.png"))
	{
		std::cout << "problem loading player shoot blank texture" << std::endl;
	}

	if (!textureLive.loadFromFile("ASSETS\\IMAGES\\player shoot live sheet.png"))
	{
		std::cout << "problem loading player shoot live texture" << std::endl;
	}

	if (!textureLiveSelf.loadFromFile("ASSETS\\IMAGES\\player shoot self live-Sheet.png"))
	{
		std::cout << "problem loading player shoot self live texture" << std::endl;
	}

	if (!textureBlankSelf.loadFromFile("ASSETS\\IMAGES\\player shoot self blank-Sheet.png"))
	{
		std::cout << "problem loading player shoot self blank texture" << std::endl;
	}

	if (!textureHit.loadFromFile("ASSETS\\IMAGES\\player tased-Sheet.png"))
	{
		std::cout << "problem loading player tased texture" << std::endl;
	}

	sprite.setTexture(textureLive);
	sprite.setPosition(-25, 300);
	sprite.setTextureRect(sf::IntRect(0, 0, 192, 128));
	sprite.setScale(2.5f, 2.5f);

}

/// <summary>
/// returns the sprite
/// </summary>
sf::Sprite Player::getBody()
{
	return sprite;
}

/// <summary>
/// sets whether an animation is playing and what animation SHOULD be playing
/// </summary>
void Player::setAnimationPlaying(bool t_AnimationPlaying, int t_animationToPlay)
{
	animationPlaying = t_AnimationPlaying;
	currentAnimation = t_animationToPlay;
}

bool Player::getAnimationPlaying() // returns whether animation playing or not
{
	return animationPlaying;
}

void Player::playAnimation() // calls currently selected animation to play
{
	switch (currentAnimation)
	{
	case 0:
		break;
	case 1:
		animationLiveShoot();
		break;
	case 2:
		animationBlankShoot();
		break;
	case 3:
		animationLiveSelf();
		break;
	case 4:
		animationBlankSelf();
		break;
	case 5:
		animationHit();
		break;
	}
}

void Player::animationLiveShoot() // animation for shooting live
{
	sprite.setTexture(textureLive);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= PLAYER_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != playerFrame)
	{
		playerFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Player::animationBlankShoot() // animation for shooting blank
{
	sprite.setTexture(textureBlank);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= PLAYER_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != playerFrame)
	{
		playerFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Player::animationLiveSelf() // animation for shooting self with live
{
	sprite.setTexture(textureLiveSelf);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= PLAYER_SELF_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != playerFrame)
	{
		playerFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Player::animationBlankSelf() // animation for shooting self with blank
{
	sprite.setTexture(textureBlankSelf);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= PLAYER_SELF_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != playerFrame)
	{
		playerFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Player::animationHit() // animation for being hit
{
	sprite.setTexture(textureHit);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= PLAYER_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != playerFrame)
	{
		playerFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

/// <summary>
/// initializes inventory array
/// </summary>
void Player::initializeInventoryArray()
{
	for (int index = 0; index < MAX_ITEMS; index++)
	{
		inventoryArray[index] = 0;
	}
}

// returns invetory item from space specified
int Player::getInventoryArray(int t_index)
{
	return inventoryArray[t_index];
}

// sets inventory item from space specified
void Player::setInventoryArray(int t_index, int t_item)
{
	inventoryArray[t_index] = t_item;
}

// sets amount of items currently being held
void Player::setItemsHeld(int t_itemsHeld)
{
	itemsHeld = itemsHeld;
}

// returns amount of items held
int Player::getItemsHeld()
{
	return itemsHeld;
}

// Sets the player's health
void Player::setHealth(int t_healthToAdd)
{
	health = health + t_healthToAdd;
}

// Get the player's health
int Player::getHealth() const
{
	return health;
}

// resets variables to default
void Player::reset()
{
	health = 5;
	itemsHeld = 0;
	animationPlaying = false;
	currentAnimation = 0;

	for (int index = 0; index < MAX_ITEMS; index++)
	{
		setInventoryArray(index, 0);
	}
}
