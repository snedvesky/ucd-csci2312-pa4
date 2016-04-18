//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Piece.cpp
#include "Game.h"
#include "Piece.h"

namespace Gaming {
    
    unsigned int Piece::__idGen = 100;
    
    Piece::~Piece() {
    
    }
    
    Piece::Piece(const Game &g, const Position &p) : __game(g), __position(p), __finished(false), __turned(false) {
        
        __id = __idGen++;
        
    }
    
    std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        
        piece.print(os);
        return os;
    }
    
    
}