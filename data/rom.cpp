/*
This file was produced by 3D Exploration Plugin: CPP Export filter.

3D Exploration

Copyright (c) 1999-2000 X Dimension Software

WWW         http://www.xdsoft.com/explorer/
eMail       info@xdsoft.com
*/
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>


struct sample_MATERIAL{
 GLfloat ambient[3];
 GLfloat diffuse[3];
 GLfloat specular[3];
 GLfloat emission[3];
 GLfloat alpha;
 GLfloat phExp;
 int   texture;
};

static sample_MATERIAL materials [4] = {
 {{0.0f,0.0f,0.0f},	{0.784314f,0.784314f,0.784314f},	{0.0f,0.0f,0.0f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //Default
 {{0.0f,0.0f,0.0f},	{0.0f,0.501961f,1.0f},	{0.0f,0.0f,0.0f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //theodor
 {{0.0f,0.0f,0.0f},	{0.0f,0.501961f,0.0f},	{0.0f,0.0f,0.0f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //kaare_konradi
 {{0.0f,0.0f,0.0f},	{1.0f,0.501961f,1.0f},	{0.0f,0.0f,0.0f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1} //kaare_valebrokk
};

// 176 Verticies
// 948 Texture Coordinates
// 316 Triangles

static GLint face_indicies[316][6] = {
// Object #-1
	{2,0,1 ,0,1,2 }, {173,174,163 ,3,4,5 }, {163,3,0 ,6,7,8 }, {168,173,163 ,9,10,11 },
	{2,168,0 ,12,13,14 }, {168,163,0 ,15,16,17 }, {7,3,163 ,18,19,20 },
	{2,166,168 ,21,22,23 }, {164,7,163 ,24,25,26 }, {6,7,164 ,27,28,29 },
	{6,166,2 ,30,31,32 }, {166,6,164 ,33,34,35 }, {4,1,0 ,36,37,38 },
	{4,5,1 ,39,40,41 }, {5,2,1 ,42,43,44 }, {5,6,162 ,45,46,47 },
	{5,167,2 ,48,49,50 }, {5,162,167 ,51,52,53 }, {3,4,0 ,54,55,56 },
	{7,4,160 ,57,58,59 }, {160,4,3 ,60,61,62 }, {159,157,156 ,63,64,65 },
	{159,158,157 ,66,67,68 }, {9,32,8 ,69,70,71 }, {33,32,9 ,72,73,74 },
	{10,33,9 ,75,76,77 }, {34,33,10 ,78,79,80 }, {11,34,10 ,81,82,83 },
	{35,34,11 ,84,85,86 }, {12,35,11 ,87,88,89 }, {36,35,12 ,90,91,92 },
	{13,36,12 ,93,94,95 }, {37,36,13 ,96,97,98 }, {14,37,13 ,99,100,101 },
	{38,37,14 ,102,103,104 }, {39,14,15 ,105,106,107 }, {39,38,14 ,108,109,110 },
	{40,15,16 ,111,112,113 }, {40,39,15 ,114,115,116 }, {41,16,17 ,117,118,119 },
	{41,40,16 ,120,121,122 }, {42,17,18 ,123,124,125 }, {42,41,17 ,126,127,128 },
	{43,18,19 ,129,130,131 }, {43,42,18 ,132,133,134 }, {44,19,20 ,135,136,137 },
	{44,43,19 ,138,139,140 }, {45,20,21 ,141,142,143 }, {45,44,20 ,144,145,146 },
	{46,21,22 ,147,148,149 }, {46,45,21 ,150,151,152 }, {47,22,23 ,153,154,155 },
	{47,46,22 ,156,157,158 }, {48,23,24 ,159,160,161 }, {48,47,23 ,162,163,164 },
	{49,24,25 ,165,166,167 }, {49,48,24 ,168,169,170 }, {50,25,26 ,171,172,173 },
	{50,49,25 ,174,175,176 }, {27,50,26 ,177,178,179 }, {51,50,27 ,180,181,182 },
	{28,51,27 ,183,184,185 }, {52,51,28 ,186,187,188 }, {29,52,28 ,189,190,191 },
	{53,52,29 ,192,193,194 }, {30,53,29 ,195,196,197 }, {54,53,30 ,198,199,200 },
	{31,54,30 ,201,202,203 }, {55,54,31 ,204,205,206 }, {8,55,31 ,207,208,209 },
	{32,55,8 ,210,211,212 }, {81,80,82 ,213,214,215 }, {82,80,83 ,216,217,218 },
	{83,80,84 ,219,220,221 }, {84,80,85 ,222,223,224 }, {85,80,86 ,225,226,227 },
	{80,103,86 ,228,229,230 }, {103,87,86 ,231,232,233 }, {103,88,87 ,234,235,236 },
	{103,89,88 ,237,238,239 }, {103,90,89 ,240,241,242 }, {103,102,90 ,243,244,245 },
	{102,101,90 ,246,247,248 }, {101,100,90 ,249,250,251 }, {100,91,90 ,252,253,254 },
	{100,92,91 ,255,256,257 }, {100,93,92 ,258,259,260 }, {100,99,93 ,261,262,263 },
	{99,98,93 ,264,265,266 }, {98,97,93 ,267,268,269 }, {97,94,93 ,270,271,272 },
	{96,95,94 ,273,274,275 }, {97,96,94 ,276,277,278 }, {79,56,32 ,279,280,281 },
	{33,79,32 ,282,283,284 }, {32,57,55 ,285,286,287 }, {56,57,32 ,288,289,290 },
	{57,58,54 ,291,292,293 }, {55,57,54 ,294,295,296 }, {58,59,53 ,297,298,299 },
	{54,58,53 ,300,301,302 }, {53,60,52 ,303,304,305 }, {59,60,53 ,306,307,308 },
	{52,61,51 ,309,310,311 }, {60,61,52 ,312,313,314 }, {61,62,50 ,315,316,317 },
	{51,61,50 ,318,319,320 }, {63,49,50 ,321,322,323 }, {62,63,50 ,324,325,326 },
	{63,48,49 ,327,328,329 }, {63,64,48 ,330,331,332 }, {64,47,48 ,333,334,335 },
	{64,65,47 ,336,337,338 }, {66,46,47 ,339,340,341 }, {65,66,47 ,342,343,344 },
	{67,45,46 ,345,346,347 }, {66,67,46 ,348,349,350 }, {67,44,45 ,351,352,353 },
	{67,68,44 ,354,355,356 }, {69,43,44 ,357,358,359 }, {68,69,44 ,360,361,362 },
	{69,42,43 ,363,364,365 }, {69,70,42 ,366,367,368 }, {70,41,42 ,369,370,371 },
	{70,71,41 ,372,373,374 }, {72,40,41 ,375,376,377 }, {71,72,41 ,378,379,380 },
	{73,39,40 ,381,382,383 }, {72,73,40 ,384,385,386 }, {73,38,39 ,387,388,389 },
	{73,74,38 ,390,391,392 }, {38,75,37 ,393,394,395 }, {74,75,38 ,396,397,398 },
	{75,76,36 ,399,400,401 }, {37,75,36 ,402,403,404 }, {76,77,35 ,405,406,407 },
	{36,76,35 ,408,409,410 }, {35,78,34 ,411,412,413 }, {77,78,35 ,414,415,416 },
	{34,79,33 ,417,418,419 }, {78,79,34 ,420,421,422 }, {107,105,104 ,423,424,425 },
	{107,106,105 ,426,427,428 }, {56,80,81 ,429,430,431 }, {57,56,81 ,432,433,434 },
	{111,109,108 ,435,436,437 }, {111,110,109 ,438,439,440 }, {58,82,83 ,441,442,443 },
	{59,58,83 ,444,445,446 }, {115,113,112 ,447,448,449 }, {115,114,113 ,450,451,452 },
	{60,84,85 ,453,454,455 }, {61,60,85 ,456,457,458 }, {119,117,116 ,459,460,461 },
	{119,118,117 ,462,463,464 }, {87,62,86 ,465,466,467 }, {63,62,87 ,468,469,470 },
	{122,121,120 ,471,472,473 }, {123,122,120 ,474,475,476 }, {89,64,88 ,477,478,479 },
	{65,64,89 ,480,481,482 }, {126,125,124 ,483,484,485 }, {127,126,124 ,486,487,488 },
	{91,66,90 ,489,490,491 }, {67,66,91 ,492,493,494 }, {131,129,128 ,495,496,497 },
	{130,129,131 ,498,499,500 }, {93,68,92 ,501,502,503 }, {69,68,93 ,504,505,506 },
	{134,133,132 ,507,508,509 }, {135,134,132 ,510,511,512 }, {95,70,94 ,513,514,515 },
	{71,70,95 ,516,517,518 }, {138,137,136 ,519,520,521 }, {139,138,136 ,522,523,524 },
	{97,72,96 ,525,526,527 }, {73,72,97 ,528,529,530 }, {143,141,140 ,531,532,533 },
	{142,141,143 ,534,535,536 }, {74,98,99 ,537,538,539 }, {75,74,99 ,540,541,542 },
	{147,145,144 ,543,544,545 }, {147,146,145 ,546,547,548 }, {76,100,101 ,549,550,551 },
	{77,76,101 ,552,553,554 }, {151,149,148 ,555,556,557 }, {151,150,149 ,558,559,560 },
	{78,102,103 ,561,562,563 }, {79,78,103 ,564,565,566 }, {107,104,80 ,567,568,569 },
	{56,107,80 ,570,571,572 }, {105,103,80 ,573,574,575 }, {104,105,80 ,576,577,578 },
	{105,79,103 ,579,580,581 }, {106,79,105 ,582,583,584 }, {79,107,56 ,585,586,587 },
	{106,107,79 ,588,589,590 }, {58,108,82 ,591,592,593 }, {58,111,108 ,594,595,596 },
	{108,81,82 ,597,598,599 }, {108,109,81 ,600,601,602 }, {110,57,81 ,603,604,605 },
	{109,110,81 ,606,607,608 }, {110,111,58 ,609,610,611 }, {57,110,58 ,612,613,614 },
	{115,112,84 ,615,616,617 }, {60,115,84 ,618,619,620 }, {113,83,84 ,621,622,623 },
	{112,113,84 ,624,625,626 }, {113,59,83 ,627,628,629 }, {114,59,113 ,630,631,632 },
	{59,115,60 ,633,634,635 }, {114,115,59 ,636,637,638 }, {119,116,86 ,639,640,641 },
	{62,119,86 ,642,643,644 }, {117,85,86 ,645,646,647 }, {116,117,86 ,648,649,650 },
	{117,61,85 ,651,652,653 }, {118,61,117 ,654,655,656 }, {61,119,62 ,657,658,659 },
	{118,119,61 ,660,661,662 }, {123,120,87 ,663,664,665 }, {88,123,87 ,666,667,668 },
	{121,63,87 ,669,670,671 }, {120,121,87 ,672,673,674 }, {121,64,63 ,675,676,677 },
	{121,122,64 ,678,679,680 }, {64,123,88 ,681,682,683 }, {64,122,123 ,684,685,686 },
	{90,124,89 ,687,688,689 }, {127,124,90 ,690,691,692 }, {124,65,89 ,693,694,695 },
	{125,65,124 ,696,697,698 }, {126,66,65 ,699,700,701 }, {125,126,65 ,702,703,704 },
	{126,127,90 ,705,706,707 }, {66,126,90 ,708,709,710 }, {92,128,91 ,711,712,713 },
	{131,128,92 ,714,715,716 }, {128,67,91 ,717,718,719 }, {129,67,128 ,720,721,722 },
	{130,68,67 ,723,724,725 }, {129,130,67 ,726,727,728 }, {130,131,92 ,729,730,731 },
	{68,130,92 ,732,733,734 }, {135,132,93 ,735,736,737 }, {94,135,93 ,738,739,740 },
	{133,69,93 ,741,742,743 }, {132,133,93 ,744,745,746 }, {133,70,69 ,747,748,749 },
	{133,134,70 ,750,751,752 }, {70,135,94 ,753,754,755 }, {70,134,135 ,756,757,758 },
	{96,136,95 ,759,760,761 }, {139,136,96 ,762,763,764 }, {136,71,95 ,765,766,767 },
	{137,71,136 ,768,769,770 }, {138,72,71 ,771,772,773 }, {137,138,71 ,774,775,776 },
	{138,139,96 ,777,778,779 }, {72,138,96 ,780,781,782 }, {98,140,97 ,783,784,785 },
	{143,140,98 ,786,787,788 }, {140,73,97 ,789,790,791 }, {141,73,140 ,792,793,794 },
	{142,74,73 ,795,796,797 }, {141,142,73 ,798,799,800 }, {142,143,98 ,801,802,803 },
	{74,142,98 ,804,805,806 }, {76,144,100 ,807,808,809 }, {76,147,144 ,810,811,812 },
	{144,99,100 ,813,814,815 }, {144,145,99 ,816,817,818 }, {146,75,99 ,819,820,821 },
	{145,146,99 ,822,823,824 }, {146,147,76 ,825,826,827 }, {75,146,76 ,828,829,830 },
	{151,148,102 ,831,832,833 }, {78,151,102 ,834,835,836 }, {149,101,102 ,837,838,839 },
	{148,149,102 ,840,841,842 }, {149,77,101 ,843,844,845 }, {150,77,149 ,846,847,848 },
	{77,151,78 ,849,850,851 }, {150,151,77 ,852,853,854 }, {4,155,152 ,855,856,857 },
	{4,7,155 ,858,859,860 }, {5,152,153 ,861,862,863 }, {5,4,152 ,864,865,866 },
	{154,5,153 ,867,868,869 }, {6,5,154 ,870,871,872 }, {161,6,154 ,873,874,875 },
	{161,154,155 ,876,877,878 }, {7,161,155 ,879,880,881 }, {155,156,152 ,882,883,884 },
	{159,156,155 ,885,886,887 }, {156,153,152 ,888,889,890 }, {156,157,153 ,891,892,893 },
	{157,154,153 ,894,895,896 }, {157,158,154 ,897,898,899 }, {158,155,154 ,900,901,902 },
	{158,159,155 ,903,904,905 }, {171,170,175 ,906,907,908 }, {172,171,175 ,909,910,911 },
	{172,175,173 ,912,913,914 }, {173,175,174 ,915,916,917 }, {170,166,164 ,918,919,920 },
	{170,171,166 ,921,922,923 }, {169,173,168 ,924,925,926 }, {169,172,173 ,927,928,929 },
	{171,172,169 ,930,931,932 }, {166,171,169 ,933,934,935 }, {174,165,163 ,936,937,938 },
	{170,164,165 ,939,940,941 }, {175,170,165 ,942,943,944 }, {175,165,174 ,945,946,947 }
};
static GLfloat vertices [176][3] = {
{-0.4f,-0.5f,-0.2f},{0.4f,-0.5f,-0.2f},{0.4f,0.166667f,-0.2f},
{-0.4f,0.166667f,-0.2f},{-0.4f,-0.5f,0.2f},{0.4f,-0.5f,0.2f},
{0.4f,0.166667f,0.2f},{-0.4f,0.166667f,0.2f},{0.133333f,-0.166667f,-0.2f},
{0.12879f,-0.132157f,-0.2f},{0.11547f,-0.1f,-0.2f},{0.0942809f,-0.0723858f,-0.2f},
{0.0666667f,-0.0511966f,-0.2f},{0.0345092f,-0.0378766f,-0.2f},{0.0f,-0.0333333f,-0.2f},
{-0.0345092f,-0.0378766f,-0.2f},{-0.0666667f,-0.0511966f,-0.2f},{-0.0942809f,-0.0723858f,-0.2f},
{-0.11547f,-0.1f,-0.2f},{-0.12879f,-0.132157f,-0.2f},{-0.133333f,-0.166667f,-0.2f},
{-0.12879f,-0.201176f,-0.2f},{-0.11547f,-0.233333f,-0.2f},{-0.0942809f,-0.260948f,-0.2f},
{-0.0666667f,-0.282137f,-0.2f},{-0.0345092f,-0.295457f,-0.2f},{0.0f,-0.3f,-0.2f},
{0.0345092f,-0.295457f,-0.2f},{0.0666667f,-0.282137f,-0.2f},{0.0942809f,-0.260948f,-0.2f},
{0.11547f,-0.233333f,-0.2f},{0.12879f,-0.201176f,-0.2f},{0.133333f,-0.166667f,-0.186667f},
{0.12879f,-0.132157f,-0.186667f},{0.11547f,-0.1f,-0.186667f},{0.0942809f,-0.0723858f,-0.186667f},
{0.0666667f,-0.0511966f,-0.186667f},{0.0345092f,-0.0378766f,-0.186667f},{0.0f,-0.0333333f,-0.186667f},
{-0.0345092f,-0.0378766f,-0.186667f},{-0.0666667f,-0.0511966f,-0.186667f},{-0.0942809f,-0.0723858f,-0.186667f},
{-0.11547f,-0.1f,-0.186667f},{-0.12879f,-0.132157f,-0.186667f},{-0.133333f,-0.166667f,-0.186667f},
{-0.12879f,-0.201176f,-0.186667f},{-0.11547f,-0.233333f,-0.186667f},{-0.0942809f,-0.260948f,-0.186667f},
{-0.0666667f,-0.282137f,-0.186667f},{-0.0345092f,-0.295457f,-0.186667f},{0.0f,-0.3f,-0.186667f},
{0.0345092f,-0.295457f,-0.186667f},{0.0666667f,-0.282137f,-0.186667f},{0.0942809f,-0.260948f,-0.186667f},
{0.11547f,-0.233333f,-0.186667f},{0.12879f,-0.201176f,-0.186667f},{0.0714708f,-0.166667f,-0.165333f},
{0.0690354f,-0.185165f,-0.165333f},{0.0618955f,-0.202402f,-0.165333f},{0.0505375f,-0.217204f,-0.165333f},
{0.0357354f,-0.228562f,-0.165333f},{0.018498f,-0.235702f,-0.165333f},{0.0f,-0.238137f,-0.165333f},
{-0.018498f,-0.235702f,-0.165333f},{-0.0357354f,-0.228562f,-0.165333f},{-0.0505375f,-0.217204f,-0.165333f},
{-0.0618955f,-0.202402f,-0.165333f},{-0.0690354f,-0.185165f,-0.165333f},{-0.0714708f,-0.166667f,-0.165333f},
{-0.0690354f,-0.148169f,-0.165333f},{-0.0618955f,-0.130931f,-0.165333f},{-0.0505375f,-0.116129f,-0.165333f},
{-0.0357354f,-0.104771f,-0.165333f},{-0.018498f,-0.0976312f,-0.165333f},{0.0f,-0.0951959f,-0.165333f},
{0.018498f,-0.0976312f,-0.165333f},{0.0357354f,-0.104771f,-0.165333f},{0.0505375f,-0.116129f,-0.165333f},
{0.0618955f,-0.130931f,-0.165333f},{0.0690354f,-0.148169f,-0.165333f},{0.0378498f,-0.166667f,-0.193333f},
{0.0365601f,-0.176463f,-0.193333f},{0.0327789f,-0.185592f,-0.193333f},{0.0267638f,-0.19343f,-0.193333f},
{0.0189249f,-0.199446f,-0.193333f},{0.00979625f,-0.203227f,-0.193333f},{0.0f,-0.204516f,-0.193333f},
{-0.00979625f,-0.203227f,-0.193333f},{-0.0189249f,-0.199446f,-0.193333f},{-0.0267638f,-0.19343f,-0.193333f},
{-0.0327789f,-0.185592f,-0.193333f},{-0.0365601f,-0.176463f,-0.193333f},{-0.0378498f,-0.166667f,-0.193333f},
{-0.0365601f,-0.15687f,-0.193333f},{-0.0327789f,-0.147742f,-0.193333f},{-0.0267638f,-0.139903f,-0.193333f},
{-0.0189249f,-0.133888f,-0.193333f},{-0.00979625f,-0.130107f,-0.193333f},{0.0f,-0.128817f,-0.193333f},
{0.00979625f,-0.130107f,-0.193333f},{0.0189249f,-0.133888f,-0.193333f},{0.0267638f,-0.139903f,-0.193333f},
{0.0327789f,-0.147742f,-0.193333f},{0.0365601f,-0.15687f,-0.193333f},{0.0361584f,-0.164457f,-0.187534f},
{0.0354982f,-0.159442f,-0.187534f},{0.063995f,-0.151807f,-0.162965f},{0.0656604f,-0.164457f,-0.162965f},
{0.0324189f,-0.182832f,-0.187534f},{0.0343545f,-0.178159f,-0.187534f},{0.0628512f,-0.185795f,-0.162965f},
{0.0579683f,-0.197583f,-0.162965f},{0.0199927f,-0.196876f,-0.187534f},{0.0240055f,-0.193797f,-0.187534f},
{0.0448666f,-0.214658f,-0.162965f},{0.0347437f,-0.222425f,-0.162965f},{0.0022095f,-0.202825f,-0.187534f},
{0.00722428f,-0.202165f,-0.187534f},{0.01486f,-0.230662f,-0.162965f},{0.0022095f,-0.232327f,-0.162965f},
{-0.0114927f,-0.201021f,-0.187534f},{-0.0191284f,-0.229518f,-0.162965f},{-0.0309167f,-0.224635f,-0.162965f},
{-0.0161657f,-0.199086f,-0.187534f},{-0.0271302f,-0.190672f,-0.187534f},{-0.0479913f,-0.211533f,-0.162965f},
{-0.0557588f,-0.20141f,-0.162965f},{-0.0302094f,-0.186659f,-0.187534f},{-0.0354982f,-0.173891f,-0.187534f},
{-0.063995f,-0.181527f,-0.162965f},{-0.0656604f,-0.168876f,-0.162965f},{-0.0361584f,-0.168876f,-0.187534f},
{-0.0343545f,-0.155174f,-0.187534f},{-0.0628512f,-0.147538f,-0.162965f},{-0.0579683f,-0.13575f,-0.162965f},
{-0.0324189f,-0.150501f,-0.187534f},{-0.0240055f,-0.139536f,-0.187534f},{-0.0448666f,-0.118675f,-0.162965f},
{-0.0347437f,-0.110908f,-0.162965f},{-0.0199927f,-0.136457f,-0.187534f},{-0.00722428f,-0.131168f,-0.187534f},
{-0.01486f,-0.102672f,-0.162965f},{-0.0022095f,-0.101006f,-0.162965f},{-0.0022095f,-0.130508f,-0.187534f},
{0.0161657f,-0.134248f,-0.187534f},{0.0114927f,-0.132312f,-0.187534f},{0.0191284f,-0.103815f,-0.162965f},
{0.0309167f,-0.108698f,-0.162965f},{0.0302094f,-0.146674f,-0.187534f},{0.0271302f,-0.142661f,-0.187534f},
{0.0479913f,-0.1218f,-0.162965f},{0.0557588f,-0.131923f,-0.162965f},{-0.253333f,-0.353333f,0.166667f},
{0.253333f,-0.353333f,0.166667f},{0.253333f,0.02f,0.166667f},{-0.253333f,0.02f,0.166667f},
{-0.206667f,-0.306667f,0.186667f},{0.206667f,-0.306667f,0.186667f},{0.206667f,-0.0266667f,0.186667f},
{-0.206667f,-0.0266667f,0.186667f},{-0.4f,0.166667f,0.0f},{0.0f,0.166667f,0.2f},
{0.4f,0.166667f,0.0f},{0.0f,0.166667f,-0.2f},{0.0f,0.166667f,0.0f},
{0.0f,0.166667f,-0.1f},{0.2f,0.166667f,0.0f},{0.4f,0.166667f,-0.1f},
{0.2f,0.166667f,-0.2f},{0.2f,0.166667f,-0.1f},{0.0f,0.5f,0.0f},
{0.2f,0.5f,0.0f},{0.2f,0.5f,-0.1f},{0.2f,0.5f,-0.2f},
{0.0f,0.5f,-0.2f},{0.0f,0.5f,-0.1f}
};
static GLfloat textures [948][2] = {
{0.491935f,0.658654f},{0.00806452f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.25f,0.658654f},{0.00806452f,0.658654f},{0.00806452f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.658654f},{0.491935f,0.258013f},{0.00806452f,0.258013f},
{0.491935f,0.258013f},{0.25f,0.658654f},{0.00806452f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.508013f,0.742188f},{0.908654f,0.507813f},{0.508013f,0.507813f},
{0.508013f,0.742188f},{0.908654f,0.742188f},{0.491935f,0.258013f},
{0.508013f,0.742188f},{0.491935f,0.258013f},{0.908654f,0.507813f},
{0.508013f,0.742188f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.908654f,0.257813f},{0.508013f,0.492188f},{0.508013f,0.257813f},
{0.908654f,0.492188f},{0.508013f,0.492188f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.508013f,0.49219f},{0.908654f,0.257813f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.982963f,0.62941f},{1.0f,0.5f},{1.0f,0.5f},
{0.982963f,0.62941f},{1.0f,0.5f},{0.982963f,0.62941f},
{0.933013f,0.75f},{0.982963f,0.62941f},{0.982963f,0.62941f},
{0.933013f,0.75f},{0.982963f,0.62941f},{0.933013f,0.75f},
{0.853553f,0.853553f},{0.933013f,0.75f},{0.933013f,0.75f},
{0.853553f,0.853553f},{0.933013f,0.75f},{0.853553f,0.853553f},
{0.75f,0.933013f},{0.853553f,0.853553f},{0.853553f,0.853553f},
{0.75f,0.933013f},{0.853553f,0.853553f},{0.75f,0.933013f},
{0.62941f,0.982963f},{0.75f,0.933013f},{0.75f,0.933013f},
{0.62941f,0.982963f},{0.75f,0.933013f},{0.62941f,0.982963f},
{0.5f,1.0f},{0.62941f,0.982963f},{0.62941f,0.982963f},
{0.5f,1.0f},{0.62941f,0.982963f},{0.5f,1.0f},
{0.37059f,0.982963f},{0.5f,1.0f},{0.37059f,0.982963f},
{0.37059f,0.982963f},{0.5f,1.0f},{0.5f,1.0f},
{0.25f,0.933013f},{0.37059f,0.982963f},{0.25f,0.933013f},
{0.25f,0.933013f},{0.37059f,0.982963f},{0.37059f,0.982963f},
{0.146447f,0.853553f},{0.25f,0.933013f},{0.146447f,0.853553f},
{0.146447f,0.853553f},{0.25f,0.933013f},{0.25f,0.933013f},
{0.0669873f,0.75f},{0.146447f,0.853553f},{0.0669873f,0.75f},
{0.0669873f,0.75f},{0.146447f,0.853553f},{0.146447f,0.853553f},
{0.0170371f,0.62941f},{0.0669873f,0.75f},{0.0170371f,0.62941f},
{0.0170371f,0.62941f},{0.0669873f,0.75f},{0.0669873f,0.75f},
{0.0f,0.5f},{0.0170371f,0.62941f},{0.0f,0.5f},
{0.0f,0.5f},{0.0170371f,0.62941f},{0.0170371f,0.62941f},
{0.0170371f,0.37059f},{0.0f,0.5f},{0.0170371f,0.37059f},
{0.0170371f,0.37059f},{0.0f,0.5f},{0.0f,0.5f},
{0.0669873f,0.25f},{0.0170371f,0.37059f},{0.0669873f,0.25f},
{0.0669873f,0.25f},{0.0170371f,0.37059f},{0.0170371f,0.37059f},
{0.146447f,0.146447f},{0.0669873f,0.25f},{0.146447f,0.146447f},
{0.146447f,0.146447f},{0.0669873f,0.25f},{0.0669873f,0.25f},
{0.25f,0.0669873f},{0.146447f,0.146447f},{0.25f,0.0669873f},
{0.25f,0.0669873f},{0.146447f,0.146447f},{0.146447f,0.146447f},
{0.37059f,0.0170371f},{0.25f,0.0669873f},{0.37059f,0.0170371f},
{0.37059f,0.0170371f},{0.25f,0.0669873f},{0.25f,0.0669873f},
{0.5f,0.0f},{0.37059f,0.0170371f},{0.5f,0.0f},
{0.5f,0.0f},{0.37059f,0.0170371f},{0.37059f,0.0170371f},
{0.62941f,0.0170371f},{0.5f,0.0f},{0.5f,0.0f},
{0.62941f,0.0170371f},{0.5f,0.0f},{0.62941f,0.0170371f},
{0.75f,0.0669873f},{0.62941f,0.0170371f},{0.62941f,0.0170371f},
{0.75f,0.0669873f},{0.62941f,0.0170371f},{0.75f,0.0669873f},
{0.853553f,0.146447f},{0.75f,0.0669873f},{0.75f,0.0669873f},
{0.853553f,0.146447f},{0.75f,0.0669873f},{0.853553f,0.146447f},
{0.933013f,0.25f},{0.853553f,0.146447f},{0.853553f,0.146447f},
{0.933013f,0.25f},{0.853553f,0.146447f},{0.933013f,0.25f},
{0.982963f,0.37059f},{0.933013f,0.25f},{0.933013f,0.25f},
{0.982963f,0.37059f},{0.933013f,0.25f},{0.982963f,0.37059f},
{1.0f,0.5f},{0.982963f,0.37059f},{0.982963f,0.37059f},
{1.0f,0.5f},{0.982963f,0.37059f},{1.0f,0.5f},
{0.6371f,0.463264f},{0.641937f,0.5f},{0.622921f,0.429032f},
{0.622921f,0.429032f},{0.641937f,0.5f},{0.600364f,0.399636f},
{0.600364f,0.399636f},{0.641937f,0.5f},{0.570968f,0.377079f},
{0.570968f,0.377079f},{0.641937f,0.5f},{0.536736f,0.3629f},
{0.536736f,0.3629f},{0.641937f,0.5f},{0.5f,0.358063f},
{0.641937f,0.5f},{0.6371f,0.536736f},{0.5f,0.358063f},
{0.6371f,0.536736f},{0.463264f,0.3629f},{0.5f,0.358063f},
{0.6371f,0.536736f},{0.429032f,0.377079f},{0.463264f,0.3629f},
{0.6371f,0.536736f},{0.399636f,0.399636f},{0.429032f,0.377079f},
{0.6371f,0.536736f},{0.377079f,0.429032f},{0.399636f,0.399636f},
{0.6371f,0.536736f},{0.622921f,0.570968f},{0.377079f,0.429032f},
{0.622921f,0.570968f},{0.600364f,0.600364f},{0.377079f,0.429032f},
{0.600364f,0.600364f},{0.570968f,0.622921f},{0.377079f,0.429032f},
{0.570968f,0.622921f},{0.3629f,0.463264f},{0.377079f,0.429032f},
{0.570968f,0.622921f},{0.358063f,0.5f},{0.3629f,0.463264f},
{0.570968f,0.622921f},{0.3629f,0.536736f},{0.358063f,0.5f},
{0.570968f,0.622921f},{0.536736f,0.6371f},{0.3629f,0.536736f},
{0.536736f,0.6371f},{0.5f,0.641937f},{0.3629f,0.536736f},
{0.5f,0.641937f},{0.463264f,0.6371f},{0.3629f,0.536736f},
{0.463264f,0.6371f},{0.377079f,0.570968f},{0.3629f,0.536736f},
{0.429032f,0.622921f},{0.399636f,0.600364f},{0.377079f,0.570968f},
{0.463264f,0.6371f},{0.429032f,0.622921f},{0.377079f,0.570968f},
{0.758883f,0.569367f},{0.768015f,0.5f},{1.0f,0.5f},
{0.982963f,0.62941f},{0.758883f,0.569367f},{1.0f,0.5f},
{1.0f,0.5f},{0.758883f,0.430633f},{0.982963f,0.37059f},
{0.768015f,0.5f},{0.758883f,0.430633f},{1.0f,0.5f},
{0.758883f,0.430633f},{0.732108f,0.365992f},{0.933013f,0.25f},
{0.982963f,0.37059f},{0.758883f,0.430633f},{0.933013f,0.25f},
{0.732108f,0.365992f},{0.689515f,0.310485f},{0.853553f,0.146447f},
{0.933013f,0.25f},{0.732108f,0.365992f},{0.853553f,0.146447f},
{0.853553f,0.146447f},{0.634008f,0.267892f},{0.75f,0.0669873f},
{0.689515f,0.310485f},{0.634008f,0.267892f},{0.853553f,0.146447f},
{0.75f,0.0669873f},{0.569367f,0.241117f},{0.62941f,0.0170371f},
{0.634008f,0.267892f},{0.569367f,0.241117f},{0.75f,0.0669873f},
{0.569367f,0.241117f},{0.5f,0.231985f},{0.5f,0.0f},
{0.62941f,0.0170371f},{0.569367f,0.241117f},{0.5f,0.0f},
{0.430633f,0.241117f},{0.37059f,0.0170371f},{0.5f,0.0f},
{0.5f,0.231985f},{0.430633f,0.241117f},{0.5f,0.0f},
{0.430633f,0.241117f},{0.25f,0.0669873f},{0.37059f,0.0170371f},
{0.430633f,0.241117f},{0.365992f,0.267892f},{0.25f,0.0669873f},
{0.365992f,0.267892f},{0.146447f,0.146447f},{0.25f,0.0669873f},
{0.365992f,0.267892f},{0.310485f,0.310485f},{0.146447f,0.146447f},
{0.267892f,0.365992f},{0.0669873f,0.25f},{0.146447f,0.146447f},
{0.310485f,0.310485f},{0.267892f,0.365992f},{0.146447f,0.146447f},
{0.241117f,0.430633f},{0.0170371f,0.37059f},{0.0669873f,0.25f},
{0.267892f,0.365992f},{0.241117f,0.430633f},{0.0669873f,0.25f},
{0.241117f,0.430633f},{0.0f,0.5f},{0.0170371f,0.37059f},
{0.241117f,0.430633f},{0.231985f,0.5f},{0.0f,0.5f},
{0.241117f,0.569367f},{0.0170371f,0.62941f},{0.0f,0.5f},
{0.231985f,0.5f},{0.241117f,0.569367f},{0.0f,0.5f},
{0.241117f,0.569367f},{0.0669873f,0.75f},{0.0170371f,0.62941f},
{0.241117f,0.569367f},{0.267892f,0.634008f},{0.0669873f,0.75f},
{0.267892f,0.634008f},{0.146447f,0.853553f},{0.0669873f,0.75f},
{0.267892f,0.634008f},{0.310485f,0.689515f},{0.146447f,0.853553f},
{0.365992f,0.732108f},{0.25f,0.933013f},{0.146447f,0.853553f},
{0.310485f,0.689515f},{0.365992f,0.732108f},{0.146447f,0.853553f},
{0.430633f,0.758883f},{0.37059f,0.982963f},{0.25f,0.933013f},
{0.365992f,0.732108f},{0.430633f,0.758883f},{0.25f,0.933013f},
{0.430633f,0.758883f},{0.5f,1.0f},{0.37059f,0.982963f},
{0.430633f,0.758883f},{0.5f,0.768015f},{0.5f,1.0f},
{0.5f,1.0f},{0.569367f,0.758883f},{0.62941f,0.982963f},
{0.5f,0.768015f},{0.569367f,0.758883f},{0.5f,1.0f},
{0.569367f,0.758883f},{0.634008f,0.732108f},{0.75f,0.933013f},
{0.62941f,0.982963f},{0.569367f,0.758883f},{0.75f,0.933013f},
{0.634008f,0.732108f},{0.689515f,0.689515f},{0.853553f,0.853553f},
{0.75f,0.933013f},{0.634008f,0.732108f},{0.853553f,0.853553f},
{0.853553f,0.853553f},{0.732108f,0.634008f},{0.933013f,0.75f},
{0.689515f,0.689515f},{0.732108f,0.634008f},{0.853553f,0.853553f},
{0.933013f,0.75f},{0.758883f,0.569367f},{0.982963f,0.62941f},
{0.732108f,0.634008f},{0.758883f,0.569367f},{0.933013f,0.75f},
{0.746227f,0.508286f},{0.633118f,0.527091f},{0.635594f,0.508286f},
{0.746227f,0.508286f},{0.739981f,0.555725f},{0.633118f,0.527091f},
{0.768015f,0.5f},{0.641937f,0.5f},{0.6371f,0.463264f},
{0.758883f,0.430633f},{0.768015f,0.5f},{0.6371f,0.463264f},
{0.717381f,0.384062f},{0.628829f,0.456902f},{0.621571f,0.439378f},
{0.717381f,0.384062f},{0.735692f,0.428269f},{0.628829f,0.456902f},
{0.732108f,0.365992f},{0.622921f,0.429032f},{0.600364f,0.399636f},
{0.689515f,0.310485f},{0.732108f,0.365992f},{0.600364f,0.399636f},
{0.630289f,0.290904f},{0.590021f,0.398262f},{0.574973f,0.386715f},
{0.630289f,0.290904f},{0.66825f,0.320033f},{0.590021f,0.398262f},
{0.634008f,0.267892f},{0.570968f,0.377079f},{0.536736f,0.3629f},
{0.569367f,0.241117f},{0.634008f,0.267892f},{0.536736f,0.3629f},
{0.508286f,0.253773f},{0.527091f,0.366882f},{0.508286f,0.364406f},
{0.508286f,0.253773f},{0.555725f,0.260019f},{0.527091f,0.366882f},
{0.463264f,0.3629f},{0.5f,0.231985f},{0.5f,0.358063f},
{0.430633f,0.241117f},{0.5f,0.231985f},{0.463264f,0.3629f},
{0.384062f,0.282619f},{0.428269f,0.264308f},{0.456902f,0.371171f},
{0.439378f,0.378429f},{0.384062f,0.282619f},{0.456902f,0.371171f},
{0.399636f,0.399636f},{0.365992f,0.267892f},{0.429032f,0.377079f},
{0.310485f,0.310485f},{0.365992f,0.267892f},{0.399636f,0.399636f},
{0.290904f,0.369711f},{0.320033f,0.33175f},{0.398262f,0.409979f},
{0.386715f,0.425027f},{0.290904f,0.369711f},{0.398262f,0.409979f},
{0.3629f,0.463264f},{0.267892f,0.365992f},{0.377079f,0.429032f},
{0.241117f,0.430633f},{0.267892f,0.365992f},{0.3629f,0.463264f},
{0.364406f,0.491714f},{0.260019f,0.444275f},{0.366882f,0.472909f},
{0.253773f,0.491714f},{0.260019f,0.444275f},{0.364406f,0.491714f},
{0.3629f,0.536736f},{0.231985f,0.5f},{0.358063f,0.5f},
{0.241117f,0.569367f},{0.231985f,0.5f},{0.3629f,0.536736f},
{0.282619f,0.615938f},{0.264308f,0.571731f},{0.371171f,0.543098f},
{0.378429f,0.560622f},{0.282619f,0.615938f},{0.371171f,0.543098f},
{0.399636f,0.600364f},{0.267892f,0.634008f},{0.377079f,0.570968f},
{0.310485f,0.689515f},{0.267892f,0.634008f},{0.399636f,0.600364f},
{0.369711f,0.709096f},{0.33175f,0.679967f},{0.409979f,0.601738f},
{0.425027f,0.613285f},{0.369711f,0.709096f},{0.409979f,0.601738f},
{0.463264f,0.6371f},{0.365992f,0.732108f},{0.429032f,0.622921f},
{0.430633f,0.758883f},{0.365992f,0.732108f},{0.463264f,0.6371f},
{0.491714f,0.635594f},{0.444275f,0.739981f},{0.472909f,0.633118f},
{0.491714f,0.746227f},{0.444275f,0.739981f},{0.491714f,0.635594f},
{0.5f,0.768015f},{0.5f,0.641937f},{0.536736f,0.6371f},
{0.569367f,0.758883f},{0.5f,0.768015f},{0.536736f,0.6371f},
{0.615938f,0.717381f},{0.543098f,0.628829f},{0.560622f,0.621571f},
{0.615938f,0.717381f},{0.571731f,0.735692f},{0.543098f,0.628829f},
{0.634008f,0.732108f},{0.570968f,0.622921f},{0.600364f,0.600364f},
{0.689515f,0.689515f},{0.634008f,0.732108f},{0.600364f,0.600364f},
{0.709096f,0.630289f},{0.601738f,0.590021f},{0.613285f,0.574973f},
{0.709096f,0.630289f},{0.679967f,0.66825f},{0.601738f,0.590021f},
{0.732108f,0.634008f},{0.622921f,0.570968f},{0.6371f,0.536736f},
{0.758883f,0.569367f},{0.732108f,0.634008f},{0.6371f,0.536736f},
{0.746227f,0.508286f},{0.635594f,0.508286f},{0.641937f,0.5f},
{0.768015f,0.5f},{0.746227f,0.508286f},{0.641937f,0.5f},
{0.633118f,0.527091f},{0.6371f,0.536736f},{0.641937f,0.5f},
{0.635594f,0.508286f},{0.633118f,0.527091f},{0.641937f,0.5f},
{0.633118f,0.527091f},{0.758883f,0.569367f},{0.6371f,0.536736f},
{0.739981f,0.555725f},{0.758883f,0.569367f},{0.633118f,0.527091f},
{0.758883f,0.569367f},{0.746227f,0.508286f},{0.768015f,0.5f},
{0.739981f,0.555725f},{0.746227f,0.508286f},{0.758883f,0.569367f},
{0.732108f,0.365992f},{0.621571f,0.439378f},{0.622921f,0.429032f},
{0.732108f,0.365992f},{0.717381f,0.384062f},{0.621571f,0.439378f},
{0.621571f,0.439378f},{0.6371f,0.463264f},{0.622921f,0.429032f},
{0.621571f,0.439378f},{0.628829f,0.456902f},{0.6371f,0.463264f},
{0.735692f,0.428269f},{0.758883f,0.430633f},{0.6371f,0.463264f},
{0.628829f,0.456902f},{0.735692f,0.428269f},{0.6371f,0.463264f},
{0.735692f,0.428269f},{0.717381f,0.384062f},{0.732108f,0.365992f},
{0.758883f,0.430633f},{0.735692f,0.428269f},{0.732108f,0.365992f},
{0.630289f,0.290904f},{0.574973f,0.386715f},{0.570968f,0.377079f},
{0.634008f,0.267892f},{0.630289f,0.290904f},{0.570968f,0.377079f},
{0.590021f,0.398262f},{0.600364f,0.399636f},{0.570968f,0.377079f},
{0.574973f,0.386715f},{0.590021f,0.398262f},{0.570968f,0.377079f},
{0.590021f,0.398262f},{0.689515f,0.310485f},{0.600364f,0.399636f},
{0.66825f,0.320033f},{0.689515f,0.310485f},{0.590021f,0.398262f},
{0.689515f,0.310485f},{0.630289f,0.290904f},{0.634008f,0.267892f},
{0.66825f,0.320033f},{0.630289f,0.290904f},{0.689515f,0.310485f},
{0.508286f,0.253773f},{0.508286f,0.364406f},{0.5f,0.358063f},
{0.5f,0.231985f},{0.508286f,0.253773f},{0.5f,0.358063f},
{0.527091f,0.366882f},{0.536736f,0.3629f},{0.5f,0.358063f},
{0.508286f,0.364406f},{0.527091f,0.366882f},{0.5f,0.358063f},
{0.527091f,0.366882f},{0.569367f,0.241117f},{0.536736f,0.3629f},
{0.555725f,0.260019f},{0.569367f,0.241117f},{0.527091f,0.366882f},
{0.569367f,0.241117f},{0.508286f,0.253773f},{0.5f,0.231985f},
{0.555725f,0.260019f},{0.508286f,0.253773f},{0.569367f,0.241117f},
{0.439378f,0.378429f},{0.456902f,0.371171f},{0.463264f,0.3629f},
{0.429032f,0.377079f},{0.439378f,0.378429f},{0.463264f,0.3629f},
{0.428269f,0.264308f},{0.430633f,0.241117f},{0.463264f,0.3629f},
{0.456902f,0.371171f},{0.428269f,0.264308f},{0.463264f,0.3629f},
{0.428269f,0.264308f},{0.365992f,0.267892f},{0.430633f,0.241117f},
{0.428269f,0.264308f},{0.384062f,0.282619f},{0.365992f,0.267892f},
{0.365992f,0.267892f},{0.439378f,0.378429f},{0.429032f,0.377079f},
{0.365992f,0.267892f},{0.384062f,0.282619f},{0.439378f,0.378429f},
{0.377079f,0.429032f},{0.398262f,0.409979f},{0.399636f,0.399636f},
{0.386715f,0.425027f},{0.398262f,0.409979f},{0.377079f,0.429032f},
{0.398262f,0.409979f},{0.310485f,0.310485f},{0.399636f,0.399636f},
{0.320033f,0.33175f},{0.310485f,0.310485f},{0.398262f,0.409979f},
{0.290904f,0.369711f},{0.267892f,0.365992f},{0.310485f,0.310485f},
{0.320033f,0.33175f},{0.290904f,0.369711f},{0.310485f,0.310485f},
{0.290904f,0.369711f},{0.386715f,0.425027f},{0.377079f,0.429032f},
{0.267892f,0.365992f},{0.290904f,0.369711f},{0.377079f,0.429032f},
{0.358063f,0.5f},{0.366882f,0.472909f},{0.3629f,0.463264f},
{0.364406f,0.491714f},{0.366882f,0.472909f},{0.358063f,0.5f},
{0.366882f,0.472909f},{0.241117f,0.430633f},{0.3629f,0.463264f},
{0.260019f,0.444275f},{0.241117f,0.430633f},{0.366882f,0.472909f},
{0.253773f,0.491714f},{0.231985f,0.5f},{0.241117f,0.430633f},
{0.260019f,0.444275f},{0.253773f,0.491714f},{0.241117f,0.430633f},
{0.253773f,0.491714f},{0.364406f,0.491714f},{0.358063f,0.5f},
{0.231985f,0.5f},{0.253773f,0.491714f},{0.358063f,0.5f},
{0.378429f,0.560622f},{0.371171f,0.543098f},{0.3629f,0.536736f},
{0.377079f,0.570968f},{0.378429f,0.560622f},{0.3629f,0.536736f},
{0.264308f,0.571731f},{0.241117f,0.569367f},{0.3629f,0.536736f},
{0.371171f,0.543098f},{0.264308f,0.571731f},{0.3629f,0.536736f},
{0.264308f,0.571731f},{0.267892f,0.634008f},{0.241117f,0.569367f},
{0.264308f,0.571731f},{0.282619f,0.615938f},{0.267892f,0.634008f},
{0.267892f,0.634008f},{0.378429f,0.560622f},{0.377079f,0.570968f},
{0.267892f,0.634008f},{0.282619f,0.615938f},{0.378429f,0.560622f},
{0.429032f,0.622921f},{0.409979f,0.601738f},{0.399636f,0.600364f},
{0.425027f,0.613285f},{0.409979f,0.601738f},{0.429032f,0.622921f},
{0.409979f,0.601738f},{0.310485f,0.689515f},{0.399636f,0.600364f},
{0.33175f,0.679967f},{0.310485f,0.689515f},{0.409979f,0.601738f},
{0.369711f,0.709096f},{0.365992f,0.732108f},{0.310485f,0.689515f},
{0.33175f,0.679967f},{0.369711f,0.709096f},{0.310485f,0.689515f},
{0.369711f,0.709096f},{0.425027f,0.613285f},{0.429032f,0.622921f},
{0.365992f,0.732108f},{0.369711f,0.709096f},{0.429032f,0.622921f},
{0.5f,0.641937f},{0.472909f,0.633118f},{0.463264f,0.6371f},
{0.491714f,0.635594f},{0.472909f,0.633118f},{0.5f,0.641937f},
{0.472909f,0.633118f},{0.430633f,0.758883f},{0.463264f,0.6371f},
{0.444275f,0.739981f},{0.430633f,0.758883f},{0.472909f,0.633118f},
{0.491714f,0.746227f},{0.5f,0.768015f},{0.430633f,0.758883f},
{0.444275f,0.739981f},{0.491714f,0.746227f},{0.430633f,0.758883f},
{0.491714f,0.746227f},{0.491714f,0.635594f},{0.5f,0.641937f},
{0.5f,0.768015f},{0.491714f,0.746227f},{0.5f,0.641937f},
{0.634008f,0.732108f},{0.560622f,0.621571f},{0.570968f,0.622921f},
{0.634008f,0.732108f},{0.615938f,0.717381f},{0.560622f,0.621571f},
{0.560622f,0.621571f},{0.536736f,0.6371f},{0.570968f,0.622921f},
{0.560622f,0.621571f},{0.543098f,0.628829f},{0.536736f,0.6371f},
{0.571731f,0.735692f},{0.569367f,0.758883f},{0.536736f,0.6371f},
{0.543098f,0.628829f},{0.571731f,0.735692f},{0.536736f,0.6371f},
{0.571731f,0.735692f},{0.615938f,0.717381f},{0.634008f,0.732108f},
{0.569367f,0.758883f},{0.571731f,0.735692f},{0.634008f,0.732108f},
{0.709096f,0.630289f},{0.613285f,0.574973f},{0.622921f,0.570968f},
{0.732108f,0.634008f},{0.709096f,0.630289f},{0.622921f,0.570968f},
{0.601738f,0.590021f},{0.600364f,0.600364f},{0.622921f,0.570968f},
{0.613285f,0.574973f},{0.601738f,0.590021f},{0.622921f,0.570968f},
{0.601738f,0.590021f},{0.689515f,0.689515f},{0.600364f,0.600364f},
{0.679967f,0.66825f},{0.689515f,0.689515f},{0.601738f,0.590021f},
{0.689515f,0.689515f},{0.709096f,0.630289f},{0.732108f,0.634008f},
{0.679967f,0.66825f},{0.709096f,0.630289f},{0.689515f,0.689515f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f},
{0.491935f,0.258013f},{0.491935f,0.258013f},{0.491935f,0.258013f}
};
/*Material indicies*/
/*{material index,face count}*/
static int material_ref [9][2] = {
{0,1},
{1,1},
{0,1},
{1,1},
{0,17},
{2,2},
{3,262},
{2,17},
{1,14}
};