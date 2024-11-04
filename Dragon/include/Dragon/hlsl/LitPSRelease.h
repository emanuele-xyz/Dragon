#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer CBLighting
// {
//
//   struct
//   {
//       
//       float4 ambient_color;          // Offset:    0
//       float4 light_color;            // Offset:   16
//       float4 light_direction;        // Offset:   32
//
//   } cb_lighting;                     // Offset:    0 Size:    48
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// zampler                           sampler      NA          NA             s0      1 
// albedo                            texture  float4          2d             t0      1 
// CBLighting                        cbuffer      NA          NA            cb2      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float       
// NORMAL                   0   xyz         1     NONE   float   xyz 
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c0         cb2             0         3  ( FLT, FLT, FLT, FLT)
//
//
// Sampler/Resource to DX9 shader sampler mappings:
//
// Target Sampler Source Sampler  Source Resource
// -------------- --------------- ----------------
// s0             s0              t0               
//
//
// Level9 shader bytecode:
//
    ps_2_x
    def c3, 0, 1, 0, 0
    dcl t0.xyz
    dcl t1.xy
    dcl_2d s0
    nrm r0.xyz, t0
    nrm r1.xyz, c2
    dp3 r0.x, r0, -r1
    mul r0.yzw, r0.x, c1.xxyz
    cmp r0.xyz, r0.x, r0.yzww, c3.x
    add r0.xyz, r0, c0
    texld r1, t1, s0
    mov r0.w, c3.y
    mul r0, r0, r1
    mov oC0, r0

// approximately 14 instruction slots used (1 texture, 13 arithmetic)
ps_4_0
dcl_constantbuffer CB2[3], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xy
dcl_output o0.xyzw
dcl_temps 2
dp3 r0.x, v1.xyzx, v1.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, v1.xyzx
dp3 r0.w, cb2[2].xyzx, cb2[2].xyzx
rsq r0.w, r0.w
mul r1.xyz, r0.wwww, cb2[2].xyzx
dp3 r0.x, r0.xyzx, -r1.xyzx
max r0.x, r0.x, l(0.000000)
mad r0.xyz, r0.xxxx, cb2[1].xyzx, cb2[0].xyzx
sample r1.xyzw, v2.xyxx, t0.xyzw, s0
mov r0.w, l(1.000000)
mul o0.xyzw, r0.xyzw, r1.xyzw
ret 
// Approximately 13 instruction slots used
#endif

const BYTE ps_lit_bytecode[] =
{
     68,  88,  66,  67, 143,  38, 
    104, 151,  51,  86,  86, 121, 
    171,  19, 242,  48, 150,  81, 
    181,  93,   1,   0,   0,   0, 
    176,   5,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
     76,   1,   0,   0,  24,   3, 
      0,   0, 148,   3,   0,   0, 
      8,   5,   0,   0, 124,   5, 
      0,   0,  65, 111, 110,  57, 
     12,   1,   0,   0,  12,   1, 
      0,   0,   0,   2, 255, 255, 
    216,   0,   0,   0,  52,   0, 
      0,   0,   1,   0,  40,   0, 
      0,   0,  52,   0,   0,   0, 
     52,   0,   1,   0,  36,   0, 
      0,   0,  52,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   1,   2, 255, 255, 
     81,   0,   0,   5,   3,   0, 
     15, 160,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,   7, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   1,   0,   3, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 144,   0,   8,  15, 160, 
     36,   0,   0,   2,   0,   0, 
      7, 128,   0,   0, 228, 176, 
     36,   0,   0,   2,   1,   0, 
      7, 128,   2,   0, 228, 160, 
      8,   0,   0,   3,   0,   0, 
      1, 128,   0,   0, 228, 128, 
      1,   0, 228, 129,   5,   0, 
      0,   3,   0,   0,  14, 128, 
      0,   0,   0, 128,   1,   0, 
    144, 160,  88,   0,   0,   4, 
      0,   0,   7, 128,   0,   0, 
      0, 128,   0,   0, 249, 128, 
      3,   0,   0, 160,   2,   0, 
      0,   3,   0,   0,   7, 128, 
      0,   0, 228, 128,   0,   0, 
    228, 160,  66,   0,   0,   3, 
      1,   0,  15, 128,   1,   0, 
    228, 176,   0,   8, 228, 160, 
      1,   0,   0,   2,   0,   0, 
      8, 128,   3,   0,  85, 160, 
      5,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      1,   0, 228, 128,   1,   0, 
      0,   2,   0,   8,  15, 128, 
      0,   0, 228, 128, 255, 255, 
      0,   0,  83,  72,  68,  82, 
    196,   1,   0,   0,  64,   0, 
      0,   0, 113,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   2,   0,   0,   0, 
      3,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0,  98,  16,   0,   3, 
     50,  16,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  16,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  52,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  56,   0, 
      0,   7, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  13,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     10,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  82,  68, 
     69,  70, 108,   1,   0,   0, 
      1,   0,   0,   0, 152,   0, 
      0,   0,   3,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,   0,   1,   0,   0, 
     68,   1,   0,   0, 124,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    132,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 139,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 122,  97, 
    109, 112, 108, 101, 114,   0, 
     97, 108,  98, 101, 100, 111, 
      0,  67,  66,  76, 105, 103, 
    104, 116, 105, 110, 103,   0, 
    171, 171, 139,   0,   0,   0, 
      1,   0,   0,   0, 176,   0, 
      0,   0,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 200,   0,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   0,   2,   0,   0,   0, 
     52,   1,   0,   0,   0,   0, 
      0,   0,  99,  98,  95, 108, 
    105, 103, 104, 116, 105, 110, 
    103,   0,  97, 109,  98, 105, 
    101, 110, 116,  95,  99, 111, 
    108, 111, 114,   0, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 108, 105, 
    103, 104, 116,  95,  99, 111, 
    108, 111, 114,   0, 108, 105, 
    103, 104, 116,  95, 100, 105, 
    114, 101,  99, 116, 105, 111, 
    110,   0, 212,   0,   0,   0, 
    228,   0,   0,   0,   0,   0, 
      0,   0, 244,   0,   0,   0, 
    228,   0,   0,   0,  16,   0, 
      0,   0,   0,   1,   0,   0, 
    228,   0,   0,   0,  32,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  12,   0,   0,   0, 
      3,   0,  16,   1,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78, 108,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   7,   7, 
      0,   0,  99,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   3,   3, 
      0,   0,  83,  86,  95,  80, 
    111, 115, 105, 116, 105, 111, 
    110,   0,  78,  79,  82,  77, 
     65,  76,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171
};
