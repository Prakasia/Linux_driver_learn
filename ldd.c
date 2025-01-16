#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define MAX         1024


MODULE_LICENSE("GPL"); //*
MODULE_AUTHOR("Shalu"); //!
MODULE_DESCRIPTION("Basic loadable kernel module"); //!

char message_from_user[MAX];
size_t mess_len = 0;

static struct proc_dir_entry *custom_proc_entry;
/* struct proc_dir_entry *proc_create(const char *name, 
                                    umode_t mode, 
                                    struct proc_dir_entry *parent, 
                                    const struct proc_ops *proc_ops); 
                                    
    extern void proc_remove(struct proc_dir_entry *); 

    ssize_t	(*proc_read)(struct file *, char __user *, size_t, loff_t *);

    ssize_t	(*proc_write)(struct file *, const char __user *, size_t, loff_t *);
    
    For sending an acknowledge to the user space copy_to_user(dst, src, byte)
    -------------------------------------
    in study_driver_write copy_from_user(dst, src, byte)
*/



static ssize_t study_driver_read (struct file* file_pointer,
                            char *user_space_buffer,
                            size_t count, 
                            loff_t* offset){
    int result;
/*     char msg[] = "Ack!\n";
    size_t len = strlen(msg); */

    size_t len = strlen(message_from_user);
    printk("study driver read\n");
    if(*offset >= len){
        return 0;
    }

    result = copy_to_user(user_space_buffer, message_from_user, len);
    *offset += len;
    return len;
}

static ssize_t	study_driver_write (struct file * file_pointer,
                                const char *buff, 
                                size_t count, 
                                loff_t *offset){
    int result;
    printk("study driver write\n");
    result = copy_from_user(message_from_user, buff, count);
    message_from_user[count] = '\0';
    return count;
    
}



struct proc_ops driver_proc_ops = {
    .proc_read = study_driver_read,
    .proc_write = study_driver_write
};

static int new_module_init(void){

    printk("study_driver_init : entry\n");

    custom_proc_entry = proc_create("study_driver",
                                0,
                                NULL,
                                &driver_proc_ops);

    printk("study_driver_init : exit\n");
    
    return 0;
}

static void new_module_exit(void){

    printk("study_driver_exit : entry\n");
    proc_remove(custom_proc_entry);
    printk("study_driver_exit : exit\n");

}        

module_init(new_module_init);
module_exit(new_module_exit);