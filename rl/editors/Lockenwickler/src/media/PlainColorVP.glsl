varying float depth;

void main()
{
	depth = (gl_ModelViewMatrix * gl_Vertex).z;
	gl_Position = ftransform();
}


