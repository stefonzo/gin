#include "particle.hpp"


// constructor/destructor for particle base class

particle::particle(double x, double y) {}

particle::~particle() {}

// methods for particle base class (not implemented yet)
// I'm just trying to figure out how polymorphism works in c++ (my code is pretty redundant....)

void particle::update(double dt) {}

void particle::render(SDL_Renderer* r) {}

PrimitiveParticle::PrimitiveParticle(double x, double y) : particle(x, y) {
    position.x = x;
    position.y = y;
    particle_color.a = 0;
    particle_color.r = 
    particle_color.g =
    particle_color.b =
}

PrimitiveParticle::~PrimitiveParticle() {}

void PrimitiveParticle::render(SDL_Renderer* r) {

}

void PrimitiveParticle::update(double dt) {

}