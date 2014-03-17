## About
A very magnificent Raytracer, by Alec Mouri (cs184-cq) and Austin Chen (cs184-cn)

This application is intended to be built with Microsoft Visual Studio 2013 and uses the Eigen and FreeImage libraries.

## Features

This raytracer has the following features:

* Parsing of modified .obj files (described below)
* Primitive definition of spheres and triangles
* Shading with the Phong illumination model

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-01.png)

* Linear transformations (ie. translations, rotations, scaling)
* Shadows

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-05.png)

* Specification of point and directional lights

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-10.png)

* Arbitrary camera positioning + transformations of the viewing plane

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-03.png)

* Acceleration with AABB-trees

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-07.png)
![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-08.png)

* Depth of field

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-12.png)

* Environment mapping
* Motion blur

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-51.png)

* Anti-aliasing
* Reflections
* Refractions

![](http://inst.cs.berkeley.edu/~cs184-cn/Raytracer/output/image-50.png)

* Writing to common image formats via FreeImage

## Input

The input file is a text file of the form <imagename>-<number>.test. It supports the following commands.

* `# comment` -- The hash tag denotes a comment. The line following '#' is ignored.
* `size height width` -- This must be the first line of input that is neither a comment nor a blank line. Specifies the image size.
* `maxdepth max_depth` -- The maximum recursion depth of the raytracer. Default is 0.
* `aliasing aa` -- The number of rays to be sent per pixel is aa^2. Default is 1.
* `output output_name` -- The name of the output file. The file type will be parsed from the extension.
* `camera x y z` -- x,y,z is the camera position. The camera always points towards the viewing plane centered at (0, 0, -1), unless the camera is transformed.
* `g name` -- specifies a group. All code following it and preceding a "g" denotes an object. Modifiers such as diffuse coefficients will be specified to all members of the group currently defined.
* `sphere x y z r` -- x y z is the center, r is the radius of a sphere
* `maxverts max_verts` -- The maximum number of vertices in the scene.
* `maxvertnorms max_verts` -- The maximum number of vertices with normals in the scene.
* `v v1 v2 v3` -- A vertex at v1,v2,v3
* `vn v1 v2 v3` -- A vertex normal at v1,v2,v3
* `f v1//[vn1] v2//[vn2] v3//[vn3]` -- A face with vertices v1,v2,v3 and normals vn1,vn2,vn3
* `translate x y z` -- specifies a translation along x,y,z
* `rotate x y z r` -- specifies a rotation along x,y,z by r degrees.
* `scale x y z` -- scales the object by x,y,z
* `pushTransform` -- pushes the transformation of the stack to be applied to future objects.
* `popTransform` -- pops the topmost transformation from the stack
* `directional x y z r g b` -- Specifies a direcgtional light with direction x,y,z and color r,g,b
* `point x y z r g b` -- Specifies a point light at x,y,z and color r,g,b
* `ambient r g b` -- Specifies the ambiance of the scene to be r,g,b
* `diffuse r g b` -- Specifies the diffuse coefficient of the preceding object to r g b
* `specular r g b` -- Specifies the specular component of the preceding object to be r g b
* `shininess sp` -- Specifies the roughness of the preceding object to be sp
* `emission r g b` -- Specifies the ambiant coefficient of the preceding object to be r g b
* `reflectance r g b` -- Specifies the reflectance coefficient of the preceding object to be r g b
* `envmap filename x1 y1 z1 x2 y2 z2 x3 y3 z3` -- Defines an environment map with source filename at the location specified by the parallelogram defined by (x1,y1,z1),(x2,y2,z2),(x3,y3,z3)
* `dof depth_dist` -- Specifies the location of the lens and turns depth of field on.
* `frames num` -- Specifies the number of frames to draw with motion blur.
* `velocity dx dy dz` -- Specifies a velocity to the most recently defined object.
* `acceleration dx dy dz` -- Specifies an acceleration to the most recently defined object.

The program outputs an image and a notes file, denoting the name of the input file and how long it took to render the image.