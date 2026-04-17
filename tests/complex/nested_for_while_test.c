// RUN: %cc %s -o %t
// RUN: %check_exit 10 %t

int main() {
    int sum = 0;
    for (int i = 1; i <= 3; i++) {
        int j = i;
        while (j > 0) {
            sum += j;
            j--;
        }
    }
    // i=1: sum+=1
    // i=2: sum+=2+1=3
    // i=3: sum+=3+2+1=6
    // total=1+3+6=10
    return sum;
}
