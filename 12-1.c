#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "12.txt"
#define MAX_NODES 100

typedef struct {
	bool major;
	bool terminal;
	char *name;
} Node;

typedef struct Path {
	int node;
	struct Path *tail;
} Path;

static Path *path_add(Path *path, int node) {
	Path *head = malloc(sizeof(Path));
	head->node = node;
	head->tail = path;

	return head;
}

static bool path_includes(Path *path, int node) {
	if (!path)
		return false;

	if (path->node == node)
		return true;

	return path_includes(path->tail, node);
}

static bool is_major(char *name) {
	while (*name)
		if (islower(*name++))
			return false;

	return true;
}

static bool is_terminal(char *name) {
	return (strcmp(name, "start") == 0 || strcmp(name, "end") == 0);
}

static int find(const char *name, Node *nodes, size_t len) {
	for (size_t i = 0; i < len; i++)
		if (strcmp(name, nodes[i].name) == 0)
			return i;

	return -1;
}

static int add_node(const Node node, Node *nodes, size_t *len) {
	int node_idx = find(node.name, nodes, *len);

	if (node_idx != -1)
		return node_idx;

	nodes[*len] = node;

	return (*len)++;
}

static size_t parse(char *content, Node *nodes,
		    bool adjacency_matrix[][MAX_NODES]) {
	size_t n_nodes = 0;

	for (char *line; (line = strsep(&content, "\n"));) {
		if (line[0] == '\0')
			continue;

		char *src_name = strsep(&line, "-");
		char *dst_name = strsep(&line, "-");

		int src_idx = add_node((Node){.major = is_major(src_name),
					      .terminal = is_terminal(src_name),
					      .name = src_name},
				       nodes, &n_nodes);
		int dst_idx = add_node((Node){.major = is_major(dst_name),
					      .terminal = is_terminal(dst_name),
					      .name = dst_name},
				       nodes, &n_nodes);

		adjacency_matrix[src_idx][dst_idx] = 1;
		adjacency_matrix[dst_idx][src_idx] = 1;
	}

	return n_nodes;
}

static int walk(int node, Path *path, const Node *nodes,
		bool adjacency_matrix[][MAX_NODES], size_t n_nodes) {

	if (strcmp(nodes[node].name, "end") == 0) {
		free(path);

		return 1;
	}

	int paths = 0;
	for (size_t i = 0; i < n_nodes; i++) {
		if (!adjacency_matrix[node][i])
			continue;

		if (nodes[i].major || !path_includes(path, i))
			paths += walk(i, path_add(path, i), nodes,
				      adjacency_matrix, n_nodes);
	}

	// We have to free path here, because there is no other handle to the
	// pointer. We are also done with it, so no need to keep it around.
	free(path);

	return paths;
}

static int num_paths(FILE *file) {
	fseek(file, 0L, SEEK_END);
	const size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	char *content = malloc(size + 1);
	size_t bytes_read = fread(content, sizeof(char), size, file);
	content[bytes_read] = '\0';

	// Parse the graph into an adjacency matrix
	Node nodes[MAX_NODES];
	bool adjacency_matrix[MAX_NODES][MAX_NODES] = {0};
	size_t n_nodes = parse(content, nodes, adjacency_matrix);

	// Walk the graph from the "start" node
	int start = find("start", nodes, n_nodes);
	int paths = walk(start, path_add(NULL, start), nodes, adjacency_matrix,
			 n_nodes);

	free(content);

	return paths;
}

int main() {
	FILE *file = fopen(INPUT, "r");

	int paths = num_paths(file);
	printf("Paths: %d\n", paths);

	return 0;
}
