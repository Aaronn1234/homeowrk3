#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/* Part 1: Data Generation */
void generateData(int size, const char* filename);

/* Part 2: Array Operations */
int* readDataToArray(const char* filename, int size);
void sortArray(int* arr, int size);
int linearSearch(int* arr, int size, int target);
int binarySearch(int* arr, int size, int target);

/* Part 3: BST Operations */
Node* createNode(int key);
Node* insertRecursive(Node* root, int key);
Node* insertIterative(Node* root, int key);
Node* searchBST(Node* root, int target);
void freeTree(Node* root);

/* Utility Functions */
void selectRandomElements(int* sourceArray, int sourceSize, int* targetArray, int numElements);

int main() {
    /* Seed the random number generator */
    srand((unsigned int)time(NULL));

    /* Variables for timing */
    clock_t start_time, end_time;
    double time_taken;

    int sizes[] = {30, 1000, 6000, 10000};
    const char* filenames[] = {"nodes30.txt", "nodes1000.txt", "nodes6000.txt", "nodes10000.txt"};
    const char* sfilenames[] = {"snodes30.txt", "snodes1000.txt", "snodes6000.txt", "snodes10000.txt"};
    int num_files = 4;

    /* PART 1: Generation of Data */
    printf("--- Part 1: Generating Data ---\n");
    for (int i = 0; i < num_files; i++) {
        int size = sizes[i];

        generateData(size, filenames[i]);

        int* arr = readDataToArray(filenames[i], size);
        if (arr == NULL) {
            printf("Error reading %s\n", filenames[i]);
            return 1;
        }

        start_time = clock();
        sortArray(arr, size);
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Time to sort array of size %d: %f seconds\n", sizes[i], time_taken);

        FILE* sfile = fopen(sfilenames[i], "w");
        if (sfile == NULL) {
            printf("Error opening %s for writing\n", sfilenames[i]);
            free(arr);
            return 1;
        }

        for (int j = 0; j < size; j++) {
            fprintf(sfile, "%d\n", arr[j]);
        }
        fclose(sfile);
        free(arr);
    }
    printf("Data generation complete.\n\n");

    /* PART 2 & 3 Loop: Process each dataset size */
    for (int i = 0; i < num_files; i++) {
        printf("=== Processing Dataset Size: %d ===\n", sizes[i]);

        /* --- PART 2: Search on Sorted Array --- */
        int* sortedArray = readDataToArray(sfilenames[i], sizes[i]);
        int* unsortedArray = readDataToArray(filenames[i], sizes[i]);

        if (sortedArray == NULL || unsortedArray == NULL) {
            printf("Error reading dataset files.\n");
            free(sortedArray);
            free(unsortedArray);
            return 1;
        }

        int searchTargets[10];
        selectRandomElements(sortedArray, sizes[i], searchTargets, 10);

        volatile int result1 = 0;
        start_time = clock();
        for (int j = 0; j < 10; j++) {
            result1 += linearSearch(sortedArray, sizes[i], searchTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Linear Search time for 10 elements: %f seconds\n", time_taken);

        volatile int result2 = 0;
        start_time = clock();
        for (int j = 0; j < 10; j++) {
            result2 += binarySearch(sortedArray, sizes[i], searchTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Binary Search time for 10 elements: %f seconds\n", time_taken);

        /* --- PART 3: Search and Insertion on Binary Search Tree --- */

        /* 1. Random BST */
        Node* randomBST = NULL;
        start_time = clock();
        for (int j = 0; j < sizes[i]; j++) {
            randomBST = insertIterative(randomBST, unsortedArray[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Random BST Creation time: %f seconds\n", time_taken);

        /* 2. Sorted BST */
        Node* sortedBST = NULL;
        start_time = clock()
        for (int j = 0; j < sizes[i]; j++) {
            sortedBST = insertIterative(sortedBST, sortedArray[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Sorted BST Creation time: %f seconds\n", time_taken);

        volatile Node* bstResult1 = NULL;
        start_time = clock();
        for (int j = 0; j < 10; j++) {
            bstResult1 = searchBST(randomBST, searchTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Random BST Search time for 10 elements: %f seconds\n", time_taken);

        volatile Node* bstResult2 = NULL;
        start_time = clock();
        for (int j = 0; j < 10; j++) {
            bstResult2 = searchBST(sortedBST, searchTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Sorted BST Search time for 10 elements: %f seconds\n", time_taken);

        /* Generate 10 NEW random elements between 0 and 20000 */
        int insertTargets[10];
        bool used[20001] = {false};

        for (int j = 0; j < sizes[i]; j++) {
            used[unsortedArray[j]] = true;
        }

        int count = 0;
        while (count < 10) {
            int value = rand() % 20001;
            if (!used[value]) {
                used[value] = true;
                insertTargets[count] = value;
                count++;
            }
        }

        /*
           Rebuild fresh copies of the trees for each insertion timing
           so the same 10 elements are used fairly for each algorithm.
        */

        Node* randomBSTIter = NULL;
        for (int j = 0; j < sizes[i]; j++) {
            randomBSTIter = insertIterative(randomBSTIter, unsortedArray[j]);
        }

        start_time = clock();
        for (int j = 0; j < 10; j++) {
            randomBSTIter = insertIterative(randomBSTIter, insertTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Random BST Iterative Insertion time for 10 elements: %f seconds\n", time_taken);

        Node* sortedBSTIter = NULL;
        for (int j = 0; j < sizes[i]; j++) {
            sortedBSTIter = insertIterative(sortedBSTIter, sortedArray[j]);
        }

        start_time = clock();
        for (int j = 0; j < 10; j++) {
            sortedBSTIter = insertIterative(sortedBSTIter, insertTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Sorted BST Iterative Insertion time for 10 elements: %f seconds\n", time_taken);

        Node* randomBSTRec = NULL;
        for (int j = 0; j < sizes[i]; j++) {
            randomBSTRec = insertIterative(randomBSTRec, unsortedArray[j]);
        }

        start_time = clock();
        for (int j = 0; j < 10; j++) {
            randomBSTRec = insertRecursive(randomBSTRec, insertTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Random BST Recursive Insertion time for 10 elements: %f seconds\n", time_taken);

        Node* sortedBSTRec = NULL;
        for (int j = 0; j < sizes[i]; j++) {
            sortedBSTRec = insertIterative(sortedBSTRec, sortedArray[j]);
        }

        start_time = clock();
        for (int j = 0; j < 10; j++) {
            sortedBSTRec = insertRecursive(sortedBSTRec, insertTargets[j]);
        }
        end_time = clock();
        time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Sorted BST Recursive Insertion time for 10 elements: %f seconds\n", time_taken);

        /* Cleanup memory for this iteration */
        free(sortedArray);
        free(unsortedArray);

        freeTree(randomBST);
        freeTree(sortedBST);
        freeTree(randomBSTIter);
        freeTree(sortedBSTIter);
        freeTree(randomBSTRec);
        freeTree(sortedBSTRec);

        printf("\n");

        (void)result1;
        (void)result2;
        (void)bstResult1;
        (void)bstResult2;
    }

    return 0;
}

/* ------------------------------------------------------------------- */
/* Function Implementations */
/* ------------------------------------------------------------------- */

void generateData(int size, const char* filename) {
    bool used[20001] = {false};
    int* arr = (int*)malloc(size * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int count = 0;
    while (count < size) {
        int value = rand() % 20001;
        if (!used[value]) {
            used[value] = true;
            arr[count] = value;
            count++;
        }
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening %s for writing\n", filename);
        free(arr);
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
    free(arr);
}

int* readDataToArray(const char* filename, int size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            free(arr);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return arr;
}

void sortArray(int* arr, int size) {
    int* stackLow = (int*)malloc(size * sizeof(int));
    int* stackHigh = (int*)malloc(size * sizeof(int));

    if (stackLow == NULL || stackHigh == NULL) {
        printf("Memory allocation failed in sortArray.\n");
        free(stackLow);
        free(stackHigh);
        exit(1);
    }

    int top = -1;
    stackLow[++top] = 0;
    stackHigh[top] = size - 1;

    while (top >= 0) {
        int low = stackLow[top];
        int high = stackHigh[top--];

        while (low < high) {
            int pivot = arr[high];
            int i = low - 1;

            for (int j = low; j < high; j++) {
                if (arr[j] < pivot) {
                    i++;
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }

            int temp = arr[i + 1];
            arr[i + 1] = arr[high];
            arr[high] = temp;

            int pivotIndex = i + 1;

            if (pivotIndex - 1 - low > high - (pivotIndex + 1)) {
                if (low < pivotIndex - 1) {
                    stackLow[++top] = low;
                    stackHigh[top] = pivotIndex - 1;
                }
                low = pivotIndex + 1;
            } else {
                if (pivotIndex + 1 < high) {
                    stackLow[++top] = pivotIndex + 1;
                    stackHigh[top] = high;
                }
                high = pivotIndex - 1;
            }
        }
    }

    free(stackLow);
    free(stackHigh);
}

int linearSearch(int* arr, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int binarySearch(int* arr, int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insertRecursive(Node* root, int key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->key) {
        root->left = insertRecursive(root->left, key);
    } else if (key > root->key) {
        root->right = insertRecursive(root->right, key);
    }

    return root;
}

Node* insertIterative(Node* root, int key) {
    Node* newNode = createNode(key);

    if (root == NULL) {
        return newNode;
    }

    Node* parent = NULL;
    Node* current = root;

    while (current != NULL) {
        parent = current;

        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            free(newNode);
            return root;
        }
    }

    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    return root;
}

Node* searchBST(Node* root, int target) {
    Node* current = root;

    while (current != NULL) {
        if (current->key == target) {
            return current;
        } else if (target < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void selectRandomElements(int* sourceArray, int sourceSize, int* targetArray, int numElements) {
    bool* usedIndices = (bool*)calloc(sourceSize, sizeof(bool));
    if (usedIndices == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int count = 0;
    while (count < numElements) {
        int index = rand() % sourceSize;
        if (!usedIndices[index]) {
            usedIndices[index] = true;
            targetArray[count] = sourceArray[index];
            count++;
        }
    }

    free(usedIndices);
}
