// file: simple_graph.h
// description: 
// definitions, structures and interface for an adjacency list graph
// author: bksteele, bks@cs.rit.edu
// date: 9/24/2017

#ifndef SIMPLE_GRAPH_
#define SIMPLE_GRAPH_

#include <stdio.h>                      // FILE
#include <object_list.h>                // linked list utility module

#define MAX_NAME  100                   /// maximum length of a node name

#define MAX_FILE_LINE_LENGTH 2000       ///< maximum file line length

/// GraphNode implements a simple graph node using 
/// an adjacency list with a variable number of neighbors.
/// The simple graph uses the ObjectList unordered, dynamic list module
/// to store both the list of nodes in a graph and the list of 
/// adjacent, neighboring nodes for a given node in the graph.
/// @see ObjectList

typedef struct GraphNode_S {
    char name[MAX_NAME];                ///< name of instance
    ObjectList * neighbors;             ///< neighbors of instance
} GraphNode;

/// grh_create_node creates a graph node in dynamic storage.
/// The new instance has a copy of the name passed in.
/// @return pointer to the new instance.

GraphNode * grh_create_node( char * name );

/// grh_delete_node deletes the given node.
/// Note: due to potential cycles, the deletion cannot follow the neighbors.
/// To ensure destruction of all dynamic memory, it is necessary for
/// applications using this module to maintain a comprehensive list of
/// all nodes created.

void grh_delete_node( GraphNode * node );

/// grh_delete_graph will delete the entire list of graph nodes.
/// @param graph the list of graph nodes

void grh_delete_graph( ObjectList * graph );


/// grh_find_node_by_name searches the graph (a list of GraphNode) for
/// the node whose name matches the input name.
/// @param name the name of the node to find
/// @return pointer to the instance or NULL if not found.
/// @pre-condition graph is NOT NULL and list is NULL terminated

GraphNode * grh_find_node_by_name( ObjectList * graph, char * name ) ;

/// grh_print_graph
/// @param graph the list of nodes in the graph
/// @pre-condition graph is NOT NULL and list is NULL terminated

void grh_print_graph( ObjectList * graph ) ;

/// grh_load_file loads data from the input stream and creates 
/// zero or more graph nodes, which it adds to the graph.
/// @param graph the list of nodes already in the graph
/// @param input the file stream of node information to put into the graph
/// @pre-condition graph is NOT NULL and list is NULL terminated.
/// @pre-condition input is a valid open file object ready for reading.

void grh_load_file( ObjectList * graph, FILE * input ) ;

#endif // SIMPLE_GRAPH_
