#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//Node class
struct Node {
	vector<string> values; //Containes every recipes' information
	Node *left, *right, *parent; //Every other node connected to the current node
	Node() : values(0), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(vector<string> theValues) : values(theValues), left(nullptr), right(nullptr), parent(nullptr) {}
};

class Splay {

    //General Rotation Functions
    void rotateLeft(Node* node) {
        Node* parent = node->right;
        node->right = parent->left;
        if (parent->left) {
            parent->left->parent = node;
        }
        parent->parent = node->parent;
        if (!parent->parent) {
            this->root = parent;
        }
        else if (node->parent->right == node) {
            node->parent->right = parent;
        }
        else {
            node->parent->left = parent;
        }
        parent->left = node;
        node->parent = parent;
    }

    void rotateRight(Node* node) {
        Node* parent = node->left;
        node->left = parent->right;
        if (parent->right) {
            parent->right->parent = node;
        }
        parent->parent = node->parent;
        if (!parent->parent) {
            this->root = parent;
        }
        else if (node->parent->left == node) {
            node->parent->left = parent;
        }
        else {
            node->parent->right = parent;
        }
        parent->right = node;
        node->parent = parent;
    }

    //Splay Function
    void splay(Node* node) {
        //Rotate nodes until the current node is the root
        while (node->parent) {
            //Zig Rotations
            if (node->parent->parent == nullptr) {
                //Zig Rotation
                if (node->parent->left == node) {
                    rotateRight(node->parent);
                }
                //Zag Rotation
                else {
                    rotateLeft(node->parent);
                }
            }
            //Zig Zig Rotation
            else if (node->parent->left == node && node->parent->parent->left == node->parent) {
                rotateRight(node->parent->parent);
                rotateRight(node->parent);
            }
            //Zag Zag Rotation
            else if (node->parent->right == node && node->parent->parent->right == node->parent) {
                rotateLeft(node->parent->parent);
                rotateLeft(node->parent);
            }
            //Zig Zag Rotation
            else if (node->parent->right == node && node->parent->parent->left == node->parent) {
                rotateLeft(node->parent);
                rotateRight(node->parent);
            }
            //Zag Zig Rotation
            else {
                rotateRight(node->parent);
                rotateLeft(node->parent);
            }
        }
    }

	public:
        Node* root;
        Node* insert(Node* root, vector<string> values);
        Node* search(Node* root, string value);
        void preOrder(Node* root);
        Splay() : root(nullptr) {}
		
};

//Insertion Function (Time Complexity: Worst = O(n) / Average = O(log(n))
Node* Splay::insert(Node* root, vector<string> values) {//New node and its parent
    Node* temp = root;
    Node* Parent = nullptr;

    //Loop to find where the new node's values will go into the tree, while following BST rules
    while (temp) {
        Parent = temp;
        if (temp->values[1] > values[1]) {
            temp = temp->left;
        }
        else {
            temp = temp->right;
        }
    }

    //Create the new node and set its parent
    temp = new Node(values);
    temp->parent = Parent;

    //Test if the new node is the root or to make sure the Parent points to the correct node for its left or right node
    if (Parent == nullptr) {
        root = temp;
    }
    else if (Parent->values[1] > temp->values[1]) {
        Parent->left = temp;
    }
    else {
        Parent->right = temp;
    }

    //Splay the tree so the new node becomes the root
    splay(temp);
    return temp;
}

//Search Function (Time Complexity: Worst = O(n) / Average = O(log(n))
Node* Splay::search(Node* root, string value) {
    Node* temp = root;

    //Loop through the tree until the current node's value[1] equals the wanted value
    while(temp) {
        if (temp->values[1] > value) {
            temp = temp->left;
        }
        else if(temp->values[1] < value) {
            temp = temp->right;
        }
        else {
            //Once the node is found, splay the tree so the node becomes the root
            splay(temp);
            return temp;
        }
    }

    return nullptr; //If the node does not exist
}

void Splay::preOrder(Node* root){
    if (root != nullptr){
        cout << root->values[1] << endl;
        //preOrder(root->left);
        //preOrder(root->right);
    }
}


int main() {
	Splay s; //Splay Tree Object
    int first = true; //Title line check

    //File data variables
    vector<string> row;
    string line, value = "";

    //Read the dataset file
    ifstream file("recipes_struct.csv");
    if (file.is_open()) {
        while (getline(file, line)) {
            if (first) {
                first = false;
                continue;
            }
            row.clear();
            stringstream str(line);
            while (getline(str, value, ',')) {
                row.push_back(value);
            }
            s.root = s.insert(s.root, row);
        }
    }
    file.close();
    string end = "End";
    cout << "Which recipe would you like to see?" << endl;
    string option;
    cin >> option;
    while (option != end) {
        if (option == "print") {
            s.preOrder(s.root);
            cin >> option;
            continue;
        }
        Node* curr = s.search(s.root, option);
        if (curr == nullptr) {
            cout << "naur sis" << endl;
            cin >> option;
            continue;
        }
        for (int i = 0; i < curr->values.size(); ++i) {
            cout << curr->values[i] << endl;
        }
        cin >> option;
    }
}