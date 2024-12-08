#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

string loadRandomWord(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;
    while (getline(file, word)) {
        words.push_back(word);
    }
    return words[rand() % words.size()];
}

void displayWord(const string& word, const vector<bool>& guessed) {
    for (size_t i = 0; i < word.length(); ++i) {
        if (guessed[i]) {
            cout << word[i];
        }
        else {
            cout << "_";
        }
        cout << " ";
    }
    cout << endl;
}

int main() {
    srand(time(0));
    string word = loadRandomWord("words.txt");
    vector<bool> guessed(word.length(), false);
    int chances = 6;

    while (chances > 0) {
        displayWord(word, guessed);
        cout << "Pozostalo szans: " << chances << endl;

        char guess;
        cout << "Podaj litere: ";
        cin >> guess;

        bool correct = false;
        for (size_t i = 0; i < word.length(); ++i) {
            if (word[i] == guess) {
                guessed[i] = true;
                correct = true;
            }
        }

        if (!correct) {
            --chances;
        }

        if (all_of(guessed.begin(), guessed.end(), [](bool v) { return v; })) {
            cout << "Gratulacje! Odgadles slowo: " << word << endl;
            return 0;
        }
    }

    cout << "Przegrales! Slowo to: " << word << endl;
    return 0;
}