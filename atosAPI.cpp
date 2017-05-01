/**********************************************************************
 * Tom Kueny
 **********************************************************************/
#include "File.h"
#include "Directory.h"
using namespace DISK_API;
namespace ATOS_API
{
	int fname[10],fsize[10],fstart[10],freest[10],freesize[10],m=0,n=0,start;
	bool  Create(std::string const& aFileName, int fileSize) {
			  File newFile(aFileName, fileSize);
			  add_file(newFile); //add file to set, no size = no writing, yet
			  //below is probably better for write method
		// 	  int i,flag=1,j,a;
		// 	  for(i=0;i<=m;i++){
		// 	 		if( freesize[i] >= size)
		// 				a=i,flag=0;
		// 	  if(!flag){
		// 	  	for(j=0;j<n;j++);
		// 		n++;
		// 	  	fname[j]=name;
		// 	  	fsize[j]=size;
		// 	  	fstart[j]=freest[a];
		// 	  	freest[a]=freest[a]+size;
		// 		freesize[a]=freesize[a]-size;
		   //
		// 		cout << "\n The memory map will now be: \n\n";
		// 	  	display();
		// 	  }
		// 	  else
		// 	  {
		// 		cout << "\nNo enough space is available.System compaction......\n";
		   //
		// 		flag=1;
		// 		compaction();
		// 		display();
		// 	 	for(i=0;i<=m;i++)
		// 			if( freesize[i] >= size)
		// 		 a=i,flag=0;
		// 	  if(!flag)
		// 	   {
		// 		for(j=0;j<n;j++);
		// 		   n++;
		// 		fname[j]=name;
		// 		fsize[j]=size;
		// 		fstart[j]=freest[a];
		// 		freest[a]+=size;
		// 		freesize[a]-=size;
		// 		cout << "\n The memory map will now be: \n\n";
		// 		display();
		// 	   }
		// 	   else
		// 	  	cout << "\nNo enough space.\n";
		// 	   }
		//    }

	}
	bool	Delete(std::string const& aFileName*) {
				remove_file(aFileName);
				//TODO: before/after file is removed from list, deallocate blocks
				//  int i,j,k,flag=1;
				//  for(i=0;i<n;i++)
				//  	if(fname[i]==name)
				// 	   break;
				//  if(i==n){
				// 	   	flag=0;
				// 		cout << "\nNo such file exists......\n";
				//  }
				//  else{
				// 		m++;
				// 		freest[m]=fstart[i];
				// 	    freesize[m]=fsize[i];
				//  	    for(k=i;k<n;k++){
				// 			   fname[k]=fname[k+1];
				// 			   fsize[k]=fsize[k+1];
				// 			   fstart[k]=fstart[k+1];
				// 		}
				// 	    n--;
				//   }
				//   if(flag)
				//   {
				//  	cout << "\n\n After deletion of this process the memory map will be : \n\n";
				//  	display();
				//   }
	}
	int		Open(std::string const& /*aFileName*/) {
			//find file (search algorithm?)
			//if file is not in disk, throw error
			//if file is in disk -> open file and allow for read/write
			//return 1 if successful, else
			return 0;
	} // 0 is failed.
	bool	Close(int /*aHandle*/) {
			//close file
			//return pointer to initial position
			return true;
	}
	int		Read(int /*handle*/, int /*numchards*/, char* /*buffer*/) {
			//pass data to buffer
			//increment numchars
			//handle?
			return 0;
	}
	int		Write(File &obj, int numchards, char const* newBuffer){
			//write input
			obj.buffer += newBuffer;
			//save input to file
			//write file to DISK
			return numchards;
	}

	int		Stats(std::string const& /*aFileName*/) { return 5; };
	std::vector<std::string> List() { std::vector<std::string> res; res.push_back("asapasasdFile1"); return res; }

	// int 	findFreeSpace(int fileSize);
	//compaction algorithm goal:
	//move files with free space in between them to be compacted on top of one another
	//in order to leave the highest amount of space left
	void    compaction(){
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
	// void 	display(){
	//      int i;
	//
	//      cout << "\n  ***   MEMORY MAP TABLE  ***        \n";
	//      cout << "\n\nNAME     SIZE    STARTING ADDRESS      \n\n";
	//      for(i=0;i<n;i++)
	//        cout << fname[i] << "\t\t" << fsize[i] << "\t\t" << fstart[i] << endl;
	//      cout << "\n\n";
	//      cout << "\n\n***  FREE SPACE TABLE  ***\n\n";
	//      cout << "FREE START ADDRESS             FREE SIZE        \n\n";
	//      for(i=0;i<=m;i++)
	//      cout << freest[i] << "\t\t" << freesize[i] << endl;
	//    }
}
