typedef struct Rank
{
    char name[RANKING_NAME_LENGTH];
    int score;
} Rank;



char RankingTitle[5][20] = {"THE LEGEND", "THE UNSTOPPABLE", "THE RESILIENT", "THE ACROBATIC",  "THE COURAGEOUS"};
Color RankingColor[5]    = {RED,          {80,45,150,255},   {70,90,180,255}, {245,165,35,255}, {165,120,75,255}};



const Rank DEFAULT_RANKING[RANKING_QUANTITY] =
{
    {"MARCELO WALTER", 0},
    {"RODINEI",        0},
    {"YASMIN",         0},
    {"TUTU",           0},
    {"CAUAAAA",        0}
};



void LoadRanking(Rank ranking[RANKING_QUANTITY])
{
    ifstream ranking_file;
    ranking_file.open("./data/ranking.bin", ios::binary | ios::in);
    
    if ((ranking_file.is_open()) && !(ranking_file.peek() == ifstream::traits_type::eof()))
    {
        ranking_file.read((char*)ranking, sizeof(Rank) * RANKING_QUANTITY);
        ranking_file.close();
    }
    else
        for (int i = 0; i < RANKING_QUANTITY; ++i)
            ranking[i] = DEFAULT_RANKING[i];     
}



void SaveRanking(Rank *ranking)
{
    ofstream ranking_file;
    ranking_file.open("./data/ranking.bin", ios::binary | ios::out | ios::trunc);
    
    ranking_file.write((char*)ranking, sizeof(Rank) * RANKING_QUANTITY);
    
    ranking_file.close();
}