/*
	Program 2: Weaver
	Course: CS 211, UIC
	System: Visual Studio
	Author: Alejandro Cuevas
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Checks the file to see how many words have the users size that they entered
int checkNumberOfWords(int size, char fileName[]){
	int words = 0; // Stores the amount of words that are the same as the size the user entered
	
	 FILE *filePtr  = fopen(fileName, "r"); // "r" means we open the file for reading
	
	// Check that the file was able to be opened
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", fileName);
		exit(-1);
	}

	// Read each word from file, and print them one by one
	char inputString[ 81];
	while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == size){ //Comptares size
			words += 1;
		}
	}
	// Close the file
	fclose(filePtr);

	return words;
}

void storeWords(int size, char **userWord, char fileName[]){ // Stores the words to the array
	int i = 0;
	 FILE *filePtr  = fopen(fileName, "r"); // "r" means we open the file for reading
	
	// Check that the file was able to be opened
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", fileName);
		exit(-1);
	}

	// Read each word from file, and print them one by one
	char inputString[ 81];
	while(fscanf(filePtr, "%s", inputString) != EOF) {
		if(strlen(inputString) == size){ //Comptares size
			userWord[i] = (char*)malloc(strlen(inputString) + 1);
			strcpy(userWord[i], inputString); // Stores word in the array
			i++; 
		}
	}
	// Close the file
	fclose(filePtr);
}

bool isValid(char *word, char **listOfAllWord, int numberOfPossibleWords){ // Checks that the word is a dictionary word
	bool valid = false;

	for(int i = 0; i < numberOfPossibleWords; i++){
		if(strcmp(word,listOfAllWord[i]) == 0){
			valid = true;
		}
	}

	return valid;
}

int wordChangeCheck(char *startWord, char *userWord, int size){ // Checks the amount of new changes made to the word
	int changes = 0; // The number of different changes

	for (int i = 0; i < size; i++){
		if(startWord[i] != userWord[i]){
			changes++;
		}
	}
	return changes;
}

//Prints message that introduces the game
void gameIntroduction(){
	printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
	printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
	printf("Enjoy!\n\n");
}

void menu(){
	printf("\nEnter:  1 to play again,\n");
	printf("        2 to change the number of letters in the words and then play again, or\n");
	printf("        3 to exit the program.\n");
	printf("Your choice --> ");
}

int main() {
	char fileName[] = "words.txt"; // File contains all words
	int size = 0; // Stores the letters in the word
	srand(1);
	//Introduction message of the game
	gameIntroduction();
	
	printf("How many letters do you want to have in the words? ");
	scanf("%d", &size);
	
	//Prints the total number of words with the size number
	printf("Number of %d-letter words found: %d.\n\n", size, checkNumberOfWords(size,fileName));

	//Allocating memory for the array
	char **wordOfSize = (char**)malloc((checkNumberOfWords(size, fileName) *sizeof(char*)));

	//Stores every word from the file with the same size that the user entered.
	storeWords(size, wordOfSize, fileName);

	int numberOfPossibleWords = checkNumberOfWords(size, fileName);

	bool game = true;
	char startWord[size]; // Holds the user start word
	char endWord[size]; // Holds the user end word

	while(game != false){
		printf("Enter starting and ending words, or 'r' for either for a random word: ");
		scanf("%s %s", startWord, endWord);

		if(strcmp(startWord, "r") == 0){
			strcpy(startWord, wordOfSize[rand() % numberOfPossibleWords]); // Gets a random word with the same size
		}

		if(strcmp(endWord, "r") == 0){
			strcpy(endWord, wordOfSize[rand() % numberOfPossibleWords]); // Gets a random word with the same size
		}

		if(strlen(startWord) != size){ // Make sure that the start word entered is the same size that the user entered
			printf("Your word, '%s', is not a %d-letter word. Try again.\n", startWord, size);
			continue;
		}

		if(isValid(startWord, wordOfSize, numberOfPossibleWords) == false){ // Checks if the word entered is a valid dictionary word
			printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startWord);
			continue;
		}

		if(strlen(endWord) != size){ // Make sure that end word entered is the same size that the user entered
			printf("Your word, '%s', is not a %d-letter word. Try again.\n", endWord, size);
			continue;
		}

		if(isValid(endWord, wordOfSize, numberOfPossibleWords) == false){ //Checks if the word entered is a valid dictionary word
			printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endWord);
			continue;
		}

		printf("Your starting word is: %s.\n", startWord);
		printf("Your ending word is: %s.\n", endWord);

		printf("\nON each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
		printf("You may also type in 'q' to quit guessing.\n");

		int counter = 1; // Keeps the number of moves
		char currWord[size]; // This is will be used as the start word which will be made changes
		char userWord[size]; // This will hold the word inputs that the user will enter
		strcpy(currWord, startWord);

		bool gameStart = true;

		while(gameStart){
			printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", counter, currWord, endWord);
			scanf("%s", userWord);

			if(strcmp(userWord, "q") == 0){ 
				break;  // 'q' allows the user to quit guessing
			}

			if(strlen(userWord) != size){ // Make sure the word entered is the same size
				printf("Your word, '%s', is not a %d-letter word. Try again.\n", userWord, size);
				continue;
			}

			if(!isValid(userWord, wordOfSize, numberOfPossibleWords)){ // Make sure the word is a valid dictionary word
				printf("Your word, '%s', is not a valid dictionary word. Try again.\n", userWord);
				continue;
			}

			if(wordChangeCheck(currWord, userWord, size) != 1){ //Make sure that only one change to the word has been made
				printf("Your word, '%s', is not exactly 1 character different. Try again.\n", userWord);
				continue;
			}

			if(strcmp(userWord, endWord) == 0){ //If the input is the same as the end word, it congratulates the user
				printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", startWord, endWord, counter);
				game = false;
				break;
			}
			strcpy(currWord, userWord); //If no error, currWord is replaced by the word the user entered
			counter++; // Keeps track of the number of moves made
		}
		int option = 0; // Stores the option the user inputs
		menu(); // Prints the menu options
		scanf("%d", &option);

		if(option == 1){
			//Resetting the words and counter
			strcpy(startWord, "");
			strcpy(endWord, "");
			strcpy(currWord, "");
			counter = 1;
			game = true;
		}
		else if(option == 2){
			printf("How many letters do you want to have in the words? ");
			scanf("%d", &size);

			//Freeing the original list of words that we stored from the file with the size that the user entered originally
			for(int i = 0; i < numberOfPossibleWords; i++){
				free(wordOfSize[i]);
			}
			free(wordOfSize);

			// Using new size to set the list of all words of that size and number of possible words
			numberOfPossibleWords = checkNumberOfWords(size, fileName);
			printf("Number of %d-letter words found: %d.\n", size, numberOfPossibleWords);
			wordOfSize = (char**)malloc(numberOfPossibleWords * sizeof(char*)); 
			storeWords(size, wordOfSize, fileName);
			game = true; //Reseting the while loop back to true, so that the game can be played
		}
		else if(option == 3){
			printf("\nThanks for playing!\n");
			printf("Exiting...");
			game = false; // While loop stops
		}
	}
	//Freeing every element of the list that contained every possible word with the size the user entered
	for(int i = 0; i < numberOfPossibleWords; i++){
		free(wordOfSize[i]);
	}

	free(wordOfSize);
	}