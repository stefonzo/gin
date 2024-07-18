#include "particle.hpp"


// constructor/destructor for particle base class

particle::particle(double x, double y) {}

particle::~particle() {}

// methods for particle base class (not implemented yet)
// I'm just trying to figure out how polymorphism works in c++ (my code is pretty redundant....)

void particle::update(double dt) {}

void particle::render(SDL_Renderer* r) {}


// 'Primative Particles' (just pixels)
PrimitiveParticle::PrimitiveParticle(Vec2D pos, double (*random_int)) : particle(pos.x, pos.y) {
    alive = true;
    life_span = 5000; // milliseconds
    position = pos;
}

PrimitiveParticle::~PrimitiveParticle() {}

void PrimitiveParticle::render(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, particle_color.r, particle_color.g, particle_color.b, particle_color.a);
    SDL_RenderDrawPoint(r, position.x, position.y);
}

void PrimitiveParticle::update(double dt) {

}

// rudimentary particle system

ParticleSystem::ParticleSystem(unsigned int max_n_particles) {
    n_particles = 0;
    max_particles = max_n_particles;
    rand = std::make_unique<RandomDistribution>(0, 255);
    particles.reserve(max_particles); // might get rid of this...
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::add_particle(Vec2D pos) {
    particles.insert(n_particles, PrimitiveParticle(pos, rand->GetRandomDouble()));
    n_particles++;
}

void ParticleSystem::kill_particle(void) {
    n_particles--;
}

void ParticleSystem::UpdateParticles(double dt) {
    for (unsigned int i = 0; i < n_particles; i++) {
        particles[i].update(dt);
    }
}

void ParticleSystem::RenderParticles(SDL_Renderer * r) {
    for (unsigned int i = 0; i < n_particles; i++) {
        particles[i].render(r);
    }
}

// random number wrapper class stuff

RandomDistribution::RandomDistribution(int a, int b) {
    gen = std::mt19937(rd());
    random_distribution = std::uniform_int_distribution<>(a, b);
}

RandomDistribution::~RandomDistribution() {} // what does this do right now? (nothing)

double RandomDistribution::GetRandomDouble(void) {
    return (double)(random_distribution(gen));
}