int main(void){
    //测试注释
    char* str = "123";//这是注释
    //行注释
    /*
    块注释

    */
    //测试实数
    int a = 0x13f;
    int a = 0x13;
    int a = 000013;
    int a = 0b11;
    double a = 0.12;
    double b = 1e-10;
    double b = 1.0e10;
    double b = 18.1;
    //测试字符串
    char s = 'c';
    char s = '\n';
    char* str = "char\n\"";
    //测试不同数量的界符
    a=a<=b?a<<1:a<<2;
    a<<=1;
    b++;
    if(!a){
        return 0;
    }
    return a+b;
}