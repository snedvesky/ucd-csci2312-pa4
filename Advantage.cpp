//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Advantage.cpp
#include "Advantage.h"

namespace Gaming
{
    
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;
    const char Advantage::ADVANTAGE_ID = 'D';
    
    Advantage::Advantage(const Game &g, const Position &p,double capacity) : Resource(g,p,capacity*ADVANTAGE_MULT_FACTOR) {
    }
    
    Advantage::~Advantage(){
    }
    
    void Advantage::print(std::ostream &os) const {
        os << ADVANTAGE_ID << __id;
    }
    
    double Advantage::getCapacity() const {
        return Resource::getCapacity();
    }
    
    double Advantage::consume() {
        double capacity = getCapacity();
        __capacity = 0;
        finish();
        return capacity;
    }
    
}