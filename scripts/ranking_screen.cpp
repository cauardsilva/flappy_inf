void ShowRankingScreen()
{    
    Rank ranking[RANKING_QUANTITY];
    LoadRanking(ranking);
    
    Texture2D background     = LoadTexture("./resources/sprites/scenes/background.png");
    Texture2D ranking_holder = LoadTexture("./resources/sprites/ui/ranking_holder.png");
    
    
    Player background_birds[5];
    for (int i = 0; i < 5; ++i)
    {
        background_birds[i].sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
        InitializeBackgroundBird(&background_birds[i]);
        background_birds[i].color = RankingColor[i];
    }
    
    
    ParallaxBackground parallax_background;
    for (int i = 0; i < 4; ++i)
    {
        parallax_background.layers[i]           = LoadTexture(TextFormat("./resources/sprites/scenes/%d.png", i));
        parallax_background.layers_positions[i] = 0;
    }
    parallax_background.default_speed = .5;
    
    
    Font font = LoadFont("./resources/fonts/queroquero.ttf");
    
    int frame = 0;
   
    while (!WindowShouldClose())
    {
        ++frame;
        
        
        BeginDrawing();
        
            DrawParallaxBackground(&parallax_background, 4);
            
            for (int i = 0; i < 5; ++i)
            {
                UpdateBackgroundBird(&background_birds[i]);
                DrawPlayer(background_birds[i], frame);
            }
            
           
            for (int i = 0; i < RANKING_QUANTITY; ++i)
            {
                DrawTexture(ranking_holder, 384, 107 + 120*i, WHITE);
                
                DrawTextEx(font, ranking[i].name[0] != '\0' ? ranking[i].name : "UNKNOWN", {419, 111 + i * 120}, 32, 0, BLACK);
                DrawTextEx(font, RankingTitle[i], {419, 138 + i * 120}, 32, 0, RankingColor[i]);
                DrawTextEx(font, TextFormat("%04d", ranking[i].score), {719, 138 + i*120}, 32, 0, BLACK);
                
            }
            
        EndDrawing();
    }


    UnloadTexture(background);
    UnloadTexture(ranking_holder);
    UnloadFont(font);
}