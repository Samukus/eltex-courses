#include "ioctl.h"
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Eltex Courses Developmen" );

#define MAJOR_ID 200
#define MODNAME "ioctl_random"
#define BUF_LEN 32

static int Device_Open = 0;
static char msg[BUF_LEN];	/* Текст сообщения */
static char *msg_Ptr; 		/* Указатель на это сообщение */



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

static int device_read( struct file * file, char * buffer, size_t length, loff_t *ppos ) {
   	int bytes_read = 0;		/* Количество записанных в буффер байт */
					/* Перемещение данных в буффер */
  	while (length && *msg_Ptr) {
    		put_user(*(msg_Ptr++), buffer++);	/* Так как буффер находится в пространстве пользователя */
    		length--;
    		bytes_read++;
	}
	return bytes_read;		/* Функция чтения возвращает количество байт записанных в буффер */
}


// Работа с символьным устройством в старом стиле...
static int device_open( struct inode *n, struct file *f ) {
   	if (Device_Open) return -1;	/* Если устройство уже открыто */
	Device_Open++;			/* Устройство открыто */
	sprintf(msg, "%d\n", rand() );
	msg_Ptr = msg;
	return 0;
}

static int device_release( struct inode *n, struct file *f ) {
   	Device_Open--;                  /* Устройство закрыто */
	return 0;
}

static long dev_ioctl( struct file *f, unsigned int cmd, unsigned long arg ) {
   if( ( _IOC_TYPE( cmd ) != IOC_MAGIC ) ) return -ENOTTY;
   switch( cmd ) {
      case IOCTL_GET_RANDOM:
         return rand();
         break;
      default: 
         return -ENOTTY;
   }
   return 0;
}

static struct file_operations random_fops = {
   .owner = THIS_MODULE,
   .open = device_open,
   .release = device_release,
   .read  = device_read,
   .unlocked_ioctl = dev_ioctl
};

int init_module( void ) {
   int ret = register_chrdev( MAJOR_ID, MODNAME, &random_fops );
   if( ret < 0 ) {
      printk(KERN_ALERT "Error registration device %s in module \"Random IOCtl\"\n", MODNAME);
      return ret;
   }
   printk(KERN_ALERT "Kernel module \"Random IOCtl\" successfully loaded\n");
   return ret;
}

void cleanup_module( void ) {
  unregister_chrdev( MAJOR_ID, MODNAME );
  printk(KERN_ALERT "Kernel module \"Random IOCtl\" successfully removed \n");
}


