#include "Graphs.h"

//Конструктор без параметров
Graph::Graph()
{
    for(int i = 0; i < maxSize; i++)
    {
        for(int j = 0; j < maxSize; j++)
        {
            adjMatrix[i][j] = 0;
        }
    }
}
//Конструктор с параметром
Graph::Graph(int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j <size; j++)
        {
            adjMatrix[i][j] = 0;
        }
    }
}
//Деструктор
Graph::~Graph(){}

//Метод, находит позицию вершины в векторе вершин
int Graph::GetVertPos(int vertex)
{
    for(size_t i = 0; i < vertList.size(); ++i)
    {
        if(this->vertList[i] == vertex)
            return i;
    }
    return -1;
}


//Метода, проверяет, пуст ли граф, то есть в нём нет ни одной вершины
bool Graph::IsEmpty()
{
    if(this->vertList.size())
        return false;
    else
        return true;
}


//Метод проверяет, достигло ли количество вершин в графе максимально возможного предела
bool Graph::IsFull()
{
    return(vertList.size()==maxSize);
}


//Метод возвращает количество вершин в графе, то есть размер вектора вершин
int Graph::GetAmountVerts()
{
    return this->vertList.size();
}


//Метод возвращает количество ребер в графе
int Graph::GetAmountEdges()
{
    int amount = 0;
    if(!this->IsEmpty())
    {
        for(int i = 0, vertListSize = this->vertList.size(); i < vertListSize; ++i)
        {
            for(int j = 0; j < vertListSize; ++j)
            {
                if(this->adjMatrix[i][j] == this->adjMatrix[j][i] && this->adjMatrix[i][j] != 0)
                {
                    amount+=1;
                }
            }
        }
        return (amount/2);
    }
    else return 0;
}

//Метод возвращает вес ребра
int Graph::GetWeight(int vertex1, int vertex2)
{
    if(!this->IsEmpty())
    {
        int vertPos1 = GetVertPos(vertex1);
        int vertPos2 = GetVertPos(vertex2);
        return adjMatrix[vertPos1][vertPos2];
    }
    return 0;
}

//Метод возвращает список соседей
vector<int> Graph::GetNbrs(int vertex)
{
    vector<int> nbrsList;
    int vertPos = this->GetVertPos(vertex);
    if(vertPos != (-1))
    {
        for(int i = 0, vertListSize = this->vertList.size(); i < vertListSize; i++)
        {
            if(this->adjMatrix[vertPos][i] != 0 && this->adjMatrix[i][vertPos] != 0)
            {
                nbrsList.push_back(this->vertList[i]);
            }
        }
    }
    return nbrsList;
}

//Метод служит для добавления вершины
bool Graph::InsertVertex(int vertex) {
    if (IsFull()) {
        return false;
    }
    vertList.push_back(vertex);
    return true;
}

//Метод вставляет между вершинами vertex1 и vertex2 ребро весом weight (для взвешенного графа)
bool Graph::InsertEdge(int vertex1, int vertex2, int weight) {

    int vertPos1 = GetVertPos(vertex1);
    int vertPos2 = GetVertPos(vertex2);

    if (vertPos1 != -1 && vertPos2 != -1) {
        if (this->adjMatrix[vertPos1][vertPos2] != 0
            && this->adjMatrix[vertPos2][vertPos1] != 0) {
            return false;
        }
        else {
            this->adjMatrix[vertPos1][vertPos2] = weight;
            this->adjMatrix[vertPos2][vertPos1] = weight;
        }
    }
    else {
        return false;
    }
}

// Метод, который возвращает строку в виде матрицы смежности
string Graph::PrintToString() const {

    ostringstream oss;

    if (this->IsEmpty()) {
        return "Граф пуст";
    }

    // Заголовки
    for(int i = 0; i < this->vertList.size(); ++i) {
        oss << "    " << this->vertList[i];
    }
    oss << "\n";

    // Строки матрицы
    for (int i = 0; i < this->vertList.size(); ++i) {
        oss << this->vertList[i] << "  ";
        for (int j = 0; j < this->vertList.size(); ++j) {
            oss << " " << this->adjMatrix[i][j] << "\t";
        }
        oss << "\n";
    }

    return oss.str();
}
