Features :

1. 
	Triangle Primitives : Implemented the triangle functions, and rendered the 
	given images which contain triangle primitives

	- 	dunkit.png : We can see the triangular shapes on top of the 3 spheres
	- 	teapot.png : We can see the teapot rendered with two lights, one blue directional 
					 and a red point light

	<img src="/images/dunkit.png" width="45%">	<img src="/images/teapot.png" width="45%">



2.
	Refraction : Implemented basic refraction and rendered two scenes showcasing the same
	using spherical glass-like primitives

	- 	refr1.png : A glass sphere further away with refractive index 2.5
	- 	refr2.png : A glass sphere further away with refractive index 3

	<img src="/images/refr1.png" width="45%">	<img src="/images/refr2.png" width="45%">



3.
	Area Lights : Implemented area lights as a set of 27 (fixed) number of point lights
	aligned in a cubical 3x3 fashion. The area light is a new type (class) of light which has all
	the properties of point lights (which apply to each of it's substituent lights) and 
	also another offset parameter with default value 0.15, which indicates how far apart 
	are the point lights from one another.

	The position given to the area light is taken to be the center point lights position,
	and shadow lights from each of the other lights is calculated to a point. We see how
	many lights are obstructed and hence, calculaate the ratio that is multiplied to the 
	specular and ambient parts.

	- 	area_0.00.png : Rendering with a single point light
	- 	area_0.15.png : Rendering with offset = 0.15 (soft shadows)
	- 	area_0.25.png : Rendering with offset = 0.25 (softer shadows)

	<img src="/images/area_0.00.png" width="32%">	<img src="/images/area_0.15.png" width="32%"> <img src="/images/area_0.25.png" width="32%">



4.
	Smooth normals for Triangle meshes : I create a map from each vertex to the triangles
	that vertex is a part of. Then calculate the average of these normals and store in the
	respective triangle primitives. Whenever I want to calculate the normal at a praticular
	position on a triangle, the result is a weighted average of the (averaged) normals at 
	it's vertices. The weights are propotional to the subtriangles

	- 	dunkit_smooth.png : Smoothed version of the old ring like items
	- 	teapot_smooth.png : Smoothed version of the old teapot (looks really nice!)

	<img src="/images/dunkit.png" width="45%">	<img src="/images/dunkit_smooth.png" width="45%">
	<img src="/images/teapot.png" width="45%">	<img src="/images/teapot_smooth.png" width="45%">

Commands :
```
1.	

	../trace dunkit.scd ../images/dunkit.png 2 				(40 s)
	../trace teapot.scd ../images/teapot.png 2				(4m 50s)

2.
	../trace refr1.scd ../images/refr1.png 2				(5 s)
	../trace refr2.scd ../images/refr2.png 2				(5 s)

3.
	../trace area_0.00.scd ../images/area_0.00.png 1		(2 s)
	../trace area_0.15.scd ../images/area_0.15.png 1		(20 s)
	../trace area_0.25.scd ../images/area_0.25.png 1		(20 s)

4.
	../trace dunkit.scd ../images/dunkit_smooth.png 1 1		(40 s)
	../trace teapot.scd ../images/teapot_smooth.png 1 1 	(5m 20s)
```