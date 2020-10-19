#include<iostream>
using namespace std;
class Date
{
    public:
        Date(int year = 1999, int month = 11, int day = 2)  //构造
            :_year(year)
             , _month(month)
             , _day(day)
    {
        cout << "Date(int,int,int):" << this << endl;

    }
        Date(const Date& temp)                              //拷贝构造
            :_year(temp._year)
             , _month(temp._month)
             , _day(temp._day)
    {
        cout << "Date(const Date&):" << this << endl;

    }
        ~Date()                                             //析构,先析构最后创建的对象
        {
            cout << "~Date():" << this << endl;

        }
        Date& operator=(const Date& temp)                   //函数运算符重载
        {
            if (this != &temp)
            {
                _year = temp._year;
                _month = temp._month;
                _day = temp._day;

            }
            return *this;

        }
    private:
        int _year;
        int _month;
        int _day;

};

void Test()
{
    Date d1(2020, 10, 19);
    Date d2(2020, 10, 20);
    Date d3(d2);
    d2 = d1;
    d1.operator=(d2.operator=(d3));
}
Date TestDate3(Date d)
{
    return d;
}
void TestDate()
{
    Date d1(2020, 10, 14);
    Date d2(2020, 10, 15);
    d1 = TestDate3(TestDate3(d2));
}
int main()
{
    TestDate();
    return 0;
}
