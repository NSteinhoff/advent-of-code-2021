#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "12.txt"
#define MAX_NODES 100

/// A node representing a cave. The name is a pointer into the input string.
typedef struct {
	bool major;
	bool terminal;
	const char *name;
} Node;

/// Linked list of node indices representing a path through the cave system.
typedef struct Path {
	size_t node;
	const struct Path *tail;
} Path;

/// Add a node to the path
static Path *path_add(const Path *path, size_t node) {
	Path *head = malloc(sizeof(Path));
	head->node = node;
	head->tail = path;

	return head;
}

/// Check if a node is in the path
static bool path_includes(const Path *path, size_t node) {
	if (!path)
		return false;

	if (path->node == node)
		return true;

	return path_includes(path->tail, node);
}

/// Check if the a node's name identifies it as a major node.
static bool is_major(const char *name) {
	for (; *name; name++)
		if (islower(*name))
			return false;

	return true;
}

/// Check if the a node's name identifies it as a terminal node.
static bool is_terminal(const char *name) {
	return (strcmp(name, "start") == 0 || strcmp(name, "end") == 0);
}

/// Find a node by name.
static size_t find(const char *name, Node *nodes, size_t len) {
	for (size_t i = 0; i < len; i++)
		if (strcmp(name, nodes[i].name) == 0)
			return i;

	return SIZE_MAX;
}

/// Add a node and return its index.
static size_t add_node(const Node node, Node *nodes, size_t *len) {
	size_t node_idx = find(node.name, nodes, *len);

	if (node_idx != SIZE_MAX)
		return node_idx;

	node_idx = (*len)++;
	nodes[node_idx] = node;
	return node_idx;
}

/// Parse input into nodes and the connection graph.
///
/// The graph is represented as an [adjacency matrix][1].
///
/// [1]: https://en.wikipedia.org/wiki/Adjacency_matrix
static size_t parse(char *content, Node *nodes,
		    bool adjacency_matrix[][MAX_NODES]) {
	size_t n_nodes = 0;

	for (char *line; (line = strsep(&content, "\n"));) {
		if (line[0] == '\0')
			continue;

		const char *src_name = strsep(&line, "-");
		const char *dst_name = strsep(&line, "-");

		size_t src_idx =
			add_node((Node){.major = is_major(src_name),
					.terminal = is_terminal(src_name),
					.name = src_name},
				 nodes, &n_nodes);
		size_t dst_idx =
			add_node((Node){.major = is_major(dst_name),
					.terminal = is_terminal(dst_name),
					.name = dst_name},
				 nodes, &n_nodes);

		adjacency_matrix[src_idx][dst_idx] = 1;
		adjacency_matrix[dst_idx][src_idx] = 1;
	}

	return n_nodes;
}

/// Return the number of possible paths walking the graph starting from `node`.
static unsigned walk(size_t node, Path *path, const Node *nodes,
		     bool adjacency_matrix[][MAX_NODES], size_t n_nodes,
		     bool revisit) {
	// This is the base case of the recursion. We've reached the 'end'
	// node, meaning that the path is complete. Return 1 to the caller to
	// count this path.
	if (strcmp(nodes[node].name, "end") == 0) {
		free(path);

		return 1;
	}

	unsigned paths = 0;
	for (size_t i = 0; i < n_nodes; i++) {
		if (!adjacency_matrix[node][i])
			continue;

		bool can_visit = nodes[i].major || !path_includes(path, i);
		bool can_revisit = revisit && !nodes[i].terminal;

		if (can_visit || can_revisit)
			paths += walk(i, path_add(path, i), nodes,
				      adjacency_matrix, n_nodes,
				      can_visit ? revisit : false);
	}

	// We have to free path here, because there is no other handle to the
	// pointer. We are also done with it, so no need to keep it around.
	free(path);

	return paths;
}

/// Return the number of paths through the cave system.
static unsigned num_paths(char *content, bool revisit) {
	// Parse the graph into an adjacency matrix
	Node nodes[MAX_NODES];
	bool adjacency_matrix[MAX_NODES][MAX_NODES] = {0};
	size_t n_nodes = parse(content, nodes, adjacency_matrix);

	// Walk the graph from the "start" node
	size_t start = find("start", nodes, n_nodes);
	unsigned paths = walk(start, path_add(NULL, start), nodes,
			      adjacency_matrix, n_nodes, revisit);

	return paths;
}

int main() {
	FILE *file = fopen(INPUT, "r");
	fseek(file, 0L, SEEK_END);
	const size_t size = (size_t)ftell(file);
	fseek(file, 0L, SEEK_SET);
	char *content = malloc(size + 1);
	size_t bytes_read = fread(content, sizeof(char), size, file);
	content[bytes_read] = '\0';
	fclose(file);

	unsigned paths = num_paths(content, true);
	printf("Paths: %u\n", paths);

	free(content);

	return 0;
}
