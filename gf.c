#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_NODES 100

typedef struct {
    int id;
    float x, y;
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int count;
} NodeList;

// Parsing input file
int parse_input_file(const char *filename, NodeList *node_list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    while (fscanf(file, "%d %f %f", &node_list->nodes[node_list->count].id,
                  &node_list->nodes[node_list->count].x,
                  &node_list->nodes[node_list->count].y) == 3) {
        node_list->count++;
    }

    fclose(file);
    return 1;
}

// Calc euclidean dist
float euclidean_distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Find the neighbor closest to the destination
int find_closest_neighbor(NodeList *node_list, int current_id, int dest_id, float radio_range) {
    Node *current_node = NULL, *dest_node = NULL;
    for (int i = 0; i < node_list->count; i++) {
        if (node_list->nodes[i].id == current_id) current_node = &node_list->nodes[i];
        if (node_list->nodes[i].id == dest_id) dest_node = &node_list->nodes[i];
    }

    if (!current_node || !dest_node) {
        fprintf(stderr, "Error: Node(s) not found.\n");
        return -1;
    }

    float dest_x = dest_node->x, dest_y = dest_node->y;
    float min_distance = INFINITY;
    int closest_neighbor = -1;

    for (int i = 0; i < node_list->count; i++) {
        if (node_list->nodes[i].id == current_id) continue;

        float neighbor_distance = euclidean_distance(current_node->x, current_node->y,
                                                      node_list->nodes[i].x, node_list->nodes[i].y);

        if (neighbor_distance <= radio_range) {
            float distance_to_dest = euclidean_distance(node_list->nodes[i].x, node_list->nodes[i].y, dest_x, dest_y);
            if (distance_to_dest < min_distance) {
                min_distance = distance_to_dest;
                closest_neighbor = node_list->nodes[i].id;
            }
        }
    }

    return closest_neighbor;
}

// Geographic Forwarding protocol main function
void geographic_forwarding(NodeList *node_list, int source, int destination, float radio_range) {
    int current = source;
    int visited[MAX_NODES] = {0};
    int route[MAX_NODES], route_count = 0;

    while (current != -1) {
        if (visited[current]) {
            printf("Routing loop detected. Destination unreachable.\n");
            return;
        }

        route[route_count++] = current;
        visited[current] = 1;

        if (current == destination) {
            printf("Route: ");
            for (int i = 0; i < route_count; i++) {
                printf("%d", route[i]);
                if (i < route_count - 1) printf(" -> ");
            }
            printf("\n");
            return;
        }

        current = find_closest_neighbor(node_list, current, destination, radio_range);
    }

    printf("No valid neighbors found. Destination unreachable.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input.file> <source node> <destination node> <radio range>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    int source = atoi(argv[2]);
    int destination = atoi(argv[3]);
    float radio_range = atof(argv[4]);

    NodeList node_list = {0};

    if (!parse_input_file(input_file, &node_list)) {
        return EXIT_FAILURE;
    }

    geographic_forwarding(&node_list, source, destination, radio_range);

    return EXIT_SUCCESS;
}

