//Assignment4(b):LBG algorithm implementation.

#include "stdafx.h"
#include<vector>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<float.h>
#include<stdio.h>
#include<sstream>
#include<string.h>
#include<iomanip>
#include<time.h>
#define univSize 6340
#define threshold_delta 0.0001
using namespace std;
/*
	defining a data-type vect using structure,which has 2-members i.e a array of type long double which stores
	the ci's values for a vector, and a cluster_no which stores in which cluster or region that vector belongs to.
*/
struct vect{
	long double cepstral[12];
	int cluster_no;
};


struct vect codebook[8];
struct vect centroid[8];
struct vect universe[univSize];

/*
	centroid_init() basically set all the ci's value of the centroid's vector to zero and cluster_no to zero.
	In case of centroid cluster_no stores the number of universe vector's belonging to that centroid's region.
*/
void centroid_init(){
	int i, j;
	int k=8;
	for(i=0;i<k;i++)
	{
		centroid[i].cluster_no=0;//cluster field stores number of elements in the cluster as index already indicates the cluster number
		for(j=0;j<12;j++)
			centroid[i].cepstral[j]=0;
	}
}
/*
	tokhura_distance() basically finds the distortion of a vector from a referende vector.
*/
long double tokhura_distance(int code_book_index,int vect_ind){
	double w[]={1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
	long double diff;
	//long double fin_dis=0;
	long double tem_dis=0;
	for(int i=0;i<12;i++){
		diff=(universe[vect_ind].cepstral[i]-codebook[code_book_index].cepstral[i]);

		tem_dis+=w[i]*diff*diff;

	}

	return tem_dis;
}
//updating the centroid's ci's and cluster_no for a vector i.e in which region that vector belongs to.
//In case of centorid vetor's cluster_no field will store how many vectors of the universe belongs to that centroid.
void centroid_cluster_form(int vec_ind){
	int reg_no=universe[vec_ind].cluster_no;
	centroid[reg_no].cluster_no++;//incrementing the vector no belonging to he region
	int i;
	for(i=0;i<12;i++){
		centroid[reg_no].cepstral[i]+= universe[vec_ind].cepstral[i];
	}
}

//vect_classification() helps in deciding which vector belongs to which region and also helps in setting the centroids value and forming clusters.
//and finally return the distortion of a vector from its' reference vecotor in the codebook i.e from the region in which that particular train vector
//of universe belongs to in the codebook.This funcition calls tokhura_distance() which return distance of a vector from any vector of a region(i.e codebook)
//and saving those value in a array named codebook_dist[] whose indices corresponds to a region i.e if index=0 then it will we storing the distance of that
//vector form region 0 and so on. At last we will have distance of a particular vector of universe form all the 8 vectors of the codebook stored in the 
//codebook_dist[]. Now scan that array and look for the min distance stored in it and note the min_ind.Now assign that universe's vector cluster_no to that 
//min_ind and also return the distortion corresponding to that index(which will helps in computing overall_distortion.

long double vect_classification(int vect_ind,int k){
	long double *codebook_dist=new long double[k];
	long double distance=0.0;
	for(int codebook_i=0;codebook_i<k;codebook_i++){
			//storing the distortion of a vector present in universe at row no. vect_ind from all the 8-regions 
		//and storing the distortion in code_book[] with the help of tokhura_distance().
		codebook_dist[codebook_i]=tokhura_distance(codebook_i,vect_ind);
	}
	//Now, following part of code, will scan the codebook_dist[] and look at which index the distortion is minimum then update that index
	//to the cluster_no field of the vector present at row... vect_ind in the universe.
	long double min=codebook_dist[0];
	int min_ind=0;
	for(int i=0;i<k;i++){
		if(min>codebook_dist[i]){
			min=codebook_dist[i];
			min_ind=i;
		}
	}
	//updating the cluster of each vector 
	universe[vect_ind].cluster_no=min_ind;
	centroid_cluster_form(vect_ind);
	return codebook_dist[min_ind];//returning the minimum distortion of the vector from the regions.
}
/*
	In this function basically, I have updated the centroid vector's value to the codebook's vector,while updating value I have divided each 
	ci's value with univSize because in centroid I had stored the ci's value without dividing by univSize(i.e summation of all ci's only).
	and atlast I have set the centroid's to it's initial values.
*/
void codebook_updation(int k){
	for(int region_no=0;region_no<k;region_no++){
		//empty cell problem
		if(centroid[region_no].cluster_no==0){
			cout<<"Empty cell at codebook entry"<<endl;
		}
		for(int ci_ind=0;ci_ind<12;ci_ind++){

			codebook[region_no].cepstral[ci_ind]=(centroid[region_no].cepstral[ci_ind])/(centroid[region_no].cluster_no);
		}
	}
	centroid_init();//reset the centroid value.
}
void k_meanAlgo(int k){
	
	long double average_distortion=DBL_MAX,previous_distortion=DBL_MAX;
	int iteration=1;

	do{
		//overall_distortion keeps the sum of all the distortion value returned by a vector.
		long double overall_distortion=0.0;
		
		/*
		  assigning the nearest neighbour to all the vector out of 8 region and at the same time getting the distortion of the vector 
		  from the region it belongs to(i.e minimum distortion of that vector from any region) and centroid's ci's value of that region is also
		  updated.These all functionality is performed using vect_classification() .
		*/
		for(int vec_no=0;vec_no<univSize;vec_no ++){
			
			long double temp_distortion=vect_classification(vec_no,k);
			overall_distortion+=temp_distortion; //summing up all the distortion value retuned.
		}

		previous_distortion=average_distortion;
		
		average_distortion=overall_distortion/univSize;
		//now, updating the codebook vector's with the help of centroid vector's
		codebook_updation(k);	
		
		iteration++;
		}while((previous_distortion-average_distortion)>threshold_delta );
	cout<<" K-Mean runs : "<<iteration<<" interations.";
	cout<<" Final distortion :"<<average_distortion<<endl;

	
}
/*
	split_codebook() will split the currently available codebook and double the size of the current codebook.
	for splitting the codebook epsilon=0.3 is used and the vector is splitted using the factor of (1+epsilon)
	and factor of (1-epsilon).
*/

void split_codebook(int row){
	double epsilon=0.3;

	struct vect *temp_codebook=new struct vect[2*row];
	for(int codebook_row=0;codebook_row<row;codebook_row++){
		for(int ci_ind=0;ci_ind<12;ci_ind++){
			long double curr_value=codebook[codebook_row].cepstral[ci_ind];
			/*
				now, spiltting the vector and storint that into temp_codebook,which will be updated later to the original codebook.
			*/
			temp_codebook[2*codebook_row].cepstral[ci_ind] = curr_value * (1 + epsilon);
			temp_codebook[2*codebook_row+1].cepstral[ci_ind] = curr_value * (1 - epsilon);
		}
	}
	/*
		updating the original codebook value with the help of temp_codebook.
	*/
	for(int codebook_row=0;codebook_row<2*row;codebook_row++){
		for(int ci_ind=0;ci_ind<12;ci_ind++){
			codebook[codebook_row].cepstral[ci_ind]=temp_codebook[codebook_row].cepstral[ci_ind];
		}
	}
	
}
//codebook_init() will initialize the codebook with a single vector which is the centorid of the entire universe.
//for getting the centroid vector of the entire universe simply sum-up the ci's off all the vectors present in the
//universe and divide that with size of the universe i.e univSize.
void codebook_init(){

	
	long double cepstral_sum;
	for(int i = 0; i < 12; i++)
	{
		cepstral_sum = 0;

		for(int j = 0; j < univSize; j++)
			cepstral_sum += universe[j].cepstral[i]; //summing up all the cepstral values of the vectors

		codebook[0].cepstral[i] = cepstral_sum / univSize;

	}
}
void write_to_file(){
	//dumping final codebook values to file and also printing on screen.
	FILE* writeFile;
	writeFile=fopen("Final_codebook.txt","w+");
	cout<<endl;
	cout<<"Final CodeBook Vectors are:\n "<<endl;
	fprintf(writeFile," %s","Final codebook using LBG Algo:\n");
	for(int region_no=0;region_no<8;region_no++){
		for(int ci_ind=0;ci_ind<12;ci_ind++){
			cout<<codebook[region_no].cepstral[ci_ind]<<" ";
			fprintf(writeFile,"%lf  ",codebook[region_no].cepstral[ci_ind]);
		}
		fprintf(writeFile,"\n");
		cout<<endl;
	}

	fclose(writeFile);
}
void LBG_algo(){
	int k_current=1;
	//will initialize the centroid value.
	centroid_init();
	// will initialize the codebook by only one vector,i.e by the single centroid of all the universe vector.
	codebook_init();
	int iter=1;
	//while loop will run till codebook size becomes equal to k=8 and in each iteratin k-mean algorithm will be called
	while(k_current<8){
		cout<<iter++<<"-> ";
		//will splitting each codebook vector into 2 vector.
		split_codebook(k_current);
		k_meanAlgo(2*k_current);
		k_current*=2;
	}
	write_to_file();
}
//will read the cse file present in the universe.csv and store tha in universe[] array of type struct vect and also set the cluster_no of each vector
// to -1(i.e currently they donot belongs to any region).
void read_universeFile(){
	ifstream readFile;
	readFile.open("Universe.csv",ios::in);
	if (!readFile) {
        cout << "Error in opening file";
        exit(1); // terminate with error
    }

	string line;
	int ceps_ind;
	int vect_ind=0;
	while(getline(readFile,line))
	{
		ceps_ind=0;
		stringstream   linestream(line);
		string  value;

		while(getline(linestream,value,','))
		{
			double valuei;
			valuei=stod(value);						//stod (string to double conversion function)
			universe[vect_ind].cepstral[ceps_ind] = stod(value);	//insert read cepstral coefficient
			ceps_ind++;
			
		}
		
		universe[vect_ind].cluster_no = -1;	// means cluster not alloted yet
		vect_ind++;
	}

}
int _tmain(int argc, _TCHAR* argv[])
{	
	
	
	read_universeFile();

	centroid_init();

	cout<<"\n\t\t--------------------------------------------------\n\n";
	cout<<"\t\t\t\tWelcome to LBG ALGO :\n";
	cout<<"\t\t--------------------------------------------------\n\n";
	LBG_algo();
	cout<<"\n************************ Terminating Program ***************************\n"<<endl;
	return 0;

}

