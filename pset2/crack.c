#include </home/etgonehomie/projects/cs50/libcs50-9.0.1/src/cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <crypt.h>

/*
1. program will accept only 1 command-line arguemtn: a hashed password
2. if program executed without meeting this 1 command-line argument criteria
   print an error of my choice and exit immediatly returning 1;
3. Program must proceed to crack the given password (as quickly as possible)
   printing the password in the clear followed by \n
4. Assume each password has been hashed with C's DES-based 
	(not MD5-based) crypt function
5. Assume each password is no longer than 5 characters. Gasp!
6. Assume that each password is composed entirely of alpha charc (upper/lower)

Examples:
./crack
Usage: ./crack hash

./crack 50cI2vYkF0YU2
LOL
*/

/*
DESCRIPTION
       crypt()  is  the  password  encryption  function.  It is based on the Data Encryption
       Standard algorithm with variations intended (among other things) to discourage use of
       hardware implementations of a key search.

       key is a user's typed password.

       salt  is  a  two-character  string chosen from the set [a-zA-Z0-9./].  This string is
       used to perturb the algorithm in one of 4096 different ways.

       By taking the lowest 7 bits of each of the first  eight  characters  of  the  key,  a
       56-bit  key  is  obtained.   This 56-bit key is used to encrypt repeatedly a constant
       string (usually a string consisting of all zeros).  The returned value points to  the
       encrypted  password, a series of 13 printable ASCII characters (the first two charac‐
       ters represent the salt itself).  The return value points to static data  whose  con‐
       tent is overwritten by each call.

       Warning:  the  key  space consists of 2**56 equal 7.2e16 possible values.  Exhaustive
       searches of this key space are possible using massively  parallel  computers.   Soft‐
       ware,  such as crack(1), is available which will search the portion of this key space
       that is generally used by humans for passwords.  Hence, password selection should, at
       minimum,  avoid  common  words and names.  The use of a passwd(1) program that checks
       for crackable passwords during the selection process is recommended.

       The DES algorithm itself has a few quirks which make the use of the crypt() interface
       a very poor choice for anything other than password authentication.  If you are plan‐
       ning on using the crypt() interface for a cryptography project, don't do  it:  get  a
       good book on encryption and one of the widely available DES libraries.

       crypt_r()  is  a  reentrant  version of crypt().  The structure pointed to by data is
       used to store result data and bookkeeping information.  Other than allocating it, the
       only  thing that the caller should do with this structure is to set data->initialized
       to zero before the first call to crypt_r().

       $ crypt("abc", "ab") = abFZSxKKdq5s6
       	> This says that the hashed encryption of "abc" using a salt of "ab" is abFZSxKKdq5s6
       		using the DES encryption method

*/

bool decrypt(string guess, string hash, int hash_len);

int main(int argc, string argv[]){
	// printf("a encrypted with salt ab is %s\n", crypt("a", "ab"));
	// printf("a encrypted with salt AB is %s\n", crypt("a", "AB"));

	// 312k possibilities with 5 char with upper/lower case alpha restrictions 50^5 
	// this formula is (possible letter value ^ # of letters in password)

	// what about the salt, what is the salt?
	// The salt is always the first 2 characters of the hash
	// with DES encryption the salt will be upper/lower alpha only
       
    // Ensure user inputs password to encrypt
    if(argc != 2){
    	printf("Please input 1-5 upper/lowercase alpha only password.\n");
            return 1;
    }

    string input_pw = argv[1];
    int pw_len = strlen(input_pw);
    if(pw_len > 5){
    	printf("Please input 1-5 upper/lowercase alpha only password.\n");
            return 1;
    }

    // Ensure password is all alpha
    for(int i = 0; i < pw_len; i++){
        if(!isalpha(input_pw[i])){
            printf("Please input 1-5 upper/lowercase alpha only password.\n");
            return 1;
        }
    }

    // Show user encrypted hash password
    string salt = get_string("What 2 alpha character key do you want to encrypt your password with? ");
       
    // Ensure salt is all alpha and only 2 char
    int salt_len = strlen(salt);
    if(salt_len != 2){
        printf("Please input 2 upper/lowercase alpha only key.\n");
        return 1; 
    } 
    for(int i = 0; i < salt_len; i++){
        if(!isalpha(salt[i])){
            printf("Please input 2 upper/lowercase alpha only key.\n");
            return 1;
        }
    }
       
    // Get the hashed password
    // Need to copy hash string into char or it gets overwritten by
	// future crypt function
    string hash_pw = crypt(input_pw, salt);
    int hash_len = strlen(hash_pw);
    char real_hash[hash_len];
	strcpy(real_hash, hash_pw);

    printf("your hashed password is: %s\n\n", hash_pw);
    printf("decrypting.....\n....\n...\n..\n");

    string guess_pw = NULL;
    bool found_pw = false;

    // Get list of alphs to try and decrypt
    char str1[2] = "a";
    char str2[3] = "aa";
    char str3[4] = "aaa"; 
    char str4[5] = "aaaa";
    char str5[6] = "aaaaa";

    string alpha_all = "abcdefghijklmnopqrstuvwxyz"
    	"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int alpha_len = strlen(alpha_all);
  	
  	// Try decrypting 1 char password
    for(int i = 0; i < alpha_len; i++){
    	// printf("alpha[%i]: %c\n", i, alpha_all[i]);
    	str1[0] = alpha_all[i];
    	// printf("str1: %s\n", str1);
   		found_pw = decrypt(str1, real_hash, hash_len);
   		if(found_pw){
   			printf("password is: %s\n", str1);
   			return 0;
   		}
    }

  	// Try decrypting 2 char password
    for(int i = 0; i < alpha_len; i++){
    	// printf("alpha[%i]: %c\n", i, alpha_all[i]);
    	str2[0] = alpha_all[i];
    	for(int j = 0; j < alpha_len; j++){
    		str2[1] = alpha_all[j];
    		// printf("str2: %s\n", str2);
    		found_pw = decrypt(str2, real_hash, hash_len);
   			if(found_pw){
   				printf("password is: %s\n", str2);
   				return 0;
   			}	
   		}
    }
    
    // Try decrypting 3 char password
    for(int i = 0; i < alpha_len; i++){
    	// printf("alpha[%i]: %c\n", i, alpha_all[i]);
    	str3[0] = alpha_all[i];
    	for(int j = 0; j < alpha_len; j++){
    		str3[1] = alpha_all[j];
    		// printf("str2: %s\n", str2);
    		for(int k = 0; k < alpha_len; k++){
    			str3[2] = alpha_all[k];
    			found_pw = decrypt(str3, real_hash, hash_len);
   				if(found_pw){
   					printf("password is: %s\n", str3);
   					return 0;
   				}	
   			}
   		}
    }

    // Try decrypting 4 char password
    for(int i = 0; i < alpha_len; i++){
    	// printf("alpha[%i]: %c\n", i, alpha_all[i]);
    	str4[0] = alpha_all[i];
    	for(int j = 0; j < alpha_len; j++){
    		str4[1] = alpha_all[j];
    		// printf("str2: %s\n", str2);
    		for(int k = 0; k < alpha_len; k++){
    			str4[2] = alpha_all[k];
    			for(int l = 0; l < alpha_len; l++){
    				str4[3] = alpha_all[l];
    				found_pw = decrypt(str4, real_hash, hash_len);
   					if(found_pw){
   						printf("password is: %s\n", str4);
   						return 0;
	   				}	
	   			}
   			}
   		}
    }

    // ~312 million possibilities.
    // Try decrypting 5 char password
    for(int i = 0; i < alpha_len; i++){
    	// printf("alpha[%i]: %c\n", i, alpha_all[i]);
    	str5[0] = alpha_all[i];
    	for(int j = 0; j < alpha_len; j++){
    		str5[1] = alpha_all[j];
    		// printf("str2: %s\n", str2);
    		for(int k = 0; k < alpha_len; k++){
    			str5[2] = alpha_all[k];
    			for(int l = 0; l < alpha_len; l++){
    				str5[3] = alpha_all[l];
    				for(int m = 0; m < alpha_len; m++){
    					str5[4] = alpha_all[m];
    					found_pw = decrypt(str5, real_hash, hash_len);
   						if(found_pw){
   							printf("password is: %s\n", str5);
   							return 0;
	   					}	
    				}
    			}
   			}
   		}
    }
    printf("pasword not found.\n");
	exit(0);
}

// Determine if the given guess is the correct password for
// the give hash. The hash is created using DES encryption.
// More specficially it is used using the crypt() function.
// This returns true if the guess is correct and false otherwise
bool decrypt(string guess, string hash, int hash_len){
	
	// Need to copy hash string into char or it gets overwritten by
	// future crypt function
	// char real_hash[hash_len];
	// strcpy(real_hash, hash);
	// printf("real hash: %s\n", real_hash);
	// printf("guess: %s\n", guess);

	// Salt for encryption is first 2 char of hash
	char salt[3] = "aa";
	salt[0] = hash[0];
	salt [1] = hash[1];
	// printf("salt is: %c%c\n", salt[0], salt[1]);


	// If guessed password matches hashed, return true, else false
	printf("guess: %s\n", guess);
	string guess_hash = crypt(guess, salt);
	// printf("guessed_hash = %s\n", guess_hash);
	if(strcmp(hash, guess_hash)){
		return false;
	}	
	return true;
}