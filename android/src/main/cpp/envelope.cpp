
#include "envelope.h"
using namespace std;

EVP_PKEY *envelope::localKeypair;

envelope::envelope()
{
    localKeypair = NULL;
    //init();
}

envelope::~envelope()
{
    localKeypair = NULL;
}

// int envelope::init() {
// }

int envelope::generateRsaKeypair()
{
    EVP_PKEY_CTX *context = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    if (EVP_PKEY_keygen_init(context) <= 0)
    {
        return FAILURE;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(context, RSA_KEYLEN) <= 0)
    {
        return FAILURE;
    }

    if (EVP_PKEY_keygen(context, &localKeypair) <= 0)
    {
        return FAILURE;
    }

    EVP_PKEY_CTX_free(context);

    return SUCCESS;
}

int envelope::writeKeyToFile(FILE *file, int key)
{
    switch (key)
    {
    case KEY_SERVER_PRI:
        if (!PEM_write_PrivateKey(file, localKeypair, NULL, NULL, 0, 0, NULL))
        {
            return FAILURE;
        }
        break;

    case KEY_SERVER_PUB:
        if (!PEM_write_PUBKEY(file, localKeypair))
        {
            return FAILURE;
        }
        break;

    default:
        return FAILURE;
    }

    return SUCCESS;
}

int envelope::do_evp_seal(FILE *rsa_pkey_file, FILE *in_file, FILE *out_file)
{
    int retval = 0;
    RSA *rsa_pkey = NULL;
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    unsigned char buffer[4096];
    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH];
    size_t len = 0;
    size_t len_out = 0;
    unsigned char *ek;
    size_t eklen = 0;
    //uint32_t eklen_n=0;
    unsigned char iv[EVP_MAX_IV_LENGTH];

    if (!PEM_read_RSA_PUBKEY(rsa_pkey_file, &rsa_pkey, NULL, NULL))
    {
        fprintf(stderr, "Error loading RSA Public Key File.\n");
        ERR_print_errors_fp(stderr);
        retval = 2;
        goto out;
    }

    printf("-1");

    if (!EVP_PKEY_assign_RSA(pkey, rsa_pkey))
    {
        fprintf(stderr, "EVP_PKEY_assign_RSA: failed.\n");
        retval = 3;
        goto out;
    }

    printf("-2");

    EVP_CIPHER_CTX_init(ctx);
    ek = (unsigned char *)malloc(EVP_PKEY_size(pkey));
    if (!EVP_SealInit(ctx, EVP_aes_128_cbc(), &ek, (int *)&eklen, iv, &pkey, 1))
    {
        fprintf(stderr, "EVP_SealInit: failed.\n");
        retval = 3;
        goto out_free;
    }

    printf("0");

    /* First we write out the encrypted key length, then the encrypted key,
     * then the iv (the IV length is fixed by the cipher we have chosen).
     */

    //eklen_n = htonl(eklen);
    if (fwrite(&eklen, sizeof eklen, 1, out_file) != 1)
    {
        printf("1");
        perror("output file");
        retval = 5;
        goto out_free;
    }
    if (fwrite(ek, eklen, 1, out_file) != 1)
    {
        printf("2");
        perror("output file");
        retval = 5;
        goto out_free;
    }
    if (fwrite(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, out_file) != 1)
    {
        printf("3");
        perror("output file");
        retval = 5;
        goto out_free;
    }

    /* Now we process the input file and write the encrypted data to the
     * output file. */
    fprintf(stderr, "Start encrypt\n");

    while ((len = fread(buffer, 1, sizeof buffer, in_file)) > 0)
    {
        if (!EVP_SealUpdate(ctx, buffer_out, (int *)&len_out, buffer, len))
        {
            fprintf(stderr, "EVP_SealUpdate: failed.\n");
            retval = 3;
            goto out_free;
        }

        if (fwrite(buffer_out, len_out, 1, out_file) != 1)
        {
            perror("output file");
            retval = 5;
            goto out_free;
        }
    }

    if (ferror(in_file))
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }

    if (!EVP_SealFinal(ctx, buffer_out, (int *)&len_out))
    {
        fprintf(stderr, "EVP_SealFinal: failed.\n");
        retval = 3;
        goto out_free;
    }

    if (fwrite(buffer_out, len_out, 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }

    fprintf(stderr, "Encrypted successfully\n");

out_free:
    EVP_PKEY_free(pkey);
    free(ek);

out:
    return retval;
}

int envelope ::do_evp_unseal(FILE *rsa_pkey_file, FILE *in_file, FILE *out_file)
{
    int retval = 0;
    RSA *rsa_pkey = NULL;
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    unsigned char buffer[4096];
    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH];
    size_t len = 0;
    size_t len_out = 0;
    unsigned char *ek;
    size_t eklen = 0;
    //int eklen_n=0;
    unsigned char iv[EVP_MAX_IV_LENGTH];

    if (!PEM_read_RSAPrivateKey(rsa_pkey_file, &rsa_pkey, NULL, NULL))
    {
        fprintf(stderr, "Error loading RSA Private Key File.\n");
        ERR_print_errors_fp(stderr);
        retval = 2;
        goto out;
    }

    if (!EVP_PKEY_assign_RSA(pkey, rsa_pkey))
    {
        fprintf(stderr, "EVP_PKEY_assign_RSA: failed.\n");
        retval = 3;
        goto out;
    }

    EVP_CIPHER_CTX_init(ctx);
    ek = (unsigned char *)malloc(EVP_PKEY_size(pkey));

    /* First need to fetch the encrypted key length, encrypted key and IV */

    if (fread(&eklen, sizeof eklen, 1, in_file) != 1)
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }
    //eklen = ntohl(eklen_n);
    if (eklen > EVP_PKEY_size(pkey))
    {
        fprintf(stderr, "Bad encrypted key length (%u > %d)\n", (unsigned int)eklen,
                EVP_PKEY_size(pkey));
        retval = 4;
        goto out_free;
    }
    if (fread(ek, eklen, 1, in_file) != 1)
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }
    if (fread(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, in_file) != 1)
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }
    if (!EVP_OpenInit(ctx, EVP_aes_128_cbc(), ek, eklen, iv, pkey))
    {
        fprintf(stderr, "EVP_OpenInit: failed.\n");
        retval = 3;
        goto out_free;
    }

    fprintf(stderr, "Start decrypt\n");

    while ((len = fread(buffer, 1, sizeof buffer, in_file)) > 0)
    {
        if (!EVP_OpenUpdate(ctx, buffer_out, (int *)&len_out, buffer, len))
        {
            fprintf(stderr, "EVP_OpenUpdate: failed.\n");
            retval = 3;
            goto out_free;
        }

        if (fwrite(buffer_out, len_out, 1, out_file) != 1)
        {
            perror("output file");
            retval = 5;
            goto out_free;
        }
    }

    if (ferror(in_file))
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }

    if (!EVP_OpenFinal(ctx, buffer_out, (int *)&len_out))
    {
        fprintf(stderr, "EVP_OpenFinal: failed.\n");
        retval = 3;
        goto out_free;
    }

    if (fwrite(buffer_out, len_out, 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }

    fprintf(stderr, "Decrypted successfully\n");

out_free:
    EVP_PKEY_free(pkey);
    free(ek);

out:
    return retval;
}
