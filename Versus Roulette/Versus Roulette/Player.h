/// <summary>
/// @author Nathan Mas
/// @author Natalia Ryl
/// @date April 2024
/// </summary>
/// Header file for Player class
#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Globals.h"
#include <iostream>


class Player
{

public:

	Player();
	void setupSprite();
	sf::Sprite getBody();

	void setAnimationPlaying(bool t_AnimationPlaying, int t_animationToPlay);
	bool getAnimationPlaying();

	void playAnimation(); // is updated every frame

	void animationLiveShoot();
	void animationBlankShoot();
	void animationLiveSelf();
	void animationBlankSelf();
	void animationHit();

	//inventory
	void initializeInventoryArray();
	int getInventoryArray(int t_index);
	void setInventoryArray(int t_index, int t_item);
	void setItemsHeld(int t_itemsHeld);
	int getItemsHeld();

	//health
	void setHealth(int t_healthToAdd);
	int getHealth() const;

	void reset();

private:

	sf::Sprite sprite; // player's body
	sf::Texture textureLive;
	sf::Texture textureBlank;
	sf::Texture textureLiveSelf;
	sf::Texture textureBlankSelf;
	sf::Texture textureHit;

	bool animationPlaying; // is the animation playing?
	int currentAnimation; // what animation is being played?

	int playerFrame;
	const int PLAYER_FRAMES = 11;
	const int PLAYER_SELF_FRAMES = 9;
	float frameCounter;
	float frameIncrement;

	//inventory array
	int inventoryArray[MAX_ITEMS];
	int itemsHeld;

	int health;
};