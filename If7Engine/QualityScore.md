## INDIface7 TechNote #3259
# Quality Score Calculation
First Draft in progress, March 1, 2025A

The Frontal Face Object Detection Quality Score 
_(FFODQS for short)_is a key part of EclipseIR's
Intellectual Property. 
This allowed users to specify the likelyhood of *faceness*
they wanted to see rather than the core technology
operating like an on-or-off light switch.

In the legacy implementation, the raw potential face
detection rectangles were grouped using one of three
propietary grouping algorithms.
The result of those calculations was a list of
high likelyhood faces and a quality score from
1 to 999 for each one.

In the new **INDI***face* implementation, 
the grouping function has been
incorporated into `OpenCV v4.x`'s detection capability.
This would take us back to lightswitch functionality.

## Proposed Calculation

I am looking at a two tier approach to the new
quality score calculation. 
See below for details on the raw detection parameters.

### Gross Category

At the default `Factor` of 1.10 I surmise that a grouping
of 1 to 9 neighbors would correspond to Quality 100 to 900
with ten or more maxed out at 999. 
This is where I am going to start at y'all's evaluation 
using **v7.90C**.

If this proves reasonable, the next iteration would 
adjust quality calculation for other `Factor` values.
For example, 1.05 (denser) would yield Quality 50 through 950 
for 1 to 19 neighbors and max out at 20. 
Or, at 1.20 (undenser),  200-400-600-800 
for 1-2-3-4 neighbors maxed out at 5. 
This will be evaluated in **v7.90D**.

### Score Adjustment

Rather than a on/off switch, we have a rotary switch with
four to nineteen positions.
To break ties between detections with the same number of
neighbors, 
I propose adjusting between 700 and 800, for example,
by a smaller score that gives more weight 750-795 for example
detection sizes near the 'sweet spot' and lesser scores
like 710-745 for tiny or huge heads 
with the same number of neighbors.

## Parameters

### Raw Arguments

These are the parameters that feed directly into the
detection algorithm:

* Factor

The scale factor controls the density of the rectangles 
that are overlayed on the input frame image.
For 32-pixel core cascades and factor of 1.10, 
32 * 1.000, 
32 * 1.100,
32 * 1.121,
32 * 1.331,
32 * 1.464, &c are scanned, limited by the
minimum and maximum detection sizes.
For factor 1.20 it would be 
32 * 1.000,
32 * 1.200,
32 * 1.440,
32 * 1.728,
32 * 2.074, &c. 
Note that this is considerably faster, 
but with the possible loss of accuracy.
Again, a science project, to quote Mr. Sutter, for a later date.

* MinNeighbors

Initially we will start at 1, so that we see all the results.
In the future this may be used in conjunction with MinQuality
to see if it provides any speed improvements versus
just throwing away results that don't pass muster.
Another science project.

* MinSize
* MaxSize

These would be calculated as in the legacy engine from the
inverse of MaxAcross and MinAcross applied to the size of
the input image. 
Note that this can be used to improve both the speed
and accuracy.
Further explaination is outside the scope of my memory;
I did it once 20 years ago, but not sure I have the mental
faculties to do so today.

* Flags

The algorithm adjustment option flags are rarely used. 
For now we won't use any and can experiment with them at
a much later date.


