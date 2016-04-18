
#include <iostream>

//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Agent.cpp
#include "Piece.h"
#include "Agent.h"
#include "Resource.h"

using namespace std;

namespace Gaming {
    const double Agent::AGENT_FATIGUE_RATE = 0.3;
    
    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g, p) {
        __energy = energy;
    }
    
    Agent::~Agent() {
    }
    
    void Agent::age() {
        __energy -= AGENT_FATIGUE_RATE;
    }
    
    Piece &Agent::operator*(Piece &other) {
        Piece *p = &other;
        Resource *rez = dynamic_cast<Resource*>(p);
        if (rez) {
            interact(rez);
        }
        
        Agent *agent = dynamic_cast<Agent*>(p);
        
        if (agent) {
            interact(agent);
        }
        if (!isFinished()) {
            
            Position newP;
            newP = other.getPosition();
            Position oldP;
            oldP = getPosition();
            setPosition(newP);
            other.setPosition(oldP);
        }
        return *this;
    }
    
    Piece &Agent::interact(Agent *other) {
        if (__energy == other->__energy) {
            finish();
            other->finish();
        }
        else {
            if (__energy > other->__energy) {
                __energy -= other->__energy;
                other->finish();
            }
            else {
                other->__energy -= __energy;
                finish();
            }
        }
        return *this;
    }
    
    Piece &Agent::interact(Resource *other) {
        __energy += other->consume();
        return *this;
    }
}