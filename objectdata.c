static short face_indicies[785][3] = {
// Object #-1
	{0,2,1 }, {32,2,0 }, {1,2,3 }, {4,5,7 }, {5,8,7 }, {5,6,8 },
	{9,6,5 }, {9,5,10 }, {10,5,4 }, {12,9,10 }, {32,11,12 }, {12,11,9 },
	{9,11,6 }, {13,10,4 }, {14,10,13 }, {3,14,13 }, {14,12,10 },
	{32,12,2 }, {2,12,14 }, {2,14,3 }, {4,7,16 }, {16,15,17 }, {18,16,17 },
	{18,4,16 }, {3,13,19 }, {19,13,18 }, {13,4,18 }, {19,18,17 },
	{21,23,20 }, {20,26,15 }, {20,22,26 }, {23,22,20 }, {23,28,22 },
	{26,24,25 }, {15,26,25 }, {26,27,24 }, {22,27,26 }, {22,28,27 },
	{28,24,27 }, {28,0,24 }, {29,23,21 }, {8,29,21 }, {6,29,8 },
	{29,31,30 }, {29,30,23 }, {6,31,29 }, {11,32,31 }, {31,32,30 },
	{33,17,25 }, {17,15,25 }, {24,33,25 }, {24,34,33 }, {34,19,33 },
	{34,3,19 }, {19,17,33 }, {0,1,24 }, {24,1,34 }, {1,3,34 }, {30,28,23 },
	{30,35,28 }, {32,35,30 }, {35,0,28 }, {35,32,0 }, {42,40,41 },
	{37,42,41 }, {36,37,41 }, {43,40,42 }, {38,43,42 }, {37,38,42 },
	{38,39,43 }, {43,44,40 }, {45,44,43 }, {39,45,43 }, {52,48,51 },
	{52,49,48 }, {48,47,51 }, {53,49,52 }, {53,52,54 }, {54,52,51 },
	{55,53,54 }, {56,54,51 }, {55,54,56 }, {57,55,56 }, {62,60,64 },
	{63,62,64 }, {63,64,61 }, {60,65,64 }, {60,66,65 }, {66,67,65 },
	{64,69,68 }, {61,64,68 }, {65,69,64 }, {70,68,69 }, {65,70,69 },
	{65,67,70 }, {71,74,70 }, {67,71,70 }, {70,74,72 }, {70,72,68 },
	{75,73,74 }, {74,73,153 }, {74,153,72 }, {75,77,73 }, {76,77,75 },
	{160,76,75 }, {76,78,77 }, {77,154,73 }, {77,78,154 }, {154,78,92 },
	{82,79,80 }, {82,80,83 }, {83,80,81 }, {151,86,85 }, {89,85,87 },
	{89,151,85 }, {88,89,78 }, {88,151,89 }, {90,88,78 }, {87,84,91 },
	{92,87,91 }, {89,87,92 }, {78,89,92 }, {94,93,63 }, {112,111,95 },
	{95,111,94 }, {111,93,94 }, {98,96,95 }, {146,96,98 }, {96,112,95 },
	{97,146,98 }, {99,100,97 }, {100,146,97 }, {98,95,101 }, {97,98,101 },
	{79,102,80 }, {102,101,80 }, {102,97,101 }, {80,101,81 }, {103,99,102 },
	{99,97,102 }, {151,103,86 }, {151,99,103 }, {111,110,109 }, {110,107,109 },
	{112,110,111 }, {96,110,112 }, {113,114,115 }, {115,114,106 },
	{117,113,115 }, {115,106,105 }, {104,115,105 }, {107,116,108 },
	{116,115,108 }, {116,117,115 }, {108,115,104 }, {117,118,113 },
	{122,118,117 }, {122,117,119 }, {120,121,122 }, {122,121,118 },
	{116,119,117 }, {123,119,116 }, {123,116,107 }, {120,122,123 },
	{122,119,123 }, {128,124,125 }, {118,128,125 }, {113,118,125 },
	{128,126,127 }, {128,127,124 }, {121,120,126 }, {121,126,128 },
	{121,128,118 }, {130,127,129 }, {127,130,124 }, {127,132,129 },
	{126,132,127 }, {126,120,132 }, {132,135,131 }, {133,135,132 },
	{120,133,132 }, {132,131,129 }, {135,134,131 }, {135,136,138 },
	{133,136,135 }, {134,139,167 }, {134,167,131 }, {135,139,134 },
	{135,138,139 }, {142,138,140 }, {139,138,142 }, {139,142,137 },
	{137,142,141 }, {142,90,141 }, {140,90,142 }, {113,125,114 },
	{143,123,110 }, {143,120,123 }, {110,123,107 }, {133,120,143 },
	{145,133,143 }, {145,143,144 }, {144,143,110 }, {136,145,138 },
	{136,133,145 }, {144,110,96 }, {146,144,96 }, {145,144,146 },
	{138,145,100 }, {145,146,100 }, {147,130,148 }, {124,130,147 },
	{130,129,148 }, {148,129,149 }, {129,131,149 }, {149,131,150 },
	{140,100,99 }, {138,100,140 }, {151,140,99 }, {90,151,88 }, {90,140,151 },
	{153,152,72 }, {73,154,153 }, {154,152,153 }, {154,92,152 },
	{137,157,155 }, {157,158,156 }, {159,137,155 }, {141,158,157 },
	{137,141,157 }, {67,156,71 }, {156,158,71 }, {158,161,160 },
	{158,141,161 }, {71,75,74 }, {160,75,71 }, {158,160,71 }, {161,90,78 },
	{141,90,161 }, {161,76,160 }, {161,78,76 }, {162,170,169 }, {162,169,68 },
	{169,61,68 }, {68,163,162 }, {163,170,162 }, {81,170,163 }, {72,163,68 },
	{83,81,163 }, {164,163,72 }, {152,164,72 }, {92,91,152 }, {152,91,164 },
	{91,84,164 }, {131,165,150 }, {165,167,166 }, {131,167,165 },
	{167,139,168 }, {167,168,166 }, {139,159,168 }, {139,137,159 },
	{94,63,169 }, {169,63,61 }, {95,94,169 }, {170,95,169 }, {81,101,170 },
	{101,95,170 }, {51,47,46 }, {51,46,171 }, {172,171,36 }, {173,171,172 },
	{174,173,172 }, {173,51,171 }, {57,56,174 }, {174,56,173 }, {56,51,173 },
	{175,45,39 }, {175,50,58 }, {176,174,172 }, {176,172,41 }, {172,36,41 },
	{40,176,41 }, {40,177,176 }, {177,174,176 }, {57,174,177 }, {44,59,177 },
	{59,57,177 }, {44,177,40 }, {206,179,178 }, {178,179,180 }, {182,183,181 },
	{183,196,181 }, {184,183,182 }, {184,182,185 }, {185,182,181 },
	{187,184,185 }, {206,186,187 }, {187,186,184 }, {184,186,183 },
	{188,185,181 }, {189,185,188 }, {180,189,188 }, {189,187,185 },
	{206,187,179 }, {179,187,189 }, {179,189,180 }, {181,196,190 },
	{181,190,191 }, {191,190,192 }, {193,191,192 }, {193,181,191 },
	{180,188,194 }, {194,188,193 }, {188,181,193 }, {194,193,192 },
	{190,196,195 }, {195,201,190 }, {195,197,201 }, {195,198,197 },
	{198,203,197 }, {201,199,200 }, {190,201,200 }, {201,202,199 },
	{197,202,201 }, {197,203,202 }, {203,199,202 }, {203,178,199 },
	{205,195,196 }, {183,205,196 }, {205,204,195 }, {186,206,205 },
	{205,206,204 }, {186,205,183 }, {207,192,200 }, {192,190,200 },
	{199,207,200 }, {199,208,207 }, {208,194,207 }, {208,180,194 },
	{194,192,207 }, {199,178,208 }, {178,180,208 }, {204,203,198 },
	{204,198,195 }, {204,209,203 }, {206,209,204 }, {209,178,203 },
	{209,206,178 }, {210,213,212 }, {211,213,210 }, {212,213,214 },
	{216,217,215 }, {217,231,215 }, {218,217,216 }, {218,216,219 },
	{219,216,215 }, {221,218,219 }, {211,220,221 }, {221,220,218 },
	{218,220,217 }, {222,219,215 }, {223,219,222 }, {214,223,222 },
	{223,221,219 }, {211,221,213 }, {213,221,223 }, {213,223,214 },
	{215,231,224 }, {215,224,225 }, {225,224,226 }, {227,225,226 },
	{227,215,225 }, {214,222,228 }, {228,222,227 }, {222,215,227 },
	{228,227,226 }, {231,229,230 }, {224,231,230 }, {230,232,224 },
	{230,233,232 }, {229,233,230 }, {229,234,233 }, {234,239,233 },
	{237,235,236 }, {232,237,236 }, {224,232,236 }, {238,235,237 },
	{233,238,237 }, {232,233,237 }, {233,239,238 }, {239,235,238 },
	{239,210,235 }, {429,241,246 }, {429,246,240 }, {242,229,231 },
	{217,242,231 }, {244,243,242 }, {242,243,229 }, {217,244,242 },
	{220,211,245 }, {220,245,244 }, {244,245,243 }, {220,244,217 },
	{262,248,247 }, {251,250,326 }, {251,326,249 }, {252,250,251 },
	{252,253,250 }, {247,254,251 }, {254,252,251 }, {246,251,249 },
	{247,251,246 }, {256,252,254 }, {257,254,247 }, {260,422,253 },
	{260,253,256 }, {256,253,252 }, {255,260,256 }, {262,258,248 },
	{248,257,247 }, {260,259,422 }, {260,255,258 }, {261,260,258 },
	{261,258,262 }, {260,261,259 }, {266,241,265 }, {269,271,267 },
	{417,273,272 }, {415,272,274 }, {417,272,415 }, {275,273,417 },
	{276,417,406 }, {276,275,417 }, {276,406,279 }, {279,406,281 },
	{281,294,280 }, {288,287,285 }, {292,290,291 }, {292,291,285 },
	{291,286,285 }, {287,292,285 }, {293,290,292 }, {293,294,290 },
	{280,294,293 }, {295,435,291 }, {435,286,291 }, {290,295,291 },
	{296,435,295 }, {294,296,295 }, {294,295,290 }, {297,299,298 },
	{407,302,299 }, {297,407,299 }, {407,300,302 }, {301,284,283 },
	{298,301,283 }, {282,298,283 }, {299,301,298 }, {301,302,284 },
	{299,302,301 }, {302,289,284 }, {302,277,289 }, {300,277,302 },
	{304,306,303 }, {306,305,303 }, {306,307,305 }, {305,309,310 },
	{309,311,310 }, {308,312,307 }, {307,312,309 }, {307,309,305 },
	{312,313,309 }, {315,313,312 }, {308,315,312 }, {315,297,314 },
	{313,315,314 }, {313,314,309 }, {316,298,282 }, {314,298,316 },
	{314,316,309 }, {314,297,298 }, {318,320,317 }, {318,363,320 },
	{318,319,363 }, {320,363,332 }, {321,319,318 }, {425,321,318 },
	{322,318,317 }, {425,318,322 }, {319,325,304 }, {323,325,319 },
	{321,323,319 }, {425,322,324 }, {324,322,317 }, {325,306,304 },
	{325,307,306 }, {325,308,307 }, {323,308,325 }, {326,321,425 },
	{333,328,361 }, {328,327,361 }, {330,327,328 }, {329,330,328 },
	{320,332,330 }, {320,330,331 }, {331,330,329 }, {317,320,331 },
	{330,332,327 }, {334,328,333 }, {334,333,335 }, {329,328,334 },
	{355,324,337 }, {324,317,337 }, {355,337,336 }, {333,361,338 },
	{339,338,340 }, {333,338,339 }, {335,333,339 }, {340,342,339 },
	{342,341,339 }, {341,335,339 }, {343,344,340 }, {344,342,340 },
	{344,345,342 }, {343,345,344 }, {349,346,343 }, {346,347,343 },
	{346,348,347 }, {349,348,346 }, {350,347,348 }, {345,351,352 },
	{345,352,342 }, {347,353,345 }, {353,351,345 }, {347,345,343 },
	{347,350,353 }, {353,354,351 }, {353,303,354 }, {350,303,353 },
	{338,361,343 }, {338,343,340 }, {426,324,355 }, {358,355,336 },
	{357,355,358 }, {357,426,355 }, {356,426,357 }, {359,358,336 },
	{360,358,359 }, {356,357,358 }, {311,409,358 }, {409,356,358 },
	{311,358,360 }, {361,362,349 }, {361,349,343 }, {327,362,361 },
	{363,348,362 }, {332,363,362 }, {332,362,327 }, {362,348,349 },
	{363,364,348 }, {319,364,363 }, {319,304,364 }, {352,365,342 },
	{365,335,341 }, {342,365,341 }, {351,367,366 }, {351,368,367 },
	{354,368,351 }, {303,305,354 }, {360,359,367 }, {359,336,367 },
	{368,360,367 }, {354,310,360 }, {354,360,368 }, {305,310,354 },
	{310,311,360 }, {364,350,348 }, {364,304,350 }, {370,369,371 },
	{350,304,303 }, {373,370,371 }, {372,404,373 }, {404,370,373 },
	{373,371,376 }, {374,375,376 }, {376,375,373 }, {375,372,373 },
	{378,372,375 }, {377,372,378 }, {383,382,380 }, {382,381,380 },
	{392,384,382 }, {384,381,382 }, {384,386,385 }, {392,386,384 },
	{384,385,387 }, {384,387,381 }, {386,392,388 }, {390,393,389 },
	{390,389,369 }, {386,388,390 }, {390,388,393 }, {388,392,391 },
	{391,392,382 }, {389,393,394 }, {396,395,394 }, {393,388,396 },
	{393,396,394 }, {391,397,396 }, {388,391,396 }, {394,398,399 },
	{398,379,399 }, {379,374,399 }, {395,398,394 }, {397,391,400 },
	{382,383,400 }, {391,382,400 }, {389,394,401 }, {371,401,376 },
	{399,376,401 }, {394,399,401 }, {399,374,376 }, {403,369,370 },
	{403,390,369 }, {402,390,403 }, {385,390,402 }, {385,386,390 },
	{403,370,404 }, {402,403,404 }, {377,404,372 }, {405,385,404 },
	{385,402,404 }, {405,404,377 }, {387,385,405 }, {369,389,401 },
	{371,369,401 }, {294,281,296 }, {281,406,296 }, {421,408,407 },
	{407,408,300 }, {421,270,408 }, {300,408,277 }, {408,278,277 },
	{270,278,408 }, {410,428,409 }, {410,409,311 }, {309,410,311 },
	{428,356,409 }, {411,410,309 }, {411,412,410 }, {265,412,411 },
	{412,428,410 }, {413,316,282 }, {413,411,316 }, {316,411,309 },
	{416,417,415 }, {406,417,416 }, {406,416,296 }, {308,418,315 },
	{418,419,315 }, {418,263,419 }, {263,268,420 }, {268,267,420 },
	{419,420,315 }, {263,420,419 }, {315,420,297 }, {420,421,407 },
	{420,407,297 }, {267,421,420 }, {271,270,421 }, {267,271,421 },
	{321,423,323 }, {423,422,323 }, {422,259,424 }, {422,308,323 },
	{422,424,308 }, {424,418,308 }, {259,418,424 }, {264,263,418 },
	{259,264,418 }, {250,321,326 }, {250,423,321 }, {253,422,423 },
	{253,423,250 }, {249,326,425 }, {246,249,427 }, {266,247,241 },
	{266,262,247 }, {241,247,246 }, {426,425,324 }, {249,425,427 },
	{427,425,426 }, {240,246,427 }, {427,426,356 }, {428,240,427 },
	{428,427,356 }, {265,429,412 }, {412,429,428 }, {429,240,428 },
	{265,241,429 }, {430,226,236 }, {226,224,236 }, {235,430,236 },
	{235,431,430 }, {431,228,430 }, {431,214,228 }, {228,226,430 },
	{210,212,235 }, {235,212,431 }, {212,214,431 }, {243,432,234 },
	{432,239,234 }, {243,234,229 }, {432,433,239 }, {245,433,432 },
	{243,245,432 }, {245,211,433 }, {415,274,414 }, {415,414,434 },
	{433,210,239 }, {433,211,210 }, {435,434,286 }, {435,415,434 },
	{296,416,435 }, {416,415,435 }, {437,439,436 }, {437,440,439 },
	{438,440,437 }, {442,441,440 }, {438,442,440 }, {442,447,441 },
	{445,443,444 }, {439,445,444 }, {436,439,444 }, {440,445,439 },
	{446,443,445 }, {441,446,445 }, {440,441,445 }, {441,447,446 },
	{447,443,446 }, {447,448,443 }, {449,438,437 }, {449,437,436 },
	{450,438,449 }, {451,450,449 }, {452,453,450 }, {450,453,438 },
	{451,452,450 }, {452,455,454 }, {455,452,451 }, {457,451,456 },
	{451,449,456 }, {459,451,457 }, {459,457,458 }, {458,457,456 },
	{454,455,459 }, {459,455,451 }, {460,458,456 }, {461,458,460 },
	{454,459,461 }, {461,459,458 }, {454,461,448 }, {456,449,462 },
	{462,449,436 }, {463,462,436 }, {464,462,463 }, {465,464,463 },
	{464,456,462 }, {461,460,465 }, {465,460,464 }, {460,456,464 },
	{452,466,453 }, {453,466,442 }, {466,447,442 }, {453,442,438 },
	{452,454,466 }, {466,454,447 }, {454,448,447 }, {467,465,463 },
	{467,463,444 }, {463,436,444 }, {443,467,444 }, {461,465,467 },
	{443,448,467 }, {448,461,467 }
};
static float vertices [468][3] = {
{-0.267144f,0.295125f,0.00699484f},{-0.232859f,0.273449f,0.00284787f},{-0.225106f,0.236971f,0.039063f},
{-0.192295f,0.233841f,-0.000610375f},{-0.213472f,0.172638f,-0.00530527f},{-0.241148f,0.161592f,0.0289947f},
{-0.276535f,0.179314f,0.0497804f},{-0.249246f,0.165162f,-0.00566163f},{-0.276418f,0.167999f,0.0209886f},
{-0.244054f,0.17633f,0.0563419f},{-0.209217f,0.173493f,0.0289365f},{-0.275994f,0.201305f,0.0690087f},
{-0.222286f,0.186749f,0.0564953f},{-0.186911f,0.196405f,-0.00429981f},{-0.200217f,0.199742f,0.0334468f},
{-0.27415f,0.227319f,-0.0638711f},{-0.238519f,0.192257f,-0.0432085f},{-0.214735f,0.230136f,-0.0665656f},
{-0.203389f,0.204043f,-0.0420389f},{-0.194389f,0.230341f,-0.038333f},{-0.307529f,0.225557f,-0.0383139f},
{-0.302054f,0.192659f,-0.00536341f},{-0.323741f,0.270587f,-0.0372981f},{-0.336464f,0.247892f,0.000213273f},
{-0.267588f,0.292415f,-0.0395844f},{-0.271819f,0.260489f,-0.0717274f},{-0.306944f,0.270843f,-0.0560791f},
{-0.302198f,0.29324f,-0.0301894f},{-0.307943f,0.290047f,0.00243979f},{-0.309075f,0.197157f,0.0282751f},
{-0.327077f,0.236168f,0.0416467f},{-0.306516f,0.208973f,0.0537975f},{-0.287242f,0.231858f,0.0662245f},
{-0.230886f,0.255071f,-0.0647548f},{-0.222476f,0.267755f,-0.0350841f},{-0.303744f,0.264721f,0.0383434f},
{-0.0933547f,0.0394028f,-0.321658f},{-0.140451f,0.0854441f,-0.319702f},{-0.156183f,0.102194f,-0.287032f},
{-0.152971f,0.104294f,-0.250354f},{-0.0883753f,0.10216f,-0.313286f},{-0.08972f,0.0754661f,-0.331508f},
{-0.123895f,0.101287f,-0.317487f},{-0.126909f,0.120275f,-0.289115f},{-0.0875921f,0.118646f,-0.280652f},
{-0.118452f,0.119553f,-0.24895f},{-0.0966174f,0.00359264f,-0.287758f},{-0.0953574f,-0.018178f,-0.258113f},
{-0.096854f,-0.0185433f,-0.231609f},{-0.095248f,-0.00937919f,-0.201572f},{-0.0902686f,0.0466476f,-0.182311f},
{-0.0551519f,-0.0160949f,-0.25851f},{-0.0624405f,-0.0209099f,-0.224943f},{-0.0635547f,-0.00902215f,-0.197959f},
{-0.0372068f,-0.00827797f,-0.226801f},{-0.0361566f,0.000964743f,-0.208715f},{-0.0211776f,0.0125543f,-0.255696f},
{-0.0258916f,0.046571f,-0.252247f},{-0.0878288f,0.0916308f,-0.216609f},{-0.0569967f,0.090981f,-0.249186f},
{-0.0296242f,0.162435f,-0.114419f},{0.0408383f,0.171976f,-0.168992f},{0.00809291f,0.125527f,-0.126331f},
{0.0390951f,0.0857001f,-0.111967f},{0.00408479f,0.199312f,-0.168907f},{-0.0186782f,0.223442f,-0.158827f},
{-0.0430136f,0.211701f,-0.112939f},{-0.0329637f,0.245855f,-0.109396f},{0.0406379f,0.215269f,-0.180174f},
{0.0119318f,0.231117f,-0.171452f},{0.00438161f,0.257957f,-0.151839f},{-0.0238738f,0.287946f,-0.105882f},
{0.0559333f,0.284371f,-0.139491f},{0.0429819f,0.318928f,-0.12104f},{0.0292724f,0.314626f,-0.127475f},
{0.00090636f,0.309082f,-0.0921025f},{0.0159314f,0.295469f,-0.0442857f},{0.0430876f,0.312172f,-0.0779686f},
{0.0579533f,0.297525f,-0.0527112f},{0.129249f,0.169043f,-0.0717355f},{0.138362f,0.169759f,-0.0987465f},
{0.123591f,0.181402f,-0.123049f},{0.13512f,0.185998f,-0.0873651f},{0.13012f,0.204352f,-0.117057f},
{0.126419f,0.219777f,-0.103899f},{0.118412f,0.193932f,-0.0539547f},{0.108588f,0.177404f,-0.041234f},
{0.123494f,0.22089f,-0.0675618f},{0.0553572f,0.236809f,0.00648451f},{0.112779f,0.233317f,-0.0405741f},
{0.040267f,0.247122f,0.0118441f},{0.113268f,0.257091f,-0.106322f},{0.100142f,0.279786f,-0.0924124f},
{0.0502499f,0.0422847f,-0.08094f},{0.0689472f,0.0965428f,-0.120581f},{0.0973297f,0.0940499f,-0.0986692f},
{0.12731f,0.00589767f,1.99308e-005f},{0.107803f,0.110158f,-0.0456282f},{0.11031f,0.0964736f,-0.0673862f},
{0.0870497f,0.13337f,-0.00714163f},{0.0822021f,0.0713369f,0.0308582f},{0.125837f,0.145988f,-0.0991671f},
{0.120319f,0.149431f,-0.0659933f},{0.0997869f,0.158581f,-0.0320131f},{0.0966571f,-0.0849079f,-0.0487141f},
{0.0736991f,-0.0823211f,-0.0614958f},{0.0402801f,-0.0852253f,-0.0602872f},{0.106682f,-0.0797956f,0.000284471f},
{0.104013f,-0.0816423f,-0.0277413f},{0.0883297f,-0.0232101f,-0.0440813f},{0.114079f,-0.051812f,0.0128454f},
{0.101058f,-0.0131805f,-0.0358738f},{0.116474f,0.0211302f,-0.041451f},{0.0465431f,-0.129564f,0.0053452f},
{0.0382535f,-0.114826f,-0.0213573f},{0.0886786f,-0.110462f,-0.0293228f},{0.102039f,-0.11094f,0.00742531f},
{0.0690679f,-0.131882f,0.0228418f},{0.0391583f,-0.127631f,0.0359955f},{0.0804339f,-0.122999f,0.0389063f},
{0.0447835f,-0.0801925f,0.0643656f},{0.0417842f,-0.10964f,0.062645f},{0.0668534f,-0.119134f,0.0527195f},
{0.0909691f,-0.103128f,0.0387055f},{-0.0165675f,-0.08702f,0.0068914f},{0.0156839f,-0.114492f,0.00450634f},
{-0.000378161f,-0.0877043f,0.0579605f},{-0.0159139f,-0.0780066f,0.0464982f},{0.00977517f,-0.108815f,0.0498137f},
{-0.0223451f,-0.00829008f,0.00772347f},{-0.034434f,-0.0497085f,0.00943721f},{-0.00797661f,0.0273715f,0.00991463f},
{0.00186295f,-0.0339309f,0.0462121f},{0.0497532f,-0.0353393f,0.0771045f},{0.0044714f,0.0315238f,0.0519812f},
{0.0165086f,0.0130725f,0.0676713f},{0.0486084f,0.00226417f,0.0791722f},{0.00410535f,0.174013f,0.00240034f},
{0.0472523f,0.0571925f,0.05735f},{0.0123708f,0.0792779f,0.0314212f},{0.0444268f,0.167073f,0.0208546f},
{-0.000431594f,0.22658f,0.00014201f},{0.0256788f,0.172544f,0.0186568f},{0.0904355f,-0.0616619f,0.0569706f},
{0.113356f,-0.0313516f,0.0495263f},{0.0940957f,-0.0190605f,0.0721036f},{0.11488f,0.0169203f,0.0270955f},
{-0.0251769f,-0.0521907f,-0.0288771f},{-0.0283389f,-0.0159462f,-0.0310344f},{0.000979442f,0.0210794f,-0.0413582f},
{0.0136039f,0.0426668f,-0.0489103f},{0.061444f,0.170052f,0.0150568f},{0.0784467f,0.295091f,-0.131822f},
{0.0561656f,0.310457f,-0.133288f},{0.0660248f,0.311717f,-0.118032f},{-0.0256813f,0.218286f,-0.0530454f},
{-0.0329727f,0.258757f,-0.0858246f},{-0.0214937f,0.22796f,-0.0382307f},{-0.0315614f,0.260497f,-0.0554435f},
{-0.0176705f,0.171674f,-0.0200946f},{-0.0147354f,0.291281f,-0.0598896f},{0.00898658f,0.273963f,-0.0171023f},
{0.0767657f,0.199041f,-0.170221f},{0.0950352f,0.234647f,-0.136506f},{0.0990156f,0.25166f,-0.128911f},
{-0.00439429f,0.0608763f,-0.0191447f},{-0.0103438f,0.0936031f,-0.0384247f},{-0.00770078f,0.083566f,-0.00316063f},
{-0.0352866f,0.164242f,-0.0363124f},{0.0766097f,0.169972f,-0.167981f},{0.0963352f,0.171191f,-0.160522f},
{-0.0604474f,0.00784978f,-0.296095f},{-0.0589269f,0.0419685f,-0.321004f},{-0.0328156f,0.0185576f,-0.29043f},
{-0.0313072f,0.0416315f,-0.304224f},{-0.128229f,0.0848627f,-0.207195f},{-0.0536916f,0.0673451f,-0.317445f},
{-0.0486773f,0.083306f,-0.289335f},{0.0221994f,-0.375432f,0.110343f},{0.0564846f,-0.433125f,0.141943f},
{0.0842948f,-0.436098f,0.10453f},{0.0521451f,-0.49566f,0.100383f},{0.0418937f,-0.5f,0.130583f},
{0.0114762f,-0.493074f,0.137914f},{0.0402775f,-0.488575f,0.156052f},{0.0697279f,-0.490016f,0.130434f},
{0.0120698f,-0.466916f,0.169306f},{0.0593609f,-0.47945f,0.156539f},{0.0882784f,-0.469377f,0.101251f},
{0.0779372f,-0.467415f,0.135445f},{0.0121941f,-0.453751f,0.054686f},{0.0436707f,-0.472444f,0.0673734f},
{0.06394f,-0.440021f,0.0469601f},{0.0735856f,-0.46267f,0.0686921f},{0.0817949f,-0.439544f,0.0710929f},
{-0.0288948f,-0.445443f,0.0861176f},{0.00696895f,-0.491091f,0.0998245f},{-0.0323057f,-0.402605f,0.0699105f},
{-0.0426547f,-0.414644f,0.104526f},{0.0188938f,-0.380136f,0.0674391f},{0.0153447f,-0.412226f,0.0402389f},
{-0.0165266f,-0.402265f,0.0531166f},{-0.012839f,-0.379584f,0.0758932f},{-0.018432f,-0.381765f,0.10708f},
{-0.0357408f,-0.435168f,0.144335f},{-0.021101f,-0.462434f,0.147802f},{0.0018181f,-0.438792f,0.168217f},
{0.0507507f,-0.417406f,0.0470487f},{0.0582616f,-0.4043f,0.0724254f},{-0.0143369f,-0.406718f,0.14175f},
{0.262038f,0.117795f,-0.186f},{0.257856f,0.0533518f,-0.124976f},{0.294213f,0.0937851f,-0.190171f},
{0.301655f,0.0560686f,-0.152122f},{0.330758f,0.0529579f,-0.193709f},{0.298959f,-0.0140743f,-0.198521f},
{0.28823f,-0.0192576f,-0.164001f},{0.253038f,-0.0129878f,-0.154677f},{0.286314f,-0.00554023f,-0.135718f},
{0.317794f,-0.00626116f,-0.165256f},{0.253519f,0.018175f,-0.118691f},{0.306389f,0.0059294f,-0.136223f},
{0.336464f,0.0168959f,-0.197531f},{0.325282f,0.019229f,-0.159981f},{0.253835f,0.0329061f,-0.250428f},
{0.290042f,0.0124596f,-0.235653f},{0.310748f,0.0490512f,-0.256589f},{0.321446f,0.0242249f,-0.233196f},
{0.328934f,0.0494196f,-0.229891f},{0.194157f,0.0464901f,-0.195799f},{0.218617f,0.0440605f,-0.232445f},
{0.247833f,-0.0114126f,-0.199403f},{0.213321f,0.0876663f,-0.247131f},{0.198652f,0.0921493f,-0.229697f},
{0.185653f,0.0797476f,-0.192609f},{0.261141f,0.114126f,-0.233816f},{0.258441f,0.0795f,-0.265261f},
{0.228413f,0.100329f,-0.249253f},{0.225378f,0.116927f,-0.225326f},{0.216339f,0.114152f,-0.190847f},
{0.0989488f,-0.0428087f,-0.0821683f},{0.118873f,-0.00341445f,-0.0449283f},{0.215508f,0.0137387f,-0.162287f},
{0.194715f,0.0497377f,-0.141621f},{0.217988f,0.0255286f,-0.132312f},{0.221167f,0.0501337f,-0.125707f},
{0.120785f,-0.0858498f,-0.0605138f},{0.119854f,-0.0860008f,-0.0278315f},{0.0987125f,-0.0863108f,0.00125229f},
{0.0428532f,-0.106187f,-0.0620718f},{0.00099479f,-0.117504f,-0.0195451f},{0.0401803f,-0.120248f,-0.0361198f},
{0.0319711f,-0.121445f,-0.00151383f},{0.00391918f,-0.10724f,0.0259503f},{0.0582522f,-0.111723f,-0.0171369f},
{0.0191602f,-0.0911546f,0.0502745f},{0.0274592f,-0.105723f,0.025504f},{0.0620598f,-0.0963806f,0.0136073f},
{0.0553287f,-0.0843566f,0.043967f},{0.00348558f,-0.0546339f,0.0491404f},{0.000149579f,-0.0858299f,0.0617856f},
{0.0525321f,-0.0777805f,0.05878f},{0.0941151f,-0.0606397f,0.0423177f},{0.00441893f,0.0492465f,0.00521661f},
{0.00513126f,-0.0126153f,0.0347889f},{0.0867346f,0.0427582f,-0.0581955f},{0.111562f,-0.0173373f,-0.00393572f},
{-0.00741132f,0.135956f,-0.0168571f},{-0.00383133f,0.102529f,-0.0126943f},{0.0230478f,0.131769f,-0.0139509f},
{0.0120131f,0.195571f,0.000793016f},{0.0142411f,0.159549f,0.0019645f},{0.0987536f,0.118612f,-0.0422411f},
{0.0775716f,0.136385f,-0.0166904f},{0.110123f,0.120318f,-0.0719463f},{0.0783612f,0.165569f,-0.00623277f},
{0.0711297f,0.199843f,-0.0107525f},{-0.0115583f,0.278366f,-0.0644418f},{-0.0141103f,0.23475f,-0.0272632f},
{0.0645497f,0.244531f,-0.0335908f},{0.0753314f,0.276758f,-0.0662164f},{0.104726f,0.240018f,-0.0441707f},
{0.00347151f,0.175983f,-0.148148f},{-0.00242114f,0.221201f,-0.155733f},{-0.00148603f,0.25377f,-0.141971f},
{0.0539252f,0.230186f,-0.157826f},{0.106266f,0.189539f,-0.137826f},{0.0299683f,0.2777f,-0.139517f},
{0.0180454f,0.268536f,-0.142022f},{-0.0147814f,0.278218f,-0.107563f},{0.106426f,0.253837f,-0.122219f},
{0.10831f,0.226959f,-0.140991f},{0.0659335f,0.266657f,-0.137222f},{0.0624882f,0.28448f,-0.102842f},
{0.10768f,0.267375f,-0.0688813f},{0.135654f,0.219217f,-0.108578f},{0.134205f,0.22369f,-0.065527f},
{-0.0497119f,0.170542f,-0.0686213f},{-0.0391675f,0.205646f,-0.127859f},{-0.0551566f,0.223507f,-0.108363f},
{-0.0526585f,0.233449f,-0.0645283f},{-0.0366362f,0.240626f,-0.128416f},{-0.0531359f,0.239585f,-0.0999738f},
{-0.148106f,0.111189f,-0.204837f},{-0.160444f,0.0683234f,-0.17965f},{-0.0867227f,0.127666f,-0.187497f},
{-0.0948102f,0.0950254f,-0.162735f},{-0.0551767f,0.0901427f,-0.120652f},{-0.0234623f,0.0400619f,-0.051737f},
{-0.0148647f,0.137943f,-0.148742f},{-0.0574802f,0.135272f,-0.221248f},{-0.0230636f,0.13046f,-0.180252f},
{-0.0297699f,0.108291f,-0.124084f},{-0.0231896f,0.112045f,-0.102893f},{-0.0323348f,0.156905f,-0.118129f},
{-0.0194105f,0.0634342f,-0.070166f},{-0.0028116f,0.157085f,-0.143016f},{-0.0660279f,-0.0134117f,-0.206157f},
{-0.0966892f,-0.0143564f,-0.162298f},{-0.119273f,0.00484649f,-0.144949f},{-0.106435f,-0.0136152f,-0.200745f},
{-0.0364636f,-0.0779914f,-0.0664983f},{-0.0528405f,-0.0332367f,-0.151732f},{-0.0506824f,-0.0498912f,-0.0686576f},
{-0.023983f,-0.0245091f,-0.156466f},{-0.0667556f,-0.0146187f,-0.0942492f},{-0.00259419f,-0.11125f,-0.0485044f},
{-0.144936f,-0.00766147f,-0.261604f},{-0.10136f,-0.0147515f,-0.293276f},{-0.07291f,-0.0171263f,-0.256713f},
{-0.106149f,-0.024697f,-0.23561f},{-0.0718035f,-0.0151025f,-0.232295f},{-0.136925f,-0.0102029f,-0.211676f},
{-0.0902753f,0.00821416f,-0.322775f},{-0.0446028f,0.0106914f,-0.279868f},{-0.052541f,0.0449849f,-0.314724f},
{-0.0461982f,0.0508789f,-0.291149f},{-0.0381053f,0.0188975f,-0.252893f},{-0.136267f,0.0245979f,-0.334397f},
{-0.107764f,0.0418923f,-0.343822f},{-0.139722f,0.0493655f,-0.343546f},{-0.0916413f,0.0679614f,-0.330168f},
{-0.128587f,0.0837844f,-0.329198f},{-0.163815f,0.0555554f,-0.330985f},{-0.15648f,0.0580542f,-0.337451f},
{-0.154315f,0.0918567f,-0.317684f},{-0.176284f,0.0557869f,-0.289247f},{-0.18086f,0.0897312f,-0.239304f},
{-0.180116f,0.0575596f,-0.218787f},{-0.179136f,0.0410494f,-0.283279f},{-0.179128f,0.0736618f,-0.203431f},
{-0.135841f,0.114856f,-0.281804f},{-0.130391f,0.0997741f,-0.311692f},{-0.170165f,0.102987f,-0.243343f},
{-0.124904f,0.139718f,-0.244331f},{-0.00696389f,0.0350828f,-0.232823f},{0.0194494f,0.0287096f,-0.179445f},
{0.00287399f,0.049678f,-0.220562f},{-0.00458775f,0.0687205f,-0.25177f},{-0.0274773f,0.0887834f,-0.27602f},
{-0.0356826f,0.115418f,-0.258188f},{-0.143837f,0.00967127f,-0.317991f},{-0.167712f,0.0152184f,-0.251302f},
{-0.15359f,0.0143682f,-0.190693f},{-0.169094f,0.0528763f,-0.181446f},{-0.0872175f,0.0782216f,-0.318907f},
{-0.0864104f,0.0865567f,-0.305551f},{-0.0783891f,0.0950654f,-0.298305f},{-0.103422f,0.133539f,-0.274402f},
{-0.288152f,0.1655f,-0.0511635f},{-0.266954f,0.164747f,-0.0619371f},{-0.2778f,0.182302f,-0.0760781f},
{-0.207534f,0.192895f,-0.0530667f},{-0.250519f,0.18638f,-0.0788529f},{-0.265968f,0.263825f,-0.0615428f},
{-0.23322f,0.228408f,-0.0623931f},{-0.282826f,0.204301f,-0.0823468f},{-0.205844f,0.199944f,-0.0249226f},
{-0.230079f,0.241934f,-0.0511115f},{-0.271562f,0.272403f,-0.0521861f},{-0.24266f,0.2435f,0.0126426f},
{-0.233253f,0.2028f,0.0536539f},{-0.278118f,0.260153f,0.0585288f},{-0.276537f,0.268898f,0.0275569f},
{-0.254865f,0.182572f,0.0750166f},{-0.261225f,0.163656f,0.0422628f},{-0.282597f,0.179206f,0.0760092f},
{-0.235895f,0.181403f,0.0546334f},{-0.29594f,0.192947f,0.074846f},{-0.312349f,0.185269f,-0.0446073f},
{-0.29516f,0.163184f,-0.00430142f},{-0.311755f,0.215582f,0.0570156f},{-0.282592f,0.2247f,0.0745912f},
{-0.318791f,0.186458f,-0.0170373f},{-0.315103f,0.209213f,-0.0566466f},{-0.321362f,0.240217f,-0.0162382f},
{-0.318764f,0.218877f,0.0281584f},{-0.317986f,0.2377f,0.0305772f},{-0.30573f,0.266639f,-0.0443177f},
{-0.295796f,0.259668f,-0.0622418f},{-0.302354f,0.263169f,0.0193074f},{-0.301109f,0.203109f,-0.0760554f},
{-0.249645f,0.150942f,-0.00709588f},{-0.258409f,0.152589f,-0.0397157f},{-0.213251f,0.164917f,-0.0400839f},
{-0.222554f,0.175809f,0.0188499f},{0.124099f,0.203642f,-0.0350833f},{-0.0597429f,0.200351f,-0.0544071f},
{-0.0501925f,0.207779f,-0.0375085f},{0.00215434f,0.0939496f,-0.164839f},{0.0174867f,0.102259f,-0.136412f},
{0.0349367f,0.106812f,-0.121791f},{0.0626144f,0.0483987f,-0.0937112f},{0.045944f,0.116366f,-0.116804f},
{0.118345f,0.147037f,-0.097063f},{0.124374f,0.140449f,-0.0547437f},{0.141485f,0.17031f,-0.0512785f},
{0.127806f,0.169082f,-0.0300438f},{-0.0177824f,0.0290686f,-0.00744687f},{-0.0218476f,0.0706018f,-0.0286435f},
{-0.0350274f,0.135003f,-0.0297727f},{-0.0412759f,0.183221f,-0.0238137f},{-0.00915763f,-0.0734996f,0.0512089f},
{-0.0310345f,-0.0877228f,-0.0213134f},{-0.0177077f,-0.0177127f,0.00409945f},{-0.0194845f,-0.0650021f,-0.10196f},
{0.0143954f,0.00643052f,-0.172332f},{0.0495942f,-0.0139572f,-0.141772f},{0.0291047f,0.0591433f,-0.124668f},
{0.116609f,-0.0206492f,-0.061357f},{0.296255f,0.0734707f,-0.256394f},{0.303467f,0.0870718f,-0.228534f},
{0.194543f,0.0737429f,-0.156223f},{0.222268f,0.0873194f,-0.150408f},{0.130613f,0.169313f,-0.101042f},
{0.143981f,0.190839f,-0.10421f},{0.0497495f,0.429868f,0.216523f},{0.00934697f,0.427205f,0.244069f},
{-0.0167944f,0.429835f,0.285507f},{0.0165548f,0.462421f,0.219858f},{-0.00783403f,0.476959f,0.237408f},
{-0.00803933f,0.48755f,0.25798f},{-0.022741f,0.463275f,0.289685f},{0.0539975f,0.493592f,0.246866f},
{0.0536769f,0.463914f,0.21085f},{0.0217463f,0.487922f,0.227067f},{0.0210965f,0.5f,0.256958f},
{0.0116725f,0.492894f,0.290009f},{0.0584168f,0.489995f,0.291063f},{0.0434257f,0.374565f,0.278957f},
{0.010072f,0.401013f,0.315877f},{0.0470758f,0.378984f,0.318558f},{0.00739892f,0.427744f,0.340127f},
{-0.0122992f,0.432795f,0.3177f},{0.0537104f,0.449102f,0.331392f},{0.0509574f,0.407947f,0.343822f},
{0.0832332f,0.378663f,0.282743f},{0.0769359f,0.377986f,0.312925f},{0.0977739f,0.398343f,0.315869f},
{0.0826175f,0.39798f,0.331617f},{0.109087f,0.407435f,0.286965f},{0.0955836f,0.438732f,0.304628f},
{0.0803805f,0.400501f,0.244301f},{0.0826219f,0.432854f,0.220925f},{0.103592f,0.411769f,0.253173f},
{0.104771f,0.432633f,0.24036f},{0.00216266f,0.457553f,0.325569f},{0.0907404f,0.458156f,0.236296f}
};


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "kancr.h"
// 468 Verticies
// 785 Triangles

object *load_object(){
	int vertex_index[785][3];
	int material_index[785];
	int i, j;
	int mcount=0;
	int mindex=0;
	for(i=0;i<785;i++){
		for(j=0;j<3;j++){
			int vi=face_indicies[i][j];
			vertex_index[i][j] = vi;
		}
		material_index[i] = 0;
	}
	return make_object(
		(vector *)vertices,
		468,
		(int*)vertex_index,
		NULL,
		(int*)material_index,
		785,
		NULL,
		0,
		NULL,
		7
	);
}