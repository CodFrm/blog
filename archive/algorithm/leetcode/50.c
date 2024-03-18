double myPow(double x, int n) {
    double ret = 1;
    for (int i = n; i != 0; i = i / 2) {
        if (i % 2 != 0) ret *= x;
        x *= x;
    }
    return n > 0 ? ret : 1 / ret;
}
/*
double myPow(double x, int n) {
    //如果n为零，表明幂数为0，直接返回1
    if(n == 0) 
        return 1;      
    //如果n为零，表明幂数为0，直接返回x
    if(n == 1) 
        return x;    
    //p值相当于将幂数折半，减少重复计算
    int p=n/2;
    //如果n为负数，那么将x值取其倒数，然后再将p的值乘以-1，这样就相当于
    //求x倒数的n次幂，你为正数
    if(n < 0) { 
        //p值改为正数
        p=-p;
        //x值取其倒数
        x = 1 / x; 
    }      
    double res = myPow(x, p);
    //为了提高时间复杂度，避免重复计算，可以将n不断除二来减少计算量
    //当n为32时，除二就为16，那么只需要将x^16与自身相乘即可，若n为奇数
    //那么只需要再乘以x即可
    if(n % 2 == 0) 
        return res * res;  
    return res * res * x;   
}
*/