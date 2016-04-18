//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Simple.cpp
#include <sstream>
#include <string>
#include <iomanip>
#include "Simple.h"
#include <time.h>

namespace Gaming {
    
    const char Simple::SIMPLE_ID = 'S';
    
    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy) {
    }
    
    Simple::~Simple() {
    }
    
    ActionType Simple::takeTurn(const Surroundings &s) const {
        
        std::vector<int> pos;
        srand(time(NULL));
        
        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == PieceType::ADVANTAGE || s.array[i] == PieceType::FOOD) {
                pos.push_back(i);
            }
        }
        if (pos.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == PieceType::EMPTY) {
                    pos.push_back(i);
                }
            }
        }
        if (pos.size() > 0) {
            int pIndex = pos[rand() % pos.size()];
            if (pos.size() == 1) {
                pIndex = pos[0];
            }
            ActionType move;
            switch (pIndex) {
                case 0:
                    move = NW;
                    break;
                case 1:
                    move = N;
                    break;
                case 2:
                    move = NE;
                    break;
                case 3:
                    move = W;
                    break;
                case 4:
                    move = STAY;
                    break;
                case 5:
                    move = E;
                    break;
                case 6:
                    move = SW;
                    break;
                case 7:
                    move = S;
                    break;
                case 8:
                    move = SE;
                    break;
                default:
                    move = STAY;
            }
            return move;
        }
        return ActionType::STAY;
    }
    
    void Simple::print(std::ostream &os) const {

        os << SIMPLE_ID << __id;
    }
}