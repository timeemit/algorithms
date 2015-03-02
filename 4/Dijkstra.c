#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Edge {
  unsigned int source_node_id;
  unsigned int destination_node_id;
  unsigned int weight;
};

struct EdgeSet {
  struct Edge * edges;
  unsigned int length;
};

struct Node {
  unsigned int index;
  struct EdgeSet edgeset;
};

struct NodeSet {
  struct Node * nodes;
  unsigned int length;
};

void report_edge( struct Edge edge ) {
  printf("New Edge!\nSource: %i Destination: %i Weight: %i\n\n", edge.source_node_id, edge.destination_node_id, edge.weight);
};

int next_number(FILE * read_file, char stop_char){
    char this_character = 0;
    char * string = "";
    this_character = fgetc(read_file);
    printf("Stop character: %c\n", stop_char);
    while ( this_character != stop_char && this_character != '\n' ){
      char * temp_string;

      printf("Character: %c\n", this_character);

      temp_string = malloc( sizeof(string) );
      strcpy( temp_string, string );
      printf("Temp String: %s\n", temp_string);

      if ( strlen( string ) > 1 ) {
        string = ( char * ) realloc( string, sizeof(string) + sizeof(char) );
      } else {
        string = ( char * ) malloc( sizeof(string) + sizeof(char) );
      }
      strcpy( string, temp_string );

      printf("Index: %lu\n", strlen(string) );
      string[ strlen(string) ] = this_character;
      printf("Final String: %s\n\n", string);

      free(temp_string);

      this_character = fgetc(read_file);
    }

    fseek(read_file, -1, SEEK_CUR); // Rewind one character

    return atoi(string);
};

int main( int argc, char *argv[] ) {
  FILE* read_file;
  unsigned int number_of_nodes = atoi( argv[2] );
  struct NodeSet all_nodes;
  int node_index = 0;
  char this_char;

  all_nodes.nodes = malloc( number_of_nodes * sizeof(struct NodeSet) );
  all_nodes.length = 0;

  // Read file
  read_file = fopen(argv[1], "r");
  while ( (this_char = fgetc(read_file)) >= 0 ) {
    struct Node new_node;

    new_node.edgeset.length = 0;
    fseek(read_file, -1, SEEK_CUR); // Rewind one character to read new vertex index

    // Get vertex nmber
    new_node.index = next_number(read_file, ' ');
    fseek(read_file, +1, SEEK_CUR); // Move forward one character

    printf("New Node Index: %i\n\n", new_node.index);

    while(1) {
      struct Edge new_edge;
      struct Edge * temp_edges;
      memcpy(temp_edges, new_node.edgeset.edges, new_node.edgeset.length * sizeof(struct Edge));

      new_edge.source_node_id = new_node.index;

      new_edge.destination_node_id = next_number(read_file, ',');
      fseek(read_file, +1, SEEK_CUR); // Move forward one character

      new_edge.weight = next_number(read_file, ' ');
      report_edge(new_edge);

      // Append the the edge to the new_node's EdgeSet
      printf("hello\n");
      printf("%u\n", new_node.edgeset.length);
      printf("hello\n");

      if ( new_node.edgeset.length < 1 ){
        printf("Mallocing\n");
        new_node.edgeset.edges = ( struct Edge * ) malloc( ( new_node.edgeset.length + 1 ) * sizeof(struct Edge) );
      } else {
        printf("Reallocing\n");
        new_node.edgeset.edges = ( struct Edge * ) realloc( new_node.edgeset.edges, ( new_node.edgeset.length + 1 ) * sizeof(struct Edge) );
      }

      printf("1\n");
      memcpy(new_node.edgeset.edges, temp_edges, new_node.edgeset.length * sizeof(struct Edge));
      printf("2\n");
      new_node.edgeset.edges[new_node.edgeset.length] = new_edge;
      printf("3\n");
      new_node.edgeset.length += 1;

      this_char = fgetc(read_file);
      printf("%c ( %u )\n", this_char, this_char);
      if ( this_char == '\n' || this_char < 0) {
        printf("Breaking!\n");
        break;
      }
    }
  }

  printf("Finished!\n");

  if ( read_file != NULL ) fclose(read_file);
  
  printf("Closed!\n");

  return 0;
}
