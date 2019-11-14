#include <linux/kernel.h>

#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>            
#include <linux/uaccess.h>          
#include <linux/mutex.h>	 
#include<linux/moduleparam.h>
#include<linux/mm.h>
#include <linux/scatterlist.h>  
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/kmod.h>
#include <linux/string.h>
#include <linux/completion.h>
#include <crypto/skcipher.h>
#include <linux/crypto.h>
#include <crypto/hash.h>
#include <linux/stat.h> 

/* void hexdump(unsigned char *buf, unsigned int len);
 void crypt_SOB(char escolha);*/

asmlinkage ssize_t write_crypt(int fd, const void *buf, size_t nbytes);
{
	printk("valores fd = %d\n buf = %s\n nbytes = %d\n", fd, *buf, nbytes);
	return 0;
}
/*
 void hexdump(unsigned char *buf, unsigned int len)
{
	while (len--){
		printk("%2d %02x", len, *buf++);	
		//printk("%2d %c", len, *buf++);
	}
	printk("\n");
}

 void crypt_SOB(char message*, int len){

	
	char *algo = "ecb(aes)"; // modificado: anterior "aes"
	int   mode = CRYPTO_skcipher_MODE_CBC;
	int   mask = CRYPTO_skcipher_MODE_MASK; // adicionado
	int  i,j, k, zerar = 0;	
	char  messageSaida[513] = "\0";
	struct crypto_skcipher *tfm; // utilzamos skcipher ao inves de tfm
	struct scatterlist sg[2]; // entender o q eh scatterlist
	struct skcipher_request *req = NULL; // necessario apra criptografar
	int    ret;
	char   *input, *output;
	char keyInterno[16] = "0123456789ABCDEF";
		


	tfm = crypto_alloc_skcipher(algo, mode, mask);

	if (IS_ERR(tfm)) {
			printk(" Erro ao alocar tfm");
			return ;
	}
	
	ret = crypto_skcipher_setkey(tfm, keyInterno, 16); // estava errado, o correto eh strlen, n sizeof

	if (ret) {
			printk(KERN_ERR PFX "erro no ret \n"); // FALTA FALAR SOBRE ESSE ERRO DE FLAG
			goto out;
	}

	req = skcipher_request_alloc(tfm, GFP_KERNEL); // o que é GFP_KERNEL????
		if (!req) {
		printk("could not allocate tfm request\n");
		goto out;
		}

	input = kmalloc(DATA_SIZE, GFP_KERNEL); // Parametros estavam invertidos
	if (!input) {
			printk(KERN_ERR PFX "kmalloc(input) failed\n");
			goto out;
	}

	output = kmalloc(DATA_SIZE, GFP_KERNEL); // Parametros estavam invertidos
	if (!output) {
			printk(KERN_ERR PFX "kmalloc(output) failed\n");
			kfree(input);
			goto out;
	}

	j = 0;
	k = 0;
	
	
	sg_init_one(&sg[0], input, DATA_SIZE);          //
	sg_init_one(&sg[1], output, DATA_SIZE);      	//      entrou no lugar do FILL_SG();

	skcipher_request_set_crypt(req, &sg[0], &sg[1], DATA_SIZE, NULL); // ordem ods parametros: requisição, origem, destino, tamanho, iv;
	
	while( j < len)){

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

	printk("Message: "); hexdump(message, 32);

	kfree(output);
	kfree(input);

	out:
	crypto_free_skcipher(tfm);
	skcipher_request_free(req);
	
	return message;

}*/