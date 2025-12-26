#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
enum CardValue
{
	Ace = 11,
	Ten = 10,
	Nine = 0,
	King = 4,
	Queen = 3,
	Jack = 2
};
enum CardSuit
{
	Spades = 4,
	Hearts = 3,
	Diamonds = 2,
	Clubs = 1
};
enum class Color
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Aqua = 3,
	Red = 4,
	Purple = 5,
	Yellow = 6,
	White = 7,
	Gray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightAqua = 11,
	LightRed = 12,
	LightPurple = 13,
	LightYellow = 14,
	BrightWhite = 15
};
struct Card
{
	CardValue value;
	CardSuit suit;
};
struct Player
{
	Card* deck;
	size_t gamePoints = 0;
	size_t roundPoints = 0;
	size_t cardsInHand = 0;
};
int main()
{
    
}
