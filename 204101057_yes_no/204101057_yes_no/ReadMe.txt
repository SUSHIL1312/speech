Assignment No:02
Roll No: 204101057

Objective: Detecting Yes/No using recording module.

###  Firstly Build program ---->Start without debugging in visual studion 2010.

### After running the program a terminal window will come.Then start speaking yes/no for 
	recording the data.This data will gets stored in  yesNoIn.txt file which is further used for detection.

	
### This program is working correctly on my own voice ### 

-->For this I have taken two vector named 'ste'(for storing the short term energy) and 
	'arr'(for storing the input from the.txt file).
-->Firstly Read all the input values from the .txt file and push all the values in 
	vector 'arr' and calculated STE value of Blocks(each of size 320) and strored in vector 'ste'.
-->I have scan the 'arr' using a for loop and keep once I read 320 values,looked the 'ste' vector and read the value .
-->After observing the Background/Silene Signal noted that all the ste value in each block (of 320 unit) for Background remains <100000.
	Thus considering ste values >10000 for our evaluation purpose and ignoring the rest values.
	(STE value is helping in distinguishing background sound singnal with speech signal.)
-->for deciding start and end marker:
	taken three variable start,end and flag all initialised to 0.
	when the ste value>10000 and flag=0(i.e first time seeing ste value>10000) putted a marker here i.e from here onward 
	some speech process has begun.Keep reading the ste values and once it becomes <100000 put a 'end' marker here i.e consider
	 till here region for our purpose.
-->After getting our start and end marker, i.e we got the region for evaluation.Now take this region and for deciding yes/no focus only on last
	15% of the region  and calcuate the zcr values. In this region ZCR value of 'yes' will be very high as it contains "s" which is a fricative &
	the ZCR value of "NO" will be less  as it is containing a vowel 'o' at the end.
	(I have taken zcr value >120 for  yes and  lesser zcr value for 'no'(15<zcr<120) after observing few examples.)

