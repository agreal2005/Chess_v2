#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>  // For directory traversal (C++17 feature)
#include "search.h"    // Your EvalBar class and related headers

namespace fs = std::filesystem;
int gamePhase;
psTables pst;

// Helper function to get the correct file stream based on material value
std::ofstream& getFileStream(int material, 
                             std::ofstream &openingFile, 
                             std::ofstream &middlegameFile, 
                             std::ofstream &endgameFile) {
    if (material <= 20) {
        return endgameFile;
    } else if (material > 74) {
        return openingFile;
    } else {
        return middlegameFile;
    }
}

void processFENFiles(const std::string &fensDir, const std::string &evalsDir, 
                     const std::string &openingFile, 
                     const std::string &middlegameFile, 
                     const std::string &endgameFile) {
    // Open files for the three phases
    std::ofstream openFile(openingFile);
    std::ofstream middleFile(middlegameFile);
    std::ofstream endFile(endgameFile);

    if (!openFile.is_open() || !middleFile.is_open() || !endFile.is_open()) {
        std::cerr << "Error: Could not open one of the output files.\n";
        return;
    }

    EvalBar evalBar;  // Create an EvalBar object

    // Iterate through all FEN files in the given directory
    for (const auto &entry : fs::directory_iterator(fensDir)) {
        std::ifstream fenFile(entry.path());
        std::string evalFilePath = evalsDir + "/" + entry.path().filename().string();

        std::ifstream evalFile(evalFilePath);
        if (!fenFile.is_open() || !evalFile.is_open()) {
            std::cerr << "Error: Could not open " << entry.path() << " or its eval file.\n";
            continue;
        }

        std::string fen, evalStr;
        while (std::getline(fenFile, fen) && std::getline(evalFile, evalStr)) {
            Board_FEN brd(fen);
            vector<vector<char>> board = brd.return_board();
            int material = get_material(brd.return_board());  // Calculate material score

            // Initialize moves and evaluation parameters
            Moves mvs(board, brd.return_turn(), brd.return_ep(), 
                      brd.return_eps(), brd.castle_options());
            EvalParams params(mvs, brd, fen);  
            AllEvalScores scores = evalBar.complete_TrainingEval(params);  // Extract features

            // Get the correct file stream based on material score
            std::ofstream &file = getFileStream(material, openFile, middleFile, endFile);

            // Write features and the target score to the correct dataset
            file << scores.PawnStructScore << ","
                 << scores.MaterialScore << ","
                 << scores.OutpostScore << ","
                 << scores.HangingPiecePenalty << ","
                 << scores.WeakerAttackedPenalty << ","
                 << scores.MobilityScore << ","
                 << scores.PiecesEval << ","
                 << scores.KingSafetyScore << ","
                 << scores.TrappedScore << ","
                 << scores.PiecesquareScore << ","
                 << evalStr << "\n";  // Write the target score
        }

        fenFile.close();
        evalFile.close();
    }

    // Close all files
    openFile.close();
    middleFile.close();
    endFile.close();

    std::cout << "Training data generated for all phases.\n";
}

int main() {
    std::string fensDir = "./fens";          // Directory containing FEN files
    std::string evalsDir = "./evals";        // Directory containing evaluation results
    std::string openingFile = "opening_data.csv";    // Opening phase data
    std::string middlegameFile = "middlegame_data.csv";  // Middlegame phase data
    std::string endgameFile = "endgame_data.csv";    // Endgame phase data

    processFENFiles(fensDir, evalsDir, openingFile, middlegameFile, endgameFile);
    return 0;
}
