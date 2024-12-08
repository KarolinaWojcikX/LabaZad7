#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <set>
#include <cctype>

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

struct Score {
    string name;
    int points;
    
    Score(const string& n, int p) : name(n), points(p) {}
};

vector<Score> loadHighScores(const string& filename) {
    vector<Score> scores;
    ifstream file(filename);
    string name;
    int points;
    while (file >> name >> points) {
        scores.emplace_back(name, points);
    }
    return scores;
}

void saveHighScores(const string& filename, const vector<Score>& scores) {
    ofstream file(filename);
    for (const auto& score : scores) {
        file << score.name << " " << score.points << endl;
    }
}

void displayHighScores(const vector<Score>& scores) {
    cout << "Najlepsze wyniki:" << endl;
    for (size_t i = 0; i < min(scores.size(), size_t(10)); ++i) {
        cout << i + 1 << ". " << scores[i].name << ": " << scores[i].points << endl;
    }
}

int calculateScore(int remainingChances, double timeSpent) {
    return static_cast<int>(1000 * remainingChances / (1 + timeSpent));
}

void displayGuessedLetters(const set<char>& guessedLetters) {
    cout << "Zgadnięte litery: ";
    for (char c : guessedLetters) {
        cout << c << " ";
    }
    cout << endl;
}
int main() {
    srand(time(0));
    string word = loadRandomWord("words.txt");
    vector<bool> guessed(word.length(), false);
    int chances = 6;
     set<char> guessedLetters;

    auto startTime = chrono::steady_clock::now();

    while (chances > 0) {
        displayWord(word, guessed);
        cout << "Pozostalo szans: " << chances << endl;
        displayGuessedLetters(guessedLetters);

        char guess;
        cout << "Podaj litere: ";
        cin >> guess;
        guess = tolower(guess);

         if (guessedLetters.count(guess) > 0) {
            cout << "Już zgadywałeś tę literę. Spróbuj innej." << endl;
            continue;
        }

        guessedLetters.insert(guess);

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

            auto endTime = chrono::steady_clock::now();
            double timeSpent = chrono::duration<double>(endTime - startTime).count();
            int score = calculateScore(chances, timeSpent);

            cout << "Gratulacje! Odgadles slowo: " << word << endl;
            cout << "Twoj wynik: " << score << " punktow" << endl;

            string playerName;
            cout << "Podaj swoje imie: ";
            cin >> playerName;

            vector<Score> highScores = loadHighScores("highscores.txt");
            highScores.emplace_back(playerName, score);
            sort(highScores.begin(), highScores.end(), [](const Score& a, const Score& b) {
                return a.points > b.points;
            });

            saveHighScores("highscores.txt", highScores);
            displayHighScores(highScores);
            return 0;
        }
    }
    

    cout << "Przegrales! Slowo to: " << word << endl;
    return 0;
}