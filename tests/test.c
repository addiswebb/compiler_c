int main() {
    // char res[] = "abcdefg";
    // char res[] = "abcdefgh"; // Illegal instruction
    // char res[10] = "abcdefghi"; // Segfault
    char res[] = "abcdefghi123124531"; // Segfault
    return sizeof(res);
}
