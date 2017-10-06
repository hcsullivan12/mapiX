# mapiX

mapiX is a software dedicated to taking reconstructed data (3d space points) 
and converting the data to 2D histograms for pixel readout.

## Setting environment and build mapiX

In the working directory, type the following command to set the environment and build.

```
. setup.sh

mkdir build
cd build
cmake ../
make
```
##Running mapiX

mapiX takes in a text file containing the 3D space point data. The data should be 
formatted in 3 columns beginning with Run, Subrun, and Event, following by the space points like the following:

```
Run, Subrun, Event
6258 , 189 , 9319

X1-coordinate , Y1-coordinate , Z1-coordinate
X2-coordinate , Y2-coordinate , Z2-coordinate
.
.
.
etc.
```
Type the following command to run the program.

```
./mapiX [path/to/TrackData.txt]
```

