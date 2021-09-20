#include "envelope.h"
#include <log.h>
#include <stdint.h>

extern "C"{ 

  char* appendToString(const char *string, char *suffix) {
  char *appenedString = (char*)malloc(strlen(string) + strlen(suffix) + 1);

  if(appenedString == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(1);
  }

  sprintf(appenedString, "%s%s", string, suffix);
  return appenedString;
}

    void free_string(char *str){
        //Free native memory in C which was allocated in C
        free(str);
    }



    int createRsakeypair(const char *str,int length){
        __android_log_print(ANDROID_LOG_ERROR, "flutter1","%s",strerror(errno));
        

        envelope env;
        env.generateRsaKeypair();
        __android_log_print(ANDROID_LOG_ERROR, "flutter2","%s",strerror(errno));

        FILE *rsa_pkey_file;
      rsa_pkey_file = fopen(appendToString(str, (char*)"/pu.txt"), "wb");
      __android_log_print(ANDROID_LOG_ERROR, "ouput","%s",str);
    if (!rsa_pkey_file)
    {
         __android_log_print(ANDROID_LOG_ERROR, "flutter3","%s",strerror(errno));

        fprintf(stderr, "Error create PEM RSA Public Key File.\n");
        //exit(2);
        return -1;
    }
     FILE *rsa_prikey_file;
     rsa_prikey_file = fopen(appendToString(str, (char*)"/pr.txt"), "wb");
    if (!rsa_prikey_file)
    {
         __android_log_print(ANDROID_LOG_ERROR, "flutter4","%s",strerror(errno));

        fprintf(stderr, "Error create PEM RSA Private Key File.\n");
        //exit(2);
        return -1;
    }

     __android_log_print(ANDROID_LOG_ERROR, "flutter5","%s",strerror(errno));

  
     // Write the RSA keys to stdout
     env.writeKeyToFile(rsa_prikey_file, KEY_SERVER_PRI);
     env.writeKeyToFile(rsa_pkey_file, KEY_SERVER_PUB);
     
     fprintf(stderr, "Successfully generate RSA key pair.\n");

     fclose(rsa_pkey_file);
     fclose(rsa_prikey_file);
    __android_log_print(ANDROID_LOG_ERROR, "flutter6","%s",strerror(errno));
     return 0;
    
    }

    int encrypt(const char *str){
        envelope env;

        FILE *rsa_pkey_file;
        rsa_pkey_file = fopen("", "rb");
        if (!rsa_pkey_file)
        {
        
        fprintf(stderr, "Error loading PEM RSA Public Key File.\n");
        exit(2);
        }

    FILE *encrypt_file = fopen("", "rb");
    if (!encrypt_file)
    {
        
        fprintf(stderr, "Error to open encrypt_file.\n");
        exit(2);
    }

    FILE *decrypt_file = fopen(appendToString("",(char*)".enc"), "wb");
    if (!decrypt_file)
    {
        
        fprintf(stderr, "Error to write encrypt_file.\n");
        exit(2);
    }

   fprintf(stderr, "do_evp_seal");

    int rv = env.do_evp_seal(rsa_pkey_file, encrypt_file, decrypt_file);

    fclose(rsa_pkey_file);
    fclose(encrypt_file);
    fclose(decrypt_file);

    return rv;

    }






    
}




// int main(int argc, char **argv)
// {
//     if (argc != 4)
//     {
//         fprintf(stderr, "Generate RSA keypair usage: aes_evo(0,Public Key filename,Private Key name)\n");
//         fprintf(stderr, "Encrypt envelope usage: aes_evo(1,PublicKey filename,Encrypt filename)\n");
//         fprintf(stderr, "Decrypt envelope usage: aes_evo(2,PrivateKey filename,Encrypted filename)\n");
//         exit(1);
//     }

//     envelope env;
//     int rv=0;

//     if(*argv[1]=='0'){//generateRsaKeypair
//       env.generateRsaKeypair();
//       FILE *rsa_pkey_file;
//       rsa_pkey_file = fopen(appendToString(argv[2], (char*)".txt"), "wb");
//     if (!rsa_pkey_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error create PEM RSA Public Key File.\n");
//         exit(2);
//     }
//      FILE *rsa_prikey_file;
//      rsa_prikey_file = fopen(appendToString(argv[3], (char*)".txt"), "wb");
//     if (!rsa_prikey_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error create PEM RSA Private Key File.\n");
//         exit(2);
//     }
  
//      // Write the RSA keys to stdout
//      env.writeKeyToFile(rsa_prikey_file, KEY_SERVER_PRI);
//      env.writeKeyToFile(rsa_pkey_file, KEY_SERVER_PUB);
     
//      fprintf(stderr, "Successfully generate RSA key pair.\n");

//      fclose(rsa_pkey_file);
//      fclose(rsa_prikey_file);
    
//     }else if(*argv[1]=='1'){//encrypt
//     FILE *rsa_pkey_file;
//     rsa_pkey_file = fopen(argv[2], "rb");
//     if (!rsa_pkey_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error loading PEM RSA Public Key File.\n");
//         exit(2);
//     }

//     FILE *encrypt_file = fopen(argv[3], "rb");
//     if (!encrypt_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error to open encrypt_file.\n");
//         exit(2);
//     }

//     FILE *decrypt_file = fopen(appendToString(argv[3],(char*)".enc"), "wb");
//     if (!decrypt_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error to write encrypt_file.\n");
//         exit(2);
//     }

//    fprintf(stderr, "do_evp_seal");

//     rv = env.do_evp_seal(rsa_pkey_file, encrypt_file, decrypt_file);

//     fclose(rsa_pkey_file);
//     fclose(encrypt_file);
//     fclose(decrypt_file);
//     }else if(*argv[1]=='2'){//decrypt
//     FILE *rsa_pkey_file;
//     rsa_pkey_file = fopen(argv[2], "rb");
//     if (!rsa_pkey_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error loading PEM RSA private Key File.\n");
//         exit(2);
//     }

//     FILE *encrypt_file = fopen(argv[3], "rb");
//     if (!encrypt_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error to open encrypt_file.\n");
//         exit(2);
//     }

//     FILE *decrypt_file = fopen(appendToString(argv[3],(char*)".dec"), "wb");
//     if (!decrypt_file)
//     {
//         perror(argv[1]);
//         fprintf(stderr, "Error to write decrypt_file.\n");
//         exit(2);
//     }

//    fprintf(stderr, "do_evp_unseal");

//     rv = env.do_evp_unseal(rsa_pkey_file, encrypt_file, decrypt_file);

//     fclose(rsa_pkey_file);
//     fclose(encrypt_file);
//     fclose(decrypt_file);
//     }
//     return rv;
// }