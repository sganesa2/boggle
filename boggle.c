// Program 2 -- Boggle-ish
// An approximation of the Boggle game
// This game ONLY works with a 4x4 Boggle tray
//
// Usage:
//    When prompted, player enters an arbitrary random seed.
//    When prompted, player enters the number of seconds for timer.
//    Program prints letter grid and waits the specified number of seconds. Player writes down words.
//    When time expires, players enters words and collects points.
//    Player enters q to quit.
//
// PUT YOUR NAME IN THIS PROGRAM HEADER!!!

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

unsigned int lfsrNext();                     // get next LFSR value
void seedRandom(unsigned int seed);          // initialize random number generator
unsigned int getRandom(unsigned int limit);  // return random number between 0 and limit-1
void permute(unsigned int values[], int size);

void shakeTray(void);   // "shake" Boggle dice and set up tray
void printTray(char letters[]);   // print tray in 4x4 grid
void startTimer(unsigned int seconds);      // count seconds
unsigned int scoreWord(const char word[]);  // check if word in grid and return score
bool findWord(const char word[]);           // check if word is in grid
int midtray(int* pos, int count, char qword[]);
int outertray(int* pos, int count, char qword[]);

// global variables
// justification
// (1) Values are needed by multiple functions.
// (2) Passing as arguments makes the function interfaces awkward and hard to use.
// (3) There's no need for keeping multiple local copies of these data structures.

unsigned int lfsr = 1;
unsigned int trayDice[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
char trayLetters[16] = {0};

const char dice[16][6] = {{'A', 'A', 'C', 'I', 'O', 'T'}, // die 0
                          {'A', 'H', 'M', 'O', 'R', 'S'}, // die 1
                          {'E', 'G', 'K', 'L', 'U', 'Y'}, // die 2
                          {'A', 'B', 'I', 'L', 'T', 'Y'}, // die 3
                          {'A', 'C', 'D', 'E', 'M', 'P'}, // die 4
                          {'E', 'G', 'I', 'N', 'T', 'V'}, // die 5
                          {'G', 'I', 'L', 'R', 'U', 'W'}, // die 6
                          {'E', 'L', 'P', 'S', 'T', 'U'}, // die 7
                          {'D', 'E', 'N', 'O', 'S', 'W'}, // die 8
                          {'A', 'C', 'E', 'L', 'R', 'S'}, // die 9
                          {'A', 'B', 'J', 'M', 'O', 'Q'}, // die 10
                          {'E', 'E', 'F', 'H', 'I', 'Y'}, // die 11
                          {'E', 'H', 'I', 'N', 'P', 'S'}, // die 12
                          {'D', 'K', 'N', 'O', 'T', 'U'}, // die 13
                          {'A', 'D', 'E', 'N', 'V', 'Z'}, // die 14
                          {'B', 'I', 'F', 'O', 'R', 'X'}  // die 15
};


// Boggle-ish main function
// (1) Gets random seed and timer duration from user
// (2) Set up letter tray
// (3) Run timer -- user writes down words
// (4) User provides list of words. Program checks and scores -- enter "q" to quit.
int main() {
    unsigned int seed;
    unsigned int seconds;
    printf("Enter a random seed: ");
    fflush(stdout);
    scanf("%i", &seed);
    seedRandom(seed);  // if zero, seed is ignored
    printf("Timer (seconds): ");
    fflush(stdout);
    scanf("%d", &seconds);

    shakeTray();
    printTray(trayLetters);
    startTimer(seconds);

    char word[18] = "";
    unsigned int total = 0;
    unsigned int score = 0;
    while (1) {
        // get word from user, at most 16 characters
        printf("Word: ");
        fflush(stdout);
        scanf("%17s", word);
        if (strcasecmp(word, "q") == 0) break;  // "q" means quit
        // convert to uppercase, to make comparisons easier
        for (int i = 0; word[i] && (i < 17); i++) word[i] = toupper(word[i]);
        score = scoreWord(word);
        total += score;        // convert to uppercase, to make comparisons easier
        // also, check for any non-letter characters, and report a score of zero
        bool justLetters = true;
        for (int i = 0; word[i] && (i < 18); i++) {
            if (isalpha(word[i])) word[i] = toupper(word[i]);
            else {
                justLetters = false;
                break;  // no need to look any further
            }
        }
        if (justLetters) score = scoreWord(word);
        else score = 0;
        printf("Word score = %d, Total score = %d\n", score, total);
    }
    printf("TOTAL: %d\n", total);
    return 0;
}

// lfsrNext -- function to advance an LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned int lfsrNext(void) {
    lfsr ^= lfsr >> 7;
    lfsr ^= lfsr << 9;
    lfsr ^= lfsr >> 13;
    return lfsr;
}

// startTimer -- counts from 0 to (seconds-1)
// prints dots and numbers to help user keep track of time
// prints a message when time expires
void startTimer(unsigned int seconds) {
    unsigned int t = 0;
    unsigned int display = 1;
    while (t++ < seconds) {
        sleep(1);  // do nothing for one second
        // when wakes up, print either a dot or a number
        if ((display % 5) == 0) printf(" %d ", display);
        else printf(".");
        fflush(stdout);
        // at the end of each minute, go to a new line
        if (display == 60) { printf("\n"); display = 0; }
        display++;
    }
    if (display != 1) printf("\n");
    printf("\nTIME'S UP!\n");
}


// ----------------------------------------------------------------
// Put your function definitions below this line.
// Do not change anything above the line.
// You can also add more function declarations, if needed.
void seedRandom(unsigned int seed)//#1 Func // This function will assign lfsr an initial value 'seed' if seed!=0
{
    if(seed!=0)
    {
        lfsr=seed;
    }
}

//getRandom is used to enerate a random number in any part of the program
unsigned int getRandom(unsigned int limit) //#2 Func
{
    return (lfsrNext() % limit); // gives a random value between 0 and limit-1
}

//Permute is used to randomize the trayDice array
void permute(unsigned int values[], int size) //#4 Func
{int temp=0;
    for(int i=size-1;i>0;i--)
    {
        int j = getRandom(i+1);//find a random value for j between 0 and i
        temp=values[i]; //swap values to permute the array
        values[i]=values[j];
        values[j]=temp;
    }}

//shakeTray will find out the trayLetters array based on the permuted trayDice array
void shakeTray()
{
    int dicenumber;
    int x;
    permute(trayDice,16);
    for(int i=0;i<16;i++)
    {
        dicenumber=trayDice[i];
        x= getRandom(6); //there are six faces in a die. Thus getRandom(n) gives a random value between 0 and 5.
        trayLetters[i]=dice[dicenumber][x];
    }
}

//printTray will print out all the characters in trayLetters in a 4x4 grid format
void printTray(char letters[])
{
    int i=0;
    int count=0;
    for(i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(letters[count]=='Q')  //replace 'Q' with "Qu" only while printing
            {
                printf("%cu ",letters[count]);
            }
            else {
                printf("%c  ", letters[count]);
            }
            count++;
        }
        printf("\n");
    }
}

//scoreWord is used to find out the score for the user entered word based on results from findWord()
//score is assigned based on length of the word entered by the user
unsigned int scoreWord(const char word[]) //To find score
{
    int length = strlen(word);
    int score;
    if((length<3)||(length>16))
    {
        return 0;
    }
    else
    {
        if(findWord(word))
        {
            if((length==3)||(length==4))
            {
                score=1;
                return score;
            }
            else if((length==5))
            {
                score=2;
                return score;
            }
            else if((length==6))
            {
                score=3;
                return score;
            }
            else if((length==7))
            {
                score=5;
                return score;
            }
            else if((length>=8))
            {
                score=11;
                return score;
            }
        }
    }
    return 0;
}

//findword() is used to check if the word can be formed LEGALLY on the grid
bool findWord(const char word[])
{
    int i;
    int count=0; //this variable checks for the number of letters in word[] that have been found LEGALLY in the grid
    int position[16] = {0}; //position[] array is used to record the positions of each element of the word[] in the 4x4 grid
    int x=1; //x and y are used to check conditions for the subsequent while loop
    int y=1;
    char qword[16]; //qword[] is the same as word[], but replaces "Qu" with 'Q'
    //the following loop finds out qword[]
    for(int j=0;word[j]!='\0';j++)
    {
        if((word[j]=='Q')&&(word[j+1]=='U'))
        {
            qword[j]=word[j];
            j++;
        }
        else
        {
            qword[j]=word[j];
        }
    }
    //loop to check the first occurence of the first letter of word[] in trayLetters[]
    for(i=0;trayLetters[i]!='\0';i++)
    {
        if(count==0)
        {
            if(qword[0]==trayLetters[i])
            {
                position[0]=i;
                while((x!=0)||(y!=0))
                {
                    //if the first letter of word[] is in the middle of the grid go to midtray()
                    if ((position[count] == 5) || (position[count] == 6) || (position[count] == 9) ||
                        (position[count] == 10))
                    {
                        count++; //count increments everytime a valid position has been found for each letter in word[] in trayLetters[]
                        x = midtray(position, count, qword); // the previous while loop runs as long as x is 1. x becomes zero when the element could not be found, or if the position has been repeated
                        y=0;
                    }
                    //otherwise it must lie in the outer part of the grid. So implement outertray() function
                    else {
                        count++;
                        y = outertray(position, count, qword);
                        x=0;
                    }
                }
            }
            if(i==(strlen(trayLetters)-1)) // if no match is found for the first letter, return false. The word is not legal
            {
                return false;
            }
        }
        else if(count>=strlen(qword)) //this will be true if all letters of word[] are legally formed on the 4x4 grid
        {
            return true;
        }
        else if(count<strlen(qword)) //this will be true only if the word[] was not found in the tray after evaluating a few correct letters
        {
            return false;
        }
    }
    return false;
}

//midtray is used to check if the subsequent letters are found in neighbouring positions
int midtray(int* pos, int count, char qword[]) //pos is a pointer to position[]
{
    int i=0;
    pos=pos+count-1; //this will change pos everytime count increments so that *pos is not always calculated at the 0th element of position[]
    //the following arrays are all the neighbouring positions of the "postions" in the middle of the grid
    int pos5[8]={0,1,2,4,6,8,9,10};
    int pos6[8]={1,2,3,5,7,9,10,11};
    int pos9[8]={4,5,6,8,10,12,13,14};
    int pos10[8]={5,6,7,9,11,13,14,15};
    if(*(pos)==5)
    {
        while(i!=8) //check for each adjacent position till match is found
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count]==trayLetters[pos5[i]]) //if match is found
            {
                for(int s=1;s<count;s++) //check if the same position has ever been used before.
                {
                    if(*(pos-count+s)==*(pos)) //if yes, then the word is not legal
                    {
                        return 0;
                    }
                }
                //else store that position in position[] array
                pos++;
                *pos=pos5[i];
                return 1;
            }
            if(i==7)
            {
                return 0;
            }
            i++;
        }
    }
    //all the following if statements are the same the one above, but for other positions
    if(*(pos)==6)
    {
        while(i!=8)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos6[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos6[i];
                return 1;
            }
            if(i==7)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==9)
    {
        while(i!=8)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos9[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos9[i];
                return 1;
            }
            if(i==7)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==10)
    {
        while(i!=8)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos10[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos10[i];
                return 1;
            }
            if(i==7)
            {
                return 0;
            }
            i++;
        }
    }
    return 0;
}

//to calculate if the letter exists in outer postions of the grid
int outertray(int* pos, int count, char qword[])
{
    int i=0;
    pos=pos+count-1;

    int pos0[3]={1,4,5};
    int pos3[3]={2,6,7};
    int pos12[3]={8,13,9};
    int pos15[3]={14,11,10};

    int pos1[5]={0,2,4,5,6};
    int pos2[5]={1,3,5,6,7};
    int pos4[5]={0,1,5,8,9};
    int pos7[5]={2,3,6,10,11};
    int pos8[5]={4,5,9,12,13};
    int pos11[5]={6,7,10,14,15};
    int pos13[5]={12,8,9,10,14};
    int pos14[5]={13,9,10,11,15};

    if(*(pos)==0)
    {
        while(i!=3)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos0[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos0[i];
                return 1;
            }
            if(i==2)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==3)
    {
        while(i!=3)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos3[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos3[i];
                return 1;
            }
            if(i==2)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==12)
    {
        while(i!=3)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos12[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos12[i];
                return 1;
            }
            if(i==2)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==15)
    {
        while(i!=3)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos15[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos15[i];
                return 1;
            }
            if(i==2)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==1)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos1[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos1[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==2)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos2[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos2[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==4)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos4[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos4[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==7)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos7[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos7[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==8)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos8[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos8[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==11)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos11[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos11[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==13)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos13[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos13[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    if(*(pos)==14)
    {
        while(i!=5)
        {
            if(count>strlen(qword))
            {
                return 0;
            }
            if (qword[count] == trayLetters[pos14[i]])
            {
                for(int s=1;s<count;s++)
                {
                    if(*(pos-count+s)==*(pos))
                    {
                        return 0;
                    }
                }
                pos++;
                *pos=pos14[i];
                return 1;
            }
            if(i==4)
            {
                return 0;
            }
            i++;
        }
    }
    return 0;
}