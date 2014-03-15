A very magnificent Raytracer, by Alec Mouri and Austin Chen

This application is intended to be built with Microsoft Visual Studio 2013.

This raytracer supports linear translation, basic ellipsoid and polygon drawing, reading of modified obj files,
refractions, mirror reflections, motion blur, Phong shading, shadows, point and directional lighting, and png writing.

The input file is always of the name "input.test". It has the following commands:

# comment -- The hash tag denotes a comment. The line following '#' is ignored.
size height width -- This must be the first line of input that is neither a comment nor a blank line. Specifies the image size.
maxdepth max_depth -- The maximum recursion depth of the raytracer. Default is 5.
aliasing aa -- The number of rays to be sent per pixel is aa^2. Default is 1.
output output_name -- The name of the output file.
camera x y z lx ly lz upx upy upz fov -- x,y,z is the camera position. lx, ly, lz is the place where the camera is looking. upx upy upz is the direction of the up vector. fov is the field of view. Proper behavior should have (lx,ly,lz) = (0,0,1), (upx,upy,upz) = (0,1,0), and fov = 1
g name -- specifies a group. All code following it and preceding a "g" denotes an object.
sphere x y z r -- x y z is the center, r is the radius of a sphere
maxverts max_verts -- The maximum number of vertices in the scene.
maxvertnorms max_verts -- The maximum number of vertices with normals in the scene.
v v1 v2 v3 -- A vertex at v1,v2,v3
vn v1 v2 v3 -- A vertex normal at v1,v2,v3
f v1//[vn1] v2//[vn2] v3//[vn3] -- A face with vertices v1,v2,v3 and normals vn1,vn2,vn3
translate x y z -- specifies a translation along x,y,z
rotate x y z r -- specifies a rotation along x,y,z by r degrees.
scale x y z -- scales the object by x,y,z
pushTransform -- pushes the transformation of the stack to be applied to future objects.
popTransform -- pops the topmost transformation from the stack
directional x y z r g b -- Specifies a direcgtional light with direction x,y,z and color r,g,b
point x y z r g b -- Specifies a point light at x,y,z and color r,g,b
ambient r g b -- Specifies the ambiance of the scene to be r,g,b
diffuse r g b -- Specifies the diffuse coefficient of the preceding object to r g b
specular r g b -- Specifies the specular component of the preceding object to be r g b
shininess sp -- Specifies the roughness of the preceding object to be sp
emission r g b -- Specifies the ambiant coefficient of the preceding object to be r g b
reflectance r g b -- Specifies the reflectance coefficient of the preceding object to be r g b
