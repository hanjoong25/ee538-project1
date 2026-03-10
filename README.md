# EE538: Project 1: Opinion Diffusion using Linked Lists

## Overview
In this project, you will simulate how opinions spread in a simple "society" of voters. Each voter holds one of two opinions — represented by 0 or 1. Voters influence one another: at each step, to fit into their social circle, voters adopt the majority opinion of their friends. This is repeated multiple times until no one is changing their opinions anymore or enough time (number of iterations) has passed.


## Data Representation
- The startercode loads the inputs from files in your workspace: opinions.txt and edge_list.txt.
opinions,txt contains a list of initial opinions. The file looks like

0, 0

1, 1

2, 1

...

Each line corresponding to a voter -- a line 2, 1 means voter 2 has opinion 1.

The other file "edge_list.txt" contains a pair of indices denoting who is influenced by whom. The file looks like

0, 2

2, 3

1, 5

...

0, 2 corresponds to: voter 0 influences voter 2.

## Sample Output
The program should print the state of the system at regular intervals. For example:

Iteration 0: fraction of 1's = 0.52

Iteration 100: fraction of 1's = 0.47

Iteration 200: fraction of 1's = 0.38

Consensus reached: all 0’s

## Part 1
Your task is to clone this repo and complete the startercode and complete the following:

```
void build_adj_matrix()
{
    // (1) allocate matrix adj of appropriate size


    // (2) run through edge list and populate adj
}

double calculate_fraction_of_ones()
{
    // (3) Calculate the fraction of nodes with opinion 1 and return it.
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) Count the number of neighbours with opinion 0 and opinion 1. Return the majority (0 or 1). 
    //If tie, return 0.
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // (5) For each node, calculate the majority opinion among its neighbours and update the node's opinion.
    // Return true if any node's opinion changed, false otherwise.
    
}
```

Then, in main():

```
 cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    // (6) Run until consensus or max iterations
    //while( ... )
    
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
```

Test your code by editing the input .txt files.