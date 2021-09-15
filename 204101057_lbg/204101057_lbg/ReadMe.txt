
LBM Algorithm implementaion with codebook size of k=8.
Step1:
	Initially the codebook is initialized by a single vector,which is centroid of the whole univese vector.
	for finding this single vector.I have sum-up the all the cepstal values of all the vectors available
	in the universe and then divide that with the total vectors present in the universe.
Step2:
	Now,the obtained codebook is splitted i.e. each vector of the universe set is splitted into two sets.
	thus our codebook size gets doubled each time.
step3:
	now, the k-mean algorithm is called on this codebook which will help us in getting a optimised codebook.

These,two steps are repeatedly called till we reach a codebook of size k=8;
	
	Functionality of split_codebook():
		split_codebook() will split the currently available codebook and double the size of the current codebook.
		for splitting the codebook epsilon=0.3 is used and the vector is splitted using the factor of (1+epsilon)
		and factor of (1-epsilon).