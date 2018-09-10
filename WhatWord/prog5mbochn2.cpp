// Program #4, WhatWord.
//
//	Class: CS 141, Fall 2016.
//	System : Windows 10, Microsoft Visual Studio 2015
//	Lab : Tuesday 8am lab,
//	TA : Guixiang Ma
//	Authors : Michal Bochnak
//
// Write a program in C / C++ to create a 4x4 Boggle board. To play a similar game online see wordtwist.org.
//
//
// Running your program should look like the following:
//
// Author: Dale Reed
//	Program : #5, Boggle
//	TA : Claude Shannon, Th 4 - 5
//	Nov 5, 2016
//
//	Welcome to the game of Boggle, where you play against the clock
//	to see how many words you can find using adjacent letters on the
//	board.Each letter can be used only once for a given word.
//
//	When prompted to provide input you may also :
//		Enter 'r' to reset the board to user - defined values.
//			Enter 's' to solve the board and display all possible words.
//			Enter 't' to toggle the timer off / on.
//			Enter 'x' to exit the program.
//
//			263410 words were read in from the dictionary.
//
//
//			60 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 0
//			1. Enter a word : plain
//			Worth 4 points.
//			Words so far are : plain
//
//			55 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 4
//			2. Enter a word : can
//			Worth 1 points.
//			Words so far are : can plain
//
//			52 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 5
//			3. Enter a word : map
//			Worth 1 points.
//			Words so far are : can map plain
//
//			50 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 6
//			4. Enter a word : can
//			Sorry, that word was already previously found.
//			Words so far are : can map plain
//
//			50 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 6
//			4. Enter a word : acrl
//			was not found in the dictionary.
//
//			45 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 6
//			4. Enter a word : rail
//			cannot be formed on this board.
//
//			11 seconds remaining
//			a c r l
//			n e a p
//			p u i m
//			s a a n
//			Score : 6
//			4. Enter a word : s
//			Enter min and max word lengths to display : 3 16
//			Words between 3 and 16 are :


#include <stdio.h>		
#include <stdlib.h>		// standard library
#include <time.h>		// or time functions
#include <iostream>		// for standard input / output
#include <fstream>		// for inStream 
#include <cassert>		// For the assert statement

using namespace std;


// define constants for word and dictionary sizes
#define MAX_WORD_SIZE 17
#define NUMBER_OF_WORDS_IN_DICTIONARY 263534

const char dictionaryFileName[] = "dictionary.txt";		// File name for dictionary words


// function declaration
void DisplayIdentifyingInformation();
void DisplayGameInstructions();
void DisplayABoard(char board[]);
void FillOutBoardWithRandomLetters(char board[], double list[]);
void AllocateArray(char ** & matrix);
int BinarySearch(const char searchWord[], char **&dictionary, int numberOfDictionaryWords);
bool CheckForAdjacentSpots(char word[], char tempBoard[], int index, int foundAtSpot);
bool CheckIfWordIsOnTheBoard(char word[], char board[], int index, int foundAtSpot);
void VerifyIfWordIsValid(char word[], char **&dictionary, int &score, bool wordsFound[NUMBER_OF_WORDS_IN_DICTIONARY], char board[]);
void ReadInTheWordsFromDictionary(char **&dictionary);
void InitializeTheArrayToFalse(bool wordsFound[]);
void DisplayGuessedWords(char **&dictionary, bool wordsFound[]);
void SetABoardManually(char board[]);
bool Timer(int active, int &startTime);
void DisplayTimer(int timeLeft);
void DisplaySolutions(char **&dictionary, char board[]);
int NumberOfPointsScored(char word[]);


int main() {

	// array board - initially stores asterisks and periods, it will be used
	// for filling the board with random letters, letters will be inserted in place 
	// of periods
	char board[37] = "*******....**....**....**....*******";
	// array list - stores frequency of letters
	double list[26] = {	0.07680,  //  a
						0.09485,  //  b
						0.13527,  //  c
						0.16824,  //  d
						0.28129,  //  e
						0.29299,  //  f
						0.32033,  //  g
						0.34499,  //  h
						0.43625,  //  i
						0.43783,  //  j
						0.44627,  //  k
						0.49865,  //  l
						0.52743,  //  m
						0.59567,  //  n
						0.66222,  //  o
						0.69246,  //  p
						0.69246,  //  q
						0.76380,  //  r
						0.86042,  //  s
						0.92666,  //  t
						0.95963,  //  u
						0.96892,  //  v
						0.97616,  //  w
						0.97892,  //  x
						0.99510,  //  y
						1.00000 }; //  z

	FILE *pDictionaryFile = fopen(dictionaryFileName, "r");		// open dictionary file
	char ** dictionary;		// stores dictionary words
	int moveNumber = 0;		// counts move numbers
	int score = 0;			// total of collented points
	char word[17];			// word entered by user while playing the game
	int index = -1;			// index of word in dictionary
	bool wordsFound[NUMBER_OF_WORDS_IN_DICTIONARY];	// keeps track of indeces of found words
	char guessedWords[1000][17];	// stores words guessed by user	
	bool active = true;		// false - timer OFF, true - timer ON
	int startTime = time(NULL);	// starting point for timer
	bool timeIsUp = false;	// true - time is up, false - time is not up yet

	
	DisplayIdentifyingInformation();
	DisplayGameInstructions();
	// Initialize each element of bool array which keeps track of found words to false
	InitializeTheArrayToFalse(wordsFound);
	// Allocate memory for dictionary
	AllocateArray(dictionary);
	ReadInTheWordsFromDictionary(dictionary);
	// Fill out the board with 16 random letters
	FillOutBoardWithRandomLetters(board, list);

	
	// while loop allows to enter word for each move
	while (1) {
		// Verify if time is up or not
		timeIsUp = Timer(active, startTime);
		DisplayABoard(board);
		moveNumber++;		// increment moveNumber
		cout << "   Score: " << score << endl;
		// Prompt user to guess the word
		cout << moveNumber << ". " << "Enter a word: ";
		cin >> word;		// read the word entered by user
		
		// handle menu options
		if (strcmp(word, "r") == 0) {
			SetABoardManually(board);	// Set the board by user defined values
		}
		else if (strcmp(word, "s") == 0) {
			DisplaySolutions(dictionary, board);	// Display Possible solutions
		}
		// tooggles timer ON or OFF
		else if(strcmp(word, "t") == 0) {	
			if (active == true) {
				active = false;	// if timer is ON, turn it OFF
			}
			else {
				active = true;	// if timer is OFF, turn it ON
			}
		}
		else if (strcmp(word, "x") == 0) {
			exit(-1);			// ends the game
		}
		else {
			// Verifies if word is valid (check if it is in Dictionary and on a Board
			VerifyIfWordIsValid(word, dictionary, score, wordsFound, board);
		}
		if (timeIsUp) {		// Time is up, ends the game
			cout << "Time is up" << endl;
			exit(-1);
		}
	}

	return 0;
}	// end of main...



// Functions definitions

// ----------------------------------------------------------------------------
void DisplayIdentifyingInformation()
{
	cout << "Program #4, DoubleDown" << endl
		<< "Author: Michal Bochnak" << endl
		<< "TA: Guixiang Ma" << endl
		<< "Lab: Tuesday 8am" << endl
		<< "Nov 21, 2016" << endl << endl;
}


// ----------------------------------------------------------------------------
void DisplayGameInstructions() {
	cout << "Welcome to the game of Boggle, where you play against the clock" << endl
		<< "to see how many words you can find using adjacent letters on the" << endl
		<< "board. Each letter can be used only once for a given word." << endl
		<< endl
		<< "When prompted to provide input you may also :" << endl
		<< "Enter 'r' to reset the board to user - defined values." << endl
		<< "Enter 's' to solve the board and display all possible words." << endl
		<< "Enter 't' to toggle the timer off / on." << endl
		<< "Enter 'x' to exit the program." << endl << endl;
}


// ----------------------------------------------------------------------------
// Displays the board on the screen
// Input: char array with board
void DisplayABoard(char board[]) {

	// * * * * * *		0  1  2  3  4  5
	// * . . . . *		6  7  8  9  10 11
	// * . . . . *		12 13 14 15 16 17
	// * . . . . *		18 19 20 21 22 23
	// * . . . . *		24 25 26 27 28 29
	// * * * * * *		30 31 32 33 34 35

	cout << endl;		// New line for formatting

	for (int boardSpot = 7; boardSpot < 29; boardSpot++) {	// iterates thru array elements
		if ((boardSpot % 6) == 0) {			// after 6 elements are printed, go to next line
			cout << endl;			
		}
		// Do not display the asterisk
		if ((boardSpot != 11) && (boardSpot != 12) && (boardSpot != 17)
			&& (boardSpot != 18) && (boardSpot != 23) && (boardSpot != 24)) 
			cout << board[boardSpot] << " ";	// space between letters
	}
	cout << endl;			// new line for formatting output
}


// ----------------------------------------------------------------------------
// Fill out the board with random characters
// Input: char array with board
void FillOutBoardWithRandomLetters(char board[], double list[]) {
	double randomNumber = 0;	// holds the random number
	char drawnLetter = '!';		// hold the drawn letter
	srand(time(NULL));			// randomize seed
	
	// Iterate thru the board from spot 7 to spot 28 and insert random letters
	for (int boardSpot = 7; boardSpot <= 28; boardSpot++) {
		if (board[boardSpot] == '.') {						// insert a letter if there is a period
			randomNumber = ((rand() / (double)RAND_MAX));	// generate random number
			int letterNumber = 1;							// loop 
			// iterate thru 26 letters
			for (letterNumber = 0; letterNumber < 26; letterNumber++)  {				
				// break out of the loop if randomNUmber is less then current value of letterNumber
				if (randomNumber < list[letterNumber]) {
					break;		
				}
			}
			// drawnLetter = a(65) + letterNumber(index on the list)
			drawnLetter = 'a' + letterNumber;
			board[boardSpot] = drawnLetter;					// update spot on the board
		}		
	}
}


// ----------------------------------------------------------------------------
// Based on Professor's Reed function
// Allocate memory for dictionary words array.
// Note that the '&' is needed so that the new array 
// address is passed back as a reference parameter.
void AllocateArray(char ** & matrix)
{
	// Allocate space for large array of C-style strings
	matrix = new char*[NUMBER_OF_WORDS_IN_DICTIONARY];

	// For each array entry, allocate space for the string to be stored there
	for (int i = 0; i < NUMBER_OF_WORDS_IN_DICTIONARY; i++) {
		matrix[i] = new char[MAX_WORD_SIZE];

		// just to be safe, initialize C-string to all null characters 
		for (int j = 0; j < MAX_WORD_SIZE; j++) {
			matrix[i][j] = NULL;
		}//end for (int j=0...
	}//end for (int i...
}//end allocateArray()


 // BinarySearch function retrieved from previous program
 // Use binary search to look up the word in the dictionary array,
 // returning index if found, -1 otherwise
int BinarySearch(char searchWord[],		// word to be looked up
	char **&dictionary,					// the dictionary of words
	int numberOfDictionaryWords)		// number of words in the dictionary
{
	int low, mid, high;			// array indices for binary search
	int searchResult = -1;		// Stores index of word if search succeeded, else -1

	// Binary search for word
	low = 0;
	high = numberOfDictionaryWords - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		// searchResult negative value means word is to the left, positive value means
		// word is to the right, value of 0 means word was found
		searchResult = strcmp(searchWord, dictionary[mid]);
		if (searchResult == 0) {
			// Word IS in dictionary, so return the index where the word was found			
			return mid;
		}
		else if (searchResult < 0) {
			high = mid - 1;		// word should be located prior to mid location
		}
		else {
			low = mid + 1;		// word should be located after mid location
		}
	}
	return -1;					// Word was not found
} // end binarySearch()


// ----------------------------------------------------------------------------
// Check for the next letter occurance around the actual letter position
// to verify if word appears on the board
// Input: user word, board, index of the letter in word, spot where letter was found
bool CheckForAdjacentSpots(char word[], char tempBoard[], int index, int foundAtSpot) {
	int moves[] = { -7, -6, -5, -1, 1, 5, 6, 7 };	// poossible moves

	// Base case
	if (word[index] == '\0') {
		return true;
	}

	// Iterates thru the 8 possible moves around actual letter
	for (int moveNum = 0; moveNum < 8; moveNum++) {
		int nextSpot = foundAtSpot + moves[moveNum];	// try one of 8 moves
		if (tempBoard[nextSpot] == word[index]) {		// letters match
			// update foundAtSpot to the location of the next letter found
			foundAtSpot = nextSpot;						
			tempBoard[nextSpot] = '*';		// replace the spot by asterisks
			if (CheckForAdjacentSpots(word, tempBoard, ++index, foundAtSpot)) {
				return true;	// word found
			}
		}
	}
	return false;		// Word not found
}


// ----------------------------------------------------------------------------
// Verifies if word appears on the board by finding the first letter
// and later calls the recursive function to check for adjacent spots
// for the whole word
// Input: user word, board array, index of the letter of the word, spot where letter was found
bool CheckIfWordIsOnTheBoard(char word[], char board[], int index, int foundAtSpot) {
	bool winStatus = -1;		// win found = true, win not found = false
	char tempBoard[37];			// local copy of the board
	strcpy(tempBoard, board);	// update local copy with current board

	// Look up for the first letter occurance on the board
 	for (int spotNumber = 7; spotNumber < 29; spotNumber++) {
		if (word[index] == tempBoard[spotNumber]) {		// letter found
			foundAtSpot = spotNumber;	// assign location to foundAtSpot
			tempBoard[spotNumber] = '*';	// replace the letter by asterisks
			// check for adjacent spots for the whole word by calling
			// recursive function CheckForAdjacentSpots
			winStatus = CheckForAdjacentSpots(word, tempBoard, ++index, foundAtSpot);
			index = 0;	// reset the index to look up for another occurance of first letter
			// update local copy of board with actual board to retrieve
			// the letters in place of asterisks
			strcpy(tempBoard, board);	
			if (winStatus) {
				return true;		// word was found on the board
			}
		}
	}
	return false;		// word was not found on the board
}


// ----------------------------------------------------------------------------
// Validates the word first checking if it is in dictionary and then by checking 
// if it appears on the board by calling function CheckIfWordIsOnTheBoard
// Input: user word, whole dictionary, score, bool array of words found, board array
void VerifyIfWordIsValid(char word[], char **&dictionary, int &score,
				bool wordsFound[NUMBER_OF_WORDS_IN_DICTIONARY], char board[]) {

	int points = 0;					// points for actual word
	int dictionaryWordIndex = -1;	// index of word in dictionary
	
	// Display error message if the entered word was outside
	// of range ( 3 - 16 letters)
	if ((strlen(word) < 3) || (strlen(word) > 16)) {
		cout << "Words should consist of 3 to 16 letters." << endl <<endl;
		return;
	}

	// verify if the word is in dictionary and assign index
	dictionaryWordIndex = BinarySearch(word, dictionary, NUMBER_OF_WORDS_IN_DICTIONARY);
	if (dictionaryWordIndex != -1) {	// word was in 
		// Display error message if word was already used
		if (wordsFound[dictionaryWordIndex] == true) {
			cout << "Sorry, that word was already previously found." << endl;
			DisplayGuessedWords(dictionary, wordsFound);
			return;
		}
		// check if word is on the board and add error message if it is not
		if (CheckIfWordIsOnTheBoard(word, board, 0, -1)) {
			wordsFound[dictionaryWordIndex] = true;		// word was found, set value to true
			points = NumberOfPointsScored(word);		// assign string lenghth to points
			score += points;							// increment total score by points
			cout << "   Worth " << points << " points." << endl;
			DisplayGuessedWords(dictionary, wordsFound);
		}
		else {
			cout << "cannot be formed on this board." << endl;
		}
	}
	else {
		cout << "word was not found in dictionary" << endl << endl;;
	}
}


// ----------------------------------------------------------------------------
// Based on Professor's Reed function
// Reads in the dictionary words into array
void ReadInTheWordsFromDictionary(char **&dictionary){
	// Now read the words from the dictionary
	ifstream inStream;                          // declare an input stream for my use
	int wordRow = 0;                            // Row for the current word
	int numOfWords = 0;							// counts the words read in
	inStream.open("dictionary.txt");			// open the file
	assert(!inStream.fail());					// make sure file open was OK

	// Keep repeating while input from the file yields a word
	while (inStream >> dictionary[wordRow]) {
		numOfWords++;							// increment number of total words
		if (strlen(dictionary[wordRow]) > 2 && strlen(dictionary[wordRow]) < 17) {
			strlwr(dictionary[wordRow]);		// make sure word is 
			// append the NULL at the end of each word
			dictionary[wordRow][strlen(dictionary[wordRow])] = NULL;
			wordRow++;							// forward the word index
		}
	}

	// Display info about read in words
	cout << "The dictionary total number of words is: " << numOfWords << endl;
	cout << "Number of words of the right length is: " << wordRow << endl << endl;
}


// ----------------------------------------------------------------------------
// Initialize the bool array wordsFound to all false
// Input: bool array with words found
void InitializeTheArrayToFalse(bool wordsFound[]) {
	for (int index = 0; index < NUMBER_OF_WORDS_IN_DICTIONARY; index++) {
		wordsFound[index] = false;
	}
}


// ----------------------------------------------------------------------------
// Displays already guessed words
// Input: whole dictionary, bool array with words found
void DisplayGuessedWords(char **&dictionary, bool wordsFound[]) {
	cout << "Words so far are: ";
	// for loop go thru words of 3 to 16 letters in legth
	for (int wordLength = 3; wordLength < 17; wordLength++) {
		// for loop goes thru whole dictionary
		for (int wordIndex = 0; wordIndex < NUMBER_OF_WORDS_IN_DICTIONARY; wordIndex++) {
			// if word was already guessed and string length is equal to actual
			// word length display it
			if ((wordsFound[wordIndex] == true) && (strlen(dictionary[wordIndex]) == wordLength)) {
				cout << dictionary[wordIndex] << " ";
			}
		}
	}
	cout << endl << endl;		// extra lines for output formatting
}


// ----------------------------------------------------------------------------
// Set the board by user defined values
// Input: board array
void SetABoardManually(char board[]) {
	cout << "Enter 16 characters to be used to set the board: ";
	int rowFull = 0;		// jumps to next line if it is 4

	// fill the board with users values
	for (int spotNum = 7; spotNum <= 28; spotNum++) {
		cin >> board[spotNum];
		rowFull++;				// increment number of row elements
		if (rowFull == 4) {		// row is full
			spotNum += 2;		// jump to the next row
			rowFull = 0;		// reset counter of elements in actual row
		}
	}
}


// ----------------------------------------------------------------------------
// Verifies if time is up or not
// // Input: status of timer (active or not), initial time
bool Timer(int active, int &startTime) {
	int actualTime = time(NULL);				// actual time
	int elapsedTime = actualTime - startTime;	// time difference
	int timeLeft = 0;							// remaining time
	// time is up = true, time is not up = false
	bool timeIsUp = false;

	// Display the Timer
	if (active) {
			timeLeft = 60 - elapsedTime;
			DisplayTimer(timeLeft);
		}

	// Time is up, set timeIsUp to true
	if (timeLeft < 0) {
		timeIsUp = true;	// Time is up
	}

	return timeIsUp;		// return value of timeIsUp
}


// ----------------------------------------------------------------------------
// Displays the time left
// Input: remaining time
void DisplayTimer(int timeLeft) {	
		cout << "   " << timeLeft << " seconds remaining";	
}


// ----------------------------------------------------------------------------
// Displays all the posiible solutions
// Input: whole dictionary, board array
void DisplaySolutions(char **&dictionary, char board[]) {
	int minWordLength = 0,		// minimum word length
		maxWordLength = 0,		// maximum word length
		numOfSolutions = 0;

	// Prompt user for values and read the in varables
	cout << "Enter min and max word lengths to display: ";
	cin >> minWordLength >> maxWordLength;
	cout << "Words between " << minWordLength << " and " << maxWordLength << " are:" << endl;
	
	// Iterate over specified word length 
	for (int wordLength = minWordLength; wordLength <= maxWordLength; wordLength++ ) {
		// Iterate over the dictionary words
		for (int index = 0; index < NUMBER_OF_WORDS_IN_DICTIONARY; index++) {
			if (strlen(dictionary[index]) == wordLength) {	// word have desired legth
				if (CheckIfWordIsOnTheBoard(dictionary[index], board, 0, -1)) {
					cout << dictionary[index] << " ";	// word was found, display it
					++numOfSolutions;
				}
			}
		}
	}
	cout << endl << "Number of solutions: " << numOfSolutions << endl << endl; 
	// Exit the game after solutions are printed
	cout << endl << "Exiting the program."  <<  endl;
	exit(-1);
}



// ----------------------------------------------------------------------------
// Returns the number of points for word, depends in word length 
// Input: user word 
int NumberOfPointsScored(char word[]) {
	// word o length = 3, return 1 point
	if (strlen(word) == 3) {
		return 1;
	}
	// word o length = 4, return 2 points
	else if (strlen(word) == 4) {
		return 2;
	}
	// word o length = 5, return 4 points
	else if (strlen(word) == 5) {
		return 4;
	}
	// word o length > 5, return as many points as letters in the word
	else {
		return strlen(word);
	}
}