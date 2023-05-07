# boggle

Download and run main.c to play a simple version of the the very popular word game, Boggle. 

**Note**:
This program includes several component of Boggle, but avoids some of the harder parts, like validating that a string is actually a word in the dictionary.

## Rules of the game:

The player’s task is to find as many words as possible in a random 4x4 grid of letters. This program provides allows one or more humans to play Boggle with the computer, rather than using a physical Boggle game. Here’s how the program will work:

1. Player enters a seed number that will determine the sequence of random numbers used
throughout the run. This allows the same game to be played over and over, which will aid in
debugging and testing.
2. Player enters a time limit, in seconds(but the program doesn't really enforce this time limit). This is the amount of time the program will pause to allow humans to search for words in the grid of letters. (This may be zero, if the player wants to skip the timer.)
3. The program then “shakes” the virtual tray of letters and prints the 4x4 grid.
4. The program will pause for the number of seconds specified, printing cues as time progresses.
During this time, the human(s) should write down (on paper) as many words as possible,
following the rules below.
5. When the timer expires, the program prompts the player to enter a word that was found. The
program will (a) verify that the word can actually be found in the grid, and (b) assign a score
based on the length of the word.
6. Step 5 repeats, and the total score is accumulated, as long as the player keeps typing words. The
player enters “q” to quit, and the total score is printed.

## Important functions in the program:

1. **Shaking the tray**: Shuffles all the letters in our tray of letters.
2. **Printing the tray**: Prints a neat 4x4 set of letters(our tray) onto the console.
3. **Scoring a word**: Prints scores of each word guessed by the player. The word score is the number of letters in the guessed word. Addidional rules include:
a) If the word is less than three characters, it will have a score of zero.
b) If the word is more than 16 characters, it couldn’t possibly be found on the grid, so it will also have a score of zero.
4. **Finding a word**: Determines whether any legal sequence of positions in the grid can be used to create the word. The player recieves a '1' or a '0' based on whether the word is present in the tray or not.
