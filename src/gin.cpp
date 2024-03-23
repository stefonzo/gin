#include "gin.hpp"

void gin::init(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        printf("SDL could not intialize! SDL Error: %s\n", SDL_GetError());
    
    // set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning: Linear texture filtering not enabled!\n");
    
    // create window
    g_window = SDL_CreateWindow(g_window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_window_width, g_window_height, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    
    // create renderer for window
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    
    // initialize renderer color
    SDL_SetRenderDrawColor(g_renderer, 100, 0, 100, 0xFF);

    // initialize PNG loading
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image could not intialize! SDL_image Error: %s\n", IMG_GetError());
    }

    // load spritesheet from file
    sprite_sheet.load_from_file("tileset.png", g_renderer);

    // initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    // open font for use
    pixel_font.open_font("test.ttf", 32);

    // render text
    pixel_font.load_from_rendered_text("Stephen Robinson", g_renderer);

    // load animation texture
    stick_figure.load_spritesheet(g_renderer, "foo.png");
}

gin::gin(unsigned window_width, unsigned window_height, std::string window_title) {
    g_window_height = window_height;
    g_window_width = window_width;
    g_window_name = window_title; 
    init();
}

void gin::cleanup(void) {
    // clean up ttf
    pixel_font.free();

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    g_renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

gin::~gin() {
    cleanup();
}

void gin::loop(void) {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // clear screen
        SDL_RenderClear(g_renderer);

        // render full spritesheet???
        sprite_sheet.render(300, 300, g_renderer, &tile_sprite);

        // render ttf?
        pixel_font.render(200, 200, g_renderer);

        // render animation
        stick_figure.play(g_renderer, 0, 0);

        // update screen
        SDL_RenderPresent(g_renderer);
    }
}


texture::texture() {
    m_texture = NULL;
    m_width = 0;
    m_height = 0;
}

void texture::free(void) {
    // free texture if it exists
    if (m_texture != NULL) {
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
        m_width = 0;
        m_height = 0;
    }
}

texture::~texture() {
    free();
}

void texture::load_from_file(std::string path, SDL_Renderer* r) {
    // get rid of preexisting texture
    free();

    // load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }

    // create texture from surface pixels
    m_texture = SDL_CreateTextureFromSurface(r, loaded_surface);
    if (m_texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        // get image dimensions
        m_width = loaded_surface->w;
        m_height = loaded_surface->h;
    }

    // get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
}

void texture::render(int x, int y, SDL_Renderer* r, SDL_Rect* clip) {
    // set rendering space and render to screen
    SDL_Rect render_quad = {x, y, m_width, m_height};

    // set clip rendering dimensions
    if (clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopy(r, m_texture, clip,  &render_quad);
}

void texture::setWidth(int width) {m_width = width;}
void texture::setHeight(int height) {m_height = height;}
int texture::getWidth(void) {return m_width;}
int texture::getHeight(void) {return m_height;}

ttf::ttf() {
    m_font_texture = NULL;
    m_font = NULL;
    m_width = 0;
    m_height = 0;

    ttf_color.r = 0;
    ttf_color.g = 0;
    ttf_color.b = 0;
    ttf_color.a = 0;
}

ttf::~ttf() {}

void ttf::free(void) {
    // free texture if it exists
    if (m_font_texture != NULL) {
        SDL_DestroyTexture(m_font_texture);
        m_font_texture = NULL;
        m_width = 0;
        m_height = 0;
    }
    TTF_CloseFont(m_font);
    m_font = NULL;
}

void ttf::open_font(std::string font_path, int pt_size) {
    m_font = TTF_OpenFont(font_path.c_str(), pt_size);
    if (m_font == NULL) {
        printf("Failed to load pixel font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void ttf::load_from_rendered_text(std::string text, SDL_Renderer* r) {
    // get rid of preexisting texture
    if (m_font_texture != NULL) {
        SDL_DestroyTexture(m_font_texture);
        m_font_texture = NULL;
        m_width = 0;
        m_height = 0;
    }

    // render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid(m_font, text.c_str(), ttf_color);
    if (text_surface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    // create texture from surface pixels
    m_font_texture = SDL_CreateTextureFromSurface(r, text_surface);
    if (m_font_texture == NULL) {
        printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
    } else {
        m_width = text_surface->w;
        m_height = text_surface->h;
    }

    // get rid of old surface
    SDL_FreeSurface(text_surface);
}

void ttf::render(int x, int y, SDL_Renderer* r, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // set rendering space and render to screen
    SDL_Rect render_quad = {x, y, m_width, m_height};

    // set clip rendering dimensions
    if (clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopyEx(r, m_font_texture, clip, &render_quad, angle, center, flip);
}

animation::animation(int frames, int clip_width, int clip_height, int col, int row) {
    n_animation_frames = frames;
    frame_width = clip_width;
    frame_height = clip_height;
    cols = col;
    rows = row;
    pause_animation = false;
    frame = 0;
    animation_sprite_clips = new SDL_Rect[n_animation_frames];
}

animation::~animation() {
    delete[] animation_sprite_clips;
}

void animation::set_animation_clips(void) {
    int frame_index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            animation_sprite_clips[frame_index].x = j * frame_width;
            animation_sprite_clips[frame_index].y = i * frame_height;

            // ensure that frame coordinates are within texture bounds
             if (animation_sprite_clips[frame_index].x + frame_width > animation_texture.getWidth()) {
                 animation_sprite_clips[frame_index].w = animation_texture.getWidth() - animation_sprite_clips[frame_index].x;
             } else {
                animation_sprite_clips[frame_index].w = frame_width;
            }

             if (animation_sprite_clips[frame_index].y + frame_height > animation_texture.getHeight()) {
                 animation_sprite_clips[frame_index].h = animation_texture.getHeight() - animation_sprite_clips[frame_index].y;
             } else {
                animation_sprite_clips[frame_index].h = frame_height;
            }

            frame_index++;

            // break the loop if all the frames are processed
            if (frame_index >= n_animation_frames) {
                break;
            }
        }
    }
}

void animation::load_spritesheet(SDL_Renderer* r, std::string path) {
    animation_texture.load_from_file(path.c_str(), r);
    set_animation_clips();
}

void animation::play(SDL_Renderer* r, int x, int y) {
    //render current frame
    SDL_Rect* current_clip = &animation_sprite_clips[frame / n_animation_frames];
    animation_texture.render(x, y, r, current_clip);

    // go to next frame
    if (pause_animation != true) {
        frame++;
    }

    // cycle animation
    if (frame / n_animation_frames >= n_animation_frames) {
        frame = 0;
    }
}

void animation::pause(void) {
    pause_animation = true;
}

void animation::unpause(void) {
    pause_animation = false;
}