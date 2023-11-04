#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

// Fonction pour tokenizer un texte en mots
std::vector<std::string> tokenize(const std::string &text) {
    std::vector<std::string> tokens;
    std::string word;
    for (char c : text) {
        if (std::isalnum(c)) {
            word.push_back(std::tolower(c));
        } else if (!word.empty()) {
            tokens.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) {
        tokens.push_back(word);
    }
    return tokens;
}

// Fonction pour construire un vecteur de fréquence de mots
std::unordered_map<std::string, int> buildWordFrequencyVector(const std::vector<std::string> &tokens) {
    std::unordered_map<std::string, int> frequencyVector;
    for (const std::string &token : tokens) {
        frequencyVector[token]++;
    }
    return frequencyVector;
}

// Fonction pour calculer la similarité cosinus
double cosineSimilarity(const std::unordered_map<std::string, int> &vector1, const std::unordered_map<std::string, int> &vector2) {
    double dotProduct = 0.0;
    double magnitudeVector1 = 0.0;
    double magnitudeVector2 = 0.0;

    for (const auto &pair : vector1) {
        const std::string &word = pair.first;
        int count1 = pair.second;
        int count2 = vector2.count(word) ? vector2.at(word) : 0;

        dotProduct += count1 * count2;
        magnitudeVector1 += count1 * count1;
    }

    for (const auto &pair : vector2) {
        magnitudeVector2 += pair.second * pair.second;
    }

    if (magnitudeVector1 == 0 || magnitudeVector2 == 0) {
        return 0.0;
    }

    return dotProduct / (std::sqrt(magnitudeVector1) * std::sqrt(magnitudeVector2));
}

int main() {
    std::string file1, file2;

    // Spécifiez les noms des fichiers que vous souhaitez comparer
    file1 = "fichier1.txt";
    file2 = "fichier2.txt";

    // Lisez le contenu des fichiers
    std::ifstream fileStream1(file1);
    std::ifstream fileStream2(file2);

    if (!fileStream1.is_open() || !fileStream2.is_open()) {
        std::cerr << "Impossible d'ouvrir les fichiers." << std::endl;
        return 1;
    }

    std::string content1((std::istreambuf_iterator<char>(fileStream1)), std::istreambuf_iterator<char>());
    std::string content2((std::istreambuf_iterator<char>(fileStream2)), std::istreambuf_iterator<char>());

    fileStream1.close();
    fileStream2.close();

    // Tokenize les textes
    std::vector<std::string> tokens1 = tokenize(content1);
    std::vector<std::string> tokens2 = tokenize(content2);

    // Construire des vecteurs de fréquence de mots
    std::unordered_map<std::string, int> vector1 = buildWordFrequencyVector(tokens1);
    std::unordered_map<std::string, int> vector2 = buildWordFrequencyVector(tokens2);

    // Calculer la similarité cosinus
    double similarity = cosineSimilarity(vector1, vector2);
    double similarityPercentage = similarity * 100.0;

    std::cout << "Similarité cosinus entre " << file1 << " et " << file2 << " : " << similarityPercentage <<  "%" << std::endl;

    return 0;
}
