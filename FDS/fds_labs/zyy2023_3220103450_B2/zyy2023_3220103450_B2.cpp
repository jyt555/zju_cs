/* Author: Jiang Yutong; ID:3220103450; No:02 */
#include<bits/stdc++.h>
 
int main(){
    int N, M;
    int k, pre;
    int flag = 0;
    std::multiset<int> curr, next; /* current list and next list */
    std::multiset<int>::iterator p;

    scanf("%d %d", &N, &M);
    while(N--){
        scanf("%d", &k);
        if(flag){
            if(k >= pre) curr.insert(k);
            else next.insert(k);

            if(curr.empty()){ /* current list is empty */
                flag = 0;
                curr = next;
                next.clear();
                printf("\n");
            }else{
                pre = *curr.begin();
                printf(" %d", pre);
                curr.erase(curr.begin());
            }
        }else curr.insert(k);

        if(!flag && curr.size() == M){
            flag = 1; /* full, flag = 1 */
            printf("%d", *curr.begin());
            pre = *curr.begin();
            curr.erase(curr.begin());
        }
    }
    if(flag && !curr.empty()) printf(" "); /* continue to print this line */
    if(!curr.empty()){
        printf("%d", *curr.begin());
        p = curr.begin();
        p++;
        for(; p != curr.end(); p++) printf(" %d", *p);
    }
    if(!curr.empty() && !next.empty()){
        printf("\n");
    }
    if(!next.empty()){
        p = next.begin();
        printf("%d", *(p++));
        for(; p != next.end(); p++) printf(" %d", *p);
        printf("\n");
    }
    return 0;
}