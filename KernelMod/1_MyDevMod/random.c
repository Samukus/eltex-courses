#include <linux/module.h> 	/* Необходимо для написания любого модуля ядра */ 
#include <linux/kernel.h> 	/* Здесь определение KERN_ALEPT (записи см. в /var/log/kern.log) */
#include <linux/fs.h>		/* Определение структур file и file_operations */
#include <asm/uaccess.h>	/* определение функции put_user */
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex&Gerald Development (inc)");
MODULE_DESCRIPTION("Module \"Random\".");

#define DEVICE_NAME "mydev"	/* Имя устройства, будет отображаться в /dev   */
#define BUF_LEN 80		/* Максимальная длина сообщения */
#define MAJOR 198		/* Старший номер устройства нашего драйвера */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
/* size_t == unsigned int, ssize_t == int */


static int Device_Open = 0;	/* 0 - устройство закрыто
				 * 1 - устройство открыто */
static char msg[BUF_LEN];	/* Текст сообщения */
static char *msg_Ptr; 		/* Указатель на это сообщение */

/* Данная структура, описывает что должно происходить при работе с устройством */
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};


/*------- Реализация Рандома ------*/
static int a = 45;
static int c = 21;
static int m = 100;
static int seed = 2;

static int rand(void) {
	seed = (a * seed + c) % m;
	return seed;
}
/*--------------------------------*/

int init_module(void)
{
	int ret = register_chrdev(MAJOR, DEVICE_NAME, &fops);	/* Получение старшего номера устройства */

	if (ret < 0) {    
		printk(KERN_ALERT "Error registration device %s in module \"Random\"\n", DEVICE_NAME);
		return ret;	/* Возврат не 0 считается ошибкой */
	}

	printk(KERN_ALERT "Kernel module \"Random\" successfully loaded\n");
	printk(KERN_ALERT "Major ID: %d\n", MAJOR);
	/* Теперь нужно подключить это устройство командой sudo mknod /dev/chardev c MAJOR 0 */
	/* Затем можно будет что-нибудь прочитать из этого устройства командой cat /dev/mydev */

	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(MAJOR, DEVICE_NAME);
	printk(KERN_ALERT "Kernel module \"Random\" successfully removed \n");
}

/* Данная функция запускается при открытии файла устройства */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open) return -1;	/* Если устройство уже открыто */
	Device_Open++;			/* Устройство открыто */
	sprintf(msg, "%d\n", rand() );
	msg_Ptr = msg;
	return 0;
}

/* Данная функция запускается при закрытии файла устройства */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;                  /* Устройство закрыто */
	return 0;
}

/* Данная функция запускается при попытке прочтения уже открытого файла устройства */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{
	int bytes_read = 0;		/* Количество записанных в буффер байт */
	/* Перемещение данных в буффер */
  	while (length && *msg_Ptr) {
    		put_user(*(msg_Ptr++), buffer++);	/* Так как буффер находится в пространстве пользователя */
    		length--;
    		bytes_read++;
	}
	return bytes_read;		/* Функция чтения возвращает количество байт записанных в буффер */
}

/* Данная функция запускается при попытке записи в  файл устройства */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  printk(KERN_ALERT "This device is not for writing, asshole $)\n");
  return len;
}
