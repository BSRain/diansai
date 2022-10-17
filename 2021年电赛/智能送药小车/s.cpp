#include<iostream>
#include<stdlib.h>
using namespace std;
 int count;
int main()
{
  
    int num;
     int temp,sign,result;
   cin >> num;
    if(num==0)
        return 0;
 
    sign=(num<0)?-1:1;
    temp=sign*num;
    result=0;
    do
    {
        result=result*10+temp%10;
        temp=temp/10;
        count++;
    }while(temp);
    cout<<count<<"Î»Êı"<<endl;
    cout<<result*sign<<endl;

}
