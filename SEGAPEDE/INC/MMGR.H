
/* Prototypes for MMGR.C */
    void mmgr_init(unsigned char *location, unsigned short size, unsigned short indices);
    void mount_block(unsigned char *location, unsigned short size);
    unsigned short alloc_record(unsigned short size);
    void free_record(short index);
    void *lock_record(short index);
    void unlock_record(short index);
    void garbage_collect(void);
