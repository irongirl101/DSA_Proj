#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
// turns out i was overcomplicating everything, i just needed to do disconnected graphs 
// then search for closest deadline, and bfs for till that 
// by implementing a bfs, i will be taking queues into account as well 
// could have a queue for all the deadlines in order?  
#define SIZE 100

struct queue {
  int items[SIZE];
  int front;
  int rear;
};

struct Node {
  char *string;
  struct Node *next;
};

struct Adjlist {
  struct Node *head;
};

struct Graph {
  int numVertices;
  struct Adjlist *array;
  int *visited;
};

// Queue operations
struct queue *createQueue() {
  struct queue *q = malloc(sizeof(struct queue));
  q->front = q->rear = -1;
  return q;
}

int isEmpty(struct queue *q) {
  return q->rear == -1;
}

void enqueue(struct queue *q, int value) {
  if (q->rear == SIZE - 1)
    printf("Queue Full\n");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

int dequeue(struct queue *q) {
  if (isEmpty(q))
    return -1;
  int item = q->items[q->front];
  q->front++;
  if (q->front > q->rear)
    q->front = q->rear = -1;
  return item;
}

// Graph functions
struct Node *createNode(char *str) {
  struct Node *newNode = malloc(sizeof(struct Node));
  newNode->string = str;
  newNode->next = NULL;
  return newNode;
}

struct Graph *createGraph(int vertices) {
  struct Graph *graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;
  graph->array = malloc(vertices * sizeof(struct Adjlist));
  graph->visited = malloc(vertices * sizeof(int));

  for (int i = 0; i < vertices; i++) {
    graph->array[i].head = NULL;
    graph->visited[i] = 0;
  }
  return graph;
}

void addEdge(struct Graph *graph, int src, char *str) {
  struct Node *newNode = createNode(str);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
}

void bfs(struct Graph *graph, int startVertex) {
  struct queue *q = createQueue();
  graph->visited[startVertex] = 1;
  enqueue(q, startVertex);

  while (!isEmpty(q)) {
    int currentVertex = dequeue(q);
    printf("Visited %d\n", currentVertex);

    struct Node *temp = graph->array[currentVertex].head;
    while (temp) {
      int adjVertex = atoi(temp->string);
      if (graph->visited[adjVertex] == 0) {
        graph->visited[adjVertex] = 1;
        enqueue(q, adjVertex);
      }
      temp = temp->next;
    }
  }
}

void printGraph(struct Graph *graph) {
  for (int v = 1; v < graph->numVertices; v++) {
    struct Node *temp = graph->array[v].head;
    printf("\nAdjacency list of vertex %d\n head ", v);
    while (temp) {
      printf("-> %s", temp->string);
      temp = temp->next;
    }
    printf("\n");
  }
}

int main() {
  int numVertices = 10; // 1-based indexing
  struct Graph *graph = createGraph(numVertices);

  // Component 1
  addEdge(graph, 1, "2");
  addEdge(graph, 1, "3");
  addEdge(graph, 1, "4");
  addEdge(graph, 2, "5");

  // Component 2
  addEdge(graph, 6, "7");
  addEdge(graph, 6, "8");
  addEdge(graph, 8, "9");

  printGraph(graph);

  printf("\nBFS traversal (for disconnected graph):\n");
  for (int i = 1; i < numVertices; i++) {
    if (graph->visited[i] == 0)
      bfs(graph, i);
  }

  return 0;
}
