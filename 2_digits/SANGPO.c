#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <mpi.h>

//Password Cracking using MPI 2 numbers
 //To compile:
    // mpicc -o SANGPO SANGPO.c -lrt -lcrypt
     
  //To run 3 processes:
   // mpirun -n 3 ./SANGPO


int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {

"$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",

"$6$KB$bLt//JygennXNkYWLUEsGM5FVpdhky4h2ebcL9Z7DWbOfirSEZ96PuIYeBBLNR5sQJfKcp8vOMB4U3DLxlw/Y.",

"$6$KB$Vjiz/5R8IhhNOuzTIkhIo0Jvx577qqDqbtdMPWTDYp7/JcB7xBTXwMqtvZG9t5xtyzNWhi9NHjxLMkSj0kNHF1",

"$6$KB$oqxIeObweFV8EcKpAtMtw4yFMwB0SaV.JGNGwc.k9ZIEVETjPVE6bEYQ2JqKaLzGyLM3Afk6/9Ohzf/0io1a40"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void kernel_function1(char *salt_and_encrypted){
  int P, Q, R;     
  char salt[7];    

  char plain[7];  
  char *enc;       
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(P='A'; P<='M'; P++){
    for(Q='A'; Q<='Z'; Q++){
      for(R=0; R<=99; R++){
	printf("Instance 1:");
	sprintf(plain, "%c%c%02d",P, Q, R);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void kernel_function2(char *salt_and_encrypted){
  int P, Q, R;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(P='N'; P<='Z'; P++){
    for(Q='A'; Q<='Z'; Q++){
      for(R=0; R<=99; R++){
	printf("Instance 2:");
	sprintf(plain, "%c%c%02d",P, Q, R);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	} else {
	  printf(" %-8d%s %s\n", count, plain, enc);
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}


int time_difference(struct timespec *start, struct timespec *finish,
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

int main(int argc, char** argv) {
 struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  int size, rank;
int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){

      int x;
    
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);


 
    } else if(rank==1){
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function1(encrypted_passwords[i]);
        }
      }
      else{
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function2(encrypted_passwords[i]);
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
