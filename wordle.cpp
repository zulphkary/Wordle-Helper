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
void feedback();
bool contains(std::string theWord, char c, int pos);
void suggestion();

 
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
    cout << "Suggested Guess Attempts:" <<findBestWord() <<endl;

    // 5 Guess Attempts
    for (int i=0; i<5;++i)
    {
        feedback();

        suggestion();
        if (theWords.size()==1)
        {
            break;
        }


    }
    return 0;

}

void suggestion()
{
    // clear counts
    initCounts();

    // retake stats
    for (int i=0; i<theWords.size(); ++i)
    {
        statWord(theWords[i]);
    }
    cout<<"Suggested Next Guess: " <<findBestWord()<<endl<<endl;
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
void feedback()
{
    // Ask user for feedback 
    cout << "Input for each index whether the guess was false/wrong/true (F/W/T)";
    cout << "E.g. TFTWF" << endl;
    
    
    // Take user input and readjust theWords vector
    char c;
    int n;
    for (int i=0; i<5; ++i)
    {
        cin >> c>> n;
        cout << c<< n << endl;
        for (int j=0; j<theWords.size(); ++j)
        {
            bool doDelete = false;
            switch (n)
            {
                case 0:
                    cout << "print 1" << endl;
                    doDelete = contains (theWords[j], c, -1);
                    cout << "print 2" << endl;
                    break;
                case 1:
                    doDelete = !contains (theWords [j], c, -1) || contains (theWords[j], c, i); 
                    break;
                case 2:
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

// void feedback()
// {
//     // Ask user for feedback 
//     cout << "Input for each index whether the guess was false/wrong/true (F/W/T)";
//     cout << "E.g. TFTWF" << endl;
    
//     char c;
//     int n;
//     for (int i=0; i<5; ++i)
//     {
//         cin>>n;
//         for (int j=0;j<theWords.size();++j)
//         {
//             bool doDelete=false;
//             switch (n)
//             {
//                 case 0:
//                    doDelete=contains(theWords[j],c,-1);
//                    break;
//                 case 1:
//                     doDelete=!contains(theWords[j],c,-1)||contains(theWords[j],c,i);
//                     break;
//                 case 2:
//                    doDelete=!contains(theWords[j],c,i);
//                    break;
//                 default:
//                     cout<< "error"<<endl;
//             }
//             if (doDelete)
//             {
//                 theWords.erase(theWords.begin()+j);
//                 --j;
//             }
//         }

//         }
//     cout<< endl;
//     //ouput data to file
//     ofstream fout("possibleAnswers.txt", ios::out);

//     //print data
//     for (int i=0; i<theWords.size(); ++i)
//     {
//         fout<< theWords[i]<<endl;
//     }

//     fout.close();

// }


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
