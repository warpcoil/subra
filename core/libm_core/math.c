#include "math.h"
#include "oslib/stdint.h"

/* origin: FreeBSD /usr/src/lib/msun/src/e_acos.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

static const double
pio2_hi = 1.57079632679489655800e+00, /* 0x3FF921FB, 0x54442D18 */
pio2_lo = 6.12323399573676603587e-17, /* 0x3C91A626, 0x33145C07 */
/* coefficients for R(x^2) */
pS0 =  1.66666666666666657415e-01, /* 0x3FC55555, 0x55555555 */
pS1 = -3.25565818622400915405e-01, /* 0xBFD4D612, 0x03EB6F7D */
pS2 =  2.01212532134862925881e-01, /* 0x3FC9C155, 0x0E884455 */
pS3 = -4.00555345006794114027e-02, /* 0xBFA48228, 0xB5688F3B */
pS4 =  7.91534994289814532176e-04, /* 0x3F49EFE0, 0x7501B288 */
pS5 =  3.47933107596021167570e-05, /* 0x3F023DE1, 0x0DFDF709 */
qS1 = -2.40339491173441421878e+00, /* 0xC0033A27, 0x1C8A2D4B */
qS2 =  2.02094576023350569471e+00, /* 0x40002AE5, 0x9C598AC8 */
qS3 = -6.88283971605453293030e-01, /* 0xBFE6066C, 0x1B8D0159 */
qS4 =  7.70381505559019352791e-02; /* 0x3FB3B8C5, 0xB12E9282 */

static const double
pi     = 3.1415926535897931160E+00, /* 0x400921FB, 0x54442D18 */
pi_lo  = 1.2246467991473531772E-16; /* 0x3CA1A626, 0x33145C07 */

static const int init_jk[] = {3,4,4,6}; /* initial value for jk */

/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 *
 *              integer array, contains the (24*i)-th to (24*i+23)-th
 *              bit of 2/pi after binary point. The corresponding
 *              floating value is
 *
 *                      ipio2[i] * 2^(-24(i+1)).
 *
 * NB: This table must have at least (e0-3)/24 + jk terms.
 *     For quad precision (e0 <= 16360, jk = 6), this is 686.
 */
static const int32_t ipio2[] = {
0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A,
0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129,
0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C, 0x7026B4, 0x5F7E41,
0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8,
0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A, 0x6D1F6D, 0x367ECF,
0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5,
0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB, 0x5FB11F, 0x8D5D08,
0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3,
0x91615E, 0xE61B08, 0x659985, 0x5F14A0, 0x68408D, 0xFFD880,
0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B,

#if LDBL_MAX_EXP > 1024
0x47C419, 0xC367CD, 0xDCE809, 0x2A8359, 0xC4768B, 0x961CA6,
0xDDAF44, 0xD15719, 0x053EA5, 0xFF0705, 0x3F7E33, 0xE832C2,
0xDE4F98, 0x327DBB, 0xC33D26, 0xEF6B1E, 0x5EF89F, 0x3A1F35,
0xCAF27F, 0x1D87F1, 0x21907C, 0x7C246A, 0xFA6ED5, 0x772D30,
0x433B15, 0xC614B5, 0x9D19C3, 0xC2C4AD, 0x414D2C, 0x5D000C,
0x467D86, 0x2D71E3, 0x9AC69B, 0x006233, 0x7CD2B4, 0x97A7B4,
0xD55537, 0xF63ED7, 0x1810A3, 0xFC764D, 0x2A9D64, 0xABD770,
0xF87C63, 0x57B07A, 0xE71517, 0x5649C0, 0xD9D63B, 0x3884A7,
0xCB2324, 0x778AD6, 0x23545A, 0xB91F00, 0x1B0AF1, 0xDFCE19,
0xFF319F, 0x6A1E66, 0x615799, 0x47FBAC, 0xD87F7E, 0xB76522,
0x89E832, 0x60BFE6, 0xCDC4EF, 0x09366C, 0xD43F5D, 0xD7DE16,
0xDE3B58, 0x929BDE, 0x2822D2, 0xE88628, 0x4D58E2, 0x32CAC6,
0x16E308, 0xCB7DE0, 0x50C017, 0xA71DF3, 0x5BE018, 0x34132E,
0x621283, 0x014883, 0x5B8EF5, 0x7FB0AD, 0xF2E91E, 0x434A48,
0xD36710, 0xD8DDAA, 0x425FAE, 0xCE616A, 0xA4280A, 0xB499D3,
0xF2A606, 0x7F775C, 0x83C2A3, 0x883C61, 0x78738A, 0x5A8CAF,
0xBDD76F, 0x63A62D, 0xCBBFF4, 0xEF818D, 0x67C126, 0x45CA55,
0x36D9CA, 0xD2A828, 0x8D61C2, 0x77C912, 0x142604, 0x9B4612,
0xC459C4, 0x44C5C8, 0x91B24D, 0xF31700, 0xAD43D4, 0xE54929,
0x10D5FD, 0xFCBE00, 0xCC941E, 0xEECE70, 0xF53E13, 0x80F1EC,
0xC3E7B3, 0x28F8C7, 0x940593, 0x3E71C1, 0xB3092E, 0xF3450B,
0x9C1288, 0x7B20AB, 0x9FB52E, 0xC29247, 0x2F327B, 0x6D550C,
0x90A772, 0x1FE76B, 0x96CB31, 0x4A1679, 0xE27941, 0x89DFF4,
0x9794E8, 0x84E6E2, 0x973199, 0x6BED88, 0x365F5F, 0x0EFDBB,
0xB49A48, 0x6CA467, 0x427271, 0x325D8D, 0xB8159F, 0x09E5BC,
0x25318D, 0x3974F7, 0x1C0530, 0x010C0D, 0x68084B, 0x58EE2C,
0x90AA47, 0x02E774, 0x24D6BD, 0xA67DF7, 0x72486E, 0xEF169F,
0xA6948E, 0xF691B4, 0x5153D1, 0xF20ACF, 0x339820, 0x7E4BF5,
0x6863B2, 0x5F3EDD, 0x035D40, 0x7F8985, 0x295255, 0xC06437,
0x10D86D, 0x324832, 0x754C5B, 0xD4714E, 0x6E5445, 0xC1090B,
0x69F52A, 0xD56614, 0x9D0727, 0x50045D, 0xDB3BB4, 0xC576EA,
0x17F987, 0x7D6B49, 0xBA271D, 0x296996, 0xACCCC6, 0x5414AD,
0x6AE290, 0x89D988, 0x50722C, 0xBEA404, 0x940777, 0x7030F3,
0x27FC00, 0xA871EA, 0x49C266, 0x3DE064, 0x83DD97, 0x973FA3,
0xFD9443, 0x8C860D, 0xDE4131, 0x9D3992, 0x8C70DD, 0xE7B717,
0x3BDF08, 0x2B3715, 0xA0805C, 0x93805A, 0x921110, 0xD8E80F,
0xAF806C, 0x4BFFDB, 0x0F9038, 0x761859, 0x15A562, 0xBBCB61,
0xB989C7, 0xBD4010, 0x04F2D2, 0x277549, 0xF6B6EB, 0xBB22DB,
0xAA140A, 0x2F2689, 0x768364, 0x333B09, 0x1A940E, 0xAA3A51,
0xC2A31D, 0xAEEDAF, 0x12265C, 0x4DC26D, 0x9C7A2D, 0x9756C0,
0x833F03, 0xF6F009, 0x8C402B, 0x99316D, 0x07B439, 0x15200C,
0x5BC3D8, 0xC492F5, 0x4BADC6, 0xA5CA4E, 0xCD37A7, 0x36A9E6,
0x9492AB, 0x6842DD, 0xDE6319, 0xEF8C76, 0x528B68, 0x37DBFC,
0xABA1AE, 0x3115DF, 0xA1AE00, 0xDAFB0C, 0x664D64, 0xB705ED,
0x306529, 0xBF5657, 0x3AFF47, 0xB9F96A, 0xF3BE75, 0xDF9328,
0x3080AB, 0xF68C66, 0x15CB04, 0x0622FA, 0x1DE4D9, 0xA4B33D,
0x8F1B57, 0x09CD36, 0xE9424E, 0xA4BE13, 0xB52333, 0x1AAAF0,
0xA8654F, 0xA5C1D2, 0x0F3F0B, 0xCD785B, 0x76F923, 0x048B7B,
0x721789, 0x53A6C6, 0xE26E6F, 0x00EBEF, 0x584A9B, 0xB7DAC4,
0xBA66AA, 0xCFCF76, 0x1D02D1, 0x2DF1B1, 0xC1998C, 0x77ADC3,
0xDA4886, 0xA05DF7, 0xF480C6, 0x2FF0AC, 0x9AECDD, 0xBC5C3F,
0x6DDED0, 0x1FC790, 0xB6DB2A, 0x3A25A3, 0x9AAF00, 0x9353AD,
0x0457B6, 0xB42D29, 0x7E804B, 0xA707DA, 0x0EAA76, 0xA1597B,
0x2A1216, 0x2DB7DC, 0xFDE5FA, 0xFEDB89, 0xFDBE89, 0x6C76E4,
0xFCA906, 0x70803E, 0x156E85, 0xFF87FD, 0x073E28, 0x336761,
0x86182A, 0xEABD4D, 0xAFE7B3, 0x6E6D8F, 0x396795, 0x5BBF31,
0x48D784, 0x16DF30, 0x432DC7, 0x356125, 0xCE70C9, 0xB8CB30,
0xFD6CBF, 0xA200A4, 0xE46C05, 0xA0DD5A, 0x476F21, 0xD21262,
0x845CB9, 0x496170, 0xE0566B, 0x015299, 0x375550, 0xB7D51E,
0xC4F133, 0x5F6E13, 0xE4305D, 0xA92E85, 0xC3B21D, 0x3632A1,
0xA4B708, 0xD4B1EA, 0x21F716, 0xE4698F, 0x77FF27, 0x80030C,
0x2D408D, 0xA0CD4F, 0x99A520, 0xD3A2B3, 0x0A5D2F, 0x42F9B4,
0xCBDA11, 0xD0BE7D, 0xC1DB9B, 0xBD17AB, 0x81A2CA, 0x5C6A08,
0x17552E, 0x550027, 0xF0147F, 0x8607E1, 0x640B14, 0x8D4196,
0xDEBE87, 0x2AFDDA, 0xB6256B, 0x34897B, 0xFEF305, 0x9EBFB9,
0x4F6A68, 0xA82A4A, 0x5AC44F, 0xBCF82D, 0x985AD7, 0x95C7F4,
0x8D4D0D, 0xA63A20, 0x5F57A4, 0xB13F14, 0x953880, 0x0120CC,
0x86DD71, 0xB6DEC9, 0xF560BF, 0x11654D, 0x6B0701, 0xACB08C,
0xD0C0B2, 0x485551, 0x0EFB1E, 0xC37295, 0x3B06A3, 0x3540C0,
0x7BDC06, 0xCC45E0, 0xFA294E, 0xC8CAD6, 0x41F3E8, 0xDE647C,
0xD8649B, 0x31BED9, 0xC397A4, 0xD45877, 0xC5E369, 0x13DAF0,
0x3C3ABA, 0x461846, 0x5F7555, 0xF5BDD2, 0xC6926E, 0x5D2EAC,
0xED440E, 0x423E1C, 0x87C461, 0xE9FD29, 0xF3D6E7, 0xCA7C22,
0x35916F, 0xC5E008, 0x8DD7FF, 0xE26A6E, 0xC6FDB0, 0xC10893,
0x745D7C, 0xB2AD6B, 0x9D6ECD, 0x7B723E, 0x6A11C6, 0xA9CFF7,
0xDF7329, 0xBAC9B5, 0x5100B7, 0x0DB2E2, 0x24BA74, 0x607DE5,
0x8AD874, 0x2C150D, 0x0C1881, 0x94667E, 0x162901, 0x767A9F,
0xBEFDFD, 0xEF4556, 0x367ED9, 0x13D9EC, 0xB9BA8B, 0xFC97C4,
0x27A831, 0xC36EF1, 0x36C594, 0x56A8D8, 0xB5A8B4, 0x0ECCCF,
0x2D8912, 0x34576F, 0x89562C, 0xE3CE99, 0xB920D6, 0xAA5E6B,
0x9C2A3E, 0xCC5F11, 0x4A0BFD, 0xFBF4E1, 0x6D3B8E, 0x2C86E2,
0x84D4E9, 0xA9B4FC, 0xD1EEEF, 0xC9352E, 0x61392F, 0x442138,
0xC8D91B, 0x0AFC81, 0x6A4AFB, 0xD81C2F, 0x84B453, 0x8C994E,
0xCC2254, 0xDC552A, 0xD6C6C0, 0x96190B, 0xB8701A, 0x649569,
0x605A26, 0xEE523F, 0x0F117F, 0x11B5F4, 0xF5CBFC, 0x2DBC34,
0xEEBC34, 0xCC5DE8, 0x605EDD, 0x9B8E67, 0xEF3392, 0xB817C9,
0x9B5861, 0xBC57E1, 0xC68351, 0x103ED8, 0x4871DD, 0xDD1C2D,
0xA118AF, 0x462C21, 0xD7F359, 0x987AD9, 0xC0549E, 0xFA864F,
0xFC0656, 0xAE79E5, 0x362289, 0x22AD38, 0xDC9367, 0xAAE855,
0x382682, 0x9BE7CA, 0xA40D51, 0xB13399, 0x0ED7A9, 0x480569,
0xF0B265, 0xA7887F, 0x974C88, 0x36D1F9, 0xB39221, 0x4A827B,
0x21CF98, 0xDC9F40, 0x5547DC, 0x3A74E1, 0x42EB67, 0xDF9DFE,
0x5FD45E, 0xA4677B, 0x7AACBA, 0xA2F655, 0x23882B, 0x55BA41,
0x086E59, 0x862A21, 0x834739, 0xE6E389, 0xD49EE5, 0x40FB49,
0xE956FF, 0xCA0F1C, 0x8A59C5, 0x2BFA94, 0xC5C1D3, 0xCFC50F,
0xAE5ADB, 0x86C547, 0x624385, 0x3B8621, 0x94792C, 0x876110,
0x7B4C2A, 0x1A2C80, 0x12BF43, 0x902688, 0x893C78, 0xE4C4A8,
0x7BDBE5, 0xC23AC4, 0xEAF426, 0x8A67F7, 0xBF920D, 0x2BA365,
0xB1933D, 0x0B7CBD, 0xDC51A4, 0x63DD27, 0xDDE169, 0x19949A,
0x9529A8, 0x28CE68, 0xB4ED09, 0x209F44, 0xCA984E, 0x638270,
0x237C7E, 0x32B90F, 0x8EF5A7, 0xE75614, 0x08F121, 0x2A9DB5,
0x4D7E6F, 0x5119A5, 0xABF9B5, 0xD6DF82, 0x61DD96, 0x023616,
0x9F3AC4, 0xA1A283, 0x6DED72, 0x7A8D39, 0xA9B882, 0x5C326B,
0x5B2746, 0xED3400, 0x7700D2, 0x55F4FC, 0x4D5901, 0x8071E0,
#endif
};

static const double PIo2[] = {
  1.57079625129699707031e+00, /* 0x3FF921FB, 0x40000000 */
  7.54978941586159635335e-08, /* 0x3E74442D, 0x00000000 */
  5.39030252995776476554e-15, /* 0x3CF84698, 0x80000000 */
  3.28200341580791294123e-22, /* 0x3B78CC51, 0x60000000 */
  1.27065575308067607349e-29, /* 0x39F01B83, 0x80000000 */
  1.22933308981111328932e-36, /* 0x387A2520, 0x40000000 */
  2.73370053816464559624e-44, /* 0x36E38222, 0x80000000 */
  2.16741683877804819444e-51, /* 0x3569F31D, 0x00000000 */
};

/*
 * invpio2:  53 bits of 2/pi
 * pio2_1:   first  33 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 33 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  33 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */
static const double
toint1   = 1.5/DBL_EPSILON,
invpio2 = 6.36619772367581382433e-01, /* 0x3FE45F30, 0x6DC9C883 */
pio2_1  = 1.57079632673412561417e+00, /* 0x3FF921FB, 0x54400000 */
pio2_1t = 6.07710050650619224932e-11, /* 0x3DD0B461, 0x1A626331 */
pio2_2  = 6.07710050630396597660e-11, /* 0x3DD0B461, 0x1A600000 */
pio2_2t = 2.02226624879595063154e-21, /* 0x3BA3198A, 0x2E037073 */
pio2_3  = 2.02226624871116645580e-21, /* 0x3BA3198A, 0x2E000000 */
pio2_3t = 8.47842766036889956997e-32; /* 0x397B839A, 0x252049C1 */

static const double
bp[]   = {1.0, 1.5,},
dp_h[] = { 0.0, 5.84962487220764160156e-01,}, /* 0x3FE2B803, 0x40000000 */
dp_l[] = { 0.0, 1.35003920212974897128e-08,}, /* 0x3E4CFDEB, 0x43CFD006 */
two53  =  9007199254740992.0, /* 0x43400000, 0x00000000 */
huge   =  1.0e300,
tiny   =  1.0e-300;

static const double
half[2] = {0.5,-0.5},
ln2hi = 6.93147180369123816490e-01, /* 0x3fe62e42, 0xfee00000 */
ln2lo = 1.90821492927058770002e-10, /* 0x3dea39ef, 0x35793c76 */
invln2 = 1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
L1 =  5.99999999999994648725e-01, /* 0x3FE33333, 0x33333303 */
L2 =  4.28571428578550184252e-01, /* 0x3FDB6DB6, 0xDB6FABFF */
L3 =  3.33333329818377432918e-01, /* 0x3FD55555, 0x518F264D */
L4 =  2.72728123808534006489e-01, /* 0x3FD17460, 0xA91D4101 */
L5 =  2.30660745775561754067e-01, /* 0x3FCD864A, 0x93C9DB65 */
L6 =  2.06975017800338417784e-01, /* 0x3FCA7E28, 0x4A454EEF */
P1   =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2   = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3   =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4   = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5   =  4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

static const double T[] = {
             3.33333333333334091986e-01, /* 3FD55555, 55555563 */
             1.33333333333201242699e-01, /* 3FC11111, 1110FE7A */
             5.39682539762260521377e-02, /* 3FABA1BA, 1BB341FE */
             2.18694882948595424599e-02, /* 3F9664F4, 8406D637 */
             8.86323982359930005737e-03, /* 3F8226E3, E96E8493 */
             3.59207910759131235356e-03, /* 3F6D6D22, C9560328 */
             1.45620945432529025516e-03, /* 3F57DBC8, FEE08315 */
             5.88041240820264096874e-04, /* 3F4344D8, F2F26501 */
             2.46463134818469906812e-04, /* 3F3026F7, 1A8D1068 */
             7.81794442939557092300e-05, /* 3F147E88, A03792A6 */
             7.14072491382608190305e-05, /* 3F12B80F, 32F0A7E9 */
            -1.85586374855275456654e-05, /* BEF375CB, DB605373 */
             2.59073051863633712884e-05, /* 3EFB2A70, 74BF7AD4 */
},
pio4 =       7.85398163397448278999e-01, /* 3FE921FB, 54442D18 */
pio4lo =     3.06161699786838301793e-17; /* 3C81A626, 33145C07 */

static const double atanhi[] = {
  4.63647609000806093515e-01, /* atan(0.5)hi 0x3FDDAC67, 0x0561BB4F */
  7.85398163397448278999e-01, /* atan(1.0)hi 0x3FE921FB, 0x54442D18 */
  9.82793723247329054082e-01, /* atan(1.5)hi 0x3FEF730B, 0xD281F69B */
  1.57079632679489655800e+00, /* atan(inf)hi 0x3FF921FB, 0x54442D18 */
};

static const double atanlo[] = {
  2.26987774529616870924e-17, /* atan(0.5)lo 0x3C7A2B7F, 0x222F65E2 */
  3.06161699786838301793e-17, /* atan(1.0)lo 0x3C81A626, 0x33145C07 */
  1.39033110312309984516e-17, /* atan(1.5)lo 0x3C700788, 0x7AF0CBBD */
  6.12323399573676603587e-17, /* atan(inf)lo 0x3C91A626, 0x33145C07 */
};

static const double aT[] = {
  3.33333333333329318027e-01, /* 0x3FD55555, 0x5555550D */
 -1.99999999998764832476e-01, /* 0xBFC99999, 0x9998EBC4 */
  1.42857142725034663711e-01, /* 0x3FC24924, 0x920083FF */
 -1.11111104054623557880e-01, /* 0xBFBC71C6, 0xFE231671 */
  9.09088713343650656196e-02, /* 0x3FB745CD, 0xC54C206E */
 -7.69187620504482999495e-02, /* 0xBFB3B0F2, 0xAF749A6D */
  6.66107313738753120669e-02, /* 0x3FB10D66, 0xA0D03D51 */
 -5.83357013379057348645e-02, /* 0xBFADDE2D, 0x52DEFD9A */
  4.97687799461593236017e-02, /* 0x3FA97B4B, 0x24760DEB */
 -3.65315727442169155270e-02, /* 0xBFA2B444, 0x2C6A6C2F */
  1.62858201153657823623e-02, /* 0x3F90AD3A, 0xE322DA11 */
};

static const double
C1  =  4.16666666666666019037e-02, /* 0x3FA55555, 0x5555554C */
C2  = -1.38888888888741095749e-03, /* 0xBF56C16C, 0x16C15177 */
C3  =  2.48015872894767294178e-05, /* 0x3EFA01A0, 0x19CB1590 */
C4  = -2.75573143513906633035e-07, /* 0xBE927E4F, 0x809C52AD */
C5  =  2.08757232129817482790e-09, /* 0x3E21EE9E, 0xBDB4B1C4 */
C6  = -1.13596475577881948265e-11; /* 0xBDA8FAE9, 0xBE8838D4 */

static const double
S1  = -1.66666666666666324348e-01, /* 0xBFC55555, 0x55555549 */
S2  =  8.33333333332248946124e-03, /* 0x3F811111, 0x1110F8A6 */
S3  = -1.98412698298579493134e-04, /* 0xBF2A01A0, 0x19C161D5 */
S4  =  2.75573137070700676789e-06, /* 0x3EC71DE3, 0x57B1FE7D */
S5  = -2.50507602534068634195e-08, /* 0xBE5AE5E6, 0x8A2B9CEB */
S6  =  1.58969099521155010221e-10; /* 0x3DE5D93A, 0x5ACFD57C */

static const double
ln2_hi = 6.93147180369123816490e-01,  /* 3fe62e42 fee00000 */
ln2_lo = 1.90821492927058770002e-10,  /* 3dea39ef 35793c76 */
Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7 = 1.479819860511658591e-01,  /* 3FC2F112 DF3E5244 */
lg2     =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
lg2_h   =  6.93147182464599609375e-01, /* 0x3FE62E43, 0x00000000 */
lg2_l   = -1.90465429995776804525e-09, /* 0xBE205C61, 0x0CA86C39 */
ovt     =  8.0085662595372944372e-017, /* -(1024-log2(ovfl+.5ulp)) */
cp      =  9.61796693925975554329e-01, /* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
cp_h    =  9.61796700954437255859e-01, /* 0x3FEEC709, 0xE0000000 =(float)cp */
cp_l    = -7.02846165095275826516e-09, /* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h*/
ivln2   =  1.44269504088896338700e+00, /* 0x3FF71547, 0x652B82FE =1/ln2 */
ivln2_h =  1.44269502162933349609e+00, /* 0x3FF71547, 0x60000000 =24b 1/ln2*/
ivln2_l =  1.92596299112661746887e-08; /* 0x3E54AE0B, 0xF85DDF44 =1/ln2 tail*/

static const double_t toint = 1/DBL_EPSILON;

static double R(double z)
{
    double_t p, q;
    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
    q = 1.0+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
    return p/q;
}

/* Internal methods */
double scalbn(double x, int n)
{
    union {double f; uint64_t i;} u;
    double_t y = x;

    if (n > 1023) {
        y *= 0x1p1023;
        n -= 1023;
        if (n > 1023) {
            y *= 0x1p1023;
            n -= 1023;
            if (n > 1023)
                n = 1023;
        }
    } else if (n < -1022) {
        y *= 0x1p-1022;
        n += 1022;
        if (n < -1022) {
            y *= 0x1p-1022;
            n += 1022;
            if (n < -1022)
                n = -1022;
        }
    }
    u.i = (uint64_t)(0x3ff+n)<<52;
    x = y * u.f;
    return x;
}

int __rem_pio2_large(double *x, double *y, int e0, int nx, int prec)
{
    int32_t jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,m,q0,ih;
    double z,fw,f[20],fq[20],q[20];

    /* initialize jk*/
    jk = init_jk[prec];
    jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
    jx = nx-1;
    jv = (e0-3)/24;  if(jv<0) jv=0;
    q0 = e0-24*(jv+1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
    j = jv-jx; m = jx+jk;
    for (i=0; i<=m; i++,j++)
        f[i] = j<0 ? 0.0 : (double)ipio2[j];

    /* compute q[0],q[1],...q[jk] */
    for (i=0; i<=jk; i++) {
        for (j=0,fw=0.0; j<=jx; j++)
            fw += x[j]*f[jx+i-j];
        q[i] = fw;
    }

    jz = jk;
recompute:
    /* distill q[] into iq[] reversingly */
    for (i=0,j=jz,z=q[jz]; j>0; i++,j--) {
        fw    = (double)(int32_t)(0x1p-24*z);
        iq[i] = (int32_t)(z - 0x1p24*fw);
        z     = q[j-1]+fw;
    }

    /* compute n */
    z  = scalbn(z,q0);       /* actual value of z */
    z -= 8.0*floor(z*0.125); /* trim off integer >= 8 */
    n  = (int32_t)z;
    z -= (double)n;
    ih = 0;
    if (q0 > 0) {  /* need iq[jz-1] to determine n */
        i  = iq[jz-1]>>(24-q0); n += i;
        iq[jz-1] -= i<<(24-q0);
        ih = iq[jz-1]>>(23-q0);
    }
    else if (q0 == 0) ih = iq[jz-1]>>23;
    else if (z >= 0.5) ih = 2;

    if (ih > 0) {  /* q > 0.5 */
        n += 1; carry = 0;
        for (i=0; i<jz; i++) {  /* compute 1-q */
            j = iq[i];
            if (carry == 0) {
                if (j != 0) {
                    carry = 1;
                    iq[i] = 0x1000000 - j;
                }
            } else
                iq[i] = 0xffffff - j;
        }
        if (q0 > 0) {  /* rare case: chance is 1 in 12 */
            switch(q0) {
            case 1:
                iq[jz-1] &= 0x7fffff; break;
            case 2:
                iq[jz-1] &= 0x3fffff; break;
            }
        }
        if (ih == 2) {
            z = 1.0 - z;
            if (carry != 0)
                z -= scalbn(1.0,q0);
        }
    }

    /* check if recomputation is needed */
    if (z == 0.0) {
        j = 0;
        for (i=jz-1; i>=jk; i--) j |= iq[i];
        if (j == 0) {  /* need recomputation */
            for (k=1; iq[jk-k]==0; k++);  /* k = no. of terms needed */

            for (i=jz+1; i<=jz+k; i++) {  /* add q[jz+1] to q[jz+k] */
                f[jx+i] = (double)ipio2[jv+i];
                for (j=0,fw=0.0; j<=jx; j++)
                    fw += x[j]*f[jx+i-j];
                q[i] = fw;
            }
            jz += k;
            goto recompute;
        }
    }

    /* chop off zero terms */
    if (z == 0.0) {
        jz -= 1;
        q0 -= 24;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 24;
        }
    } else { /* break z into 24-bit if necessary */
        z = scalbn(z,-q0);
        if (z >= 0x1p24) {
            fw = (double)(int32_t)(0x1p-24*z);
            iq[jz] = (int32_t)(z - 0x1p24*fw);
            jz += 1;
            q0 += 24;
            iq[jz] = (int32_t)fw;
        } else
            iq[jz] = (int32_t)z;
    }

    /* convert integer "bit" chunk to floating-point value */
    fw = scalbn(1.0,q0);
    for (i=jz; i>=0; i--) {
        q[i] = fw*(double)iq[i];
        fw *= 0x1p-24;
    }

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
    for(i=jz; i>=0; i--) {
        for (fw=0.0,k=0; k<=jp && k<=jz-i; k++)
            fw += PIo2[k]*q[i+k];
        fq[jz-i] = fw;
    }

    /* compress fq[] into y[] */
    switch(prec) {
    case 0:
        fw = 0.0;
        for (i=jz; i>=0; i--)
            fw += fq[i];
        y[0] = ih==0 ? fw : -fw;
        break;
    case 1:
    case 2:
        fw = 0.0;
        for (i=jz; i>=0; i--)
            fw += fq[i];
        // TODO: drop excess precision here once double_t is used
        fw = (double)fw;
        y[0] = ih==0 ? fw : -fw;
        fw = fq[0]-fw;
        for (i=1; i<=jz; i++)
            fw += fq[i];
        y[1] = ih==0 ? fw : -fw;
        break;
    case 3:  /* painful */
        for (i=jz; i>0; i--) {
            fw      = fq[i-1]+fq[i];
            fq[i]  += fq[i-1]-fw;
            fq[i-1] = fw;
        }
        for (i=jz; i>1; i--) {
            fw      = fq[i-1]+fq[i];
            fq[i]  += fq[i-1]-fw;
            fq[i-1] = fw;
        }
        for (fw=0.0,i=jz; i>=2; i--)
            fw += fq[i];
        if (ih==0) {
            y[0] =  fq[0]; y[1] =  fq[1]; y[2] =  fw;
        } else {
            y[0] = -fq[0]; y[1] = -fq[1]; y[2] = -fw;
        }
    }
    return n&7;
}

/* caller must handle the case when reduction is not needed: |x| ~<= pi/4 */
int __rem_pio2(double x, double *y)
{
    union {double f; uint64_t i;} u = {x};
    double_t z,w,t,r,fn;
    double tx[3],ty[2];
    uint32_t ix;
    int sign, n, ex, ey, i;

    sign = u.i>>63;
    ix = u.i>>32 & 0x7fffffff;
    if (ix <= 0x400f6a7a) {  /* |x| ~<= 5pi/4 */
        if ((ix & 0xfffff) == 0x921fb)  /* |x| ~= pi/2 or 2pi/2 */
            goto medium;  /* cancellation -- use medium case */
        if (ix <= 0x4002d97c) {  /* |x| ~<= 3pi/4 */
            if (!sign) {
                z = x - pio2_1;  /* one round good to 85 bits */
                y[0] = z - pio2_1t;
                y[1] = (z-y[0]) - pio2_1t;
                return 1;
            } else {
                z = x + pio2_1;
                y[0] = z + pio2_1t;
                y[1] = (z-y[0]) + pio2_1t;
                return -1;
            }
        } else {
            if (!sign) {
                z = x - 2*pio2_1;
                y[0] = z - 2*pio2_1t;
                y[1] = (z-y[0]) - 2*pio2_1t;
                return 2;
            } else {
                z = x + 2*pio2_1;
                y[0] = z + 2*pio2_1t;
                y[1] = (z-y[0]) + 2*pio2_1t;
                return -2;
            }
        }
    }
    if (ix <= 0x401c463b) {  /* |x| ~<= 9pi/4 */
        if (ix <= 0x4015fdbc) {  /* |x| ~<= 7pi/4 */
            if (ix == 0x4012d97c)  /* |x| ~= 3pi/2 */
                goto medium;
            if (!sign) {
                z = x - 3*pio2_1;
                y[0] = z - 3*pio2_1t;
                y[1] = (z-y[0]) - 3*pio2_1t;
                return 3;
            } else {
                z = x + 3*pio2_1;
                y[0] = z + 3*pio2_1t;
                y[1] = (z-y[0]) + 3*pio2_1t;
                return -3;
            }
        } else {
            if (ix == 0x401921fb)  /* |x| ~= 4pi/2 */
                goto medium;
            if (!sign) {
                z = x - 4*pio2_1;
                y[0] = z - 4*pio2_1t;
                y[1] = (z-y[0]) - 4*pio2_1t;
                return 4;
            } else {
                z = x + 4*pio2_1;
                y[0] = z + 4*pio2_1t;
                y[1] = (z-y[0]) + 4*pio2_1t;
                return -4;
            }
        }
    }
    if (ix < 0x413921fb) {  /* |x| ~< 2^20*(pi/2), medium size */
medium:
        /* rint(x/(pi/2)), Assume round-to-nearest. */
        fn = (double_t)x*invpio2 + toint1 - toint1;
        n = (int32_t)fn;
        r = x - fn*pio2_1;
        w = fn*pio2_1t;  /* 1st round, good to 85 bits */
        y[0] = r - w;
        u.f = y[0];
        ey = u.i>>52 & 0x7ff;
        ex = ix>>20;
        if (ex - ey > 16) { /* 2nd round, good to 118 bits */
            t = r;
            w = fn*pio2_2;
            r = t - w;
            w = fn*pio2_2t - ((t-r)-w);
            y[0] = r - w;
            u.f = y[0];
            ey = u.i>>52 & 0x7ff;
            if (ex - ey > 49) {  /* 3rd round, good to 151 bits, covers all cases */
                t = r;
                w = fn*pio2_3;
                r = t - w;
                w = fn*pio2_3t - ((t-r)-w);
                y[0] = r - w;
            }
        }
        y[1] = (r - y[0]) - w;
        return n;
    }
    /*
     * all other (large) arguments
     */
    if (ix >= 0x7ff00000) {  /* x is inf or NaN */
        y[0] = y[1] = x - x;
        return 0;
    }
    /* set z = scalbn(|x|,-ilogb(x)+23) */
    u.f = x;
    u.i &= (uint64_t)-1>>12;
    u.i |= (uint64_t)(0x3ff + 23)<<52;
    z = u.f;
    for (i=0; i < 2; i++) {
        tx[i] = (double)(int32_t)z;
        z     = (z-tx[i])*0x1p24;
    }
    tx[i] = z;
    /* skip zero terms, first term is non-zero */
    while (tx[i] == 0.0)
        i--;
    n = __rem_pio2_large(tx,ty,(int)(ix>>20)-(0x3ff+23),i+1,1);
    if (sign) {
        y[0] = -ty[0];
        y[1] = -ty[1];
        return -n;
    }
    y[0] = ty[0];
    y[1] = ty[1];
    return n;
}

double __cos(double x, double y)
{
    double_t hz,z,r,w;

    z  = x*x;
    w  = z*z;
    r  = z*(C1+z*(C2+z*C3)) + w*w*(C4+z*(C5+z*C6));
    hz = 0.5*z;
    w  = 1.0-hz;
    return w + (((1.0-w)-hz) + (z*r-x*y));
}

double __sin(double x, double y, int iy)
{
    double_t z,r,v,w;

    z = x*x;
    w = z*z;
    r = S2 + z*(S3 + z*S4) + z*w*(S5 + z*S6);
    v = z*x;
    if (iy == 0)
        return x + v*(S1 + z*r);
    else
        return x - ((z*(0.5*y - v*r) - y) - v*S1);
}

double __tan(double x, double y, int odd)
{
    double_t z, r, v, w, s, a;
    double w0, a0;
    uint32_t hx;
    int big, sign;

    GET_HIGH_WORD(hx,x);
    big = (hx&0x7fffffff) >= 0x3FE59428; /* |x| >= 0.6744 */
    if (big) {
        sign = hx>>31;
        if (sign) {
            x = -x;
            y = -y;
        }
        x = (pio4 - x) + (pio4lo - y);
        y = 0.0;
    }
    z = x * x;
    w = z * z;
    /*
     * Break x^5*(T[1]+x^2*T[2]+...) into
     * x^5(T[1]+x^4*T[3]+...+x^20*T[11]) +
     * x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12]))
     */
    r = T[1] + w*(T[3] + w*(T[5] + w*(T[7] + w*(T[9] + w*T[11]))));
    v = z*(T[2] + w*(T[4] + w*(T[6] + w*(T[8] + w*(T[10] + w*T[12])))));
    s = z * x;
    r = y + z*(s*(r + v) + y) + s*T[0];
    w = x + r;
    if (big) {
        s = 1 - 2*odd;
        v = s - 2.0 * (x + (r - w*w/(w + s)));
        return sign ? -v : v;
    }
    if (!odd)
        return w;
    /* -1.0/(x+r) has up to 2ulp error, so compute it accurately */
    w0 = w;
    SET_LOW_WORD(w0, 0);
    v = r - (w0 - x);       /* w0+v = r+x */
    a0 = a = -1.0 / w;
    SET_LOW_WORD(a0, 0);
    return a0 + a*(1.0 + a0*w0 + a0*v);
}

double acos(double x)
{
    double z,w,s,c,df;
    uint32_t hx,ix;

    GET_HIGH_WORD(hx, x);
    ix = hx & 0x7fffffff;
    /* |x| >= 1 or nan */
    if (ix >= 0x3ff00000) {
        uint32_t lx;

        GET_LOW_WORD(lx,x);
        if (((ix-0x3ff00000) | lx) == 0) { //-Wparenthesis
            /* acos(1)=0, acos(-1)=pi */
            if (hx >> 31)
                return 2*pio2_hi + 0x1p-120f;
            return 0;
        }
        return 0/(x-x);
    }
    /* |x| < 0.5 */
    if (ix < 0x3fe00000) {
        if (ix <= 0x3c600000)  /* |x| < 2**-57 */
            return pio2_hi + 0x1p-120f;
        return pio2_hi - (x - (pio2_lo-x*R(x*x)));
    }
    /* x < -0.5 */
    if (hx >> 31) {
        z = (1.0+x)*0.5;
        s = sqrt(z);
        w = R(z)*s-pio2_lo;
        return 2*(pio2_hi - (s+w));
    }
    /* x > 0.5 */
    z = (1.0-x)*0.5;
    s = sqrt(z);
    df = s;
    SET_LOW_WORD(df,0);
    c = (z-df*df)/(s+df);
    w = R(z)*s+c;
    return 2*(df+w);
}

double asin(double x)
{
    double z,r,s;
    uint32_t hx,ix;

    GET_HIGH_WORD(hx, x);
    ix = hx & 0x7fffffff;
    /* |x| >= 1 or nan */
    if (ix >= 0x3ff00000) {
        uint32_t lx;
        GET_LOW_WORD(lx, x);
        if (((ix-0x3ff00000) | lx) == 0)
            /* asin(1) = +-pi/2 with inexact */
            return x*pio2_hi + 0x1p-120f;
        return 0/(x-x);
    }
    /* |x| < 0.5 */
    if (ix < 0x3fe00000) {
        /* if 0x1p-1022 <= |x| < 0x1p-26, avoid raising underflow */
        if (ix < 0x3e500000 && ix >= 0x00100000)
            return x;
        return x + x*R(x*x);
    }
    /* 1 > |x| >= 0.5 */
    z = (1 - fabs(x))*0.5;
    s = sqrt(z);
    r = R(z);
    if (ix >= 0x3fef3333) {  /* if |x| > 0.975 */
        x = pio2_hi-(2*(s+s*r)-pio2_lo);
    } else {
        double f,c;
        /* f+c = sqrt(z) */
        f = s;
        SET_LOW_WORD(f,0);
        c = (z-f*f)/(s+f);
        x = 0.5*pio2_hi - (2*s*r - (pio2_lo-2*c) - (0.5*pio2_hi-2*f));
    }
    if (hx >> 31)
        return -x;
    return x;
}

double atan(double x)
{
    double_t w,s1,s2,z;
    uint32_t ix,sign;
    int id;

    GET_HIGH_WORD(ix, x);
    sign = ix >> 31;
    ix &= 0x7fffffff;
    if (ix >= 0x44100000) {   /* if |x| >= 2^66 */
        if (isnan(x))
            return x;
        z = atanhi[3] + 0x1p-120f;
        return sign ? -z : z;
    }
    if (ix < 0x3fdc0000) {    /* |x| < 0.4375 */
        if (ix < 0x3e400000) {  /* |x| < 2^-27 */
            if (ix < 0x00100000)
                /* raise underflow for subnormal x */
                FORCE_EVAL((float)x);
            return x;
        }
        id = -1;
    } else {
        x = fabs(x);
        if (ix < 0x3ff30000) {  /* |x| < 1.1875 */
            if (ix < 0x3fe60000) {  /*  7/16 <= |x| < 11/16 */
                id = 0;
                x = (2.0*x-1.0)/(2.0+x);
            } else {                /* 11/16 <= |x| < 19/16 */
                id = 1;
                x = (x-1.0)/(x+1.0);
            }
        } else {
            if (ix < 0x40038000) {  /* |x| < 2.4375 */
                id = 2;
                x = (x-1.5)/(1.0+1.5*x);
            } else {                /* 2.4375 <= |x| < 2^66 */
                id = 3;
                x = -1.0/x;
            }
        }
    }
    /* end of argument reduction */
    z = x*x;
    w = z*z;
    /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
    s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
    s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
    if (id < 0)
        return x - x*(s1+s2);
    z = atanhi[id] - (x*(s1+s2) - atanlo[id] - x);
    return sign ? -z : z;
}

double atan2(double y, double x)
{
    double z;
    uint32_t m,lx,ly,ix,iy;

    if (isnan(x) || isnan(y))
        return x+y;
    EXTRACT_WORDS(ix, lx, x);
    EXTRACT_WORDS(iy, ly, y);
    if (((ix-0x3ff00000) | lx) == 0)  /* x = 1.0 */
        return atan(y);
    m = ((iy>>31)&1) | ((ix>>30)&2);  /* 2*sign(x)+sign(y) */
    ix = ix & 0x7fffffff;
    iy = iy & 0x7fffffff;

    /* when y = 0 */
    if ((iy|ly) == 0) {
        switch(m) {
        case 0:
        case 1: return y;   /* atan(+-0,+anything)=+-0 */
        case 2: return  pi; /* atan(+0,-anything) = pi */
        case 3: return -pi; /* atan(-0,-anything) =-pi */
        }
    }
    /* when x = 0 */
    if ((ix|lx) == 0)
        return m&1 ? -pi/2 : pi/2;
    /* when x is INF */
    if (ix == 0x7ff00000) {
        if (iy == 0x7ff00000) {
            switch(m) {
            case 0: return  pi/4;   /* atan(+INF,+INF) */
            case 1: return -pi/4;   /* atan(-INF,+INF) */
            case 2: return  3*pi/4; /* atan(+INF,-INF) */
            case 3: return -3*pi/4; /* atan(-INF,-INF) */
            }
        } else {
            switch(m) {
            case 0: return  0.0; /* atan(+...,+INF) */
            case 1: return -0.0; /* atan(-...,+INF) */
            case 2: return  pi;  /* atan(+...,-INF) */
            case 3: return -pi;  /* atan(-...,-INF) */
            }
        }
    }
    /* |y/x| > 0x1p64 */
    if (ix+(64<<20) < iy || iy == 0x7ff00000)
        return m&1 ? -pi/2 : pi/2;

    /* z = atan(|y/x|) without spurious underflow */
    if ((m&2) && iy+(64<<20) < ix)  /* |y/x| < 0x1p-64, x<0 */
        z = 0;
    else
        z = atan(fabs(y/x));
    switch (m) {
    case 0: return z;              /* atan(+,+) */
    case 1: return -z;             /* atan(-,+) */
    case 2: return pi - (z-pi_lo); /* atan(+,-) */
    default: /* case 3 */
        return (z-pi_lo) - pi; /* atan(-,-) */
    }
}

double ceil(double x)
{
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;

    if (e >= 0x3ff+52 || x == 0)
        return x;
    /* y = int(x) - x, where int(x) is an integer neighbor of x */
    if (u.i >> 63)
        y = x - toint + toint - x;
    else
        y = x + toint - toint - x;
    /* special case because of non-nearest rounding modes */
    if (e <= 0x3ff-1) {
        FORCE_EVAL(y);
        return u.i >> 63 ? -0.0 : 1;
    }
    if (y < 0)
        return x + y + 1;
    return x + y;
}

double cos(double x)
{
    double y[2];
    uint32_t ix;
    unsigned n;

    GET_HIGH_WORD(ix, x);
    ix &= 0x7fffffff;

    /* |x| ~< pi/4 */
    if (ix <= 0x3fe921fb) {
        if (ix < 0x3e46a09e) {  /* |x| < 2**-27 * sqrt(2) */
            /* raise inexact if x!=0 */
            FORCE_EVAL(x + 0x1p120f);
            return 1.0;
        }
        return __cos(x, 0);
    }

    /* cos(Inf or NaN) is NaN */
    if (ix >= 0x7ff00000)
        return x-x;

    /* argument reduction */
    n = __rem_pio2(x, y);
    switch (n&3) {
    case 0: return  __cos(y[0], y[1]);
    case 1: return -__sin(y[0], y[1], 1);
    case 2: return -__cos(y[0], y[1]);
    default:
        return  __sin(y[0], y[1], 1);
    }
}

double exp(double x)
{
    double_t hi, lo, c, xx, y;
    int k, sign;
    uint32_t hx;

    GET_HIGH_WORD(hx, x);
    sign = hx>>31;
    hx &= 0x7fffffff;  /* high word of |x| */

    /* special cases */
    if (hx >= 0x4086232b) {  /* if |x| >= 708.39... */
        if (isnan(x))
            return x;
        if (x > 709.782712893383973096) {
            /* overflow if x!=inf */
            x *= 0x1p1023;
            return x;
        }
        if (x < -708.39641853226410622) {
            /* underflow if x!=-inf */
            FORCE_EVAL((float)(-0x1p-149/x));
            if (x < -745.13321910194110842)
                return 0;
        }
    }

    /* argument reduction */
    if (hx > 0x3fd62e42) {  /* if |x| > 0.5 ln2 */
        if (hx >= 0x3ff0a2b2)  /* if |x| >= 1.5 ln2 */
            k = (int)(invln2*x + half[sign]);
        else
            k = 1 - sign - sign;
        hi = x - k*ln2hi;  /* k*ln2hi is exact here */
        lo = k*ln2lo;
        x = hi - lo;
    } else if (hx > 0x3e300000)  {  /* if |x| > 2**-28 */
        k = 0;
        hi = x;
        lo = 0;
    } else {
        /* inexact if x!=0 */
        FORCE_EVAL(0x1p1023 + x);
        return 1 + x;
    }

    /* x is now in primary range */
    xx = x*x;
    c = x - xx*(P1+xx*(P2+xx*(P3+xx*(P4+xx*P5))));
    y = 1 + (x*c/(2-c) - lo + hi);
    if (k == 0)
        return y;
    return scalbn(y, k);
}

double fabs(double x)
{
    union {double f; uint64_t i;} u = {x};
    u.i &= -1ULL/2;
    return u.f;
}

double floor(double x)
{
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;

    if (e >= 0x3ff+52 || x == 0)
        return x;
    /* y = int(x) - x, where int(x) is an integer neighbor of x */
    if (u.i >> 63)
        y = x - toint + toint - x;
    else
        y = x + toint - toint - x;
    /* special case because of non-nearest rounding modes */
    if (e <= 0x3ff-1) {
        FORCE_EVAL(y);
        return u.i >> 63 ? -1 : 0;
    }
    if (y > 0)
        return x + y - 1;
    return x + y;
}

double fmod(double x, double y)
{
    union {double f; uint64_t i;} ux = {x}, uy = {y};
    int ex = ux.i>>52 & 0x7ff;
    int ey = uy.i>>52 & 0x7ff;
    int sx = ux.i>>63;
    uint64_t i;

    /* in the followings uxi should be ux.i, but then gcc wrongly adds */
    /* float load/store to inner loops ruining performance and code size */
    uint64_t uxi = ux.i;

    if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
        return (x*y)/(x*y);
    if (uxi<<1 <= uy.i<<1) {
        if (uxi<<1 == uy.i<<1)
            return 0*x;
        return x;
    }

    /* normalize x and y */
    if (!ex) {
        for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
        uxi <<= -ex + 1;
    } else {
        uxi &= -1ULL >> 12;
        uxi |= 1ULL << 52;
    }
    if (!ey) {
        for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
        uy.i <<= -ey + 1;
    } else {
        uy.i &= -1ULL >> 12;
        uy.i |= 1ULL << 52;
    }

    /* x mod y */
    for (; ex > ey; ex--) {
        i = uxi - uy.i;
        if (i >> 63 == 0) {
            if (i == 0)
                return 0*x;
            uxi = i;
        }
        uxi <<= 1;
    }
    i = uxi - uy.i;
    if (i >> 63 == 0) {
        if (i == 0)
            return 0*x;
        uxi = i;
    }
    for (; uxi>>52 == 0; uxi <<= 1, ex--);

    /* scale result */
    if (ex > 0) {
        uxi -= 1ULL << 52;
        uxi |= (uint64_t)ex << 52;
    } else {
        uxi >>= -ex + 1;
    }
    uxi |= (uint64_t)sx << 63;
    ux.i = uxi;
    return ux.f;
}

double frexp(double x, int *e)
{
    union { double d; uint64_t i; } y = { x };
    int ee = y.i>>52 & 0x7ff;

    if (!ee) {
        if (x) {
            x = frexp(x*0x1p64, e);
            *e -= 64;
        } else *e = 0;
        return x;
    } else if (ee == 0x7ff) {
        return x;
    }

    *e = ee - 0x3fe;
    y.i &= 0x800fffffffffffffull;
    y.i |= 0x3fe0000000000000ull;
    return y.d;
}

double ldexp(double x, int n)
{
    return scalbn(x, n);
}

double log(double x)
{
    union {double f; uint64_t i;} u = {x};
    double_t hfsq,f,s,z,R,w,t1,t2,dk;
    uint32_t hx;
    int k;

    hx = u.i>>32;
    k = 0;
    if (hx < 0x00100000 || hx>>31) {
        if (u.i<<1 == 0)
            return -1/(x*x);  /* log(+-0)=-inf */
        if (hx>>31)
            return (x-x)/0.0; /* log(-#) = NaN */
        /* subnormal number, scale x up */
        k -= 54;
        x *= 0x1p54;
        u.f = x;
        hx = u.i>>32;
    } else if (hx >= 0x7ff00000) {
        return x;
    } else if (hx == 0x3ff00000 && u.i<<32 == 0)
        return 0;

    /* reduce x into [sqrt(2)/2, sqrt(2)] */
    hx += 0x3ff00000 - 0x3fe6a09e;
    k += (int)(hx>>20) - 0x3ff;
    hx = (hx&0x000fffff) + 0x3fe6a09e;
    u.i = (uint64_t)hx<<32 | (u.i&0xffffffff);
    x = u.f;

    f = x - 1.0;
    hfsq = 0.5*f*f;
    s = f/(2.0+f);
    z = s*s;
    w = z*z;
    t1 = w*(Lg2+w*(Lg4+w*Lg6));
    t2 = z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7)));
    R = t2 + t1;
    dk = k;
    return s*(hfsq+R) + dk*ln2_lo - hfsq + f + dk*ln2_hi;
}

double pow(double x, double y)
{
    double z,ax,z_h,z_l,p_h,p_l;
    double y1,t1,t2,r,s,t,u,v,w;
    int32_t i,j,k,yisint,n;
    int32_t hx,hy,ix,iy;
    uint32_t lx,ly;

    EXTRACT_WORDS(hx, lx, x);
    EXTRACT_WORDS(hy, ly, y);
    ix = hx & 0x7fffffff;
    iy = hy & 0x7fffffff;

    /* x**0 = 1, even if x is NaN */
    if ((iy|ly) == 0)
        return 1.0;
    /* 1**y = 1, even if y is NaN */
    if (hx == 0x3ff00000 && lx == 0)
        return 1.0;
    /* NaN if either arg is NaN */
    if (ix > 0x7ff00000 || (ix == 0x7ff00000 && lx != 0) ||
        iy > 0x7ff00000 || (iy == 0x7ff00000 && ly != 0))
        return x + y;

    /* determine if y is an odd int when x < 0
     * yisint = 0       ... y is not an integer
     * yisint = 1       ... y is an odd int
     * yisint = 2       ... y is an even int
     */
    yisint = 0;
    if (hx < 0) {
        if (iy >= 0x43400000)
            yisint = 2; /* even integer y */
        else if (iy >= 0x3ff00000) {
            k = (iy>>20) - 0x3ff;  /* exponent */
            if (k > 20) {
                uint32_t j = ly>>(52-k);
                if ((j<<(52-k)) == ly)
                    yisint = 2 - (j&1);
            } else if (ly == 0) {
                uint32_t j = iy>>(20-k);
                if ((j<<(20-k)) == iy)
                    yisint = 2 - (j&1);
            }
        }
    }

    /* special value of y */
    if (ly == 0) {
        if (iy == 0x7ff00000) {  /* y is +-inf */
            if (((ix-0x3ff00000)|lx) == 0)  /* (-1)**+-inf is 1 */
                return 1.0;
            else if (ix >= 0x3ff00000) /* (|x|>1)**+-inf = inf,0 */
                return hy >= 0 ? y : 0.0;
            else                       /* (|x|<1)**+-inf = 0,inf */
                return hy >= 0 ? 0.0 : -y;
        }
        if (iy == 0x3ff00000) {    /* y is +-1 */
            if (hy >= 0)
                return x;
            y = 1/x;
            return y;
        }
        if (hy == 0x40000000)    /* y is 2 */
            return x*x;
        if (hy == 0x3fe00000) {  /* y is 0.5 */
            if (hx >= 0)     /* x >= +0 */
                return sqrt(x);
        }
    }

    ax = fabs(x);
    /* special value of x */
    if (lx == 0) {
        if (ix == 0x7ff00000 || ix == 0 || ix == 0x3ff00000) { /* x is +-0,+-inf,+-1 */
            z = ax;
            if (hy < 0)   /* z = (1/|x|) */
                z = 1.0/z;
            if (hx < 0) {
                if (((ix-0x3ff00000)|yisint) == 0) {
                    z = (z-z)/(z-z); /* (-1)**non-int is NaN */
                } else if (yisint == 1)
                    z = -z;          /* (x<0)**odd = -(|x|**odd) */
            }
            return z;
        }
    }

    s = 1.0; /* sign of result */
    if (hx < 0) {
        if (yisint == 0) /* (x<0)**(non-int) is NaN */
            return (x-x)/(x-x);
        if (yisint == 1) /* (x<0)**(odd int) */
            s = -1.0;
    }

    /* |y| is huge */
    if (iy > 0x41e00000) { /* if |y| > 2**31 */
        if (iy > 0x43f00000) {  /* if |y| > 2**64, must o/uflow */
            if (ix <= 0x3fefffff)
                return hy < 0 ? huge*huge : tiny*tiny;
            if (ix >= 0x3ff00000)
                return hy > 0 ? huge*huge : tiny*tiny;
        }
        /* over/underflow if x is not close to one */
        if (ix < 0x3fefffff)
            return hy < 0 ? s*huge*huge : s*tiny*tiny;
        if (ix > 0x3ff00000)
            return hy > 0 ? s*huge*huge : s*tiny*tiny;
        /* now |1-x| is tiny <= 2**-20, suffice to compute
           log(x) by x-x^2/2+x^3/3-x^4/4 */
        t = ax - 1.0;       /* t has 20 trailing zeros */
        w = (t*t)*(0.5 - t*(0.3333333333333333333333-t*0.25));
        u = ivln2_h*t;      /* ivln2_h has 21 sig. bits */
        v = t*ivln2_l - w*ivln2;
        t1 = u + v;
        SET_LOW_WORD(t1, 0);
        t2 = v - (t1-u);
    } else {
        double ss,s2,s_h,s_l,t_h,t_l;
        n = 0;
        /* take care subnormal number */
        if (ix < 0x00100000) {
            ax *= two53;
            n -= 53;
            GET_HIGH_WORD(ix,ax);
        }
        n += ((ix)>>20) - 0x3ff;
        j = ix & 0x000fffff;
        /* determine interval */
        ix = j | 0x3ff00000;   /* normalize ix */
        if (j <= 0x3988E)      /* |x|<sqrt(3/2) */
            k = 0;
        else if (j < 0xBB67A)  /* |x|<sqrt(3)   */
            k = 1;
        else {
            k = 0;
            n += 1;
            ix -= 0x00100000;
        }
        SET_HIGH_WORD(ax, ix);

        /* compute ss = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
        u = ax - bp[k];        /* bp[0]=1.0, bp[1]=1.5 */
        v = 1.0/(ax+bp[k]);
        ss = u*v;
        s_h = ss;
        SET_LOW_WORD(s_h, 0);
        /* t_h=ax+bp[k] High */
        t_h = 0.0;
        SET_HIGH_WORD(t_h, ((ix>>1)|0x20000000) + 0x00080000 + (k<<18));
        t_l = ax - (t_h-bp[k]);
        s_l = v*((u-s_h*t_h)-s_h*t_l);
        /* compute log(ax) */
        s2 = ss*ss;
        r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));
        r += s_l*(s_h+ss);
        s2 = s_h*s_h;
        t_h = 3.0 + s2 + r;
        SET_LOW_WORD(t_h, 0);
        t_l = r - ((t_h-3.0)-s2);
        /* u+v = ss*(1+...) */
        u = s_h*t_h;
        v = s_l*t_h + t_l*ss;
        /* 2/(3log2)*(ss+...) */
        p_h = u + v;
        SET_LOW_WORD(p_h, 0);
        p_l = v - (p_h-u);
        z_h = cp_h*p_h;        /* cp_h+cp_l = 2/(3*log2) */
        z_l = cp_l*p_h+p_l*cp + dp_l[k];
        /* log2(ax) = (ss+..)*2/(3*log2) = n + dp_h + z_h + z_l */
        t = (double)n;
        t1 = ((z_h + z_l) + dp_h[k]) + t;
        SET_LOW_WORD(t1, 0);
        t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
    }

    /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
    y1 = y;
    SET_LOW_WORD(y1, 0);
    p_l = (y-y1)*t1 + y*t2;
    p_h = y1*t1;
    z = p_l + p_h;
    EXTRACT_WORDS(j, i, z);
    if (j >= 0x40900000) {                      /* z >= 1024 */
        if (((j-0x40900000)|i) != 0)        /* if z > 1024 */
            return s*huge*huge;         /* overflow */
        if (p_l + ovt > z - p_h)
            return s*huge*huge;         /* overflow */
    } else if ((j&0x7fffffff) >= 0x4090cc00) {  /* z <= -1075 */  // FIXME: instead of abs(j) use unsigned j
        if (((j-0xc090cc00)|i) != 0)        /* z < -1075 */
            return s*tiny*tiny;         /* underflow */
        if (p_l <= z - p_h)
            return s*tiny*tiny;         /* underflow */
    }
    /*
     * compute 2**(p_h+p_l)
     */
    i = j & 0x7fffffff;
    k = (i>>20) - 0x3ff;
    n = 0;
    if (i > 0x3fe00000) {  /* if |z| > 0.5, set n = [z+0.5] */
        n = j + (0x00100000>>(k+1));
        k = ((n&0x7fffffff)>>20) - 0x3ff;  /* new k for n */
        t = 0.0;
        SET_HIGH_WORD(t, n & ~(0x000fffff>>k));
        n = ((n&0x000fffff)|0x00100000)>>(20-k);
        if (j < 0)
            n = -n;
        p_h -= t;
    }
    t = p_l + p_h;
    SET_LOW_WORD(t, 0);
    u = t*lg2_h;
    v = (p_l-(t-p_h))*lg2 + t*lg2_l;
    z = u + v;
    w = v - (z-u);
    t = z*z;
    t1 = z - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
    r = (z*t1)/(t1-2.0) - (w + z*w);
    z = 1.0 - (r-z);
    GET_HIGH_WORD(j, z);
    j += n<<20;
    if ((j>>20) <= 0)  /* subnormal output */
        z = scalbn(z,n);
    else
        SET_HIGH_WORD(z, j);
    return s*z;
}

double round(double x)
{
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;

    if (e >= 0x3ff+52)
        return x;
    if (u.i >> 63)
        x = -x;
    if (e < 0x3ff-1) {
        /* raise inexact if x!=0 */
        FORCE_EVAL(x + toint);
        return 0*u.f;
    }
    y = x + toint - toint - x;
    if (y > 0.5)
        y = y + x - 1;
    else if (y <= -0.5)
        y = y + x + 1;
    else
        y = y + x;
    if (u.i >> 63)
        y = -y;
    return y;
}

double sin(double x)
{
    double y[2];
    uint32_t ix;
    unsigned n;

    /* High word of x. */
    GET_HIGH_WORD(ix, x);
    ix &= 0x7fffffff;

    /* |x| ~< pi/4 */
    if (ix <= 0x3fe921fb) {
        if (ix < 0x3e500000) {  /* |x| < 2**-26 */
            /* raise inexact if x != 0 and underflow if subnormal*/
            FORCE_EVAL(ix < 0x00100000 ? x/0x1p120f : x+0x1p120f);
            return x;
        }
        return __sin(x, 0.0, 0);
    }

    /* sin(Inf or NaN) is NaN */
    if (ix >= 0x7ff00000)
        return x - x;

    /* argument reduction needed */
    n = __rem_pio2(x, y);
    switch (n&3) {
    case 0: return  __sin(y[0], y[1], 1);
    case 1: return  __cos(y[0], y[1]);
    case 2: return -__sin(y[0], y[1], 1);
    default:
        return -__cos(y[0], y[1]);
    }
}

double sqrt(double x)
{
    double z;
    int32_t sign = (int)0x80000000;
    int32_t ix0,s0,q,m,t,i;
    uint32_t r,t1,s1,ix1,q1;

    EXTRACT_WORDS(ix0, ix1, x);

    /* take care of Inf and NaN */
    if ((ix0&0x7ff00000) == 0x7ff00000) {
        return x*x + x;  /* sqrt(NaN)=NaN, sqrt(+inf)=+inf, sqrt(-inf)=sNaN */
    }
    /* take care of zero */
    if (ix0 <= 0) {
        if (((ix0&~sign)|ix1) == 0)
            return x;  /* sqrt(+-0) = +-0 */
        if (ix0 < 0)
            return (x-x)/(x-x);  /* sqrt(-ve) = sNaN */
    }
    /* normalize x */
    m = ix0>>20;
    if (m == 0) {  /* subnormal x */
        while (ix0 == 0) {
            m -= 21;
            ix0 |= (ix1>>11);
            ix1 <<= 21;
        }
        for (i=0; (ix0&0x00100000) == 0; i++)
            ix0<<=1;
        m -= i - 1;
        ix0 |= ix1>>(32-i);
        ix1 <<= i;
    }
    m -= 1023;    /* unbias exponent */
    ix0 = (ix0&0x000fffff)|0x00100000;
    if (m & 1) {  /* odd m, double x to make it even */
        ix0 += ix0 + ((ix1&sign)>>31);
        ix1 += ix1;
    }
    m >>= 1;      /* m = [m/2] */

    /* generate sqrt(x) bit by bit */
    ix0 += ix0 + ((ix1&sign)>>31);
    ix1 += ix1;
    q = q1 = s0 = s1 = 0;  /* [q,q1] = sqrt(x) */
    r = 0x00200000;        /* r = moving bit from right to left */

    while (r != 0) {
        t = s0 + r;
        if (t <= ix0) {
            s0   = t + r;
            ix0 -= t;
            q   += r;
        }
        ix0 += ix0 + ((ix1&sign)>>31);
        ix1 += ix1;
        r >>= 1;
    }

    r = sign;
    while (r != 0) {
        t1 = s1 + r;
        t  = s0;
        if (t < ix0 || (t == ix0 && t1 <= ix1)) {
            s1 = t1 + r;
            if ((t1&sign) == sign && (s1&sign) == 0)
                s0++;
            ix0 -= t;
            if (ix1 < t1)
                ix0--;
            ix1 -= t1;
            q1 += r;
        }
        ix0 += ix0 + ((ix1&sign)>>31);
        ix1 += ix1;
        r >>= 1;
    }

    /* use floating add to find out rounding direction */
    if ((ix0|ix1) != 0) {
        z = 1.0 - tiny; /* raise inexact flag */
        if (z >= 1.0) {
            z = 1.0 + tiny;
            if (q1 == (uint32_t)0xffffffff) {
                q1 = 0;
                q++;
            } else if (z > 1.0) {
                if (q1 == (uint32_t)0xfffffffe)
                    q++;
                q1 += 2;
            } else
                q1 += q1 & 1;
        }
    }
    ix0 = (q>>1) + 0x3fe00000;
    ix1 = q1>>1;
    if (q&1)
        ix1 |= sign;
    ix0 += m << 20;
    INSERT_WORDS(z, ix0, ix1);
    return z;
}

double tan(double x)
{
    double y[2];
    uint32_t ix;
    unsigned n;

    GET_HIGH_WORD(ix, x);
    ix &= 0x7fffffff;

    /* |x| ~< pi/4 */
    if (ix <= 0x3fe921fb) {
        if (ix < 0x3e400000) { /* |x| < 2**-27 */
            /* raise inexact if x!=0 and underflow if subnormal */
            FORCE_EVAL(ix < 0x00100000 ? x/0x1p120f : x+0x1p120f);
            return x;
        }
        return __tan(x, 0.0, 0);
    }

    /* tan(Inf or NaN) is NaN */
    if (ix >= 0x7ff00000)
        return x - x;

    /* argument reduction */
    n = __rem_pio2(x, y);
    return __tan(y[0], y[1], n&1);
}
