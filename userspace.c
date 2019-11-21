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

int main()
{
	char entrada[512], valorCrypt[1024], valorDecrypt[1024];
	void *buf;
	char caminho[60] = "/home/puc/Documentos/valorCripy\0";
	int tamanho, fd, i;
	long int ret_syscall;
	FILE *fp;

	printf("Digite o valor a ser criptografado: ");
	__fpurge(stdin);
	scanf("%[^\n]%*c", entrada);
	
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
	printf("System call sys_hello returned %ld\n", ret_syscall);

	close(fd);
	
	fp = fopen(caminho, "rt");
	if(fp == NULL) {
		printf("Problema leitura");
		return 1;
	}

	fseek(fp, 0, SEEK_SET);
	fscanf(fp, "%s", valorCrypt);
	printf("\nValor criptografada: %s\n", valorCrypt);
	fclose(fp);

	// usando syscall read

	fd = open(local, O_RDONLY);
        buf = valorCrypt;
	tamanho = strlen(valorCrypt);
	ret_status = syscall(549, fd, buf,tamanho);
	close(fd);

	//--------------------
	
	for(i = 0; i < tamanho; i++) {
		sprintf(&valorDecrypt[i], "%c",((char *)buf)[i]);
	}

	printf("\nValor descriptografado: %s\n", valorDecrypt);



        return 0;
}
