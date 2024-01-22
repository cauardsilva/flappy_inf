#include <fstream>


#define RANKING_QUANTITY 5


using namespace std;


typedef struct Rank
{
    char name[17];
    int score;
} Rank;



const Rank DEFAULT_RANKING[RANKING_QUANTITY] =
{
    {"Marcelo Walter", 0},
    {"Rodinei",        0},
    {"Yasmin",         0},
    {"Tutu",           0},
    {"Cauaaaa",        0}
};



Rank *LoadRanking()
{
    Rank *ranking = new Rank[RANKING_QUANTITY];
    
    ifstream ranking_file;
    ranking_file.open("./data/ranking.bin", ios::binary | ios::in);
    
    if ((ranking_file.is_open()) && !(ranking_file.peek() == ifstream::traits_type::eof()))
    {
        ranking_file.read((char*)ranking, sizeof(Rank) * RANKING_QUANTITY);
        ranking_file.close();
    }
    else
        *ranking = *DEFAULT_RANKING;

      
    return ranking;
}



void SaveRanking(Rank *ranking)
{
    ofstream ranking_file;
    ranking_file.open("./data/ranking.bin", ios::binary | ios::out | ios::trunc);
    
    ranking_file.write((char*)ranking, sizeof(Rank) * RANKING_QUANTITY);
    
    ranking_file.close();
}