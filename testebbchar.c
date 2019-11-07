#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               
	static char receive[BUFFER_LENGTH];     

	void hexdump(unsigned char *buf, unsigned int len);	

	int converteHexa(char valor);

	int main(){

		int ret, fd;
		char string[BUFFER_LENGTH+1], stringFinal[(BUFFER_LENGTH*2)+1];//257:256+1, 513:256*2+1
		int i, len, tamanhoMessage;
		char stringEnvio[(BUFFER_LENGTH*2)+1];
		int op;
		char hexa[2];
		int hexa1, hexa2;

		printf("Starting testeebbchar...\n");
		fd = open("/dev/cryptoDevice", O_RDWR);             // Open the device with read/write access

		if (fd < 0){
			perror("Failed to open the device...");
			return errno;
		}

		printf("Informe o formato da sua entrada\n1- Ascii\n2- Hexadecimal\nOpcao: ");
		scanf("%d", &op);

		printf("Digite sua string: ");
		__fpurge(stdin);
		scanf("%[^\n]%*c", string);
		
		

		if (op == 2){
			stringEnvio[0] = string[0];
			stringEnvio[1] = ' ';
			stringEnvio[2] = '\0';

	
			hexa1 = converteHexa(string[2]);
			hexa2 = converteHexa(string[3]);

			hexa1 = hexa1 * 16;
			hexa1 += hexa2;		
			len = strlen(string);
		
			for(i = 2; i<len-2; i++){

				stringFinal[i-2] = hexa1;

				hexa1 = converteHexa(string[i*2]);
				hexa2 = converteHexa(string[(i*2) + 1]);
	
				hexa1 = hexa1 * 16;
				hexa1 += hexa2;

		}

		strcat(stringEnvio, stringFinal);
		strcat(stringEnvio, "\0");
		tamanhoMessage = strlen(string) / 2;

		}
		else {
			strcpy(stringEnvio, string);
			tamanhoMessage = strlen(string);
		}



		if((tamanhoMessage-2) % 16 == 0) // -2 dvido a letra da opção e
			tamanhoMessage = (tamanhoMessage-2) / 16;
		else
			tamanhoMessage = 1 + ( (tamanhoMessage-2) / 16);


	   	ret = write(fd, stringEnvio, strlen(stringEnvio));
	   
		if (ret < 0){
			perror("Failed to write the message to the device.");
			return errno;
		}

		printf("Writing message to the device [%s].\n", string);

		printf("Press ENTER to read back from the device...\n");
		getchar();

		printf("Reading from the device...\n");


		ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM

		if (ret < 0){
			perror("Failed to read the message from the device.");
			return errno;
		}
		printf("The received message is: ");//%s\n", receive);

		
		hexdump(receive, tamanhoMessage * 16);
	
		printf("End of the program\n");
		return 0;
	}

	void hexdump(unsigned char *buf, unsigned int len)
	{
		while (len--){
			printf("%02x", *buf++);

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



