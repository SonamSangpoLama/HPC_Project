#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o sangpo sangpo.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./sangpo > sangpo_results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$H8s0k9/1RQ783G9gF69Xkn.MI.Dq5Ox0va/dFlkknNjO7trgekVOjTv1BKCb.nm3vqxmtO2mOplhmFkwZXecz0",

"$6$KB$bLt//JygennXNkYWLUEsGM5FVpdhky4h2ebcL9Z7DWbOfirSEZ96PuIYeBBLNR5sQJfKcp8vOMB4U3DLxlw/Y.",

"$6$KB$Vjiz/5R8IhhNOuzTIkhIo0Jvx577qqDqbtdMPWTDYp7/JcB7xBTXwMqtvZG9t5xtyzNWhi9NHjxLMkSj0kNHF1",

"$6$KB$oqxIeObweFV8EcKpAtMtw4yFMwB0SaV.JGNGwc.k9ZIEVETjPVE6bEYQ2JqKaLzGyLM3Afk6/9Ohzf/0io1a40"
};
/**
 Required by lack of standard function in C.   
*/

void sangpo(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations#
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void thread_possix()
{
  int s;
pthread_t t1, t2;

    void *kernel_function_1();
    void *kernel_function_2();

     for(s=0;s<n_passwords;s<s++) {
    pthread_create(&t1, NULL,kernel_function_1, encrypted_passwords[s]);
    pthread_create(&t2, NULL,kernel_function_2, encrypted_passwords[s]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
 }
}

void *kernel_function_1(char *salt_and_encrypted){
  int s, t, u;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  sangpo(salt, salt_and_encrypted, 0, 6);

  for(s='A'; s<='M'; s++){
    for(t='A'; t<='Z'; t++){
      for(u=0; u<=99; u++){
        sprintf(plain, "%c%c%02d", s,t,u);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
        else {
          printf(" %-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
  printf("%d solutions explored\n", count);
}


void *kernel_function_2(char *salt_and_encrypted){
  int a, r, x;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  sangpo(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(r='A'; r<='Z'; r++){
      for(x=0; x<=99; x++){
        sprintf(plain, "%c%c%02d", a,r,x);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
	else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

//Calculating time

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
	    ds--;
	    dn += 1000000000; 
  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}
int main(int argc, char *argv[])
{
  	
	struct timespec start, finish;   
  	long long int time_elapsed;

  	clock_gettime(CLOCK_MONOTONIC, &start);

  	
	
    		thread_possix();
  	
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		                                 (time_elapsed/1.0e9)); 
  return 0;
}
