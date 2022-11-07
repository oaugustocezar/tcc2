#include <relic_bc.h>
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
#include <openssl/bio.h>
#include <locale.h>

#define MAX_MSG 1024
#define KEY_LEN 32

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
    setlocale(LC_ALL,"Portuguese"); 
    int conexao;

    criarSocket(&conexao);

    uint8_t mensagem[MAX_MSG];
    uint8_t crypto[MAX_MSG];
    uint8_t plain[MAX_MSG];
     uint8_t cipherText[MAX_MSG];
    int in_len, out_len;
    uint8_t *key = "01234567890123450123456789012345";
    uint8_t *iv = "0123456789012345";
    FILE *fp, *fp3;
    /*
    fp = fopen("/home/pi/Documents/TCC/foto2.jpg", "rb");
    in_len = fread(&mensagem, sizeof(uint8_t), MAX_MSG, fp);
    fclose(fp);*/

    double t2, t3, t4, t5;
    int errnum, ret_Val;
    struct timeval utime;
    in_len = 0;

     if ((in_len = read(conexao, crypto, MAX_MSG)) < 0)
    {
        perror("Erro ao receber dados do cliente: ");

        return -1;
    }

    // BIO_dump_fp(stdout, crypto, in_len);   

    
    out_len = MAX_MSG;  
            if (ret_Val = gettimeofday(&utime, NULL) != 0){

                errnum = errno;
                printf("Value of errno: %d\n", errno);
                perror("Error printed by perror");
                printf("Error opening file: %s\n", strerror( errnum ));
            }else{
                t2 = utime.tv_sec + ( utime.tv_usec / 1000000.0 );

            }   
    
    if (bc_aes_cbc_dec(plain, &out_len, crypto, in_len, key, KEY_LEN, iv))
    {
        printf("Erro dec\n");
    }else{
        printf("Sucesso Dec\n");
        plain[out_len] = '\0';
    }
    
    // printf("%s",plain);

    in_len = out_len;
    out_len = MAX_MSG;

            if (ret_Val = gettimeofday(&utime, NULL) != 0){

                errnum = errno;
                printf("Value of errno: %d\n", errno);
                perror("Error printed by perror");
                printf("Error opening file: %s\n", strerror( errnum ));
            }else{
                t3 = utime.tv_sec + ( utime.tv_usec / 1000000.0 );

            }   

                    if (ret_Val = gettimeofday(&utime, NULL) != 0){

                errnum = errno;
                printf("Value of errno: %d\n", errno);
                perror("Error printed by perror");
                printf("Error opening file: %s\n", strerror( errnum ));
            }else{
                t4 = utime.tv_sec + ( utime.tv_usec / 1000000.0 );

            }   

    if(bc_aes_cbc_enc(cipherText, &out_len, plain, in_len, key, KEY_LEN, iv))
       {
        printf("Erro enc\n");
    }else{
        printf("Sucesso enc\n");
        cipherText[out_len] = '\0';
    }

            if (ret_Val = gettimeofday(&utime, NULL) != 0){

                errnum = errno;
                printf("Value of errno: %d\n", errno);
                perror("Error printed by perror");
                printf("Error opening file: %s\n", strerror( errnum ));
            }else{
                t4 = utime.tv_sec + ( utime.tv_usec / 1000000.0 );

            }   

    if(write(conexao, cipherText, out_len) < 0){
        printf("Erro Ao Enviar");
    }else{
        printf("Enviado");
    }
   
    
}