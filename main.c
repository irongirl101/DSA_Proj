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


// ===================== QUEUE IMPLEMENTATION =====================

// structure for queue 
struct queue {
    int items[SIZE];   // Array to hold queue elements
    int front;         // Index of the front element
    int rear;          // Index of the last element
};

// Function to create and initialize a new queue
struct queue* createQueue() {
    struct queue* q = (struct queue*)malloc(sizeof(struct queue)); // Allocate memory for queue
    q->front = -1; // Initialize front index
    q->rear = -1;  // Initialize rear index
    return q;       // Return pointer to queue
}

// Function to check if the queue is empty
int isEmpty(struct queue* q) {
    return q->rear == -1; // Returns 1 if empty, else 0
}

// Function to add an element to the queue
void enqueue(struct queue* q, int value) {
    if (q->rear == SIZE - 1) { // Check for overflow
        printf("Queue full\n");
        return;
    }
    if (q->front == -1)        // If queue is empty, initialize front
        q->front = 0;
    q->items[++q->rear] = value; // Increment rear and insert value
}

// Function to remove and return the front element from queue
int dequeue(struct queue* q) {
    if (q->front == -1) // If queue is empty
        return -1;
    int item = q->items[q->front++]; // Retrieve and increment front
    if (q->front > q->rear)          // Reset if queue becomes empty
        q->front = q->rear = -1;
    return item;                     // Return dequeued item
}

// ===================== GRAPH STRUCTURES =====================

// Node structure representing an edge in the adjacency list
struct Node {
    int vertex;            // Destination vertex index
    struct Node* next;     // Pointer to next node (linked list)
    int deadline;          // Deadline (acts as weight for Dijkstra)
};

// Adjacency list for a vertex
struct adjlist {
    struct Node* head;     // Pointer to head node of the list
};

// Graph structure representing the whole graph
struct Graph {
    int numVertices;                            // Number of vertices currently in graph
    struct adjlist* array;                      // Array of adjacency lists
    int* visited;                               // Visited array for traversal
    char vertexNames[MAX_VERTICES][MAX_NAME_LEN]; // Stores task names for each vertex
};

// Function to create a graph with given number of vertices
struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph)); // Allocate memory for graph
    graph->numVertices = 0;                             // Start with 0 vertices
    graph->array = malloc(vertices * sizeof(struct adjlist)); // Allocate adjacency lists
    graph->visited = malloc(vertices * sizeof(int));           // Allocate visited array

    // Initialize each adjacency list and visited flag
    for (int i = 0; i < vertices; i++) {
        graph->array[i].head = NULL;
        graph->visited[i] = 0;
    }
    return graph; // Return created graph
}

// Function to find the index of a vertex by its name or create a new one if not found
int getIndex(struct Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertexNames[i], name) == 0) // Compare names
            return i; // Return existing index if found
    }
    // If name not found, add new vertex
    strcpy(graph->vertexNames[graph->numVertices], name);
    graph->numVertices++;
    return graph->numVertices - 1; // Return new vertex index
}

// Function to create a new node for adjacency list
struct Node* createNode(int vertexIndex, int deadline) {
    struct Node* newNode = malloc(sizeof(struct Node)); // Allocate memory for node
    newNode->vertex = vertexIndex;                      // Set vertex index
    newNode->deadline = deadline;                       // Set deadline (edge weight)
    newNode->next = NULL;                               // Initialize next pointer
    return newNode;                                     // Return node
}

// Function to add a directed edge from src to dest with a deadline (weight)
void addEdge(struct Graph* graph, const char* srcName, const char* destName, int deadline) {
    int src = getIndex(graph, srcName);  // Get source vertex index
    int dest = getIndex(graph, destName); // Get destination vertex index

    struct Node* newNode = createNode(dest, deadline); // Create edge node
    newNode->next = graph->array[src].head;            // Insert at head of list
    graph->array[src].head = newNode;                  // Update head pointer
}

// ==================== CRITICAL PATH (Topological Sort) ====================

// Function to perform topological sorting (for dependency ordering)
void topologicalsort(struct Graph* graph) {
    int v = graph->numVertices;              // Total vertices
    int in_degree[MAX_VERTICES] = {0};       // Array to store in-degrees

    // Compute in-degrees of all vertices
    for (int i = 0; i < v; i++) {
        struct Node* temp = graph->array[i].head;
        while (temp) {
            in_degree[temp->vertex]++; // Increment in-degree for destination vertex
            temp = temp->next;
        }
    }

    struct queue* q = createQueue(); // Create queue for vertices with in-degree 0
    for (int i = 0; i < v; i++) {
        if (in_degree[i] == 0)
            enqueue(q, i); // Add vertices with no dependencies
    }

    int count = 0;                    // Counter for visited vertices
    int order[MAX_VERTICES];          // Array to store topological order

    // Process vertices in queue
    while (!isEmpty(q)) {
        int u = dequeue(q);           // Get vertex with in-degree 0
        order[count++] = u;           // Add it to order

        struct Node* temp = graph->array[u].head;
        while (temp) {
            in_degree[temp->vertex]--; // Decrease in-degree of neighbors
            if (in_degree[temp->vertex] == 0)
                enqueue(q, temp->vertex); // Enqueue if in-degree becomes 0
            temp = temp->next;
        }
    }

    // Check for cycle (if count != total vertices)
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
    free(q); // Free queue memory
    }

// ==================== DIJKSTRA’S ALGORITHM ====================

// Function to find vertex with minimum distance value
int minDistance(int dist[], int visited[], int vertices) {
    int min = INT_MAX, minIndex = -1;
    for (int v = 0; v < vertices; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], minIndex = v;
    return minIndex;
}

// Function to perform Dijkstra's algorithm (shortest path based on deadlines)
void dijkstra(struct Graph* graph, const char* startName, const char* endName) {
    int V = graph->numVertices;              // Total number of vertices
    int dist[MAX_VERTICES];                  // Distance array
    int visited[MAX_VERTICES] = {0};         // Visited flags

    int start = getIndex(graph, startName);  // Get index of start node
    int end = getIndex(graph, endName);      // Get index of end node

    // Initialize all distances as infinite
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;

    dist[start] = 0; // Distance to start node is 0

    // Find shortest paths for all vertices
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited, V); // Pick vertex with minimum distance
        if (u == -1) break;                   // Break if unreachable
        visited[u] = 1;                       // Mark vertex as processed

        struct Node* temp = graph->array[u].head;
        while (temp) {
            int v = temp->vertex;
            int weight = temp->deadline; // Treat deadline as edge weight
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight; // Update distance if shorter path found
            temp = temp->next;
        }
    }

    // Display result
    if (dist[end] == INT_MAX)
        printf("\nNo path exists between %s and %s.\n", startName, endName);
    else
        printf("\nShortest path from %s to %s = %d (based on deadlines)\n", startName, endName, dist[end]);
}

// ==================== GRAPH PRINT FUNCTION ====================

// Function to print adjacency list representation of the graph
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

// ==================== MENU FUNCTIONS ====================

// Function to add multiple tasks and their deadlines
void addTasks(struct Graph* graph, int deadlines[]) {
    int tasks;
    printf("Enter number of tasks to add: ");
    scanf("%d", &tasks);
    getchar(); // Clear newline from buffer

    for (int i = 0; i < tasks; i++) {
        char name[MAX_NAME_LEN];
        printf("Enter name for task %d: ", i + 1);
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove trailing newline
        getIndex(graph, name);             // Add task to graph
        printf("Enter deadline for %s: ", name);
        scanf("%d", &deadlines[getIndex(graph, name)]); // Store deadline
        getchar(); // Clear input buffer
    }
}

// Function to add dependencies between tasks
void addDependencies(struct Graph* graph, int deadlines[]) {
    char task[MAX_NAME_LEN];
    printf("Enter task name to add dependencies for: ");
    fgets(task, MAX_NAME_LEN, stdin);
    task[strcspn(task, "\n")] = '\0'; // Remove newline

    char depTask[MAX_NAME_LEN];
    int n;
    printf("How many dependencies does '%s' have? ", task);
    scanf("%d", &n);
    getchar(); // Clear newline

    for (int i = 0; i < n; i++) {
        printf("Enter name of dependency %d: ", i + 1);
        fgets(depTask, MAX_NAME_LEN, stdin);
        depTask[strcspn(depTask, "\n")] = '\0';
        // Add an edge from dependency to task using its deadline
        addEdge(graph, depTask, task, deadlines[getIndex(graph, task)]);
    }
}

// getting index for task
int findTaskIndex(struct Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertexNames[i], name) == 0)
            return i;
    }
    return -1; 
}
// traversing every task a node is dependent on
void suggestNextTasks(struct Graph* g, char* taskName){
    int index = findTaskIndex(g, taskName); 
    if(index==-1){
        printf("Not Found"); 
        return; 
    }
    struct Node* temp = g->array[index].head;

    if (temp == NULL) {
        printf("\nTask '%s' has no immediate subsequent tasks (it may be a final task).\n", taskName);
        return;
    }

    printf("\nTasks that can be started after '%s':\n", taskName);
    while (temp) {
        printf(" - %s\n", g->vertexNames[temp->vertex]);
        temp = temp->next;
    }
    printf("\n");

}
// traverse till you find task, and then update deadline
void RescheduleTasks(struct Graph* graph, int deadlines[]) {
    char taskName[MAX_NAME_LEN];
    
    printf("Enter the name of the task to edit: ");
    fgets(taskName, MAX_NAME_LEN, stdin);
    taskName[strcspn(taskName, "\n")] = '\0';

    // 1. Find the task safely
    int index = findTaskIndex(graph, taskName);

    // 2. Check if it exists
    if (index == -1) {
        printf("\nError: Task '%s' not found.\n", taskName);
        return;
    }

    // 3. Get new deadline
    int newDeadline;
    printf("Current deadline for '%s' is: %d\n", taskName, deadlines[index]);
    printf("Enter new deadline: ");
    scanf("%d", &newDeadline);
    getchar(); // Consume the newline character left by scanf

    // 4. Update the array
    deadlines[index] = newDeadline;

    printf("\nSuccess: Deadline for '%s' updated to %d.\n", taskName, newDeadline);
}
// export plan is just printing the graph-> no exporting into a file

void exportPlan(struct Graph* graph, int deadlines[]){
    char filename[MAX_NAME_LEN];
    FILE* fptr;

    if (graph->numVertices == 0) {
        printf("\nNo tasks to export.\n");
        return;
    }

    printf("Enter filename to export to (e.g., tasks.txt): ");
    fgets(filename, MAX_NAME_LEN, stdin);
    filename[strcspn(filename, "\n")] = '\0';

    // 1. Open the file
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("\nError: Could not open file '%s' for writing.\n", filename);
        return;
    }

    // 2. Write Tasks and Deadlines
    // We use a simple "task_name,deadline" format
    fprintf(fptr, "[TASKS]\n");
    for (int i = 0; i < graph->numVertices; i++) {
        fprintf(fptr, "%s,%d\n", graph->vertexNames[i], deadlines[i]);
    }

    // 3. Write Dependencies (Edges)
    // We use a simple "prerequisite_task,dependent_task" format
    fprintf(fptr, "\n[DEPENDENCIES]\n");
    for (int i = 0; i < graph->numVertices; i++) {
        struct Node* temp = graph->array[i].head;
        const char* srcName = graph->vertexNames[i];
        
        while (temp) {
            const char* destName = graph->vertexNames[temp->vertex];
            fprintf(fptr, "%s,%s\n", srcName, destName);
            temp = temp->next;
        }
    }

    // 4. Close the file and confirm
    fclose(fptr);
    printf("\nSuccess: All tasks and dependencies exported to '%s'.\n", filename);
}
// ==================== MAIN ====================

int main() {
    struct Graph* graph = createGraph(MAX_VERTICES);
    int deadlines[MAX_VERTICES] = {0};

    int choice;
    char start[MAX_NAME_LEN], end[MAX_NAME_LEN];
    char taskName[MAX_NAME_LEN]; 
    while (1) {
        printf("========== TASK SCHEDULER ==========\n");
        printf("1. Add Tasks\n");
        printf("2. Add Dependencies\n");
        printf("3. Critical Path Detection\n");
        printf("4. Shortest Path Between Two Tasks\n");
        printf("5. Display Graph\n");
        printf("6. Suggest Next Tasks\n"); 
        printf("7. Exit\n");
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
                
                printf("Enter task name to find next tasks for: \n");
                fgets(taskName, MAX_NAME_LEN, stdin);
                taskName[strcspn(taskName, "\n")] = '\0';
                suggestNextTasks(graph, taskName);
                break;

            case 7:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

