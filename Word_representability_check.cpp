#include <bits/stdc++.h>
using namespace std;

string str;           // Configuration
int n, adj[10][10] = {0}, diag[10][10] = {0}, tempCheckTransitiveDiag[10][10],x=0;
vector<pair<int, int>> vect; // Vector to store edges
// int cnt = 0;

// Function to initialize matrix to a specific value
void make_matrix_to(int diag[][10], int n, int value) {
    for (int m = 0; m < n; ++m)
        for (int l = 0; l < n; ++l)
            diag[m][l] = value;
}

// Function to reverse a character array
void reverse(char temp[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        swap(*(temp + start), *(temp + end));
        start++;
        end--;
    }
}

// Function to convert an integer to a string (with base support)
char* itoa_(int num, char* temp, int base) {
    int i = 0;
    bool isNegative = false;
    if (num == 0) {
        temp[i++] = '0';
        temp[i] = '\0';
        return temp;
    }
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
    while (num != 0) {
        int rem = num % base;
        temp[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if (isNegative)
        temp[i++] = '-';

    reverse(temp, i);
    return temp;
}

// Function to generate binary orientation from integer
void binary_orientation(unsigned n, int edges) {
    char temp[edges + 1] = {};
    str = itoa_(n, temp, 2); // Finds the unique binary configuration
    if (str.size() != edges) {
        int a = str.size();
        for (int i = 0; i < edges; ++i) {
            if (i < edges - a)
                temp[i] = '0';
            else
                temp[i] = str[a + i - edges];
        }
        str = temp;
    }
}

void findAllPaths(vector<vector<int>>& graph, int src, int dest, vector<int>& path, vector<vector<int>>& allPaths) {
    // Add the current vertex to the path
    path.push_back(src);

    // If the current vertex is the destination, add the current path to the list of all paths
    if (src == dest) {
        allPaths.push_back(path);
    } else {
        // Explore all neighbors of the current vertex
        for (int i = 0; i < graph.size(); ++i) {
            // Check if there is an edge from 'src' to 'i' and 'i' is not already in the path
            if (graph[src][i] && find(path.begin(), path.end(), i) == path.end()) {
                // Recursively find all paths from 'i' to 'dest'
                findAllPaths(graph, i, dest, path, allPaths);
            }
        }
    }

    // Remove the current vertex from the path to backtrack
    path.pop_back();
}

// Function to return all paths from vertex 'a' to vertex 'b' in the graph
vector<vector<int>> allPaths(vector<vector<int>>& graph, int a, int b) {
    vector<vector<int>> allPaths;
    vector<int> path;

    // Find all paths from 'a' to 'b'
    findAllPaths(graph, a, b, path, allPaths);

    return allPaths;
}

int checkTransitivity(const vector<vector<int>>& graph, const vector<int>& path) {
    // Iterate through the path vertices except the last one
    for (int i = 0; i < path.size(); i++) {
		for(int j=i+1; j<path.size(); j++){
			// std::cout<<"hi"<<endl;
			// std::cout<<path[i] << " " << path[j] << " " << graph[path[i]][path[j]] << endl;
			if(graph[path[i]][path[j]]==0) return 0;
		}
        
    }
    // Transitivity property holds
	
    return 1;
}

bool hasCycleUtil(const vector<vector<int>>& graph, int v, vector<bool>& visited, vector<bool>& recursionStack) {
    if (!visited[v]) {
        visited[v] = true;
        recursionStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        for (int i = 0; i < graph.size(); ++i) {
            if (graph[v][i]) {
                if (!visited[i] && hasCycleUtil(graph, i, visited, recursionStack)) {
                    return true;
                } else if (recursionStack[i]) {
                    return true;
                }
            }
        }
    }
    recursionStack[v] = false; // Remove the vertex from recursion stack
    return false;
}

bool hasCycle(const vector<vector<int>>& graph) {
    int V = graph.size();
    vector<bool> visited(V, false);
    vector<bool> recursionStack(V, false);

    // Call the recursive helper function to detect cycle in different DFS trees
    for (int i = 0; i < V; ++i) {
        if (hasCycleUtil(graph, i, visited, recursionStack)) {
            return true;
        }
    }
    return false;
}



// Function to check transitivity
bool transitivity_check(int diag[][10], int n) {

	vector<vector<int>>diag2(n,vector<int>(n,0));
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			diag2[i][j]=diag[i][j];
		}
	}
    // int ordering[n][n] = {0};
    make_matrix_to(tempCheckTransitiveDiag, n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (diag[i][j] == 1)
                tempCheckTransitiveDiag[i][j] = 1;
        }
    }

    // For semi-transitivity
	bool flag=0;
	if(!hasCycle(diag2)){
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				vector<vector<int>>paths = allPaths(diag2,i,j);
				if(paths.size()>1){
					bool flag1 = 0;
					for(int k=0;k<paths.size();k++){
						if(paths[k].size()==2){
							flag1=1;
						}
					}
					if(flag1){
					for(int k=0;k<paths.size();k++){
						if(!checkTransitivity(diag2,paths[k])){

							flag=1;
							break;
						}		 
					}
					}
					

				}
				if(flag) break;
			}
		if(flag) break;
	}
	}
	else{
		flag=1;
	}
    return !flag;
}

// Function to find all digraphs
bool find_all_digraph(int n, int edges, int diag[][10]) {
    // Finds all the (2^edges) digraph configurations/ adjacency matrices
    int x = pow(2, edges);
    for (int i = 0; i < x; i++) {
        binary_orientation(i, edges); // Gets the orientation
        make_matrix_to(diag, n, 0);    // Initialize the digraph to 0 before marking the directed edges
        for (int j = 0; j < str.size(); ++j) { // edges=str.size()
            if (str[j] == '0')
                diag[vect[j].first][vect[j].second] = 1;
            else if (str[j] == '1')
                diag[vect[j].second][vect[j].first] = 1;
        }
		
        if (transitivity_check(diag, n)) {
            return 1;
        } else
            continue;
    }
    return 0;
}

int main() {
    string word, filename, opfile, checkword = "Graph";

    int EDGES;
    std::cout << "Enter the order of graph: ";
    cin >> n;

    ostringstream str1;
    str1 << n;
    opfile = "Output_" + str1.str() + ".txt";
    filename = "order" + to_string(n) + ".txt";
    fstream file;
    ofstream fout;

    file.open(filename.c_str());
    fout.open(opfile.c_str());
    if (file.is_open() && fout.is_open()) {
        while (file >> word) {
            if (word == checkword) {
                EDGES = 0;
                str = "";
                adj[10][10] = {0};
                diag[10][10] = {0};
                tempCheckTransitiveDiag[10][10] = {0};
                vect.clear();
                file >> word;
                file >> word;
                file >> word;
                file >> word;
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        int tempo;
                        stringstream test_word(word);
                        test_word >> tempo;
                        adj[i][j] = tempo;
                        if (j > i && adj[i][j] == 1) {
                            vect.push_back(make_pair(i, j));
                            EDGES++;
                        }
                        if (!(i == n - 1 && j == n - 1))
                            file >> word;
                    }
                }
                if (EDGES != (n * (n - 1)) / 2) {
                    bool ANS = find_all_digraph(n, EDGES, diag);
                    if (ANS == 0) {
                        fout << "NO\n";
                        x++;
                    } else
                        fout << "YES\n";
                } else {
                    fout << "YES\n";
                }
            }
        }
    } else
        std::cout << "Error in file opening.";
    file.close();
    fout.close();
    std::cout << "\nFile Completed\n"<< x << " non word representable graphs were found"<<endl;
    return 0;
}
