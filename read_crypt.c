#include<linux/sched.h>
#include<linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <crypto/md5.h>
#include <crypto/internal/skcipher.h>
#include <crypto/rng.h>
#include <linux/kernel.h>
#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>            
#include <linux/uaccess.h>          	 
#include<linux/moduleparam.h>
#include<linux/mm.h>
#include <linux/scatterlist.h>  
#include <linux/kmod.h>
#include <linux/string.h>
#include <linux/completion.h>
#include <crypto/skcipher.h>
#include <linux/crypto.h>
#include <crypto/hash.h>
#include <linux/stat.h> 

#define DATA_SIZE       16

void hexdumpRead(unsigned char *buf, unsigned int len)
{
	while (len--){
		printk("%2d %02x", len, *buf++);	
		//printk("%2d %c", len, *buf++);
	}
	printk("\n");
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


void decrypt_SOB(unsigned char message[], int tamanho){

	
	
	int   	mode = 0;
	int   	mask = 0; 
	int    	ret;

	int  	i,j, k, zerar = 0;	
	struct 	crypto_skcipher *tfm; 
	struct 	scatterlist sg[2]; 
	struct 	skcipher_request *req = NULL; 
	
		 char 	*algo = "ecb(aes)";
	unsigned char 	*input, *output, *saida = NULL;
		 char 	keyInterno[16] = "0123456789ABCDEF";
	unsigned char  	messageSaida[513] = "\0";	


	tfm = crypto_alloc_skcipher(algo, mode, mask);

	if (IS_ERR(tfm)) {
			printk(" Erro ao alocar tfm");
			return ;
	}
	
	ret = crypto_skcipher_setkey(tfm, keyInterno, 16);

	if (ret) {
			printk("erro no ret \n"); 
			goto out;
	}

	req = skcipher_request_alloc(tfm, GFP_KERNEL); 
		if (!req) {
		printk("could not allocate tfm request\n");
		goto out;
		}

	input = kmalloc(DATA_SIZE, GFP_KERNEL); 
	if (!input) {
			printk("kmalloc(input) failed\n");
			goto out;
	}

	output = kmalloc(DATA_SIZE, GFP_KERNEL); 
	if (!output) {
			printk("kmalloc(output) failed\n");
			kfree(input);
			goto out;
	}

	j = 0;
	k = 0;
	
	
	sg_init_one(&sg[0], input, DATA_SIZE);          
	sg_init_one(&sg[1], output, DATA_SIZE);      	

	skcipher_request_set_crypt(req, &sg[0], &sg[1], DATA_SIZE, NULL); // ordem ods parametros: requisição, origem, destino, tamanho, iv;
	
	while( j < tamanho){

		strcpy(input, "\0");

		for(i=0; i < 16; i++){
			
			if(zerar == 1)
			{
				input[i] = 0;	
			}

			else if(message[j] == '\0')
			{
				input[i] = 0;
				zerar = 1;
			}
			else
			{
				input[i] = message[j];
			}

			j++;

		}
		zerar = 0;
		
		crypto_skcipher_decrypt(req);

		saida = sg_virt(&sg[1]);	
		
		printk("Entrada: "); hexdumpRead(input, 16);
		printk("Saida: "); hexdumpRead(output, 16);

		for(i = 0; i < 16; i++){
			messageSaida[k] = saida[i];
			k++;
		}
		messageSaida[k] = '\0';									
		
	}

	strcpy(message, messageSaida);
	message[j] = '\0';

	printk("Message: "); hexdumpRead(message, tamanho);

	kfree(output);
	kfree(input);

	out:
	crypto_free_skcipher(tfm);
	skcipher_request_free(req);

}




asmlinkage ssize_t read_crypt(int fd, const void *buf, size_t nbytes){

	int i, hexa1, hexa2, len;
	int tamanho = nbytes;
	mm_segment_t old_fs;

	unsigned char message[512];
	
	printk("entrei no read------------------------\n");

	if(fd < 0){
		return 1;
	}
	
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	sys_read(fd, buf, nbytes);
	printk("leitura: %s", ((uint8_t*)buf));
	printk("\n");

	

	hexa1 = converteHexa(((uint8_t*)buf)[0]);
	hexa2 = converteHexa(((uint8_t*)buf)[1]);

	hexa1 = hexa1 * 16;
	hexa1 += hexa2;		
	
	for(i = 0; i < tamanho / 2; i++){

		message[i] = hexa1;

		hexa1 = converteHexa( ((uint8_t*)buf)[(i+1)*2] );
		hexa2 = converteHexa( ((uint8_t*)buf)[((i+1) * 2) + 1] );

		hexa1 = hexa1 * 16;
		hexa1 += hexa2;

	}
	message[i] = '\0';
	tamanho = strlen(message);

	printk("valor message antes = "); hexdumpRead(message, tamanho);

	decrypt_SOB(message, tamanho);

	printk("valor message depois = "); hexdumpRead(message, tamanho);

	for(i = 0; i < tamanho; i++){

		((uint8_t*)buf)[i] = message[i];
	
	}
	
	printk("valor de retorno = %s\n", message);

	set_fs(old_fs);
	
	
	return 0;
}
