# import numpy as np

# def generate_sudoku(size):
#     # Create an empty square grid of the desired size
#     grid = np.zeros((size, size), dtype=int)
    
#     # Create a list of all possible values for each cell
#     possibilities = np.arange(1, size+1)
    
#     # Function to check if a given number can be placed in a given cell
#     def is_valid(grid, row, col, num):
#         # Check if the number is already in the row or column
#         if num in grid[row] or num in grid[:,col]:
#             return False
        
#         # Check if the number is already in the subgrid
#         subgrid_size = int(np.sqrt(size))
#         start_row = (row // subgrid_size) * subgrid_size
#         start_col = (col // subgrid_size) * subgrid_size
#         for i in range(subgrid_size):
#             for j in range(subgrid_size):
#                 if grid[start_row + i][start_col + j] == num:
#                     return False
#         return True
    
#     # Function to fill in the grid using backtracking
#     def solve(grid):
#         for row in range(size):
#             for col in range(size):
#                 if grid[row][col] == 0:
#                     for num in possibilities:
#                         if is_valid(grid, row, col, num):
#                             grid[row][col] = num
#                             if solve(grid):
#                                 return True
#                             grid[row][col] = 0
#                     return False
#         return True
    
#     # Generate the puzzle
#     solve(grid)
#     return grid
def gen(sz):
    import random
    li = []
    for i in range(sz):
        temp = []
        for j in range(sz):
            temp.append(random.randint(1,n))
        li.append(temp)
    return li

def gen1(sz):
    import numpy as np
    import math
    a = np.zeros((sz,sz))
    n = int(math.sqrt(sz))
    
n = int(input("Enter the size: "))
sudoku = gen1(n)
f = open("in.txt", "w")
for i in range(n):
    for j in range(n):
        f.write(f"{sudoku[i][j]} ")
    f.write("\n")
# f.write("Woops! I have deleted the content!")
f.close()