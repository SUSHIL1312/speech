//Roll No: 204101057

Task: Vowel Detection 

For this firstly I have recorded 20 samples for each vowels using cool-edit software and
 clipped all those and saved as text file using proper naming convention.

Storage used:[vectors and arrays]

	arr-vector will store the input values from the txt file after subtracting teh dcShift values.

	finSig-vector will store only (320*5) values i.e only 5-frames which are our 5 steady frames
		which will be used for the furher processing.

	sinWind-vector will store the raise sin coefficients values,which will be used while raising the cepstral
		coefficient.

	ai_coefficient will stor the ai's value of 1 file(i.e 5 frame).

	cepstral-vector will store the cepstral values of one complete file (i.e 5 frame).

	cepstralRaised-vector will  sote the raised cepstral values of one completer file(i.e 5-frames).

	cepstral2d: It is a 2d vector of size (50*12).It will contain all the raised cepstral values of 
		one completer file ,each set of 5-row from top will be one file,so total it have 50 rows
		so,will store values for 10-files.

	dumpedData-2d vector,of dimenson (25*12) in which I have loaded all the dumped reference value of 
		each vowel.Top 5 row is for vowel-a , next 5 row is for vowel-e and so on..

	tokhruDi-2d vector will be of (10*5)dimension in which each row will store the distance of 
		each testing file and each column value of a row will corresponds to distance of that 
		file with vowels i.e at index=0 distance will be from vowel-a ,at index=1 distance will be for
		vowel-e ,index-2 will be for vowel-i ,index-3 will be of vowel-o ,index-4 will be of vowel-u.

	Procedure:
		->firstly performed the DC-shift while loading each file.
		->After Dc-shift I have performed Normalization, then selected the 5 steady frame and stored those 
			frames in finSig vector.
		->Then find the ri's,ai's and ci's of each frame and stored in vectors.
		->ai's and ci's are stored in ai_coefficient and cepstral vector respectively,in both these vector
			I have pused the ai's and ci's of complete one file (i.e 5-frame) using push_back().
		->Then performed raised ci's which is stored in cepstralRaised vector,and later stored in cepstral2d
			vector.
		->now, the cepstral2d vector will contains the raised cepstral values of all 10-files of one vowel.
			then I find the average cepstal value by adding frame-"i" of each file where 1=<i<=5.
			and we will get average cepstral values of all frames,then dump those values in a text file 
			during the training process which is used while testing any vowel.

	     	->Now while testing we will also compute till the cepstal2d (i.e finding the raised cepstral value)similar
			as the training step.After that i have loaded the dumped reference file in dumpedData-vector
			and used that during calculating the tokhura distance.
		->During Calculating the tokhura distance I have taken the dumpedData which has the refernce ci's of 
			all 5-vowel i.e top 5-rows is the avg ci's of vowel-a and next 5-row is for vowel-e and so on..
			while calculating the distance, I have computed the ci's of all the 10 testing file of any
			vowel in cepstal2d vector,and just used cepstral2d vector and dumpedData vector for calculating
			the tokhura Distance and stored the result in a another 2d-vector i.e tokhuraDi in which each 
			row corresponds for a test file and each column in a particular row correspods to the distance 
			of that test file with any vowel.
		->Detecting the Vowel:
			After all the value are filled in tokhuraDi  vector.I have simply scan each row and find the 
			index of min distance value in that row and print the corresponding vowel as detected vowel.

	----------------------------The End---------------------------------	
		
	
		
	
	
