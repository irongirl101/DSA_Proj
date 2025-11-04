#include <stdio.h> 
#include <stdlib.h> 

#define MAX_vertices 100
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
    for (int v = 0; v < graph->numVertices; ++v) {
        struct Node* temp = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\n head ", v);
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
    addEdge(graph, 0, "no");
    addEdge(graph, 0, "yes");
    addEdge(graph, 1, "try");
    addEdge(graph, 2, "noo");
    addEdge(graph, 2, "go");
    addEdge(graph, 1, "banana");

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