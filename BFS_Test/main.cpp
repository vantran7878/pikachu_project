#include <iostream>
#include "object.h"
#include <cstring>

const int MAX = 100;
const int x_rotation[4] = {-1, 0, 1, 0}; //Left, Down, Right, Top
const int y_rotation[4] = {0, 1, 0, -1};

using namespace std;

int** init2DMat(int row, int col)
{
    int** mat = new int* [row + 2];
    for (int i = 0; i <= row + 1; ++i)
    mat[i]= new int[col + 2];
    return mat;
}

void readfile(string filename, int** &mat , int &row, int &col)
{
    ifstream fin;
    fin.open(filename);

    fin >> row >> col;
    fin.ignore();

    mat = init2DMat(row, col);

    for (int i = 1; i <= row; ++i)
    {
        for (int j = 1; j <= col; ++j)
            fin >> mat[i][j];
        fin.ignore();
    }

    fin.close();
}


void printMat(int** mat, int row, int col)
{
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= col; j++)
            std::cout << mat[i][j] << ' ';
        std::cout << '\n';
    }
}


void printMat_Block(Block mat[][MAX], int row, int col)
{
    for (int i = 0; i <= row; i++)
    {
        for (int j = 0; j <= col; j++)
            std::cout << mat[i][j].Y  << ',' << mat[i][j].X << ' ';
        std::cout << '\n';
    }
}

void make_free_matrix(Block free[][MAX], int** mat, int row, int col)
{
    for (int i = 0; i <= row + 1; i++)
    {
        for (int j = 0; j <= col + 1; j++)
        {
            if (mat[i][j] == 0)
            {
                free[i][j].X = -1; //if mat has value = 0, it's blank , use for special map with blank
                free[i][j].Y = -1;
            }
            free[i][j].X = -2; //-2 is blocked
            free[i][j].Y = -2;
        }
    }

    for (int i = 0; i <= row + 1; ++i)
    {
        free[i][0].X = -1;
        free[i][0].Y = -1;
        free[i][col + 1].X = -1;
        free[i][col + 1].Y = -1;
    }

    for (int i = 0; i <= col + 1; ++i)
    {
        free[0][i].X = -1;
        free[0][i].Y = -1;
        free[row + 1][i].X = -1;
        free[row + 1][i].Y = -1;
    }

}

bool check_index_valid(int x, int y, int width, int height)
{
    if (x < 0 || y < 0) return false;
    else if (x >= width || y >= height ) return false;
    else return true;
}

void fill_minus_one(Block free[][MAX], int row, int col)
{
    Block m2, m1;
    m2.X = -2;
    m2.Y = -2;
    m1.Y = -1;
    m1.X = -1;

    for (int i = 0; i <= row; i++)
    {
        for (int j = 0; j <= col; j++)
            if (free[i][j] != m2 && free[i][j] != m1)
                free[i][j] = m1;
    }
}

int trace_corner(Block free[][MAX], Block finish, Block start) //traversal back, from finish to start
{
    int corner = 0;
    Block current = finish, free_current, free_prev; //current block, the free of the current block and
    //the free of the previous block
    free_current = free[current.Y][current.X];

    if (free_current == start) return 0; //the nearby block is the start block
    else
    {
        free_prev = free[free_current.Y][free_current.X];
        while (free_prev != start)
        {
            if (current.X != free_prev.X && current.Y != free_prev.Y) corner++;
            if (corner > 2) return corner;

            current = free_current;
            free_current = free_prev;
            free_prev = free[free_prev.Y][free_prev.X];
            //drawing in hear
        }
        if (current.X != free_prev.X && current.Y != free_prev.Y) corner++;
        return corner;
    }
}

bool BFS(int row, int col, Block b_first, Block b_second, Block free[][MAX])
{
    queue_path trace;
    trace.enqueue(b_first);

    free[b_first.Y][b_first.X].X = b_first.X;
    free[b_first.Y][b_first.X].Y = b_first.Y;

    free[b_second.Y][b_second.X].X = -1;
    free[b_second.Y][b_second.X].Y = -1;

    //printMat_Block(free, row + 1, col + 1);

    do
    {
        Block u = trace.dequeue();
        for (int i = 0 ; i < 4; ++i)
        {
            int X1 = u.X + x_rotation[i];
            int Y1 = u.Y + y_rotation[i];
            if (check_index_valid(X1, Y1, col + 2, row + 2))
            {
                if (free[Y1][X1].X == -1 && free[Y1][X1].Y == -1)
                {
                    Block add;
                    add.Y = Y1; add.X = X1;
                    trace.enqueue(add);
                    free[Y1][X1].X = u.X;
                    free[Y1][X1].Y = u.Y;
                    if (trace.path[trace.head] == b_second) break;
                }
            }
        }
    }
    while (!trace.isEmpty() && (trace.path[trace.head] != b_second));

    //printMat_Block(free, row + 1, col + 1);

    int corner = trace_corner(free, trace.path[trace.head], b_first);

    if (corner >= 3 || trace.isEmpty()) return false;
    else if (trace.path[trace.head] == b_second) return true;

}

bool check_finished(int** mat, int row, int col)
{
    for (int i = 1; i <= row; ++i)
    {
        for (int j = 1; j <= col; ++j)
            if (mat[i][j] != 0) return false;
    }
    return true;
}

void game_play(int** mat, int row, int col, Block b_first, Block b_second)
{
    Block free[MAX][MAX]; //free is the matrix for checking the block in matrix is visited or not, also
    //it could be use to store the father block which it being visited.

    readfile("map.txt", mat, row, col);
    printMat(mat, row, col);
    make_free_matrix(free, mat, row, col);

    while(!check_finished(mat, row, col))
    {
        std::cout << "enter first block (y then x): ";
        std::cin >> b_first.Y >> b_first.X;
        std::cout << "enter second block (y then x): ";
        std::cin >> b_second.Y >> b_second.X;

        while (!BFS(row, col, b_first, b_second, free) || !(mat[b_first.Y][b_first.X] == mat[b_second.Y][b_second.X]))
        { //re-enter if false
            free[b_first.Y][b_first.X].X = -2;
            free[b_first.Y][b_first.X].Y = -2;
            free[b_second.Y][b_second.X].X = -2;
            free[b_second.Y][b_second.X].X = -2;
            std::cout << "cannot make move!\nRe-enter 2 block\n";
            std::cout << "enter first block (y then x): ";
            std::cin >> b_first.Y >> b_first.X;
            std::cout << "enter second block (y then x): ";
            std::cin >> b_second.Y >> b_second.X;
        }

        mat[b_first.Y][b_first.X] = 0;
        mat[b_second.Y][b_second.X] = 0;

        fill_minus_one(free, row + 1, col + 1); //because the free matrix store the father block, we need to
        // re-value it to -1 (the blank value) to re-play the game

        printMat(mat, row, col);
        //printMat_Block(free, row + 1, col + 1);
        // ve duong di
    }
    cout << "You won";
}


char* int_to_char(int num)
{
    char* num_char = new char[10];
    int i = 0;
    while (num != 0)
    {
        num_char[i++] = (char(num % 10) + '0');
        num /= 10;
    }
    num_char[i] = '\0';
    for (int j = 0; j < i / 2; j++)
    {
        char temp = num_char[j];
        num_char[j] = num_char[i - j - 1];
        num_char[i - j - 1] = temp;
    }

    return num_char;
}

int main()
{
    /*int** mat = nullptr;
    int row, col;
    Block b_first, b_second;

    game_play(mat, row, col, b_first, b_second);
    */
    int num;
    std::cin >> num;
    //char* temp = int_to_char(num);
    char* p_temp = int_to_char(num);

    std::cout << p_temp;
    return 0;
}
