# multiAlternatingFFT
VU rendering is a standard view meter in principle.  It provides a sampling of a Fast Fourier Transform `FFT`.  and renders of serveral samples.  

The code in this will render either 6 or 10 columns which are alternated by a Thread.
Additionally, the Columns alternate direction from up to down also using a thread. 

Lastly the columns frequency secion is rotated so for example if a 6 column render the iteration will move the column 6 to column 5's position and column 1 in column 6's position, which then becomes 5 is 4, 4 is 3, and 3 is 2 finally 2 is 1.  The same applies for when there is a 10 column render

## How to get this going

Download the project, compile the the code with the arduino compiler and deploy to your favorite arduino.

You will need to accomodate the LED strips that you are using and load the appropriate libraries outlined in the code.  But I suspect you know that one already.  Enjoy!

## Schematic Diagram
![Schematic Diagram](https://github.com/AGHG46087/multiAlternatingFFT/blob/master/vu_schematic.jpg "Schematic Diagram")


## Wiring Diagram
![Wiring Diagram](https://github.com/AGHG46087/multiAlternatingFFT/blob/master/vu_wiring.jpg "Wiring Diagram")


## Shield Diagram
![Shield Diagram](https://github.com/AGHG46087/multiAlternatingFFT/blob/master/vu_shield.jpg "Shield Diagram")


 

