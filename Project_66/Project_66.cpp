#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
constexpr int DECK_SIZE = 24;
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
WORD getCurrentConsoleColor()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&info
	);
	return info.wAttributes;
}
void setColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}
void resetColor(WORD originalColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), originalColor);
}
void printSuit(CardSuit suit, WORD originalColor)
{
	if (suit == CardSuit::Spades) {
		std::cout << "\xE2\x99\xA0";
	}
	else if (suit == CardSuit::Hearts) {
		setColor(Color::LightRed);
		std::cout << "\xE2\x99\xA5";
		resetColor(originalColor);
	}
	else if (suit == CardSuit::Diamonds) {
		setColor(Color::LightRed);
		std::cout << "\xE2\x99\xA6";
		resetColor(originalColor);
	}
	else {
		std::cout << "\xE2\x99\xA3";
	}
}
void printValue(CardValue value)
{
	if (value == Ace) {
		std::cout << "A";
	}
	else if (value == Ten) {
		std::cout << "10";
	}
	else if (value == Nine) {
		std::cout << "9";
	}
	else if (value == Jack) {
		std::cout << "J";
	}
	else if (value == King) {
		std::cout << "K";
	}
	else if (value == Queen) {
		std::cout << "Q";
	}
}
void fillDeck(Card* deck, size_t deckSize)
{
	if (!deck) {
		return;
	}
	for (size_t i = 0, j = 9; i < deckSize; i++, j++)
	{
		if (j == 15) {
			j = 9;
		}
		switch (j) {
		case 9: deck[i].value = CardValue::Nine; break;
		case 10: deck[i].value = CardValue::Ten; break;
		case 11: deck[i].value = CardValue::Jack; break;
		case 12: deck[i].value = CardValue::Queen; break;
		case 13: deck[i].value = CardValue::King; break;
		case 14: deck[i].value = CardValue::Ace;
		}
		if (i < 6) {
			deck[i].suit = CardSuit::Spades;
		}
		else if (i < 12) {
			deck[i].suit = CardSuit::Hearts;
		}
		else if (i < 18) {
			deck[i].suit = CardSuit::Diamonds;
		}
		else {
			deck[i].suit = CardSuit::Clubs;
		}
	}
}
void cardSwap(Card& card1, Card& card2)
{
	Card temp = card1;
	card1 = card2;
	card2 = temp;
}
void shuffleDeck(Card* deck, size_t deckSize)
{
	if (!deck) {
		return;
	}
	srand(time(nullptr));
	for (size_t i = 0; i < deckSize; i++)
	{
		size_t randomNum = i + (rand() % (deckSize - i));
		cardSwap(deck[i], deck[randomNum]);
	}
}
void printDeck(Card* deck, size_t deckSize, WORD originalColor) // used only for testing
{
	if (!deck) {
		return;
	}
	for (size_t i = 0; i < deckSize; i++)
	{
		printValue(deck[i].value);
		printSuit(deck[i].suit, originalColor);
		std::cout << std::endl;
	}
}
int main()
{
	WORD originalColor = getCurrentConsoleColor();
	Card* deck = new Card[DECK_SIZE];
	size_t deckSize = DECK_SIZE;
	fillDeck(deck, deckSize);
	shuffleDeck(deck, deckSize);
	printDeck(deck, deckSize, originalColor);
}
