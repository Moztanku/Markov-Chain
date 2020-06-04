#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<random>
#include<time.h>
#include<windows.h>


class MarkovWord {
public:
	MarkovWord(std::string str) { predecessor = str; }
	~MarkovWord() {}
	void append(std::string str){
		for (int i = 0; i < successorCount.size(); i++) {
			if (str == successorStr.at(i)) {
				successorCount.at(i) += 1;
				return;
			}
		}
		successorStr.push_back(str);
		successorCount.push_back(1);
		return;
	}
	std::string returnPre() { return predecessor; }
	std::string generateSucc() {
		int totalWords = 1;
		for (int i = 0; i < successorCount.size(); i++) {
			totalWords += successorCount.at(i);
		}
		int whichWord = rand() % totalWords;
		totalWords = 0;
		for (int i = 0; i < successorCount.size(); i++) {
			totalWords += successorCount.at(i);
			if (totalWords >= whichWord)
				return successorStr.at(i);
		}
	}
//private:
	std::string predecessor;
	std::vector<std::string> successorStr;
	std::vector<int> successorCount;
};
class markovChain {
public:
	markovChain();
	~markovChain();
	void parseInput(std::vector<std::string>& input);
	std::string generateString(int length, unsigned int seed);
//private:
	MarkovWord findWord(std::string str);
	std::vector<MarkovWord> markovWords;
};
std::vector<std::string> readInput();
void simplifyText(std::vector<std::string>& vec);

int main() {
   	std::vector<std::string> myInput = readInput();
	simplifyText(myInput);
	markovChain chain;
	chain.parseInput(myInput);
	srand(time(0));
	for (int i = 0; i < 10; i++) {
		std::cout << chain.generateString(rand() % 12 + 4, time(0)) << " ";
	}

	return 0;
}
std::vector<std::string>readInput() {
	std::vector<std::string> input;
	std::ifstream file;
	file.open("input.txt",std::ios::in);
	if (file.is_open()) {
		std::string temp;
		while (std::getline(file,temp))
		{
			std::vector<std::string> singleWords;
			int lastWord = 0;
			for (int i = 0; i < temp.size(); i++) {
				if (temp.at(i) == ' ') {
					singleWords.push_back(temp.substr(lastWord, i - lastWord));
					lastWord = i+1;
				}
			}
			input.insert(input.end(), singleWords.begin(), singleWords.end());
		}
	}
	else {
		std::cout << "Unable to open file ;c" << std::endl;
	}
	file.close();
	return input;
}
void simplifyText(std::vector<std::string>& vec) {
	std::vector<std::string> illegalCharacters = {".",",","-"," "};
	for (int i = 0; i < vec.size(); i++) {
		if (vec.at(i).back()==',' || vec.at(i).back() == '.' || vec.at(i).back() == ';' || vec.at(i).back() == ':') {
			vec.at(i) = vec.at(i).substr(0,vec.at(i).size()-1);
		}
		vec.at(i).front() = std::tolower(vec.at(i).front());
	}
}
markovChain::markovChain() {}
markovChain::~markovChain(){}
void markovChain::parseInput(std::vector<std::string> &input){
	int totalCount = input.size();
	std::vector<std::string> strPosAtVec;
	MarkovWord temp(input.at(0));
	markovWords.push_back(MarkovWord(input.at(0)));
	strPosAtVec.push_back(input.at(0));
	int counter = 0;
	for (int i = 1; i < totalCount - 1; i++) {
		for (int j = 0; j < strPosAtVec.size(); j++) {
			if (input.at(i) == strPosAtVec.at(j)) {
				markovWords.at(j).append(input.at(i+1));
				break;
			}
			else counter++;
		}
		if (counter == strPosAtVec.size()) {
			markovWords.push_back(MarkovWord(input.at(i)));
			markovWords.back().append(input.at(i + 1));
			strPosAtVec.push_back(input.at(i));
		}
		counter = 0;
		}
}
std::string markovChain::generateString(int length, unsigned int seed){
	/*for (int i = 0; i < markovWords.size(); i++) {
		std::cout <<i+1 << ". " << markovWords.at(i).predecessor <<" | "<< markovWords.at(i).successorStr.size() << std::endl;
	}*/
	//srand(seed);
	int lastWordUsed = rand() % markovWords.size();
	std::string randomString = markovWords.at(lastWordUsed).returnPre();
	std::string returnStr = randomString;
	returnStr.front() = std::toupper(returnStr.front());
	for (int i = 0; i < length; i++) {
		randomString = findWord(randomString).generateSucc();
		returnStr.append(" ");
		returnStr.append(randomString);
	}
	returnStr.append(".");
	return returnStr;
}
MarkovWord markovChain::findWord(std::string str) {
	for (int i = 0; i < markovWords.size(); i++) {
		if (markovWords.at(i).returnPre() == str)
			return markovWords.at(i);
	}
	return MarkovWord("NULL");
}