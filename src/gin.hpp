#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <cstdint>
#include <vector>
#include <random>
#include "particle.hpp"

class texture {
    private:
        int m_width, m_height;
        SDL_Texture* m_texture;
        void free(void);
    public:
        void setWidth(int width);
        void setHeight(int height);
        int getWidth(void);
        int getHeight(void);
        void load_from_file(std::string path, SDL_Renderer* r);
        void render(int x, int y, SDL_Renderer* r, SDL_Rect* clip);
        texture();
        ~texture();
};

class ttf {
    private:
        TTF_Font* m_font;
        SDL_Texture* m_font_texture;
        int m_width, m_height;
    public:
        ttf();
        ~ttf();
        SDL_Color ttf_color;
        int getWidth(void);
        int getHeight(void);
        void free(void);
        void open_font(std::string font_path, int pt_size);
        void load_from_rendered_text(std::string text, SDL_Renderer* r);
        void render(int x, int y, SDL_Renderer* r, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

class animation {
    private:
        int n_animation_frames, frame_width, frame_height, cols, rows;
        unsigned frame;
        bool pause_animation;
        SDL_Rect* animation_sprite_clips;
        texture animation_texture;
        void set_animation_clips(void);
    public:
        animation(int frames, int clip_width, int clip_height, int col, int row);
        ~animation();
        void load_spritesheet(SDL_Renderer* r, std::string path);
        void play(SDL_Renderer* r, int x, int y);
        void pause(void);
        void unpause(void);
};

class gin {
    private:
        SDL_Window* g_window = NULL;
        SDL_Renderer* g_renderer = NULL;
        unsigned g_window_height, g_window_width;
        std::string g_window_name;
        std::vector<texture> m_textures;
        void init(void);
        void cleanup(void);

        // testing textures, ttf, animations
        texture sprite_sheet;
        ttf pixel_font;
        animation stick_figure = animation(4, 64, 205, 4, 1);

        // clip rectangles
        SDL_Rect tile_sprite = {0, 0, 100, 100}; 
    public:
        gin(unsigned window_width, unsigned window_height, std::string window_title);
        ~gin();
        void loop(void); // main app loop 
};