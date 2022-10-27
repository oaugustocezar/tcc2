#include <wolfssl/wolfcrypt/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#define MAX_MSG 1024
#include <wolfssl/options.h>
#define DEBUG 0
#define KEY_LEN 32

void defineKey(int key_len, byte *key, byte *iv)
{
    byte iv128[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
    iv = iv128;
    if (key_len == 16)
    {
        byte key128[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
        key = key128;
    }
    else if (key_len == 24)
    {
        byte key192[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4};
        key = key192;
    }
    else if (key_len == 32)
    {
        byte key256[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6};
        key = key256;
    }
}

int criarSocket(int *conexao)
{
    int socket_desc, c;
    struct sockaddr_in servidor, cliente;
    // para pegar o IP e porta do cliente
    char *cliente_ip;
    int cliente_port;

    /*********************************************************/
    //Criando um socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Nao foi possivel criar o socket\n");
        return -1;
    }

    int reuso = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuso, sizeof(reuso)) < 0)
    {
        perror("NÃ£o foi possÃ­vel reusar endereÃ§o");
        return -1;
    }
#ifdef SO_REUSEPORT
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, (const char *)&reuso, sizeof(reuso)) < 0)
    {
        perror("NÃ£o foi possÃ­vel reusar porta");
        return -1;
    }
#endif

    //Preparando a struct do socket
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY; // Obtem IP do S.O.
    servidor.sin_port = htons(1234);

    //Associando o socket a porta e endereco
    if (bind(socket_desc, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        perror("Erro ao fazer bind\n");
        return -1;
    }
    puts("Bind efetuado com sucesso\n");

    // Ouvindo por conexoes
    listen(socket_desc, 3);
    /*********************************************************/

    //Aceitando e tratando conexoes
    puts("Aguardando por conexoes...");
    c = sizeof(struct sockaddr_in);
    *conexao = accept(socket_desc, (struct sockaddr *)&cliente, (socklen_t *)&c);
    if (*conexao < 0)
    {
        perror("Erro ao receber conexao\n");
        return -1;
    }
    /*********************************************************/

    /*********comunicao entre cliente/servidor****************/

    // pegando IP e porta do cliente
    cliente_ip = inet_ntoa(cliente.sin_addr);
    cliente_port = ntohs(cliente.sin_port);
    printf("cliente conectou\nIP:PORTA -> %s:%d\n", cliente_ip, cliente_port);
}

int main()
{
    int conexao;

    //criarSocket(&conexao);

    byte mensagem[AES_BLOCK_SIZE * 16];
    memcpy(mensagem, "latitude: 0.0, longitude: 0.0, velocidade: 0.0 e o resto é apenas enchecao de linguica para dar o numero de characteres latitude: 0.0, longitude: 0.0, velocidade: 0.0 e o resto é apenas enchecao de linguica para dar o numero de characteres testando", 208);

    double t2, t3, t4, t5;
    int errnum, ret_Val;
    struct timeval utime;

    byte cipher[AES_BLOCK_SIZE * 100]; // Some multiple of 16 bytes
    //byte *key;
    byte key128[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5};
    byte key192[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3};
    byte key256[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5};
    byte iv[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5};
    Aes enc, dec;
    byte *key;
    defineKey(KEY_LEN, key, iv);
    int ret = 0;
    int in_len = 0;

    /*if ((in_len = read(conexao, &cipher, MAX_MSG)) < 0)
    {
        perror("Erro ao receber dados do cliente: ");

        return -1;
    }*/
    if (KEY_LEN == 16)
    {
        wc_AesSetKey(&enc, key128, KEY_LEN, iv, AES_ENCRYPTION);
    }
    else if (KEY_LEN == 24)
    {
        wc_AesSetKey(&enc, key192, KEY_LEN, iv, AES_ENCRYPTION);
    }
    else if (KEY_LEN == 32)
    {
        wc_AesSetKey(&enc, key256, KEY_LEN, iv, AES_ENCRYPTION);
    }

    if (ret_Val = gettimeofday(&utime, NULL) != 0)
    {

        errnum = errno;
        printf("Value of errno: %d\n", errno);
        perror("Error printed by perror");
        printf("Error opening file: %s\n", strerror(errnum));
    }
    else
    {
        t4 = utime.tv_sec + (utime.tv_usec / 1000000.0);
    }

    if ((ret = wc_AesCbcEncrypt(&enc, cipher, mensagem, sizeof(mensagem))) != 0)
    {
        if (DEBUG)
            printf("erro");
    }
    else
    {
        if (DEBUG)
            printf("suceso");
    }

    if (ret_Val = gettimeofday(&utime, NULL) != 0)
    {

        errnum = errno;
        printf("Value of errno: %d\n", errno);
        perror("Error printed by perror");
        printf("Error opening file: %s\n", strerror(errnum));
    }
    else
    {
        t5 = utime.tv_sec + (utime.tv_usec / 1000000.0);
    }

    byte plain[AES_BLOCK_SIZE * 100];

    if (KEY_LEN == 16)
    {
        wc_AesSetKey(&dec, key128, KEY_LEN, iv, AES_DECRYPTION);
    }
    else if (KEY_LEN == 24)
    {
        wc_AesSetKey(&dec, key192, KEY_LEN, iv, AES_DECRYPTION);
    }
    else if (KEY_LEN == 32)
    {
        wc_AesSetKey(&dec, key256, KEY_LEN, iv, AES_DECRYPTION);
    }

    if (ret_Val = gettimeofday(&utime, NULL) != 0)
    {

        errnum = errno;
        printf("Value of errno: %d\n", errno);
        perror("Error printed by perror");
        printf("Error opening file: %s\n", strerror(errnum));
    }
    else
    {
        t2 = utime.tv_sec + (utime.tv_usec / 1000000.0);
    }

    if ((ret = wc_AesCbcDecrypt(&dec, plain, cipher, sizeof(cipher))) != 0)
    {
        if (DEBUG)
            printf("erro");
    }
    else
    {
        if (DEBUG)
            printf("suceso");
    }

    if (ret_Val = gettimeofday(&utime, NULL) != 0)
    {

        errnum = errno;
        printf("Value of errno: %d\n", errno);
        perror("Error printed by perror");
        printf("Error opening file: %s\n", strerror(errnum));
    }
    else
    {
        t3 = utime.tv_sec + (utime.tv_usec / 1000000.0);
    }

    plain[sizeof(plain)] = '\0';
    //printf("%s", plain);

    FILE *fp, *fp3;
    fp = fopen("/home/pi/Documents/TCC/confirmacao.txt", "wb+");

    fprintf(fp, "%s\n", plain);

    fp3 = fopen("tempos_exec_Client_mensagemRPIWolf.csv", "a");

    if (fprintf(fp3, "%d,%.10f,%.10f,%.10f,%.10f\n", KEY_LEN, t2, t3, t4, t5) < 0)
    {
        printf("Erro de gravação no arquivo\n");
    }

    //printf("%s",plain);
}