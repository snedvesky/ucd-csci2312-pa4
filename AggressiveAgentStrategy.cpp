//Scott Nedvesky 104976030
//CSC 2312
//PA4
//April 15, 2016
//AggressiveAgentStrategy.cpp
#include <vector>
#include "Game.h"
#include "DefaultAgentStrategy.h"
#include "AggressiveAgentStrategy.h"

namespace Gaming
{
    
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;
    
    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy) {
        __agentEnergy = agentEnergy;
    }
    
    AggressiveAgentStrategy::~AggressiveAgentStrategy() {
    }
    
    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const {

        std::vector<int> Advantages, Food, Emptyspot, simple, strategy;
        Position position(1,1);
        for(int i=0;i < s.array.size();i++)
        {
            switch (s.array[i])
            {
                case SIMPLE: simple.push_back(i);
                    break;
                case STRATEGIC: strategy.push_back(i);
                    break;
                case ADVANTAGE: Advantages.push_back(i);
                    break;
                case EMPTY: Emptyspot.push_back(i);
                    break;
                case FOOD: Food.push_back(i);
                    break;
                    
            }
            
        }
        
        if(simple.size() > 0 && __agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD)
        {
            return Game::reachSurroundings(position,Game::randomPosition(simple));
        }
        else if(Advantages.size() > 0)
        {
            return Game::reachSurroundings(position,Game::randomPosition(Advantages));
        }
        else if(Food.size() > 0)
        {
            return Game::reachSurroundings(position,Game::randomPosition(Food));
        }
        else
            return STAY;
    }
    
    
}