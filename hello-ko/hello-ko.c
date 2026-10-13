#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define DEVICE_NAME "noridev_hello"
#define CLASS_NAME "noridev_class"

#define BUFFER_SIZE ((size_t)1024)

static dev_t dev_num;
static struct cdev nori_cdev;
static struct class *nori_class;
static struct device *nori_device;

static char devbuf[BUFFER_SIZE];

static int nori_open(struct inode *inode, struct file *filp)
{
	pr_info("device opened.");
	return 0;
}

static ssize_t nori_read(struct file *filp, char __user *buf, size_t count,
			 loff_t *fpos)
{
	ssize_t ret;
	unsigned long copy_bytes;
	char *head;

	if (!count) {
		return 0;
	}

	head = devbuf + *fpos;
	if (*fpos + count > BUFFER_SIZE) {
		count = BUFFER_SIZE - *fpos;
		*fpos = BUFFER_SIZE;
	}
	copy_bytes = copy_to_user(buf, head, count);
	ret = count - copy_bytes;
	*fpos = *fpos == BUFFER_SIZE ? 0 : *fpos + ret;

	return ret;
}

static ssize_t nori_write(struct file *filp, const char __user *buf,
			  size_t count, loff_t *fpos)
{
	
}

static const struct file_operations nori_fops = { .owner = THIS_MODULE,
						  .read = nori_read,
						  .write = nori_write };

static int __init nori_init(void)
{
	cdev_init(&nori_cdev, &nori_fops);

	get_random_bytes(devbuf, BUFFER_SIZE);
	pr_info("buffer initialized.");

	pr_info("device initialized.");
}

static int __exit nori_exit(void)
{
}

module_init(nori_init);
module_exit(nori_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NOR1EQ0");
MODULE_DESCRIPTION("NORIDEV Hello.ko");
