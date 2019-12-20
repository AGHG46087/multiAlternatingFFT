# multiAlternatingFFT
VU rendering is rather simple in nature.  It provides a sampleing of a Fast Fourier Transform FFT.  and renders of serveral samples.  

The code in this will render either 6 or 10 columns which are alternated by a Thread.
Additionally, the Columns alternate direction from up to down also using a thread. 

Lastly the columns frequency secion is rotated so for example if a 6 column render the iteration will sequence column 6 in column 5 position and column 1 in column 6 position, which then becomes 5 is 4, 4 is 3, and 3 is 2 finally 2 is 1.  The same applies for when there is a 10 column render

## How to get this going

Download the project, compile the the code with the arduino compiler and deploy to your favorite arduino.

Yolu will need to accomodate the LED strips and load the appropriate libraries.  But I suspect you know that one already.

