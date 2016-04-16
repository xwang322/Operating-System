#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <math.h>

#define isunsigned(a) (a>=0 && ~a>=0)

void
usage() 
{
   fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
}

// Main function
int 
main (int argc, char *argv[])
{
   // argument
   char *infile = "/no/such/file"; 
   char *outfile = "/no/such/file";
   unsigned lowvalue = 0;
   unsigned lowvalue_tmp = 0;
   unsigned highvalue = 0;
   unsigned long highvalue_tmp = 0;
   int argu_count = 0;
   // input params
   int c;
   opterr = 0;
   while ((c = getopt(argc, argv, "i:o:l:h:")) != -1){
        argu_count++;         
        switch (c){
         case 'i':
              infile = strdup(optarg);
              break; 
         case 'o':
              outfile = strdup(optarg);
              break;
         case 'l':
              lowvalue_tmp = strtoul(optarg, NULL, 10);
              if (lowvalue_tmp > pow(2,32)-1){
                 fprintf(stderr,"Error: Invalid range value\n");
                 exit(1);
              }
              else lowvalue = (int)lowvalue_tmp;
              int i = 0;
              for (i = 0; i < strlen(optarg); i++){
                  if (optarg[i] < 48 || optarg[i] > 57){
                     fprintf(stderr, "Error: Invalid range value\n");
                     exit(1);
                     break;
                  }
              }
              break;
         case 'h':
              highvalue_tmp = strtoul(optarg, NULL, 10);
              if (highvalue_tmp > pow(2,32)-1){
                 fprintf(stderr,"Error: Invalid range value\n");
                 exit(1);
              }
              else highvalue = (int)highvalue_tmp;
              int j = 0;
              for (j = 0; j < strlen(optarg); j++){
                  if (optarg[j] < 48 || optarg[j] > 57){
                     fprintf(stderr, "Error: Invalid range value\n");
                     exit(1);
                     break;
                  }
              }
              break;
         default:
              break;
         }
   }
   if (argu_count != 4){
      fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
      exit(1);
   }
   // Invalid range
   if (highvalue < lowvalue){    
      fprintf(stderr,"Error: Invalid range value\n");
      exit(1);
   }
  
   // Invalid files 
   int fd_in = open(infile, O_RDONLY); 
   if (fd_in < 0) {
      fprintf(stderr, "Error: Cannot open file %s\n", infile);
      exit(1);
    }

    // Obtain the file information 
    struct stat filestatus; 
    if (fstat(fd_in, &filestatus) < 0){
       fprintf(stderr, "Error: Cannot get the file information\n");
       exit(1);
    }

    // Allocate memory 
    rec_t *r = (rec_t *)malloc(filestatus.st_size); 
    if (r == NULL) {
       fprintf(stderr, "Error: Fail to allocate memory\n");
       close(fd_in);
       free(r);
       exit(1);
    }

    // Read input file 
    int i = 0;
    while (1) {	
        int rc;
        rc = read(fd_in, &r[i], sizeof(rec_t)); 
        if (rc == 0) 
           break;
        if (rc < 0) {
           fprintf(stderr, "Error: Cannot read file %s\n", infile);
           close(fd_in);
           free(r);
           exit(1);
        }
        i = i+1;
    }

    // Comparison and sort the array 
    int
    compar (const void *a, const void *b){
       rec_t *Akey = (rec_t *)a;
       rec_t *Bkey = (rec_t *)b;
       return (Akey->key - Bkey->key);
    }			
    qsort((void *)r, i, sizeof(rec_t), compar);

    // Write output file 
    int fd_out = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd_out < 0) {
       fprintf(stderr, "Error: Cannot open file %s\n", outfile);
       exit(1);
    }
    int j;
    for (j = 0; j < i; j++){
        if (r[j].key >= lowvalue && r[j].key <= highvalue){ 
           write(fd_out, &r[j], sizeof(rec_t)); 
        }
     }

     // clean up memory
     free(r);
     free(infile);
     free(outfile);
     close(fd_in);
     close(fd_out);
     exit(0);
     return 0;
}











