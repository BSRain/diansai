//FLASH.h
void write_flash_char (unsigned int addr, char *array,int count);
void write_flash_int (unsigned int addr, int *array,int count);
char read_flash_char0(unsigned int addr);
void read_flash_char1(unsigned int addr,char *array,int count); //��һ������
int read_flash_int0(unsigned int addr); //ż��ַ,��һ����
void read_flash_int1(unsigned int addr,int *array, int count); //����������
void init_flash(void);