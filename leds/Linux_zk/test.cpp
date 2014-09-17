#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
   unsigned char temp[2];
   cin >> temp;
   while(temp[1])
   {
      cout << setw(2) << setfill('0') << temp[0] - 160;  // 高 2 位
      cout << setw(2) << setfill('0') << temp[1] - 160 << endl; // 低 2 位
      temp[1] = 0;
      cin >> temp;
   }
   return 0;
}
