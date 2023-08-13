#include "HexBoard.h"

#include <cmath>
#include <cstdio>
#include <iostream>

#include "queue.h"
#include "stack.h"

HexBoard::HexBoard(int radius) {
  this->radius = radius;
  start_coord = {0, 0}; // The default start_coord is (0, 0).
  end_coord = {0, 0};
  longest_path_dist = 0;
  // creating new HexCells and inserting them into this->cells by calling
  // createBoard()
  create_board();

  // setting cells to have the appropriate neighbours by calling
  // configure_cells().
  configure_cells();
}

void HexBoard::create_board() {
  // int amountOfCells = 1+3*radius*(radius-1);
  int d = radius - 1;
  for (int q = 0; q <= d; q++) {        // does include the outer
    for (int r = -d; r <= d - q; r++) { // rhs of the grid
      cells[{q, r}] = new HexCell(q, r);
    }
  }

  for (int q = -d; q < 0; q++) {          // does include the outer
    for (int r = -(d + q); r <= d; r++) { // lhs of the grid, exclude 0
      cells[{q, r}] = new HexCell(q, r);
    }
  }
} // create hexcell

void HexBoard::configure_cells() {
  int d = radius - 2;

  for (int q = 0; q <= d; q++) {        // does include the outer
    for (int r = -d; r <= d - q; r++) { // rhs of the grid
      vector<pair<int, int>> myNeighbour = get_neigh_coords({q, r});
      for (int i = 0; i < 6; i++) {
        cells[{q, r}]->neighbours[i] = cells[myNeighbour[i]];
      }
    }
  }

  for (int q = -d; q < 0; q++) {          // doesn't include the outer
    for (int r = -(d + q); r <= d; r++) { // lhs of the grid, exclude 0
      vector<pair<int, int>> myNeighbour = get_neigh_coords({q, r});
      for (int i = 0; i < 6; i++) {
        cells[{q, r}]->neighbours[i] = cells[myNeighbour[i]];
      }
    }
  }

  d++;
  if (d == 0) { // check if radius ==1
    return;
  }

  assign({0, -d}, 0); // top 0
  assign({d, -d}, 1); // 1
  assign({d, 0}, 2);  // 2
  assign({0, d}, 3);  // bottom 3
  assign({-d, d}, 4); // 4
  assign({-d, 0}, 5); // 5

  for (int q = 1; q < d; q++) {
    assign({q, -d}, -2);     // up right -2
    assign({d, -q}, -3);     // right -3
    assign({q, d - q}, -4);  // down right  -4
    assign({-q, d}, -5);     // down left -5
    assign({-d, q}, -6);     // left -6
    assign({-q, q - d}, -7); // up left -7
  }
}

void HexBoard::assign(pair<int, int> coord, int code) {
  vector<pair<int, int>> myNeighbour = get_neigh_coords(coord);
  if (code >= 0) {
    code += 2;
    for (int i = 0; i < 3; i++) {
      cells[coord]->neighbours[(code + i) % 6] =
          cells[myNeighbour[(code + i) % 6]];
    }
  } else {
    code = abs(code);
    for (int i = 0; i < 4; i++) {
      cells[coord]->neighbours[(code + i) % 6] =
          cells[myNeighbour[(code + i) % 6]];
    }
  }
}


void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
  // Create a stack for DFS and mark start_coord as visited
  Stack<pair<int, int>> s;
  map<pair<int, int>, bool> visited;
  s.push(start_coord);
  visited[start_coord] = true;
  cells[start_coord]->path_dist_from_start = 0;
  // Keep track of longest path distance
  longest_path_dist = 0;

  // DFS loop
  while (!s.isEmpty()) {
    // Peek the top of the stack and choose a neighbour to visit
    pair<int, int> curr_coord = s.peek();
    HexCell *curr_cell = cells[curr_coord];

    HexCell *neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob);

    if (neigh_cell != nullptr) {

      // printf("curr_coor: (%d, %d) - %d steps from (0,0)",curr_coord.first,
      // curr_coord.second, curr_cell->path_dist_from_start);

      pair<int, int> neigh_coord = {neigh_cell->q, neigh_cell->r};

      // Link curr_cell with neigh_cell
      curr_cell->link(neigh_cell);

      // Set neigh_coord as visited
      visited[neigh_coord] = true;

      // Update neigh_cell's path distance
      neigh_cell->path_dist_from_start = curr_cell->path_dist_from_start + 1;

      // Update longest path distance if necessary
      longest_path_dist = max(longest_path_dist, neigh_cell->path_dist_from_start);

      HexCell *endCell = cells[end_coord];

      // Update end coordinate if its path distance to start is strictly greater
      if ((neigh_cell->path_dist_from_start >= endCell->path_dist_from_start) && neigh_cell->is_edge()) {
        end_coord = neigh_coord;
      }

      // Push neigh_cell into the stack
      s.push(neigh_coord);

      // printf(" - maxStep: %d; neigh_coor steps: %d\n",
      // longest_path_dist,neigh_cell->path_dist_from_start);

    } else {
      // Pop the stack if chooseNeighbour returns nullptr
      s.pop();
      // printf("goin backward, curr_cell: (%d, %d) - steps: %d\n",
      // curr_cell->q, curr_cell->r, curr_cell->path_dist_from_start);
    }
  }
}

void HexBoard::solve_maze() {
  Stack<pair<int, int>> s;
  Stack<pair<int, int>> helperS;
  map<pair<int, int>, bool> visited;
  s.push(start_coord);
  visited[start_coord] = true;

  // DFS loop
  while (!s.isEmpty()) {

    // Peek the top of the stack and choose a neighbour to visit
    pair<int, int> curr_coord = s.peek();
    HexCell *curr_cell = cells[curr_coord];

    visited[curr_coord] = true;

    if(visited.find(end_coord) != visited.end()){ //curr_cell is the end_cell
      break;
    }

    HexCell *neigh_cell;
    pair<int, int> neigh_coord;
    bool push = false; 

    for (int dir = 0; dir < 6; dir++) {
      neigh_cell = curr_cell->links[dir]; //neigh_cell = neighbour

      if (neigh_cell != nullptr) { // if not null
        neigh_coord = {neigh_cell->q, neigh_cell->r}; 

        if (visited.find(neigh_coord) == visited.end()) { // and is not visited
          s.push(neigh_coord); //push neigh_cell into stack
          push = true;
          break;
        }
      }
    }
    
    if (!push){ // curr_cell has no more unvisted link, dead-end
      s.pop();
    }

    // for (int dir = 0; dir < 6; dir++) {
    //   neigh_cell = curr_cell->links[dir];
    //   if (neigh_cell != nullptr) {
    //     neigh_coord = {neigh_cell->q, neigh_cell->r};
    //     if (visited.find(neigh_coord) != visited.end()) {
    //       neigh_cell = nullptr;
    //       neigh_coord = {0,0};
    //     } else {
    //       break;
    //     }
    //   }
    // }
    // if (neigh_cell != nullptr ) {
    //   visited[neigh_coord] = true;
    //   // Push neigh_cell into the stack
    //   s.push(neigh_coord);
    //   //   printf("movin"); printf("@(%d, %d)\n", neigh_coord.first, neigh_coord.second);

    // } else {
    //   // Pop the stack if chooseNeighbour returns nullptr or if the neighbour is
    //   // already visited
    //   if (visited.find(end_coord) != visited.end()){
    //     // printf("at end coord\n");
    //     break;
    //   }
    //   s.pop();
    // //   printf("wrong path...");
    // //   printf("@(%d, %d)\n", neigh_coord.first, neigh_coord.second);
    // }
  }

  while (!s.isEmpty()) {//{myStack = {data = std::vector of length 0, capacity 16, n1 = 0, n2 = -1}}
    helperS.push(s.pop());
  }

  while (!helperS.isEmpty()) {
    // printf("solution path++\n");
    solution.push_back(helperS.pop());
  }
}