#include "envelope.h"
#include <log.h>
#include <stdint.h>

extern "C"
{

    char *appendToString(const char *string, char *suffix)
    {
        char *appenedString = (char *)malloc(strlen(string) + strlen(suffix) + 1);

        if (appenedString == NULL)
        {
            fprintf(stderr, "Failed to allocate memory\n");
            exit(1);
        }

        sprintf(appenedString, "%s%s", string, suffix);
        return appenedString;
    }

    void free_string(char *str)
    {
        //Free native memory in C which was allocated in C
        free(str);
    }

    int createRsakeypair(const char *keyPath)
    {
        __android_log_print(ANDROID_LOG_ERROR, "flutter1", "%s", strerror(errno));

        envelope env;
        env.generateRsaKeypair();
        __android_log_print(ANDROID_LOG_ERROR, "flutter2", "%s", strerror(errno));

        FILE *rsa_pkey_file;
        rsa_pkey_file = fopen(appendToString(keyPath, (char *)"/pu.txt"), "wb");
        __android_log_print(ANDROID_LOG_ERROR, "ouput", "%s", keyPath);
        if (!rsa_pkey_file)
        {
            __android_log_print(ANDROID_LOG_ERROR, "flutter3", "%s", strerror(errno));

            fprintf(stderr, "Error create PEM RSA Public Key File.\n");
            //exit(2);
            return -1;
        }
        FILE *rsa_prikey_file;
        rsa_prikey_file = fopen(appendToString(keyPath, (char *)"/pr.txt"), "wb");
        if (!rsa_prikey_file)
        {
            __android_log_print(ANDROID_LOG_ERROR, "flutter4", "%s", strerror(errno));

            fprintf(stderr, "Error create PEM RSA Private Key File.\n");
            //exit(2);
            return -1;
        }

        __android_log_print(ANDROID_LOG_ERROR, "flutter5", "%s", strerror(errno));

        // Write the RSA keys to stdout
        env.writeKeyToFile(rsa_prikey_file, KEY_SERVER_PRI);
        env.writeKeyToFile(rsa_pkey_file, KEY_SERVER_PUB);

        fprintf(stderr, "Successfully generate RSA key pair.\n");

        fclose(rsa_pkey_file);
        fclose(rsa_prikey_file);
        __android_log_print(ANDROID_LOG_ERROR, "flutter6", "%s", strerror(errno));
        return 0;
    }

    int encrypt(const char *puKeypath, const char *encryptfile)
    {
        envelope env;

        FILE *rsa_pkey_file;
        rsa_pkey_file = fopen(puKeypath, "rb");
        if (!rsa_pkey_file)
        {
            __android_log_print(ANDROID_LOG_ERROR, "encrypt1", "%s", strerror(errno));

            fprintf(stderr, "Error loading PEM RSA Public Key File.\n");
            return -1;
        }

        FILE *encrypt_file = fopen(encryptfile, "rb");
        if (!encrypt_file)
        {
            __android_log_print(ANDROID_LOG_ERROR, "encrypt2", "%s", strerror(errno));
            fprintf(stderr, "Error to open encrypt_file.\n");
            return -1;
        }

        FILE *decrypt_file = fopen(appendToString(encryptfile, (char *)".enc"), "wb");
        if (!decrypt_file)
        {
            __android_log_print(ANDROID_LOG_ERROR, "encrypt3", "%s", strerror(errno));
            fprintf(stderr, "Error to write encrypt_file.\n");
            return -1;
        }

        fprintf(stderr, "do_evp_seal");

        int rv = env.do_evp_seal(rsa_pkey_file, encrypt_file, decrypt_file);

        fclose(rsa_pkey_file);
        fclose(encrypt_file);
        fclose(decrypt_file);

        return rv;
    }

    int decrypt(const char *prKeypath, const char *decryptfile)
    {
        envelope env;
        FILE *rsa_pkey_file;
        rsa_pkey_file = fopen(prKeypath, "rb");
        if (!rsa_pkey_file)
        {
            fprintf(stderr, "Error loading PEM RSA private Key File.\n");
        }

        FILE *encrypt_file = fopen(decryptfile, "rb");
        if (!encrypt_file)
        {
            fprintf(stderr, "Error to open encrypt_file.\n");
        }

        FILE *decrypt_file = fopen(appendToString(decryptfile, (char *)".dec"), "wb");
        if (!decrypt_file)
        {
            fprintf(stderr, "Error to write decrypt_file.\n");
        }

        fprintf(stderr, "do_evp_unseal");

        int rv = env.do_evp_unseal(rsa_pkey_file, encrypt_file, decrypt_file);

        fclose(rsa_pkey_file);
        fclose(encrypt_file);
        fclose(decrypt_file);
        return rv;
    }
}
