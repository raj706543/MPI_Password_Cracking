#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
/***********************************************************************
  Compile with:
    mpicc -o password_cracking_mpi_ques_a password_cracking_mpi_ques_a.c -lcrypt

  we can redirect result by applying

    mpirun -n 3 ./password_cracking_mpi_ques_a > results.txt

************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$rqui/Gfisf4qvsogixADaVpkVsOGAghXxKdBS0r3wCNUJi./Dn42XESctUs/XJrtDNvJJrw/am4JbYLwyGh3k/",

"$6$KB$VUd.nPzI.2FPsDejNAQBnLd871l2Wng9cLZ394uJoxGzGjIQx3nncdQ0VZLCm8pYPedT/1O2ZodYe8pSc2rcr0",

"$6$KB$ETQF2cDxOEsAHXdew41jrT9AoTAmRRttmKIWKsRb/MMIb2DHDDUk8sv.5wA8PHOPYv3DTSdYMs6SHlniD4N0o.",

"$6$KB$iVaMnAP0YFgiGcQUvehyr4RYZycLZZU5.hD8qHVJU.NCSCd/8ZjpHQxhF06udW7KdJFZ4Zx8v8InSeUh/yv1B."
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the 
*/

void function_1(char *salt_and_encrypted){
  int a, y, c, e;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(y='A'; y<='Z'; y++){
      for(c=0; c<=99; c++){
	for(e=0; e<=99; e++){
  //printf("Instance 1");
        sprintf(plain, "%c%c%02d%2d", a, y, c, e); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
	}
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int a, y, c, e;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(y='A'; y<='Z'; y++){
      for(c=0; c<=99; c++){
	for(e=0; e<=99; e++){
        //printf("Instance 2");
        sprintf(plain, "%c%c%02d%2d", a, y, c, e); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
	}
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}
int time_difference(struct timespec *start, 
                    struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(){

  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 

int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){
      int a;
      int y;
      int i;
	MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
      MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){
	int i;
        int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		function_1(encrypted_passwords[i]);
	}
      }
	else if(rank == 2){
	int i;
      	int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		function_2(encrypted_passwords[i]);
	}
	}
    }
}  
  MPI_Finalize(); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
	(time_elapsed/1.0e9));


  return 0;
}


