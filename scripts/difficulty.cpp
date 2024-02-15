// CHILL:    Just sit down and relax.
// DEFAULT:  The base Flappy Quero Quero experience.
// HARD:     For those who need a bigger challenge.
// INSANITY: Good luck. You'll need it!
// CUSTOM:   Go crazy and create your own game mode.
enum GameDifficulty {CHILL = 0, DEFAULT = 1, HARD = 2, INSANITY = 3, CUSTOM = 4, FIRST = CHILL, LAST = CUSTOM, SERVER = 5};

Color DifficultyColor[5] = {{160,160,235,255}, {245,245,245,255}, {235,160,140,255}, {155,80,155,80}, {255,235,180,255}};

char DifficultyText[5][10] = {"CHILL", "DEFAULT", "HARD", "INSANE", "CUSTOM"};


typedef struct
{
    float score_threshold;
    float gap_height,         gap_height_increment;
    float gap_position_delta, gap_position_delta_increment;
    float obstacle_speed,     obstacle_speed_increment;
    float obstacle_distance,  obstacle_distance_increment;
    float player_position_x;
    float player_jump_speed;
    float player_gravity;
} Difficulty;


const Difficulty DEFAULT_DIFFICULTY =
{
    1000,   // score_threshold
    
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
    
    if      ((GameDifficulty)difficulty_selector == CUSTOM) difficulty_file.open("./data/difficulties/custom_difficulty.txt");
	else if ((GameDifficulty)difficulty_selector == SERVER) difficulty_file.open("./data/difficulties/server_difficulty.txt");
    else                                                    difficulty_file.open(TextFormat("./data/difficulties/difficulty%d.txt", difficulty_selector));
    
    
    float difficulty_settings[12];
    Difficulty difficulty;
    
    if ((difficulty_file.is_open()) && !(difficulty_file.peek() == ifstream::traits_type::eof()))
    {
        for (int i = 0; i < 12; ++i)
            difficulty_file >> difficulty_settings[i];

        
        difficulty.score_threshold              = difficulty_settings[0];
        difficulty.gap_height                   = difficulty_settings[1];
        difficulty.gap_height_increment         = difficulty_settings[2];
        difficulty.gap_position_delta           = difficulty_settings[3];
        difficulty.gap_position_delta_increment = difficulty_settings[4];
        difficulty.obstacle_speed               = difficulty_settings[5];
        difficulty.obstacle_speed_increment     = difficulty_settings[6];
        difficulty.obstacle_distance            = difficulty_settings[7];
        difficulty.obstacle_distance_increment  = difficulty_settings[8];
        difficulty.player_position_x            = difficulty_settings[9];
        difficulty.player_jump_speed            = difficulty_settings[10];
        difficulty.player_gravity               = difficulty_settings[11];
        
        
        difficulty_file.close();
    }
    else
        difficulty = DEFAULT_DIFFICULTY;


    return difficulty;
}

void UpdateDifficulty(Difficulty *difficulty)
{
    if (difficulty->gap_height  - difficulty->gap_height_increment > PLAYER_HEIGHT * 2)
        difficulty->gap_height -= difficulty->gap_height_increment;
    else 
        difficulty->gap_height = PLAYER_HEIGHT * 2;
    
    
    if (difficulty->gap_position_delta  + difficulty->gap_position_delta_increment < WINDOW_HEIGHT)
        difficulty->gap_position_delta += difficulty->gap_position_delta_increment;
    else
        difficulty->gap_position_delta = WINDOW_HEIGHT;
    
    
    if (difficulty->obstacle_speed  + difficulty->obstacle_speed_increment < 400)
        difficulty->obstacle_speed += difficulty->obstacle_speed_increment;
    else
        difficulty->obstacle_speed = 400;
    
    
    if (difficulty->obstacle_distance  - difficulty->obstacle_distance_increment > 0)
        difficulty->obstacle_distance -= difficulty->obstacle_distance_increment;   
    else
        difficulty->obstacle_distance = 0;
}