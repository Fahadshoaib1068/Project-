#include<iostream>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<iomanip>
#include<fstream>
#include<cstring>
using namespace std;

#define DECK_SIZE 108  //total deck
#define PLAYER_HAND_SIZE 7 //per person cards

// Constants for colors
const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;

bool skip = false;         // Track whether the next player's turn is skipped
bool reverse = false;      // Track the direction of play
int drawCardCount = 0;     // Track how many cards the next player should draw
int currentPlayer = 1;     // Track the current player (1 or 2)
int top_card[2];        // 0 for colour and 1 for type

//Function prototypes
void initializeDeck(int deck[][15]);
void shuffleDeck(int deck[][15], int deck_size); 
void dealCards(int deck[][15], int player[][15], int& deckIndex);
void PrintPlayerHand(int player[][15], int handSize);
void displayTopCard();
void drawCard(int deck[][15], int& deckIndex, int player[][15], int& handSize);
bool isValidPlay(int playerColor, int playerType);
void PrintUpdatedBoard(int player1[][15], int player1HandSize, int player2[][15], int player2HandSize);
void UpdateDiscardPile(int playedCardColor, int playedCardType);
void handleSpecialCard(int specialCardType);
void playTurn(int player[][15], int& handSize, int& deckIndex, int deck[][15], int player1[][15], int player1HandSize, int player2[][15], int player2HandSize);
bool callUno(int playerHandSize);
void saveLoadGameResult(int player1Wins, int player2Wins);
void HomePageMenu()
{
	cout<<setw(70)<<"WELCOME TO A GAME OF UNO\n\n"<<endl;
    cout<<setw(30)<<"1. PLAY GAME (P)"<<endl<<endl;
    cout<<setw(30)<<"2. SEE SCORE (S)"<<endl<<endl;
    cout<<setw(25)<<"3. Exit (E)"<<endl;
}

void setColor(int textColor, int bgColor) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,(bgColor << 4) | textColor);
}

int main() 
{
    while(true)
    {
    	system("cls");
	int deck[DECK_SIZE][15] = {0};
    int player1[DECK_SIZE][15] = {0};
    int player2[DECK_SIZE][15] = {0};
    int deckIndex = 0;
    int player1HandSize = PLAYER_HAND_SIZE;
    int player2HandSize = PLAYER_HAND_SIZE;
    int player1Wins = 0;
    int player2Wins = 0;
    char choose;
    char play;
	
    initializeDeck(deck);
    shuffleDeck(deck, DECK_SIZE);
    dealCards(deck, player1, deckIndex);
    dealCards(deck, player2, deckIndex);

    top_card[0] = deck[deckIndex][0];
    top_card[1] = deck[deckIndex][1];
    ++deckIndex%10;
    HomePageMenu();

    cin>>choose;	
    while(choose!= 'P' && choose!='S' && choose!='E')
    {
    	cout<<"ERROR #404 -------"<<endl;
    	cout<<"Enter Again : ";
    	cin>>choose;
	}
	
    switch(choose){
   case 'P':
   	case 'p':
	{
	
	 system("cls"); 
 
   while(true)
   {
	  //cout<<"Player 1 Turn:"<<endl;
     playTurn(player1, player1HandSize, deckIndex, deck, player1, player1HandSize, player2, player2HandSize);
     
     if(player1HandSize == 0) 
	 {
        cout<<"Player 1 wins!\n";
        player1Wins++; // Increment Player 1's win counter
        break;
     }
     
	 system("cls"); 
 
     setColor(WHITE, BLACK);
     cout<<"Player 2 Turn: "<<endl<<endl;
       
     playTurn(player2, player2HandSize, deckIndex, deck, player1, player1HandSize, player2, player2HandSize);
     if(player2HandSize == 0) 
	{
        cout<<"Player 2 wins!\n";
        player2Wins++; // Increment Player 2's win counter
        break;
    }
    system("cls");
  
   }

// After the game loop, save the results
saveLoadGameResult(player1Wins, player2Wins);
break;
}
case 'S':
case 's':
	{
		//After the game loop, save the results
   cout<<endl;
   saveLoadGameResult(player1Wins, player2Wins);
	}
case 'E':
	case 'e':
	{
		char exit;
		cout<<"Do you want to exit the game(E) or anything: ";
		cin>> exit;
		if(exit=='E' || exit=='e')
		{
		
		cout<<"Are you sure. You want to Exit(Y,N) or E: ";
		cin>>play;
		while(play != 'n' && play !='N' && play != 'Y' && play != 'y' && play!='E')
		{
			cout<<"===INVALID ENTRY===="<<endl;
			cout<<"Are you sure. You want to Exit(Y,N)  : ";
		    cin>>play;
		}
		if(play=='Y')
		{
			return 0;
		}
		break;
	}
	}

}
}
}
// Initialization
void initializeDeck(int deck[][15])  
{
	// This will increment itself in such way that it stores all 108 cards of a deck
    int index = 0; 
	//counter to keep track of card position
	
    for(int c=0;c<4;c++)  //loop for colours
	{
        for(int n=0;n <= 9;n++) //loop for numbers 0 to 9
		{
        	
            deck[index][0] = c; 
            deck[index][1] = n;   
            ++index;
            
            if(n!= 0) // used for duplication of cards
			{
                deck[index][0] = c;
                deck[index][1] = n;
                ++index;
            }
        }
        
        for(int special = 10;special <= 12;special++)  // loop for the special characters 10-12
		{
            deck[index][0] = c;          // for the colours
            deck[index][1] = special;          // i.e skip, draw and reverse
            ++index;
            deck[index][0] = c;
            deck[index][1] = special;
            ++index;
        }
    }
    
    for(int i=0;i < 4;i++) 
	{
        deck[index][0] = -1;  // this is used for all these special charcters storaging 
        deck[index][1] = 13;  // Wild Card
        ++index;
        deck[index][0] = -1; 
        deck[index][1] = 14;  // Wild Draw Four Card
        ++index;
        
    }
}

// Shuffling
void shuffleDeck(int deck[][15], int deck_size) 
{
	srand(time(0));
	
    for(int i=0;i<deck_size*2;i++) //  number of swaps
    {
        // Generate two random numbers from the deck
        int randomIndex1 = rand() % deck_size;
        int randomIndex2 = rand() % deck_size;

        // Swap the two randomly chosen cards
        int tempColor = deck[randomIndex1][0];
        int tempType = deck[randomIndex1][1];
        
        deck[randomIndex1][0] = deck[randomIndex2][0];
        deck[randomIndex1][1] = deck[randomIndex2][1];
        
        deck[randomIndex2][0] = tempColor;
        deck[randomIndex2][1] = tempType;
    }
}

// Deal Cards
void dealCards(int deck[][15], int player[][15], int& deckIndex)  // pass by refrence is used to modify the orignal function or array
{
    for(int i = 0; i < PLAYER_HAND_SIZE; ++i)   // player_hand_size is for each person reciving a fixed number of card i.e 7
	{
        player[i][0] = deck[deckIndex][0];
        player[i][1] = deck[deckIndex][1];
        ++deckIndex;  // to move to the next card of the deck
    }
}

// Player Hands
void PrintPlayerHand(int player[][15], int handSize) 
{
    for(int i = 0; i < handSize;i++) 
	{
        int colour = player[i][0];
        int type = player[i][1];

        // Set background color for card
        switch(colour) 
		{
            case 0: setColor(BLACK, RED); break;
            case 1: setColor(BLACK, BLUE); break;
            case 2: setColor(BLACK, GREEN); break;
            case 3: setColor(BLACK, YELLOW); break;
            default: setColor(BLACK, WHITE); break;  
        }
        cout<<setw(2)<<" -------  "; //display first line of card
       
    }
    cout<<endl;

    for(int i = 0; i < handSize;i++) 
	{
        int colour = player[i][0];     //display 2nd line of card
        switch(colour) 
		{
            case 0: setColor(BLACK, RED); break;
            case 1: setColor(BLACK, BLUE); break;
            case 2: setColor(BLACK, GREEN); break;
            case 3: setColor(BLACK, YELLOW); break;
            default: setColor(BLACK, WHITE); break;  // Wild
        }
        cout <<setw(2)<<" -     -  ";

      
    }
    cout<<endl;

    for(int i = 0; i < handSize;i++) 
	{
        int colour = player[i][0];
        int type = player[i][1];

        switch(colour) 
		{
            case 0: setColor(BLACK, RED); break;   //display 3nd line of card
            case 1: setColor(BLACK, BLUE); break;
            case 2: setColor(BLACK, GREEN); break;
            case 3: setColor(BLACK, YELLOW); break;
            default: setColor(BLACK, WHITE); break;   // Wild
        }
        cout<<setw(2)<<" -";
        if(type <= 9)   //display card number/type inside card
		cout<<"  "<<type<<"  "; 
        else if(type == 10) 
		cout<<"  S  ";
        else if(type == 11) 
		cout<<"  R  ";
        else if(type == 12) 
		cout<<" +2  ";
        else if(type == 13) 
		cout<<"  W  ";
        else if(type == 14) 
		cout<<" +4  ";
        cout<<setw(2)<< "-  ";
     
    }
    cout<<endl;

    for(int i = 0; i < handSize;i++) 
	{
        int colour = player[i][0];
        switch(colour) 
		{
            case 0: setColor(BLACK, RED); break;
            case 1: setColor(BLACK, BLUE); break;
            case 2: setColor(BLACK, GREEN); break;
            case 3: setColor(BLACK, YELLOW); break;
            default: setColor(BLACK, WHITE); break;  // display 2nd lsst line of card
        }
        cout <<setw(2)<<" -     -  ";
       
    }
    cout<<endl;

    for(int i = 0; i <handSize;i++) 
	{
        int colour = player[i][0];
        switch(colour) 
		{
            case 0: setColor(BLACK, RED); break;
            case 1: setColor(BLACK, BLUE); break;
            case 2: setColor(BLACK, GREEN); break;
            case 3: setColor(BLACK, YELLOW); break;
            default: setColor(BLACK, WHITE); break;  // display last line of card
        }
        cout<<setw(2)<<" -------  ";
        setColor(WHITE,BLACK);
      
    }
    cout<<endl;
    setColor(WHITE,BLACK);
}

// Top Card
void displayTopCard()
{
	int colour=top_card[0]; // stores colour of topcolour from main
	int number=top_card[1]; //stores number of topcolour from main
	cout<<"\nTop Card:\n";
	
	switch(colour)
	{
		case 0:
			colour=RED;
			break;
		case 1:
			colour=BLUE;
			break;
		case 2:
			colour=GREEN;
			break;
		case 3:
			colour=YELLOW;
			break;
	}
	
	setColor(BLACK, colour);
	cout<<" -------"<<endl; //display top card
	cout<<" -     -" <<endl;
	if(number>9)
	{
		cout<<" - "<<number<<"  -"<<endl;
	}
	else
	{
		cout<<" -  "<<number<<"  -"<<endl;
	}
	cout<<" -     -" <<endl;
	cout<<" -------"  <<endl;
	setColor(WHITE, BLACK);
	cout<<endl;
}

// Draw Cards
void drawCard(int deck[][15], int& deckIndex, int player[][15], int& handSize) 
{
    if(deckIndex < DECK_SIZE) 
	{
        player[handSize][0] = deck[deckIndex][0];
        player[handSize][1] = deck[deckIndex][1];
        ++handSize;
        ++deckIndex;
    }
	else 
	{
        cout<<"No cards to draw"<<endl;
    }
}

//  Valid Play
bool isValidPlay(int playerColor, int playerType) 
{
    // Take only two variables and access the top card globally
   return (playerColor == top_card[0] || playerType == top_card[1] ||  playerType == 13 || playerType == 14);
}

// Discard Pile
void UpdateDiscardPile(int playedCardColor, int playedCardType) 
{
    // Update the global top_card variable to the played card
    top_card[0] = playedCardColor; // Update color
    top_card[1] = playedCardType;  // Update type

    cout<<"New Top Card: ";
    displayTopCard(); // Optional: Display the updated top card
}

// Updated Board
void PrintUpdatedBoard(int player1[][15], int player1HandSize, int player2[][15], int player2HandSize) 
{
//    system("cls");  // Clear the screen for a fresh view of the board
    cout << "=========== UNO Game Board ============\n\n";
    setColor(WHITE, BLACK);
    cout<<endl;
}

// Special Cards
void handleSpecialCard(int specialCardType)
 {
    switch (specialCardType) 
	{
        case 10:  // Skip
            cout<<"Special Card Played: Skip!\n";
            skip = true;
            break;

        case 11:  // Reverse
            cout<<"Special Card Played: Reverse!\n";
            reverse = !reverse;
            break;

        case 12:  // Draw Two
            cout<<"Special Card Played: Draw Two!\n";
            drawCardCount += 2;
            break;

        case 13:  // Wild
            cout<<"Special Card Played: Wild Card! Choose a color (0-Red, 1-Blue, 2-Green, 3-Yellow): ";
            cin>>top_card[0];
            
            while (top_card[0] < 0 || top_card[0] > 3) 
			{
                cout<<"Invalid color. Choose again (0-Red, 1-Blue, 2-Green, 3-Yellow): ";
                cin>>top_card[0];
            }
            break;

        case 14:  // Wild Draw Four
            cout<<"Special Card Played: Wild Draw Four! Choose a color (0-Red, 1-Blue, 2-Green, 3-Yellow): ";
            cin>>top_card[0];
            
            while(top_card[0] < 0 || top_card[0] > 3) 
			{
                cout<<"Invalid color. Choose again (0-Red, 1-Blue, 2-Green, 3-Yellow): ";
                cin>>top_card[0];
            }
            drawCardCount += 4;
            break;
    }
}

// Play Turn
void playTurn(int player[][15], int& handSize, int& deckIndex, int deck[][15], int player1[][15], int player1HandSize, int player2[][15], int player2HandSize) 
{
    
	int move;

    // Print the updated board at the start of the turn
    PrintUpdatedBoard(player1, player1HandSize, player2, player2HandSize);
   cout<<"=======================================\n\n";
    cout<<"Your Turn:\n";
    displayTopCard();   // Display the current top card
    setColor(WHITE, BLACK);
    cout<<"=======================================\n\n";
    cout<<"Your Cards:\n";
    PrintPlayerHand(player, handSize);
    setColor(WHITE, BLACK);

    if(skip) 
	{
        cout<<"Your turn is skipped."<<endl;
        skip = false;  // Reset skip flag
        return;
    }

    if(reverse) 
	{
        cout << "Your turn is reversed."<<endl;
        reverse = false;  // Reset reverse flag
        return;
    }

    if(drawCardCount>0) 
   {
        cout<<"The next player has to draw "<<drawCardCount<< " cards......"<<endl;
        if(drawCardCount == 4) 
		{
            cout<<"Player draws 4 cards!"<<endl;
            for(int i =0;i<4;i++) 
			{
                drawCard(deck, deckIndex, player, handSize);  //Draw 4 cards
            }
            PrintPlayerHand(player, handSize);
        }
        else if(drawCardCount == 2)
		{
            cout<<"Player draws 2 cards!"<<endl;
            for(int i=0;i<2;i++) 
			{
                drawCard(deck, deckIndex, player, handSize);  // Draw 2 cards
            }
            PrintPlayerHand(player, handSize);
        }
        
        drawCardCount=0;  //Initialize count to 0 again for next turn
    }
    
    if(!callUno(handSize))
    {
    	for(int i=0;i<2;i++) 
			{
                drawCard(deck, deckIndex, player, handSize);  // Draw 2 cards
            }
        PrintPlayerHand(player, handSize);
        return;
	}
    cout<<"Enter card index to play (1-"<<handSize<<") or 0 to draw: ";
    cin>>move;

    while(move < 0 || move > handSize) 
	{
        cout<<"Invalid Move.\n";
        cout<<"Enter card index to play (1-"<<handSize<<") or 0 to draw: ";
        cin>>move;
    }

    if(move == 0) 
	{
        drawCard(deck, deckIndex, player, handSize);
        return;
    }

    --move;  // Adjust move to index
    int playerColor = player[move][0];
    int playerType = player[move][1];

    if(!isValidPlay(playerColor, playerType)) 
	{
        cout<<"Invalid move! Draw a card.\n";
        drawCard(deck, deckIndex, player, handSize);
        return;
    }

    // Update discard pile
    UpdateDiscardPile(playerColor, playerType);

    // Remove played card
    for(int i = move;i < handSize - 1;i++) 
	{
        player[i][0] = player[i + 1][0];
        player[i][1] = player[i + 1][1];
    }
    --handSize;

    // Process special cards
    handleSpecialCard(playerType);
}

// UNO CALL
bool callUno(int playerHandSize)
{
	if(playerHandSize==2)
	{
		char call;
		cout<<"two cards are left call UNO! by entering 'U': ";
		cin>>call;
		if(call!='u'&&call!='U')
		{
			cout<<"UNO was not called, You have penalty of drawing two cards from deck "<<endl;
			return false;
		}
		return true;
	}
	return true;
}

// Save Game
void saveLoadGameResult(int player1Wins, int player2Wins) 
{
    ofstream file("game_history.txt", ios::trunc); // Open file in append mode
    if(!file) 
	{
        // Check if file opening failed
        cout << "Error opening file for saving game results.\n";
        return;
    }

    // Save the game results (win counts)
    file << "Player 1 Wins: "<<player1Wins << endl;
    file << "Player 2 Wins: "<<player2Wins << endl;

    file.close();
    cout << "Game has been saved successfully.\n";
}
