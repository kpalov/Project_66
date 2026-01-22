/**
*
* Solution to course project # 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Krasimir Palov
* @idnumber 08MI0600589
* @compiler VC
*
* main file
*
*/

#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
constexpr int DECK_SIZE = 24;
constexpr int CARDS_IN_HAND = 6;
constexpr int BUFFER = 100;
constexpr int MAX_ROUNDS = 100;
// Structures
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
	size_t cardsInHand = 0;
	size_t gamePoints = 0;
	size_t roundPoints = 0;
	Card hand[CARDS_IN_HAND];
	Card lastCardDrawn;
};
struct GameSettings
{
	size_t targetPoints = 11;
	size_t nonTrumpMarriage = 20;
	size_t trumpMarriage = 40;
	bool showPoints = true;
	bool lastTrick = true;
};
struct Trick
{
	Card player1;
	Card player2;
	bool firstPlayer = false;
	bool winner;
};
struct RoundState
{
	CardSuit marriageSuit = Spades;
	CardSuit trump = Spades;
	bool strictRules = false;
	bool playerInTurn = false;
	bool firstPlayer = false;
	bool announceMarriage = false;
	bool isFirstTrick = true;

};
struct GameHistory
{
	size_t player1RoundPoints = 0;
	size_t player2RoundPoints = 0;
	size_t earnedPoints = 0;
	bool winner;
	bool tied = false;
};
struct Deck
{
	size_t size = DECK_SIZE;
	size_t topCardIndex = 0;
	Card deck[DECK_SIZE];
};
// string manipulations
size_t pow(size_t num, size_t power)
{
	size_t result = 1;
	for (size_t i = 0; i < power; i++)
	{
		result *= num;
	}
	return result;
}
int strlen(char* str)
{
	if (!str) {
		return 0;
	}
	int count = 0;
	for (size_t i = 0; str[i]; i++)
	{
		count++;
	}
	return count;
}
int strcomp(const char* str1, const char* str2)
{
	if (strlen(str1) == 0) {
		return -1;
	}
	for (size_t i = 0; str1[i]; i++)
	{
		if (str1[i] < str2[i]) return 1;
		else if (str1[i] > str2[i]) return -1;
	}
	return 0;
}
bool strContains(const char* str1, const char* str2)
{
	size_t size1 = strlen(str1);
	size_t size2 = strlen(str2);
	if (size2 > size1) {
		return false;
	}
	for (size_t i = 0; i <= size1 - size2; i++)
	{
		for (size_t pos1 = 0, pos2 = i; pos1 < size2; pos1++, pos2++)
		{
			if (str2[pos1] != str1[pos2]) {
				break;
			}
			if (pos1 == size2 - 1) {
				return true;
			}
		}
	}
	return false;
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
size_t charToNumber(char ch)
{
	return ch - '0';
}
size_t stringToNumber(char* str) {
	size_t size = strlen(str);
	size_t num = 0;
	for (size_t i = 0; str[i]; i++)
	{
		num = num * 10 + charToNumber(str[i]);
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
// Deck and card manipulations
void fillDeck(Deck& deck)
{
	for (size_t i = 0, j = 9; i < DECK_SIZE; i++, j++)
	{
		if (j == 15) {
			j = 9;
		}
		switch (j) {
		case 9: deck.deck[i].value = CardValue::Nine; break;
		case 10: deck.deck[i].value = CardValue::Ten; break;
		case 11: deck.deck[i].value = CardValue::Jack; break;
		case 12: deck.deck[i].value = CardValue::Queen; break;
		case 13: deck.deck[i].value = CardValue::King; break;
		case 14: deck.deck[i].value = CardValue::Ace;
		}
		if (i < 6) {
			deck.deck[i].suit = CardSuit::Spades;
		}
		else if (i < 12) {
			deck.deck[i].suit = CardSuit::Hearts;
		}
		else if (i < 18) {
			deck.deck[i].suit = CardSuit::Diamonds;
		}
		else {
			deck.deck[i].suit = CardSuit::Clubs;
		}
	}
}
void cardSwap(Card& card1, Card& card2)
{
	Card temp = card1;
	card1 = card2;
	card2 = temp;
}
void shuffleDeck(Deck& deck)
{
	srand(time(nullptr));
	for (size_t i = 0; i < DECK_SIZE; i++)
	{
		size_t randomNum = i + (rand() % (DECK_SIZE - i));
		cardSwap(deck.deck[i], deck.deck[randomNum]);
	}
}
void orderHand(Player& player)
{
	size_t maxIndex = 0;
	for (size_t i = 0; i < player.cardsInHand - 1; i++)
	{
		maxIndex = i;
		for (size_t j = i + 1; j < player.cardsInHand; j++)
		{
			if (player.hand[j].suit > player.hand[maxIndex].suit) {
				maxIndex = j;
			}
			else if (player.hand[j].suit == player.hand[maxIndex].suit) {
				if (player.hand[j].value > player.hand[maxIndex].value) {
					maxIndex = j;
				}
			}
		}
		if (maxIndex != i) {
			cardSwap(player.hand[maxIndex], player.hand[i]);
		}
	}
}
bool cardEquals(Card card1, Card card2)
{
	return card1.suit == card2.suit && card1.value == card2.value;
}
bool isCardInHand(Player player, Card card)
{
	for (size_t i = 0; i < player.cardsInHand; i++)
	{
		if (cardEquals(player.hand[i], card)) {
			return true;
		}
	}
	return false;
}
bool isSuitInHand(Player player, CardSuit suit)
{
	for (size_t i = 0; i < player.cardsInHand; i++)
	{
		if (player.hand[i].suit == suit) {
			return true;
		}
	}
	return false;
}
bool isSuitsEqual(CardSuit suit1, CardSuit suit2) {
	return suit1 == suit2;
}
// color manip
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
// print card values
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
void printCard(Card card, WORD originalColor) {
	printValue(card.value);
	printSuit(card.suit, originalColor);
}
// game functions
void hand(Player player, RoundState state, WORD originalColor)
{
	if (!state.isFirstTrick && !state.strictRules) {
		std::cout << "You drew ";
		printCard(player.lastCardDrawn, originalColor);
		std::cout << std::endl;
	}
	if (!state.playerInTurn) {
		std::cout << "Your hand (P1): [";
	}
	else {
		std::cout << "Your hand (P2): [";
	}
	for (size_t i = 0; i < player.cardsInHand; i++)
	{
		printValue(player.hand[i].value);
		printSuit(player.hand[i].suit, originalColor);
		if (i == player.cardsInHand - 1) {
			std::cout << "]";
		}
		else {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
}
void startingDeal(Player player[], Deck& deck)
{
	constexpr int NUMBER_OF_DEALED_CARDS = 12;
	for (size_t i = 0, pos1 = 0, pos2 = 0; i < NUMBER_OF_DEALED_CARDS; i++)
	{
		if (i < 3) {
			player[0].hand[pos1] = deck.deck[i];
			pos1++;
		}
		else if (i < 6) {
			player[1].hand[pos2] = deck.deck[i];
			pos2++;
		}
		else if (i < 9) {
			player[0].hand[pos1] = deck.deck[i];
			pos1++;
		}
		else {
			player[1].hand[pos2] = deck.deck[i];
			pos2++;
		}
		deck.topCardIndex++;
		deck.size--;
	}
	//the second for is to send the top card to the bottom of the deck
	for (size_t i = deck.topCardIndex; i < DECK_SIZE - 2; i++)
	{
		cardSwap(deck.deck[i], deck.deck[i + 1]);
	}
	player[0].cardsInHand = NUMBER_OF_DEALED_CARDS / 2;
	player[1].cardsInHand = NUMBER_OF_DEALED_CARDS / 2;
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
	std::cout << "1) Target points to win[" << settings.targetPoints << "]" << std::endl;
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
			settings.targetPoints = changeFirstSetting(input);
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
bool checkStrictRules(Player player, size_t playedIndex, RoundState state, Trick currentTrick) {
	if (!currentTrick.firstPlayer) {
		if (isSuitsEqual(player.hand[playedIndex].suit, currentTrick.player1.suit)) {
			return true;
		}
		else if (isSuitInHand(player, currentTrick.player1.suit)) {
			std::cout << "You must play the exact suit or the trump suit!" << std::endl;
			return false;
		}
		else if (isSuitsEqual(player.hand[playedIndex].suit, state.trump)) {
			return true;
		}
		else if (isSuitInHand(player, state.trump)) {
			std::cout << "You must play the exact suit or the trump suit!" << std::endl;
			return false;
		}
	}
	else {
		if (isSuitsEqual(player.hand[playedIndex].suit, currentTrick.player2.suit)) {
			return true;
		}
		else if (isSuitInHand(player, currentTrick.player2.suit)) {
			std::cout << "You must play the exact suit or the trump suit!" << std::endl;
			return false;
		}
		else if (isSuitsEqual(player.hand[playedIndex].suit, state.trump)) {
			return true;
		}
		else if (isSuitInHand(player, state.trump)) {
			std::cout << "You must play the exact suit or the trump suit!" << std::endl;
			return false;
		}
	}
	return true;
}
int cardComp(Card card1, Card card2, RoundState state) {
	if (!state.firstPlayer) {
		if (card1.suit != card2.suit) {
			if (card2.suit == state.trump) {
				return 1;
			}
			else {
				return -1;
			}
		}
		else {
			if (card1.value > card2.value) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}
	else {
		if (card1.suit != card2.suit) {
			if (card1.suit == state.trump) {
				return -1;
			}
			else {
				return 1;
			}
		}
		else {
			if (card1.value > card2.value) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}

}
size_t cardIndex(Player player, Card card) {
	for (size_t i = 0; i < player.cardsInHand; i++)
	{
		if (cardEquals(player.hand[i], card)) {
			return i;
		}
	}
	return -1;
}
void trumpSuit(CardSuit trump, WORD originalColor)
{
	std::cout << "Trump suit: ";
	printSuit(trump, originalColor);
	std::cout << std::endl;
}
void switchNine(Deck& deck, Player& player, RoundState state)
{
	Card trumpNine = { Nine, state.trump };
	if (state.isFirstTrick) {
		std::cout << "You cannot switch now it's the first trick!" << std::endl;
		return;
	}
	else if (!isCardInHand(player, trumpNine)) {
		std::cout << "You don't have trump nine to be able to switch" << std::endl;
		return;
	}
	else if (state.firstPlayer != state.playerInTurn) {
		std::cout << "You have to be first to switch nine" << std::endl;
		return;
	}
	else if (deck.size <= 2) {
		std::cout << "You cannot switch nine now there are two or less cards in the deck!" << std::endl;
		return;
	}
	else if (state.strictRules) {
		std::cout << "You cannot switch nine because the deck is closed" << std::endl;
		return;
	}
	else {
		std::cout << "Player" << (int)state.playerInTurn + 1 << " switched nine with the bottom card" << std::endl;
		cardSwap(player.hand[cardIndex(player, trumpNine)], deck.deck[DECK_SIZE - 1]);
	}
}
void close(Player player, Deck deck, RoundState& state)
{
	if (state.isFirstTrick) {
		std::cout << "You cannot close now it's the first hand" << std::endl;
		return;
	}
	else if (state.firstPlayer != state.playerInTurn) {
		std::cout << "You cannot close now you have to be first" << std::endl;
		return;
	}
	else if (deck.size <= 2) {
		std::cout << "You cannot close now there are two or less cards in the deck!" << std::endl;
		return;
	}
	else if (state.strictRules == true) {
		std::cout << "You cannot close now the game is already closed!" << std::endl;
		return;
	}
	else {
		state.strictRules = true;
		std::cout << "Player " << (int)(state.firstPlayer) + 1 << " closed the deck. Strict rules apply now." << std::endl;
	}
}
void lastTrick(Trick last, RoundState state, WORD originalColor) {
	if (state.isFirstTrick) {
		std::cout << "It's the first trick there is no previous" << std::endl;
		return;
	}
	else {
		if (last.firstPlayer == 1) {
			std::cout << "Player 1: ";
			printCard(last.player2, originalColor);
			std::cout << std::endl << "Player 2: ";
			printCard(last.player1, originalColor);
			std::cout << std::endl << "Winner: Player" << (int)(last.winner) + 1;
			std::cout << std::endl;
		}
		else {
			std::cout << "Player 1: ";
			printCard(last.player1, originalColor);
			std::cout << std::endl << "Player 2: ";
			printCard(last.player2, originalColor);
			std::cout << std::endl;
			std::cout << std::endl << "Winner: Player" << (int)(last.winner) + 1 << std::endl;
		}
	}
}
void marriage(Player& player, GameSettings settings, RoundState& state, char* input, WORD originalColor)
{
	Card king, queen;
	if (strlen(input) != 10 || !strContains(input, " ")) { // Check if the play input is in the desired input form
		std::cout << "Incorrect input, please try again." << std::endl;
		return;
	}
	else if (state.playerInTurn != state.firstPlayer) {
		std::cout << "You have to be first to announce marriage" << std::endl;
		return;
	}
	else if (state.isFirstTrick) {
		std::cout << "It's the first trick you cannot announce now!" << std::endl;
		return;
	}
	if (input[9] == 'h' || input[9] == 'd' || input[9] == 's' || input[9] == 'c') {
		CardSuit announcedSuit;
		switch (input[9]) {
		case 'h': announcedSuit = Hearts; break;
		case 's': announcedSuit = Spades; break;
		case 'c': announcedSuit = Clubs; break;
		case 'd': announcedSuit = Diamonds; break;
		}
		king = { King, announcedSuit };
		queen = { Queen, announcedSuit };
		if (isCardInHand(player, king) && isCardInHand(player, queen)) {
			state.announceMarriage = true;
			std::cout << "Player" << (int)(state.firstPlayer) + 1 << " declared marriage:";
			printCard(queen, originalColor);
			std::cout << "+";
			printCard(king, originalColor);
			if (announcedSuit == state.trump) {
				std::cout << "(trump suit)" << std::endl;
				std::cout << "You earned " << settings.trumpMarriage << " points." << std::endl;
				std::cout << "You must play ";
				printCard(queen, originalColor);
				std::cout << " or ";
				printCard(king, originalColor);
				std::cout << std::endl;
				player.roundPoints += settings.trumpMarriage;
				state.marriageSuit = announcedSuit;
			}
			else {
				std::cout << "(non-trump suit)" << std::endl;
				std::cout << "You earned " << settings.nonTrumpMarriage << " points." << std::endl;
				std::cout << "You must play ";
				printCard(queen, originalColor);
				std::cout << " or ";
				printCard(king, originalColor);
				std::cout << std::endl;
				player.roundPoints += settings.nonTrumpMarriage;
				state.marriageSuit = announcedSuit;
			}
		}
		else {
			std::cout << "You don't have marriage you want to announce!" << std::endl;
			return;
		}
	}
	else {
		std::cout << "Incorrect suit, please try again." << std::endl;
		return;
	}
}
void status(Deck deck, Player player[], RoundState state, GameSettings settings, WORD originalColor)
{
	if (settings.showPoints) {
		std::cout << "Player 1: " << player[0].gamePoints << " game points" << std::endl;
		std::cout << "Player 1: " << player[0].roundPoints << " round points" << std::endl;
		std::cout << "Player 2: " << player[1].gamePoints << " game points" << std::endl;
		std::cout << "Player 2: " << player[1].roundPoints << " round points" << std::endl;

	}
	trumpSuit(state.trump, originalColor);
	std::cout << "Cards left in deck: " << deck.size << std::endl;
	std::cout << "Bottom card: ";
	printCard(deck.deck[DECK_SIZE - 1], originalColor);
	std::cout << std::endl;
	hand(player[state.playerInTurn], state, originalColor);
}
void history(GameHistory gameHistory[], int numberOfRounds, Player player[2])
{
	if (!numberOfRounds) {
		std::cout << "Round 1: Ongoing" << std::endl;
		std::cout << "Overall: Player 1 - " << player[0].gamePoints << " | Player 2 - " << player[1].gamePoints << std::endl;
		return;
	}
	for (size_t i = 0; i < numberOfRounds; i++)
	{
		if (gameHistory[i].tied) {
			std::cout << "Round " << (i + 1) << ": Tied game! | Player 1:" << gameHistory[i].player1RoundPoints
				<< " points | Player 2: " << gameHistory[i].player2RoundPoints << " points"
				<< std::endl;
		}
		std::cout << "Round " << (i + 1) << ":" << " Winner - Player " << (int)gameHistory[i].winner + 1 << "(+" << gameHistory[i].earnedPoints
			<< ") | Player 1: " << gameHistory[i].player1RoundPoints << " points | Player 2: " << gameHistory[i].player2RoundPoints << " points"
			<< std::endl;
	}
	std::cout << "Overall: Player 1 - " << player[0].gamePoints << " | Player 2 - " << player[1].gamePoints << std::endl;
}
void rules()
{
	std::cout << "Santase (66)" << std::endl
		<< "Each player gets 6 cards. After the initial deal the top card is flipped and put at the bottom of the deck." << std::endl
		<< "This is card shows the trump suit." << std::endl
		<< "Card values: A=11, 10=10, K=4, Q=3, J=2, 9=0." << std::endl
		<< "A marriage (K+Q of the same suit) gives 20 points, or 40 if the trump suit."
		<< "(These values can be changed in the settings menu before the game starts)" << std::endl
		<< "These are the functions a player can call in this game:" << std::endl
		<< "Warning!!! These functions are called only when the input is in correct form!!!" << std::endl
		<< "Settings - This function let's the player modify some settings of the game before its start" << std::endl
		<< "Start - Starts a new game" << std::endl
		<< "Player [Index 0-5] - Let's the player in turn play the desired card from their hand" << std::endl
		<< "Hand - Lets the player in turn see their hand" << std::endl
		<< "Status: Gives a detail description of the game's status (round and game points, trump suit, ect.)" << std::endl
		<< "Switch-nine - Lets the player in turn switch trump nine from his hand with the bottom card" << std::endl
		<< "Trump - Prints the trump suit" << std::endl
		<< "Last-trick - Shows the last played trick" << std::endl
		<< "Marriage [first letter of suit] - Lets a player who has won at least one trick and is first to declare marriage" << std::endl
		<< "History - Gives information about previous rounds" << std::endl
		<< "Close - Closes the deck and enforces strict rules" << std::endl
		<< "Stop - If a player thinks that he has 66 points he can stop the round, game points are calculated and a new round is started"
		<< std::endl
		<< "Surrender - The player in turn surrenders the round and the other player wins the round" << std::endl
		<< "Surrender-forever - The player in turn surrenders the game and the other player wins" << std::endl
		<< "Save [file name] - Saves the game to a text file "
		<< "(Warning if you try to save a game to an already existing game file the old data will be lost!)" << std::endl
		<< "Load [file name] - Load a saved game from a text file"
		<< "(Warning this function works only with saved file created by the save function listed above!!!)" << std::endl
		<< "Strict rules: The secondd player must answer the suit that the first player has played if he has it." << std::endl
		<< "If he doesn't have it he must play a trump card and if he doesn't have that either he is free to play whatever." << std::endl
		<< "Important note: At the start of a new game the person who won the previous game starts first." << std::endl;
}
// file manip
void subStrForFiles(char* input, int start, int end)
{
	int pos = 0;
	for (int i = start; i < end; i++)
	{
		input[pos++] = input[i];
		if (i == end - 1) {
			input[pos] = '\0';
		}
	}
	input[pos++] = '.';
	input[pos++] = 't';
	input[pos++] = 'x';
	input[pos++] = 't';
	input[pos] = '\0';

}
size_t numberSubStr(char* str)
{
	int pos = 0;
	for (size_t i = 0; str[i]; i++)
	{
		if (isCharANumber(str[i])) {
			str[pos++] = str[i];
		}
	}
	str[pos] = '\0';
	return stringToNumber(str);
}
void writeSuit(CardSuit suit, std::ofstream& file)
{
	if (suit == CardSuit::Spades) {
		file << "S";
	}
	else if (suit == CardSuit::Hearts) {
		file << "H";
	}
	else if (suit == CardSuit::Diamonds) {
		file << "D";
	}
	else {
		file << "C";
	}
}
void writeValue(CardValue value, std::ofstream& file)
{
	if (value == Ace) {
		file << "A";
	}
	else if (value == Ten) {
		file << "1"; // it is easier to read if it is single digit number
	}
	else if (value == Nine) {
		file << "9";
	}
	else if (value == Jack) {
		file << "J";
	}
	else if (value == King) {
		file << "K";
	}
	else if (value == Queen) {
		file << "Q";
	}
}
void save(Deck deck, Player player[], GameSettings gameSettings, RoundState state, GameHistory gameHistory[], Trick last, int numberOfRounds, char* input)
{
	if (strlen(input) <= 5) { // the correct input for save function call is save [nameOfFile] which has at least more than 5 characters including the whitespace
		std::cout << "Incorrect file name" << std::endl;
		return;
	}
	subStrForFiles(input, 5, strlen(input));
	std::ofstream file;
	file.open(input);
	if (!file) {
		std::cout << "File could not be opened. Try again";
		return;
	}
	file << "Rounds_played " << numberOfRounds << std::endl;
	file << "Settings" << std::endl;
	file << "Target_points " << gameSettings.targetPoints << std::endl;
	file << "Trump_marriage " << gameSettings.trumpMarriage << std::endl;
	file << "Non_trump_marriage " << gameSettings.nonTrumpMarriage << std::endl;
	file << "Last_trick " << gameSettings.lastTrick << std::endl;
	file << "Show_points " << gameSettings.showPoints << std::endl << std::endl;

	file << "Round_state" << std::endl;
	file << "Player_in_turn " << (int)state.playerInTurn << std::endl;
	file << "First_player " << (int)state.firstPlayer << std::endl;
	file << "Is_first_trick " << (int)state.isFirstTrick << std::endl;
	file << "Strict_rules " << (int)state.strictRules << std::endl;
	file << "Announced_marriage " << (int)state.announceMarriage << std::endl;
	file << "Marriage_suit ";
	writeSuit(state.marriageSuit, file);
	file << std::endl;
	file << "Trump: ";
	writeSuit(state.trump, file);
	file << std::endl << std::endl;
	file << "Last_trick: " << std::endl;
	file << "Player1: ";
	writeValue(last.player1.value, file);
	writeSuit(last.player1.suit, file);
	file << std::endl;
	file << "Player2: ";
	writeValue(last.player2.value, file);
	writeSuit(last.player2.suit, file);
	file << std::endl;
	file << "Winner: " << (int)last.winner << std::endl;
	file << "First_player: " << (int)last.firstPlayer << std::endl;
	file << std::endl;
	file << "Players" << std::endl;
	for (int i = 0; i < 2; i++) {
		file << "Player " << i << std::endl;
		file << "Game_points " << player[i].gamePoints << std::endl;
		file << "Round_points " << player[i].roundPoints << std::endl;
		file << "Cards_in_hand " << player[i].cardsInHand << std::endl;

		file << "Last_card ";
		writeValue(player[i].lastCardDrawn.value, file);
		writeSuit(player[i].lastCardDrawn.suit, file);
		file << std::endl;

		file << "Hand ";
		for (int j = 0; j < player[i].cardsInHand; j++) {
			writeValue(player[i].hand[j].value, file);
			writeSuit(player[i].hand[j].suit, file);
			if (j + 1 < player[i].cardsInHand) file << ',';
		}
		file << std::endl;
	}

	file << "Deck " << deck.size << std::endl;
	file << "Top_card_index " << deck.topCardIndex << std::endl;
	for (size_t i = deck.topCardIndex; i < DECK_SIZE; i++) {
		writeValue(deck.deck[i].value, file);
		writeSuit(deck.deck[i].suit, file);
		if (i < DECK_SIZE - 1) file << ',';
	}
	file << std::endl << std::endl;

	file << "History " << std::endl;
	for (int i = 0; i < numberOfRounds; i++) {
		file << "Round " << i << ' '
			<< "Winner " << (int)gameHistory[i].winner << ' '
			<< "Earned " << gameHistory[i].earnedPoints << ' '
			<< "Player1 " << gameHistory[i].player1RoundPoints << ' '
			<< "Player2 " << gameHistory[i].player2RoundPoints << ' '
			<< "Tied " << gameHistory[i].tied << std::endl;
	}

	std::cout << "Game successfully saved to file " << input << std::endl;
	file.close();
}
CardSuit readSuit(std::ifstream& file)
{
	char ch;
	file >> ch;
	if (ch == ' ' || ch == '\n') {
		file >> ch;
	}
	if (ch == 'S') {
		return Spades;
	}
	else if (ch == 'D') {
		return Diamonds;
	}
	else if (ch == 'H') {
		return Hearts;
	}
	else {
		return Clubs;
	}
}
CardValue readValue(std::ifstream& file)
{
	char ch = file.get();
	if (ch == ' ' || ch == '\n') {
		file >> ch;
	}
	if (ch == 'A') {
		return Ace;
	}
	else if (ch == 'K') {
		return King;
	}
	else if (ch == 'Q') {
		return Queen;
	}
	else if (ch == '1') {
		return Ten;
	}
	else if (ch == '9') {
		return Nine;
	}
	else {
		return Jack;
	}
}
Card readCard(std::ifstream& file)
{
	Card card;
	card.value = readValue(file);
	card.suit = readSuit(file);
	return card;
}
void readHand(Card hand[], size_t cardsInHand, std::ifstream& file)
{
	char ch;
	for (size_t i = 0; i < cardsInHand; i++)
	{
		hand[i] = readCard(file);
		if (i != cardsInHand - 1) {
			file >> ch;
		}
	}
}
void readDeck(Deck& deck, std::ifstream& file)
{
	char ch;
	for (size_t i = deck.topCardIndex; i < DECK_SIZE; i++)
	{
		deck.deck[i] = readCard(file);
		if (i != DECK_SIZE - 1) {
			file >> ch;
		}
	}
}
bool load(Deck& deck, Player player[], GameSettings& gameSettings, RoundState& state, GameHistory gameHistory[], Trick& last, int& numberOfRounds, char* input)
{
	if (strlen(input) <= 5) { // the correct input for save function call is load [nameOfFile] which has at least more than 5 characters including the whitespace
		std::cout << "Incorrect file name" << std::endl;
		return false;
	}
	char temp[BUFFER];
	subStrForFiles(input, 5, strlen(input));
	std::ifstream file;
	file.open(input);
	if (!file) {
		std::cout << "File could not be opened. Try again";
		return false;
	}
	int num = 0; // reading variable
	file >> temp >> num;
	numberOfRounds = num;
	// settings
	file >> temp;
	file >> temp >> num;
	gameSettings.targetPoints = num;
	file >> temp >> num;
	gameSettings.trumpMarriage = num;
	file >> temp >> num;
	gameSettings.nonTrumpMarriage = num;
	file >> temp >> num;
	gameSettings.lastTrick = (bool)num;
	file >> temp >> num;
	gameSettings.showPoints = (bool)num;
	// state
	file >> temp;
	file >> temp >> num;
	state.playerInTurn = (bool)num;
	file >> temp >> num;
	state.firstPlayer = (bool)num;
	file >> temp >> num;
	state.isFirstTrick = (bool)num;
	file >> temp >> num;
	state.strictRules = (bool)num;
	file >> temp >> num;
	state.announceMarriage = (bool)num;
	file >> temp;
	state.marriageSuit = readSuit(file);
	file >> temp;
	state.trump = readSuit(file);
	file >> temp; // last trick
	file >> temp;
	last.player1 = readCard(file);
	file >> temp;
	last.player2 = readCard(file);
	file >> temp >> num;
	last.winner = (bool)num;
	file >> temp >> num;
	last.firstPlayer = (bool)num;

	file >> temp; // players
	for (int i = 0; i < 2; i++) {
		file >> temp >> temp;

		file >> temp >> player[i].gamePoints;
		file >> temp >> player[i].roundPoints;
		file >> temp >> player[i].cardsInHand;

		file >> temp;
		player[i].lastCardDrawn = readCard(file);
		file >> temp; // hand
		readHand(player[i].hand, player[i].cardsInHand, file);
	}
	// deck
	file >> temp >> deck.size;
	file >> temp >> deck.topCardIndex;
	readDeck(deck, file);
	// history
	file >> temp;
	for (int i = 0; i < numberOfRounds; i++) {
		file >> temp >> temp;               // Round i
		file >> temp >> num;
		gameHistory[i].winner = (bool)num;
		file >> temp >> num;
		gameHistory[i].earnedPoints = num;
		file >> temp >> num;
		gameHistory[i].player1RoundPoints = num;
		file >> temp >> num;
		gameHistory[i].player2RoundPoints = num;
		file >> temp >> num;
		gameHistory[i].tied = (bool)num;
	}
	file.close();
	std::cout << "Game loaded from file!" << std::endl;
	return true;
}
// round and game functions
void draw(Player player[], Deck& deck, RoundState state)
{
	player[0].cardsInHand++;
	player[1].cardsInHand++;
	player[state.firstPlayer].hand[player[state.firstPlayer].cardsInHand - 1] = deck.deck[deck.topCardIndex];
	player[state.firstPlayer].lastCardDrawn = deck.deck[deck.topCardIndex];
	deck.topCardIndex++;
	deck.size--;
	player[!state.firstPlayer].hand[player[!state.firstPlayer].cardsInHand - 1] = deck.deck[deck.topCardIndex];
	player[!state.firstPlayer].lastCardDrawn = deck.deck[deck.topCardIndex];
	deck.topCardIndex++;
	deck.size--;
}
void resetRound(Player player[], RoundState& state, Deck& deck, WORD originalColor)
{
	deck.size = DECK_SIZE;
	deck.topCardIndex = 0;
	for (size_t i = 0; i < 2; i++)
	{
		player[i].cardsInHand = CARDS_IN_HAND;
		player[i].roundPoints = 0;
	}
	state.isFirstTrick = true;
	state.strictRules = false;
	fillDeck(deck);
	shuffleDeck(deck);
	startingDeal(player, deck);
	state.trump = deck.deck[DECK_SIZE - 1].suit;
	orderHand(player[0]);
	orderHand(player[1]);
}
bool play(Player& player, RoundState& state, Trick& currentTrick, char* input, WORD originalColor)
{
	if (strlen(input) != 6 || !strContains(input, " ")) { // Check if the play input is in the desired input form
		std::cout << "Incorrect card index, please try again." << std::endl;
		return false;
	}
	else if (!isCharANumber(input[5])) { // Here the input array is already in the correct input form so the desired number is in the fifth index of the string
		std::cout << "Incorrect card index, please try again." << std::endl;
		return false;
	}
	else if (charToNumber(input[5]) >= player.cardsInHand || charToNumber(input[5]) < 0) {
		std::cout << "Incorrect card index, please try again." << std::endl;
		return false;
	}
	size_t playedIndex = charToNumber(input[5]);
	if (state.firstPlayer == state.playerInTurn) {
		if (state.announceMarriage) {
			Card king = { King, state.marriageSuit }, queen = { Queen, state.marriageSuit };
			if (!cardEquals(player.hand[playedIndex], king) && !cardEquals(player.hand[playedIndex], queen)) {
				std::cout << "You have announced marriage. You must play ";
				printCard(king, originalColor);
				std::cout << " or ";
				printCard(queen, originalColor);
				std::cout << std::endl;
				return false;
			}
			state.announceMarriage = false;
		}
		if (!state.firstPlayer) {
			currentTrick.player1 = player.hand[playedIndex];
		}
		else {
			currentTrick.player2 = player.hand[playedIndex];
		}
		currentTrick.firstPlayer = state.firstPlayer;
	}
	else {
		if (state.strictRules) {
			if (!checkStrictRules(player, playedIndex, state, currentTrick)) {
				return false;
			}
		}
		if (!state.playerInTurn) {
			currentTrick.player1 = player.hand[playedIndex];
		}
		else {
			currentTrick.player2 = player.hand[playedIndex];
		}
	}
	std::cout << "Player " << (int)state.playerInTurn + 1 << " played: ";
	printValue(player.hand[playedIndex].value);
	printSuit(player.hand[playedIndex].suit, originalColor);
	std::cout << std::endl;
	cardSwap(player.hand[playedIndex], player.hand[player.cardsInHand - 1]);
	player.cardsInHand--;
	return true;
}
void round(Deck& deck, Player player[], GameSettings settings, RoundState& state, GameHistory gameHistory[], int& numberOfRounds, char* input, WORD originalColor)
{
	Trick currentTrick, last;
	while (true) {
		std::cout << "Player " << ((int)state.playerInTurn + 1) << " turn:" << std::endl;
		while (true) {
			userInput(input);
			formatUserInput(input);
			if (!strcomp(input, "settings")) {
				std::cout << "Cannot change the settings while in game!" << std::endl;
				continue;
			}
			else if (!strcomp(input, "start")) {
				std::cout << "Game already started!" << std::endl;
				continue;
			}
			else if (!strcomp(input, "rules")) {
				rules();
				continue;
			}
			else if (!strcomp(input, "hand")) {
				hand(player[state.playerInTurn], state, originalColor);
				continue;
			}
			else if (!strcomp(input, "trump")) {
				trumpSuit(state.trump, originalColor);
				continue;
			}
			else if (!strcomp(input, "switch-nine")) {
				switchNine(deck, player[state.playerInTurn], state);
				continue;
			}
			else if (!strcomp(input, "close")) {
				close(player[state.playerInTurn], deck, state);
				continue;
			}
			else if (!strcomp(input, "last-trick")) {
				lastTrick(last, state, originalColor);
				continue;
			}
			else if (!strcomp(input, "status")) {
				status(deck, player, state, settings, originalColor);
				continue;
			}
			else if (strContains(input, "marriage ")) {
				marriage(player[state.playerInTurn], settings, state, input, originalColor);
				continue;
			}
			else if (!strcomp(input, "history")) {
				history(gameHistory, numberOfRounds, player);
				continue;
			}
			else if (strContains(input, "save ")) {
				if (state.firstPlayer != state.playerInTurn) {
					std::cout << "You must be first in order to save!" << std::endl;
				}
				else {
					save(deck, player, settings, state, gameHistory, last, numberOfRounds, input);
				}
				continue;
			}
			else if (strContains(input, "load ")) {
				if (load(deck, player, settings, state, gameHistory, last, numberOfRounds, input)) {
					break;
				}
				continue;
			}
			else if (!strcomp(input, "stop")) {
				if (state.firstPlayer != state.playerInTurn) {
					std::cout << "You cannot stop now you have to be first!" << std::endl;
					continue;
				}
				break;
			}
			else if (!strcomp(input, "exit")) {
				break;
			}
			else if (!strcomp(input, "surrender")) {
				break;
			}
			else if (!strcomp(input, "surrender-forever")) {
				break;
			}
			else if (strContains(input, "play")) { // the correct input for the play function is "play [num]" where num is an index and num is in range [0-5]
				if (play(player[state.playerInTurn], state, currentTrick, input, originalColor)) {
					break;
				}
			}
			else {
				std::cout << "Incorrect input. Try again." << std::endl;
			}
		}
		//end of current trick
		if (!strcomp(input, "stop") || !strcomp(input, "exit") || !strcomp(input, "surrender") || !strcomp(input, "surrender-forever")) {
			break;
		}
		if (strContains(input, "txt")) {
			continue;
		}
		else if (state.playerInTurn != state.firstPlayer) {
			if (cardComp(currentTrick.player1, currentTrick.player2, state) == 1) { // Player 2 won the trick
				player[1].roundPoints += currentTrick.player1.value + currentTrick.player2.value;
				std::cout << "Player 2 wins the trick! (+" << (currentTrick.player1.value + currentTrick.player2.value) << " points)" << std::endl;
				last = currentTrick;
				last.winner = true; // second player won
				state.playerInTurn = last.winner;
				state.firstPlayer = last.winner;
				if (!state.strictRules) {
					draw(player, deck, state);
					orderHand(player[0]);
					orderHand(player[1]);
				}

			}
			else {
				player[0].roundPoints += currentTrick.player1.value + currentTrick.player2.value;
				std::cout << "Player 1 wins the trick! (+" << (currentTrick.player1.value + currentTrick.player2.value) << " points)" << std::endl;
				last = currentTrick;
				last.winner = false; // first player won
				state.playerInTurn = last.winner;
				state.firstPlayer = last.winner;
				if (!state.strictRules) {
					draw(player, deck, state);
					orderHand(player[0]);
					orderHand(player[1]);
				}

			}
			if (player[state.playerInTurn].cardsInHand == 0) {
				if (settings.lastTrick) {
					player[last.winner].roundPoints += 10;
				}
				break;
			}
			state.isFirstTrick = false;
		}
		else {
			state.playerInTurn = !state.playerInTurn;
		}
		if (deck.size == 0) {
			state.strictRules = true;
			deck.size++;
			std::cout << "All cards in deck are taken. Strict rules apply now!" << std::endl;
		}
	}
}
void game(Deck& deck, Player player[], GameSettings settings, RoundState& state, GameHistory* gameHistory, int& numberOfRounds, char* input, WORD originalColor)
{
	std::cout << "Game started!" << std::endl;
	do {
		round(deck, player, settings, state, gameHistory, numberOfRounds, input, originalColor);
		if (!strcomp(input, "exit")) {
			break;
		}
		if (!strcomp(input, "surrender")) {
			std::cout << "Player" << (int)state.playerInTurn + 1 << " surrendered the round." << std::endl;
			std::cout << "Player" << (int)!state.playerInTurn + 1 << " won the round!";
			if (player[state.playerInTurn].roundPoints == 0) {
				player[!state.playerInTurn].gamePoints += 3;
				gameHistory[numberOfRounds].earnedPoints = 3;
				std::cout << "(+3)" << std::endl;
			}
			else {
				player[!state.playerInTurn].gamePoints += 2;
				gameHistory[numberOfRounds].earnedPoints = 2;
				std::cout << "(+2)" << std::endl;
			}
			gameHistory[numberOfRounds].winner = !state.playerInTurn;
			gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
			gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
			numberOfRounds++;
			state.firstPlayer = !state.playerInTurn;
			state.playerInTurn = state.firstPlayer;
		}
		else if (!strcomp(input, "surrender-forever")) {
			std::cout << "Player" << (int)state.playerInTurn + 1 << " surrendered the game." << std::endl;
			std::cout << "Player" << (int)!state.playerInTurn + 1 << " wins the game." << std::endl;
			state.firstPlayer = !state.playerInTurn;
			state.strictRules = state.firstPlayer;
			break;
		}
		else if (!strcomp(input, "stop")) {
			if (player[state.firstPlayer].roundPoints >= 66) {
				std::cout << "Player " << (int)state.firstPlayer + 1 << " won the round!" << std::endl;
				if (player[!state.firstPlayer].roundPoints >= 33) {
					player[state.firstPlayer].gamePoints += 1;
					gameHistory[numberOfRounds].earnedPoints = 1;
					std::cout << "(+1)" << std::endl;
				}
				else if (player[!state.firstPlayer].roundPoints == 0) {
					player[state.firstPlayer].gamePoints += 3;
					gameHistory[numberOfRounds].earnedPoints = 3;
					std::cout << "(+3)" << std::endl;
				}
				else {
					player[state.firstPlayer].gamePoints += 2;
					gameHistory[numberOfRounds].earnedPoints = 2;
					std::cout << "(+2)" << std::endl;
				}
				state.playerInTurn = state.firstPlayer;
				gameHistory[numberOfRounds].winner = state.firstPlayer;
				gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
				gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
				numberOfRounds++;
			}
			else {
				std::cout << "Player " << (int)(!state.firstPlayer) + 1 << " won the round!" << std::endl;
				if (player[state.firstPlayer].roundPoints == 0) {
					player[!state.firstPlayer].gamePoints += 3;
					gameHistory[numberOfRounds].earnedPoints = 3;
					std::cout << "(+3)" << std::endl;
				}
				else {
					player[!state.firstPlayer].gamePoints += 2;
					gameHistory[numberOfRounds].earnedPoints = 2;
					std::cout << "(+2)" << std::endl;
				}
				state.firstPlayer = !state.firstPlayer;
				state.playerInTurn = state.firstPlayer;
				gameHistory[numberOfRounds].winner = !state.firstPlayer;
				gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
				gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
				numberOfRounds++;
			}
		}
		else {
			if (player[state.firstPlayer].roundPoints == player[!state.firstPlayer].roundPoints) {
				std::cout << "Tied round!" << std::endl;
				state.firstPlayer = 0;
				state.playerInTurn = state.firstPlayer;
				gameHistory[numberOfRounds].tied = true;
				gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
				gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
				numberOfRounds++;
			}
			else if (player[state.firstPlayer].roundPoints > player[!state.firstPlayer].roundPoints) {
				std::cout << "Player " << (int)state.firstPlayer + 1 << " won the round!" << std::endl;
				if (player[!state.firstPlayer].roundPoints >= 33) {
					player[state.firstPlayer].gamePoints += 1;
					gameHistory[numberOfRounds].earnedPoints = 1;
					std::cout << "(+1)" << std::endl;
				}
				else if (player[!state.firstPlayer].roundPoints == 0) {
					player[state.firstPlayer].gamePoints += 3;
					gameHistory[numberOfRounds].earnedPoints = 3;
					std::cout << "(+3)" << std::endl;
				}
				else {
					player[state.firstPlayer].gamePoints += 2;
					gameHistory[numberOfRounds].earnedPoints = 2;
					std::cout << "(+2)" << std::endl;
				}
				state.playerInTurn = state.firstPlayer;
				gameHistory[numberOfRounds].winner = state.firstPlayer;
				gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
				gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
				numberOfRounds++;
			}
			else {
				std::cout << "Player " << (int)(!state.firstPlayer) + 1 << " won the round!" << std::endl;
				if (player[state.firstPlayer].roundPoints >= 33) {
					player[!state.firstPlayer].gamePoints += 1;
					gameHistory[numberOfRounds].earnedPoints = 1;
					std::cout << "(+1)" << std::endl;
				}
				else if (player[state.firstPlayer].roundPoints == 0) {
					player[!state.firstPlayer].gamePoints += 3;
					gameHistory[numberOfRounds].earnedPoints = 3;
					std::cout << "(+3)" << std::endl;
				}
				else {
					player[!state.firstPlayer].gamePoints += 2;
					gameHistory[numberOfRounds].earnedPoints = 2;
					std::cout << "(+2)" << std::endl;
				}
				state.firstPlayer = !state.firstPlayer;
				state.playerInTurn = state.firstPlayer;
				gameHistory[numberOfRounds].winner = state.firstPlayer;
				gameHistory[numberOfRounds].player1RoundPoints = player[0].roundPoints;
				gameHistory[numberOfRounds].player2RoundPoints = player[1].roundPoints;
				numberOfRounds++;
			}
		}
		if (player[0].gamePoints >= 11) {
			std::cout << "Player 1 wins the game!" << std::endl;
			state.firstPlayer = false;
			break;
		}
		else if (player[1].gamePoints >= 11) {
			std::cout << "Player 2 wins the game!" << std::endl;
			state.firstPlayer = true;
			break;
		}
		else if (numberOfRounds == MAX_ROUNDS) {
			std::cout << "You have defined all odds and have played 100 rounds! Can you believe it! Let's call it a day and end the game." << std::endl;
			if (player[0].gamePoints > player[1].gamePoints) {
				std::cout << "Player 1 wins the game!" << std::endl;
				state.firstPlayer = false;
			}
			else {
				std::cout << "Player 2 wins the game!" << std::endl;
				state.firstPlayer = true;
			}
			break;
		}
		resetRound(player, state, deck, originalColor);
	} while (player[0].gamePoints < settings.targetPoints && player[1].gamePoints < settings.targetPoints);
}
int main()
{
	SetConsoleOutputCP(CP_UTF8);
	WORD originalColor = getCurrentConsoleColor();
	Player player[2];
	char* input = new char[BUFFER];
	bool startedGame = false;
	GameSettings gameSettings;
	RoundState state;
	Deck deck;
	GameHistory gameHistory[MAX_ROUNDS];
	int numberOfRounds = 0;
	do {
		std::cout << "Santase(66)" << std::endl;
		std::cout << "To start game enter 'start'." << std::endl;
		std::cout << "To view the rules enter 'rules'." << std::endl;
		std::cout << "To change the settings of the game enter 'settings'." << std::endl;
		std::cout << "To exit application enter 'exit'" << std::endl;
		userInput(input);
		formatUserInput(input);
		if (!strcomp(input, "start")) {
			startedGame = true;
			resetRound(player, state, deck, originalColor);
			player[0].gamePoints = 0;
			player[1].gamePoints = 0;
			numberOfRounds = 0;
			game(deck, player, gameSettings, state, gameHistory, numberOfRounds, input, originalColor);
		}
		else if (!strcomp(input, "settings") && !startedGame) {
			settings(gameSettings, input);
		}
		else if (!strcomp(input, "rules")) {
			rules();
		}
		else if (!strcomp(input, "exit")) {
			delete[] input;
			return 0;
		}
		else {
			std::cout << "Incorrect input. Try again." << std::endl;
		}
	} while (true);

	delete[] input;
}