/// <summary>
/// @author Tymoteusz Walichnowski
/// @author Shane Moran
/// @date April 2024
/// </summary>
/// Contains the game globals
/// 
#include <SFML/Graphics.hpp>

#pragma once
//game screens
const int static MAIN_MENU = 0;
const int static GAMEPLAY = 1;
const int static INSTRUCTIONS = 2;
const int static GAME_OVER = 3;
const int static INVENTORY = 4;

//animations
const int static SHOOT_OPPONENT_LIVE = 1;
const int static SHOOT_OPPONENT_BLANK = 2;
const int static SHOOT_SELF_LIVE = 3;
const int static SHOOT_SELF_BLANK = 4;
const int static GETTING_HIT = 5;

//items
const int static OIL_DRINK = 1;
const int static SCANNER = 2;
const int static PAUSE_REMOTE = 3;
const int static OVERCHARGER = 4;
const int static RUBBISH_BIN = 5;

const static sf::IntRect NULL_RECT(0, 0, 0, 0);
const static sf::IntRect OIL_DRINK_RECT(0, 0, 64, 64);
const static sf::IntRect SCANNER_RECT(128, 0, 64, 64);
const static sf::IntRect PAUSE_REMOTE_RECT(256, 0, 64, 64);
const static sf::IntRect OVERCHARGER_RECT(384, 0, 64, 64);
const static sf::IntRect RUBBISH_BIN_RECT(512, 0, 64, 64);

//player/enemy user (may need to set these up in game.h as they're only used in game.cpp so far
const int PLAYER = 0;
const int ENEMY = 1;

// player health bar
const sf::IntRect PLAYER_BATTERY_5_RECT(0, 0, 64, 64);
const sf::IntRect PLAYER_BATTERY_4_RECT(64, 0, 64, 64);
const sf::IntRect PLAYER_BATTERY_3_RECT(128, 0, 64, 64);
const sf::IntRect PLAYER_BATTERY_2_RECT(192, 0, 64, 64);
const sf::IntRect PLAYER_BATTERY_1_RECT(256, 0, 64, 64);
const sf::IntRect PLAYER_BATTERY_0_RECT(320, 0, 64, 64);

// enemy health bar
const sf::IntRect ENEMY_BATTERY_5_RECT(320, 0, 64, 64);
const sf::IntRect ENEMY_BATTERY_4_RECT(256, 0, 64, 64);
const sf::IntRect ENEMY_BATTERY_3_RECT(192, 0, 64, 64);
const sf::IntRect ENEMY_BATTERY_2_RECT(128, 0, 64, 64);
const sf::IntRect ENEMY_BATTERY_1_RECT(64, 0, 64, 64);
const sf::IntRect ENEMY_BATTERY_0_RECT(0, 0, 64, 64);

//inventory
static const int MAX_ITEMS = 4;

// the size of the screen in pixels used in the game
const float SCREEN_WIDTH = 800;   
const float SCREEN_HEIGHT = 600;