#include<iostream>
#include<fstream>
#include<string>
#include<vector>

std::vector<std::string> theWords;
int counts[130]; // 5 words * 26 char in alphabet
void initCounts();
void statWord(std::string theWords);
std::string findBestWord();
int wordScore(std::string theWords);
 
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
    cout << findBestWord() <<endl;

    return 0;

}

int wordScore(string theWord)
{
    int score=0;
    for (int i=0; i<5; ++i)
    {
        char c = theWord[i];
        int pos=c-'a';
        score += counts[pos*5]+i;

    }
    return score;
}


string findBestWord()
{
    // Open output filestream
    ofstream fout ("wordScore.txt", ios::out);

    // Init best word
    string probWord=theWords[0];
    int highestScore = wordScore(theWords[0]);

    // Calc word scores
    for (int i=0; i<theWords.size(); ++i)
    {
        // Calc score
        int score = wordScore(theWords[i]);
        fout<<theWords[i]<<" : "<< score << endl;

        // Update score if needed
        if (score >highestScore)
        {
            highestScore=score;
            probWord=theWords[i];
        }
    }

    return probWord;
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
