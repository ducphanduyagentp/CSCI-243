// file: simple_graph.c
// description: 
// implementation for an adjacency list graph
// author: Duc Phan, ddp3945@rit.edu
// date: 10/21/2017

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "simple_graph.h"

/// grh_create_node creates a graph node in dynamic storage.
/// The new instance has a copy of the name passed in.
/// @return pointer to the new instance.

GraphNode * grh_create_node( char * name ) {
    GraphNode *grh = (GraphNode *) malloc(sizeof(GraphNode));
    assert(grh != NULL);
    strncpy(grh->name, name, strlen(name));
    grh -> neighbors = ol_create();
    return grh;
}

/// grh_delete_node deletes the given node.
/// Note: due to potential cycles, the deletion cannot follow the neighbors.
/// To ensure destruction of all dynamic memory, it is necessary for
/// applications using this module to maintain a comprehensive list of
/// all nodes created.

void grh_delete_node( GraphNode * node ) {
    ol_destroy(node -> neighbors);
    free(node);
}

/// grh_delete_graph will delete the entire list of graph nodes.
/// @param graph the list of graph nodes

void grh_delete_graph( ObjectList * graph ) {
    Iter *iter = ol_iterator(graph);
    GraphNode *node = NULL;
    while (ol_has_next(iter)) {
        node = ol_next(iter);
        grh_delete_node(node);
    }
    ol_destroy(graph);
    free(iter);
}


/// grh_find_node_by_name searches the graph (a list of GraphNode) for
/// the node whose name matches the input name.
/// @param name the name of the node to find
/// @return pointer to the instance or NULL if not found.
/// @pre-condition graph is NOT NULL and list is NULL terminated

GraphNode * grh_find_node_by_name( ObjectList * graph, char * name ) {
    Iter *iter = ol_iterator(graph);
    GraphNode *node = NULL;
    while (ol_has_next(iter)) {
        node = ol_next(iter);
        if (!strncmp(node -> name, name, strlen(name))) {
            free(iter);
            return node;
        }
    }
    free(iter);
    return NULL;
}

/// grh_print_graph
/// @param graph the list of nodes in the graph
/// @pre-condition graph is NOT NULL and list is NULL terminated

void grh_print_graph( ObjectList * graph ) {
    Iter *graphIter = ol_iterator(graph);
    GraphNode *node = NULL;
    bool flag = false;
    while (ol_has_next(graphIter)) {
        if (!flag) {
            puts("Graph: {name: neighbor names list}");
            flag = true;
        }
        node = ol_next(graphIter);
        assert(node -> name != NULL);
        printf("%s: ", node -> name);
        Iter *nodeIter = ol_iterator(node -> neighbors);
        GraphNode *neighbor = NULL;
        int size = ol_get_size(node -> neighbors);
        for (int i = 0; i < size && ol_has_next(nodeIter); i++) {
            neighbor = ol_next(nodeIter);
            printf("%s", neighbor -> name);
            printf(i < size - 1 ? ", " : "");
        }
        puts("");
        free(nodeIter);
    }
    free(graphIter);
}

/// grh_load_file loads data from the input stream and creates 
/// zero or more graph nodes, which it adds to the graph.
/// @param graph the list of nodes already in the graph
/// @param input the file stream of node information to put into the graph
/// @pre-condition graph is NOT NULL and list is NULL terminated.
/// @pre-condition input is a valid open file object ready for reading.

void grh_load_file( ObjectList * graph, FILE * input ) {
    puts("Reading graph data...");
    char line[MAX_FILE_LINE_LENGTH + 1];
    char *neighbors[MAX_FILE_LINE_LENGTH];
    GraphNode *node = NULL;

    while(fgets(line, MAX_FILE_LINE_LENGTH, input)) {
        assert(line != NULL);

        if (!strlen(line) || line[0] == '\n') {
            continue;
        }

        for (int i = 0; i < MAX_FILE_LINE_LENGTH; i++) {
            neighbors[i] = NULL;
        }      

        int numNeighbors = 0;
        neighbors[numNeighbors] = strtok(line, ",\n");

        GraphNode *currentNode = NULL;
        currentNode = grh_find_node_by_name(graph, neighbors[numNeighbors]);
        if (!currentNode) {
            assert(ol_insert(graph, (void *) grh_create_node(neighbors[numNeighbors])));
        }
        currentNode = grh_find_node_by_name(graph, neighbors[numNeighbors]);

        
        while((neighbors[++numNeighbors] = strtok(NULL, ",\n"))) {
            assert(neighbors[numNeighbors] != NULL);
            node = grh_find_node_by_name(graph, neighbors[numNeighbors]);
            if (!node) {
                assert(ol_insert(graph, (void *) grh_create_node(neighbors[numNeighbors])));
            }
            node = grh_find_node_by_name(graph, neighbors[numNeighbors]);
            
            if (!grh_find_node_by_name(currentNode -> neighbors, node -> name)) {
                ol_insert(currentNode -> neighbors, (void *) node);
            }

            if (!grh_find_node_by_name(node -> neighbors, currentNode -> name)) {
                ol_insert(node -> neighbors, (void *) currentNode);
            }
        }

    }

    puts("Finished reading graph data.");
}