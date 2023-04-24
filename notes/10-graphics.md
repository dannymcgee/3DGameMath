### 10.1.1 The Two Major Approaches to Rendering

Any rendering algorithm essentially performs two tasks:

- **Visible Surface Determination**<br>
	Find the visible surface corresponding to the pixel currently being rendered
- **Lighting**<br>
	Determine what color this pixel should be

#### Forward Rendering
```
// Clear buffers
frame_buffer.clear()
depth_buffer.clear()

// Outer loop iterates over primitives (usually triangles)
foreach geoprim in scene {
	// Rasterize the primitive
	foreach (x,y) in geoprim.px_projection {
		// Test the depth buffer for occlusion
		let depth = geoprim.depth(x,y)
		if depth < depth_buffer.read(x,y):
			continue

		// Update the depth and frame buffers
		let color = compute_color(x,y, geoprim)

		frame_buffer.write(x,y) = color
		depth_buffer.write(x,y) = depth
	}
}
```

#### Deferred Rendering
```
// Clear buffers
geo_buffer.clear()
depth_buffer.clear()
frame_buffer.clear()

// Rasterize all primitives into the G-buffer
foreach geoprim in scene {
	foreach (x,y) in geoprim.px_projection {
		// Test the depth buffer for occlusion
		let depth = geoprim.depth(x,y)
		if depth < depth_buffer.read(x,y):
			continue

		// Fetch information needed for shading in the next pass
		// (e.g. material data, position, normal)
		let geo_data = geoprim.data(x,y)

		// Write the geo data and depth to the buffers
		geo_buffer.write(x,y) = geo_data
		depth_buffer.writie(x,y) = depth
	}
}

// Perform actual shading in a 2nd pass
foreach (x,y) in px_projection {
	if depth_buffer.read(x,y) == infinity {
		// No geometry here
		frame_buffer.write(x,y) = background_color
		continue
	}

	// Fetch shading info
	let geo_data = geo_buffer.read(x,y)

	// Shade the point and write it to the frame buffer
	frame_buffer.write(x,y) = compute_color(geo_data)
}
```

#### Why two different strategies?

When multiple light sources illuminate the same surface point, it may be difficult or prohibitively expensive to compute the final color in a single computation. So instead, we perform some rendering pass separately for each light source, accumulating the final result into the frame buffer.

In forward rendering, this requires iterating over every triangle for every light that affects it. In deferred rendering, we iterate over every triangle at most once, and all subsequent rendering passes occur over some subset of the _screen space pixels_.

Broadly speaking, this means that forward rendering complexity scales with the number of lights &times; the number of triangles affected by that light, while deferred rendering scales with the number of lights &times; the number of pixels affected by that light.

In addition to those performance considerations, deferred rendering can also enable some quite sophisticated shading computations by allocating additional data buffers and performing more information-gathering operations in the pre-pass. The time complexity of these additional computations scales linearly, since they don't change the number of _iterations_ required to perform them &mdash; though they can quickly inflate the space complexity due to the additional data buffers required.

While classical forward pipelines have controlled performance by setting a hard limit on the number of lights that can affect the scene at any given time, many modern "Forward+" render pipelines split the difference between forward and deferred tradeoffs, e.g. by "clustering" the view frustum into chunks and limiting the number of lights that can affect each chunk.

### 10.1.2 Describing Surface Properties: The BRDF

Bidirectional Reflectance Distribution Function:

> &#119891; ( **x**, **&omega;**<sub>in</sub>, **&omega;**<sub>out</sub>, &lambda; ) where:
>
> **x** = a point<br>
> **&omega;**<sub>in</sub> = incoming (incident) light direction<br>
> **&omega;**<sub>out</sub> = outgoing (reflected) light direction<br>
> &lambda; = color (wavelength) of the light


The result of this function is a scalar that describes the relative likelihood that light incident at the point **x** from direction **&omega;**<sub>in</sub> will be reflected in the outgoing direction **&omega;**<sub>out</sub>.

Variations:
* **BSSDF**: _Bidirectional Surface Scattering Distribution Function_ &mdash; Incorporates translucence and light refraction by allowing the direction vectors to point back into the surface
* **BSDF**: _Bidirectional Scattering Distribution Function_ &mdash; Drops the "surface" to express volumetric effects such as fog and subsurface scattering at any point in space

Constraints:
* **Normalization** &mdash; 0 &leq; reflected light &leq; incident light
* **Helmholtz Recipricocity** &mdash; &#119891; ( **x**, **&omega;**<sub>1</sub>, **&omega;**<sub>2</sub>, &lambda; ) == &#119891; ( **x**, **&omega;**<sub>2</sub>, **&omega;**<sub>1</sub>, &lambda; )

### 10.1.3 A Very Brief Introduction to Colorimetry and Radiometry

#### Common Radiometric Terms

| Quantity             | Units             | SI unit   | Rough translation                                  |
|----------------------|-------------------|-----------|----------------------------------------------------|
| Radiant energy       | Energy            | J         | Total illumination duringan interval of time       |
| Radiant flux         | Power             | W         | Brightness of a finite area from all directions    |
| Radiant flux density | Power / unit area | W/m&sup2; | Brightness of a single point from all directions   |
| Irradiance           | Power / unit area | W/m&sup2; | Radiant flux density of incident light             |
| Radiant exitance     | Power / unit area | W/m&sup2; | Radiant flux density of emitted light              |
| Radiosity            | Power / unit area | W/m&sup2; | Radient flux density of emitted or reflected light |
| Radiance             | *                 | W/(m&sup2; &middot; sr) | Brightness of a ray                  |

\* Power per unit projected area, per unit solid angle

#### Units of measurement from radiometry and photometry

| Radiometric term | Photometric term   | SI Photometric unit                       |
|------------------|--------------------|-------------------------------------------|
| Radiant energy   | Luminous energy    | talbot, or lumen second ( lm &middot; s ) |
| Radiant flux     | Luminous flux*     | lumen ( lm )                              |
| Irradiance       | Illuminance        | lux ( lx = lm/m&sup2; )                   |
| Radiant exitance | Luminous emittance | lux ( lx = lm/m&sup2; )                   |
| Radiance         | Luminance          | lm/(m&sup2; &middot; sr)                  |

\* AKA Luminous power

#### Key Points

* Vague words such as "intensity" and "brightness" are best avoided when the more specific radiometric terms can be used.
* Use radiant flux to measure the total brightness of a finite area, in all directions.
* Use radiant flux density to measure the brightness at a single point, in all directions. Irradiance and radiant exitance refer to radiant flux density of light that is incident and emitted, respectively. Radiosity is the radiant flux density of light that is leaving a surface, whether the light was reflected or emitted.
* Due to Lambert's law, a given ray contributes more differential irradiance when it strikes a surface at a perpendicular angle compared to a glancing angle.
* Use radiance to measure the brightness of a ray. More specifically, radiance is the flux per unit projected angle, per solid angle. We use projected area so that the value for a given ray is a property of a ray alone and does not depend on the orientation of the surface used to measure the flux density.
* Practical realities thwart our best intentions of doing things "the right way" when it comes to using proper units. Numerical integration is a lot like taking a weighted average, which hides the change of units that really occurs. Point lights and other Dirac deltas add further confusion.

### 10.1.4 The Rendering Equation

> **_L_**<sub>out</sub>(**x**, **&omega;**<sub>out</sub>, &lambda;)
>    = **_L_**<sub>emis</sub>(**x**, **&omega;**<sub>out</sub>, &lambda;) +
>    &int;<sub>&Omega;</sub> **_L_**<sub>in</sub>(**x**, **&omega;**<sub>in</sub>, &lambda;)
>    &#119891;(**x**, **&omega;**<sub>in</sub>, **&omega;**<sub>out</sub>, &lambda;)
>    (-**&omega;**<sub>in</sub> &middot; **n**) _d_ **&omega;**<sub>in</sub>

The whole equation governs a balance of radiance at a single surface point **x** for a single wavelength (color channel) &lambda;.

#### Terms
* **_L_**<sub>out</sub>(**x**, **&omega;**<sub>out</sub>, &lambda;) &mdash; "The radiance leaving the point in the direction **&omega;**<sub>out</sub>"
	- Substituting **x** for the surface at a pixel and **&omega;**<sub>out</sub> for the direction from **x** to the camera is how we get our pixel color
* **_L_**<sub>emis</sub>(**x**, **&omega;**<sub>out</sub>, &lambda;) &mdash; "the radiance _emitted_ from **x** in the direction **&omega;**<sub>out</sub>."
	- This is zero unless the surface is emissive.
* &int;<sub>&Omega;</sub> ... _d_ **&omega;**<sub>in</sub> &mdash; "the light _reflected_ from **x** in the direction of **&omega;**<sub>out</sub>."
	- &int;<sub>&Omega;</sub> is intended to mean "sum over the hemisphere of possible incoming directions." In practice, of course, we must sample a discrete number of directions, turning the &int; into a &sum;.
* **_L_**<sub>in</sub>(**x**, **&omega;**<sub>in</sub>, &lambda;) &mdash; The radiance incident from the direction **&omega;**<sub>in</sub>
* &#119891;(**x**, **&omega;**<sub>in</sub>, **&omega;**<sub>out</sub>, &lambda;) &mdash; The BRDF
* -**&omega;**<sub>in</sub> &middot; **n** &mdash; The Lambert factor
	- More light can be reflected when **&omega;**<sub>in</sub> is more perpendicular to the surface
	- The dot product peaks at 1 in the perpendicular direction and trails off to zero as the angle of incidence becomes more glancing

### [U of U | Intro to Graphics](https://www.youtube.com/playlist?list=PLplnkTzzqsZTfYh4UbhLGpI5kGd5oW_Hh)

#### Gamma Correction

> **c**<sub>out</sub> = **c**<sub>in</sub><sup>&gamma;</sup> = **c**<sub>desired</sub><sup>1/&gamma;</sup><br>
> sRGB: &gamma; &approx; 2.2

#### Color Blending

##### Alpha (Standard)

> **c** = &alpha;<sub>f</sub> **c**<sub>f</sub> + ( 1 - &alpha;<sub>f</sub> ) **c**<sub>b</sub><br>
> &nbsp;&nbsp; = _lerp_( **c**<sub>f</sub> , **c**<sub>b</sub> , &alpha;<sub>f</sub> )
>
> where<br>
> _<sub>f</sub> = foreground<br>
> _<sub>b</sub> = background

if &alpha;<sub>b</sub> &ne; 1:<br>
> **c** = ( &alpha;<sub>f</sub> **c**<sub>f</sub> + ( 1 - &alpha;<sub>f</sub> ) &alpha;<sub>b</sub> **c**<sub>b</sub> ) / &alpha;<br>
> &alpha; = &alpha;<sub>f</sub> + ( 1 - &alpha;<sub>f</sub> ) &alpha;<sub>b</sub>

##### Additive
> **c** = &alpha;<sub>f</sub> **c**<sub>f</sub> + **c**<sub>b</sub>

##### Difference
> **c** = | &alpha;<sub>f</sub> **c**<sub>f</sub> - **c**<sub>b</sub> |

##### Multiply
> **c** = &alpha;<sub>f</sub> ( **c**<sub>f</sub> &middot; **c**<sub>b</sub> ) + ( 1 - &alpha;<sub>f</sub> ) **c**<sub>b</sub>

##### Screen
> **c** = 1 - ( 1 - **c**<sub>f</sub> ) ( 1 - **c**<sub>b</sub> )

#### 2D Transformation

#### Rotation
> [ *x'* , *y'* ] = *x* [ cos &theta; , -sin &theta; ] + *y* [ sin &theta; , cos &theta; ]<br>

```cpp
auto Vec2::rotate(flt theta) const -> Vec2
{
	return (*this) * Mat2x2{
		{ cos(theta), -sin(theta) },
		{ sin(theta),  cos(theta) },
	};
	// OR:
	return Mat2x2{
		{  cos(theta), sin(theta) },
		{ -sin(theta), cos(theta) },
	} * (*this);
}
```
