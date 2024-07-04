#include <stdio.h>
#define MAX_NUM 100000
#define INFINITY 1000000
void swap(int* a, int* b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int main(void)
{

    int n;  // the number of items and the coupons (<= 10^5)
    int d;  // the amount of money (<= 10^6)
    int price[MAX_NUM+5];   // the price of each items
    int coupons[MAX_NUM+5] = {0};   // the coupons
    int num = 0;    // the max_number of the items we can buy

    int index[MAX_NUM+5] = {0};
    // index[1] = 3 means item2(ascending sort) use coupons4(descending sort) next time
    int min_price;  // the min price for next item we buy
    int item;       // pointer for item we buy
    int flag = 0;

    scanf("%d %d", &n, &d);
    for(int i = 0; i < n; i++)
        scanf("%d", &price[i]);
    for(int i = 0; i < n; i++)
        scanf("%d", &coupons[i]);

    for(int i = 1; i < n; i++){ // bubble sort
        for(int j = 0; j < n - i; j++)
        {
            if(price[j] > price[j+1]) swap(&price[j], &price[j+1]);         // ascending order
            if(coupons[j] < coupons[j+1]) swap(&coupons[j], &coupons[j+1]); // descending order
        }
    }
    // for(int i = 0; i < n; i++) printf("%d ",coupons[i]); printf("(sort)\n"); //* test bubble sort
    while(!flag)
    {
        min_price = INFINITY;
        for(int i = 0; i < n; i++)
        {
            if(min_price > price[i] - coupons[index[i]])    // for every item, the min price is price[i] - coupons[index[i]]
            {
                min_price = price[i] - coupons[index[i]];
                item = i;
            }
        }
        if(d < min_price)   // can't buy item anymore
        {
            flag = 1;
            break;
        }else{
            // printf("buy item%d, cost: %d\n", item, min_price); //* test by print item we buy
            d -= min_price;
            num++;
            index[item]++;
        }
    }
    printf("%d %d", num, d);
    return 0;
}