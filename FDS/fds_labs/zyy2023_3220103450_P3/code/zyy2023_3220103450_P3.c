#include <stdio.h>
#include <stdlib.h>

#define MaxNum 1001
int Nv, Ne; /* Nv is the number of vertices, and Ne is the number of the edges */
int M[MaxNum][MaxNum] = {0}; /* the adjacent matrix of the graph */
int Dijkstra();
int main(void)
{
    int k;  /* there are totally k cases */
    int i, opr1, opr2, weight;

    scanf("%d %d",&Nv, &Ne);
    for(i=0; i<Ne; i++){
        scanf("%d %d %d",&opr1, &opr2, &weight);
        M[opr1][opr2] = M[opr2][opr1] = weight; /* create the adjacent matrix of the graph */
    }
    scanf("%d",&k);
    for(i=0; i<k; i++){
        if(Dijkstra()) printf("Yes\n");
        else printf("No\n");
    }
    return 0;
}
int Dijkstra()
{
    int i, p, Seq[MaxNum]; /* the array for sequence */
    int Sel[MaxNum] = {0}, Dist[MaxNum] = {0}; /* the array for selected vertices, and the array for their distances */

    for(i=0; i<Nv; i++) scanf("%d", &Seq[i]); /* load the sequence */
    Sel[0] = Seq[0];
    for(i=1; i<Nv; i++) /* initialize the distance array */
        if(M[Seq[0]][Seq[i]]) Dist[Seq[i]] = M[Seq[0]][Seq[i]];
    for(i=1; i<Nv; i++){ /* whether vertex_i is to be selected next */
        for(p=i+1; p<Nv; p++)
            if(Dist[Seq[p]])
                if(Dist[Seq[p]] < Dist[Seq[i]]) return 0;   /* if vertex_i is not the smallest node, it's not Dijkstra sequence */
        Sel[i] = Seq[i]; /* else vertex_i is selected */
        for(p=i+1; p<Nv; p++)
            if(M[Seq[i]][Seq[p]]) /* if vertex_p isn't adjacent to vertex_i, the distance is no need to update */
                if(!Dist[Seq[p]] || Dist[Seq[p]] > Dist[Seq[i]] + M[Seq[i]][Seq[p]]) /* remember to change distance 'zero' */
                    Dist[Seq[p]] = Dist[Seq[i]] + M[Seq[i]][Seq[p]]; /* update the distance array */
    }
    return 1;
}