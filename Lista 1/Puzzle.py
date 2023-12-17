from queue import PriorityQueue
import math
import random

class PuzzleState:
    def __init__(self, state, parent=None, move=None, cost=0, depth=0):
        self.state = state
        self.parent = parent
        self.move = move
        self.cost = cost
        self.depth = depth
        
    def __eq__(self, other):
        return self.state == other.state
    
    def __lt__(self, other):
        return self.cost < other.cost
    
def generatePuzzle():
    puzzle = [[0 for _ in range(3)] for _ in range(3)]
    numbers = [1, 2, 3, 4, 5, 6, 7, 8]
    random.shuffle(numbers)
    i = 0
    for row in range(3):
        for col in range(3):
            if row == 2 and col == 2:
                puzzle[row][col] = 0
            else:
                puzzle[row][col] = numbers[i]
                i += 1
    return puzzle

def countInversions(arr):
    count = 0
    for i in range(len(arr)):
        for j in range(i+1, len(arr)):
            if arr[i] > arr[j]:
                count += 1
    return count

def isSolvable(puzzle):
    arr = [val for sublist in puzzle for val in sublist]
    inversions = countInversions(arr)
    return inversions % 2 == 0

def findEmptyTile(puzzle):
    for row in range(len(puzzle)):
        for col in range(len(puzzle[row])):
            if puzzle[row][col] == 0:
                return (row, col)
    return None

def generateMoves(puzzle):
    moves = []
    emptyTile = findEmptyTile(puzzle)
    if emptyTile:
        row, col = emptyTile

        if row > 0:
            newPuzzle = [row[:] for row in puzzle]
            newPuzzle[row][col] = newPuzzle[row - 1][col]
            newPuzzle[row - 1][col] = 0
            moves.append(('up', newPuzzle))
        if row < 2:
            newPuzzle = [row[:] for row in puzzle]
            newPuzzle[row][col] = newPuzzle[row + 1][col]
            newPuzzle[row + 1][col] = 0
            moves.append(('down', newPuzzle))
        if col > 0:
            newPuzzle = [row[:] for row in puzzle]
            newPuzzle[row][col] = newPuzzle[row][col - 1]
            newPuzzle[row][col - 1] = 0
            moves.append(('left', newPuzzle))
        if col < 2:
            newPuzzle = [row[:] for row in puzzle]
            newPuzzle[row][col] = newPuzzle[row][col + 1]
            newPuzzle[row][col + 1] = 0
            moves.append(('right', newPuzzle))
    return moves

def estimateManhattanDistance(state1, state2):
    distance = 0
    for row in range(len(state1)):
        for col in range(len(state1[row])):
            tile = state1[row][col]
            if tile == 0:
                continue
            goalRow, goalCol = findEmptyTile(state2)
            if goalRow is not None and goalCol is not None:
                distance += abs(row - goalRow) + abs(col - goalCol)
    return distance

def isGoal(puzzle):
    flatPuzzle = [item for sublist in puzzle for item in sublist]
    return flatPuzzle == [1, 2, 3, 4, 5, 6, 7, 8, 0]

def aStar(initialState, goalState):

    start = PuzzleState(initialState)
    start.cost = estimateManhattanDistance(goalState, start.state)
    start.depth = 0
    start.parent = None
    
    frontier = PriorityQueue()
    frontier.put(start)
    visited = []
    
    while not frontier.empty():
        currentState = frontier.get()
        
        if isGoal(currentState.state):
            path = []
            states = []
            while currentState.parent is not None:
                path.append(currentState.move)
                states.append(currentState.state)
                currentState = currentState.parent
            path.reverse()
            states.reverse()
            return (path,states)
        
        if currentState.state not in visited:
            visited.append(currentState.state)
        
        for move, nextState in generateMoves(currentState.state):

            if nextState in visited:
                continue
            
            nextCost = currentState.cost + 1
            nextDepth = currentState.depth + 1
            
            nextPuzzleState = PuzzleState(nextState, currentState, move, nextCost, nextDepth)
            nextPuzzleState.cost = nextCost + estimateManhattanDistance(goalState, nextState)
            
            frontier.put(nextPuzzleState)
    
    return (None,None)

def testAStar():
    puzzles = []
    goalState = [[1, 2, 3], [4, 5, 6], [7, 8, 0]]
    for _ in range(3):
        puzzle = generatePuzzle()
        puzzles.append(puzzle)

    for puzzle in puzzles:
        if isSolvable(puzzle):
            paths, states = aStar(puzzle, goalState)
            if paths is not None:
                print("Znaleziono rozwiązanie dla puzzla:")
                printPuzzle(puzzle)
                print("Stany:")
                for i in range(len(paths)):
                    print(paths[i])
                    printPuzzle(states[i])
        else:
            print("Nie można rozwiązać tego puzzla:")
            printPuzzle(puzzle)

def printPuzzle(puzzle):
    for row in puzzle:
        print(row)
    print()

testAStar()