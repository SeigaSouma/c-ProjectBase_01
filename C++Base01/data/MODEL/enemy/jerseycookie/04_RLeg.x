xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 139;
 2.35091;-19.07457;1.35730;,
 3.93514;-17.43606;2.27195;,
 2.27195;-17.43606;3.93514;,
 1.35730;-19.07457;2.35091;,
 0.00000;-17.43606;4.54391;,
 0.00000;-19.07457;2.71459;,
 -2.27195;-17.43606;3.93514;,
 -1.35730;-19.07457;2.35091;,
 -3.93514;-17.43606;2.27195;,
 -2.35091;-19.07457;1.35730;,
 -4.54391;-17.43606;0.00000;,
 -2.71459;-19.07457;0.00000;,
 -3.93514;-17.43606;-2.27195;,
 -2.35091;-19.07457;-1.35730;,
 -2.27195;-17.43606;-3.93514;,
 -1.35730;-19.07457;-2.35091;,
 0.00000;-17.43606;-4.54391;,
 0.00000;-19.07457;-2.71459;,
 2.27195;-17.43606;-3.93514;,
 1.35730;-19.07457;-2.35091;,
 3.93514;-17.43606;-2.27195;,
 2.35091;-19.07457;-1.35730;,
 4.54391;-17.43606;-0.00000;,
 2.71459;-19.07457;-0.00000;,
 3.93514;-17.43606;2.27195;,
 2.35091;-19.07457;1.35730;,
 4.86925;-15.21413;2.81126;,
 2.81126;-15.21413;4.86925;,
 0.00000;-15.21413;5.62252;,
 -2.81126;-15.21413;4.86925;,
 -4.86925;-15.21413;2.81126;,
 -5.62253;-15.21413;0.00000;,
 -4.86925;-15.21413;-2.81126;,
 -2.81126;-15.21413;-4.86925;,
 0.00000;-15.21413;-5.62252;,
 2.81126;-15.21413;-4.86925;,
 4.86925;-15.21413;-2.81126;,
 5.62252;-15.21413;-0.00000;,
 4.86925;-15.21413;2.81126;,
 5.49617;-11.41597;3.17321;,
 3.17321;-11.41597;5.49617;,
 0.00000;-11.41597;6.34643;,
 -3.17321;-11.41597;5.49617;,
 -5.49616;-11.41597;3.17321;,
 -6.34643;-11.41597;0.00000;,
 -5.49616;-11.41597;-3.17321;,
 -3.17321;-11.41597;-5.49617;,
 0.00000;-11.41597;-6.34643;,
 3.17321;-11.41597;-5.49617;,
 5.49617;-11.41597;-3.17321;,
 6.34643;-11.41597;-0.00000;,
 5.49617;-11.41597;3.17321;,
 5.49617;-5.97365;3.17321;,
 3.17321;-5.97365;5.49617;,
 0.00000;-5.97365;6.34643;,
 -3.17321;-5.97365;5.49617;,
 -5.49616;-5.97365;3.17321;,
 -6.34643;-5.97365;0.00000;,
 -5.49616;-5.97365;-3.17321;,
 -3.17321;-5.97365;-5.49617;,
 0.00000;-5.97365;-6.34643;,
 3.17321;-5.97365;-5.49617;,
 5.49617;-5.97365;-3.17321;,
 6.34643;-5.97365;-0.00000;,
 5.49617;-5.97365;3.17321;,
 0.53556;-20.04589;0.92761;,
 0.00000;-20.04589;-0.00000;,
 0.92761;-20.04589;0.53556;,
 0.00000;-20.04589;1.07111;,
 -0.53555;-20.04589;0.92761;,
 -0.92761;-20.04589;0.53556;,
 -1.07112;-20.04589;0.00000;,
 -0.92761;-20.04589;-0.53556;,
 -0.53555;-20.04589;-0.92761;,
 0.00000;-20.04589;-1.07111;,
 0.53556;-20.04589;-0.92761;,
 0.92761;-20.04589;-0.53556;,
 1.07112;-20.04589;-0.00000;,
 3.99325;5.55561;2.30550;,
 0.00000;5.55562;0.00000;,
 2.30550;5.55561;3.99325;,
 0.00000;5.55561;4.61101;,
 -2.30550;5.55561;3.99325;,
 -3.99325;5.55561;2.30551;,
 -4.61101;5.55561;0.00000;,
 -3.99325;5.55561;-2.30550;,
 -2.30550;5.55561;-3.99325;,
 0.00000;5.55561;-4.61101;,
 2.30550;5.55561;-3.99325;,
 3.99325;5.55561;-2.30551;,
 4.61101;5.55561;0.00000;,
 1.35730;-19.07457;2.35091;,
 2.35091;-19.07457;1.35730;,
 0.00000;-19.07457;2.71459;,
 -1.35730;-19.07457;2.35091;,
 -2.35091;-19.07457;1.35730;,
 -2.71459;-19.07457;0.00000;,
 -2.35091;-19.07457;-1.35730;,
 -1.35730;-19.07457;-2.35091;,
 0.00000;-19.07457;-2.71459;,
 1.35730;-19.07457;-2.35091;,
 2.35091;-19.07457;-1.35730;,
 2.71459;-19.07457;-0.00000;,
 5.49617;-5.97365;3.17321;,
 5.49617;0.00000;3.17321;,
 3.17321;0.00000;5.49617;,
 3.17321;-5.97365;5.49617;,
 0.00000;0.00000;6.34643;,
 0.00000;-5.97365;6.34643;,
 -3.17321;0.00000;5.49617;,
 -3.17321;-5.97365;5.49617;,
 -5.49616;0.00000;3.17321;,
 -5.49616;-5.97365;3.17321;,
 -6.34643;0.00000;0.00000;,
 -6.34643;-5.97365;0.00000;,
 -5.49616;0.00000;-3.17321;,
 -5.49616;-5.97365;-3.17321;,
 -3.17321;0.00000;-5.49617;,
 -3.17321;-5.97365;-5.49617;,
 0.00000;0.00000;-6.34643;,
 0.00000;-5.97365;-6.34643;,
 3.17321;0.00000;-5.49617;,
 3.17321;-5.97365;-5.49617;,
 5.49617;0.00000;-3.17321;,
 5.49617;-5.97365;-3.17321;,
 6.34643;0.00000;0.00000;,
 6.34643;-5.97365;-0.00000;,
 4.82745;2.92483;2.78713;,
 2.78713;2.92483;4.82745;,
 0.00000;2.92483;5.57426;,
 -2.78713;2.92483;4.82745;,
 -4.82745;2.92483;2.78713;,
 -5.57426;2.92483;0.00000;,
 -4.82745;2.92483;-2.78713;,
 -2.78713;2.92483;-4.82745;,
 0.00000;2.92483;-5.57426;,
 2.78713;2.92483;-4.82745;,
 4.82745;2.92483;-2.78713;,
 5.57426;2.92483;0.00000;;
 
 120;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;1,26,27,2;,
 4;2,27,28,4;,
 4;4,28,29,6;,
 4;6,29,30,8;,
 4;8,30,31,10;,
 4;10,31,32,12;,
 4;12,32,33,14;,
 4;14,33,34,16;,
 4;16,34,35,18;,
 4;18,35,36,20;,
 4;20,36,37,22;,
 4;22,37,38,24;,
 4;26,39,40,27;,
 4;27,40,41,28;,
 4;28,41,42,29;,
 4;29,42,43,30;,
 4;30,43,44,31;,
 4;31,44,45,32;,
 4;32,45,46,33;,
 4;33,46,47,34;,
 4;34,47,48,35;,
 4;35,48,49,36;,
 4;36,49,50,37;,
 4;37,50,51,38;,
 4;39,52,53,40;,
 4;40,53,54,41;,
 4;41,54,55,42;,
 4;42,55,56,43;,
 4;43,56,57,44;,
 4;44,57,58,45;,
 4;45,58,59,46;,
 4;46,59,60,47;,
 4;47,60,61,48;,
 4;48,61,62,49;,
 4;49,62,63,50;,
 4;50,63,64,51;,
 3;65,66,67;,
 3;68,66,65;,
 3;69,66,68;,
 3;70,66,69;,
 3;71,66,70;,
 3;72,66,71;,
 3;73,66,72;,
 3;74,66,73;,
 3;75,66,74;,
 3;76,66,75;,
 3;77,66,76;,
 3;67,66,77;,
 3;78,79,80;,
 3;80,79,81;,
 3;81,79,82;,
 3;82,79,83;,
 3;83,79,84;,
 3;84,79,85;,
 3;85,79,86;,
 3;86,79,87;,
 3;87,79,88;,
 3;88,79,89;,
 3;89,79,90;,
 3;90,79,78;,
 4;91,65,67,92;,
 4;93,68,65,91;,
 4;94,69,68,93;,
 4;95,70,69,94;,
 4;96,71,70,95;,
 4;97,72,71,96;,
 4;98,73,72,97;,
 4;99,74,73,98;,
 4;100,75,74,99;,
 4;101,76,75,100;,
 4;102,77,76,101;,
 4;92,67,77,102;,
 4;103,104,105,106;,
 4;106,105,107,108;,
 4;108,107,109,110;,
 4;110,109,111,112;,
 4;112,111,113,114;,
 4;114,113,115,116;,
 4;116,115,117,118;,
 4;118,117,119,120;,
 4;120,119,121,122;,
 4;122,121,123,124;,
 4;124,123,125,126;,
 4;126,125,104,103;,
 4;104,127,128,105;,
 4;105,128,129,107;,
 4;107,129,130,109;,
 4;109,130,131,111;,
 4;111,131,132,113;,
 4;113,132,133,115;,
 4;115,133,134,117;,
 4;117,134,135,119;,
 4;119,135,136,121;,
 4;121,136,137,123;,
 4;123,137,138,125;,
 4;125,138,127,104;,
 4;127,78,80,128;,
 4;128,80,81,129;,
 4;129,81,82,130;,
 4;130,82,83,131;,
 4;131,83,84,132;,
 4;132,84,85,133;,
 4;133,85,86,134;,
 4;134,86,87,135;,
 4;135,87,88,136;,
 4;136,88,89,137;,
 4;137,89,90,138;,
 4;138,90,78,127;;
 
 MeshMaterialList {
  1;
  120;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.680784;0.429804;0.103529;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  122;
  0.511465;-0.806973;0.295294;,
  0.295293;-0.806973;0.511465;,
  -0.000000;-0.806973;0.590588;,
  -0.295293;-0.806973;0.511465;,
  -0.511464;-0.806973;0.295295;,
  -0.590589;-0.806973;0.000000;,
  -0.511464;-0.806973;-0.295295;,
  -0.295293;-0.806973;-0.511465;,
  -0.000000;-0.806973;-0.590588;,
  0.295293;-0.806973;-0.511465;,
  0.511465;-0.806973;-0.295294;,
  0.590589;-0.806973;-0.000000;,
  0.690801;-0.603097;0.398834;,
  0.398834;-0.603097;0.690801;,
  0.000000;-0.603097;0.797668;,
  -0.398834;-0.603097;0.690801;,
  -0.690800;-0.603097;0.398834;,
  -0.797667;-0.603098;0.000000;,
  -0.690800;-0.603097;-0.398834;,
  -0.398834;-0.603097;-0.690801;,
  0.000000;-0.603097;-0.797668;,
  0.398834;-0.603097;-0.690801;,
  0.690801;-0.603097;-0.398834;,
  0.797668;-0.603097;-0.000000;,
  0.821931;-0.315024;0.474542;,
  0.474542;-0.315024;0.821931;,
  0.000000;-0.315024;0.949084;,
  -0.474542;-0.315024;0.821931;,
  -0.821930;-0.315024;0.474542;,
  -0.949084;-0.315024;0.000000;,
  -0.821930;-0.315024;-0.474542;,
  -0.474542;-0.315024;-0.821931;,
  0.000000;-0.315024;-0.949084;,
  0.474542;-0.315024;-0.821931;,
  0.821931;-0.315024;-0.474542;,
  0.949084;-0.315024;-0.000000;,
  0.862184;-0.094084;0.497782;,
  0.497782;-0.094084;0.862184;,
  0.000000;-0.094084;0.995564;,
  -0.497783;-0.094083;0.862184;,
  -0.862184;-0.094083;0.497783;,
  -0.995564;-0.094083;0.000000;,
  -0.862184;-0.094083;-0.497783;,
  -0.497783;-0.094083;-0.862184;,
  0.000000;-0.094084;-0.995564;,
  0.497782;-0.094084;-0.862184;,
  0.862184;-0.094084;-0.497782;,
  0.995564;-0.094084;-0.000000;,
  0.866025;0.000000;0.500000;,
  0.500000;0.000000;0.866025;,
  0.000000;0.000000;1.000000;,
  -0.500001;0.000000;0.866025;,
  -0.866025;0.000000;0.500000;,
  -1.000000;0.000000;0.000000;,
  -0.866025;0.000000;-0.500000;,
  -0.500001;0.000000;-0.866025;,
  0.000000;0.000000;-1.000000;,
  0.500000;0.000000;-0.866025;,
  0.866025;0.000000;-0.500000;,
  1.000000;0.000000;0.000000;,
  0.227350;-0.964926;0.131261;,
  0.131260;-0.964926;0.227350;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;-0.964926;0.262521;,
  -0.131259;-0.964926;0.227350;,
  -0.227350;-0.964926;0.131262;,
  -0.262522;-0.964926;0.000000;,
  -0.227350;-0.964926;-0.131262;,
  -0.131259;-0.964926;-0.227350;,
  -0.000000;-0.964926;-0.262521;,
  0.131260;-0.964926;-0.227350;,
  0.227350;-0.964926;-0.131261;,
  0.262522;-0.964926;-0.000000;,
  0.858807;0.128840;0.495833;,
  0.495833;0.128840;0.858807;,
  0.000000;0.128841;0.991665;,
  -0.495834;0.128840;0.858807;,
  -0.858807;0.128840;0.495833;,
  -0.991665;0.128840;0.000000;,
  -0.858807;0.128840;-0.495833;,
  -0.495834;0.128840;-0.858807;,
  0.000000;0.128841;-0.991665;,
  0.495833;0.128840;-0.858807;,
  0.858807;0.128840;-0.495833;,
  0.991665;0.128841;0.000000;,
  0.826161;0.299905;0.476985;,
  0.476985;0.299905;0.826161;,
  0.000000;0.299905;0.953969;,
  -0.476985;0.299904;0.826162;,
  -0.826162;0.299904;0.476985;,
  -0.953969;0.299904;-0.000000;,
  -0.826162;0.299904;-0.476985;,
  -0.476985;0.299904;-0.826161;,
  0.000000;0.299905;-0.953969;,
  0.476985;0.299904;-0.826162;,
  0.826161;0.299904;-0.476985;,
  0.953969;0.299904;0.000000;,
  0.000002;1.000000;0.000001;,
  0.000001;1.000000;0.000002;,
  -0.000000;1.000000;-0.000000;,
  0.000000;1.000000;0.000002;,
  -0.000001;1.000000;0.000002;,
  -0.000002;1.000000;0.000001;,
  -0.000002;1.000000;-0.000000;,
  -0.000002;1.000000;-0.000001;,
  -0.000001;1.000000;-0.000002;,
  0.000000;1.000000;-0.000002;,
  0.000001;1.000000;-0.000002;,
  0.000002;1.000000;-0.000001;,
  0.000002;1.000000;0.000000;,
  0.813227;0.343825;0.469517;,
  0.469517;0.343825;0.813227;,
  0.000000;0.343824;0.939034;,
  -0.469517;0.343824;0.813228;,
  -0.813227;0.343824;0.469518;,
  -0.939034;0.343824;-0.000000;,
  -0.813227;0.343825;-0.469517;,
  -0.469517;0.343825;-0.813227;,
  0.000000;0.343824;-0.939034;,
  0.469517;0.343824;-0.813228;,
  0.813227;0.343824;-0.469518;,
  0.939034;0.343824;0.000000;;
  120;
  4;0,12,13,1;,
  4;1,13,14,2;,
  4;2,14,15,3;,
  4;3,15,16,4;,
  4;4,16,17,5;,
  4;5,17,18,6;,
  4;6,18,19,7;,
  4;7,19,20,8;,
  4;8,20,21,9;,
  4;9,21,22,10;,
  4;10,22,23,11;,
  4;11,23,12,0;,
  4;12,24,25,13;,
  4;13,25,26,14;,
  4;14,26,27,15;,
  4;15,27,28,16;,
  4;16,28,29,17;,
  4;17,29,30,18;,
  4;18,30,31,19;,
  4;19,31,32,20;,
  4;20,32,33,21;,
  4;21,33,34,22;,
  4;22,34,35,23;,
  4;23,35,24,12;,
  4;24,36,37,25;,
  4;25,37,38,26;,
  4;26,38,39,27;,
  4;27,39,40,28;,
  4;28,40,41,29;,
  4;29,41,42,30;,
  4;30,42,43,31;,
  4;31,43,44,32;,
  4;32,44,45,33;,
  4;33,45,46,34;,
  4;34,46,47,35;,
  4;35,47,36,24;,
  4;36,48,49,37;,
  4;37,49,50,38;,
  4;38,50,51,39;,
  4;39,51,52,40;,
  4;40,52,53,41;,
  4;41,53,54,42;,
  4;42,54,55,43;,
  4;43,55,56,44;,
  4;44,56,57,45;,
  4;45,57,58,46;,
  4;46,58,59,47;,
  4;47,59,48,36;,
  3;61,62,60;,
  3;63,62,61;,
  3;64,62,63;,
  3;65,62,64;,
  3;66,62,65;,
  3;67,62,66;,
  3;68,62,67;,
  3;69,62,68;,
  3;70,62,69;,
  3;71,62,70;,
  3;72,62,71;,
  3;60,62,72;,
  3;97,99,98;,
  3;98,99,100;,
  3;100,99,101;,
  3;101,99,102;,
  3;102,99,103;,
  3;103,99,104;,
  3;104,99,105;,
  3;105,99,106;,
  3;106,99,107;,
  3;107,99,108;,
  3;108,99,109;,
  3;109,99,97;,
  4;1,61,60,0;,
  4;2,63,61,1;,
  4;3,64,63,2;,
  4;4,65,64,3;,
  4;5,66,65,4;,
  4;6,67,66,5;,
  4;7,68,67,6;,
  4;8,69,68,7;,
  4;9,70,69,8;,
  4;10,71,70,9;,
  4;11,72,71,10;,
  4;0,60,72,11;,
  4;48,73,74,49;,
  4;49,74,75,50;,
  4;50,75,76,51;,
  4;51,76,77,52;,
  4;52,77,78,53;,
  4;53,78,79,54;,
  4;54,79,80,55;,
  4;55,80,81,56;,
  4;56,81,82,57;,
  4;57,82,83,58;,
  4;58,83,84,59;,
  4;59,84,73,48;,
  4;73,85,86,74;,
  4;74,86,87,75;,
  4;75,87,88,76;,
  4;76,88,89,77;,
  4;77,89,90,78;,
  4;78,90,91,79;,
  4;79,91,92,80;,
  4;80,92,93,81;,
  4;81,93,94,82;,
  4;82,94,95,83;,
  4;83,95,96,84;,
  4;84,96,85,73;,
  4;85,110,111,86;,
  4;86,111,112,87;,
  4;87,112,113,88;,
  4;88,113,114,89;,
  4;89,114,115,90;,
  4;90,115,116,91;,
  4;91,116,117,92;,
  4;92,117,118,93;,
  4;93,118,119,94;,
  4;94,119,120,95;,
  4;95,120,121,96;,
  4;96,121,110,85;;
 }
 MeshTextureCoords {
  139;
  0.375000;0.687500;,
  0.375000;0.593520;,
  0.395830;0.593520;,
  0.395830;0.687500;,
  0.416670;0.593520;,
  0.416670;0.687500;,
  0.437500;0.593520;,
  0.437500;0.687500;,
  0.458330;0.593520;,
  0.458330;0.687500;,
  0.479170;0.593520;,
  0.479170;0.687500;,
  0.500000;0.593520;,
  0.500000;0.687500;,
  0.520830;0.593520;,
  0.520830;0.687500;,
  0.541670;0.593520;,
  0.541670;0.687500;,
  0.562500;0.593520;,
  0.562500;0.687500;,
  0.583330;0.593520;,
  0.583330;0.687500;,
  0.604170;0.593520;,
  0.604170;0.687500;,
  0.625000;0.593520;,
  0.625000;0.687500;,
  0.375000;0.499530;,
  0.395830;0.499530;,
  0.416670;0.499530;,
  0.437500;0.499530;,
  0.458330;0.499530;,
  0.479170;0.499530;,
  0.500000;0.499530;,
  0.520830;0.499530;,
  0.541670;0.499530;,
  0.562500;0.499530;,
  0.583330;0.499530;,
  0.604170;0.499530;,
  0.625000;0.499530;,
  0.375000;0.405540;,
  0.395830;0.405540;,
  0.416670;0.405540;,
  0.437500;0.405540;,
  0.458330;0.405540;,
  0.479170;0.405540;,
  0.500000;0.405540;,
  0.520830;0.405540;,
  0.541670;0.405540;,
  0.562500;0.405540;,
  0.583330;0.405540;,
  0.604170;0.405540;,
  0.625000;0.405540;,
  0.375000;0.311560;,
  0.395830;0.311560;,
  0.416670;0.311560;,
  0.437500;0.311560;,
  0.458330;0.311560;,
  0.479170;0.311560;,
  0.500000;0.311560;,
  0.520830;0.311560;,
  0.541670;0.311560;,
  0.562500;0.311560;,
  0.583330;0.311560;,
  0.604170;0.311560;,
  0.625000;0.311560;,
  0.578120;0.979070;,
  0.500000;0.850000;,
  0.635320;0.921880;,
  0.500000;1.000000;,
  0.421880;0.979070;,
  0.364680;0.921880;,
  0.343750;0.843750;,
  0.364680;0.765620;,
  0.421880;0.708430;,
  0.500000;0.687500;,
  0.578120;0.708430;,
  0.635320;0.765620;,
  0.656250;0.843750;,
  0.635320;0.078120;,
  0.500000;0.162500;,
  0.578120;0.020930;,
  0.500000;0.000000;,
  0.421880;0.020930;,
  0.364680;0.078120;,
  0.343750;0.156250;,
  0.364680;0.234380;,
  0.421880;0.291570;,
  0.500000;0.312500;,
  0.578120;0.291570;,
  0.635320;0.234380;,
  0.656250;0.156250;,
  0.578120;0.979070;,
  0.635320;0.921880;,
  0.500000;1.000000;,
  0.421880;0.979070;,
  0.364680;0.921880;,
  0.343750;0.843750;,
  0.364680;0.765620;,
  0.421880;0.708430;,
  0.500000;0.687500;,
  0.578120;0.708430;,
  0.635320;0.765620;,
  0.656250;0.843750;,
  0.635320;0.078120;,
  0.635320;0.078120;,
  0.578120;0.020930;,
  0.578120;0.020930;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.421880;0.020930;,
  0.421880;0.020930;,
  0.364680;0.078120;,
  0.364680;0.078120;,
  0.343750;0.156250;,
  0.343750;0.156250;,
  0.364680;0.234380;,
  0.364680;0.234380;,
  0.421880;0.291570;,
  0.421880;0.291570;,
  0.500000;0.312500;,
  0.500000;0.312500;,
  0.578120;0.291570;,
  0.578120;0.291570;,
  0.635320;0.234380;,
  0.635320;0.234380;,
  0.656250;0.156250;,
  0.656250;0.156250;,
  0.635320;0.078120;,
  0.578120;0.020930;,
  0.500000;0.000000;,
  0.421880;0.020930;,
  0.364680;0.078120;,
  0.343750;0.156250;,
  0.364680;0.234380;,
  0.421880;0.291570;,
  0.500000;0.312500;,
  0.578120;0.291570;,
  0.635320;0.234380;,
  0.656250;0.156250;;
 }
}