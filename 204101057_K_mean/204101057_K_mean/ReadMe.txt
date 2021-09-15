			Assignment NO:4 (k-mean algorithm implementation)

K-mean implementation for getting the codebook which is having total k=8 vectors.
Each vectors of the codebook represent a region,and each vector of the universe set belongs to one 
of the regions of the codebook.
firstly a user-defined dataType i.e struct vect is created which have 2 member field.
a) array cepstral of size 12 which is of long double type and contains cepstral values.
b) cluster_no variable of type integer,which will store in which a vector is belonging to.

Now, i have created three arrays of type struct vect :
	a) codebook[8] : this will store the 8 vectors and each vector here represents a region.
	b)centroid[8] : this also contains 8 vectors,will be used while updating the codebook.Clusters 
			will be formed.In this case cepstral_no will be storing the number of vectors
			from the universe belonging to a region.
	c)universe[univSize]: This will store all the vectors present in the training set.


	Firstly, uiverse.csv file is readed using the read_universeFile() and all the vectors is stored in the
		universe[].

	Codebook is now initialized using codebook_init(k) function, here we randomly initialize codebook 
		by 8 vectors from the universe.
	Now,k_meanAlgo() is called from the main():
		Inside the k_meanAlgo():
		for each vector in the universe,vect_classification() will be called and this will return a distortion
			value of that vector from the vector of the codebook in whose region that universe vector is 
			belonging to.
		now, the returned distortion values of every vector is added up and we will obtain the overall_distortion
			which is then divided by the univSize to obain the average_distortion.
			
		Functionality performed by vect_classification():
			vect_classification() helps in deciding which vector belongs to which region and also helps in setting the 
			centroids value and forming clusters and finally return the distortion of a vector from its' reference vecotor 
			in the codebook i.e from the region in which that particular train vector of universe belongs to in the codebook.
			This funcition calls tokhura_distance() which return distance of a vector from any vector of a region(i.e codebook)
			and saving those value in a array named codebook_dist[] whose indices corresponds to a region i.e if index=0 then it 
			will we storing the distance of that vector form region 0 and so on. At last we will have distance of a particular vector
			 of universe form all the 8 vectors of the codebook stored in the codebook_dist[]. Now scan that array and look for the min
 			distance stored in it and note the min_ind.Now assign that universe's vector cluster_no to that min_ind and also return the
			distortion corresponding to that index(which will helps in computing overall_distortion.

		now, codebook_updation() is called:
			In this function basically, I have updated the centroid vector's value to the codebook's vector,while updating value 
			I have divided eachci's value with univSize because in centroid I had stored the ci's value without dividing by 
			univSize(i.e summation of all ci's only) and atlast I have set the centroid's to it's initial values.

		now, these are called till  (previous_distortion-avg_distortion)>0.0001.
	Finally, we get the final codebook which is then stored in a file.

	 