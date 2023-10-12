#include "AWHeaders(M-Carbon).h"
#include <Resources.h>



struct ResourceDescription
{
	UInt32 rty;
	UInt32 rid;

	UInt32 dty;
	UInt32 did;
};

// keep them alphabetically sorted on field 3
static ResourceDescription sToTransfer[] = {

{ 'EMSG', 1, 'EMSG', 1 },
{ 'EMSG', 3, 'EMSG', 3 },
{ 'EMSG', 4, 'EMSG', 4 },
{ 'EMSG', 6, 'EMSG', 6 },
{ 'EMSG', 8, 'EMSG', 8 },
{ 'EMSG', 10, 'EMSG', 10 },
{ 'EMSG', 19, 'EMSG', 19 },

// the banner non movie version
{ 'GIFf', 128, 'GIFf', 128 },

// the many icons
{ 'cicn', 128, 'ICON', 128 },
{ 'cicn', 129, 'ICON', 129 },
{ 'cicn', 130, 'ICON', 130 },
{ 'cicn', 131, 'ICON', 131 },
{ 'cicn', 132, 'ICON', 132 },
{ 'cicn', 133, 'ICON', 133 },
{ 'cicn', 134, 'ICON', 134 },
{ 'cicn', 135, 'ICON', 135 },
{ 'cicn', 136, 'ICON', 136 },
{ 'cicn', 137, 'ICON', 137 },
{ 'cicn', 138, 'ICON', 138 },
{ 'cicn', 139, 'ICON', 139 },
{ 'cicn', 140, 'ICON', 140 },
{ 'cicn', 141, 'ICON', 141 },
{ 'cicn', 142, 'ICON', 142 },
{ 'cicn', 143, 'ICON', 143 },
{ 'cicn', 144, 'ICON', 144 },
{ 'cicn', 145, 'ICON', 145 },
{ 'cicn', 146, 'ICON', 146 },
{ 'cicn', 147, 'ICON', 147 },
{ 'cicn', 148, 'ICON', 148 },
{ 'cicn', 149, 'ICON', 149 },
{ 'cicn', 150, 'ICON', 150 },
{ 'cicn', 151, 'ICON', 151 },
{ 'cicn', 152, 'ICON', 152 },
{ 'cicn', 153, 'ICON', 153 },
{ 'cicn', 154, 'ICON', 154 },
{ 'cicn', 155, 'ICON', 155 },
{ 'cicn', 156, 'ICON', 156 },
{ 'cicn', 157, 'ICON', 157 },
{ 'cicn', 158, 'ICON', 158 },
{ 'cicn', 159, 'ICON', 159 },
{ 'cicn', 160, 'ICON', 160 },
{ 'cicn', 161, 'ICON', 161 },
{ 'cicn', 162, 'ICON', 162 },
{ 'cicn', 163, 'ICON', 163 },
{ 'cicn', 164, 'ICON', 164 },
{ 'cicn', 165, 'ICON', 165 },
{ 'cicn', 166, 'ICON', 166 },
{ 'cicn', 167, 'ICON', 167 },
{ 'cicn', 168, 'ICON', 168 },
{ 'cicn', 169, 'ICON', 169 },
{ 'cicn', 170, 'ICON', 170 },
{ 'cicn', 171, 'ICON', 171 },
{ 'cicn', 172, 'ICON', 172 },
{ 'cicn', 173, 'ICON', 173 },
{ 'cicn', 174, 'ICON', 174 },
{ 'cicn', 175, 'ICON', 175 },
{ 'cicn', 176, 'ICON', 176 },
{ 'cicn', 177, 'ICON', 177 },
{ 'cicn', 178, 'ICON', 178 },
{ 'cicn', 179, 'ICON', 179 },
{ 'cicn', 180, 'ICON', 180 },
{ 'cicn', 181, 'ICON', 181 },
{ 'cicn', 182, 'ICON', 182 },
{ 'cicn', 183, 'ICON', 183 },
{ 'cicn', 184, 'ICON', 184 },
{ 'cicn', 185, 'ICON', 185 },
{ 'cicn', 186, 'ICON', 186 },
{ 'cicn', 187, 'ICON', 187 },
{ 'cicn', 188, 'ICON', 188 },
{ 'cicn', 189, 'ICON', 189 },
{ 'cicn', 190, 'ICON', 190 },
{ 'cicn', 191, 'ICON', 191 },
{ 'cicn', 192, 'ICON', 192 },
{ 'cicn', 193, 'ICON', 193 },
{ 'cicn', 194, 'ICON', 194 },
{ 'cicn', 195, 'ICON', 195 },
{ 'cicn', 196, 'ICON', 196 },
{ 'cicn', 197, 'ICON', 197 },
{ 'cicn', 198, 'ICON', 198 },
{ 'cicn', 199, 'ICON', 199 },
{ 'cicn', 200, 'ICON', 200 },
{ 'cicn', 201, 'ICON', 201 },
{ 'cicn', 202, 'ICON', 202 },
{ 'cicn', 203, 'ICON', 203 },
{ 'cicn', 204, 'ICON', 204 },
{ 'cicn', 205, 'ICON', 205 },
{ 'cicn', 206, 'ICON', 206 },
{ 'cicn', 207, 'ICON', 207 },
{ 'cicn', 208, 'ICON', 208 },
{ 'cicn', 209, 'ICON', 209 },
{ 'cicn', 210, 'ICON', 210 },
{ 'cicn', 211, 'ICON', 211 },
{ 'cicn', 212, 'ICON', 212 },
{ 'cicn', 213, 'ICON', 213 },
{ 'cicn', 214, 'ICON', 214 },
{ 'cicn', 215, 'ICON', 215 },
{ 'cicn', 216, 'ICON', 216 },
{ 'cicn', 217, 'ICON', 217 },
{ 'cicn', 218, 'ICON', 218 },
{ 'cicn', 219, 'ICON', 219 },
{ 'cicn', 220, 'ICON', 220 },
{ 'cicn', 221, 'ICON', 221 },
{ 'cicn', 222, 'ICON', 222 },
{ 'cicn', 223, 'ICON', 223 },
{ 'cicn', 224, 'ICON', 224 },
{ 'cicn', 225, 'ICON', 225 },
{ 'cicn', 226, 'ICON', 226 },
{ 'cicn', 227, 'ICON', 227 },
{ 'cicn', 230, 'ICON', 230 },
{ 'cicn', 231, 'ICON', 231 },
{ 'cicn', 232, 'ICON', 232 },
{ 'cicn', 233, 'ICON', 233 },
{ 'cicn', 234, 'ICON', 234 },
{ 'cicn', 235, 'ICON', 235 },
{ 'cicn', 236, 'ICON', 236 },
{ 'cicn', 237, 'ICON', 237 },
{ 'cicn', 238, 'ICON', 238 },
{ 'cicn', 239, 'ICON', 239 },
{ 'cicn', 240, 'ICON', 240 },
{ 'cicn', 241, 'ICON', 241 },
{ 'cicn', 242, 'ICON', 242 },
{ 'cicn', 243, 'ICON', 243 },
{ 'cicn', 244, 'ICON', 244 },
{ 'cicn', 245, 'ICON', 245 },
{ 'cicn', 246, 'ICON', 246 },
{ 'cicn', 247, 'ICON', 247 },
{ 'cicn', 250, 'ICON', 250 },
{ 'cicn', 251, 'ICON', 251 },
{ 'cicn', 252, 'ICON', 252 },
{ 'cicn', 253, 'ICON', 253 },
{ 'cicn', 276, 'ICON', 276 },
{ 'cicn', 277, 'ICON', 277 },
{ 'cicn', 278, 'ICON', 278 },
{ 'cicn', 400, 'ICON', 400 },
{ 'cicn', 401, 'ICON', 401 },
{ 'cicn', 402, 'ICON', 402 },
{ 'cicn', 403, 'ICON', 403 },
{ 'cicn', 404, 'ICON', 404 },
{ 'cicn', 405, 'ICON', 405 },
{ 'cicn', 406, 'ICON', 406 },
{ 'cicn', 407, 'ICON', 407 },
{ 'cicn', 408, 'ICON', 408 },
{ 'cicn', 409, 'ICON', 409 },
{ 'cicn', 410, 'ICON', 410 },
{ 'cicn', 411, 'ICON', 411 },
{ 'cicn', 412, 'ICON', 412 },
{ 'cicn', 413, 'ICON', 413 },
{ 'cicn', 414, 'ICON', 414 },
{ 'cicn', 415, 'ICON', 415 },
{ 'cicn', 416, 'ICON', 416 },
{ 'cicn', 417, 'ICON', 417 },
{ 'cicn', 418, 'ICON', 418 },
{ 'cicn', 419, 'ICON', 419 },
{ 'cicn', 420, 'ICON', 420 },
{ 'cicn', 421, 'ICON', 421 },
{ 'cicn', 422, 'ICON', 422 },
{ 'cicn', 423, 'ICON', 423 },
{ 'cicn', 424, 'ICON', 424 },
{ 'cicn', 425, 'ICON', 425 },
{ 'cicn', 426, 'ICON', 426 },
{ 'cicn', 427, 'ICON', 427 },
{ 'cicn', 500, 'ICON', 500 },
{ 'cicn', 666, 'ICON', 666 },
{ 'cicn', 1250, 'ICON', 1250 },
{ 'cicn', 1251, 'ICON', 1251 },
{ 'cicn', 1968, 'ICON', 1968 },
{ 'cicn', 1969, 'ICON', 1969 },
{ 'cicn', 2000, 'ICON', 2000 },
{ 'cicn', 2001, 'ICON', 2001 },
{ 'cicn', 2002, 'ICON', 2002 },
{ 'cicn', 2003, 'ICON', 2003 },
{ 'cicn', 2004, 'ICON', 2004 },
{ 'cicn', 2006, 'ICON', 2006 },
{ 'cicn', 2007, 'ICON', 2007 },
{ 'cicn', 2008, 'ICON', 2008 },
{ 'cicn', 2009, 'ICON', 2009 },
{ 'cicn', 2010, 'ICON', 2010 },
{ 'cicn', 2011, 'ICON', 2011 },
{ 'cicn', 2012, 'ICON', 2012 },
{ 'cicn', 2013, 'ICON', 2013 },
{ 'cicn', 2014, 'ICON', 2014 },
{ 'cicn', 2015, 'ICON', 2015 },
{ 'cicn', 2016, 'ICON', 2016 },
{ 'cicn', 2017, 'ICON', 2017 },
{ 'cicn', 2018, 'ICON', 2018 },
{ 'cicn', 2019, 'ICON', 2019 },
{ 'cicn', 2020, 'ICON', 2020 },
{ 'cicn', 2021, 'ICON', 2021 },
{ 'cicn', 2022, 'ICON', 2022 },
{ 'cicn', 2023, 'ICON', 2023 },
{ 'cicn', 2024, 'ICON', 2024 },
{ 'cicn', 2025, 'ICON', 2025 },
{ 'cicn', 2026, 'ICON', 2026 },
{ 'cicn', 2027, 'ICON', 2027 },
{ 'cicn', 2028, 'ICON', 2028 },
{ 'cicn', 2029, 'ICON', 2029 },
{ 'cicn', 2030, 'ICON', 2030 },
{ 'cicn', 2031, 'ICON', 2031 },
{ 'cicn', 2032, 'ICON', 2032 },
{ 'cicn', 2033, 'ICON', 2033 },
{ 'cicn', 2034, 'ICON', 2034 },
{ 'cicn', 2035, 'ICON', 2035 },
{ 'cicn', 2036, 'ICON', 2036 },
{ 'cicn', 2037, 'ICON', 2037 },
{ 'cicn', 2038, 'ICON', 2038 },
{ 'cicn', 2040, 'ICON', 2040 },
{ 'cicn', 2041, 'ICON', 2041 },
{ 'cicn', 2042, 'ICON', 2042 },
{ 'cicn', 2043, 'ICON', 2043 },
{ 'cicn', 2044, 'ICON', 2044 },
{ 'cicn', 2045, 'ICON', 2045 },
{ 'cicn', 2046, 'ICON', 2046 },
{ 'cicn', 2047, 'ICON', 2047 },
{ 'cicn', 2048, 'ICON', 2048 },
{ 'cicn', 2049, 'ICON', 2049 },
{ 'cicn', 2050, 'ICON', 2050 },
{ 'cicn', 2051, 'ICON', 2051 },
{ 'cicn', 2052, 'ICON', 2052 },
{ 'cicn', 2053, 'ICON', 2053 },
{ 'cicn', 2054, 'ICON', 2054 },
{ 'cicn', 2055, 'ICON', 2055 },
{ 'cicn', 2056, 'ICON', 2056 },
{ 'cicn', 2057, 'ICON', 2057 },
{ 'cicn', 2058, 'ICON', 2058 },
{ 'cicn', 2059, 'ICON', 2059 },
{ 'cicn', 2060, 'ICON', 2060 },
{ 'cicn', 2061, 'ICON', 2061 },
{ 'cicn', 2062, 'ICON', 2062 },
{ 'cicn', 2063, 'ICON', 2063 },
{ 'cicn', 2064, 'ICON', 2064 },
{ 'cicn', 2065, 'ICON', 2065 },
{ 'cicn', 2066, 'ICON', 2066 },
{ 'cicn', 2067, 'ICON', 2067 },
{ 'cicn', 2070, 'ICON', 2070 },
{ 'cicn', 2071, 'ICON', 2071 },
{ 'cicn', 2072, 'ICON', 2072 },
{ 'cicn', 2073, 'ICON', 2073 },
{ 'cicn', 2075, 'ICON', 2075 },
{ 'cicn', 2079, 'ICON', 2079 },
{ 'cicn', 2098, 'ICON', 2098 },
{ 'cicn', 2100, 'ICON', 2100 },
{ 'cicn', 2101, 'ICON', 2101 },
{ 'cicn', 2102, 'ICON', 2102 },
{ 'cicn', 2103, 'ICON', 2103 },
{ 'cicn', 2104, 'ICON', 2104 },
{ 'cicn', 2105, 'ICON', 2105 },
{ 'cicn', 2106, 'ICON', 2106 },
{ 'cicn', 2107, 'ICON', 2107 },
{ 'cicn', 2108, 'ICON', 2108 },
{ 'cicn', 2109, 'ICON', 2109 },
{ 'cicn', 2110, 'ICON', 2110 },
{ 'cicn', 2112, 'ICON', 2112 },
{ 'cicn', 2113, 'ICON', 2113 },
{ 'cicn', 2115, 'ICON', 2115 },
{ 'cicn', 2116, 'ICON', 2116 },
{ 'cicn', 2117, 'ICON', 2117 },
{ 'cicn', 2118, 'ICON', 2118 },
{ 'cicn', 2119, 'ICON', 2119 },
{ 'cicn', 2120, 'ICON', 2120 },
{ 'cicn', 2121, 'ICON', 2121 },
{ 'cicn', 2122, 'ICON', 2122 },
{ 'cicn', 2123, 'ICON', 2123 },
{ 'cicn', 2124, 'ICON', 2124 },
{ 'cicn', 2125, 'ICON', 2125 },
{ 'cicn', 2126, 'ICON', 2126 },
{ 'cicn', 2223, 'ICON', 2223 },
{ 'cicn', 2400, 'ICON', 2400 },
{ 'cicn', 2401, 'ICON', 2401 },
{ 'cicn', 2402, 'ICON', 2402 },
{ 'cicn', 2403, 'ICON', 2403 },
{ 'cicn', 2404, 'ICON', 2404 },
{ 'cicn', 2500, 'ICON', 2500 },
{ 'cicn', 2501, 'ICON', 2501 },
{ 'cicn', 2502, 'ICON', 2502 },
{ 'cicn', 2503, 'ICON', 2503 },
{ 'cicn', 2504, 'ICON', 2504 },
{ 'cicn', 2505, 'ICON', 2505 },
{ 'cicn', 2506, 'ICON', 2506 },
{ 'cicn', 2507, 'ICON', 2507 },
{ 'cicn', 2528, 'ICON', 2528 },
{ 'cicn', 2529, 'ICON', 2529 },
{ 'cicn', 2530, 'ICON', 2530 },
{ 'cicn', 2531, 'ICON', 2531 },
{ 'cicn', 2532, 'ICON', 2532 },
{ 'cicn', 2533, 'ICON', 2533 },
{ 'cicn', 2534, 'ICON', 2534 },
{ 'cicn', 2535, 'ICON', 2535 },
{ 'cicn', 2536, 'ICON', 2536 },
{ 'cicn', 2537, 'ICON', 2537 },
{ 'cicn', 2538, 'ICON', 2538 },
{ 'cicn', 2539, 'ICON', 2539 },
{ 'cicn', 2540, 'ICON', 2540 },
{ 'cicn', 2541, 'ICON', 2541 },
{ 'cicn', 2542, 'ICON', 2542 },
{ 'cicn', 2543, 'ICON', 2543 },
{ 'cicn', 2544, 'ICON', 2544 },
{ 'cicn', 2545, 'ICON', 2545 },
{ 'cicn', 2546, 'ICON', 2546 },
{ 'cicn', 2547, 'ICON', 2547 },
{ 'cicn', 2548, 'ICON', 2548 },
{ 'cicn', 2549, 'ICON', 2549 },
{ 'cicn', 2550, 'ICON', 2550 },
{ 'cicn', 2551, 'ICON', 2551 },
{ 'cicn', 2552, 'ICON', 2552 },
{ 'cicn', 2553, 'ICON', 2553 },
{ 'cicn', 2554, 'ICON', 2554 },
{ 'cicn', 2555, 'ICON', 2555 },
{ 'cicn', 2556, 'ICON', 2556 },
{ 'cicn', 2557, 'ICON', 2557 },
{ 'cicn', 2558, 'ICON', 2558 },
{ 'cicn', 2559, 'ICON', 2559 },
{ 'cicn', 2560, 'ICON', 2560 },
{ 'cicn', 2561, 'ICON', 2561 },
{ 'cicn', 2562, 'ICON', 2562 },
{ 'cicn', 2563, 'ICON', 2563 },
{ 'cicn', 2564, 'ICON', 2564 },
{ 'cicn', 2565, 'ICON', 2565 },
{ 'cicn', 2566, 'ICON', 2566 },
{ 'cicn', 2567, 'ICON', 2567 },
{ 'cicn', 2568, 'ICON', 2568 },
{ 'cicn', 2569, 'ICON', 2569 },
{ 'cicn', 2570, 'ICON', 2570 },
{ 'cicn', 2571, 'ICON', 2571 },
{ 'cicn', 2572, 'ICON', 2572 },
{ 'cicn', 2573, 'ICON', 2573 },
{ 'cicn', 2574, 'ICON', 2574 },
{ 'cicn', 2575, 'ICON', 2575 },
{ 'cicn', 2576, 'ICON', 2576 },
{ 'cicn', 2577, 'ICON', 2577 },
{ 'cicn', 2578, 'ICON', 2578 },
{ 'cicn', 2579, 'ICON', 2579 },
{ 'cicn', 2580, 'ICON', 2580 },
{ 'cicn', 2581, 'ICON', 2581 },
{ 'cicn', 2582, 'ICON', 2582 },
{ 'cicn', 2583, 'ICON', 2583 },
{ 'cicn', 2584, 'ICON', 2584 },
{ 'cicn', 2585, 'ICON', 2585 },
{ 'cicn', 2586, 'ICON', 2586 },
{ 'cicn', 2587, 'ICON', 2587 },
{ 'cicn', 2588, 'ICON', 2588 },
{ 'cicn', 2589, 'ICON', 2589 },
{ 'cicn', 2590, 'ICON', 2590 },
{ 'cicn', 2591, 'ICON', 2591 },
{ 'cicn', 2592, 'ICON', 2592 },
{ 'cicn', 2593, 'ICON', 2593 },
{ 'cicn', 2594, 'ICON', 2594 },
{ 'cicn', 2595, 'ICON', 2595 },
{ 'cicn', 2596, 'ICON', 2596 },
{ 'cicn', 2597, 'ICON', 2597 },
{ 'cicn', 2598, 'ICON', 2598 },
{ 'cicn', 2599, 'ICON', 2599 },
{ 'cicn', 2600, 'ICON', 2600 },
{ 'cicn', 2818, 'ICON', 2818 },
{ 'cicn', 3394, 'ICON', 3394 },
{ 'cicn', 3395, 'ICON', 3395 },
{ 'cicn', 4000, 'ICON', 4000 },
{ 'cicn', 4001, 'ICON', 4001 },
{ 'cicn', 4002, 'ICON', 4002 },
{ 'cicn', 4003, 'ICON', 4003 },
{ 'cicn', 4004, 'ICON', 4004 },
{ 'cicn', 4005, 'ICON', 4005 },
{ 'cicn', 4006, 'ICON', 4006 },
{ 'cicn', 4007, 'ICON', 4007 },
{ 'cicn', 4008, 'ICON', 4008 },
{ 'cicn', 4009, 'ICON', 4009 },
{ 'cicn', 4010, 'ICON', 4010 },
{ 'cicn', 4011, 'ICON', 4011 },
{ 'cicn', 4012, 'ICON', 4012 },
{ 'cicn', 4013, 'ICON', 4013 },
{ 'cicn', 4014, 'ICON', 4014 },
{ 'cicn', 4015, 'ICON', 4015 },
{ 'cicn', 4016, 'ICON', 4016 },
{ 'cicn', 4017, 'ICON', 4017 },
{ 'cicn', 4018, 'ICON', 4018 },
{ 'cicn', 4019, 'ICON', 4019 },
{ 'cicn', 4020, 'ICON', 4020 },
{ 'cicn', 4021, 'ICON', 4021 },
{ 'cicn', 4022, 'ICON', 4022 },
{ 'cicn', 4023, 'ICON', 4023 },
{ 'cicn', 4024, 'ICON', 4024 },
{ 'cicn', 4025, 'ICON', 4025 },
{ 'cicn', 4026, 'ICON', 4026 },
{ 'cicn', 4027, 'ICON', 4027 },
{ 'cicn', 4028, 'ICON', 4028 },
{ 'cicn', 4029, 'ICON', 4029 },
{ 'cicn', 4030, 'ICON', 4030 },
{ 'cicn', 4031, 'ICON', 4031 },
{ 'cicn', 4032, 'ICON', 4032 },
{ 'cicn', 4033, 'ICON', 4033 },
{ 'cicn', 4034, 'ICON', 4034 },
{ 'cicn', 4035, 'ICON', 4035 },
{ 'cicn', 4036, 'ICON', 4036 },
{ 'cicn', 4037, 'ICON', 4037 },
{ 'cicn', 4038, 'ICON', 4038 },
{ 'cicn', 4039, 'ICON', 4039 },
{ 'cicn', 4040, 'ICON', 4040 },
{ 'cicn', 4041, 'ICON', 4041 },
{ 'cicn', 4042, 'ICON', 4042 },
{ 'cicn', 4043, 'ICON', 4043 },
{ 'cicn', 4044, 'ICON', 4044 },
{ 'cicn', 4045, 'ICON', 4045 },
{ 'cicn', 4046, 'ICON', 4046 },
{ 'cicn', 4047, 'ICON', 4047 },
{ 'cicn', 4048, 'ICON', 4048 },
{ 'cicn', 4049, 'ICON', 4049 },
{ 'cicn', 4050, 'ICON', 4050 },
{ 'cicn', 4051, 'ICON', 4051 },
{ 'cicn', 4052, 'ICON', 4052 },
{ 'cicn', 4053, 'ICON', 4053 },
{ 'cicn', 4054, 'ICON', 4054 },
{ 'cicn', 4055, 'ICON', 4055 },
{ 'cicn', 4056, 'ICON', 4056 },
{ 'cicn', 4057, 'ICON', 4057 },
{ 'cicn', 4058, 'ICON', 4058 },
{ 'cicn', 4059, 'ICON', 4059 },
{ 'cicn', 4060, 'ICON', 4060 },
{ 'cicn', 4061, 'ICON', 4061 },
{ 'cicn', 4062, 'ICON', 4062 },
{ 'cicn', 4063, 'ICON', 4063 },
{ 'cicn', 4064, 'ICON', 4064 },
{ 'cicn', 4065, 'ICON', 4065 },
{ 'cicn', 4066, 'ICON', 4066 },
{ 'cicn', 4067, 'ICON', 4067 },
{ 'cicn', 4068, 'ICON', 4068 },
{ 'cicn', 4069, 'ICON', 4069 },
{ 'cicn', 4070, 'ICON', 4070 },
{ 'cicn', 4071, 'ICON', 4071 },
{ 'cicn', 4072, 'ICON', 4072 },
{ 'cicn', 4073, 'ICON', 4073 },
{ 'cicn', 4074, 'ICON', 4074 },
{ 'cicn', 4075, 'ICON', 4075 },
{ 'cicn', 4076, 'ICON', 4076 },
{ 'cicn', 4077, 'ICON', 4077 },
{ 'cicn', 4078, 'ICON', 4078 },
{ 'cicn', 4079, 'ICON', 4079 },
{ 'cicn', 4080, 'ICON', 4080 },
{ 'cicn', 4081, 'ICON', 4081 },
{ 'cicn', 4082, 'ICON', 4082 },
{ 'cicn', 4083, 'ICON', 4083 },
{ 'cicn', 4084, 'ICON', 4084 },
{ 'cicn', 4085, 'ICON', 4085 },
{ 'cicn', 4086, 'ICON', 4086 },
{ 'cicn', 4087, 'ICON', 4087 },
{ 'cicn', 4088, 'ICON', 4088 },
{ 'cicn', 4089, 'ICON', 4089 },
{ 'cicn', 4090, 'ICON', 4090 },
{ 'cicn', 4091, 'ICON', 4091 },
{ 'cicn', 4092, 'ICON', 4092 },
{ 'cicn', 4093, 'ICON', 4093 },
{ 'cicn', 4094, 'ICON', 4094 },
{ 'cicn', 4095, 'ICON', 4095 },
{ 'cicn', 4096, 'ICON', 4096 },
{ 'cicn', 4097, 'ICON', 4097 },
{ 'cicn', 4098, 'ICON', 4098 },
{ 'cicn', 4099, 'ICON', 4099 },
{ 'cicn', 4100, 'ICON', 4100 },
{ 'cicn', 4101, 'ICON', 4101 },
{ 'cicn', 4102, 'ICON', 4102 },
{ 'cicn', 4103, 'ICON', 4103 },
{ 'cicn', 4104, 'ICON', 4104 },
{ 'cicn', 4105, 'ICON', 4105 },
{ 'cicn', 4106, 'ICON', 4106 },
{ 'cicn', 4107, 'ICON', 4107 },
{ 'cicn', 4108, 'ICON', 4108 },
{ 'cicn', 4109, 'ICON', 4109 },
{ 'cicn', 4110, 'ICON', 4110 },
{ 'cicn', 4111, 'ICON', 4111 },
{ 'cicn', 4112, 'ICON', 4112 },
{ 'cicn', 4113, 'ICON', 4113 },
{ 'cicn', 4114, 'ICON', 4114 },
{ 'cicn', 4115, 'ICON', 4115 },
{ 'cicn', 4116, 'ICON', 4116 },
{ 'cicn', 4117, 'ICON', 4117 },
{ 'cicn', 4118, 'ICON', 4118 },
{ 'cicn', 4119, 'ICON', 4119 },
{ 'cicn', 4120, 'ICON', 4120 },
{ 'cicn', 4121, 'ICON', 4121 },
{ 'cicn', 4122, 'ICON', 4122 },
{ 'cicn', 4123, 'ICON', 4123 },
{ 'cicn', 4124, 'ICON', 4124 },
{ 'cicn', 4125, 'ICON', 4125 },
{ 'cicn', 4126, 'ICON', 4126 },
{ 'cicn', 4127, 'ICON', 4127 },
{ 'cicn', 4128, 'ICON', 4128 },
{ 'cicn', 4129, 'ICON', 4129 },
{ 'cicn', 4130, 'ICON', 4130 },
{ 'cicn', 4131, 'ICON', 4131 },
{ 'cicn', 4132, 'ICON', 4132 },
{ 'cicn', 4133, 'ICON', 4133 },
{ 'cicn', 4134, 'ICON', 4134 },
{ 'cicn', 4135, 'ICON', 4135 },
{ 'cicn', 4136, 'ICON', 4136 },
{ 'cicn', 4137, 'ICON', 4137 },
{ 'cicn', 4138, 'ICON', 4138 },
{ 'cicn', 4139, 'ICON', 4139 },
{ 'cicn', 4140, 'ICON', 4140 },
{ 'cicn', 4141, 'ICON', 4141 },
{ 'cicn', 4142, 'ICON', 4142 },
{ 'cicn', 4143, 'ICON', 4143 },
{ 'cicn', 4144, 'ICON', 4144 },
{ 'cicn', 4145, 'ICON', 4145 },
{ 'cicn', 4146, 'ICON', 4146 },
{ 'cicn', 4147, 'ICON', 4147 },
{ 'cicn', 4148, 'ICON', 4148 },
{ 'cicn', 4149, 'ICON', 4149 },
{ 'cicn', 4150, 'ICON', 4150 },
{ 'cicn', 4151, 'ICON', 4151 },
{ 'cicn', 4152, 'ICON', 4152 },
{ 'cicn', 4153, 'ICON', 4153 },
{ 'cicn', 4154, 'ICON', 4154 },
{ 'cicn', 4155, 'ICON', 4155 },
{ 'cicn', 4156, 'ICON', 4156 },
{ 'cicn', 4157, 'ICON', 4157 },
{ 'cicn', 4158, 'ICON', 4158 },
{ 'cicn', 4159, 'ICON', 4159 },
{ 'cicn', 4160, 'ICON', 4160 },
{ 'cicn', 4161, 'ICON', 4161 },
{ 'cicn', 4162, 'ICON', 4162 },
{ 'cicn', 4163, 'ICON', 4163 },
{ 'cicn', 4164, 'ICON', 4164 },
{ 'cicn', 4165, 'ICON', 4165 },
{ 'cicn', 4166, 'ICON', 4166 },
{ 'cicn', 4167, 'ICON', 4167 },
{ 'cicn', 4168, 'ICON', 4168 },
{ 'cicn', 4169, 'ICON', 4169 },
{ 'cicn', 4170, 'ICON', 4170 },
{ 'cicn', 4171, 'ICON', 4171 },
{ 'cicn', 4172, 'ICON', 4172 },
{ 'cicn', 4173, 'ICON', 4173 },
{ 'cicn', 4174, 'ICON', 4174 },
{ 'cicn', 4175, 'ICON', 4175 },
{ 'cicn', 4176, 'ICON', 4176 },
{ 'cicn', 4177, 'ICON', 4177 },
{ 'cicn', 4178, 'ICON', 4178 },
{ 'cicn', 4179, 'ICON', 4179 },
{ 'cicn', 4180, 'ICON', 4180 },
{ 'cicn', 4181, 'ICON', 4181 },
{ 'cicn', 4182, 'ICON', 4182 },
{ 'cicn', 4183, 'ICON', 4183 },
{ 'cicn', 4184, 'ICON', 4184 },
{ 'cicn', 4185, 'ICON', 4185 },
{ 'cicn', 4186, 'ICON', 4186 },
{ 'cicn', 4187, 'ICON', 4187 },
{ 'cicn', 4188, 'ICON', 4188 },
{ 'cicn', 4189, 'ICON', 4189 },
{ 'cicn', 4190, 'ICON', 4190 },
{ 'cicn', 4191, 'ICON', 4191 },
{ 'cicn', 4192, 'ICON', 4192 },
{ 'cicn', 4193, 'ICON', 4193 },
{ 'cicn', 4194, 'ICON', 4194 },
{ 'cicn', 4195, 'ICON', 4195 },
{ 'cicn', 4196, 'ICON', 4196 },
{ 'cicn', 4197, 'ICON', 4197 },
{ 'cicn', 4198, 'ICON', 4198 },
{ 'cicn', 4199, 'ICON', 4199 },
{ 'cicn', 4200, 'ICON', 4200 },
{ 'cicn', 4201, 'ICON', 4201 },
{ 'cicn', 4202, 'ICON', 4202 },
{ 'cicn', 4203, 'ICON', 4203 },
{ 'cicn', 4204, 'ICON', 4204 },
{ 'cicn', 4205, 'ICON', 4205 },
{ 'cicn', 4206, 'ICON', 4206 },
{ 'cicn', 4207, 'ICON', 4207 },
{ 'cicn', 4208, 'ICON', 4208 },
{ 'cicn', 4209, 'ICON', 4209 },
{ 'cicn', 4210, 'ICON', 4210 },
{ 'cicn', 4211, 'ICON', 4211 },
{ 'cicn', 4212, 'ICON', 4212 },
{ 'cicn', 4213, 'ICON', 4213 },
{ 'cicn', 4214, 'ICON', 4214 },
{ 'cicn', 4215, 'ICON', 4215 },
{ 'cicn', 4216, 'ICON', 4216 },
{ 'cicn', 4217, 'ICON', 4217 },
{ 'cicn', 4218, 'ICON', 4218 },
{ 'cicn', 4219, 'ICON', 4219 },
{ 'cicn', 4220, 'ICON', 4220 },
{ 'cicn', 4221, 'ICON', 4221 },
{ 'cicn', 4222, 'ICON', 4222 },
{ 'cicn', 4223, 'ICON', 4223 },
{ 'cicn', 4224, 'ICON', 4224 },
{ 'cicn', 4225, 'ICON', 4225 },
{ 'cicn', 4226, 'ICON', 4226 },
{ 'cicn', 4227, 'ICON', 4227 },
{ 'cicn', 4228, 'ICON', 4228 },
{ 'cicn', 4229, 'ICON', 4229 },
{ 'cicn', 4230, 'ICON', 4230 },
{ 'cicn', 4231, 'ICON', 4231 },
{ 'cicn', 4232, 'ICON', 4232 },
{ 'cicn', 4233, 'ICON', 4233 },
{ 'cicn', 4234, 'ICON', 4234 },
{ 'cicn', 4235, 'ICON', 4235 },
{ 'cicn', 4236, 'ICON', 4236 },
{ 'cicn', 4238, 'ICON', 4238 },
{ 'cicn', 4240, 'ICON', 4240 },
{ 'cicn', 4241, 'ICON', 4241 },
{ 'cicn', 4242, 'ICON', 4242 },
{ 'cicn', 4243, 'ICON', 4243 },
{ 'cicn', 4244, 'ICON', 4244 },
{ 'cicn', 4245, 'ICON', 4245 },
{ 'cicn', 4246, 'ICON', 4246 },
{ 'cicn', 4247, 'ICON', 4247 },
{ 'cicn', 4248, 'ICON', 4248 },
{ 'cicn', 4249, 'ICON', 4249 },
{ 'cicn', 4250, 'ICON', 4250 },
{ 'cicn', 4251, 'ICON', 4251 },
{ 'cicn', 4252, 'ICON', 4252 },
{ 'cicn', 4253, 'ICON', 4253 },
{ 'cicn', 4254, 'ICON', 4254 },
{ 'cicn', 5998, 'ICON', 5998 },
{ 'cicn', 6000, 'ICON', 6000 },
{ 'cicn', 6001, 'ICON', 6001 },
{ 'cicn', 6002, 'ICON', 6002 },
{ 'cicn', 6003, 'ICON', 6003 },
{ 'cicn', 6004, 'ICON', 6004 },
{ 'cicn', 6005, 'ICON', 6005 },
{ 'cicn', 6008, 'ICON', 6008 },
{ 'cicn', 6009, 'ICON', 6009 },
{ 'cicn', 6010, 'ICON', 6010 },
{ 'cicn', 6011, 'ICON', 6011 },
{ 'cicn', 6012, 'ICON', 6012 },
{ 'cicn', 6013, 'ICON', 6013 },
{ 'cicn', 6014, 'ICON', 6014 },
{ 'cicn', 6015, 'ICON', 6015 },
{ 'cicn', 6016, 'ICON', 6016 },
{ 'cicn', 6017, 'ICON', 6017 },
{ 'cicn', 6018, 'ICON', 6018 },
{ 'cicn', 6023, 'ICON', 6023 },
{ 'cicn', 6025, 'ICON', 6025 },
{ 'cicn', 6026, 'ICON', 6026 },
{ 'cicn', 6027, 'ICON', 6027 },
{ 'cicn', 6028, 'ICON', 6028 },
{ 'cicn', 6029, 'ICON', 6029 },
{ 'cicn', 6030, 'ICON', 6030 },
{ 'cicn', 6031, 'ICON', 6031 },
{ 'cicn', 6032, 'ICON', 6032 },
{ 'cicn', 6033, 'ICON', 6033 },
{ 'cicn', 6034, 'ICON', 6034 },
{ 'cicn', 6035, 'ICON', 6035 },
{ 'cicn', 14068, 'ICON', 14068 },
{ 'cicn', 14069, 'ICON', 14069 },
{ 'cicn', 14070, 'ICON', 14070 },
{ 'cicn', 14071, 'ICON', 14071 },
{ 'cicn', 14072, 'ICON', 14072 },
{ 'cicn', 20000, 'ICON', 20000 },
{ 'cicn', 30000, 'ICON', 30000 },
{ 'cicn', 31337, 'ICON', 31337 },
{ 'cicn', 428, 'ICON', 428 },
{ 'cicn', 429, 'ICON', 429 },
{ 'cicn', 430, 'ICON', 430 },
{ 'cicn', 431, 'ICON', 431 },
{ 'cicn', 432, 'ICON', 432 },
{ 'cicn', 1000, 'ICON', 1000 },
{ 'cicn', 1001, 'ICON', 1001 },
{ 'cicn', 1002, 'ICON', 1002 },
{ 'cicn', 1003, 'ICON', 1003 },
// message boxes
{ 'MSGB', 128, 'MSGB', 128 },
{ 'MSGB', 129, 'MSGB', 129 },
{ 'MSGB', 130, 'MSGB', 130 },
{ 'MSGB', 131, 'MSGB', 131 },
{ 'MSGB', 132, 'MSGB', 132 },
{ 'MSGB', 133, 'MSGB', 133 },
{ 'MSGB', 134, 'MSGB', 134 },
{ 'MSGB', 135, 'MSGB', 135 },
{ 'MSGB', 136, 'MSGB', 136 },
{ 'MSGB', 137, 'MSGB', 137 },
{ 'MSGB', 138, 'MSGB', 138 },
{ 'MSGB', 139, 'MSGB', 139 },
{ 'MSGB', 140, 'MSGB', 140 },
{ 'MSGB', 141, 'MSGB', 141 },
{ 'MSGB', 142, 'MSGB', 142 },
{ 'MSGB', 143, 'MSGB', 143 },
{ 'MSGB', 144, 'MSGB', 144 },
{ 'MSGB', 145, 'MSGB', 145 },
{ 'MSGB', 146, 'MSGB', 146 },
{ 'MSGB', 147, 'MSGB', 147 },
{ 'MSGB', 148, 'MSGB', 148 },
{ 'MSGB', 149, 'MSGB', 149 },
{ 'MSGB', 150, 'MSGB', 150 },
{ 'MSGB', 151, 'MSGB', 151 },
{ 'MSGB', 160, 'MSGB', 160 },
{ 'MSGB', 161, 'MSGB', 161 },
{ 'MSGB', 162, 'MSGB', 162 },
{ 'MSGB', 163, 'MSGB', 163 },
// the banner movie
{ 'MooV', 128, 'MooV', 128 },
// not sure about this
{ 'PICT', 130, 'PIXM', 130 },
//{ 'ppat', 128, 'ppat', 128 },
{ 'snd ', 129, 'SOUN', 129 },
{ 'snd ', 130, 'SOUN', 130 },
{ 'snd ', 131, 'SOUN', 131 },
{ 'snd ', 132, 'SOUN', 132 },
{ 'snd ', 133, 'SOUN', 133 },
{ 'snd ', 134, 'SOUN', 134 },
{ 'snd ', 135, 'SOUN', 135 },
{ 'snd ', 136, 'SOUN', 136 },
{ 'snd ', 137, 'SOUN', 137 },
{ 'snd ', 138, 'SOUN', 138 },
// the autoupdate binaries may not be compatible between mac and windows ?!
{ 'harc', 128, 'harc', 128 },
};


static void ShowMessage(UInt8* msg)
{
  	SMsgBox mb;
	ClearStruct(mb);
	mb.messageData = msg+1;
	mb.messageSize = msg[0];
	MsgBox(mb);
} // Show Message
static UInt8* FourCharToStr255(UInt32 n)
{
	static UInt8 txt[5];
	txt[0] = 4;
	*(UInt32*)(txt+1) = n;
	return txt;
}

class MacResourceFile
{
	SInt16 mHandle;

public:

	MacResourceFile(UInt8* folderName, UInt8* srcRezName)
	: 	mHandle(-1)
	,	mResHandle(0)
	{
		mHandle = UFileSys::OpenResourceFork(kProgramFolder,folderName, srcRezName, 0);
		if (mHandle == -1)
			throw 1; // fail
		CleanCache();	
	} // constructor

    ~MacResourceFile()
    {
    	CleanCache();

    	if (mHandle != -1)
    		::CloseResFile(mHandle);
    	mHandle = -1;
    }
    
    THdl mResHandle;
    SInt16 mResID;
    ResType mResType;
    UInt8 mResName[256];
    UInt32 mResSize;
    
    void CleanCache()
    {
    	if (mResHandle != 0)
    		delete mResHandle;
		mResHandle = 0;
		mResName[0] = 0;	
		mResSize = 0;
    }
    
    THdl GetResource(ResType rt, SInt16 id)
    {
		CleanCache();
		
    	SInt16 cr = ::CurResFile();
    	::UseResFile(mHandle);
    	Handle h = ::GetResource(rt, id);
    	::UseResFile(cr);
    	if (h != 0)
    	{
    		::GetResInfo(h, &mResID, &mResType, mResName);
    		mResSize = ::GetResourceSizeOnDisk(h);
			Uint32 z = ::GetHandleSize(h);	
			::HLock(h);
    		mResHandle = UMemory::NewHandle(h, mResSize);
    		::HUnlock(h);
    	}
    	return mResHandle; // may be null
    }
    
    void RemoveResource(ResType rt, SInt16 id)
    {
		CleanCache();	

    	SInt16 cr = ::CurResFile();
    	::UseResFile(mHandle);
    	Handle h = ::GetResource(rt, id);
    	if (h != 0)
    	{
			::RemoveResource(h);
			::DisposeHandle(h);
		}
    	::UseResFile(cr);
    }
    void AddResource(ResType rt, SInt16 id, THdl reshandle, UInt8* name)
    {
		CleanCache();	

    	SInt16 cr = ::CurResFile();
    	::UseResFile(mHandle);
    	Handle h = ::GetResource(rt, id);
    	if (h != 0)
    	{
			::RemoveResource(h);
			::DisposeHandle(h);
		}
		::AddResource((Handle)reshandle, rt, id, name);
		
    	::UseResFile(cr);
    }

    void AddFromFile(Uint32 dty, Int32 did, UInt8* name,
    				 UInt8* folderName, UInt8* importName )
    {

		TFSRefObj* file = UFileSys::New(kProgramFolder, folderName, importName);
		if (!file->Exists())
		{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "File %#s/%#s does not exist, did not import"
					, folderName, importName);
			ShowMessage(buf);
			return;
		}
  		
  		file->Open(perm_RdWrDenyWr);
    	
    	UInt32 resSize = file->GetSize();
    	THdl resHandle = UMemory::NewHandle(resSize);
    	UInt32 z = file->Read(0, ((UInt8*)resHandle->Lock())-4, resSize);
    	resHandle->Unlock();
    	file->Close();
    	delete file;
    	
    	if (z != resSize)
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Resource '%#s' %d ('%#s') only partially read, cannot import '%#s'"
					, FourCharToStr255(dty), did, name, importName);
			ShowMessage(buf);
    	}
    	else
    	{
			AddResource(dty, did, resHandle, name);
		}
    }
    
    
}; // class MacResourceFile







class HL_DatFile
{
	TFSRefObj* file;
	TRez datFile;
public:
	HL_DatFile(UInt8* folderName, UInt8* dstDatName, bool create)
	:   datFile(0)
	,   file(0)
	{
    	file = UFileSys::New(kProgramFolder, folderName, dstDatName);
//		if (file->Exists())
//			file->DeleteFile();
		if (create)
			file->CreateFileAndOpen('BINA', 'hDmp', kAlwaysCreateFile);
	   	else
	   		file->Open(perm_RdWr); //perm_RdWrDenyWr
		
		datFile = URez::NewFromFile(file, create); 
	}
	~HL_DatFile()
	{
//    	if (mResHandle != 0)
//    		delete mResHandle;
		mResHandle = 0;

		if (datFile != 0)
    		datFile->Save();
    	delete datFile;
    	
    	if (file != 0)
    		file->Close();
    	delete file;
	}
	void AddItem(Uint32 ty, Int32 id, THdl data, Uint8 *name, Uint32 attr)
	{
    	datFile->AddItem(ty,id, data, name, attr);
	}
	
	// after a get these are set properly
	THdl mResHandle;
	UInt8 mResName[256];
	UInt32 mResAttr;
	UInt32 mResSize;
	UInt32 mResType;
	UInt32 mResID;
	
	bool GetItem(Uint32 ty, Int32 id)
	{
    	if (mResHandle != 0)
    		delete mResHandle;
		mResHandle = 0;
		mResName[0] = 0;
		mResType = ty;
		mResID = id;
		if (! datFile->ItemExists(ty, id))
			return false;
    	THdl mh = datFile->LoadItem(ty,id);
    	mResName[0] = datFile->GetItemName(ty,id, mResName+1, sizeof(mResName)-1);
    	mResAttr = datFile->GetItemAttributes(ty,id);
    	mResSize = datFile->GetItemSize(ty,id);
    	UInt32 z2 = mh->GetSize();
    	if (mResSize != z2)
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Size mismatch %d != %d in resource '%#s' %d"
					, mResSize, z2, FourCharToStr255(ty),id);
			ShowMessage(buf);
    	}
    	mResHandle = UMemory::NewHandle(mh->Lock(), mResSize);
		mh->Unlock();
    	delete mh;
    	
    	return true;
	}
    void ExportToFile(UInt8* folderName, UInt8* exportName)
    {
    	if (mResHandle == 0)
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Resource '%#s' %d does not exist, did not export"
					, FourCharToStr255(mResType), mResID);
			ShowMessage(buf);
			return;
    	}
    	
		TFSRefObj* file = UFileSys::New(kProgramFolder, folderName, exportName);
		
		// RezKitchen or IconEdit needs this file type
		file->CreateFileAndOpen('BINA', 'hDmp', kAlwaysCreateFile);
    	
    	Uint32 z = file->Write(0, mResHandle->Lock(), mResSize);
    	mResHandle->Unlock();
    	
    	file->Close();
    	delete file;
    	
    	if (z != mResSize)
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Resource '%#s' %d ('%#s') only partially exported"
					, FourCharToStr255(mResType), mResID, mResName);
			ShowMessage(buf);
    	}
    }
    void ImportFromFile(Uint32 ty, Int32 id, UInt8* inDatName, UInt8* folderName, UInt8* importName)
    {
//    	if (mResHandle != 0)
//    		delete mResHandle;
		mResHandle = 0;
		mResName[0] = 0;
		mResType = ty;
		mResID = id;
		
		TFSRefObj* file = UFileSys::New(kProgramFolder, folderName, importName);
		if (!file->Exists())
		{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "File %#s/%#s does not exist, did not import"
					, folderName, importName);
			ShowMessage(buf);
			return;
		}
		
		if (datFile->ItemExists(ty, id))
			datFile->RemoveItem(ty,id);
    	
  		file->Open(perm_RdWrDenyWr);
    	
    	mResName[0] = UMemory::Copy(mResName+1, inDatName+1, inDatName[0]);
    	mResSize = file->GetSize();
    	mResHandle = UMemory::NewHandle(mResSize);
    	UInt32 z = file->Read(0, mResHandle->Lock(), mResSize);
    	mResHandle->Unlock();
    	
    	file->Close();
    	delete file;
    	
    	if (z != mResSize)
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Resource '%#s' %d ('%#s') only partially read, cannot import"
					, FourCharToStr255(mResType), mResID, mResName);
			ShowMessage(buf);
    	}
    	
    	datFile->AddItem(ty,id, mResHandle, mResName, 0);
    }
}; // class HL_DatFile


class CMyApplication : public CApplication
{
public:

	CMyApplication()
	{
	}

    void Transfer( HL_DatFile& oldDatFile
    			 , MacResourceFile& resf, Uint32 rty, Int32 rid
    			 , HL_DatFile& datFile, Uint32 dty, Int32 did )
    {
    	if (oldDatFile.GetItem(dty,did))
    	{
    		datFile.AddItem(dty,did, oldDatFile.mResHandle, oldDatFile.mResName, oldDatFile.mResAttr );
		}
    	else
    	{
			THdl h = resf.GetResource(rty,rid); // => ICON in .dat
			datFile.AddItem(dty,did, h, resf.mResName, 0 );
			
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "New resource '%#s' %d ('%#s') not found in old .dat, copied from mac .rsrc"
					, FourCharToStr255(dty),did, resf.mResName );
			ShowMessage(buf);
		}
    }
	void Rez2Dat(UInt8* folderName, UInt8* srcDatName, UInt8* srcRezName, UInt8* dstDatName)
	{
//		TFSRefObj* folder = UFileSys::New(kProgramFolder, nil, folderName);
//		scopekill(TFSRefObj, folder);
//		if (!folder->Exists())
//		{
//			Uint8 buf[256];
//			buf[0] = UText::Format(buf+1, sizeof(buf)-1, "Cannot find folder '%#s'", folderName);
//			ShowMessage(buf);
//			return;
//		}
		try
		{
			HL_DatFile oldDatFile(folderName, srcDatName, false);
			MacResourceFile resf(folderName, srcRezName);
			HL_DatFile datFile(folderName, dstDatName, true);
			for (int k=0; k<sizeof(sToTransfer)/sizeof(*sToTransfer); ++k)
			{
				try
				{
					Transfer( oldDatFile
							, resf, sToTransfer[k].rty, sToTransfer[k].rid
							, datFile, sToTransfer[k].dty, sToTransfer[k].did );
				}
				catch(SError& err)
				{
					UError::Alert(err);
					break;
				}
			}
//			Transfer(resf, 'cicn', 431, datFile,'ICON', 431);
		}
		catch(int)
		{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1, "Cannot open resource file '%#s' in folder '%#s'"
				, srcRezName, folderName);
			ShowMessage(buf);
			return;
		}
	}
    void ExportToFile(HL_DatFile& datFile, Uint32 dty, Int32 did, UInt8* folderName)
    {
    	if (!datFile.GetItem(dty,did))
    	{
			Uint8 buf[256];
			buf[0] = UText::Format(buf+1, sizeof(buf)-1
					, "Resource '%#s' %d not found in .dat, cannot export"
					, FourCharToStr255(dty),did);
			ShowMessage(buf);
			return;
    	}
    	UInt8 exportName[256];
		exportName[0] = UText::Format(exportName+1, sizeof(exportName)-1
				, "%d_%d.%#s", did, datFile.mResAttr, FourCharToStr255(dty) );
		datFile.ExportToFile(folderName, exportName);
    }
    
    
    
	void ExportFromDat(UInt8* folderName, UInt8* srcDatName)
	{
		HL_DatFile datFile(folderName, srcDatName, false);
		//ExportToFile(datFile,'ICON', 133, folderName);
		ExportToFile(datFile,'MooV', 128, folderName);
		ExportToFile(datFile,'GIFf', 128, folderName);
	}
	void ImportFromFiles(UInt8* folderName, UInt8* dstDatName)
	{
		HL_DatFile datFile(folderName, dstDatName, false);
//		datFile.ImportFromFile('ICON', 428, folderName, "\phelpToolbar");
//		datFile.ImportFromFile('ICON', 429, "\pmsgBoard", folderName, "\pMsgBoard#429.HLpixmap");
//		datFile.ImportFromFile('ICON', 430, folderName, "\popenDloadFolder");
//		datFile.ImportFromFile('MooV', 128, "\p19BannerISP.mov", folderName, "\p19BannerISP.mov");
//		datFile.ImportFromFile('GIFf', 128, "\p19BannerISP.gif", folderName, "\p19BannerISP.gif");
//		datFile.ImportFromFile('PIXM', 130, "\pSplash", folderName, "\p19SplashISP.HLpixmap");
		datFile.ImportFromFile('ICON', 429, "\pmsgBoard", folderName, "\pMsgBoard#429.HLpixmap");
		datFile.ImportFromFile('ICON', 432, "\pSecuriphone", folderName, "\pSecuriphone#432.HLpixmap");
		datFile.ImportFromFile('ICON', 433, "\pHL-ISP2", folderName, "\pHL-ISP2#433.HLpixmap");
		datFile.ImportFromFile('ICON', 434, "\pXsprings", folderName, "\pXsprings#434.HLpixmap");
	}
	void ImportFromFilesToMacRez(UInt8* folderName, UInt8* rsrcName)
	{
		MacResourceFile rsrcFile(folderName, rsrcName);
//		datFile.ImportFromFile('ICON', 428, folderName, "\phelpToolbar");
//		datFile.ImportFromFile('ICON', 429, folderName, "\pmsgBoard");
//		datFile.ImportFromFile('ICON', 430, folderName, "\popenDloadFolder");
//		datFile.ImportFromFile('ICON', 431, folderName, "\pHL-ISP");
//		datFile.ImportFromFile('ICON', 432, folderName, "\pSecuriphone");
//		rsrcFile.AddFromFile('MooV', 128, "\p19Banner.mov", folderName, "\p19Banner.mov");
//		rsrcFile.AddFromFile('GIFf', 128, "\p19Banner.gif", folderName, "\p19Banner.gif");
	}    

	void StartUp()
	{
//		Rez2Dat("\pClient", "\photline_old.dat", "\pHotlineClient(M-Carbon)", "\photline.dat");

//		ExportFromDat("\pClient", "\photline.dat");		
		ImportFromFiles("\pImages", "\phlci19.dat");
		ImportFromFiles("\pImages", "\phlc19.dat");
//		ImportFromFilesToMacRez("\pClient", "\photline.rsrc");
		
		ShowMessage("\pAll Done, closing now");
	} // Startup
}; // class CMyApplication

int main()
{
	UOperatingSystem::Init();
	
	try
	{
		CMyApplication app;
		UApplication::Process();
		app.StartUp();
		app.Quit();
		app.Run();
		return 0;
	}
	catch(SError& err)
	{
		UError::Alert(err);
		return 1;
	}
}
