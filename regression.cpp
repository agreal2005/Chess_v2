#include "search.h"

vector<pair<string, double>> PositionDatabase;
float matwt = 1;
float pawnwt = 1;
float outpostwt = 1;
float hangingwt = 1;
float weakerattacwt = 1;
float pieceswt = 1;
float pstwt = 1; // Implement this
float trappedwt = 1;
float kingwt = 1;
float mobilitywt = 1;
float GradientDescentConstant = 0.01;
int depth = 5;

void FitPawns(){
    float pawnwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        pawnwtDiffError += (Scores.TotalScore-ActualScore)*Scores.PawnStructScore;     
    }
    pawnwtDiffError = pawnwtDiffError/PositionDatabase.size();
    pawnwt -= GradientDescentConstant*pawnwtDiffError;
}

void FitOutPost(){

    float outpostwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        outpostwtDiffError += (Scores.TotalScore-ActualScore)*Scores.OutpostScore;     
    }
    outpostwtDiffError = outpostwtDiffError/PositionDatabase.size();
    outpostwt -= GradientDescentConstant*outpostwtDiffError;
}

void FitHanging(){

    float hangingwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        hangingwtDiffError += (Scores.TotalScore-ActualScore)*Scores.HangingPiecePenalty;     
    }
    hangingwtDiffError = hangingwtDiffError/PositionDatabase.size();
    hangingwt -= GradientDescentConstant*hangingwtDiffError;
}

void FitWeaker(){

    float weakerattacwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        weakerattacwtDiffError += (Scores.TotalScore-ActualScore)*Scores.WeakerAttackedPenalty;     
    }
    weakerattacwtDiffError = weakerattacwtDiffError/PositionDatabase.size();
    weakerattacwt -= GradientDescentConstant*weakerattacwtDiffError;
}

void FitMobility(){

    float mobilitywtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        mobilitywtDiffError += (Scores.TotalScore-ActualScore)*Scores.MobilityScore;     
    }
    mobilitywtDiffError = mobilitywtDiffError/PositionDatabase.size();
    mobilitywt -= GradientDescentConstant*mobilitywtDiffError;
}

void FitPiecesEval(){

    float pieceswtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        pieceswtDiffError += (Scores.TotalScore-ActualScore)*Scores.PiecesEval;     
    }
    pieceswtDiffError = pieceswtDiffError/PositionDatabase.size();
    pieceswt -= GradientDescentConstant*pieceswtDiffError;
}

void FitKingSafety(){

    float kingwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        kingwtDiffError += (Scores.TotalScore-ActualScore)*Scores.KingSafetyScore;     
    }
    kingwtDiffError = kingwtDiffError/PositionDatabase.size();
    kingwt -= GradientDescentConstant*kingwtDiffError;
}

void FitTrapped(){

    float trappedwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        trappedwtDiffError += (Scores.TotalScore-ActualScore)*Scores.TrappedScore;     
    }
    trappedwtDiffError = trappedwtDiffError/PositionDatabase.size();
    trappedwt -= GradientDescentConstant*trappedwtDiffError;
}

void FitMaterial(){
 
    float matwtDiffError = 0;
    for(auto instance : PositionDatabase){
        string FENTemp = instance.first; 
        float ActualScore = instance.second;
        EvalBar Tree(FENTemp);
        pair<string, AllEvalScores> Prediction = Tree.TrainingTree(FENTemp, depth, 0, -(inf+100), inf+100); 
        AllEvalScores Scores = Prediction.second;
        matwtDiffError += (Scores.TotalScore-ActualScore)*Scores.MaterialScore;     
    }
    matwtDiffError = matwtDiffError/PositionDatabase.size();
    matwt -= GradientDescentConstant*matwtDiffError;
}

void FitWeights(){
    FitHanging();
    FitMaterial();
    FitMobility();
    FitOutPost();
    FitPawns();
    FitPiecesEval();
    FitTrapped();
    FitWeaker();
    FitKingSafety();
}

int main(){
    // Opening is material > 74
    // Endgame is material <= 20
    // Middlegame is anything in between
    int number_of_iterations = 1000;
    for(int i=0; i<number_of_iterations; i++){
        FitWeights();
    }
    /*
        Based on the testing scenario
        store results in 
        ./openingTraining/ow.txt 
        ./middlegameTraining/mw.txt 
        ./endgameTraining/ew.txt
        in this ORDER:
        matwt
        pawnwt
        outpostwt
        hangingwt
        weakerattacwt
        pieceswt 
        pstwt
        trappedwt
        kingwt
        mobilitywt
    */
}