// задание: рассчитать расстояние Voyager 1 до земли
#include<linux/kernel.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include <linux/version.h>

#define procfsName "SomeTest"
static struct proc_dir_entry *our_proc_file = NULL;

static ssize_t procfile_read(
struct file *file_pointer, char __user *buffer,
size_t buffer_length, loff_t* offset) 
{
    //Идея: зашел на сайт, украл расстояние, затем замерил время через ktime, получил расстояние на момент замера
    //Затем считается разница от момента замера и умножается на скорость 17км/с
    u64 checkpointDistKm = 21179366657ULL;
    u64 checkpointTime = 1765523803ULL;
    u64 voyagerSpeedKmPerSec = 17ULL;
    
    u64 curTime = ktime_get_real_seconds();
    
    if (*offset > 0) 
    {
        return 0; 
    }
    u64 dist = (curTime-checkpointTime)*voyagerSpeedKmPerSec+checkpointDistKm;
    u64 distAE = (dist/149597870ULL);
    char s[256] = "";
    u64 len = snprintf(s, sizeof(s), "Voyager-1 distance to earth: %llu km | %llu AE\n", dist, distAE);
    
    if (copy_to_user(buffer, s, len)) 
    {
        printk(KERN_ERR "copy_to_user failed\n");
        return -EFAULT;
    }
    
    pr_info("procfile read %s\n",file_pointer->f_path.dentry->d_name.name);
    
    *offset = len;
    return len;
}

static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
};

static int __init ModuleInit(void)
{  
    pr_info("Welcome to the Tomsk State University\n");
    our_proc_file = proc_create(procfsName, 0644, NULL, &proc_file_fops);
    return 0;
}  
  
static void __exit ModuleExit(void)  
{
    proc_remove(our_proc_file);
    pr_info("Tomsk State University forever!\n");
    pr_info("/proc/ removed\n");
}  
  
module_init(ModuleInit);  
module_exit(ModuleExit);
MODULE_LICENSE("GPL");
