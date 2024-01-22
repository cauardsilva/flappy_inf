#include <fstream>


using namespace std;


// CHILL:          Just sit down and relax.
// NORMAL:         The base Flappy Quero Quero experience.
// HARD:           For those who need a bigger challenge.
// INSANITY:       Good luck. You'll need it!
// INFINITY PIPES: Your fingers will need some rest after this one.
// CUSTOM:         Go crazy and create your own game mode.
enum GameDifficulty {CHILL = 0, NORMAL = 1, HARD = 2, INSANITY = 3, INFINITY = 4, CUSTOM = 5};



typedef struct Difficulty
{
    float score_threshold;
    float initial_gap_height,         gap_height_increment;
    float initial_gap_position_delta, gap_position_delta_increment;
    float initial_obstacle_speed,     obstacle_speed_increment;
    float initial_obstacle_distance,  obstacle_distance_increment;
    float initial_player_position_x;
    float initial_player_jump_speed;
    float initial_player_acceleration;
} Difficulty;


const Difficulty DEFAULT_DIFFICULTY =
{
    1000,     // score_threshold
    
    350, 0, // gap_height
    800, 0, // gap_position_delta
    6,   0, // obstacle_speed
    400, 0, // obstacle_distance
    
    96,     // player_position_x
    10,     // player_jump_speed
    .3      // player_acceleration
};


Difficulty LoadDifficulty(int difficulty_selector)
{
    ifstream difficulty_file;
    
    if ((GameDifficulty)difficulty_selector != CUSTOM)
        difficulty_file.open(TextFormat("./data/difficulties/difficulty%d.txt", difficulty_selector));
    else
        difficulty_file.open("./data/difficulties/custom_difficulty.txt");
    
    
    float difficulty_settings[12];
    Difficulty difficulty;
    
    if ((difficulty_file.is_open()) && !(difficulty_file.peek() == ifstream::traits_type::eof()))
    {
        for (int i = 0; i < 12; ++i)
            difficulty_file >> difficulty_settings[i];

        
        difficulty.score_threshold              = difficulty_settings[0] ? difficulty_settings[0]  : DEFAULT_DIFFICULTY.score_threshold;
        
        difficulty.initial_gap_height           = difficulty_settings[1] ? difficulty_settings[1]  : DEFAULT_DIFFICULTY.initial_gap_height;
        
        difficulty.gap_height_increment         = difficulty_settings[2];
        
        difficulty.initial_gap_position_delta   = difficulty_settings[3] ? difficulty_settings[3]  : DEFAULT_DIFFICULTY.initial_gap_position_delta;
        
        difficulty.gap_position_delta_increment = difficulty_settings[4];
         
        difficulty.initial_obstacle_speed       = difficulty_settings[5] ? difficulty_settings[5]  : DEFAULT_DIFFICULTY.initial_obstacle_speed;
        
        difficulty.obstacle_speed_increment     = difficulty_settings[6];
        
        difficulty.initial_obstacle_distance    = difficulty_settings[7];
        
        difficulty.obstacle_distance_increment  = difficulty_settings[8];
        
        difficulty.initial_player_position_x    = difficulty_settings[9];
        
        difficulty.initial_player_jump_speed    = difficulty_settings[10] ? difficulty_settings[10] : DEFAULT_DIFFICULTY.initial_player_jump_speed;
        
        difficulty.initial_player_acceleration  = difficulty_settings[11] ? difficulty_settings[11] : DEFAULT_DIFFICULTY.initial_player_acceleration;
        
        
        difficulty_file.close();
    }
    else
        difficulty = DEFAULT_DIFFICULTY;


    return difficulty;
}