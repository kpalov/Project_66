#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
constexpr int DECK_SIZE = 24;
constexpr int CARDS_IN_HAND = 6;
constexpr int BUFFER = 100;
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
struct GameSettings
{
	size_t targetPoints = 11;
	size_t nonTrumpMarriage = 20;
	size_t trumpMarriage = 40;
	bool showPoints = true;
	bool lastTrick = true;
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
size_t pow(size_t num, size_t power)
{
	size_t result = 1;
	for (size_t i = 0; i < power; i++)
	{
		result *= num;
	}
	return result;
}
int strcomp(const char* str1, const char* str2)
{
	for (size_t i = 0; str1[i]; i++)
	{
		if (str1[i] < str2[i]) return 1;
		else if (str1[i] > str2[i]) return -1;
	}
	return 0;
}
bool isUpperCase(char ch)
{
	return (ch >= 'A' && ch <= 'Z');
}
void toLowerCase(char* str)
{
	if (!str) {
		return;
	}
	for (size_t i = 0; str[i]; i++)
	{
		if (isUpperCase(str[i])) {
			str[i] = (str[i] - 'A') + 'a';
		}
	}
}
size_t strlen(char* str)
{
	if (!str) {
		return 0;
	}
	size_t count = 0;
	for (size_t i = 0; str[i]; i++)
	{
		count++;
	}
	return count;
}
size_t charToNumber(char ch)
{
	return ch - '0';
}
size_t stringToNumber(char* str) {
	size_t size = strlen(str);
	size_t num = 0;
	for (size_t i = size - 1, power = 0; power < size; i--, power++)
	{
		num += pow(10, power) * charToNumber(str[i]);
	}
	return num;
}
bool isStringANumber(char* str)
{
	if (!str) {
		return false;
	}
	for (size_t i = 0; str[i]; i++)
	{
		if (str[i] < '0' || str[i] > '9') return false;
	}
	return true;
}
bool isCharANumber(char ch)
{
	return ch >= '0' && ch <= '9';
}
void trim(char* input)
{
	if (!input || input[0] == '\0') {
		return;
	}
	bool seenSpace = true;
	size_t pos = 0;
	for (size_t i = 0; input[i]; i++)
	{
		if (input[i] != ' ' && input[i] != '\t') {
			seenSpace = false;
			input[pos++] = input[i];
		}
		else if (input[i] == ' ' && !seenSpace) {
			seenSpace = true;
			input[pos++] = ' ';
		}
		if (input[i + 1] == '\0') {
			input[pos] = '\0';
		}
	}
	if (input[pos - 1] == ' ') {
		input[pos - 1] = '\0';
	}
}
void formatUserInput(char* input)
{
	if (!input) {
		return;
	}
	trim(input);
	toLowerCase(input);
}
void userInput(char* input)
{
	std::cin.getline(input, BUFFER);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
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
void startingDeal(const int NUMBER_OF_DEALED_CARDS, Card* deck, Player& player1, Player& player2)
{
	if (!deck) {
		return;
	}
	for (size_t i = 0, pos1 = 0, pos2 = 0; i < NUMBER_OF_DEALED_CARDS; i++)
	{
		if (i < 3) {
			player1.deck[pos1] = deck[i];
			pos1++;
		}
		else if (i < 6) {
			player2.deck[pos2] = deck[i];
			pos2++;
		}
		else if (i < 9) {
			player1.deck[pos1] = deck[i];
			pos1++;
		}
		else {
			player2.deck[pos2] = deck[i];
			pos2++;
		}
	}
	player1.cardsInHand = NUMBER_OF_DEALED_CARDS / 2;
	player2.cardsInHand = NUMBER_OF_DEALED_CARDS / 2;
}
void orderHand(Card* hand, size_t length)
{
	if (!hand) {
		return;
	}
	size_t maxIndex = 0;
	for (size_t i = 0; i < length - 1; i++)
	{
		maxIndex = i;
		for (size_t j = i + 1; j < length; j++)
		{
			if (hand[j].suit > hand[maxIndex].suit) {
				maxIndex = j;
			}
			else if (hand[j].suit == hand[maxIndex].suit) {
				if (hand[j].value > hand[maxIndex].value) {
					maxIndex = j;
				}
			}
		}
		if (maxIndex != i) {
			cardSwap(hand[maxIndex], hand[i]);
		}
	}
}
void hand(Player player, size_t playerInTurn, WORD originalColor)
{
	if (playerInTurn == 1) {
		std::cout << std::endl << "Your hand (P1): [";
	}
	else {
		std::cout << std::endl << "Your hand (P2): [";
	}
	for (size_t i = 0; i < player.cardsInHand; i++)
	{
		printValue(player.deck[i].value);
		printSuit(player.deck[i].suit, originalColor);
		if (i == player.cardsInHand - 1) {
			std::cout << "]";
		}
		else {
			std::cout << ", ";
		}
	}
}
size_t changeFirstSetting(char* input)
{
	while (true) {
		std::cout << "Enter new target points to win within range [5-20]" << std::endl;
		userInput(input);
		formatUserInput(input);
		if (isStringANumber(input)) {
			if (stringToNumber(input) <= 5 || stringToNumber(input) >= 20) {
				std::cout << "Input out of bounds. Please try again." << std::endl;
			}
			else {
				return stringToNumber(input);
			}
		}
		else {
			std::cout << "Incorrect input. Please try again." << std::endl;
		}
	}
}
size_t changeSecondSetting(char* input)
{
	while (true) {
		std::cout << "Enter new non-trump marriage points within range [5-40]" << std::endl;
		userInput(input);
		formatUserInput(input);
		if (isStringANumber(input)) {
			if (stringToNumber(input) <= 5 || stringToNumber(input) >= 40) {
				std::cout << "Input out of bounds. Please try again." << std::endl;
			}
			else {
				return stringToNumber(input);
			}
		}
		else {
			std::cout << "Incorrect input. Please try again." << std::endl;
		}
	}
}
size_t changeThirdSetting(char* input)
{
	while (true) {
		std::cout << "Enter new trump marriage points within range [20-60]" << std::endl;
		userInput(input);
		formatUserInput(input);
		if (isStringANumber(input)) {
			if (stringToNumber(input) <= 20 || stringToNumber(input) >= 60) {
				std::cout << "Input out of bounds. Please try again." << std::endl;
			}
			else {
				return stringToNumber(input);
			}
		}
		else {
			std::cout << "Incorrect input. Please try again." << std::endl;
		}
	}
}
void settings(GameSettings settings, char* input)
{
	std::cout << "Santase (66)" << std::endl;
	std::cout << "1) Target points to win[" << settings.showPoints << "]" << std::endl;
	std::cout << "2) Non-Trump marriage points [" << settings.nonTrumpMarriage << "]" << std::endl;
	std::cout << "3) Trump marriage points [" << settings.trumpMarriage << "]" << std::endl;
	if (settings.showPoints) {
		std::cout << "4) Show players' points [on]" << std::endl;
	}
	else {
		std::cout << "4) Show players' points [off]" << std::endl;
	}
	if (settings.lastTrick) {
		std::cout << "5) Last trick +10 [on]" << std::endl;
	}
	else {
		std::cout << "5) Last trick +10 [off]" << std::endl;
	}
	while (true) {
		std::cout << "Enter number to change or 'back' to return:" << std::endl;
		userInput(input);
		formatUserInput(input);
		if (!strcomp(input, "1")) {
			settings.showPoints = changeFirstSetting(input);
			std::cout << "Target points successfully changed to " << settings.showPoints << std::endl;
		}
		else if (!strcomp(input, "2")) {
			settings.nonTrumpMarriage = changeSecondSetting(input);
			std::cout << "Non-trump marriage points successfully changed to " << settings.nonTrumpMarriage << std::endl;
		}
		else if (!strcomp(input, "3")) {
			settings.trumpMarriage = changeThirdSetting(input);
			std::cout << "Trump marriage points successfully changed to " << settings.trumpMarriage << std::endl;
		}
		else if (!strcomp(input, "4")) {
			if (settings.showPoints) {
				settings.showPoints = false;
				std::cout << "Showing players' points is switched off" << std::endl;
			}
			else {
				settings.showPoints = true;
				std::cout << "Showing players' points is switched on" << std::endl;
			}
		}
		else if (!strcomp(input, "5")) {
			if (settings.lastTrick) {
				settings.lastTrick = false;
				std::cout << "Last trick is switched off" << std::endl;
			}
			else {
				settings.lastTrick = true;
				std::cout << "Last trick is switched on" << std::endl;
			}
		}
		else if (!strcomp(input, "back")) {
			break;
		}
		else {
			continue;
		}
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	WORD originalColor = getCurrentConsoleColor();
	Card* deck = new Card[DECK_SIZE];
	Player player1, player2;
	player1.deck = new Card[CARDS_IN_HAND];
	player2.deck = new Card[CARDS_IN_HAND];
	size_t deckSize = DECK_SIZE;
	//fillDeck(deck, deckSize);
	//shuffleDeck(deck, deckSize);
	//printDeck(deck, deckSize, originalColor);
	//startingDeal(12, deck, player1, player2);
	//orderHand(player1.deck, player1.cardsInHand);
	//orderHand(player2.deck, player2.cardsInHand);
	//hand(player1, 1, originalColor);
	//hand(player2, 2, originalColor);
	//deck = resizeDeck(deck, 11, deckSize, deckSize, true);
	//printDeck(deck, deckSize, originalColor);
	char* input = new char[BUFFER];
	userInput(input);
	formatUserInput(input);
	std::cout << input;
}
