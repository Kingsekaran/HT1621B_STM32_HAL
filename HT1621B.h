#ifndef  HT1621B_H_
#define HT1621B_H_   //防止重复包含

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

//#define CS   13  //Pin 2 as chip selection output
//#define WR   12  //Pin 3 as read clock output
//#define DATA 8  //Pin 4 as Serial data output

#define CS1     HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_SET)//digitalWrite(CS, HIGH)
#define CS0     HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_RESET)//digitalWrite(CS, LOW)
#define WR1     HAL_GPIO_WritePin(GPIOA, WR_Pin, GPIO_PIN_SET)//digitalWrite(WR, HIGH)
#define WR0     HAL_GPIO_WritePin(GPIOA, WR_Pin, GPIO_PIN_RESET)//digitalWrite(WR, LOW)
#define DATA1   HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET)//digitalWrite(DATA, HIGH)
#define DATA0   HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_RESET)//digitalWrite(DATA, LOW)

#define uchar   unsigned char
#define uint   unsigned int

#define  ComMode    0x52
#define  RCosc      0x30
#define  LCD_on     0x06
#define  LCD_off    0x04
#define  Sys_en     0x02
#define  CTRl_cmd   0x80
#define  Data_cmd   0xa0

//class HT1621B
//{
//public:
//HT1621B();
//	void HT1621B_begin(int cs, int wr, int data);
void HT1621B_begin();
void Init_1621(void);
void HT1621_all_on(uchar num);
void HT1621_all_off(uchar num);
void num_print(int num, short int dp);
void Err_print();
void minus_print();
void battey_level(short unsigned int level);
void signal_method(short unsigned int method, short unsigned int strength);
void channel_count(int channel);
void units(short unsigned int unit); // temp=0, humi=1
void clear(int len); // length is 16
void symbol_HIGH();
void symbol_LOW();
void symbol_REFRESH();
void set_print();
void ON_print();
//private:
//int CS;
//int WR;
//int DATA;
uchar addr;
void Write_1621(uchar addr, uchar sdata);
void SendCmd_1621(uchar command);
void SendBit_1621(uchar sdata, uchar cnt);

//};
#endif
