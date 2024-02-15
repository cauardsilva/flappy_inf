typedef struct
{
  Texture2D layers[10];
  float     layers_positions[10];
  float default_speed;
} ParallaxBackground;



void DrawParallaxBackground(ParallaxBackground *parallax_background, int num_layers = 1, Color color = WHITE)
{
    for (int i = 0; i < num_layers; ++i)
    {
        DrawTexture(parallax_background->layers[i], parallax_background->layers_positions[i],                      0, color);
        DrawTexture(parallax_background->layers[i], parallax_background->layers_positions[i] + WINDOW_WIDTH + 100, 0, color);
        
        parallax_background->layers_positions[i] -= parallax_background->default_speed * i;
        
        if (parallax_background->layers_positions[i] <= -(WINDOW_WIDTH + 100))
            parallax_background->layers_positions[i] = 0;
    }
}