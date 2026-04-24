#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.


/****************************************************************/

// Simple vector to hold each node's opinion (0 or 1).
std::vector<int> opinions;

// Adjacency list using a vector-of-vectors.
// in_neighbors[v] stores every node u such that u -> v (u influences v).
//
// Project 1.1 version used a V-by-V adjacency matrix (also a vector of vectors),
// which is O(V^2) space even when the graph has only E << V^2 edges.
// A list-of-neighbors keeps only the edges that actually exist: O(V + E) space.
// For a large sparse social network, this is the key improvement.
//
// Consequence for time complexity:
//   get_majority_friend_opinions: O(V) -> O(deg(node))
//   update_opinions per call:      O(V^2) -> O(V + E)
std::vector<std::vector<int>> in_neighbors;

// Edge list only needed between read_edges() and build_adj_matrix().
std::vector<std::vector<int>> edge_list;

// Scratch buffer for synchronous updates. Resized once in build_adj_matrix()
// and reused every iteration instead of being reallocated each call.
std::vector<int> new_opinions_buf;

// Running count of nodes whose opinion is 1. We maintain it as opinions
// change, trading a tiny amount of extra space for a big time saving
// (Dynamic Programming lecture, slide 4: "Use space vs Time").
// This turns calculate_fraction_of_ones() from O(V) into O(1).
int num_ones = 0;

void build_adj_matrix()
{
    // (1) Allocate one empty list per node.
    in_neighbors.assign(total_nodes, vector<int>());

    // (2) Walk the edge list once. edge[0] -> edge[1] means edge[0]
    //     influences edge[1], so add edge[0] to edge[1]'s in-neighbor list.
    for (int i = 0; i < (int)edge_list.size(); i++) {
        int src = edge_list[i][0];
        int dst = edge_list[i][1];
        in_neighbors[dst].push_back(src);
    }

    // Allocate the update scratch buffer once, now that total_nodes is known.
    new_opinions_buf.resize(total_nodes);
}

double calculate_fraction_of_ones()
{
    // (3) O(1): we maintain num_ones incrementally when opinions change.
    return (double)num_ones / total_nodes;
}

// For a given node, count majority opinion among its in-neighbors. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) Only visit actual in-neighbors: O(deg(node)), not O(V).
    int count0 = 0, count1 = 0;
    for (int i = 0; i < (int)in_neighbors[node].size(); i++) {
        int u = in_neighbors[node][i];
        if (opinions[u] == 0) count0++;
        else count1++;
    }
    return (count1 > count0) ? 1 : 0;
}

// Calculate new opinions for all voters and return if anyone's opinion changed.
bool update_opinions()
{
    // (5) Single pass over all nodes: compute new opinion, detect change,
    //     and recount ones all at once. Total edge visits across the loop
    //     equal E, so this runs in O(V + E) per call.
    bool changed = false;
    int new_ones = 0;
    for (int i = 0; i < total_nodes; i++) {
        int new_op = get_majority_friend_opinions(i);
        new_opinions_buf[i] = new_op;
        if (new_op != opinions[i]) changed = true;
        if (new_op == 1) new_ones++;
    }
    // Apply synchronously, then update the running count.
    for (int i = 0; i < total_nodes; i++) {
        opinions[i] = new_opinions_buf[i];
    }
    num_ones = new_ones;
    return changed;
}

int main() {
    // Read input files
    read_opinions("opinions.txt");
    read_edges("edge_list.txt");

    // Convert edge list into adjacency list now that total_nodes is known.
    build_adj_matrix();

    // Initialize the running count of 1's from the starting opinions.
    num_ones = 0;
    for (int i = 0; i < total_nodes; i++) {
        if (opinions[i] == 1) num_ones++;
    }

    cout << "Total nodes: " << total_nodes << endl;

    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;

    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = "
         << calculate_fraction_of_ones() << endl;

    /// (6)  //////////////////////////////////////////////
    while (opinions_changed && iteration < max_iterations) {
        opinions_changed = update_opinions();
        iteration++;
        cout << "Iteration " << iteration << ": fraction of 1's = "
             << calculate_fraction_of_ones() << endl;
    }
    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = "
         << final_fraction << endl;

    if (final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if (final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;

    return 0;
}


/*********** Functions to read files **************************/

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while (file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if (id >= total_nodes) total_nodes = id + 1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;

    while (file >> source >> target)
    {
        edge_list.push_back({source, target});
        if (source >= total_nodes) total_nodes = source + 1;
        if (target >= total_nodes) total_nodes = target + 1;
    }
    file.close();
}

/********************************************************************** */
