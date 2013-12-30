#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

attribute vec3 g_Vertex;
attribute vec4 g_Color;

varying   vec4 vColor;

void main(void)
{
   gl_Position  = g_mProj * g_mView * g_mTran * vec4(g_Vertex, 1.0);
   vColor = g_Color;
}

//Fragment Shader
varying   vec4 vColor;

void main( void )
{
  gl_FragColor = vColor;
}
