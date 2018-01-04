# mapiX

mapiX is a software dedicated to taking reconstructed data (3d space points) 
and converting the data to 2D histograms for pixel readout.

The .json configuration file in config lists the mapping for the ROI and pixels to TPC channel numbers. The ROI and pixels coordinates are written in the following coordinate system: inside the TPC looking at the pixel PCB, the origin is located at the top left-hand corner pixel, coordinates are in units of pixels (Z = 8 means 8 pixels right), the locations of ROIs are determined by the ROI's associated top left hand corner pixel.

This software does the conversion from this pixel coordinate system to the TPC coordinate system. 

## Setting environment and build mapiX

In the working directory, type the following command to set the environment and build.

```
. setup.sh

mkdir build
cd build
cmake ../
make
```
## Running mapiX

Create a directory called data to store the ouput.

```
mkdir output
```

mapiX takes in a text file containing the 3D space point data. The data should be 
formatted in 3 columns beginning with Run, Subrun, and Event, followed by the space points like the following:

```
Run 6258
Subrun 189
Event 9319
X1-coordinate Y1-coordinate Z1-coordinate
X2-coordinate Y2-coordinate Z2-coordinate
.
.
.
etc.
```

mapiX also needs input parameters to do the mapping. Create a rapidjson file 
that gives pertinent information.

Type the following command to run the program.

```
./mapiX [path/to/TrackData.txt] [path/to/RapidJsonFile]
```

