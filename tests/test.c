typedef struct {
    int count;
    int capacity;
    int element_size;
    void *data;
} Array;
void *get(const Array *arr, int index);
typedef struct Type Type;
typedef struct {
    const char *name;
    Type *type;
    int offset;
} StructMember;

struct Type {
    union {
        struct {
            char *name;
            Array members_array;
        } _struct;
    };
};
static inline StructMember *get_struct_member(const Type *struct_t, int index) {
    return (StructMember *)get(&struct_t->_struct.members_array, index);
}
