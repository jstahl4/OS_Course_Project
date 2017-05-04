#include<iostream>
using namespace std;

void create(int,int);
void del(int);
void compaction();
void display();

int fname[10],fsize[10],fstart[10],freest[10],freesize[10],m=0,n=0,start;

int main()
   {
     int name,size,ch,i;
     int *ptr;
     ptr=(int *)malloc(sizeof(int));
     start=freest[0]=*ptr;
     freesize[0]=500;

     cout <<"\n\n";
     cout <<" Free start address         Free Size        \n\n";

     for(i=0;i<=m;i++){
       cout << freest[i] << "\t" << freesize[i];
       cout <<"\n\n";
     }
     while(1){
      cout << "1.Create.\n";
      cout << "2.Delete.\n";
      cout << "3.Compaction.\n";
      cout << "4.Exit.\n";
      cout << "Enter your choice: ";
      cin >> ch;
      switch(ch)
       {
         case 1:
            cout << "\nEnter the name of file: ";
              cin >> name;
            cout << "\nEnter the size of the file: ";
              cin >> size;
            create(name,size);
            break;
         case 2:
            cout <<"\nEnter the file name which u want to delete: ";
              cin >> name;
            del(name);
            break;
         case 3:
             compaction();
             cout << "\nAfter compaction the tables will be:\n";
             display();
             break;
         case 4:
             exit(1);
         default:
             cout <<"\nYou have entered a wrong choice.\n";
       }
    }

 }


 void create(int name,int size)
   {
     int i,flag=1,j,a;

      for(i=0;i<=m;i++)
    if( freesize[i] >= size)
       a=i,flag=0;
      if(!flag)
       {
     for(j=0;j<n;j++);
         n++;
     fname[j]=name;
     fsize[j]=size;
     fstart[j]=freest[a];
     freest[a]=freest[a]+size;
                freesize[a]=freesize[a]-size;

                cout << "\n The memory map will now be: \n\n";
     display();
       }
      else
       {
        cout << "\nNo enough space is available.System compaction......";

         flag=1;

         compaction();
         display();

        for(i=0;i<=m;i++)
           if( freesize[i] >= size)
        a=i,flag=0;
     if(!flag)
      {
       for(j=0;j<n;j++);
          n++;
       fname[j]=name;
       fsize[j]=size;
       fstart[j]=freest[a];
       freest[a]+=size;
       freesize[a]-=size;
       cout << "\n The memory map will now be: \n\n";
       display();
      }
     else
     cout << "\nNo enough space.\n";
       }
  }

void del(int name)
  {
    int i,j,k,flag=1;
     for(i=0;i<n;i++)
       if(fname[i]==name)
           break;
     if(i==n)
       {
    flag=0;
    cout << "\nNo such process exists......\n";
       }
      else
       {
       m++;
       freest[m]=fstart[i];
       freesize[m]=fsize[i];
     for(k=i;k<n;k++)
                   {
           fname[k]=fname[k+1];
                      fsize[k]=fsize[k+1];
                      fstart[k]=fstart[k+1];
                   }
                   n--;
       }
      if(flag)
       {
     cout << "\n\n After deletion of this process the memory map will be : \n\n";
     display();
       }
   }

void compaction()
  {
    int i,j,size1=0,f_size=0;
      if(fstart[0]!=start)
       {
    fstart[0]=start;
    for(i=1;i<n;i++)
      fstart[i]=fstart[i-1]+fsize[i-1];
      }
     else
       {
      for(i=1;i<n;i++)
       fstart[i]=fstart[i-1]+fsize[i-1];
       }
     f_size=freesize[0];

     for(j=0;j<=m;j++)
       size1+=freesize[j];
      freest[0]=freest[0]-(size1-f_size);
      freesize[0]=size1;
      m=0;
 }

void display()
    {
     int i;

     cout << "\n  ***   MEMORY MAP TABLE  ***        \n";
     cout << "\n\nNAME     SIZE    STARTING ADDRESS      \n\n";
      for(i=0;i<n;i++)
       cout << fname[i] << "\t\t" << fsize[i] << "\t\t" << fstart[i] << endl;
     cout << "\n\n";
     cout << "\n\n***  FREE SPACE TABLE  ***\n\n";
     cout << "FREE START ADDRESS             FREE SIZE        \n\n";
     for(i=0;i<=m;i++)
     cout << freest[i] << "\t\t" << freesize[i] << endl;
   }
