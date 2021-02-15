=============================================================================
NDLZ: codec use and benchmarks
=============================================================================

NDLZ is a multidimensional codec that uses Caterva to divide a
superchunk in cells in order to make the compression task easier for
linear compressors.

This codec has been implemented for 4x4 and 8x8 cells in ndlz.c and
ndlz8.c files respectively. Furthermore, blosc2.c has been modified to
support not only NDLZ, but also the combination of NDLZ with
the rest of codecs.

In order to test the efficiency of NDLZ it was created test_ndlz.c.
This benchmark computes the compression ratios and times of different
images depending on what codec is used.

This way, graphs can be produced using the images results. This graphs
are really useful to obtain clear conclusions about the evaluated codec
quality.

.. image:: /images/img7.png
  :width: 90%
  :align: center

The following graph analyzes the previous image compression using
different codecs:

.. image:: /images/graph_image7.png
  :width: 90%
  :align: center

