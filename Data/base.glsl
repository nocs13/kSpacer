//#version 120
uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;
uniform vec4   g_vAmbient;
uniform vec4   g_vLight;
uniform vec3   g_vEye;
uniform vec3   g_vEyeDir;
uniform float  g_fTime;
uniform float  g_fRandom;

varying vec3   N;
varying vec3   V;
varying vec3   L;
varying vec2   Texcoord;
varying float  Ldist;
varying float  Lforse;

attribute vec3 g_Vertex;
attribute vec3 g_Normal;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

void main(void)
{
   mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                     g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                     g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);
   //mRot = inverse(mRot);
   //mRot = transpose(mRot);
   V = vec4(g_mTran * vec4(g_Vertex, 1.0)).xyz;
   N = normalize(mRot * g_Normal);
   //N = g_Normal;
   L = normalize(g_vLight.xyz - V);
   Ldist = distance(g_vLight.xyz, V); //length(g_vLight.xyz - V)
   Lforse = 100.0 * g_vLight.w / (1.0 + Ldist);

   gl_Position   = g_mProj * g_mView * vec4(V, 1.0);
   gl_FrontColor = g_Color;
   Texcoord      = g_Texcoord;
}

//Fragment Shader
uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;
uniform vec3      g_vEyeDir;

varying vec2   Texcoord;
varying vec3   N;
varying vec3   V;
varying vec3   L;
varying float  Ldist;
varying float  Lforse;

void main( void )
{
 vec4 color     = texture2D(g_txColor,    Texcoord);
 vec4 normal    = texture2D(g_txNormal,   Texcoord);
 vec4 specular  = texture2D(g_txSpecular, Texcoord);

 normal = (2.0 * normal) - 1.0;
 normal.xyz = normal.xyz;// + N;
 //normal.xyz = N;
 normal.xyz = normalize(normal.xyz);


 float intensity  = 1.0;
       intensity  = max(dot(normal.xyz, normalize(L)), 0.0);// * Lforse;
       intensity  = clamp(intensity, 0.2, 1.0);
       //intensity  = max(dot(normal.xyz, L), 0.0);

 vec4  col = vec4(color.xyz * intensity, color.w) + vec4(specular.xyz, 0);

 col = clamp(col, 0.0, 1.0);
 //col.xyz = normal.xyz;

 gl_FragColor = gl_Color * col;
}
