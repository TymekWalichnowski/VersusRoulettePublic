/// <summary>
/// @author Natalia Ryl
/// @date April 2024
/// </summary>

#include "Player.h"   
#include <SFML/Graphics/Transformable.hpp>
#include "Game.h"
#include "Globals.h"
#include "Enemy.h"

Enemy::Enemy() //default constructor
{
	setupSprite();
	initializeInventoryArray();
	animationPlaying = false;
	currentAnimation = 0;
	enemyFrame = -1;
	frameCounter = 0.0f;
	frameIncrement = 0.2f;

	itemsHeld = 0;
	health = 5;
}

/// <summary>
/// sets up the enemy sprite
/// </summary>
void Enemy::setupSprite()
{
	//loads blank and live spritesheet textures
	if (!textureBlank.loadFromFile("ASSETS\\IMAGES\\enemy shoot blank sheet.png"))
	{
		std::cout << "problem loading enemy shoot blank texture" << std::endl;
	}
	if (!textureLive.loadFromFile("ASSETS\\IMAGES\\enemy shoot live sheet.png"))
	{
		std::cout << "problem loading enemy shoot live texture" << std::endl;
	}
	if (!textureBlankSelf.loadFromFile("ASSETS\\IMAGES\\enemy shoot self blank-Sheet.png"))
	{
		std::cout << "problem loading enemy shoot self blank texture" << std::endl;
	}
	if (!textureLiveSelf.loadFromFile("ASSETS\\IMAGES\\enemy shoot self live-Sheet.png"))
	{
		std::cout << "problem loading enemy shoot self live texture" << std::endl;
	}

	if (!textureHit.loadFromFile("ASSETS\\IMAGES\\enemy tased-Sheet.png"))
	{
		std::cout << "problem loading enemy tased texture" << std::endl;
	}

	sprite.setTexture(textureLive);
	sprite.setTextureRect(sf::IntRect(0, 0, 192, 128));
	sprite.setPosition(350, 300);
	sprite.setScale(2.5f, 2.5f);
}

/// <summary>
/// returns the sprite
/// </summary>
sf::Sprite Enemy::getBody()
{
	return sprite;
}

/// <summary>
/// sets whether an animation is playing and what animation SHOULD be playing
/// </summary>
void Enemy::setAnimationPlaying(bool t_AnimationPlaying, int t_animationToPlay)
{
	animationPlaying = t_AnimationPlaying;
	currentAnimation = t_animationToPlay;
}

bool Enemy::getAnimationPlaying() // returns whether animation playing or not
{
	return animationPlaying;
}

void Enemy::playAnimation() // calls currently selected animation to play
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

void Enemy::animationLiveShoot() // animation for shooting live
{
	sprite.setTexture(textureLive);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= ENEMY_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != enemyFrame)
	{
		enemyFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Enemy::animationBlankShoot() // animation for shooting blank
{
	sprite.setTexture(textureBlank);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= ENEMY_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != enemyFrame)
	{
		enemyFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Enemy::animationLiveSelf() // animation for shooting self with live
{
	sprite.setTexture(textureLiveSelf);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= ENEMY_SELF_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != enemyFrame)
	{
		enemyFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Enemy::animationBlankSelf() // animation for shooting self with  blank
{
	sprite.setTexture(textureBlankSelf);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= ENEMY_SELF_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != enemyFrame)
	{
		enemyFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

void Enemy::animationHit() // animation for being hit
{
	sprite.setTexture(textureHit);

	int frame = 0;
	const int FRAME_WIDTH = 192;
	const int FRAME_HEIGHT = 128;

	frameCounter += frameIncrement;
	frame = static_cast<int>(frameCounter);
	if (frame >= ENEMY_FRAMES)
	{
		frame = 0;
		frameCounter = 0.0f;
		animationPlaying = false;
	}
	if (frame != enemyFrame)
	{
		enemyFrame = frame;
		sprite.setTextureRect(sf::IntRect(frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
	}
}

/// <summary>
/// initializes inventory array
/// </summary>
void Enemy::initializeInventoryArray()
{
	for (int index = 0; index < MAX_ITEMS; index++)
	{
		inventoryArray[index] = 0;
	}
}

// returns invetory item from space specified
int Enemy::getInventoryArray(int t_index)
{
	return inventoryArray[t_index];
}

// sets inventory item from space specified
void Enemy::setInventoryArray(int t_index, int t_item)
{
	inventoryArray[t_index] = t_item;
}

// sets amount of items currently being held
void Enemy::setItemsHeld(int t_itemsHeld)
{
	itemsHeld = itemsHeld;
}

// returns amount of items held
int Enemy::getItemsHeld()
{
	return itemsHeld;
}

// Sets the enemy's health
void Enemy::setHealth(int t_healthToAdd)
{
	health = health + t_healthToAdd;
}

// Get the enemy's health
int Enemy::getHealth() const
{
	return health;
}

// resets variables to default
void Enemy::reset() 
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
