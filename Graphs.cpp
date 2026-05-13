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
void Graph::InsertVertex(int vertex) {
    if (!this->IsFull()) {
        this->vertList.push_back(vertex);
    }
    else {
        cout << "Граф уже заполнен. Невозможно добавить новую вершину " << endl;
        return;
    }
}

//Метод вставляет между вершинами vertex1 и vertex2 ребро весом weight (для взвешенного графа)
void Graph::InsertEdge(int vertex1, int vertex2, int weight) {

    int vertPos1 = GetVertPos(vertex1);
    int vertPos2 = GetVertPos(vertex2);

    if (vertPos1 != -1 && vertPos2 != -1) {
        if (this->adjMatrix[vertPos1][vertPos2] != 0
            && this->adjMatrix[vertPos2][vertPos1] != 0) {
            cout << "Ребро между вершинами уже есть" << endl;
            return;
        }
        else {
            this->adjMatrix[vertPos1][vertPos2] = weight;
            this->adjMatrix[vertPos2][vertPos1] = weight;
        }
    }
    else {
        cout << "Обеих вершин (или одной из них) нет в графе " << endl;
        return;
    }
}

//Этот метод используется для печати матрицы смежности графа
void Graph::Print() {
    if (!this->IsEmpty()) {
        cout << "Матрица смежности графа:\n";
        cout << "   ";
        for(int i = 1, vertListSize = this->vertList.size(); i <= vertListSize; ++i)
        {
            cout << i << "     ";
        }
        cout << endl;
        for (int i = 0, vertListSize = this->vertList.size(); i < vertListSize; ++i) {
            cout << this->vertList[i] << ' ';
            for (int j = 0; j < vertListSize; ++j) {
                cout << ' ' << this->adjMatrix[i][j] << '\t';
            }
            cout << endl;
        }
    }
    else {
        cout << "Граф пуст " << endl;
    }
}
