/* Taylor Noah
 * Summer '21
 * Open Source Software Development
 * Homework #1
 */

/* This program reads in a file, finds all words that
 * are valid "alphabetic" words and then finds the dominant
 * letter in each of those words and the count of that letter
 * in each valid word.
 */


#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void parse_file_to_words();
void find_dominant_letter_and_count(struct domword* valid_word);
void print_valid_word(struct domword* valid_word);
bool is_valid(char);
void collect_garbage();

const int MAX = 256;
const int MAX_SM = 64;
ifstream read_in;

//data struct to hold valid words, thier dominant letter, and the dominant letter count
struct domword{
    char* word;
    char letter;
    int count;
};

//data struct to hold all found valid words and thier data
domword** valid_words = NULL;
//valid word index for creating new structs
int vwi = 0;


int main(int argv, char* argc[]){

    //Ensure there are two arguments
    if(argv != 2){
        cout<<"\nTo Run: ./domlets <input_file>\n";
        exit(-1);
    }

    //Open file designated by user
    read_in.open(argc[1]);

    //Ensure file exists
    if(!read_in) {
        cout<<"\nNo file by the name "<<argc[1]<<" found.\n";
        exit(-1);
    }

    //init data structure
    valid_words = new domword*[MAX_SM];
    for(int i=0; i<MAX_SM; ++i)
        valid_words[i] = NULL;


    //parse file into words
    //saves into a global data structure
    parse_file_to_words();

    //for each valid word find the dominant letter and its count
    for(int i=0; i<vwi; ++i)
    {
        find_dominant_letter_and_count(valid_words[i]);
        cout<<"\n\n";
        print_valid_word(valid_words[i]);
    }


    /*
    domword* test = new domword;
    test->word = new char[20];
    strcpy(test->word, "balloon");
    find_dominant_letter_and_count(test);
    print_valid_word(test);
    */
    
    //close file and exit
    read_in.close();
    collect_garbage();
    return 0;
}


/* Read file line by line.
 * Find valid words and add to sata structure
 */
void parse_file_to_words(){

    //Ensure file is open
    if(!read_in.is_open()){
        cerr<<"'\n'Error: File Not Open in function dom_lets()\n";
        exit(-1);
    }

    
    //create char* to catch lines of file
    //read in first line
    //set len to length of line
    char line[MAX];
    read_in.getline(line, MAX, '\n');
    int len = strlen(line);

    // i is for pretty output
    int a=0;

    //continue reading new lines if 
    // eof bit not flipped
    while(!read_in.eof()){
        cout<<'\n'<<a<<") "<<line;

        //find valid words using indexes 'i' and 'j'
        for(int i=0; i<len; ++i)
        {
            // valid words are at the begining of the line or start with a space
            if(i == 0 || line[i] == ' ')
            {
                int j=i+1;
                //to mark current word as valid or not
                bool valid = true;
                while(j<len && line[j] != ' ')
                {
                    //if there is ever an invalid char in the current word, invalidate the word
                    if(!is_valid(line[j]))
                        valid = false;
                    ++j;
                }

                //if the word is valid, find the dom letter and its count
                if(valid)
                {
                    valid_words[vwi] = new domword;
                    valid_words[vwi]->word = new char[j-i+1];
                    for(int y=0; y<j-i; ++y)
                        valid_words[vwi]->word[y] = 'p';
                   // cout<<"\n\tLength word: "<<j-i+1<<" | Strlen word: "<<strlen(valid_words[vwi]->word);
                    //if i is not 0 account for the space char and offset
                    if(i == 0)
                    {
                        for(int k=0; k<j; ++k)
                            valid_words[vwi]->word[k] = line[k];
                    //    cout<<"\n\tChars copied: "<<j;
                    }
                    else
                    {
                        for(int k=0; k<j-i; ++k)
                             valid_words[vwi]->word[k]= line[k+1+i];
                     //   cout<<"\n\tChars copied: "<<j-i;
                    }
                    //cout<<"\n\tValid word found: "<<valid_words[vwi]->word;
                    ++vwi;
                }
            }
        }
        
        //read a new line and set the new length of it
        read_in.getline(line, MAX, '\n');
        len = strlen(line);
        ++a;
    }
}



// Finds the dominant letter in a word and the count of that letter
void find_dominant_letter_and_count(domword* valid_word)
{
    if(!valid_word) return;

    int len = strlen(valid_word->word);

    int counts[26];
    for(int i=0; i<26; ++i)
        counts[i] = 0;

    for(int i=0; i<len; ++i)
        ++counts[int(valid_word->word[i]) - 97];               

    for(int i=0; i<26; ++i)
    {
        if(counts[i] > valid_word->count)
        {
            valid_word->count = counts[i];
            valid_word->letter = char(i+97);
        }
    }
}

void print_valid_word(domword* valid_word)
{
    cout<<"Word: "<<valid_word->word;
    cout<<"\nDominant Letter: "<<valid_word->letter;
    cout<<"\nDL Count: "<<valid_word->count;
}


/* Checks to see if achar is valid under the given rule:
 *  Is either:
 *     an alphabetical letter
 *     a space
 *     a delimiter (\000 or \n)
 */
bool is_valid(char check){
    if(!isalpha(check) && check != ' ' && check != 0 && check != '\n')
        return false;
    return true;
}



// Deallocates memory held in domword** global data struct
void collect_garbage(){
    if(!valid_words) 
        return;
    for(int i=0; i<vwi; ++i)
    {
        delete[] valid_words[i]->word;
        valid_words[i]->word = NULL;
        delete valid_words[i];
        valid_words[i] = NULL;
    }
    delete [] valid_words;
    valid_words = NULL;
}
