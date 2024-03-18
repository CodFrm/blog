int main()
{
    int a = 0;
    for (int i = 0; i < 3; i++)
    {
        a += i;
    }
}
//gcc -g -c test.c
//objdump -d -M intel -S test.o