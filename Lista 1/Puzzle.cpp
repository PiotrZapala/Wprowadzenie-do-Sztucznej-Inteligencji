#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <map>

using namespace std;

const vector<vector<int>> goalState = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
int totalStates1 = 0;
int pathLength1 = 0;
int totalStates2 = 0;
int pathLength2 = 0;

struct PuzzleState {
    vector<vector<int>> state;
    PuzzleState* parent;
    char move;
    int cost;
    int depth;

    PuzzleState(vector<vector<int>> state, PuzzleState* parent = nullptr, char move = ' ', int cost = 0, int depth = 0)
        : state(state), parent(parent), move(move), cost(cost), depth(depth) {}

};

bool ComparePuzzleStates(const PuzzleState* a, const PuzzleState* b) {
    return a->cost > b->cost;
}

vector<vector<int>> generatePuzzle() {
    vector<vector<int>> puzzle(3, vector<int>(3, 0));
    vector<int> numbers {1, 2, 3, 4, 5, 6, 7, 8};
    random_device rd;
    mt19937 g(rd());
    shuffle(numbers.begin(), numbers.end(), g);
    int i = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (row == 2 && col == 2) {
                puzzle[row][col] = 0;
            }
            else {
                puzzle[row][col] = numbers[i];
                i++;
            }
        }
    }
    return puzzle;
}

int countInversions(const vector<int>& arr) {
    int count = 0;
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[i] > arr[j]) {
                count++;
            }
        }
    }
    return count;
}

bool isSolvable(const vector<vector<int>>& puzzle) {
    vector<int> arr;
    for (const auto& row : puzzle) {
        for (const auto& val : row) {
            arr.push_back(val);
        }
    }
    int inversions = countInversions(arr);
    return inversions % 2 == 0;
}

pair<int, int> findEmptyTile(const vector<vector<int>>& puzzle) {
    for (int row = 0; row < puzzle.size(); row++) {
        for (int col = 0; col < puzzle[row].size(); col++) {
            if (puzzle[row][col] == 0) {
                return { row, col };
            }
        }
    }
    return { -1, -1 };
}

vector<pair<char, vector<vector<int>>>> generateMoves(const vector<vector<int>>& puzzle) {
    vector<pair<char, vector<vector<int>>>> moves;
    auto emptyTile = findEmptyTile(puzzle);
    int row = emptyTile.first;
    int col = emptyTile.second;
    if (row > 0) {
        auto newPuzzle = puzzle;
        swap(newPuzzle[row][col], newPuzzle[row - 1][col]);
        moves.push_back(make_pair('u', newPuzzle));
    }
    if (row < 2) {
        auto newPuzzle = puzzle;
        swap(newPuzzle[row][col], newPuzzle[row + 1][col]);
        moves.push_back(make_pair('d', newPuzzle));
    }
    if (col > 0) {
        auto newPuzzle = puzzle;
        swap(newPuzzle[row][col], newPuzzle[row][col - 1]);
        moves.push_back(make_pair('l', newPuzzle));
    }
    if (col < 2) {
        auto newPuzzle = puzzle;
        swap(newPuzzle[row][col], newPuzzle[row][col + 1]);
        moves.push_back(make_pair('r', newPuzzle));
    }
    return moves;
}

int manhattanDistance(const vector<vector<int>>& current) {
    int dist = 0;
    int n = current.size();
    if (current == goalState) {
        return dist;
    }
    vector<int> flattenedGoalState;
    for (const auto& row : goalState) {
        flattenedGoalState.insert(flattenedGoalState.end(), row.begin(), row.end());
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int value = current[i][j];
            if (value != 0) {
                auto it = find(flattenedGoalState.begin(), flattenedGoalState.end(), value);
                int index = distance(flattenedGoalState.begin(), it);
                int row_goal = index / n;
                int col_goal = index % n;
                dist += abs(i - row_goal) + abs(j - col_goal);
            }
        }
    }
    return dist;
}

int misplacedTiles(const vector<vector<int>>& current) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (current[i][j] != goalState[i][j]) {
                count++;
            }
        }
    }
    return count;
}

bool isGoal(const vector<vector<int>>& puzzle) {
    vector<int> flatPuzzle;
    for (int row = 0; row < puzzle.size(); row++) {
        for (int col = 0; col < puzzle[row].size(); col++) {
            flatPuzzle.push_back(puzzle[row][col]);
        }
    }
    return flatPuzzle == vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 0};
}

void aStarManhattan(vector<vector<int>>& initialState) {
    PuzzleState* start = new PuzzleState(initialState);
    start->cost = manhattanDistance(start->state);
    start->depth = 0;
    start->parent = nullptr;
    priority_queue<PuzzleState*, std::vector<PuzzleState*>, decltype(&ComparePuzzleStates)> frontier(&ComparePuzzleStates);
    frontier.push(start);
    vector<vector<vector<int>>> visited;

    while (!frontier.empty()) {
        PuzzleState* current = frontier.top();
        frontier.pop();
        if (isGoal(current->state)) {
            cout << "Znaleziono rozwiązanie!" << endl;
            vector<char> path;
            vector<vector<vector<int>>> states;
            while (current->parent != nullptr) {
                path.push_back(current->move);
                states.push_back(current->state);
                current = current->parent;
            }
            cout << endl;
            reverse(path.begin(), path.end());
            reverse(states.begin(), states.end());
            for (int i = 0; i < states.size(); i++) {
                cout << "Ruch:" << path[i] << "\n";    
                for (const auto& row : states[i]) {
                    for (int num : row) {
                        cout << num << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            cout << "Liczba odwiedzonych stanów: " << visited.size() << "\n";
            cout << "Długość ścieżki: " << path.size() << "\n";
            totalStates1 += visited.size();
            pathLength1 += path.size();
            return;
        }

        if (find(visited.begin(), visited.end(), current->state) == visited.end()) {
            visited.push_back(current->state);
        }   
        auto moves = generateMoves(current->state);
        for(const auto& move : moves) {
            auto nextState = move.second;
            
            if (find(visited.begin(), visited.end(), nextState) != visited.end()) {
                continue;
            }
            
            auto nextCost = current->cost + 1;
            auto nextDepth = current->depth + 1;
            
            PuzzleState* nextPuzzleState = new PuzzleState(nextState, current, move.first, nextCost, nextDepth);
            nextPuzzleState->cost = nextCost + manhattanDistance(nextState);
            
            frontier.push(nextPuzzleState);
        }
    }
    cout << "Nie znaleziono rozwiązania!" << endl;
}

void aStarMisplacedTiles(vector<vector<int>>& initialState) {
    PuzzleState* start = new PuzzleState(initialState);
    start->cost = misplacedTiles(start->state);
    start->depth = 0;
    start->parent = nullptr;
    priority_queue<PuzzleState*, std::vector<PuzzleState*>, decltype(&ComparePuzzleStates)> frontier(&ComparePuzzleStates);
    frontier.push(start);
    vector<vector<vector<int>>> visited;

    while (!frontier.empty()) {
        PuzzleState* current = frontier.top();
        frontier.pop();
        if (isGoal(current->state)) {
            cout << "Znaleziono rozwiązanie!" << endl;
            vector<char> path;
            vector<vector<vector<int>>> states;
            while (current->parent != nullptr) {
                path.push_back(current->move);
                states.push_back(current->state);
                current = current->parent;
            }
            cout << endl;
            reverse(path.begin(), path.end());
            reverse(states.begin(), states.end());
            for (int i = 0; i < states.size(); i++) {
                cout << "Ruch:" << path[i] << "\n";    
                for (const auto& row : states[i]) {
                    for (int num : row) {
                        cout << num << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            cout << "Liczba odwiedzonych stanów: " << visited.size() << "\n";
            cout << "Długość ścieżki: " << path.size() << "\n";
            totalStates2 += visited.size();
            pathLength2 += path.size();
            return;
        }

        if (find(visited.begin(), visited.end(), current->state) == visited.end()) {
            visited.push_back(current->state);
        }   
        auto moves = generateMoves(current->state);
        for(const auto& move : moves) {
            auto nextState = move.second;
            
            if (find(visited.begin(), visited.end(), nextState) != visited.end()) {
                continue;
            }
            
            auto nextCost = current->cost + 1;
            auto nextDepth = current->depth + 1;
            
            PuzzleState* nextPuzzleState = new PuzzleState(nextState, current, move.first, nextCost, nextDepth);
            nextPuzzleState->cost = nextCost + misplacedTiles(nextState);
            
            frontier.push(nextPuzzleState);
        }
    }
    cout << "Nie znaleziono rozwiązania!" << endl;
}

void testAStar() {
    int numberOfTries = 20;
    vector<vector<vector<int>>> puzzles;
    vector<vector<int>> puzz;
    do {
        puzz = generatePuzzle();
        if (isSolvable(puzz)) {
            puzzles.push_back(puzz);
        }
    } while (puzzles.size()<=numberOfTries);
    for (vector<vector<int>> puzzle : puzzles) {
        if (isSolvable(puzzle)) {
            cout << "Tę układankę można rozwiązać!" << endl;
            for (const auto& row : puzzle) {
                for (int num : row) {
                    cout << num << " ";
                }
                cout << endl;
            }
            cout << endl;
            vector<vector<int>> puzzle1 = puzzle;
            cout << "Manhattan!" << endl;
            aStarManhattan(puzzle);
            //cout << "Misplaced Tiles!" << endl;
            //aStarMisplacedTiles(puzzle1);
        } else {
            cout << "Nie można rozwiązać tej układanki!" << endl;
            for (const auto& row : puzzle) {
                for (int num : row) {
                    cout << num << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
    }
    cout << "Średnia długość ścieżki dla heurystyki Manhattan! "<< pathLength1/numberOfTries << "\n";
    cout << "Średnia liczba odwiedzonych stanów dla heurystyki Manhattan! " << totalStates1/numberOfTries << "\n";
    //cout << "Średnia długość ścieżki dla heurystyki Misplaced Tiles! "<< pathLength2/numberOfTries << "\n";
    //cout << "Średnia liczba odwiedzonych stanów dla heurystyki Misplaced Tiles! " << totalStates2/numberOfTries << "\n";
}

int main() {
    testAStar();
    return 0;
}