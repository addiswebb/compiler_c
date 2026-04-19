// RUN: %cc %s -o %t
// RUN: %check_exit 102 %t

int global_int = 42;
double global_double;
int global_array[5];
int *global_ptr;

int main() {
    int result;

    // Assign to uninitialized globals
    global_double = 3.14;
    global_array[0] = 10;
    global_array[1] = 20;
    global_array[2] = 30;
    global_ptr = &global_int;

    // Use the globals
    result = global_int + global_array[0] + global_array[1] + global_array[2]; // 42 + 10 + 20 + 30 = 102

    return result;
}
