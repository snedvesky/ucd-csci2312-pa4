//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//DefaultAgentStrategy.cpp
#include<vector>
#include"Game.h"
#include"DefaultAgentStrategy.h"

namespace Gaming
{
    DefaultAgentStrategy::DefaultAgentStrategy(){
    }
    
    DefaultAgentStrategy::~DefaultAgentStrategy(){
    }
    
    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const {
        
        std::vector<int> Simple, Strategy, Advantage, Food, Empty;
        Position position(1,1);
        for(unsigned i = 0; i < 9; ++i)
            switch(s.array[i])
        {
            case SIMPLE: Simple.push_back(i);
                break;
            case STRATEGIC: Strategy.push_back(i);
                break;
            case ADVANTAGE: Advantage.push_back(i);
                break;
            case FOOD: Food.push_back(i);
                break;
            case EMPTY: Empty.push_back(i);
                break;
        }
        
        if(Advantage.size())
            return Game::reachSurroundings(position, Game::randomPosition(Advantage));
        
        if(Food.size())
            return Game::reachSurroundings(position, Game::randomPosition(Food));
        
        if(Empty.size())
            return Game::reachSurroundings(position, Game::randomPosition(Empty));
        
        if(Simple.size())
            return Game::reachSurroundings(position, Game::randomPosition(Simple));
        
        return STAY;
    }
}