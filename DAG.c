#include <stdio.h> 
#include <stdlib.h> 

// TOPIC: SMART TO DO LIST WITH DEPENDENCIES AND DEADLINES -> should be able to tell which task to do next (thats were level order traversal or bfs comes into picture, it is also useful for telling when a task is over)
// rather than using direct BFS, we need to have a pointer at each level, and once everything in that level is cleared, we can go to the next level 
// and cause its directed, its already in a given order 

/*#define MAX_vertices 100
#define MAX_length 100

struct Node{
    char *string; 
    struct Node* next; 
}; 

struct Adjlist{
    struct Node* head; 
};

struct Graph{
    int numVertices; 
    struct Adjlist* array; 

}; 

struct Node* createNode(char *str){
    struct Node* newnode = (struct Node*)malloc(sizeof(struct Node)); 
    newnode->string = str;
    newnode->next = NULL;
    return newnode; 
}

struct Graph* createGraph(int vertices){
    struct Graph* newgraph = (struct Graph*)malloc(sizeof(struct Graph)); 
    newgraph->numVertices= vertices;
    newgraph->array = (struct Adjlist*)malloc(vertices* sizeof(struct Adjlist)); 
    for (int i = 0; i < vertices; ++i) {
        newgraph->array[i].head = NULL;
    }
    return newgraph;
}

void addEdge(struct Graph* graph, int src, char *str) {
    struct Node* newNode = createNode(str);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}


void printGraph(struct Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        struct Node* temp = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\n head ", v+1);
        while (temp) {
            printf("-> %s", temp->string);
            temp = temp->next;
        }
        printf("\n");
    }
}


int main() {
    int numVertices = 5;
    struct Graph* graph = createGraph(numVertices);
    addEdge(graph, 0, "2");
    addEdge(graph, 0, "3");
    addEdge(graph, 0, "4");
    addEdge(graph, 1, "5");
    addEdge(graph, 3, "5");
    

    printGraph(graph);

    // Free allocated memory (important for real applications)
    for (int i = 0; i < numVertices; ++i) {
        struct Node* current = graph->array[i].head;
        while (current != NULL) {
            struct Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->array);
    free(graph);

    return 0;
}
*/



