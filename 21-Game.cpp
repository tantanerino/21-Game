#include <iostream> //Input output stream
#include <vector> //Vector library
#include <algorithm> //Algorithm library used for card shuffling
#include <ctime> //Library used for randomization based off current time to seed a random number generator
#include <random> //Seeding random engine to randomize order of deck and shuffle the deck function

using namespace std;//Removes need to write std:: before anything used in the standard library

//Card struct to create a user-initialized data type as a card that holds attributes being the cards rank and suit
struct Card{
    string rank; //Card rank (1, 2, 3 ...)
    string suit; //Card suit (hearts, spades, ...)

    Card(string r, string s) : rank(r), suit(s) {} //Card constructor used when accessing cards directly in the createDeck function
};

//Create deck function instantiates the deck (vector) usign the suts and ranks constant arrays (since the ranks and suits are constant)
vector<Card> createDeck(){
    vector<Card> deck;//Initialize deck vector array empty, then will be added to through the for-loop
    //Initialize suits and ranks arrays
    const string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    //Nested for-loop loops per suit, the size of the rank array (13) and would push back an element in the deck array with a card data type for the cards rank and its corresponding suit
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck.push_back(Card(ranks[j], suits[i]));
        }
    }

    return deck;
}

//Will shuffle the deck of cards randomizing the order from start to end
void shuffleCards(vector<Card>& deck){
    unsigned seed = time(0); //Generate a unique seed based off the time
    default_random_engine rng(seed); //Random number generator using that seed
    shuffle(deck.begin(), deck.end(), rng); //Shuffle the deck vector (randomly swap elements) from the beginning to end of the deck using the seeded RNG
}

//Checks if the deck is running low and will reshuffle the deck passed through by reference
void checkDeckSize(vector<Card>& deck){
    //If the deck size is below 10, recreate the deck and shuffle the cars
    if (deck.size() < 10) {
        deck = createDeck();
        shuffleCards(deck);
        cout << "----Deck size low. Reshuffling new deck.----" << endl;
    }
}

//Each time card values are printed, this function gets called. Takes in a constant card parameter by reference (since the specific card being passed through doesn't change), and a boolean set to false, but if the dealer calls this function on its first turn will output the hidden card
void ASCII(const Card& card, bool hidden = false){
    if(hidden){//If hidden = true, output the back of the card
        cout << "__________" << endl;
        cout << "|########|" << endl;
        cout << "|########|" << endl;
        cout << "|########|" << endl;
        cout << "|########|" << endl;
        cout << "|########|" << endl;
        cout << "[________]" << endl;
        return;//Exits the function early
    }
    
    //Create a displayRank and suit string, and equal them to the initial of the cards rank, and its suit
    string displayRank, displaySuit;
    
    if(card.rank == "Ace"){
        displayRank = "A";
    }else if(card.rank == "Jack"){
        displayRank = "K";
    }else if(card.rank == "Queen"){
        displayRank = "Q";
    }else if(card.rank == "King"){
        displayRank = "K";
    }else{
        displayRank = card.rank;
    }
    
    //Change the suit name to the corresponding symbol
    if(card.suit == "Spades"){
        displaySuit = "♠";
    }else if(card.suit == "Clubs"){
        displaySuit = "♣";
    }else if(card.suit == "Hearts"){
        displaySuit = "♥";
    }else if(card.suit == "Diamonds"){
        displaySuit = "♦";
    }
    
    //Display ASCII of the card with a design and the displayRank/Suit
    cout << "__________" << endl;
    cout << "| " << displayRank << "      |" << endl;
    cout << "|        |" << endl;
    cout << "|    " << displaySuit << "   |" << endl;
    cout << "|        |" << endl;
    cout << "|      " << displayRank << " |" << endl;
    cout << "[________]" << endl;
}

//Dealer class
class Dealer{
private:
    //Private members of the dealers hand, and how much their hand totals to
    vector<Card> hand;
    int handTotal = 0;
public:
    //Passes through the deck by reference to directly access the elements(card) in the deck, and not creating a copy.
    void addCard(vector<Card>& deck){
        hand.push_back(deck.back()); //Push back the hand (add one card to the hand) from the back (top) of the deck
        deck.pop_back(); //Removes the last (element) card from the deck because it was added to the hand
    }
    
    //Calculates the hand total
    int calculateHT(){
        handTotal = 0; //Set the hand total to 0
        int aceNum = 0; //Counts how many aces in hand for ace logic (1 or 11)
        //Loops through the hand, determinining what card is in the hand and adding the corresponding value
        for(int i = 0; i < hand.size(); i++){
            //If statements to check what rank and add the corresponding value
            if(hand[i].rank == "Ace"){
                handTotal += 11;
                aceNum++;
            }else if (hand[i].rank == "Jack" || hand[i].rank == "Queen" || hand[i].rank == "King"){
                handTotal += 10;
            }else{
                handTotal += stoi(hand[i].rank); //Converts the rank string into an integer if the rank is just a number and not A,J,Q,K
            }
        }
        
        //While-loop checks if the hand total is greater than 21 (meaning it will bust) and there is an available ace thats set to equal 11, will subtract 10 from the handTotal to make the ace = 1
        while(handTotal > 21 && aceNum > 0){
            handTotal -= 10;
            aceNum--;
        }
        return handTotal;
    }
    
    //Returns the value of the hidden card once its the dealers 2nd turn
    Card getHiddenCard(){
        return hand[1];  //Second card is face down
    }
    
    //Non-return type function that prints the dealers hand. Initializes a parameter revealAll, and on first run will be passed through as false to trigger the if-statement to only print the first card in hand, and the other card facedown
    void printHand(bool revealAll = true){
        //If statement checks if the dealer will reveal their card or not
        if(!revealAll){
            cout << "------------------" << endl;
            //Show only the first card
            ASCII(hand[0]);
            ASCII(hand[0], true);
        }else{
            cout << "------------------" << endl;
            //Prints out hand
            for (int i = 0; i < hand.size(); i++) {
                ASCII(hand[i]);
            }
            
            //Finds dealers hand total
            cout << "---" << endl;
            cout << "Dealer total: " << calculateHT() << endl;
            cout << "---" << endl;
        }
    }
    
    //Passes through deck by reference, and will automatically add a card to the hand until it's value totals over 17
    void play(vector<Card>& deck){
        while (calculateHT() <= 16) {
            checkDeckSize(deck);
            addCard(deck);
        }
    }
   
    //Returns a true/false if the hand is totaled over 21, therefore the dealer will bust
    bool checkBust(){
        return calculateHT() > 21;
    }
    
    //Returns the dealers hand total
    int getTotal(){
        return handTotal;
    }
    
    void resetHand(){
        hand.clear();
        handTotal = 0;
    }
};

//Player class to hold members that control a players actions
class Player{
private:
    //Private attributes like the individual players name, hand, and whether they've stayed or busted
    string name;
    vector<Card> hand;
    int handTotal = 0;
    int wins = 0;
    int losses = 0;
    int ties = 0;
public:
    Player(string playerName) : name(playerName) {} //Player constructor that takes the name parameter
    
    //Passes through the deck by reference to directly access the elements(card) in the deck, and not creating a copy
    void addCard(vector<Card>& deck){
        hand.push_back(deck.back());
        deck.pop_back();
    }
    
    //Calculates the players hand total
    int calculateHT(vector<Card>& hand){
        handTotal = 0;
        int aceNum = 0;
        
        //Loops through the hand, determinining what card is in the hand and adding the corresponding value
        for(int i = 0; i < hand.size(); i++){
            string rank = hand[i].rank;
            //if statements to check what rnak and add the corresponding value
            if(hand[i].rank == "Ace"){
                handTotal += 11;
                aceNum++;
            }else if (hand[i].rank == "Jack" || hand[i].rank == "Queen" || hand[i].rank == "King"){
                handTotal += 10;
            }else{
                handTotal += stoi(rank);
            }
        }
        
        //While-loop iterates through each ace in the hand, and decreses the value by 10 while the hand total is greater than 21
        while(handTotal > 21 && aceNum > 0){
            handTotal -= 10;
            aceNum--;
        }
        return handTotal;
    }
    
    //Iterates through the hand and prints the rank and suit
    void printHand(vector<Card>& hand){
        for (int i = 0; i < hand.size(); i++) {
            ASCII(hand[i]);
        }
    }
   
    //Checks if player handtotal is greater than 21, if so return a true such that the player has busted
    bool checkBust(){
        return calculateHT(hand) > 21;
    }
    
    //Get function returns users name
    string getName(){
        return name;
    }
    
    //Get function that returns the players hand
    vector<Card>& getHand(){
        return hand;
    }
    
    //Resets the players hand
    void resetHand(){
        hand.clear();
        handTotal = 0;
    }
    
    //Adds score status to the players private members for the ending scoreboard
    void addWin(){
        wins++;
    }
    
    void addLoss(){
        losses++;
    }
    
    void addTie(){
        ties++;
    }
    
    //Prints out the final scores of each player
    void printScores(){
        cout << "--------------------" << endl;
        cout << name << " Stats: " << endl;
        cout << "Wins: " << wins << endl;
        cout << "Losses: " << losses << endl;
        cout << "Ties: " << ties << endl;
    }
};

//AI class if the user chooses to include an AI player
class AI{
private:
    //Private members of the dealers hand, and how much their hand totals to
    vector<Card> hand;
    int handTotal = 0;
    int wins = 0;
    int losses = 0;
    int ties = 0;
    int randomHit = 16 + (rand() % 3);
public:
    
    //Passes through the deck by reference to directly access the elements(card) in the deck, and not creating a copy.
    void addCard(vector<Card>& deck){
        hand.push_back(deck.back()); //Push back the hand (add one card to the hand) from the back (top) of the deck
        deck.pop_back(); //Removes the last (element) card from the deck because it was added to the hand
    }
    
    //Calculates the hand total
    int calculateHT(){
        handTotal = 0; //Set the hand total to 0
        int aceNum = 0; //Counts how many aces in hand for ace logic (1 or 11)
        
        //Loops through the hand, determinining what card is in the hand and adding the corresponding value
        for(int i = 0; i < hand.size(); i++){
            //If statements to check what rank and add the corresponding value
            if(hand[i].rank == "Ace"){
                handTotal += 11;
                aceNum++;
            }else if(hand[i].rank == "Jack" || hand[i].rank == "Queen" || hand[i].rank == "King"){
                handTotal += 10;
            }else{
                handTotal += stoi(hand[i].rank); //Converts the rank string into an integer if the rank is just a number and not A,J,Q,K
            }
        }
        
        //While-loop checks if the hand total is greater than 21 (meaning it will bust) and there is an available ace thats set to equal 11, will subtract 10 from the handTotal to make the ace = 1
        while(handTotal > 21 && aceNum > 0){
            handTotal -= 10;
            aceNum--;
        }
        
        return handTotal;
    }
    
    //Get function that returns the ai's hand
    vector<Card>& getHand(){
        return hand;
    }
    
    //Iterates through the hand and prints the rank and suit
    void printHand(){
        for(int i = 0; i < hand.size(); i++){
            ASCII(hand[i]);
        }
    }
    
    //Passes through deck by reference, and will automatically add a card to the hand until it's value totals over 17
    void play(vector<Card>& deck){
        while(calculateHT() <= 16){
            checkDeckSize(deck);
            addCard(deck);
        }
    }
    
    //Returns a true/false if the hand is totaled over 21, therefore the dealer will bust
    bool checkBust(){
        return calculateHT() > 21;
    }
    
    //Returns the dealers hand total
    int getTotal(){
        return handTotal;
    }
    
    void resetHand(){
        hand.clear();
        handTotal = 0;
    }
    
    //Adds score status to the ais private members for the ending scoreboard
    void addWin(){
        wins++;
    }
    
    void addLoss(){
        losses++;
    }
    
    void addTie(){
        ties++;
    }
    
    //Prints out the final scores of each AI
    void printScores(){
        cout << "Wins: " << wins << endl;
        cout << "Losses: " << losses << endl;
        cout << "Ties: " << ties << endl;
    }
};

int main(){
    vector<Player> players; //Initializes a vector player object for each player
    int playerNum; //Integer playrnum to take in user inputted how many players will be playing
    cout << R"(
     _____  __    _____                      
    / __  \/  |  |  __ \                     
    `' / /'`| |  | |  \/ __ _ _ __ ___   ___ 
      / /   | |  | | __ / _` | '_ ` _ \ / _ \
    ./ /____| |_ | |_\ \ (_| | | | | | |  __/
    \_____/\___/  \____/\__,_|_| |_| |_|\___|
    )" << endl;
    cout << "Enter number of players: ";
    cin >> playerNum;
    
    //Iterates through each player and takes in their name
    for(int i = 0; i < playerNum; i++){
        string pName;
        cout << "Enter name of Player " << i + 1 << ": ";
        cin >> pName;
        players.push_back(Player(pName));
    }
    
    int aiNum; //Int aiNum for however many ais are to be added
    cout << "Would you like to add AI players? If so, how many? (Type 0 if no AIs are wanted): ";
    cin >> aiNum;
    vector<AI> bot(aiNum); //Initializes the array with the # element value of aiNum integer
    
    char choice;
    do{
        //Resets the deck for each iteration of a new round
        vector<Card> deck = createDeck(); //Initializes a deck vector, that stores the created Deck
        shuffleCards(deck); //Calls the shuffleCards function, passes through the deck to shuffle
        
        //Resets the players hand each iteration of a new round
        for(int i = 0; i < playerNum; i++){
            players[i].resetHand();
        }
        
        //Iterates through each player and adds 2 initial cards to their hand
        for(int i = 0; i < playerNum; i++){
            checkDeckSize(deck);
            players[i].addCard(deck);
            checkDeckSize(deck);
            players[i].addCard(deck);
            
        }
        //Resets the AI's hand each iteration of a new round
        for(int i = 0; i < aiNum; i++){
            bot[i].resetHand();
        }
        
        //Iterates through each ai and adds 2 initial cards to their hand
        for(int i = 0; i < aiNum; i++){
            checkDeckSize(deck);
            bot[i].addCard(deck);
            checkDeckSize(deck);
            bot[i].addCard(deck);
        }
        
        //Create a dealer object, add two cards, and then tell the dealer to play
        Dealer dealer;
        //Makes sure the dealers hand is cleared from the previous round
        dealer.resetHand();
        checkDeckSize(deck);
        dealer.addCard(deck);
        checkDeckSize(deck);
        dealer.addCard(deck);
        dealer.play(deck);
        
        
        //First turn, print the dealers hand but pass through a false bool to trigger the if-statement such that it outputs one card face-up and another face-down
        cout << "--------------------" << endl;
        cout << "Dealers hand: " << endl;
        dealer.printHand(false);
        cout << endl;
        
        //Turn-based system that iterates through all players until they all stay/bust
        for(int i = 0; i < playerNum; i++){
            Player &p = players[i]; //Initialize player object for each iteration, for a new player to access the player class
            
            while(true){//Always be true (loop infinetly), will break once the player busts/stays
                if(p.checkBust()){//Check if the player has busted (their hand total is > 21
                    cout << p.getName() << " Busted" << endl; //Output player Name and that they've busted
                    break; //Break out of while-loop to end this players turn
                    
                }
                
                //Print players hand
                cout << "--------------------" << endl;
                cout << p.getName() << "'s Hand:" << endl;
                p.printHand(p.getHand());
                cout << "Total: " << p.calculateHT(p.getHand()) << endl;
                
                //Prompt user to hit/stay
                cout << "Hit or Stay? (h/s): ";
                char choice;
                cin >> choice;
                
                //Makes sure the user input is valid
                while (choice != 'h' && choice != 's' && choice != 'H' && choice != 'S') {
                    cout << "Invalid choice, please enter h or s" << endl;
                    cin >> choice;
                }
                
                //Checks whether player has hit, or stayed
                if(choice == 'h'){
                    checkDeckSize(deck);
                    p.addCard(deck); //Add a card to the players hand
                    cout << "--------------------" << endl;
                    cout << p.getName() << "'s hand:" << endl;
                    p.printHand(p.getHand()); //Print the players hand
                    cout << "New total: " << p.calculateHT(p.getHand()) << endl;
                }else{
                    //Player choses to stay, show final total and end their turn
                    cout << p.getName() << " stays with their total: " << p.calculateHT(p.getHand()) << endl;
                    break;
                }
            }
        }
        
        //Iterates through each AI and prints their hand and total
        for(int i = 0; i < aiNum; i++){
            cout << "--------------------" << endl;
            cout << "AI " << i + 1 << "'s Hand:" << endl;
            bot[i].play(deck);
            bot[i].printHand();
            cout << "AI " << i + 1 << " Total: " << bot[i].calculateHT() << endl;
        }
        
        //Dealer reveals their full hand, and will keep playing until HT > 17
        cout << endl;
        cout << "Dealer reveals face-down card:" << endl;
        dealer.printHand(true);
        
        //Iterates through each player, compares their total to the dealers and considers win-conditions
        for(int i = 0; i < playerNum; i++){
            Player& p = players[i];
            
            //Get dealer and player hand totals
            int dealerTotal = dealer.getTotal();
            int playerTotal = p.calculateHT(p.getHand());
            
            if(p.checkBust()){ //Checks if the player busted
                cout << p.getName() << " busted. Dealer wins." << endl;
                p.addLoss();
            }else if(dealer.checkBust()){ //Checks if the dealer busted
                cout << "The dealer has busted. " <<  p.getName() << " won!" << endl;
                p.addWin();
            }else if(dealerTotal > playerTotal){ //Dealer total is greater than a players total
                cout << p.getName() << "'s hand is less than the dealers. " << "Dealer wins." << endl;
                p.addLoss();
            }else if(dealerTotal < playerTotal){ //Player total is greater than the dealer total
                cout << p.getName() <<  " wins!" << endl;
                p.addWin();
            }else{//Player and dealer tie
                cout << "The dealer ties with " << p.getName() << ". " << endl;
                p.addTie();
            }
        }
        
        //Same conditionals as the player, but compares the ai to dealer instead
        for(int i = 0; i < aiNum; i++){
            AI& b = bot[i];
            
            //Get dealer and player hand totals
            int dealerTotal = dealer.getTotal();
            int aiTotal = b.calculateHT();
            
            if(b.checkBust()){ //Checks if the ai busted
                cout << "AI " << i + 1 << " busted. Dealer wins." << endl;
                b.addLoss();
            }else if(dealer.checkBust()){ //Checks if the dealer busted
                cout << "The dealer has busted. " <<  "AI " << i + 1 << " won!" << endl;
                b.addWin();
            }else if(dealerTotal > aiTotal){ //Dealer total is greater than a ai total
                cout << "Dealer wins." << endl;
                b.addLoss();
            }else if(dealerTotal < aiTotal){ //ai total is greater than the dealer total
                cout << "AI " << i + 1 <<  " wins!" << endl;
                b.addWin();
            }else{//ai and dealer tie
                cout << "The dealer ties with " << "AI " << i + 1 << ". " << endl;
                b.addTie();
            }
            
        }
        
        //Prompt user if they'd like to play another game
        cout << "Would you like to keep playing (y)? Otherwise, enter any key to exit: ";
        cin >> choice;
    }while(choice == 'y' || choice == 'Y');
    
    //Use player and ai vectors and iterate through each players/ai final scores
    for(int i = 0; i < playerNum; i++){
        players[i].printScores();
    }
    
    for(int i = 0; i < aiNum; i++){
        cout << "--------------------" << endl;
        cout << "AI " << i + 1 << " Stats:" << endl;
        bot[i].printScores();
    }
    
    return 0;
}
