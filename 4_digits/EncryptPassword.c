#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>

/******************************************************************************
  This program is used to set challenges for password cracking programs. 
  Encrypts using SHA-512.
  
  Compile with:
    cc -o EncryptPassword EncryptPassword.c -lcrypt
    
  To encrypt the password "pass":
    ./EncryptPassword pass
    
  It doesn't do any checking, just does the job or fails ungracefully.

  Dr Kevan Buckley, University of Wolverhampton, 2017
******************************************************************************/

#define SALT "$6$KB$"

int main(int argc, char *argv[]){
  
  printf("%s\n", crypt(argv[1], SALT));

  return 0;
}
