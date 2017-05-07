
/*
Author: Cordel Hurst
Course: CSCI 135
Instructor: Eric Schweitzer/Subash Shankar
Assignment: 135 Project 1A

This program accepts user input of a sentence with a minimum of 3 words 
and a maximum of 4. The words are converted to lower case and checked against
as many as four files as required for one of four catagories: nouns, verbs,
pronouns and adjectives. The order of the types of words is expected to fit
any of 5 structures:
1. <sentence> ::= <noun><verb><noun>
2. <sentence> ::= <noun><verb><adjective><noun>
3. <sentence> ::= <pronoun><verb><noun>
4. <sentence> ::= <pronoun><verb><adjective><noun>
5. <sentence> ::= <pronoun><verb><pronoun>
Successful matches of input word groups that are in an order that fits one of the 
above 'grammatical' structures triggers print to console statement of successfully
matched  words and the grammatical structure the ordered words match. The program 
is then exited. Any failure to fit ordered words with one of the above or failure 
to find a match from any appropriate list causes a print to console statement that 
reflects the failure and program is exited.
*/

#include<iostream>
#include<string>	//for string handling
#include<fstream>	//for advanced file operations - ifstream

using namespace std;

int scrollWhiteSpace(istream &);
int userPrompt(string &, string &, string &, string &);

void toLowerCase(string &);
void processWords(string &word1, string &word2, string &word3);
void processWords(string &word1, string &word2, string &word3, string &word4);

bool compareWords(string wordIn, string fileToOpen);
bool openFile(ifstream &, string fileName);
bool processAdjective(string word3);
bool processNoun(string wordIn);
bool processPronoun(string wordIn);
bool processVerb(string word2);
void switchGo(int sentSize, int &switchParam, string word1, string word2, string word3, string word4);

//Driver w/switch
//Precondition: none
//Postcondition: Program exited following declaration of successful match or failure to match
//words and sentence conditions.
int main()
{
	int sentSize;
	int switchParam;

	string word1;
	string word2;
	string word3;
	string word4;

	sentSize = userPrompt(word1, word2, word3, word4);

	//sentence size of 5 is not a legal sentence and will cause the program
	//to declare illegal sentence and exit
	if (sentSize == 5)
	{
		switchParam = 5;
	}
	else
	{
		switchParam = 4;//sets switch to start with verb comparison
		if (sentSize == 3)
		{
			processWords(word1, word2, word3);//ensures all lower case, 3 word sentence
		}
		if (sentSize == 4)
		{
			processWords(word1, word2, word3, word4);//ensures all lower case, 4 word sentence
		}
	}

	//Switch is where all the action really takes place...
	switchGo(sentSize, switchParam, word1, word2, word3, word4);

	return 0;
}

//Prompts user for input
//Precondition: None
//Postcondition: Upon successful entry of user data, returns either 3 or 4 assigned words
//and appropriate sentence size to be processed. Otherwise, sentence size of 5 os returned,
//signaling illegal sentence structure.
int userPrompt(string &word1, string &word2, string &word3, string &word4)
{
	int temp;

	cout << "Enter a sentence of three or four words to be evaluated: \n";
	cin >> word1 >> word2 >> word3;

	temp = scrollWhiteSpace(cin);//iterates through whitespace

	if (temp == EOF || temp == 10)
	{
		cin.clear();
		return 3;
		//sentence size 3, process word2 vs verbs
			//if match then proc word3 vs nouns. if fail then word1 vs pronoun followed by word3 vs pronoun
			//if word3 vs nouns == success, word1 vs nouns. if word1 vs nouns == success, then done.
			//if fail, then word1 vs pronoun
			//any other failure is final, triggers fail msg and exit
	}
	else
	{
		cin >> word4; 
		temp = scrollWhiteSpace(cin);//iterates through trailing whitespace if necessary @ end

		if (temp != EOF && temp != 10) //signals if next item is NOT expected eof
		{
			cin.clear();
			return 5;
		}
		else
		{
			return 4;
			//sentence size 4, process word2 verb
				//if match then proc word3 vs adjectives 
				//then word4 vs nouns. if success, then word1 vs noun.
				//if word1 vs nouns == fail, then word1 vs pronoun
				//any other failure is final, triggers fail msg and exit
		}
	}
}

//Processes user input via comparision against appropriate file data
//precondition: Three or Four strings in all lower case, a parameter for first initiation of switch
//and the size of the sentence input (number of words)
//Postcondition: Output of statement to console that either declares successful match 
//of a particular rule and word order or failure after either of which, program is
//exited.
void switchGo(int sentSize, int &switchParam, string word1, string word2, string word3, string word4)
{
	int count;
	int nounTru;
	string wordIn;

	count = 0; //pass count for words vs noun check, 0 or 1. Pass count vs pronoun, 1 or 2
	nounTru = 0; //for pronoun order check following noun check success or failure, 0 == fail, 1 == success

		/*
		word2 is checked against verbs first. Failure to match triggers illegal
		sentence message and exit of program. Path that follows a successful match
		dependent on sentence size:

		For 3 word sentences, last word, word3 is checked vs nouns. Failure to match
		sets path to check word1 and word3 vs pronouns. Failure to match either one
		triggers illegal sentence message and exit of program. If word1 vs nouns == success,
		then done and appropriate success msg triggered
						<sentence> ::= <noun>		<verb>	<noun>
						<sentence> ::= <pronoun>	<verb>	<noun>
						<sentence> ::= <pronoun>	<verb>	<pronoun>

		For 4 word sentences, word3 is checked vs adjectives, then word4 vs nouns.
		If successful, then word1 checked vs noun. If word1 vs nouns fails, then word1
		checked vs pronouns. Any other failure is final, triggers illegal sentence
		message and exitof the program.
					<sentence> ::= <noun>		<verb>	<adjective>	<noun>
					<sentence> ::= <pronoun>	<verb>	<adjective>	<noun>
		*/
	do
	{
		switch (switchParam)
		{
		case 1://<pronoun>
			if (nounTru || (sentSize == 4))
			{
				wordIn = word1;
				if (!processPronoun(wordIn))
				{
					switchParam = 5;
				}
				else
				{
					if (sentSize == 4)
					{
						cout << "Your sentence is a legal sentence by rule 4.\n";
						switchParam = 6;
					}
					else //if (nounTru) //sentence size 3, nounTru == 1, then successful previous match, nouns vs 3rd word, thus next compare for word1 vs pronouns
					{
						cout << "Your sentence is a legal sentence by rule 3.\n";
						switchParam = 6;
					}
				}
			}
			else//Sentence size 3, last word, word3 is NOT a noun and possible pronoun. Check word1, if success, check word3 vs pronoun
			{
				if (count == 1) //First pass, vs word1
				{
					wordIn = word1;
					if (!processPronoun(wordIn))
					{
						switchParam = 5;
					}
					else //successful match vs word1, send word3 vs pronoun
					{
						switchParam = 1;
					}
				}
				else //second pass, vs word3
				{
					wordIn = word3;
					if (!processPronoun(wordIn))
					{
						switchParam = 5;
					}
					else //successful match vs word3, final
					{
						cout << "Your sentence is a legal sentence by rule 5.\n";
						switchParam = 6;
					}
				}
			}
			count = 2; //Signals that word1 succesfully checked, allowing word3 to be checked vs pronoun if appropriate.
			break;
		case 2://<noun>

			if (count == 0)//zero (0) is first pass
			{
				if (sentSize == 3)
				{
					wordIn = word3;
				}
				else // for sentence size 4
				{
					wordIn = word4;
				}

				if (!processNoun(wordIn))
				{
					if (sentSize == 3) //failure, first pass , count 0
					{
						switchParam = 1; //send to check vs pronoun
					}
					else//sentence size 4, first pass, count 0
					{
						switchParam = 5;//Failure, illegal sentence
					}
				}
				else //successful match!
				{
					switchParam = 2; //send to check word1 vs noun
					nounTru = 1; //flag for successful noun match - used in pronoun check, case 1
				}
			}
			else //for second pass
			{
				wordIn = word1;
				if (!processNoun(wordIn))
				{
					//second pass through for sentence, word1 failure.
					//failure sends to check vs pronouns if (sentSize == 3)
					{
						switchParam = 1;//sends to check vs pronouns
					}
				}
				else if (sentSize == 3) //successful match!
				{
					cout << "Your sentence is a legal sentence by rule 1.\n";
					switchParam = 6;
				}
				else //sentence size 4 successful match!
				{
					cout << "Your sentence is a legal sentence by rule 2.\n";
					switchParam = 6;
				}
			}
			count = 1;
			break;
		case 3://<adjective> ONLY in 4 word sentences, ONLY in position 3 as word3

			if (!processAdjective(word3))
			{
				switchParam = 5;
			}
			else
			{
				switchParam = 2;//nouns
			}
			break;
		case 4://<verb>

			if (!processVerb(word2))
			{
				switchParam = 5;
			}
			else if (sentSize == 3)
			{
				switchParam = 2;//nouns
				break;
			}
			else//sentence size 4
			{
				switchParam = 3;//adjectives
				break;
			}
		case 5://<not a legal sentence>

			cout << "Your sentence is not a legal sentence.\n";
			switchParam = 6;
			break;
		default:
			cerr << "Something broke... An unknown error has occurred.\n";
		}
	} while (switchParam != 6);
}

//iterates through whitespace after a word that may be or is expected to be last entry
//in order to find eof
//Precondition: 3rd or 4th word of sentence input
//Postcondition: cin loaded with next non-whitespace data
int scrollWhiteSpace(istream &cin)
{
	int temp = cin.peek();
	//9 == horizontal tab
	while (temp == ' ' || temp == 9)
	{
		cin.ignore();
		temp = cin.peek();
	}
	return temp;
}

///iterates through string input and modifies strings upper case letters to lower case if needed
//Precondition: Valid string of letters
//Postcondition: Returns string with only lower case letters
void toLowerCase(string &wordIn)
{
	for (int i = 0; i < wordIn.length(); i++)
	{
		if ((wordIn[i] >= 'A') && (wordIn[i] <= 'Z')) //looks for upper case
		{
			wordIn[i] = (char)((int)wordIn[i] + 32); //modifies accordinly by replacement
		}
	}
}

//Cycles three words for toLowerCase input
//Precondition: Approved three word input
//Postcondition: Returns all words as lower case
void processWords(string &word1, string &word2, string &word3)
{
	string wordIn;

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			wordIn = word1;
		}
		else if (i == 1)
		{
			word1 = wordIn;
			wordIn = word2;
		}
		else
		{
			word2 = wordIn;
			wordIn = word3;
		}
		toLowerCase(wordIn);
	}
	word3 = wordIn;
}

//Cycles four words for toLowerCase input
//Precondition: Approved four word input
//Postcondition: Returns all words as lower case
void processWords(string &word1, string &word2, string &word3, string &word4)
{
	string wordIn;

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			wordIn = word1;
		}
		else if (i == 1)
		{
			word1 = wordIn;
			wordIn = word2;
		}
		else if (i == 2)
		{
			word2 = wordIn;
			wordIn = word3;
		}
		else
		{
			word3 = wordIn;
			wordIn = word4;
		}
		toLowerCase(wordIn);
	}
	word4 = wordIn;
}

//Calls compareWords() for adjectives
//Precondition: Valid string, position 3 in sentence of 4 words.
//Postcondition: Returns true if a match is found, returns false otherwise.
bool processAdjective(string word3)
{

	string fileToOpen = "adjectives";
	//cout << "Adjectives!\n";

	if (compareWords(word3, fileToOpen))
	{
		return true;
	}
	return false;
}

//Calls compareWords() for nouns
//Precondition: Valid string, position is variable (except word2) in sentence of either 3 or 4 words.
//Postcondition: Returns true if a match is found, returns false otherwise.
bool processNoun(string wordIn)
{
	string fileToOpen = "nouns";

	if (compareWords(wordIn, fileToOpen))
	{
		return true;
	}
	return false;
}

//Calls compareWords() for pronouns
//Precondition: Valid string, position is 1 in sentence of either 3 or 4 words or 
//	position 3 in sentence of 3 words
//Postcondition: Returns true if a match is found, returns false otherwise.
bool processPronoun(string wordIn)
{
	string fileToOpen = "pronouns";

	if (compareWords(wordIn, fileToOpen))
	{
		return true;
	}
	return false;
}

//Calls compareWords() for verbs
//Precondition: Valid string, position 2 in sentence of either 3 or 4 words.
//Postcondition: Returns true if a match is found, returns false otherwise.
bool processVerb(string word2)
{
	string fileToOpen = "verbs";

	if (compareWords(word2, fileToOpen))
	{
		return true;
	}
	return false;
}

//Iterates through file and checks each word in file for match with input word
//Precondition: Requires valid string, file of words to compare, name of file
//Postcondition: Returns true if a match is found, returns false otherwise.
//Closes file
bool compareWords(string wordIn, string fileToOpen)
{
	bool wordCheck;
	ifstream wordFile;
	string dataIn;

	openFile(wordFile, fileToOpen);
	
	int testCount = 0;
	//check first letter of word in file. compare and if ==, process remaining 
	//elements until failure or end of wordIn. if fail, clear, move to next word in file
	while (!wordFile.eof())
	{
		wordFile >> dataIn;
		
		if (wordIn[0] == dataIn[0] && wordIn.length() == dataIn.length())
		{
			int i = wordIn.length() - 1;
			wordCheck = true;

			do
			{
				if (wordIn[i] == dataIn[i])
				{
					i--;

					if (i < 0)
					{
						return true;
					}
				}
				else
				{
					wordCheck = false;
				}
			} 
			while (i >= 0 && wordCheck);
		}
		wordFile.ignore();
	}
	wordFile.close();
	return false;
}

//Attempts to open requested file
//Precondition: File of words to open, name of file
//Postcondition: Returns 1 if successful file opening, otherwise returns 0 and 
//prints errors to cerr
bool openFile(ifstream &inputFile, string fileName)
{
	inputFile.open(fileName);

	if (inputFile.fail())
	{
		cerr << "File error. Either the file could not be opened or could not "
			<< "be found with the path specified.\n";

		cerr << "File Status:\n";
		cerr << " eof bit: " << inputFile.eof() << endl;
		cerr << " fail bit: " << inputFile.fail() << endl;
		cerr << " bad bit: " << inputFile.bad() << endl;
		cerr << " good bit: " << inputFile.good() << endl;
		inputFile.clear();
		return 0;
	}
	else
	{
		return 1;
	}
}