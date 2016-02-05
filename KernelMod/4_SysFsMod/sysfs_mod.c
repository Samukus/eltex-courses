#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>

#define SYSFS_NAME "sysfs_mod"
MODULE_LICENSE("GPL");

/*------Реализация Рандома------*/
static int constant = 65539;
static int m = 2147483648;
static unsigned long seed = 1; //  Начальное значение

static unsigned long rand(void) {
	seed = ( constant * seed) % m;
	return seed;
}
/*-------------------------------*/

struct my_attr {
    struct attribute attr;
    int value;
};


static struct my_attr random = {
    .attr.name="GetRandom",
    .attr.mode = 0444,
    .value = 0,
};

static struct my_attr rand_const = {
    .attr.name="RandomConst",
    .attr.mode = 0666,
    .value = 0,
};

static struct attribute * myattr[] = {
    &random.attr,
	&rand_const.attr,
    NULL
};

static ssize_t default_show(struct kobject *kobj, struct attribute *attr, char *buf)
{		
	struct my_attr *a = container_of(attr, struct my_attr, attr);
	if ( strcmp(a->attr.name,"GetRandom") == 0 )
		return scnprintf(buf, PAGE_SIZE, "%lu\n", rand() );
    return scnprintf(buf, PAGE_SIZE, "%d\n", a->value);
}

static ssize_t default_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t len)
{
    struct my_attr *a = container_of(attr, struct my_attr, attr);	
	sscanf(buf, "%d", &a->value);
	
	if ( strcmp(a->attr.name,"RandomConst") == 0 )  {
			seed = a->value;
	}
	
    return strlen(buf);
}

static struct sysfs_ops myops = {
    .show = default_show,
    .store = default_store,
};

static struct kobj_type mytype = {
    .sysfs_ops = &myops,
    .default_attrs = myattr,
};

struct kobject *mykobj;

int init_module(void)
{
    if( !( mykobj = kzalloc(sizeof(*mykobj), GFP_KERNEL) ) ) {
		printk("Memory allocation error\n");	
		return -1;
	}


	kobject_init(mykobj, &mytype);

	if ( kobject_add(mykobj, NULL, "%s", SYSFS_NAME) ) {
		printk("Sysfs creation failed\n");
		kobject_put(mykobj);
		mykobj = NULL;
		return -1;
    }
	printk("Sysfs dir: \"/sys/%s\" created\n",SYSFS_NAME);
	rand_const.value = seed;

	printk("Module sysfs_random successfuly loaded\n");
    return 0;
}

void cleanup_module(void)
{
    if (mykobj) {
        kobject_put(mykobj);
        kfree(mykobj);
		printk("Module sysfs_random successfuly removed\n");
    }
}


