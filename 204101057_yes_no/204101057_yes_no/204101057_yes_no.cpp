// 204101057_yes_no.cpp : Defines the entry point for the console application.
//
/*
	Assignment no:02(using recording module)
	Roll NO:204101057
*/
#include "stdafx.h"
#include <iostream>
using namespace std;
#include<fstream>
#include<vector>
//function declaration
void calSTE();
void detectWord(int s,int e);

/*making 2  vector 'ste' (for storing short term energy value)  and 
'arr'( for stroing all the input value of .txt file).
*/
vector <int> ste;
vector <int> arr;

/*
the detectWord() takes two value i.e. start and end of the region in which we need to 
detect yes/no.

*/
void detectWord(int s,int e){
	int zcr=0;//for counting zcr value.
	/*
	pos,neg helps in keeping track whether signal crossing the x-axis.
	*/
	int pos=0,neg=0,flag=0;
	/*sgmtSize just keeps length of region under evaluation*/
	int sgmtSize=e-s+1;
	/*toStart help in making sure in counting zcr value in last 15% of the region only.*/
	int toStart=s+sgmtSize*0.85;
	
	pos=arr[toStart]>=0?1:0;
	neg=arr[toStart]<0?1:0;
	for(int i=toStart;i<e;i++){
		/*
			if initially signal is in positive side and moves to negative side then increment zcr value.
		*/
		if(pos==1 && arr[i]<0){
			zcr++;
			pos=0;
			neg=1;
		}
		/*
			if initially signal is in negative side and moves to positive side then also increment zcr value.
		*/
		if(neg ==1 && arr[i]>0){
			zcr++;
			pos=1;
			neg=0;
		}


	}
	
	/*
		After observing few patterns decided that if zcr>120 then say 'yes' else say 'no' for lesser zcr value but should also be greater than 15 
		and ignoring at remaining  zcr values.
	*/
	
	if(zcr>120)cout<<"\tYES"<<endl;
	else if(zcr>15 && zcr<120)cout<<"\tNO"<<endl;
}
/*
	calSTE() reads the .txt file and claculate the short term energy value of 320 unit at a time and 
	stored in vector ste.In this I am also inserting all input value from .txt file to vector 'arr'.
*/
void calSTE(){
	long long x;
	ifstream readFile;
	/*input the file name here*/
	readFile.open("yesNoIn.txt");

	if (!readFile) {
        cout << "Error in opening file";
        exit(1); // terminate with error
    }
	/*read the file till it is not empty*/
	while(!readFile.eof()){
		int count=0;
		long long sum=0;
		/*
			Reading the input one by one and finding ste value of 320 unit and pushing into ste vector.
		*/
		while (readFile >> x && count<320) {
			arr.push_back(x);
			sum = sum + x*x;
			count++;
		}
		int temp=sum/320;
		
		ste.push_back(temp);
	}
	
	readFile.close();
	
}

int _tmain(int argc, _TCHAR* argv[])
{	/*using system() to call Recording_Module.exe and providing some delay for recording speech and saving in file,which is further being used.*/
	system("Recording_Module.exe 8 yesNoIn.wav yesNoIn.txt");
	/*calling calSTE() for calculating ste values.*/
	 calSTE();
	 
	 /*j variable helps in reading ste vector*/
	 int j=0;
	 long long en;
	 /*
		cntUnt helps in eliminating randon high ste value, and make sure that only considering the region having atleast 5 continuous
		ste>10000 for our purpose.
	 */
	 int cntUnt=0;
	 /*start will contains the starting marker and end will contain end marker for deciding our region for evaluation
	    flag will help in setting start and end marker. sgmt will helps in deciding when to call decideWord().
	 */
	 int start=0,end=0,flag=0,sgmt=0;
	 int count=0;
	 cout<<"You have Spoken :-"<<endl;
	 for(int i=0;i<arr.size()-5*320;i++){
		count++;
		if(count==320){
			count=0;
			en=ste[j];
			/*
				only considering the ste value which are > 10000, as after observation found that all the ste values of 
				silent was less than 10000  .
			*/
			if(en>10000){
			
				cntUnt++;
				if(flag==0){flag=1;
					start=i;
				}
			}
			else if(flag==1){
				end=i;
				sgmt=1;
				flag=0;
			}
			

			j++;
		}
		/*
			calling detectWord() only when sgmt=1 and cntUnt>5 i.e atleast 6 continuous higher ste value for considering.
		*/
		if(sgmt==1 && cntUnt>5)
			{	
				detectWord(start,end);
			 sgmt=0;
			 cntUnt=0;
			
			}
	 }
	 return 0;
}


