#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

#define DEVICE_NAME "NORIDEV"
#define CLASS_NAME "NORIDEV_CLASS"

static dev_t dev_num;
static struct cdev nori_cdev;
static struct class *nori_cdev;
static struct device *nori_device;



static const struct file_operations fops = {

};

static int __init nori_init(void) {
    
}


module_init(nori_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NOR1EQ0");
MODULE_DESCRIPTION("NORIDEV Hello.ko");
