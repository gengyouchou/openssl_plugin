#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
//#include <arpa/inet.h> /* For htonl() */
//#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#pragma comment(lib,"ws2_32.lib")


//#ifndef CRYPTO_H
//#define CRYPTO_H

#define RSA_KEYLEN 2048
//#define AES_ROUNDS 6

//#define PSEUDO_CLIENT

//#define USE_PBKDF

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
//#define KEY_CLIENT_PUB 2
//#define KEY_AES        3
//#define KEY_AES_IV     4

class envelope {
  public:
    envelope();
    ~envelope();

    //int getLocalPublicKey(unsigned char **publicKey);
    //int getLocalPrivateKey(unsigned char **privateKey);
    int writeKeyToFile(FILE *file, int key);
    int do_evp_seal(FILE *rsa_pkey_file, FILE *in_file, FILE *out_file);
    int do_evp_unseal(FILE *rsa_pkey_file, FILE *in_file, FILE *out_file);
    int generateRsaKeypair();

  private:
    static EVP_PKEY *localKeypair;

    //EVP_CIPHER_CTX *rsaEncryptContext;
    //EVP_CIPHER_CTX *aesEncryptContext;

    //EVP_CIPHER_CTX *rsaDecryptContext;
    //EVP_CIPHER_CTX *aesDecryptContext;

    //unsigned char *aesKey;
    //unsigned char *aesIv;

    //size_t aesKeyLength;
    //size_t aesIvLength;

    //int init();
    
};

//#endif
