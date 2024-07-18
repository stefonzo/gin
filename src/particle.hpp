#include <stdio.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <memory>

struct Vec2D{
    double x, y;
};

class particle {
    protected:
        bool alive;
        double life_span;
        Vec2D position, velocity, acceleration;
    public:
        particle(double x, double y);
        virtual ~particle();
        virtual void update(double dt);
        virtual void render(SDL_Renderer* r);
};

class PrimitiveParticle : public particle {
    private:
        SDL_Color particle_color;
    public:
        PrimitiveParticle(Vec2D pos, double (*random_int));
        ~PrimitiveParticle();
        void update(double dt);
        void render(SDL_Renderer* r);
};

// Wrapper for c++ random number generation
class RandomDistribution {
    private:
        std::random_device rd;
        std::mt19937 gen;
        int a, b;
    public:
        std::uniform_int_distribution<> random_distribution;
        RandomDistribution(int a, int b);
        ~RandomDistribution();
        double GetRandomDouble(void);
};

class ParticleSystem {
    private:
        std::vector<PrimitiveParticle> particles;
        unsigned int n_particles, max_particles;
        std::unique_ptr<RandomDistribution> rand;
        void UpdateParticles(double dt);
        void RenderParticles(SDL_Renderer *r);
    public:
        ParticleSystem(unsigned int max_n_particles);
        ~ParticleSystem();
        void add_particle(Vec2D pos);
        void kill_particle(void);
};

// helpler functions

