#include <stdio.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

struct Vec2D{
    double x, y;
};

class particle {
    protected:
        bool alive;
        double life_span;
        Vec2D position, velocity, acceleration;
    public:
        particle(double x, double y) {}
        virtual ~particle() {}
        virtual void update(double dt);
        virtual void render(SDL_Renderer* r);
};

class PrimitiveParticle : public particle {
    private:
        SDL_Color particle_color;
    public:
        PrimitiveParticle(double x, double y);
        ~PrimitiveParticle();
        void update(double dt);
        void render(SDL_Renderer* r);
};

class ParticleSystem{
    private:
        
};