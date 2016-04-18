//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Food.cpp
#include <iomanip>
#include "Food.h"

namespace Gaming {
    
    const char Food::FOOD_ID = 'F';
    
    Food::Food(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {
    }
    
    Food::~Food() {
    }
    
    void Food::print(std::ostream &os) const {
        
        os << FOOD_ID << __id;
    }
}