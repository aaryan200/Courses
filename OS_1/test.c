#include<stdio.h>
#include<stdbool.h>
#include<math.h>

/*Return true if num is a perfect number
else return false*/
bool check_perfect(int num)
{
    if (num <= 1)
        return false;
    float exact_sqrt=sqrt(num);
    int sqrt_of_num=floor(exact_sqrt);
    printf("Sqrt = %d\n",sqrt_of_num);
    int sum_of_factors = 1; // 1 is a trivial factor
    if (exact_sqrt-sqrt_of_num<=1.0e-5) sum_of_factors-=sqrt_of_num;
    for (int i = 2; i <= sqrt_of_num; i++)
    {
        if (num % i == 0)
            sum_of_factors += (i+num/i);
    }
    if (sum_of_factors == num)
        return true;
    return false;
}

int main() {
    int n;
    scanf("%d",&n);
    printf("%d\n",check_perfect(n));
    return 0;
}