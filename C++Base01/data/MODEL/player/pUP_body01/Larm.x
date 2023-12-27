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
 29;
 16.58807;-5.46472;-4.91185;,
 0.00000;-5.02653;-4.24397;,
 0.00000;5.02650;-4.24397;,
 16.58807;5.46472;-4.91185;,
 -0.76379;5.65013;0.00000;,
 -0.76379;5.65013;-4.14936;,
 -0.76379;0.00002;-4.14936;,
 -0.76379;0.00002;0.00000;,
 0.00000;-5.02653;4.24398;,
 16.58807;-5.46472;4.91185;,
 16.58807;5.46472;4.91185;,
 0.00000;5.02650;4.24398;,
 17.37422;-6.14264;4.51110;,
 17.37422;-6.14264;-4.51110;,
 17.37422;6.14271;-4.51110;,
 17.37422;6.14271;4.51110;,
 16.58807;6.68837;-4.01323;,
 0.00000;5.77893;-3.69141;,
 0.00000;5.77893;3.69141;,
 16.58807;6.68837;4.01323;,
 16.58807;-6.68837;4.01323;,
 0.00000;-5.77890;3.69141;,
 0.00000;-5.77890;-3.69141;,
 16.58807;-6.68837;-4.01323;,
 -0.76379;-5.65010;-4.14936;,
 -0.76379;-5.65010;4.14936;,
 -0.76379;0.00002;4.14936;,
 -0.76379;5.65013;4.14936;,
 -0.76379;-5.65010;0.00000;;
 
 33;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;13,0,3,14;,
 4;14,3,2,5;,
 3;1,24,2;,
 3;2,6,5;,
 3;6,2,24;,
 4;24,1,0,13;,
 3;25,8,26;,
 3;26,8,27;,
 3;27,8,11;,
 4;27,11,10,15;,
 4;15,10,9,12;,
 4;12,9,8,25;,
 4;14,16,19,15;,
 4;15,19,18,27;,
 3;5,4,17;,
 3;4,27,17;,
 3;17,27,18;,
 4;5,17,16,14;,
 4;12,20,23,13;,
 4;13,23,22,24;,
 3;24,22,28;,
 3;28,22,25;,
 3;25,22,21;,
 4;25,21,20,12;,
 4;24,28,7,6;,
 4;26,27,4,7;,
 4;28,25,26,7;;
 
 MeshMaterialList {
  1;
  33;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "body_UV_Yellow.jpg";
   }
  }
 }
 MeshNormals {
  31;
  0.886073;-0.463546;-0.000000;,
  0.886059;0.463573;0.000000;,
  -0.088705;-0.325938;-0.941220;,
  -1.000000;0.000000;0.000000;,
  -0.073796;-0.436075;0.896879;,
  -0.088713;0.325939;0.941219;,
  0.131306;-0.119943;-0.984059;,
  0.131305;0.119933;-0.984060;,
  -0.090078;0.069680;-0.993494;,
  -0.067553;-0.116590;-0.990880;,
  -0.090086;-0.069687;0.993493;,
  -0.067557;0.116583;0.990881;,
  0.131306;0.119936;0.984060;,
  0.131306;-0.119946;0.984059;,
  0.171227;0.966661;0.190390;,
  0.171227;0.966661;-0.190390;,
  -0.091076;0.979137;0.181649;,
  -0.122382;0.986597;-0.107929;,
  0.171246;-0.966655;0.190402;,
  0.171246;-0.966655;-0.190402;,
  -0.122382;-0.986597;-0.107935;,
  -0.091077;-0.979135;0.181659;,
  -0.122943;0.000000;0.992414;,
  -0.166285;-0.986078;0.000000;,
  0.140620;-0.447825;-0.882994;,
  0.140621;0.447824;-0.882995;,
  -0.073793;0.436071;-0.896882;,
  -0.122929;0.000000;-0.992415;,
  0.140621;0.447827;0.882993;,
  0.140620;-0.447828;0.882993;,
  -0.166285;0.986078;0.000000;;
  33;
  4;6,9,8,7;,
  4;3,3,3,3;,
  4;10,13,12,11;,
  4;0,0,1,1;,
  4;15,17,16,14;,
  4;18,21,20,19;,
  4;24,6,7,25;,
  4;25,7,8,26;,
  3;9,2,8;,
  3;8,27,26;,
  3;27,8,2;,
  4;2,9,6,24;,
  3;4,10,22;,
  3;22,10,5;,
  3;5,10,11;,
  4;5,11,12,28;,
  4;28,12,13,29;,
  4;29,13,10,4;,
  4;1,15,14,1;,
  4;28,14,16,5;,
  3;30,30,17;,
  3;30,30,17;,
  3;17,30,16;,
  4;26,17,15,25;,
  4;0,18,19,0;,
  4;24,19,20,2;,
  3;23,20,23;,
  3;23,20,23;,
  3;23,20,21;,
  4;4,21,18,29;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;3,3,3,3;;
 }
 MeshTextureCoords {
  29;
  0.299157;0.221996;,
  0.138340;0.216236;,
  0.138340;0.084084;,
  0.299157;0.078324;,
  0.130935;0.075886;,
  0.130935;0.075886;,
  0.130935;0.150160;,
  0.130935;0.150160;,
  0.138340;0.216236;,
  0.299157;0.221996;,
  0.299157;0.078324;,
  0.138340;0.084084;,
  0.306779;0.230908;,
  0.306779;0.230908;,
  0.306779;0.069411;,
  0.306779;0.069411;,
  0.299157;0.062238;,
  0.138340;0.074193;,
  0.138340;0.074193;,
  0.299157;0.062238;,
  0.299157;0.238082;,
  0.138340;0.226127;,
  0.138340;0.226127;,
  0.299157;0.238082;,
  0.130935;0.224433;,
  0.130935;0.224433;,
  0.130935;0.150160;,
  0.130935;0.075886;,
  0.130935;0.224433;;
 }
}