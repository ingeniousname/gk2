A C++ SDL rasterizer project

The surface of the screen is divided into congruent triangles and filled with the scan-line algorithm using the AET (Active Edge Table) data structure. The canvas is filled with a texture loaded from disk, the texture is repeated if too small. Lighting is calculated in each pixel based on the Phong model.

Additionally, there is an option to load a normal map that will be applied over the whole canvas. To visualize the lighting effects a simple animation of a light source over the canvas is played.

Example of using a brick wall normal map over the texture of the map itself:

![](media/gk2.gif)