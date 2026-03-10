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

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// global adjacency matrix initialized later
std::vector<std::vector<int>> adj;

// edge list: each row contains {source, target}
std::vector<std::vector<int>> edge_list;

void build_adj_matrix()
{
    // (1) allocate matrix adj of appropriate size
    adj.assign(total_nodes, vector<int>(total_nodes, 0));

    // (2) run through edge list and populate adj
    for (auto& edge : edge_list) {
        adj[edge[0]][edge[1]] = 1;
    }
}

double calculate_fraction_of_ones()
{
    // (3) Calculate the fraction of nodes with opinion 1 and return it.
    int count = 0;
    for (int op : opinions) {
        if (op == 1) count++;
    }
    return (double)count / total_nodes;
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) adj[i][node] == 1 means i influences node.
    // Count opinions of all influencers; return majority (tie -> 0).
    int count0 = 0, count1 = 0;
    for (int i = 0; i < total_nodes; i++) {
        if (adj[i][node] == 1) {
            if (opinions[i] == 0) count0++;
            else count1++;
        }
    }
    return (count1 > count0) ? 1 : 0;
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // (5) Compute all new opinions synchronously, then apply them.
    vector<int> new_opinions(total_nodes);
    for (int i = 0; i < total_nodes; i++) {
        new_opinions[i] = get_majority_friend_opinions(i);
    }
    bool changed = false;
    for (int i = 0; i < total_nodes; i++) {
        if (new_opinions[i] != opinions[i]) changed = true;
        opinions[i] = new_opinions[i];
    }
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();
    
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
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
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
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */