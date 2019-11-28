#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include <stdio_ext.h>
#include <string.h>

void hexdump(unsigned char *buf, unsigned int len);

int converteHexa(char valor);

int main()
{
	char entrada[512], valorCrypt[1024], valorDecrypt[1024];
	void *buf;
	char caminho[100]; // "/home/teste/Documentos/valorCrp\0";
	char nomeArquivo[20];
	int tamanho, fd, i, hexa1, hexa2, len;
	long int ret_syscall;
	FILE *fp;

	printf("Digite o valor a ser criptografado:\n");
	__fpurge(stdin);
	scanf("%[^\n]%*c", entrada);
	
	printf("Digite o local onde sera salvo arquivo (exemplo: /home/teste/Documentos/):\n");
	__fpurge(stdin);
	scanf("%[^\n]%*c", nomeArquivo);

	printf("Digite o nome do arquivo:\n");
	__fpurge(stdin);
	scanf("%[^\n]%*c", nomeArquivo);

	strcat(caminho,nomeArquivo);
	strcar(caminho,"\0");

	tamanho = strlen(entrada);
	buf = entrada;

	// usando syscall write

	fd = open(caminho, O_WRONLY|O_CREAT, 0644);
	
	if(fd < 0){
		printf("Caminho informado nao eh aceito!");
		return 1;
	}

	ret_syscall = syscall(548, fd, buf, tamanho);

	//----------------
	
	close(fd);
	
	fp = fopen(caminho, "rt");
	if(fp == NULL) {
		printf("Problema leitura\n");
		return 1;
	}

	fseek(fp, 0, SEEK_SET);
	fscanf(fp, "%s", valorCrypt);
	printf("\n\nValor real armazenado no arquivo: %s\n\n", valorCrypt);
	fclose(fp);
	strcpy(valorDecrypt, "\0");

	 // usando syscall read 

	fd = open(caminho, O_RDONLY);
        buf = valorDecrypt;
	tamanho = strlen(valorCrypt);
	ret_syscall = syscall(549, fd, buf,tamanho);
	close(fd);

	//--------------------
	printf("\n\nValor após tratamento da read_crypt: ");
	for(i = 0; i < tamanho/2; i++) {
		printf("%c", ((uint8_t*)buf)[i]);
	}
	
	
	
	printf("\n");



        return 0;
}

void hexdump(unsigned char *buf, unsigned int len)
{
	while (len--){
		printf("%2d %02x\n", len, *buf++);	
		//printk("%2d %c", len, *buf++);
	}
	printf("\n");
}

int converteHexa(char valor){


	switch(valor){

		case '0':
			return 00;
		break;

		case '1':
			return 01;
		break;

		case '2':
			return 02;
		break;

		case '3':
			return 03;
		break;

		case '4':
			return 04;
		break;

		case '5':
			return 05;
		break;

		case '6':
			return 06;
		break;

		case '7':
			return 07;
		break;

		case '8':
			return 8;
		break;

		case '9':
			return 9;
		break;

		case 'a':
		case 'A':
			return 10;
		break;

		case 'b':
		case 'B':
			return 11;
		break;

		case 'c':
		case 'C':
			return 12;
		break;

		case 'd':
		case 'D':
			return 13;
		break;

		case 'e':
		case 'E':
			return 14;
		break;

		case 'f':
		case 'F':
			return 15;
		break;
	}
}

