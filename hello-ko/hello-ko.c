#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

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

#ifndef KBUILD_MODNAME
#define KBUILD_MODNAME DEVICE_NAME
#endif

static dev_t dev_num;
static struct cdev nori_cdev;
static struct class *nori_class;
static struct device *nori_device;

static char devbuf[BUFFER_SIZE];

static ssize_t nori_read(struct file *filp, char __user *buf, size_t count,
			 loff_t *fpos)
{
	ssize_t ret;
	unsigned long copy_bytes;
	char *head;

	if (!count)
		return 0;

	head = devbuf + *fpos;
	if (*fpos + count > BUFFER_SIZE) {
		count = BUFFER_SIZE - *fpos;
		*fpos = BUFFER_SIZE;
	}
	copy_bytes = copy_to_user(buf, head, count);
	ret = count - copy_bytes;
	*fpos = *fpos >= BUFFER_SIZE ? 0 : *fpos + ret;

	return ret;
}

static ssize_t nori_write(struct file *filp, const char __user *buf,
			  size_t count, loff_t *fpos)
{
	ssize_t ret;
	unsigned long copy_bytes;
	char *head;

	if (!count)
		return 0;

	head = devbuf + *fpos;
	if (*fpos + count > BUFFER_SIZE) {
		count = BUFFER_SIZE - *fpos;
		*fpos = BUFFER_SIZE;
	}
	copy_bytes = copy_from_user(head, buf, count);
	ret = count - copy_bytes;
	*fpos = *fpos >= BUFFER_SIZE ? 0 : *fpos + ret;

	return ret;
}

static int nori_open(struct inode *inode, struct file *filp)
{
	pr_info("device opened.");
	return 0;
}

static int nori_release(struct inode *inode, struct file *filp)
{
	pr_info("device closed.");
	return 0;
}

static const struct file_operations nori_fops = { .owner = THIS_MODULE,
						  .read = nori_read,
						  .write = nori_write,
						  .open = nori_open,
						  .release = nori_release };

static int __init nori_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (ret)
		return ret;
	pr_debug("new device number: %d", dev_num);

	cdev_init(&nori_cdev, &nori_fops);
	nori_cdev.owner = THIS_MODULE;
	ret = cdev_add(&nori_cdev, dev_num, 1);
	if (ret)
		goto err_cdev;

	nori_class = class_create(CLASS_NAME);
	if (IS_ERR(nori_class)) {
		ret = PTR_ERR(nori_class);
		goto err_class;
	}

	nori_device =
		device_create(nori_class, NULL, dev_num, NULL, DEVICE_NAME);
	if (IS_ERR(nori_device)) {
		ret = PTR_ERR(nori_device);
		goto err_device;
	}

	get_random_bytes(devbuf, BUFFER_SIZE);
	pr_info("buffer initialized.");

	pr_info("device initialized.");
	return 0;

err_device:
	class_destroy(nori_class);
err_class:
	cdev_del(&nori_cdev);
err_cdev:
	unregister_chrdev_region(dev_num, 1);
	return ret;
}

static int __exit nori_exit(void)
{
	device_destroy(nori_class, dev_num);
	class_destroy(nori_class);
	cdev_del(&nori_cdev);
	unregister_chrdev_region(dev_num, 1);
	return 0;
}

module_init(nori_init);
module_exit(nori_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NOR1EQ0");
MODULE_DESCRIPTION("NORIDEV Hello.ko");
