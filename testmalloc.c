#include <stdio.h>
#include <stdlib.h>

// Assume your malloc implementation is included here
void *malloc(size_t size);

struct person {
    char name[50];
    int age;
};

int main() {
    // Test 1: Allocate memory for a single integer
    int *p = malloc(sizeof(int));
    if (p == NULL) {
        printf("Memory allocation failed for integer\n");
        return 1;
    }
    *p = 42;
    printf("Allocated integer: %d\n", *p);

    // Test 2: Allocate memory for a struct
    struct person *person_ptr = malloc(sizeof(struct person));
    if (person_ptr == NULL) {
        printf("Memory allocation failed for struct person\n");
        return 1;
    }
    snprintf(person_ptr->name, sizeof(person_ptr->name), "John Doe");
    person_ptr->age = 30;
    printf("Allocated struct person: Name = %s, Age = %d\n", person_ptr->name, person_ptr->age);

    // Test 3: Allocate memory for an array of integers
    int *arr = malloc(5 * sizeof(int));  // Allocating space for 5 integers
    if (arr == NULL) {
        printf("Memory allocation failed for array\n");
        return 1;
    }
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    printf("Allocated array: ");
    for (int i = 0; i < 5; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Free memory (you would implement free in your own version)
    // free(p);
    // free(person_ptr);
    // free(arr);

    return 0;
}

