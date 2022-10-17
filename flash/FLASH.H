//FLASH.h
void write_flash_char (unsigned int addr, char *array,int count);
void write_flash_int (unsigned int addr, int *array,int count);
char read_flash_char0(unsigned int addr);
void read_flash_char1(unsigned int addr,char *array,int count); //读一串数据
int read_flash_int0(unsigned int addr); //偶地址,读一个字
void read_flash_int1(unsigned int addr,int *array, int count); //读整形数组
void init_flash(void);