#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /Zpc /Tps_3_0 /Emain /Fh ShaderHeader/model_lit_ps.h
//    Shader/model_lit_ps.fx
//
//
// Parameters:
//
//   sampler2D Sampler_sampler_colorTex;
//   sampler2D Sampler_sampler_normalTex;
//   float4 _141_fEmissiveScaling;
//   float4 _141_fLightAmbient;
//   float4 _141_fLightColor;
//   float4 _141_fLightDirection;
//   float4 _141_miscFlags;
//
//
// Registers:
//
//   Name                      Reg   Size
//   ------------------------- ----- ----
//   _141_fLightDirection      c0       1
//   _141_fLightColor          c1       1
//   _141_fLightAmbient        c2       1
//   _141_fEmissiveScaling     c10      1
//   _141_miscFlags            c17      1
//   Sampler_sampler_colorTex  s0       1
//   Sampler_sampler_normalTex s1       1
//

    ps_3_0
    def c3, 1.1920929e-007, 0.416666657, 1.05499995, -0.0549999997
    def c4, 0, -1, -0, -0.5
    def c5, 0.305306017, 0.682171106, 0.0125228781, 0
    dcl_texcoord_centroid v0
    dcl_texcoord1_centroid v1.xy
    dcl_texcoord2 v2.xyz
    dcl_texcoord3 v3.xyz
    dcl_texcoord4 v4.xyz
    dcl_2d s0
    dcl_2d s1
    abs r0.x, c17.x
    texld r1, v1, s0
    max r0.yzw, r1_abs.xxyz, c3.x
    log r2.x, r0.y
    log r2.y, r0.z
    log r2.z, r0.w
    mul r0.yzw, r2.xxyz, c3.y
    exp r2.x, r0.y
    exp r2.y, r0.z
    exp r2.z, r0.w
    mad r0.yzw, r2.xxyz, c3.z, c3.w
    max r2.xyz, r0.yzww, c4.x
    cmp r1.xyz, -r0.x, r1, r2
    mul r1, r1, v0
    cmp r2, -r1_abs.w, c4.y, c4.z
    texkill r2
    texld r2, v1, s1
    add r0.yzw, r2.xxyz, c4.w
    add r0.yzw, r0, r0
    mul r2.xyz, r0.z, v3
    mad r2.xyz, r0.y, v4, r2
    mad r0.yzw, r0.w, v2.xxyz, r2.xxyz
    nrm r2.xyz, r0.yzww
    dp3 r0.y, c0, r2
    mul r2.xyz, r0.y, c1
    cmp r0.yzw, r0.y, r2.xxyz, c4.x
    add r0.yzw, r0, c2.xxyz
    mul r0.yzw, r0, r1.xxyz
    mul r0.yzw, r0, c10.x
    mad r1.xyz, r0.yzww, c5.x, c5.y
    mad r1.xyz, r0.yzww, r1, c5.z
    mul r1.xyz, r0.yzww, r1
    min r2.xyz, r1, r0.yzww
    cmp oC0.xyz, -r0.x, r0.yzww, r2
    mov oC0.w, r1.w

// approximately 37 instruction slots used (2 texture, 35 arithmetic)
#endif

const BYTE g_ps30_main[] =
{
      0,   3, 255, 255, 254, 255, 
    107,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 119,   1, 
      0,   0,   0,   3, 255, 255, 
      7,   0,   0,   0,  28,   0, 
      0,   0,  16,   1,   0,   0, 
    112,   1,   0,   0, 168,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   2,   0, 196,   0, 
      0,   0,   0,   0,   0,   0, 
    212,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   6,   0, 
    240,   0,   0,   0,   0,   0, 
      0,   0,   0,   1,   0,   0, 
      2,   0,  10,   0,   1,   0, 
     42,   0,  24,   1,   0,   0, 
      0,   0,   0,   0,  40,   1, 
      0,   0,   2,   0,   2,   0, 
      1,   0,  10,   0,  24,   1, 
      0,   0,   0,   0,   0,   0, 
     59,   1,   0,   0,   2,   0, 
      1,   0,   1,   0,   6,   0, 
     24,   1,   0,   0,   0,   0, 
      0,   0,  76,   1,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      2,   0,  24,   1,   0,   0, 
      0,   0,   0,   0,  97,   1, 
      0,   0,   2,   0,  17,   0, 
      1,   0,  70,   0,  24,   1, 
      0,   0,   0,   0,   0,   0, 
     83,  97, 109, 112, 108, 101, 
    114,  95, 115,  97, 109, 112, 
    108, 101, 114,  95,  99, 111, 
    108, 111, 114,  84, 101, 120, 
      0, 171, 171, 171,   4,   0, 
     12,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  83,  97, 109, 112, 
    108, 101, 114,  95, 115,  97, 
    109, 112, 108, 101, 114,  95, 
    110, 111, 114, 109,  97, 108, 
     84, 101, 120,   0, 171, 171, 
      4,   0,  12,   0,   1,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  95,  49, 
     52,  49,  95, 102,  69, 109, 
    105, 115, 115, 105, 118, 101, 
     83,  99,  97, 108, 105, 110, 
    103,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  95,  49,  52,  49, 
     95, 102,  76, 105, 103, 104, 
    116,  65, 109,  98, 105, 101, 
    110, 116,   0,  95,  49,  52, 
     49,  95, 102,  76, 105, 103, 
    104, 116,  67, 111, 108, 111, 
    114,   0,  95,  49,  52,  49, 
     95, 102,  76, 105, 103, 104, 
    116,  68, 105, 114, 101,  99, 
    116, 105, 111, 110,   0,  95, 
     49,  52,  49,  95, 109, 105, 
    115,  99,  70, 108,  97, 103, 
    115,   0, 112, 115,  95,  51, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  57,  46,  50,  57, 
     46,  57,  53,  50,  46,  51, 
     49,  49,  49,   0,  81,   0, 
      0,   5,   3,   0,  15, 160, 
      0,   0,   0,  52,  85,  85, 
    213,  62,  61,  10, 135,  63, 
    174,  71,  97, 189,  81,   0, 
      0,   5,   4,   0,  15, 160, 
      0,   0,   0,   0,   0,   0, 
    128, 191,   0,   0,   0, 128, 
      0,   0,   0, 191,  81,   0, 
      0,   5,   5,   0,  15, 160, 
     18,  81, 156,  62, 196, 162, 
     46,  63, 194,  44,  77,  60, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      0,   0,  79, 144,  31,   0, 
      0,   2,   5,   0,   1, 128, 
      1,   0,  67, 144,  31,   0, 
      0,   2,   5,   0,   2, 128, 
      2,   0,   7, 144,  31,   0, 
      0,   2,   5,   0,   3, 128, 
      3,   0,   7, 144,  31,   0, 
      0,   2,   5,   0,   4, 128, 
      4,   0,   7, 144,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      1,   8,  15, 160,  35,   0, 
      0,   2,   0,   0,   1, 128, 
     17,   0,   0, 160,  66,   0, 
      0,   3,   1,   0,  15, 128, 
      1,   0, 228, 144,   0,   8, 
    228, 160,  11,   0,   0,   3, 
      0,   0,  14, 128,   1,   0, 
    144, 139,   3,   0,   0, 160, 
     15,   0,   0,   2,   2,   0, 
      1, 128,   0,   0,  85, 128, 
     15,   0,   0,   2,   2,   0, 
      2, 128,   0,   0, 170, 128, 
     15,   0,   0,   2,   2,   0, 
      4, 128,   0,   0, 255, 128, 
      5,   0,   0,   3,   0,   0, 
     14, 128,   2,   0, 144, 128, 
      3,   0,  85, 160,  14,   0, 
      0,   2,   2,   0,   1, 128, 
      0,   0,  85, 128,  14,   0, 
      0,   2,   2,   0,   2, 128, 
      0,   0, 170, 128,  14,   0, 
      0,   2,   2,   0,   4, 128, 
      0,   0, 255, 128,   4,   0, 
      0,   4,   0,   0,  14, 128, 
      2,   0, 144, 128,   3,   0, 
    170, 160,   3,   0, 255, 160, 
     11,   0,   0,   3,   2,   0, 
      7, 128,   0,   0, 249, 128, 
      4,   0,   0, 160,  88,   0, 
      0,   4,   1,   0,   7, 128, 
      0,   0,   0, 129,   1,   0, 
    228, 128,   2,   0, 228, 128, 
      5,   0,   0,   3,   1,   0, 
     15, 128,   1,   0, 228, 128, 
      0,   0, 228, 144,  88,   0, 
      0,   4,   2,   0,  15, 128, 
      1,   0, 255, 140,   4,   0, 
     85, 160,   4,   0, 170, 160, 
     65,   0,   0,   1,   2,   0, 
     15, 128,  66,   0,   0,   3, 
      2,   0,  15, 128,   1,   0, 
    228, 144,   1,   8, 228, 160, 
      2,   0,   0,   3,   0,   0, 
     14, 128,   2,   0, 144, 128, 
      4,   0, 255, 160,   2,   0, 
      0,   3,   0,   0,  14, 128, 
      0,   0, 228, 128,   0,   0, 
    228, 128,   5,   0,   0,   3, 
      2,   0,   7, 128,   0,   0, 
    170, 128,   3,   0, 228, 144, 
      4,   0,   0,   4,   2,   0, 
      7, 128,   0,   0,  85, 128, 
      4,   0, 228, 144,   2,   0, 
    228, 128,   4,   0,   0,   4, 
      0,   0,  14, 128,   0,   0, 
    255, 128,   2,   0, 144, 144, 
      2,   0, 144, 128,  36,   0, 
      0,   2,   2,   0,   7, 128, 
      0,   0, 249, 128,   8,   0, 
      0,   3,   0,   0,   2, 128, 
      0,   0, 228, 160,   2,   0, 
    228, 128,   5,   0,   0,   3, 
      2,   0,   7, 128,   0,   0, 
     85, 128,   1,   0, 228, 160, 
     88,   0,   0,   4,   0,   0, 
     14, 128,   0,   0,  85, 128, 
      2,   0, 144, 128,   4,   0, 
      0, 160,   2,   0,   0,   3, 
      0,   0,  14, 128,   0,   0, 
    228, 128,   2,   0, 144, 160, 
      5,   0,   0,   3,   0,   0, 
     14, 128,   0,   0, 228, 128, 
      1,   0, 144, 128,   5,   0, 
      0,   3,   0,   0,  14, 128, 
      0,   0, 228, 128,  10,   0, 
      0, 160,   4,   0,   0,   4, 
      1,   0,   7, 128,   0,   0, 
    249, 128,   5,   0,   0, 160, 
      5,   0,  85, 160,   4,   0, 
      0,   4,   1,   0,   7, 128, 
      0,   0, 249, 128,   1,   0, 
    228, 128,   5,   0, 170, 160, 
      5,   0,   0,   3,   1,   0, 
      7, 128,   0,   0, 249, 128, 
      1,   0, 228, 128,  10,   0, 
      0,   3,   2,   0,   7, 128, 
      1,   0, 228, 128,   0,   0, 
    249, 128,  88,   0,   0,   4, 
      0,   8,   7, 128,   0,   0, 
      0, 129,   0,   0, 249, 128, 
      2,   0, 228, 128,   1,   0, 
      0,   2,   0,   8,   8, 128, 
      1,   0, 255, 128, 255, 255, 
      0,   0
};
