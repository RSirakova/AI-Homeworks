#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

// функции свързани с изчисляване на разстояние на Манхатън
int manhatanDistance(int x1, int y1, int x2, int y2)
{
    int distance = abs(x1 - x2) + abs(y1 - y2);
    return distance;
}

void getPositionInDestionationState(int number, int& x, int& y, int** destination, int countRows)
{
    bool found = false;
    for (int i = 0; i < countRows && !found; i++)
    {
        for (int j = 0; j < countRows && !found; ++j)
        {
            if (destination[i][j] == number)
            {
                x = i;
                y = j;
                found = true;
            }
        }
    }
}

int manhatanDistanceForState(int** grid, int countRows, int** destination)
{
    int distance = 0;
    for (int i = 0; i < countRows; ++i)
    {
        for (int j = 0; j < countRows; ++j)
        {

            int number = grid[i][j];
            if (number != 0)
            {
                int destination_pos_x;
                int destination_pos_y;
                getPositionInDestionationState(number, destination_pos_x, destination_pos_y, destination, countRows);

                distance += manhatanDistance(i, j, destination_pos_x, destination_pos_y);
            }
        }
    }
    return distance;
}

// структура съдържаща информация за състоянията
struct Node
{
    //съхраняване на матрицата
    int** grid;

    //предходното състояние
    Node* parent;

    //кординатите на празното квадратче
    int x, y;

    //информация относно размера на матрицата - пазя и двете за удобство
    int N, countRows;

    //чрез какво движение сме стигнали до даденото състояние от предходното 
    //възмножни стойности ' '- за началното,'l'-left,'r'-right,'u'-up,'d'-down
    char movement;

    Node(int** grid_, int x_, int y_, Node* parent_, int N_, char movement_) :x(x_), y(y_), parent(parent_), N(N_), movement(movement_)
    {
        countRows = sqrt(N + 1);
        grid = new int* [countRows];

        if (!grid)
        {
            cout << "Not enough memory!" << endl;
        }

        for (int i = 0; i < countRows; ++i)
        {
            grid[i] = new int[countRows];
        }

        for (int i = 0; i < countRows; ++i)
        {
            for (int j = 0; j < countRows; ++j)
            {
                grid[i][j] = grid_[i][j];
            }
        }
    }

    ~Node()
    {
        for (int i = 0; i < countRows; ++i)
            if (grid[i])
                delete[] grid[i];

        if (grid)
            delete[] grid;
    }
};

void findCordinateOfBlanSpace(int** source, int& x, int& y, int countRows)
{
    bool found = false;
    for (int i = 0; i < countRows && !found; ++i)
    {
        for (int j = 0; j < countRows && !found; ++j)
        {
            if (source[i][j] == 0)
            {
                x = i;
                y = j;
                found = true;
            }
        }
    }
    // за проверка,при некоректно въведена входна матрица.
    if (!found)
    {
        x = -1;
        y = -1;
    }
}

void printGrid(int** grid, int countRows)
{
    for (int i = 0; i < countRows; ++i)
    {
        for (int j = 0; j < countRows; ++j)
        {
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
}

int isInLimits(int x, int y, int countRows)
{
    return (x >= 0 && x < countRows&& y >= 0 && y < countRows);
}
Node* newNode(int** grid, int x, int y, int newX, int newY, Node* parent, int N, char movement)
{
    Node* node = new Node(grid, newX, newY, parent, N, movement);
    swap(node->grid[x][y], node->grid[newX][newY]);

    return node;
}
bool compare(int** m1, int** m2, int countRows)
{
    for (int i = 0; i < countRows; ++i)
    {
        for (int j = 0; j < countRows; ++j)
        {
            if (m1[i][j] != m2[i][j])
                return false;
        }
    }
    return true;
}
const int FOUND = -1;
const int MAX = INT_MAX;

vector<Node*> createNewStare(Node* parent, int x, int y, int countRows, int N)
{
    vector<Node*> v;
    Node* n;
    if (isInLimits(x - 1, y, countRows))
    {
        n = newNode(parent->grid, x, y, x - 1, y, parent, N, 'd');//down
        v.push_back(n);

    }
    if (isInLimits(x + 1, y, countRows))
    {
        n = newNode(parent->grid, x, y, x + 1, y, parent, N, 'u');//up
        v.push_back(n);

    }
    if (isInLimits(x, y + 1, countRows))
    {
        n = newNode(parent->grid, x, y, x, y + 1, parent, N, 'l');//left
        v.push_back(n);

    }
    if (isInLimits(x, y - 1, countRows))
    {
        n = newNode(parent->grid, x, y, x, y - 1, parent, N, 'r');//right
        v.push_back(n);

    }
    return v;
}
string getMovement(char movement)
{
    switch (movement)
    {
    case 'l': return "left";
    case 'r': return "right";
    case 'd': return "down";
    case 'u': return "up";
    default: return " ";
    }
}

// функции реализиращи IDA*
int search(Node* node, int g, int threshold, int** destination, vector<string>& path)
{
    int h = manhatanDistanceForState(node->grid, node->countRows, destination);
    int f = g + h;

    if (f > threshold)
    {
        return f;
    }
    else
    {
        //  cout << "--------------------------------------" << endl;
        //  cout << " g= " << g << " h= " << h << " f= " << f << endl;
         // printGrid(node->grid, node->countRows);
        if (getMovement(node->movement) != " ")
            path.push_back(getMovement(node->movement));
        // cout << "--------------------------------------" << endl;
    }
    if (compare(node->grid, destination, node->countRows))
        return FOUND;
    int min = MAX;
    Node* tempNode;


    vector<Node*> v = createNewStare(node, node->x, node->y, node->countRows, node->N);

    for (int i = 0; i < v.size(); ++i)
    {
        tempNode = v[i];
        int temp = search(tempNode, g + 1, threshold, destination, path);
        if (temp == FOUND)
            return FOUND;
        if (temp < min)
            min = temp;
    }
    return min;

}
void IDAstar(Node* start, int** destination, vector<string>& path)
{
    int threshold = manhatanDistanceForState(start->grid, start->countRows, destination);
    bool stop = false;

    while (!stop)
    {

        // cout << "threshold: " << threshold << endl;
        int temp = search(start, 0, threshold, destination, path);
        if (temp == FOUND)
        {
            cout << "FOUND" << endl;
            stop = true;
        }
        if (temp > MAX)
        {
            cout << "NO SOLUTION!" << endl;
            stop = true;
        }
        threshold = temp;
    }
}
int getInversionCount(vector<int> arr)
{
    int inv_count = 0;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j] && arr[j] != 0)
            {
                inv_count++;
            }

    return inv_count;
}

// функция проверяваща дали дадено начално състояние има решение
bool isSolvable(Node* sourceState)
{
    int size = (sourceState->N) + 1;
    vector<int> row_major_order;
    int row_index_blank_square = sourceState->x;

    for (int i = 0; i < (sourceState->countRows); ++i)
    {
        for (int j = 0; j < (sourceState->countRows); ++j)
        {
            row_major_order.push_back(sourceState->grid[i][j]);
        }
    }

    int invCount = getInversionCount(row_major_order);

    if ((sourceState->countRows) % 2 == 0)
    {
        if (((row_index_blank_square + invCount) % 2) != 0)
            return true;
        else
            return false;
    }
    else
    {
        if (invCount % 2 == 0)
            return true;
        else
            return false;
    }

}

void N_Puzzle(Node* sourceState, int** destinationState, vector<string>& path)
{
    if (isSolvable(sourceState))
    {
        IDAstar(sourceState, destinationState, path);
    }
    else
    {
        cout << "NO SOLUTION FOUND!" << endl;
    }
}

// Функция проверяваща дали дадено число е точен квадрат (1,4),9,16,25..
// Сложност: O(log(N))
bool isPerfectSquare(int N)
{
    long long left = 1, right = N;

    while (left <= right)
    {
        long long mid = (left + right) / 2;

        if (mid * mid == N)
        {
            return true;
        }

        if (mid * mid < N)
        {
            left = mid + 1;
        }

        else
        {
            right = mid - 1;
        }
    }
    return false;
}

// функции за прочитане на аргументите от входа - с валидация
void readSizeOfGrid(int& N)
{
    bool correctN;
    do
    {
        cout << "Insert size of grid (N>=8) N=";
        cin >> N;
        correctN = (N >= 8) && isPerfectSquare(N + 1);
    } while (!correctN);
}

void readIndexOfBlankSquareInSolution(int& x, int& y, int countRowOfGrid)
{
    do
    {
        cout << "Insert coordinate of blank square in solution: ";
        cout << "x= ";
        cin >> x;
        if (x == -1)
        {
            // ако се въведе -1 по подразбиране 'празната клетка' е в най-долния десен ъгъл на матрицата
            x = countRowOfGrid - 1;
            y = countRowOfGrid - 1;
            break;
        }
        cout << "y=";
        cin >> y;
    } while ((x != 0 && y != 0));


}

void  insertSourceArray(int** source, int countRows, int N)
{
    //Масивът visited служи, за да можем да идентифицираме повтарящи се елементи.
    bool* visited = new bool[N + 1];
    for (int i = 0; i < N + 1; ++i)
    {
        visited[i] = false;
    }

    cout << "Insert source array:" << endl;

    for (int i = 0; i < countRows; ++i)
    {
        for (int j = 0; j < countRows; ++j)
        {

            cout << "source[" << i << "][" << j << "]=";
            int value;
            cin >> value;

            if (visited[value] == false && value >= 0 && value <= N)
            {
                visited[value] = true;
            }
            else
            {
                while ((visited[value] != 0) || value<0 || value>N)
                {
                    cout << "Matrix must have only unique value from 0 to " << N << " .Please, insert new value:" << " source[" << i << "][" << j << "]=";
                    cin >> value;
                }
            }
            source[i][j] = value;
        }
    }

    delete[] visited;

}




void mainTask()
{
    int N;
    readSizeOfGrid(N);

    int countRows = (int)(sqrt(N + 1));
    int x_cordinate_of_blank_square_in_solution, y_cordinate_of_blank_square_in_solution;
    readIndexOfBlankSquareInSolution(x_cordinate_of_blank_square_in_solution, y_cordinate_of_blank_square_in_solution, countRows);

    int** source = new int* [countRows];

    for (int i = 0; i < countRows; ++i)
    {
        source[i] = new int[countRows];
    }

    int countBlankSquare = 0;

    insertSourceArray(source, countRows, N);

    int** destination = new int* [countRows];

    for (int i = 0; i < countRows; ++i)
    {
        destination[i] = new int[countRows];
    }

    int number = 1;
    for (int i = 0; i < countRows; ++i)
    {
        for (int j = 0; j < countRows; ++j)
        {
            if (i == 0 && j == 0 && x_cordinate_of_blank_square_in_solution == 0 && y_cordinate_of_blank_square_in_solution == 0)
            {
                destination[i][j] = 0;
            }
            else if (i == (countRows - 1) && j == (countRows - 1) && x_cordinate_of_blank_square_in_solution == (countRows - 1) && y_cordinate_of_blank_square_in_solution == (countRows - 1))
            {
                destination[i][j] = 0;
            }
            else
            {
                destination[i][j] = number;
                number++;
            }
        }
    }

    int x_cordinate_in_source, y_cordinate_in_source;
    findCordinateOfBlanSpace(source, x_cordinate_in_source, y_cordinate_in_source, countRows);

    Node parent(source, x_cordinate_in_source, y_cordinate_in_source, nullptr, N, ' ');
    Node* p = &parent;

    vector<string> path;

    N_Puzzle(p, destination, path);

    cout << "Length of sequence: " << path.size() << endl;

    for (int i = 0; i < path.size(); ++i)
    {
        cout << path[i] << endl;
    }

    for (int i = 0; i < countRows; ++i)
        if (source[i])
            delete[] source[i];

    if (source)
        delete[] source;

    for (int i = 0; i < countRows; ++i)
        if (destination[i])
            delete[] destination[i];

    if (destination)
        delete[] destination;
}


int main()
{
    mainTask();
}
