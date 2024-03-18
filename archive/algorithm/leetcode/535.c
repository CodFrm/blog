#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hash[1024];
/** Encodes a URL to a shortened URL. */
char* encode(char* longUrl) {
    char* ret = malloc(30);
    int x = 0;
    for (int i = 0; i < strlen(longUrl); i++) {
        x += longUrl[i];
    }
    x %= 1024;
    hash[x] = malloc(1024);
    strcpy(hash[x], longUrl);
    // if(strlen(hahs[x])>=3){

    // }
    sprintf(ret, "http://tinyurl.com/%d", x);
    return ret;
}

/** Decodes a shortened URL to its original URL. */
char* decode(char* shortUrl) {
    const char* s = strrchr(shortUrl, '/');
    int x = atoi(&s[1]);
    return hash[x];
}
//直接返回原字符leetcode也是可以通过
// Your functions will be called as such:
// char* s = encode(s);
// decode(s);

int main() {
    char* s = encode("https://leetcode.com/problems/design-tinyurl");
    s = decode(s);
    getchar();
    return 0;
}