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


// Queue structure for BFS/topological sorting
struct queue {
    int items[SIZE];
    int front;
    int rear;
};

struct queue* createQueue() {
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(struct queue* q) {
    return q->rear == -1;
}

void enqueue(struct queue* q, int value) {
    if (q->rear == SIZE - 1) {
        printf("Queue full\n");
        return;
    }
    if (q->front == -1)
        q->front = 0;
    q->items[++q->rear] = value;
}

int dequeue(struct queue* q) {
    if (q->front == -1)
        return -1;
    int item = q->items[q->front++];
    if (q->front > q->rear)
        q->front = q->rear = -1;
    return item;
}

// Node for adjacency list
struct Node {
    int vertex;
    struct Node* next;
    int deadline;
};

// Adjacency list
struct adjlist {
    struct Node* head;
};

// Graph
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

    struct Node* newNode = createNode(dest, deadline);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// ==================== CRITICAL PATH (Topological Sort) ====================

void topologicalsort(struct Graph* graph) {
    int v = graph->numVertices;
    int in_degree[MAX_VERTICES] = {0};

    // Compute in-degrees
    for (int i = 0; i < v; i++) {
        struct Node* temp = graph->array[i].head;
        while (temp) {
            in_degree[temp->vertex]++;
            temp = temp->next;
        }
    }

    struct queue* q = createQueue();
    for (int i = 0; i < v; i++) {
        if (in_degree[i] == 0)
            enqueue(q, i);
    }

    int count = 0;
    int order[MAX_VERTICES];

    while (!isEmpty(q)) {
        int u = dequeue(q);
        order[count++] = u;

        struct Node* temp = graph->array[u].head;
        while (temp) {
            in_degree[temp->vertex]--;
            if (in_degree[temp->vertex] == 0)
                enqueue(q, temp->vertex);
            temp = temp->next;
        }
    }

    if (count != v) {
        printf("\nError: Graph contains a cycle (dependency loop detected)\n");
    } else {
        printf("\nValid Task Execution Order:\n");
        for (int i = 0; i < v; i++) {
            printf("%s", graph->vertexNames[order[i]]);
            if (i < v - 1)
                printf(" -> ");
        }
        printf("\n");
    }
    free(q);
}

// ==================== DIJKSTRA’s ALGORITHM ====================

int minDistance(int dist[], int visited[], int vertices) {
    int min = INT_MAX, minIndex = -1;
    for (int v = 0; v < vertices; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], minIndex = v;
    return minIndex;
}

void dijkstra(struct Graph* graph, const char* startName, const char* endName) {
    int V = graph->numVertices;
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};

    int start = getIndex(graph, startName);
    int end = getIndex(graph, endName);

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
            int weight = temp->deadline; // treat deadline as weight
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
            temp = temp->next;
        }
    }

    if (dist[end] == INT_MAX)
        printf("\nNo path exists between %s and %s.\n", startName, endName);
    else
        printf("\nShortest path from %s to %s = %d (based on deadlines)\n", startName, endName, dist[end]);
}

// ==================== GRAPH PRINT ====================

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

// ==================== MENU OPTIONS ====================

void addTasks(struct Graph* graph, int deadlines[]) {
    int tasks;
    printf("Enter number of tasks to add: ");
    scanf("%d", &tasks);
    getchar();

    for (int i = 0; i < tasks; i++) {
        char name[MAX_NAME_LEN];
        printf("Enter name for task %d: ", i + 1);
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = '\0';
        getIndex(graph, name);

        printf("Enter deadline for %s: ", name);
        scanf("%d", &deadlines[getIndex(graph, name)]);
        getchar();
    }
}

void addDependencies(struct Graph* graph, int deadlines[]) {
    char task[MAX_NAME_LEN];
    printf("Enter task name to add dependencies for: ");
    fgets(task, MAX_NAME_LEN, stdin);
    task[strcspn(task, "\n")] = '\0';

    char depTask[MAX_NAME_LEN];
    int n;
    printf("How many dependencies does '%s' have? ", task);
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; i++) {
        printf("Enter name of dependency %d: ", i + 1);
        fgets(depTask, MAX_NAME_LEN, stdin);
        depTask[strcspn(depTask, "\n")] = '\0';
        addEdge(graph, depTask, task, deadlines[getIndex(graph, task)]);
    }
}

// ==================== MAIN ====================

int main() {
    struct Graph* graph = createGraph(MAX_VERTICES);
    int deadlines[MAX_VERTICES] = {0};

    int choice;
    char start[MAX_NAME_LEN], end[MAX_NAME_LEN];

    while (1) {
        printf("\n========== TASK SCHEDULER ==========\n");
        printf("1. Add Tasks\n");
        printf("2. Add Dependencies\n");
        printf("3. Critical Path Detection (Topological Sort)\n");
        printf("4. Shortest Path Between Two Tasks (Dijkstra)\n");
        printf("5. Display Graph\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addTasks(graph, deadlines);
                break;

            case 2:
                addDependencies(graph, deadlines);
                break;

            case 3:
                topologicalsort(graph);
                break;

            case 4:
                printf("Enter start task name: ");
                fgets(start, MAX_NAME_LEN, stdin);
                start[strcspn(start, "\n")] = '\0';
                printf("Enter end task name: ");
                fgets(end, MAX_NAME_LEN, stdin);
                end[strcspn(end, "\n")] = '\0';
                dijkstra(graph, start, end);
                break;

            case 5:
                printGraph(graph);
                break;

            case 6:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
