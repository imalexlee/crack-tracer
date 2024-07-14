
../out/debug/crack-tracer:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    rsp,0x8
    1008:	48 8b 05 d9 2f 00 00 	mov    rax,QWORD PTR [rip+0x2fd9]        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   rax,rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   rax
    1016:	48 83 c4 08          	add    rsp,0x8
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 92 2f 00 00    	push   QWORD PTR [rip+0x2f92]        # 3fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 93 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f93]        # 3fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nop    DWORD PTR [rax]
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop

Disassembly of section .plt.got:

0000000000001050 <__cxa_finalize@plt>:
    1050:	f3 0f 1e fa          	endbr64 
    1054:	f2 ff 25 9d 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f9d]        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    105b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

Disassembly of section .plt.sec:

0000000000001060 <__stack_chk_fail@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 5d 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f5d]        # 3fc8 <__stack_chk_fail@GLIBC_2.4>
    106b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000001070 <printf@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 55 2f 00 00 	bnd jmp QWORD PTR [rip+0x2f55]        # 3fd0 <printf@GLIBC_2.2.5>
    107b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

Disassembly of section .text:

0000000000001080 <_start>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	31 ed                	xor    ebp,ebp
    1086:	49 89 d1             	mov    r9,rdx
    1089:	5e                   	pop    rsi
    108a:	48 89 e2             	mov    rdx,rsp
    108d:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
    1091:	50                   	push   rax
    1092:	54                   	push   rsp
    1093:	45 31 c0             	xor    r8d,r8d
    1096:	31 c9                	xor    ecx,ecx
    1098:	48 8d 3d f6 0c 00 00 	lea    rdi,[rip+0xcf6]        # 1d95 <main>
    109f:	ff 15 33 2f 00 00    	call   QWORD PTR [rip+0x2f33]        # 3fd8 <__libc_start_main@GLIBC_2.34>
    10a5:	f4                   	hlt    
    10a6:	66 2e 0f 1f 84 00 00 	cs nop WORD PTR [rax+rax*1+0x0]
    10ad:	00 00 00 

00000000000010b0 <deregister_tm_clones>:
    10b0:	48 8d 3d 59 2f 00 00 	lea    rdi,[rip+0x2f59]        # 4010 <__TMC_END__>
    10b7:	48 8d 05 52 2f 00 00 	lea    rax,[rip+0x2f52]        # 4010 <__TMC_END__>
    10be:	48 39 f8             	cmp    rax,rdi
    10c1:	74 15                	je     10d8 <deregister_tm_clones+0x28>
    10c3:	48 8b 05 16 2f 00 00 	mov    rax,QWORD PTR [rip+0x2f16]        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    10ca:	48 85 c0             	test   rax,rax
    10cd:	74 09                	je     10d8 <deregister_tm_clones+0x28>
    10cf:	ff e0                	jmp    rax
    10d1:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]
    10d8:	c3                   	ret    
    10d9:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

00000000000010e0 <register_tm_clones>:
    10e0:	48 8d 3d 29 2f 00 00 	lea    rdi,[rip+0x2f29]        # 4010 <__TMC_END__>
    10e7:	48 8d 35 22 2f 00 00 	lea    rsi,[rip+0x2f22]        # 4010 <__TMC_END__>
    10ee:	48 29 fe             	sub    rsi,rdi
    10f1:	48 89 f0             	mov    rax,rsi
    10f4:	48 c1 ee 3f          	shr    rsi,0x3f
    10f8:	48 c1 f8 03          	sar    rax,0x3
    10fc:	48 01 c6             	add    rsi,rax
    10ff:	48 d1 fe             	sar    rsi,1
    1102:	74 14                	je     1118 <register_tm_clones+0x38>
    1104:	48 8b 05 e5 2e 00 00 	mov    rax,QWORD PTR [rip+0x2ee5]        # 3ff0 <_ITM_registerTMCloneTable@Base>
    110b:	48 85 c0             	test   rax,rax
    110e:	74 08                	je     1118 <register_tm_clones+0x38>
    1110:	ff e0                	jmp    rax
    1112:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
    1118:	c3                   	ret    
    1119:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001120 <__do_global_dtors_aux>:
    1120:	f3 0f 1e fa          	endbr64 
    1124:	80 3d e5 2e 00 00 00 	cmp    BYTE PTR [rip+0x2ee5],0x0        # 4010 <__TMC_END__>
    112b:	75 2b                	jne    1158 <__do_global_dtors_aux+0x38>
    112d:	55                   	push   rbp
    112e:	48 83 3d c2 2e 00 00 	cmp    QWORD PTR [rip+0x2ec2],0x0        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1135:	00 
    1136:	48 89 e5             	mov    rbp,rsp
    1139:	74 0c                	je     1147 <__do_global_dtors_aux+0x27>
    113b:	48 8b 3d c6 2e 00 00 	mov    rdi,QWORD PTR [rip+0x2ec6]        # 4008 <__dso_handle>
    1142:	e8 09 ff ff ff       	call   1050 <__cxa_finalize@plt>
    1147:	e8 64 ff ff ff       	call   10b0 <deregister_tm_clones>
    114c:	c6 05 bd 2e 00 00 01 	mov    BYTE PTR [rip+0x2ebd],0x1        # 4010 <__TMC_END__>
    1153:	5d                   	pop    rbp
    1154:	c3                   	ret    
    1155:	0f 1f 00             	nop    DWORD PTR [rax]
    1158:	c3                   	ret    
    1159:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000001160 <frame_dummy>:
    1160:	f3 0f 1e fa          	endbr64 
    1164:	e9 77 ff ff ff       	jmp    10e0 <register_tm_clones>

0000000000001169 <_Z9print_vecDv8_f>:
struct Sphere {
  Vec3 center;
  float radius;
};

void print_vec(__m256 vec) {
    1169:	f3 0f 1e fa          	endbr64 
    116d:	4c 8d 54 24 08       	lea    r10,[rsp+0x8]
    1172:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
    1176:	41 ff 72 f8          	push   QWORD PTR [r10-0x8]
    117a:	55                   	push   rbp
    117b:	48 89 e5             	mov    rbp,rsp
    117e:	41 52                	push   r10
    1180:	48 83 ec 28          	sub    rsp,0x28
    1184:	c5 fc 29 45 d0       	vmovaps YMMWORD PTR [rbp-0x30],ymm0
  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4],
         vec[5], vec[6], vec[7]);
    1189:	c5 fa 10 45 ec       	vmovss xmm0,DWORD PTR [rbp-0x14]
  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4],
    118e:	c5 fa 5a f0          	vcvtss2sd xmm6,xmm0,xmm0
         vec[5], vec[6], vec[7]);
    1192:	c5 fa 10 45 e8       	vmovss xmm0,DWORD PTR [rbp-0x18]
  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4],
    1197:	c5 fa 5a e8          	vcvtss2sd xmm5,xmm0,xmm0
         vec[5], vec[6], vec[7]);
    119b:	c5 fa 10 45 e4       	vmovss xmm0,DWORD PTR [rbp-0x1c]
  printf("%f %f %f %f %f %f %f %f\n", vec[0], vec[1], vec[2], vec[3], vec[4],
    11a0:	c5 fa 5a e0          	vcvtss2sd xmm4,xmm0,xmm0
    11a4:	c5 fa 10 45 e0       	vmovss xmm0,DWORD PTR [rbp-0x20]
    11a9:	c5 fa 5a d8          	vcvtss2sd xmm3,xmm0,xmm0
    11ad:	c5 fa 10 45 dc       	vmovss xmm0,DWORD PTR [rbp-0x24]
    11b2:	c5 fa 5a d0          	vcvtss2sd xmm2,xmm0,xmm0
    11b6:	c5 fa 10 45 d8       	vmovss xmm0,DWORD PTR [rbp-0x28]
    11bb:	c5 fa 5a c8          	vcvtss2sd xmm1,xmm0,xmm0
    11bf:	c5 fa 10 45 d4       	vmovss xmm0,DWORD PTR [rbp-0x2c]
    11c4:	c5 fa 5a c0          	vcvtss2sd xmm0,xmm0,xmm0
    11c8:	c5 fa 10 7d d0       	vmovss xmm7,DWORD PTR [rbp-0x30]
    11cd:	c5 c2 5a ff          	vcvtss2sd xmm7,xmm7,xmm7
    11d1:	c4 e1 f9 7e f8       	vmovq  rax,xmm7
    11d6:	c5 cb 10 fe          	vmovsd xmm7,xmm6,xmm6
    11da:	c5 d3 10 f5          	vmovsd xmm6,xmm5,xmm5
    11de:	c5 db 10 ec          	vmovsd xmm5,xmm4,xmm4
    11e2:	c5 e3 10 e3          	vmovsd xmm4,xmm3,xmm3
    11e6:	c5 eb 10 da          	vmovsd xmm3,xmm2,xmm2
    11ea:	c5 f3 10 d1          	vmovsd xmm2,xmm1,xmm1
    11ee:	c5 fb 10 c8          	vmovsd xmm1,xmm0,xmm0
    11f2:	c4 e1 f9 6e c0       	vmovq  xmm0,rax
    11f7:	48 8d 05 15 0e 00 00 	lea    rax,[rip+0xe15]        # 2013 <_ZL7CMPLTPS+0x1>
    11fe:	48 89 c7             	mov    rdi,rax
    1201:	b8 08 00 00 00       	mov    eax,0x8
    1206:	e8 65 fe ff ff       	call   1070 <printf@plt>
}
    120b:	90                   	nop
    120c:	4c 8b 55 f8          	mov    r10,QWORD PTR [rbp-0x8]
    1210:	c9                   	leave  
    1211:	49 8d 62 f8          	lea    rsp,[r10-0x8]
    1215:	c3                   	ret    

0000000000001216 <_Z10sphere_hitRK10RayClusterf6Sphere>:
  __m256 norm_y;
  __m256 norm_z;
};

// Returns hit t values or 0 depending on if this ray hit this sphere
__m256 sphere_hit(const RayCluster &rays, float t_max, Sphere sphere) {
    1216:	f3 0f 1e fa          	endbr64 
    121a:	55                   	push   rbp
    121b:	48 89 e5             	mov    rbp,rsp
    121e:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
    1222:	48 81 ec e0 08 00 00 	sub    rsp,0x8e0
    1229:	48 89 7c 24 28       	mov    QWORD PTR [rsp+0x28],rdi
    122e:	c5 fa 11 44 24 24    	vmovss DWORD PTR [rsp+0x24],xmm0
    1234:	c5 f9 6f c2          	vmovdqa xmm0,xmm2
    1238:	c5 f9 d6 4c 24 14    	vmovq  QWORD PTR [rsp+0x14],xmm1
    123e:	c5 f9 d6 44 24 1c    	vmovq  QWORD PTR [rsp+0x1c],xmm0
    1244:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    124b:	00 00 
    124d:	48 89 84 24 d8 08 00 	mov    QWORD PTR [rsp+0x8d8],rax
    1254:	00 
    1255:	31 c0                	xor    eax,eax
    1257:	48 8d 44 24 14       	lea    rax,[rsp+0x14]
    125c:	48 89 44 24 38       	mov    QWORD PTR [rsp+0x38],rax
}

extern __inline __m256 __attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm256_broadcast_ss (float const *__X)
{
  return (__m256) __builtin_ia32_vbroadcastss256 (__X);
    1261:	48 8b 44 24 38       	mov    rax,QWORD PTR [rsp+0x38]
    1266:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    126b:	90                   	nop
  __m256 sphere_center_x = _mm256_broadcast_ss(&sphere.center.x);
    126c:	c5 fc 29 44 24 60    	vmovaps YMMWORD PTR [rsp+0x60],ymm0
    1272:	48 8d 44 24 14       	lea    rax,[rsp+0x14]
    1277:	48 83 c0 04          	add    rax,0x4
    127b:	48 89 44 24 40       	mov    QWORD PTR [rsp+0x40],rax
    1280:	48 8b 44 24 40       	mov    rax,QWORD PTR [rsp+0x40]
    1285:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    128a:	90                   	nop
  __m256 sphere_center_y = _mm256_broadcast_ss(&sphere.center.y);
    128b:	c5 fc 29 84 24 80 00 	vmovaps YMMWORD PTR [rsp+0x80],ymm0
    1292:	00 00 
    1294:	48 8d 44 24 14       	lea    rax,[rsp+0x14]
    1299:	48 83 c0 08          	add    rax,0x8
    129d:	48 89 44 24 48       	mov    QWORD PTR [rsp+0x48],rax
    12a2:	48 8b 44 24 48       	mov    rax,QWORD PTR [rsp+0x48]
    12a7:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    12ac:	90                   	nop
  __m256 sphere_center_z = _mm256_broadcast_ss(&sphere.center.z);
    12ad:	c5 fc 29 84 24 a0 00 	vmovaps YMMWORD PTR [rsp+0xa0],ymm0
    12b4:	00 00 

  __m256 oc_x = _mm256_sub_ps(rays.orig_x, sphere_center_x);
    12b6:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    12bb:	c5 fc 28 40 60       	vmovaps ymm0,YMMWORD PTR [rax+0x60]
    12c0:	c5 fc 29 84 24 c0 03 	vmovaps YMMWORD PTR [rsp+0x3c0],ymm0
    12c7:	00 00 
    12c9:	c5 fc 28 44 24 60    	vmovaps ymm0,YMMWORD PTR [rsp+0x60]
    12cf:	c5 fc 29 84 24 e0 03 	vmovaps YMMWORD PTR [rsp+0x3e0],ymm0
    12d6:	00 00 
  return (__m256) ((__v8sf)__A - (__v8sf)__B);
    12d8:	c5 fc 28 84 24 c0 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x3c0]
    12df:	00 00 
    12e1:	c5 fc 5c 84 24 e0 03 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x3e0]
    12e8:	00 00 
    12ea:	c5 fc 29 84 24 c0 00 	vmovaps YMMWORD PTR [rsp+0xc0],ymm0
    12f1:	00 00 
  __m256 oc_y = _mm256_sub_ps(rays.orig_y, sphere_center_y);
    12f3:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    12f8:	c5 fc 28 80 80 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0x80]
    12ff:	00 
    1300:	c5 fc 29 84 24 80 03 	vmovaps YMMWORD PTR [rsp+0x380],ymm0
    1307:	00 00 
    1309:	c5 fc 28 84 24 80 00 	vmovaps ymm0,YMMWORD PTR [rsp+0x80]
    1310:	00 00 
    1312:	c5 fc 29 84 24 a0 03 	vmovaps YMMWORD PTR [rsp+0x3a0],ymm0
    1319:	00 00 
    131b:	c5 fc 28 84 24 80 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x380]
    1322:	00 00 
    1324:	c5 fc 5c 84 24 a0 03 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x3a0]
    132b:	00 00 
    132d:	c5 fc 29 84 24 e0 00 	vmovaps YMMWORD PTR [rsp+0xe0],ymm0
    1334:	00 00 
  __m256 oc_z = _mm256_sub_ps(rays.orig_z, sphere_center_z);
    1336:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    133b:	c5 fc 28 80 a0 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0xa0]
    1342:	00 
    1343:	c5 fc 29 84 24 40 03 	vmovaps YMMWORD PTR [rsp+0x340],ymm0
    134a:	00 00 
    134c:	c5 fc 28 84 24 a0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xa0]
    1353:	00 00 
    1355:	c5 fc 29 84 24 60 03 	vmovaps YMMWORD PTR [rsp+0x360],ymm0
    135c:	00 00 
    135e:	c5 fc 28 84 24 40 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x340]
    1365:	00 00 
    1367:	c5 fc 5c 84 24 60 03 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x360]
    136e:	00 00 
    1370:	c5 fc 29 84 24 00 01 	vmovaps YMMWORD PTR [rsp+0x100],ymm0
    1377:	00 00 

  __m256 a = _mm256_mul_ps(rays.dir_x, rays.dir_x);
    1379:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    137e:	c5 fc 28 00          	vmovaps ymm0,YMMWORD PTR [rax]
    1382:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    1387:	c5 fc 28 08          	vmovaps ymm1,YMMWORD PTR [rax]
    138b:	c5 fc 29 8c 24 00 03 	vmovaps YMMWORD PTR [rsp+0x300],ymm1
    1392:	00 00 
    1394:	c5 fc 29 84 24 20 03 	vmovaps YMMWORD PTR [rsp+0x320],ymm0
    139b:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    139d:	c5 fc 28 84 24 00 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x300]
    13a4:	00 00 
    13a6:	c5 fc 59 84 24 20 03 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x320]
    13ad:	00 00 
    13af:	c5 fc 29 84 24 20 01 	vmovaps YMMWORD PTR [rsp+0x120],ymm0
    13b6:	00 00 
  a = _mm256_fmadd_ps(rays.dir_y, rays.dir_y, a);
    13b8:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    13bd:	c5 fc 28 40 20       	vmovaps ymm0,YMMWORD PTR [rax+0x20]
    13c2:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    13c7:	c5 fc 28 48 20       	vmovaps ymm1,YMMWORD PTR [rax+0x20]
    13cc:	c5 fc 29 8c 24 a0 02 	vmovaps YMMWORD PTR [rsp+0x2a0],ymm1
    13d3:	00 00 
    13d5:	c5 fc 29 84 24 c0 02 	vmovaps YMMWORD PTR [rsp+0x2c0],ymm0
    13dc:	00 00 
    13de:	c5 fc 28 84 24 20 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x120]
    13e5:	00 00 
    13e7:	c5 fc 29 84 24 e0 02 	vmovaps YMMWORD PTR [rsp+0x2e0],ymm0
    13ee:	00 00 

extern __inline __m256
__attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm256_fmadd_ps (__m256 __A, __m256 __B, __m256 __C)
{
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    13f0:	c5 fc 28 8c 24 c0 02 	vmovaps ymm1,YMMWORD PTR [rsp+0x2c0]
    13f7:	00 00 
    13f9:	c5 fc 28 84 24 e0 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x2e0]
    1400:	00 00 
    1402:	c4 e2 75 b8 84 24 a0 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x2a0]
    1409:	02 00 00 
                                             (__v8sf)__C);
    140c:	90                   	nop
    140d:	c5 fc 29 84 24 20 01 	vmovaps YMMWORD PTR [rsp+0x120],ymm0
    1414:	00 00 
  a = _mm256_fmadd_ps(rays.dir_z, rays.dir_z, a);
    1416:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    141b:	c5 fc 28 40 40       	vmovaps ymm0,YMMWORD PTR [rax+0x40]
    1420:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    1425:	c5 fc 28 48 40       	vmovaps ymm1,YMMWORD PTR [rax+0x40]
    142a:	c5 fc 29 8c 24 00 04 	vmovaps YMMWORD PTR [rsp+0x400],ymm1
    1431:	00 00 
    1433:	c5 fc 29 84 24 20 04 	vmovaps YMMWORD PTR [rsp+0x420],ymm0
    143a:	00 00 
    143c:	c5 fc 28 84 24 20 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x120]
    1443:	00 00 
    1445:	c5 fc 29 84 24 40 04 	vmovaps YMMWORD PTR [rsp+0x440],ymm0
    144c:	00 00 
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    144e:	c5 fc 28 8c 24 20 04 	vmovaps ymm1,YMMWORD PTR [rsp+0x420]
    1455:	00 00 
    1457:	c5 fc 28 84 24 40 04 	vmovaps ymm0,YMMWORD PTR [rsp+0x440]
    145e:	00 00 
    1460:	c4 e2 75 b8 84 24 00 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x400]
    1467:	04 00 00 
                                             (__v8sf)__C);
    146a:	90                   	nop
    146b:	c5 fc 29 84 24 20 01 	vmovaps YMMWORD PTR [rsp+0x120],ymm0
    1472:	00 00 

  __m256 b = _mm256_mul_ps(rays.dir_x, oc_x);
    1474:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    1479:	c5 fc 28 00          	vmovaps ymm0,YMMWORD PTR [rax]
    147d:	c5 fc 29 84 24 c0 04 	vmovaps YMMWORD PTR [rsp+0x4c0],ymm0
    1484:	00 00 
    1486:	c5 fc 28 84 24 c0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xc0]
    148d:	00 00 
    148f:	c5 fc 29 84 24 e0 04 	vmovaps YMMWORD PTR [rsp+0x4e0],ymm0
    1496:	00 00 
    1498:	c5 fc 28 84 24 c0 04 	vmovaps ymm0,YMMWORD PTR [rsp+0x4c0]
    149f:	00 00 
    14a1:	c5 fc 59 84 24 e0 04 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x4e0]
    14a8:	00 00 
    14aa:	c5 fc 29 84 24 40 01 	vmovaps YMMWORD PTR [rsp+0x140],ymm0
    14b1:	00 00 
  b = _mm256_fmadd_ps(rays.dir_y, oc_y, b);
    14b3:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    14b8:	c5 fc 28 40 20       	vmovaps ymm0,YMMWORD PTR [rax+0x20]
    14bd:	c5 fc 29 84 24 60 04 	vmovaps YMMWORD PTR [rsp+0x460],ymm0
    14c4:	00 00 
    14c6:	c5 fc 28 84 24 e0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xe0]
    14cd:	00 00 
    14cf:	c5 fc 29 84 24 80 04 	vmovaps YMMWORD PTR [rsp+0x480],ymm0
    14d6:	00 00 
    14d8:	c5 fc 28 84 24 40 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x140]
    14df:	00 00 
    14e1:	c5 fc 29 84 24 a0 04 	vmovaps YMMWORD PTR [rsp+0x4a0],ymm0
    14e8:	00 00 
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    14ea:	c5 fc 28 8c 24 80 04 	vmovaps ymm1,YMMWORD PTR [rsp+0x480]
    14f1:	00 00 
    14f3:	c5 fc 28 84 24 a0 04 	vmovaps ymm0,YMMWORD PTR [rsp+0x4a0]
    14fa:	00 00 
    14fc:	c4 e2 75 b8 84 24 60 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x460]
    1503:	04 00 00 
                                             (__v8sf)__C);
    1506:	90                   	nop
    1507:	c5 fc 29 84 24 40 01 	vmovaps YMMWORD PTR [rsp+0x140],ymm0
    150e:	00 00 
  b = _mm256_fmadd_ps(rays.dir_z, oc_z, b);
    1510:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    1515:	c5 fc 28 40 40       	vmovaps ymm0,YMMWORD PTR [rax+0x40]
    151a:	c5 fc 29 84 24 00 05 	vmovaps YMMWORD PTR [rsp+0x500],ymm0
    1521:	00 00 
    1523:	c5 fc 28 84 24 00 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x100]
    152a:	00 00 
    152c:	c5 fc 29 84 24 20 05 	vmovaps YMMWORD PTR [rsp+0x520],ymm0
    1533:	00 00 
    1535:	c5 fc 28 84 24 40 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x140]
    153c:	00 00 
    153e:	c5 fc 29 84 24 40 05 	vmovaps YMMWORD PTR [rsp+0x540],ymm0
    1545:	00 00 
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    1547:	c5 fc 28 8c 24 20 05 	vmovaps ymm1,YMMWORD PTR [rsp+0x520]
    154e:	00 00 
    1550:	c5 fc 28 84 24 40 05 	vmovaps ymm0,YMMWORD PTR [rsp+0x540]
    1557:	00 00 
    1559:	c4 e2 75 b8 84 24 00 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x500]
    1560:	05 00 00 
                                             (__v8sf)__C);
    1563:	90                   	nop
    1564:	c5 fc 29 84 24 40 01 	vmovaps YMMWORD PTR [rsp+0x140],ymm0
    156b:	00 00 
    156d:	c5 fc 28 84 24 c0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xc0]
    1574:	00 00 
    1576:	c5 fc 29 84 24 c0 05 	vmovaps YMMWORD PTR [rsp+0x5c0],ymm0
    157d:	00 00 
    157f:	c5 fc 28 84 24 c0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xc0]
    1586:	00 00 
    1588:	c5 fc 29 84 24 e0 05 	vmovaps YMMWORD PTR [rsp+0x5e0],ymm0
    158f:	00 00 
    1591:	c5 fc 28 84 24 c0 05 	vmovaps ymm0,YMMWORD PTR [rsp+0x5c0]
    1598:	00 00 
    159a:	c5 fc 59 84 24 e0 05 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x5e0]
    15a1:	00 00 

  __m256 c = _mm256_mul_ps(oc_x, oc_x);
    15a3:	c5 fc 29 84 24 60 01 	vmovaps YMMWORD PTR [rsp+0x160],ymm0
    15aa:	00 00 
    15ac:	c5 fc 28 84 24 e0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xe0]
    15b3:	00 00 
    15b5:	c5 fc 29 84 24 60 05 	vmovaps YMMWORD PTR [rsp+0x560],ymm0
    15bc:	00 00 
    15be:	c5 fc 28 84 24 e0 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xe0]
    15c5:	00 00 
    15c7:	c5 fc 29 84 24 80 05 	vmovaps YMMWORD PTR [rsp+0x580],ymm0
    15ce:	00 00 
    15d0:	c5 fc 28 84 24 60 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x160]
    15d7:	00 00 
    15d9:	c5 fc 29 84 24 a0 05 	vmovaps YMMWORD PTR [rsp+0x5a0],ymm0
    15e0:	00 00 
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    15e2:	c5 fc 28 8c 24 80 05 	vmovaps ymm1,YMMWORD PTR [rsp+0x580]
    15e9:	00 00 
    15eb:	c5 fc 28 84 24 a0 05 	vmovaps ymm0,YMMWORD PTR [rsp+0x5a0]
    15f2:	00 00 
    15f4:	c4 e2 75 b8 84 24 60 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x560]
    15fb:	05 00 00 
                                             (__v8sf)__C);
    15fe:	90                   	nop
  c = _mm256_fmadd_ps(oc_y, oc_y, c);
    15ff:	c5 fc 29 84 24 60 01 	vmovaps YMMWORD PTR [rsp+0x160],ymm0
    1606:	00 00 
    1608:	c5 fc 28 84 24 00 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x100]
    160f:	00 00 
    1611:	c5 fc 29 84 24 00 06 	vmovaps YMMWORD PTR [rsp+0x600],ymm0
    1618:	00 00 
    161a:	c5 fc 28 84 24 00 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x100]
    1621:	00 00 
    1623:	c5 fc 29 84 24 20 06 	vmovaps YMMWORD PTR [rsp+0x620],ymm0
    162a:	00 00 
    162c:	c5 fc 28 84 24 60 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x160]
    1633:	00 00 
    1635:	c5 fc 29 84 24 40 06 	vmovaps YMMWORD PTR [rsp+0x640],ymm0
    163c:	00 00 
  return (__m256)__builtin_ia32_vfmaddps256 ((__v8sf)__A, (__v8sf)__B,
    163e:	c5 fc 28 8c 24 20 06 	vmovaps ymm1,YMMWORD PTR [rsp+0x620]
    1645:	00 00 
    1647:	c5 fc 28 84 24 40 06 	vmovaps ymm0,YMMWORD PTR [rsp+0x640]
    164e:	00 00 
    1650:	c4 e2 75 b8 84 24 00 	vfmadd231ps ymm0,ymm1,YMMWORD PTR [rsp+0x600]
    1657:	06 00 00 
                                             (__v8sf)__C);
    165a:	90                   	nop
  c = _mm256_fmadd_ps(oc_z, oc_z, c);
    165b:	c5 fc 29 84 24 60 01 	vmovaps YMMWORD PTR [rsp+0x160],ymm0
    1662:	00 00 

  float rad_2 = sphere.radius * sphere.radius;
    1664:	c5 fa 10 4c 24 20    	vmovss xmm1,DWORD PTR [rsp+0x20]
    166a:	c5 fa 10 44 24 20    	vmovss xmm0,DWORD PTR [rsp+0x20]
    1670:	c5 f2 59 c0          	vmulss xmm0,xmm1,xmm0
    1674:	c5 fa 11 44 24 30    	vmovss DWORD PTR [rsp+0x30],xmm0
    167a:	48 8d 44 24 30       	lea    rax,[rsp+0x30]
    167f:	48 89 44 24 50       	mov    QWORD PTR [rsp+0x50],rax
  return (__m256) __builtin_ia32_vbroadcastss256 (__X);
    1684:	48 8b 44 24 50       	mov    rax,QWORD PTR [rsp+0x50]
    1689:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    168e:	90                   	nop
  __m256 rad_2_vec = _mm256_broadcast_ss(&rad_2);
    168f:	c5 fc 29 84 24 80 01 	vmovaps YMMWORD PTR [rsp+0x180],ymm0
    1696:	00 00 
    1698:	c5 fc 28 84 24 60 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x160]
    169f:	00 00 
    16a1:	c5 fc 29 84 24 00 07 	vmovaps YMMWORD PTR [rsp+0x700],ymm0
    16a8:	00 00 
    16aa:	c5 fc 28 84 24 80 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x180]
    16b1:	00 00 
    16b3:	c5 fc 29 84 24 20 07 	vmovaps YMMWORD PTR [rsp+0x720],ymm0
    16ba:	00 00 
  return (__m256) ((__v8sf)__A - (__v8sf)__B);
    16bc:	c5 fc 28 84 24 00 07 	vmovaps ymm0,YMMWORD PTR [rsp+0x700]
    16c3:	00 00 
    16c5:	c5 fc 5c 84 24 20 07 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x720]
    16cc:	00 00 

  c = _mm256_sub_ps(c, rad_2_vec);
    16ce:	c5 fc 29 84 24 60 01 	vmovaps YMMWORD PTR [rsp+0x160],ymm0
    16d5:	00 00 
    16d7:	c5 fc 28 84 24 20 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x120]
    16de:	00 00 
    16e0:	c5 fc 29 84 24 c0 06 	vmovaps YMMWORD PTR [rsp+0x6c0],ymm0
    16e7:	00 00 
    16e9:	c5 fc 28 84 24 60 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x160]
    16f0:	00 00 
    16f2:	c5 fc 29 84 24 e0 06 	vmovaps YMMWORD PTR [rsp+0x6e0],ymm0
    16f9:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    16fb:	c5 fc 28 84 24 c0 06 	vmovaps ymm0,YMMWORD PTR [rsp+0x6c0]
    1702:	00 00 
    1704:	c5 fc 59 84 24 e0 06 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x6e0]
    170b:	00 00 

  __m256 discrim = _mm256_mul_ps(a, c);
    170d:	c5 fc 29 84 24 a0 01 	vmovaps YMMWORD PTR [rsp+0x1a0],ymm0
    1714:	00 00 
    1716:	c5 fc 28 84 24 40 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x140]
    171d:	00 00 
    171f:	c5 fc 29 84 24 60 06 	vmovaps YMMWORD PTR [rsp+0x660],ymm0
    1726:	00 00 
    1728:	c5 fc 28 84 24 40 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x140]
    172f:	00 00 
    1731:	c5 fc 29 84 24 80 06 	vmovaps YMMWORD PTR [rsp+0x680],ymm0
    1738:	00 00 
    173a:	c5 fc 28 84 24 a0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1a0]
    1741:	00 00 
    1743:	c5 fc 29 84 24 a0 06 	vmovaps YMMWORD PTR [rsp+0x6a0],ymm0
    174a:	00 00 

extern __inline __m256
__attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm256_fmsub_ps (__m256 __A, __m256 __B, __m256 __C)
{
  return (__m256)__builtin_ia32_vfmsubps256 ((__v8sf)__A, (__v8sf)__B,
    174c:	c5 fc 28 8c 24 80 06 	vmovaps ymm1,YMMWORD PTR [rsp+0x680]
    1753:	00 00 
    1755:	c5 fc 28 84 24 a0 06 	vmovaps ymm0,YMMWORD PTR [rsp+0x6a0]
    175c:	00 00 
    175e:	c4 e2 75 ba 84 24 60 	vfmsub231ps ymm0,ymm1,YMMWORD PTR [rsp+0x660]
    1765:	06 00 00 
                                             (__v8sf)__C);
    1768:	90                   	nop
  discrim = _mm256_fmsub_ps(b, b, discrim);
    1769:	c5 fc 29 84 24 a0 01 	vmovaps YMMWORD PTR [rsp+0x1a0],ymm0
    1770:	00 00 

extern __inline __m256 __attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm256_setzero_ps (void)
{
  return __extension__ (__m256){ 0.0, 0.0, 0.0, 0.0,
				 0.0, 0.0, 0.0, 0.0 };
    1772:	c5 f8 57 c0          	vxorps xmm0,xmm0,xmm0

  __m256 zeros = _mm256_setzero_ps();
    1776:	c5 fc 29 84 24 c0 01 	vmovaps YMMWORD PTR [rsp+0x1c0],ymm0
    177d:	00 00 
  __m256 res = _mm256_cmp_ps(discrim, zeros, CMPNLTPS);
    177f:	c5 fc 28 84 24 a0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1a0]
    1786:	00 00 
    1788:	c5 fc 28 8c 24 c0 01 	vmovaps ymm1,YMMWORD PTR [rsp+0x1c0]
    178f:	00 00 
    1791:	c5 fc c2 c1 05       	vcmpnltps ymm0,ymm0,ymm1
    1796:	c5 fc 29 84 24 e0 01 	vmovaps YMMWORD PTR [rsp+0x1e0],ymm0
    179d:	00 00 
    179f:	c5 fc 28 84 24 e0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1e0]
    17a6:	00 00 
    17a8:	c5 fc 29 84 24 40 07 	vmovaps YMMWORD PTR [rsp+0x740],ymm0
    17af:	00 00 
    17b1:	c5 fc 28 84 24 e0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1e0]
    17b8:	00 00 
    17ba:	c5 fc 29 84 24 60 07 	vmovaps YMMWORD PTR [rsp+0x760],ymm0
    17c1:	00 00 
  return __builtin_ia32_vtestzps256 ((__v8sf)__M, (__v8sf)__V);
    17c3:	b8 00 00 00 00       	mov    eax,0x0
    17c8:	c5 fc 28 84 24 40 07 	vmovaps ymm0,YMMWORD PTR [rsp+0x740]
    17cf:	00 00 
    17d1:	c4 e2 7d 0e 84 24 60 	vtestps ymm0,YMMWORD PTR [rsp+0x760]
    17d8:	07 00 00 
    17db:	0f 94 c0             	sete   al
    17de:	90                   	nop
  int no_hit = _mm256_testz_ps(res, res);
    17df:	89 44 24 34          	mov    DWORD PTR [rsp+0x34],eax

  if (no_hit) {
    17e3:	83 7c 24 34 00       	cmp    DWORD PTR [rsp+0x34],0x0
    17e8:	74 0e                	je     17f8 <_Z10sphere_hitRK10RayClusterf6Sphere+0x5e2>
    return zeros;
    17ea:	c5 fc 28 84 24 c0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1c0]
    17f1:	00 00 
    17f3:	e9 8e 01 00 00       	jmp    1986 <_Z10sphere_hitRK10RayClusterf6Sphere+0x770>
    17f8:	c5 fc 28 84 24 a0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1a0]
    17ff:	00 00 
    1801:	c5 fc 29 84 24 80 07 	vmovaps YMMWORD PTR [rsp+0x780],ymm0
    1808:	00 00 
  return (__m256) __builtin_ia32_rsqrtps256 ((__v8sf)__A);
    180a:	c5 fc 52 84 24 80 07 	vrsqrtps ymm0,YMMWORD PTR [rsp+0x780]
    1811:	00 00 
    1813:	90                   	nop
  }

  __m256 recip_sqrt_d = _mm256_rsqrt_ps(discrim);
    1814:	c5 fc 29 84 24 00 02 	vmovaps YMMWORD PTR [rsp+0x200],ymm0
    181b:	00 00 
    181d:	c5 fc 28 84 24 00 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x200]
    1824:	00 00 
    1826:	c5 fc 29 84 24 c0 07 	vmovaps YMMWORD PTR [rsp+0x7c0],ymm0
    182d:	00 00 
    182f:	c5 fc 28 84 24 a0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1a0]
    1836:	00 00 
    1838:	c5 fc 29 84 24 e0 07 	vmovaps YMMWORD PTR [rsp+0x7e0],ymm0
    183f:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    1841:	c5 fc 28 84 24 c0 07 	vmovaps ymm0,YMMWORD PTR [rsp+0x7c0]
    1848:	00 00 
    184a:	c5 fc 59 84 24 e0 07 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x7e0]
    1851:	00 00 
  __m256 sqrt_d = _mm256_mul_ps(recip_sqrt_d, discrim);
    1853:	c5 fc 29 84 24 20 02 	vmovaps YMMWORD PTR [rsp+0x220],ymm0
    185a:	00 00 
    185c:	c5 fc 28 84 24 20 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x120]
    1863:	00 00 
    1865:	c5 fc 29 84 24 a0 07 	vmovaps YMMWORD PTR [rsp+0x7a0],ymm0
    186c:	00 00 
  return (__m256) __builtin_ia32_rcpps256 ((__v8sf)__A);
    186e:	c5 fc 53 84 24 a0 07 	vrcpps ymm0,YMMWORD PTR [rsp+0x7a0]
    1875:	00 00 
    1877:	90                   	nop
  __m256 recip_a = _mm256_rcp_ps(a);
    1878:	c5 fc 29 84 24 40 02 	vmovaps YMMWORD PTR [rsp+0x240],ymm0
    187f:	00 00 
    1881:	c5 fc 28 84 24 40 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x140]
    1888:	00 00 
    188a:	c5 fc 29 84 24 40 08 	vmovaps YMMWORD PTR [rsp+0x840],ymm0
    1891:	00 00 
    1893:	c5 fc 28 84 24 20 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x220]
    189a:	00 00 
    189c:	c5 fc 29 84 24 60 08 	vmovaps YMMWORD PTR [rsp+0x860],ymm0
    18a3:	00 00 
  return (__m256) ((__v8sf)__A - (__v8sf)__B);
    18a5:	c5 fc 28 84 24 40 08 	vmovaps ymm0,YMMWORD PTR [rsp+0x840]
    18ac:	00 00 
    18ae:	c5 fc 5c 84 24 60 08 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x860]
    18b5:	00 00 

  __m256 root = _mm256_sub_ps(b, sqrt_d);
    18b7:	c5 fc 29 84 24 60 02 	vmovaps YMMWORD PTR [rsp+0x260],ymm0
    18be:	00 00 
    18c0:	c5 fc 28 84 24 60 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x260]
    18c7:	00 00 
    18c9:	c5 fc 29 84 24 00 08 	vmovaps YMMWORD PTR [rsp+0x800],ymm0
    18d0:	00 00 
    18d2:	c5 fc 28 84 24 40 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x240]
    18d9:	00 00 
    18db:	c5 fc 29 84 24 20 08 	vmovaps YMMWORD PTR [rsp+0x820],ymm0
    18e2:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    18e4:	c5 fc 28 84 24 00 08 	vmovaps ymm0,YMMWORD PTR [rsp+0x800]
    18eb:	00 00 
    18ed:	c5 fc 59 84 24 20 08 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x820]
    18f4:	00 00 
  root = _mm256_mul_ps(root, recip_a);
    18f6:	c5 fc 29 84 24 60 02 	vmovaps YMMWORD PTR [rsp+0x260],ymm0
    18fd:	00 00 
    18ff:	48 8d 44 24 24       	lea    rax,[rsp+0x24]
    1904:	48 89 44 24 58       	mov    QWORD PTR [rsp+0x58],rax
  return (__m256) __builtin_ia32_vbroadcastss256 (__X);
    1909:	48 8b 44 24 58       	mov    rax,QWORD PTR [rsp+0x58]
    190e:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    1913:	90                   	nop

  // allow through roots within the max t value
  __m256 t_max_vec = _mm256_broadcast_ss(&t_max);
    1914:	c5 fc 29 84 24 80 02 	vmovaps YMMWORD PTR [rsp+0x280],ymm0
    191b:	00 00 
  res = _mm256_cmp_ps(root, t_max_vec, CMPLTPS);
    191d:	c5 fc 28 84 24 60 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x260]
    1924:	00 00 
    1926:	c5 fc 28 8c 24 80 02 	vmovaps ymm1,YMMWORD PTR [rsp+0x280]
    192d:	00 00 
    192f:	c5 fc c2 c1 01       	vcmpltps ymm0,ymm0,ymm1
    1934:	c5 fc 29 84 24 e0 01 	vmovaps YMMWORD PTR [rsp+0x1e0],ymm0
    193b:	00 00 
    193d:	c5 fc 28 84 24 60 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x260]
    1944:	00 00 
    1946:	c5 fc 29 84 24 80 08 	vmovaps YMMWORD PTR [rsp+0x880],ymm0
    194d:	00 00 
    194f:	c5 fc 28 84 24 e0 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1e0]
    1956:	00 00 
    1958:	c5 fc 29 84 24 a0 08 	vmovaps YMMWORD PTR [rsp+0x8a0],ymm0
    195f:	00 00 
  return (__m256) __builtin_ia32_andps256 ((__v8sf)__A, (__v8sf)__B);
    1961:	c5 fc 28 84 24 80 08 	vmovaps ymm0,YMMWORD PTR [rsp+0x880]
    1968:	00 00 
    196a:	c5 fc 54 84 24 a0 08 	vandps ymm0,ymm0,YMMWORD PTR [rsp+0x8a0]
    1971:	00 00 
    1973:	90                   	nop
  root = _mm256_and_ps(root, res);
    1974:	c5 fc 29 84 24 60 02 	vmovaps YMMWORD PTR [rsp+0x260],ymm0
    197b:	00 00 

  return root;
    197d:	c5 fc 28 84 24 60 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x260]
    1984:	00 00 
}
    1986:	48 8b 84 24 d8 08 00 	mov    rax,QWORD PTR [rsp+0x8d8]
    198d:	00 
    198e:	64 48 2b 04 25 28 00 	sub    rax,QWORD PTR fs:0x28
    1995:	00 00 
    1997:	74 05                	je     199e <_Z10sphere_hitRK10RayClusterf6Sphere+0x788>
    1999:	e8 c2 f6 ff ff       	call   1060 <__stack_chk_fail@plt>
    199e:	c9                   	leave  
    199f:	c3                   	ret    

00000000000019a0 <_Z17create_hit_recordRK10RayClusterDv8_f6Sphere>:

HitRecord create_hit_record(const RayCluster &rays, __m256 t_vals,
                            Sphere sphere) {
    19a0:	f3 0f 1e fa          	endbr64 
    19a4:	55                   	push   rbp
    19a5:	48 89 e5             	mov    rbp,rsp
    19a8:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
    19ac:	48 81 ec e8 03 00 00 	sub    rsp,0x3e8
    19b3:	48 89 7c 24 c0       	mov    QWORD PTR [rsp-0x40],rdi
    19b8:	48 89 74 24 b8       	mov    QWORD PTR [rsp-0x48],rsi
    19bd:	c5 fc 29 44 24 88    	vmovaps YMMWORD PTR [rsp-0x78],ymm0
    19c3:	c5 f9 6f c2          	vmovdqa xmm0,xmm2
    19c7:	c5 f9 d6 4c 24 a8    	vmovq  QWORD PTR [rsp-0x58],xmm1
    19cd:	c5 f9 d6 44 24 b0    	vmovq  QWORD PTR [rsp-0x50],xmm0
  HitRecord hit_rec;
  hit_rec.t = t_vals;
    19d3:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    19d8:	c5 fc 28 44 24 88    	vmovaps ymm0,YMMWORD PTR [rsp-0x78]
    19de:	c5 fc 29 00          	vmovaps YMMWORD PTR [rax],ymm0
  __m256 dir_xt = _mm256_mul_ps(rays.dir_x, t_vals);
    19e2:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    19e7:	c5 fc 28 00          	vmovaps ymm0,YMMWORD PTR [rax]
    19eb:	c5 fc 29 84 24 a8 03 	vmovaps YMMWORD PTR [rsp+0x3a8],ymm0
    19f2:	00 00 
    19f4:	c5 fc 28 44 24 88    	vmovaps ymm0,YMMWORD PTR [rsp-0x78]
    19fa:	c5 fc 29 84 24 c8 03 	vmovaps YMMWORD PTR [rsp+0x3c8],ymm0
    1a01:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    1a03:	c5 fc 28 84 24 a8 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x3a8]
    1a0a:	00 00 
    1a0c:	c5 fc 59 84 24 c8 03 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x3c8]
    1a13:	00 00 
    1a15:	c5 fc 29 44 24 e8    	vmovaps YMMWORD PTR [rsp-0x18],ymm0
  __m256 dir_yt = _mm256_mul_ps(rays.dir_y, t_vals);
    1a1b:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    1a20:	c5 fc 28 40 20       	vmovaps ymm0,YMMWORD PTR [rax+0x20]
    1a25:	c5 fc 29 84 24 68 03 	vmovaps YMMWORD PTR [rsp+0x368],ymm0
    1a2c:	00 00 
    1a2e:	c5 fc 28 44 24 88    	vmovaps ymm0,YMMWORD PTR [rsp-0x78]
    1a34:	c5 fc 29 84 24 88 03 	vmovaps YMMWORD PTR [rsp+0x388],ymm0
    1a3b:	00 00 
    1a3d:	c5 fc 28 84 24 68 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x368]
    1a44:	00 00 
    1a46:	c5 fc 59 84 24 88 03 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x388]
    1a4d:	00 00 
    1a4f:	c5 fc 29 44 24 08    	vmovaps YMMWORD PTR [rsp+0x8],ymm0
  __m256 dir_zt = _mm256_mul_ps(rays.dir_z, t_vals);
    1a55:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    1a5a:	c5 fc 28 40 40       	vmovaps ymm0,YMMWORD PTR [rax+0x40]
    1a5f:	c5 fc 29 84 24 28 03 	vmovaps YMMWORD PTR [rsp+0x328],ymm0
    1a66:	00 00 
    1a68:	c5 fc 28 44 24 88    	vmovaps ymm0,YMMWORD PTR [rsp-0x78]
    1a6e:	c5 fc 29 84 24 48 03 	vmovaps YMMWORD PTR [rsp+0x348],ymm0
    1a75:	00 00 
    1a77:	c5 fc 28 84 24 28 03 	vmovaps ymm0,YMMWORD PTR [rsp+0x328]
    1a7e:	00 00 
    1a80:	c5 fc 59 84 24 48 03 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x348]
    1a87:	00 00 
    1a89:	c5 fc 29 44 24 28    	vmovaps YMMWORD PTR [rsp+0x28],ymm0

  hit_rec.orig_x = _mm256_add_ps(rays.orig_x, dir_xt);
    1a8f:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    1a94:	c5 fc 28 40 60       	vmovaps ymm0,YMMWORD PTR [rax+0x60]
    1a99:	c5 fc 29 84 24 e8 02 	vmovaps YMMWORD PTR [rsp+0x2e8],ymm0
    1aa0:	00 00 
    1aa2:	c5 fc 28 44 24 e8    	vmovaps ymm0,YMMWORD PTR [rsp-0x18]
    1aa8:	c5 fc 29 84 24 08 03 	vmovaps YMMWORD PTR [rsp+0x308],ymm0
    1aaf:	00 00 
  return (__m256) ((__v8sf)__A + (__v8sf)__B);
    1ab1:	c5 fc 28 84 24 e8 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x2e8]
    1ab8:	00 00 
    1aba:	c5 fc 58 84 24 08 03 	vaddps ymm0,ymm0,YMMWORD PTR [rsp+0x308]
    1ac1:	00 00 
    1ac3:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1ac8:	c5 fc 29 40 20       	vmovaps YMMWORD PTR [rax+0x20],ymm0
  hit_rec.orig_y = _mm256_add_ps(rays.orig_y, dir_yt);
    1acd:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    1ad2:	c5 fc 28 80 80 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0x80]
    1ad9:	00 
    1ada:	c5 fc 29 84 24 a8 02 	vmovaps YMMWORD PTR [rsp+0x2a8],ymm0
    1ae1:	00 00 
    1ae3:	c5 fc 28 44 24 08    	vmovaps ymm0,YMMWORD PTR [rsp+0x8]
    1ae9:	c5 fc 29 84 24 c8 02 	vmovaps YMMWORD PTR [rsp+0x2c8],ymm0
    1af0:	00 00 
    1af2:	c5 fc 28 84 24 a8 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x2a8]
    1af9:	00 00 
    1afb:	c5 fc 58 84 24 c8 02 	vaddps ymm0,ymm0,YMMWORD PTR [rsp+0x2c8]
    1b02:	00 00 
    1b04:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1b09:	c5 fc 29 40 40       	vmovaps YMMWORD PTR [rax+0x40],ymm0
  hit_rec.orig_z = _mm256_add_ps(rays.orig_z, dir_zt);
    1b0e:	48 8b 44 24 b8       	mov    rax,QWORD PTR [rsp-0x48]
    1b13:	c5 fc 28 80 a0 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0xa0]
    1b1a:	00 
    1b1b:	c5 fc 29 84 24 68 02 	vmovaps YMMWORD PTR [rsp+0x268],ymm0
    1b22:	00 00 
    1b24:	c5 fc 28 44 24 28    	vmovaps ymm0,YMMWORD PTR [rsp+0x28]
    1b2a:	c5 fc 29 84 24 88 02 	vmovaps YMMWORD PTR [rsp+0x288],ymm0
    1b31:	00 00 
    1b33:	c5 fc 28 84 24 68 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x268]
    1b3a:	00 00 
    1b3c:	c5 fc 58 84 24 88 02 	vaddps ymm0,ymm0,YMMWORD PTR [rsp+0x288]
    1b43:	00 00 
    1b45:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1b4a:	c5 fc 29 40 60       	vmovaps YMMWORD PTR [rax+0x60],ymm0
    1b4f:	48 8d 44 24 a8       	lea    rax,[rsp-0x58]
    1b54:	48 89 44 24 e0       	mov    QWORD PTR [rsp-0x20],rax
  return (__m256) __builtin_ia32_vbroadcastss256 (__X);
    1b59:	48 8b 44 24 e0       	mov    rax,QWORD PTR [rsp-0x20]
    1b5e:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    1b63:	90                   	nop

  __m256 sphere_center_x = _mm256_broadcast_ss(&sphere.center.x);
    1b64:	c5 fc 29 44 24 48    	vmovaps YMMWORD PTR [rsp+0x48],ymm0
    1b6a:	48 8d 44 24 a8       	lea    rax,[rsp-0x58]
    1b6f:	48 83 c0 04          	add    rax,0x4
    1b73:	48 89 44 24 d8       	mov    QWORD PTR [rsp-0x28],rax
    1b78:	48 8b 44 24 d8       	mov    rax,QWORD PTR [rsp-0x28]
    1b7d:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    1b82:	90                   	nop
  __m256 sphere_center_y = _mm256_broadcast_ss(&sphere.center.y);
    1b83:	c5 fc 29 44 24 68    	vmovaps YMMWORD PTR [rsp+0x68],ymm0
    1b89:	48 8d 44 24 a8       	lea    rax,[rsp-0x58]
    1b8e:	48 83 c0 08          	add    rax,0x8
    1b92:	48 89 44 24 d0       	mov    QWORD PTR [rsp-0x30],rax
    1b97:	48 8b 44 24 d0       	mov    rax,QWORD PTR [rsp-0x30]
    1b9c:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    1ba1:	90                   	nop
  __m256 sphere_center_z = _mm256_broadcast_ss(&sphere.center.z);
    1ba2:	c5 fc 29 84 24 88 00 	vmovaps YMMWORD PTR [rsp+0x88],ymm0
    1ba9:	00 00 

  hit_rec.norm_x = _mm256_sub_ps(hit_rec.orig_x, sphere_center_x);
    1bab:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1bb0:	c5 fc 28 40 20       	vmovaps ymm0,YMMWORD PTR [rax+0x20]
    1bb5:	c5 fc 29 84 24 28 02 	vmovaps YMMWORD PTR [rsp+0x228],ymm0
    1bbc:	00 00 
    1bbe:	c5 fc 28 44 24 48    	vmovaps ymm0,YMMWORD PTR [rsp+0x48]
    1bc4:	c5 fc 29 84 24 48 02 	vmovaps YMMWORD PTR [rsp+0x248],ymm0
    1bcb:	00 00 
  return (__m256) ((__v8sf)__A - (__v8sf)__B);
    1bcd:	c5 fc 28 84 24 28 02 	vmovaps ymm0,YMMWORD PTR [rsp+0x228]
    1bd4:	00 00 
    1bd6:	c5 fc 5c 84 24 48 02 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x248]
    1bdd:	00 00 
    1bdf:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1be4:	c5 fc 29 80 80 00 00 	vmovaps YMMWORD PTR [rax+0x80],ymm0
    1beb:	00 

  hit_rec.norm_y = _mm256_sub_ps(hit_rec.orig_y, sphere_center_y);
    1bec:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1bf1:	c5 fc 28 40 40       	vmovaps ymm0,YMMWORD PTR [rax+0x40]
    1bf6:	c5 fc 29 84 24 e8 01 	vmovaps YMMWORD PTR [rsp+0x1e8],ymm0
    1bfd:	00 00 
    1bff:	c5 fc 28 44 24 68    	vmovaps ymm0,YMMWORD PTR [rsp+0x68]
    1c05:	c5 fc 29 84 24 08 02 	vmovaps YMMWORD PTR [rsp+0x208],ymm0
    1c0c:	00 00 
    1c0e:	c5 fc 28 84 24 e8 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1e8]
    1c15:	00 00 
    1c17:	c5 fc 5c 84 24 08 02 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x208]
    1c1e:	00 00 
    1c20:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1c25:	c5 fc 29 80 a0 00 00 	vmovaps YMMWORD PTR [rax+0xa0],ymm0
    1c2c:	00 
  hit_rec.norm_z = _mm256_sub_ps(hit_rec.orig_z, sphere_center_z);
    1c2d:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1c32:	c5 fc 28 40 60       	vmovaps ymm0,YMMWORD PTR [rax+0x60]
    1c37:	c5 fc 29 84 24 a8 01 	vmovaps YMMWORD PTR [rsp+0x1a8],ymm0
    1c3e:	00 00 
    1c40:	c5 fc 28 84 24 88 00 	vmovaps ymm0,YMMWORD PTR [rsp+0x88]
    1c47:	00 00 
    1c49:	c5 fc 29 84 24 c8 01 	vmovaps YMMWORD PTR [rsp+0x1c8],ymm0
    1c50:	00 00 
    1c52:	c5 fc 28 84 24 a8 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x1a8]
    1c59:	00 00 
    1c5b:	c5 fc 5c 84 24 c8 01 	vsubps ymm0,ymm0,YMMWORD PTR [rsp+0x1c8]
    1c62:	00 00 
    1c64:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1c69:	c5 fc 29 80 c0 00 00 	vmovaps YMMWORD PTR [rax+0xc0],ymm0
    1c70:	00 
    1c71:	48 8d 44 24 a8       	lea    rax,[rsp-0x58]
    1c76:	48 83 c0 0c          	add    rax,0xc
    1c7a:	48 89 44 24 c8       	mov    QWORD PTR [rsp-0x38],rax
  return (__m256) __builtin_ia32_vbroadcastss256 (__X);
    1c7f:	48 8b 44 24 c8       	mov    rax,QWORD PTR [rsp-0x38]
    1c84:	c4 e2 7d 18 00       	vbroadcastss ymm0,DWORD PTR [rax]
    1c89:	90                   	nop

  __m256 recip_radius = _mm256_broadcast_ss(&sphere.radius);
    1c8a:	c5 fc 29 84 24 a8 00 	vmovaps YMMWORD PTR [rsp+0xa8],ymm0
    1c91:	00 00 
    1c93:	c5 fc 28 84 24 a8 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xa8]
    1c9a:	00 00 
    1c9c:	c5 fc 29 84 24 88 01 	vmovaps YMMWORD PTR [rsp+0x188],ymm0
    1ca3:	00 00 
  return (__m256) __builtin_ia32_rcpps256 ((__v8sf)__A);
    1ca5:	c5 fc 53 84 24 88 01 	vrcpps ymm0,YMMWORD PTR [rsp+0x188]
    1cac:	00 00 
    1cae:	90                   	nop
  recip_radius = _mm256_rcp_ps(recip_radius);
    1caf:	c5 fc 29 84 24 a8 00 	vmovaps YMMWORD PTR [rsp+0xa8],ymm0
    1cb6:	00 00 

  hit_rec.norm_x = _mm256_mul_ps(hit_rec.norm_x, recip_radius);
    1cb8:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1cbd:	c5 fc 28 80 80 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0x80]
    1cc4:	00 
    1cc5:	c5 fc 29 84 24 48 01 	vmovaps YMMWORD PTR [rsp+0x148],ymm0
    1ccc:	00 00 
    1cce:	c5 fc 28 84 24 a8 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xa8]
    1cd5:	00 00 
    1cd7:	c5 fc 29 84 24 68 01 	vmovaps YMMWORD PTR [rsp+0x168],ymm0
    1cde:	00 00 
  return (__m256) ((__v8sf)__A * (__v8sf)__B);
    1ce0:	c5 fc 28 84 24 48 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x148]
    1ce7:	00 00 
    1ce9:	c5 fc 59 84 24 68 01 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x168]
    1cf0:	00 00 
    1cf2:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1cf7:	c5 fc 29 80 80 00 00 	vmovaps YMMWORD PTR [rax+0x80],ymm0
    1cfe:	00 
  hit_rec.norm_y = _mm256_mul_ps(hit_rec.norm_y, recip_radius);
    1cff:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1d04:	c5 fc 28 80 a0 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0xa0]
    1d0b:	00 
    1d0c:	c5 fc 29 84 24 08 01 	vmovaps YMMWORD PTR [rsp+0x108],ymm0
    1d13:	00 00 
    1d15:	c5 fc 28 84 24 a8 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xa8]
    1d1c:	00 00 
    1d1e:	c5 fc 29 84 24 28 01 	vmovaps YMMWORD PTR [rsp+0x128],ymm0
    1d25:	00 00 
    1d27:	c5 fc 28 84 24 08 01 	vmovaps ymm0,YMMWORD PTR [rsp+0x108]
    1d2e:	00 00 
    1d30:	c5 fc 59 84 24 28 01 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0x128]
    1d37:	00 00 
    1d39:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1d3e:	c5 fc 29 80 a0 00 00 	vmovaps YMMWORD PTR [rax+0xa0],ymm0
    1d45:	00 
  hit_rec.norm_z = _mm256_mul_ps(hit_rec.norm_z, recip_radius);
    1d46:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1d4b:	c5 fc 28 80 c0 00 00 	vmovaps ymm0,YMMWORD PTR [rax+0xc0]
    1d52:	00 
    1d53:	c5 fc 29 84 24 c8 00 	vmovaps YMMWORD PTR [rsp+0xc8],ymm0
    1d5a:	00 00 
    1d5c:	c5 fc 28 84 24 a8 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xa8]
    1d63:	00 00 
    1d65:	c5 fc 29 84 24 e8 00 	vmovaps YMMWORD PTR [rsp+0xe8],ymm0
    1d6c:	00 00 
    1d6e:	c5 fc 28 84 24 c8 00 	vmovaps ymm0,YMMWORD PTR [rsp+0xc8]
    1d75:	00 00 
    1d77:	c5 fc 59 84 24 e8 00 	vmulps ymm0,ymm0,YMMWORD PTR [rsp+0xe8]
    1d7e:	00 00 
    1d80:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1d85:	c5 fc 29 80 c0 00 00 	vmovaps YMMWORD PTR [rax+0xc0],ymm0
    1d8c:	00 

  // TODO: deal with switching face normals later when dealing with
  // dialectric materials

  return hit_rec;
    1d8d:	90                   	nop
}
    1d8e:	48 8b 44 24 c0       	mov    rax,QWORD PTR [rsp-0x40]
    1d93:	c9                   	leave  
    1d94:	c3                   	ret    

0000000000001d95 <main>:

int main() {
    1d95:	f3 0f 1e fa          	endbr64 
    1d99:	4c 8d 54 24 08       	lea    r10,[rsp+0x8]
    1d9e:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
    1da2:	41 ff 72 f8          	push   QWORD PTR [r10-0x8]
    1da6:	55                   	push   rbp
    1da7:	48 89 e5             	mov    rbp,rsp
    1daa:	41 52                	push   r10
    1dac:	48 81 ec e8 00 00 00 	sub    rsp,0xe8
    1db3:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    1dba:	00 00 
    1dbc:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
    1dc0:	31 c0                	xor    eax,eax

  RayCluster rays = {};
    1dc2:	48 8d 95 10 ff ff ff 	lea    rdx,[rbp-0xf0]
    1dc9:	b8 00 00 00 00       	mov    eax,0x0
    1dce:	b9 18 00 00 00       	mov    ecx,0x18
    1dd3:	48 89 d7             	mov    rdi,rdx
    1dd6:	f3 48 ab             	rep stos QWORD PTR es:[rdi],rax

  sphere_hit(rays, 100, spheres[0]);
    1dd9:	be 00 00 00 00       	mov    esi,0x0
    1dde:	48 8d 05 4b 02 00 00 	lea    rax,[rip+0x24b]        # 2030 <_ZL7spheres>
    1de5:	48 89 c7             	mov    rdi,rax
    1de8:	e8 71 00 00 00       	call   1e5e <_ZNKSt5arrayI6SphereLm1EEixEm>
    1ded:	48 8b 08             	mov    rcx,QWORD PTR [rax]
    1df0:	48 8b 50 08          	mov    rdx,QWORD PTR [rax+0x8]
    1df4:	48 8d 85 10 ff ff ff 	lea    rax,[rbp-0xf0]
    1dfb:	c4 e1 f9 6e c9       	vmovq  xmm1,rcx
    1e00:	c4 e1 f9 6e d2       	vmovq  xmm2,rdx
    1e05:	8b 15 35 02 00 00    	mov    edx,DWORD PTR [rip+0x235]        # 2040 <_ZL7spheres+0x10>
    1e0b:	c5 f9 6e c2          	vmovd  xmm0,edx
    1e0f:	48 89 c7             	mov    rdi,rax
    1e12:	e8 ff f3 ff ff       	call   1216 <_Z10sphere_hitRK10RayClusterf6Sphere>
  return 0;
    1e17:	b8 00 00 00 00       	mov    eax,0x0
}
    1e1c:	48 8b 55 e8          	mov    rdx,QWORD PTR [rbp-0x18]
    1e20:	64 48 2b 14 25 28 00 	sub    rdx,QWORD PTR fs:0x28
    1e27:	00 00 
    1e29:	74 05                	je     1e30 <main+0x9b>
    1e2b:	e8 30 f2 ff ff       	call   1060 <__stack_chk_fail@plt>
    1e30:	4c 8b 55 f8          	mov    r10,QWORD PTR [rbp-0x8]
    1e34:	c9                   	leave  
    1e35:	49 8d 62 f8          	lea    rsp,[r10-0x8]
    1e39:	c3                   	ret    

0000000000001e3a <_ZNSt14__array_traitsI6SphereLm1EE6_S_refERA1_KS0_m>:
      typedef _Tp _Type[_Nm];
      typedef __is_swappable<_Tp> _Is_swappable;
      typedef __is_nothrow_swappable<_Tp> _Is_nothrow_swappable;

      static constexpr _Tp&
      _S_ref(const _Type& __t, std::size_t __n) noexcept
    1e3a:	f3 0f 1e fa          	endbr64 
    1e3e:	55                   	push   rbp
    1e3f:	48 89 e5             	mov    rbp,rsp
    1e42:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
    1e46:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
      { return const_cast<_Tp&>(__t[__n]); }
    1e4a:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
    1e4e:	48 c1 e0 04          	shl    rax,0x4
    1e52:	48 89 c2             	mov    rdx,rax
    1e55:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
    1e59:	48 01 d0             	add    rax,rdx
    1e5c:	5d                   	pop    rbp
    1e5d:	c3                   	ret    

0000000000001e5e <_ZNKSt5arrayI6SphereLm1EEixEm>:
	__glibcxx_requires_subscript(__n);
	return _AT_Type::_S_ref(_M_elems, __n);
      }

      constexpr const_reference
      operator[](size_type __n) const noexcept
    1e5e:	f3 0f 1e fa          	endbr64 
    1e62:	55                   	push   rbp
    1e63:	48 89 e5             	mov    rbp,rsp
    1e66:	48 83 ec 10          	sub    rsp,0x10
    1e6a:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
    1e6e:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
      {
#if __cplusplus >= 201402L
	__glibcxx_requires_subscript(__n);
#endif
	return _AT_Type::_S_ref(_M_elems, __n);
    1e72:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
    1e76:	48 8b 55 f0          	mov    rdx,QWORD PTR [rbp-0x10]
    1e7a:	48 89 d6             	mov    rsi,rdx
    1e7d:	48 89 c7             	mov    rdi,rax
    1e80:	e8 b5 ff ff ff       	call   1e3a <_ZNSt14__array_traitsI6SphereLm1EE6_S_refERA1_KS0_m>
      }
    1e85:	c9                   	leave  
    1e86:	c3                   	ret    

Disassembly of section .fini:

0000000000001e88 <_fini>:
    1e88:	f3 0f 1e fa          	endbr64 
    1e8c:	48 83 ec 08          	sub    rsp,0x8
    1e90:	48 83 c4 08          	add    rsp,0x8
    1e94:	c3                   	ret    
