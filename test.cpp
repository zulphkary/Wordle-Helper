#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <unordered_set>


std::vector<std::string> theWords;
int counts[130]; // 5 words * 26 char in alphabet
void initCounts();
void statWord(std::string theWords);
std::string findStrongestWord();
int wordStrength(std::string theWords);
void guessFeedback();
bool contains(std::string theWord, char c, int pos);
void makeSuggestion();

 
using namespace std;

int main()
{
    // Init counts
    initCounts();
    
    // Open input filestream

    ifstream fin("words.txt", ios::in);

    // Read in each line

    string input;
    while (fin>>input)
    {
        theWords.push_back(input);
        statWord(input);
    }

    // Close input filestream

    fin.close();

    // Open output filestream
    ofstream fout("stats.txt", ios::out);

    // Print stats to output file
    for (char c = 'a'; c<= 'z'; ++c)
    {
        fout<< c<< "";
        int pos=c-'a';
        for (int i=0; i<5; ++i)
        {
            fout<<counts[pos*5+i] <<" ";
        }
        cout<<endl;
    }

    //Close output filestream
    fout.close();

    // Find and print most likely word 
    cout << "Suggested Guess Attempts:" <<findStrongestWord() <<endl;

    // 5 Guess Attempts
    for (int i=0; i<5;++i)
    {
        guessFeedback();

        makeSuggestion();
        if (theWords.size()==1)
        {
            break;
        }


    }
    return 0;

}

void makeSuggestion()
{
    // clear counts
    initCounts();

    // retake stats
    for (int i=0; i<theWords.size(); ++i)
    {
        statWord(theWords[i]);
    }
    //Find new suggsted word
    cout<<"Suggested Next Guess: " << findStrongestWord()<<endl<<endl;
}

bool contains(string theWords,char c, int pos)
{
    if (pos==-1)
    {
        for (int i=0; i<5; ++i)
        {
            if (theWords[i]==c)
            {
                return true;
            }
        }
    } else {
        if (theWords[pos] == c)
        {
            return true;
        }
    }
    return false;
}
void guessFeedback()
{
    // Ask user for feedback 
    cout << "Input the feedback from your previous guess, it does not have to be the suggested word";
    cout << "Write each character followed by a number which indicates the feedback you received on the word \n";
    cout << "Where 0 denotes that the letter not in the word \n";
    cout << "Where 1 denotes that the letter is in the word and in the wrong place (this should be true if the same letter is denoted as yellow or green)\n";
    cout << "Where 2 denotes that the letter is in the word and in the correct place\n";
    cout << "E.g. s0c0a1r2y2" << endl;
    
    
    // Take user input and readjust theWords vector
    char c;
    int n;
    for (int i=0; i<5; ++i)
    {
        cin >> c >> n;
        //cout << c<< n << endl;
        for (int j=0; j<theWords.size(); ++j)
        {
            bool doDelete = false;
            switch (n)
            {
                case 0: // this value is not in the answer
                    doDelete = contains (theWords[j], c, -1);
                    break;
                case 1: // this value is in the answer, but not in the correct position 
                    doDelete = !contains (theWords [j], c, -1) || contains (theWords[j], c, i); 
                    break;
                case 2: // this value is in the answer and is in the correct position
                    doDelete = !contains (theWords[j], c, i);
                    break;
                default:
                    cout << "Something went wrong.\n"; 
                    break;
            }
            if (doDelete)
            {
                theWords.erase(theWords.begin() + j);
                --j;
            }
        }
    }
    cout << endl;
    // Open output filestream
    ofstream fout ("remaining.txt", ios::out);

    // Print remaining words
    for (int i=0; i<theWords.size(); ++i)
    {
        fout << theWords[i] << endl;
    }

    fout.close();

}


// int wordStrength(string theWord)
// {
//     int strength = 0;
//     double x = 2;  // Adjust the value of x as needed


//     // Calculate the original strength  // Check for repeating characters
//     unordered_set<char> uniqueChars;
//     for (char c : theWord)
//     {
//         if (!uniqueChars.insert(c).second)
//         {
//             // Character is a repeat, decrement the strength
//             strength -= static_cast<int>(x * 100);  // Decrement by x% of 100
//         }
//     }
//     for (int i = 0; i < 5; ++i)
//     {
//         char c = theWord[i];
//         int pos = c - 'a';
//         strength += counts[pos * 5] + i;
//     }

//     for (char c : theWord)
//     {
//         if (!uniqueChars.insert(c).second)
//         {
//             // Character is a repeat, increase the strength
//             strength += static_cast<int>(x * 100);  // Increase by x% of 100
//         }
//     }
//     return strength;
// }

int wordStrength(string theWord)
{
    int strength = 0;
    double x = 2;  // Adjust the value of x as needed

    // Check for repeating characters and add/subtract points accordingly
    unordered_set<char> uniqueChars;
    for (char c : theWord)
    {
        if (!uniqueChars.insert(c).second)
        {
            // Character is a repeat, decrement the strength
            strength -= static_cast<int>(x * 100);  // Decrement by x% of 100
        }
    }

    // Calculate the original strength with additional points for vowels
    for (int i = 0; i < 5; ++i)
    {
        char c = theWord[i];
        int pos = c - 'a';
        strength += counts[pos * 5] + i;

        // Add additional points if the letter is a vowel
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        {
            strength += static_cast<int>(x * 50);  // Increase by x% of 50 for vowels
        }
    }

    return strength;
}



string findStrongestWord()
{
    // Open output filestream
    ofstream fout ("wordStrengths.txt", ios::out);

    // Init best word
    string strongestWord=theWords[0];
    int strongestStrength = wordStrength(theWords[0]);

    // Calc word scores
    for (int i=0; i<theWords.size(); ++i)
    {
        // Calc score
        int strength = wordStrength(theWords[i]);
        fout<<theWords[i]<<" : "<< strength << endl;

        // Update score if needed
        if (strength >strongestStrength)
        {
            strongestStrength=strength;
            strongestWord=theWords[i];
        }
    }

    return strongestWord;
}

void statWord(string theWord)
{
    for (int i=0; i<5; ++i)
    {
        char c = theWord[i];
        int pos= c - 'a';
        counts [pos*5+i]++;
    }
    
}

void initCounts()
{
    for (int i=0; i<130; ++i)
    {
        counts[i]=0;
    }
}
