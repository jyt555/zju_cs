#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 1024
int main(int argc, char *argv[]){
    FILE *infile,*outfile;
    int rc;
    if( argc < 3 )  {
        printf("usage: %s %s\n", argv[0], "infile outfile");
        exit(1);
    }
      
    outfile = fopen(argv[2], "wb" );
    infile = fopen(argv[1], "rb");
    unsigned char buf[MAXLEN];	
    if( outfile == NULL || infile == NULL )   {
        printf("%s, %s",argv[1],"not exit\n");
        exit(1);
    }   
   /*续*/

    while( (rc = fread(buf,sizeof(unsigned char), MAXLEN,infile)) != 0 )
    {
        fwrite( buf, sizeof( unsigned char ), rc, outfile );
} 
    fclose(infile);
    fclose(outfile);
    system("PAUSE"); 
    return 0;
}
