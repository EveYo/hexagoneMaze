#include "HexCell.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

HexCell::HexCell(int Q, int R) {
    q = Q;
    r= R;
    path_dist_from_start =0;
   
    for (int i = 0; i < 6; i++) {
        links[i] = nullptr;
        neighbours[i] = nullptr;
    }
}

void HexCell::link(HexCell* cell) { //can use bi-direction, but maybe slower?


    //position = get_neighbour_side(cell);
    if(this->distance(cell)!= 1){
        return;
    } // If cell is nullptr or is not a neighbour of this cell, do nothing
    
    int position = getPosition(cell);
    links[position] = cell;

    position +=3;
    if (position>=6)
        position-=6;

    cell->links[position] = this; // link both directions
}

int HexCell::get_neighbour_side(const HexCell* cell) const {

    int position = -1; //If didn't find neighbour, return -1
    for(int i = 0; i< 6; i++){
        if(neighbours[i] == cell){
            position = i;
            break;
        }
    }
    return position;
}

int HexCell:: getPosition(const HexCell* cell)const {
    int otherQ = cell->q;
    int otherR = cell->r;

    if(this->q == otherQ){
        if (this->r > otherR){
            return 0;
        } else {
            return 3;
        }
    }else if(this->r == otherR){
        if (this->q > otherQ){
            return 5; 
        } else {
            return 2;
        }
    } else{
        if (this->r > otherR){
            return 1;
        } else {
            return 4;
        }
    }
}

bool HexCell::is_linked(const HexCell* cell) const {
    
    bool isLinked = false;
    if (cell==NULL)
        return false;
    
    for(int i = 0; i< 6; i++){
        if(links[i] == cell){
            isLinked= true;
            break;
        }
    }
    return isLinked;
}

int HexCell::distance(const HexCell* cell) const {
   
    if (cell == NULL){
        return -1;
    }
   int diffQ = this->q - cell->q;
   int diffR = this->r - cell->r;
   
    //calculate 2 points distance in hexboard?
    //Pattern --------------------
    //case 1: if one is 0 = take the one positive
    //case 2: ++ / -- = add them up and take their abs value
    //case 3: +- = take positive
    if ((diffQ>0 && diffR>0) || (diffQ<0 && diffR<0)){
        return abs(diffQ+diffR);
    }

    diffQ = abs(diffQ);
    diffR = abs(diffR);

    return std::max(diffQ, diffR);
}

bool HexCell::is_edge() const {
    for(int i = 0; i< 6; i++){
        if(neighbours[i] == nullptr)
            return true;
    }
    return false;
}