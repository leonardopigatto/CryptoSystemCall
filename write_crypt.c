#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <linux/string.h>
#include <crypto/md5.h>
#include <crypto/internal/skcipher.h>
#include <crypto/rng.h>

#define DATA_SIZE       16

void hexdump(unsigned char *buf, unsigned int len)
{
	while (len--){
		printk("%2d %02x", len, *buf++);	
		//printk("%2d %c", len, *buf++);
	}
	printk("\n");
}



int crypt_SOB(unsigned char message[], int tamanho){

	printk("entrei\n");
	
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

			else if(j > tamanho)
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
		
		crypto_skcipher_encrypt(req);	

		saida = sg_virt(&sg[1]);	
		
		printk("Entrada: "); hexdump(input, 16);
		printk("Saida: "); hexdump(output, 16);

		for(i = 0; i < 16; i++){
			messageSaida[k] = saida[i];
			k++;
		}
		messageSaida[k] = '\0';									
		
	}

	strcpy(message, messageSaida);
	message[j] = '\0';

	printk("Message: "); hexdump(message, j);

	kfree(output);
	kfree(input);

	out:
	crypto_free_skcipher(tfm);
	skcipher_request_free(req);
	
	return j;

}




asmlinkage ssize_t write_crypt(int fd, const void *buf, size_t nbytes){

	int i;
	int tamanho = nbytes;
	unsigned char message[512], arquivo[1024];

	for(i = 0; i < tamanho; i++) {
		sprintf(&message[i], "%c",((char *)buf)[i]);
	}
	printk("valor message = %s", message);
	tamanho = crypt_SOB(message, tamanho);

	for(i = 0; i < tamanho; i++) {
		sprintf(&arquivo[i*2], "%02x", message[i]);
	}

	printk("valor em arquivo = %s\n", arquivo);
	
	if (fd >= 0){
		mm_segment_t old_fs = get_fs();
		set_fs(KERNEL_DS);
		sys_write(fd, arquivo, tamanho*2);
		set_fs(old_fs);
		return 0;
  	}


	return 1;
}
