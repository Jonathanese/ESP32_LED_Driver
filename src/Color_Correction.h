#pragma once
#include <Arduino.h>
#define CORRECTION_PRECISION 257
#define CORRECTION_R 255
#define CORRECTION_G 203
#define CORRECTION_B 155
#define CORRECTION_W 168
const uint16_t GAMMA_LUT[] = {
  0,0,2,4,7,11,17,24,32,42,53,65,79,94,111,129,
  148,169,192,216,242,270,299,330,362,396,432,469,508,549,591,635,
  681,729,779,830,883,938,995,1053,1113,1175,1239,1305,1373,1443,1514,1587,
  1663,1740,1819,1900,1983,2068,2155,2243,2334,2427,2521,2618,2717,2817,2920,3024,
  3131,3240,3350,3463,3578,3694,3813,3934,4057,4182,4309,4438,4570,4703,4838,4976,
  5115,5257,5401,5547,5695,5845,5998,6152,6309,6468,6629,6792,6957,7124,7294,7466,
  7640,7816,7994,8175,8358,8543,8730,8919,9111,9305,9501,9699,9900,10102,10307,10515,
  10724,10936,11150,11366,11585,11806,12029,12254,12482,12712,12944,13179,13416,13655,13896,14140,
  14386,14635,14885,15138,15394,15652,15912,16174,16439,16706,16975,17247,17521,17798,18077,18358,
  18642,18928,19216,19507,19800,20095,20393,20694,20996,21301,21609,21919,22231,22546,22863,23182,
  23504,23829,24156,24485,24817,25151,25487,25826,26168,26512,26858,27207,27558,27912,28268,28627,
  28988,29351,29717,30086,30457,30830,31206,31585,31966,32349,32735,33124,33514,33908,34304,34702,
  35103,35507,35913,36321,36732,37146,37562,37981,38402,38825,39252,39680,40112,40546,40982,41421,
  41862,42306,42753,43202,43654,44108,44565,45025,45487,45951,46418,46888,47360,47835,48313,48793,
  49275,49761,50249,50739,51232,51728,52226,52727,53230,53736,54245,54756,55270,55787,56306,56828,
  57352,57879,58409,58941,59476,60014,60554,61097,61642,62190,62741,63295,63851,64410,64971,65535 };

