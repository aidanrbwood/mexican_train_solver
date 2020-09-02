#pragma once
#include "DominoTile.h"
#include <vector>
#include <initializer_list>

class DominoList : public vector<DominoTile*>
{
    public:
        int m_points;

        DominoList(std::initializer_list<DominoTile*> l) : vector<DominoTile*>(l), m_points(0)
        {
            for (DominoTile* d : *this)
            {
                m_points += d->first;
                m_points += d->second;
            } 
        }
        DominoList() : vector<DominoTile*>(), m_points(0) {}
        
        void push_back(DominoTile* d)
        {
            m_points += d->first;
            m_points += d->second;
            ((vector<DominoTile*>*)this)->push_back(d);
        }
};  
