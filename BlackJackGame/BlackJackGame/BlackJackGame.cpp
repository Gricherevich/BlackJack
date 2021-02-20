#include <iostream>
#include <time.h>
#include <math.h>
#include <array>
#include <string>
#include <utility>
#include <iomanip>
#include <random>

static uint16_t chipsNumbers{ 1000 }, betChips;
static uint16_t chipsBotNumbers{ 10000 }, betBotChips;
static const uint16_t DECK_SIZE{ 52 };
static const uint16_t MAX_PLAYER_SIZE{ 10 };

enum cardStrength
{
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE,
	SIZE_OF_STRENGTH
};

enum cardSuit
{
	DIAMONDS,
	SPADES,
	CLUBS,
	HEARTS,
	SIZE_OF_SUIT
};

struct Card
{
	cardStrength paramStrength;
	cardSuit paramSuit;
};

bool startExitGame()
{
	std::string playerInput;

	while (true)
	{
		std::getline(std::cin, playerInput);
		if (playerInput == "play")
			return true;
		if (playerInput == "exit")
			return false;
		else
			std::cout << "Unknown command. Print again.\n";
	}
}

bool replay()
{
	std::string playerInput;

	std::getline(std::cin, playerInput);

	if (playerInput == "enough")
		return false;
}

//make deck of cards
void getDeck(Card *oneCard)
{
	for (int i = 0; i < DECK_SIZE;)
	{
		for (int outIndex = 0; outIndex < SIZE_OF_SUIT; outIndex++)
		{
			for (int inIndex = 0; inIndex < SIZE_OF_STRENGTH; inIndex++)
			{
				oneCard[i].paramSuit = static_cast<cardSuit>(outIndex);
				oneCard[i].paramStrength = static_cast<cardStrength>(inIndex);
				++i;
			}
		}
	}
}

void printCard(Card oneCard)
{
	switch (oneCard.paramStrength)
	{
	case TWO: std::cout << "TWO "; break;
	case THREE: std::cout << "THREE "; break;
	case FOUR: std::cout << "FOUR "; break;
	case FIVE: std::cout << "FIVE "; break;
	case SIX: std::cout << "SIX "; break;
	case SEVEN: std::cout << "SEVEN "; break;
	case EIGHT: std::cout << "EIGHT "; break;
	case NINE: std::cout << "NINE "; break;
	case TEN: std::cout << "TEN "; break;
	case JACK: std::cout << "JACK "; break;
	case QUEEN: std::cout << "QUEEN "; break;
	case KING: std::cout << "KING "; break;
	case ACE: std::cout << "ACE "; break;
	}
	switch (oneCard.paramSuit)
	{
	case DIAMONDS: std::cout << "DIAMONDS \n"; break;
	case SPADES: std::cout << "SPADES \n"; break;
	case HEARTS: std::cout << "HEARTS \n"; break;
	case CLUBS: std::cout << "CLUBS \n"; break;
	}
}

void shuffleCard(Card *deckOfCard)
{
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> uid(0, DECK_SIZE - 1);

	for (int index = 0; index < DECK_SIZE; index++)
	{
		for (int indexIn = index + 1; indexIn < DECK_SIZE; indexIn++)
		{
			std::swap(deckOfCard[index], deckOfCard[uid(gen)]);
		}
	}
}
// return value of card 
uint16_t getCardValue(Card oneCard)
{
	switch (oneCard.paramStrength)
	{
	case TWO: return 2; break;
	case THREE: return 3; break;
	case FOUR: return 4; break;
	case FIVE: return 5; break;
	case SIX: return 6; break;
	case SEVEN: return 7; break;
	case EIGHT: return 8; break;
	case NINE: return 9; break;
	case TEN: return 10; break;
	case JACK: return 10; break;
	case QUEEN: return 10; break;
	case KING: return 10; break;
	case ACE: return 1; break;
	}
}

// give you cards 
Card getCard(Card playerCard, Card *deckOfCard, int16_t &result)
{
	std::random_device rd;
	std::uniform_int_distribution<int> uid(0, 50);

	playerCard = deckOfCard[uid(rd)];
	result += getCardValue(playerCard);
	return playerCard;

}

void winCards(const int16_t botResult,const int16_t playerResult, int16_t &playerBet, int16_t &botBet)
{
	uint16_t resultOfBet = playerBet + botBet;

	if ((botResult > 21) && (playerResult > 21))
	{
		chipsNumbers -= playerBet;
		chipsBotNumbers -= betBotChips;
		std::cout << "Both lost\n";
	}
	if ((botResult > 21) && (playerResult <= 21))
	{
		chipsNumbers += resultOfBet;
		std::cout << "Player wins.\n";
	}
	if ((botResult <= 21) && (playerResult > 21))
	{
		chipsBotNumbers += resultOfBet;
		std::cout << "Diller wins\n";
	}
	if (botResult == playerResult)
	{
		chipsNumbers += playerBet;
		chipsBotNumbers += betBotChips;
		std::cout << "Draw\n";
	}

	std::cout << "The winning amount is " << resultOfBet << "\n";
}

int16_t getBet()
{
	std::cout << "You have " << chipsNumbers << " chips: \n";
	while (true)
	{
		std::cout << "Input your bet: \n";
		std::cin >> betChips;

		if (betChips > chipsNumbers)
		{
			std::cout << "You don't have enough chips. Bet again!";
			continue;
		}
		else
		{
			chipsNumbers -= betChips;
			return betChips;
		}
	}


}

int16_t getBetBot(int16_t const playsedChips)
{

	std::mt19937 botBetRand(time(0));
	std::uniform_int_distribution<> difference(-100, 100);

	if (playsedChips)
	{
		betBotChips = playsedChips + difference(botBetRand);
		chipsBotNumbers -= betBotChips;
		return betBotChips;
	}
	else
		return 0;

}

int main()
{
	Card oneCard[52]{};
	getDeck(oneCard);
	
	std::cout << "\t\tWELCOME TO THE BLACKJACK TABLE\n"
		<< "For start game print 'play', for exit print 'exit'.\n";

	while (startExitGame())
	{
		int16_t result{ 0 };
		shuffleCard(oneCard);
		Card cardOnTheTableFirst{};
		Card cardOnTheTableSecond{};

		if (chipsNumbers == 0)
		{
			std::cout << "You lose! You spend all chips!\n";
			break;
		}

		int16_t playerBet = getBet();
		int16_t botBet = getBetBot(playerBet);

		cardOnTheTableSecond = getCard(cardOnTheTableSecond, oneCard, result);
		bool isWork = false;
		std::cout << "Your cards: \n";
		while (replay())
		{
			cardOnTheTableFirst = getCard(cardOnTheTableFirst, oneCard, result);
			printCard(cardOnTheTableFirst);
			if (!isWork)
			{
				printCard(cardOnTheTableSecond);
				isWork = true;
			}
			std::cout << result << std::endl;
			if (result == 21)
			{
				std::cout << "\nBlackjack!\n";
				break;
			}
			else if (result > 21)
			{
				std::cout << "\nExcess!\n";
				break;
			}
		}

		//bot logic
		Card botFirstCard{}, botSecondCard{};
		int16_t botResult{ 0 };
		bool isbotWork = false;
		botSecondCard = getCard(botSecondCard, oneCard, botResult);

		std::cout << "Diller cards:\n";
		while (botResult < 17)
		{
			botFirstCard = getCard(botFirstCard, oneCard, botResult);
			printCard(botFirstCard);
			if (!isbotWork)
			{
				printCard(botSecondCard);
				isbotWork = true;
			}
			if (botResult == 21)
			{
				std::cout << "\nBlackjack!\n";
				break;
			}
			else if (botResult > 21)
			{
				std::cout << "\nExcess!\n";
				break;
			}
		}
		std::cout << botResult << std::endl;
		winCards(botResult, result, playerBet, botBet);
		std::cout << "\nPlay again?(print 'play', or 'exit')\n";
	}

	return 0;
}

