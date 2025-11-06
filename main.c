#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 100
#define MAX_VERTICES 100
#define MAX_NAME_LEN 50

// turns out i was overcomplicating everything, i just needed to do disconnected graphs 
// then search for closest deadline, and bfs for till that 
// by implementing a bfs, i will be taking queues into account as well 
// could have a queue for all the deadlines in order?  


// for deadlines, i will need to find the smallest deadline, and then bfs till that, then again bfs for the next smallest 
// oh dji from start node to end node 

// queue structure for bfs 
struct queue{
    int items[SIZE]; // max number of indices
    int front; 
    int rear; 
};

struct queue* createQueue(){
    struct queue* q = (struct queue*)malloc(sizeof(struct queue)); 
    // init front and rear to -1 
    q->front = -1; 
    q->rear = -1; 
    return q;
}

int isEmpty(struct queue* q) {
    return q->rear == -1;
}

void enqueue(struct queue* q, int value){
    if(q->rear++==SIZE){
        printf("Queue at max size"); 
        return; 
    }
    if(q->front == -1){
        q->front=0; 
    }
    q->items[q->rear] = value; 
}

int dequeue(struct queue* q){
    if(q->front==-1){
        printf("Empty"); 
        return -1; 
    }
    int item = q->items[q->front]; 
    q->front++; 
    if(q->front > q->rear){
        q->front = q->rear = -1; 
    }
    return item; 
}

// adjacency list 
struct Node{
    int vertex; 
    struct Node* next; 
    int deadline; 
}; 

struct adjlist{
    struct Node* head; 
}; 

// graph 
struct Graph {
    int numVertices;
    struct adjlist* array;
    int* visited;
    char vertexNames[MAX_VERTICES][MAX_NAME_LEN];
    char lastAddedName[MAX_NAME_LEN];
};

struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = 0;  
    graph->array = malloc(vertices * sizeof(struct adjlist));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->array[i].head = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}


int getIndex(struct Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertexNames[i], name) == 0)
            return i;
    }
    strcpy(graph->vertexNames[graph->numVertices], name);
    graph->numVertices++;
    return graph->numVertices - 1;
}


struct Node* createNode(int vertexIndex, int deadline) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = vertexIndex;
    newNode->deadline = deadline; 
    newNode->next = NULL;
    return newNode;
}

void addEdge(struct Graph* graph, const char* srcName, const char* destName, int deadline) {
    int src = getIndex(graph, srcName);
    int dest = getIndex(graph, destName);

    struct Node* newNode = createNode(dest,deadline);
    newNode->next = graph->array[src].head;
    
    graph->array[src].head = newNode;
    strcpy(graph->lastAddedName,destName);  
}


/*void bfs(struct Graph* graph, const char* startName) {
    int startVertex = getIndex(graph, startName);
    struct queue* q = createQueue();

    graph->visited[startVertex] = 1;
    enqueue(q, startVertex);

    //printf("\nBFS starting from %s:\n", startName);

    while (!isEmpty(q)) {
        int current = dequeue(q);
        printf("%s\n", graph->vertexNames[current]);

        struct Node* temp = graph->array[current].head;
        while (temp) {
            int adj = temp->vertex;
            if (graph->visited[adj] == 0) {
                graph->visited[adj] = 1;
                enqueue(q, adj);
            }
            temp = temp->next;
        }
    }
}*/


/*int minDistance(int dist[], int visited[], int vertices) {
    int min = INT_MAX, minIndex = -1;
    for (int v = 0; v < vertices; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], minIndex = v;
    return minIndex;
}

void dijkstra(struct Graph* graph, const char* startName) {
    int V = graph->numVertices;
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};

    int start = getIndex(graph, startName);

    // init distances as infinity
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;

    dist[start] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited, V);
        if (u == -1) break;
        visited[u] = 1;

        struct Node* temp = graph->array[u].head;
        while (temp) {
            int v = temp->vertex;
            int weight = temp->deadline;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
            temp = temp->next;
        }
    }

    printf("\nShortest paths from %s:\n", startName);
    for (int i = 0; i < V; i++) {
        printf("%s : ", graph->vertexNames[i]);
        if (dist[i] == INT_MAX)
            printf("unreachable\n");
        else
            printf("%d\n", dist[i]);
    }
}
*/


// why topological sort? it does everything for you, crit path detection, order in which to do it, and suggests path 

void topologicalsort(struct Graph* graph){
    int v = graph->numVertices; 
    int in_degree[MAX_VERTICES] = {0}; // taking into account vertices with 0 in degree 

    // traverse the adjacency list of each vertex to count incoming edges
    for(int i = 0; i< v; i++){
        struct Node* temp = graph->array[i].head; 
        while (temp!=NULL)
        {
            in_degree[temp->vertex]++; 
            temp = temp->next; 
        }
        
    }

    // add all the vertices with 0 in degree 
    struct queue* q = createQueue(); 
    for(int i = 0; i< v; i++){
        if (in_degree[i] == 0) {
            enqueue(q, i);
        }
    }

    int visitedCount = 0;
    int order[MAX_VERTICES]; // sorted array 

    while (!isEmpty(q)) {
        // dequeue a vertex with in-degree 0
        int u = dequeue(q);
        // add it to our sorted list
        order[visitedCount] = u;
        visitedCount++;

        // iterate through dependencies 
        struct Node* temp = graph->array[u].head;
        while (temp) {
            int v = temp->vertex;
            
            // remove edge 
            in_degree[v]--;

            // if now has 0, enqueue 
            if (in_degree[v] == 0) {
                enqueue(q, v);
            }
            temp = temp->next;
        }
    }

    // check for cycle 
    if (visitedCount != v) {
        printf("\nError: The graph contains a cycle! A valid task order is not possible.\n");
        printf("The following tasks are part of a dependency loop:\n");
        for (int i = 0; i < v; i++) {
            if (in_degree[i] > 0) {
                printf("- %s\n", graph->vertexNames[i]);
            }
        }
    } else {
        // print sorted order 
        printf("\nvalid task order \n");
        for (int i = 0; i < v; i++) {
            printf("%s", graph->vertexNames[order[i]]);
            if (i < v - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    free(q);
}


void printGraph(struct Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        struct Node* temp = graph->array[v].head;
        printf("\nAdjacency list of %s\n head", graph->vertexNames[v]);
        while (temp) {
            printf(" -> %s", graph->vertexNames[temp->vertex]);
            temp = temp->next;
        }
        printf("\n");
    }
}

int* sort(int arr[]){

}

int main() {
    struct Graph* graph = createGraph(MAX_VERTICES);
    int arr[MAX_VERTICES];  
    int tasks;
    char allTaskNames[MAX_VERTICES][MAX_NAME_LEN];
    int allTaskDeadlines[MAX_VERTICES];

    printf("Enter number of tasks\n");
    scanf("%d", &tasks);
    getchar();

    for (int i = 0; i < tasks; i++) {
        printf("Enter name for task %d:\n", i + 1);
        fgets(allTaskNames[i], MAX_NAME_LEN, stdin);
        allTaskNames[i][strcspn(allTaskNames[i], "\n")] = '\0';

        getIndex(graph, allTaskNames[i]); // adds each task as a vertex 
       
        printf("Enter deadline for %s:\n", allTaskNames[i]);
        scanf("%d", &allTaskDeadlines[i]);
        getchar();
    } 

       for (int i = 0; i < tasks; i++) {
        char dependency;
        printf("Does task '%s' have dependencies? (y/n)\n", allTaskNames[i]);
        scanf(" %c", &dependency);
        getchar();

        if (dependency == 'y') {
            int no_tasks;
            printf("Number of tasks '%s' is dependent on?\n", allTaskNames[i]);
            scanf("%d", &no_tasks);
            getchar();
            for (int j = 0; j < no_tasks; j++) {
                char task_dependent[MAX_NAME_LEN] = {0};
                printf("Enter task name that '%s' depends on:\n", allTaskNames[i]);
                fgets(task_dependent, MAX_NAME_LEN, stdin);
                task_dependent[strcspn(task_dependent, "\n")] = '\0';
                
                
                addEdge(graph, task_dependent, allTaskNames[i], allTaskDeadlines[i]);
            }
        }

    }
    
    //for(int i = 0; i<sizeof(arr)/sizeof(arr[0]);i++){
       // printf("%d",arr[i]);
    //}

    //printGraph(graph);

    topologicalsort(graph);

    return 0;
}
