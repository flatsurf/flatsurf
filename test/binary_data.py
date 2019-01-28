#*********************************************************************
#  This file is part of Polygon.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Polygon is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  Polygon is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

expected_output={
    (1, 1, 2, 2): r"""
max loc = 2
alpha =  2 beta = 2 gamma = 1 delta =1
a =  2.0944 b = 2.0944 c= 1.0472 d=1.0472
a1_min = 4.44089e-16 a1_max = 1.0472
a1 = 0.713864
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 2.64682 p-q = 2.00113
p = (1,0) q = (2.00057,1.73303) r = (-1.00057,1.73303)
Imaginary of t_cx = -4.44089e-16
p_algt = (1 0)+(0 0)t1
r_algt = (0 0)+(-1 1)t1
Checking p_alt (0,0)
Checking q_alt (4.44089e-16,-8.88178e-16)
Checking r_alt (0,0)
Checking p_algt (0,0)
Checking q_algt (4.44089e-16,-8.88178e-16)
Checking r_algt (0,0)
Number field: min poly (1 -1 1)
#Field Degree=2 minimal polynomial=(1 -1 1)
#Field generator embedding: (0.500000000000000110.8660254037844386
#Params:1
#t1=2.0011307609945161
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (1.000565380497258516356851,1.733030075315736828756030)
E2 ( V2 V3 F0 NULL ) (-3.001130760994516144535282,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (1.000565380497257850223036,-1.733030075315736828756030)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0)+(0 0)t1
#E1: (0 0)+(0 1)t1
#E2: (-1 0)+(-1 0)t1
#E3: (0 0)+(1 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.333333, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.333333, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.333333
In RetriangulateSurface
Removing V2
F13 V1(E13) V0(E15) a=0, can_bisect=0
F13 V1(E13) V15(E9) a=0.333333, can_bisect=1
F13 V1(E13) V0(E13) a=0.804483, can_bisect=1
F13 V1(E13) V1(E22) a=1.5708, can_bisect=1
F13 V1(E13) V0(E3) a=0.408689, can_bisect=1
F13 V1(E13) V15(E15) a=1.0472, can_bisect=1
F13 V1(E13) V0(E22) a=0.408689, can_bisect=1
F13 V1(E13) V1(E0) a=1.5708, can_bisect=1
F13 V1(E13) V0(E9) a=0.804483, can_bisect=1
F13 V1(E13) V15(E3) a=0.333333, can_bisect=1
F13 V1(E13) V0(E0) a=0, can_bisect=0
F13 V0(E15) V15(E9) a=0, can_bisect=0
F13 V0(E15) V0(E13) a=0.523599, can_bisect=1
F13 V0(E15) V1(E22) a=0.804483, can_bisect=1
F13 V0(E15) V0(E3) a=0.190265, can_bisect=1
F13 V0(E15) V15(E15) a=0.891084, can_bisect=1
F13 V0(E15) V0(E22) a=0.523599, can_bisect=1
F13 V0(E15) V1(E0) a=0.408689, can_bisect=1
F13 V0(E15) V0(E9) a=0.190265, can_bisect=1
F13 V0(E15) V15(E3) a=0.242489, can_bisect=0
F13 V0(E15) V0(E0) a=2.61799, can_bisect=0
F13 V15(E9) V0(E13) a=0, can_bisect=0
F13 V15(E9) V1(E22) a=0.333333, can_bisect=1
F13 V15(E9) V0(E3) a=0.242489, can_bisect=0
F13 V15(E9) V15(E15) a=0.523599, can_bisect=1
F13 V15(E9) V0(E22) a=0.891084, can_bisect=1
F13 V15(E9) V1(E0) a=1.0472, can_bisect=1
F13 V15(E9) V0(E9) a=0.891084, can_bisect=1
F13 V15(E9) V15(E3) a=0.523599, can_bisect=1
F13 V15(E9) V0(E0) a=0.242489, can_bisect=0
F13 V0(E13) V1(E22) a=0, can_bisect=0
F13 V0(E13) V0(E3) a=2.61799, can_bisect=0
F13 V0(E13) V15(E15) a=0.242489, can_bisect=0
F13 V0(E13) V0(E22) a=0.190265, can_bisect=1
F13 V0(E13) V1(E0) a=0.408689, can_bisect=1
F13 V0(E13) V0(E9) a=0.523599, can_bisect=1
F13 V0(E13) V15(E3) a=0.891084, can_bisect=1
F13 V0(E13) V0(E0) a=0.190265, can_bisect=1
F13 V1(E22) V0(E3) a=0, can_bisect=0
F13 V1(E22) V15(E15) a=0.333333, can_bisect=1
F13 V1(E22) V0(E22) a=0.804483, can_bisect=1
F13 V1(E22) V1(E0) a=1.5708, can_bisect=1
F13 V1(E22) V0(E9) a=0.408689, can_bisect=1
F13 V1(E22) V15(E3) a=1.0472, can_bisect=1
F13 V1(E22) V0(E0) a=0.408689, can_bisect=1
F13 V0(E3) V15(E15) a=0, can_bisect=0
F13 V0(E3) V0(E22) a=0.523599, can_bisect=1
F13 V0(E3) V1(E0) a=0.804483, can_bisect=1
F13 V0(E3) V0(E9) a=0.190265, can_bisect=1
F13 V0(E3) V15(E3) a=0.891084, can_bisect=1
F13 V0(E3) V0(E0) a=0.523599, can_bisect=1
F13 V15(E15) V0(E22) a=0, can_bisect=0
F13 V15(E15) V1(E0) a=0.333333, can_bisect=1
F13 V15(E15) V0(E9) a=0.242489, can_bisect=0
F13 V15(E15) V15(E3) a=0.523599, can_bisect=1
F13 V15(E15) V0(E0) a=0.891084, can_bisect=1
F13 V0(E22) V1(E0) a=0, can_bisect=0
F13 V0(E22) V0(E9) a=2.61799, can_bisect=0
F13 V0(E22) V15(E3) a=0.242489, can_bisect=0
F13 V0(E22) V0(E0) a=0.190265, can_bisect=1
F13 V1(E0) V0(E9) a=0, can_bisect=0
F13 V1(E0) V15(E3) a=0.333333, can_bisect=1
F13 V1(E0) V0(E0) a=0.804483, can_bisect=1
F13 V0(E9) V15(E3) a=0, can_bisect=0
F13 V0(E9) V0(E0) a=0.523599, can_bisect=1
F13 V15(E3) V0(E0) a=0, can_bisect=0
F13 best_angle = 1.5708
F15 V1(E22) V0(E3) a=0, can_bisect=0
F15 V1(E22) V15(E15) a=0.333333, can_bisect=1
F15 V1(E22) V0(E22) a=0.804483, can_bisect=1
F15 V1(E22) V1(E0) a=1.0472, can_bisect=1
F15 V1(E22) V0(E9) a=0.408689, can_bisect=1
F15 V1(E22) V15(E3) a=0.523599, can_bisect=1
F15 V1(E22) V0(E0) a=0.114909, can_bisect=1
F15 V1(E22) V1(E35) a=0, can_bisect=0
F15 V0(E3) V15(E15) a=0, can_bisect=0
F15 V0(E3) V0(E22) a=0.523599, can_bisect=1
F15 V0(E3) V1(E0) a=0.804483, can_bisect=1
F15 V0(E3) V0(E9) a=0.190265, can_bisect=1
F15 V0(E3) V15(E3) a=0.891084, can_bisect=1
F15 V0(E3) V0(E0) a=0.523599, can_bisect=1
F15 V0(E3) V1(E35) a=0.114909, can_bisect=1
F15 V15(E15) V0(E22) a=0, can_bisect=0
F15 V15(E15) V1(E0) a=0.333333, can_bisect=1
F15 V15(E15) V0(E9) a=0.242489, can_bisect=0
F15 V15(E15) V15(E3) a=0.523599, can_bisect=1
F15 V15(E15) V0(E0) a=0.891084, can_bisect=1
F15 V15(E15) V1(E35) a=0.523599, can_bisect=1
F15 V0(E22) V1(E0) a=0, can_bisect=0
F15 V0(E22) V0(E9) a=2.61799, can_bisect=0
F15 V0(E22) V15(E3) a=0.242489, can_bisect=0
F15 V0(E22) V0(E0) a=0.190265, can_bisect=1
F15 V0(E22) V1(E35) a=0.408689, can_bisect=1
F15 V1(E0) V0(E9) a=0, can_bisect=0
F15 V1(E0) V15(E3) a=0.333333, can_bisect=1
F15 V1(E0) V0(E0) a=0.804483, can_bisect=1
F15 V1(E0) V1(E35) a=1.0472, can_bisect=1
F15 V0(E9) V15(E3) a=0, can_bisect=0
F15 V0(E9) V0(E0) a=0.523599, can_bisect=1
F15 V0(E9) V1(E35) a=0.804483, can_bisect=1
F15 V15(E3) V0(E0) a=0, can_bisect=0
F15 V15(E3) V1(E35) a=0.333333, can_bisect=1
F15 V0(E0) V1(E35) a=0, can_bisect=0
F15 best_angle = 1.0472
F17 V1(E0) V0(E9) a=0, can_bisect=0
F17 V1(E0) V15(E3) a=0.333333, can_bisect=1
F17 V1(E0) V0(E0) a=0.804483, can_bisect=1
F17 V1(E0) V1(E35) a=1.0472, can_bisect=1
F17 V1(E0) V1(E36) a=0, can_bisect=0
F17 V0(E9) V15(E3) a=0, can_bisect=0
F17 V0(E9) V0(E0) a=0.523599, can_bisect=1
F17 V0(E9) V1(E35) a=0.804483, can_bisect=1
F17 V0(E9) V1(E36) a=0.114909, can_bisect=1
F17 V15(E3) V0(E0) a=0, can_bisect=0
F17 V15(E3) V1(E35) a=0.333333, can_bisect=1
F17 V15(E3) V1(E36) a=0.523599, can_bisect=1
F17 V0(E0) V1(E35) a=0, can_bisect=0
F17 V0(E0) V1(E36) a=0.114909, can_bisect=1
F17 V1(E35) V1(E36) a=0, can_bisect=0
F17 best_angle = 1.0472
F18 V1(E0) V0(E9) a=0, can_bisect=0
F18 V1(E0) V15(E3) a=0.333333, can_bisect=1
F18 V1(E0) V0(E0) a=0.280884, can_bisect=1
F18 V1(E0) V1(E37) a=0, can_bisect=0
F18 V0(E9) V15(E3) a=0, can_bisect=0
F18 V0(E9) V0(E0) a=0.523599, can_bisect=1
F18 V0(E9) V1(E37) a=0.280884, can_bisect=1
F18 V15(E3) V0(E0) a=0, can_bisect=0
F18 V15(E3) V1(E37) a=0.333333, can_bisect=1
F18 V0(E0) V1(E37) a=0, can_bisect=0
F18 best_angle = 0.523599
F21 V0(E0) V1(E37) a=0, can_bisect=0
F21 V0(E0) V1(E0) a=0.280884, can_bisect=1
F21 V0(E0) V0(E38) a=0, can_bisect=0
F21 V1(E37) V1(E0) a=0, can_bisect=0
F21 V1(E37) V0(E38) a=0.280884, can_bisect=1
F21 V1(E0) V0(E38) a=0, can_bisect=0
F21 best_angle = 0.280884
F16 V1(E22) V0(E3) a=0, can_bisect=0
F16 V1(E22) V15(E15) a=0.333333, can_bisect=1
F16 V1(E22) V0(E22) a=0.280884, can_bisect=1
F16 V1(E22) V1(E36) a=0, can_bisect=0
F16 V0(E3) V15(E15) a=0, can_bisect=0
F16 V0(E3) V0(E22) a=0.523599, can_bisect=1
F16 V0(E3) V1(E36) a=0.280884, can_bisect=1
F16 V15(E15) V0(E22) a=0, can_bisect=0
F16 V15(E15) V1(E36) a=0.333333, can_bisect=1
F16 V0(E22) V1(E36) a=0, can_bisect=0
F16 best_angle = 0.523599
F25 V0(E22) V1(E36) a=0, can_bisect=0
F25 V0(E22) V1(E22) a=0.280884, can_bisect=1
F25 V0(E22) V0(E40) a=0, can_bisect=0
F25 V1(E36) V1(E22) a=0, can_bisect=0
F25 V1(E36) V0(E40) a=0.280884, can_bisect=1
F25 V1(E22) V0(E40) a=0, can_bisect=0
F25 best_angle = 0.280884
F14 V1(E13) V0(E15) a=0, can_bisect=0
F14 V1(E13) V15(E9) a=0.333333, can_bisect=1
F14 V1(E13) V0(E13) a=0.280884, can_bisect=1
F14 V1(E13) V1(E35) a=0, can_bisect=0
F14 V0(E15) V15(E9) a=0, can_bisect=0
F14 V0(E15) V0(E13) a=0.523599, can_bisect=1
F14 V0(E15) V1(E35) a=0.280884, can_bisect=1
F14 V15(E9) V0(E13) a=0, can_bisect=0
F14 V15(E9) V1(E35) a=0.333333, can_bisect=1
F14 V0(E13) V1(E35) a=0, can_bisect=0
F14 best_angle = 0.523599
F29 V0(E13) V1(E35) a=0, can_bisect=0
F29 V0(E13) V1(E13) a=0.280884, can_bisect=1
F29 V0(E13) V0(E42) a=0, can_bisect=0
F29 V1(E35) V1(E13) a=0, can_bisect=0
F29 V1(E35) V0(E42) a=0.280884, can_bisect=1
F29 V1(E13) V0(E42) a=0, can_bisect=0
F29 best_angle = 0.280884
Removing V15
Longest Edge: 3.60743
Looking for shortest saddle...1
Clearing denominators: multiplying by 1
#Field Degree=2 minimal polynomial=(1 -1 1)
#Field generator embedding: (0.500000000000000110.8660254037844386
#Params:1
#t1=2.0011307609945161
# Vertices: V0 V1
# UEdges: 
E0 ( V0 V1 F22 F23 ) (1.000000000000000000000000,0.000000000000000000000000)
E13 ( V1 V0 F31 F30 ) (0.499999999999999888977698,-0.866025403784438596588302)
E22 ( V1 V0 F27 F26 ) (0.500000000000000111022302,0.866025403784438596588302)
E35 ( V1 V1 F19 F30 ) (3.001696141491774660892133,1.733030075315736828756030)
E36 ( V1 V1 F19 F26 ) (-3.001696141491773772713714,1.733030075315736828756030)
E37 ( V1 V1 F19 F22 ) (-0.000000000000000666133815,-3.466060150631473657512061)
E38 ( V0 V0 F23 F32 ) (-0.000000000000000444089210,-3.466060150631473657512061)
E39 ( V0 V1 F23 F22 ) (1.000000000000000666133815,3.466060150631473657512061)
E40 ( V0 V0 F27 F32 ) (-3.001696141491773772713714,1.733030075315736828756030)
E41 ( V0 V1 F27 F26 ) (2.501696141491773772713714,-2.599055479100175425344332)
E42 ( V0 V0 F31 F32 ) (3.001696141491774660892133,1.733030075315736828756030)
E43 ( V0 V1 F31 F30 ) (-3.501696141491774660892133,-0.867004671531298232167728)
# Faces: 
F19 +E35 +E36 +E37
F22 +E0 -E37 -E39
F23 -E0 +E38 +E39
F26 -E22 -E36 -E41
F27 +E22 +E40 +E41
F30 -E13 -E35 -E43
F31 +E13 +E42 +E43
F32 -E42 -E40 -E38
#UEdges Algebraic 
#E0: (1 0)+(0 0)t1
#E13: (1 -1)+(0 0)t1
#E22: (0 1)+(0 0)t1
#E35: (0 0)+(1 1)t1
#E36: (0 0)+(-2 1)t1
#E37: (0 0)+(1 -2)t1
#E38: (0 0)+(1 -2)t1
#E39: (1 0)+(-1 2)t1
#E40: (0 0)+(-2 1)t1
#E41: (0 -1)+(2 -1)t1
#E42: (0 0)+(1 1)t1
#E43: (-1 1)+(-1 -1)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V0(12) (4 PI) V1(12) (4 PI) 
# edges: 12
## E0(2) E13(2) E22(2) E35(2) E36(2) E37(2) E38(2) E39(2) E40(2) E41(2) E42(2) E43(2) 
# faces: 8
# genus: 2
# area: 20.8022
sweeping start V0 depth = 45.6095
File =  depth = 45.6095 follow_depth = 228.047 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 0 unusual 0 close 64854 reject 0
V0: AaXX V1: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0416314 Max Area: 0.874635
    total: 2.60752 1.58518 sym = 2
    raw = 996 short saddle = 4.58443 short cyl = 4.58443 tag #2
V0: AaBb V1: CdDc   cylinders |1|1.49972|2.49972|
Moduli: |4.99943|8.33962|1| Min Area: 1 Max Area: 1
    total: 0.017017 0.0103451 sym = 4
    raw = 13 short saddle = 3.46606 short cyl = 3.46606 tag #0
V0: AdBc V1: aCbD   cylinders |1|1.66679|
Moduli: |1.66868|1| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 1 short cyl = 9.00339 tag #5

total cylinders: 2.72795 1.65839 raw = 521 rejected = 0
""",
    (1, 1, 3, 5): r"""
max loc = 3
alpha =  5 beta = 1 gamma = 1 delta =3
a =  3.14159 b = 0.628319 c= 0.628319 d=1.88496
a1_min = 1.88496 a1_max = 2.51327
a1 = 2.31327
angle a check: 0
angle b check: -2.22045e-16
angle c check: 3.14159
angle d check: 0
p = 1 q = 2.95861 p-q = 3.70865
p = (1,0) q = (-2.00036,2.17989) r = (-1.29207,1.21008e-16)
Imaginary of t_cx = -1.21008e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (0,4.44089e-16)
Checking r_alt (0,-1.21008e-16)
Checking p_algt (0,0)
Checking q_algt (0,4.44089e-16)
Checking r_algt (0,-1.21008e-16)
Number field: min poly (1 -1 1 -1 1)
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.2920734071754343
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-3.000363042347531816744777,2.179891349721031712505237)
E2 ( V2 V3 F0 NULL ) (0.708289635172097531423674,-2.179891349721031712505237)
E3 ( V3 V0 F0 NULL ) (1.292073407175434285321103,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-1 0 0 1)+(-1 0 0 1)t1
#E2: (0 0 0 -1)+(0 0 0 -1)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.2, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.2
In RetriangulateSurface
Removing V0
F21 V3(E10) V40(E6) a=0, can_bisect=0
F21 V3(E10) V21(E1) a=0.628319, can_bisect=1
F21 V3(E10) V40(E2) a=0, can_bisect=0
F21 V40(E6) V21(E1) a=0, can_bisect=0
F21 V40(E6) V40(E2) a=0.942478, can_bisect=0
F21 V21(E1) V40(E2) a=0, can_bisect=0
F21 best_angle = 0.628319
Removing V10
F24 V40(E38) V3(E16) a=0, can_bisect=0
F24 V40(E38) V40(E1) a=0.942478, can_bisect=0
F24 V40(E38) V21(E21) a=0, can_bisect=0
F24 V3(E16) V40(E1) a=0, can_bisect=0
F24 V3(E16) V21(E21) a=0.628319, can_bisect=1
F24 V40(E1) V21(E21) a=0, can_bisect=0
F24 best_angle = 0.628319
Removing V17
F27 V40(E21) V21(E45) a=0, can_bisect=0
F27 V40(E21) V40(E41) a=0.942478, can_bisect=0
F27 V40(E21) V3(E2) a=0, can_bisect=0
F27 V21(E45) V40(E41) a=0, can_bisect=0
F27 V21(E45) V3(E2) a=0.628319, can_bisect=1
F27 V40(E41) V3(E2) a=0, can_bisect=0
F27 best_angle = 0.628319
Removing V21
F30 V40(E38) V3(E16) a=0, can_bisect=0
F30 V40(E38) V40(E2) a=1.5708, can_bisect=0
F30 V40(E38) V3(E10) a=0.169861, can_bisect=1
F30 V40(E38) V40(E25) a=0.314159, can_bisect=1
F30 V40(E38) V22(E47) a=0.628319, can_bisect=1
F30 V40(E38) V40(E31) a=0.314159, can_bisect=0
F30 V40(E38) V30(E53) a=0.395879, can_bisect=1
F30 V40(E38) V40(E41) a=1.14248, can_bisect=0
F30 V40(E38) V3(E2) a=0, can_bisect=0
F30 V3(E16) V40(E2) a=0, can_bisect=0
F30 V3(E16) V3(E10) a=0.942478, can_bisect=1
F30 V3(E16) V40(E25) a=0.169861, can_bisect=1
F30 V3(E16) V22(E47) a=1.69764, can_bisect=1
F30 V3(E16) V40(E31) a=0.2, can_bisect=1
F30 V3(E16) V30(E53) a=1.69764, can_bisect=1
F30 V3(E16) V40(E41) a=0.169861, can_bisect=1
F30 V3(E16) V3(E2) a=0.942478, can_bisect=1
F30 V40(E2) V3(E10) a=0, can_bisect=0
F30 V40(E2) V40(E25) a=1.14248, can_bisect=0
F30 V40(E2) V22(E47) a=0.395879, can_bisect=1
F30 V40(E2) V40(E31) a=0.314159, can_bisect=0
F30 V40(E2) V30(E53) a=0.628319, can_bisect=1
F30 V40(E2) V40(E41) a=0.314159, can_bisect=1
F30 V40(E2) V3(E2) a=0.169861, can_bisect=1
F30 V3(E10) V40(E25) a=0, can_bisect=0
F30 V3(E10) V22(E47) a=0.875506, can_bisect=1
F30 V3(E10) V40(E31) a=0.658458, can_bisect=0
F30 V3(E10) V30(E53) a=1.69764, can_bisect=1
F30 V3(E10) V40(E41) a=0.2, can_bisect=1
F30 V3(E10) V3(E2) a=1.5708, can_bisect=1
F30 V40(E25) V22(E47) a=0, can_bisect=0
F30 V40(E25) V40(E31) a=1.14248, can_bisect=0
F30 V40(E25) V30(E53) a=0.860758, can_bisect=0
F30 V40(E25) V40(E41) a=0.942478, can_bisect=0
F30 V40(E25) V3(E2) a=0.2, can_bisect=1
F30 V22(E47) V40(E31) a=0, can_bisect=0
F30 V22(E47) V30(E53) a=1.3708, can_bisect=1
F30 V22(E47) V40(E41) a=0.860758, can_bisect=0
F30 V22(E47) V3(E2) a=1.69764, can_bisect=1
F30 V40(E31) V30(E53) a=0, can_bisect=0
F30 V40(E31) V40(E41) a=1.14248, can_bisect=0
F30 V40(E31) V3(E2) a=0.658458, can_bisect=0
F30 V30(E53) V40(E41) a=0, can_bisect=0
F30 V30(E53) V3(E2) a=0.875506, can_bisect=1
F30 V40(E41) V3(E2) a=0, can_bisect=0
F30 best_angle = 1.69764
F32 V22(E47) V40(E31) a=0, can_bisect=0
F32 V22(E47) V30(E53) a=1.3708, can_bisect=1
F32 V22(E47) V40(E41) a=0.860758, can_bisect=0
F32 V22(E47) V3(E2) a=0.881998, can_bisect=1
F32 V22(E47) V40(E38) a=0.440999, can_bisect=1
F32 V22(E47) V3(E62) a=0, can_bisect=0
F32 V40(E31) V30(E53) a=0, can_bisect=0
F32 V40(E31) V40(E41) a=1.14248, can_bisect=0
F32 V40(E31) V3(E2) a=0.658458, can_bisect=0
F32 V40(E31) V40(E38) a=0.314159, can_bisect=0
F32 V40(E31) V3(E62) a=0.187319, can_bisect=1
F32 V30(E53) V40(E41) a=0, can_bisect=0
F32 V30(E53) V3(E2) a=0.875506, can_bisect=1
F32 V30(E53) V40(E38) a=0.395879, can_bisect=1
F32 V30(E53) V3(E62) a=0.374639, can_bisect=1
F32 V40(E41) V3(E2) a=0, can_bisect=0
F32 V40(E41) V40(E38) a=1.14248, can_bisect=0
F32 V40(E41) V3(E62) a=0.169861, can_bisect=1
F32 V3(E2) V40(E38) a=0, can_bisect=0
F32 V3(E2) V3(E62) a=0.942478, can_bisect=1
F32 V40(E38) V3(E62) a=0, can_bisect=0
F32 best_angle = 1.3708
F34 V30(E53) V40(E41) a=0, can_bisect=0
F34 V30(E53) V3(E2) a=0.875506, can_bisect=1
F34 V30(E53) V40(E38) a=0.395879, can_bisect=1
F34 V30(E53) V3(E62) a=0.374639, can_bisect=1
F34 V30(E53) V22(E63) a=0, can_bisect=0
F34 V40(E41) V3(E2) a=0, can_bisect=0
F34 V40(E41) V40(E38) a=1.14248, can_bisect=0
F34 V40(E41) V3(E62) a=0.169861, can_bisect=1
F34 V40(E41) V22(E63) a=0.860758, can_bisect=0
F34 V3(E2) V40(E38) a=0, can_bisect=0
F34 V3(E2) V3(E62) a=0.942478, can_bisect=1
F34 V3(E2) V22(E63) a=0.501479, can_bisect=1
F34 V40(E38) V3(E62) a=0, can_bisect=0
F34 V40(E38) V22(E63) a=0.440999, can_bisect=1
F34 V3(E62) V22(E63) a=0, can_bisect=0
F34 best_angle = 0.942478
F36 V3(E62) V22(E63) a=0, can_bisect=0
F36 V3(E62) V30(E53) a=0.374639, can_bisect=1
F36 V3(E62) V40(E41) a=0.144299, can_bisect=1
F36 V3(E62) V3(E64) a=0, can_bisect=0
F36 V22(E63) V30(E53) a=0, can_bisect=0
F36 V22(E63) V40(E41) a=0.860758, can_bisect=0
F36 V22(E63) V3(E64) a=0.501479, can_bisect=1
F36 V30(E53) V40(E41) a=0, can_bisect=0
F36 V30(E53) V3(E64) a=0.875506, can_bisect=1
F36 V40(E41) V3(E64) a=0, can_bisect=0
F36 best_angle = 0.875506
F38 V3(E64) V3(E62) a=0, can_bisect=0
F38 V3(E64) V22(E63) a=0.259868, can_bisect=1
F38 V3(E64) V30(E65) a=0, can_bisect=0
F38 V3(E62) V22(E63) a=0, can_bisect=0
F38 V3(E62) V30(E65) a=0.374639, can_bisect=1
F38 V22(E63) V30(E65) a=0, can_bisect=0
F38 best_angle = 0.374639
F31 V3(E16) V40(E2) a=0, can_bisect=0
F31 V3(E16) V3(E10) a=0.755158, can_bisect=1
F31 V3(E16) V40(E25) a=0.169861, can_bisect=1
F31 V3(E16) V22(E62) a=0, can_bisect=0
F31 V40(E2) V3(E10) a=0, can_bisect=0
F31 V40(E2) V40(E25) a=1.14248, can_bisect=0
F31 V40(E2) V22(E62) a=0.395879, can_bisect=1
F31 V3(E10) V40(E25) a=0, can_bisect=0
F31 V3(E10) V22(E62) a=0.875506, can_bisect=1
F31 V40(E25) V22(E62) a=0, can_bisect=0
F31 best_angle = 0.875506
F42 V22(E62) V3(E16) a=0, can_bisect=0
F42 V22(E62) V40(E2) a=0.395879, can_bisect=1
F42 V22(E62) V3(E67) a=0, can_bisect=0
F42 V3(E16) V40(E2) a=0, can_bisect=0
F42 V3(E16) V3(E67) a=0.755158, can_bisect=1
F42 V40(E2) V3(E67) a=0, can_bisect=0
F42 best_angle = 0.755158
Removing V22
F45 V40(E16) V3(E41) a=0, can_bisect=0
F45 V40(E16) V40(E31) a=1.14248, can_bisect=0
F45 V40(E16) V30(E66) a=0.395879, can_bisect=1
F45 V40(E16) V3(E68) a=0.144299, can_bisect=1
F45 V40(E16) V3(E10) a=0, can_bisect=0
F45 V3(E41) V40(E31) a=0, can_bisect=0
F45 V3(E41) V30(E66) a=0.875506, can_bisect=1
F45 V3(E41) V3(E68) a=0.501479, can_bisect=1
F45 V3(E41) V3(E10) a=0.942478, can_bisect=1
F45 V40(E31) V30(E66) a=0, can_bisect=0
F45 V40(E31) V3(E68) a=0.187319, can_bisect=1
F45 V40(E31) V3(E10) a=0.169861, can_bisect=1
F45 V30(E66) V3(E68) a=0, can_bisect=0
F45 V30(E66) V3(E10) a=0.881998, can_bisect=1
F45 V3(E68) V3(E10) a=0, can_bisect=0
F45 best_angle = 0.942478
F46 V3(E41) V40(E31) a=0, can_bisect=0
F46 V3(E41) V30(E66) a=0.875506, can_bisect=1
F46 V3(E41) V3(E68) a=0.501479, can_bisect=1
F46 V3(E41) V3(E69) a=0, can_bisect=0
F46 V40(E31) V30(E66) a=0, can_bisect=0
F46 V40(E31) V3(E68) a=0.187319, can_bisect=1
F46 V40(E31) V3(E69) a=0.144299, can_bisect=1
F46 V30(E66) V3(E68) a=0, can_bisect=0
F46 V30(E66) V3(E69) a=0.755158, can_bisect=1
F46 V3(E68) V3(E69) a=0, can_bisect=0
F46 best_angle = 0.875506
F49 V30(E66) V3(E68) a=0, can_bisect=0
F49 V30(E66) V3(E69) a=0.755158, can_bisect=1
F49 V30(E66) V3(E70) a=0, can_bisect=0
F49 V3(E68) V3(E69) a=0, can_bisect=0
F49 V3(E68) V3(E70) a=0.501479, can_bisect=1
F49 V3(E69) V3(E70) a=0, can_bisect=0
F49 best_angle = 0.755158
Removing V30
F52 V3(E41) V40(E10) a=0, can_bisect=0
F52 V3(E41) V3(E38) a=0.942478, can_bisect=1
F52 V3(E41) V40(E41) a=0.169861, can_bisect=1
F52 V3(E41) V3(E64) a=1.25664, can_bisect=1
F52 V3(E41) V3(E68) a=0.628319, can_bisect=1
F52 V3(E41) V3(E69) a=0, can_bisect=0
F52 V40(E10) V3(E38) a=0, can_bisect=0
F52 V40(E10) V40(E41) a=1.5708, can_bisect=0
F52 V40(E10) V3(E64) a=0.169861, can_bisect=1
F52 V40(E10) V3(E68) a=0.628319, can_bisect=1
F52 V40(E10) V3(E69) a=0.144299, can_bisect=1
F52 V3(E38) V40(E41) a=0, can_bisect=0
F52 V3(E38) V3(E64) a=0.942478, can_bisect=1
F52 V3(E38) V3(E68) a=0.628319, can_bisect=1
F52 V3(E38) V3(E69) a=0.628319, can_bisect=1
F52 V40(E41) V3(E64) a=0, can_bisect=0
F52 V40(E41) V3(E68) a=0.144299, can_bisect=1
F52 V40(E41) V3(E69) a=0.628319, can_bisect=1
F52 V3(E64) V3(E68) a=0, can_bisect=0
F52 V3(E64) V3(E69) a=0.628319, can_bisect=1
F52 V3(E68) V3(E69) a=0, can_bisect=0
F52 best_angle = 1.25664
F54 V3(E64) V3(E68) a=0, can_bisect=0
F54 V3(E64) V3(E69) a=0.628319, can_bisect=1
F54 V3(E64) V3(E72) a=0, can_bisect=0
F54 V3(E68) V3(E69) a=0, can_bisect=0
F54 V3(E68) V3(E72) a=0.628319, can_bisect=1
F54 V3(E69) V3(E72) a=0, can_bisect=0
F54 best_angle = 0.628319
F53 V3(E41) V40(E10) a=0, can_bisect=0
F53 V3(E41) V3(E38) a=0.628319, can_bisect=1
F53 V3(E41) V40(E41) a=0.169861, can_bisect=1
F53 V3(E41) V3(E72) a=0, can_bisect=0
F53 V40(E10) V3(E38) a=0, can_bisect=0
F53 V40(E10) V40(E41) a=1.5708, can_bisect=0
F53 V40(E10) V3(E72) a=0.169861, can_bisect=1
F53 V3(E38) V40(E41) a=0, can_bisect=0
F53 V3(E38) V3(E72) a=0.628319, can_bisect=1
F53 V40(E41) V3(E72) a=0, can_bisect=0
F53 best_angle = 0.628319
F58 V3(E72) V3(E41) a=0, can_bisect=0
F58 V3(E72) V40(E10) a=0.144299, can_bisect=1
F58 V3(E72) V3(E74) a=0, can_bisect=0
F58 V3(E41) V40(E10) a=0, can_bisect=0
F58 V3(E41) V3(E74) a=0.628319, can_bisect=1
F58 V40(E10) V3(E74) a=0, can_bisect=0
F58 best_angle = 0.628319
Removing V40
F61 V3(E64) V3(E68) a=0, can_bisect=0
F61 V3(E64) V3(E69) a=0.628319, can_bisect=1
F61 V3(E64) V3(E75) a=0.628319, can_bisect=1
F61 V3(E64) V3(E74) a=0, can_bisect=0
F61 V3(E68) V3(E69) a=0, can_bisect=0
F61 V3(E68) V3(E75) a=0.628319, can_bisect=1
F61 V3(E68) V3(E74) a=0.628319, can_bisect=1
F61 V3(E69) V3(E75) a=0, can_bisect=0
F61 V3(E69) V3(E74) a=0.628319, can_bisect=1
F61 V3(E75) V3(E74) a=0, can_bisect=0
F61 best_angle = 0.628319
F63 V3(E69) V3(E75) a=0, can_bisect=0
F63 V3(E69) V3(E74) a=0.628319, can_bisect=1
F63 V3(E69) V3(E76) a=0, can_bisect=0
F63 V3(E75) V3(E74) a=0, can_bisect=0
F63 V3(E75) V3(E76) a=0.628319, can_bisect=1
F63 V3(E74) V3(E76) a=0, can_bisect=0
F63 best_angle = 0.628319
Longest Edge: 4.35978
Looking for shortest saddle...2.69449
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.2920734071754343
# Vertices: V3
# UEdges: 
E64 ( V3 V3 F55 F62 ) (-2.562615973931912538574807,0.832644403811550581906431)
E68 ( V3 V3 F55 F62 ) (-1.583783772003336531852824,-2.179891349721031712505237)
E69 ( V3 V3 F64 F56 ) (-1.583783772003336309808219,2.179891349721031712505237)
E72 ( V3 V3 F56 F60 ) (2.562615973931912982664016,3.527138295630512843104043)
E73 ( V3 V3 F56 F55 ) (-4.146399745935249292472236,-1.347246945909481130598806)
E74 ( V3 V3 F60 F65 ) (0.000000000000000000000000,2.694493891818962261197612)
E75 ( V3 V3 F60 F64 ) (2.562615973931912982664016,0.832644403811550581906431)
E76 ( V3 V3 F65 F62 ) (4.146399745935249292472236,1.347246945909481130598806)
E77 ( V3 V3 F65 F64 ) (-4.146399745935249292472236,1.347246945909481130598806)
# Faces: 
F55 +E64 +E68 -E73
F56 -E69 +E72 +E73
F60 +E74 -E72 +E75
F62 -E64 -E68 -E76
F64 +E69 -E75 -E77
F65 -E74 +E76 +E77
#UEdges Algebraic 
#E64: (0 -1 0 1)+(0 -1 0 1)t1
#E68: (-1 0 0 -1)+(-1 0 0 -1)t1
#E69: (-1 0 1 0)+(-1 0 1 0)t1
#E72: (0 1 1 0)+(0 1 1 0)t1
#E73: (-1 -1 0 0)+(-1 -1 0 0)t1
#E74: (-1 2 -1 1)+(-1 2 -1 1)t1
#E75: (1 -1 2 -1)+(1 -1 2 -1)t1
#E76: (1 1 0 0)+(1 1 0 0)t1
#E77: (-2 1 -1 1)+(-2 1 -1 1)t1
# File =  perturb = 0 rescale = 1
# vertices: 1
## V3(18) (6 PI) 
# edges: 9
## E64(2) E68(2) E69(2) E72(2) E73(2) E74(2) E75(2) E76(2) E77(2) 
# faces: 6
# genus: 2
# area: 24.9824
sweeping start V3 depth = 49.9824
File =  depth = 49.9824 follow_depth = 249.912 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 46 unusual 0 close 192711 reject 0
V3: AbBaCc   cylinders |1|1.61803|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.50186 0.913019 sym = 6
    raw = 1721 short saddle = 2.69449 short cyl = 4.35978 tag #0

total cylinders: 3.00022 1.82392 raw = 573 rejected = 0
""",
    (1, 1, 4, 6): r"""
max loc = 3
alpha =  6 beta = 1 gamma = 1 delta =4
a =  3.14159 b = 0.523599 c= 0.523599 d=2.0944
a1_min = 2.0944 a1_max = 2.61799
a1 = 2.45133
angle a check: 0
angle b check: -1.11022e-16
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 3.01393 p-q = 3.8382
p = (1,0) q = (-2.32398,1.9191) r = (-1.21598,1.06531e-16)
Imaginary of t_cx = -1.06531e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (-4.44089e-16,4.44089e-16)
Checking r_alt (0,-1.06531e-16)
Checking p_algt (0,0)
Checking q_algt (-4.44089e-16,4.44089e-16)
Checking r_algt (0,-1.06531e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.2159835222155908
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-3.323975283323385987443999,1.919098024606419539139779)
E2 ( V2 V3 F0 NULL ) (1.107991761107795181118263,-1.919098024606419539139779)
E3 ( V3 V0 F0 NULL ) (1.215983522215590806325736,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-2 0 1 0)+(-2 0 1 0)t1
#E2: (1 0 -1 0)+(1 0 -1 0)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.166667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.166667
In RetriangulateSurface
Removing V0
F25 V3(E10) V40(E6) a=0, can_bisect=0
F25 V3(E10) V21(E1) a=0.523599, can_bisect=1
F25 V3(E10) V40(E2) a=0, can_bisect=0
F25 V40(E6) V21(E1) a=0, can_bisect=0
F25 V40(E6) V40(E2) a=1.0472, can_bisect=0
F25 V21(E1) V40(E2) a=0, can_bisect=0
F25 best_angle = 0.523599
Removing V10
F28 V40(E38) V15(E16) a=0, can_bisect=0
F28 V40(E38) V40(E1) a=1.0472, can_bisect=0
F28 V40(E38) V21(E21) a=0, can_bisect=0
F28 V15(E16) V40(E1) a=0, can_bisect=0
F28 V15(E16) V21(E21) a=0.523599, can_bisect=1
F28 V40(E1) V21(E21) a=0, can_bisect=0
F28 best_angle = 0.523599
Removing V17
F31 V40(E21) V21(E45) a=0, can_bisect=0
F31 V40(E21) V40(E41) a=1.0472, can_bisect=0
F31 V40(E21) V3(E2) a=0, can_bisect=0
F31 V21(E45) V40(E41) a=0, can_bisect=0
F31 V21(E45) V3(E2) a=0.523599, can_bisect=1
F31 V40(E41) V3(E2) a=0, can_bisect=0
F31 best_angle = 0.523599
Removing V21
F34 V40(E38) V15(E16) a=0, can_bisect=0
F34 V40(E38) V40(E2) a=1.5708, can_bisect=0
F34 V40(E38) V3(E10) a=0, can_bisect=1
F34 V40(E38) V40(E25) a=0, can_bisect=1
F34 V40(E38) V22(E47) a=0.417709, can_bisect=1
F34 V40(E38) V40(E31) a=0.166667, can_bisect=1
F34 V40(E38) V30(E53) a=0.417709, can_bisect=1
F34 V40(E38) V40(E67) a=0, can_bisect=0
F34 V40(E38) V59(E59) a=0.268726, can_bisect=1
F34 V40(E38) V40(E41) a=1.21386, can_bisect=0
F34 V40(E38) V3(E2) a=0, can_bisect=0
F34 V15(E16) V40(E2) a=0, can_bisect=0
F34 V15(E16) V3(E10) a=1.0472, can_bisect=1
F34 V15(E16) V40(E25) a=0, can_bisect=1
F34 V15(E16) V22(E47) a=1.7128, can_bisect=1
F34 V15(E16) V40(E31) a=0.356792, can_bisect=0
F34 V15(E16) V30(E53) a=2.0944, can_bisect=1
F34 V15(E16) V40(E67) a=0.356792, can_bisect=0
F34 V15(E16) V59(E59) a=1.7128, can_bisect=1
F34 V15(E16) V40(E41) a=0, can_bisect=0
F34 V15(E16) V3(E2) a=1.0472, can_bisect=1
F34 V40(E2) V3(E10) a=0, can_bisect=0
F34 V40(E2) V40(E25) a=1.21386, can_bisect=0
F34 V40(E2) V22(E47) a=0.268726, can_bisect=1
F34 V40(E2) V40(E31) a=0, can_bisect=0
F34 V40(E2) V30(E53) a=0.417709, can_bisect=1
F34 V40(E2) V40(E67) a=0.166667, can_bisect=1
F34 V40(E2) V59(E59) a=0.417709, can_bisect=1
F34 V40(E2) V40(E41) a=0, can_bisect=1
F34 V40(E2) V3(E2) a=0, can_bisect=1
F34 V3(E10) V40(E25) a=0, can_bisect=0
F34 V3(E10) V22(E47) a=0.824318, can_bisect=1
F34 V3(E10) V40(E31) a=0.690265, can_bisect=0
F34 V3(E10) V30(E53) a=1.7128, can_bisect=1
F34 V3(E10) V40(E67) a=0.356792, can_bisect=0
F34 V3(E10) V59(E59) a=2.0944, can_bisect=1
F34 V3(E10) V40(E41) a=0.333473, can_bisect=1
F34 V3(E10) V3(E2) a=1.5708, can_bisect=1
F34 V40(E25) V22(E47) a=0, can_bisect=0
F34 V40(E25) V40(E31) a=1.21386, can_bisect=0
F34 V40(E25) V30(E53) a=0.778472, can_bisect=0
F34 V40(E25) V40(E67) a=0.690265, can_bisect=0
F34 V40(E25) V59(E59) a=0.0607767, can_bisect=1
F34 V40(E25) V40(E41) a=0.166667, can_bisect=1
F34 V40(E25) V3(E2) a=0.333473, can_bisect=1
F34 V22(E47) V40(E31) a=0, can_bisect=0
F34 V22(E47) V30(E53) a=1.40413, can_bisect=1
F34 V22(E47) V40(E67) a=0.42154, can_bisect=0
F34 V22(E47) V59(E59) a=1.92773, can_bisect=1
F34 V22(E47) V40(E41) a=0.0607767, can_bisect=1
F34 V22(E47) V3(E2) a=2.0944, can_bisect=1
F34 V40(E31) V30(E53) a=0, can_bisect=0
F34 V40(E31) V40(E67) a=1.21386, can_bisect=0
F34 V40(E31) V59(E59) a=0.42154, can_bisect=0
F34 V40(E31) V40(E41) a=0.690265, can_bisect=0
F34 V40(E31) V3(E2) a=0.356792, can_bisect=0
F34 V30(E53) V40(E67) a=0, can_bisect=0
F34 V30(E53) V59(E59) a=1.40413, can_bisect=1
F34 V30(E53) V40(E41) a=0.778472, can_bisect=0
F34 V30(E53) V3(E2) a=1.7128, can_bisect=1
F34 V40(E67) V59(E59) a=0, can_bisect=0
F34 V40(E67) V40(E41) a=1.21386, can_bisect=0
F34 V40(E67) V3(E2) a=0.690265, can_bisect=0
F34 V59(E59) V40(E41) a=0, can_bisect=0
F34 V59(E59) V3(E2) a=0.824318, can_bisect=1
F34 V40(E41) V3(E2) a=0, can_bisect=0
F34 best_angle = 2.0944
F36 V30(E53) V40(E67) a=0, can_bisect=0
F36 V30(E53) V59(E59) a=1.0472, can_bisect=1
F36 V30(E53) V40(E41) a=0.778472, can_bisect=0
F36 V30(E53) V3(E2) a=0.738527, can_bisect=1
F36 V30(E53) V40(E38) a=0.417709, can_bisect=1
F36 V30(E53) V15(E74) a=0, can_bisect=0
F36 V40(E67) V59(E59) a=0, can_bisect=0
F36 V40(E67) V40(E41) a=1.21386, can_bisect=0
F36 V40(E67) V3(E2) a=0.690265, can_bisect=0
F36 V40(E67) V40(E38) a=0, can_bisect=0
F36 V40(E67) V15(E74) a=0.333473, can_bisect=0
F36 V59(E59) V40(E41) a=0, can_bisect=0
F36 V59(E59) V3(E2) a=0.824318, can_bisect=1
F36 V59(E59) V40(E38) a=0.268726, can_bisect=1
F36 V59(E59) V15(E74) a=0.308671, can_bisect=1
F36 V40(E41) V3(E2) a=0, can_bisect=0
F36 V40(E41) V40(E38) a=1.21386, can_bisect=0
F36 V40(E41) V15(E74) a=0, can_bisect=0
F36 V3(E2) V40(E38) a=0, can_bisect=0
F36 V3(E2) V15(E74) a=1.0472, can_bisect=1
F36 V40(E38) V15(E74) a=0, can_bisect=0
F36 best_angle = 1.0472
F38 V59(E59) V40(E41) a=0, can_bisect=0
F38 V59(E59) V3(E2) a=0.824318, can_bisect=1
F38 V59(E59) V40(E38) a=0.268726, can_bisect=1
F38 V59(E59) V15(E74) a=0.308671, can_bisect=1
F38 V59(E59) V30(E75) a=0, can_bisect=0
F38 V40(E41) V3(E2) a=0, can_bisect=0
F38 V40(E41) V40(E38) a=1.21386, can_bisect=0
F38 V40(E41) V15(E74) a=0, can_bisect=0
F38 V40(E41) V30(E75) a=0.778472, can_bisect=0
F38 V3(E2) V40(E38) a=0, can_bisect=0
F38 V3(E2) V15(E74) a=1.0472, can_bisect=1
F38 V3(E2) V30(E75) a=0.308671, can_bisect=1
F38 V40(E38) V15(E74) a=0, can_bisect=0
F38 V40(E38) V30(E75) a=0.417709, can_bisect=1
F38 V15(E74) V30(E75) a=0, can_bisect=0
F38 best_angle = 1.0472
F40 V15(E74) V30(E75) a=0, can_bisect=0
F40 V15(E74) V59(E59) a=0.308671, can_bisect=1
F40 V15(E74) V40(E41) a=0, can_bisect=0
F40 V15(E74) V3(E76) a=0, can_bisect=0
F40 V30(E75) V59(E59) a=0, can_bisect=0
F40 V30(E75) V40(E41) a=0.778472, can_bisect=0
F40 V30(E75) V3(E76) a=0.308671, can_bisect=1
F40 V59(E59) V40(E41) a=0, can_bisect=0
F40 V59(E59) V3(E76) a=0.824318, can_bisect=1
F40 V40(E41) V3(E76) a=0, can_bisect=0
F40 best_angle = 0.824318
F42 V3(E76) V15(E74) a=0, can_bisect=0
F42 V3(E76) V30(E75) a=0.158715, can_bisect=1
F42 V3(E76) V59(E77) a=0, can_bisect=0
F42 V15(E74) V30(E75) a=0, can_bisect=0
F42 V15(E74) V59(E77) a=0.308671, can_bisect=1
F42 V30(E75) V59(E77) a=0, can_bisect=0
F42 best_angle = 0.308671
F35 V15(E16) V40(E2) a=0, can_bisect=0
F35 V15(E16) V3(E10) a=1.0472, can_bisect=1
F35 V15(E16) V40(E25) a=0, can_bisect=1
F35 V15(E16) V22(E47) a=0.308671, can_bisect=1
F35 V15(E16) V40(E31) a=0.333473, can_bisect=0
F35 V15(E16) V30(E74) a=0, can_bisect=0
F35 V40(E2) V3(E10) a=0, can_bisect=0
F35 V40(E2) V40(E25) a=1.21386, can_bisect=0
F35 V40(E2) V22(E47) a=0.268726, can_bisect=1
F35 V40(E2) V40(E31) a=0, can_bisect=0
F35 V40(E2) V30(E74) a=0.417709, can_bisect=1
F35 V3(E10) V40(E25) a=0, can_bisect=0
F35 V3(E10) V22(E47) a=0.824318, can_bisect=1
F35 V3(E10) V40(E31) a=0.690265, can_bisect=0
F35 V3(E10) V30(E74) a=0.738527, can_bisect=1
F35 V40(E25) V22(E47) a=0, can_bisect=0
F35 V40(E25) V40(E31) a=1.21386, can_bisect=0
F35 V40(E25) V30(E74) a=0.778472, can_bisect=0
F35 V22(E47) V40(E31) a=0, can_bisect=0
F35 V22(E47) V30(E74) a=1.0472, can_bisect=1
F35 V40(E31) V30(E74) a=0, can_bisect=0
F35 best_angle = 1.0472
F46 V30(E74) V15(E16) a=0, can_bisect=0
F46 V30(E74) V40(E2) a=0.417709, can_bisect=1
F46 V30(E74) V3(E10) a=0.308671, can_bisect=1
F46 V30(E74) V40(E25) a=0.778472, can_bisect=0
F46 V30(E74) V22(E79) a=0, can_bisect=0
F46 V15(E16) V40(E2) a=0, can_bisect=0
F46 V15(E16) V3(E10) a=1.0472, can_bisect=1
F46 V15(E16) V40(E25) a=0, can_bisect=1
F46 V15(E16) V22(E79) a=0.308671, can_bisect=1
F46 V40(E2) V3(E10) a=0, can_bisect=0
F46 V40(E2) V40(E25) a=1.21386, can_bisect=0
F46 V40(E2) V22(E79) a=0.268726, can_bisect=1
F46 V3(E10) V40(E25) a=0, can_bisect=0
F46 V3(E10) V22(E79) a=0.824318, can_bisect=1
F46 V40(E25) V22(E79) a=0, can_bisect=0
F46 best_angle = 1.0472
F48 V3(E10) V40(E25) a=0, can_bisect=0
F48 V3(E10) V22(E79) a=0.824318, can_bisect=1
F48 V3(E10) V30(E74) a=0.308671, can_bisect=1
F48 V3(E10) V15(E80) a=0, can_bisect=0
F48 V40(E25) V22(E79) a=0, can_bisect=0
F48 V40(E25) V30(E74) a=0.778472, can_bisect=0
F48 V40(E25) V15(E80) a=0, can_bisect=1
F48 V22(E79) V30(E74) a=0, can_bisect=0
F48 V22(E79) V15(E80) a=0.308671, can_bisect=1
F48 V30(E74) V15(E80) a=0, can_bisect=0
F48 best_angle = 0.824318
F50 V22(E79) V30(E74) a=0, can_bisect=0
F50 V22(E79) V15(E80) a=0.308671, can_bisect=1
F50 V22(E79) V3(E81) a=0, can_bisect=0
F50 V30(E74) V15(E80) a=0, can_bisect=0
F50 V30(E74) V3(E81) a=0.158715, can_bisect=1
F50 V15(E80) V3(E81) a=0, can_bisect=0
F50 best_angle = 0.308671
Removing V22
F53 V3(E10) V40(E16) a=0, can_bisect=0
F53 V3(E10) V15(E52) a=1.0472, can_bisect=1
F53 V3(E10) V40(E31) a=0, can_bisect=1
F53 V3(E10) V30(E74) a=0.738527, can_bisect=1
F53 V3(E10) V15(E80) a=0, can_bisect=0
F53 V40(E16) V15(E52) a=0, can_bisect=0
F53 V40(E16) V40(E31) a=1.21386, can_bisect=0
F53 V40(E16) V30(E74) a=0.268726, can_bisect=1
F53 V40(E16) V15(E80) a=0, can_bisect=1
F53 V15(E52) V40(E31) a=0, can_bisect=0
F53 V15(E52) V30(E74) a=0.824318, can_bisect=1
F53 V15(E52) V15(E80) a=0.523599, can_bisect=1
F53 V40(E31) V30(E74) a=0, can_bisect=0
F53 V40(E31) V15(E80) a=0.333473, can_bisect=1
F53 V30(E74) V15(E80) a=0, can_bisect=0
F53 best_angle = 1.0472
F55 V15(E52) V40(E31) a=0, can_bisect=0
F55 V15(E52) V30(E74) a=0.824318, can_bisect=1
F55 V15(E52) V15(E80) a=0.523599, can_bisect=1
F55 V15(E52) V3(E83) a=0, can_bisect=0
F55 V40(E31) V30(E74) a=0, can_bisect=0
F55 V40(E31) V15(E80) a=0.333473, can_bisect=1
F55 V40(E31) V3(E83) a=0, can_bisect=1
F55 V30(E74) V15(E80) a=0, can_bisect=0
F55 V30(E74) V3(E83) a=0.738527, can_bisect=1
F55 V15(E80) V3(E83) a=0, can_bisect=0
F55 best_angle = 0.824318
F57 V30(E74) V15(E80) a=0, can_bisect=0
F57 V30(E74) V3(E83) a=0.738527, can_bisect=1
F57 V30(E74) V15(E84) a=0, can_bisect=0
F57 V15(E80) V3(E83) a=0, can_bisect=0
F57 V15(E80) V15(E84) a=0.523599, can_bisect=1
F57 V3(E83) V15(E84) a=0, can_bisect=0
F57 best_angle = 0.738527
Removing V30
F60 V15(E52) V40(E10) a=0, can_bisect=0
F60 V15(E52) V3(E41) a=1.0472, can_bisect=1
F60 V15(E52) V40(E67) a=0, can_bisect=1
F60 V15(E52) V59(E78) a=1.35587, can_bisect=1
F60 V15(E52) V15(E80) a=0.523599, can_bisect=1
F60 V15(E52) V3(E83) a=0, can_bisect=0
F60 V40(E10) V3(E41) a=0, can_bisect=0
F60 V40(E10) V40(E67) a=1.21386, can_bisect=0
F60 V40(E10) V59(E78) a=0.268726, can_bisect=1
F60 V40(E10) V15(E80) a=0.333473, can_bisect=1
F60 V40(E10) V3(E83) a=0, can_bisect=1
F60 V3(E41) V40(E67) a=0, can_bisect=0
F60 V3(E41) V59(E78) a=0.824318, can_bisect=1
F60 V3(E41) V15(E80) a=0.308671, can_bisect=1
F60 V3(E41) V3(E83) a=0.523599, can_bisect=1
F60 V40(E67) V59(E78) a=0, can_bisect=0
F60 V40(E67) V15(E80) a=0.713724, can_bisect=0
F60 V40(E67) V3(E83) a=0.333473, can_bisect=1
F60 V59(E78) V15(E80) a=0, can_bisect=0
F60 V59(E78) V3(E83) a=0.738527, can_bisect=1
F60 V15(E80) V3(E83) a=0, can_bisect=0
F60 best_angle = 1.35587
F62 V59(E78) V15(E80) a=0, can_bisect=0
F62 V59(E78) V3(E83) a=0.738527, can_bisect=1
F62 V59(E78) V15(E86) a=0, can_bisect=0
F62 V15(E80) V3(E83) a=0, can_bisect=0
F62 V15(E80) V15(E86) a=0.523599, can_bisect=1
F62 V3(E83) V15(E86) a=0, can_bisect=0
F62 best_angle = 0.738527
F61 V15(E52) V40(E10) a=0, can_bisect=0
F61 V15(E52) V3(E41) a=0.738527, can_bisect=1
F61 V15(E52) V40(E67) a=0, can_bisect=1
F61 V15(E52) V59(E86) a=0, can_bisect=0
F61 V40(E10) V3(E41) a=0, can_bisect=0
F61 V40(E10) V40(E67) a=1.21386, can_bisect=0
F61 V40(E10) V59(E86) a=0.268726, can_bisect=1
F61 V3(E41) V40(E67) a=0, can_bisect=0
F61 V3(E41) V59(E86) a=0.824318, can_bisect=1
F61 V40(E67) V59(E86) a=0, can_bisect=0
F61 best_angle = 0.824318
F66 V59(E86) V15(E52) a=0, can_bisect=0
F66 V59(E86) V40(E10) a=0.268726, can_bisect=1
F66 V59(E86) V3(E88) a=0, can_bisect=0
F66 V15(E52) V40(E10) a=0, can_bisect=0
F66 V15(E52) V3(E88) a=0.738527, can_bisect=1
F66 V40(E10) V3(E88) a=0, can_bisect=0
F66 best_angle = 0.738527
Removing V40
F69 V3(E77) V59(E60) a=0, can_bisect=0
F69 V3(E77) V15(E76) a=0.579812, can_bisect=1
F69 V3(E77) V3(E80) a=0.523599, can_bisect=1
F69 V3(E77) V15(E83) a=1.0472, can_bisect=1
F69 V3(E77) V3(E89) a=0.523599, can_bisect=1
F69 V3(E77) V15(E60) a=0.579812, can_bisect=1
F69 V3(E77) V59(E88) a=0, can_bisect=0
F69 V59(E60) V15(E76) a=0, can_bisect=0
F69 V59(E60) V3(E80) a=0, can_bisect=1
F69 V59(E60) V15(E83) a=0.399932, can_bisect=1
F69 V59(E60) V3(E89) a=0.673367, can_bisect=1
F69 V59(E60) V15(E60) a=0.250595, can_bisect=1
F69 V59(E60) V59(E88) a=1.5708, can_bisect=0
F69 V15(E76) V3(E80) a=0, can_bisect=0
F69 V15(E76) V15(E83) a=0.523599, can_bisect=1
F69 V15(E76) V3(E89) a=1.0472, can_bisect=1
F69 V15(E76) V15(E60) a=1.5708, can_bisect=1
F69 V15(E76) V59(E88) a=0.250595, can_bisect=1
F69 V3(E80) V15(E83) a=0, can_bisect=0
F69 V3(E80) V3(E89) a=0.523599, can_bisect=1
F69 V3(E80) V15(E60) a=1.0472, can_bisect=1
F69 V3(E80) V59(E88) a=0.673367, can_bisect=1
F69 V15(E83) V3(E89) a=0, can_bisect=0
F69 V15(E83) V15(E60) a=0.523599, can_bisect=1
F69 V15(E83) V59(E88) a=0.399932, can_bisect=1
F69 V3(E89) V15(E60) a=0, can_bisect=0
F69 V3(E89) V59(E88) a=0, can_bisect=0
F69 V15(E60) V59(E88) a=0, can_bisect=0
F69 best_angle = 1.5708
F71 V15(E60) V59(E88) a=0, can_bisect=0
F71 V15(E60) V3(E77) a=0.523599, can_bisect=1
F71 V15(E60) V59(E60) a=0.250595, can_bisect=1
F71 V15(E60) V15(E90) a=0, can_bisect=0
F71 V59(E88) V3(E77) a=0, can_bisect=0
F71 V59(E88) V59(E60) a=1.5708, can_bisect=0
F71 V59(E88) V15(E90) a=0.250595, can_bisect=1
F71 V3(E77) V59(E60) a=0, can_bisect=0
F71 V3(E77) V15(E90) a=0.523599, can_bisect=1
F71 V59(E60) V15(E90) a=0, can_bisect=0
F71 best_angle = 0.523599
F73 V15(E90) V15(E60) a=0, can_bisect=0
F73 V15(E90) V59(E88) a=0.216791, can_bisect=1
F73 V15(E90) V3(E91) a=0, can_bisect=0
F73 V15(E60) V59(E88) a=0, can_bisect=0
F73 V15(E60) V3(E91) a=0.523599, can_bisect=1
F73 V59(E88) V3(E91) a=0, can_bisect=0
F73 best_angle = 0.523599
F70 V15(E76) V3(E80) a=0, can_bisect=0
F70 V15(E76) V15(E83) a=0.523599, can_bisect=1
F70 V15(E76) V3(E89) a=0.523599, can_bisect=1
F70 V15(E76) V15(E90) a=0, can_bisect=0
F70 V3(E80) V15(E83) a=0, can_bisect=0
F70 V3(E80) V3(E89) a=0.523599, can_bisect=1
F70 V3(E80) V15(E90) a=0.523599, can_bisect=1
F70 V15(E83) V3(E89) a=0, can_bisect=0
F70 V15(E83) V15(E90) a=0.523599, can_bisect=1
F70 V3(E89) V15(E90) a=0, can_bisect=0
F70 best_angle = 0.523599
F76 V3(E80) V15(E83) a=0, can_bisect=0
F76 V3(E80) V3(E89) a=0.523599, can_bisect=1
F76 V3(E80) V15(E93) a=0, can_bisect=0
F76 V15(E83) V3(E89) a=0, can_bisect=0
F76 V15(E83) V15(E93) a=0.523599, can_bisect=1
F76 V3(E89) V15(E93) a=0, can_bisect=0
F76 best_angle = 0.523599
Removing V59
F80 V15(E91) V3(E76) a=0, can_bisect=0
F80 V15(E91) V15(E80) a=0.523599, can_bisect=1
F80 V15(E91) V3(E83) a=1.0472, can_bisect=1
F80 V15(E91) V15(E89) a=0.523599, can_bisect=1
F80 V15(E91) V3(E92) a=0, can_bisect=0
F80 V3(E76) V15(E80) a=0, can_bisect=0
F80 V3(E76) V3(E83) a=0.523599, can_bisect=1
F80 V3(E76) V15(E89) a=1.0472, can_bisect=1
F80 V3(E76) V3(E92) a=0.523599, can_bisect=1
F80 V15(E80) V3(E83) a=0, can_bisect=0
F80 V15(E80) V15(E89) a=0.523599, can_bisect=1
F80 V15(E80) V3(E92) a=1.0472, can_bisect=1
F80 V3(E83) V15(E89) a=0, can_bisect=0
F80 V3(E83) V3(E92) a=0.523599, can_bisect=1
F80 V15(E89) V3(E92) a=0, can_bisect=0
F80 best_angle = 1.0472
F82 V3(E92) V15(E91) a=0, can_bisect=0
F82 V3(E92) V3(E76) a=0.523599, can_bisect=1
F82 V3(E92) V15(E95) a=0, can_bisect=0
F82 V15(E91) V3(E76) a=0, can_bisect=0
F82 V15(E91) V15(E95) a=0.523599, can_bisect=1
F82 V3(E76) V15(E95) a=0, can_bisect=0
F82 best_angle = 0.523599
F81 V15(E80) V3(E83) a=0, can_bisect=0
F81 V15(E80) V15(E89) a=0.523599, can_bisect=1
F81 V15(E80) V3(E95) a=0, can_bisect=0
F81 V3(E83) V15(E89) a=0, can_bisect=0
F81 V3(E83) V3(E95) a=0.523599, can_bisect=1
F81 V15(E89) V3(E95) a=0, can_bisect=0
F81 best_angle = 0.523599
Longest Edge: 4.43197
Looking for shortest saddle...2.21598
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.2159835222155908
# Vertices: V3 V15
# UEdges: 
E76 ( V3 V15 F84 F77 ) (-2.215983522215591250414946,0.000000000000000000000000)
E80 ( V15 V3 F85 F78 ) (-1.107991761107795625207473,-1.919098024606419539139779)
E83 ( V3 V15 F85 F78 ) (1.107991761107794959073658,-1.919098024606419539139779)
E89 ( V15 V3 F86 F79 ) (2.215983522215590806325736,0.000000000000000000000000)
E90 ( V15 V15 F75 F77 ) (0.000000000000001332267630,3.838196049212839078279558)
E91 ( V3 V15 F75 F83 ) (1.107991761107794737029053,-1.919098024606419539139779)
E92 ( V15 V3 F75 F83 ) (-1.107991761107795625207473,-1.919098024606419539139779)
E93 ( V3 V15 F77 F79 ) (-2.215983522215590362236526,3.838196049212839078279558)
E94 ( V3 V3 F79 F78 ) (0.000000000000000666133815,3.838196049212839078279558)
E95 ( V15 V3 F84 F86 ) (2.215983522215590362236526,-3.838196049212839078279558)
E96 ( V3 V3 F84 F83 ) (0.000000000000000888178420,3.838196049212839078279558)
E97 ( V15 V15 F86 F85 ) (-0.000000000000000666133815,-3.838196049212839078279558)
# Faces: 
F75 +E91 +E90 +E92
F77 -E90 -E76 +E93
F78 -E80 -E83 -E94
F79 -E89 -E93 +E94
F83 -E92 -E91 -E96
F84 +E76 +E95 +E96
F85 +E80 +E83 -E97
F86 +E89 -E95 +E97
#UEdges Algebraic 
#E76: (-1 0 0 0)+(-1 0 0 0)t1
#E80: (0 0 -1 0)+(0 0 -1 0)t1
#E83: (1 0 -1 0)+(1 0 -1 0)t1
#E89: (1 0 0 0)+(1 0 0 0)t1
#E90: (-1 0 2 0)+(-1 0 2 0)t1
#E91: (1 0 -1 0)+(1 0 -1 0)t1
#E92: (0 0 -1 0)+(0 0 -1 0)t1
#E93: (-2 0 2 0)+(-2 0 2 0)t1
#E94: (-1 0 2 0)+(-1 0 2 0)t1
#E95: (2 0 -2 0)+(2 0 -2 0)t1
#E96: (-1 0 2 0)+(-1 0 2 0)t1
#E97: (1 0 -2 0)+(1 0 -2 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V3(12) (4 PI) V15(12) (4 PI) 
# edges: 12
## E76(2) E80(2) E83(2) E89(2) E90(2) E91(2) E92(2) E93(2) E94(2) E95(2) E96(2) E97(2) 
# faces: 8
# genus: 2
# area: 25.5161
sweeping start V3 depth = 50.5135
File =  depth = 50.5135 follow_depth = 252.568 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 20 unusual 0 close 164237 reject 0
V3: AcBd V15: aDbC   cylinders |1|1|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.39932 0.850683 sym = 4
    raw = 1069 short saddle = 2.21598 short cyl = 6.64795 tag #0
V3: AbBa V15: CcDd   cylinders |1|1|2|
Moduli: |1|1|1| Min Area: 1 Max Area: 1
    total: 0.76969 0.467916 sym = 4
    raw = 588 short saddle = 3.8382 short cyl = 3.8382 tag #1

total cylinders: 5.10509 3.10352 raw = 975 rejected = 0
""",
    (1, 2, 1, 2): r"""
max loc = 1
alpha =  2 beta = 1 gamma = 2 delta =1
a =  2.0944 b = 1.0472 c= 2.0944 d=1.0472
a1_min = 8.88178e-16 a1_max = 2.0944
a1 = 1.42773
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-0.800317,1.38619)
Imaginary of t_cx = -5.55112e-16
p_algt = (1 0)+(0 0)t1
r_algt = (0 0)+(-1 1)t1
Checking p_alt (0,0)
Checking q_alt (9.15934e-16,-8.88178e-16)
Checking r_alt (-1.11022e-16,-4.44089e-16)
Checking p_algt (0,0)
Checking q_algt (9.15934e-16,-8.88178e-16)
Checking r_algt (-1.11022e-16,-4.44089e-16)
Number field: min poly (1 -1 1)
#Field Degree=2 minimal polynomial=(1 -1 1)
#Field generator embedding: (0.500000000000000110.8660254037844386
#Params:1
#t1=1.6006331954839124
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741955959577353,1.386189009429731333611358)
E2 ( V2 V3 F0 NULL ) (-1.000000000000000000000000,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (0.800316597741955959577353,-1.386189009429731333611358)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0)+(0 0)t1
#E1: (0 0)+(-1 1)t1
#E2: (-1 0)+(0 0)t1
#E3: (0 0)+(1 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.391038, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F13 V16(E5) V24(E19) a=0, can_bisect=0
F13 V16(E5) V16(E29) a=0.90413, can_bisect=1
F13 V16(E5) V24(E25) a=0.666667, can_bisect=1
F13 V16(E5) V16(E11) a=0.90413, can_bisect=1
F13 V16(E5) V24(E4) a=0, can_bisect=0
F13 V24(E19) V16(E29) a=0, can_bisect=0
F13 V24(E19) V24(E25) a=0.143068, can_bisect=1
F13 V24(E19) V16(E11) a=0.666667, can_bisect=1
F13 V24(E19) V24(E4) a=0.143068, can_bisect=1
F13 V16(E29) V24(E25) a=0, can_bisect=0
F13 V16(E29) V16(E11) a=0.90413, can_bisect=1
F13 V16(E29) V24(E4) a=0.666667, can_bisect=1
F13 V24(E25) V16(E11) a=0, can_bisect=0
F13 V24(E25) V24(E4) a=0.143068, can_bisect=1
F13 V16(E11) V24(E4) a=0, can_bisect=0
F13 best_angle = 0.90413
F15 V16(E11) V24(E4) a=0, can_bisect=0
F15 V16(E11) V16(E5) a=0.90413, can_bisect=1
F15 V16(E11) V24(E19) a=0.523599, can_bisect=1
F15 V16(E11) V16(E35) a=0, can_bisect=0
F15 V24(E4) V16(E5) a=0, can_bisect=0
F15 V24(E4) V24(E19) a=0.143068, can_bisect=1
F15 V24(E4) V16(E35) a=0.523599, can_bisect=1
F15 V16(E5) V24(E19) a=0, can_bisect=0
F15 V16(E5) V16(E35) a=0.90413, can_bisect=1
F15 V24(E19) V16(E35) a=0, can_bisect=0
F15 best_angle = 0.90413
F17 V16(E35) V16(E11) a=0, can_bisect=0
F17 V16(E35) V24(E4) a=0.523599, can_bisect=1
F17 V16(E35) V16(E36) a=0, can_bisect=0
F17 V16(E11) V24(E4) a=0, can_bisect=0
F17 V16(E11) V16(E36) a=0.90413, can_bisect=1
F17 V24(E4) V16(E36) a=0, can_bisect=0
F17 best_angle = 0.90413
Removing V3
F20 V24(E5) V16(E19) a=0, can_bisect=0
F20 V24(E5) V24(E29) a=0.90413, can_bisect=1
F20 V24(E5) V16(E25) a=0.666667, can_bisect=1
F20 V24(E5) V24(E11) a=0.90413, can_bisect=1
F20 V24(E5) V16(E4) a=0, can_bisect=0
F20 V16(E19) V24(E29) a=0, can_bisect=0
F20 V16(E19) V16(E25) a=0.143068, can_bisect=1
F20 V16(E19) V24(E11) a=0.666667, can_bisect=1
F20 V16(E19) V16(E4) a=0.143068, can_bisect=1
F20 V24(E29) V16(E25) a=0, can_bisect=0
F20 V24(E29) V24(E11) a=0.90413, can_bisect=1
F20 V24(E29) V16(E4) a=0.666667, can_bisect=1
F20 V16(E25) V24(E11) a=0, can_bisect=0
F20 V16(E25) V16(E4) a=0.143068, can_bisect=1
F20 V24(E11) V16(E4) a=0, can_bisect=0
F20 best_angle = 0.90413
F22 V24(E11) V16(E4) a=0, can_bisect=0
F22 V24(E11) V24(E5) a=0.90413, can_bisect=1
F22 V24(E11) V16(E19) a=0.523599, can_bisect=1
F22 V24(E11) V24(E38) a=0, can_bisect=0
F22 V16(E4) V24(E5) a=0, can_bisect=0
F22 V16(E4) V16(E19) a=0.143068, can_bisect=1
F22 V16(E4) V24(E38) a=0.523599, can_bisect=1
F22 V24(E5) V16(E19) a=0, can_bisect=0
F22 V24(E5) V24(E38) a=0.90413, can_bisect=1
F22 V16(E19) V24(E38) a=0, can_bisect=0
F22 best_angle = 0.90413
F24 V24(E38) V24(E11) a=0, can_bisect=0
F24 V24(E38) V16(E4) a=0.523599, can_bisect=1
F24 V24(E38) V24(E39) a=0, can_bisect=0
F24 V24(E11) V16(E4) a=0, can_bisect=0
F24 V24(E11) V24(E39) a=0.90413, can_bisect=1
F24 V16(E4) V24(E39) a=0, can_bisect=0
F24 best_angle = 0.90413
Longest Edge: 1.73205
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 1
#Field Degree=2 minimal polynomial=(1 -1 1)
#Field generator embedding: (0.500000000000000110.8660254037844386
#Params:1
#t1=1.6006331954839124
# Vertices: V16 V24
# UEdges: 
E4 ( V16 V24 F25 F18 ) (0.199683402258044040422647,1.386189009429731333611358)
E5 ( V16 V24 F16 F23 ) (0.199683402258043596333437,-1.386189009429731333611358)
E11 ( V16 V24 F18 F25 ) (-1.300316597741955959577353,0.520163605645292737023055)
E19 ( V24 V16 F16 F23 ) (1.300316597741956403666563,0.520163605645292737023055)
E25 ( V24 V16 F14 F21 ) (-1.100633195483912363243917,0.866025403784438596588302)
E29 ( V16 V24 F14 F21 ) (1.100633195483912363243917,0.866025403784438596588302)
E35 ( V16 V16 F19 F14 ) (0.000000000000000000000000,1.732050807568877193176604)
E36 ( V16 V16 F19 F16 ) (1.500000000000000000000000,-0.866025403784438596588302)
E37 ( V16 V16 F19 F18 ) (-1.500000000000000000000000,-0.866025403784438596588302)
E38 ( V24 V24 F26 F21 ) (0.000000000000000000000000,-1.732050807568877193176604)
E39 ( V24 V24 F26 F23 ) (-1.500000000000000000000000,0.866025403784438596588302)
E40 ( V24 V24 F26 F25 ) (1.500000000000000000000000,0.866025403784438596588302)
# Faces: 
F14 +E29 +E25 -E35
F16 +E5 +E19 -E36
F18 +E11 -E4 -E37
F19 +E36 +E35 +E37
F21 -E29 -E25 -E38
F23 -E5 -E19 -E39
F25 -E11 +E4 -E40
F26 +E39 +E38 +E40
#UEdges Algebraic 
#E4: (1 0)+(-1 1)t1
#E5: (1 0)+(0 -1)t1
#E11: (0 -1)+(-1 1)t1
#E19: (1 -1)+(0 1)t1
#E25: (0 1)+(-1 0)t1
#E29: (-1 1)+(1 0)t1
#E35: (-1 2)+(0 0)t1
#E36: (2 -1)+(0 0)t1
#E37: (-1 -1)+(0 0)t1
#E38: (1 -2)+(0 0)t1
#E39: (-2 1)+(0 0)t1
#E40: (1 1)+(0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V16(12) (4 PI) V24(12) (4 PI) 
# edges: 12
## E4(2) E5(2) E11(2) E19(2) E25(2) E29(2) E35(2) E36(2) E37(2) E38(2) E39(2) E40(2) 
# faces: 8
# genus: 2
# area: 8.31713
sweeping start V16 depth = 28.8394
File =  depth = 28.8394 follow_depth = 144.197 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 10 unusual 0 close 31827 reject 0
V16: AaXX V24: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0120598 Max Area: 0.755899
    total: 2.1677 1.3178 sym = 2
    raw = 828 short saddle = 3.93548 short cyl = 3.93548 tag #1
V16: AXBc V24: aCbX   cylinders |1|
Moduli: |1| Min Area: 0.285685 Max Area: 0.44069
    total: 0.0837758 0.0509296 sym = 3
    raw = 48 short saddle = 4.19108 short cyl = 12.5733 tag #3
V16: AaBb V24: CdDc   cylinders |1|1.58063|2.58063|
Moduli: |1.50719|1|1.19654| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 28.4178 short cyl = 28.4178 tag #4
V16: AaXc V24: BXCb   cylinders |1|
Moduli: |1| Min Area: 0.13068 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #5
V16: AbBa V24: CcDd   cylinders |1|1.60063|2.60063|
Moduli: |8.82252|1|4.00793| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 1.73205 short cyl = 1.73205 tag #6
V16: AXBb V24: aXcC   cylinders |1|
Moduli: |1| Min Area: 0.13068 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #7

total cylinders: 2.38761 1.45149 raw = 456 rejected = 0
""",
    (1, 2, 2, 3): r"""
max loc = 3
alpha =  3 beta = 1 gamma = 2 delta =2
a =  2.35619 b = 0.785398 c= 1.5708 d=1.5708
a1_min = 0.785398 a1_max = 2.35619
a1 = 1.85619
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 1.4749 p-q = 2.00146
p = (1,0) q = (-0.415244,1.41524) r = (-0.915244,0.915244)
Imaginary of t_cx = -2.22045e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(0 0 0 1)t1
Checking p_alt (0,0)
Checking q_alt (4.996e-16,4.44089e-16)
Checking r_alt (1.11022e-16,0)
Checking p_algt (0,0)
Checking q_algt (4.996e-16,4.44089e-16)
Checking r_algt (1.11022e-16,0)
Number field: min poly (1 0 0 0 1)
#Field Degree=4 minimal polynomial=(1 0 0 0 1)
#Field generator embedding: (0.707106781186547570.70710678118654746
#Params:1
#t1=1.2943502809015872
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-1.415243860856224777933221,1.415243860856225222022431)
E2 ( V2 V3 F0 NULL ) (-0.500000000000000111022302,-0.500000000000000111022302)
E3 ( V3 V0 F0 NULL ) (0.915243860856224777933221,-0.915243860856225111000128)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-1/2 0 1/2 0)+(0 0 0 1)t1
#E2: (-1/2 0 -1/2 0)+(0 0 0 0)t1
#E3: (0 0 0 0)+(0 0 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.5, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.339608, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.5
In RetriangulateSurface
Removing V1
F17 V30(E5) V5(E25) a=0, can_bisect=0
F17 V30(E5) V30(E17) a=1.0708, can_bisect=1
F17 V30(E5) V2(E37) a=0.244636, can_bisect=1
F17 V30(E5) V30(E41) a=1.85619, can_bisect=1
F17 V30(E5) V5(E31) a=0.244636, can_bisect=1
F17 V30(E5) V30(E11) a=1.0708, can_bisect=1
F17 V30(E5) V2(E4) a=0, can_bisect=0
F17 V5(E25) V30(E17) a=0, can_bisect=0
F17 V5(E25) V2(E37) a=1.2854, can_bisect=0
F17 V5(E25) V30(E41) a=0.244636, can_bisect=1
F17 V5(E25) V5(E31) a=0.5, can_bisect=1
F17 V5(E25) V30(E11) a=0.244636, can_bisect=1
F17 V5(E25) V2(E4) a=1.2854, can_bisect=0
F17 V30(E17) V2(E37) a=0, can_bisect=0
F17 V30(E17) V30(E41) a=1.0708, can_bisect=1
F17 V30(E17) V5(E31) a=0.244636, can_bisect=1
F17 V30(E17) V30(E11) a=1.85619, can_bisect=1
F17 V30(E17) V2(E4) a=0.244636, can_bisect=1
F17 V2(E37) V30(E41) a=0, can_bisect=0
F17 V2(E37) V5(E31) a=1.2854, can_bisect=0
F17 V2(E37) V30(E11) a=0.244636, can_bisect=1
F17 V2(E37) V2(E4) a=0.5, can_bisect=1
F17 V30(E41) V5(E31) a=0, can_bisect=0
F17 V30(E41) V30(E11) a=1.0708, can_bisect=1
F17 V30(E41) V2(E4) a=0.244636, can_bisect=1
F17 V5(E31) V30(E11) a=0, can_bisect=0
F17 V5(E31) V2(E4) a=1.2854, can_bisect=0
F17 V30(E11) V2(E4) a=0, can_bisect=0
F17 best_angle = 1.85619
F19 V30(E11) V2(E4) a=0, can_bisect=0
F19 V30(E11) V30(E5) a=0.785398, can_bisect=1
F19 V30(E11) V5(E25) a=0.244636, can_bisect=1
F19 V30(E11) V30(E47) a=0, can_bisect=0
F19 V2(E4) V30(E5) a=0, can_bisect=0
F19 V2(E4) V5(E25) a=1.2854, can_bisect=0
F19 V2(E4) V30(E47) a=0.244636, can_bisect=1
F19 V30(E5) V5(E25) a=0, can_bisect=0
F19 V30(E5) V30(E47) a=0.785398, can_bisect=1
F19 V5(E25) V30(E47) a=0, can_bisect=0
F19 best_angle = 0.785398
F21 V30(E47) V30(E11) a=0, can_bisect=0
F21 V30(E47) V2(E4) a=0.244636, can_bisect=1
F21 V30(E47) V30(E48) a=0, can_bisect=0
F21 V30(E11) V2(E4) a=0, can_bisect=0
F21 V30(E11) V30(E48) a=0.785398, can_bisect=1
F21 V2(E4) V30(E48) a=0, can_bisect=0
F21 best_angle = 0.785398
F18 V30(E17) V2(E37) a=0, can_bisect=0
F18 V30(E17) V30(E41) a=0.785398, can_bisect=1
F18 V30(E17) V5(E31) a=0.244636, can_bisect=1
F18 V30(E17) V30(E47) a=0, can_bisect=0
F18 V2(E37) V30(E41) a=0, can_bisect=0
F18 V2(E37) V5(E31) a=1.2854, can_bisect=0
F18 V2(E37) V30(E47) a=0.244636, can_bisect=1
F18 V30(E41) V5(E31) a=0, can_bisect=0
F18 V30(E41) V30(E47) a=0.785398, can_bisect=1
F18 V5(E31) V30(E47) a=0, can_bisect=0
F18 best_angle = 0.785398
F25 V30(E47) V30(E17) a=0, can_bisect=0
F25 V30(E47) V2(E37) a=0.244636, can_bisect=1
F25 V30(E47) V30(E50) a=0, can_bisect=0
F25 V30(E17) V2(E37) a=0, can_bisect=0
F25 V30(E17) V30(E50) a=0.785398, can_bisect=1
F25 V2(E37) V30(E50) a=0, can_bisect=0
F25 best_angle = 0.785398
Removing V2
F28 V30(E3) V3(E18) a=0, can_bisect=0
F28 V30(E3) V30(E51) a=0, can_bisect=0
F28 V30(E3) V30(E18) a=0.5, can_bisect=1
F28 V30(E3) V3(E3) a=0.741174, can_bisect=1
F28 V30(E3) V30(E49) a=0, can_bisect=0
F28 V3(E18) V30(E51) a=0, can_bisect=0
F28 V3(E18) V30(E18) a=0.741174, can_bisect=1
F28 V3(E18) V3(E3) a=1.5708, can_bisect=1
F28 V3(E18) V30(E49) a=0.741174, can_bisect=1
F28 V30(E51) V30(E18) a=0, can_bisect=0
F28 V30(E51) V3(E3) a=0.741174, can_bisect=1
F28 V30(E51) V30(E49) a=0.5, can_bisect=1
F28 V30(E18) V3(E3) a=0, can_bisect=0
F28 V30(E18) V30(E49) a=0, can_bisect=1
F28 V3(E3) V30(E49) a=0, can_bisect=0
F28 best_angle = 1.5708
F30 V3(E3) V30(E49) a=0, can_bisect=0
F30 V3(E3) V30(E3) a=0.741174, can_bisect=1
F30 V3(E3) V3(E52) a=0, can_bisect=0
F30 V30(E49) V30(E3) a=0, can_bisect=0
F30 V30(E49) V3(E52) a=0.741174, can_bisect=1
F30 V30(E3) V3(E52) a=0, can_bisect=0
F30 best_angle = 0.741174
F29 V3(E18) V30(E51) a=0, can_bisect=0
F29 V3(E18) V30(E18) a=0.741174, can_bisect=1
F29 V3(E18) V3(E52) a=0, can_bisect=0
F29 V30(E51) V30(E18) a=0, can_bisect=0
F29 V30(E51) V3(E52) a=0.741174, can_bisect=1
F29 V30(E18) V3(E52) a=0, can_bisect=0
F29 best_angle = 0.741174
Flipping E52
Removing V3
F37 V30(E51) V30(E55) a=0, can_bisect=0
F37 V30(E51) V30(E49) a=0.5, can_bisect=1
F37 V30(E51) V30(E55) a=0, can_bisect=0
F37 V30(E55) V30(E49) a=0, can_bisect=0
F37 V30(E55) V30(E55) a=0.329623, can_bisect=1
F37 V30(E49) V30(E55) a=0, can_bisect=0
F37 best_angle = 0.5
Removing V5
F40 V30(E30) V9(E9) a=0, can_bisect=0
F40 V30(E30) V30(E48) a=0, can_bisect=1
F40 V30(E30) V30(E9) a=0.5, can_bisect=1
F40 V30(E30) V9(E30) a=0.741174, can_bisect=1
F40 V30(E30) V30(E50) a=0, can_bisect=0
F40 V9(E9) V30(E48) a=0, can_bisect=0
F40 V9(E9) V30(E9) a=0.741174, can_bisect=1
F40 V9(E9) V9(E30) a=1.5708, can_bisect=1
F40 V9(E9) V30(E50) a=0.741174, can_bisect=1
F40 V30(E48) V30(E9) a=0, can_bisect=0
F40 V30(E48) V9(E30) a=0.741174, can_bisect=1
F40 V30(E48) V30(E50) a=0.5, can_bisect=1
F40 V30(E9) V9(E30) a=0, can_bisect=0
F40 V30(E9) V30(E50) a=0, can_bisect=0
F40 V9(E30) V30(E50) a=0, can_bisect=0
F40 best_angle = 1.5708
F42 V9(E30) V30(E50) a=0, can_bisect=0
F42 V9(E30) V30(E30) a=0.741174, can_bisect=1
F42 V9(E30) V9(E57) a=0, can_bisect=0
F42 V30(E50) V30(E30) a=0, can_bisect=0
F42 V30(E50) V9(E57) a=0.741174, can_bisect=1
F42 V30(E30) V9(E57) a=0, can_bisect=0
F42 best_angle = 0.741174
F41 V9(E9) V30(E48) a=0, can_bisect=0
F41 V9(E9) V30(E9) a=0.741174, can_bisect=1
F41 V9(E9) V9(E57) a=0, can_bisect=0
F41 V30(E48) V30(E9) a=0, can_bisect=0
F41 V30(E48) V9(E57) a=0.741174, can_bisect=1
F41 V30(E9) V9(E57) a=0, can_bisect=0
F41 best_angle = 0.741174
Flipping E57
Removing V9
F49 V30(E48) V30(E60) a=0, can_bisect=0
F49 V30(E48) V30(E50) a=0.5, can_bisect=1
F49 V30(E48) V30(E60) a=0, can_bisect=0
F49 V30(E60) V30(E50) a=0, can_bisect=0
F49 V30(E60) V30(E60) a=0.329623, can_bisect=1
F49 V30(E50) V30(E60) a=0, can_bisect=0
F49 best_angle = 0.5
Longest Edge: 2.94981
Looking for shortest saddle...1.41421
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 0 0 1)
#Field generator embedding: (0.707106781186547570.70710678118654746
#Params:1
#t1=1.2943502809015872
# Vertices: V30
# UEdges: 
E47 ( V30 V30 F23 F27 ) (0.000000000000000444089210,2.000000000000000000000000)
E48 ( V30 V30 F23 F50 ) (0.999999999999999666933093,-0.999999999999999888977698)
E49 ( V30 V30 F23 F39 ) (-1.000000000000000000000000,-1.000000000000000000000000)
E50 ( V30 V30 F27 F51 ) (-0.999999999999999666933093,0.999999999999999888977698)
E51 ( V30 V30 F27 F38 ) (1.000000000000000000000000,1.000000000000000000000000)
E55 ( V30 V30 F39 F38 ) (-2.830487721712449555866442,0.830487721712450222000257)
E56 ( V30 V30 F39 F38 ) (1.830487721712449555866442,-1.830487721712450222000257)
E60 ( V30 V30 F51 F50 ) (0.830487721712450222000257,2.830487721712449555866442)
E61 ( V30 V30 F51 F50 ) (-1.830487721712449999955652,-1.830487721712449555866442)
# Faces: 
F23 +E48 +E47 +E49
F27 +E50 -E47 +E51
F38 -E51 -E55 -E56
F39 -E49 +E55 +E56
F50 -E48 -E60 -E61
F51 -E50 +E60 +E61
#UEdges Algebraic 
#E47: (0 0 2 0)+(0 0 0 0)t1
#E48: (1 0 -1 0)+(0 0 0 0)t1
#E49: (-1 0 -1 0)+(0 0 0 0)t1
#E50: (-1 0 1 0)+(0 0 0 0)t1
#E51: (1 0 1 0)+(0 0 0 0)t1
#E55: (-1 0 -1 0)+(0 0 0 2)t1
#E56: (0 0 0 0)+(0 0 0 -2)t1
#E60: (-1 0 1 0)+(0 2 0 0)t1
#E61: (0 0 0 0)+(0 -2 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 1
## V30(18) (6 PI) 
# edges: 9
## E47(2) E48(2) E49(2) E50(2) E51(2) E55(2) E56(2) E60(2) E61(2) 
# faces: 6
# genus: 2
# area: 9.32195
sweeping start V30 depth = 30.5319
File =  depth = 30.5319 follow_depth = 152.659 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 32 unusual 0 close 47657 reject 0
V30: AXBbXa   cylinders |1|
Moduli: |1| Min Area: 0.00250178 Max Area: 0.899561
    total: 1.5708 0.95493 sym = 4
    raw = 1200 short saddle = 4.24539 short cyl = 4.24539 tag #1
V30: AcBbCa   cylinders |1|1.79298|
Moduli: |6.99133|1| Min Area: 1 Max Area: 1
    total: 0.020944 0.0127324 sym = 6
    raw = 24 short saddle = 12.1131 short cyl = 15.2753 tag #3
V30: AcBbCa   cylinders |1|3.79298|
Moduli: |7.20393|1| Min Area: 1 Max Area: 1
    total: 0.0139626 0.00848826 sym = 6
    raw = 16 short saddle = 28.1663 short cyl = 28.1663 tag #4
V30: AaBcCb   cylinders |1|1.26106|
Moduli: |1|3.88504| Min Area: 1 Max Area: 1
    total: 0.010472 0.0063662 sym = 6
    raw = 12 short saddle = 2 short cyl = 7.66098 tag #2
V30: AcBbCa   cylinders |1|2.83049|
Moduli: |5.18117|1| Min Area: 1 Max Area: 1
    total: 0.010472 0.0063662 sym = 6
    raw = 12 short saddle = 1.41421 short cyl = 1.41421 tag #5

total cylinders: 1.68075 1.02177 raw = 321 rejected = 0
""",
    (1, 2, 2, 5): r"""
max loc = 3
alpha =  5 beta = 1 gamma = 2 delta =2
a =  3.14159 b = 0.628319 c= 1.25664 d=1.25664
a1_min = 1.25664 a1_max = 2.51327
a1 = 2.11327
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 1.50939 p-q = 2.19926
p = (1,0) q = (-0.779239,1.29269) r = (-1.19926,1.43518e-16)
Imaginary of t_cx = -1.43518e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (1.11022e-16,0)
Checking r_alt (0,-1.43518e-16)
Checking p_algt (0,0)
Checking q_algt (1.11022e-16,0)
Checking r_algt (0,-1.43518e-16)
Number field: min poly (1 -1 1 -1 1)
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.1992598512654478
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-1.779238594720266464221936,1.292692506532768126348287)
E2 ( V2 V3 F0 NULL ) (-0.420021256545181298491798,-1.292692506532768126348287)
E3 ( V3 V0 F0 NULL ) (1.199259851265447762713734,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-1 1 -1 1)+(-1 1 -1 1)t1
#E2: (0 -1 1 -1)+(0 -1 1 -1)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.4, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.4
In RetriangulateSurface
Removing V0
F21 V3(E10) V36(E6) a=0, can_bisect=0
F21 V3(E10) V1(E1) a=0.628319, can_bisect=1
F21 V3(E10) V36(E2) a=0, can_bisect=0
F21 V36(E6) V1(E1) a=0, can_bisect=0
F21 V36(E6) V36(E2) a=0.314159, can_bisect=1
F21 V1(E1) V36(E2) a=0, can_bisect=0
F21 best_angle = 0.628319
Removing V1
F24 V36(E25) V22(E47) a=0, can_bisect=0
F24 V36(E25) V36(E43) a=1.34248, can_bisect=0
F24 V36(E25) V17(E17) a=0.0375808, can_bisect=1
F24 V36(E25) V36(E31) a=0.0858407, can_bisect=1
F24 V36(E25) V30(E53) a=0.4, can_bisect=1
F24 V36(E25) V36(E37) a=0.0858407, can_bisect=1
F24 V36(E25) V10(E11) a=0.762419, can_bisect=1
F24 V36(E25) V36(E2) a=0.314159, can_bisect=1
F24 V36(E25) V3(E10) a=0, can_bisect=0
F24 V22(E47) V36(E43) a=0, can_bisect=0
F24 V22(E47) V17(E17) a=1.1708, can_bisect=1
F24 V22(E47) V36(E31) a=0.0375808, can_bisect=1
F24 V22(E47) V30(E53) a=1.79911, can_bisect=1
F24 V22(E47) V36(E37) a=0.4, can_bisect=1
F24 V22(E47) V10(E11) a=1.79911, can_bisect=1
F24 V22(E47) V36(E2) a=0.762419, can_bisect=1
F24 V22(E47) V3(E10) a=0.694475, can_bisect=1
F24 V36(E43) V17(E17) a=0, can_bisect=0
F24 V36(E43) V36(E31) a=1.34248, can_bisect=0
F24 V36(E43) V30(E53) a=0.0375808, can_bisect=1
F24 V36(E43) V36(E37) a=0.0858407, can_bisect=1
F24 V36(E43) V10(E11) a=0.4, can_bisect=1
F24 V36(E43) V36(E2) a=0.0858407, can_bisect=1
F24 V36(E43) V3(E10) a=0.628319, can_bisect=0
F24 V17(E17) V36(E31) a=0, can_bisect=0
F24 V17(E17) V30(E53) a=1.1708, can_bisect=1
F24 V17(E17) V36(E37) a=0.0375808, can_bisect=1
F24 V17(E17) V10(E11) a=1.79911, can_bisect=1
F24 V17(E17) V36(E2) a=0.4, can_bisect=1
F24 V17(E17) V3(E10) a=1.06368, can_bisect=1
F24 V36(E31) V30(E53) a=0, can_bisect=0
F24 V36(E31) V36(E37) a=1.34248, can_bisect=0
F24 V36(E31) V10(E11) a=0.0375808, can_bisect=1
F24 V36(E31) V36(E2) a=0.0858407, can_bisect=1
F24 V36(E31) V3(E10) a=0.4, can_bisect=1
F24 V30(E53) V36(E37) a=0, can_bisect=0
F24 V30(E53) V10(E11) a=1.1708, can_bisect=1
F24 V30(E53) V36(E2) a=0.0375808, can_bisect=1
F24 V30(E53) V3(E10) a=1.06368, can_bisect=1
F24 V36(E37) V10(E11) a=0, can_bisect=0
F24 V36(E37) V36(E2) a=1.34248, can_bisect=0
F24 V36(E37) V3(E10) a=0.628319, can_bisect=0
F24 V10(E11) V36(E2) a=0, can_bisect=0
F24 V10(E11) V3(E10) a=0.694475, can_bisect=1
F24 V36(E2) V3(E10) a=0, can_bisect=0
F24 best_angle = 1.79911
F26 V30(E53) V36(E37) a=0, can_bisect=0
F26 V30(E53) V10(E11) a=1.1708, can_bisect=1
F26 V30(E53) V36(E2) a=0.0375808, can_bisect=1
F26 V30(E53) V3(E10) a=0.749519, can_bisect=1
F26 V30(E53) V36(E25) a=0.314159, can_bisect=1
F26 V30(E53) V22(E60) a=0, can_bisect=0
F26 V36(E37) V10(E11) a=0, can_bisect=0
F26 V36(E37) V36(E2) a=1.34248, can_bisect=0
F26 V36(E37) V3(E10) a=0.628319, can_bisect=0
F26 V36(E37) V36(E25) a=0.0858407, can_bisect=1
F26 V36(E37) V22(E60) a=0.314159, can_bisect=1
F26 V10(E11) V36(E2) a=0, can_bisect=0
F26 V10(E11) V3(E10) a=0.694475, can_bisect=1
F26 V10(E11) V36(E25) a=0.762419, can_bisect=1
F26 V10(E11) V22(E60) a=0.628319, can_bisect=1
F26 V36(E2) V3(E10) a=0, can_bisect=0
F26 V36(E2) V36(E25) a=0.314159, can_bisect=1
F26 V36(E2) V22(E60) a=0.762419, can_bisect=1
F26 V3(E10) V36(E25) a=0, can_bisect=0
F26 V3(E10) V22(E60) a=0.694475, can_bisect=1
F26 V36(E25) V22(E60) a=0, can_bisect=0
F26 best_angle = 1.1708
F28 V10(E11) V36(E2) a=0, can_bisect=0
F28 V10(E11) V3(E10) a=0.694475, can_bisect=1
F28 V10(E11) V36(E25) a=0.762419, can_bisect=1
F28 V10(E11) V22(E60) a=0.628319, can_bisect=1
F28 V10(E11) V30(E61) a=0, can_bisect=0
F28 V36(E2) V3(E10) a=0, can_bisect=0
F28 V36(E2) V36(E25) a=0.314159, can_bisect=1
F28 V36(E2) V22(E60) a=0.762419, can_bisect=1
F28 V36(E2) V30(E61) a=0.0375808, can_bisect=1
F28 V3(E10) V36(E25) a=0, can_bisect=0
F28 V3(E10) V22(E60) a=0.694475, can_bisect=1
F28 V3(E10) V30(E61) a=0.507118, can_bisect=1
F28 V36(E25) V22(E60) a=0, can_bisect=0
F28 V36(E25) V30(E61) a=0.314159, can_bisect=1
F28 V22(E60) V30(E61) a=0, can_bisect=0
F28 best_angle = 0.762419
F30 V36(E25) V22(E60) a=0, can_bisect=0
F30 V36(E25) V30(E61) a=0.314159, can_bisect=1
F30 V36(E25) V10(E62) a=0, can_bisect=0
F30 V22(E60) V30(E61) a=0, can_bisect=0
F30 V22(E60) V10(E62) a=0.580059, can_bisect=1
F30 V30(E61) V10(E62) a=0, can_bisect=0
F30 best_angle = 0.580059
F29 V10(E11) V36(E2) a=0, can_bisect=0
F29 V10(E11) V3(E10) a=0.524581, can_bisect=1
F29 V10(E11) V36(E62) a=0, can_bisect=0
F29 V36(E2) V3(E10) a=0, can_bisect=0
F29 V36(E2) V36(E62) a=0.314159, can_bisect=1
F29 V3(E10) V36(E62) a=0, can_bisect=0
F29 best_angle = 0.524581
F25 V22(E47) V36(E43) a=0, can_bisect=0
F25 V22(E47) V17(E17) a=0.628319, can_bisect=1
F25 V22(E47) V36(E31) a=0.0375808, can_bisect=1
F25 V22(E47) V30(E60) a=0, can_bisect=0
F25 V36(E43) V17(E17) a=0, can_bisect=0
F25 V36(E43) V36(E31) a=1.34248, can_bisect=0
F25 V36(E43) V30(E60) a=0.0375808, can_bisect=1
F25 V17(E17) V36(E31) a=0, can_bisect=0
F25 V17(E17) V30(E60) a=0.628319, can_bisect=1
F25 V36(E31) V30(E60) a=0, can_bisect=0
F25 best_angle = 0.628319
F36 V30(E60) V22(E47) a=0, can_bisect=0
F36 V30(E60) V36(E43) a=0.0375808, can_bisect=1
F36 V30(E60) V17(E65) a=0, can_bisect=0
F36 V22(E47) V36(E43) a=0, can_bisect=0
F36 V22(E47) V17(E65) a=0.628319, can_bisect=1
F36 V36(E43) V17(E65) a=0, can_bisect=0
F36 best_angle = 0.628319
Removing V10
F39 V36(E25) V22(E60) a=0, can_bisect=0
F39 V36(E25) V30(E53) a=0.314159, can_bisect=1
F39 V36(E25) V36(E38) a=0.714159, can_bisect=1
F39 V36(E25) V3(E16) a=0.628319, can_bisect=1
F39 V36(E25) V36(E2) a=0.314159, can_bisect=1
F39 V36(E25) V3(E10) a=0, can_bisect=0
F39 V22(E60) V30(E53) a=0, can_bisect=0
F39 V22(E60) V36(E38) a=0.314159, can_bisect=1
F39 V22(E60) V3(E16) a=0.749519, can_bisect=1
F39 V22(E60) V36(E2) a=0.894218, can_bisect=1
F39 V22(E60) V3(E10) a=0.694475, can_bisect=1
F39 V30(E53) V36(E38) a=0, can_bisect=0
F39 V30(E53) V3(E16) a=0.694475, can_bisect=1
F39 V30(E53) V36(E2) a=0.894218, can_bisect=1
F39 V30(E53) V3(E10) a=0.749519, can_bisect=1
F39 V36(E38) V3(E16) a=0, can_bisect=0
F39 V36(E38) V36(E2) a=0.314159, can_bisect=1
F39 V36(E38) V3(E10) a=0.628319, can_bisect=1
F39 V3(E16) V36(E2) a=0, can_bisect=0
F39 V3(E16) V3(E10) a=0.314159, can_bisect=1
F39 V36(E2) V3(E10) a=0, can_bisect=0
F39 best_angle = 0.894218
F41 V36(E2) V3(E10) a=0, can_bisect=0
F41 V36(E2) V36(E25) a=0.314159, can_bisect=1
F41 V36(E2) V22(E67) a=0, can_bisect=0
F41 V3(E10) V36(E25) a=0, can_bisect=0
F41 V3(E10) V22(E67) a=0.524581, can_bisect=1
F41 V36(E25) V22(E67) a=0, can_bisect=0
F41 best_angle = 0.524581
F40 V22(E60) V30(E53) a=0, can_bisect=0
F40 V22(E60) V36(E38) a=0.314159, can_bisect=1
F40 V22(E60) V3(E16) a=0.458858, can_bisect=1
F40 V22(E60) V36(E67) a=0, can_bisect=0
F40 V30(E53) V36(E38) a=0, can_bisect=0
F40 V30(E53) V3(E16) a=0.694475, can_bisect=1
F40 V30(E53) V36(E67) a=0.724838, can_bisect=1
F40 V36(E38) V3(E16) a=0, can_bisect=0
F40 V36(E38) V36(E67) a=0.314159, can_bisect=1
F40 V3(E16) V36(E67) a=0, can_bisect=0
F40 best_angle = 0.724838
F44 V30(E53) V36(E38) a=0, can_bisect=0
F44 V30(E53) V3(E16) a=0.524581, can_bisect=1
F44 V30(E53) V36(E69) a=0, can_bisect=0
F44 V36(E38) V3(E16) a=0, can_bisect=0
F44 V36(E38) V36(E69) a=0.314159, can_bisect=1
F44 V3(E16) V36(E69) a=0, can_bisect=0
F44 best_angle = 0.524581
Removing V17
F48 V36(E16) V3(E2) a=0, can_bisect=0
F48 V36(E16) V36(E31) a=0.314159, can_bisect=1
F48 V36(E16) V30(E60) a=0.580059, can_bisect=1
F48 V36(E16) V22(E47) a=0, can_bisect=0
F48 V3(E2) V36(E31) a=0, can_bisect=0
F48 V3(E2) V30(E60) a=0.694475, can_bisect=1
F48 V3(E2) V22(E47) a=0.694475, can_bisect=1
F48 V36(E31) V30(E60) a=0, can_bisect=0
F48 V36(E31) V22(E47) a=0.580059, can_bisect=1
F48 V30(E60) V22(E47) a=0, can_bisect=0
F48 best_angle = 0.694475
F50 V30(E60) V22(E47) a=0, can_bisect=0
F50 V30(E60) V36(E16) a=0.524581, can_bisect=1
F50 V30(E60) V3(E71) a=0, can_bisect=0
F50 V22(E47) V36(E16) a=0, can_bisect=0
F50 V22(E47) V3(E71) a=0.694475, can_bisect=1
F50 V36(E16) V3(E71) a=0, can_bisect=0
F50 best_angle = 0.694475
Removing V22
F53 V30(E69) V36(E2) a=0, can_bisect=0
F53 V30(E69) V3(E10) a=0.458858, can_bisect=1
F53 V30(E69) V36(E26) a=0.894218, can_bisect=1
F53 V30(E69) V3(E38) a=0.983439, can_bisect=1
F53 V30(E69) V36(E16) a=0.524581, can_bisect=1
F53 V30(E69) V3(E71) a=0, can_bisect=0
F53 V36(E2) V3(E10) a=0, can_bisect=0
F53 V36(E2) V36(E26) a=0.314159, can_bisect=1
F53 V36(E2) V3(E38) a=0.628319, can_bisect=1
F53 V36(E2) V36(E16) a=0.676578, can_bisect=1
F53 V36(E2) V3(E71) a=0.362419, can_bisect=1
F53 V3(E10) V36(E26) a=0, can_bisect=0
F53 V3(E10) V3(E38) a=0.314159, can_bisect=1
F53 V3(E10) V36(E16) a=0.628319, can_bisect=1
F53 V3(E10) V3(E71) a=0.780316, can_bisect=1
F53 V36(E26) V3(E38) a=0, can_bisect=0
F53 V36(E26) V36(E16) a=0.314159, can_bisect=1
F53 V36(E26) V3(E71) a=0.628319, can_bisect=1
F53 V3(E38) V36(E16) a=0, can_bisect=0
F53 V3(E38) V3(E71) a=0.314159, can_bisect=1
F53 V36(E16) V3(E71) a=0, can_bisect=0
F53 best_angle = 0.983439
F55 V3(E38) V36(E16) a=0, can_bisect=0
F55 V3(E38) V3(E71) a=0.314159, can_bisect=1
F55 V3(E38) V30(E73) a=0, can_bisect=0
F55 V36(E16) V3(E71) a=0, can_bisect=0
F55 V36(E16) V30(E73) a=0.458858, can_bisect=1
F55 V3(E71) V30(E73) a=0, can_bisect=0
F55 best_angle = 0.458858
F54 V30(E69) V36(E2) a=0, can_bisect=0
F54 V30(E69) V3(E10) a=0.458858, can_bisect=1
F54 V30(E69) V36(E26) a=0.43536, can_bisect=1
F54 V30(E69) V3(E73) a=0, can_bisect=0
F54 V36(E2) V3(E10) a=0, can_bisect=0
F54 V36(E2) V36(E26) a=0.314159, can_bisect=1
F54 V36(E2) V3(E73) a=0.628319, can_bisect=1
F54 V3(E10) V36(E26) a=0, can_bisect=0
F54 V3(E10) V3(E73) a=0.314159, can_bisect=1
F54 V36(E26) V3(E73) a=0, can_bisect=0
F54 best_angle = 0.628319
F58 V36(E2) V3(E10) a=0, can_bisect=0
F58 V36(E2) V36(E26) a=0.314159, can_bisect=1
F58 V36(E2) V3(E75) a=0, can_bisect=0
F58 V3(E10) V36(E26) a=0, can_bisect=0
F58 V3(E10) V3(E75) a=0.314159, can_bisect=1
F58 V36(E26) V3(E75) a=0, can_bisect=0
F58 best_angle = 0.314159
Removing V30
F62 V3(E2) V36(E10) a=0, can_bisect=0
F62 V3(E2) V3(E26) a=0.314159, can_bisect=1
F62 V3(E2) V36(E38) a=0.628319, can_bisect=1
F62 V3(E2) V3(E16) a=0.942478, can_bisect=1
F62 V3(E2) V36(E75) a=0.628319, can_bisect=1
F62 V3(E2) V3(E38) a=0.314159, can_bisect=1
F62 V3(E2) V36(E16) a=0, can_bisect=0
F62 V36(E10) V3(E26) a=0, can_bisect=0
F62 V36(E10) V36(E38) a=0.314159, can_bisect=1
F62 V36(E10) V3(E16) a=0.628319, can_bisect=1
F62 V36(E10) V36(E75) a=0.942478, can_bisect=1
F62 V36(E10) V3(E38) a=0.628319, can_bisect=1
F62 V36(E10) V36(E16) a=0.314159, can_bisect=1
F62 V3(E26) V36(E38) a=0, can_bisect=0
F62 V3(E26) V3(E16) a=0.314159, can_bisect=1
F62 V3(E26) V36(E75) a=0.628319, can_bisect=1
F62 V3(E26) V3(E38) a=0.942478, can_bisect=1
F62 V3(E26) V36(E16) a=0.628319, can_bisect=1
F62 V36(E38) V3(E16) a=0, can_bisect=0
F62 V36(E38) V36(E75) a=0.314159, can_bisect=1
F62 V36(E38) V3(E38) a=0.628319, can_bisect=1
F62 V36(E38) V36(E16) a=0.942478, can_bisect=1
F62 V3(E16) V36(E75) a=0, can_bisect=0
F62 V3(E16) V3(E38) a=0.314159, can_bisect=1
F62 V3(E16) V36(E16) a=1.25664, can_bisect=1
F62 V36(E75) V3(E38) a=0, can_bisect=0
F62 V36(E75) V36(E16) a=0.314159, can_bisect=1
F62 V3(E38) V36(E16) a=0, can_bisect=0
F62 best_angle = 1.25664
F64 V36(E16) V3(E2) a=0, can_bisect=0
F64 V36(E16) V36(E10) a=0.314159, can_bisect=1
F64 V36(E16) V3(E26) a=0.628319, can_bisect=1
F64 V36(E16) V36(E38) a=0.314159, can_bisect=1
F64 V36(E16) V3(E77) a=0, can_bisect=0
F64 V3(E2) V36(E10) a=0, can_bisect=0
F64 V3(E2) V3(E26) a=0.314159, can_bisect=1
F64 V3(E2) V36(E38) a=0.628319, can_bisect=1
F64 V3(E2) V3(E77) a=0.314159, can_bisect=1
F64 V36(E10) V3(E26) a=0, can_bisect=0
F64 V36(E10) V36(E38) a=0.314159, can_bisect=1
F64 V36(E10) V3(E77) a=0.628319, can_bisect=1
F64 V3(E26) V36(E38) a=0, can_bisect=0
F64 V3(E26) V3(E77) a=0.314159, can_bisect=1
F64 V36(E38) V3(E77) a=0, can_bisect=0
F64 best_angle = 0.628319
F66 V3(E26) V36(E38) a=0, can_bisect=0
F66 V3(E26) V3(E77) a=0.314159, can_bisect=1
F66 V3(E26) V36(E78) a=0, can_bisect=0
F66 V36(E38) V3(E77) a=0, can_bisect=0
F66 V36(E38) V36(E78) a=0.314159, can_bisect=1
F66 V3(E77) V36(E78) a=0, can_bisect=0
F66 best_angle = 0.314159
F65 V36(E16) V3(E2) a=0, can_bisect=0
F65 V36(E16) V36(E10) a=0.314159, can_bisect=1
F65 V36(E16) V3(E78) a=0, can_bisect=0
F65 V3(E2) V36(E10) a=0, can_bisect=0
F65 V3(E2) V3(E78) a=0.314159, can_bisect=1
F65 V36(E10) V3(E78) a=0, can_bisect=0
F65 best_angle = 0.314159
F63 V3(E16) V36(E75) a=0, can_bisect=0
F63 V3(E16) V3(E38) a=0.314159, can_bisect=1
F63 V3(E16) V36(E77) a=0, can_bisect=0
F63 V36(E75) V3(E38) a=0, can_bisect=0
F63 V36(E75) V36(E77) a=0.314159, can_bisect=1
F63 V3(E38) V36(E77) a=0, can_bisect=0
F63 best_angle = 0.314159
Longest Edge: 4.39852
Looking for shortest saddle...1.35922
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.1992598512654478
# Vertices: V3 V36
# UEdges: 
E2 ( V36 V3 F60 F69 ) (-0.420021256545181298491798,-1.292692506532768126348287)
E10 ( V3 V36 F60 F70 ) (0.420021256545181242980647,-1.292692506532768126348287)
E16 ( V3 V36 F71 F69 ) (-1.099629925632723992379169,-0.798927906039546331662393)
E26 ( V36 V3 F61 F67 ) (1.099629925632723992379169,-0.798927906039546331662393)
E38 ( V36 V3 F67 F72 ) (-1.359217338175085387774743,-0.000000000000000000000000)
E75 ( V36 V3 F71 F61 ) (1.099629925632723992379169,-3.384312919105082695381270)
E76 ( V36 V36 F61 F60 ) (-0.000000000000000055511151,-2.585385013065536252696575)
E77 ( V3 V36 F68 F72 ) (1.359217338175085387774743,-4.183240825144628693976756)
E78 ( V36 V3 F68 F70 ) (1.099629925632723992379169,3.384312919105082251292060)
E79 ( V3 V3 F68 F67 ) (-2.458847263807809380153913,0.798927906039546331662393)
E80 ( V36 V36 F70 F69 ) (1.519651182177905290870967,2.091620412572314346988378)
E81 ( V3 V3 F72 F71 ) (0.000000000000000000000000,-4.183240825144628693976756)
# Faces: 
F60 +E2 +E10 -E76
F61 +E26 -E75 +E76
F67 -E26 +E38 -E79
F68 +E77 +E78 +E79
F69 -E16 -E2 -E80
F70 -E10 -E78 +E80
F71 +E16 +E75 -E81
F72 -E38 -E77 +E81
#UEdges Algebraic 
#E2: (0 -1 1 -1)+(0 -1 1 -1)t1
#E10: (1 -1 0 0)+(1 -1 0 0)t1
#E16: (-1 1 -1 0)+(-1 1 -1 0)t1
#E26: (0 1 -1 0)+(0 1 -1 0)t1
#E38: (0 0 -1 1)+(0 0 -1 1)t1
#E75: (1 -1 0 -1)+(1 -1 0 -1)t1
#E76: (1 -2 1 -1)+(1 -2 1 -1)t1
#E77: (0 0 0 -2)+(0 0 0 -2)t1
#E78: (0 1 0 1)+(0 1 0 1)t1
#E79: (0 -1 0 1)+(0 -1 0 1)t1
#E80: (1 0 0 1)+(1 0 0 1)t1
#E81: (0 0 -1 -1)+(0 0 -1 -1)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V3(12) (4 PI) V36(12) (4 PI) 
# edges: 12
## E2(2) E10(2) E16(2) E26(2) E38(2) E75(2) E76(2) E77(2) E78(2) E79(2) E80(2) E81(2) 
# faces: 8
# genus: 2
# area: 14.2148
sweeping start V3 depth = 37.7026
File =  depth = 37.7026 follow_depth = 188.513 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 42 unusual 0 close 157671 reject 0
V3: AdBc V36: aCbD   cylinders |1|1.61803|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.72918 1.05122 sym = 4
    raw = 1321 short saddle = 1.35922 short cyl = 4.91769 tag #0
V3: AaBb V36: CdDc   cylinders |1|1.61803|2.61803|
Moduli: |1|2|1| Min Area: 1 Max Area: 1
    total: 0.916298 0.557042 sym = 4
    raw = 700 short saddle = 2.58539 short cyl = 2.58539 tag #1

total cylinders: 6.20465 3.77197 raw = 1185 rejected = 0
""",
    (1, 3, 2, 2): r"""
max loc = 1
alpha =  3 beta = 2 gamma = 2 delta =1
a =  2.35619 b = 1.5708 c= 1.5708 d=0.785398
a1_min = 0 a1_max = 1.5708
a1 = 1.0708
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 1.11022e-16
p = 1 q = 2.08583 p-q = 1.83049
p = (1,0) q = (1,1.83049) r = (-1.83049,1.83049)
Imaginary of t_cx = -4.44089e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(0 0 0 1)t1
Checking p_alt (0,0)
Checking q_alt (4.44089e-16,0)
Checking r_alt (4.44089e-16,2.22045e-16)
Checking p_algt (0,0)
Checking q_algt (4.44089e-16,0)
Checking r_algt (4.44089e-16,2.22045e-16)
Number field: min poly (1 0 0 0 1)
#Field Degree=4 minimal polynomial=(1 0 0 0 1)
#Field generator embedding: (0.707106781186547570.70710678118654746
#Params:1
#t1=2.5887005618031762
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000222044605,1.830487721712451332223281)
E2 ( V2 V3 F0 NULL ) (-2.830487721712450888134072,0.000000000000000222044605)
E3 ( V3 V0 F0 NULL ) (1.830487721712450666089467,-1.830487721712451554267886)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 0 0 0)+(0 1/2 0 1/2)t1
#E2: (-1 0 0 0)+(0 -1/2 0 1/2)t1
#E3: (0 0 0 0)+(0 0 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.5, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.211343, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.5
In RetriangulateSurface
Removing V1
F17 V10(E5) V2(E25) a=0, can_bisect=0
F17 V10(E5) V10(E11) a=1.0708, can_bisect=1
F17 V10(E5) V2(E4) a=0, can_bisect=0
F17 V2(E25) V10(E11) a=0, can_bisect=0
F17 V2(E25) V2(E4) a=0.5, can_bisect=1
F17 V10(E11) V2(E4) a=0, can_bisect=0
F17 best_angle = 1.0708
Removing V2
F20 V10(E15) V15(E27) a=0, can_bisect=0
F20 V10(E15) V10(E47) a=0.785398, can_bisect=1
F20 V10(E15) V10(E9) a=1.0708, can_bisect=1
F20 V10(E15) V15(E3) a=0.339608, can_bisect=1
F20 V10(E15) V10(E47) a=0, can_bisect=0
F20 V15(E27) V10(E47) a=0, can_bisect=0
F20 V15(E27) V10(E9) a=0.339608, can_bisect=1
F20 V15(E27) V15(E3) a=0.785398, can_bisect=1
F20 V15(E27) V10(E47) a=0.339608, can_bisect=1
F20 V10(E47) V10(E9) a=0, can_bisect=0
F20 V10(E47) V15(E3) a=0.339608, can_bisect=1
F20 V10(E47) V10(E47) a=1.0708, can_bisect=1
F20 V10(E9) V15(E3) a=0, can_bisect=0
F20 V10(E9) V10(E47) a=0.785398, can_bisect=1
F20 V15(E3) V10(E47) a=0, can_bisect=0
F20 best_angle = 1.0708
F22 V10(E9) V15(E3) a=0, can_bisect=0
F22 V10(E9) V10(E47) a=0.5, can_bisect=1
F22 V10(E9) V10(E48) a=0, can_bisect=0
F22 V15(E3) V10(E47) a=0, can_bisect=0
F22 V15(E3) V10(E48) a=0.339608, can_bisect=1
F22 V10(E47) V10(E48) a=0, can_bisect=0
F22 best_angle = 0.5
F21 V10(E15) V15(E27) a=0, can_bisect=0
F21 V10(E15) V10(E47) a=0.5, can_bisect=1
F21 V10(E15) V10(E48) a=0, can_bisect=0
F21 V15(E27) V10(E47) a=0, can_bisect=0
F21 V15(E27) V10(E48) a=0.339608, can_bisect=1
F21 V10(E47) V10(E48) a=0, can_bisect=0
F21 best_angle = 0.5
Removing V15
F27 V10(E31) V28(E41) a=0, can_bisect=0
F27 V10(E31) V10(E50) a=0.5, can_bisect=1
F27 V10(E31) V10(E37) a=0.785398, can_bisect=1
F27 V10(E31) V28(E17) a=0.37422, can_bisect=1
F27 V10(E31) V10(E49) a=0, can_bisect=0
F27 V28(E41) V10(E50) a=0, can_bisect=0
F27 V28(E41) V10(E37) a=0.37422, can_bisect=1
F27 V28(E41) V28(E17) a=1.0708, can_bisect=1
F27 V28(E41) V10(E49) a=0.37422, can_bisect=1
F27 V10(E50) V10(E37) a=0, can_bisect=0
F27 V10(E50) V28(E17) a=0.37422, can_bisect=1
F27 V10(E50) V10(E49) a=0.785398, can_bisect=1
F27 V10(E37) V28(E17) a=0, can_bisect=0
F27 V10(E37) V10(E49) a=0.5, can_bisect=1
F27 V28(E17) V10(E49) a=0, can_bisect=0
F27 best_angle = 1.0708
F29 V28(E17) V10(E49) a=0, can_bisect=0
F29 V28(E17) V10(E31) a=0.37422, can_bisect=1
F29 V28(E17) V28(E51) a=0, can_bisect=0
F29 V10(E49) V10(E31) a=0, can_bisect=0
F29 V10(E49) V28(E51) a=0.37422, can_bisect=1
F29 V10(E31) V28(E51) a=0, can_bisect=0
F29 best_angle = 0.37422
F28 V28(E41) V10(E50) a=0, can_bisect=0
F28 V28(E41) V10(E37) a=0.37422, can_bisect=1
F28 V28(E41) V28(E51) a=0, can_bisect=0
F28 V10(E50) V10(E37) a=0, can_bisect=0
F28 V10(E50) V28(E51) a=0.37422, can_bisect=1
F28 V10(E37) V28(E51) a=0, can_bisect=0
F28 best_angle = 0.37422
Flipping E51
Removing V21
F36 V28(E31) V10(E41) a=0, can_bisect=0
F36 V28(E31) V28(E37) a=0.5, can_bisect=1
F36 V28(E31) V10(E17) a=0, can_bisect=0
F36 V10(E41) V28(E37) a=0, can_bisect=0
F36 V10(E41) V10(E17) a=1.0708, can_bisect=1
F36 V28(E37) V10(E17) a=0, can_bisect=0
F36 best_angle = 1.0708
Removing V28
F39 V10(E50) V10(E54) a=0, can_bisect=0
F39 V10(E50) V10(E55) a=0.785398, can_bisect=1
F39 V10(E50) V10(E49) a=1.0708, can_bisect=1
F39 V10(E50) V10(E54) a=0.211343, can_bisect=1
F39 V10(E50) V10(E55) a=0, can_bisect=0
F39 V10(E54) V10(E55) a=0, can_bisect=0
F39 V10(E54) V10(E49) a=0.211343, can_bisect=1
F39 V10(E54) V10(E54) a=0.37422, can_bisect=1
F39 V10(E54) V10(E55) a=0, can_bisect=1
F39 V10(E55) V10(E49) a=0, can_bisect=0
F39 V10(E55) V10(E54) a=0, can_bisect=0
F39 V10(E55) V10(E55) a=1.0708, can_bisect=1
F39 V10(E49) V10(E54) a=0, can_bisect=0
F39 V10(E49) V10(E55) a=0.785398, can_bisect=1
F39 V10(E54) V10(E55) a=0, can_bisect=0
F39 best_angle = 1.0708
F41 V10(E55) V10(E50) a=0, can_bisect=0
F41 V10(E55) V10(E54) a=0, can_bisect=1
F41 V10(E55) V10(E56) a=0, can_bisect=0
F41 V10(E50) V10(E54) a=0, can_bisect=0
F41 V10(E50) V10(E56) a=0.5, can_bisect=1
F41 V10(E54) V10(E56) a=0, can_bisect=0
F41 best_angle = 0.5
F40 V10(E55) V10(E49) a=0, can_bisect=0
F40 V10(E55) V10(E54) a=0, can_bisect=0
F40 V10(E55) V10(E56) a=0, can_bisect=0
F40 V10(E49) V10(E54) a=0, can_bisect=0
F40 V10(E49) V10(E56) a=0.5, can_bisect=1
F40 V10(E54) V10(E56) a=0, can_bisect=0
F40 best_angle = 0.5
Longest Edge: 5.1774
Looking for shortest saddle...2
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 0 0 1)
#Field generator embedding: (0.707106781186547570.70710678118654746
#Params:1
#t1=2.5887005618031762
# Vertices: V10
# UEdges: 
E47 ( V10 V10 F24 F26 ) (2.000000000000000000000000,0.000000000000000000000000)
E48 ( V10 V10 F24 F26 ) (-1.999999999999999333866185,3.660975443424902664446563)
E49 ( V10 V10 F24 F44 ) (-0.000000000000000666133815,-3.660975443424902664446563)
E50 ( V10 V10 F26 F42 ) (0.000000000000000666133815,3.660975443424902664446563)
E54 ( V10 V10 F44 F42 ) (-3.660975443424902664446563,-3.660975443424902220357353)
E55 ( V10 V10 F45 F43 ) (0.000000000000000444089210,2.000000000000000000000000)
E56 ( V10 V10 F43 F45 ) (-3.660975443424901776268143,2.000000000000000444089210)
E57 ( V10 V10 F43 F42 ) (3.660975443424901776268143,-0.000000000000000444089210)
E58 ( V10 V10 F45 F44 ) (-3.660975443424901776268143,0.000000000000000444089210)
# Faces: 
F24 +E47 +E48 +E49
F26 -E47 -E48 +E50
F42 -E50 -E54 -E57
F43 +E56 -E55 +E57
F44 -E49 +E54 -E58
F45 -E56 +E55 +E58
#UEdges Algebraic 
#E47: (2 0 0 0)+(0 0 0 0)t1
#E48: (-2 0 0 0)+(0 1 0 1)t1
#E49: (0 0 0 0)+(0 -1 0 -1)t1
#E50: (0 0 0 0)+(0 1 0 1)t1
#E54: (0 0 0 0)+(0 -2 0 0)t1
#E55: (0 0 2 0)+(0 0 0 0)t1
#E56: (0 0 2 0)+(0 -1 0 1)t1
#E57: (0 0 0 0)+(0 1 0 -1)t1
#E58: (0 0 0 0)+(0 -1 0 1)t1
# File =  perturb = 0 rescale = 1
# vertices: 1
## V10(18) (6 PI) 
# edges: 9
## E47(2) E48(2) E49(2) E50(2) E54(2) E55(2) E56(2) E57(2) E58(2) 
# faces: 6
# genus: 2
# area: 28.0466
sweeping start V10 depth = 52.9591
File =  depth = 52.9591 follow_depth = 264.795 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 12 unusual 0 close 41673 reject 0
V10: AXBbXa   cylinders |1|
Moduli: |1| Min Area: 0.261063 Max Area: 0.771668
    total: 1.63363 0.993127 sym = 4
    raw = 1248 short saddle = 6.74162 short cyl = 6.74162 tag #2
V10: AbBaCc   cylinders |1|1.62217|
Moduli: |6.3253|1| Min Area: 1 Max Area: 1
    total: 0.0349066 0.0212207 sym = 6
    raw = 40 short saddle = 20.8445 short cyl = 33.5028 tag #3
V10: AbBaCc   cylinders |1|1.6467|
Moduli: |1.36757|1| Min Area: 1 Max Area: 1
    total: 0.0113446 0.00689671 sym = 6
    raw = 13 short saddle = 5.1774 short cyl = 8.00583 tag #0
V10: AaBcCb   cylinders |1|1.5463|
Moduli: |1|1.18378| Min Area: 1 Max Area: 1
    total: 0.010472 0.0063662 sym = 6
    raw = 12 short saddle = 2 short cyl = 3.66098 tag #4

total cylinders: 1.75406 1.06634 raw = 335 rejected = 0
""",
    (1, 4, 3, 4): r"""
max loc = 1
alpha =  4 beta = 3 gamma = 4 delta =1
a =  2.0944 b = 1.5708 c= 2.0944 d=0.523599
a1_min = -0.523599 a1_max = 1.5708
a1 = 0.90413
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -2.22045e-16
p = 1 q = 1.61716 p-q = 1.2709
p = (1,0) q = (1,1.2709) r = (-1.60063,2.77238)
Imaginary of t_cx = -1.55431e-15
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (3.33067e-16,-1.77636e-15)
Checking r_alt (0,-4.44089e-16)
Checking p_algt (0,0)
Checking q_algt (3.33067e-16,-1.77636e-15)
Checking r_algt (0,-4.44089e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=3.2012663909678238
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000222044605,1.270901743383348936333732)
E2 ( V2 V3 F0 NULL ) (-2.600633195483911475065497,1.501476275476113286799773)
E3 ( V3 V0 F0 NULL ) (1.600633195483911253020892,-2.772378018859462223133505)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (1/3 0 -2/3 0)+(-1/3 0 2/3 0)t1
#E2: (-4/3 0 2/3 0)+(-2/3 0 1/3 0)t1
#E3: (0 0 0 0)+(1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.229846, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F25 V54(E5) V5(E31) a=0, can_bisect=0
F25 V54(E5) V10(E11) a=0.90413, can_bisect=1
F25 V54(E5) V34(E4) a=0, can_bisect=0
F25 V5(E31) V10(E11) a=0, can_bisect=0
F25 V5(E31) V34(E4) a=0.666667, can_bisect=1
F25 V10(E11) V34(E4) a=0, can_bisect=0
F25 best_angle = 0.90413
Removing V21
F28 V34(E37) V54(E59) a=0, can_bisect=0
F28 V34(E37) V5(E49) a=0.666667, can_bisect=1
F28 V34(E37) V10(E17) a=0, can_bisect=0
F28 V54(E59) V5(E49) a=0, can_bisect=0
F28 V54(E59) V10(E17) a=0.90413, can_bisect=1
F28 V5(E49) V10(E17) a=0, can_bisect=0
F28 best_angle = 0.90413
Removing V27
F31 V5(E43) V10(E65) a=0, can_bisect=0
F31 V5(E43) V34(E55) a=0.666667, can_bisect=1
F31 V5(E43) V54(E23) a=0, can_bisect=0
F31 V10(E65) V34(E55) a=0, can_bisect=0
F31 V10(E65) V54(E23) a=0.90413, can_bisect=1
F31 V34(E55) V54(E23) a=0, can_bisect=0
F31 best_angle = 0.90413
Removing V32
F34 V5(E59) V54(E55) a=0, can_bisect=0
F34 V5(E59) V34(E11) a=0.380531, can_bisect=1
F34 V5(E59) V10(E49) a=0.436821, can_bisect=1
F34 V5(E59) V5(E23) a=0.90413, can_bisect=1
F34 V5(E59) V54(E4) a=0.937031, can_bisect=1
F34 V5(E59) V34(E17) a=1.42773, can_bisect=1
F34 V5(E59) V10(E43) a=0.937031, can_bisect=1
F34 V5(E59) V5(E5) a=0.90413, can_bisect=1
F34 V5(E59) V54(E37) a=0.436821, can_bisect=1
F34 V5(E59) V34(E65) a=0.380531, can_bisect=1
F34 V5(E59) V10(E31) a=0, can_bisect=0
F34 V54(E55) V34(E11) a=0, can_bisect=0
F34 V54(E55) V10(E49) a=0.143068, can_bisect=1
F34 V54(E55) V5(E23) a=0.436821, can_bisect=1
F34 V54(E55) V54(E4) a=0.666667, can_bisect=1
F34 V54(E55) V34(E17) a=0.937031, can_bisect=1
F34 V54(E55) V10(E43) a=1.19027, can_bisect=1
F34 V54(E55) V5(E5) a=0.937031, can_bisect=1
F34 V54(E55) V54(E37) a=0.666667, can_bisect=1
F34 V54(E55) V34(E65) a=0.436821, can_bisect=1
F34 V54(E55) V10(E31) a=0.143068, can_bisect=1
F34 V34(E11) V10(E49) a=0, can_bisect=0
F34 V34(E11) V5(E23) a=0.380531, can_bisect=1
F34 V34(E11) V54(E4) a=0.436821, can_bisect=1
F34 V34(E11) V34(E17) a=0.90413, can_bisect=1
F34 V34(E11) V10(E43) a=0.937031, can_bisect=1
F34 V34(E11) V5(E5) a=1.42773, can_bisect=1
F34 V34(E11) V54(E37) a=0.937031, can_bisect=1
F34 V34(E11) V34(E65) a=0.90413, can_bisect=1
F34 V34(E11) V10(E31) a=0.436821, can_bisect=1
F34 V10(E49) V5(E23) a=0, can_bisect=0
F34 V10(E49) V54(E4) a=0.143068, can_bisect=1
F34 V10(E49) V34(E17) a=0.436821, can_bisect=1
F34 V10(E49) V10(E43) a=0.666667, can_bisect=1
F34 V10(E49) V5(E5) a=0.937031, can_bisect=1
F34 V10(E49) V54(E37) a=1.19027, can_bisect=1
F34 V10(E49) V34(E65) a=0.937031, can_bisect=1
F34 V10(E49) V10(E31) a=0.666667, can_bisect=1
F34 V5(E23) V54(E4) a=0, can_bisect=0
F34 V5(E23) V34(E17) a=0.380531, can_bisect=1
F34 V5(E23) V10(E43) a=0.436821, can_bisect=1
F34 V5(E23) V5(E5) a=0.90413, can_bisect=1
F34 V5(E23) V54(E37) a=0.937031, can_bisect=1
F34 V5(E23) V34(E65) a=1.42773, can_bisect=1
F34 V5(E23) V10(E31) a=0.937031, can_bisect=1
F34 V54(E4) V34(E17) a=0, can_bisect=0
F34 V54(E4) V10(E43) a=0.143068, can_bisect=1
F34 V54(E4) V5(E5) a=0.436821, can_bisect=1
F34 V54(E4) V54(E37) a=0.666667, can_bisect=1
F34 V54(E4) V34(E65) a=0.937031, can_bisect=1
F34 V54(E4) V10(E31) a=1.19027, can_bisect=1
F34 V34(E17) V10(E43) a=0, can_bisect=0
F34 V34(E17) V5(E5) a=0.380531, can_bisect=1
F34 V34(E17) V54(E37) a=0.436821, can_bisect=1
F34 V34(E17) V34(E65) a=0.90413, can_bisect=1
F34 V34(E17) V10(E31) a=0.937031, can_bisect=1
F34 V10(E43) V5(E5) a=0, can_bisect=0
F34 V10(E43) V54(E37) a=0.143068, can_bisect=1
F34 V10(E43) V34(E65) a=0.436821, can_bisect=1
F34 V10(E43) V10(E31) a=0.666667, can_bisect=1
F34 V5(E5) V54(E37) a=0, can_bisect=0
F34 V5(E5) V34(E65) a=0.380531, can_bisect=1
F34 V5(E5) V10(E31) a=0.436821, can_bisect=1
F34 V54(E37) V34(E65) a=0, can_bisect=0
F34 V54(E37) V10(E31) a=0.143068, can_bisect=1
F34 V34(E65) V10(E31) a=0, can_bisect=0
F34 best_angle = 1.42773
F36 V34(E17) V10(E43) a=0, can_bisect=0
F36 V34(E17) V5(E5) a=0.380531, can_bisect=1
F36 V34(E17) V54(E37) a=0.436821, can_bisect=1
F36 V34(E17) V34(E65) a=0.523599, can_bisect=1
F36 V34(E17) V10(E31) a=0.270364, can_bisect=1
F36 V34(E17) V5(E74) a=0, can_bisect=0
F36 V10(E43) V5(E5) a=0, can_bisect=0
F36 V10(E43) V54(E37) a=0.143068, can_bisect=1
F36 V10(E43) V34(E65) a=0.436821, can_bisect=1
F36 V10(E43) V10(E31) a=0.666667, can_bisect=1
F36 V10(E43) V5(E74) a=0.270364, can_bisect=1
F36 V5(E5) V54(E37) a=0, can_bisect=0
F36 V5(E5) V34(E65) a=0.380531, can_bisect=1
F36 V5(E5) V10(E31) a=0.436821, can_bisect=1
F36 V5(E5) V5(E74) a=0.523599, can_bisect=1
F36 V54(E37) V34(E65) a=0, can_bisect=0
F36 V54(E37) V10(E31) a=0.143068, can_bisect=1
F36 V54(E37) V5(E74) a=0.436821, can_bisect=1
F36 V34(E65) V10(E31) a=0, can_bisect=0
F36 V34(E65) V5(E74) a=0.380531, can_bisect=1
F36 V10(E31) V5(E74) a=0, can_bisect=0
F36 best_angle = 0.666667
F38 V10(E31) V5(E74) a=0, can_bisect=0
F38 V10(E31) V34(E17) a=0.270364, can_bisect=1
F38 V10(E31) V10(E75) a=0, can_bisect=0
F38 V5(E74) V34(E17) a=0, can_bisect=0
F38 V5(E74) V10(E75) a=0.270364, can_bisect=1
F38 V34(E17) V10(E75) a=0, can_bisect=0
F38 best_angle = 0.270364
F37 V10(E43) V5(E5) a=0, can_bisect=0
F37 V10(E43) V54(E37) a=0.143068, can_bisect=1
F37 V10(E43) V34(E65) a=0.229846, can_bisect=1
F37 V10(E43) V10(E75) a=0, can_bisect=0
F37 V5(E5) V54(E37) a=0, can_bisect=0
F37 V5(E5) V34(E65) a=0.380531, can_bisect=1
F37 V5(E5) V10(E75) a=0.229846, can_bisect=1
F37 V54(E37) V34(E65) a=0, can_bisect=0
F37 V54(E37) V10(E75) a=0.143068, can_bisect=1
F37 V34(E65) V10(E75) a=0, can_bisect=0
F37 best_angle = 0.380531
F42 V34(E65) V10(E75) a=0, can_bisect=0
F42 V34(E65) V10(E43) a=0.229846, can_bisect=1
F42 V34(E65) V5(E77) a=0, can_bisect=0
F42 V10(E75) V10(E43) a=0, can_bisect=0
F42 V10(E75) V5(E77) a=0.229846, can_bisect=1
F42 V10(E43) V5(E77) a=0, can_bisect=0
F42 best_angle = 0.229846
F35 V5(E59) V54(E55) a=0, can_bisect=0
F35 V5(E59) V34(E11) a=0.380531, can_bisect=1
F35 V5(E59) V10(E49) a=0.436821, can_bisect=1
F35 V5(E59) V5(E23) a=0.523599, can_bisect=1
F35 V5(E59) V54(E4) a=0.270364, can_bisect=1
F35 V5(E59) V34(E74) a=0, can_bisect=0
F35 V54(E55) V34(E11) a=0, can_bisect=0
F35 V54(E55) V10(E49) a=0.143068, can_bisect=1
F35 V54(E55) V5(E23) a=0.436821, can_bisect=1
F35 V54(E55) V54(E4) a=0.666667, can_bisect=1
F35 V54(E55) V34(E74) a=0.270364, can_bisect=1
F35 V34(E11) V10(E49) a=0, can_bisect=0
F35 V34(E11) V5(E23) a=0.380531, can_bisect=1
F35 V34(E11) V54(E4) a=0.436821, can_bisect=1
F35 V34(E11) V34(E74) a=0.523599, can_bisect=1
F35 V10(E49) V5(E23) a=0, can_bisect=0
F35 V10(E49) V54(E4) a=0.143068, can_bisect=1
F35 V10(E49) V34(E74) a=0.436821, can_bisect=1
F35 V5(E23) V54(E4) a=0, can_bisect=0
F35 V5(E23) V34(E74) a=0.380531, can_bisect=1
F35 V54(E4) V34(E74) a=0, can_bisect=0
F35 best_angle = 0.666667
F46 V54(E4) V34(E74) a=0, can_bisect=0
F46 V54(E4) V5(E59) a=0.270364, can_bisect=1
F46 V54(E4) V54(E79) a=0, can_bisect=0
F46 V34(E74) V5(E59) a=0, can_bisect=0
F46 V34(E74) V54(E79) a=0.270364, can_bisect=1
F46 V5(E59) V54(E79) a=0, can_bisect=0
F46 best_angle = 0.270364
F45 V54(E55) V34(E11) a=0, can_bisect=0
F45 V54(E55) V10(E49) a=0.143068, can_bisect=1
F45 V54(E55) V5(E23) a=0.229846, can_bisect=1
F45 V54(E55) V54(E79) a=0, can_bisect=0
F45 V34(E11) V10(E49) a=0, can_bisect=0
F45 V34(E11) V5(E23) a=0.380531, can_bisect=1
F45 V34(E11) V54(E79) a=0.229846, can_bisect=1
F45 V10(E49) V5(E23) a=0, can_bisect=0
F45 V10(E49) V54(E79) a=0.143068, can_bisect=1
F45 V5(E23) V54(E79) a=0, can_bisect=0
F45 best_angle = 0.380531
F50 V5(E23) V54(E79) a=0, can_bisect=0
F50 V5(E23) V54(E55) a=0.229846, can_bisect=1
F50 V5(E23) V34(E81) a=0, can_bisect=0
F50 V54(E79) V54(E55) a=0, can_bisect=0
F50 V54(E79) V34(E81) a=0.229846, can_bisect=1
F50 V54(E55) V34(E81) a=0, can_bisect=0
F50 best_angle = 0.229846
Extra: Flipping E75
Extra: Flipping E76
Extra: Flipping E78
Extra: Flipping E79
Extra: Flipping E80
Extra: Flipping E82
Longest Edge: 6.00591
Looking for shortest saddle...1.61716
Clearing denominators: multiplying by 3
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=3.2012663909678238
# Vertices: V5 V10 V34 V54
# UEdges: 
E4 ( V54 V34 F64 F27 ) (3.000000000000000888178420,3.812705230150046809001196)
E5 ( V54 V5 F26 F41 ) (2.999999999999998223643161,-3.812705230150046809001196)
E11 ( V10 V34 F27 F49 ) (-2.999999999999998223643161,3.812705230150046809001196)
E17 ( V34 V10 F54 F30 ) (1.801899586451736423597936,4.504428826428338972220899)
E23 ( V5 V54 F64 F33 ) (4.801899586451735757464121,0.691723596278292385264308)
E31 ( V5 V10 F26 F58 ) (3.000000000000000888178420,3.812705230150046809001196)
E37 ( V54 V34 F41 F30 ) (-4.801899586451733981107282,0.691723596278292385264308)
E43 ( V10 V5 F54 F33 ) (-1.801899586451734425196491,4.504428826428338972220899)
E49 ( V10 V5 F49 F29 ) (4.801899586451733981107282,-0.691723596278292385264308)
E55 ( V54 V34 F60 F32 ) (1.801899586451734425196491,-4.504428826428338972220899)
E59 ( V5 V54 F60 F29 ) (-1.801899586451736423597936,-4.504428826428338972220899)
E65 ( V34 V10 F58 F32 ) (-4.801899586451735757464121,-0.691723596278292385264308)
E71 ( V54 V10 F27 F26 ) (5.999999999999998223643161,0.000000000000000000000000)
E72 ( V54 V10 F30 F29 ) (-2.999999999999998223643161,5.196152422706632023619022)
E73 ( V10 V54 F33 F32 ) (3.000000000000001332267630,5.196152422706632023619022)
E74 ( V5 V34 F55 F61 ) (15.603799172903466185857724,-9.008857652856676168084959)
E77 ( V5 V34 F57 F41 ) (-7.801899586451733092928862,4.504428826428338972220899)
E81 ( V34 V5 F63 F49 ) (7.801899586451733092928862,-4.504428826428338972220899)
E83 ( V34 V5 F55 F54 ) (0.000000000000002598554567,9.008857652856677944441799)
E84 ( V5 V5 F55 F57 ) (-15.603799172903471514928242,0.000000000000000000000000)
E85 ( V34 V5 F57 F58 ) (-7.801899586451735757464121,-4.504428826428338972220899)
E86 ( V5 V34 F61 F60 ) (-0.000000000000002598554567,-9.008857652856677944441799)
E87 ( V34 V34 F61 F63 ) (15.603799172903471514928242,0.000000000000000000000000)
E88 ( V5 V34 F63 F64 ) (7.801899586451735757464121,4.504428826428338972220899)
# Faces: 
F26 +E5 +E31 -E71
F27 +E11 -E4 +E71
F29 -E59 -E49 -E72
F30 -E17 -E37 +E72
F32 -E65 -E55 -E73
F33 -E23 -E43 +E73
F41 -E5 +E37 -E77
F49 -E11 +E49 -E81
F54 -E83 +E17 +E43
F55 +E84 +E74 +E83
F57 +E85 -E84 +E77
F58 -E85 +E65 -E31
F60 -E86 +E59 +E55
F61 +E87 -E74 +E86
F63 +E88 -E87 +E81
F64 -E88 +E23 +E4
#UEdges Algebraic 
#E4: (4 0 -2 0)+(-1 0 2 0)t1
#E5: (2 0 2 0)+(1 0 -2 0)t1
#E11: (-2 0 -2 0)+(-1 0 2 0)t1
#E17: (-4 0 2 0)+(1 0 1 0)t1
#E23: (-2 0 4 0)+(2 0 -1 0)t1
#E31: (4 0 -2 0)+(-1 0 2 0)t1
#E37: (-2 0 4 0)+(-1 0 -1 0)t1
#E43: (2 0 2 0)+(-2 0 1 0)t1
#E49: (2 0 -4 0)+(1 0 1 0)t1
#E55: (-2 0 -2 0)+(2 0 -1 0)t1
#E59: (4 0 -2 0)+(-1 0 -1 0)t1
#E65: (2 0 -4 0)+(-2 0 1 0)t1
#E71: (6 0 0 0)+(0 0 0 0)t1
#E72: (-6 0 6 0)+(0 0 0 0)t1
#E73: (0 0 6 0)+(0 0 0 0)t1
#E74: (8 0 -4 0)+(4 0 -2 0)t1
#E77: (-4 0 2 0)+(-2 0 1 0)t1
#E81: (4 0 -2 0)+(2 0 -1 0)t1
#E83: (-2 0 4 0)+(-1 0 2 0)t1
#E84: (-6 0 0 0)+(-3 0 0 0)t1
#E85: (-2 0 -2 0)+(-1 0 -1 0)t1
#E86: (2 0 -4 0)+(1 0 -2 0)t1
#E87: (6 0 0 0)+(3 0 0 0)t1
#E88: (2 0 2 0)+(1 0 1 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 4
## V5(15) (4 PI) V10(9) (4 PI) V34(15) (4 PI) V54(9) (4 PI) 
# edges: 24
## E4(2) E5(2) E11(2) E17(2) E23(2) E31(2) E37(2) E43(2) E49(2) E55(2) E59(2) E65(2) E71(2) E72(2) E73(2) E74(2) E77(2) E81(2) E83(2) E84(2) E85(2) E86(2) E87(2) E88(2) 
# faces: 16
# genus: 3
# area: 328.186
sweeping start V5 depth = 60.3864
File =  depth = 60.3864 follow_depth = 301.932 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 32 unusual 0 close 18167 reject 0
V5: AaXX V10: BXXb V34: CXXc V54: DdXX   cylinders |1|1|
Moduli: |1|1| Min Area: 0.114117 Max Area: 0.369889
    total: 1.41372 0.859437 sym = 2
    raw = 60 short saddle = 31.1407 short cyl = 31.1407 tag #3
V5: AbBa V10: CXXc V34: DdEe V54: FfXX   cylinders |1|1|2|2|
Moduli: |2.5741|2.5741|1|1| Min Area: 0.16799 Max Area: 0.16799
    total: 0.282743 0.171887 sym = 4
    raw = 24 short saddle = 46.3654 short cyl = 46.3654 tag #8
V5: AfBe V34: aEbF V10: CdDc V54: GgHh   cylinders |1|1|1.62475|1.62475|2.62475|
Moduli: |2.70507|2.70507|1|1|1.0306| Min Area: 1 Max Area: 1
    total: 0.153153 0.0931056 sym = 4
    raw = 13 short saddle = 7.62541 short cyl = 16.6343 tag #0
V5: AbBa V10: ChDg V54: cGdH V34: EeFf   cylinders |1|1|1.84643|1.84643|2.84643|
Moduli: |10.1773|10.1773|1|1|3.57547| Min Area: 1 Max Area: 1
    total: 0.141372 0.0859437 sym = 4
    raw = 12 short saddle = 6 short cyl = 15.6038 tag #9
V5: AdXX V54: aXXD V10: BXXc V34: bCXX   cylinders |1|
Moduli: |1| Min Area: 0.209115 Max Area: 0.209115
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 13.1679 short cyl = 39.5036 tag #10

total cylinders: 6.07898 3.69558 raw = 129 rejected = 0
""",
    (1, 5, 1, 3): r"""
max loc = 1
alpha =  5 beta = 1 gamma = 3 delta =1
a =  3.14159 b = 0.628319 c= 1.88496 d=0.628319
a1_min = 0.628319 a1_max = 2.51327
a1 = 1.91327
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 1.04099 p-q = 1.66818
p = (1,0) q = (-0.349586,0.980532) r = (-1.69917,2.17722e-16)
Imaginary of t_cx = -2.17722e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (4.44089e-16,0)
Checking r_alt (0,-2.17722e-16)
Checking p_algt (0,0)
Checking q_algt (4.44089e-16,0)
Checking r_algt (0,-2.17722e-16)
Number field: min poly (1 -1 1 -1 1)
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.6991724496857903
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-1.349586224842895143538613,0.980531787558568623808242)
E2 ( V2 V3 F0 NULL ) (-1.349586224842895143538613,-0.980531787558568623808242)
E3 ( V3 V0 F0 NULL ) (1.699172449685790287077225,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 -1 1 0)+(0 -1 1 0)t1
#E2: (-1 1 -1 0)+(-1 1 -1 0)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.6, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.6
In RetriangulateSurface
Removing V0
F21 V15(E10) V2(E6) a=0, can_bisect=0
F21 V15(E10) V1(E1) a=0.628319, can_bisect=1
F21 V15(E10) V2(E2) a=0, can_bisect=0
F21 V2(E6) V1(E1) a=0, can_bisect=0
F21 V2(E6) V2(E2) a=0.942478, can_bisect=1
F21 V1(E1) V2(E2) a=0, can_bisect=0
F21 best_angle = 0.942478
Removing V1
F24 V2(E25) V17(E17) a=0, can_bisect=0
F24 V2(E25) V2(E43) a=1.54248, can_bisect=0
F24 V2(E25) V40(E47) a=0.151657, can_bisect=1
F24 V2(E25) V2(E37) a=0.285841, can_bisect=1
F24 V2(E25) V36(E53) a=0.6, can_bisect=1
F24 V2(E25) V2(E31) a=0.285841, can_bisect=1
F24 V2(E25) V10(E11) a=0.494135, can_bisect=1
F24 V2(E25) V2(E59) a=0, can_bisect=0
F24 V17(E17) V2(E43) a=0, can_bisect=0
F24 V17(E17) V40(E47) a=0.970796, can_bisect=1
F24 V17(E17) V2(E37) a=0.151657, can_bisect=1
F24 V17(E17) V36(E53) a=1.59911, can_bisect=1
F24 V17(E17) V2(E31) a=0.6, can_bisect=1
F24 V17(E17) V10(E11) a=1.59911, can_bisect=1
F24 V17(E17) V2(E59) a=0.494135, can_bisect=1
F24 V2(E43) V40(E47) a=0, can_bisect=0
F24 V2(E43) V2(E37) a=1.54248, can_bisect=0
F24 V2(E43) V36(E53) a=0.151657, can_bisect=1
F24 V2(E43) V2(E31) a=0.285841, can_bisect=1
F24 V2(E43) V10(E11) a=0.6, can_bisect=1
F24 V2(E43) V2(E59) a=0.285841, can_bisect=1
F24 V40(E47) V2(E37) a=0, can_bisect=0
F24 V40(E47) V36(E53) a=0.970796, can_bisect=1
F24 V40(E47) V2(E31) a=0.151657, can_bisect=1
F24 V40(E47) V10(E11) a=1.59911, can_bisect=1
F24 V40(E47) V2(E59) a=0.6, can_bisect=1
F24 V2(E37) V36(E53) a=0, can_bisect=0
F24 V2(E37) V2(E31) a=1.54248, can_bisect=0
F24 V2(E37) V10(E11) a=0.151657, can_bisect=1
F24 V2(E37) V2(E59) a=0.285841, can_bisect=1
F24 V36(E53) V2(E31) a=0, can_bisect=0
F24 V36(E53) V10(E11) a=0.970796, can_bisect=1
F24 V36(E53) V2(E59) a=0.151657, can_bisect=1
F24 V2(E31) V10(E11) a=0, can_bisect=0
F24 V2(E31) V2(E59) a=1.54248, can_bisect=0
F24 V10(E11) V2(E59) a=0, can_bisect=0
F24 best_angle = 1.59911
F26 V10(E11) V2(E59) a=0, can_bisect=0
F26 V10(E11) V2(E25) a=0.494135, can_bisect=1
F26 V10(E11) V17(E60) a=0, can_bisect=0
F26 V2(E59) V2(E25) a=0, can_bisect=0
F26 V2(E59) V17(E60) a=0.494135, can_bisect=1
F26 V2(E25) V17(E60) a=0, can_bisect=0
F26 best_angle = 0.494135
F25 V17(E17) V2(E43) a=0, can_bisect=0
F25 V17(E17) V40(E47) a=0.970796, can_bisect=1
F25 V17(E17) V2(E37) a=0.151657, can_bisect=1
F25 V17(E17) V36(E53) a=0.628319, can_bisect=1
F25 V17(E17) V2(E31) a=0.314159, can_bisect=1
F25 V17(E17) V10(E60) a=0, can_bisect=0
F25 V2(E43) V40(E47) a=0, can_bisect=0
F25 V2(E43) V2(E37) a=1.54248, can_bisect=0
F25 V2(E43) V36(E53) a=0.151657, can_bisect=1
F25 V2(E43) V2(E31) a=0.285841, can_bisect=1
F25 V2(E43) V10(E60) a=0.314159, can_bisect=1
F25 V40(E47) V2(E37) a=0, can_bisect=0
F25 V40(E47) V36(E53) a=0.970796, can_bisect=1
F25 V40(E47) V2(E31) a=0.151657, can_bisect=1
F25 V40(E47) V10(E60) a=0.628319, can_bisect=1
F25 V2(E37) V36(E53) a=0, can_bisect=0
F25 V2(E37) V2(E31) a=1.54248, can_bisect=0
F25 V2(E37) V10(E60) a=0.151657, can_bisect=1
F25 V36(E53) V2(E31) a=0, can_bisect=0
F25 V36(E53) V10(E60) a=0.970796, can_bisect=1
F25 V2(E31) V10(E60) a=0, can_bisect=0
F25 best_angle = 0.970796
F30 V36(E53) V2(E31) a=0, can_bisect=0
F30 V36(E53) V10(E60) a=0.970796, can_bisect=1
F30 V36(E53) V17(E17) a=0.628319, can_bisect=1
F30 V36(E53) V2(E43) a=0.134183, can_bisect=1
F30 V36(E53) V40(E62) a=0, can_bisect=0
F30 V2(E31) V10(E60) a=0, can_bisect=0
F30 V2(E31) V17(E17) a=0.314159, can_bisect=1
F30 V2(E31) V2(E43) a=0.285841, can_bisect=1
F30 V2(E31) V40(E62) a=0.134183, can_bisect=1
F30 V10(E60) V17(E17) a=0, can_bisect=0
F30 V10(E60) V2(E43) a=0.314159, can_bisect=1
F30 V10(E60) V40(E62) a=0.628319, can_bisect=1
F30 V17(E17) V2(E43) a=0, can_bisect=0
F30 V17(E17) V40(E62) a=0.970796, can_bisect=1
F30 V2(E43) V40(E62) a=0, can_bisect=0
F30 best_angle = 0.970796
F32 V10(E60) V17(E17) a=0, can_bisect=0
F32 V10(E60) V2(E43) a=0.314159, can_bisect=1
F32 V10(E60) V40(E62) a=0.628319, can_bisect=1
F32 V10(E60) V36(E63) a=0, can_bisect=0
F32 V17(E17) V2(E43) a=0, can_bisect=0
F32 V17(E17) V40(E62) a=0.970796, can_bisect=1
F32 V17(E17) V36(E63) a=0.628319, can_bisect=1
F32 V2(E43) V40(E62) a=0, can_bisect=0
F32 V2(E43) V36(E63) a=0.134183, can_bisect=1
F32 V40(E62) V36(E63) a=0, can_bisect=0
F32 best_angle = 0.970796
F34 V40(E62) V36(E63) a=0, can_bisect=0
F34 V40(E62) V10(E60) a=0.628319, can_bisect=1
F34 V40(E62) V17(E64) a=0, can_bisect=0
F34 V36(E63) V10(E60) a=0, can_bisect=0
F34 V36(E63) V17(E64) a=0.628319, can_bisect=1
F34 V10(E60) V17(E64) a=0, can_bisect=0
F34 best_angle = 0.628319
Removing V10
F37 V36(E53) V2(E10) a=0, can_bisect=0
F37 V36(E53) V15(E16) a=0.249619, can_bisect=1
F37 V36(E53) V2(E59) a=1.10498, can_bisect=1
F37 V36(E53) V2(E25) a=0.6, can_bisect=1
F37 V36(E53) V17(E64) a=0.628319, can_bisect=1
F37 V36(E53) V40(E62) a=0, can_bisect=0
F37 V2(E10) V15(E16) a=0, can_bisect=0
F37 V2(E10) V2(E59) a=0.942478, can_bisect=1
F37 V2(E10) V2(E25) a=0.628319, can_bisect=1
F37 V2(E10) V17(E64) a=0.6, can_bisect=1
F37 V2(E10) V40(E62) a=0.134183, can_bisect=1
F37 V15(E16) V2(E59) a=0, can_bisect=0
F37 V15(E16) V2(E25) a=0.144299, can_bisect=1
F37 V15(E16) V17(E64) a=0.462314, can_bisect=1
F37 V15(E16) V40(E62) a=0.462314, can_bisect=1
F37 V2(E59) V2(E25) a=0, can_bisect=0
F37 V2(E59) V17(E64) a=0.494135, can_bisect=1
F37 V2(E59) V40(E62) a=0.942478, can_bisect=1
F37 V2(E25) V17(E64) a=0, can_bisect=0
F37 V2(E25) V40(E62) a=0.134183, can_bisect=1
F37 V17(E64) V40(E62) a=0, can_bisect=0
F37 best_angle = 1.10498
F39 V2(E59) V2(E25) a=0, can_bisect=0
F39 V2(E59) V17(E64) a=0.494135, can_bisect=1
F39 V2(E59) V40(E62) a=0.448343, can_bisect=1
F39 V2(E59) V36(E66) a=0, can_bisect=0
F39 V2(E25) V17(E64) a=0, can_bisect=0
F39 V2(E25) V40(E62) a=0.134183, can_bisect=1
F39 V2(E25) V36(E66) a=0.6, can_bisect=1
F39 V17(E64) V40(E62) a=0, can_bisect=0
F39 V17(E64) V36(E66) a=0.628319, can_bisect=1
F39 V40(E62) V36(E66) a=0, can_bisect=0
F39 best_angle = 0.628319
F41 V36(E66) V2(E59) a=0, can_bisect=0
F41 V36(E66) V2(E25) a=0.314159, can_bisect=1
F41 V36(E66) V17(E67) a=0, can_bisect=0
F41 V2(E59) V2(E25) a=0, can_bisect=0
F41 V2(E59) V17(E67) a=0.494135, can_bisect=1
F41 V2(E25) V17(E67) a=0, can_bisect=0
F41 best_angle = 0.494135
F38 V36(E53) V2(E10) a=0, can_bisect=0
F38 V36(E53) V15(E16) a=0.249619, can_bisect=1
F38 V36(E53) V2(E66) a=0, can_bisect=0
F38 V2(E10) V15(E16) a=0, can_bisect=0
F38 V2(E10) V2(E66) a=0.494135, can_bisect=1
F38 V15(E16) V2(E66) a=0, can_bisect=0
F38 best_angle = 0.494135
Removing V15
F46 V2(E59) V2(E17) a=0, can_bisect=0
F46 V2(E59) V17(E25) a=0.307472, can_bisect=1
F46 V2(E59) V2(E47) a=0.628319, can_bisect=1
F46 V2(E59) V40(E43) a=0.942478, can_bisect=1
F46 V2(E59) V2(E53) a=0.628319, can_bisect=1
F46 V2(E59) V36(E37) a=0.307472, can_bisect=1
F46 V2(E59) V2(E69) a=0, can_bisect=0
F46 V2(E17) V17(E25) a=0, can_bisect=0
F46 V2(E17) V2(E47) a=0.342478, can_bisect=1
F46 V2(E17) V40(E43) a=0.606687, can_bisect=1
F46 V2(E17) V2(E53) a=0.970796, can_bisect=1
F46 V2(E17) V36(E37) a=0.942478, can_bisect=1
F46 V2(E17) V2(E69) a=0.628319, can_bisect=1
F46 V17(E25) V2(E47) a=0, can_bisect=0
F46 V17(E25) V40(E43) a=0.285841, can_bisect=1
F46 V17(E25) V2(E53) a=0.606687, can_bisect=1
F46 V17(E25) V36(E37) a=0.914159, can_bisect=1
F46 V17(E25) V2(E69) a=0.942478, can_bisect=1
F46 V2(E47) V40(E43) a=0, can_bisect=0
F46 V2(E47) V2(E53) a=0.342478, can_bisect=1
F46 V2(E47) V36(E37) a=0.606687, can_bisect=1
F46 V2(E47) V2(E69) a=0.970796, can_bisect=1
F46 V40(E43) V2(E53) a=0, can_bisect=0
F46 V40(E43) V36(E37) a=0.285841, can_bisect=1
F46 V40(E43) V2(E69) a=0.606687, can_bisect=1
F46 V2(E53) V36(E37) a=0, can_bisect=0
F46 V2(E53) V2(E69) a=0.342478, can_bisect=1
F46 V36(E37) V2(E69) a=0, can_bisect=0
F46 best_angle = 0.970796
F48 V2(E69) V2(E59) a=0, can_bisect=0
F48 V2(E69) V2(E17) a=0.628319, can_bisect=1
F48 V2(E69) V17(E25) a=0.314159, can_bisect=1
F48 V2(E69) V2(E70) a=0, can_bisect=0
F48 V2(E59) V2(E17) a=0, can_bisect=0
F48 V2(E59) V17(E25) a=0.307472, can_bisect=1
F48 V2(E59) V2(E70) a=0.628319, can_bisect=1
F48 V2(E17) V17(E25) a=0, can_bisect=0
F48 V2(E17) V2(E70) a=0.342478, can_bisect=1
F48 V17(E25) V2(E70) a=0, can_bisect=0
F48 best_angle = 0.628319
F50 V2(E17) V17(E25) a=0, can_bisect=0
F50 V2(E17) V2(E70) a=0.342478, can_bisect=1
F50 V2(E17) V2(E71) a=0, can_bisect=0
F50 V17(E25) V2(E70) a=0, can_bisect=0
F50 V17(E25) V2(E71) a=0.314159, can_bisect=1
F50 V2(E70) V2(E71) a=0, can_bisect=0
F50 best_angle = 0.342478
F47 V2(E47) V40(E43) a=0, can_bisect=0
F47 V2(E47) V2(E53) a=0.342478, can_bisect=1
F47 V2(E47) V36(E37) a=0.320846, can_bisect=1
F47 V2(E47) V2(E70) a=0, can_bisect=0
F47 V40(E43) V2(E53) a=0, can_bisect=0
F47 V40(E43) V36(E37) a=0.285841, can_bisect=1
F47 V40(E43) V2(E70) a=0.320846, can_bisect=1
F47 V2(E53) V36(E37) a=0, can_bisect=0
F47 V2(E53) V2(E70) a=0.342478, can_bisect=1
F47 V36(E37) V2(E70) a=0, can_bisect=0
F47 best_angle = 0.342478
F54 V2(E70) V2(E47) a=0, can_bisect=0
F54 V2(E70) V40(E43) a=0.307472, can_bisect=1
F54 V2(E70) V2(E73) a=0, can_bisect=0
F54 V2(E47) V40(E43) a=0, can_bisect=0
F54 V2(E47) V2(E73) a=0.342478, can_bisect=1
F54 V40(E43) V2(E73) a=0, can_bisect=0
F54 best_angle = 0.342478
Removing V17
F57 V2(E43) V40(E62) a=0, can_bisect=0
F57 V2(E43) V36(E66) a=0.134183, can_bisect=1
F57 V2(E43) V2(E59) a=0.628319, can_bisect=1
F57 V2(E43) V2(E72) a=0, can_bisect=0
F57 V40(E62) V36(E66) a=0, can_bisect=0
F57 V40(E62) V2(E59) a=0.448343, can_bisect=1
F57 V40(E62) V2(E72) a=0.494135, can_bisect=1
F57 V36(E66) V2(E59) a=0, can_bisect=0
F57 V36(E66) V2(E72) a=0.808295, can_bisect=1
F57 V2(E59) V2(E72) a=0, can_bisect=0
F57 best_angle = 0.808295
F59 V2(E72) V2(E43) a=0, can_bisect=0
F59 V2(E72) V40(E62) a=0.448343, can_bisect=1
F59 V2(E72) V36(E75) a=0, can_bisect=0
F59 V2(E43) V40(E62) a=0, can_bisect=0
F59 V2(E43) V36(E75) a=0.134183, can_bisect=1
F59 V40(E62) V36(E75) a=0, can_bisect=0
F59 best_angle = 0.448343
Removing V36
F62 V40(E47) V2(E73) a=0, can_bisect=0
F62 V40(E47) V2(E69) a=0.494135, can_bisect=1
F62 V40(E47) V2(E59) a=0.808295, can_bisect=1
F62 V40(E47) V2(E76) a=0, can_bisect=0
F62 V2(E73) V2(E69) a=0, can_bisect=0
F62 V2(E73) V2(E59) a=0.628319, can_bisect=1
F62 V2(E73) V2(E76) a=0.134183, can_bisect=1
F62 V2(E69) V2(E59) a=0, can_bisect=0
F62 V2(E69) V2(E76) a=0.628319, can_bisect=1
F62 V2(E59) V2(E76) a=0, can_bisect=0
F62 best_angle = 0.808295
F63 V40(E47) V2(E73) a=0, can_bisect=0
F63 V40(E47) V2(E69) a=0.494135, can_bisect=1
F63 V40(E47) V2(E77) a=0, can_bisect=0
F63 V2(E73) V2(E69) a=0, can_bisect=0
F63 V2(E73) V2(E77) a=0.314159, can_bisect=1
F63 V2(E69) V2(E77) a=0, can_bisect=0
F63 best_angle = 0.494135
Removing V40
F67 V2(E59) V2(E72) a=0, can_bisect=0
F67 V2(E59) V2(E74) a=0.628319, can_bisect=1
F67 V2(E59) V2(E73) a=0.628319, can_bisect=1
F67 V2(E59) V2(E69) a=0, can_bisect=0
F67 V2(E72) V2(E74) a=0, can_bisect=0
F67 V2(E72) V2(E73) a=0.628319, can_bisect=1
F67 V2(E72) V2(E69) a=0.628319, can_bisect=1
F67 V2(E74) V2(E73) a=0, can_bisect=0
F67 V2(E74) V2(E69) a=0.628319, can_bisect=1
F67 V2(E73) V2(E69) a=0, can_bisect=0
F67 best_angle = 0.628319
F69 V2(E69) V2(E59) a=0, can_bisect=0
F69 V2(E69) V2(E72) a=0.628319, can_bisect=1
F69 V2(E69) V2(E79) a=0, can_bisect=0
F69 V2(E59) V2(E72) a=0, can_bisect=0
F69 V2(E59) V2(E79) a=0.628319, can_bisect=1
F69 V2(E72) V2(E79) a=0, can_bisect=0
F69 best_angle = 0.628319
Longest Edge: 3.17307
Looking for shortest saddle...1.96106
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=1.6991724496857903
# Vertices: V2
# UEdges: 
E59 ( V2 V2 F49 F70 ) (0.000000000000000000000000,1.961063575117137247616483)
E69 ( V2 V2 F70 F49 ) (-1.865082291984223372693918,-0.606001971760886437579074)
E70 ( V2 V2 F52 F56 ) (0.000000000000000000000000,-3.173067518638910122774632)
E71 ( V2 V2 F52 F49 ) (1.865082291984223372693918,2.567065546878023685195558)
E72 ( V2 V2 F52 F71 ) (-1.865082291984223372693918,0.606001971760886326556772)
E73 ( V2 V2 F56 F68 ) (1.152684248261805821655912,-1.586533759319455061387316)
E74 ( V2 V2 F56 F68 ) (-1.152684248261805821655912,-1.586533759319455061387316)
E79 ( V2 V2 F71 F68 ) (0.000000000000000000000000,3.173067518638910122774632)
E80 ( V2 V2 F71 F70 ) (-1.865082291984223372693918,-2.567065546878023685195558)
# Faces: 
F49 -E69 +E59 -E71
F52 +E70 +E71 +E72
F56 +E73 -E70 +E74
F68 -E74 -E73 -E79
F70 +E69 -E59 -E80
F71 -E72 +E79 +E80
#UEdges Algebraic 
#E59: (1 -2 2 0)+(1 -2 2 0)t1
#E69: (0 -2 2 -1)+(0 -2 2 -1)t1
#E70: (1 -2 1 -1)+(1 -2 1 -1)t1
#E71: (1 0 0 1)+(1 0 0 1)t1
#E72: (-2 2 -1 0)+(-2 2 -1 0)t1
#E73: (0 -1 2 -2)+(0 -1 2 -2)t1
#E74: (1 -1 -1 1)+(1 -1 -1 1)t1
#E79: (-1 2 -1 1)+(-1 2 -1 1)t1
#E80: (-1 0 0 -1)+(-1 0 0 -1)t1
# File =  perturb = 0 rescale = 1
# vertices: 1
## V2(18) (6 PI) 
# edges: 9
## E59(2) E69(2) E70(2) E71(2) E72(2) E73(2) E74(2) E79(2) E80(2) 
# faces: 6
# genus: 2
# area: 13.2331
sweeping start V2 depth = 36.3774
File =  depth = 36.3774 follow_depth = 181.887 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 0 unusual 0 close 191617 reject 0
V2: AbBaCc   cylinders |1|1.61803|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.50098 0.912488 sym = 6
    raw = 1720 short saddle = 1.96106 short cyl = 3.17307 tag #0

total cylinders: 3.00022 1.82392 raw = 573 rejected = 0
""",
    (1, 5, 2, 2): r"""
max loc = 1
alpha =  5 beta = 2 gamma = 2 delta =1
a =  3.14159 b = 1.25664 c= 1.25664 d=0.628319
a1_min = 0.628319 a1_max = 1.88496
a1 = 1.48496
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 2.44225 p-q = 2.55848
p = (1,0) q = (0.209387,2.43326) r = (-3.1397,4.05219e-16)
Imaginary of t_cx = -4.05219e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (4.996e-16,-8.88178e-16)
Checking r_alt (0,-4.05219e-16)
Checking p_algt (0,0)
Checking q_algt (4.996e-16,-8.88178e-16)
Checking r_algt (0,-4.05219e-16)
Number field: min poly (1 -1 1 -1 1)
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=3.1397030519560873
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.790612931257776652138602,2.433256402809929497266239)
E2 ( V2 V3 F0 NULL ) (-3.349090120698310801827802,-2.433256402809929497266239)
E3 ( V3 V0 F0 NULL ) (3.139703051956087342944102,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-1 1 0 0)+(-1 1 0 0)t1
#E2: (0 -1 0 0)+(0 -1 0 0)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.4, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.4
In RetriangulateSurface
Removing V0
F21 V3(E10) V40(E6) a=0, can_bisect=0
F21 V3(E10) V21(E1) a=0.628319, can_bisect=1
F21 V3(E10) V40(E2) a=0, can_bisect=0
F21 V40(E6) V21(E1) a=0, can_bisect=0
F21 V40(E6) V40(E2) a=0.314159, can_bisect=1
F21 V21(E1) V40(E2) a=0, can_bisect=0
F21 best_angle = 0.628319
Removing V3
F24 V40(E17) V17(E43) a=0, can_bisect=0
F24 V40(E17) V40(E47) a=1.79911, can_bisect=0
F24 V40(E17) V22(E25) a=0.327797, can_bisect=1
F24 V40(E17) V40(E11) a=0.542478, can_bisect=1
F24 V40(E17) V10(E37) a=0.856637, can_bisect=1
F24 V40(E17) V40(E53) a=0.542478, can_bisect=1
F24 V40(E17) V30(E31) a=0.727797, can_bisect=1
F24 V40(E17) V40(E6) a=0.314159, can_bisect=1
F24 V40(E17) V21(E1) a=0, can_bisect=0
F24 V17(E43) V40(E47) a=0, can_bisect=0
F24 V17(E43) V22(E25) a=0.714159, can_bisect=1
F24 V17(E43) V40(E11) a=0.327797, can_bisect=1
F24 V17(E43) V10(E37) a=1.34248, can_bisect=1
F24 V17(E43) V40(E53) a=0.856637, can_bisect=1
F24 V17(E43) V30(E31) a=1.34248, can_bisect=1
F24 V17(E43) V40(E6) a=0.727797, can_bisect=1
F24 V17(E43) V21(E1) a=0.501032, can_bisect=1
F24 V40(E47) V22(E25) a=0, can_bisect=0
F24 V40(E47) V40(E11) a=1.79911, can_bisect=0
F24 V40(E47) V10(E37) a=0.327797, can_bisect=1
F24 V40(E47) V40(E53) a=0.542478, can_bisect=1
F24 V40(E47) V30(E31) a=0.856637, can_bisect=1
F24 V40(E47) V40(E6) a=0.542478, can_bisect=1
F24 V40(E47) V21(E1) a=0.228319, can_bisect=1
F24 V22(E25) V40(E11) a=0, can_bisect=0
F24 V22(E25) V10(E37) a=0.714159, can_bisect=1
F24 V22(E25) V40(E53) a=0.327797, can_bisect=1
F24 V22(E25) V30(E31) a=1.34248, can_bisect=1
F24 V22(E25) V40(E6) a=0.856637, can_bisect=1
F24 V22(E25) V21(E1) a=0.987084, can_bisect=1
F24 V40(E11) V10(E37) a=0, can_bisect=0
F24 V40(E11) V40(E53) a=1.79911, can_bisect=0
F24 V40(E11) V30(E31) a=0.327797, can_bisect=1
F24 V40(E11) V40(E6) a=0.542478, can_bisect=1
F24 V40(E11) V21(E1) a=0.856637, can_bisect=1
F24 V10(E37) V40(E53) a=0, can_bisect=0
F24 V10(E37) V30(E31) a=0.714159, can_bisect=1
F24 V10(E37) V40(E6) a=0.327797, can_bisect=1
F24 V10(E37) V21(E1) a=0.987084, can_bisect=1
F24 V40(E53) V30(E31) a=0, can_bisect=0
F24 V40(E53) V40(E6) a=1.79911, can_bisect=0
F24 V40(E53) V21(E1) a=0.228319, can_bisect=1
F24 V30(E31) V40(E6) a=0, can_bisect=0
F24 V30(E31) V21(E1) a=0.501032, can_bisect=1
F24 V40(E6) V21(E1) a=0, can_bisect=0
F24 best_angle = 1.34248
F26 V10(E37) V40(E53) a=0, can_bisect=0
F26 V10(E37) V30(E31) a=0.714159, can_bisect=1
F26 V10(E37) V40(E6) a=0.327797, can_bisect=1
F26 V10(E37) V21(E1) a=0.672924, can_bisect=1
F26 V10(E37) V40(E17) a=0.314159, can_bisect=1
F26 V10(E37) V17(E60) a=0, can_bisect=0
F26 V40(E53) V30(E31) a=0, can_bisect=0
F26 V40(E53) V40(E6) a=1.79911, can_bisect=0
F26 V40(E53) V21(E1) a=0.228319, can_bisect=1
F26 V40(E53) V40(E17) a=0.542478, can_bisect=1
F26 V40(E53) V17(E60) a=0.314159, can_bisect=1
F26 V30(E31) V40(E6) a=0, can_bisect=0
F26 V30(E31) V21(E1) a=0.501032, can_bisect=1
F26 V30(E31) V40(E17) a=0.727797, can_bisect=1
F26 V30(E31) V17(E60) a=0.628319, can_bisect=1
F26 V40(E6) V21(E1) a=0, can_bisect=0
F26 V40(E6) V40(E17) a=0.314159, can_bisect=1
F26 V40(E6) V17(E60) a=0.727797, can_bisect=1
F26 V21(E1) V40(E17) a=0, can_bisect=0
F26 V21(E1) V17(E60) a=0.501032, can_bisect=1
F26 V40(E17) V17(E60) a=0, can_bisect=0
F26 best_angle = 0.727797
F28 V40(E17) V17(E60) a=0, can_bisect=0
F28 V40(E17) V10(E37) a=0.314159, can_bisect=1
F28 V40(E17) V40(E53) a=0.214681, can_bisect=1
F28 V40(E17) V30(E61) a=0, can_bisect=0
F28 V17(E60) V10(E37) a=0, can_bisect=0
F28 V17(E60) V40(E53) a=0.314159, can_bisect=1
F28 V17(E60) V30(E61) a=0.413638, can_bisect=1
F28 V10(E37) V40(E53) a=0, can_bisect=0
F28 V10(E37) V30(E61) a=0.714159, can_bisect=1
F28 V40(E53) V30(E61) a=0, can_bisect=0
F28 best_angle = 0.714159
F30 V30(E61) V40(E17) a=0, can_bisect=0
F30 V30(E61) V17(E60) a=0.413638, can_bisect=1
F30 V30(E61) V10(E62) a=0, can_bisect=0
F30 V40(E17) V17(E60) a=0, can_bisect=0
F30 V40(E17) V10(E62) a=0.314159, can_bisect=1
F30 V17(E60) V10(E62) a=0, can_bisect=0
F30 best_angle = 0.413638
F27 V30(E31) V40(E6) a=0, can_bisect=0
F27 V30(E31) V21(E1) a=0.401554, can_bisect=1
F27 V30(E31) V40(E61) a=0, can_bisect=0
F27 V40(E6) V21(E1) a=0, can_bisect=0
F27 V40(E6) V40(E61) a=0.314159, can_bisect=1
F27 V21(E1) V40(E61) a=0, can_bisect=0
F27 best_angle = 0.401554
F25 V17(E43) V40(E47) a=0, can_bisect=0
F25 V17(E43) V22(E25) a=0.628319, can_bisect=1
F25 V17(E43) V40(E11) a=0.327797, can_bisect=1
F25 V17(E43) V10(E60) a=0, can_bisect=0
F25 V40(E47) V22(E25) a=0, can_bisect=0
F25 V40(E47) V40(E11) a=1.79911, can_bisect=0
F25 V40(E47) V10(E60) a=0.327797, can_bisect=1
F25 V22(E25) V40(E11) a=0, can_bisect=0
F25 V22(E25) V10(E60) a=0.628319, can_bisect=1
F25 V40(E11) V10(E60) a=0, can_bisect=0
F25 best_angle = 0.628319
F36 V10(E60) V17(E43) a=0, can_bisect=0
F36 V10(E60) V40(E47) a=0.214681, can_bisect=1
F36 V10(E60) V22(E65) a=0, can_bisect=0
F36 V17(E43) V40(E47) a=0, can_bisect=0
F36 V17(E43) V22(E65) a=0.628319, can_bisect=1
F36 V40(E47) V22(E65) a=0, can_bisect=0
F36 best_angle = 0.628319
Removing V10
F39 V40(E1) V21(E21) a=0, can_bisect=0
F39 V40(E1) V40(E53) a=0.314159, can_bisect=1
F39 V40(E1) V30(E63) a=0.727797, can_bisect=1
F39 V40(E1) V17(E66) a=0.214681, can_bisect=1
F39 V40(E1) V22(E25) a=0, can_bisect=0
F39 V21(E21) V40(E53) a=0, can_bisect=0
F39 V21(E21) V30(E63) a=0.501032, can_bisect=1
F39 V21(E21) V17(E66) a=0.583713, can_bisect=1
F39 V21(E21) V22(E25) a=0.501032, can_bisect=1
F39 V40(E53) V30(E63) a=0, can_bisect=0
F39 V40(E53) V17(E66) a=0.314159, can_bisect=1
F39 V40(E53) V22(E25) a=0.727797, can_bisect=1
F39 V30(E63) V17(E66) a=0, can_bisect=0
F39 V30(E63) V22(E25) a=0.628319, can_bisect=1
F39 V17(E66) V22(E25) a=0, can_bisect=0
F39 best_angle = 0.727797
F41 V22(E25) V40(E1) a=0, can_bisect=0
F41 V22(E25) V21(E21) a=0.401554, can_bisect=1
F41 V22(E25) V40(E67) a=0, can_bisect=0
F41 V40(E1) V21(E21) a=0, can_bisect=0
F41 V40(E1) V40(E67) a=0.314159, can_bisect=1
F41 V21(E21) V40(E67) a=0, can_bisect=0
F41 best_angle = 0.401554
F40 V40(E53) V30(E63) a=0, can_bisect=0
F40 V40(E53) V17(E66) a=0.314159, can_bisect=1
F40 V40(E53) V22(E67) a=0, can_bisect=0
F40 V30(E63) V17(E66) a=0, can_bisect=0
F40 V30(E63) V22(E67) a=0.413638, can_bisect=1
F40 V17(E66) V22(E67) a=0, can_bisect=0
F40 best_angle = 0.413638
Removing V17
F46 V30(E61) V40(E21) a=0, can_bisect=0
F46 V30(E61) V21(E45) a=0.369032, can_bisect=1
F46 V30(E61) V40(E47) a=0.314159, can_bisect=1
F46 V30(E61) V22(E69) a=0, can_bisect=0
F46 V40(E21) V21(E45) a=0, can_bisect=0
F46 V40(E21) V40(E47) a=0.314159, can_bisect=1
F46 V40(E21) V22(E69) a=0.727797, can_bisect=1
F46 V21(E45) V40(E47) a=0, can_bisect=0
F46 V21(E45) V22(E69) a=0.501032, can_bisect=1
F46 V40(E47) V22(E69) a=0, can_bisect=0
F46 best_angle = 0.727797
F47 V40(E21) V21(E45) a=0, can_bisect=0
F47 V40(E21) V40(E47) a=0.314159, can_bisect=1
F47 V40(E21) V22(E70) a=0, can_bisect=0
F47 V21(E45) V40(E47) a=0, can_bisect=0
F47 V21(E45) V22(E70) a=0.401554, can_bisect=1
F47 V40(E47) V22(E70) a=0, can_bisect=0
F47 best_angle = 0.401554
Removing V22
F51 V40(E33) V21(E6) a=0, can_bisect=0
F51 V40(E33) V40(E1) a=0.314159, can_bisect=1
F51 V40(E33) V21(E21) a=0.628319, can_bisect=1
F51 V40(E33) V40(E53) a=0.942478, can_bisect=1
F51 V40(E33) V30(E61) a=0.727797, can_bisect=1
F51 V40(E33) V40(E21) a=0.314159, can_bisect=1
F51 V40(E33) V21(E45) a=0, can_bisect=0
F51 V21(E6) V40(E1) a=0, can_bisect=0
F51 V21(E6) V21(E21) a=0.314159, can_bisect=1
F51 V21(E6) V40(E53) a=0.628319, can_bisect=1
F51 V21(E6) V30(E61) a=0.987084, can_bisect=1
F51 V21(E6) V40(E21) a=0.628319, can_bisect=1
F51 V21(E6) V21(E45) a=0.314159, can_bisect=1
F51 V40(E1) V21(E21) a=0, can_bisect=0
F51 V40(E1) V40(E53) a=0.314159, can_bisect=1
F51 V40(E1) V30(E61) a=0.727797, can_bisect=1
F51 V40(E1) V40(E21) a=0.842999, can_bisect=1
F51 V40(E1) V21(E45) a=0.628319, can_bisect=1
F51 V21(E21) V40(E53) a=0, can_bisect=0
F51 V21(E21) V30(E61) a=0.501032, can_bisect=1
F51 V21(E21) V40(E21) a=0.52884, can_bisect=1
F51 V21(E21) V21(E45) a=0.942478, can_bisect=1
F51 V40(E53) V30(E61) a=0, can_bisect=0
F51 V40(E53) V40(E21) a=0.214681, can_bisect=1
F51 V40(E53) V21(E45) a=0.856637, can_bisect=1
F51 V30(E61) V40(E21) a=0, can_bisect=0
F51 V30(E61) V21(E45) a=0.369032, can_bisect=1
F51 V40(E21) V21(E45) a=0, can_bisect=0
F51 best_angle = 0.987084
F53 V30(E61) V40(E21) a=0, can_bisect=0
F53 V30(E61) V21(E45) a=0.369032, can_bisect=1
F53 V30(E61) V40(E33) a=0.358765, can_bisect=1
F53 V30(E61) V21(E72) a=0, can_bisect=0
F53 V40(E21) V21(E45) a=0, can_bisect=0
F53 V40(E21) V40(E33) a=0.314159, can_bisect=1
F53 V40(E21) V21(E72) a=0.628319, can_bisect=1
F53 V21(E45) V40(E33) a=0, can_bisect=0
F53 V21(E45) V21(E72) a=0.314159, can_bisect=1
F53 V40(E33) V21(E72) a=0, can_bisect=0
F53 best_angle = 0.628319
F54 V40(E21) V21(E45) a=0, can_bisect=0
F54 V40(E21) V40(E33) a=0.314159, can_bisect=1
F54 V40(E21) V21(E73) a=0, can_bisect=0
F54 V21(E45) V40(E33) a=0, can_bisect=0
F54 V21(E45) V21(E73) a=0.314159, can_bisect=1
F54 V40(E33) V21(E73) a=0, can_bisect=0
F54 best_angle = 0.314159
F52 V21(E6) V40(E1) a=0, can_bisect=0
F52 V21(E6) V21(E21) a=0.314159, can_bisect=1
F52 V21(E6) V40(E53) a=0.358765, can_bisect=1
F52 V21(E6) V30(E72) a=0, can_bisect=0
F52 V40(E1) V21(E21) a=0, can_bisect=0
F52 V40(E1) V40(E53) a=0.314159, can_bisect=1
F52 V40(E1) V30(E72) a=0.369032, can_bisect=1
F52 V21(E21) V40(E53) a=0, can_bisect=0
F52 V21(E21) V30(E72) a=0.501032, can_bisect=1
F52 V40(E53) V30(E72) a=0, can_bisect=0
F52 best_angle = 0.501032
F59 V30(E72) V21(E6) a=0, can_bisect=0
F59 V30(E72) V40(E1) a=0.369032, can_bisect=1
F59 V30(E72) V21(E75) a=0, can_bisect=0
F59 V21(E6) V40(E1) a=0, can_bisect=0
F59 V21(E6) V21(E75) a=0.314159, can_bisect=1
F59 V40(E1) V21(E75) a=0, can_bisect=0
F59 best_angle = 0.369032
Removing V30
F62 V21(E21) V40(E45) a=0, can_bisect=0
F62 V21(E21) V21(E33) a=0.314159, can_bisect=1
F62 V21(E21) V40(E6) a=0.628319, can_bisect=1
F62 V21(E21) V21(E1) a=0.942478, can_bisect=1
F62 V21(E21) V40(E73) a=0.628319, can_bisect=1
F62 V21(E21) V21(E6) a=0.314159, can_bisect=1
F62 V21(E21) V40(E1) a=0, can_bisect=0
F62 V40(E45) V21(E33) a=0, can_bisect=0
F62 V40(E45) V40(E6) a=0.314159, can_bisect=1
F62 V40(E45) V21(E1) a=0.628319, can_bisect=1
F62 V40(E45) V40(E73) a=0.942478, can_bisect=1
F62 V40(E45) V21(E6) a=0.628319, can_bisect=1
F62 V40(E45) V40(E1) a=0.314159, can_bisect=1
F62 V21(E33) V40(E6) a=0, can_bisect=0
F62 V21(E33) V21(E1) a=0.314159, can_bisect=1
F62 V21(E33) V40(E73) a=0.628319, can_bisect=1
F62 V21(E33) V21(E6) a=0.942478, can_bisect=1
F62 V21(E33) V40(E1) a=0.628319, can_bisect=1
F62 V40(E6) V21(E1) a=0, can_bisect=0
F62 V40(E6) V40(E73) a=0.314159, can_bisect=1
F62 V40(E6) V21(E6) a=0.628319, can_bisect=1
F62 V40(E6) V40(E1) a=0.942478, can_bisect=1
F62 V21(E1) V40(E73) a=0, can_bisect=0
F62 V21(E1) V21(E6) a=0.314159, can_bisect=1
F62 V21(E1) V40(E1) a=1.25664, can_bisect=1
F62 V40(E73) V21(E6) a=0, can_bisect=0
F62 V40(E73) V40(E1) a=0.314159, can_bisect=1
F62 V21(E6) V40(E1) a=0, can_bisect=0
F62 best_angle = 1.25664
F64 V40(E1) V21(E21) a=0, can_bisect=0
F64 V40(E1) V40(E45) a=0.314159, can_bisect=1
F64 V40(E1) V21(E33) a=0.628319, can_bisect=1
F64 V40(E1) V40(E6) a=0.314159, can_bisect=1
F64 V40(E1) V21(E77) a=0, can_bisect=0
F64 V21(E21) V40(E45) a=0, can_bisect=0
F64 V21(E21) V21(E33) a=0.314159, can_bisect=1
F64 V21(E21) V40(E6) a=0.628319, can_bisect=1
F64 V21(E21) V21(E77) a=0.314159, can_bisect=1
F64 V40(E45) V21(E33) a=0, can_bisect=0
F64 V40(E45) V40(E6) a=0.314159, can_bisect=1
F64 V40(E45) V21(E77) a=0.628319, can_bisect=1
F64 V21(E33) V40(E6) a=0, can_bisect=0
F64 V21(E33) V21(E77) a=0.314159, can_bisect=1
F64 V40(E6) V21(E77) a=0, can_bisect=0
F64 best_angle = 0.628319
F66 V40(E6) V21(E77) a=0, can_bisect=0
F66 V40(E6) V40(E1) a=0.314159, can_bisect=1
F66 V40(E6) V21(E78) a=0, can_bisect=0
F66 V21(E77) V40(E1) a=0, can_bisect=0
F66 V21(E77) V21(E78) a=0.314159, can_bisect=1
F66 V40(E1) V21(E78) a=0, can_bisect=0
F66 best_angle = 0.314159
F65 V21(E21) V40(E45) a=0, can_bisect=0
F65 V21(E21) V21(E33) a=0.314159, can_bisect=1
F65 V21(E21) V40(E78) a=0, can_bisect=0
F65 V40(E45) V21(E33) a=0, can_bisect=0
F65 V40(E45) V40(E78) a=0.314159, can_bisect=1
F65 V21(E33) V40(E78) a=0, can_bisect=0
F65 best_angle = 0.314159
F63 V21(E1) V40(E73) a=0, can_bisect=0
F63 V21(E1) V21(E6) a=0.314159, can_bisect=1
F63 V21(E1) V40(E77) a=0, can_bisect=0
F63 V40(E73) V21(E6) a=0, can_bisect=0
F63 V40(E73) V40(E77) a=0.314159, can_bisect=1
F63 V21(E6) V40(E77) a=0, can_bisect=0
F63 best_angle = 0.314159
Longest Edge: 8.27941
Looking for shortest saddle...2.55848
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 -1 1 -1 1)
#Field generator embedding: (0.809016994374947450.58778525229247314
#Params:1
#t1=3.1397030519560873
# Vertices: V21 V40
# UEdges: 
E1 ( V21 V40 F71 F68 ) (-0.790612931257776652138602,2.433256402809929497266239)
E6 ( V40 V21 F67 F72 ) (0.790612931257776763160905,2.433256402809929497266239)
E21 ( V40 V21 F56 F69 ) (-2.069851525978043671472051,1.503835160279842098063341)
E33 ( V21 V40 F70 F57 ) (2.069851525978043671472051,1.503835160279842098063341)
E45 ( V21 V40 F56 F69 ) (-2.558477189440534260711502,-0.000000000000000000000000)
E73 ( V40 V21 F71 F57 ) (-6.698180241396620715477184,0.000000000000000000000000)
E74 ( V40 V40 F57 F56 ) (-4.628328715418577488094343,1.503835160279842098063341)
E77 ( V21 V40 F67 F72 ) (-8.279406103912174685888203,0.000000000000000000000000)
E78 ( V21 V40 F68 F70 ) (6.698180241396620715477184,0.000000000000000000000000)
E79 ( V40 V40 F68 F67 ) (-7.488793172654397700682694,2.433256402809929497266239)
E80 ( V21 V21 F70 F69 ) (4.628328715418577488094343,-1.503835160279842098063341)
E81 ( V21 V21 F72 F71 ) (-7.488793172654397700682694,2.433256402809929497266239)
# Faces: 
F56 +E21 +E45 -E74
F57 -E33 -E73 +E74
F67 +E6 +E77 -E79
F68 -E1 +E78 +E79
F69 -E21 -E45 -E80
F70 +E33 -E78 +E80
F71 +E1 +E73 -E81
F72 -E6 -E77 +E81
#UEdges Algebraic 
#E1: (-1 1 0 0)+(-1 1 0 0)t1
#E6: (0 1 -1 1)+(0 1 -1 1)t1
#E21: (0 -1 1 0)+(0 -1 1 0)t1
#E33: (1 -1 1 0)+(1 -1 1 0)t1
#E45: (0 0 -1 1)+(0 0 -1 1)t1
#E73: (-1 0 -1 1)+(-1 0 -1 1)t1
#E74: (0 -1 0 1)+(0 -1 0 1)t1
#E77: (-2 0 0 0)+(-2 0 0 0)t1
#E78: (1 0 1 -1)+(1 0 1 -1)t1
#E79: (-2 1 -1 1)+(-2 1 -1 1)t1
#E80: (0 1 0 -1)+(0 1 0 -1)t1
#E81: (-2 1 -1 1)+(-2 1 -1 1)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V21(12) (4 PI) V40(12) (4 PI) 
# edges: 12
## E1(2) E6(2) E21(2) E33(2) E45(2) E73(2) E74(2) E77(2) E78(2) E79(2) E80(2) E81(2) 
# faces: 8
# genus: 2
# area: 50.3648
sweeping start V21 depth = 70.9682
File =  depth = 70.9682 follow_depth = 354.841 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 48 unusual 0 close 172970 reject 0
V21: AdBc V40: aCbD   cylinders |1|1.61803|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.72788 1.05042 sym = 4
    raw = 1320 short saddle = 2.55848 short cyl = 9.25666 tag #0
V21: AaBb V40: CdDc   cylinders |1|1.61803|2.61803|
Moduli: |1|2|1| Min Area: 1 Max Area: 1
    total: 0.916298 0.557042 sym = 4
    raw = 700 short saddle = 4.86651 short cyl = 4.86651 tag #1

total cylinders: 6.20465 3.77197 raw = 1185 rejected = 0
""",
    (1, 5, 2, 4): r"""
max loc = 1
alpha =  5 beta = 2 gamma = 4 delta =1
a =  2.61799 b = 1.0472 c= 2.0944 d=0.523599
a1_min = 0 a1_max = 2.0944
a1 = 1.42773
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-2.40095,1.38619)
Imaginary of t_cx = 0
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(0 -1 0 1)t1
Checking p_alt (0,0)
Checking q_alt (1.08247e-15,-4.44089e-16)
Checking r_alt (8.88178e-16,2.22045e-16)
Checking p_algt (0,0)
Checking q_algt (1.08247e-15,-4.44089e-16)
Checking r_algt (8.88178e-16,2.22045e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.7723780188594627
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741955515488144,1.386189009429731333611358)
E2 ( V2 V3 F0 NULL ) (-2.600633195483911919154707,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (2.400949793225867434642851,-1.386189009429731333611358)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 0 0 0)+(0 -1/3 0 2/3)t1
#E2: (-1 0 0 0)+(0 -2/3 0 1/3)t1
#E3: (0 0 0 0)+(0 1 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.136568, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F25 V10(E5) V36(E25) a=0, can_bisect=0
F25 V10(E5) V10(E47) a=0.90413, can_bisect=1
F25 V10(E5) V36(E37) a=0.666667, can_bisect=1
F25 V10(E5) V10(E11) a=0.90413, can_bisect=1
F25 V10(E5) V36(E4) a=0, can_bisect=0
F25 V36(E25) V10(E47) a=0, can_bisect=0
F25 V36(E25) V36(E37) a=0.143068, can_bisect=1
F25 V36(E25) V10(E11) a=0.666667, can_bisect=1
F25 V36(E25) V36(E4) a=0.143068, can_bisect=1
F25 V10(E47) V36(E37) a=0, can_bisect=0
F25 V10(E47) V10(E11) a=0.90413, can_bisect=1
F25 V10(E47) V36(E4) a=0.666667, can_bisect=1
F25 V36(E37) V10(E11) a=0, can_bisect=0
F25 V36(E37) V36(E4) a=0.143068, can_bisect=1
F25 V10(E11) V36(E4) a=0, can_bisect=0
F25 best_angle = 0.90413
F27 V10(E47) V36(E37) a=0, can_bisect=0
F27 V10(E47) V10(E11) a=0.90413, can_bisect=1
F27 V10(E47) V36(E4) a=0.523599, can_bisect=1
F27 V10(E47) V10(E71) a=0, can_bisect=0
F27 V36(E37) V10(E11) a=0, can_bisect=0
F27 V36(E37) V36(E4) a=0.143068, can_bisect=1
F27 V36(E37) V10(E71) a=0.523599, can_bisect=1
F27 V10(E11) V36(E4) a=0, can_bisect=0
F27 V10(E11) V10(E71) a=0.90413, can_bisect=1
F27 V36(E4) V10(E71) a=0, can_bisect=0
F27 best_angle = 0.90413
F29 V10(E11) V36(E4) a=0, can_bisect=0
F29 V10(E11) V10(E71) a=0.90413, can_bisect=1
F29 V10(E11) V10(E72) a=0, can_bisect=0
F29 V36(E4) V10(E71) a=0, can_bisect=0
F29 V36(E4) V10(E72) a=0.523599, can_bisect=1
F29 V10(E71) V10(E72) a=0, can_bisect=0
F29 best_angle = 0.90413
Removing V15
F32 V10(E43) V28(E17) a=0, can_bisect=0
F32 V10(E43) V10(E4) a=0.143068, can_bisect=1
F32 V10(E43) V36(E5) a=0.436821, can_bisect=1
F32 V10(E43) V10(E31) a=0.666667, can_bisect=1
F32 V10(E43) V28(E53) a=0.937031, can_bisect=1
F32 V10(E43) V10(E25) a=1.19027, can_bisect=1
F32 V10(E43) V36(E47) a=0.937031, can_bisect=1
F32 V10(E43) V10(E67) a=0.666667, can_bisect=1
F32 V10(E43) V28(E59) a=0.436821, can_bisect=1
F32 V10(E43) V10(E37) a=0.143068, can_bisect=1
F32 V10(E43) V36(E11) a=0, can_bisect=0
F32 V28(E17) V10(E4) a=0, can_bisect=0
F32 V28(E17) V36(E5) a=0.380531, can_bisect=1
F32 V28(E17) V10(E31) a=0.436821, can_bisect=1
F32 V28(E17) V28(E53) a=0.90413, can_bisect=1
F32 V28(E17) V10(E25) a=0.937031, can_bisect=1
F32 V28(E17) V36(E47) a=1.42773, can_bisect=1
F32 V28(E17) V10(E67) a=0.937031, can_bisect=1
F32 V28(E17) V28(E59) a=0.90413, can_bisect=1
F32 V28(E17) V10(E37) a=0.436821, can_bisect=1
F32 V28(E17) V36(E11) a=0.380531, can_bisect=1
F32 V10(E4) V36(E5) a=0, can_bisect=0
F32 V10(E4) V10(E31) a=0.143068, can_bisect=1
F32 V10(E4) V28(E53) a=0.436821, can_bisect=1
F32 V10(E4) V10(E25) a=0.666667, can_bisect=1
F32 V10(E4) V36(E47) a=0.937031, can_bisect=1
F32 V10(E4) V10(E67) a=1.19027, can_bisect=1
F32 V10(E4) V28(E59) a=0.937031, can_bisect=1
F32 V10(E4) V10(E37) a=0.666667, can_bisect=1
F32 V10(E4) V36(E11) a=0.436821, can_bisect=1
F32 V36(E5) V10(E31) a=0, can_bisect=0
F32 V36(E5) V28(E53) a=0.380531, can_bisect=1
F32 V36(E5) V10(E25) a=0.436821, can_bisect=1
F32 V36(E5) V36(E47) a=0.90413, can_bisect=1
F32 V36(E5) V10(E67) a=0.937031, can_bisect=1
F32 V36(E5) V28(E59) a=1.42773, can_bisect=1
F32 V36(E5) V10(E37) a=0.937031, can_bisect=1
F32 V36(E5) V36(E11) a=0.90413, can_bisect=1
F32 V10(E31) V28(E53) a=0, can_bisect=0
F32 V10(E31) V10(E25) a=0.143068, can_bisect=1
F32 V10(E31) V36(E47) a=0.436821, can_bisect=1
F32 V10(E31) V10(E67) a=0.666667, can_bisect=1
F32 V10(E31) V28(E59) a=0.937031, can_bisect=1
F32 V10(E31) V10(E37) a=1.19027, can_bisect=1
F32 V10(E31) V36(E11) a=0.937031, can_bisect=1
F32 V28(E53) V10(E25) a=0, can_bisect=0
F32 V28(E53) V36(E47) a=0.380531, can_bisect=1
F32 V28(E53) V10(E67) a=0.436821, can_bisect=1
F32 V28(E53) V28(E59) a=0.90413, can_bisect=1
F32 V28(E53) V10(E37) a=0.937031, can_bisect=1
F32 V28(E53) V36(E11) a=1.42773, can_bisect=1
F32 V10(E25) V36(E47) a=0, can_bisect=0
F32 V10(E25) V10(E67) a=0.143068, can_bisect=1
F32 V10(E25) V28(E59) a=0.436821, can_bisect=1
F32 V10(E25) V10(E37) a=0.666667, can_bisect=1
F32 V10(E25) V36(E11) a=0.937031, can_bisect=1
F32 V36(E47) V10(E67) a=0, can_bisect=0
F32 V36(E47) V28(E59) a=0.380531, can_bisect=1
F32 V36(E47) V10(E37) a=0.436821, can_bisect=1
F32 V36(E47) V36(E11) a=0.90413, can_bisect=1
F32 V10(E67) V28(E59) a=0, can_bisect=0
F32 V10(E67) V10(E37) a=0.143068, can_bisect=1
F32 V10(E67) V36(E11) a=0.436821, can_bisect=1
F32 V28(E59) V10(E37) a=0, can_bisect=0
F32 V28(E59) V36(E11) a=0.380531, can_bisect=1
F32 V10(E37) V36(E11) a=0, can_bisect=0
F32 best_angle = 1.42773
F34 V36(E11) V10(E43) a=0, can_bisect=0
F34 V36(E11) V28(E17) a=0.380531, can_bisect=1
F34 V36(E11) V10(E4) a=0.436821, can_bisect=1
F34 V36(E11) V36(E5) a=0.523599, can_bisect=1
F34 V36(E11) V10(E31) a=0.270364, can_bisect=1
F34 V36(E11) V28(E74) a=0, can_bisect=0
F34 V10(E43) V28(E17) a=0, can_bisect=0
F34 V10(E43) V10(E4) a=0.143068, can_bisect=1
F34 V10(E43) V36(E5) a=0.436821, can_bisect=1
F34 V10(E43) V10(E31) a=0.666667, can_bisect=1
F34 V10(E43) V28(E74) a=0.270364, can_bisect=1
F34 V28(E17) V10(E4) a=0, can_bisect=0
F34 V28(E17) V36(E5) a=0.380531, can_bisect=1
F34 V28(E17) V10(E31) a=0.436821, can_bisect=1
F34 V28(E17) V28(E74) a=0.523599, can_bisect=1
F34 V10(E4) V36(E5) a=0, can_bisect=0
F34 V10(E4) V10(E31) a=0.143068, can_bisect=1
F34 V10(E4) V28(E74) a=0.436821, can_bisect=1
F34 V36(E5) V10(E31) a=0, can_bisect=0
F34 V36(E5) V28(E74) a=0.380531, can_bisect=1
F34 V10(E31) V28(E74) a=0, can_bisect=0
F34 best_angle = 0.666667
F36 V10(E31) V28(E74) a=0, can_bisect=0
F36 V10(E31) V36(E11) a=0.270364, can_bisect=1
F36 V10(E31) V10(E75) a=0, can_bisect=0
F36 V28(E74) V36(E11) a=0, can_bisect=0
F36 V28(E74) V10(E75) a=0.270364, can_bisect=1
F36 V36(E11) V10(E75) a=0, can_bisect=0
F36 best_angle = 0.270364
F35 V10(E43) V28(E17) a=0, can_bisect=0
F35 V10(E43) V10(E4) a=0.143068, can_bisect=1
F35 V10(E43) V36(E5) a=0.229846, can_bisect=1
F35 V10(E43) V10(E75) a=0, can_bisect=0
F35 V28(E17) V10(E4) a=0, can_bisect=0
F35 V28(E17) V36(E5) a=0.380531, can_bisect=1
F35 V28(E17) V10(E75) a=0.229846, can_bisect=1
F35 V10(E4) V36(E5) a=0, can_bisect=0
F35 V10(E4) V10(E75) a=0.143068, can_bisect=1
F35 V36(E5) V10(E75) a=0, can_bisect=0
F35 best_angle = 0.380531
F40 V36(E5) V10(E75) a=0, can_bisect=0
F40 V36(E5) V10(E43) a=0.229846, can_bisect=1
F40 V36(E5) V28(E77) a=0, can_bisect=0
F40 V10(E75) V10(E43) a=0, can_bisect=0
F40 V10(E75) V28(E77) a=0.229846, can_bisect=1
F40 V10(E43) V28(E77) a=0, can_bisect=0
F40 best_angle = 0.229846
F33 V28(E53) V10(E25) a=0, can_bisect=0
F33 V28(E53) V36(E47) a=0.380531, can_bisect=1
F33 V28(E53) V10(E67) a=0.436821, can_bisect=1
F33 V28(E53) V28(E59) a=0.523599, can_bisect=1
F33 V28(E53) V10(E37) a=0.270364, can_bisect=1
F33 V28(E53) V36(E74) a=0, can_bisect=0
F33 V10(E25) V36(E47) a=0, can_bisect=0
F33 V10(E25) V10(E67) a=0.143068, can_bisect=1
F33 V10(E25) V28(E59) a=0.436821, can_bisect=1
F33 V10(E25) V10(E37) a=0.666667, can_bisect=1
F33 V10(E25) V36(E74) a=0.270364, can_bisect=1
F33 V36(E47) V10(E67) a=0, can_bisect=0
F33 V36(E47) V28(E59) a=0.380531, can_bisect=1
F33 V36(E47) V10(E37) a=0.436821, can_bisect=1
F33 V36(E47) V36(E74) a=0.523599, can_bisect=1
F33 V10(E67) V28(E59) a=0, can_bisect=0
F33 V10(E67) V10(E37) a=0.143068, can_bisect=1
F33 V10(E67) V36(E74) a=0.436821, can_bisect=1
F33 V28(E59) V10(E37) a=0, can_bisect=0
F33 V28(E59) V36(E74) a=0.380531, can_bisect=1
F33 V10(E37) V36(E74) a=0, can_bisect=0
F33 best_angle = 0.666667
F44 V10(E37) V36(E74) a=0, can_bisect=0
F44 V10(E37) V28(E53) a=0.270364, can_bisect=1
F44 V10(E37) V10(E79) a=0, can_bisect=0
F44 V36(E74) V28(E53) a=0, can_bisect=0
F44 V36(E74) V10(E79) a=0.270364, can_bisect=1
F44 V28(E53) V10(E79) a=0, can_bisect=0
F44 best_angle = 0.270364
F43 V10(E25) V36(E47) a=0, can_bisect=0
F43 V10(E25) V10(E67) a=0.143068, can_bisect=1
F43 V10(E25) V28(E59) a=0.229846, can_bisect=1
F43 V10(E25) V10(E79) a=0, can_bisect=0
F43 V36(E47) V10(E67) a=0, can_bisect=0
F43 V36(E47) V28(E59) a=0.380531, can_bisect=1
F43 V36(E47) V10(E79) a=0.229846, can_bisect=1
F43 V10(E67) V28(E59) a=0, can_bisect=0
F43 V10(E67) V10(E79) a=0.143068, can_bisect=1
F43 V28(E59) V10(E79) a=0, can_bisect=0
F43 best_angle = 0.380531
F48 V28(E59) V10(E79) a=0, can_bisect=0
F48 V28(E59) V10(E25) a=0.229846, can_bisect=1
F48 V28(E59) V36(E81) a=0, can_bisect=0
F48 V10(E79) V10(E25) a=0, can_bisect=0
F48 V10(E79) V36(E81) a=0.229846, can_bisect=1
F48 V10(E25) V36(E81) a=0, can_bisect=0
F48 best_angle = 0.229846
Removing V21
F51 V28(E31) V10(E53) a=0, can_bisect=0
F51 V28(E31) V28(E67) a=0.143068, can_bisect=1
F51 V28(E31) V10(E59) a=0.666667, can_bisect=1
F51 V28(E31) V28(E43) a=0.143068, can_bisect=1
F51 V28(E31) V10(E17) a=0, can_bisect=0
F51 V10(E53) V28(E67) a=0, can_bisect=0
F51 V10(E53) V10(E59) a=0.90413, can_bisect=1
F51 V10(E53) V28(E43) a=0.666667, can_bisect=1
F51 V10(E53) V10(E17) a=0.90413, can_bisect=1
F51 V28(E67) V10(E59) a=0, can_bisect=0
F51 V28(E67) V28(E43) a=0.143068, can_bisect=1
F51 V28(E67) V10(E17) a=0.666667, can_bisect=1
F51 V10(E59) V28(E43) a=0, can_bisect=0
F51 V10(E59) V10(E17) a=0.90413, can_bisect=1
F51 V28(E43) V10(E17) a=0, can_bisect=0
F51 best_angle = 0.90413
F53 V10(E17) V28(E31) a=0, can_bisect=0
F53 V10(E17) V10(E53) a=0.90413, can_bisect=1
F53 V10(E17) V28(E67) a=0.523599, can_bisect=1
F53 V10(E17) V10(E83) a=0, can_bisect=0
F53 V28(E31) V10(E53) a=0, can_bisect=0
F53 V28(E31) V28(E67) a=0.143068, can_bisect=1
F53 V28(E31) V10(E83) a=0.523599, can_bisect=1
F53 V10(E53) V28(E67) a=0, can_bisect=0
F53 V10(E53) V10(E83) a=0.90413, can_bisect=1
F53 V28(E67) V10(E83) a=0, can_bisect=0
F53 best_angle = 0.90413
F55 V10(E53) V28(E67) a=0, can_bisect=0
F55 V10(E53) V10(E83) a=0.90413, can_bisect=1
F55 V10(E53) V10(E84) a=0, can_bisect=0
F55 V28(E67) V10(E83) a=0, can_bisect=0
F55 V28(E67) V10(E84) a=0.523599, can_bisect=1
F55 V10(E83) V10(E84) a=0, can_bisect=0
F55 best_angle = 0.90413
Extra: Flipping E75
Extra: Flipping E76
Extra: Flipping E78
Extra: Flipping E79
Extra: Flipping E80
Extra: Flipping E82
Longest Edge: 5.20127
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 3
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.7723780188594627
# Vertices: V10 V28 V36
# UEdges: 
E4 ( V10 V36 F39 F30 ) (0.599050206774133453535569,4.158567028289193778789468)
E5 ( V10 V36 F26 F63 ) (0.599050206774131122067217,-4.158567028289192890611048)
E11 ( V10 V36 F30 F59 ) (-3.900949793225866990553641,1.560490816935878211069166)
E17 ( V28 V10 F39 F54 ) (3.301899586451735757464121,2.598076211353315567720301)
E25 ( V36 V10 F26 F65 ) (3.900949793225867878732060,1.560490816935877544935352)
E31 ( V10 V28 F63 F54 ) (-3.301899586451734425196491,2.598076211353316011809511)
E37 ( V36 V10 F28 F69 ) (-3.301899586451734425196491,2.598076211353316011809511)
E43 ( V10 V28 F59 F52 ) (3.900949793225867878732060,1.560490816935877544935352)
E47 ( V10 V36 F28 F47 ) (3.301899586451735757464121,2.598076211353315567720301)
E53 ( V28 V10 F65 F56 ) (-3.900949793225866990553641,1.560490816935878211069166)
E59 ( V28 V10 F69 F52 ) (0.599050206774131122067217,-4.158567028289192890611048)
E67 ( V10 V28 F47 F56 ) (-0.599050206774133453535569,-4.158567028289193778789468)
E71 ( V10 V10 F31 F26 ) (4.499999999999998223643161,-2.598076211353316011809511)
E72 ( V10 V10 F31 F28 ) (0.000000000000001332267630,5.196152422706632023619022)
E73 ( V10 V10 F31 F30 ) (-4.500000000000000888178420,-2.598076211353316011809511)
E74 ( V28 V36 F60 F66 ) (-7.801899586451739310177800,-13.513286479285017804841118)
E77 ( V28 V36 F62 F39 ) (3.900949793225869210999690,6.756643239642509790598979)
E81 ( V36 V28 F68 F47 ) (-3.900949793225869210999690,-6.756643239642509790598979)
E83 ( V10 V10 F57 F52 ) (-4.499999999999998223643161,2.598076211353316011809511)
E84 ( V10 V10 F57 F54 ) (-0.000000000000001332267630,-5.196152422706632023619022)
E85 ( V10 V10 F57 F56 ) (4.500000000000000888178420,2.598076211353316011809511)
E86 ( V36 V28 F60 F59 ) (7.801899586451735757464121,-0.000000000000000461693686)
E87 ( V28 V28 F60 F62 ) (0.000000000000003640736062,13.513286479285019581197957)
E88 ( V36 V28 F62 F63 ) (-3.900949793225866102375221,6.756643239642509790598979)
E89 ( V28 V36 F66 F65 ) (-7.801899586451735757464121,0.000000000000000461693686)
E90 ( V36 V36 F66 F68 ) (-0.000000000000003640736062,-13.513286479285019581197957)
E91 ( V28 V36 F68 F69 ) (3.900949793225866102375221,-6.756643239642509790598979)
# Faces: 
F26 +E5 +E25 -E71
F28 +E47 +E37 -E72
F30 +E11 -E4 -E73
F31 +E71 +E72 +E73
F39 +E17 +E4 -E77
F47 -E47 +E67 -E81
F52 -E59 -E43 -E83
F54 -E17 -E31 -E84
F56 -E53 -E67 -E85
F57 +E83 +E84 +E85
F59 -E86 -E11 +E43
F60 +E87 +E74 +E86
F62 +E88 -E87 +E77
F63 -E88 -E5 +E31
F65 -E89 +E53 -E25
F66 +E90 -E74 +E89
F68 +E91 -E90 +E81
F69 -E91 +E59 -E37
#UEdges Algebraic 
#E4: (3 0 0 0)+(0 -1 0 2)t1
#E5: (3 0 0 0)+(0 -1 0 -1)t1
#E11: (0 0 -3 0)+(0 -1 0 2)t1
#E17: (-3 0 3 0)+(0 2 0 -1)t1
#E25: (3 0 -3 0)+(0 1 0 1)t1
#E31: (0 0 3 0)+(0 -2 0 1)t1
#E37: (0 0 3 0)+(0 -2 0 1)t1
#E43: (3 0 -3 0)+(0 1 0 1)t1
#E47: (-3 0 3 0)+(0 2 0 -1)t1
#E53: (0 0 -3 0)+(0 -1 0 2)t1
#E59: (3 0 0 0)+(0 -1 0 -1)t1
#E67: (-3 0 0 0)+(0 1 0 -2)t1
#E71: (6 0 -3 0)+(0 0 0 0)t1
#E72: (-3 0 6 0)+(0 0 0 0)t1
#E73: (-3 0 -3 0)+(0 0 0 0)t1
#E74: (0 0 -6 0)+(0 -2 0 -2)t1
#E77: (0 0 3 0)+(0 1 0 1)t1
#E81: (0 0 -3 0)+(0 -1 0 -1)t1
#E83: (-6 0 3 0)+(0 0 0 0)t1
#E84: (3 0 -6 0)+(0 0 0 0)t1
#E85: (3 0 3 0)+(0 0 0 0)t1
#E86: (3 0 0 0)+(0 2 0 -1)t1
#E87: (-3 0 6 0)+(0 0 0 3)t1
#E88: (-3 0 3 0)+(0 -1 0 2)t1
#E89: (-3 0 0 0)+(0 -2 0 1)t1
#E90: (3 0 -6 0)+(0 0 0 -3)t1
#E91: (3 0 -3 0)+(0 1 0 -2)t1
# File =  perturb = 0 rescale = 1
# vertices: 3
## V10(24) (10 PI) V28(15) (4 PI) V36(15) (4 PI) 
# edges: 27
## E4(2) E5(2) E11(2) E17(2) E25(2) E31(2) E37(2) E43(2) E47(2) E53(2) E59(2) E67(2) E71(2) E72(2) E73(2) E74(2) E77(2) E81(2) E83(2) E84(2) E85(2) E86(2) E87(2) E88(2) E89(2) E90(2) E91(2) 
# faces: 18
# genus: 4
# area: 269.523
sweeping start V10 depth = 54.7238
File =  depth = 54.7238 follow_depth = 273.619 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 50 unusual 0 close 38660 reject 0
V10: AXBXCcXbXa V28: DdXX V36: EXXe   cylinders |1|1|
Moduli: |1|1| Min Area: 0.0436706 Max Area: 0.571857
    total: 0.942478 0.572958 sym = 6
    raw = 120 short saddle = 32.0062 short cyl = 32.0062 tag #3
V10: AaBbXcCdDX   cylinders |1|1|
Moduli: |1|1| Min Area: 0.277729 Max Area: 0.277729
    total: 0.565487 0.343775 sym = 8
    raw = 96 short saddle = 19.0834 short cyl = 19.0834 tag #6
V10: AeBdCcDfEa V36: bIgH V28: FhGi   cylinders |1|1|1.61548|1.61548|
Moduli: |1|1|1.0306|1.0306| Min Area: 1 Max Area: 1
    total: 0.141372 0.0859437 sym = 10
    raw = 30 short saddle = 7.8019 short cyl = 23.4057 tag #5
V10: AdBcCbDaEe V28: FfGg V36: HiIh   cylinders |1|1|1.62475|1.62475|2.62475|2.62475|
Moduli: |1|1|3.39245|3.39245|2.48095|2.48095| Min Area: 1 Max Area: 1
    total: 0.141372 0.0859437 sym = 10
    raw = 30 short saddle = 5.19615 short cyl = 8.31713 tag #8

total cylinders: 5.56062 3.38045 raw = 118 rejected = 0
""",
    (1, 6, 1, 4): r"""
max loc = 1
alpha =  6 beta = 1 gamma = 4 delta =1
a =  3.14159 b = 0.523599 c= 2.0944 d=0.523599
a1_min = 0.523599 a1_max = 2.61799
a1 = 1.95133
angle a check: 0
angle b check: 1.11022e-16
angle c check: 3.14159
angle d check: -2.22045e-16
p = 1 q = 0.808578 p-q = 1.50148
p = (1,0) q = (-0.300317,0.750738) r = (-1.60063,1.66697e-16)
Imaginary of t_cx = -1.66697e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (7.21645e-16,-2.22045e-16)
Checking r_alt (0,-1.66697e-16)
Checking p_algt (0,0)
Checking q_algt (7.21645e-16,-2.22045e-16)
Checking r_algt (0,-1.66697e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.6006331954839121
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-1.300316597741955959577353,0.750738137738056643399887)
E2 ( V2 V3 F0 NULL ) (-1.300316597741956403666563,-0.750738137738056643399887)
E3 ( V3 V0 F0 NULL ) (1.600633195483912141199312,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (-2/3 0 1/3 0)+(-2/3 0 1/3 0)t1
#E2: (-1/3 0 -1/3 0)+(-1/3 0 -1/3 0)t1
#E3: (0 0 0 0)+(1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V0
F25 V15(E10) V5(E6) a=0, can_bisect=0
F25 V15(E10) V38(E1) a=0.523599, can_bisect=1
F25 V15(E10) V2(E2) a=0, can_bisect=0
F25 V5(E6) V38(E1) a=0, can_bisect=0
F25 V5(E6) V2(E2) a=1.0472, can_bisect=1
F25 V38(E1) V2(E2) a=0, can_bisect=0
F25 best_angle = 1.0472
Removing V10
F28 V5(E10) V15(E16) a=0, can_bisect=0
F28 V5(E10) V2(E1) a=1.0472, can_bisect=1
F28 V5(E10) V38(E33) a=0, can_bisect=0
F28 V15(E16) V2(E1) a=0, can_bisect=0
F28 V15(E16) V38(E33) a=0.523599, can_bisect=1
F28 V2(E1) V38(E33) a=0, can_bisect=0
F28 best_angle = 1.0472
Removing V15
F31 V5(E71) V2(E17) a=0, can_bisect=0
F31 V5(E71) V17(E25) a=0.229846, can_bisect=1
F31 V5(E71) V5(E47) a=0.523599, can_bisect=1
F31 V5(E71) V40(E43) a=0.776834, can_bisect=1
F31 V5(E71) V2(E59) a=1.0472, can_bisect=1
F31 V5(E71) V58(E67) a=0.776834, can_bisect=1
F31 V5(E71) V5(E53) a=0.523599, can_bisect=1
F31 V5(E71) V36(E37) a=0.229846, can_bisect=1
F31 V5(E71) V2(E72) a=0, can_bisect=0
F31 V2(E17) V17(E25) a=0, can_bisect=0
F31 V2(E17) V5(E47) a=0.380531, can_bisect=1
F31 V2(E17) V40(E43) a=0.436821, can_bisect=1
F31 V2(E17) V2(E59) a=0.90413, can_bisect=1
F31 V2(E17) V58(E67) a=0.937031, can_bisect=1
F31 V2(E17) V5(E53) a=1.0472, can_bisect=1
F31 V2(E17) V36(E37) a=0.776834, can_bisect=1
F31 V2(E17) V2(E72) a=0.523599, can_bisect=1
F31 V17(E25) V5(E47) a=0, can_bisect=0
F31 V17(E25) V40(E43) a=0.143068, can_bisect=1
F31 V17(E25) V2(E59) a=0.436821, can_bisect=1
F31 V17(E25) V58(E67) a=0.666667, can_bisect=1
F31 V17(E25) V5(E53) a=0.937031, can_bisect=1
F31 V17(E25) V36(E37) a=1.19027, can_bisect=1
F31 V17(E25) V2(E72) a=0.776834, can_bisect=1
F31 V5(E47) V40(E43) a=0, can_bisect=0
F31 V5(E47) V2(E59) a=0.380531, can_bisect=1
F31 V5(E47) V58(E67) a=0.436821, can_bisect=1
F31 V5(E47) V5(E53) a=0.90413, can_bisect=1
F31 V5(E47) V36(E37) a=0.937031, can_bisect=1
F31 V5(E47) V2(E72) a=1.0472, can_bisect=1
F31 V40(E43) V2(E59) a=0, can_bisect=0
F31 V40(E43) V58(E67) a=0.143068, can_bisect=1
F31 V40(E43) V5(E53) a=0.436821, can_bisect=1
F31 V40(E43) V36(E37) a=0.666667, can_bisect=1
F31 V40(E43) V2(E72) a=0.937031, can_bisect=1
F31 V2(E59) V58(E67) a=0, can_bisect=0
F31 V2(E59) V5(E53) a=0.380531, can_bisect=1
F31 V2(E59) V36(E37) a=0.436821, can_bisect=1
F31 V2(E59) V2(E72) a=0.90413, can_bisect=1
F31 V58(E67) V5(E53) a=0, can_bisect=0
F31 V58(E67) V36(E37) a=0.143068, can_bisect=1
F31 V58(E67) V2(E72) a=0.436821, can_bisect=1
F31 V5(E53) V36(E37) a=0, can_bisect=0
F31 V5(E53) V2(E72) a=0.380531, can_bisect=1
F31 V36(E37) V2(E72) a=0, can_bisect=0
F31 best_angle = 1.19027
F33 V36(E37) V2(E72) a=0, can_bisect=0
F33 V36(E37) V5(E71) a=0.229846, can_bisect=1
F33 V36(E37) V2(E17) a=0.253235, can_bisect=1
F33 V36(E37) V17(E73) a=0, can_bisect=0
F33 V2(E72) V5(E71) a=0, can_bisect=0
F33 V2(E72) V2(E17) a=0.523599, can_bisect=1
F33 V2(E72) V17(E73) a=0.253235, can_bisect=1
F33 V5(E71) V2(E17) a=0, can_bisect=0
F33 V5(E71) V17(E73) a=0.229846, can_bisect=1
F33 V2(E17) V17(E73) a=0, can_bisect=0
F33 best_angle = 0.523599
F35 V2(E17) V17(E73) a=0, can_bisect=0
F35 V2(E17) V36(E37) a=0.253235, can_bisect=1
F35 V2(E17) V2(E74) a=0, can_bisect=0
F35 V17(E73) V36(E37) a=0, can_bisect=0
F35 V17(E73) V2(E74) a=0.253235, can_bisect=1
F35 V36(E37) V2(E74) a=0, can_bisect=0
F35 best_angle = 0.253235
F32 V17(E25) V5(E47) a=0, can_bisect=0
F32 V17(E25) V40(E43) a=0.143068, can_bisect=1
F32 V17(E25) V2(E59) a=0.436821, can_bisect=1
F32 V17(E25) V58(E67) a=0.523599, can_bisect=1
F32 V17(E25) V5(E53) a=0.253235, can_bisect=1
F32 V17(E25) V36(E73) a=0, can_bisect=0
F32 V5(E47) V40(E43) a=0, can_bisect=0
F32 V5(E47) V2(E59) a=0.380531, can_bisect=1
F32 V5(E47) V58(E67) a=0.436821, can_bisect=1
F32 V5(E47) V5(E53) a=0.90413, can_bisect=1
F32 V5(E47) V36(E73) a=0.253235, can_bisect=1
F32 V40(E43) V2(E59) a=0, can_bisect=0
F32 V40(E43) V58(E67) a=0.143068, can_bisect=1
F32 V40(E43) V5(E53) a=0.436821, can_bisect=1
F32 V40(E43) V36(E73) a=0.523599, can_bisect=1
F32 V2(E59) V58(E67) a=0, can_bisect=0
F32 V2(E59) V5(E53) a=0.380531, can_bisect=1
F32 V2(E59) V36(E73) a=0.436821, can_bisect=1
F32 V58(E67) V5(E53) a=0, can_bisect=0
F32 V58(E67) V36(E73) a=0.143068, can_bisect=1
F32 V5(E53) V36(E73) a=0, can_bisect=0
F32 best_angle = 0.90413
F39 V5(E53) V36(E73) a=0, can_bisect=0
F39 V5(E53) V17(E25) a=0.253235, can_bisect=1
F39 V5(E53) V5(E76) a=0, can_bisect=0
F39 V36(E73) V17(E25) a=0, can_bisect=0
F39 V36(E73) V5(E76) a=0.253235, can_bisect=1
F39 V17(E25) V5(E76) a=0, can_bisect=0
F39 best_angle = 0.253235
F38 V5(E47) V40(E43) a=0, can_bisect=0
F38 V5(E47) V2(E59) a=0.380531, can_bisect=1
F38 V5(E47) V58(E67) a=0.293753, can_bisect=1
F38 V5(E47) V5(E76) a=0, can_bisect=0
F38 V40(E43) V2(E59) a=0, can_bisect=0
F38 V40(E43) V58(E67) a=0.143068, can_bisect=1
F38 V40(E43) V5(E76) a=0.293753, can_bisect=1
F38 V2(E59) V58(E67) a=0, can_bisect=0
F38 V2(E59) V5(E76) a=0.380531, can_bisect=1
F38 V58(E67) V5(E76) a=0, can_bisect=0
F38 best_angle = 0.380531
F43 V2(E59) V58(E67) a=0, can_bisect=0
F43 V2(E59) V5(E76) a=0.380531, can_bisect=1
F43 V2(E59) V5(E78) a=0, can_bisect=0
F43 V58(E67) V5(E76) a=0, can_bisect=0
F43 V58(E67) V5(E78) a=0.229846, can_bisect=1
F43 V5(E76) V5(E78) a=0, can_bisect=0
F43 best_angle = 0.380531
Removing V17
F46 V36(E75) V2(E21) a=0, can_bisect=0
F46 V36(E75) V38(E6) a=0.253235, can_bisect=1
F46 V36(E75) V5(E76) a=0.253235, can_bisect=1
F46 V36(E75) V5(E53) a=0, can_bisect=0
F46 V2(E21) V38(E6) a=0, can_bisect=0
F46 V2(E21) V5(E76) a=1.0472, can_bisect=1
F46 V2(E21) V5(E53) a=0.270364, can_bisect=1
F46 V38(E6) V5(E76) a=0, can_bisect=0
F46 V38(E6) V5(E53) a=0.190126, can_bisect=1
F46 V5(E76) V5(E53) a=0, can_bisect=0
F46 best_angle = 1.0472
F48 V5(E76) V5(E53) a=0, can_bisect=0
F48 V5(E76) V36(E75) a=0.253235, can_bisect=1
F48 V5(E76) V2(E80) a=0, can_bisect=0
F48 V5(E53) V36(E75) a=0, can_bisect=0
F48 V5(E53) V2(E80) a=0.270364, can_bisect=1
F48 V36(E75) V2(E80) a=0, can_bisect=0
F48 best_angle = 0.270364
Removing V36
F51 V5(E33) V38(E39) a=0, can_bisect=0
F51 V5(E33) V2(E74) a=1.0472, can_bisect=1
F51 V5(E33) V2(E81) a=0, can_bisect=0
F51 V38(E39) V2(E74) a=0, can_bisect=0
F51 V38(E39) V2(E81) a=0.190126, can_bisect=1
F51 V2(E74) V2(E81) a=0, can_bisect=0
F51 best_angle = 1.0472
Removing V38
F54 V5(E72) V2(E71) a=0, can_bisect=0
F54 V5(E72) V5(E80) a=0.523599, can_bisect=1
F54 V5(E72) V2(E43) a=0.666667, can_bisect=1
F54 V5(E72) V40(E47) a=0.839063, can_bisect=1
F54 V5(E72) V5(E67) a=0.143068, can_bisect=1
F54 V5(E72) V58(E59) a=0.459649, can_bisect=1
F54 V5(E72) V2(E82) a=0, can_bisect=0
F54 V2(E71) V5(E80) a=0, can_bisect=0
F54 V2(E71) V2(E43) a=0.523599, can_bisect=1
F54 V2(E71) V40(E47) a=0.839063, can_bisect=1
F54 V2(E71) V5(E67) a=0.666667, can_bisect=1
F54 V2(E71) V58(E59) a=0.839063, can_bisect=1
F54 V2(E71) V2(E82) a=0.523599, can_bisect=1
F54 V5(E80) V2(E43) a=0, can_bisect=0
F54 V5(E80) V40(E47) a=0.459649, can_bisect=1
F54 V5(E80) V5(E67) a=0.143068, can_bisect=1
F54 V5(E80) V58(E59) a=0.839063, can_bisect=1
F54 V5(E80) V2(E82) a=0.666667, can_bisect=1
F54 V2(E43) V40(E47) a=0, can_bisect=0
F54 V2(E43) V5(E67) a=1.71386, can_bisect=0
F54 V2(E43) V58(E59) a=0.079118, can_bisect=1
F54 V2(E43) V2(E82) a=0.143068, can_bisect=1
F54 V40(E47) V5(E67) a=0, can_bisect=0
F54 V40(E47) V58(E59) a=0.90413, can_bisect=1
F54 V40(E47) V2(E82) a=0.079118, can_bisect=1
F54 V5(E67) V58(E59) a=0, can_bisect=0
F54 V5(E67) V2(E82) a=1.71386, can_bisect=0
F54 V58(E59) V2(E82) a=0, can_bisect=0
F54 best_angle = 0.90413
F56 V58(E59) V2(E82) a=0, can_bisect=0
F56 V58(E59) V5(E72) a=0.459649, can_bisect=1
F56 V58(E59) V2(E71) a=0.839063, can_bisect=1
F56 V58(E59) V5(E80) a=0.731733, can_bisect=1
F56 V58(E59) V2(E43) a=0.0639499, can_bisect=1
F56 V58(E59) V40(E83) a=0, can_bisect=0
F56 V2(E82) V5(E72) a=0, can_bisect=0
F56 V2(E82) V2(E71) a=0.523599, can_bisect=1
F56 V2(E82) V5(E80) a=0.666667, can_bisect=1
F56 V2(E82) V2(E43) a=0.143068, can_bisect=1
F56 V2(E82) V40(E83) a=0.0639499, can_bisect=1
F56 V5(E72) V2(E71) a=0, can_bisect=0
F56 V5(E72) V5(E80) a=0.523599, can_bisect=1
F56 V5(E72) V2(E43) a=0.666667, can_bisect=1
F56 V5(E72) V40(E83) a=0.731733, can_bisect=1
F56 V2(E71) V5(E80) a=0, can_bisect=0
F56 V2(E71) V2(E43) a=0.523599, can_bisect=1
F56 V2(E71) V40(E83) a=0.839063, can_bisect=1
F56 V5(E80) V2(E43) a=0, can_bisect=0
F56 V5(E80) V40(E83) a=0.459649, can_bisect=1
F56 V2(E43) V40(E83) a=0, can_bisect=0
F56 best_angle = 0.839063
F58 V2(E71) V5(E80) a=0, can_bisect=0
F58 V2(E71) V2(E43) a=0.523599, can_bisect=1
F58 V2(E71) V40(E83) a=0.416269, can_bisect=1
F58 V2(E71) V58(E84) a=0, can_bisect=0
F58 V5(E80) V2(E43) a=0, can_bisect=0
F58 V5(E80) V40(E83) a=0.459649, can_bisect=1
F58 V5(E80) V58(E84) a=0.630928, can_bisect=1
F58 V2(E43) V40(E83) a=0, can_bisect=0
F58 V2(E43) V58(E84) a=0.0639499, can_bisect=1
F58 V40(E83) V58(E84) a=0, can_bisect=0
F58 best_angle = 0.630928
F59 V5(E80) V2(E43) a=0, can_bisect=0
F59 V5(E80) V40(E83) a=0.379414, can_bisect=1
F59 V5(E80) V58(E85) a=0, can_bisect=0
F59 V2(E43) V40(E83) a=0, can_bisect=0
F59 V2(E43) V58(E85) a=0.0639499, can_bisect=1
F59 V40(E83) V58(E85) a=0, can_bisect=0
F59 best_angle = 0.379414
F57 V58(E59) V2(E82) a=0, can_bisect=0
F57 V58(E59) V5(E72) a=0.459649, can_bisect=1
F57 V58(E59) V2(E84) a=0, can_bisect=0
F57 V2(E82) V5(E72) a=0, can_bisect=0
F57 V2(E82) V2(E84) a=0.315464, can_bisect=1
F57 V5(E72) V2(E84) a=0, can_bisect=0
F57 best_angle = 0.459649
Removing V40
F65 V2(E78) V5(E67) a=0, can_bisect=0
F65 V2(E78) V58(E85) a=0.459649, can_bisect=1
F65 V2(E78) V5(E80) a=0, can_bisect=0
F65 V5(E67) V58(E85) a=0, can_bisect=0
F65 V5(E67) V5(E80) a=0.315464, can_bisect=1
F65 V58(E85) V5(E80) a=0, can_bisect=0
F65 best_angle = 0.459649
Removing V58
F68 V5(E79) V2(E82) a=0, can_bisect=0
F68 V5(E79) V5(E72) a=0.523599, can_bisect=1
F68 V5(E79) V2(E71) a=1.0472, can_bisect=1
F68 V5(E79) V5(E80) a=0.523599, can_bisect=1
F68 V5(E79) V2(E78) a=0, can_bisect=0
F68 V2(E82) V5(E72) a=0, can_bisect=0
F68 V2(E82) V2(E71) a=0.523599, can_bisect=1
F68 V2(E82) V5(E80) a=1.0472, can_bisect=1
F68 V2(E82) V2(E78) a=0.523599, can_bisect=1
F68 V5(E72) V2(E71) a=0, can_bisect=0
F68 V5(E72) V5(E80) a=0.523599, can_bisect=1
F68 V5(E72) V2(E78) a=1.0472, can_bisect=1
F68 V2(E71) V5(E80) a=0, can_bisect=0
F68 V2(E71) V2(E78) a=0.523599, can_bisect=1
F68 V5(E80) V2(E78) a=0, can_bisect=0
F68 best_angle = 1.0472
F70 V2(E71) V5(E80) a=0, can_bisect=0
F70 V2(E71) V2(E78) a=0.523599, can_bisect=1
F70 V2(E71) V5(E89) a=0, can_bisect=0
F70 V5(E80) V2(E78) a=0, can_bisect=0
F70 V5(E80) V5(E89) a=0.523599, can_bisect=1
F70 V2(E78) V5(E89) a=0, can_bisect=0
F70 best_angle = 0.523599
F69 V5(E79) V2(E82) a=0, can_bisect=0
F69 V5(E79) V5(E72) a=0.523599, can_bisect=1
F69 V5(E79) V2(E89) a=0, can_bisect=0
F69 V2(E82) V5(E72) a=0, can_bisect=0
F69 V2(E82) V2(E89) a=0.523599, can_bisect=1
F69 V5(E72) V2(E89) a=0, can_bisect=0
F69 best_angle = 0.523599
Longest Edge: 3.00295
Looking for shortest saddle...1.50148
Clearing denominators: multiplying by 3
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.6006331954839121
# Vertices: V2 V5
# UEdges: 
E71 ( V5 V2 F34 F72 ) (0.000000000000000000000000,4.504428826428339860399319)
E72 ( V5 V2 F73 F34 ) (-3.900949793225868766910480,-2.252214413214169930199660)
E74 ( V2 V2 F53 F34 ) (3.900949793225868766910480,6.756643239642509790598979)
E76 ( V5 V5 F50 F45 ) (-3.900949793225869210999690,-6.756643239642509790598979)
E78 ( V5 V2 F45 F71 ) (-3.900949793225867878732060,-2.252214413214169930199660)
E79 ( V2 V5 F45 F74 ) (-0.000000000000000999200722,-4.504428826428339860399319)
E80 ( V2 V5 F50 F71 ) (-3.900949793225866546464431,2.252214413214169930199660)
E81 ( V5 V2 F50 F53 ) (7.801899586451733981107282,4.504428826428339860399319)
E82 ( V5 V2 F53 F73 ) (3.900949793225866546464431,-2.252214413214169930199660)
E89 ( V5 V2 F72 F74 ) (-7.801899586451733092928862,4.504428826428339860399319)
E90 ( V5 V5 F72 F71 ) (7.801899586451733981107282,0.000000000000000000000000)
E91 ( V2 V2 F74 F73 ) (-7.801899586451735757464121,0.000000000000000000000000)
# Faces: 
F34 -E72 +E71 -E74
F45 -E76 +E78 +E79
F50 +E80 +E76 +E81
F53 +E74 -E81 +E82
F71 -E80 -E78 -E90
F72 +E89 -E71 +E90
F73 -E82 +E72 -E91
F74 -E89 -E79 +E91
#UEdges Algebraic 
#E71: (-1 0 2 0)+(-1 0 2 0)t1
#E72: (-1 0 -1 0)+(-1 0 -1 0)t1
#E74: (0 0 3 0)+(0 0 3 0)t1
#E76: (0 0 -3 0)+(0 0 -3 0)t1
#E78: (-1 0 -1 0)+(-1 0 -1 0)t1
#E79: (1 0 -2 0)+(1 0 -2 0)t1
#E80: (-2 0 1 0)+(-2 0 1 0)t1
#E81: (2 0 2 0)+(2 0 2 0)t1
#E82: (2 0 -1 0)+(2 0 -1 0)t1
#E89: (-4 0 2 0)+(-4 0 2 0)t1
#E90: (3 0 0 0)+(3 0 0 0)t1
#E91: (-3 0 0 0)+(-3 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V2(12) (4 PI) V5(12) (4 PI) 
# edges: 12
## E71(2) E72(2) E74(2) E76(2) E78(2) E79(2) E80(2) E81(2) E82(2) E89(2) E90(2) E91(2) 
# faces: 8
# genus: 2
# area: 105.429
sweeping start V2 depth = 34.2263
File =  depth = 34.2263 follow_depth = 171.131 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 16 unusual 0 close 21263 reject 0
V2: AcBd V5: aDbC   cylinders |1|1|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.41372 0.859437 sym = 4
    raw = 120 short saddle = 4.50443 short cyl = 13.5133 tag #0
V2: AbBa V5: CcDd   cylinders |1|1|2|
Moduli: |1|1|1| Min Area: 1 Max Area: 1
    total: 0.989602 0.601606 sym = 4
    raw = 84 short saddle = 7.8019 short cyl = 7.8019 tag #1

total cylinders: 5.79624 3.52369 raw = 123 rejected = 0
""",
    (1, 8, 3, 6): r"""
max loc = 1
alpha =  8 beta = 3 gamma = 6 delta =1
a =  2.79253 b = 1.0472 c= 2.0944 d=0.349066
a1_min = 0 a1_max = 2.0944
a1 = 1.42773
angle a check: -4.44089e-16
angle b check: 0
angle c check: 3.14159
angle d check: 4.996e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-3.80852,1.38619)
Imaginary of t_cx = -1.11022e-15
p_algt = (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0)+(0 0 -1 0 0 1)t1
Checking p_alt (0,0)
Checking q_alt (1.30451e-15,-1.33227e-15)
Checking r_alt (-8.88178e-16,2.22045e-16)
Checking p_algt (0,0)
Checking q_algt (1.30451e-15,-1.33227e-15)
Checking r_algt (-8.88178e-16,2.22045e-16)
Number field: min poly (1 0 0 -1 0 0 1)
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=4.0529455252283562
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741955293443539,1.386189009429730445432938)
E2 ( V2 V3 F0 NULL ) (-4.008206404762399088781422,0.000000000000000666133815)
E3 ( V3 V0 F0 NULL ) (3.808523002504354160180355,-1.386189009429731111566753)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
#E1: (0 0 0 0 0 0)+(0 1/3 -2/3 0 1/3 1/3)t1
#E2: (-1 0 0 0 0 0)+(0 -1/3 -1/3 0 -1/3 2/3)t1
#E3: (0 0 0 0 0 0)+(0 0 1 0 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.068398, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F37 V48(E5) V36(E25) a=0, can_bisect=0
F37 V48(E5) V48(E59) a=0.90413, can_bisect=1
F37 V48(E5) V36(E37) a=0.666667, can_bisect=1
F37 V48(E5) V48(E11) a=0.90413, can_bisect=1
F37 V48(E5) V36(E4) a=0, can_bisect=0
F37 V36(E25) V48(E59) a=0, can_bisect=0
F37 V36(E25) V36(E37) a=0.143068, can_bisect=1
F37 V36(E25) V48(E11) a=0.666667, can_bisect=1
F37 V36(E25) V36(E4) a=0.143068, can_bisect=1
F37 V48(E59) V36(E37) a=0, can_bisect=0
F37 V48(E59) V48(E11) a=0.90413, can_bisect=1
F37 V48(E59) V36(E4) a=0.666667, can_bisect=1
F37 V36(E37) V48(E11) a=0, can_bisect=0
F37 V36(E37) V36(E4) a=0.143068, can_bisect=1
F37 V48(E11) V36(E4) a=0, can_bisect=0
F37 best_angle = 0.90413
F38 V48(E5) V36(E25) a=0, can_bisect=0
F38 V48(E5) V48(E59) a=0.90413, can_bisect=1
F38 V48(E5) V36(E37) a=0.523599, can_bisect=1
F38 V48(E5) V48(E107) a=0, can_bisect=0
F38 V36(E25) V48(E59) a=0, can_bisect=0
F38 V36(E25) V36(E37) a=0.143068, can_bisect=1
F38 V36(E25) V48(E107) a=0.523599, can_bisect=1
F38 V48(E59) V36(E37) a=0, can_bisect=0
F38 V48(E59) V48(E107) a=0.90413, can_bisect=1
F38 V36(E37) V48(E107) a=0, can_bisect=0
F38 best_angle = 0.90413
F41 V48(E59) V36(E37) a=0, can_bisect=0
F41 V48(E59) V48(E107) a=0.90413, can_bisect=1
F41 V48(E59) V48(E108) a=0, can_bisect=0
F41 V36(E37) V48(E107) a=0, can_bisect=0
F41 V36(E37) V48(E108) a=0.523599, can_bisect=1
F41 V48(E107) V48(E108) a=0, can_bisect=0
F41 best_angle = 0.90413
Removing V15
F44 V48(E43) V40(E77) a=0, can_bisect=0
F44 V48(E43) V48(E49) a=0.143068, can_bisect=1
F44 V48(E43) V16(E17) a=0.327213, can_bisect=1
F44 V48(E43) V48(E4) a=0.492134, can_bisect=1
F44 V48(E43) V36(E5) a=0.671324, can_bisect=1
F44 V48(E43) V48(E31) a=0.8412, can_bisect=1
F44 V48(E43) V40(E71) a=1.01775, can_bisect=1
F44 V48(E43) V48(E55) a=1.19027, can_bisect=1
F44 V48(E43) V16(E65) a=1.3648, can_bisect=1
F44 V48(E43) V48(E25) a=1.19027, can_bisect=1
F44 V48(E43) V36(E59) a=1.01775, can_bisect=1
F44 V48(E43) V48(E97) a=0.8412, can_bisect=1
F44 V48(E43) V40(E89) a=0.671324, can_bisect=1
F44 V48(E43) V48(E103) a=0.492134, can_bisect=1
F44 V48(E43) V16(E83) a=0.327213, can_bisect=1
F44 V48(E43) V48(E37) a=0.143068, can_bisect=1
F44 V48(E43) V36(E11) a=0, can_bisect=0
F44 V40(E77) V48(E49) a=0, can_bisect=0
F44 V40(E77) V16(E17) a=0.205998, can_bisect=1
F44 V40(E77) V48(E4) a=0.327213, can_bisect=1
F44 V40(E77) V36(E5) a=0.555064, can_bisect=1
F44 V40(E77) V48(E31) a=0.671324, can_bisect=1
F44 V40(E77) V40(E71) a=0.90413, can_bisect=1
F44 V40(E77) V48(E55) a=1.01775, can_bisect=1
F44 V40(E77) V16(E65) a=1.2532, can_bisect=1
F44 V40(E77) V48(E25) a=1.3648, can_bisect=1
F44 V40(E77) V36(E59) a=1.2532, can_bisect=1
F44 V40(E77) V48(E97) a=1.01775, can_bisect=1
F44 V40(E77) V40(E89) a=0.90413, can_bisect=1
F44 V40(E77) V48(E103) a=0.671324, can_bisect=1
F44 V40(E77) V16(E83) a=0.555064, can_bisect=1
F44 V40(E77) V48(E37) a=0.327213, can_bisect=1
F44 V40(E77) V36(E11) a=0.205998, can_bisect=1
F44 V48(E49) V16(E17) a=0, can_bisect=0
F44 V48(E49) V48(E4) a=0.143068, can_bisect=1
F44 V48(E49) V36(E5) a=0.327213, can_bisect=1
F44 V48(E49) V48(E31) a=0.492134, can_bisect=1
F44 V48(E49) V40(E71) a=0.671324, can_bisect=1
F44 V48(E49) V48(E55) a=0.8412, can_bisect=1
F44 V48(E49) V16(E65) a=1.01775, can_bisect=1
F44 V48(E49) V48(E25) a=1.19027, can_bisect=1
F44 V48(E49) V36(E59) a=1.3648, can_bisect=1
F44 V48(E49) V48(E97) a=1.19027, can_bisect=1
F44 V48(E49) V40(E89) a=1.01775, can_bisect=1
F44 V48(E49) V48(E103) a=0.8412, can_bisect=1
F44 V48(E49) V16(E83) a=0.671324, can_bisect=1
F44 V48(E49) V48(E37) a=0.492134, can_bisect=1
F44 V48(E49) V36(E11) a=0.327213, can_bisect=1
F44 V16(E17) V48(E4) a=0, can_bisect=0
F44 V16(E17) V36(E5) a=0.205998, can_bisect=1
F44 V16(E17) V48(E31) a=0.327213, can_bisect=1
F44 V16(E17) V40(E71) a=0.555064, can_bisect=1
F44 V16(E17) V48(E55) a=0.671324, can_bisect=1
F44 V16(E17) V16(E65) a=0.90413, can_bisect=1
F44 V16(E17) V48(E25) a=1.01775, can_bisect=1
F44 V16(E17) V36(E59) a=1.2532, can_bisect=1
F44 V16(E17) V48(E97) a=1.3648, can_bisect=1
F44 V16(E17) V40(E89) a=1.2532, can_bisect=1
F44 V16(E17) V48(E103) a=1.01775, can_bisect=1
F44 V16(E17) V16(E83) a=0.90413, can_bisect=1
F44 V16(E17) V48(E37) a=0.671324, can_bisect=1
F44 V16(E17) V36(E11) a=0.555064, can_bisect=1
F44 V48(E4) V36(E5) a=0, can_bisect=0
F44 V48(E4) V48(E31) a=0.143068, can_bisect=1
F44 V48(E4) V40(E71) a=0.327213, can_bisect=1
F44 V48(E4) V48(E55) a=0.492134, can_bisect=1
F44 V48(E4) V16(E65) a=0.671324, can_bisect=1
F44 V48(E4) V48(E25) a=0.8412, can_bisect=1
F44 V48(E4) V36(E59) a=1.01775, can_bisect=1
F44 V48(E4) V48(E97) a=1.19027, can_bisect=1
F44 V48(E4) V40(E89) a=1.3648, can_bisect=1
F44 V48(E4) V48(E103) a=1.19027, can_bisect=1
F44 V48(E4) V16(E83) a=1.01775, can_bisect=1
F44 V48(E4) V48(E37) a=0.8412, can_bisect=1
F44 V48(E4) V36(E11) a=0.671324, can_bisect=1
F44 V36(E5) V48(E31) a=0, can_bisect=0
F44 V36(E5) V40(E71) a=0.205998, can_bisect=1
F44 V36(E5) V48(E55) a=0.327213, can_bisect=1
F44 V36(E5) V16(E65) a=0.555064, can_bisect=1
F44 V36(E5) V48(E25) a=0.671324, can_bisect=1
F44 V36(E5) V36(E59) a=0.90413, can_bisect=1
F44 V36(E5) V48(E97) a=1.01775, can_bisect=1
F44 V36(E5) V40(E89) a=1.2532, can_bisect=1
F44 V36(E5) V48(E103) a=1.3648, can_bisect=1
F44 V36(E5) V16(E83) a=1.2532, can_bisect=1
F44 V36(E5) V48(E37) a=1.01775, can_bisect=1
F44 V36(E5) V36(E11) a=0.90413, can_bisect=1
F44 V48(E31) V40(E71) a=0, can_bisect=0
F44 V48(E31) V48(E55) a=0.143068, can_bisect=1
F44 V48(E31) V16(E65) a=0.327213, can_bisect=1
F44 V48(E31) V48(E25) a=0.492134, can_bisect=1
F44 V48(E31) V36(E59) a=0.671324, can_bisect=1
F44 V48(E31) V48(E97) a=0.8412, can_bisect=1
F44 V48(E31) V40(E89) a=1.01775, can_bisect=1
F44 V48(E31) V48(E103) a=1.19027, can_bisect=1
F44 V48(E31) V16(E83) a=1.3648, can_bisect=1
F44 V48(E31) V48(E37) a=1.19027, can_bisect=1
F44 V48(E31) V36(E11) a=1.01775, can_bisect=1
F44 V40(E71) V48(E55) a=0, can_bisect=0
F44 V40(E71) V16(E65) a=0.205998, can_bisect=1
F44 V40(E71) V48(E25) a=0.327213, can_bisect=1
F44 V40(E71) V36(E59) a=0.555064, can_bisect=1
F44 V40(E71) V48(E97) a=0.671324, can_bisect=1
F44 V40(E71) V40(E89) a=0.90413, can_bisect=1
F44 V40(E71) V48(E103) a=1.01775, can_bisect=1
F44 V40(E71) V16(E83) a=1.2532, can_bisect=1
F44 V40(E71) V48(E37) a=1.3648, can_bisect=1
F44 V40(E71) V36(E11) a=1.2532, can_bisect=1
F44 V48(E55) V16(E65) a=0, can_bisect=0
F44 V48(E55) V48(E25) a=0.143068, can_bisect=1
F44 V48(E55) V36(E59) a=0.327213, can_bisect=1
F44 V48(E55) V48(E97) a=0.492134, can_bisect=1
F44 V48(E55) V40(E89) a=0.671324, can_bisect=1
F44 V48(E55) V48(E103) a=0.8412, can_bisect=1
F44 V48(E55) V16(E83) a=1.01775, can_bisect=1
F44 V48(E55) V48(E37) a=1.19027, can_bisect=1
F44 V48(E55) V36(E11) a=1.3648, can_bisect=1
F44 V16(E65) V48(E25) a=0, can_bisect=0
F44 V16(E65) V36(E59) a=0.205998, can_bisect=1
F44 V16(E65) V48(E97) a=0.327213, can_bisect=1
F44 V16(E65) V40(E89) a=0.555064, can_bisect=1
F44 V16(E65) V48(E103) a=0.671324, can_bisect=1
F44 V16(E65) V16(E83) a=0.90413, can_bisect=1
F44 V16(E65) V48(E37) a=1.01775, can_bisect=1
F44 V16(E65) V36(E11) a=1.2532, can_bisect=1
F44 V48(E25) V36(E59) a=0, can_bisect=0
F44 V48(E25) V48(E97) a=0.143068, can_bisect=1
F44 V48(E25) V40(E89) a=0.327213, can_bisect=1
F44 V48(E25) V48(E103) a=0.492134, can_bisect=1
F44 V48(E25) V16(E83) a=0.671324, can_bisect=1
F44 V48(E25) V48(E37) a=0.8412, can_bisect=1
F44 V48(E25) V36(E11) a=1.01775, can_bisect=1
F44 V36(E59) V48(E97) a=0, can_bisect=0
F44 V36(E59) V40(E89) a=0.205998, can_bisect=1
F44 V36(E59) V48(E103) a=0.327213, can_bisect=1
F44 V36(E59) V16(E83) a=0.555064, can_bisect=1
F44 V36(E59) V48(E37) a=0.671324, can_bisect=1
F44 V36(E59) V36(E11) a=0.90413, can_bisect=1
F44 V48(E97) V40(E89) a=0, can_bisect=0
F44 V48(E97) V48(E103) a=0.143068, can_bisect=1
F44 V48(E97) V16(E83) a=0.327213, can_bisect=1
F44 V48(E97) V48(E37) a=0.492134, can_bisect=1
F44 V48(E97) V36(E11) a=0.671324, can_bisect=1
F44 V40(E89) V48(E103) a=0, can_bisect=0
F44 V40(E89) V16(E83) a=0.205998, can_bisect=1
F44 V40(E89) V48(E37) a=0.327213, can_bisect=1
F44 V40(E89) V36(E11) a=0.555064, can_bisect=1
F44 V48(E103) V16(E83) a=0, can_bisect=0
F44 V48(E103) V48(E37) a=0.143068, can_bisect=1
F44 V48(E103) V36(E11) a=0.327213, can_bisect=1
F44 V16(E83) V48(E37) a=0, can_bisect=0
F44 V16(E83) V36(E11) a=0.205998, can_bisect=1
F44 V48(E37) V36(E11) a=0, can_bisect=0
F44 best_angle = 1.3648
F46 V36(E59) V48(E97) a=0, can_bisect=0
F46 V36(E59) V40(E89) a=0.205998, can_bisect=1
F46 V36(E59) V48(E103) a=0.327213, can_bisect=1
F46 V36(E59) V16(E83) a=0.555064, can_bisect=1
F46 V36(E59) V48(E37) a=0.671324, can_bisect=1
F46 V36(E59) V36(E11) a=0.523599, can_bisect=1
F46 V36(E59) V48(E43) a=0.351086, can_bisect=1
F46 V36(E59) V40(E77) a=0.174533, can_bisect=1
F46 V36(E59) V48(E110) a=0, can_bisect=0
F46 V48(E97) V40(E89) a=0, can_bisect=0
F46 V48(E97) V48(E103) a=0.143068, can_bisect=1
F46 V48(E97) V16(E83) a=0.327213, can_bisect=1
F46 V48(E97) V48(E37) a=0.492134, can_bisect=1
F46 V48(E97) V36(E11) a=0.671324, can_bisect=1
F46 V48(E97) V48(E43) a=0.8412, can_bisect=1
F46 V48(E97) V40(E77) a=1.01775, can_bisect=1
F46 V48(E97) V48(E110) a=0.174533, can_bisect=1
F46 V40(E89) V48(E103) a=0, can_bisect=0
F46 V40(E89) V16(E83) a=0.205998, can_bisect=1
F46 V40(E89) V48(E37) a=0.327213, can_bisect=1
F46 V40(E89) V36(E11) a=0.555064, can_bisect=1
F46 V40(E89) V48(E43) a=0.671324, can_bisect=1
F46 V40(E89) V40(E77) a=0.90413, can_bisect=1
F46 V40(E89) V48(E110) a=0.347046, can_bisect=1
F46 V48(E103) V16(E83) a=0, can_bisect=0
F46 V48(E103) V48(E37) a=0.143068, can_bisect=1
F46 V48(E103) V36(E11) a=0.327213, can_bisect=1
F46 V48(E103) V48(E43) a=0.492134, can_bisect=1
F46 V48(E103) V40(E77) a=0.671324, can_bisect=1
F46 V48(E103) V48(E110) a=0.523599, can_bisect=1
F46 V16(E83) V48(E37) a=0, can_bisect=0
F46 V16(E83) V36(E11) a=0.205998, can_bisect=1
F46 V16(E83) V48(E43) a=0.327213, can_bisect=1
F46 V16(E83) V40(E77) a=0.555064, can_bisect=1
F46 V16(E83) V48(E110) a=0.671324, can_bisect=1
F46 V48(E37) V36(E11) a=0, can_bisect=0
F46 V48(E37) V48(E43) a=0.143068, can_bisect=1
F46 V48(E37) V40(E77) a=0.327213, can_bisect=1
F46 V48(E37) V48(E110) a=0.492134, can_bisect=1
F46 V36(E11) V48(E43) a=0, can_bisect=0
F46 V36(E11) V40(E77) a=0.205998, can_bisect=1
F46 V36(E11) V48(E110) a=0.327213, can_bisect=1
F46 V48(E43) V40(E77) a=0, can_bisect=0
F46 V48(E43) V48(E110) a=0.143068, can_bisect=1
F46 V40(E77) V48(E110) a=0, can_bisect=0
F46 best_angle = 1.01775
F48 V40(E77) V48(E110) a=0, can_bisect=0
F48 V40(E77) V36(E59) a=0.174533, can_bisect=1
F48 V40(E77) V48(E111) a=0, can_bisect=0
F48 V48(E110) V36(E59) a=0, can_bisect=0
F48 V48(E110) V48(E111) a=0.172513, can_bisect=1
F48 V36(E59) V48(E111) a=0, can_bisect=0
F48 best_angle = 0.174533
F47 V48(E97) V40(E89) a=0, can_bisect=0
F47 V48(E97) V48(E103) a=0.143068, can_bisect=1
F47 V48(E97) V16(E83) a=0.327213, can_bisect=1
F47 V48(E97) V48(E37) a=0.492134, can_bisect=1
F47 V48(E97) V36(E11) a=0.346429, can_bisect=1
F47 V48(E97) V48(E43) a=0.176553, can_bisect=1
F47 V48(E97) V40(E111) a=0, can_bisect=0
F47 V40(E89) V48(E103) a=0, can_bisect=0
F47 V40(E89) V16(E83) a=0.205998, can_bisect=1
F47 V40(E89) V48(E37) a=0.327213, can_bisect=1
F47 V40(E89) V36(E11) a=0.555064, can_bisect=1
F47 V40(E89) V48(E43) a=0.671324, can_bisect=1
F47 V40(E89) V40(E111) a=0.172513, can_bisect=1
F47 V48(E103) V16(E83) a=0, can_bisect=0
F47 V48(E103) V48(E37) a=0.143068, can_bisect=1
F47 V48(E103) V36(E11) a=0.327213, can_bisect=1
F47 V48(E103) V48(E43) a=0.492134, can_bisect=1
F47 V48(E103) V40(E111) a=0.351703, can_bisect=1
F47 V16(E83) V48(E37) a=0, can_bisect=0
F47 V16(E83) V36(E11) a=0.205998, can_bisect=1
F47 V16(E83) V48(E43) a=0.327213, can_bisect=1
F47 V16(E83) V40(E111) a=0.521579, can_bisect=1
F47 V48(E37) V36(E11) a=0, can_bisect=0
F47 V48(E37) V48(E43) a=0.143068, can_bisect=1
F47 V48(E37) V40(E111) a=0.327213, can_bisect=1
F47 V36(E11) V48(E43) a=0, can_bisect=0
F47 V36(E11) V40(E111) a=0.205998, can_bisect=1
F47 V48(E43) V40(E111) a=0, can_bisect=0
F47 best_angle = 0.671324
F52 V48(E43) V40(E111) a=0, can_bisect=0
F52 V48(E43) V48(E97) a=0.169876, can_bisect=1
F52 V48(E43) V40(E113) a=0, can_bisect=0
F52 V40(E111) V48(E97) a=0, can_bisect=0
F52 V40(E111) V40(E113) a=0.172513, can_bisect=1
F52 V48(E97) V40(E113) a=0, can_bisect=0
F52 best_angle = 0.172513
F51 V40(E89) V48(E103) a=0, can_bisect=0
F51 V40(E89) V16(E83) a=0.205998, can_bisect=1
F51 V40(E89) V48(E37) a=0.327213, can_bisect=1
F51 V40(E89) V36(E11) a=0.169876, can_bisect=1
F51 V40(E89) V48(E113) a=0, can_bisect=0
F51 V48(E103) V16(E83) a=0, can_bisect=0
F51 V48(E103) V48(E37) a=0.143068, can_bisect=1
F51 V48(E103) V36(E11) a=0.327213, can_bisect=1
F51 V48(E103) V48(E113) a=0.17919, can_bisect=1
F51 V16(E83) V48(E37) a=0, can_bisect=0
F51 V16(E83) V36(E11) a=0.205998, can_bisect=1
F51 V16(E83) V48(E113) a=0.327213, can_bisect=1
F51 V48(E37) V36(E11) a=0, can_bisect=0
F51 V48(E37) V48(E113) a=0.143068, can_bisect=1
F51 V36(E11) V48(E113) a=0, can_bisect=0
F51 best_angle = 0.327213
F56 V36(E11) V48(E113) a=0, can_bisect=0
F56 V36(E11) V40(E89) a=0.169876, can_bisect=1
F56 V36(E11) V48(E115) a=0, can_bisect=0
F56 V48(E113) V40(E89) a=0, can_bisect=0
F56 V48(E113) V48(E115) a=0.16492, can_bisect=1
F56 V40(E89) V48(E115) a=0, can_bisect=0
F56 best_angle = 0.169876
F55 V48(E103) V16(E83) a=0, can_bisect=0
F55 V48(E103) V48(E37) a=0.143068, can_bisect=1
F55 V48(E103) V36(E115) a=0, can_bisect=0
F55 V16(E83) V48(E37) a=0, can_bisect=0
F55 V16(E83) V36(E115) a=0.16492, can_bisect=1
F55 V48(E37) V36(E115) a=0, can_bisect=0
F55 best_angle = 0.16492
F45 V48(E49) V16(E17) a=0, can_bisect=0
F45 V48(E49) V48(E4) a=0.143068, can_bisect=1
F45 V48(E49) V36(E5) a=0.327213, can_bisect=1
F45 V48(E49) V48(E31) a=0.492134, can_bisect=1
F45 V48(E49) V40(E71) a=0.671324, can_bisect=1
F45 V48(E49) V48(E55) a=0.523599, can_bisect=1
F45 V48(E49) V16(E65) a=0.347046, can_bisect=1
F45 V48(E49) V48(E25) a=0.174533, can_bisect=1
F45 V48(E49) V36(E110) a=0, can_bisect=0
F45 V16(E17) V48(E4) a=0, can_bisect=0
F45 V16(E17) V36(E5) a=0.205998, can_bisect=1
F45 V16(E17) V48(E31) a=0.327213, can_bisect=1
F45 V16(E17) V40(E71) a=0.555064, can_bisect=1
F45 V16(E17) V48(E55) a=0.671324, can_bisect=1
F45 V16(E17) V16(E65) a=0.90413, can_bisect=1
F45 V16(E17) V48(E25) a=1.01775, can_bisect=1
F45 V16(E17) V36(E110) a=0.174533, can_bisect=1
F45 V48(E4) V36(E5) a=0, can_bisect=0
F45 V48(E4) V48(E31) a=0.143068, can_bisect=1
F45 V48(E4) V40(E71) a=0.327213, can_bisect=1
F45 V48(E4) V48(E55) a=0.492134, can_bisect=1
F45 V48(E4) V16(E65) a=0.671324, can_bisect=1
F45 V48(E4) V48(E25) a=0.8412, can_bisect=1
F45 V48(E4) V36(E110) a=0.351086, can_bisect=1
F45 V36(E5) V48(E31) a=0, can_bisect=0
F45 V36(E5) V40(E71) a=0.205998, can_bisect=1
F45 V36(E5) V48(E55) a=0.327213, can_bisect=1
F45 V36(E5) V16(E65) a=0.555064, can_bisect=1
F45 V36(E5) V48(E25) a=0.671324, can_bisect=1
F45 V36(E5) V36(E110) a=0.523599, can_bisect=1
F45 V48(E31) V40(E71) a=0, can_bisect=0
F45 V48(E31) V48(E55) a=0.143068, can_bisect=1
F45 V48(E31) V16(E65) a=0.327213, can_bisect=1
F45 V48(E31) V48(E25) a=0.492134, can_bisect=1
F45 V48(E31) V36(E110) a=0.671324, can_bisect=1
F45 V40(E71) V48(E55) a=0, can_bisect=0
F45 V40(E71) V16(E65) a=0.205998, can_bisect=1
F45 V40(E71) V48(E25) a=0.327213, can_bisect=1
F45 V40(E71) V36(E110) a=0.555064, can_bisect=1
F45 V48(E55) V16(E65) a=0, can_bisect=0
F45 V48(E55) V48(E25) a=0.143068, can_bisect=1
F45 V48(E55) V36(E110) a=0.327213, can_bisect=1
F45 V16(E65) V48(E25) a=0, can_bisect=0
F45 V16(E65) V36(E110) a=0.205998, can_bisect=1
F45 V48(E25) V36(E110) a=0, can_bisect=0
F45 best_angle = 1.01775
F62 V48(E25) V36(E110) a=0, can_bisect=0
F62 V48(E25) V48(E49) a=0.172513, can_bisect=1
F62 V48(E25) V16(E118) a=0, can_bisect=0
F62 V36(E110) V48(E49) a=0, can_bisect=0
F62 V36(E110) V16(E118) a=0.174533, can_bisect=1
F62 V48(E49) V16(E118) a=0, can_bisect=0
F62 best_angle = 0.174533
F61 V16(E17) V48(E4) a=0, can_bisect=0
F61 V16(E17) V36(E5) a=0.205998, can_bisect=1
F61 V16(E17) V48(E31) a=0.327213, can_bisect=1
F61 V16(E17) V40(E71) a=0.521579, can_bisect=1
F61 V16(E17) V48(E55) a=0.351703, can_bisect=1
F61 V16(E17) V16(E65) a=0.172513, can_bisect=1
F61 V16(E17) V48(E118) a=0, can_bisect=0
F61 V48(E4) V36(E5) a=0, can_bisect=0
F61 V48(E4) V48(E31) a=0.143068, can_bisect=1
F61 V48(E4) V40(E71) a=0.327213, can_bisect=1
F61 V48(E4) V48(E55) a=0.492134, can_bisect=1
F61 V48(E4) V16(E65) a=0.671324, can_bisect=1
F61 V48(E4) V48(E118) a=0.176553, can_bisect=1
F61 V36(E5) V48(E31) a=0, can_bisect=0
F61 V36(E5) V40(E71) a=0.205998, can_bisect=1
F61 V36(E5) V48(E55) a=0.327213, can_bisect=1
F61 V36(E5) V16(E65) a=0.555064, can_bisect=1
F61 V36(E5) V48(E118) a=0.346429, can_bisect=1
F61 V48(E31) V40(E71) a=0, can_bisect=0
F61 V48(E31) V48(E55) a=0.143068, can_bisect=1
F61 V48(E31) V16(E65) a=0.327213, can_bisect=1
F61 V48(E31) V48(E118) a=0.492134, can_bisect=1
F61 V40(E71) V48(E55) a=0, can_bisect=0
F61 V40(E71) V16(E65) a=0.205998, can_bisect=1
F61 V40(E71) V48(E118) a=0.327213, can_bisect=1
F61 V48(E55) V16(E65) a=0, can_bisect=0
F61 V48(E55) V48(E118) a=0.143068, can_bisect=1
F61 V16(E65) V48(E118) a=0, can_bisect=0
F61 best_angle = 0.671324
F66 V16(E65) V48(E118) a=0, can_bisect=0
F66 V16(E65) V16(E17) a=0.172513, can_bisect=1
F66 V16(E65) V48(E120) a=0, can_bisect=0
F66 V48(E118) V16(E17) a=0, can_bisect=0
F66 V48(E118) V48(E120) a=0.169876, can_bisect=1
F66 V16(E17) V48(E120) a=0, can_bisect=0
F66 best_angle = 0.172513
F65 V48(E4) V36(E5) a=0, can_bisect=0
F65 V48(E4) V48(E31) a=0.143068, can_bisect=1
F65 V48(E4) V40(E71) a=0.327213, can_bisect=1
F65 V48(E4) V48(E55) a=0.17919, can_bisect=1
F65 V48(E4) V16(E120) a=0, can_bisect=0
F65 V36(E5) V48(E31) a=0, can_bisect=0
F65 V36(E5) V40(E71) a=0.205998, can_bisect=1
F65 V36(E5) V48(E55) a=0.327213, can_bisect=1
F65 V36(E5) V16(E120) a=0.169876, can_bisect=1
F65 V48(E31) V40(E71) a=0, can_bisect=0
F65 V48(E31) V48(E55) a=0.143068, can_bisect=1
F65 V48(E31) V16(E120) a=0.327213, can_bisect=1
F65 V40(E71) V48(E55) a=0, can_bisect=0
F65 V40(E71) V16(E120) a=0.205998, can_bisect=1
F65 V48(E55) V16(E120) a=0, can_bisect=0
F65 best_angle = 0.327213
F70 V40(E71) V48(E55) a=0, can_bisect=0
F70 V40(E71) V16(E120) a=0.205998, can_bisect=1
F70 V40(E71) V48(E122) a=0, can_bisect=0
F70 V48(E55) V16(E120) a=0, can_bisect=0
F70 V48(E55) V48(E122) a=0.16492, can_bisect=1
F70 V16(E120) V48(E122) a=0, can_bisect=0
F70 best_angle = 0.205998
F69 V48(E4) V36(E5) a=0, can_bisect=0
F69 V48(E4) V48(E31) a=0.143068, can_bisect=1
F69 V48(E4) V40(E122) a=0, can_bisect=0
F69 V36(E5) V48(E31) a=0, can_bisect=0
F69 V36(E5) V40(E122) a=0.16492, can_bisect=1
F69 V48(E31) V40(E122) a=0, can_bisect=0
F69 best_angle = 0.16492
Removing V21
F75 V16(E55) V48(E65) a=0, can_bisect=0
F75 V16(E55) V16(E103) a=0.143068, can_bisect=1
F75 V16(E55) V48(E83) a=0.666667, can_bisect=1
F75 V16(E55) V16(E49) a=0.143068, can_bisect=1
F75 V16(E55) V48(E17) a=0, can_bisect=0
F75 V48(E65) V16(E103) a=0, can_bisect=0
F75 V48(E65) V48(E83) a=0.90413, can_bisect=1
F75 V48(E65) V16(E49) a=0.666667, can_bisect=1
F75 V48(E65) V48(E17) a=0.90413, can_bisect=1
F75 V16(E103) V48(E83) a=0, can_bisect=0
F75 V16(E103) V16(E49) a=0.143068, can_bisect=1
F75 V16(E103) V48(E17) a=0.666667, can_bisect=1
F75 V48(E83) V16(E49) a=0, can_bisect=0
F75 V48(E83) V48(E17) a=0.90413, can_bisect=1
F75 V16(E49) V48(E17) a=0, can_bisect=0
F75 best_angle = 0.90413
F77 V48(E83) V16(E49) a=0, can_bisect=0
F77 V48(E83) V48(E17) a=0.90413, can_bisect=1
F77 V48(E83) V16(E55) a=0.523599, can_bisect=1
F77 V48(E83) V48(E125) a=0, can_bisect=0
F77 V16(E49) V48(E17) a=0, can_bisect=0
F77 V16(E49) V16(E55) a=0.143068, can_bisect=1
F77 V16(E49) V48(E125) a=0.523599, can_bisect=1
F77 V48(E17) V16(E55) a=0, can_bisect=0
F77 V48(E17) V48(E125) a=0.90413, can_bisect=1
F77 V16(E55) V48(E125) a=0, can_bisect=0
F77 best_angle = 0.90413
F79 V48(E17) V16(E55) a=0, can_bisect=0
F79 V48(E17) V48(E125) a=0.90413, can_bisect=1
F79 V48(E17) V48(E126) a=0, can_bisect=0
F79 V16(E55) V48(E125) a=0, can_bisect=0
F79 V16(E55) V48(E126) a=0.523599, can_bisect=1
F79 V48(E125) V48(E126) a=0, can_bisect=0
F79 best_angle = 0.90413
Removing V44
F82 V48(E77) V40(E31) a=0, can_bisect=0
F82 V48(E77) V48(E71) a=0.90413, can_bisect=1
F82 V48(E77) V40(E97) a=0.666667, can_bisect=1
F82 V48(E77) V48(E89) a=0.90413, can_bisect=1
F82 V48(E77) V40(E43) a=0, can_bisect=0
F82 V40(E31) V48(E71) a=0, can_bisect=0
F82 V40(E31) V40(E97) a=0.143068, can_bisect=1
F82 V40(E31) V48(E89) a=0.666667, can_bisect=1
F82 V40(E31) V40(E43) a=0.143068, can_bisect=1
F82 V48(E71) V40(E97) a=0, can_bisect=0
F82 V48(E71) V48(E89) a=0.90413, can_bisect=1
F82 V48(E71) V40(E43) a=0.666667, can_bisect=1
F82 V40(E97) V48(E89) a=0, can_bisect=0
F82 V40(E97) V40(E43) a=0.143068, can_bisect=1
F82 V48(E89) V40(E43) a=0, can_bisect=0
F82 best_angle = 0.90413
F84 V48(E89) V40(E43) a=0, can_bisect=0
F84 V48(E89) V48(E77) a=0.90413, can_bisect=1
F84 V48(E89) V40(E31) a=0.523599, can_bisect=1
F84 V48(E89) V48(E128) a=0, can_bisect=0
F84 V40(E43) V48(E77) a=0, can_bisect=0
F84 V40(E43) V40(E31) a=0.143068, can_bisect=1
F84 V40(E43) V48(E128) a=0.523599, can_bisect=1
F84 V48(E77) V40(E31) a=0, can_bisect=0
F84 V48(E77) V48(E128) a=0.90413, can_bisect=1
F84 V40(E31) V48(E128) a=0, can_bisect=0
F84 best_angle = 0.90413
F86 V48(E77) V40(E31) a=0, can_bisect=0
F86 V48(E77) V48(E128) a=0.90413, can_bisect=1
F86 V48(E77) V48(E129) a=0, can_bisect=0
F86 V40(E31) V48(E128) a=0, can_bisect=0
F86 V40(E31) V48(E129) a=0.523599, can_bisect=1
F86 V48(E128) V48(E129) a=0, can_bisect=0
F86 best_angle = 0.90413
Extra: Flipping E110
Extra: Flipping E111
Extra: Flipping E113
Extra: Flipping E115
Extra: Flipping E118
Extra: Flipping E120
Extra: Flipping E122
Longest Edge: 7.89463
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 3
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=4.0529455252283562
# Vertices: V16 V36 V40 V48
# UEdges: 
E4 ( V48 V36 F102 F39 ) (0.599050206774134119669384,4.158567028289191114254209)
E5 ( V48 V36 F40 F73 ) (0.599050206774134452736291,-4.158567028289192002432628)
E11 ( V48 V36 F39 F93 ) (-3.900949793225866546464431,1.560490816935875102444697)
E17 ( V16 V48 F102 F80 ) (2.214176277786664037705577,3.570709213853306085439954)
E25 ( V36 V48 F40 F97 ) (3.900949793225865214196801,1.560490816935875990623117)
E31 ( V48 V40 F73 F87 ) (-2.214176277786663593616368,3.570709213853307417707583)
E37 ( V36 V48 F42 F59 ) (-3.301899586451731760661232,2.598076211353316011809511)
E43 ( V48 V40 F93 F85 ) (4.199413027617459270857125,0.132178298093466928264661)
E49 ( V48 V16 F90 F78 ) (3.131974441883864646030133,2.800585112864161452250755)
E55 ( V16 V48 F80 F71 ) (3.991365074142807056745141,-1.312076874242936952441596)
E59 ( V48 V36 F42 F92 ) (3.301899586451733092928862,2.598076211353316011809511)
E65 ( V16 V48 F97 F76 ) (-4.199413027617459270857125,0.132178298093467261331568)
E71 ( V40 V48 F71 F83 ) (-3.131974441883863313762504,2.800585112864164116786014)
E77 ( V48 V40 F87 F90 ) (-3.991365074142807056745141,-1.312076874242935398129362)
E83 ( V16 V48 F59 F78 ) (1.985236749830795011106943,-3.702887511946773457793824)
E89 ( V48 V40 F85 F96 ) (0.859390632258943076848823,4.112661987107099292870771)
E97 ( V48 V40 F92 F83 ) (-1.985236749830796343374573,-3.702887511946775678239874)
E103 ( V16 V48 F76 F96 ) (-0.859390632258942410715008,4.112661987107097516513932)
E107 ( V48 V48 F39 F43 ) (4.500000000000001776356839,2.598076211353316011809511)
E108 ( V48 V48 F43 F40 ) (4.500000000000000000000000,-2.598076211353316011809511)
E109 ( V48 V48 F43 F42 ) (0.000000000000001332267630,5.196152422706632023619022)
E112 ( V40 V36 F91 F89 ) (-8.100362820843322708697087,22.255563939656784810949830)
E114 ( V40 V40 F94 F91 ) (-13.387499157125851922955917,15.954600216356695341346494)
E116 ( V36 V40 F95 F94 ) (-5.287136336282525661545151,14.526287697514284502631199)
E117 ( V16 V36 F95 F59 ) (5.287136336282526549723570,-6.300963723300089469603336)
E119 ( V36 V16 F98 F89 ) (15.223702336869996187829202,-18.142901952549689070792738)
E121 ( V16 V16 F99 F98 ) (7.123339516026669926418435,-19.571214471392096356794355)
E123 ( V40 V16 F99 F71 ) (-7.123339516026669926418435,4.112661987107101069227610)
E124 ( V36 V40 F101 F73 ) (-2.813226484560797935330356,7.729276242142499420140211)
E125 ( V48 V48 F81 F76 ) (3.340022395358516860142117,3.980483689013631032338481)
E126 ( V48 V48 F81 F78 ) (-5.117211191714659435092472,0.902302399082612449632279)
E127 ( V48 V48 F81 F80 ) (1.777188796356143019039564,-4.882786088096243481970760)
E128 ( V48 V48 F88 F83 ) (5.117211191714659435092472,0.902302399082611450431557)
E129 ( V48 V48 F88 F85 ) (-3.340022395358515971963698,3.980483689013632364606110)
E130 ( V48 V48 F88 F87 ) (-1.777188796356143463128774,-4.882786088096242593792340)
E131 ( V40 V16 F89 F90 ) (7.123339516026669926418435,4.112661987107095740157092)
E132 ( V36 V40 F91 F92 ) (-5.287136336282529214258830,-6.300963723300092134138595)
E133 ( V40 V36 F93 F94 ) (-8.100362820843326261410766,1.428312518842408618269246)
E134 ( V40 V16 F95 F96 ) (-0.000000000000000506213262,-8.225323974214196809384703)
E135 ( V36 V16 F97 F98 ) (8.100362820843322708697087,1.428312518842408174180036)
E136 ( V16 V40 F99 F101 ) (0.000000000000000839280169,15.458552484284998840280423)
E137 ( V16 V36 F101 F102 ) (2.813226484560797935330356,7.729276242142498531961792)
# Faces: 
F39 +E11 -E4 +E107
F40 +E5 +E25 -E108
F42 +E59 +E37 -E109
F43 -E107 +E108 +E109
F59 +E83 -E37 -E117
F71 +E71 -E55 -E123
F73 -E5 +E31 -E124
F76 -E65 +E103 -E125
F78 -E83 -E49 -E126
F80 -E17 +E55 -E127
F81 +E125 +E126 +E127
F83 -E71 -E97 -E128
F85 +E89 -E43 -E129
F87 +E77 -E31 -E130
F88 +E128 +E129 +E130
F89 +E131 -E119 -E112
F90 -E131 -E77 +E49
F91 +E132 -E114 +E112
F92 -E132 -E59 +E97
F93 +E133 -E11 +E43
F94 -E133 +E114 -E116
F95 +E134 +E117 +E116
F96 -E134 -E89 -E103
F97 +E135 +E65 -E25
F98 -E135 +E119 -E121
F99 +E136 +E123 +E121
F101 +E137 +E124 -E136
F102 -E137 +E17 +E4
#UEdges Algebraic 
#E4: (3 0 0 0 0 0)+(0 1 -2 0 1 1)t1
#E5: (3 0 0 0 0 0)+(0 -2 1 0 1 -2)t1
#E11: (0 0 0 -3 0 0)+(0 1 -2 0 1 1)t1
#E17: (0 -3 0 0 3 0)+(-1 0 1 2 0 -2)t1
#E25: (3 0 0 -3 0 0)+(0 2 -1 0 -1 2)t1
#E31: (0 0 3 0 0 0)+(-1 -1 0 2 -1 0)t1
#E37: (0 0 0 3 0 0)+(0 -1 -1 0 2 -1)t1
#E43: (0 0 0 0 0 -3)+(2 -1 0 -1 2 0)t1
#E49: (0 3 0 0 -3 0)+(-2 0 2 1 0 -1)t1
#E55: (0 0 0 0 -3 0)+(-1 0 1 2 0 -2)t1
#E59: (-3 0 0 3 0 0)+(0 1 1 0 -2 1)t1
#E65: (0 0 0 0 -3 0)+(-1 0 1 -1 0 1)t1
#E71: (0 0 -3 0 0 0)+(1 -2 0 1 1 0)t1
#E77: (0 0 0 0 0 -3)+(-1 -1 0 2 -1 0)t1
#E83: (0 3 0 0 0 0)+(2 0 -2 -1 0 1)t1
#E89: (0 0 -3 0 0 3)+(2 -1 0 -1 2 0)t1
#E97: (0 0 -3 0 0 3)+(-1 2 0 -1 -1 0)t1
#E103: (0 3 0 0 0 0)+(-1 0 1 -1 0 1)t1
#E107: (3 0 0 3 0 0)+(0 0 0 0 0 0)t1
#E108: (6 0 0 -3 0 0)+(0 0 0 0 0 0)t1
#E109: (-3 0 0 6 0 0)+(0 0 0 0 0 0)t1
#E112: (-3 0 0 3 0 3)+(-2 -1 1 1 2 1)t1
#E114: (0 0 -3 0 0 6)+(-3 0 0 0 3 0)t1
#E116: (0 0 -3 3 0 3)+(-1 -2 2 -1 4 -1)t1
#E117: (0 3 0 -3 0 0)+(2 1 -1 -1 -2 2)t1
#E119: (3 3 0 -3 -3 0)+(1 2 1 -2 -1 -2)t1
#E121: (0 3 0 0 -6 0)+(0 0 3 -3 0 -3)t1
#E123: (0 0 -3 0 3 0)+(2 -2 -1 -1 1 2)t1
#E124: (-3 0 3 0 0 0)+(-1 1 -1 2 -2 2)t1
#E125: (0 3 0 0 3 0)+(0 0 0 0 0 0)t1
#E126: (0 -6 0 0 3 0)+(0 0 0 0 0 0)t1
#E127: (0 3 0 0 -6 0)+(0 0 0 0 0 0)t1
#E128: (0 0 6 0 0 -3)+(0 0 0 0 0 0)t1
#E129: (0 0 -3 0 0 6)+(0 0 0 0 0 0)t1
#E130: (0 0 -3 0 0 -3)+(0 0 0 0 0 0)t1
#E131: (0 3 0 0 -3 3)+(-1 1 2 -1 1 -1)t1
#E132: (3 0 -3 -3 0 3)+(-1 1 -1 -1 1 -1)t1
#E133: (0 0 0 -3 0 3)+(-2 2 -2 1 -1 1)t1
#E134: (0 -3 3 0 0 -3)+(-1 1 -1 2 -2 -1)t1
#E135: (3 0 0 -3 3 0)+(1 2 -2 1 -1 1)t1
#E136: (0 -3 3 0 3 0)+(-2 2 -2 4 -1 1)t1
#E137: (3 -3 0 0 3 0)+(-1 1 -1 2 1 -1)t1
# File =  perturb = 0 rescale = 1
# vertices: 4
## V16(16) (4 PI) V36(15) (4 PI) V40(17) (4 PI) V48(36) (16 PI) 
# edges: 42
## E4(2) E5(2) E11(2) E17(2) E25(2) E31(2) E37(2) E43(2) E49(2) E55(2) E59(2) E65(2) E71(2) E77(2) E83(2) E89(2) E97(2) E103(2) E107(2) E108(2) E109(2) E112(2) E114(2) E116(2) E117(2) E119(2) E121(2) E123(2) E124(2) E125(2) E126(2) E127(2) E128(2) E129(2) E130(2) E131(2) E132(2) E133(2) E134(2) E135(2) E136(2) E137(2) 
# faces: 28
# genus: 6
# area: 562.328
sweeping start V16 depth = 79.0449
File =  depth = 79.0449 follow_depth = 395.224 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 50 unusual 0 close 33273 reject 0
V16: AfBe V40: aEbF V36: CcDd V48: GlHkIjJiKhLgMnNm   cylinders |1|2.50414|3.50414|3.83656|4.70624|5.71595|6.23833|6.58563|
Moduli: |2.91199|6.40962|5.41147|1.08887|2.28699|1.6883|1.38481|1| Min Area: 1 Max Area: 1
    total: 0.223838 0.136077 sym = 4
    raw = 19 short saddle = 5.19615 short cyl = 8.31713 tag #0
V16: AbBa V36: CeDf V40: cFdE V48: GkHjIiJhKgLnMmNl   cylinders |1|2.50414|3.50414|3.83656|4.70624|5.71595|6.23833|6.58563|
Moduli: |2.91199|6.40962|5.41147|1.08887|2.28699|1.6883|1.38481|1| Min Area: 1 Max Area: 1
    total: 0.212058 0.128916 sym = 4
    raw = 18 short saddle = 5.19615 short cyl = 8.31713 tag #6
V16: AXXa V48: BbXXXXXXXXXXXXXX   cylinders |1|
Moduli: |1| Min Area: 0.051233 Max Area: 0.110361
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 45.8266 short cyl = 45.8266 tag #3
V16: AaXX V36: BXXb V48: CXXXXXXcXXXXXXXX   cylinders |1|
Moduli: |1| Min Area: 0.0416091 Max Area: 0.0416091
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 39.8265 short cyl = 39.8265 tag #7
V16: AaXX V48: BXXXXXXXXXXXXXXb   cylinders |1|
Moduli: |1| Min Area: 0.051233 Max Area: 0.110361
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 45.8266 short cyl = 45.8266 tag #9
V16: AXXa V40: BbXX V48: CXXXXXXXXcXXXXXX   cylinders |1|
Moduli: |1| Min Area: 0.0416091 Max Area: 0.0416091
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 39.8265 short cyl = 39.8265 tag #10
V16: AaXX V48: BXXXXXXbXXXcCXXX   cylinders |1|
Moduli: |1| Min Area: 0.0310662 Max Area: 0.0310662
    total: 0.141372 0.0859437 sym = 2
    raw = 6 short saddle = 53.9546 short cyl = 53.9546 tag #4
V16: AXXa V48: BXXXXXCcXXXXXbXX   cylinders |1|
Moduli: |1| Min Area: 0.0355098 Max Area: 0.0355098
    total: 0.141372 0.0859437 sym = 2
    raw = 6 short saddle = 46.3758 short cyl = 46.3758 tag #5
V16: AaXX V48: BXXXXXCXXcXXXXXb   cylinders |1|
Moduli: |1| Min Area: 0.0355098 Max Area: 0.0355098
    total: 0.141372 0.0859437 sym = 2
    raw = 6 short saddle = 46.3758 short cyl = 46.3758 tag #11
V16: AXXa V48: BbXXXcXXXXXXCXXX   cylinders |1|
Moduli: |1| Min Area: 0.0310662 Max Area: 0.0310662
    total: 0.141372 0.0859437 sym = 2
    raw = 6 short saddle = 53.9546 short cyl = 53.9546 tag #12

total cylinders: 5.46637 3.32316 raw = 116 rejected = 0
""",
    (1, 8, 3, 8): r"""
max loc = 1
alpha =  8 beta = 3 gamma = 8 delta =1
a =  2.51327 b = 0.942478 c= 2.51327 d=0.314159
a1_min = -0.314159 a1_max = 2.19911
a1 = 1.39911
angle a check: 0
angle b check: -1.11022e-16
angle c check: 3.14159
angle d check: 1.66533e-16
p = 1 q = 1.12778 p-q = 1.37351
p = (1,0) q = (0.192669,1.1112) r = (-2.92264,2.12342)
Imaginary of t_cx = -2.22045e-16
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(-1 0 1 0 -1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (1.08247e-15,-8.88178e-16)
Checking r_alt (8.88178e-16,-1.33227e-15)
Checking p_algt (0,0)
Checking q_algt (1.08247e-15,-8.88178e-16)
Checking r_algt (8.88178e-16,-1.33227e-15)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=3.6125795824929936
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.807331490147339403407045,1.111196466865849608041117)
E2 ( V2 V3 F0 NULL ) (-3.115306785621443808054210,1.012224534456430902196189)
E3 ( V3 V0 F0 NULL ) (2.922638275768783433505860,-2.123421001322280510237306)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (1/5 0 -2/5 0 3/5 0 -4/5 0)+(-1/5 0 2/5 0 -3/5 0 4/5 0)t1
#E2: (-6/5 0 2/5 0 -3/5 0 4/5 0)+(-4/5 0 3/5 0 -2/5 0 1/5 0)t1
#E3: (0 0 0 0 0 0 0 0)+(1 0 -1 0 1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.8, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.13216, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.8
In RetriangulateSurface
Removing V69
F41 V10(E109) V5(E83) a=0, can_bisect=0
F41 V10(E109) V60(E103) a=2.3708, can_bisect=0
F41 V10(E109) V65(E95) a=0.110314, can_bisect=1
F41 V10(E109) V10(E49) a=0.171681, can_bisect=1
F41 V10(E109) V5(E89) a=0.377639, can_bisect=1
F41 V10(E109) V60(E61) a=0.485841, can_bisect=1
F41 V10(E109) V65(E11) a=0.66784, can_bisect=1
F41 V10(E109) V10(E55) a=0.8, can_bisect=1
F41 V10(E109) V5(E23) a=0.964827, can_bisect=1
F41 V10(E109) V60(E4) a=1.11416, can_bisect=1
F41 V10(E109) V65(E17) a=0.964827, can_bisect=1
F41 V10(E109) V10(E43) a=0.8, can_bisect=1
F41 V10(E109) V5(E5) a=0.66784, can_bisect=1
F41 V10(E109) V60(E37) a=0.485841, can_bisect=1
F41 V10(E109) V65(E77) a=0.377639, can_bisect=1
F41 V10(E109) V10(E31) a=0.171681, can_bisect=1
F41 V10(E109) V5(E71) a=0.110314, can_bisect=1
F41 V10(E109) V60(E115) a=2.3708, can_bisect=0
F41 V10(E109) V65(E65) a=0, can_bisect=0
F41 V5(E83) V60(E103) a=0, can_bisect=0
F41 V5(E83) V65(E95) a=0.456637, can_bisect=1
F41 V5(E83) V10(E49) a=0.110314, can_bisect=1
F41 V5(E83) V5(E89) a=0.770796, can_bisect=1
F41 V5(E83) V60(E61) a=0.377639, can_bisect=1
F41 V5(E83) V65(E11) a=1.08496, can_bisect=1
F41 V5(E83) V10(E55) a=0.66784, can_bisect=1
F41 V5(E83) V5(E23) a=1.39911, can_bisect=1
F41 V5(E83) V60(E4) a=0.964827, can_bisect=1
F41 V5(E83) V65(E17) a=1.71327, can_bisect=1
F41 V5(E83) V10(E43) a=0.964827, can_bisect=1
F41 V5(E83) V5(E5) a=1.39911, can_bisect=1
F41 V5(E83) V60(E37) a=0.66784, can_bisect=1
F41 V5(E83) V65(E77) a=1.08496, can_bisect=1
F41 V5(E83) V10(E31) a=0.377639, can_bisect=1
F41 V5(E83) V5(E71) a=0.770796, can_bisect=1
F41 V5(E83) V60(E115) a=0.110314, can_bisect=1
F41 V5(E83) V65(E65) a=0.456637, can_bisect=1
F41 V60(E103) V65(E95) a=0, can_bisect=0
F41 V60(E103) V10(E49) a=2.3708, can_bisect=0
F41 V60(E103) V5(E89) a=0.110314, can_bisect=1
F41 V60(E103) V60(E61) a=0.171681, can_bisect=1
F41 V60(E103) V65(E11) a=0.377639, can_bisect=1
F41 V60(E103) V10(E55) a=0.485841, can_bisect=1
F41 V60(E103) V5(E23) a=0.66784, can_bisect=1
F41 V60(E103) V60(E4) a=0.8, can_bisect=1
F41 V60(E103) V65(E17) a=0.964827, can_bisect=1
F41 V60(E103) V10(E43) a=1.11416, can_bisect=1
F41 V60(E103) V5(E5) a=0.964827, can_bisect=1
F41 V60(E103) V60(E37) a=0.8, can_bisect=1
F41 V60(E103) V65(E77) a=0.66784, can_bisect=1
F41 V60(E103) V10(E31) a=0.485841, can_bisect=1
F41 V60(E103) V5(E71) a=0.377639, can_bisect=1
F41 V60(E103) V60(E115) a=0.171681, can_bisect=1
F41 V60(E103) V65(E65) a=0.110314, can_bisect=1
F41 V65(E95) V10(E49) a=0, can_bisect=0
F41 V65(E95) V5(E89) a=0.456637, can_bisect=1
F41 V65(E95) V60(E61) a=0.110314, can_bisect=1
F41 V65(E95) V65(E11) a=0.770796, can_bisect=1
F41 V65(E95) V10(E55) a=0.377639, can_bisect=1
F41 V65(E95) V5(E23) a=1.08496, can_bisect=1
F41 V65(E95) V60(E4) a=0.66784, can_bisect=1
F41 V65(E95) V65(E17) a=1.39911, can_bisect=1
F41 V65(E95) V10(E43) a=0.964827, can_bisect=1
F41 V65(E95) V5(E5) a=1.71327, can_bisect=1
F41 V65(E95) V60(E37) a=0.964827, can_bisect=1
F41 V65(E95) V65(E77) a=1.39911, can_bisect=1
F41 V65(E95) V10(E31) a=0.66784, can_bisect=1
F41 V65(E95) V5(E71) a=1.08496, can_bisect=1
F41 V65(E95) V60(E115) a=0.377639, can_bisect=1
F41 V65(E95) V65(E65) a=0.770796, can_bisect=1
F41 V10(E49) V5(E89) a=0, can_bisect=0
F41 V10(E49) V60(E61) a=2.3708, can_bisect=0
F41 V10(E49) V65(E11) a=0.110314, can_bisect=1
F41 V10(E49) V10(E55) a=0.171681, can_bisect=1
F41 V10(E49) V5(E23) a=0.377639, can_bisect=1
F41 V10(E49) V60(E4) a=0.485841, can_bisect=1
F41 V10(E49) V65(E17) a=0.66784, can_bisect=1
F41 V10(E49) V10(E43) a=0.8, can_bisect=1
F41 V10(E49) V5(E5) a=0.964827, can_bisect=1
F41 V10(E49) V60(E37) a=1.11416, can_bisect=1
F41 V10(E49) V65(E77) a=0.964827, can_bisect=1
F41 V10(E49) V10(E31) a=0.8, can_bisect=1
F41 V10(E49) V5(E71) a=0.66784, can_bisect=1
F41 V10(E49) V60(E115) a=0.485841, can_bisect=1
F41 V10(E49) V65(E65) a=0.377639, can_bisect=1
F41 V5(E89) V60(E61) a=0, can_bisect=0
F41 V5(E89) V65(E11) a=0.456637, can_bisect=1
F41 V5(E89) V10(E55) a=0.110314, can_bisect=1
F41 V5(E89) V5(E23) a=0.770796, can_bisect=1
F41 V5(E89) V60(E4) a=0.377639, can_bisect=1
F41 V5(E89) V65(E17) a=1.08496, can_bisect=1
F41 V5(E89) V10(E43) a=0.66784, can_bisect=1
F41 V5(E89) V5(E5) a=1.39911, can_bisect=1
F41 V5(E89) V60(E37) a=0.964827, can_bisect=1
F41 V5(E89) V65(E77) a=1.71327, can_bisect=1
F41 V5(E89) V10(E31) a=0.964827, can_bisect=1
F41 V5(E89) V5(E71) a=1.39911, can_bisect=1
F41 V5(E89) V60(E115) a=0.66784, can_bisect=1
F41 V5(E89) V65(E65) a=1.08496, can_bisect=1
F41 V60(E61) V65(E11) a=0, can_bisect=0
F41 V60(E61) V10(E55) a=2.3708, can_bisect=0
F41 V60(E61) V5(E23) a=0.110314, can_bisect=1
F41 V60(E61) V60(E4) a=0.171681, can_bisect=1
F41 V60(E61) V65(E17) a=0.377639, can_bisect=1
F41 V60(E61) V10(E43) a=0.485841, can_bisect=1
F41 V60(E61) V5(E5) a=0.66784, can_bisect=1
F41 V60(E61) V60(E37) a=0.8, can_bisect=1
F41 V60(E61) V65(E77) a=0.964827, can_bisect=1
F41 V60(E61) V10(E31) a=1.11416, can_bisect=1
F41 V60(E61) V5(E71) a=0.964827, can_bisect=1
F41 V60(E61) V60(E115) a=0.8, can_bisect=1
F41 V60(E61) V65(E65) a=0.66784, can_bisect=1
F41 V65(E11) V10(E55) a=0, can_bisect=0
F41 V65(E11) V5(E23) a=0.456637, can_bisect=1
F41 V65(E11) V60(E4) a=0.110314, can_bisect=1
F41 V65(E11) V65(E17) a=0.770796, can_bisect=1
F41 V65(E11) V10(E43) a=0.377639, can_bisect=1
F41 V65(E11) V5(E5) a=1.08496, can_bisect=1
F41 V65(E11) V60(E37) a=0.66784, can_bisect=1
F41 V65(E11) V65(E77) a=1.39911, can_bisect=1
F41 V65(E11) V10(E31) a=0.964827, can_bisect=1
F41 V65(E11) V5(E71) a=1.71327, can_bisect=1
F41 V65(E11) V60(E115) a=0.964827, can_bisect=1
F41 V65(E11) V65(E65) a=1.39911, can_bisect=1
F41 V10(E55) V5(E23) a=0, can_bisect=0
F41 V10(E55) V60(E4) a=2.3708, can_bisect=0
F41 V10(E55) V65(E17) a=0.110314, can_bisect=1
F41 V10(E55) V10(E43) a=0.171681, can_bisect=1
F41 V10(E55) V5(E5) a=0.377639, can_bisect=1
F41 V10(E55) V60(E37) a=0.485841, can_bisect=1
F41 V10(E55) V65(E77) a=0.66784, can_bisect=1
F41 V10(E55) V10(E31) a=0.8, can_bisect=1
F41 V10(E55) V5(E71) a=0.964827, can_bisect=1
F41 V10(E55) V60(E115) a=1.11416, can_bisect=1
F41 V10(E55) V65(E65) a=0.964827, can_bisect=1
F41 V5(E23) V60(E4) a=0, can_bisect=0
F41 V5(E23) V65(E17) a=0.456637, can_bisect=1
F41 V5(E23) V10(E43) a=0.110314, can_bisect=1
F41 V5(E23) V5(E5) a=0.770796, can_bisect=1
F41 V5(E23) V60(E37) a=0.377639, can_bisect=1
F41 V5(E23) V65(E77) a=1.08496, can_bisect=1
F41 V5(E23) V10(E31) a=0.66784, can_bisect=1
F41 V5(E23) V5(E71) a=1.39911, can_bisect=1
F41 V5(E23) V60(E115) a=0.964827, can_bisect=1
F41 V5(E23) V65(E65) a=1.71327, can_bisect=1
F41 V60(E4) V65(E17) a=0, can_bisect=0
F41 V60(E4) V10(E43) a=2.3708, can_bisect=0
F41 V60(E4) V5(E5) a=0.110314, can_bisect=1
F41 V60(E4) V60(E37) a=0.171681, can_bisect=1
F41 V60(E4) V65(E77) a=0.377639, can_bisect=1
F41 V60(E4) V10(E31) a=0.485841, can_bisect=1
F41 V60(E4) V5(E71) a=0.66784, can_bisect=1
F41 V60(E4) V60(E115) a=0.8, can_bisect=1
F41 V60(E4) V65(E65) a=0.964827, can_bisect=1
F41 V65(E17) V10(E43) a=0, can_bisect=0
F41 V65(E17) V5(E5) a=0.456637, can_bisect=1
F41 V65(E17) V60(E37) a=0.110314, can_bisect=1
F41 V65(E17) V65(E77) a=0.770796, can_bisect=1
F41 V65(E17) V10(E31) a=0.377639, can_bisect=1
F41 V65(E17) V5(E71) a=1.08496, can_bisect=1
F41 V65(E17) V60(E115) a=0.66784, can_bisect=1
F41 V65(E17) V65(E65) a=1.39911, can_bisect=1
F41 V10(E43) V5(E5) a=0, can_bisect=0
F41 V10(E43) V60(E37) a=2.3708, can_bisect=0
F41 V10(E43) V65(E77) a=0.110314, can_bisect=1
F41 V10(E43) V10(E31) a=0.171681, can_bisect=1
F41 V10(E43) V5(E71) a=0.377639, can_bisect=1
F41 V10(E43) V60(E115) a=0.485841, can_bisect=1
F41 V10(E43) V65(E65) a=0.66784, can_bisect=1
F41 V5(E5) V60(E37) a=0, can_bisect=0
F41 V5(E5) V65(E77) a=0.456637, can_bisect=1
F41 V5(E5) V10(E31) a=0.110314, can_bisect=1
F41 V5(E5) V5(E71) a=0.770796, can_bisect=1
F41 V5(E5) V60(E115) a=0.377639, can_bisect=1
F41 V5(E5) V65(E65) a=1.08496, can_bisect=1
F41 V60(E37) V65(E77) a=0, can_bisect=0
F41 V60(E37) V10(E31) a=2.3708, can_bisect=0
F41 V60(E37) V5(E71) a=0.110314, can_bisect=1
F41 V60(E37) V60(E115) a=0.171681, can_bisect=1
F41 V60(E37) V65(E65) a=0.377639, can_bisect=1
F41 V65(E77) V10(E31) a=0, can_bisect=0
F41 V65(E77) V5(E71) a=0.456637, can_bisect=1
F41 V65(E77) V60(E115) a=0.110314, can_bisect=1
F41 V65(E77) V65(E65) a=0.770796, can_bisect=1
F41 V10(E31) V5(E71) a=0, can_bisect=0
F41 V10(E31) V60(E115) a=2.3708, can_bisect=0
F41 V10(E31) V65(E65) a=0.110314, can_bisect=1
F41 V5(E71) V60(E115) a=0, can_bisect=0
F41 V5(E71) V65(E65) a=0.456637, can_bisect=1
F41 V60(E115) V65(E65) a=0, can_bisect=0
F41 best_angle = 1.71327
F43 V5(E5) V60(E37) a=0, can_bisect=0
F43 V5(E5) V65(E77) a=0.456637, can_bisect=1
F43 V5(E5) V10(E31) a=0.110314, can_bisect=1
F43 V5(E5) V5(E71) a=0.770796, can_bisect=1
F43 V5(E5) V60(E115) a=0.377639, can_bisect=1
F43 V5(E5) V65(E65) a=0.628319, can_bisect=1
F43 V5(E5) V10(E109) a=0.496158, can_bisect=1
F43 V5(E5) V5(E83) a=0.314159, can_bisect=1
F43 V5(E5) V60(E103) a=0.164827, can_bisect=1
F43 V5(E5) V65(E119) a=0, can_bisect=0
F43 V60(E37) V65(E77) a=0, can_bisect=0
F43 V60(E37) V10(E31) a=2.3708, can_bisect=0
F43 V60(E37) V5(E71) a=0.110314, can_bisect=1
F43 V60(E37) V60(E115) a=0.171681, can_bisect=1
F43 V60(E37) V65(E65) a=0.377639, can_bisect=1
F43 V60(E37) V10(E109) a=0.485841, can_bisect=1
F43 V60(E37) V5(E83) a=0.66784, can_bisect=1
F43 V60(E37) V60(E103) a=0.8, can_bisect=1
F43 V60(E37) V65(E119) a=0.164827, can_bisect=1
F43 V65(E77) V10(E31) a=0, can_bisect=0
F43 V65(E77) V5(E71) a=0.456637, can_bisect=1
F43 V65(E77) V60(E115) a=0.110314, can_bisect=1
F43 V65(E77) V65(E65) a=0.770796, can_bisect=1
F43 V65(E77) V10(E109) a=0.377639, can_bisect=1
F43 V65(E77) V5(E83) a=1.08496, can_bisect=1
F43 V65(E77) V60(E103) a=0.66784, can_bisect=1
F43 V65(E77) V65(E119) a=0.314159, can_bisect=1
F43 V10(E31) V5(E71) a=0, can_bisect=0
F43 V10(E31) V60(E115) a=2.3708, can_bisect=0
F43 V10(E31) V65(E65) a=0.110314, can_bisect=1
F43 V10(E31) V10(E109) a=0.171681, can_bisect=1
F43 V10(E31) V5(E83) a=0.377639, can_bisect=1
F43 V10(E31) V60(E103) a=0.485841, can_bisect=1
F43 V10(E31) V65(E119) a=0.496158, can_bisect=1
F43 V5(E71) V60(E115) a=0, can_bisect=0
F43 V5(E71) V65(E65) a=0.456637, can_bisect=1
F43 V5(E71) V10(E109) a=0.110314, can_bisect=1
F43 V5(E71) V5(E83) a=0.770796, can_bisect=1
F43 V5(E71) V60(E103) a=0.377639, can_bisect=1
F43 V5(E71) V65(E119) a=0.628319, can_bisect=1
F43 V60(E115) V65(E65) a=0, can_bisect=0
F43 V60(E115) V10(E109) a=2.3708, can_bisect=0
F43 V60(E115) V5(E83) a=0.110314, can_bisect=1
F43 V60(E115) V60(E103) a=0.171681, can_bisect=1
F43 V60(E115) V65(E119) a=0.377639, can_bisect=1
F43 V65(E65) V10(E109) a=0, can_bisect=0
F43 V65(E65) V5(E83) a=0.456637, can_bisect=1
F43 V65(E65) V60(E103) a=0.110314, can_bisect=1
F43 V65(E65) V65(E119) a=0.770796, can_bisect=1
F43 V10(E109) V5(E83) a=0, can_bisect=0
F43 V10(E109) V60(E103) a=2.3708, can_bisect=0
F43 V10(E109) V65(E119) a=0.110314, can_bisect=1
F43 V5(E83) V60(E103) a=0, can_bisect=0
F43 V5(E83) V65(E119) a=0.456637, can_bisect=1
F43 V60(E103) V65(E119) a=0, can_bisect=0
F43 best_angle = 1.08496
F45 V5(E83) V60(E103) a=0, can_bisect=0
F45 V5(E83) V65(E119) a=0.456637, can_bisect=1
F45 V5(E83) V5(E5) a=0.314159, can_bisect=1
F45 V5(E83) V60(E37) a=0.13216, can_bisect=1
F45 V5(E83) V65(E120) a=0, can_bisect=0
F45 V60(E103) V65(E119) a=0, can_bisect=0
F45 V60(E103) V5(E5) a=0.164827, can_bisect=1
F45 V60(E103) V60(E37) a=0.8, can_bisect=1
F45 V60(E103) V65(E120) a=0.13216, can_bisect=1
F45 V65(E119) V5(E5) a=0, can_bisect=0
F45 V65(E119) V60(E37) a=0.164827, can_bisect=1
F45 V65(E119) V65(E120) a=0.314159, can_bisect=1
F45 V5(E5) V60(E37) a=0, can_bisect=0
F45 V5(E5) V65(E120) a=0.456637, can_bisect=1
F45 V60(E37) V65(E120) a=0, can_bisect=0
F45 best_angle = 0.8
F47 V60(E37) V65(E120) a=0, can_bisect=0
F47 V60(E37) V5(E83) a=0.13216, can_bisect=1
F47 V60(E37) V60(E121) a=0, can_bisect=0
F47 V65(E120) V5(E83) a=0, can_bisect=0
F47 V65(E120) V60(E121) a=0.13216, can_bisect=1
F47 V5(E83) V60(E121) a=0, can_bisect=0
F47 best_angle = 0.13216
F46 V60(E103) V65(E119) a=0, can_bisect=0
F46 V60(E103) V5(E5) a=0.164827, can_bisect=1
F46 V60(E103) V60(E121) a=0, can_bisect=0
F46 V65(E119) V5(E5) a=0, can_bisect=0
F46 V65(E119) V60(E121) a=0.164827, can_bisect=1
F46 V5(E5) V60(E121) a=0, can_bisect=0
F46 best_angle = 0.164827
F44 V65(E77) V10(E31) a=0, can_bisect=0
F44 V65(E77) V5(E71) a=0.456637, can_bisect=1
F44 V65(E77) V60(E115) a=0.110314, can_bisect=1
F44 V65(E77) V65(E65) a=0.314159, can_bisect=1
F44 V65(E77) V10(E109) a=0.205958, can_bisect=1
F44 V65(E77) V5(E120) a=0, can_bisect=0
F44 V10(E31) V5(E71) a=0, can_bisect=0
F44 V10(E31) V60(E115) a=2.3708, can_bisect=0
F44 V10(E31) V65(E65) a=0.110314, can_bisect=1
F44 V10(E31) V10(E109) a=0.171681, can_bisect=1
F44 V10(E31) V5(E120) a=0.205958, can_bisect=1
F44 V5(E71) V60(E115) a=0, can_bisect=0
F44 V5(E71) V65(E65) a=0.456637, can_bisect=1
F44 V5(E71) V10(E109) a=0.110314, can_bisect=1
F44 V5(E71) V5(E120) a=0.314159, can_bisect=1
F44 V60(E115) V65(E65) a=0, can_bisect=0
F44 V60(E115) V10(E109) a=2.3708, can_bisect=0
F44 V60(E115) V5(E120) a=0.110314, can_bisect=1
F44 V65(E65) V10(E109) a=0, can_bisect=0
F44 V65(E65) V5(E120) a=0.456637, can_bisect=1
F44 V10(E109) V5(E120) a=0, can_bisect=0
F44 best_angle = 0.456637
F53 V5(E71) V60(E115) a=0, can_bisect=0
F53 V5(E71) V65(E65) a=0.456637, can_bisect=1
F53 V5(E71) V10(E109) a=0.110314, can_bisect=1
F53 V5(E71) V5(E120) a=0.314159, can_bisect=1
F53 V5(E71) V65(E124) a=0, can_bisect=0
F53 V60(E115) V65(E65) a=0, can_bisect=0
F53 V60(E115) V10(E109) a=2.3708, can_bisect=0
F53 V60(E115) V5(E120) a=0.110314, can_bisect=1
F53 V60(E115) V65(E124) a=0.0613675, can_bisect=1
F53 V65(E65) V10(E109) a=0, can_bisect=0
F53 V65(E65) V5(E120) a=0.456637, can_bisect=1
F53 V65(E65) V65(E124) a=0.314159, can_bisect=1
F53 V10(E109) V5(E120) a=0, can_bisect=0
F53 V10(E109) V65(E124) a=0.205958, can_bisect=1
F53 V5(E120) V65(E124) a=0, can_bisect=0
F53 best_angle = 0.456637
F55 V5(E120) V65(E124) a=0, can_bisect=0
F55 V5(E120) V5(E71) a=0.314159, can_bisect=1
F55 V5(E120) V60(E115) a=0.0613675, can_bisect=1
F55 V5(E120) V65(E125) a=0, can_bisect=0
F55 V65(E124) V5(E71) a=0, can_bisect=0
F55 V65(E124) V60(E115) a=0.0613675, can_bisect=1
F55 V65(E124) V65(E125) a=0.314159, can_bisect=1
F55 V5(E71) V60(E115) a=0, can_bisect=0
F55 V5(E71) V65(E125) a=0.456637, can_bisect=1
F55 V60(E115) V65(E125) a=0, can_bisect=0
F55 best_angle = 0.456637
F57 V65(E125) V5(E120) a=0, can_bisect=0
F57 V65(E125) V65(E124) a=0.314159, can_bisect=1
F57 V65(E125) V5(E126) a=0, can_bisect=0
F57 V5(E120) V65(E124) a=0, can_bisect=0
F57 V5(E120) V5(E126) a=0.314159, can_bisect=1
F57 V65(E124) V5(E126) a=0, can_bisect=0
F57 best_angle = 0.314159
F42 V65(E95) V10(E49) a=0, can_bisect=0
F42 V65(E95) V5(E89) a=0.456637, can_bisect=1
F42 V65(E95) V60(E61) a=0.110314, can_bisect=1
F42 V65(E95) V65(E11) a=0.770796, can_bisect=1
F42 V65(E95) V10(E55) a=0.377639, can_bisect=1
F42 V65(E95) V5(E23) a=0.628319, can_bisect=1
F42 V65(E95) V60(E4) a=0.496158, can_bisect=1
F42 V65(E95) V65(E17) a=0.314159, can_bisect=1
F42 V65(E95) V10(E43) a=0.164827, can_bisect=1
F42 V65(E95) V5(E119) a=0, can_bisect=0
F42 V10(E49) V5(E89) a=0, can_bisect=0
F42 V10(E49) V60(E61) a=2.3708, can_bisect=0
F42 V10(E49) V65(E11) a=0.110314, can_bisect=1
F42 V10(E49) V10(E55) a=0.171681, can_bisect=1
F42 V10(E49) V5(E23) a=0.377639, can_bisect=1
F42 V10(E49) V60(E4) a=0.485841, can_bisect=1
F42 V10(E49) V65(E17) a=0.66784, can_bisect=1
F42 V10(E49) V10(E43) a=0.8, can_bisect=1
F42 V10(E49) V5(E119) a=0.164827, can_bisect=1
F42 V5(E89) V60(E61) a=0, can_bisect=0
F42 V5(E89) V65(E11) a=0.456637, can_bisect=1
F42 V5(E89) V10(E55) a=0.110314, can_bisect=1
F42 V5(E89) V5(E23) a=0.770796, can_bisect=1
F42 V5(E89) V60(E4) a=0.377639, can_bisect=1
F42 V5(E89) V65(E17) a=1.08496, can_bisect=1
F42 V5(E89) V10(E43) a=0.66784, can_bisect=1
F42 V5(E89) V5(E119) a=0.314159, can_bisect=1
F42 V60(E61) V65(E11) a=0, can_bisect=0
F42 V60(E61) V10(E55) a=2.3708, can_bisect=0
F42 V60(E61) V5(E23) a=0.110314, can_bisect=1
F42 V60(E61) V60(E4) a=0.171681, can_bisect=1
F42 V60(E61) V65(E17) a=0.377639, can_bisect=1
F42 V60(E61) V10(E43) a=0.485841, can_bisect=1
F42 V60(E61) V5(E119) a=0.496158, can_bisect=1
F42 V65(E11) V10(E55) a=0, can_bisect=0
F42 V65(E11) V5(E23) a=0.456637, can_bisect=1
F42 V65(E11) V60(E4) a=0.110314, can_bisect=1
F42 V65(E11) V65(E17) a=0.770796, can_bisect=1
F42 V65(E11) V10(E43) a=0.377639, can_bisect=1
F42 V65(E11) V5(E119) a=0.628319, can_bisect=1
F42 V10(E55) V5(E23) a=0, can_bisect=0
F42 V10(E55) V60(E4) a=2.3708, can_bisect=0
F42 V10(E55) V65(E17) a=0.110314, can_bisect=1
F42 V10(E55) V10(E43) a=0.171681, can_bisect=1
F42 V10(E55) V5(E119) a=0.377639, can_bisect=1
F42 V5(E23) V60(E4) a=0, can_bisect=0
F42 V5(E23) V65(E17) a=0.456637, can_bisect=1
F42 V5(E23) V10(E43) a=0.110314, can_bisect=1
F42 V5(E23) V5(E119) a=0.770796, can_bisect=1
F42 V60(E4) V65(E17) a=0, can_bisect=0
F42 V60(E4) V10(E43) a=2.3708, can_bisect=0
F42 V60(E4) V5(E119) a=0.110314, can_bisect=1
F42 V65(E17) V10(E43) a=0, can_bisect=0
F42 V65(E17) V5(E119) a=0.456637, can_bisect=1
F42 V10(E43) V5(E119) a=0, can_bisect=0
F42 best_angle = 1.08496
F61 V65(E17) V10(E43) a=0, can_bisect=0
F61 V65(E17) V5(E119) a=0.456637, can_bisect=1
F61 V65(E17) V65(E95) a=0.314159, can_bisect=1
F61 V65(E17) V10(E49) a=0.13216, can_bisect=1
F61 V65(E17) V5(E128) a=0, can_bisect=0
F61 V10(E43) V5(E119) a=0, can_bisect=0
F61 V10(E43) V65(E95) a=0.164827, can_bisect=1
F61 V10(E43) V10(E49) a=0.8, can_bisect=1
F61 V10(E43) V5(E128) a=0.13216, can_bisect=1
F61 V5(E119) V65(E95) a=0, can_bisect=0
F61 V5(E119) V10(E49) a=0.164827, can_bisect=1
F61 V5(E119) V5(E128) a=0.314159, can_bisect=1
F61 V65(E95) V10(E49) a=0, can_bisect=0
F61 V65(E95) V5(E128) a=0.456637, can_bisect=1
F61 V10(E49) V5(E128) a=0, can_bisect=0
F61 best_angle = 0.8
F63 V10(E49) V5(E128) a=0, can_bisect=0
F63 V10(E49) V65(E17) a=0.13216, can_bisect=1
F63 V10(E49) V10(E129) a=0, can_bisect=0
F63 V5(E128) V65(E17) a=0, can_bisect=0
F63 V5(E128) V10(E129) a=0.13216, can_bisect=1
F63 V65(E17) V10(E129) a=0, can_bisect=0
F63 best_angle = 0.13216
F62 V10(E43) V5(E119) a=0, can_bisect=0
F62 V10(E43) V65(E95) a=0.164827, can_bisect=1
F62 V10(E43) V10(E129) a=0, can_bisect=0
F62 V5(E119) V65(E95) a=0, can_bisect=0
F62 V5(E119) V10(E129) a=0.164827, can_bisect=1
F62 V65(E95) V10(E129) a=0, can_bisect=0
F62 best_angle = 0.164827
F60 V5(E89) V60(E61) a=0, can_bisect=0
F60 V5(E89) V65(E11) a=0.456637, can_bisect=1
F60 V5(E89) V10(E55) a=0.110314, can_bisect=1
F60 V5(E89) V5(E23) a=0.314159, can_bisect=1
F60 V5(E89) V60(E4) a=0.205958, can_bisect=1
F60 V5(E89) V65(E128) a=0, can_bisect=0
F60 V60(E61) V65(E11) a=0, can_bisect=0
F60 V60(E61) V10(E55) a=2.3708, can_bisect=0
F60 V60(E61) V5(E23) a=0.110314, can_bisect=1
F60 V60(E61) V60(E4) a=0.171681, can_bisect=1
F60 V60(E61) V65(E128) a=0.205958, can_bisect=1
F60 V65(E11) V10(E55) a=0, can_bisect=0
F60 V65(E11) V5(E23) a=0.456637, can_bisect=1
F60 V65(E11) V60(E4) a=0.110314, can_bisect=1
F60 V65(E11) V65(E128) a=0.314159, can_bisect=1
F60 V10(E55) V5(E23) a=0, can_bisect=0
F60 V10(E55) V60(E4) a=2.3708, can_bisect=0
F60 V10(E55) V65(E128) a=0.110314, can_bisect=1
F60 V5(E23) V60(E4) a=0, can_bisect=0
F60 V5(E23) V65(E128) a=0.456637, can_bisect=1
F60 V60(E4) V65(E128) a=0, can_bisect=0
F60 best_angle = 0.456637
F69 V65(E11) V10(E55) a=0, can_bisect=0
F69 V65(E11) V5(E23) a=0.456637, can_bisect=1
F69 V65(E11) V60(E4) a=0.110314, can_bisect=1
F69 V65(E11) V65(E128) a=0.314159, can_bisect=1
F69 V65(E11) V5(E132) a=0, can_bisect=0
F69 V10(E55) V5(E23) a=0, can_bisect=0
F69 V10(E55) V60(E4) a=2.3708, can_bisect=0
F69 V10(E55) V65(E128) a=0.110314, can_bisect=1
F69 V10(E55) V5(E132) a=0.0613675, can_bisect=1
F69 V5(E23) V60(E4) a=0, can_bisect=0
F69 V5(E23) V65(E128) a=0.456637, can_bisect=1
F69 V5(E23) V5(E132) a=0.314159, can_bisect=1
F69 V60(E4) V65(E128) a=0, can_bisect=0
F69 V60(E4) V5(E132) a=0.205958, can_bisect=1
F69 V65(E128) V5(E132) a=0, can_bisect=0
F69 best_angle = 0.456637
F71 V65(E128) V5(E132) a=0, can_bisect=0
F71 V65(E128) V65(E11) a=0.314159, can_bisect=1
F71 V65(E128) V10(E55) a=0.0613675, can_bisect=1
F71 V65(E128) V5(E133) a=0, can_bisect=0
F71 V5(E132) V65(E11) a=0, can_bisect=0
F71 V5(E132) V10(E55) a=0.0613675, can_bisect=1
F71 V5(E132) V5(E133) a=0.314159, can_bisect=1
F71 V65(E11) V10(E55) a=0, can_bisect=0
F71 V65(E11) V5(E133) a=0.456637, can_bisect=1
F71 V10(E55) V5(E133) a=0, can_bisect=0
F71 best_angle = 0.456637
F73 V5(E133) V65(E128) a=0, can_bisect=0
F73 V5(E133) V5(E132) a=0.314159, can_bisect=1
F73 V5(E133) V65(E134) a=0, can_bisect=0
F73 V65(E128) V5(E132) a=0, can_bisect=0
F73 V65(E128) V65(E134) a=0.314159, can_bisect=1
F73 V5(E132) V65(E134) a=0, can_bisect=0
F73 best_angle = 0.314159
Extra: Flipping E121
Extra: Flipping E122
Extra: Flipping E123
Extra: Flipping E129
Extra: Flipping E130
Extra: Flipping E131
Longest Edge: 6.55125
Looking for shortest saddle...1
Clearing denominators: multiplying by 5
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=3.6125795824929936
# Vertices: V1 V5 V10 V60 V65
# UEdges: 
E0 ( V60 V1 F1 F3 ) (5.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V65 F1 F5 ) (-4.036657450736696794990621,5.555982334329248040205584)
E4 ( V60 V65 F70 F1 ) (0.963342549263302982964774,5.555982334329248040205584)
E5 ( V60 V5 F3 F79 ) (0.963342549263300762518725,-5.555982334329248928384004)
E6 ( V5 V1 F3 F11 ) (4.036657450736698571347461,5.555982334329248928384004)
E11 ( V10 V65 F5 F72 ) (-5.581742422611434051304968,0.800699752853482049630429)
E13 ( V1 V10 F5 F17 ) (1.545084971874737256314347,4.755282581475766434664365)
E17 ( V65 V10 F87 F8 ) (2.486363984357748080356032,5.061122672282155399159365)
E21 ( V65 V1 F8 F14 ) (6.531448956232484448491959,2.122196410819791267954315)
E22 ( V1 V10 F8 F20 ) (-4.045084971874736368135927,2.938926261462364575294259)
E23 ( V5 V60 F70 F10 ) (4.986363984357748080356032,2.633086170655975166710050)
E27 ( V5 V1 F10 F16 ) (6.531448956232482672135120,-2.122196410819791267954315)
E28 ( V1 V60 F10 F22 ) (-1.545084971874735479957508,4.755282581475766434664365)
E29 ( V10 V1 F11 F23 ) (-1.545084971874735479957508,4.755282581475766434664365)
E31 ( V5 V10 F11 F52 ) (5.581742422611434051304968,0.800699752853484270076478)
E37 ( V60 V65 F79 F14 ) (-4.986363984357746303999193,2.633086170655975166710050)
E38 ( V60 V1 F14 F26 ) (1.545084971874737256314347,4.755282581475766434664365)
E43 ( V10 V5 F87 F16 ) (-2.486363984357746748088402,5.061122672282156287337784)
E44 ( V10 V1 F16 F28 ) (4.045084971874736368135927,2.938926261462365019383469)
E48 ( V1 V5 F17 F29 ) (6.531448956232484448491959,2.122196410819791267954315)
E49 ( V5 V10 F17 F85 ) (-4.986363984357746303999193,2.633086170655975166710050)
E55 ( V10 V5 F72 F20 ) (4.045084971874737256314347,3.928645585556548525119069)
E57 ( V1 V5 F20 F32 ) (0.000000000000001540915066,6.867571847018913544502539)
E61 ( V60 V65 F68 F22 ) (5.581742422611434051304968,0.800699752853484270076478)
E63 ( V1 V65 F22 F34 ) (4.036657450736698571347461,5.555982334329248928384004)
E65 ( V10 V65 F23 F54 ) (4.986363984357748080356032,2.633086170655975166710050)
E66 ( V65 V1 F23 F35 ) (-6.531448956232482672135120,2.122196410819791267954315)
E71 ( V5 V60 F56 F26 ) (-5.581742422611434051304968,0.800699752853482049630429)
E75 ( V5 V1 F26 F38 ) (-4.036657450736696794990621,5.555982334329248040205584)
E77 ( V65 V10 F52 F28 ) (-4.045084971874734591779088,3.928645585556548525119069)
E81 ( V65 V1 F28 F40 ) (0.000000000000001540915066,6.867571847018913544502539)
E83 ( V60 V5 F29 F81 ) (2.486363984357748080356032,5.061122672282155399159365)
E85 ( V1 V60 F29 F40 ) (4.045084971874736368135927,-2.938926261462364575294259)
E89 ( V5 V60 F68 F32 ) (4.045084971874734591779088,-3.928645585556548525119069)
E94 ( V1 V60 F32 F35 ) (4.045084971874736368135927,2.938926261462365019383469)
E95 ( V65 V10 F85 F34 ) (0.963342549263300762518725,-5.555982334329248928384004)
E100 ( V1 V10 F34 F38 ) (5.000000000000000000000000,-0.000000000000000000000000)
E103 ( V65 V60 F35 F81 ) (-2.486363984357746748088402,5.061122672282156287337784)
E109 ( V10 V5 F54 F38 ) (-0.963342549263302982964774,-5.555982334329248040205584)
E115 ( V60 V65 F56 F40 ) (-4.045084971874737256314347,-3.928645585556548525119069)
E119 ( V65 V5 F80 F86 ) (31.153067856214448738683132,10.122245344564312574675569)
E120 ( V65 V5 F78 F58 ) (-25.203361322593394788782462,-8.189068504985225871450893)
E124 ( V65 V5 F59 F52 ) (-9.626827394486168643084056,3.127945832703064255042591)
E125 ( V65 V5 F58 F54 ) (-5.949706533621050397186991,-8.189068504985222318737215)
E126 ( V5 V65 F59 F56 ) (-9.626827394486172195797735,-3.127945832703066031399430)
E127 ( V65 V65 F59 F58 ) (19.253654788972344391595470,0.000000000000002220446049)
E128 ( V5 V65 F84 F74 ) (25.203361322593394788782462,8.189068504985225871450893)
E132 ( V5 V65 F75 F68 ) (9.626827394486168643084056,-3.127945832703064255042591)
E133 ( V5 V65 F74 F70 ) (5.949706533621050397186991,8.189068504985222318737215)
E134 ( V65 V5 F75 F72 ) (9.626827394486172195797735,3.127945832703066031399430)
E135 ( V5 V5 F75 F74 ) (-19.253654788972344391595470,-0.000000000000002220446049)
E136 ( V5 V5 F78 F80 ) (31.153067856214438080542095,0.000000000000000555111512)
E137 ( V5 V65 F78 F79 ) (-5.949706533621048620830152,8.189068504985224095094054)
E138 ( V5 V65 F80 F81 ) (-0.000000000000000304438448,-10.122245344564312574675569)
E139 ( V65 V65 F84 F86 ) (-31.153067856214438080542095,-0.000000000000000555111512)
E140 ( V65 V5 F84 F85 ) (5.949706533621048620830152,-8.189068504985224095094054)
E141 ( V65 V5 F86 F87 ) (0.000000000000000304438448,10.122245344564312574675569)
# Faces: 
F1 +E0 +E1 -E4
F3 +E5 +E6 -E0
F5 +E11 -E1 +E13
F8 -E17 +E21 +E22
F10 -E23 +E27 +E28
F11 +E29 -E6 +E31
F14 +E38 -E21 -E37
F16 +E44 -E27 -E43
F17 -E13 +E48 +E49
F20 -E22 +E57 -E55
F22 -E28 +E63 -E61
F23 +E65 +E66 -E29
F26 -E71 +E75 -E38
F28 -E77 +E81 -E44
F29 +E83 -E48 +E85
F32 -E89 -E57 +E94
F34 -E95 -E63 +E100
F35 -E94 -E66 +E103
F38 -E100 -E75 -E109
F40 -E85 -E81 -E115
F52 +E77 -E31 -E124
F54 -E65 +E109 -E125
F56 +E71 +E115 -E126
F58 +E125 -E120 -E127
F59 +E124 +E126 +E127
F68 +E89 +E61 -E132
F70 +E23 +E4 -E133
F72 -E11 +E55 -E134
F74 +E133 -E128 -E135
F75 +E132 +E134 +E135
F78 +E137 +E120 +E136
F79 -E137 -E5 +E37
F80 +E138 +E119 -E136
F81 -E138 -E83 -E103
F84 +E140 +E128 +E139
F85 -E140 +E95 -E49
F86 +E141 -E119 -E139
F87 -E141 +E17 +E43
#UEdges Algebraic 
#E0: (5 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (1 0 -2 0 3 0 -4 0)+(-1 0 2 0 -3 0 4 0)t1
#E4: (6 0 -2 0 3 0 -4 0)+(-1 0 2 0 -3 0 4 0)t1
#E5: (4 0 2 0 2 0 -1 0)+(1 0 -2 0 -2 0 1 0)t1
#E6: (1 0 -2 0 -2 0 1 0)+(-1 0 2 0 2 0 -1 0)t1
#E11: (1 0 -2 0 -2 0 -4 0)+(-1 0 2 0 -3 0 4 0)t1
#E13: (0 0 0 0 5 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E17: (-6 0 2 0 -3 0 4 0)+(1 0 3 0 -2 0 1 0)t1
#E21: (-1 0 -3 0 2 0 -1 0)+(1 0 3 0 -2 0 1 0)t1
#E22: (-5 0 5 0 -5 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E23: (-4 0 3 0 -2 0 6 0)+(4 0 -3 0 2 0 -1 0)t1
#E27: (-4 0 3 0 -2 0 1 0)+(4 0 -3 0 2 0 -1 0)t1
#E28: (0 0 0 0 0 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E29: (0 0 0 0 0 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E31: (1 0 -2 0 -2 0 -4 0)+(-1 0 2 0 2 0 -1 0)t1
#E37: (1 0 3 0 3 0 1 0)+(-1 0 -3 0 2 0 -1 0)t1
#E38: (0 0 0 0 5 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E43: (4 0 2 0 2 0 -1 0)+(-4 0 3 0 -2 0 1 0)t1
#E44: (0 0 5 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E48: (-1 0 -3 0 2 0 -1 0)+(1 0 3 0 -2 0 1 0)t1
#E49: (1 0 3 0 3 0 1 0)+(-1 0 -3 0 2 0 -1 0)t1
#E55: (4 0 -3 0 2 0 -6 0)+(1 0 -2 0 3 0 1 0)t1
#E57: (-1 0 2 0 -3 0 -1 0)+(1 0 -2 0 3 0 1 0)t1
#E61: (1 0 -2 0 -2 0 -4 0)+(-1 0 2 0 2 0 -1 0)t1
#E63: (1 0 -2 0 -2 0 1 0)+(-1 0 2 0 2 0 -1 0)t1
#E65: (-4 0 3 0 -2 0 6 0)+(4 0 -3 0 2 0 -1 0)t1
#E66: (4 0 -3 0 2 0 -1 0)+(-4 0 3 0 -2 0 1 0)t1
#E71: (1 0 -2 0 -2 0 -4 0)+(-1 0 2 0 -3 0 4 0)t1
#E75: (1 0 -2 0 3 0 -4 0)+(-1 0 2 0 -3 0 4 0)t1
#E77: (-1 0 -3 0 -3 0 -1 0)+(1 0 -2 0 3 0 1 0)t1
#E81: (-1 0 2 0 -3 0 -1 0)+(1 0 -2 0 3 0 1 0)t1
#E83: (-6 0 2 0 -3 0 4 0)+(1 0 3 0 -2 0 1 0)t1
#E85: (5 0 -5 0 5 0 -5 0)+(0 0 0 0 0 0 0 0)t1
#E89: (1 0 3 0 3 0 1 0)+(-1 0 2 0 -3 0 -1 0)t1
#E94: (0 0 5 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E95: (4 0 2 0 2 0 -1 0)+(1 0 -2 0 -2 0 1 0)t1
#E100: (5 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E103: (4 0 2 0 2 0 -1 0)+(-4 0 3 0 -2 0 1 0)t1
#E109: (-6 0 2 0 -3 0 4 0)+(1 0 -2 0 3 0 -4 0)t1
#E115: (-4 0 3 0 -2 0 6 0)+(-1 0 2 0 -3 0 -1 0)t1
#E119: (8 0 4 0 4 0 -2 0)+(2 0 6 0 -4 0 2 0)t1
#E120: (-7 0 -1 0 -6 0 3 0)+(-3 0 1 0 -4 0 2 0)t1
#E124: (-2 0 -1 0 -1 0 3 0)+(2 0 -4 0 1 0 2 0)t1
#E125: (-2 0 -1 0 -1 0 -2 0)+(-3 0 1 0 1 0 -3 0)t1
#E126: (-3 0 1 0 -4 0 2 0)+(-2 0 4 0 -6 0 3 0)t1
#E127: (5 0 0 0 5 0 -5 0)+(0 0 0 0 5 0 -5 0)t1
#E128: (7 0 1 0 6 0 -3 0)+(3 0 -1 0 4 0 -2 0)t1
#E132: (2 0 1 0 1 0 -3 0)+(-2 0 4 0 -1 0 -2 0)t1
#E133: (2 0 1 0 1 0 2 0)+(3 0 -1 0 -1 0 3 0)t1
#E134: (3 0 -1 0 4 0 -2 0)+(2 0 -4 0 6 0 -3 0)t1
#E135: (-5 0 0 0 -5 0 5 0)+(0 0 0 0 -5 0 5 0)t1
#E136: (10 0 0 0 5 0 -5 0)+(5 0 0 0 0 0 0 0)t1
#E137: (-3 0 1 0 1 0 2 0)+(-2 0 -1 0 4 0 -2 0)t1
#E138: (2 0 -4 0 1 0 -3 0)+(3 0 -6 0 4 0 -2 0)t1
#E139: (-10 0 0 0 -5 0 5 0)+(-5 0 0 0 0 0 0 0)t1
#E140: (3 0 -1 0 -1 0 -2 0)+(2 0 1 0 -4 0 2 0)t1
#E141: (-2 0 4 0 -1 0 3 0)+(-3 0 6 0 -4 0 2 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 5
## V1(20) (6 PI) V5(32) (8 PI) V10(15) (8 PI) V60(15) (8 PI) V65(32) (8 PI) 
# edges: 57
## E0(2) E1(2) E4(2) E5(2) E6(2) E11(2) E13(2) E17(2) E21(2) E22(2) E23(2) E27(2) E28(2) E29(2) E31(2) E37(2) E38(2) E43(2) E44(2) E48(2) E49(2) E55(2) E57(2) E61(2) E63(2) E65(2) E66(2) E71(2) E75(2) E77(2) E81(2) E83(2) E85(2) E89(2) E94(2) E95(2) E100(2) E103(2) E109(2) E115(2) E119(2) E120(2) E124(2) E125(2) E126(2) E127(2) E128(2) E132(2) E133(2) E134(2) E135(2) E136(2) E137(2) E138(2) E139(2) E140(2) E141(2) 
# faces: 38
# genus: 8
# area: 1191.98
sweeping start V1 depth = 69.0503
File =  depth = 69.0503 follow_depth = 345.251 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 222 unusual 0 close 69634 reject 0
V1: AbBiCc V60: aNhOkLjM V5: DdEgFfGe V10: HnImJlKo V65: PqQpRsSr   cylinders |1|1|2.2965|2.2965|2.61803|2.61803|3.71581|3.71581|4.33384|4.33384|
Moduli: |10.2274|10.2274|4.18435|4.18435|1|1|2.31695|2.31695|1.6011|1.6011| Min Area: 1 Max Area: 1
    total: 0.676315 0.41115 sym = 6
    raw = 31 short saddle = 5 short cyl = 19.2537 tag #0
V1: AaBcCe V65: bRdSgPfQ V5: DrEqFpGs V10: HkIjJiKh V60: LlMoNnOm   cylinders |1|1|1.7247|1.7247|2.06592|2.06592|3.34273|3.34273|3.96077|3.96077|
Moduli: |2.46949|2.46949|3.69554|3.69554|5.00881|5.00881|1.91719|1.91719|1|1| Min Area: 1 Max Area: 1
    total: 0.654498 0.397887 sym = 6
    raw = 30 short saddle = 6.86757 short cyl = 21.2342 tag #5
V1: AXBbXa V5: CXDhXXXc V60: dXXXgGXH V10: EXXXFfXj V65: eJXXXIiX   cylinders |1|1|1.61803|1.61803|
Moduli: |4.14556|4.14556|1|1| Min Area: 0.105095 Max Area: 0.105095
    total: 0.654498 0.397887 sym = 4
    raw = 20 short saddle = 11.7251 short cyl = 42.4218 tag #2
V1: AXXaXX V5: BbXXXXXX V10: CcXXXXXX V60: DXXXXXXd V65: EXXXXXXe   cylinders |1|1|
Moduli: |1|1| Min Area: 0.197823 Max Area: 0.197823
    total: 1.309 0.795775 sym = 2
    raw = 20 short saddle = 36.0862 short cyl = 36.0862 tag #6
V1: AXBbXa V5: CXXXDeXc V10: dXfFXXXE V60: GXHhXXXj V65: gJXIiXXX   cylinders |1|1|1.61803|1.61803|
Moduli: |4.14556|4.14556|1|1| Min Area: 0.105095 Max Area: 0.105095
    total: 0.654498 0.397887 sym = 4
    raw = 20 short saddle = 11.7251 short cyl = 42.4218 tag #9

total cylinders: 21.0749 12.812 raw = 161 rejected = 0
""",
    (1, 8, 5, 6): r"""
max loc = 1
alpha =  8 beta = 5 gamma = 6 delta =1
a =  2.51327 b = 1.5708 c= 1.88496 d=0.314159
a1_min = -0.314159 a1_max = 1.5708
a1 = 0.970796
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 4.996e-16
p = 1 q = 1.77103 p-q = 1.4617
p = (1,0) q = (1,1.4617) r = (-4.44849,3.23202)
Imaginary of t_cx = 4.44089e-16
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(-1 0 1 0 -1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (-1.11022e-16,-1.55431e-15)
Checking r_alt (1.77636e-15,-8.88178e-16)
Checking p_algt (0,0)
Checking q_algt (-1.11022e-16,-1.55431e-15)
Checking r_algt (1.77636e-15,-8.88178e-16)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=5.4986375526780602
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.000000000000000222044605,1.461695947078100132898726)
E2 ( V2 V3 F0 NULL ) (-5.448491226024819944484534,1.770322114087639508994698)
E3 ( V3 V0 F0 NULL ) (4.448491226024819944484534,-3.232018061165739641893424)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (3/5 0 -6/5 0 4/5 0 -2/5 0)+(-3/5 0 6/5 0 -4/5 0 2/5 0)t1
#E2: (-8/5 0 6/5 0 -4/5 0 2/5 0)+(-2/5 0 -1/5 0 -1/5 0 3/5 0)t1
#E3: (0 0 0 0 0 0 0 0)+(1 0 -1 0 1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.6, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.0929177, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.6
In RetriangulateSurface
Removing V1
F41 V58(E5) V66(E31) a=0, can_bisect=0
F41 V58(E5) V17(E11) a=0.970796, can_bisect=1
F41 V58(E5) V78(E4) a=0, can_bisect=0
F41 V66(E31) V17(E11) a=0, can_bisect=0
F41 V66(E31) V78(E4) a=0.6, can_bisect=1
F41 V17(E11) V78(E4) a=0, can_bisect=0
F41 best_angle = 0.970796
Removing V21
F44 V78(E61) V58(E83) a=0, can_bisect=0
F44 V78(E61) V66(E67) a=0.6, can_bisect=1
F44 V78(E61) V17(E17) a=0, can_bisect=0
F44 V58(E83) V66(E67) a=0, can_bisect=0
F44 V58(E83) V17(E17) a=0.970796, can_bisect=1
F44 V66(E67) V17(E17) a=0, can_bisect=0
F44 best_angle = 0.970796
Removing V27
F47 V66(E73) V17(E89) a=0, can_bisect=0
F47 V66(E73) V78(E79) a=0.6, can_bisect=1
F47 V66(E73) V58(E23) a=0, can_bisect=0
F47 V17(E89) V78(E79) a=0, can_bisect=0
F47 V17(E89) V58(E23) a=0.970796, can_bisect=1
F47 V78(E79) V58(E23) a=0, can_bisect=0
F47 best_angle = 0.970796
Removing V38
F50 V58(E101) V66(E49) a=0, can_bisect=0
F50 V58(E101) V17(E107) a=0.970796, can_bisect=1
F50 V58(E101) V78(E37) a=0, can_bisect=0
F50 V66(E49) V17(E107) a=0, can_bisect=0
F50 V66(E49) V78(E37) a=0.6, can_bisect=1
F50 V17(E107) V78(E37) a=0, can_bisect=0
F50 best_angle = 0.970796
Removing V44
F53 V17(E95) V78(E55) a=0, can_bisect=0
F53 V17(E95) V58(E113) a=0.970796, can_bisect=1
F53 V17(E95) V66(E43) a=0, can_bisect=0
F53 V78(E55) V58(E113) a=0, can_bisect=0
F53 V78(E55) V66(E43) a=0.6, can_bisect=1
F53 V58(E113) V66(E43) a=0, can_bisect=0
F53 best_angle = 0.970796
Removing V80
F56 V78(E107) V17(E67) a=0, can_bisect=0
F56 V78(E107) V66(E23) a=0.0283185, can_bisect=1
F56 V78(E107) V58(E4) a=0.225624, can_bisect=1
F56 V78(E107) V78(E17) a=0.342478, can_bisect=1
F56 V78(E107) V17(E43) a=0.520062, can_bisect=1
F56 V78(E107) V66(E5) a=0.656637, can_bisect=1
F56 V78(E107) V58(E37) a=0.824165, can_bisect=1
F56 V78(E107) V78(E95) a=0.970796, can_bisect=1
F56 V78(E107) V17(E73) a=1.13112, can_bisect=1
F56 V78(E107) V66(E101) a=1.28496, can_bisect=1
F56 V78(E107) V58(E61) a=1.13112, can_bisect=1
F56 V78(E107) V78(E89) a=0.970796, can_bisect=1
F56 V78(E107) V17(E31) a=0.824165, can_bisect=1
F56 V78(E107) V66(E83) a=0.656637, can_bisect=1
F56 V78(E107) V58(E55) a=0.520062, can_bisect=1
F56 V78(E107) V78(E11) a=0.342478, can_bisect=1
F56 V78(E107) V17(E49) a=0.225624, can_bisect=1
F56 V78(E107) V66(E113) a=0.0283185, can_bisect=1
F56 V78(E107) V58(E79) a=0, can_bisect=0
F56 V17(E67) V66(E23) a=0, can_bisect=0
F56 V17(E67) V58(E4) a=0.285841, can_bisect=1
F56 V17(E67) V78(E17) a=0.225624, can_bisect=1
F56 V17(E67) V17(E43) a=0.6, can_bisect=1
F56 V17(E67) V66(E5) a=0.520062, can_bisect=1
F56 V17(E67) V58(E37) a=0.914159, can_bisect=1
F56 V17(E67) V78(E95) a=0.824165, can_bisect=1
F56 V17(E67) V17(E73) a=1.22832, can_bisect=1
F56 V17(E67) V66(E101) a=1.13112, can_bisect=1
F56 V17(E67) V58(E61) a=1.54248, can_bisect=1
F56 V17(E67) V78(E89) a=1.13112, can_bisect=1
F56 V17(E67) V17(E31) a=1.22832, can_bisect=1
F56 V17(E67) V66(E83) a=0.824165, can_bisect=1
F56 V17(E67) V58(E55) a=0.914159, can_bisect=1
F56 V17(E67) V78(E11) a=0.520062, can_bisect=1
F56 V17(E67) V17(E49) a=0.6, can_bisect=1
F56 V17(E67) V66(E113) a=0.225624, can_bisect=1
F56 V17(E67) V58(E79) a=0.285841, can_bisect=1
F56 V66(E23) V58(E4) a=0, can_bisect=0
F56 V66(E23) V78(E17) a=0.0283185, can_bisect=1
F56 V66(E23) V17(E43) a=0.225624, can_bisect=1
F56 V66(E23) V66(E5) a=0.342478, can_bisect=1
F56 V66(E23) V58(E37) a=0.520062, can_bisect=1
F56 V66(E23) V78(E95) a=0.656637, can_bisect=1
F56 V66(E23) V17(E73) a=0.824165, can_bisect=1
F56 V66(E23) V66(E101) a=0.970796, can_bisect=1
F56 V66(E23) V58(E61) a=1.13112, can_bisect=1
F56 V66(E23) V78(E89) a=1.28496, can_bisect=1
F56 V66(E23) V17(E31) a=1.13112, can_bisect=1
F56 V66(E23) V66(E83) a=0.970796, can_bisect=1
F56 V66(E23) V58(E55) a=0.824165, can_bisect=1
F56 V66(E23) V78(E11) a=0.656637, can_bisect=1
F56 V66(E23) V17(E49) a=0.520062, can_bisect=1
F56 V66(E23) V66(E113) a=0.342478, can_bisect=1
F56 V66(E23) V58(E79) a=0.225624, can_bisect=1
F56 V58(E4) V78(E17) a=0, can_bisect=0
F56 V58(E4) V17(E43) a=0.285841, can_bisect=1
F56 V58(E4) V66(E5) a=0.225624, can_bisect=1
F56 V58(E4) V58(E37) a=0.6, can_bisect=1
F56 V58(E4) V78(E95) a=0.520062, can_bisect=1
F56 V58(E4) V17(E73) a=0.914159, can_bisect=1
F56 V58(E4) V66(E101) a=0.824165, can_bisect=1
F56 V58(E4) V58(E61) a=1.22832, can_bisect=1
F56 V58(E4) V78(E89) a=1.13112, can_bisect=1
F56 V58(E4) V17(E31) a=1.54248, can_bisect=1
F56 V58(E4) V66(E83) a=1.13112, can_bisect=1
F56 V58(E4) V58(E55) a=1.22832, can_bisect=1
F56 V58(E4) V78(E11) a=0.824165, can_bisect=1
F56 V58(E4) V17(E49) a=0.914159, can_bisect=1
F56 V58(E4) V66(E113) a=0.520062, can_bisect=1
F56 V58(E4) V58(E79) a=0.6, can_bisect=1
F56 V78(E17) V17(E43) a=0, can_bisect=0
F56 V78(E17) V66(E5) a=0.0283185, can_bisect=1
F56 V78(E17) V58(E37) a=0.225624, can_bisect=1
F56 V78(E17) V78(E95) a=0.342478, can_bisect=1
F56 V78(E17) V17(E73) a=0.520062, can_bisect=1
F56 V78(E17) V66(E101) a=0.656637, can_bisect=1
F56 V78(E17) V58(E61) a=0.824165, can_bisect=1
F56 V78(E17) V78(E89) a=0.970796, can_bisect=1
F56 V78(E17) V17(E31) a=1.13112, can_bisect=1
F56 V78(E17) V66(E83) a=1.28496, can_bisect=1
F56 V78(E17) V58(E55) a=1.13112, can_bisect=1
F56 V78(E17) V78(E11) a=0.970796, can_bisect=1
F56 V78(E17) V17(E49) a=0.824165, can_bisect=1
F56 V78(E17) V66(E113) a=0.656637, can_bisect=1
F56 V78(E17) V58(E79) a=0.520062, can_bisect=1
F56 V17(E43) V66(E5) a=0, can_bisect=0
F56 V17(E43) V58(E37) a=0.285841, can_bisect=1
F56 V17(E43) V78(E95) a=0.225624, can_bisect=1
F56 V17(E43) V17(E73) a=0.6, can_bisect=1
F56 V17(E43) V66(E101) a=0.520062, can_bisect=1
F56 V17(E43) V58(E61) a=0.914159, can_bisect=1
F56 V17(E43) V78(E89) a=0.824165, can_bisect=1
F56 V17(E43) V17(E31) a=1.22832, can_bisect=1
F56 V17(E43) V66(E83) a=1.13112, can_bisect=1
F56 V17(E43) V58(E55) a=1.54248, can_bisect=1
F56 V17(E43) V78(E11) a=1.13112, can_bisect=1
F56 V17(E43) V17(E49) a=1.22832, can_bisect=1
F56 V17(E43) V66(E113) a=0.824165, can_bisect=1
F56 V17(E43) V58(E79) a=0.914159, can_bisect=1
F56 V66(E5) V58(E37) a=0, can_bisect=0
F56 V66(E5) V78(E95) a=0.0283185, can_bisect=1
F56 V66(E5) V17(E73) a=0.225624, can_bisect=1
F56 V66(E5) V66(E101) a=0.342478, can_bisect=1
F56 V66(E5) V58(E61) a=0.520062, can_bisect=1
F56 V66(E5) V78(E89) a=0.656637, can_bisect=1
F56 V66(E5) V17(E31) a=0.824165, can_bisect=1
F56 V66(E5) V66(E83) a=0.970796, can_bisect=1
F56 V66(E5) V58(E55) a=1.13112, can_bisect=1
F56 V66(E5) V78(E11) a=1.28496, can_bisect=1
F56 V66(E5) V17(E49) a=1.13112, can_bisect=1
F56 V66(E5) V66(E113) a=0.970796, can_bisect=1
F56 V66(E5) V58(E79) a=0.824165, can_bisect=1
F56 V58(E37) V78(E95) a=0, can_bisect=0
F56 V58(E37) V17(E73) a=0.285841, can_bisect=1
F56 V58(E37) V66(E101) a=0.225624, can_bisect=1
F56 V58(E37) V58(E61) a=0.6, can_bisect=1
F56 V58(E37) V78(E89) a=0.520062, can_bisect=1
F56 V58(E37) V17(E31) a=0.914159, can_bisect=1
F56 V58(E37) V66(E83) a=0.824165, can_bisect=1
F56 V58(E37) V58(E55) a=1.22832, can_bisect=1
F56 V58(E37) V78(E11) a=1.13112, can_bisect=1
F56 V58(E37) V17(E49) a=1.54248, can_bisect=1
F56 V58(E37) V66(E113) a=1.13112, can_bisect=1
F56 V58(E37) V58(E79) a=1.22832, can_bisect=1
F56 V78(E95) V17(E73) a=0, can_bisect=0
F56 V78(E95) V66(E101) a=0.0283185, can_bisect=1
F56 V78(E95) V58(E61) a=0.225624, can_bisect=1
F56 V78(E95) V78(E89) a=0.342478, can_bisect=1
F56 V78(E95) V17(E31) a=0.520062, can_bisect=1
F56 V78(E95) V66(E83) a=0.656637, can_bisect=1
F56 V78(E95) V58(E55) a=0.824165, can_bisect=1
F56 V78(E95) V78(E11) a=0.970796, can_bisect=1
F56 V78(E95) V17(E49) a=1.13112, can_bisect=1
F56 V78(E95) V66(E113) a=1.28496, can_bisect=1
F56 V78(E95) V58(E79) a=1.13112, can_bisect=1
F56 V17(E73) V66(E101) a=0, can_bisect=0
F56 V17(E73) V58(E61) a=0.285841, can_bisect=1
F56 V17(E73) V78(E89) a=0.225624, can_bisect=1
F56 V17(E73) V17(E31) a=0.6, can_bisect=1
F56 V17(E73) V66(E83) a=0.520062, can_bisect=1
F56 V17(E73) V58(E55) a=0.914159, can_bisect=1
F56 V17(E73) V78(E11) a=0.824165, can_bisect=1
F56 V17(E73) V17(E49) a=1.22832, can_bisect=1
F56 V17(E73) V66(E113) a=1.13112, can_bisect=1
F56 V17(E73) V58(E79) a=1.54248, can_bisect=1
F56 V66(E101) V58(E61) a=0, can_bisect=0
F56 V66(E101) V78(E89) a=0.0283185, can_bisect=1
F56 V66(E101) V17(E31) a=0.225624, can_bisect=1
F56 V66(E101) V66(E83) a=0.342478, can_bisect=1
F56 V66(E101) V58(E55) a=0.520062, can_bisect=1
F56 V66(E101) V78(E11) a=0.656637, can_bisect=1
F56 V66(E101) V17(E49) a=0.824165, can_bisect=1
F56 V66(E101) V66(E113) a=0.970796, can_bisect=1
F56 V66(E101) V58(E79) a=1.13112, can_bisect=1
F56 V58(E61) V78(E89) a=0, can_bisect=0
F56 V58(E61) V17(E31) a=0.285841, can_bisect=1
F56 V58(E61) V66(E83) a=0.225624, can_bisect=1
F56 V58(E61) V58(E55) a=0.6, can_bisect=1
F56 V58(E61) V78(E11) a=0.520062, can_bisect=1
F56 V58(E61) V17(E49) a=0.914159, can_bisect=1
F56 V58(E61) V66(E113) a=0.824165, can_bisect=1
F56 V58(E61) V58(E79) a=1.22832, can_bisect=1
F56 V78(E89) V17(E31) a=0, can_bisect=0
F56 V78(E89) V66(E83) a=0.0283185, can_bisect=1
F56 V78(E89) V58(E55) a=0.225624, can_bisect=1
F56 V78(E89) V78(E11) a=0.342478, can_bisect=1
F56 V78(E89) V17(E49) a=0.520062, can_bisect=1
F56 V78(E89) V66(E113) a=0.656637, can_bisect=1
F56 V78(E89) V58(E79) a=0.824165, can_bisect=1
F56 V17(E31) V66(E83) a=0, can_bisect=0
F56 V17(E31) V58(E55) a=0.285841, can_bisect=1
F56 V17(E31) V78(E11) a=0.225624, can_bisect=1
F56 V17(E31) V17(E49) a=0.6, can_bisect=1
F56 V17(E31) V66(E113) a=0.520062, can_bisect=1
F56 V17(E31) V58(E79) a=0.914159, can_bisect=1
F56 V66(E83) V58(E55) a=0, can_bisect=0
F56 V66(E83) V78(E11) a=0.0283185, can_bisect=1
F56 V66(E83) V17(E49) a=0.225624, can_bisect=1
F56 V66(E83) V66(E113) a=0.342478, can_bisect=1
F56 V66(E83) V58(E79) a=0.520062, can_bisect=1
F56 V58(E55) V78(E11) a=0, can_bisect=0
F56 V58(E55) V17(E49) a=0.285841, can_bisect=1
F56 V58(E55) V66(E113) a=0.225624, can_bisect=1
F56 V58(E55) V58(E79) a=0.6, can_bisect=1
F56 V78(E11) V17(E49) a=0, can_bisect=0
F56 V78(E11) V66(E113) a=0.0283185, can_bisect=1
F56 V78(E11) V58(E79) a=0.225624, can_bisect=1
F56 V17(E49) V66(E113) a=0, can_bisect=0
F56 V17(E49) V58(E79) a=0.285841, can_bisect=1
F56 V66(E113) V58(E79) a=0, can_bisect=0
F56 best_angle = 1.54248
F58 V17(E49) V66(E113) a=0, can_bisect=0
F58 V17(E49) V58(E79) a=0.285841, can_bisect=1
F58 V17(E49) V78(E107) a=0.225624, can_bisect=1
F58 V17(E49) V17(E67) a=0.6, can_bisect=1
F58 V17(E49) V66(E23) a=0.520062, can_bisect=1
F58 V17(E49) V58(E4) a=0.628319, can_bisect=1
F58 V17(E49) V78(E17) a=0.481687, can_bisect=1
F58 V17(E49) V17(E43) a=0.314159, can_bisect=1
F58 V17(E49) V66(E5) a=0.160328, can_bisect=1
F58 V17(E49) V58(E124) a=0, can_bisect=0
F58 V66(E113) V58(E79) a=0, can_bisect=0
F58 V66(E113) V78(E107) a=0.0283185, can_bisect=1
F58 V66(E113) V17(E67) a=0.225624, can_bisect=1
F58 V66(E113) V66(E23) a=0.342478, can_bisect=1
F58 V66(E113) V58(E4) a=0.520062, can_bisect=1
F58 V66(E113) V78(E17) a=0.656637, can_bisect=1
F58 V66(E113) V17(E43) a=0.824165, can_bisect=1
F58 V66(E113) V66(E5) a=0.970796, can_bisect=1
F58 V66(E113) V58(E124) a=0.160328, can_bisect=1
F58 V58(E79) V78(E107) a=0, can_bisect=0
F58 V58(E79) V17(E67) a=0.285841, can_bisect=1
F58 V58(E79) V66(E23) a=0.225624, can_bisect=1
F58 V58(E79) V58(E4) a=0.6, can_bisect=1
F58 V58(E79) V78(E17) a=0.520062, can_bisect=1
F58 V58(E79) V17(E43) a=0.914159, can_bisect=1
F58 V58(E79) V66(E5) a=0.824165, can_bisect=1
F58 V58(E79) V58(E124) a=0.314159, can_bisect=1
F58 V78(E107) V17(E67) a=0, can_bisect=0
F58 V78(E107) V66(E23) a=0.0283185, can_bisect=1
F58 V78(E107) V58(E4) a=0.225624, can_bisect=1
F58 V78(E107) V78(E17) a=0.342478, can_bisect=1
F58 V78(E107) V17(E43) a=0.520062, can_bisect=1
F58 V78(E107) V66(E5) a=0.656637, can_bisect=1
F58 V78(E107) V58(E124) a=0.481687, can_bisect=1
F58 V17(E67) V66(E23) a=0, can_bisect=0
F58 V17(E67) V58(E4) a=0.285841, can_bisect=1
F58 V17(E67) V78(E17) a=0.225624, can_bisect=1
F58 V17(E67) V17(E43) a=0.6, can_bisect=1
F58 V17(E67) V66(E5) a=0.520062, can_bisect=1
F58 V17(E67) V58(E124) a=0.628319, can_bisect=1
F58 V66(E23) V58(E4) a=0, can_bisect=0
F58 V66(E23) V78(E17) a=0.0283185, can_bisect=1
F58 V66(E23) V17(E43) a=0.225624, can_bisect=1
F58 V66(E23) V66(E5) a=0.342478, can_bisect=1
F58 V66(E23) V58(E124) a=0.520062, can_bisect=1
F58 V58(E4) V78(E17) a=0, can_bisect=0
F58 V58(E4) V17(E43) a=0.285841, can_bisect=1
F58 V58(E4) V66(E5) a=0.225624, can_bisect=1
F58 V58(E4) V58(E124) a=0.6, can_bisect=1
F58 V78(E17) V17(E43) a=0, can_bisect=0
F58 V78(E17) V66(E5) a=0.0283185, can_bisect=1
F58 V78(E17) V58(E124) a=0.225624, can_bisect=1
F58 V17(E43) V66(E5) a=0, can_bisect=0
F58 V17(E43) V58(E124) a=0.285841, can_bisect=1
F58 V66(E5) V58(E124) a=0, can_bisect=0
F58 best_angle = 0.970796
F60 V66(E5) V58(E124) a=0, can_bisect=0
F60 V66(E5) V17(E49) a=0.160328, can_bisect=1
F60 V66(E5) V66(E125) a=0, can_bisect=0
F60 V58(E124) V17(E49) a=0, can_bisect=0
F60 V58(E124) V66(E125) a=0.160328, can_bisect=1
F60 V17(E49) V66(E125) a=0, can_bisect=0
F60 best_angle = 0.160328
F59 V66(E113) V58(E79) a=0, can_bisect=0
F59 V66(E113) V78(E107) a=0.0283185, can_bisect=1
F59 V66(E113) V17(E67) a=0.225624, can_bisect=1
F59 V66(E113) V66(E23) a=0.342478, can_bisect=1
F59 V66(E113) V58(E4) a=0.450735, can_bisect=1
F59 V66(E113) V78(E17) a=0.314159, can_bisect=1
F59 V66(E113) V17(E43) a=0.146631, can_bisect=1
F59 V66(E113) V66(E125) a=0, can_bisect=0
F59 V58(E79) V78(E107) a=0, can_bisect=0
F59 V58(E79) V17(E67) a=0.285841, can_bisect=1
F59 V58(E79) V66(E23) a=0.225624, can_bisect=1
F59 V58(E79) V58(E4) a=0.6, can_bisect=1
F59 V58(E79) V78(E17) a=0.520062, can_bisect=1
F59 V58(E79) V17(E43) a=0.914159, can_bisect=1
F59 V58(E79) V66(E125) a=0.146631, can_bisect=1
F59 V78(E107) V17(E67) a=0, can_bisect=0
F59 V78(E107) V66(E23) a=0.0283185, can_bisect=1
F59 V78(E107) V58(E4) a=0.225624, can_bisect=1
F59 V78(E107) V78(E17) a=0.342478, can_bisect=1
F59 V78(E107) V17(E43) a=0.520062, can_bisect=1
F59 V78(E107) V66(E125) a=0.314159, can_bisect=1
F59 V17(E67) V66(E23) a=0, can_bisect=0
F59 V17(E67) V58(E4) a=0.285841, can_bisect=1
F59 V17(E67) V78(E17) a=0.225624, can_bisect=1
F59 V17(E67) V17(E43) a=0.6, can_bisect=1
F59 V17(E67) V66(E125) a=0.450735, can_bisect=1
F59 V66(E23) V58(E4) a=0, can_bisect=0
F59 V66(E23) V78(E17) a=0.0283185, can_bisect=1
F59 V66(E23) V17(E43) a=0.225624, can_bisect=1
F59 V66(E23) V66(E125) a=0.342478, can_bisect=1
F59 V58(E4) V78(E17) a=0, can_bisect=0
F59 V58(E4) V17(E43) a=0.285841, can_bisect=1
F59 V58(E4) V66(E125) a=0.225624, can_bisect=1
F59 V78(E17) V17(E43) a=0, can_bisect=0
F59 V78(E17) V66(E125) a=0.0283185, can_bisect=1
F59 V17(E43) V66(E125) a=0, can_bisect=0
F59 best_angle = 0.914159
F64 V17(E43) V66(E125) a=0, can_bisect=0
F64 V17(E43) V66(E113) a=0.146631, can_bisect=1
F64 V17(E43) V58(E127) a=0, can_bisect=0
F64 V66(E125) V66(E113) a=0, can_bisect=0
F64 V66(E125) V58(E127) a=0.146631, can_bisect=1
F64 V66(E113) V58(E127) a=0, can_bisect=0
F64 best_angle = 0.146631
F63 V58(E79) V78(E107) a=0, can_bisect=0
F63 V58(E79) V17(E67) a=0.285841, can_bisect=1
F63 V58(E79) V66(E23) a=0.225624, can_bisect=1
F63 V58(E79) V58(E4) a=0.314159, can_bisect=1
F63 V58(E79) V78(E17) a=0.177584, can_bisect=1
F63 V58(E79) V17(E127) a=0, can_bisect=0
F63 V78(E107) V17(E67) a=0, can_bisect=0
F63 V78(E107) V66(E23) a=0.0283185, can_bisect=1
F63 V78(E107) V58(E4) a=0.225624, can_bisect=1
F63 V78(E107) V78(E17) a=0.342478, can_bisect=1
F63 V78(E107) V17(E127) a=0.177584, can_bisect=1
F63 V17(E67) V66(E23) a=0, can_bisect=0
F63 V17(E67) V58(E4) a=0.285841, can_bisect=1
F63 V17(E67) V78(E17) a=0.225624, can_bisect=1
F63 V17(E67) V17(E127) a=0.314159, can_bisect=1
F63 V66(E23) V58(E4) a=0, can_bisect=0
F63 V66(E23) V78(E17) a=0.0283185, can_bisect=1
F63 V66(E23) V17(E127) a=0.225624, can_bisect=1
F63 V58(E4) V78(E17) a=0, can_bisect=0
F63 V58(E4) V17(E127) a=0.285841, can_bisect=1
F63 V78(E17) V17(E127) a=0, can_bisect=0
F63 best_angle = 0.342478
F68 V78(E17) V17(E127) a=0, can_bisect=0
F68 V78(E17) V58(E79) a=0.177584, can_bisect=1
F68 V78(E17) V78(E129) a=0, can_bisect=0
F68 V17(E127) V58(E79) a=0, can_bisect=0
F68 V17(E127) V78(E129) a=0.177584, can_bisect=1
F68 V58(E79) V78(E129) a=0, can_bisect=0
F68 best_angle = 0.177584
F67 V78(E107) V17(E67) a=0, can_bisect=0
F67 V78(E107) V66(E23) a=0.0283185, can_bisect=1
F67 V78(E107) V58(E4) a=0.116854, can_bisect=1
F67 V78(E107) V78(E129) a=0, can_bisect=0
F67 V17(E67) V66(E23) a=0, can_bisect=0
F67 V17(E67) V58(E4) a=0.285841, can_bisect=1
F67 V17(E67) V78(E129) a=0.116854, can_bisect=1
F67 V66(E23) V58(E4) a=0, can_bisect=0
F67 V66(E23) V78(E129) a=0.0283185, can_bisect=1
F67 V58(E4) V78(E129) a=0, can_bisect=0
F67 best_angle = 0.285841
F72 V58(E4) V78(E129) a=0, can_bisect=0
F72 V58(E4) V78(E107) a=0.116854, can_bisect=1
F72 V58(E4) V17(E131) a=0, can_bisect=0
F72 V78(E129) V78(E107) a=0, can_bisect=0
F72 V78(E129) V17(E131) a=0.116854, can_bisect=1
F72 V78(E107) V17(E131) a=0, can_bisect=0
F72 best_angle = 0.116854
F57 V58(E37) V78(E95) a=0, can_bisect=0
F57 V58(E37) V17(E73) a=0.285841, can_bisect=1
F57 V58(E37) V66(E101) a=0.225624, can_bisect=1
F57 V58(E37) V58(E61) a=0.6, can_bisect=1
F57 V58(E37) V78(E89) a=0.520062, can_bisect=1
F57 V58(E37) V17(E31) a=0.628319, can_bisect=1
F57 V58(E37) V66(E83) a=0.481687, can_bisect=1
F57 V58(E37) V58(E55) a=0.314159, can_bisect=1
F57 V58(E37) V78(E11) a=0.160328, can_bisect=1
F57 V58(E37) V17(E124) a=0, can_bisect=0
F57 V78(E95) V17(E73) a=0, can_bisect=0
F57 V78(E95) V66(E101) a=0.0283185, can_bisect=1
F57 V78(E95) V58(E61) a=0.225624, can_bisect=1
F57 V78(E95) V78(E89) a=0.342478, can_bisect=1
F57 V78(E95) V17(E31) a=0.520062, can_bisect=1
F57 V78(E95) V66(E83) a=0.656637, can_bisect=1
F57 V78(E95) V58(E55) a=0.824165, can_bisect=1
F57 V78(E95) V78(E11) a=0.970796, can_bisect=1
F57 V78(E95) V17(E124) a=0.160328, can_bisect=1
F57 V17(E73) V66(E101) a=0, can_bisect=0
F57 V17(E73) V58(E61) a=0.285841, can_bisect=1
F57 V17(E73) V78(E89) a=0.225624, can_bisect=1
F57 V17(E73) V17(E31) a=0.6, can_bisect=1
F57 V17(E73) V66(E83) a=0.520062, can_bisect=1
F57 V17(E73) V58(E55) a=0.914159, can_bisect=1
F57 V17(E73) V78(E11) a=0.824165, can_bisect=1
F57 V17(E73) V17(E124) a=0.314159, can_bisect=1
F57 V66(E101) V58(E61) a=0, can_bisect=0
F57 V66(E101) V78(E89) a=0.0283185, can_bisect=1
F57 V66(E101) V17(E31) a=0.225624, can_bisect=1
F57 V66(E101) V66(E83) a=0.342478, can_bisect=1
F57 V66(E101) V58(E55) a=0.520062, can_bisect=1
F57 V66(E101) V78(E11) a=0.656637, can_bisect=1
F57 V66(E101) V17(E124) a=0.481687, can_bisect=1
F57 V58(E61) V78(E89) a=0, can_bisect=0
F57 V58(E61) V17(E31) a=0.285841, can_bisect=1
F57 V58(E61) V66(E83) a=0.225624, can_bisect=1
F57 V58(E61) V58(E55) a=0.6, can_bisect=1
F57 V58(E61) V78(E11) a=0.520062, can_bisect=1
F57 V58(E61) V17(E124) a=0.628319, can_bisect=1
F57 V78(E89) V17(E31) a=0, can_bisect=0
F57 V78(E89) V66(E83) a=0.0283185, can_bisect=1
F57 V78(E89) V58(E55) a=0.225624, can_bisect=1
F57 V78(E89) V78(E11) a=0.342478, can_bisect=1
F57 V78(E89) V17(E124) a=0.520062, can_bisect=1
F57 V17(E31) V66(E83) a=0, can_bisect=0
F57 V17(E31) V58(E55) a=0.285841, can_bisect=1
F57 V17(E31) V78(E11) a=0.225624, can_bisect=1
F57 V17(E31) V17(E124) a=0.6, can_bisect=1
F57 V66(E83) V58(E55) a=0, can_bisect=0
F57 V66(E83) V78(E11) a=0.0283185, can_bisect=1
F57 V66(E83) V17(E124) a=0.225624, can_bisect=1
F57 V58(E55) V78(E11) a=0, can_bisect=0
F57 V58(E55) V17(E124) a=0.285841, can_bisect=1
F57 V78(E11) V17(E124) a=0, can_bisect=0
F57 best_angle = 0.970796
F76 V78(E11) V17(E124) a=0, can_bisect=0
F76 V78(E11) V58(E37) a=0.160328, can_bisect=1
F76 V78(E11) V78(E133) a=0, can_bisect=0
F76 V17(E124) V58(E37) a=0, can_bisect=0
F76 V17(E124) V78(E133) a=0.160328, can_bisect=1
F76 V58(E37) V78(E133) a=0, can_bisect=0
F76 best_angle = 0.160328
F75 V78(E95) V17(E73) a=0, can_bisect=0
F75 V78(E95) V66(E101) a=0.0283185, can_bisect=1
F75 V78(E95) V58(E61) a=0.225624, can_bisect=1
F75 V78(E95) V78(E89) a=0.342478, can_bisect=1
F75 V78(E95) V17(E31) a=0.450735, can_bisect=1
F75 V78(E95) V66(E83) a=0.314159, can_bisect=1
F75 V78(E95) V58(E55) a=0.146631, can_bisect=1
F75 V78(E95) V78(E133) a=0, can_bisect=0
F75 V17(E73) V66(E101) a=0, can_bisect=0
F75 V17(E73) V58(E61) a=0.285841, can_bisect=1
F75 V17(E73) V78(E89) a=0.225624, can_bisect=1
F75 V17(E73) V17(E31) a=0.6, can_bisect=1
F75 V17(E73) V66(E83) a=0.520062, can_bisect=1
F75 V17(E73) V58(E55) a=0.914159, can_bisect=1
F75 V17(E73) V78(E133) a=0.146631, can_bisect=1
F75 V66(E101) V58(E61) a=0, can_bisect=0
F75 V66(E101) V78(E89) a=0.0283185, can_bisect=1
F75 V66(E101) V17(E31) a=0.225624, can_bisect=1
F75 V66(E101) V66(E83) a=0.342478, can_bisect=1
F75 V66(E101) V58(E55) a=0.520062, can_bisect=1
F75 V66(E101) V78(E133) a=0.314159, can_bisect=1
F75 V58(E61) V78(E89) a=0, can_bisect=0
F75 V58(E61) V17(E31) a=0.285841, can_bisect=1
F75 V58(E61) V66(E83) a=0.225624, can_bisect=1
F75 V58(E61) V58(E55) a=0.6, can_bisect=1
F75 V58(E61) V78(E133) a=0.450735, can_bisect=1
F75 V78(E89) V17(E31) a=0, can_bisect=0
F75 V78(E89) V66(E83) a=0.0283185, can_bisect=1
F75 V78(E89) V58(E55) a=0.225624, can_bisect=1
F75 V78(E89) V78(E133) a=0.342478, can_bisect=1
F75 V17(E31) V66(E83) a=0, can_bisect=0
F75 V17(E31) V58(E55) a=0.285841, can_bisect=1
F75 V17(E31) V78(E133) a=0.225624, can_bisect=1
F75 V66(E83) V58(E55) a=0, can_bisect=0
F75 V66(E83) V78(E133) a=0.0283185, can_bisect=1
F75 V58(E55) V78(E133) a=0, can_bisect=0
F75 best_angle = 0.914159
F80 V58(E55) V78(E133) a=0, can_bisect=0
F80 V58(E55) V78(E95) a=0.146631, can_bisect=1
F80 V58(E55) V17(E135) a=0, can_bisect=0
F80 V78(E133) V78(E95) a=0, can_bisect=0
F80 V78(E133) V17(E135) a=0.146631, can_bisect=1
F80 V78(E95) V17(E135) a=0, can_bisect=0
F80 best_angle = 0.146631
F79 V17(E73) V66(E101) a=0, can_bisect=0
F79 V17(E73) V58(E61) a=0.285841, can_bisect=1
F79 V17(E73) V78(E89) a=0.225624, can_bisect=1
F79 V17(E73) V17(E31) a=0.314159, can_bisect=1
F79 V17(E73) V66(E83) a=0.177584, can_bisect=1
F79 V17(E73) V58(E135) a=0, can_bisect=0
F79 V66(E101) V58(E61) a=0, can_bisect=0
F79 V66(E101) V78(E89) a=0.0283185, can_bisect=1
F79 V66(E101) V17(E31) a=0.225624, can_bisect=1
F79 V66(E101) V66(E83) a=0.342478, can_bisect=1
F79 V66(E101) V58(E135) a=0.177584, can_bisect=1
F79 V58(E61) V78(E89) a=0, can_bisect=0
F79 V58(E61) V17(E31) a=0.285841, can_bisect=1
F79 V58(E61) V66(E83) a=0.225624, can_bisect=1
F79 V58(E61) V58(E135) a=0.314159, can_bisect=1
F79 V78(E89) V17(E31) a=0, can_bisect=0
F79 V78(E89) V66(E83) a=0.0283185, can_bisect=1
F79 V78(E89) V58(E135) a=0.225624, can_bisect=1
F79 V17(E31) V66(E83) a=0, can_bisect=0
F79 V17(E31) V58(E135) a=0.285841, can_bisect=1
F79 V66(E83) V58(E135) a=0, can_bisect=0
F79 best_angle = 0.342478
F84 V66(E83) V58(E135) a=0, can_bisect=0
F84 V66(E83) V17(E73) a=0.177584, can_bisect=1
F84 V66(E83) V66(E137) a=0, can_bisect=0
F84 V58(E135) V17(E73) a=0, can_bisect=0
F84 V58(E135) V66(E137) a=0.177584, can_bisect=1
F84 V17(E73) V66(E137) a=0, can_bisect=0
F84 best_angle = 0.177584
F83 V66(E101) V58(E61) a=0, can_bisect=0
F83 V66(E101) V78(E89) a=0.0283185, can_bisect=1
F83 V66(E101) V17(E31) a=0.116854, can_bisect=1
F83 V66(E101) V66(E137) a=0, can_bisect=0
F83 V58(E61) V78(E89) a=0, can_bisect=0
F83 V58(E61) V17(E31) a=0.285841, can_bisect=1
F83 V58(E61) V66(E137) a=0.116854, can_bisect=1
F83 V78(E89) V17(E31) a=0, can_bisect=0
F83 V78(E89) V66(E137) a=0.0283185, can_bisect=1
F83 V17(E31) V66(E137) a=0, can_bisect=0
F83 best_angle = 0.285841
F88 V17(E31) V66(E137) a=0, can_bisect=0
F88 V17(E31) V66(E101) a=0.116854, can_bisect=1
F88 V17(E31) V58(E139) a=0, can_bisect=0
F88 V66(E137) V66(E101) a=0, can_bisect=0
F88 V66(E137) V58(E139) a=0.116854, can_bisect=1
F88 V66(E101) V58(E139) a=0, can_bisect=0
F88 best_angle = 0.116854
Extra: Flipping E125
Extra: Flipping E126
Extra: Flipping E128
Extra: Flipping E129
Extra: Flipping E130
Extra: Flipping E132
Extra: Flipping E133
Extra: Flipping E134
Extra: Flipping E136
Extra: Flipping E137
Extra: Flipping E138
Extra: Flipping E140
Longest Edge: 10.9973
Looking for shortest saddle...1.77103
Clearing denominators: multiplying by 5
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=5.4986375526780602
# Vertices: V17 V58 V66 V78
# UEdges: 
E4 ( V58 V78 F102 F43 ) (4.999999999999999111821580,7.308479735390500664493629)
E5 ( V58 V66 F42 F96 ) (5.000000000000000000000000,-7.308479735390500664493629)
E11 ( V17 V78 F43 F108 ) (-5.000000000000000000000000,7.308479735390500664493629)
E17 ( V78 V17 F102 F46 ) (0.250731633266202957877056,8.851610570438197100884281)
E23 ( V66 V58 F71 F49 ) (5.405692304679479143203480,7.013727022756347295739943)
E31 ( V66 V17 F42 F114 ) (4.999999999999999111821580,7.308479735390500664493629)
E37 ( V58 V78 F104 F52 ) (-5.405692304679474702311381,7.013727022756349072096782)
E43 ( V17 V66 F96 F55 ) (-0.250731633266199072096470,8.851610570438200653597960)
E49 ( V17 V66 F92 F51 ) (5.405692304679474702311381,-7.013727022756349072096782)
E55 ( V58 V78 F108 F54 ) (0.250731633266199072096470,-8.851610570438200653597960)
E61 ( V78 V58 F46 F87 ) (8.340901577015674916992793,2.973758047513467950295762)
E67 ( V66 V17 F45 F71 ) (-8.340901577015674916992793,-2.973758047513467950295762)
E73 ( V66 V17 F49 F110 ) (8.495862248428950991296915,-2.496838140195183797231948)
E79 ( V78 V58 F48 F98 ) (-8.495862248428950991296915,2.496838140195183797231948)
E83 ( V66 V58 F114 F45 ) (-0.250731633266202957877056,-8.851610570438197100884281)
E89 ( V78 V17 F87 F48 ) (-5.405692304679479143203480,-7.013727022756347295739943)
E95 ( V78 V17 F104 F54 ) (-8.340901577015673140635954,2.973758047513468838474182)
E101 ( V58 V66 F51 F110 ) (8.495862248428949214940076,2.496838140195183797231948)
E107 ( V17 V78 F52 F98 ) (-8.495862248428949214940076,-2.496838140195183797231948)
E113 ( V58 V66 F55 F92 ) (-8.340901577015673140635954,2.973758047513468838474182)
E119 ( V58 V17 F43 F42 ) (9.999999999999998223643161,0.000000000000000000000000)
E120 ( V58 V17 F46 F45 ) (-8.090169943749472736271855,5.877852522924729150588519)
E121 ( V17 V58 F49 F48 ) (-3.090169943749470959915016,9.510565162951532869328730)
E122 ( V58 V17 F52 F51 ) (3.090169943749474512628694,9.510565162951532869328730)
E123 ( V17 V58 F55 F54 ) (8.090169943749472736271855,5.877852522924731815123778)
E124 ( V58 V17 F93 F105 ) (-44.484912260248208326629538,-32.320180611657399083469500)
E127 ( V58 V17 F95 F99 ) (35.989050011819252006262104,26.147575376098512123235196)
E131 ( V17 V58 F101 F71 ) (13.746593881695154948374693,9.987485070269814357857285)
E135 ( V17 V58 F107 F111 ) (-35.989050011819252006262104,-26.147575376098512123235196)
E139 ( V58 V17 F113 F87 ) (-13.746593881695154948374693,-9.987485070269814357857285)
E141 ( V17 V58 F93 F92 ) (13.746593881695147842947335,-9.987485070269816134214125)
E142 ( V58 V58 F93 F95 ) (30.738318378553056930968523,42.307665681927211664969946)
E143 ( V17 V58 F95 F96 ) (-5.250731633266198628007260,16.160090305828699541734750)
E144 ( V58 V17 F99 F98 ) (16.991724496857901982593830,0.000000000000003052357008)
E145 ( V17 V17 F99 F101 ) (18.997325514961357129095632,26.147575376098512123235196)
E146 ( V58 V17 F101 F102 ) (5.250731633266202180720938,16.160090305828699541734750)
E147 ( V58 V17 F105 F104 ) (-13.746593881695147842947335,9.987485070269816134214125)
E148 ( V17 V17 F105 F107 ) (-30.738318378553056930968523,-42.307665681927211664969946)
E149 ( V58 V17 F107 F108 ) (5.250731633266198628007260,-16.160090305828699541734750)
E150 ( V17 V58 F111 F110 ) (-16.991724496857901982593830,-0.000000000000003052357008)
E151 ( V58 V58 F111 F113 ) (-18.997325514961357129095632,-26.147575376098512123235196)
E152 ( V17 V58 F113 F114 ) (-5.250731633266202180720938,-16.160090305828699541734750)
# Faces: 
F42 +E5 +E31 -E119
F43 +E11 -E4 +E119
F45 -E83 +E67 -E120
F46 -E17 +E61 +E120
F48 -E89 +E79 -E121
F49 -E23 +E73 +E121
F51 +E101 -E49 -E122
F52 +E107 -E37 +E122
F54 -E95 -E55 -E123
F55 +E113 -E43 +E123
F71 -E67 +E23 -E131
F87 -E61 +E89 -E139
F92 -E141 +E49 -E113
F93 +E142 +E124 +E141
F95 +E143 -E142 +E127
F96 -E143 +E43 -E5
F98 -E144 -E79 -E107
F99 +E145 -E127 +E144
F101 +E146 -E145 +E131
F102 -E146 +E4 +E17
F104 -E147 +E37 +E95
F105 +E148 -E124 +E147
F107 +E149 -E148 +E135
F108 -E149 +E55 -E11
F110 -E150 -E73 -E101
F111 +E151 -E135 +E150
F113 +E152 -E151 +E139
F114 -E152 -E31 +E83
#UEdges Algebraic 
#E4: (8 0 -6 0 4 0 -2 0)+(-3 0 6 0 -4 0 2 0)t1
#E5: (2 0 6 0 -4 0 2 0)+(3 0 -6 0 4 0 -2 0)t1
#E11: (-2 0 -6 0 4 0 -2 0)+(-3 0 6 0 -4 0 2 0)t1
#E17: (-8 0 6 0 -4 0 2 0)+(3 0 -1 0 -1 0 3 0)t1
#E23: (-2 0 4 0 -6 0 8 0)+(2 0 -4 0 6 0 -3 0)t1
#E31: (8 0 -6 0 4 0 -2 0)+(-3 0 6 0 -4 0 2 0)t1
#E37: (-2 0 4 0 4 0 -2 0)+(2 0 -4 0 1 0 2 0)t1
#E43: (2 0 6 0 -4 0 2 0)+(-2 0 -1 0 4 0 -2 0)t1
#E49: (2 0 -4 0 -4 0 2 0)+(-2 0 4 0 -1 0 -2 0)t1
#E55: (-2 0 -6 0 4 0 -2 0)+(2 0 1 0 -4 0 2 0)t1
#E61: (2 0 -4 0 6 0 -8 0)+(3 0 -1 0 -1 0 3 0)t1
#E67: (-2 0 4 0 -6 0 8 0)+(-3 0 1 0 1 0 -3 0)t1
#E73: (-2 0 4 0 -6 0 -2 0)+(2 0 -4 0 6 0 -3 0)t1
#E79: (2 0 -4 0 6 0 2 0)+(-2 0 4 0 -6 0 3 0)t1
#E83: (8 0 -6 0 4 0 -2 0)+(-3 0 1 0 1 0 -3 0)t1
#E89: (2 0 -4 0 6 0 -8 0)+(-2 0 4 0 -6 0 3 0)t1
#E95: (2 0 -4 0 -4 0 2 0)+(-2 0 -1 0 4 0 -2 0)t1
#E101: (2 0 -4 0 6 0 2 0)+(-2 0 4 0 -1 0 -2 0)t1
#E107: (-2 0 4 0 -6 0 -2 0)+(2 0 -4 0 1 0 2 0)t1
#E113: (2 0 -4 0 -4 0 2 0)+(-2 0 -1 0 4 0 -2 0)t1
#E119: (10 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E120: (-10 0 10 0 -10 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E121: (0 0 0 0 0 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E122: (0 0 0 0 10 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E123: (0 0 10 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E124: (0 0 0 0 0 0 0 0)+(0 0 -10 0 0 0 0 0)t1
#E127: (0 0 0 0 0 0 0 0)+(5 0 0 0 5 0 0 0)t1
#E131: (0 0 0 0 0 0 0 0)+(5 0 -5 0 5 0 0 0)t1
#E135: (0 0 0 0 0 0 0 0)+(-5 0 0 0 -5 0 0 0)t1
#E139: (0 0 0 0 0 0 0 0)+(-5 0 5 0 -5 0 0 0)t1
#E141: (0 0 0 0 0 0 0 0)+(0 0 5 0 -5 0 0 0)t1
#E142: (0 0 0 0 0 0 0 0)+(0 0 5 0 5 0 0 0)t1
#E143: (0 0 0 0 0 0 0 0)+(-5 0 5 0 0 0 0 0)t1
#E144: (0 0 0 0 0 0 0 0)+(0 0 0 0 5 0 -5 0)t1
#E145: (0 0 0 0 0 0 0 0)+(5 0 0 0 0 0 5 0)t1
#E146: (0 0 0 0 0 0 0 0)+(0 0 5 0 -5 0 5 0)t1
#E147: (0 0 0 0 0 0 0 0)+(0 0 -5 0 5 0 0 0)t1
#E148: (0 0 0 0 0 0 0 0)+(0 0 -5 0 -5 0 0 0)t1
#E149: (0 0 0 0 0 0 0 0)+(5 0 -5 0 0 0 0 0)t1
#E150: (0 0 0 0 0 0 0 0)+(0 0 0 0 -5 0 5 0)t1
#E151: (0 0 0 0 0 0 0 0)+(-5 0 0 0 0 0 -5 0)t1
#E152: (0 0 0 0 0 0 0 0)+(0 0 -5 0 5 0 -5 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 4
## V17(32) (8 PI) V58(32) (8 PI) V66(10) (6 PI) V78(10) (6 PI) 
# edges: 42
## E4(2) E5(2) E11(2) E17(2) E23(2) E31(2) E37(2) E43(2) E49(2) E55(2) E61(2) E67(2) E73(2) E79(2) E83(2) E89(2) E95(2) E101(2) E107(2) E113(2) E119(2) E120(2) E121(2) E122(2) E123(2) E124(2) E127(2) E131(2) E135(2) E139(2) E141(2) E142(2) E143(2) E144(2) E145(2) E146(2) E147(2) E148(2) E149(2) E150(2) E151(2) E152(2) 
# faces: 28
# genus: 6
# area: 2799.01
sweeping start V17 depth = 105.811
File =  depth = 105.811 follow_depth = 529.057 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 72 unusual 0 close 45992 reject 0
V17: AhBgCfDe V58: aEdFcGbH V66: IkJjKi V78: LmMlNn   cylinders |1|1|1.12832|1.12832|1.61803|1.61803|1.74636|2.31538|
Moduli: |3.97657|3.97657|1.20403|1.20403|1|1|1.4073|1.38127| Min Area: 1 Max Area: 1
    total: 0.670861 0.407835 sym = 8
    raw = 41 short saddle = 10 short cyl = 54.4849 tag #0
V17: AdBcCbDa V58: EgFfGeHh V66: InJmKl V78: iLkMjN   cylinders |1|1|1.98176|1.98176|3.20656|3.20656|3.59979|3.82459|
Moduli: |39.5196|39.5196|1|1|13.3246|13.3246|17.7632|6.38616| Min Area: 1 Max Area: 1
    total: 0.654498 0.397887 sym = 8
    raw = 40 short saddle = 14.617 short cyl = 32.3202 tag #4
V17: AaXXXXXX V58: BXXXXXXb   cylinders |1|
Moduli: |1| Min Area: 0.11547 Max Area: 0.11547
    total: 1.309 0.795775 sym = 2
    raw = 20 short saddle = 63.3498 short cyl = 63.3498 tag #7

total cylinders: 13.2209 8.03732 raw = 101 rejected = 0
""",
    (2, 2, 4, 4): r"""
max loc = 2
alpha =  4 beta = 4 gamma = 2 delta =2
a =  2.0944 b = 2.0944 c= 1.0472 d=1.0472
a1_min = 4.44089e-16 a1_max = 1.0472
a1 = 0.713864
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 2.64682 p-q = 2.00113
p = (1,0) q = (2.00057,1.73303) r = (-1.00057,1.73303)
Imaginary of t_cx = -5.55112e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (4.44089e-16,-8.88178e-16)
Checking r_alt (2.22045e-16,0)
Checking p_algt (0,0)
Checking q_algt (4.44089e-16,-8.88178e-16)
Checking r_algt (2.22045e-16,0)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.0011307609945161
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (1.000565380497258516356851,1.733030075315736828756030)
E2 ( V2 V3 F0 NULL ) (-3.001130760994516144535282,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (1.000565380497257628178431,-1.733030075315736828756030)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 0 0 0)+(0 0 1 0)t1
#E2: (-1 0 0 0)+(-1 0 0 0)t1
#E3: (0 0 0 0)+(1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.333333, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.333333, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.333333
In RetriangulateSurface
Removing V2
F13 V1(E13) V0(E15) a=0, can_bisect=0
F13 V1(E13) V15(E9) a=0.333333, can_bisect=1
F13 V1(E13) V0(E13) a=0.804483, can_bisect=1
F13 V1(E13) V1(E22) a=1.5708, can_bisect=1
F13 V1(E13) V0(E3) a=0.408689, can_bisect=1
F13 V1(E13) V15(E15) a=1.0472, can_bisect=1
F13 V1(E13) V0(E22) a=0.408689, can_bisect=1
F13 V1(E13) V1(E0) a=1.5708, can_bisect=1
F13 V1(E13) V0(E9) a=0.804483, can_bisect=1
F13 V1(E13) V15(E3) a=0.333333, can_bisect=1
F13 V1(E13) V0(E0) a=0, can_bisect=0
F13 V0(E15) V15(E9) a=0, can_bisect=0
F13 V0(E15) V0(E13) a=0.523599, can_bisect=1
F13 V0(E15) V1(E22) a=0.804483, can_bisect=1
F13 V0(E15) V0(E3) a=0.190265, can_bisect=1
F13 V0(E15) V15(E15) a=0.891084, can_bisect=1
F13 V0(E15) V0(E22) a=0.523599, can_bisect=1
F13 V0(E15) V1(E0) a=0.408689, can_bisect=1
F13 V0(E15) V0(E9) a=0.190265, can_bisect=1
F13 V0(E15) V15(E3) a=0.242489, can_bisect=0
F13 V0(E15) V0(E0) a=2.61799, can_bisect=0
F13 V15(E9) V0(E13) a=0, can_bisect=0
F13 V15(E9) V1(E22) a=0.333333, can_bisect=1
F13 V15(E9) V0(E3) a=0.242489, can_bisect=0
F13 V15(E9) V15(E15) a=0.523599, can_bisect=1
F13 V15(E9) V0(E22) a=0.891084, can_bisect=1
F13 V15(E9) V1(E0) a=1.0472, can_bisect=1
F13 V15(E9) V0(E9) a=0.891084, can_bisect=1
F13 V15(E9) V15(E3) a=0.523599, can_bisect=1
F13 V15(E9) V0(E0) a=0.242489, can_bisect=0
F13 V0(E13) V1(E22) a=0, can_bisect=0
F13 V0(E13) V0(E3) a=2.61799, can_bisect=0
F13 V0(E13) V15(E15) a=0.242489, can_bisect=0
F13 V0(E13) V0(E22) a=0.190265, can_bisect=1
F13 V0(E13) V1(E0) a=0.408689, can_bisect=1
F13 V0(E13) V0(E9) a=0.523599, can_bisect=1
F13 V0(E13) V15(E3) a=0.891084, can_bisect=1
F13 V0(E13) V0(E0) a=0.190265, can_bisect=1
F13 V1(E22) V0(E3) a=0, can_bisect=0
F13 V1(E22) V15(E15) a=0.333333, can_bisect=1
F13 V1(E22) V0(E22) a=0.804483, can_bisect=1
F13 V1(E22) V1(E0) a=1.5708, can_bisect=1
F13 V1(E22) V0(E9) a=0.408689, can_bisect=1
F13 V1(E22) V15(E3) a=1.0472, can_bisect=1
F13 V1(E22) V0(E0) a=0.408689, can_bisect=1
F13 V0(E3) V15(E15) a=0, can_bisect=0
F13 V0(E3) V0(E22) a=0.523599, can_bisect=1
F13 V0(E3) V1(E0) a=0.804483, can_bisect=1
F13 V0(E3) V0(E9) a=0.190265, can_bisect=1
F13 V0(E3) V15(E3) a=0.891084, can_bisect=1
F13 V0(E3) V0(E0) a=0.523599, can_bisect=1
F13 V15(E15) V0(E22) a=0, can_bisect=0
F13 V15(E15) V1(E0) a=0.333333, can_bisect=1
F13 V15(E15) V0(E9) a=0.242489, can_bisect=0
F13 V15(E15) V15(E3) a=0.523599, can_bisect=1
F13 V15(E15) V0(E0) a=0.891084, can_bisect=1
F13 V0(E22) V1(E0) a=0, can_bisect=0
F13 V0(E22) V0(E9) a=2.61799, can_bisect=0
F13 V0(E22) V15(E3) a=0.242489, can_bisect=0
F13 V0(E22) V0(E0) a=0.190265, can_bisect=1
F13 V1(E0) V0(E9) a=0, can_bisect=0
F13 V1(E0) V15(E3) a=0.333333, can_bisect=1
F13 V1(E0) V0(E0) a=0.804483, can_bisect=1
F13 V0(E9) V15(E3) a=0, can_bisect=0
F13 V0(E9) V0(E0) a=0.523599, can_bisect=1
F13 V15(E3) V0(E0) a=0, can_bisect=0
F13 best_angle = 1.5708
F15 V1(E22) V0(E3) a=0, can_bisect=0
F15 V1(E22) V15(E15) a=0.333333, can_bisect=1
F15 V1(E22) V0(E22) a=0.804483, can_bisect=1
F15 V1(E22) V1(E0) a=1.0472, can_bisect=1
F15 V1(E22) V0(E9) a=0.408689, can_bisect=1
F15 V1(E22) V15(E3) a=0.523599, can_bisect=1
F15 V1(E22) V0(E0) a=0.114909, can_bisect=1
F15 V1(E22) V1(E35) a=0, can_bisect=0
F15 V0(E3) V15(E15) a=0, can_bisect=0
F15 V0(E3) V0(E22) a=0.523599, can_bisect=1
F15 V0(E3) V1(E0) a=0.804483, can_bisect=1
F15 V0(E3) V0(E9) a=0.190265, can_bisect=1
F15 V0(E3) V15(E3) a=0.891084, can_bisect=1
F15 V0(E3) V0(E0) a=0.523599, can_bisect=1
F15 V0(E3) V1(E35) a=0.114909, can_bisect=1
F15 V15(E15) V0(E22) a=0, can_bisect=0
F15 V15(E15) V1(E0) a=0.333333, can_bisect=1
F15 V15(E15) V0(E9) a=0.242489, can_bisect=0
F15 V15(E15) V15(E3) a=0.523599, can_bisect=1
F15 V15(E15) V0(E0) a=0.891084, can_bisect=1
F15 V15(E15) V1(E35) a=0.523599, can_bisect=1
F15 V0(E22) V1(E0) a=0, can_bisect=0
F15 V0(E22) V0(E9) a=2.61799, can_bisect=0
F15 V0(E22) V15(E3) a=0.242489, can_bisect=0
F15 V0(E22) V0(E0) a=0.190265, can_bisect=1
F15 V0(E22) V1(E35) a=0.408689, can_bisect=1
F15 V1(E0) V0(E9) a=0, can_bisect=0
F15 V1(E0) V15(E3) a=0.333333, can_bisect=1
F15 V1(E0) V0(E0) a=0.804483, can_bisect=1
F15 V1(E0) V1(E35) a=1.0472, can_bisect=1
F15 V0(E9) V15(E3) a=0, can_bisect=0
F15 V0(E9) V0(E0) a=0.523599, can_bisect=1
F15 V0(E9) V1(E35) a=0.804483, can_bisect=1
F15 V15(E3) V0(E0) a=0, can_bisect=0
F15 V15(E3) V1(E35) a=0.333333, can_bisect=1
F15 V0(E0) V1(E35) a=0, can_bisect=0
F15 best_angle = 1.0472
F17 V1(E0) V0(E9) a=0, can_bisect=0
F17 V1(E0) V15(E3) a=0.333333, can_bisect=1
F17 V1(E0) V0(E0) a=0.804483, can_bisect=1
F17 V1(E0) V1(E35) a=1.0472, can_bisect=1
F17 V1(E0) V1(E36) a=0, can_bisect=0
F17 V0(E9) V15(E3) a=0, can_bisect=0
F17 V0(E9) V0(E0) a=0.523599, can_bisect=1
F17 V0(E9) V1(E35) a=0.804483, can_bisect=1
F17 V0(E9) V1(E36) a=0.114909, can_bisect=1
F17 V15(E3) V0(E0) a=0, can_bisect=0
F17 V15(E3) V1(E35) a=0.333333, can_bisect=1
F17 V15(E3) V1(E36) a=0.523599, can_bisect=1
F17 V0(E0) V1(E35) a=0, can_bisect=0
F17 V0(E0) V1(E36) a=0.114909, can_bisect=1
F17 V1(E35) V1(E36) a=0, can_bisect=0
F17 best_angle = 1.0472
F18 V1(E0) V0(E9) a=0, can_bisect=0
F18 V1(E0) V15(E3) a=0.333333, can_bisect=1
F18 V1(E0) V0(E0) a=0.280884, can_bisect=1
F18 V1(E0) V1(E37) a=0, can_bisect=0
F18 V0(E9) V15(E3) a=0, can_bisect=0
F18 V0(E9) V0(E0) a=0.523599, can_bisect=1
F18 V0(E9) V1(E37) a=0.280884, can_bisect=1
F18 V15(E3) V0(E0) a=0, can_bisect=0
F18 V15(E3) V1(E37) a=0.333333, can_bisect=1
F18 V0(E0) V1(E37) a=0, can_bisect=0
F18 best_angle = 0.523599
F21 V0(E0) V1(E37) a=0, can_bisect=0
F21 V0(E0) V1(E0) a=0.280884, can_bisect=1
F21 V0(E0) V0(E38) a=0, can_bisect=0
F21 V1(E37) V1(E0) a=0, can_bisect=0
F21 V1(E37) V0(E38) a=0.280884, can_bisect=1
F21 V1(E0) V0(E38) a=0, can_bisect=0
F21 best_angle = 0.280884
F16 V1(E22) V0(E3) a=0, can_bisect=0
F16 V1(E22) V15(E15) a=0.333333, can_bisect=1
F16 V1(E22) V0(E22) a=0.280884, can_bisect=1
F16 V1(E22) V1(E36) a=0, can_bisect=0
F16 V0(E3) V15(E15) a=0, can_bisect=0
F16 V0(E3) V0(E22) a=0.523599, can_bisect=1
F16 V0(E3) V1(E36) a=0.280884, can_bisect=1
F16 V15(E15) V0(E22) a=0, can_bisect=0
F16 V15(E15) V1(E36) a=0.333333, can_bisect=1
F16 V0(E22) V1(E36) a=0, can_bisect=0
F16 best_angle = 0.523599
F25 V0(E22) V1(E36) a=0, can_bisect=0
F25 V0(E22) V1(E22) a=0.280884, can_bisect=1
F25 V0(E22) V0(E40) a=0, can_bisect=0
F25 V1(E36) V1(E22) a=0, can_bisect=0
F25 V1(E36) V0(E40) a=0.280884, can_bisect=1
F25 V1(E22) V0(E40) a=0, can_bisect=0
F25 best_angle = 0.280884
F14 V1(E13) V0(E15) a=0, can_bisect=0
F14 V1(E13) V15(E9) a=0.333333, can_bisect=1
F14 V1(E13) V0(E13) a=0.280884, can_bisect=1
F14 V1(E13) V1(E35) a=0, can_bisect=0
F14 V0(E15) V15(E9) a=0, can_bisect=0
F14 V0(E15) V0(E13) a=0.523599, can_bisect=1
F14 V0(E15) V1(E35) a=0.280884, can_bisect=1
F14 V15(E9) V0(E13) a=0, can_bisect=0
F14 V15(E9) V1(E35) a=0.333333, can_bisect=1
F14 V0(E13) V1(E35) a=0, can_bisect=0
F14 best_angle = 0.523599
F29 V0(E13) V1(E35) a=0, can_bisect=0
F29 V0(E13) V1(E13) a=0.280884, can_bisect=1
F29 V0(E13) V0(E42) a=0, can_bisect=0
F29 V1(E35) V1(E13) a=0, can_bisect=0
F29 V1(E35) V0(E42) a=0.280884, can_bisect=1
F29 V1(E13) V0(E42) a=0, can_bisect=0
F29 best_angle = 0.280884
Removing V15
Longest Edge: 3.60743
Looking for shortest saddle...1
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.0011307609945161
# Vertices: V0 V1
# UEdges: 
E0 ( V0 V1 F23 F22 ) (1.000000000000000000000000,0.000000000000000000000000)
E13 ( V1 V0 F31 F30 ) (0.499999999999999777955395,-0.866025403784438596588302)
E22 ( V1 V0 F27 F26 ) (0.500000000000000222044605,0.866025403784438596588302)
E35 ( V1 V1 F19 F30 ) (3.001696141491774660892133,1.733030075315736828756030)
E36 ( V1 V1 F19 F26 ) (-3.001696141491773772713714,1.733030075315736828756030)
E37 ( V1 V1 F19 F22 ) (-0.000000000000000888178420,-3.466060150631473657512061)
E38 ( V0 V0 F23 F32 ) (-0.000000000000000888178420,-3.466060150631473657512061)
E39 ( V1 V0 F23 F22 ) (-0.999999999999999111821580,3.466060150631473657512061)
E40 ( V0 V0 F27 F32 ) (-3.001696141491773772713714,1.733030075315736828756030)
E41 ( V0 V1 F27 F26 ) (2.501696141491773772713714,-2.599055479100175425344332)
E42 ( V0 V0 F31 F32 ) (3.001696141491774660892133,1.733030075315736828756030)
E43 ( V0 V1 F31 F30 ) (-3.501696141491774660892133,-0.867004671531298232167728)
# Faces: 
F19 +E35 +E36 +E37
F22 -E37 -E0 -E39
F23 +E38 +E0 +E39
F26 -E22 -E36 -E41
F27 +E22 +E40 +E41
F30 -E13 -E35 -E43
F31 +E13 +E42 +E43
F32 -E42 -E40 -E38
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E13: (1 0 -1 0)+(0 0 0 0)t1
#E22: (0 0 1 0)+(0 0 0 0)t1
#E35: (0 0 0 0)+(1 0 1 0)t1
#E36: (0 0 0 0)+(-2 0 1 0)t1
#E37: (0 0 0 0)+(1 0 -2 0)t1
#E38: (0 0 0 0)+(1 0 -2 0)t1
#E39: (-1 0 0 0)+(-1 0 2 0)t1
#E40: (0 0 0 0)+(-2 0 1 0)t1
#E41: (0 0 -1 0)+(2 0 -1 0)t1
#E42: (0 0 0 0)+(1 0 1 0)t1
#E43: (-1 0 1 0)+(-1 0 -1 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V0(12) (4 PI) V1(12) (4 PI) 
# edges: 12
## E0(2) E13(2) E22(2) E35(2) E36(2) E37(2) E38(2) E39(2) E40(2) E41(2) E42(2) E43(2) 
# faces: 8
# genus: 2
# area: 20.8022
sweeping start V0 depth = 45.6095
File =  depth = 45.6095 follow_depth = 228.047 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 4 unusual 0 close 64535 reject 0
V0: AaXX V1: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0416314 Max Area: 0.874635
    total: 2.60752 1.58518 sym = 2
    raw = 996 short saddle = 4.58443 short cyl = 4.58443 tag #2
V0: AaBb V1: CdDc   cylinders |1|1.49972|2.49972|
Moduli: |4.99943|8.33962|1| Min Area: 1 Max Area: 1
    total: 0.017017 0.0103451 sym = 4
    raw = 13 short saddle = 3.46606 short cyl = 3.46606 tag #0
V0: AdBc V1: aCbD   cylinders |1|1.66679|
Moduli: |1.66868|1| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 1 short cyl = 9.00339 tag #5

total cylinders: 2.72795 1.65839 raw = 521 rejected = 0
""",
    (2, 2, 6, 10): r"""
max loc = 3
alpha =  10 beta = 2 gamma = 2 delta =6
a =  3.14159 b = 0.628319 c= 0.628319 d=1.88496
a1_min = 1.88496 a1_max = 2.51327
a1 = 2.31327
angle a check: 0
angle b check: -2.22045e-16
angle c check: 3.14159
angle d check: 0
p = 1 q = 2.95861 p-q = 3.70865
p = (1,0) q = (-2.00036,2.17989) r = (-1.29207,1.21008e-16)
Imaginary of t_cx = -1.21008e-16
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(-1 0 0 0 0 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (4.44089e-16,-4.44089e-16)
Checking r_alt (0,-1.21008e-16)
Checking p_algt (0,0)
Checking q_algt (4.44089e-16,-4.44089e-16)
Checking r_algt (0,-1.21008e-16)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=1.2920734071754343
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-3.000363042347531372655567,2.179891349721030824326817)
E2 ( V2 V3 F0 NULL ) (0.708289635172097087334464,-2.179891349721030824326817)
E3 ( V3 V0 F0 NULL ) (1.292073407175434285321103,-0.000000000000000000000000)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (-1 0 0 0 0 0 1 0)+(-1 0 0 0 0 0 1 0)t1
#E2: (0 0 0 0 0 0 -1 0)+(0 0 0 0 0 0 -1 0)t1
#E3: (0 0 0 0 0 0 0 0)+(1 0 0 0 0 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.2, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.2
In RetriangulateSurface
Removing V0
F21 V3(E10) V40(E6) a=0, can_bisect=0
F21 V3(E10) V21(E1) a=0.628319, can_bisect=1
F21 V3(E10) V40(E2) a=0, can_bisect=0
F21 V40(E6) V21(E1) a=0, can_bisect=0
F21 V40(E6) V40(E2) a=0.942478, can_bisect=0
F21 V21(E1) V40(E2) a=0, can_bisect=0
F21 best_angle = 0.628319
Removing V10
F24 V40(E38) V3(E16) a=0, can_bisect=0
F24 V40(E38) V40(E1) a=0.942478, can_bisect=0
F24 V40(E38) V21(E21) a=0, can_bisect=0
F24 V3(E16) V40(E1) a=0, can_bisect=0
F24 V3(E16) V21(E21) a=0.628319, can_bisect=1
F24 V40(E1) V21(E21) a=0, can_bisect=0
F24 best_angle = 0.628319
Removing V17
F27 V40(E21) V21(E45) a=0, can_bisect=0
F27 V40(E21) V40(E41) a=0.942478, can_bisect=0
F27 V40(E21) V3(E2) a=0, can_bisect=0
F27 V21(E45) V40(E41) a=0, can_bisect=0
F27 V21(E45) V3(E2) a=0.628319, can_bisect=1
F27 V40(E41) V3(E2) a=0, can_bisect=0
F27 best_angle = 0.628319
Removing V21
F30 V40(E38) V3(E16) a=0, can_bisect=0
F30 V40(E38) V40(E2) a=1.5708, can_bisect=0
F30 V40(E38) V3(E10) a=0.169861, can_bisect=1
F30 V40(E38) V40(E25) a=0.314159, can_bisect=1
F30 V40(E38) V22(E47) a=0.628319, can_bisect=1
F30 V40(E38) V40(E31) a=0.314159, can_bisect=0
F30 V40(E38) V30(E53) a=0.395879, can_bisect=1
F30 V40(E38) V40(E41) a=1.14248, can_bisect=0
F30 V40(E38) V3(E2) a=0, can_bisect=0
F30 V3(E16) V40(E2) a=0, can_bisect=0
F30 V3(E16) V3(E10) a=0.942478, can_bisect=1
F30 V3(E16) V40(E25) a=0.169861, can_bisect=1
F30 V3(E16) V22(E47) a=1.69764, can_bisect=1
F30 V3(E16) V40(E31) a=0.2, can_bisect=1
F30 V3(E16) V30(E53) a=1.69764, can_bisect=1
F30 V3(E16) V40(E41) a=0.169861, can_bisect=1
F30 V3(E16) V3(E2) a=0.942478, can_bisect=1
F30 V40(E2) V3(E10) a=0, can_bisect=0
F30 V40(E2) V40(E25) a=1.14248, can_bisect=0
F30 V40(E2) V22(E47) a=0.395879, can_bisect=1
F30 V40(E2) V40(E31) a=0.314159, can_bisect=0
F30 V40(E2) V30(E53) a=0.628319, can_bisect=1
F30 V40(E2) V40(E41) a=0.314159, can_bisect=1
F30 V40(E2) V3(E2) a=0.169861, can_bisect=1
F30 V3(E10) V40(E25) a=0, can_bisect=0
F30 V3(E10) V22(E47) a=0.875506, can_bisect=1
F30 V3(E10) V40(E31) a=0.658458, can_bisect=0
F30 V3(E10) V30(E53) a=1.69764, can_bisect=1
F30 V3(E10) V40(E41) a=0.2, can_bisect=1
F30 V3(E10) V3(E2) a=1.5708, can_bisect=1
F30 V40(E25) V22(E47) a=0, can_bisect=0
F30 V40(E25) V40(E31) a=1.14248, can_bisect=0
F30 V40(E25) V30(E53) a=0.860758, can_bisect=0
F30 V40(E25) V40(E41) a=0.942478, can_bisect=0
F30 V40(E25) V3(E2) a=0.2, can_bisect=1
F30 V22(E47) V40(E31) a=0, can_bisect=0
F30 V22(E47) V30(E53) a=1.3708, can_bisect=1
F30 V22(E47) V40(E41) a=0.860758, can_bisect=0
F30 V22(E47) V3(E2) a=1.69764, can_bisect=1
F30 V40(E31) V30(E53) a=0, can_bisect=0
F30 V40(E31) V40(E41) a=1.14248, can_bisect=0
F30 V40(E31) V3(E2) a=0.658458, can_bisect=0
F30 V30(E53) V40(E41) a=0, can_bisect=0
F30 V30(E53) V3(E2) a=0.875506, can_bisect=1
F30 V40(E41) V3(E2) a=0, can_bisect=0
F30 best_angle = 1.69764
F32 V30(E53) V40(E41) a=0, can_bisect=0
F32 V30(E53) V3(E2) a=0.875506, can_bisect=1
F32 V30(E53) V40(E38) a=0.395879, can_bisect=1
F32 V30(E53) V3(E62) a=0, can_bisect=0
F32 V40(E41) V3(E2) a=0, can_bisect=0
F32 V40(E41) V40(E38) a=1.14248, can_bisect=0
F32 V40(E41) V3(E62) a=0.169861, can_bisect=1
F32 V3(E2) V40(E38) a=0, can_bisect=0
F32 V3(E2) V3(E62) a=0.755158, can_bisect=1
F32 V40(E38) V3(E62) a=0, can_bisect=0
F32 best_angle = 0.875506
F34 V3(E2) V40(E38) a=0, can_bisect=0
F34 V3(E2) V3(E62) a=0.755158, can_bisect=1
F34 V3(E2) V30(E63) a=0, can_bisect=0
F34 V40(E38) V3(E62) a=0, can_bisect=0
F34 V40(E38) V30(E63) a=0.395879, can_bisect=1
F34 V3(E62) V30(E63) a=0, can_bisect=0
F34 best_angle = 0.755158
F31 V3(E16) V40(E2) a=0, can_bisect=0
F31 V3(E16) V3(E10) a=0.942478, can_bisect=1
F31 V3(E16) V40(E25) a=0.169861, can_bisect=1
F31 V3(E16) V22(E47) a=0.374639, can_bisect=1
F31 V3(E16) V40(E31) a=0.187319, can_bisect=1
F31 V3(E16) V30(E62) a=0, can_bisect=0
F31 V40(E2) V3(E10) a=0, can_bisect=0
F31 V40(E2) V40(E25) a=1.14248, can_bisect=0
F31 V40(E2) V22(E47) a=0.395879, can_bisect=1
F31 V40(E2) V40(E31) a=0.314159, can_bisect=0
F31 V40(E2) V30(E62) a=0.440999, can_bisect=1
F31 V3(E10) V40(E25) a=0, can_bisect=0
F31 V3(E10) V22(E47) a=0.875506, can_bisect=1
F31 V3(E10) V40(E31) a=0.658458, can_bisect=0
F31 V3(E10) V30(E62) a=0.881998, can_bisect=1
F31 V40(E25) V22(E47) a=0, can_bisect=0
F31 V40(E25) V40(E31) a=1.14248, can_bisect=0
F31 V40(E25) V30(E62) a=0.860758, can_bisect=0
F31 V22(E47) V40(E31) a=0, can_bisect=0
F31 V22(E47) V30(E62) a=1.3708, can_bisect=1
F31 V40(E31) V30(E62) a=0, can_bisect=0
F31 best_angle = 1.3708
F38 V30(E62) V3(E16) a=0, can_bisect=0
F38 V30(E62) V40(E2) a=0.440999, can_bisect=1
F38 V30(E62) V3(E10) a=0.501479, can_bisect=1
F38 V30(E62) V40(E25) a=0.860758, can_bisect=0
F38 V30(E62) V22(E65) a=0, can_bisect=0
F38 V3(E16) V40(E2) a=0, can_bisect=0
F38 V3(E16) V3(E10) a=0.942478, can_bisect=1
F38 V3(E16) V40(E25) a=0.169861, can_bisect=1
F38 V3(E16) V22(E65) a=0.374639, can_bisect=1
F38 V40(E2) V3(E10) a=0, can_bisect=0
F38 V40(E2) V40(E25) a=1.14248, can_bisect=0
F38 V40(E2) V22(E65) a=0.395879, can_bisect=1
F38 V3(E10) V40(E25) a=0, can_bisect=0
F38 V3(E10) V22(E65) a=0.875506, can_bisect=1
F38 V40(E25) V22(E65) a=0, can_bisect=0
F38 best_angle = 0.942478
F40 V3(E10) V40(E25) a=0, can_bisect=0
F40 V3(E10) V22(E65) a=0.875506, can_bisect=1
F40 V3(E10) V30(E62) a=0.501479, can_bisect=1
F40 V3(E10) V3(E66) a=0, can_bisect=0
F40 V40(E25) V22(E65) a=0, can_bisect=0
F40 V40(E25) V30(E62) a=0.860758, can_bisect=0
F40 V40(E25) V3(E66) a=0.144299, can_bisect=1
F40 V22(E65) V30(E62) a=0, can_bisect=0
F40 V22(E65) V3(E66) a=0.374639, can_bisect=1
F40 V30(E62) V3(E66) a=0, can_bisect=0
F40 best_angle = 0.875506
F42 V22(E65) V30(E62) a=0, can_bisect=0
F42 V22(E65) V3(E66) a=0.374639, can_bisect=1
F42 V22(E65) V3(E67) a=0, can_bisect=0
F42 V30(E62) V3(E66) a=0, can_bisect=0
F42 V30(E62) V3(E67) a=0.259868, can_bisect=1
F42 V3(E66) V3(E67) a=0, can_bisect=0
F42 best_angle = 0.374639
Removing V22
F45 V3(E10) V40(E16) a=0, can_bisect=0
F45 V3(E10) V3(E41) a=0.942478, can_bisect=1
F45 V3(E10) V40(E31) a=0.169861, can_bisect=1
F45 V3(E10) V30(E62) a=0.881998, can_bisect=1
F45 V3(E10) V3(E66) a=0, can_bisect=0
F45 V40(E16) V3(E41) a=0, can_bisect=0
F45 V40(E16) V40(E31) a=1.14248, can_bisect=0
F45 V40(E16) V30(E62) a=0.395879, can_bisect=1
F45 V40(E16) V3(E66) a=0.144299, can_bisect=1
F45 V3(E41) V40(E31) a=0, can_bisect=0
F45 V3(E41) V30(E62) a=0.875506, can_bisect=1
F45 V3(E41) V3(E66) a=0.501479, can_bisect=1
F45 V40(E31) V30(E62) a=0, can_bisect=0
F45 V40(E31) V3(E66) a=0.187319, can_bisect=1
F45 V30(E62) V3(E66) a=0, can_bisect=0
F45 best_angle = 0.942478
F47 V3(E41) V40(E31) a=0, can_bisect=0
F47 V3(E41) V30(E62) a=0.875506, can_bisect=1
F47 V3(E41) V3(E66) a=0.501479, can_bisect=1
F47 V3(E41) V3(E69) a=0, can_bisect=0
F47 V40(E31) V30(E62) a=0, can_bisect=0
F47 V40(E31) V3(E66) a=0.187319, can_bisect=1
F47 V40(E31) V3(E69) a=0.144299, can_bisect=1
F47 V30(E62) V3(E66) a=0, can_bisect=0
F47 V30(E62) V3(E69) a=0.755158, can_bisect=1
F47 V3(E66) V3(E69) a=0, can_bisect=0
F47 best_angle = 0.875506
F49 V30(E62) V3(E66) a=0, can_bisect=0
F49 V30(E62) V3(E69) a=0.755158, can_bisect=1
F49 V30(E62) V3(E70) a=0, can_bisect=0
F49 V3(E66) V3(E69) a=0, can_bisect=0
F49 V3(E66) V3(E70) a=0.501479, can_bisect=1
F49 V3(E69) V3(E70) a=0, can_bisect=0
F49 best_angle = 0.755158
Removing V30
F52 V3(E41) V40(E10) a=0, can_bisect=0
F52 V3(E41) V3(E38) a=0.942478, can_bisect=1
F52 V3(E41) V40(E41) a=0.169861, can_bisect=1
F52 V3(E41) V3(E64) a=1.25664, can_bisect=1
F52 V3(E41) V3(E66) a=0.628319, can_bisect=1
F52 V3(E41) V3(E69) a=0, can_bisect=0
F52 V40(E10) V3(E38) a=0, can_bisect=0
F52 V40(E10) V40(E41) a=1.5708, can_bisect=0
F52 V40(E10) V3(E64) a=0.169861, can_bisect=1
F52 V40(E10) V3(E66) a=0.628319, can_bisect=1
F52 V40(E10) V3(E69) a=0.144299, can_bisect=1
F52 V3(E38) V40(E41) a=0, can_bisect=0
F52 V3(E38) V3(E64) a=0.942478, can_bisect=1
F52 V3(E38) V3(E66) a=0.628319, can_bisect=1
F52 V3(E38) V3(E69) a=0.628319, can_bisect=1
F52 V40(E41) V3(E64) a=0, can_bisect=0
F52 V40(E41) V3(E66) a=0.144299, can_bisect=1
F52 V40(E41) V3(E69) a=0.628319, can_bisect=1
F52 V3(E64) V3(E66) a=0, can_bisect=0
F52 V3(E64) V3(E69) a=0.628319, can_bisect=1
F52 V3(E66) V3(E69) a=0, can_bisect=0
F52 best_angle = 1.25664
F54 V3(E64) V3(E66) a=0, can_bisect=0
F54 V3(E64) V3(E69) a=0.628319, can_bisect=1
F54 V3(E64) V3(E72) a=0, can_bisect=0
F54 V3(E66) V3(E69) a=0, can_bisect=0
F54 V3(E66) V3(E72) a=0.628319, can_bisect=1
F54 V3(E69) V3(E72) a=0, can_bisect=0
F54 best_angle = 0.628319
F53 V3(E41) V40(E10) a=0, can_bisect=0
F53 V3(E41) V3(E38) a=0.628319, can_bisect=1
F53 V3(E41) V40(E41) a=0.169861, can_bisect=1
F53 V3(E41) V3(E72) a=0, can_bisect=0
F53 V40(E10) V3(E38) a=0, can_bisect=0
F53 V40(E10) V40(E41) a=1.5708, can_bisect=0
F53 V40(E10) V3(E72) a=0.169861, can_bisect=1
F53 V3(E38) V40(E41) a=0, can_bisect=0
F53 V3(E38) V3(E72) a=0.628319, can_bisect=1
F53 V40(E41) V3(E72) a=0, can_bisect=0
F53 best_angle = 0.628319
F58 V3(E38) V40(E41) a=0, can_bisect=0
F58 V3(E38) V3(E72) a=0.628319, can_bisect=1
F58 V3(E38) V3(E74) a=0, can_bisect=0
F58 V40(E41) V3(E72) a=0, can_bisect=0
F58 V40(E41) V3(E74) a=0.144299, can_bisect=1
F58 V3(E72) V3(E74) a=0, can_bisect=0
F58 best_angle = 0.628319
Removing V40
F61 V3(E64) V3(E66) a=0, can_bisect=0
F61 V3(E64) V3(E69) a=0.628319, can_bisect=1
F61 V3(E64) V3(E74) a=0.628319, can_bisect=1
F61 V3(E64) V3(E75) a=0, can_bisect=0
F61 V3(E66) V3(E69) a=0, can_bisect=0
F61 V3(E66) V3(E74) a=0.628319, can_bisect=1
F61 V3(E66) V3(E75) a=0.628319, can_bisect=1
F61 V3(E69) V3(E74) a=0, can_bisect=0
F61 V3(E69) V3(E75) a=0.628319, can_bisect=1
F61 V3(E74) V3(E75) a=0, can_bisect=0
F61 best_angle = 0.628319
F62 V3(E66) V3(E69) a=0, can_bisect=0
F62 V3(E66) V3(E74) a=0.628319, can_bisect=1
F62 V3(E66) V3(E76) a=0, can_bisect=0
F62 V3(E69) V3(E74) a=0, can_bisect=0
F62 V3(E69) V3(E76) a=0.628319, can_bisect=1
F62 V3(E74) V3(E76) a=0, can_bisect=0
F62 best_angle = 0.628319
Longest Edge: 4.35978
Looking for shortest saddle...2.69449
Clearing denominators: multiplying by 1
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=1.2920734071754343
# Vertices: V3
# UEdges: 
E64 ( V3 V3 F55 F63 ) (-2.562615973931912094485597,0.832644403811549915772616)
E66 ( V3 V3 F55 F64 ) (-1.583783772003336975942034,-2.179891349721030824326817)
E69 ( V3 V3 F56 F64 ) (1.583783772003336309808219,-2.179891349721030824326817)
E72 ( V3 V3 F56 F60 ) (2.562615973931912982664016,3.527138295630511954925623)
E73 ( V3 V3 F56 F55 ) (-4.146399745935249292472236,-1.347246945909480908554201)
E74 ( V3 V3 F60 F65 ) (2.562615973931912094485597,0.832644403811550137817221)
E75 ( V3 V3 F60 F63 ) (0.000000000000000444089210,2.694493891818961373019192)
E76 ( V3 V3 F63 F65 ) (-2.562615973931911206307177,3.527138295630511510836413)
E77 ( V3 V3 F65 F64 ) (0.000000000000000666133815,4.359782699442061648653635)
# Faces: 
F55 +E64 +E66 -E73
F56 +E69 +E72 +E73
F60 -E72 +E74 +E75
F63 -E75 -E64 +E76
F64 -E66 -E69 -E77
F65 -E74 -E76 +E77
#UEdges Algebraic 
#E64: (0 0 -1 0 0 0 1 0)+(0 0 -1 0 0 0 1 0)t1
#E66: (-1 0 0 0 0 0 -1 0)+(-1 0 0 0 0 0 -1 0)t1
#E69: (1 0 0 0 -1 0 0 0)+(1 0 0 0 -1 0 0 0)t1
#E72: (0 0 1 0 1 0 0 0)+(0 0 1 0 1 0 0 0)t1
#E73: (-1 0 -1 0 0 0 0 0)+(-1 0 -1 0 0 0 0 0)t1
#E74: (1 0 -1 0 2 0 -1 0)+(1 0 -1 0 2 0 -1 0)t1
#E75: (-1 0 2 0 -1 0 1 0)+(-1 0 2 0 -1 0 1 0)t1
#E76: (-1 0 1 0 -1 0 2 0)+(-1 0 1 0 -1 0 2 0)t1
#E77: (0 0 0 0 1 0 1 0)+(0 0 0 0 1 0 1 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 1
## V3(18) (6 PI) 
# edges: 9
## E64(2) E66(2) E69(2) E72(2) E73(2) E74(2) E75(2) E76(2) E77(2) 
# faces: 6
# genus: 2
# area: 24.9824
sweeping start V3 depth = 49.9824
File =  depth = 49.9824 follow_depth = 249.912 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 54 unusual 0 close 164326 reject 0
V3: AbBaCc   cylinders |1|1.61803|
Moduli: |1|1| Min Area: 1 Max Area: 1
    total: 1.50098 0.912488 sym = 6
    raw = 1720 short saddle = 2.69449 short cyl = 4.35978 tag #0

total cylinders: 3.00022 1.82392 raw = 573 rejected = 0
""",
    (2, 4, 2, 4): r"""
max loc = 1
alpha =  4 beta = 2 gamma = 4 delta =2
a =  2.0944 b = 1.0472 c= 2.0944 d=1.0472
a1_min = 8.88178e-16 a1_max = 2.0944
a1 = 1.42773
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-0.800317,1.38619)
Imaginary of t_cx = -7.77156e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (1.02696e-15,-8.88178e-16)
Checking r_alt (0,-4.44089e-16)
Checking p_algt (0,0)
Checking q_algt (1.02696e-15,-8.88178e-16)
Checking r_algt (0,-4.44089e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.6006331954839124
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741955848555051,1.386189009429731333611358)
E2 ( V2 V3 F0 NULL ) (-1.000000000000000000000000,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (0.800316597741955848555051,-1.386189009429731333611358)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 0 0 0)+(-1 0 1 0)t1
#E2: (-1 0 0 0)+(0 0 0 0)t1
#E3: (0 0 0 0)+(1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.391038, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F13 V16(E5) V24(E19) a=0, can_bisect=0
F13 V16(E5) V16(E29) a=0.90413, can_bisect=1
F13 V16(E5) V24(E25) a=0.666667, can_bisect=1
F13 V16(E5) V16(E11) a=0.90413, can_bisect=1
F13 V16(E5) V24(E4) a=0, can_bisect=0
F13 V24(E19) V16(E29) a=0, can_bisect=0
F13 V24(E19) V24(E25) a=0.143068, can_bisect=1
F13 V24(E19) V16(E11) a=0.666667, can_bisect=1
F13 V24(E19) V24(E4) a=0.143068, can_bisect=1
F13 V16(E29) V24(E25) a=0, can_bisect=0
F13 V16(E29) V16(E11) a=0.90413, can_bisect=1
F13 V16(E29) V24(E4) a=0.666667, can_bisect=1
F13 V24(E25) V16(E11) a=0, can_bisect=0
F13 V24(E25) V24(E4) a=0.143068, can_bisect=1
F13 V16(E11) V24(E4) a=0, can_bisect=0
F13 best_angle = 0.90413
F15 V16(E11) V24(E4) a=0, can_bisect=0
F15 V16(E11) V16(E5) a=0.90413, can_bisect=1
F15 V16(E11) V24(E19) a=0.523599, can_bisect=1
F15 V16(E11) V16(E35) a=0, can_bisect=0
F15 V24(E4) V16(E5) a=0, can_bisect=0
F15 V24(E4) V24(E19) a=0.143068, can_bisect=1
F15 V24(E4) V16(E35) a=0.523599, can_bisect=1
F15 V16(E5) V24(E19) a=0, can_bisect=0
F15 V16(E5) V16(E35) a=0.90413, can_bisect=1
F15 V24(E19) V16(E35) a=0, can_bisect=0
F15 best_angle = 0.90413
F17 V16(E35) V16(E11) a=0, can_bisect=0
F17 V16(E35) V24(E4) a=0.523599, can_bisect=1
F17 V16(E35) V16(E36) a=0, can_bisect=0
F17 V16(E11) V24(E4) a=0, can_bisect=0
F17 V16(E11) V16(E36) a=0.90413, can_bisect=1
F17 V24(E4) V16(E36) a=0, can_bisect=0
F17 best_angle = 0.90413
Removing V3
F20 V24(E5) V16(E19) a=0, can_bisect=0
F20 V24(E5) V24(E29) a=0.90413, can_bisect=1
F20 V24(E5) V16(E25) a=0.666667, can_bisect=1
F20 V24(E5) V24(E11) a=0.90413, can_bisect=1
F20 V24(E5) V16(E4) a=0, can_bisect=0
F20 V16(E19) V24(E29) a=0, can_bisect=0
F20 V16(E19) V16(E25) a=0.143068, can_bisect=1
F20 V16(E19) V24(E11) a=0.666667, can_bisect=1
F20 V16(E19) V16(E4) a=0.143068, can_bisect=1
F20 V24(E29) V16(E25) a=0, can_bisect=0
F20 V24(E29) V24(E11) a=0.90413, can_bisect=1
F20 V24(E29) V16(E4) a=0.666667, can_bisect=1
F20 V16(E25) V24(E11) a=0, can_bisect=0
F20 V16(E25) V16(E4) a=0.143068, can_bisect=1
F20 V24(E11) V16(E4) a=0, can_bisect=0
F20 best_angle = 0.90413
F22 V24(E11) V16(E4) a=0, can_bisect=0
F22 V24(E11) V24(E5) a=0.90413, can_bisect=1
F22 V24(E11) V16(E19) a=0.523599, can_bisect=1
F22 V24(E11) V24(E38) a=0, can_bisect=0
F22 V16(E4) V24(E5) a=0, can_bisect=0
F22 V16(E4) V16(E19) a=0.143068, can_bisect=1
F22 V16(E4) V24(E38) a=0.523599, can_bisect=1
F22 V24(E5) V16(E19) a=0, can_bisect=0
F22 V24(E5) V24(E38) a=0.90413, can_bisect=1
F22 V16(E19) V24(E38) a=0, can_bisect=0
F22 best_angle = 0.90413
F24 V24(E38) V24(E11) a=0, can_bisect=0
F24 V24(E38) V16(E4) a=0.523599, can_bisect=1
F24 V24(E38) V24(E39) a=0, can_bisect=0
F24 V24(E11) V16(E4) a=0, can_bisect=0
F24 V24(E11) V24(E39) a=0.90413, can_bisect=1
F24 V16(E4) V24(E39) a=0, can_bisect=0
F24 best_angle = 0.90413
Longest Edge: 1.73205
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=1.6006331954839124
# Vertices: V16 V24
# UEdges: 
E4 ( V16 V24 F25 F18 ) (0.199683402258044151444949,1.386189009429731333611358)
E5 ( V16 V24 F16 F23 ) (0.199683402258043485311134,-1.386189009429731333611358)
E11 ( V16 V24 F18 F25 ) (-1.300316597741955959577353,0.520163605645292737023055)
E19 ( V24 V16 F16 F23 ) (1.300316597741956403666563,0.520163605645292737023055)
E25 ( V24 V16 F14 F21 ) (-1.100633195483912141199312,0.866025403784438596588302)
E29 ( V16 V24 F14 F21 ) (1.100633195483912585288522,0.866025403784438596588302)
E35 ( V16 V16 F19 F14 ) (0.000000000000000444089210,1.732050807568877193176604)
E36 ( V16 V16 F19 F16 ) (1.500000000000000000000000,-0.866025403784438596588302)
E37 ( V16 V16 F19 F18 ) (-1.500000000000000000000000,-0.866025403784438596588302)
E38 ( V24 V24 F26 F21 ) (-0.000000000000000444089210,-1.732050807568877193176604)
E39 ( V24 V24 F26 F23 ) (-1.500000000000000000000000,0.866025403784438596588302)
E40 ( V24 V24 F26 F25 ) (1.500000000000000000000000,0.866025403784438596588302)
# Faces: 
F14 +E29 +E25 -E35
F16 +E5 +E19 -E36
F18 +E11 -E4 -E37
F19 +E36 +E35 +E37
F21 -E29 -E25 -E38
F23 -E5 -E19 -E39
F25 -E11 +E4 -E40
F26 +E39 +E38 +E40
#UEdges Algebraic 
#E4: (1 0 0 0)+(-1 0 1 0)t1
#E5: (1 0 0 0)+(0 0 -1 0)t1
#E11: (0 0 -1 0)+(-1 0 1 0)t1
#E19: (1 0 -1 0)+(0 0 1 0)t1
#E25: (0 0 1 0)+(-1 0 0 0)t1
#E29: (-1 0 1 0)+(1 0 0 0)t1
#E35: (-1 0 2 0)+(0 0 0 0)t1
#E36: (2 0 -1 0)+(0 0 0 0)t1
#E37: (-1 0 -1 0)+(0 0 0 0)t1
#E38: (1 0 -2 0)+(0 0 0 0)t1
#E39: (-2 0 1 0)+(0 0 0 0)t1
#E40: (1 0 1 0)+(0 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V16(12) (4 PI) V24(12) (4 PI) 
# edges: 12
## E4(2) E5(2) E11(2) E19(2) E25(2) E29(2) E35(2) E36(2) E37(2) E38(2) E39(2) E40(2) 
# faces: 8
# genus: 2
# area: 8.31713
sweeping start V16 depth = 28.8394
File =  depth = 28.8394 follow_depth = 144.197 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 6 unusual 0 close 32676 reject 0
V16: AaXX V24: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0120598 Max Area: 0.755899
    total: 2.1677 1.3178 sym = 2
    raw = 828 short saddle = 3.93548 short cyl = 3.93548 tag #1
V16: AXBc V24: aCbX   cylinders |1|
Moduli: |1| Min Area: 0.285685 Max Area: 0.44069
    total: 0.0837758 0.0509296 sym = 3
    raw = 48 short saddle = 4.19108 short cyl = 12.5733 tag #3
V16: AaBb V24: CdDc   cylinders |1|1.58063|2.58063|
Moduli: |1.50719|1|1.19654| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 28.4178 short cyl = 28.4178 tag #4
V16: AaXc V24: BXCb   cylinders |1|
Moduli: |1| Min Area: 0.13068 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #5
V16: AbBa V24: CcDd   cylinders |1|1.60063|2.60063|
Moduli: |8.82252|1|4.00793| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 1.73205 short cyl = 1.73205 tag #6
V16: AXBb V24: aXcC   cylinders |1|
Moduli: |1| Min Area: 0.13068 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #7

total cylinders: 2.38761 1.45149 raw = 456 rejected = 0
""",
    (2, 4, 3, 3): r"""
max loc = 1
alpha =  4 beta = 3 gamma = 3 delta =2
a =  2.0944 b = 1.5708 c= 1.5708 d=1.0472
a1_min = 4.44089e-16 a1_max = 1.5708
a1 = 1.0708
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 0
p = 1 q = 2.08583 p-q = 1.83049
p = (1,0) q = (1,1.83049) r = (-1.05683,1.83049)
Imaginary of t_cx = -9.99201e-16
p_algt = (1 0 0 0)+(0 0 0 0)t1
r_algt = (0 0 0 0)+(-1 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (6.66134e-16,-8.88178e-16)
Checking r_alt (0,-4.44089e-16)
Checking p_algt (0,0)
Checking q_algt (6.66134e-16,-8.88178e-16)
Checking r_algt (0,-4.44089e-16)
Number field: min poly (1 0 -1 0 1)
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.1136651577579775
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000444089210,1.830487721712451554267886)
E2 ( V2 V3 F0 NULL ) (-2.056832578878988737614009,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (1.056832578878988293524799,-1.830487721712451554267886)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0)+(0 0 0 0)t1
#E1: (0 0 0 0)+(-1/2 0 1 0)t1
#E2: (-1 0 0 0)+(-1/2 0 0 0)t1
#E3: (0 0 0 0)+(1 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.5, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.31996, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.5
In RetriangulateSurface
Removing V1
F25 V42(E5) V2(E25) a=0, can_bisect=0
F25 V42(E5) V46(E11) a=1.0708, can_bisect=1
F25 V42(E5) V2(E4) a=0, can_bisect=0
F25 V2(E25) V46(E11) a=0, can_bisect=0
F25 V2(E25) V2(E4) a=0.5, can_bisect=1
F25 V46(E11) V2(E4) a=0, can_bisect=0
F25 best_angle = 1.0708
Removing V2
F28 V46(E15) V50(E27) a=0, can_bisect=0
F28 V46(E15) V46(E71) a=0.523599, can_bisect=1
F28 V46(E15) V42(E9) a=1.0236, can_bisect=1
F28 V46(E15) V3(E3) a=0.507647, can_bisect=1
F28 V46(E15) V42(E71) a=0, can_bisect=0
F28 V50(E27) V46(E71) a=0, can_bisect=0
F28 V50(E27) V42(E9) a=0.507647, can_bisect=1
F28 V50(E27) V3(E3) a=1.0472, can_bisect=1
F28 V50(E27) V42(E71) a=0.507647, can_bisect=1
F28 V46(E71) V42(E9) a=0, can_bisect=0
F28 V46(E71) V3(E3) a=0.507647, can_bisect=1
F28 V46(E71) V42(E71) a=1.0236, can_bisect=1
F28 V42(E9) V3(E3) a=0, can_bisect=0
F28 V42(E9) V42(E71) a=0.523599, can_bisect=1
F28 V3(E3) V42(E71) a=0, can_bisect=0
F28 best_angle = 1.0472
F30 V3(E3) V42(E71) a=0, can_bisect=0
F30 V3(E3) V46(E15) a=0.507647, can_bisect=1
F30 V3(E3) V50(E72) a=0, can_bisect=0
F30 V42(E71) V46(E15) a=0, can_bisect=0
F30 V42(E71) V50(E72) a=0.507647, can_bisect=1
F30 V46(E15) V50(E72) a=0, can_bisect=0
F30 best_angle = 0.507647
F29 V50(E27) V46(E71) a=0, can_bisect=0
F29 V50(E27) V42(E9) a=0.507647, can_bisect=1
F29 V50(E27) V3(E72) a=0, can_bisect=0
F29 V46(E71) V42(E9) a=0, can_bisect=0
F29 V46(E71) V3(E72) a=0.507647, can_bisect=1
F29 V42(E9) V3(E72) a=0, can_bisect=0
F29 best_angle = 0.507647
Removing V3
F35 V50(E74) V42(E31) a=0, can_bisect=0
F35 V50(E74) V34(E59) a=0.206077, can_bisect=1
F35 V50(E74) V42(E43) a=0.53955, can_bisect=1
F35 V50(E74) V16(E17) a=0.357072, can_bisect=1
F35 V50(E74) V42(E71) a=0.507647, can_bisect=1
F35 V50(E74) V46(E15) a=0, can_bisect=0
F35 V42(E31) V34(E59) a=0, can_bisect=0
F35 V42(E31) V42(E43) a=0.5, can_bisect=1
F35 V42(E31) V16(E17) a=1.0236, can_bisect=1
F35 V42(E31) V42(E71) a=1.03125, can_bisect=1
F35 V42(E31) V46(E15) a=0.531246, can_bisect=1
F35 V34(E59) V42(E43) a=0, can_bisect=0
F35 V34(E59) V16(E17) a=0.547198, can_bisect=1
F35 V34(E59) V42(E71) a=1.0236, can_bisect=1
F35 V34(E59) V46(E15) a=0.723959, can_bisect=1
F35 V42(E43) V16(E17) a=0, can_bisect=0
F35 V42(E43) V42(E71) a=0.5, can_bisect=1
F35 V42(E43) V46(E15) a=0.340256, can_bisect=1
F35 V16(E17) V42(E71) a=0, can_bisect=0
F35 V16(E17) V46(E15) a=0.0115515, can_bisect=1
F35 V42(E71) V46(E15) a=0, can_bisect=0
F35 best_angle = 1.03125
F37 V42(E71) V46(E15) a=0, can_bisect=0
F37 V42(E71) V50(E74) a=0.507647, can_bisect=1
F37 V42(E71) V42(E75) a=0, can_bisect=0
F37 V46(E15) V50(E74) a=0, can_bisect=0
F37 V46(E15) V42(E75) a=0.5, can_bisect=1
F37 V50(E74) V42(E75) a=0, can_bisect=0
F37 best_angle = 0.507647
F36 V42(E31) V34(E59) a=0, can_bisect=0
F36 V42(E31) V42(E43) a=0.5, can_bisect=1
F36 V42(E31) V16(E17) a=0.523599, can_bisect=1
F36 V42(E31) V42(E75) a=0, can_bisect=0
F36 V34(E59) V42(E43) a=0, can_bisect=0
F36 V34(E59) V16(E17) a=0.547198, can_bisect=1
F36 V34(E59) V42(E75) a=0.523599, can_bisect=1
F36 V42(E43) V16(E17) a=0, can_bisect=0
F36 V42(E43) V42(E75) a=0.5, can_bisect=1
F36 V16(E17) V42(E75) a=0, can_bisect=0
F36 best_angle = 0.547198
F41 V16(E17) V42(E75) a=0, can_bisect=0
F41 V16(E17) V42(E31) a=0.523599, can_bisect=1
F41 V16(E17) V34(E77) a=0, can_bisect=0
F41 V42(E75) V42(E31) a=0, can_bisect=0
F41 V42(E75) V34(E77) a=0.523599, can_bisect=1
F41 V42(E31) V34(E77) a=0, can_bisect=0
F41 best_angle = 0.523599
Removing V16
F44 V34(E59) V42(E22) a=0, can_bisect=0
F44 V34(E59) V21(E47) a=0.0076473, can_bisect=1
F44 V34(E59) V46(E27) a=0.0115515, can_bisect=1
F44 V34(E59) V50(E51) a=0.357072, can_bisect=1
F44 V34(E59) V46(E47) a=0.723959, can_bisect=1
F44 V34(E59) V21(E22) a=0.863855, can_bisect=1
F44 V34(E59) V42(E75) a=0.523599, can_bisect=1
F44 V34(E59) V42(E31) a=0, can_bisect=0
F44 V42(E22) V21(E47) a=0, can_bisect=0
F44 V42(E22) V46(E27) a=0, can_bisect=1
F44 V42(E22) V50(E51) a=0.507647, can_bisect=1
F44 V42(E22) V46(E47) a=1.0236, can_bisect=1
F44 V42(E22) V21(E22) a=1.30415, can_bisect=1
F44 V42(E22) V42(E75) a=1.0472, can_bisect=1
F44 V42(E22) V42(E31) a=0.5, can_bisect=1
F44 V21(E47) V46(E27) a=0, can_bisect=0
F44 V21(E47) V50(E51) a=0.31996, can_bisect=1
F44 V21(E47) V46(E47) a=0.790245, can_bisect=1
F44 V21(E47) V21(E22) a=1.5708, can_bisect=1
F44 V21(E47) V42(E75) a=1.30415, can_bisect=1
F44 V21(E47) V42(E31) a=0.413596, can_bisect=1
F44 V46(E27) V50(E51) a=0, can_bisect=0
F44 V46(E27) V46(E47) a=0.523599, can_bisect=1
F44 V46(E27) V21(E22) a=0.790245, can_bisect=1
F44 V46(E27) V42(E75) a=1.0236, can_bisect=1
F44 V46(E27) V42(E31) a=0.340256, can_bisect=1
F44 V50(E51) V46(E47) a=0, can_bisect=0
F44 V50(E51) V21(E22) a=0.31996, can_bisect=1
F44 V50(E51) V42(E75) a=0.507647, can_bisect=1
F44 V50(E51) V42(E31) a=0.523599, can_bisect=1
F44 V46(E47) V21(E22) a=0, can_bisect=0
F44 V46(E47) V42(E75) a=0, can_bisect=0
F44 V46(E47) V42(E31) a=0.183342, can_bisect=1
F44 V21(E22) V42(E75) a=0, can_bisect=0
F44 V21(E22) V42(E31) a=0.110003, can_bisect=1
F44 V42(E75) V42(E31) a=0, can_bisect=0
F44 best_angle = 1.5708
F46 V21(E22) V42(E75) a=0, can_bisect=0
F46 V21(E22) V42(E31) a=0.110003, can_bisect=1
F46 V21(E22) V34(E59) a=0.706941, can_bisect=1
F46 V21(E22) V42(E22) a=0.266647, can_bisect=1
F46 V21(E22) V21(E79) a=0, can_bisect=0
F46 V42(E75) V42(E31) a=0, can_bisect=0
F46 V42(E75) V34(E59) a=0.523599, can_bisect=1
F46 V42(E75) V42(E22) a=1.0472, can_bisect=1
F46 V42(E75) V21(E79) a=0.266647, can_bisect=1
F46 V42(E31) V34(E59) a=0, can_bisect=0
F46 V42(E31) V42(E22) a=0.5, can_bisect=1
F46 V42(E31) V21(E79) a=0.413596, can_bisect=1
F46 V34(E59) V42(E22) a=0, can_bisect=0
F46 V34(E59) V21(E79) a=0.0076473, can_bisect=1
F46 V42(E22) V21(E79) a=0, can_bisect=0
F46 best_angle = 1.0472
F48 V42(E22) V21(E79) a=0, can_bisect=0
F48 V42(E22) V21(E22) a=0.266647, can_bisect=1
F48 V42(E22) V42(E80) a=0, can_bisect=0
F48 V21(E79) V21(E22) a=0, can_bisect=0
F48 V21(E79) V42(E80) a=0.266647, can_bisect=1
F48 V21(E22) V42(E80) a=0, can_bisect=0
F48 best_angle = 0.266647
F47 V42(E75) V42(E31) a=0, can_bisect=0
F47 V42(E75) V34(E59) a=0.523599, can_bisect=1
F47 V42(E75) V42(E80) a=0, can_bisect=0
F47 V42(E31) V34(E59) a=0, can_bisect=0
F47 V42(E31) V42(E80) a=0.5, can_bisect=1
F47 V34(E59) V42(E80) a=0, can_bisect=0
F47 best_angle = 0.523599
F45 V21(E47) V46(E27) a=0, can_bisect=0
F45 V21(E47) V50(E51) a=0.31996, can_bisect=1
F45 V21(E47) V46(E47) a=0.266647, can_bisect=1
F45 V21(E47) V21(E79) a=0, can_bisect=0
F45 V46(E27) V50(E51) a=0, can_bisect=0
F45 V46(E27) V46(E47) a=0.523599, can_bisect=1
F45 V46(E27) V21(E79) a=0.266647, can_bisect=1
F45 V50(E51) V46(E47) a=0, can_bisect=0
F45 V50(E51) V21(E79) a=0.31996, can_bisect=1
F45 V46(E47) V21(E79) a=0, can_bisect=0
F45 best_angle = 0.523599
F54 V46(E47) V21(E79) a=0, can_bisect=0
F54 V46(E47) V21(E47) a=0.266647, can_bisect=1
F54 V46(E47) V46(E83) a=0, can_bisect=0
F54 V21(E79) V21(E47) a=0, can_bisect=0
F54 V21(E79) V46(E83) a=0.266647, can_bisect=1
F54 V21(E47) V46(E83) a=0, can_bisect=0
F54 best_angle = 0.266647
Flipping E79
Removing V21
F59 V46(E83) V46(E85) a=0, can_bisect=0
F59 V46(E83) V42(E80) a=1.0708, can_bisect=1
F59 V46(E83) V42(E85) a=0, can_bisect=0
F59 V46(E85) V42(E80) a=0, can_bisect=0
F59 V46(E85) V42(E85) a=0.233353, can_bisect=1
F59 V42(E80) V42(E85) a=0, can_bisect=0
F59 best_angle = 1.0708
Removing V32
F62 V42(E59) V34(E37) a=0, can_bisect=0
F62 V42(E59) V46(E65) a=1.0708, can_bisect=1
F62 V42(E59) V34(E31) a=0, can_bisect=0
F62 V34(E37) V46(E65) a=0, can_bisect=0
F62 V34(E37) V34(E31) a=0.5, can_bisect=1
F62 V46(E65) V34(E31) a=0, can_bisect=0
F62 best_angle = 1.0708
Removing V34
F65 V46(E51) V50(E15) a=0, can_bisect=0
F65 V46(E51) V46(E87) a=0.523599, can_bisect=1
F65 V46(E51) V42(E80) a=1.0236, can_bisect=1
F65 V46(E51) V42(E75) a=0.183342, can_bisect=1
F65 V46(E51) V42(E87) a=0, can_bisect=0
F65 V50(E15) V46(E87) a=0, can_bisect=0
F65 V50(E15) V42(E80) a=0.507647, can_bisect=1
F65 V50(E15) V42(E75) a=0.523599, can_bisect=1
F65 V50(E15) V42(E87) a=0.507647, can_bisect=1
F65 V46(E87) V42(E80) a=0, can_bisect=0
F65 V46(E87) V42(E75) a=0.183342, can_bisect=1
F65 V46(E87) V42(E87) a=1.0236, can_bisect=1
F65 V42(E80) V42(E75) a=0, can_bisect=0
F65 V42(E80) V42(E87) a=1.0472, can_bisect=1
F65 V42(E75) V42(E87) a=0, can_bisect=0
F65 best_angle = 1.0472
F67 V42(E87) V46(E51) a=0, can_bisect=0
F67 V42(E87) V50(E15) a=0.507647, can_bisect=1
F67 V42(E87) V46(E87) a=0.5, can_bisect=1
F67 V42(E87) V42(E88) a=0, can_bisect=0
F67 V46(E51) V50(E15) a=0, can_bisect=0
F67 V46(E51) V46(E87) a=0.523599, can_bisect=1
F67 V46(E51) V42(E88) a=0.5, can_bisect=1
F67 V50(E15) V46(E87) a=0, can_bisect=0
F67 V50(E15) V42(E88) a=0.507647, can_bisect=1
F67 V46(E87) V42(E88) a=0, can_bisect=0
F67 best_angle = 0.523599
F69 V46(E87) V42(E88) a=0, can_bisect=0
F69 V46(E87) V42(E87) a=0.5, can_bisect=1
F69 V46(E87) V46(E89) a=0, can_bisect=0
F69 V42(E88) V42(E87) a=0, can_bisect=0
F69 V42(E88) V46(E89) a=0.5, can_bisect=1
F69 V42(E87) V46(E89) a=0, can_bisect=0
F69 best_angle = 0.5
Removing V50
F72 V46(E83) V46(E71) a=0, can_bisect=0
F72 V46(E83) V42(E75) a=0.183342, can_bisect=1
F72 V46(E83) V42(E71) a=0.183342, can_bisect=1
F72 V46(E83) V46(E89) a=0, can_bisect=0
F72 V46(E71) V42(E75) a=0, can_bisect=0
F72 V46(E71) V42(E71) a=0.5, can_bisect=1
F72 V46(E71) V46(E89) a=1.0472, can_bisect=1
F72 V42(E75) V42(E71) a=0, can_bisect=0
F72 V42(E75) V46(E89) a=0.5, can_bisect=1
F72 V42(E71) V46(E89) a=0, can_bisect=0
F72 best_angle = 1.0472
F73 V46(E71) V42(E75) a=0, can_bisect=0
F73 V46(E71) V42(E71) a=0.5, can_bisect=1
F73 V46(E71) V46(E91) a=0, can_bisect=0
F73 V42(E75) V42(E71) a=0, can_bisect=0
F73 V42(E75) V46(E91) a=0.5, can_bisect=1
F73 V42(E71) V46(E91) a=0, can_bisect=0
F73 best_angle = 0.5
Longest Edge: 4.17166
Looking for shortest saddle...2
Clearing denominators: multiplying by 1
#Field Degree=4 minimal polynomial=(1 0 -1 0 1)
#Field generator embedding: (0.866025403784438710.49999999999999994
#Params:1
#t1=2.1136651577579775
# Vertices: V42 V46
# UEdges: 
E71 ( V42 V46 F76 F75 ) (2.000000000000000000000000,0.000000000000000000000000)
E75 ( V42 V42 F75 F66 ) (-0.000000000000000888178420,-3.660975443424903552624983)
E80 ( V42 V42 F61 F66 ) (-3.170497736636965768752816,1.830487721712451776312491)
E83 ( V46 V46 F60 F74 ) (3.170497736636965768752816,-1.830487721712451554267886)
E85 ( V42 V46 F61 F60 ) (2.170497736636965324663606,-3.562538529281328969489095)
E86 ( V46 V42 F61 F60 ) (1.000000000000000444089210,1.732050807568877415221209)
E87 ( V42 V46 F71 F70 ) (-0.999999999999999333866185,1.732050807568877193176604)
E88 ( V42 V42 F70 F66 ) (3.170497736636966656931236,1.830487721712451776312491)
E89 ( V46 V46 F71 F74 ) (-3.170497736636966656931236,-1.830487721712451554267886)
E90 ( V46 V42 F71 F70 ) (4.170497736636965768752816,0.098436914143574583135887)
E91 ( V46 V46 F74 F76 ) (-0.000000000000000888178420,-3.660975443424903552624983)
E92 ( V46 V42 F76 F75 ) (-2.000000000000000888178420,-3.660975443424903552624983)
# Faces: 
F60 +E83 -E85 -E86
F61 +E80 +E85 +E86
F66 -E80 -E75 -E88
F70 -E87 +E88 -E90
F71 +E87 +E89 +E90
F74 -E89 -E83 +E91
F75 -E71 +E75 -E92
F76 +E71 -E91 +E92
#UEdges Algebraic 
#E71: (2 0 0 0)+(0 0 0 0)t1
#E75: (0 0 0 0)+(1 0 -2 0)t1
#E80: (0 0 0 0)+(-2 0 1 0)t1
#E83: (0 0 0 0)+(2 0 -1 0)t1
#E85: (0 0 -2 0)+(2 0 -1 0)t1
#E86: (0 0 2 0)+(0 0 0 0)t1
#E87: (-2 0 2 0)+(0 0 0 0)t1
#E88: (0 0 0 0)+(1 0 1 0)t1
#E89: (0 0 0 0)+(-1 0 -1 0)t1
#E90: (2 0 -2 0)+(1 0 1 0)t1
#E91: (0 0 0 0)+(1 0 -2 0)t1
#E92: (-2 0 0 0)+(1 0 -2 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V42(12) (4 PI) V46(12) (4 PI) 
# edges: 12
## E71(2) E75(2) E80(2) E83(2) E85(2) E86(2) E87(2) E88(2) E89(2) E90(2) E91(2) E92(2) 
# faces: 8
# genus: 2
# area: 33.573
sweeping start V42 depth = 57.9422
File =  depth = 57.9422 follow_depth = 289.711 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 24 unusual 0 close 31205 reject 0
V42: AXXa V46: BbXX   cylinders |1|
Moduli: |1| Min Area: 0.0914532 Max Area: 0.73834
    total: 2.45044 1.48969 sym = 2
    raw = 936 short saddle = 6.17128 short cyl = 6.17128 tag #3
V42: AbBX V46: aCcX   cylinders |1|
Moduli: |1| Min Area: 0.255289 Max Area: 0.255289
    total: 0.0261799 0.0159155 sym = 3
    raw = 15 short saddle = 14.9424 short cyl = 44.8272 tag #4
V42: AXBb V46: aXcC   cylinders |1|
Moduli: |1| Min Area: 0.019113 Max Area: 0.255289
    total: 0.0261799 0.0159155 sym = 3
    raw = 15 short saddle = 14.9424 short cyl = 44.8272 tag #5
V42: AbBa V46: CcDd   cylinders |1|1.94622|2.94622|
Moduli: |5.89245|3.28573|1| Min Area: 1 Max Area: 1
    total: 0.017017 0.0103451 sym = 4
    raw = 13 short saddle = 3.66098 short cyl = 3.66098 tag #0
V42: AdBc V46: aCbD   cylinders |1|1.51382|
Moduli: |1|11.6233| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 2 short cyl = 12.341 tag #7

total cylinders: 2.59181 1.57563 raw = 495 rejected = 0
""",
    (2, 7, 3, 6): r"""
max loc = 1
alpha =  7 beta = 3 gamma = 6 delta =2
a =  2.44346 b = 1.0472 c= 2.0944 d=0.698132
a1_min = 4.44089e-16 a1_max = 2.0944
a1 = 1.42773
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -1.11022e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-1.652,1.38619)
Imaginary of t_cx = -6.66134e-16
p_algt = (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0)+(0 -1 0 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (8.04912e-16,-2.22045e-16)
Checking r_alt (-4.44089e-16,2.22045e-16)
Checking p_algt (0,0)
Checking q_algt (8.04912e-16,-2.22045e-16)
Checking r_algt (-4.44089e-16,2.22045e-16)
Number field: min poly (1 0 0 -1 0 0 1)
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=2.1565272705018663
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741956070599656,1.386189009429731999745172)
E2 ( V2 V3 F0 NULL ) (-1.851679134260535875000642,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (1.651995732002491834577995,-1.386189009429731999745172)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
#E1: (0 0 0 0 0 0)+(0 -2/3 1/3 0 1/3 1/3)t1
#E2: (-1 0 0 0 0 0)+(0 -1/3 -1/3 0 2/3 -1/3)t1
#E3: (0 0 0 0 0 0)+(0 1 0 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.216492, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F37 V52(E5) V36(E25) a=0, can_bisect=0
F37 V52(E5) V52(E59) a=0.90413, can_bisect=1
F37 V52(E5) V36(E37) a=0.666667, can_bisect=1
F37 V52(E5) V52(E11) a=0.90413, can_bisect=1
F37 V52(E5) V36(E4) a=0, can_bisect=0
F37 V36(E25) V52(E59) a=0, can_bisect=0
F37 V36(E25) V36(E37) a=0.143068, can_bisect=1
F37 V36(E25) V52(E11) a=0.666667, can_bisect=1
F37 V36(E25) V36(E4) a=0.143068, can_bisect=1
F37 V52(E59) V36(E37) a=0, can_bisect=0
F37 V52(E59) V52(E11) a=0.90413, can_bisect=1
F37 V52(E59) V36(E4) a=0.666667, can_bisect=1
F37 V36(E37) V52(E11) a=0, can_bisect=0
F37 V36(E37) V36(E4) a=0.143068, can_bisect=1
F37 V52(E11) V36(E4) a=0, can_bisect=0
F37 best_angle = 0.90413
F39 V52(E11) V36(E4) a=0, can_bisect=0
F39 V52(E11) V52(E5) a=0.90413, can_bisect=1
F39 V52(E11) V36(E25) a=0.523599, can_bisect=1
F39 V52(E11) V52(E107) a=0, can_bisect=0
F39 V36(E4) V52(E5) a=0, can_bisect=0
F39 V36(E4) V36(E25) a=0.143068, can_bisect=1
F39 V36(E4) V52(E107) a=0.523599, can_bisect=1
F39 V52(E5) V36(E25) a=0, can_bisect=0
F39 V52(E5) V52(E107) a=0.90413, can_bisect=1
F39 V36(E25) V52(E107) a=0, can_bisect=0
F39 best_angle = 0.90413
F41 V52(E107) V52(E11) a=0, can_bisect=0
F41 V52(E107) V36(E4) a=0.523599, can_bisect=1
F41 V52(E107) V52(E108) a=0, can_bisect=0
F41 V52(E11) V36(E4) a=0, can_bisect=0
F41 V52(E11) V52(E108) a=0.90413, can_bisect=1
F41 V36(E4) V52(E108) a=0, can_bisect=0
F41 best_angle = 0.90413
Removing V21
F44 V16(E55) V52(E65) a=0, can_bisect=0
F44 V16(E55) V16(E103) a=0.143068, can_bisect=1
F44 V16(E55) V52(E83) a=0.666667, can_bisect=1
F44 V16(E55) V16(E49) a=0.143068, can_bisect=1
F44 V16(E55) V52(E17) a=0, can_bisect=0
F44 V52(E65) V16(E103) a=0, can_bisect=0
F44 V52(E65) V52(E83) a=0.90413, can_bisect=1
F44 V52(E65) V16(E49) a=0.666667, can_bisect=1
F44 V52(E65) V52(E17) a=0.90413, can_bisect=1
F44 V16(E103) V52(E83) a=0, can_bisect=0
F44 V16(E103) V16(E49) a=0.143068, can_bisect=1
F44 V16(E103) V52(E17) a=0.666667, can_bisect=1
F44 V52(E83) V16(E49) a=0, can_bisect=0
F44 V52(E83) V52(E17) a=0.90413, can_bisect=1
F44 V16(E49) V52(E17) a=0, can_bisect=0
F44 best_angle = 0.90413
F46 V52(E83) V16(E49) a=0, can_bisect=0
F46 V52(E83) V52(E17) a=0.90413, can_bisect=1
F46 V52(E83) V16(E55) a=0.523599, can_bisect=1
F46 V52(E83) V52(E110) a=0, can_bisect=0
F46 V16(E49) V52(E17) a=0, can_bisect=0
F46 V16(E49) V16(E55) a=0.143068, can_bisect=1
F46 V16(E49) V52(E110) a=0.523599, can_bisect=1
F46 V52(E17) V16(E55) a=0, can_bisect=0
F46 V52(E17) V52(E110) a=0.90413, can_bisect=1
F46 V16(E55) V52(E110) a=0, can_bisect=0
F46 best_angle = 0.90413
F48 V52(E110) V52(E83) a=0, can_bisect=0
F48 V52(E110) V16(E49) a=0.523599, can_bisect=1
F48 V52(E110) V52(E111) a=0, can_bisect=0
F48 V52(E83) V16(E49) a=0, can_bisect=0
F48 V52(E83) V52(E111) a=0.90413, can_bisect=1
F48 V16(E49) V52(E111) a=0, can_bisect=0
F48 best_angle = 0.90413
Removing V44
F51 V52(E77) V40(E31) a=0, can_bisect=0
F51 V52(E77) V52(E71) a=0.90413, can_bisect=1
F51 V52(E77) V40(E97) a=0.666667, can_bisect=1
F51 V52(E77) V52(E89) a=0.90413, can_bisect=1
F51 V52(E77) V40(E43) a=0, can_bisect=0
F51 V40(E31) V52(E71) a=0, can_bisect=0
F51 V40(E31) V40(E97) a=0.143068, can_bisect=1
F51 V40(E31) V52(E89) a=0.666667, can_bisect=1
F51 V40(E31) V40(E43) a=0.143068, can_bisect=1
F51 V52(E71) V40(E97) a=0, can_bisect=0
F51 V52(E71) V52(E89) a=0.90413, can_bisect=1
F51 V52(E71) V40(E43) a=0.666667, can_bisect=1
F51 V40(E97) V52(E89) a=0, can_bisect=0
F51 V40(E97) V40(E43) a=0.143068, can_bisect=1
F51 V52(E89) V40(E43) a=0, can_bisect=0
F51 best_angle = 0.90413
F52 V52(E77) V40(E31) a=0, can_bisect=0
F52 V52(E77) V52(E71) a=0.90413, can_bisect=1
F52 V52(E77) V40(E97) a=0.523599, can_bisect=1
F52 V52(E77) V52(E113) a=0, can_bisect=0
F52 V40(E31) V52(E71) a=0, can_bisect=0
F52 V40(E31) V40(E97) a=0.143068, can_bisect=1
F52 V40(E31) V52(E113) a=0.523599, can_bisect=1
F52 V52(E71) V40(E97) a=0, can_bisect=0
F52 V52(E71) V52(E113) a=0.90413, can_bisect=1
F52 V40(E97) V52(E113) a=0, can_bisect=0
F52 best_angle = 0.90413
F55 V52(E113) V52(E77) a=0, can_bisect=0
F55 V52(E113) V40(E31) a=0.523599, can_bisect=1
F55 V52(E113) V52(E114) a=0, can_bisect=0
F55 V52(E77) V40(E31) a=0, can_bisect=0
F55 V52(E77) V52(E114) a=0.90413, can_bisect=1
F55 V40(E31) V52(E114) a=0, can_bisect=0
F55 best_angle = 0.90413
Longest Edge: 2.15653
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 3
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=2.1565272705018663
# Vertices: V15 V16 V36 V40 V52
# UEdges: 
E2 ( V36 V15 F2 F4 ) (-5.555037402781607625001925,0.000000000000000000000000)
E3 ( V15 V52 F2 F7 ) (4.955987196007475503733986,-4.158567028289196443324727)
E4 ( V52 V36 F2 F42 ) (0.599050206774131788201032,4.158567028289196443324727)
E5 ( V52 V36 F40 F4 ) (0.599050206774130789000310,-4.158567028289195555146307)
E9 ( V52 V15 F4 F11 ) (-4.955987196007476391912405,-4.158567028289195555146307)
E11 ( V52 V36 F42 F6 ) (-3.900949793225868766910480,1.560490816935879987426006)
E15 ( V52 V15 F6 F15 ) (-1.123431091835064954409518,6.371294326717481304456214)
E16 ( V15 V36 F6 F14 ) (-2.777518701390803368411753,-4.810803509781601761119418)
E17 ( V16 V52 F7 F47 ) (3.991365074142812385815660,1.312076874242931623371078)
E19 ( V15 V16 F7 F17 ) (0.964622121864663117918326,-5.470643902532128066695805)
E25 ( V36 V52 F40 F10 ) (3.900949793225869210999690,1.560490816935879099247586)
E26 ( V36 V15 F10 F22 ) (2.777518701390802924322543,-4.810803509781600872940999)
E27 ( V15 V52 F10 F23 ) (1.123431091835065842587937,6.371294326717481304456214)
E29 ( V40 V15 F11 F25 ) (-0.964622121864664450185956,-5.470643902532126290338965)
E31 ( V52 V40 F11 F56 ) (-3.991365074142811053548030,1.312076874242931401326473)
E37 ( V36 V52 F38 F14 ) (-3.301899586451737089731751,2.598076211353317344077141)
E39 ( V15 V52 F14 F29 ) (-6.079418287842541346321923,-2.212727298428284417042278)
E41 ( V40 V15 F15 F28 ) (-4.255405533718930044528861,3.570709213853311414510472)
E43 ( V52 V40 F15 F53 ) (3.131974441883864646030133,2.800585112864169889945742)
E48 ( V15 V52 F17 F32 ) (-3.234790905752800149741688,-5.602822200625596771317305)
E49 ( V52 V16 F17 F49 ) (4.199413027617462823570804,0.132178298093468926666105)
E55 ( V16 V52 F47 F20 ) (2.214176277786668922686886,-3.570709213853311414510472)
E56 ( V16 V15 F20 F23 ) (-4.255405533718930044528861,-3.570709213853311414510472)
E57 ( V15 V52 F20 F28 ) (6.469581811505598523126537,-0.000000000000000000000000)
E59 ( V52 V36 F38 F22 ) (3.301899586451738421999380,2.598076211353314679541882)
E63 ( V52 V15 F22 F33 ) (6.079418287842541346321923,-2.212727298428286193399117)
E65 ( V16 V52 F23 F45 ) (-3.131974441883864646030133,2.800585112864169001767323)
E71 ( V40 V52 F25 F54 ) (-4.199413027617463711749224,0.132178298093469925866827)
E72 ( V52 V15 F25 F36 ) (3.234790905752798817474059,-5.602822200625596771317305)
E77 ( V52 V40 F56 F28 ) (-2.214176277786668478597676,-3.570709213853311414510472)
E83 ( V16 V52 F29 F49 ) (-0.859390632258946407517897,-4.112661987107101069227610)
E85 ( V15 V16 F29 F36 ) (-5.220027655583594494714816,1.899934688678815986051518)
E89 ( V52 V40 F53 F32 ) (-1.985236749830794344973128,3.702887511946782339578021)
E94 ( V15 V40 F32 F33 ) (-5.220027655583594494714816,-1.899934688678813987650074)
E97 ( V52 V40 F33 F54 ) (0.859390632258947073651711,-4.112661987107101069227610)
E103 ( V16 V52 F45 F36 ) (1.985236749830795011106943,3.702887511946779675042762)
E107 ( V52 V52 F43 F38 ) (0.000000000000001332267630,5.196152422706632023619022)
E108 ( V52 V52 F43 F40 ) (4.500000000000000000000000,-2.598076211353316011809511)
E109 ( V52 V52 F43 F42 ) (-4.500000000000000000000000,-2.598076211353316011809511)
E110 ( V52 V52 F50 F45 ) (5.117211191714659435092472,0.902302399082610784297742)
E111 ( V52 V52 F50 F47 ) (-1.777188796356143463128774,-4.882786088096243481970760)
E112 ( V52 V52 F50 F49 ) (-3.340022395358515971963698,3.980483689013631476427690)
E113 ( V52 V52 F53 F57 ) (5.117211191714658546914052,-0.902302399082612449632279)
E114 ( V52 V52 F57 F54 ) (3.340022395358515971963698,3.980483689013631032338481)
E115 ( V52 V52 F57 F56 ) (1.777188796356142574950354,-4.882786088096242593792340)
# Faces: 
F2 +E2 +E3 +E4
F4 -E5 +E9 -E2
F6 -E11 +E15 +E16
F7 +E17 -E3 +E19
F10 +E26 +E27 -E25
F11 +E29 -E9 +E31
F14 -E16 +E39 -E37
F15 +E41 -E15 +E43
F17 -E19 +E48 +E49
F20 +E56 +E57 -E55
F22 -E59 +E63 -E26
F23 +E65 -E27 -E56
F25 +E71 +E72 -E29
F28 -E77 -E57 -E41
F29 +E83 -E39 +E85
F32 -E89 -E48 +E94
F33 -E94 -E63 +E97
F36 -E85 -E72 -E103
F38 +E59 +E37 -E107
F40 +E5 +E25 -E108
F42 +E11 -E4 -E109
F43 +E108 +E107 +E109
F45 -E65 +E103 -E110
F47 -E17 +E55 -E111
F49 -E83 -E49 -E112
F50 +E111 +E110 +E112
F53 +E89 -E43 +E113
F54 -E71 -E97 -E114
F56 +E77 -E31 -E115
F57 +E114 -E113 +E115
#UEdges Algebraic 
#E2: (-3 0 0 0 0 0)+(0 -1 -1 0 2 -1)t1
#E3: (0 0 0 0 0 0)+(0 3 0 0 -3 0)t1
#E4: (3 0 0 0 0 0)+(0 -2 1 0 1 1)t1
#E5: (3 0 0 0 0 0)+(0 1 -2 0 -2 1)t1
#E9: (0 0 0 0 0 0)+(0 0 -3 0 0 0)t1
#E11: (0 0 0 -3 0 0)+(0 -2 1 0 1 1)t1
#E15: (0 0 0 0 0 0)+(0 0 0 0 0 3)t1
#E16: (0 0 0 -3 0 0)+(0 -2 1 0 1 -2)t1
#E17: (0 0 0 0 0 3)+(1 1 0 1 -2 0)t1
#E19: (0 0 0 0 0 -3)+(-1 2 0 -1 -1 0)t1
#E25: (3 0 0 -3 0 0)+(0 -1 2 0 2 -1)t1
#E26: (3 0 0 -3 0 0)+(0 -1 2 0 -1 -1)t1
#E27: (0 0 0 0 0 0)+(0 0 0 0 3 0)t1
#E29: (0 0 0 0 -3 0)+(2 0 -2 -1 0 1)t1
#E31: (0 0 0 0 3 0)+(-2 0 -1 1 0 -1)t1
#E37: (0 0 0 3 0 0)+(0 -1 -1 0 -1 2)t1
#E39: (0 0 0 0 0 0)+(0 -3 0 0 0 0)t1
#E41: (0 -3 0 0 3 0)+(-1 0 1 -1 0 1)t1
#E43: (0 3 0 0 -3 0)+(1 0 -1 1 0 2)t1
#E48: (0 0 0 0 0 0)+(0 0 0 -3 0 0)t1
#E49: (0 0 0 0 0 -3)+(-1 2 0 2 -1 0)t1
#E55: (0 0 -3 0 0 0)+(1 1 0 1 -2 0)t1
#E56: (0 0 -3 0 0 0)+(-2 1 0 1 -2 0)t1
#E57: (0 0 0 0 0 0)+(3 0 0 0 0 0)t1
#E59: (-3 0 0 3 0 0)+(0 1 1 0 1 -2)t1
#E63: (0 0 0 0 0 0)+(0 0 3 0 0 -3)t1
#E65: (0 0 -3 0 0 0)+(-2 1 0 1 1 0)t1
#E71: (0 0 0 0 -3 0)+(-1 0 -2 2 0 1)t1
#E72: (0 0 0 0 0 0)+(3 0 0 -3 0 0)t1
#E77: (0 3 0 0 -3 0)+(-2 0 -1 1 0 -1)t1
#E83: (0 0 3 0 0 -3)+(1 -2 0 -2 1 0)t1
#E85: (0 0 -3 0 0 3)+(-1 -1 0 2 -1 0)t1
#E89: (0 -3 0 0 0 0)+(1 0 -1 1 0 2)t1
#E94: (0 -3 0 0 0 0)+(1 0 -1 -2 0 2)t1
#E97: (0 -3 0 0 0 0)+(1 0 2 -2 0 -1)t1
#E103: (0 0 3 0 0 -3)+(-2 1 0 1 1 0)t1
#E107: (-3 0 0 6 0 0)+(0 0 0 0 0 0)t1
#E108: (6 0 0 -3 0 0)+(0 0 0 0 0 0)t1
#E109: (-3 0 0 -3 0 0)+(0 0 0 0 0 0)t1
#E110: (0 0 6 0 0 -3)+(0 0 0 0 0 0)t1
#E111: (0 0 -3 0 0 -3)+(0 0 0 0 0 0)t1
#E112: (0 0 -3 0 0 6)+(0 0 0 0 0 0)t1
#E113: (0 6 0 0 -3 0)+(0 0 0 0 0 0)t1
#E114: (0 3 0 0 3 0)+(0 0 0 0 0 0)t1
#E115: (0 3 0 0 -6 0)+(0 0 0 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 5
## V15(18) (4 PI) V16(9) (4 PI) V36(9) (4 PI) V40(9) (4 PI) V52(45) (14 PI) 
# edges: 45
## E2(2) E3(2) E4(2) E5(2) E9(2) E11(2) E15(2) E16(2) E17(2) E19(2) E25(2) E26(2) E27(2) E29(2) E31(2) E37(2) E39(2) E41(2) E43(2) E48(2) E49(2) E55(2) E56(2) E57(2) E59(2) E63(2) E65(2) E71(2) E72(2) E77(2) E83(2) E85(2) E89(2) E94(2) E97(2) E103(2) E107(2) E108(2) E109(2) E110(2) E111(2) E112(2) E113(2) E114(2) E115(2) 
# faces: 30
# genus: 6
# area: 320.19
sweeping start V15 depth = 59.6462
File =  depth = 59.6462 follow_depth = 298.231 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 60 unusual 0 close 33039 reject 0
V15: AaXX V36: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0097896 Max Area: 0.0097896
    total: 0.84823 0.515662 sym = 2
    raw = 36 short saddle = 29.2227 short cyl = 29.2227 tag #3
V15: AbBa V16: CfDe V36: cEdF V40: GgHh V52: IlJkKjLiMoNnOm   cylinders |1|2.15684|2.17415|2.92923|3.70624|4.80861|5.21038|6.21038|6.96545|
Moduli: |27.3739|23.8014|46.5368|13.4064|21.9116|1|4.01032|6.60384|12.3334| Min Area: 1 Max Area: 1
    total: 0.424115 0.257831 sym = 4
    raw = 36 short saddle = 5.19615 short cyl = 8.31713 tag #4
V15: AXBX V52: aFfGgXXCXXdDeE V16: bXcX   cylinders |1|1.82175|
Moduli: |7.13283|1| Min Area: 0.211884 Max Area: 0.211884
    total: 0.424115 0.257831 sym = 2
    raw = 18 short saddle = 5.55504 short cyl = 35.3324 tag #0
V15: AaXX V16: BXXb V36: CXXc V52: DdXXXXXXXXXXXX   cylinders |1|4.41147|
Moduli: |5.85359|1| Min Area: 0.0728948 Max Area: 0.0728948
    total: 0.424115 0.257831 sym = 2
    raw = 18 short saddle = 50.1977 short cyl = 50.1977 tag #5
V15: AaXX V36: BXXb V40: CXXc V52: DdXXXXXXXXXXXX   cylinders |1|2.87939|
Moduli: |37.9736|1| Min Area: 0.0588941 Max Area: 0.0588941
    total: 0.424115 0.257831 sym = 2
    raw = 18 short saddle = 32.3038 short cyl = 32.3038 tag #7
V15: AXXa V36: BbXX V40: CcXX V52: DXXXXXXXXXXXXd   cylinders |1|4.41147|
Moduli: |5.85359|1| Min Area: 0.0728948 Max Area: 0.0728948
    total: 0.424115 0.257831 sym = 2
    raw = 18 short saddle = 50.1977 short cyl = 50.1977 tag #8
V15: AXXa V16: BbXX V40: CcXX V52: DXXXXXXXXXXXXd   cylinders |1|2.87939|
Moduli: |37.9736|1| Min Area: 0.0588941 Max Area: 0.0588941
    total: 0.424115 0.257831 sym = 2
    raw = 18 short saddle = 32.3038 short cyl = 32.3038 tag #10

total cylinders: 8.90642 5.41445 raw = 189 rejected = 0
""",
    (2, 7, 5, 6): r"""
max loc = 1
alpha =  7 beta = 5 gamma = 6 delta =2
a =  2.19911 b = 1.5708 c= 1.88496 d=0.628319
a1_min = -0.314159 a1_max = 1.5708
a1 = 0.970796
angle a check: 4.44089e-16
angle b check: 0
angle c check: 3.14159
angle d check: 2.22045e-16
p = 1 q = 1.77103 p-q = 1.4617
p = (1,0) q = (1,1.4617) r = (-1.69917,2.33871)
Imaginary of t_cx = 0
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 1)t1
Checking p_alt (0,0)
Checking q_alt (9.99201e-16,-1.11022e-15)
Checking r_alt (1.9984e-15,-8.88178e-16)
Checking p_algt (0,0)
Checking q_algt (9.99201e-16,-1.11022e-15)
Checking r_algt (1.9984e-15,-8.88178e-16)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=2.890804835709468
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000888178420,1.461695947078100576987936)
E2 ( V2 V3 F0 NULL ) (-2.699172449685790731166435,0.877014292432136244315188)
E3 ( V3 V0 F0 NULL ) (1.699172449685789620943410,-2.338710239510236821303124)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (3/5 0 -6/5 0 4/5 0 -2/5 0)+(0 0 0 1 0 -1 0 1)t1
#E2: (-8/5 0 6/5 0 -4/5 0 2/5 0)+(0 0 0 -1 0 1 0 0)t1
#E3: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.6, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.228508, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.6
In RetriangulateSurface
Removing V1
F41 V54(E5) V72(E31) a=0, can_bisect=0
F41 V54(E5) V54(E11) a=0.970796, can_bisect=1
F41 V54(E5) V2(E4) a=0, can_bisect=0
F41 V72(E31) V54(E11) a=0, can_bisect=0
F41 V72(E31) V2(E4) a=0.6, can_bisect=1
F41 V54(E11) V2(E4) a=0, can_bisect=0
F41 best_angle = 0.970796
Removing V21
F44 V2(E67) V54(E83) a=0, can_bisect=0
F44 V2(E67) V72(E73) a=0.6, can_bisect=1
F44 V2(E67) V54(E17) a=0, can_bisect=0
F44 V54(E83) V72(E73) a=0, can_bisect=0
F44 V54(E83) V54(E17) a=0.970796, can_bisect=1
F44 V72(E73) V54(E17) a=0, can_bisect=0
F44 best_angle = 0.970796
Removing V27
F47 V2(E61) V54(E89) a=0, can_bisect=0
F47 V2(E61) V72(E79) a=0.6, can_bisect=1
F47 V2(E61) V54(E23) a=0, can_bisect=0
F47 V54(E89) V72(E79) a=0, can_bisect=0
F47 V54(E89) V54(E23) a=0.970796, can_bisect=1
F47 V72(E79) V54(E23) a=0, can_bisect=0
F47 best_angle = 0.970796
Removing V38
F50 V54(E95) V2(E49) a=0, can_bisect=0
F50 V54(E95) V54(E101) a=0.970796, can_bisect=1
F50 V54(E95) V72(E37) a=0, can_bisect=0
F50 V2(E49) V54(E101) a=0, can_bisect=0
F50 V2(E49) V72(E37) a=0.6, can_bisect=1
F50 V54(E101) V72(E37) a=0, can_bisect=0
F50 best_angle = 0.970796
Removing V44
F53 V54(E107) V2(E55) a=0, can_bisect=0
F53 V54(E107) V54(E113) a=0.970796, can_bisect=1
F53 V54(E107) V72(E43) a=0, can_bisect=0
F53 V2(E55) V54(E113) a=0, can_bisect=0
F53 V2(E55) V72(E43) a=0.6, can_bisect=1
F53 V54(E113) V72(E43) a=0, can_bisect=0
F53 best_angle = 0.970796
Removing V68
F56 V2(E95) V54(E55) a=0, can_bisect=0
F56 V2(E95) V2(E11) a=0.342478, can_bisect=1
F56 V2(E95) V54(E49) a=0.606687, can_bisect=1
F56 V2(E95) V2(E23) a=0.970796, can_bisect=1
F56 V2(E95) V54(E4) a=1.22832, can_bisect=1
F56 V2(E95) V2(E17) a=0.970796, can_bisect=1
F56 V2(E95) V54(E61) a=0.606687, can_bisect=1
F56 V2(E95) V2(E107) a=0.342478, can_bisect=1
F56 V2(E95) V54(E67) a=0, can_bisect=0
F56 V54(E55) V2(E11) a=0, can_bisect=0
F56 V54(E55) V54(E49) a=0.285841, can_bisect=1
F56 V54(E55) V2(E23) a=0.606687, can_bisect=1
F56 V54(E55) V54(E4) a=0.914159, can_bisect=1
F56 V54(E55) V2(E17) a=1.22832, can_bisect=1
F56 V54(E55) V54(E61) a=0.914159, can_bisect=1
F56 V54(E55) V2(E107) a=0.606687, can_bisect=1
F56 V54(E55) V54(E67) a=0.285841, can_bisect=1
F56 V2(E11) V54(E49) a=0, can_bisect=0
F56 V2(E11) V2(E23) a=0.342478, can_bisect=1
F56 V2(E11) V54(E4) a=0.606687, can_bisect=1
F56 V2(E11) V2(E17) a=0.970796, can_bisect=1
F56 V2(E11) V54(E61) a=1.22832, can_bisect=1
F56 V2(E11) V2(E107) a=0.970796, can_bisect=1
F56 V2(E11) V54(E67) a=0.606687, can_bisect=1
F56 V54(E49) V2(E23) a=0, can_bisect=0
F56 V54(E49) V54(E4) a=0.285841, can_bisect=1
F56 V54(E49) V2(E17) a=0.606687, can_bisect=1
F56 V54(E49) V54(E61) a=0.914159, can_bisect=1
F56 V54(E49) V2(E107) a=1.22832, can_bisect=1
F56 V54(E49) V54(E67) a=0.914159, can_bisect=1
F56 V2(E23) V54(E4) a=0, can_bisect=0
F56 V2(E23) V2(E17) a=0.342478, can_bisect=1
F56 V2(E23) V54(E61) a=0.606687, can_bisect=1
F56 V2(E23) V2(E107) a=0.970796, can_bisect=1
F56 V2(E23) V54(E67) a=1.22832, can_bisect=1
F56 V54(E4) V2(E17) a=0, can_bisect=0
F56 V54(E4) V54(E61) a=0.285841, can_bisect=1
F56 V54(E4) V2(E107) a=0.606687, can_bisect=1
F56 V54(E4) V54(E67) a=0.914159, can_bisect=1
F56 V2(E17) V54(E61) a=0, can_bisect=0
F56 V2(E17) V2(E107) a=0.342478, can_bisect=1
F56 V2(E17) V54(E67) a=0.606687, can_bisect=1
F56 V54(E61) V2(E107) a=0, can_bisect=0
F56 V54(E61) V54(E67) a=0.285841, can_bisect=1
F56 V2(E107) V54(E67) a=0, can_bisect=0
F56 best_angle = 1.22832
F58 V54(E4) V2(E17) a=0, can_bisect=0
F58 V54(E4) V54(E61) a=0.285841, can_bisect=1
F58 V54(E4) V2(E107) a=0.606687, can_bisect=1
F58 V54(E4) V54(E67) a=0.314159, can_bisect=1
F58 V54(E4) V2(E124) a=0, can_bisect=0
F58 V2(E17) V54(E61) a=0, can_bisect=0
F58 V2(E17) V2(E107) a=0.342478, can_bisect=1
F58 V2(E17) V54(E67) a=0.606687, can_bisect=1
F58 V2(E17) V2(E124) a=0.314159, can_bisect=1
F58 V54(E61) V2(E107) a=0, can_bisect=0
F58 V54(E61) V54(E67) a=0.285841, can_bisect=1
F58 V54(E61) V2(E124) a=0.606687, can_bisect=1
F58 V2(E107) V54(E67) a=0, can_bisect=0
F58 V2(E107) V2(E124) a=0.342478, can_bisect=1
F58 V54(E67) V2(E124) a=0, can_bisect=0
F58 best_angle = 0.606687
F60 V2(E107) V54(E67) a=0, can_bisect=0
F60 V2(E107) V2(E124) a=0.342478, can_bisect=1
F60 V2(E107) V54(E125) a=0, can_bisect=0
F60 V54(E67) V2(E124) a=0, can_bisect=0
F60 V54(E67) V54(E125) a=0.307472, can_bisect=1
F60 V2(E124) V54(E125) a=0, can_bisect=0
F60 best_angle = 0.342478
F59 V54(E4) V2(E17) a=0, can_bisect=0
F59 V54(E4) V54(E61) a=0.285841, can_bisect=1
F59 V54(E4) V2(E125) a=0, can_bisect=0
F59 V2(E17) V54(E61) a=0, can_bisect=0
F59 V2(E17) V2(E125) a=0.307472, can_bisect=1
F59 V54(E61) V2(E125) a=0, can_bisect=0
F59 best_angle = 0.307472
F57 V2(E95) V54(E55) a=0, can_bisect=0
F57 V2(E95) V2(E11) a=0.342478, can_bisect=1
F57 V2(E95) V54(E49) a=0.606687, can_bisect=1
F57 V2(E95) V2(E23) a=0.314159, can_bisect=1
F57 V2(E95) V54(E124) a=0, can_bisect=0
F57 V54(E55) V2(E11) a=0, can_bisect=0
F57 V54(E55) V54(E49) a=0.285841, can_bisect=1
F57 V54(E55) V2(E23) a=0.606687, can_bisect=1
F57 V54(E55) V54(E124) a=0.314159, can_bisect=1
F57 V2(E11) V54(E49) a=0, can_bisect=0
F57 V2(E11) V2(E23) a=0.342478, can_bisect=1
F57 V2(E11) V54(E124) a=0.606687, can_bisect=1
F57 V54(E49) V2(E23) a=0, can_bisect=0
F57 V54(E49) V54(E124) a=0.285841, can_bisect=1
F57 V2(E23) V54(E124) a=0, can_bisect=0
F57 best_angle = 0.606687
F66 V54(E49) V2(E23) a=0, can_bisect=0
F66 V54(E49) V54(E124) a=0.285841, can_bisect=1
F66 V54(E49) V2(E128) a=0, can_bisect=0
F66 V2(E23) V54(E124) a=0, can_bisect=0
F66 V2(E23) V2(E128) a=0.314159, can_bisect=1
F66 V54(E124) V2(E128) a=0, can_bisect=0
F66 best_angle = 0.314159
F65 V2(E95) V54(E55) a=0, can_bisect=0
F65 V2(E95) V2(E11) a=0.307472, can_bisect=1
F65 V2(E95) V54(E128) a=0, can_bisect=0
F65 V54(E55) V2(E11) a=0, can_bisect=0
F65 V54(E55) V54(E128) a=0.285841, can_bisect=1
F65 V2(E11) V54(E128) a=0, can_bisect=0
F65 best_angle = 0.307472
Removing V74
F71 V72(E101) V54(E43) a=0, can_bisect=0
F71 V72(E101) V72(E5) a=0.342478, can_bisect=1
F71 V72(E101) V54(E37) a=0.606687, can_bisect=1
F71 V72(E101) V72(E89) a=0.970796, can_bisect=1
F71 V72(E101) V54(E31) a=1.22832, can_bisect=1
F71 V72(E101) V72(E83) a=0.970796, can_bisect=1
F71 V72(E101) V54(E79) a=0.606687, can_bisect=1
F71 V72(E101) V72(E113) a=0.342478, can_bisect=1
F71 V72(E101) V54(E73) a=0, can_bisect=0
F71 V54(E43) V72(E5) a=0, can_bisect=0
F71 V54(E43) V54(E37) a=0.285841, can_bisect=1
F71 V54(E43) V72(E89) a=0.606687, can_bisect=1
F71 V54(E43) V54(E31) a=0.914159, can_bisect=1
F71 V54(E43) V72(E83) a=1.22832, can_bisect=1
F71 V54(E43) V54(E79) a=0.914159, can_bisect=1
F71 V54(E43) V72(E113) a=0.606687, can_bisect=1
F71 V54(E43) V54(E73) a=0.285841, can_bisect=1
F71 V72(E5) V54(E37) a=0, can_bisect=0
F71 V72(E5) V72(E89) a=0.342478, can_bisect=1
F71 V72(E5) V54(E31) a=0.606687, can_bisect=1
F71 V72(E5) V72(E83) a=0.970796, can_bisect=1
F71 V72(E5) V54(E79) a=1.22832, can_bisect=1
F71 V72(E5) V72(E113) a=0.970796, can_bisect=1
F71 V72(E5) V54(E73) a=0.606687, can_bisect=1
F71 V54(E37) V72(E89) a=0, can_bisect=0
F71 V54(E37) V54(E31) a=0.285841, can_bisect=1
F71 V54(E37) V72(E83) a=0.606687, can_bisect=1
F71 V54(E37) V54(E79) a=0.914159, can_bisect=1
F71 V54(E37) V72(E113) a=1.22832, can_bisect=1
F71 V54(E37) V54(E73) a=0.914159, can_bisect=1
F71 V72(E89) V54(E31) a=0, can_bisect=0
F71 V72(E89) V72(E83) a=0.342478, can_bisect=1
F71 V72(E89) V54(E79) a=0.606687, can_bisect=1
F71 V72(E89) V72(E113) a=0.970796, can_bisect=1
F71 V72(E89) V54(E73) a=1.22832, can_bisect=1
F71 V54(E31) V72(E83) a=0, can_bisect=0
F71 V54(E31) V54(E79) a=0.285841, can_bisect=1
F71 V54(E31) V72(E113) a=0.606687, can_bisect=1
F71 V54(E31) V54(E73) a=0.914159, can_bisect=1
F71 V72(E83) V54(E79) a=0, can_bisect=0
F71 V72(E83) V72(E113) a=0.342478, can_bisect=1
F71 V72(E83) V54(E73) a=0.606687, can_bisect=1
F71 V54(E79) V72(E113) a=0, can_bisect=0
F71 V54(E79) V54(E73) a=0.285841, can_bisect=1
F71 V72(E113) V54(E73) a=0, can_bisect=0
F71 best_angle = 1.22832
F73 V54(E31) V72(E83) a=0, can_bisect=0
F73 V54(E31) V54(E79) a=0.285841, can_bisect=1
F73 V54(E31) V72(E113) a=0.606687, can_bisect=1
F73 V54(E31) V54(E73) a=0.314159, can_bisect=1
F73 V54(E31) V72(E131) a=0, can_bisect=0
F73 V72(E83) V54(E79) a=0, can_bisect=0
F73 V72(E83) V72(E113) a=0.342478, can_bisect=1
F73 V72(E83) V54(E73) a=0.606687, can_bisect=1
F73 V72(E83) V72(E131) a=0.314159, can_bisect=1
F73 V54(E79) V72(E113) a=0, can_bisect=0
F73 V54(E79) V54(E73) a=0.285841, can_bisect=1
F73 V54(E79) V72(E131) a=0.606687, can_bisect=1
F73 V72(E113) V54(E73) a=0, can_bisect=0
F73 V72(E113) V72(E131) a=0.342478, can_bisect=1
F73 V54(E73) V72(E131) a=0, can_bisect=0
F73 best_angle = 0.606687
F75 V72(E113) V54(E73) a=0, can_bisect=0
F75 V72(E113) V72(E131) a=0.342478, can_bisect=1
F75 V72(E113) V54(E132) a=0, can_bisect=0
F75 V54(E73) V72(E131) a=0, can_bisect=0
F75 V54(E73) V54(E132) a=0.307472, can_bisect=1
F75 V72(E131) V54(E132) a=0, can_bisect=0
F75 best_angle = 0.342478
F74 V54(E31) V72(E83) a=0, can_bisect=0
F74 V54(E31) V54(E79) a=0.285841, can_bisect=1
F74 V54(E31) V72(E132) a=0, can_bisect=0
F74 V72(E83) V54(E79) a=0, can_bisect=0
F74 V72(E83) V72(E132) a=0.307472, can_bisect=1
F74 V54(E79) V72(E132) a=0, can_bisect=0
F74 best_angle = 0.307472
F72 V72(E101) V54(E43) a=0, can_bisect=0
F72 V72(E101) V72(E5) a=0.342478, can_bisect=1
F72 V72(E101) V54(E37) a=0.606687, can_bisect=1
F72 V72(E101) V72(E89) a=0.314159, can_bisect=1
F72 V72(E101) V54(E131) a=0, can_bisect=0
F72 V54(E43) V72(E5) a=0, can_bisect=0
F72 V54(E43) V54(E37) a=0.285841, can_bisect=1
F72 V54(E43) V72(E89) a=0.606687, can_bisect=1
F72 V54(E43) V54(E131) a=0.314159, can_bisect=1
F72 V72(E5) V54(E37) a=0, can_bisect=0
F72 V72(E5) V72(E89) a=0.342478, can_bisect=1
F72 V72(E5) V54(E131) a=0.606687, can_bisect=1
F72 V54(E37) V72(E89) a=0, can_bisect=0
F72 V54(E37) V54(E131) a=0.285841, can_bisect=1
F72 V72(E89) V54(E131) a=0, can_bisect=0
F72 best_angle = 0.606687
F81 V54(E37) V72(E89) a=0, can_bisect=0
F81 V54(E37) V54(E131) a=0.285841, can_bisect=1
F81 V54(E37) V72(E135) a=0, can_bisect=0
F81 V72(E89) V54(E131) a=0, can_bisect=0
F81 V72(E89) V72(E135) a=0.314159, can_bisect=1
F81 V54(E131) V72(E135) a=0, can_bisect=0
F81 best_angle = 0.314159
F80 V72(E101) V54(E43) a=0, can_bisect=0
F80 V72(E101) V72(E5) a=0.307472, can_bisect=1
F80 V72(E101) V54(E135) a=0, can_bisect=0
F80 V54(E43) V72(E5) a=0, can_bisect=0
F80 V54(E43) V54(E135) a=0.285841, can_bisect=1
F80 V72(E5) V54(E135) a=0, can_bisect=0
F80 best_angle = 0.307472
Extra: Flipping E124
Extra: Flipping E125
Extra: Flipping E128
Extra: Flipping E131
Extra: Flipping E132
Extra: Flipping E135
Longest Edge: 5.39834
Looking for shortest saddle...1.77103
Clearing denominators: multiplying by 5
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=2.890804835709468
# Vertices: V2 V54 V72
# UEdges: 
E4 ( V54 V2 F89 F43 ) (5.000000000000004440892099,7.308479735390502440850469)
E5 ( V54 V72 F42 F96 ) (4.999999999999994670929482,-7.308479735390502440850469)
E11 ( V54 V2 F43 F90 ) (-4.999999999999994670929482,7.308479735390502440850469)
E17 ( V2 V54 F63 F46 ) (0.250731633266198516984957,8.851610570438200653597960)
E23 ( V2 V54 F89 F49 ) (8.495862248428954544010594,2.496838140195183353142738)
E31 ( V72 V54 F42 F95 ) (5.000000000000004440892099,7.308479735390502440850469)
E37 ( V54 V72 F96 F52 ) (-8.495862248428949214940076,2.496838140195183353142738)
E43 ( V54 V72 F84 F55 ) (-0.250731633266196851650420,8.851610570438200653597960)
E49 ( V54 V2 F90 F51 ) (8.495862248428949214940076,-2.496838140195183353142738)
E55 ( V54 V2 F69 F54 ) (0.250731633266196851650420,-8.851610570438200653597960)
E61 ( V54 V2 F63 F49 ) (-5.405692304679480031381900,7.013727022756349072096782)
E67 ( V2 V54 F46 F61 ) (8.340901577015669587922275,2.973758047513470170741812)
E73 ( V72 V54 F45 F76 ) (-8.340901577015669587922275,-2.973758047513470170741812)
E79 ( V72 V54 F48 F78 ) (-5.405692304679480031381900,7.013727022756349072096782)
E83 ( V72 V54 F78 F45 ) (-0.250731633266198516984957,-8.851610570438200653597960)
E89 ( V72 V54 F95 F48 ) (-8.495862248428954544010594,-2.496838140195183353142738)
E95 ( V54 V2 F51 F69 ) (5.405692304679478255025060,7.013727022756349072096782)
E101 ( V54 V72 F52 F84 ) (-5.405692304679478255025060,-7.013727022756349072096782)
E107 ( V54 V2 F54 F61 ) (8.340901577015669587922275,-2.973758047513468838474182)
E113 ( V54 V72 F55 F76 ) (-8.340901577015669587922275,2.973758047513468838474182)
E119 ( V54 V54 F43 F42 ) (10.000000000000000000000000,0.000000000000000000000000)
E120 ( V54 V54 F46 F45 ) (-8.090169943749472736271855,5.877852522924729150588519)
E121 ( V54 V54 F49 F48 ) (3.090169943749474512628694,9.510565162951532869328730)
E122 ( V54 V54 F52 F51 ) (-3.090169943749470959915016,9.510565162951532869328730)
E123 ( V54 V54 F55 F54 ) (8.090169943749472736271855,5.877852522924731815123778)
E126 ( V2 V2 F87 F61 ) (-16.681803154031339175844550,-0.000000000000001110223025)
E127 ( V2 V2 F88 F63 ) (-5.154960671413282291553060,15.865337593194547949337903)
E129 ( V2 V2 F91 F87 ) (-8.340901577015660706138078,25.670655468780232411063480)
E130 ( V2 V2 F91 F69 ) (-5.154960671413281403374640,-15.865337593194547949337903)
E133 ( V72 V72 F93 F76 ) (16.681803154031339175844550,0.000000000000001110223025)
E134 ( V72 V72 F94 F78 ) (5.154960671413282291553060,-15.865337593194547949337903)
E136 ( V72 V72 F97 F93 ) (8.340901577015660706138078,-25.670655468780232411063480)
E137 ( V72 V72 F97 F84 ) (5.154960671413281403374640,15.865337593194547949337903)
E138 ( V2 V2 F88 F87 ) (-8.340901577015676693349633,-25.670655468780235963777159)
E139 ( V2 V2 F88 F89 ) (13.495862248428958096724273,9.805317875585686238082417)
E140 ( V2 V2 F90 F91 ) (-13.495862248428945662226397,9.805317875585686238082417)
E141 ( V72 V72 F94 F93 ) (8.340901577015676693349633,25.670655468780235963777159)
E142 ( V72 V72 F94 F95 ) (-13.495862248428958096724273,-9.805317875585686238082417)
E143 ( V72 V72 F96 F97 ) (13.495862248428945662226397,-9.805317875585686238082417)
# Faces: 
F42 +E5 +E31 -E119
F43 +E11 -E4 +E119
F45 -E83 +E73 -E120
F46 -E17 +E67 +E120
F48 -E89 +E79 -E121
F49 -E23 -E61 +E121
F51 +E95 -E49 -E122
F52 +E101 -E37 +E122
F54 +E107 -E55 -E123
F55 +E113 -E43 +E123
F61 -E107 -E67 -E126
F63 +E17 +E61 -E127
F69 -E95 +E55 -E130
F76 -E113 -E73 -E133
F78 +E83 -E79 -E134
F84 -E101 +E43 -E137
F87 -E138 +E126 -E129
F88 +E139 +E127 +E138
F89 -E139 +E23 +E4
F90 +E140 -E11 +E49
F91 -E140 +E129 +E130
F93 -E141 +E133 -E136
F94 +E142 +E134 +E141
F95 -E142 +E89 -E31
F96 +E143 -E5 +E37
F97 -E143 +E136 +E137
#UEdges Algebraic 
#E4: (8 0 -6 0 4 0 -2 0)+(0 0 0 5 0 -5 0 5)t1
#E5: (2 0 6 0 -4 0 2 0)+(0 0 0 -5 0 5 0 -5)t1
#E11: (-2 0 -6 0 4 0 -2 0)+(0 0 0 5 0 -5 0 5)t1
#E17: (-8 0 6 0 -4 0 2 0)+(0 5 0 -5 0 5 0 0)t1
#E23: (2 0 -4 0 6 0 2 0)+(0 0 0 5 0 -5 0 0)t1
#E31: (8 0 -6 0 4 0 -2 0)+(0 0 0 5 0 -5 0 5)t1
#E37: (2 0 -4 0 6 0 2 0)+(0 0 0 0 0 -5 0 5)t1
#E43: (2 0 6 0 -4 0 2 0)+(0 -5 0 5 0 0 0 0)t1
#E49: (-2 0 4 0 -6 0 -2 0)+(0 0 0 0 0 5 0 -5)t1
#E55: (-2 0 -6 0 4 0 -2 0)+(0 5 0 -5 0 0 0 0)t1
#E61: (-2 0 4 0 4 0 -2 0)+(0 0 0 -5 0 5 0 0)t1
#E67: (2 0 -4 0 6 0 -8 0)+(0 5 0 -5 0 5 0 0)t1
#E73: (-2 0 4 0 -6 0 8 0)+(0 -5 0 5 0 -5 0 0)t1
#E79: (-2 0 4 0 4 0 -2 0)+(0 0 0 -5 0 5 0 0)t1
#E83: (8 0 -6 0 4 0 -2 0)+(0 -5 0 5 0 -5 0 0)t1
#E89: (-2 0 4 0 -6 0 -2 0)+(0 0 0 -5 0 5 0 0)t1
#E95: (-2 0 4 0 -6 0 8 0)+(0 0 0 0 0 5 0 -5)t1
#E101: (2 0 -4 0 6 0 -8 0)+(0 0 0 0 0 -5 0 5)t1
#E107: (-2 0 4 0 4 0 -2 0)+(0 5 0 -5 0 0 0 0)t1
#E113: (2 0 -4 0 -4 0 2 0)+(0 -5 0 5 0 0 0 0)t1
#E119: (10 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E120: (-10 0 10 0 -10 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E121: (0 0 0 0 10 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E122: (0 0 0 0 0 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E123: (0 0 10 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E126: (0 0 0 0 -10 0 10 0)+(0 -10 0 10 0 -5 0 0)t1
#E127: (-10 0 10 0 0 0 0 0)+(0 5 0 -10 0 10 0 0)t1
#E129: (0 0 0 0 0 0 10 0)+(0 -5 0 10 0 -5 0 5)t1
#E130: (0 0 -10 0 10 0 -10 0)+(0 5 0 -5 0 -5 0 5)t1
#E133: (0 0 0 0 10 0 -10 0)+(0 10 0 -10 0 5 0 0)t1
#E134: (10 0 -10 0 0 0 0 0)+(0 -5 0 10 0 -10 0 0)t1
#E136: (0 0 0 0 0 0 -10 0)+(0 5 0 -10 0 5 0 -5)t1
#E137: (0 0 10 0 -10 0 10 0)+(0 -5 0 5 0 5 0 -5)t1
#E138: (0 0 0 0 -10 0 0 0)+(0 -5 0 0 0 0 0 -5)t1
#E139: (10 0 -10 0 10 0 0 0)+(0 0 0 10 0 -10 0 5)t1
#E140: (0 0 -10 0 10 0 0 0)+(0 0 0 5 0 -10 0 10)t1
#E141: (0 0 0 0 10 0 0 0)+(0 5 0 0 0 0 0 5)t1
#E142: (-10 0 10 0 -10 0 0 0)+(0 0 0 -10 0 10 0 -5)t1
#E143: (0 0 10 0 -10 0 0 0)+(0 0 0 -5 0 10 0 -10)t1
# File =  perturb = 0 rescale = 1
# vertices: 3
## V2(24) (6 PI) V54(30) (14 PI) V72(24) (6 PI) 
# edges: 39
## E4(2) E5(2) E11(2) E17(2) E23(2) E31(2) E37(2) E43(2) E49(2) E55(2) E61(2) E67(2) E73(2) E79(2) E83(2) E89(2) E95(2) E101(2) E107(2) E113(2) E119(2) E120(2) E121(2) E122(2) E123(2) E126(2) E127(2) E129(2) E130(2) E133(2) E134(2) E136(2) E137(2) E138(2) E139(2) E140(2) E141(2) E142(2) E143(2) 
# faces: 26
# genus: 6
# area: 1571.02
sweeping start V2 depth = 79.2722
File =  depth = 79.2722 follow_depth = 396.361 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 96 unusual 0 close 25910 reject 0
V2: AcBbCa V54: DeEdFjGiHhIgJf V72: KlLkMm   cylinders |1|1|1.61803|1.61803|1.64809|1.64809|2.26613|2.63661|
Moduli: |4.82413|4.82413|1.21314|1.21314|1|1|1.31567|1.47151| Min Area: 1 Max Area: 1
    total: 0.654498 0.397887 sym = 6
    raw = 30 short saddle = 10 short cyl = 26.9917 tag #3
V2: AXXXXa V54: BXXXXXCcXXXXXb V72: DdXXXX   cylinders |1|1|
Moduli: |1|1| Min Area: 0.0392163 Max Area: 0.0392163
    total: 1.309 0.795775 sym = 2
    raw = 20 short saddle = 68.1929 short cyl = 68.1929 tag #1

total cylinders: 9.42478 5.72958 raw = 72 rejected = 0
""",
    (2, 8, 6 ,8): r"""
max loc = 1
alpha =  8 beta = 6 gamma = 8 delta =2
a =  2.0944 b = 1.5708 c= 2.0944 d=0.523599
a1_min = -0.523599 a1_max = 1.5708
a1 = 0.90413
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -2.22045e-16
p = 1 q = 1.61716 p-q = 1.2709
p = (1,0) q = (1,1.2709) r = (-1.60063,2.77238)
Imaginary of t_cx = -1.55431e-15
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(-1 0 0 0 1 0 0 0)t1
Checking p_alt (0,0)
Checking q_alt (3.33067e-16,-1.77636e-15)
Checking r_alt (0,-4.44089e-16)
Checking p_algt (0,0)
Checking q_algt (3.33067e-16,-1.77636e-15)
Checking r_algt (0,-4.44089e-16)
Number field: min poly (1 0 0 0 -1 0 0 0 1)
#Field Degree=8 minimal polynomial=(1 0 0 0 -1 0 0 0 1)
#Field generator embedding: (0.965925826289068310.25881904510252074
#Params:1
#t1=3.2012663909678238
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000222044605,1.270901743383348936333732)
E2 ( V2 V3 F0 NULL ) (-2.600633195483911475065497,1.501476275476113286799773)
E3 ( V3 V0 F0 NULL ) (1.600633195483911253020892,-2.772378018859462223133505)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (1/3 0 0 0 -2/3 0 0 0)+(-1/3 0 0 0 2/3 0 0 0)t1
#E2: (-4/3 0 0 0 2/3 0 0 0)+(-2/3 0 0 0 1/3 0 0 0)t1
#E3: (0 0 0 0 0 0 0 0)+(1 0 0 0 -1 0 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.229846, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F25 V54(E5) V5(E31) a=0, can_bisect=0
F25 V54(E5) V10(E11) a=0.90413, can_bisect=1
F25 V54(E5) V34(E4) a=0, can_bisect=0
F25 V5(E31) V10(E11) a=0, can_bisect=0
F25 V5(E31) V34(E4) a=0.666667, can_bisect=1
F25 V10(E11) V34(E4) a=0, can_bisect=0
F25 best_angle = 0.90413
Removing V21
F28 V34(E37) V54(E59) a=0, can_bisect=0
F28 V34(E37) V5(E49) a=0.666667, can_bisect=1
F28 V34(E37) V10(E17) a=0, can_bisect=0
F28 V54(E59) V5(E49) a=0, can_bisect=0
F28 V54(E59) V10(E17) a=0.90413, can_bisect=1
F28 V5(E49) V10(E17) a=0, can_bisect=0
F28 best_angle = 0.90413
Removing V27
F31 V5(E43) V10(E65) a=0, can_bisect=0
F31 V5(E43) V34(E55) a=0.666667, can_bisect=1
F31 V5(E43) V54(E23) a=0, can_bisect=0
F31 V10(E65) V34(E55) a=0, can_bisect=0
F31 V10(E65) V54(E23) a=0.90413, can_bisect=1
F31 V34(E55) V54(E23) a=0, can_bisect=0
F31 best_angle = 0.90413
Removing V32
F34 V5(E59) V54(E55) a=0, can_bisect=0
F34 V5(E59) V34(E11) a=0.380531, can_bisect=1
F34 V5(E59) V10(E49) a=0.436821, can_bisect=1
F34 V5(E59) V5(E23) a=0.90413, can_bisect=1
F34 V5(E59) V54(E4) a=0.937031, can_bisect=1
F34 V5(E59) V34(E17) a=1.42773, can_bisect=1
F34 V5(E59) V10(E43) a=0.937031, can_bisect=1
F34 V5(E59) V5(E5) a=0.90413, can_bisect=1
F34 V5(E59) V54(E37) a=0.436821, can_bisect=1
F34 V5(E59) V34(E65) a=0.380531, can_bisect=1
F34 V5(E59) V10(E31) a=0, can_bisect=0
F34 V54(E55) V34(E11) a=0, can_bisect=0
F34 V54(E55) V10(E49) a=0.143068, can_bisect=1
F34 V54(E55) V5(E23) a=0.436821, can_bisect=1
F34 V54(E55) V54(E4) a=0.666667, can_bisect=1
F34 V54(E55) V34(E17) a=0.937031, can_bisect=1
F34 V54(E55) V10(E43) a=1.19027, can_bisect=1
F34 V54(E55) V5(E5) a=0.937031, can_bisect=1
F34 V54(E55) V54(E37) a=0.666667, can_bisect=1
F34 V54(E55) V34(E65) a=0.436821, can_bisect=1
F34 V54(E55) V10(E31) a=0.143068, can_bisect=1
F34 V34(E11) V10(E49) a=0, can_bisect=0
F34 V34(E11) V5(E23) a=0.380531, can_bisect=1
F34 V34(E11) V54(E4) a=0.436821, can_bisect=1
F34 V34(E11) V34(E17) a=0.90413, can_bisect=1
F34 V34(E11) V10(E43) a=0.937031, can_bisect=1
F34 V34(E11) V5(E5) a=1.42773, can_bisect=1
F34 V34(E11) V54(E37) a=0.937031, can_bisect=1
F34 V34(E11) V34(E65) a=0.90413, can_bisect=1
F34 V34(E11) V10(E31) a=0.436821, can_bisect=1
F34 V10(E49) V5(E23) a=0, can_bisect=0
F34 V10(E49) V54(E4) a=0.143068, can_bisect=1
F34 V10(E49) V34(E17) a=0.436821, can_bisect=1
F34 V10(E49) V10(E43) a=0.666667, can_bisect=1
F34 V10(E49) V5(E5) a=0.937031, can_bisect=1
F34 V10(E49) V54(E37) a=1.19027, can_bisect=1
F34 V10(E49) V34(E65) a=0.937031, can_bisect=1
F34 V10(E49) V10(E31) a=0.666667, can_bisect=1
F34 V5(E23) V54(E4) a=0, can_bisect=0
F34 V5(E23) V34(E17) a=0.380531, can_bisect=1
F34 V5(E23) V10(E43) a=0.436821, can_bisect=1
F34 V5(E23) V5(E5) a=0.90413, can_bisect=1
F34 V5(E23) V54(E37) a=0.937031, can_bisect=1
F34 V5(E23) V34(E65) a=1.42773, can_bisect=1
F34 V5(E23) V10(E31) a=0.937031, can_bisect=1
F34 V54(E4) V34(E17) a=0, can_bisect=0
F34 V54(E4) V10(E43) a=0.143068, can_bisect=1
F34 V54(E4) V5(E5) a=0.436821, can_bisect=1
F34 V54(E4) V54(E37) a=0.666667, can_bisect=1
F34 V54(E4) V34(E65) a=0.937031, can_bisect=1
F34 V54(E4) V10(E31) a=1.19027, can_bisect=1
F34 V34(E17) V10(E43) a=0, can_bisect=0
F34 V34(E17) V5(E5) a=0.380531, can_bisect=1
F34 V34(E17) V54(E37) a=0.436821, can_bisect=1
F34 V34(E17) V34(E65) a=0.90413, can_bisect=1
F34 V34(E17) V10(E31) a=0.937031, can_bisect=1
F34 V10(E43) V5(E5) a=0, can_bisect=0
F34 V10(E43) V54(E37) a=0.143068, can_bisect=1
F34 V10(E43) V34(E65) a=0.436821, can_bisect=1
F34 V10(E43) V10(E31) a=0.666667, can_bisect=1
F34 V5(E5) V54(E37) a=0, can_bisect=0
F34 V5(E5) V34(E65) a=0.380531, can_bisect=1
F34 V5(E5) V10(E31) a=0.436821, can_bisect=1
F34 V54(E37) V34(E65) a=0, can_bisect=0
F34 V54(E37) V10(E31) a=0.143068, can_bisect=1
F34 V34(E65) V10(E31) a=0, can_bisect=0
F34 best_angle = 1.42773
F36 V34(E17) V10(E43) a=0, can_bisect=0
F36 V34(E17) V5(E5) a=0.380531, can_bisect=1
F36 V34(E17) V54(E37) a=0.436821, can_bisect=1
F36 V34(E17) V34(E65) a=0.523599, can_bisect=1
F36 V34(E17) V10(E31) a=0.270364, can_bisect=1
F36 V34(E17) V5(E74) a=0, can_bisect=0
F36 V10(E43) V5(E5) a=0, can_bisect=0
F36 V10(E43) V54(E37) a=0.143068, can_bisect=1
F36 V10(E43) V34(E65) a=0.436821, can_bisect=1
F36 V10(E43) V10(E31) a=0.666667, can_bisect=1
F36 V10(E43) V5(E74) a=0.270364, can_bisect=1
F36 V5(E5) V54(E37) a=0, can_bisect=0
F36 V5(E5) V34(E65) a=0.380531, can_bisect=1
F36 V5(E5) V10(E31) a=0.436821, can_bisect=1
F36 V5(E5) V5(E74) a=0.523599, can_bisect=1
F36 V54(E37) V34(E65) a=0, can_bisect=0
F36 V54(E37) V10(E31) a=0.143068, can_bisect=1
F36 V54(E37) V5(E74) a=0.436821, can_bisect=1
F36 V34(E65) V10(E31) a=0, can_bisect=0
F36 V34(E65) V5(E74) a=0.380531, can_bisect=1
F36 V10(E31) V5(E74) a=0, can_bisect=0
F36 best_angle = 0.666667
F38 V10(E31) V5(E74) a=0, can_bisect=0
F38 V10(E31) V34(E17) a=0.270364, can_bisect=1
F38 V10(E31) V10(E75) a=0, can_bisect=0
F38 V5(E74) V34(E17) a=0, can_bisect=0
F38 V5(E74) V10(E75) a=0.270364, can_bisect=1
F38 V34(E17) V10(E75) a=0, can_bisect=0
F38 best_angle = 0.270364
F37 V10(E43) V5(E5) a=0, can_bisect=0
F37 V10(E43) V54(E37) a=0.143068, can_bisect=1
F37 V10(E43) V34(E65) a=0.229846, can_bisect=1
F37 V10(E43) V10(E75) a=0, can_bisect=0
F37 V5(E5) V54(E37) a=0, can_bisect=0
F37 V5(E5) V34(E65) a=0.380531, can_bisect=1
F37 V5(E5) V10(E75) a=0.229846, can_bisect=1
F37 V54(E37) V34(E65) a=0, can_bisect=0
F37 V54(E37) V10(E75) a=0.143068, can_bisect=1
F37 V34(E65) V10(E75) a=0, can_bisect=0
F37 best_angle = 0.380531
F42 V34(E65) V10(E75) a=0, can_bisect=0
F42 V34(E65) V10(E43) a=0.229846, can_bisect=1
F42 V34(E65) V5(E77) a=0, can_bisect=0
F42 V10(E75) V10(E43) a=0, can_bisect=0
F42 V10(E75) V5(E77) a=0.229846, can_bisect=1
F42 V10(E43) V5(E77) a=0, can_bisect=0
F42 best_angle = 0.229846
F35 V5(E59) V54(E55) a=0, can_bisect=0
F35 V5(E59) V34(E11) a=0.380531, can_bisect=1
F35 V5(E59) V10(E49) a=0.436821, can_bisect=1
F35 V5(E59) V5(E23) a=0.523599, can_bisect=1
F35 V5(E59) V54(E4) a=0.270364, can_bisect=1
F35 V5(E59) V34(E74) a=0, can_bisect=0
F35 V54(E55) V34(E11) a=0, can_bisect=0
F35 V54(E55) V10(E49) a=0.143068, can_bisect=1
F35 V54(E55) V5(E23) a=0.436821, can_bisect=1
F35 V54(E55) V54(E4) a=0.666667, can_bisect=1
F35 V54(E55) V34(E74) a=0.270364, can_bisect=1
F35 V34(E11) V10(E49) a=0, can_bisect=0
F35 V34(E11) V5(E23) a=0.380531, can_bisect=1
F35 V34(E11) V54(E4) a=0.436821, can_bisect=1
F35 V34(E11) V34(E74) a=0.523599, can_bisect=1
F35 V10(E49) V5(E23) a=0, can_bisect=0
F35 V10(E49) V54(E4) a=0.143068, can_bisect=1
F35 V10(E49) V34(E74) a=0.436821, can_bisect=1
F35 V5(E23) V54(E4) a=0, can_bisect=0
F35 V5(E23) V34(E74) a=0.380531, can_bisect=1
F35 V54(E4) V34(E74) a=0, can_bisect=0
F35 best_angle = 0.666667
F46 V54(E4) V34(E74) a=0, can_bisect=0
F46 V54(E4) V5(E59) a=0.270364, can_bisect=1
F46 V54(E4) V54(E79) a=0, can_bisect=0
F46 V34(E74) V5(E59) a=0, can_bisect=0
F46 V34(E74) V54(E79) a=0.270364, can_bisect=1
F46 V5(E59) V54(E79) a=0, can_bisect=0
F46 best_angle = 0.270364
F45 V54(E55) V34(E11) a=0, can_bisect=0
F45 V54(E55) V10(E49) a=0.143068, can_bisect=1
F45 V54(E55) V5(E23) a=0.229846, can_bisect=1
F45 V54(E55) V54(E79) a=0, can_bisect=0
F45 V34(E11) V10(E49) a=0, can_bisect=0
F45 V34(E11) V5(E23) a=0.380531, can_bisect=1
F45 V34(E11) V54(E79) a=0.229846, can_bisect=1
F45 V10(E49) V5(E23) a=0, can_bisect=0
F45 V10(E49) V54(E79) a=0.143068, can_bisect=1
F45 V5(E23) V54(E79) a=0, can_bisect=0
F45 best_angle = 0.380531
F50 V5(E23) V54(E79) a=0, can_bisect=0
F50 V5(E23) V54(E55) a=0.229846, can_bisect=1
F50 V5(E23) V34(E81) a=0, can_bisect=0
F50 V54(E79) V54(E55) a=0, can_bisect=0
F50 V54(E79) V34(E81) a=0.229846, can_bisect=1
F50 V54(E55) V34(E81) a=0, can_bisect=0
F50 best_angle = 0.229846
Extra: Flipping E75
Extra: Flipping E76
Extra: Flipping E78
Extra: Flipping E79
Extra: Flipping E80
Extra: Flipping E82
Longest Edge: 6.00591
Looking for shortest saddle...1.61716
Clearing denominators: multiplying by 3
#Field Degree=8 minimal polynomial=(1 0 0 0 -1 0 0 0 1)
#Field generator embedding: (0.965925826289068310.25881904510252074
#Params:1
#t1=3.2012663909678238
# Vertices: V5 V10 V34 V54
# UEdges: 
E4 ( V54 V34 F64 F27 ) (3.000000000000000888178420,3.812705230150046809001196)
E5 ( V54 V5 F26 F41 ) (2.999999999999998223643161,-3.812705230150046809001196)
E11 ( V10 V34 F27 F49 ) (-2.999999999999998223643161,3.812705230150046809001196)
E17 ( V34 V10 F54 F30 ) (1.801899586451736423597936,4.504428826428338972220899)
E23 ( V5 V54 F64 F33 ) (4.801899586451735757464121,0.691723596278292385264308)
E31 ( V5 V10 F26 F58 ) (3.000000000000000888178420,3.812705230150046809001196)
E37 ( V54 V34 F41 F30 ) (-4.801899586451733981107282,0.691723596278292385264308)
E43 ( V10 V5 F54 F33 ) (-1.801899586451734425196491,4.504428826428338972220899)
E49 ( V10 V5 F49 F29 ) (4.801899586451733981107282,-0.691723596278292385264308)
E55 ( V54 V34 F60 F32 ) (1.801899586451734425196491,-4.504428826428338972220899)
E59 ( V5 V54 F60 F29 ) (-1.801899586451736423597936,-4.504428826428338972220899)
E65 ( V34 V10 F58 F32 ) (-4.801899586451735757464121,-0.691723596278292385264308)
E71 ( V54 V10 F27 F26 ) (5.999999999999998223643161,0.000000000000000000000000)
E72 ( V54 V10 F30 F29 ) (-2.999999999999998223643161,5.196152422706632023619022)
E73 ( V10 V54 F33 F32 ) (3.000000000000001332267630,5.196152422706632023619022)
E74 ( V5 V34 F55 F61 ) (15.603799172903466185857724,-9.008857652856676168084959)
E77 ( V5 V34 F57 F41 ) (-7.801899586451733092928862,4.504428826428338972220899)
E81 ( V34 V5 F63 F49 ) (7.801899586451733092928862,-4.504428826428338972220899)
E83 ( V34 V5 F55 F54 ) (0.000000000000002598554567,9.008857652856677944441799)
E84 ( V5 V5 F55 F57 ) (-15.603799172903471514928242,0.000000000000000000000000)
E85 ( V34 V5 F57 F58 ) (-7.801899586451735757464121,-4.504428826428338972220899)
E86 ( V5 V34 F61 F60 ) (-0.000000000000002598554567,-9.008857652856677944441799)
E87 ( V34 V34 F61 F63 ) (15.603799172903471514928242,0.000000000000000000000000)
E88 ( V5 V34 F63 F64 ) (7.801899586451735757464121,4.504428826428338972220899)
# Faces: 
F26 +E5 +E31 -E71
F27 +E11 -E4 +E71
F29 -E59 -E49 -E72
F30 -E17 -E37 +E72
F32 -E65 -E55 -E73
F33 -E23 -E43 +E73
F41 -E5 +E37 -E77
F49 -E11 +E49 -E81
F54 -E83 +E17 +E43
F55 +E84 +E74 +E83
F57 +E85 -E84 +E77
F58 -E85 +E65 -E31
F60 -E86 +E59 +E55
F61 +E87 -E74 +E86
F63 +E88 -E87 +E81
F64 -E88 +E23 +E4
#UEdges Algebraic 
#E4: (4 0 0 0 -2 0 0 0)+(-1 0 0 0 2 0 0 0)t1
#E5: (2 0 0 0 2 0 0 0)+(1 0 0 0 -2 0 0 0)t1
#E11: (-2 0 0 0 -2 0 0 0)+(-1 0 0 0 2 0 0 0)t1
#E17: (-4 0 0 0 2 0 0 0)+(1 0 0 0 1 0 0 0)t1
#E23: (-2 0 0 0 4 0 0 0)+(2 0 0 0 -1 0 0 0)t1
#E31: (4 0 0 0 -2 0 0 0)+(-1 0 0 0 2 0 0 0)t1
#E37: (-2 0 0 0 4 0 0 0)+(-1 0 0 0 -1 0 0 0)t1
#E43: (2 0 0 0 2 0 0 0)+(-2 0 0 0 1 0 0 0)t1
#E49: (2 0 0 0 -4 0 0 0)+(1 0 0 0 1 0 0 0)t1
#E55: (-2 0 0 0 -2 0 0 0)+(2 0 0 0 -1 0 0 0)t1
#E59: (4 0 0 0 -2 0 0 0)+(-1 0 0 0 -1 0 0 0)t1
#E65: (2 0 0 0 -4 0 0 0)+(-2 0 0 0 1 0 0 0)t1
#E71: (6 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E72: (-6 0 0 0 6 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E73: (0 0 0 0 6 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E74: (8 0 0 0 -4 0 0 0)+(4 0 0 0 -2 0 0 0)t1
#E77: (-4 0 0 0 2 0 0 0)+(-2 0 0 0 1 0 0 0)t1
#E81: (4 0 0 0 -2 0 0 0)+(2 0 0 0 -1 0 0 0)t1
#E83: (-2 0 0 0 4 0 0 0)+(-1 0 0 0 2 0 0 0)t1
#E84: (-6 0 0 0 0 0 0 0)+(-3 0 0 0 0 0 0 0)t1
#E85: (-2 0 0 0 -2 0 0 0)+(-1 0 0 0 -1 0 0 0)t1
#E86: (2 0 0 0 -4 0 0 0)+(1 0 0 0 -2 0 0 0)t1
#E87: (6 0 0 0 0 0 0 0)+(3 0 0 0 0 0 0 0)t1
#E88: (2 0 0 0 2 0 0 0)+(1 0 0 0 1 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 4
## V5(15) (4 PI) V10(9) (4 PI) V34(15) (4 PI) V54(9) (4 PI) 
# edges: 24
## E4(2) E5(2) E11(2) E17(2) E23(2) E31(2) E37(2) E43(2) E49(2) E55(2) E59(2) E65(2) E71(2) E72(2) E73(2) E74(2) E77(2) E81(2) E83(2) E84(2) E85(2) E86(2) E87(2) E88(2) 
# faces: 16
# genus: 3
# area: 328.186
sweeping start V5 depth = 60.3864
File =  depth = 60.3864 follow_depth = 301.932 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 32 unusual 0 close 18167 reject 0
V5: AaXX V10: BXXb V34: CXXc V54: DdXX   cylinders |1|1|
Moduli: |1|1| Min Area: 0.114117 Max Area: 0.369889
    total: 1.41372 0.859437 sym = 2
    raw = 60 short saddle = 31.1407 short cyl = 31.1407 tag #3
V5: AbBa V10: CXXc V34: DdEe V54: FfXX   cylinders |1|1|2|2|
Moduli: |2.5741|2.5741|1|1| Min Area: 0.16799 Max Area: 0.16799
    total: 0.282743 0.171887 sym = 4
    raw = 24 short saddle = 46.3654 short cyl = 46.3654 tag #8
V5: AfBe V34: aEbF V10: CdDc V54: GgHh   cylinders |1|1|1.62475|1.62475|2.62475|
Moduli: |2.70507|2.70507|1|1|1.0306| Min Area: 1 Max Area: 1
    total: 0.153153 0.0931056 sym = 4
    raw = 13 short saddle = 7.62541 short cyl = 16.6343 tag #0
V5: AbBa V10: ChDg V54: cGdH V34: EeFf   cylinders |1|1|1.84643|1.84643|2.84643|
Moduli: |10.1773|10.1773|1|1|3.57547| Min Area: 1 Max Area: 1
    total: 0.141372 0.0859437 sym = 4
    raw = 12 short saddle = 6 short cyl = 15.6038 tag #9
V5: AdXX V54: aXXD V10: BXXc V34: bCXX   cylinders |1|
Moduli: |1| Min Area: 0.209115 Max Area: 0.209115
    total: 0.282743 0.171887 sym = 2
    raw = 12 short saddle = 13.1679 short cyl = 39.5036 tag #10

total cylinders: 6.07898 3.69558 raw = 129 rejected = 0
""",
    (3, 6, 3, 6): r"""
max loc = 1
alpha =  6 beta = 3 gamma = 6 delta =3
a =  2.0944 b = 1.0472 c= 2.0944 d=1.0472
a1_min = 8.88178e-16 a1_max = 2.0944
a1 = 1.42773
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -4.44089e-16
p = 1 q = 1.4005 p-q = 1.60063
p = (1,0) q = (0.199683,1.38619) r = (-0.800317,1.38619)
Imaginary of t_cx = -4.44089e-16
p_algt = (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0)+(-1 0 0 1 0 0)t1
Checking p_alt (0,0)
Checking q_alt (8.04912e-16,-4.44089e-16)
Checking r_alt (-2.22045e-16,0)
Checking p_algt (0,0)
Checking q_algt (8.04912e-16,-4.44089e-16)
Checking r_algt (-2.22045e-16,0)
Number field: min poly (1 0 0 -1 0 0 1)
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=1.6006331954839126
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.800316597741956070599656,1.386189009429731777700567)
E2 ( V2 V3 F0 NULL ) (-1.000000000000000000000000,0.000000000000000000000000)
E3 ( V3 V0 F0 NULL ) (0.800316597741956070599656,-1.386189009429731777700567)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0)+(0 0 0 0 0 0)t1
#E1: (0 0 0 0 0 0)+(-1 0 0 1 0 0)t1
#E2: (-1 0 0 0 0 0)+(0 0 0 0 0 0)t1
#E3: (0 0 0 0 0 0)+(1 0 0 -1 0 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.666667, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.391038, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.666667
In RetriangulateSurface
Removing V1
F13 V16(E5) V24(E19) a=0, can_bisect=0
F13 V16(E5) V16(E29) a=0.90413, can_bisect=1
F13 V16(E5) V24(E25) a=0.666667, can_bisect=1
F13 V16(E5) V16(E11) a=0.90413, can_bisect=1
F13 V16(E5) V24(E4) a=0, can_bisect=0
F13 V24(E19) V16(E29) a=0, can_bisect=0
F13 V24(E19) V24(E25) a=0.143068, can_bisect=1
F13 V24(E19) V16(E11) a=0.666667, can_bisect=1
F13 V24(E19) V24(E4) a=0.143068, can_bisect=1
F13 V16(E29) V24(E25) a=0, can_bisect=0
F13 V16(E29) V16(E11) a=0.90413, can_bisect=1
F13 V16(E29) V24(E4) a=0.666667, can_bisect=1
F13 V24(E25) V16(E11) a=0, can_bisect=0
F13 V24(E25) V24(E4) a=0.143068, can_bisect=1
F13 V16(E11) V24(E4) a=0, can_bisect=0
F13 best_angle = 0.90413
F15 V16(E29) V24(E25) a=0, can_bisect=0
F15 V16(E29) V16(E11) a=0.90413, can_bisect=1
F15 V16(E29) V24(E4) a=0.523599, can_bisect=1
F15 V16(E29) V16(E35) a=0, can_bisect=0
F15 V24(E25) V16(E11) a=0, can_bisect=0
F15 V24(E25) V24(E4) a=0.143068, can_bisect=1
F15 V24(E25) V16(E35) a=0.523599, can_bisect=1
F15 V16(E11) V24(E4) a=0, can_bisect=0
F15 V16(E11) V16(E35) a=0.90413, can_bisect=1
F15 V24(E4) V16(E35) a=0, can_bisect=0
F15 best_angle = 0.90413
F17 V16(E11) V24(E4) a=0, can_bisect=0
F17 V16(E11) V16(E35) a=0.90413, can_bisect=1
F17 V16(E11) V16(E36) a=0, can_bisect=0
F17 V24(E4) V16(E35) a=0, can_bisect=0
F17 V24(E4) V16(E36) a=0.523599, can_bisect=1
F17 V16(E35) V16(E36) a=0, can_bisect=0
F17 best_angle = 0.90413
Removing V3
F20 V24(E5) V16(E19) a=0, can_bisect=0
F20 V24(E5) V24(E29) a=0.90413, can_bisect=1
F20 V24(E5) V16(E25) a=0.666667, can_bisect=1
F20 V24(E5) V24(E11) a=0.90413, can_bisect=1
F20 V24(E5) V16(E4) a=0, can_bisect=0
F20 V16(E19) V24(E29) a=0, can_bisect=0
F20 V16(E19) V16(E25) a=0.143068, can_bisect=1
F20 V16(E19) V24(E11) a=0.666667, can_bisect=1
F20 V16(E19) V16(E4) a=0.143068, can_bisect=1
F20 V24(E29) V16(E25) a=0, can_bisect=0
F20 V24(E29) V24(E11) a=0.90413, can_bisect=1
F20 V24(E29) V16(E4) a=0.666667, can_bisect=1
F20 V16(E25) V24(E11) a=0, can_bisect=0
F20 V16(E25) V16(E4) a=0.143068, can_bisect=1
F20 V24(E11) V16(E4) a=0, can_bisect=0
F20 best_angle = 0.90413
F22 V24(E29) V16(E25) a=0, can_bisect=0
F22 V24(E29) V24(E11) a=0.90413, can_bisect=1
F22 V24(E29) V16(E4) a=0.523599, can_bisect=1
F22 V24(E29) V24(E38) a=0, can_bisect=0
F22 V16(E25) V24(E11) a=0, can_bisect=0
F22 V16(E25) V16(E4) a=0.143068, can_bisect=1
F22 V16(E25) V24(E38) a=0.523599, can_bisect=1
F22 V24(E11) V16(E4) a=0, can_bisect=0
F22 V24(E11) V24(E38) a=0.90413, can_bisect=1
F22 V16(E4) V24(E38) a=0, can_bisect=0
F22 best_angle = 0.90413
F24 V24(E11) V16(E4) a=0, can_bisect=0
F24 V24(E11) V24(E38) a=0.90413, can_bisect=1
F24 V24(E11) V24(E39) a=0, can_bisect=0
F24 V16(E4) V24(E38) a=0, can_bisect=0
F24 V16(E4) V24(E39) a=0.523599, can_bisect=1
F24 V24(E38) V24(E39) a=0, can_bisect=0
F24 best_angle = 0.90413
Longest Edge: 1.73205
Looking for shortest saddle...1.4005
Clearing denominators: multiplying by 1
#Field Degree=6 minimal polynomial=(1 0 0 -1 0 0 1)
#Field generator embedding: (0.939692620785908430.34202014332566871
#Params:1
#t1=1.6006331954839126
# Vertices: V16 V24
# UEdges: 
E4 ( V16 V24 F25 F18 ) (0.199683402258043929400344,1.386189009429731777700567)
E5 ( V16 V24 F14 F21 ) (0.199683402258043485311134,-1.386189009429731777700567)
E11 ( V16 V24 F18 F25 ) (-1.300316597741956181621958,0.520163605645293070089963)
E19 ( V24 V16 F14 F21 ) (1.300316597741956403666563,0.520163605645293070089963)
E25 ( V24 V16 F16 F23 ) (-1.100633195483912363243917,0.866025403784438707610605)
E29 ( V16 V24 F16 F23 ) (1.100633195483912807333127,0.866025403784438707610605)
E35 ( V16 V16 F19 F14 ) (1.500000000000000000000000,-0.866025403784438707610605)
E36 ( V16 V16 F19 F16 ) (0.000000000000000444089210,1.732050807568877415221209)
E37 ( V16 V16 F19 F18 ) (-1.500000000000000000000000,-0.866025403784438707610605)
E38 ( V24 V24 F26 F21 ) (-1.500000000000000000000000,0.866025403784438707610605)
E39 ( V24 V24 F26 F23 ) (-0.000000000000000444089210,-1.732050807568877415221209)
E40 ( V24 V24 F26 F25 ) (1.500000000000000000000000,0.866025403784438707610605)
# Faces: 
F14 +E5 +E19 -E35
F16 +E29 +E25 -E36
F18 +E11 -E4 -E37
F19 +E35 +E36 +E37
F21 -E5 -E19 -E38
F23 -E29 -E25 -E39
F25 -E11 +E4 -E40
F26 +E38 +E39 +E40
#UEdges Algebraic 
#E4: (1 0 0 0 0 0)+(-1 0 0 1 0 0)t1
#E5: (1 0 0 0 0 0)+(0 0 0 -1 0 0)t1
#E11: (0 0 0 -1 0 0)+(-1 0 0 1 0 0)t1
#E19: (1 0 0 -1 0 0)+(0 0 0 1 0 0)t1
#E25: (0 0 0 1 0 0)+(-1 0 0 0 0 0)t1
#E29: (-1 0 0 1 0 0)+(1 0 0 0 0 0)t1
#E35: (2 0 0 -1 0 0)+(0 0 0 0 0 0)t1
#E36: (-1 0 0 2 0 0)+(0 0 0 0 0 0)t1
#E37: (-1 0 0 -1 0 0)+(0 0 0 0 0 0)t1
#E38: (-2 0 0 1 0 0)+(0 0 0 0 0 0)t1
#E39: (1 0 0 -2 0 0)+(0 0 0 0 0 0)t1
#E40: (1 0 0 1 0 0)+(0 0 0 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 2
## V16(12) (4 PI) V24(12) (4 PI) 
# edges: 12
## E4(2) E5(2) E11(2) E19(2) E25(2) E29(2) E35(2) E36(2) E37(2) E38(2) E39(2) E40(2) 
# faces: 8
# genus: 2
# area: 8.31713
sweeping start V16 depth = 28.8394
File =  depth = 28.8394 follow_depth = 144.197 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 10 unusual 0 close 32824 reject 0
V16: AaXX V24: BXXb   cylinders |1|
Moduli: |1| Min Area: 0.0120598 Max Area: 0.755899
    total: 2.1677 1.3178 sym = 2
    raw = 828 short saddle = 3.93548 short cyl = 3.93548 tag #1
V16: AXBc V24: aCbX   cylinders |1|
Moduli: |1| Min Area: 0.285685 Max Area: 0.44069
    total: 0.0837758 0.0509296 sym = 3
    raw = 48 short saddle = 4.19108 short cyl = 12.5733 tag #3
V16: AaBb V24: CdDc   cylinders |1|1.58063|2.58063|
Moduli: |1.50719|1|1.19654| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 28.4178 short cyl = 28.4178 tag #4
V16: AaXc V24: BXCb   cylinders |1|
Moduli: |1| Min Area: 0.285685 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #5
V16: AbBa V24: CcDd   cylinders |1|1.60063|2.60063|
Moduli: |8.82252|1|4.00793| Min Area: 1 Max Area: 1
    total: 0.015708 0.0095493 sym = 4
    raw = 12 short saddle = 1.73205 short cyl = 1.73205 tag #6
V16: AXBb V24: aXcC   cylinders |1|
Moduli: |1| Min Area: 0.13068 Max Area: 0.285685
    total: 0.020944 0.0127324 sym = 3
    raw = 12 short saddle = 8.58771 short cyl = 25.7631 tag #7

total cylinders: 2.38761 1.45149 raw = 456 rejected = 0
""",
    (3, 6, 5, 6): r"""
max loc = 1
alpha =  6 beta = 5 gamma = 6 delta =3
a =  1.88496 b = 1.5708 c= 1.88496 d=0.942478
a1_min = -0.314159 a1_max = 1.5708
a1 = 0.970796
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: 3.33067e-16
p = 1 q = 1.77103 p-q = 1.4617
p = (1,0) q = (1,1.4617) r = (-0.649026,1.9975)
Imaginary of t_cx = -2.22045e-16
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 1 0)t1
Checking p_alt (0,0)
Checking q_alt (3.33067e-16,-1.11022e-15)
Checking r_alt (8.88178e-16,-1.33227e-15)
Checking p_algt (0,0)
Checking q_algt (3.33067e-16,-1.11022e-15)
Checking r_algt (8.88178e-16,-1.33227e-15)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=2.1002926533064792
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (0.000000000000000222044605,1.461695947078100576987936)
E2 ( V2 V3 F0 NULL ) (-1.649026123032551360836351,0.535801066975863493624388)
E3 ( V3 V0 F0 NULL ) (0.649026123032551027769443,-1.997497014053964070612324)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (3/5 0 -6/5 0 4/5 0 -2/5 0)+(-2/5 0 4/5 0 -1/5 0 3/5 0)t1
#E2: (-8/5 0 6/5 0 -4/5 0 2/5 0)+(2/5 0 -4/5 0 1/5 0 2/5 0)t1
#E3: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 -1 0)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.6, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.375966, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.6
In RetriangulateSurface
Removing V1
F41 V54(E5) V58(E31) a=0, can_bisect=0
F41 V54(E5) V42(E11) a=0.970796, can_bisect=1
F41 V54(E5) V88(E4) a=0, can_bisect=0
F41 V58(E31) V42(E11) a=0, can_bisect=0
F41 V58(E31) V88(E4) a=0.6, can_bisect=1
F41 V42(E11) V88(E4) a=0, can_bisect=0
F41 best_angle = 0.970796
Removing V21
F44 V88(E37) V54(E71) a=0, can_bisect=0
F44 V88(E37) V58(E49) a=0.6, can_bisect=1
F44 V88(E37) V42(E17) a=0, can_bisect=0
F44 V54(E71) V58(E49) a=0, can_bisect=0
F44 V54(E71) V42(E17) a=0.970796, can_bisect=1
F44 V58(E49) V42(E17) a=0, can_bisect=0
F44 best_angle = 0.970796
Removing V27
F47 V58(E43) V42(E77) a=0, can_bisect=0
F47 V58(E43) V88(E55) a=0.6, can_bisect=1
F47 V58(E43) V54(E23) a=0, can_bisect=0
F47 V42(E77) V88(E55) a=0, can_bisect=0
F47 V42(E77) V54(E23) a=0.970796, can_bisect=1
F47 V88(E55) V54(E23) a=0, can_bisect=0
F47 best_angle = 0.970796
Removing V62
F50 V42(E83) V88(E109) a=0, can_bisect=0
F50 V42(E83) V54(E95) a=0.970796, can_bisect=1
F50 V42(E83) V58(E61) a=0, can_bisect=0
F50 V88(E109) V54(E95) a=0, can_bisect=0
F50 V88(E109) V58(E61) a=0.6, can_bisect=1
F50 V54(E95) V58(E61) a=0, can_bisect=0
F50 best_angle = 0.970796
Removing V68
F53 V42(E89) V88(E115) a=0, can_bisect=0
F53 V42(E89) V54(E101) a=0.970796, can_bisect=1
F53 V42(E89) V58(E67) a=0, can_bisect=0
F53 V88(E115) V54(E101) a=0, can_bisect=0
F53 V88(E115) V58(E67) a=0.6, can_bisect=1
F53 V54(E101) V58(E67) a=0, can_bisect=0
F53 best_angle = 0.970796
Longest Edge: 2.10029
Looking for shortest saddle...1.73389
Clearing denominators: multiplying by 5
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=2.1002926533064792
# Vertices: V32 V42 V54 V58 V88
# UEdges: 
E2 ( V88 V32 F2 F7 ) (-8.245130615162757692360174,2.679005334879317246077335)
E3 ( V32 V54 F2 F9 ) (3.245130615162755027824915,-9.987485070269819686927804)
E4 ( V54 V88 F2 F43 ) (5.000000000000000888178420,7.308479735390502440850469)
E5 ( V54 V58 F42 F4 ) (4.999999999999999111821580,-7.308479735390502440850469)
E9 ( V54 V32 F4 F13 ) (-3.245130615162758580538593,-9.987485070269821463284643)
E10 ( V32 V58 F4 F15 ) (8.245130615162757692360174,2.679005334879318578344964)
E11 ( V42 V88 F43 F6 ) (-4.999999999999999111821580,7.308479735390502440850469)
E15 ( V42 V32 F6 F17 ) (3.245130615162758580538593,9.987485070269821463284643)
E16 ( V32 V88 F6 F19 ) (-8.245130615162757692360174,-2.679005334879318578344964)
E17 ( V88 V42 F7 F46 ) (0.250731633266199627207982,8.851610570438200653597960)
E18 ( V42 V32 F7 F21 ) (-8.495862248428956320367433,-6.172605235558882519342205)
E23 ( V58 V54 F9 F49 ) (8.340901577015669587922275,-2.973758047513468838474182)
E25 ( V32 V58 F9 F23 ) (-5.095770961852914560097361,-7.013727022756350848453621)
E31 ( V58 V42 F42 F12 ) (5.000000000000000888178420,7.308479735390502440850469)
E32 ( V58 V32 F12 F25 ) (8.245130615162755916003334,-2.679005334879318578344964)
E33 ( V32 V42 F12 F27 ) (-3.245130615162755027824915,9.987485070269819686927804)
E35 ( V88 V32 F13 F29 ) (5.095770961852911895562102,-7.013727022756350848453621)
E37 ( V54 V88 F13 F46 ) (-8.340901577015671364279115,-2.973758047513470170741812)
E42 ( V32 V42 F15 F31 ) (8.495862248428954544010594,-6.172605235558881631163786)
E43 ( V42 V58 F15 F49 ) (-0.250731633266196851650420,8.851610570438200653597960)
E47 ( V58 V32 F17 F33 ) (-5.095770961852911895562102,7.013727022756350848453621)
E49 ( V42 V58 F17 F45 ) (8.340901577015671364279115,2.973758047513470170741812)
E54 ( V32 V54 F19 F35 ) (-8.495862248428954544010594,6.172605235558881631163786)
E55 ( V54 V88 F19 F48 ) (0.250731633266196851650420,-8.851610570438200653597960)
E59 ( V58 V32 F21 F35 ) (-0.000000000000001033874885,-8.669443375754063652038894)
E61 ( V42 V58 F21 F52 ) (-8.495862248428954544010594,2.496838140195181132696689)
E66 ( V32 V42 F23 F29 ) (-10.501463266532395479657680,-0.000000000000000000000000)
E67 ( V42 V58 F23 F55 ) (5.405692304679481807738739,-7.013727022756350848453621)
E71 ( V58 V54 F25 F45 ) (-0.250731633266199627207982,-8.851610570438200653597960)
E72 ( V54 V32 F25 F37 ) (8.495862248428956320367433,6.172605235558882519342205)
E77 ( V88 V42 F27 F48 ) (-8.340901577015669587922275,2.973758047513468838474182)
E79 ( V32 V88 F27 F39 ) (5.095770961852914560097361,7.013727022756350848453621)
E83 ( V88 V42 F29 F51 ) (-5.405692304679483584095578,-7.013727022756350848453621)
E89 ( V88 V42 F31 F54 ) (8.495862248428956320367433,2.496838140195181576785899)
E91 ( V32 V88 F31 F37 ) (-0.000000000000001033874885,-8.669443375754063652038894)
E95 ( V58 V54 F33 F52 ) (5.405692304679483584095578,7.013727022756350848453621)
E96 ( V54 V32 F33 F39 ) (-10.501463266532395479657680,-0.000000000000000000000000)
E101 ( V58 V54 F35 F55 ) (-8.495862248428956320367433,-2.496838140195181576785899)
E109 ( V54 V88 F37 F51 ) (8.495862248428954544010594,-2.496838140195181132696689)
E115 ( V54 V88 F39 F54 ) (-5.405692304679481807738739,7.013727022756350848453621)
E119 ( V54 V42 F43 F42 ) (10.000000000000000000000000,0.000000000000000000000000)
E120 ( V54 V42 F46 F45 ) (-8.090169943749472736271855,5.877852522924729150588519)
E121 ( V42 V54 F49 F48 ) (8.090169943749472736271855,5.877852522924731815123778)
E122 ( V42 V54 F52 F51 ) (-3.090169943749470959915016,9.510565162951532869328730)
E123 ( V42 V54 F55 F54 ) (-3.090169943749474512628694,-9.510565162951532869328730)
# Faces: 
F2 +E2 +E3 +E4
F4 -E5 +E9 +E10
F6 -E11 +E15 +E16
F7 +E17 +E18 -E2
F9 +E23 -E3 +E25
F12 +E32 +E33 -E31
F13 +E35 -E9 +E37
F15 -E10 +E42 +E43
F17 +E47 -E15 +E49
F19 -E16 +E54 +E55
F21 +E59 -E18 +E61
F23 -E25 +E66 +E67
F25 +E71 +E72 -E32
F27 +E77 -E33 +E79
F29 +E83 -E66 -E35
F31 +E89 -E42 +E91
F33 +E95 +E96 -E47
F35 +E101 -E54 -E59
F37 -E91 -E72 +E109
F39 -E79 -E96 +E115
F42 +E5 +E31 -E119
F43 +E11 -E4 +E119
F45 -E71 -E49 -E120
F46 -E17 -E37 +E120
F48 -E77 -E55 -E121
F49 -E23 -E43 +E121
F51 -E83 -E109 -E122
F52 -E95 -E61 +E122
F54 -E89 -E115 -E123
F55 -E101 -E67 +E123
#UEdges Algebraic 
#E2: (-8 0 6 0 -4 0 2 0)+(2 0 -4 0 1 0 2 0)t1
#E3: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 -5 0)t1
#E4: (8 0 -6 0 4 0 -2 0)+(-2 0 4 0 -1 0 3 0)t1
#E5: (2 0 6 0 -4 0 2 0)+(2 0 -4 0 1 0 -3 0)t1
#E9: (0 0 0 0 0 0 0 0)+(0 0 0 0 -5 0 0 0)t1
#E10: (2 0 6 0 -4 0 2 0)+(2 0 -4 0 6 0 -3 0)t1
#E11: (-2 0 -6 0 4 0 -2 0)+(-2 0 4 0 -1 0 3 0)t1
#E15: (0 0 0 0 0 0 0 0)+(0 0 0 0 5 0 0 0)t1
#E16: (-2 0 -6 0 4 0 -2 0)+(-2 0 4 0 -6 0 3 0)t1
#E17: (-8 0 6 0 -4 0 2 0)+(2 0 1 0 1 0 2 0)t1
#E18: (0 0 0 0 0 0 0 0)+(0 0 -5 0 0 0 0 0)t1
#E23: (-2 0 4 0 4 0 -2 0)+(3 0 -1 0 -1 0 -2 0)t1
#E25: (2 0 -4 0 -4 0 2 0)+(-3 0 1 0 1 0 -3 0)t1
#E31: (8 0 -6 0 4 0 -2 0)+(-2 0 4 0 -1 0 3 0)t1
#E32: (8 0 -6 0 4 0 -2 0)+(-2 0 4 0 -1 0 -2 0)t1
#E33: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 5 0)t1
#E35: (2 0 -4 0 6 0 -8 0)+(2 0 1 0 -4 0 2 0)t1
#E37: (-2 0 4 0 -6 0 8 0)+(-2 0 -1 0 -1 0 -2 0)t1
#E42: (0 0 0 0 0 0 0 0)+(5 0 -5 0 5 0 -5 0)t1
#E43: (2 0 6 0 -4 0 2 0)+(-3 0 1 0 1 0 2 0)t1
#E47: (-2 0 4 0 -6 0 8 0)+(-2 0 -1 0 4 0 -2 0)t1
#E49: (2 0 -4 0 6 0 -8 0)+(2 0 1 0 1 0 2 0)t1
#E54: (0 0 0 0 0 0 0 0)+(-5 0 5 0 -5 0 5 0)t1
#E55: (-2 0 -6 0 4 0 -2 0)+(3 0 -1 0 -1 0 -2 0)t1
#E59: (-2 0 4 0 -6 0 -2 0)+(3 0 -6 0 4 0 -2 0)t1
#E61: (2 0 -4 0 6 0 2 0)+(-3 0 1 0 -4 0 2 0)t1
#E66: (0 0 0 0 0 0 0 0)+(-5 0 0 0 0 0 0 0)t1
#E67: (2 0 -4 0 -4 0 2 0)+(2 0 1 0 1 0 -3 0)t1
#E71: (8 0 -6 0 4 0 -2 0)+(-2 0 -1 0 -1 0 -2 0)t1
#E72: (0 0 0 0 0 0 0 0)+(0 0 5 0 0 0 0 0)t1
#E77: (2 0 -4 0 -4 0 2 0)+(-3 0 1 0 1 0 2 0)t1
#E79: (-2 0 4 0 4 0 -2 0)+(3 0 -1 0 -1 0 3 0)t1
#E83: (2 0 -4 0 6 0 -8 0)+(-3 0 1 0 -4 0 2 0)t1
#E89: (2 0 -4 0 6 0 2 0)+(2 0 1 0 1 0 -3 0)t1
#E91: (-2 0 4 0 -6 0 -2 0)+(3 0 -6 0 4 0 -2 0)t1
#E95: (-2 0 4 0 -6 0 8 0)+(3 0 -1 0 4 0 -2 0)t1
#E96: (0 0 0 0 0 0 0 0)+(-5 0 0 0 0 0 0 0)t1
#E101: (-2 0 4 0 -6 0 -2 0)+(-2 0 -1 0 -1 0 3 0)t1
#E109: (-2 0 4 0 -6 0 -2 0)+(3 0 -1 0 4 0 -2 0)t1
#E115: (-2 0 4 0 4 0 -2 0)+(-2 0 -1 0 -1 0 3 0)t1
#E119: (10 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E120: (-10 0 10 0 -10 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E121: (0 0 10 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E122: (0 0 0 0 0 0 10 0)+(0 0 0 0 0 0 0 0)t1
#E123: (0 0 0 0 -10 0 0 0)+(0 0 0 0 0 0 0 0)t1
# File =  perturb = 0 rescale = 1
# vertices: 5
## V32(20) (6 PI) V42(20) (6 PI) V54(20) (6 PI) V58(15) (6 PI) V88(15) (6 PI) 
# edges: 45
## E2(2) E3(2) E4(2) E5(2) E9(2) E10(2) E11(2) E15(2) E16(2) E17(2) E18(2) E23(2) E25(2) E31(2) E32(2) E33(2) E35(2) E37(2) E42(2) E43(2) E47(2) E49(2) E54(2) E55(2) E59(2) E61(2) E66(2) E67(2) E71(2) E72(2) E77(2) E79(2) E83(2) E89(2) E91(2) E95(2) E96(2) E101(2) E109(2) E115(2) E119(2) E120(2) E121(2) E122(2) E123(2) 
# faces: 30
# genus: 6
# area: 1101.97
sweeping start V32 depth = 66.3918
File =  depth = 66.3918 follow_depth = 331.959 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 156 unusual 0 close 50337 reject 0
V32: AXXaXX V58: BXXXXb V88: CcXXXX   cylinders |1|
Moduli: |1| Min Area: 0.0343377 Max Area: 0.0343377
    total: 1.309 0.795775 sym = 2
    raw = 20 short saddle = 25.9023 short cyl = 41.9108 tag #6
V32: AbBnCc V58: aKmLoJ V42: DeEdFf V54: GiHhIg V88: jOlMkN   cylinders |1|1|1.0071|1.61803|1.61803|1.61803|1.61803|1.62952|
Moduli: |13.9426|13.9426|10.2519|12.9426|1|12.9426|1|10.2519| Min Area: 1 Max Area: 1
    total: 0.327249 0.198944 sym = 6
    raw = 15 short saddle = 8.66944 short cyl = 51.3413 tag #0
V32: AaBcCg V42: bDiEhF V54: dGfHeI V58: JkKjLl V88: MoNnOm   cylinders |1|1|1.16195|1.88007|1.88007|1.88007|2.49811|2.49811|
Moduli: |4.04203|4.04203|69.3701|18.2119|18.2119|30.2888|1|1| Min Area: 1 Max Area: 1
    total: 0.327249 0.198944 sym = 6
    raw = 15 short saddle = 10 short cyl = 43.1721 tag #4
V32: AlBbCa V88: cOkMjN V42: DfEeFd V54: GgHiIh V58: JmKoLn   cylinders |1|1|1.0071|1.61803|1.61803|1.61803|1.61803|1.62952|
Moduli: |13.9426|13.9426|10.2519|12.9426|12.9426|1|1|10.2519| Min Area: 1 Max Area: 1
    total: 0.327249 0.198944 sym = 6
    raw = 15 short saddle = 8.66944 short cyl = 51.3413 tag #7
V32: AbBdCc V54: aHfIeG V42: DgEiFh V58: JkKjLl V88: MmNoOn   cylinders |1|1|1.16195|1.88007|1.88007|1.88007|2.49811|2.49811|
Moduli: |4.04203|4.04203|69.3701|18.2119|30.2888|18.2119|1|1| Min Area: 1 Max Area: 1
    total: 0.261799 0.159155 sym = 6
    raw = 12 short saddle = 10 short cyl = 43.1721 tag #9
V32: AbBeCc V54: aHdIfG V42: DhEgFi V58: JkKjLl V88: MoNnOm   cylinders |1|1|1.16195|1.88007|1.88007|1.88007|2.49811|2.49811|
Moduli: |4.04203|4.04203|69.3701|30.2888|18.2119|18.2119|1|1| Min Area: 1 Max Area: 1
    total: 0.0654498 0.0397887 sym = 6
    raw = 3 short saddle = 10 short cyl = 43.1721 tag #11

total cylinders: 11.781 7.16197 raw = 90 rejected = 0
""",
    (3, 7, 4, 6): r"""
max loc = 1
alpha =  7 beta = 4 gamma = 6 delta =3
a =  2.19911 b = 1.25664 c= 1.88496 d=0.942478
a1_min = 0 a1_max = 1.88496
a1 = 1.28496
angle a check: 0
angle b check: 0
angle c check: 3.14159
angle d check: -1.11022e-16
p = 1 q = 1.68435 p-q = 1.69917
p = (1,0) q = (0.474927,1.61601) r = (-1.1741,1.61601)
Imaginary of t_cx = -5.55112e-16
p_algt = (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
r_algt = (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 1)t1
Checking p_alt (0,0)
Checking q_alt (7.21645e-16,-6.66134e-16)
Checking r_alt (8.88178e-16,-8.88178e-16)
Checking p_algt (0,0)
Checking q_algt (7.21645e-16,-6.66134e-16)
Checking r_algt (8.88178e-16,-8.88178e-16)
Number field: min poly (1 0 -1 0 1 0 -1 0 1)
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=1.9974970140539654
# Vertices: V0 V1 V2 V3
# UEdges: 
E0 ( V0 V1 F0 NULL ) (1.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V2 F0 NULL ) (-0.525073163326619352098135,1.616009030582870487080527)
E2 ( V2 V3 F0 NULL ) (-1.649026123032551360836351,0.000000000000000222044605)
E3 ( V3 V0 F0 NULL ) (1.174099286359170823956788,-1.616009030582870709125132)
# Faces: 
F0 +E0 +E1 +E2 +E3
#UEdges Algebraic 
#E0: (1 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (0 0 0 0 0 0 0 0)+(0 -2/5 0 4/5 0 -1/5 0 3/5)t1
#E2: (-1 0 0 0 0 0 0 0)+(0 2/5 0 -4/5 0 1/5 0 2/5)t1
#E3: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 -1)t1
F0 V0(E0) V1(E1) a=0, can_bisect=0
F0 V0(E0) V2(E2) a=0.6, can_bisect=1
F0 V0(E0) V3(E3) a=0, can_bisect=0
F0 V1(E1) V2(E2) a=0, can_bisect=0
F0 V1(E1) V3(E3) a=0.303278, can_bisect=1
F0 V2(E2) V3(E3) a=0, can_bisect=0
F0 best_angle = 0.6
In RetriangulateSurface
Longest Edge: 1.9975
Looking for shortest saddle...1
Clearing denominators: multiplying by 5
#Field Degree=8 minimal polynomial=(1 0 -1 0 1 0 -1 0 1)
#Field generator embedding: (0.951056516295153530.3090169943749474
#Params:1
#t1=1.9974970140539654
# Vertices: V1 V36 V40 V44 V52 V87
# UEdges: 
E0 ( V52 V1 F1 F3 ) (5.000000000000000000000000,0.000000000000000000000000)
E1 ( V1 V36 F1 F5 ) (-2.625365816633096649468371,8.080045152914351547224214)
E2 ( V36 V87 F2 F4 ) (-8.245130615162757692360174,0.000000000000001110223025)
E3 ( V87 V52 F2 F7 ) (5.870496431795854341828544,-8.080045152914353323581054)
E4 ( V52 V36 F2 F1 ) (2.374634183366903350531629,8.080045152914351547224214)
E5 ( V52 V36 F3 F4 ) (2.374634183366898909639531,-8.080045152914353323581054)
E6 ( V36 V1 F3 F9 ) (2.625365816633101090360469,8.080045152914353323581054)
E9 ( V52 V87 F4 F11 ) (-5.870496431795858782720643,-8.080045152914353323581054)
E11 ( V52 V36 F5 F6 ) (-6.670450788507833905782718,5.141118891451988304197585)
E13 ( V1 V52 F5 F13 ) (4.045084971874736368135927,2.938926261462365019383469)
E15 ( V52 V87 F6 F15 ) (0.000000000000001108833588,9.987485070269823239641482)
E16 ( V87 V36 F6 F14 ) (-6.670450788507834793961138,-4.846366178817834935443898)
E17 ( V40 V52 F7 F8 ) (8.418381912722313842323274,-0.238459953659144741067166)
E19 ( V87 V40 F7 F17 ) (-2.547885480926459056405520,-7.841585199255209026603097)
E21 ( V40 V44 F8 F19 ) (6.873296940847576586008927,-4.993742535134911619820741)
E22 ( V44 V52 F8 F18 ) (1.545084971874737256314347,4.755282581475766434664365)
E23 ( V52 V1 F9 F21 ) (-4.045084971874736368135927,2.938926261462364575294259)
E25 ( V36 V52 F9 F10 ) (6.670450788507837458496397,5.141118891451989192376004)
E26 ( V36 V87 F10 F22 ) (6.670450788507835682139557,-4.846366178817834935443898)
E27 ( V87 V52 F10 F23 ) (0.000000000000001108833588,9.987485070269823239641482)
E29 ( V40 V87 F11 F25 ) (2.547885480926453283245792,-7.841585199255208138424678)
E31 ( V52 V40 F11 F12 ) (-8.418381912722312065966435,-0.238459953659145851290191)
E32 ( V52 V44 F12 F26 ) (-1.545084971874735479957508,4.755282581475766434664365)
E33 ( V44 V40 F12 F27 ) (-6.873296940847577474187347,-4.993742535134911619820741)
E36 ( V1 V36 F13 F29 ) (6.873296940847576586008927,-4.993742535134911619820741)
E37 ( V36 V52 F13 F14 ) (-2.828211968972839773783789,7.932668796597276639204210)
E39 ( V87 V52 F14 F25 ) (-9.498662757480675011834137,3.086302617779442147849522)
E41 ( V40 V87 F15 F28 ) (-6.670450788507835682139557,4.846366178817834935443898)
E43 ( V52 V40 F15 F16 ) (6.670450788507837458496397,5.141118891451989192376004)
E44 ( V52 V44 F16 F27 ) (4.045084971874736368135927,-2.938926261462364575294259)
E45 ( V44 V40 F16 F31 ) (2.625365816633101090360469,8.080045152914353323581054)
E48 ( V87 V52 F17 F22 ) (-9.498662757480678564547816,-3.086302617779442147849522)
E49 ( V52 V40 F17 F18 ) (6.950777276554218175874666,-4.755282581475766434664365)
E51 ( V44 V40 F18 F26 ) (8.495862248428956320367433,-0.000000000000000000000000)
E53 ( V52 V44 F19 F24 ) (4.045084971874736368135927,2.938926261462365019383469)
E55 ( V40 V52 F19 F20 ) (2.828211968972839773783789,-7.932668796597276639204210)
E56 ( V40 V87 F20 F23 ) (-6.670450788507834793961138,-4.846366178817834935443898)
E57 ( V87 V52 F20 F33 ) (9.498662757480675011834137,-3.086302617779442147849522)
E59 ( V52 V36 F21 F22 ) (2.828211968972841106051419,7.932668796597276639204210)
E60 ( V36 V1 F21 F35 ) (-6.873296940847577474187347,-4.993742535134911619820741)
E65 ( V40 V52 F23 F24 ) (-6.670450788507833905782718,5.141118891451988304197585)
E69 ( V40 V44 F24 F37 ) (-2.625365816633096649468371,8.080045152914351547224214)
E71 ( V40 V52 F25 F26 ) (-6.950777276554220840409926,-4.755282581475766434664365)
E77 ( V52 V40 F27 F28 ) (-2.828211968972841106051419,-7.932668796597276639204210)
E81 ( V52 V87 F28 F39 ) (-9.498662757480678564547816,-3.086302617779442147849522)
E83 ( V52 V36 F29 F30 ) (8.418381912722313842323274,-0.238459953659144741067166)
E85 ( V1 V52 F29 F40 ) (-1.545084971874737256314347,-4.755282581475766434664365)
E87 ( V52 V87 F30 F38 ) (5.870496431795854341828544,-8.080045152914353323581054)
E88 ( V87 V36 F30 F39 ) (2.547885480926459056405520,7.841585199255209026603097)
E89 ( V52 V40 F31 F32 ) (-2.374634183366898909639531,8.080045152914353323581054)
E91 ( V44 V52 F31 F37 ) (5.000000000000000000000000,-0.000000000000000000000000)
E93 ( V52 V87 F32 F36 ) (5.870496431795858782720643,8.080045152914353323581054)
E94 ( V87 V40 F32 F38 ) (-8.245130615162757692360174,0.000000000000000554416794)
E95 ( V36 V52 F33 F34 ) (6.950777276554220840409926,4.755282581475766434664365)
E97 ( V87 V36 F33 F36 ) (2.547885480926453283245792,-7.841585199255208138424678)
E99 ( V36 V1 F34 F40 ) (8.495862248428956320367433,-0.000000000000000000000000)
E100 ( V1 V52 F34 F35 ) (-1.545084971874735479957508,4.755282581475766434664365)
E103 ( V36 V52 F35 F36 ) (-8.418381912722312065966435,-0.238459953659145851290191)
E109 ( V40 V52 F37 F38 ) (2.374634183366903350531629,8.080045152914351547224214)
E115 ( V52 V36 F39 F40 ) (-6.950777276554218175874666,4.755282581475766434664365)
# Faces: 
F1 +E0 +E1 -E4
F2 +E2 +E3 +E4
F3 +E5 +E6 -E0
F4 -E5 +E9 -E2
F5 +E11 -E1 +E13
F6 -E11 +E15 +E16
F7 +E17 -E3 +E19
F8 -E17 +E21 +E22
F9 +E23 -E6 +E25
F10 +E26 +E27 -E25
F11 +E29 -E9 +E31
F12 +E32 +E33 -E31
F13 -E13 +E36 +E37
F14 -E16 +E39 -E37
F15 +E41 -E15 +E43
F16 +E44 +E45 -E43
F17 -E19 +E48 +E49
F18 -E22 +E51 -E49
F19 +E53 -E21 +E55
F20 +E56 +E57 -E55
F21 +E59 +E60 -E23
F22 -E59 -E48 -E26
F23 +E65 -E27 -E56
F24 -E65 +E69 -E53
F25 +E71 -E39 -E29
F26 -E71 -E51 -E32
F27 +E77 -E33 -E44
F28 -E77 +E81 -E41
F29 +E83 -E36 +E85
F30 -E83 +E87 +E88
F31 +E89 -E45 +E91
F32 -E89 +E93 +E94
F33 +E95 -E57 +E97
F34 -E95 +E99 +E100
F35 -E100 -E60 +E103
F36 -E97 -E93 -E103
F37 -E91 -E69 +E109
F38 -E94 -E87 -E109
F39 -E88 -E81 +E115
F40 -E85 -E99 -E115
#UEdges Algebraic 
#E0: (5 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E1: (0 0 0 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E2: (-5 0 0 0 0 0 0 0)+(0 2 0 -4 0 1 0 2)t1
#E3: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 -5)t1
#E4: (5 0 0 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E5: (5 0 0 0 0 0 0 0)+(0 -2 0 -1 0 -1 0 -2)t1
#E6: (0 0 0 0 0 0 0 0)+(0 2 0 1 0 1 0 2)t1
#E9: (0 0 0 0 0 0 0 0)+(0 0 0 -5 0 0 0 0)t1
#E11: (0 0 -5 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E13: (0 0 5 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E15: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 5 0 0)t1
#E16: (0 0 -5 0 0 0 0 0)+(0 -2 0 4 0 -6 0 3)t1
#E17: (0 0 0 0 5 0 0 0)+(0 3 0 -1 0 -1 0 -2)t1
#E19: (0 0 0 0 -5 0 0 0)+(0 -3 0 1 0 1 0 -3)t1
#E21: (0 0 0 0 0 0 0 0)+(0 3 0 -1 0 -1 0 -2)t1
#E22: (0 0 0 0 5 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E23: (-5 0 5 0 -5 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E25: (5 0 -5 0 5 0 -5 0)+(0 2 0 1 0 1 0 2)t1
#E26: (5 0 -5 0 5 0 -5 0)+(0 2 0 1 0 -4 0 2)t1
#E27: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 5 0 0)t1
#E29: (0 0 0 0 0 0 -5 0)+(0 3 0 -6 0 4 0 -2)t1
#E31: (0 0 0 0 0 0 5 0)+(0 -3 0 1 0 -4 0 2)t1
#E32: (0 0 0 0 0 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E33: (0 0 0 0 0 0 0 0)+(0 -3 0 1 0 -4 0 2)t1
#E36: (0 0 0 0 0 0 0 0)+(0 3 0 -1 0 -1 0 -2)t1
#E37: (0 0 5 0 0 0 0 0)+(0 -3 0 1 0 1 0 2)t1
#E39: (0 0 0 0 0 0 0 0)+(0 -5 0 5 0 -5 0 5)t1
#E41: (-5 0 5 0 -5 0 5 0)+(0 -2 0 -1 0 4 0 -2)t1
#E43: (5 0 -5 0 5 0 -5 0)+(0 2 0 1 0 1 0 2)t1
#E44: (5 0 -5 0 5 0 -5 0)+(0 0 0 0 0 0 0 0)t1
#E45: (0 0 0 0 0 0 0 0)+(0 2 0 1 0 1 0 2)t1
#E48: (0 0 0 0 0 0 0 0)+(0 -5 0 0 0 0 0 0)t1
#E49: (0 0 0 0 -5 0 0 0)+(0 2 0 1 0 1 0 -3)t1
#E51: (0 0 0 0 0 0 0 0)+(0 2 0 1 0 1 0 -3)t1
#E53: (0 0 5 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E55: (0 0 -5 0 0 0 0 0)+(0 3 0 -1 0 -1 0 -2)t1
#E56: (0 0 -5 0 0 0 0 0)+(0 -2 0 4 0 -6 0 3)t1
#E57: (0 0 0 0 0 0 0 0)+(0 5 0 -5 0 5 0 -5)t1
#E59: (-5 0 5 0 -5 0 5 0)+(0 3 0 -1 0 4 0 -2)t1
#E60: (0 0 0 0 0 0 0 0)+(0 -3 0 1 0 -4 0 2)t1
#E65: (0 0 -5 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E69: (0 0 0 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E71: (0 0 0 0 0 0 -5 0)+(0 -2 0 -1 0 -1 0 3)t1
#E77: (5 0 -5 0 5 0 -5 0)+(0 -3 0 1 0 -4 0 2)t1
#E81: (0 0 0 0 0 0 0 0)+(0 -5 0 0 0 0 0 0)t1
#E83: (0 0 0 0 5 0 0 0)+(0 3 0 -1 0 -1 0 -2)t1
#E85: (0 0 0 0 -5 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E87: (0 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 -5)t1
#E88: (0 0 0 0 5 0 0 0)+(0 3 0 -1 0 -1 0 3)t1
#E89: (-5 0 0 0 0 0 0 0)+(0 2 0 1 0 1 0 2)t1
#E91: (5 0 0 0 0 0 0 0)+(0 0 0 0 0 0 0 0)t1
#E93: (0 0 0 0 0 0 0 0)+(0 0 0 5 0 0 0 0)t1
#E94: (-5 0 0 0 0 0 0 0)+(0 2 0 -4 0 1 0 2)t1
#E95: (0 0 0 0 0 0 5 0)+(0 2 0 1 0 1 0 -3)t1
#E97: (0 0 0 0 0 0 -5 0)+(0 3 0 -6 0 4 0 -2)t1
#E99: (0 0 0 0 0 0 0 0)+(0 2 0 1 0 1 0 -3)t1
#E100: (0 0 0 0 0 0 5 0)+(0 0 0 0 0 0 0 0)t1
#E103: (0 0 0 0 0 0 5 0)+(0 -3 0 1 0 -4 0 2)t1
#E109: (5 0 0 0 0 0 0 0)+(0 -2 0 4 0 -1 0 3)t1
#E115: (0 0 0 0 5 0 0 0)+(0 -2 0 -1 0 -1 0 3)t1
# File =  perturb = 0 rescale = 1
# vertices: 6
## V1(10) (4 PI) V36(20) (6 PI) V40(20) (6 PI) V44(10) (4 PI) V52(40) (14 PI) V87(20) (6 PI) 
# edges: 60
## E0(2) E1(2) E2(2) E3(2) E4(2) E5(2) E6(2) E9(2) E11(2) E13(2) E15(2) E16(2) E17(2) E19(2) E21(2) E22(2) E23(2) E25(2) E26(2) E27(2) E29(2) E31(2) E32(2) E33(2) E36(2) E37(2) E39(2) E41(2) E43(2) E44(2) E45(2) E48(2) E49(2) E51(2) E53(2) E55(2) E56(2) E57(2) E59(2) E60(2) E65(2) E69(2) E71(2) E77(2) E81(2) E83(2) E85(2) E87(2) E88(2) E89(2) E91(2) E93(2) E94(2) E95(2) E97(2) E99(2) E100(2) E103(2) E109(2) E115(2) 
# faces: 40
# genus: 8
# area: 1070.21
sweeping start V1 depth = 65.4282
File =  depth = 65.4282 follow_depth = 327.141 perturb = 0
Final Report:

 part done:100% weird 0 bad_angle 184 unusual 0 close 26914 reject 0
V1: AbBa V36: CdDcEe V40: FhGgHf V44: IiJj V52: KkLqMpNoOnPsQl V87: mRrStT   cylinders |1|1|1.58852|1.58852|2.54077|2.54077|2.57028|2.57028|4.1588|4.1588|5.72909|5.72909|
Moduli: |21.5228|21.5228|8.81577|8.81577|1|1|9.25785|9.25785|3.26297|3.26297|4.03084|4.03084| Min Area: 1 Max Area: 1
    total: 0.654498 0.397887 sym = 4
    raw = 20 short saddle = 9.51057 short cyl = 16.1601 tag #4
V1: AaXX V44: BXXb V52: CcXdDXXeEXFfXX V87: GgXhHX   cylinders |1|1|1.61803|1.61803|
Moduli: |12.69|12.69|1|1| Min Area: 0.253 Max Area: 0.253
    total: 1.309 0.795775 sym = 2
    raw = 20 short saddle = 54.9599 short cyl = 54.9599 tag #6

total cylinders: 19.635 11.9366 raw = 150 rejected = 0
""",
}
