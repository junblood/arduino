int potPin = 0; //定义模拟接口0 连接LM35 温度传感器
int ledred = 4;
int ledblue = 7;
void setup()
{
Serial.begin(9600);//设置波特率
pinMode(ledred, OUTPUT); 
pinMode(ledblue, OUTPUT);
}
void loop()
{
int val;//定义变量
int dat;//定义变量
val=analogRead(0);// 读取传感器的模拟值并赋值给val
dat=(125*val)>>8;//温度计算公式
Serial.print("Tep:");//原样输出显示Tep 字符串代表温度
Serial.print(dat);//输出显示dat 的值
Serial.println("C");//原样输出显示C 字符串
if (dat>=25)
{
  digitalWrite(ledred,HIGH);
  digitalWrite(ledblue,LOW);
}
else
{
  digitalWrite(ledred,LOW);
  digitalWrite(ledblue,HIGH);
}
delay(6000);
}

