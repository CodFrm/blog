#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * 最长回文子串
 * https://leetcode-cn.com/problems/longest-palindromic-substring/description/
 */

/**
 * 看网上的Manacher算法
 */
char *longestPalindrome(char *s)
{
    if (strlen(s) <= 1)
    {
        return s;
    }
    char new_s[2010] = {0};
    int p[2010] = {0};
    // memset(p, 0, sizeof(int) * (strlen(s) * 2 + 1)); //创建p
    // memset(new_s, 0, strlen(s) * 2 + 1);             //构建一个新的字符串
    //给字符串填充#分割
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        new_s[i * 2] = '#';
        new_s[(i * 2) + 1] = s[i];
    }
    new_s[len * 2] = '#';
    p[0] = 1;
    int id = 0, mx = 0, max_pos = 0, max_len = 0;
    for (int i = 1; i < strlen(new_s); i++)
    {
        if (i < mx)
        {
            p[i] = p[2 * id - i] > mx - i ? mx - i : p[2 * id - i];
        }
        else
        {
            p[i] = 1;
        }
        while (i - p[i] >= 0 && new_s[i - p[i]] == new_s[i + p[i]])
        {
            p[i]++;
        }
        if (mx < i + p[i])
        {
            id = i;
            mx = i + p[i];
        }
        if (max_len < p[i] - 1)
        {
            max_pos = i;
            max_len = p[i] - 1;
        }
    }
    char *ret = malloc(max_len + 1); //返回的字符串
    memset(ret, 0, max_len);
    int pos = max_pos / 2 + max_pos % 2 - max_len / 2 - max_len % 2;
    memcpy(ret, &s[pos], max_len);
    return ret;
}
/*
char* longestPalindrome(char* s) {
    int max = 1, len;
    int left, right;
    int maxLeft = 0;
    int totalLen = strlen(s);
    int i,j;
    char *res = (char *) calloc(1, 1001);
    if(totalLen == 1) {
        res[0] = s[0];
        return res;
    }
    for(i = 0; i < totalLen - 1; /*i++) {
        if(totalLen-i<=max/2)
            break;
        //first, pass consecutive numbers equal to each other
        right = i;
        left = i;
        while((right < totalLen - 1) && (s[right] == s[right + 1])) {
            right ++;
        }
        
        //second, set i = r + 1, for there is no len larger when start from i to r
        i = right + 1;
        
        //third, count those s[right] == s[left]
        while((left > 0) && (right < totalLen - 1) && (s[left - 1] == s[right + 1])) {
            right++;
            left--;
        }
        
        if(right - left + 1 > max) {
            maxLeft = left;
            max = right - left + 1;
        }
    }
    
   
    strncpy(res, s + maxLeft, max);
    return res;

}
*/
void print_r_c(char *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c\t", a[i]);
    }
    printf("\n");
}

void print_r(int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

int main()
{
    printf("%s\n", longestPalindrome("caba"));
    printf("%s\n", longestPalindrome("dddddddd"));
    printf("%s\n", longestPalindrome("aca"));
    printf("%s\n", longestPalindrome("feef"));
    printf("%s\n", longestPalindrome("abcba"));
    printf("%s\n", longestPalindrome("daomdukomcayjwgmmetypncdeixarhbectjcwftjjtwjrctixtrsehegwlfotpljeeqhntacfihecdjysgfmxxbjfeskvvfqdoedfxriujnoeteleftsjgdsagqvcgcdjbxgmguunhbjxyajutohgbdwqtjghdftpvidkbftqbpeahxbfyamonazvubzirhqseetaneptnpjbtrtitttxsyjckjvwtrmuwgidkofvobrwrffzrpnxbectsydbvswstfiqranjzhsebjnmprjoirqkgttahrivkcjtitdcpohwwerwgrdivqbltfnigavydxpmitttjjzyrmpaptkczzgnsovebvxezkkovgqegctxacvjfqwtiycnhartzczcgosiquhmdbljjzyrnmffcwnkyzytnsvyzayrieqyrfpxintbbiyrawxlguzaisedwabpzvevlejadztuclcpwvonehkhknicdksdcnjfaoeaqhcdkdtywilwewadcnaprcasccdcnvdgjdqirrsqwzhqqorlhbgpelpupdvuynzybcqkffnnpocidkkigimsa"));
    getchar();
    return 0;
}