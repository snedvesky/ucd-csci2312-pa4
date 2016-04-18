//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//Game.cpp
#include <sstream>
#include <iomanip>
#include <set>
#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

namespace Gaming {
    
    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;
    PositionRandomizer Game::__posRandomizer = PositionRandomizer();
  
    void Game::populate() {
        std::default_random_engine grandom;
        std::uniform_int_distribution<int> d(0, __width * __height);
        
        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;
        
        while (numStrategic > 0) {
            int i = d(grandom);
            if (i != (__width * __height) && __grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }
        
        while (numSimple > 0) {
            int i = d(grandom);
            if (i != (__width * __height) && __grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }
        
        while (numFoods > 0) {
            int i = d(grandom);
            if (i != (__width * __height) && __grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }
        
        while (numAdvantages > 0) {
            int i = d(grandom);
            if (i != (__width * __height) && __grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
            }
        }
    }

    Game::Game() : Game(3,3) {
    }
    
    Game::Game(unsigned width, unsigned height, bool manual) : __width(width), __height(height) {
        if (width < MIN_WIDTH || height < MIN_HEIGHT) {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }
        
        __status = NOT_STARTED;
        __verbose = false;
        __round = 0;
        
        for (unsigned i = 0; i < (__width * __height); ++i) {
            __grid.push_back(nullptr);
        }
        
        if (!manual) {
            populate();
        }
    }
    
    Game::~Game() {
        
    }
    
    unsigned int Game::getNumPieces() const {
        unsigned int numPieces = this->getNumResources() + this->getNumAgents();
        return numPieces;
    }
    
    unsigned int Game::getNumAgents() const {
        unsigned int numAgents = this->getNumSimple() + this->getNumStrategic();
        return numAgents;
    }
    
    unsigned int Game::getNumSimple() const {
        unsigned int numSimple = 0;
        for(auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if(*it != nullptr && (*it)->getType() == PieceType::SIMPLE)
            {
                Simple* simple = dynamic_cast<Simple *>(*it);
                if(simple)   ++numSimple;
            }
        }
        return numSimple;
    }
    
    unsigned int Game::getNumStrategic() const {
        unsigned int numStrategic = 0;
        for(auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if(*it != nullptr && (*it)->getType() == PieceType::STRATEGIC)
            {
                Strategic* strategic = dynamic_cast<Strategic *>(*it);
                if(strategic)   ++numStrategic;
            }
        }
        return numStrategic;
    }
    
    unsigned int Game::getNumResources() const {
        unsigned int numResources = 0;
        for(auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if(*it != nullptr && ((*it)->getType() == PieceType::FOOD || (*it)->getType() == PieceType::ADVANTAGE))
            {
                Resource* resource = dynamic_cast<Resource*>(*it);
                if(resource)    ++ numResources;
            }
        }
        return numResources;
    }
    
    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[y + (x * __width)] == nullptr) throw PositionEmptyEx(x, y);
        return __grid[y + (x * __width)];
    }
    
    void Game::addSimple(const Position &position) {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);
        
        __grid[index] = new Simple(*this, position, STARTING_AGENT_ENERGY);
    }
    
    void Game::addSimple(const Position &position, double energy) { // used for testing only
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);
        
        __grid[index] = new Simple(*this, position, energy);
    }
    
    void Game::addSimple(unsigned x, unsigned y) {
        Position p(x,y);
        addSimple(p);
    }
    
    void Game::addSimple(unsigned x, unsigned y, double energy) {
        Position p(x,y);
        addSimple(p, energy);
    }
    
    void Game::addStrategic(const Position &position, Strategy *s) {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);
        
        __grid[index] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }
    
    void Game::addStrategic(unsigned x, unsigned y, Strategy *s) {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);
        
        __grid[index] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
    }
    
    void Game::addFood(const Position &position) {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);
        
        __grid[index] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
    }
    
    void Game::addFood(unsigned x, unsigned y) {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);
        
        __grid[index] = new Food(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }
    
    void Game::addAdvantage(const Position &position) {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);
        
        __grid[index] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }
    
    void Game::addAdvantage(unsigned x, unsigned y) {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);
        
        __grid[index] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }
    
    const Surroundings Game::getSurroundings(const Position &pos) const {
        Surroundings sur;
        for (unsigned i = 0; i < 3; i++) {
            for (unsigned j = 0; j < 3; j++) {
                if(i == 1 && j == 1)
                    sur.array[(i * 3) + j] = PieceType::SELF;
                else if (((pos.x + i) > 0 && (pos.x + i - 1) < __height) && ((pos.y + j) > 0 && (pos.y + j - 1) < __width)) {
                    try {
                        const Piece *p = getPiece((pos.x + i) - 1, (pos.y + j) - 1);
                        sur.array[(i * 3) + j] = p->getType();
                    } catch(PositionEmptyEx &p) {
                        sur.array[(i * 3) + j] = PieceType::EMPTY;
                    }
                } else {
                    sur.array[(i * 3) + j] = PieceType::INACCESSIBLE;
                }
            }
        }
        return sur;
    }
    
    const ActionType Game::reachSurroundings(const Position &from, const Position &to) {
        if (from.x == to.x && from.y == to.y)
            return ActionType::STAY;
        
        else if (from.x == to.x && (from.y - 1) == to.y)
            return ActionType::W;
        
        else if (from.x == to.x && (from.y + 1) == to.y)
            return ActionType::E;
        
        else if ((from.x + 1) == to.x && (from.y - 1) == to.y)
            return ActionType::SW;
        
        else if ((from.x - 1) == to.x && (from.y - 1) == to.y)
            return ActionType::NW;
        
        else if ((from.x - 1) == to.x && (from.y) == to.y)
            return ActionType::N;
        
        else if ((from.x + 1) == to.x && (from.y) == to.y)
            return ActionType::S;
        
        else if ((from.x + 1) == to.x && (from.y + 1) == to.y)
            return ActionType::SE;
        
        else if ((from.x - 1) == to.x && (from.y + 1) == to.y)
            return ActionType::NE;
        
        else
            return ActionType::NE;
        
    }
    
    bool Game::isLegal(const ActionType &ac, const Position &pos) const {
        if (ac == ActionType::NW) return pos.x > 0 && pos.y > 0;
        else if (ac == ActionType::SW)
            return pos.x < (__height - 1) && pos.y > 0;
        else if (ac == ActionType::NE)
            return pos.x > 0 && pos.y < (__width - 1);
        else if (ac == ActionType::SE)
            return pos.x < (__height - 1) && pos.y < (__width - 1);
        else if (ac == ActionType::W)
            return pos.y > 0;
        else if (ac == ActionType::E)
            return pos.y < (__width - 1);
        else if (ac == ActionType::N)
            return pos.x > 0;
        else if (ac == ActionType::S)
            return pos.x < (__height - 1);
        else
            return pos.y > 0;
    }
    
    const Position Game::move(const Position &pos, const ActionType &ac) const {
        if (isLegal(ac, pos)) {
            switch (ac) {
                case STAY:
                    return pos;
                case NW:
                    return Position(pos.x - 1, pos.y - 1);
                case N:
                    return Position(pos.x - 1, pos.y);
                case NE:
                    return Position(pos.x - 1, pos.y + 1);
                case W:
                    return Position(pos.x, pos.y - 1);
                case E:
                    return Position(pos.x, pos.y + 1);
                case SW:
                    return Position(pos.x + 1, pos.y - 1);
                case S:
                    return Position(pos.x + 1, pos.y);
                case SE:
                    return Position(pos.x + 1, pos.y + 1);
            }
        } else
            return pos;
    }
    
    void Game::round() {
        std::set<Piece*> gP;
        for (auto reset = __grid.begin(); reset != __grid.end(); ++reset)
        {
            if (*reset)
            {
                gP.insert(gP.end(), *reset);
                (*reset)->setTurned(false);
            }
        }
        for (auto turn = gP.begin(); turn != gP.end(); ++turn)
        {
            if (!(*turn)->getTurned())
            {
                (*turn)->setTurned(true);
                (*turn)->age();
                ActionType ac = (*turn)->takeTurn(getSurroundings((*turn)->getPosition()));
                Position pB = (*turn)->getPosition();
                Position pA = move(pB, ac);
                if (pB.x != pA.x || pB.y != pA.y) {
                    Piece *p = __grid[pA.y + (pA.x * __width)];
                    if (p) {
                        (*(*turn)) * (*p);
                        if ((*turn)->getPosition().x != pB.x || (*turn)->getPosition().y != pB.y) {
                            __grid[pA.y + (pA.x * __width)] = (*turn);
                            __grid[pB.y + (pB.x * __width)] = p;
                        }
                    }
                    else {
                        (*turn)->setPosition(pA);
                        __grid[pA.y + (pA.x * __width)] = (*turn);
                        __grid[pB.y + (pB.x * __width)] = nullptr;
                    }
                }
            }
        }
        
        for (unsigned int i = 0; i < __grid.size(); ++i) {
            if (__grid[i] && !(__grid[i]->isViable())) {
                delete __grid[i];
                __grid[i] = nullptr;
            }
        }
        
        if (getNumResources() <= 0) {
            __status = Status::OVER;
        }
        __round++;
    }
    
    void Game::play(bool verbose) {
        __verbose = verbose;
        __status = PLAYING;
        std::cout << *this;
        while (__status != OVER) {
            round();
            if (verbose) std::cout << *this;
        }
        if (!verbose) std::cout << *this;
    }
    
    std::ostream &operator<<(std::ostream &os, const Game &game) {
        os << "Round " << game.__round << std::endl;
        for(int i = 0; i < (game.__width * game.__height); i++){
            if(game.__grid[i] == nullptr){
                os << "[    ]";
            }
            else{
                os << "[" << *game.__grid[i] << "]";
            }
            if((i+1) % game.__width == 0){
                os << std::endl;
            }
        }
        if(game.__status == Game::NOT_STARTED){
            os << "Status: " << "Not Started!" << std::endl;
        }
        if(game.__status == Game::PLAYING){
            os << "Status: " << "Playing.." << std::endl;
        }
        if(game.__status == Game::OVER){
            os << "Status: " << "Over!" << std::endl;
        }
        
        return os;
    }
}