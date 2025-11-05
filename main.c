#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100
#define MAX_VERTICES 100
#define MAX_NAME_LEN 50

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


struct Node* createNode(int vertexIndex) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = vertexIndex;
    newNode->next = NULL;
    return newNode;
}

void addEdge(struct Graph* graph, const char* srcName, const char* destName) {
    int src = getIndex(graph, srcName);
    int dest = getIndex(graph, destName);

    struct Node* newNode = createNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}


void bfs(struct Graph* graph, const char* startName) {
    int startVertex = getIndex(graph, startName);
    struct queue* q = createQueue();

    graph->visited[startVertex] = 1;
    enqueue(q, startVertex);

    printf("\nBFS starting from %s:\n", startName);

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


int main() {
    struct Graph* graph = createGraph(MAX_VERTICES);
    int tasks; 
    printf("Enter number of tasks\n");
    scanf("%d",&tasks); 

    for(int i = 0; i<tasks;i++){
        char task[MAX_NAME_LEN]; 
        char dependency;  
        printf("Enter task\n"); 
        scanf("%s",task); 
        printf("Do you have a dependency on this task?(y/n)\n");
        scanf(" %c", &dependency); 
        if(dependency=='y'){
            int no_tasks; 
            printf("number of tasks %s is dependent on?\n", task); 
            scanf("%d", &no_tasks); 
            for(int j = 0; j < no_tasks;j++){
                char task_dependent[MAX_NAME_LEN]; 
                printf("Enter task dependent"); 
                scanf("%s", task_dependent); 
                addEdge(graph, task_dependent,task); 
            }
        }
        else{
            continue; 
        }
    }
    

    printGraph(graph);

    
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->visited[i] == 0) {
            bfs(graph, graph->vertexNames[i]);
        }
    }

    return 0;
}
