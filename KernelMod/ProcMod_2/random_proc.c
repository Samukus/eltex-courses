#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>		// for basic filesystem
#include <linux/proc_fs.h>	// for the proc filesystem
#include <linux/seq_file.h>	// for sequence files

MODULE_LICENSE("GPL"); 

/*------Реализация Рандома------*/
static int a = 45;
static int c = 21;
static int m = 100;
static int seed = 2;

static int rand(void) {
	seed = (a * seed + c) % m;
	return seed;
}
/*-------------------------------*/


static struct proc_dir_entry* random_proc;

static int proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", rand() );
	return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_show, NULL);
}

static const struct file_operations fops = {
	.owner	= THIS_MODULE,
	.open	= proc_open,
	.read	= seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

int init_module(void)
{
	random_proc = proc_create("random_proc", 0, NULL, &fops);
	if (!random_proc) return -1;
	printk(KERN_ALERT "Kernel module \"Random Proc\" successfully loaded\n");
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry("random_proc", NULL);
	printk(KERN_ALERT "Kernel module \"Random\" successfully removed \n");
}
