#include <bits/stdc++.h>
using namespace std;
//struct declaration
typedef struct class
{
    int roll_no;
    string sname;
}data;
//function to sort
bool compare(data a, data b)
{
//for descending order replace with a.roll_no >b.roll_no
    if(a.roll_no < b.roll_no) return 1;
    else return 0;
}
//main function
int sort()
{
    int n,i;
    cout<<"Enter the Total number of students : \n";
    cin>>n;
//array of structure creation
    data array[n];
    cout<<"Enter roll number and student name :\n";
    for(i=0;i<n;i++)
{
    cin>>array[i].roll_no;
    cin>>array[i].sname;
}
    sort(array,array+n,compare); //sort function call
    cout<<"Sorted list : "<<endl;
    for(i=0;i<n;i++)
{
    cout<<array[i].roll_no<<" ";
    cout<<array[i].sname<<endl;
}


return 0;
}