/*
		Roll No:204101057
		Task: Vowel Recognition
*/

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <iomanip>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
#include<math.h>
vector <long double> finSig;
vector <long double> arr;

vector <long double> sineWind;
vector <long double> ai_coefficient;
vector <long double> cepstral;
vector <long double> cepstralRaised;
static const vector <vector<long double>> cepstral2d_zero(50,vector<long double>(12,0));
vector <vector<long double>> cepstral2d(50,vector<long double>(12,0));//creating 2d vector

vector <vector<long double>> dumpedData(25,vector<long double>(12,0));
vector <vector<long double>> tokhuraDi(10,vector<long double>(5,0));
/*
	dcShift() is used for finding the dc-Shift value in the file and return that value,which is later subtracted from 
	each input value of the file.
	For finding the dc-shift, I am using the strating 1100 sample i.e I am finding average using these values.
	I am performing dc-shift for each file seperatly.
*/
long double dcShift(string file_name){
	long double x;
	ifstream readFile;
	readFile.open(file_name);
	if (!readFile) {
        cout << "Error in opening file";
        exit(1); // terminate with error
    }
	
	int count=0;
	long double sum=0.0;
	long double dcVal;
	//reading the file and finding average.
	while(!readFile.eof()){
			while (readFile >> x ) {
				sum = sum + x;
				count++;
				if(count==1100)
				{
					dcVal=(long double)sum/count;
					//	cout<<"DC-Shift :"<<dcVal<<endl;
					return dcVal;
				}
			}
	}
	readFile.close();
}
/*
		raisedSineWindow is finding the value of sine window  at all 12 points and storing that in a vector sineWind. This will be used further while raising 
		the ci_coefficient.
*/
void raisedSineWindow(){
	int Q=12;
	for(int m=1;m<=Q;m++){
		long double temp=1+(6.0*sin(m*(3.14/12)));
	
		sineWind.push_back(temp);
	}

}
/*
	normalization() finds the value which has highest amplitude and normalize all the amplitude within 10000,i.e if original value in greater than 10000
	then it will compress that within 10000.
*/

void normalization(){
	long double max=arr[0];
	//finding the highest amplitude sample.
	for(unsigned int i=0;i<arr.size();i++){
		if(abs(arr[i])>abs(max))
			max=abs(arr[i]);
	}
	long double highAmplitude=max;

	//now limiting the highest amplitude to 10000,need to multiply every sample value whith a factor.
	long double factor=10000.0/highAmplitude;
	
	
	//performing normalization by multiplying each sample value by factor and at the same time doing framing i.e each frame size of 320 sample and putting all the 
	//last value to zero if not able to form one complete frame.
	for(unsigned int i=0;i<arr.size();i++)
	{
			arr[i]=(arr[i]*factor);
		
	}

	return;
}


//for calculating the cepstral coefficient.
void cepstralCoeff(int frameNo){
	//start will contain the starting index of each frame, as ai_coefficent and cepstral vectors have values for all 5-frames.
	int start=frameNo*12;
	long double c;
	
	int p=12;
	long double sum=0.0;
	
	int i=0;
	for(int m=1;m<=p;m++){
		sum=0.0;
		
		for(int k=1;k<m;k++)
		{	sum=sum+((long double)k/(long double)m)*cepstral[k-1]*ai_coefficient[start+m-1-k];}
		c=ai_coefficient[start+m-1]+sum;
		
		
		i++;
		//cout<<c<<" ";
		cepstral.push_back(c);
		
	}
	
	//Applying raised sine window on cepstral coefficient
	int j=0;
	long double sum1=0.0;
	for(int i=start;i<start+12;i++)
	{
		
		long double cRaised=cepstral[i]*sineWind[j%12];
		cepstralRaised.push_back(cRaised);
		//cout<<cRaised<<" ";
		
		j++;
	}
}



//now call durbin's algorthm for each frame.
void cal_ri_ai(int start){
	int p=12;
	
	// r's calculation firstly.
	//r-vector will store  the r's value of each frame.
	vector <long double> r;
	long double *k=new long double[p+1];
	long double *e=new long double[p+1];
	//creating 2d-array for storing alpha's.
	long double ** alpha = new long double*[p+1];
	for(int i = 0; i < p+1; ++i)
	{
		alpha[i] = new long double[p+1];
		
	}
	
	long double sum=0.0;
	
	for(int i=0;i<=p;i++)
	{	sum=0;
		for(int m=start;m<start+320-i;m++){
			sum=sum+(finSig[m]*finSig[m+i]);
			
		}
		r.push_back(sum);
		
	}
	/*cout<<"R's value are :"<<endl;
	for(int i=0;i<r.size();i++)
		cout<<r[i]<<" ";
	cout<<endl;*/

	//now, Durbin's algorithm  code

	e[0]=(r[0]);
	for(int i=1;i<=p;i++){
		long double temp_k=0;
		for(int j=1;j<i;j++){
			temp_k=temp_k+(alpha[i-1][j]*r[i-j]);
		}
		temp_k=(r[i]-temp_k)/e[i-1];
		k[i]=(temp_k);
		alpha[i][i]=(k[i]);
		
		
		for(int j=1;j<i;j++){
			alpha[i][j]=alpha[i-1][j]-k[i]*alpha[i-1][i-j];	
		}
		e[i]=(1-(k[i]*k[i]))*e[i-1];

	}
	//cout<<"coeffiencts are:"<<endl;
	for(int i=1;i<=12;i++){
		ai_coefficient.push_back(alpha[p][i]);
		//cout<<alpha[p][i]<<" ";
	}  
	r.clear();
	
	//cout<<endl;

}
/*
		steadyFrame() helps in finding 5 frames, which are around the maximum amplitude samples in the data.For this I have find out the 
		sample having maximum ste value and after getting that index, I am going (160*5) samples back from max-ste sample index and 
		from that point going rightward (320*5) samples and storing the value of these 5-frames to a vector "finSig".
*/
void steadyFrame(){
	long double sum=0;
	long double max=0;
	int max_ind=0;
	for(unsigned int i=0;i<arr.size();i++){
		sum=sum+arr[i]*arr[i];
		if((i+1)%320==0){
			if(sum>max){
				max=sum;
				max_ind=i;
			}
			sum=0;
		}
	}
	int start_ind=max_ind-(160)*5;
	
	for(int i=start_ind;i<start_ind+320*5;i++)
		finSig.push_back(arr[i]);
	
}

/*
		utility_train_test() is a general function which is used during both traning and testing time.This function is called for each file .
		It will call dcShift() and read the corresponding .txt file and subtract the dcShift-values and stores that value in arr-vector.
		Then It calls normalization() ,steadyFrame(), and then calls cal_ri_ai() and cepstralCoeff() for each of the 5 Frames.
*/
void utility_train_test(string file_name,int file_no){
	long  double x;
	long double dcValue=dcShift(file_name);
	ifstream readFile;
	readFile.open(file_name);
	if (!readFile) {
        cout << "Error in opening file";
        exit(1); // terminate with error
    }
	
	int c=0;
	int count=0;
	long double sum=0;

	while(!readFile.eof()){
			while (readFile >> x ) {
				arr.push_back(x-dcValue);
		}
	}
	
    readFile.close();

	normalization(); 
	
	steadyFrame();
	//selecting 5 frames.
	

	int totalFrame=5;
	/*
		In my case cepstral-vector ,ai_coefficient vector  will contains the ci's and ai's of complete 5-frame(i.e 5*12) values will be there.
	*/
	for(int frameNo=0;frameNo<totalFrame;frameNo++)
	{	//cout<<"\nFrame "<<frameNo<<endl;
		//passing the starting value of the corresponding frame in cal_ri_ai(). 
		cal_ri_ai(frameNo*320);
		//passing the frameNo as argument.
		cepstralCoeff(frameNo);
		
	}
	/*
			cepstral2d is a 2d-array of(50*12-dimension) which in which i have stored the calculated raised cepstral coefficients of each frame of all 10-files.
			i.e first 5 row will be of file 1 ,next 5 rows will be of file 2 and similarly for all files.
			cepstralRaised vector will contain raised cepstral values of each file i.e (5*12) values,which is stored in cepstral2d array .

	*/


	int row=(file_no)*5;
	int k=0;
	for(int i=row;i<row+5;i++)
	{	//storing the raised cepstral calues in cepstral2d array after locating correct rows in cepstral2d.
		for(int j=0;j<12;j++){
			
			cepstral2d[i][j]=cepstralRaised[(k)];
			k++;
		}
	}
	//un-comment the code to see the raised cepstral values for all files.
	/*for(int i=row;i<row+5;i++)
	{
		for(int j=0;j<12;j++){
			cout<<cepstral2d[i][j]<<"  ";
		}
		cout<<endl;
	}*/
}
/*
		tain() will call utility_train_test().After getting the cepstral2d array values having the raised cepstral values for all the 10 files.
		Calculated the average cepstal values by adding frame1 of each file ,similarly for frame2 ,and so on and stored these values in sum_c -vector.
		and further these values are dumped as reference file for corresponding vowels.which will be used while testing.
*/
void train(string vowel){
	raisedSineWindow();
	
	for(int train_no=1;train_no<=10;train_no++){
		stringstream ss;
		ss<<train_no;
		string s=ss.str();
		string file_name=vowel+"/204101057_"+vowel+"_"+(s)+(string)".txt"	;
		
		utility_train_test(file_name,train_no-1);

	
	vector <vector<long double>> sum_c(5,vector<long double>(12,0.0));
	/*
		calculating the average of corresponding frames of all files.
	*/
	for(int fr_no=0;fr_no<5;fr_no++){
		for(int i=0 ;i<12;i=i+1){
			for(int j=fr_no;j<50;j+=5){
				sum_c[fr_no][i]=sum_c[fr_no][i]+cepstral2d[j][i];
			}		
		}
	}
	
		//storing the calculated average cepstral values as .txt file.
	
	ofstream wtFile;
	wtFile.open(vowel+"_out.txt");
	for(int i=0;i<5;i++){
		for(int j=0;j<12;j++){
			long double temp=sum_c[i][j]/10.0;
			wtFile<<temp<<"\n";
		}
	}
	wtFile.close();

	//clearing all the vectors,so that fresh values can we loaded for new file.
	arr.clear();
	ai_coefficient.clear();
	cepstral.clear();
	cepstralRaised.clear();
	finSig.clear();

    
}
		cepstral2d=cepstral2d_zero;	

}


/*
	tokhuraDistanceCal() will calculate the tokhura distance of the testing vowel with all the reference vowel file.we will get one distance for each frame
	thus total we will get 5-frame so take average of these distance and stroes the value in tokhuraDi vector at appropriate position.
	tokhruDi vector will be of (10*5)dimension in which each row will store the distance of each testing file and each column value of 
	a row will corresponds to distance of that file with vowels i.e at index=0 distance will be from vowel-a ,at index=1 distance will be for
	vowel-e ,index-2 will be for vowel-i ,index-3 will be of vowel-o ,index-4 will be of vowel-u.
*/
void tokhuraDistanceCal(int row){
	long double w[12]={1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0 };
	long double sum=0;
	long double dist=0;
	int ind=0;
	for(int k=0;k<25;k=k+5){
		for(int frame=0;frame<5;frame++){
			sum=0;
			for(int i=0;i<12;i++){
				sum=sum+w[i]*(cepstral2d[row+frame][i]-dumpedData[frame+k][i])*(cepstral2d[row+frame][i]-dumpedData[frame+k][i]);
			}  
			dist=dist+sum;
			
	}
	
	tokhuraDi[row/5][ind]=dist/5.0;
	dist=0;
	ind++;
	}
}

void tokhuraDist(){
	string vowel_arr[5]={"a","e","i","o","u"};
	
	
	for(int i=0;i<5;i++){
	ifstream readFile;
	
		string file_name=(string)vowel_arr[i]+"_out.txt";
	readFile.open(file_name);
	long double x;
	if (!readFile) {
        cout << "Error in opening file";
        exit(1); // terminate with error
    }
	
	int j=0;
	int k=0;
	/*
			Reading the dumped reference file of vowels, and storing that in a 2-D matrix of size(25*12) i.e it will contain value of 
			all the 5-vowel's reference value.The top-5 row will be value of vowel-"a" with each row as it's one frame similarly for all
			other vowels.
	*/
	while(!readFile.eof()){
		while (readFile >> x ){
			dumpedData[i*5+k][j%12]=x;
			j++;
			if(j%12==0){
				j=j%12;
				k++;
			} 
		}
	}
	readFile.close();
	
	}

	/*
		calling tokhuraDistanceCal() for calculating distance for each file and this function after calculating distance will update that in a matrix.
	*/
	for(int test_set=0;test_set<10;test_set++){
		tokhuraDistanceCal(test_set*5);
		
		
	}
	/*
		un-comment this for loop for printing the resultant tokhura distance, in which each row is 
		tokhura distance for one testing file and each column of a row is a vector.
	*/

	/*
	for(int i=0;i<10;i++){
		for(int j=0;j<5;j++)
			cout<<tokhuraDi[i][j]<<" ";
		cout<<endl;
	}*/
}
/*
		detectVowel() will take the tokhuraDi vector which will contain 10 row i.e for all files of a particular vowel.
		now Simply scan the tokhuraDi vector row wise.The minimum distance value in that row will corresponds to the detected vector.
*/
int detectVowel(int ind,int testSample){
	int count=0;
	string vowel_arr[5]={"a","e","i","o","u"};
	for(int i=0;i<testSample;i++){
		int min_ind=0;
		long double min=tokhuraDi[i][0];
		for(int j=0;j<5;j++){
			if(min>tokhuraDi[i][j]){
				min=tokhuraDi[i][j];
				min_ind=j;
			}
		}
		cout<<"Vowel Detected: "<<vowel_arr[min_ind]<<endl;
		if(ind==min_ind)
			count++;
	}
	return(count);
}
/*
	test() will call utility_train_test().
*/
void test(string vowel_test){
	

	raisedSineWindow();
	
	for(int test_no=11;test_no<=20;test_no++){
		stringstream ss;
		ss<<test_no;
		string s=ss.str();
		string file_name=vowel_test+"/204101057_"+vowel_test+"_"+(s)+(string)".txt"	;
		
		utility_train_test(file_name,test_no-11);
		//now clearing all the vectors.
		arr.clear();
		ai_coefficient.clear();
		cepstral.clear();
		cepstralRaised.clear();
		finSig.clear();
	
    
}
	//tokhuraDist() which will helps in calculating the tokhura distance.
	tokhuraDist();
	
}


int _tmain(int argc, _TCHAR* argv[])
{	
	int choice;
	cout<<"\n************  Welcome to the Vowel Recognition program.  ************"<<endl;
	cout<<" Press :\n 1 for Training on data.\n 2 for Testing on pre-recorded vowels.\n 3 for Testing using recording module. "<<endl;
	cout<<"\n************************************************************************\n"<<endl;
	string vowel_arr[5]={"a","e","i","o","u"};
	while(1){
		cout<<"\nEnter your choice :";
	cin>>choice;
		switch(choice){
		case(1):{
			// calling train() for each vowel.
				for(int i=0;i<5;i++){
					string vowel;
					vowel=vowel_arr[i];
					train(vowel);
					cout<<endl;
					cout<<vowel<<"'s Training Completed..."<<endl;
				}
				break;
			}
		case(2):{
			// calling the test function for each vowel.
				int total_correct=0;
				for(int i=0;i<5;i++){
					string vowel;
					cout<<"\t\tCorrect vowel: "<<vowel_arr[i]<<endl;
					vowel=vowel_arr[i];
					//cout<<vowel_arr[i]<<endl;
					test(vowel);
					int correctly_det=detectVowel(i,10);
					total_correct+=correctly_det;
					double accuracy=(correctly_det*100)/10.0;
					cout<<"vowel '"<<vowel<<"' detected with accuracy of :"<<accuracy<<" %"<<endl;
					cout<<endl;
					//cout<<endl;
					
				}
				cout<<"Overall accuracy on Testing sample is : "<<(double)(total_correct*100)/50.0<<" %"<<endl;
				return 0;
			break;
			}
		
		case(3):
			{	/*
					while testing using the live recording module,we will have only one test file,But my cepstral2d array and tokhuraDi-vector is for
					10 files, thus in my case in cepstral array we will only focus on top-5 rows and all the remaining rows will have value zero.
					Similarly while vowel detection only scan the 0th row of tokhuraDi-vector as we only have one file and each row of tokhuraDi vector
					corresponds to a file.
				*/
				
				
				raisedSineWindow();
				cout<<"\nOnce recording Stops press any key for detecting vowel.\n"<<endl;
				system("Recording_Module.exe 4 input_file.wav 204101057_live_recording.txt");
				ifstream readFile;
				string fileName="204101057_live_recording.txt";
				//calling utility_train_test() with fileName and "0" as argument,here "0" will help in focusing row-0 of cepstarl2d and other arrays or vectors.

				utility_train_test(fileName,0);
				tokhuraDist();
				//in detectVowel() we are sending 2nd argument as 1 because we only have 1 file to test,thus will scan only one row of tokhuraDi-2dvector.
				//1st argument and return value is not of our use in this case ,so it can be any interger.

				int val=detectVowel(0,1);
				cout<<endl;
				
				return 0;
				break;
				
			
			}
		default:
			{
				cout<<"Enter correct option."<<endl;
			}
		}
	}
	return 0;
}