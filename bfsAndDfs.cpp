#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>

using namespace std;

// ----------- TREE STRUCTURE FOR BFS -----------
class TreeNode {
public:
    TreeNode *left, *right;
    int data;

    TreeNode(int val) {
        data = val;
        left = right = NULL;
    }
};

class Breadthfs {
public:
    TreeNode* insert(TreeNode* root, int data);
    void bfs(TreeNode* head);
};

TreeNode* Breadthfs::insert(TreeNode* root, int data) {
    if (!root) return new TreeNode(data);

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* temp = q.front();
        q.pop();

        if (!temp->left) {
            temp->left = new TreeNode(data);
            return root;
        } else {
            q.push(temp->left);
        }

        if (!temp->right) {
            temp->right = new TreeNode(data);
            return root;
        } else {
            q.push(temp->right);
        }
    }

    return root;
}

void Breadthfs::bfs(TreeNode* head) {
    if (!head) return;

    queue<TreeNode*> q;
    q.push(head);

    while (!q.empty()) {
        int qSize = q.size();
        vector<TreeNode*> levelNodes(qSize);

        for (int i = 0; i < qSize; ++i) {
            levelNodes[i] = q.front();
            q.pop();
        }

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            TreeNode* currNode = levelNodes[i];

            #pragma omp critical
            cout << "\t" << currNode->data;

            #pragma omp critical
            {
                if (currNode->left) q.push(currNode->left);
                if (currNode->right) q.push(currNode->right);
            }
        }
    }
    cout << endl;
}

// ----------- GRAPH STRUCTURE FOR DFS -----------
const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int node) {
    stack<int> s;
    s.push(node);

    while (!s.empty()) {
        int curr = s.top();
        s.pop();

        if (!visited[curr]) {
            visited[curr] = true;
            cout << curr << " ";

            #pragma omp parallel for
            for (int i = graph[curr].size() - 1; i >= 0; i--) {
                int adj = graph[curr][i];
                if (!visited[adj]) {
                    #pragma omp critical
                    s.push(adj);
                }
            }
        }
    }
    cout << endl;
}

// ----------- MAIN MENU TO CHOOSE BFS OR DFS -----------
int main() {
    int choice;
    cout << "\nChoose:\n1. Parallel BFS (Binary Tree)\n2. Parallel DFS (Graph)\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        TreeNode* root = NULL;
        int data;
        char ans;
        Breadthfs tree;

        do {
            cout << "Enter data => ";
            cin >> data;
            root = tree.insert(root, data);
            cout << "Do you want to insert more? (y/n): ";
            cin >> ans;
        } while (ans == 'y' || ans == 'Y');

        cout << "\nBreadth First Traversal (Parallelized):\n";
        tree.bfs(root);
    }

    else if (choice == 2) {
        int n, m, start_node;
        cout << "Enter number of Nodes, Edges, and Starting Node of the graph:\n";
        cin >> n >> m >> start_node;

        cout << "Enter edge pairs:\n";
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        #pragma omp parallel for
        for (int i = 0; i < n; i++) visited[i] = false;

        cout << "Depth First Traversal (Parallelized):\n";
        dfs(start_node);
    }

    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}
