// 204101057_K_mean.cpp : Defines the entry point for the console application.
//

/*
	Assignment 4(a): k-mean implementations , having a codebook of size k=8.
*/

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
	randomly assigned 8 vectors to the codebook from the universe using the random number generator fns to get randomly 8-indices
	using which centroid's vector is initialised.
	
*/
void codebook_init(int k){
	srand(time(0));
	for(int i=0;i<k;i++){
		int rand_ind=rand()%1000;
		//cout<<rand_ind<<endl;
		for(int j=0;j<12;j++)
			codebook[i].cepstral[j]=universe[rand_ind].cepstral[j];
		codebook[i].cluster_no=i;
	}
}
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

	long double *codebook_dist=new long double[k];//will store the distortion of a vector from all the 8-regions.
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
	centroid_init();//set the centroid value to it's initial situation.
}


void k_meanAlgo(){
	
	int k=8; //k is the codebook size.
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
			overall_distortion+=temp_distortion;//summing up all the distortion value retuned.
		}

		
		previous_distortion=average_distortion;
		
		average_distortion=overall_distortion/univSize;
		
		//now, updating the codebook vector's with the help of centroid vector's
		codebook_updation(k);	
		cout<<"Iteration no: "<<iteration;
		cout<<"  distortion: "<<average_distortion<<endl;
		iteration++;
		}while((previous_distortion-average_distortion)>threshold_delta );
	    
	cout<<"\nFinal distortion "<<average_distortion<<endl;
	
	
}

void write_to_file(){
	//dumping final codebook values to file and also printing on screen.
	FILE* writeFile;
	writeFile=fopen("Final_codebook.txt","w+");
	cout<<endl;
	cout<<"Final CodeBook Vectors are:\n "<<endl;
	fprintf(writeFile," %s","Final codebook using K-Mean Algo:\n");
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
/*
	read_universeFile() helps in reading the csv universe file and store that in universe array of data-typed struct vect.
	All the vectors were stored in this and the cluster_no was initialised to "-1" to all the vectors.
	
*/
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
	int k=8;
	cout<<"\n\t\t--------------------------------------------------\n\n";
	cout<<"\t\t\t\tWelcome to K-Mean ALGO :\n";
	cout<<"\t\t--------------------------------------------------\n\n";
	read_universeFile();
	codebook_init(k);
	centroid_init();
	k_meanAlgo();
	write_to_file();
	cout<<"\n ********************* Terminating the Program ***************************\n"<<endl;
	return 0;

}

