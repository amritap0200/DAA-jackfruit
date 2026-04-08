#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Node structure
typedef struct Node {
    int key;
    int priority;
    struct Node *left, *right;
} Node;

// Create new node
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->priority = rand();
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Right Rotation
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

// Left Rotation
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

// Insert
Node* insert(Node* root, int key) {
    if (root == NULL)
        return createNode(key);

    if (key < root->key) {
        root->left = insert(root->left, key);

        if (root->left->priority < root->priority)
            root = rotateRight(root);
    } else {
        root->right = insert(root->right, key);

        if (root->right->priority < root->priority)
            root = rotateLeft(root);
    }

    return root;
}

// Delete
Node* deleteNode(Node* root, int key) {
    if (root == NULL) return NULL;

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // Node found
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        else {
            // Both children exist
            if (root->left->priority < root->right->priority) {
                root = rotateRight(root);
                root->right = deleteNode(root->right, key);
            } else {
                root = rotateLeft(root);
                root->left = deleteNode(root->left, key);
            }
        }
    }
    return root;
}

// Search
int search(Node* root, int key) {
    if (root == NULL) return 0;

    if (root->key == key)
        return 1;

    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

// Inorder Traversal
void inorder(Node* root) {
    if (root == NULL) return;

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

// Free entire tree (important in C)
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Main function
int main() {
    srand(time(NULL)); // seed randomness

    Node* root = NULL;

    // Insert elements
    int keys[] = {50, 30, 20, 40, 70, 60, 80};
    int n = sizeof(keys)/sizeof(keys[0]);

    for (int i = 0; i < n; i++)
        root = insert(root, keys[i]);

    printf("Inorder traversal: ");
    inorder(root);
    printf("\n");

    // Search
    printf("Search 40: %s\n", search(root, 40) ? "Found" : "Not Found");
    printf("Search 25: %s\n", search(root, 25) ? "Found" : "Not Found");

    // Delete
    root = deleteNode(root, 50);

    printf("After deleting 50: ");
    inorder(root);
    printf("\n");

    freeTree(root);
    return 0;
}
