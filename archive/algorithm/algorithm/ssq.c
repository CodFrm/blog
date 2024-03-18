#include <stdio.h>

int main(){
    for(int i=0;i<=3;i++){
        for(int n=0;n<=3;n++){
            if(8-i-n<=6){
                printf("red:%d;yellow:%d,green:%d\n",i,n,8-i-n);
            }
        }
    }
    getchar();
    return 0;
}