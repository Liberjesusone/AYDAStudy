import sys
import math
from typing import List, Tuple

from lib.point2d import Point2D, real_equal
from lib.vector2d import Vector2D
from lib.segment import Segment
from lib.polygon import Polygon
from definitions import Obstacle, VisionArea
from geom import is_inside, intersects, translate, rotate, rotate_around, build_vision_area, convex_hull_graham, convex_hull_jarvis

# ----------------------------------------------------------------------
# Helper to build a Polygon from a list of coordinate pairs
def make_polygon(coords):
    points = [Point2D(x, y) for x, y in coords]
    return Polygon(points)

# ----------------------------------------------------------------------
# Helper to run a section and return points earned
def run_section(name, max_points, tests):
    print("\n" + "="*60)
    print(f"SECTION: {name} (max {max_points} points)")
    print("="*60)
    failed = 0
    for i, test_func in enumerate(tests, start=1):
        try:
            if test_func():
                print(f"  Test {i:2d}: OK")
            else:
                print(f"  Test {i:2d}: FAILED")
                failed += 1
        except Exception as e:
            print(f"  Test {i:2d}: EXCEPTION - {e}")
            failed += 1
    if failed == 0:
        print(f"\nSection passed: +{max_points} points")
        return max_points
    else:
        print(f"\nSection failed ({failed} tests failed): 0 points")
        return 0

# ----------------------------------------------------------------------
# Section 1: is_inside single obstacle (10 tests)
# ----------------------------------------------------------------------
def test_single_1():
    p = Point2D(71.3438813055, 49.5812133738)
    obs = make_polygon([
        (37.0346789935, 68.4884713872), (55.873962507, 39.6978010401),
        (86.7959967319, 74.5318691289), (86.1625060235, 76.8333157796),
        (77.0670288083, 97.3336730771), (51.294188447, 96.2119083782),
        (49.7210861882, 96.114980918)
    ])
    return not is_inside(p, obs)

def test_single_2():
    p = Point2D(45.5973525997, 13.6834730447)
    obs = make_polygon([
        (41.4045994406, 59.0020003575), (65.4984002843, 40.5047956808),
        (95.8365671661, 33.716476375), (90.4738174686, 91.1803418724),
        (63.9923456735, 97.6394444277), (57.6306682469, 91.9694255359)
    ])
    return not is_inside(p, obs)

def test_single_3():
    p = Point2D(39.2502031271, 10.0902434755)
    obs = make_polygon([
        (36.3846300646, 78.060516146), (36.8680700524, 62.9920942315),
        (45.8983717274, 30.4496232875), (80.8157835684, 30.7769126565),
        (92.722722575, 32.5505837072), (95.159313751, 81.7189480464),
        (46.6597312286, 89.7569742511)
    ])
    return not is_inside(p, obs)

def test_single_4():
    p = Point2D(67.5463109126, 43.0647374057)
    obs = make_polygon([
        (41.3271938508, 62.7634189474), (68.5703293505, 37.7207163723),
        (83.5019652442, 44.574124318), (92.8664414454, 52.1903704879),
        (88.6765940776, 85.5280786127), (59.9928117007, 93.8663100749),
        (54.4258079366, 94.2588834781)
    ])
    return is_inside(p, obs)

def test_single_5():
    p = Point2D(11.5147833448, 0.941188574721)
    obs = make_polygon([
        (30.0625074048, 98.1514876689), (31.3016429836, 49.7051746779),
        (38.7823986771, 41.9260614688), (53.3652512286, 36.0938067887),
        (86.4891734179, 33.5669909796), (79.6205468479, 67.0909238632),
        (68.8801212982, 86.0610370959), (43.0119047452, 97.3971653919)
    ])
    return not is_inside(p, obs)

def test_single_6():
    p = Point2D(85.9775175951, 5.27911704075)
    obs = make_polygon([
        (31.8226876445, 40.4592527456), (94.0052332291, 30.6302795026),
        (80.2966756799, 69.723445679), (46.0001245486, 85.698333226),
        (36.5847554167, 72.1347749795)
    ])
    return not is_inside(p, obs)

def test_single_7():
    p = Point2D(20.4255191851, 30.0861453225)
    obs = make_polygon([
        (33.4938963127, 91.2616210158), (41.3486924547, 33.718629775),
        (86.4066035305, 45.4034839), (88.1429407524, 62.9462009027),
        (75.799153429, 84.8607925537), (33.8530162761, 93.7957256341)
    ])
    return not is_inside(p, obs)

def test_single_8():
    p = Point2D(27.9195314788, 60.4869270415)
    obs = make_polygon([
        (31.1715751007, 44.8775056834), (48.446063929, 43.8078138518),
        (84.4755717762, 48.3489547183), (93.180568963, 77.3866096522),
        (56.9315671464, 96.1542835219)
    ])
    return not is_inside(p, obs)

def test_single_9():
    p = Point2D(79.9894845577, 18.8460331163)
    obs = make_polygon([
        (39.8875476884, 31.0032573573), (87.2034367153, 45.8279956123),
        (94.5397078305, 67.5786937823), (85.2840433198, 94.7050362252),
        (50.7825598701, 91.3571000169), (41.0703122186, 77.2537537973)
    ])
    return not is_inside(p, obs)

def test_single_10():
    p = Point2D(63.0056442991, 67.6514046751)
    obs = make_polygon([
        (37.8730543126, 61.7974583465), (85.9264225633, 37.5537283897),
        (86.3804983078, 96.4399295958), (57.1816671715, 98.3413839533),
        (42.6867116149, 90.1060720336)
    ])
    return is_inside(p, obs)

# ----------------------------------------------------------------------
# Section 2: is_inside list of obstacles (10 tests)
# ----------------------------------------------------------------------
obstacle_list = [
    make_polygon([
        (100.842452804, 164.654021519), (116.353736977, 137.850967413),
        (136.243813839, 105.333065969), (158.838656479, 109.779414439),
        (155.319158361, 132.683691538), (147.972251592, 149.653486997)
    ]),
    make_polygon([
        (64.6470621904, 114.143147688), (110.729760719, 77.3406591468),
        (127.735959376, 81.9899207248), (124.500389214, 100.201511431),
        (122.27610335, 109.479219155), (113.429296361, 112.83524066),
        (102.08262914, 115.577154738)
    ]),
    make_polygon([
        (60.0698166548, 71.8465847019), (81.6035997025, 61.9937071744),
        (100.762217359, 77.2914798238), (101.075871077, 111.187650542),
        (74.1181422044, 109.067690419)
    ]),
    make_polygon([
        (51.4068015547, 108.89266422), (57.1468013361, 55.837114315),
        (102.8853305, 55.6548184783), (113.688825623, 69.844104241),
        (118.295984428, 103.755681571), (99.1219678136, 114.658887093)
    ]),
    make_polygon([
        (41.988276959, 74.9580864918), (80.7393283803, 43.4310895817),
        (89.992433446, 46.8805716474), (95.8098068002, 72.1490774272),
        (85.0995711639, 95.0253137299), (51.2866068223, 87.9758957995)
    ]),
    make_polygon([
        (4.33327083323, 10.8970926268), (28.3656943355, 9.83741480443),
        (66.6389299547, 29.6805076731), (18.7389207429, 62.6207686039),
        (12.8318499517, 41.7302369378)
    ]),
    make_polygon([
        (91.5721796612, 116.612430678), (107.476358329, 98.3075852781),
        (156.200527189, 93.4163326961), (151.517101486, 126.585203854),
        (132.857109845, 142.407327114)
    ]),
    make_polygon([
        (117.364482831, 146.529976444), (127.753656508, 107.039017171),
        (157.751900107, 108.824601927), (154.248077463, 125.966756488),
        (143.827991514, 145.266818745), (122.897760833, 161.581894083)
    ]),
    make_polygon([
        (54.7121770588, 108.686303018), (60.9323249875, 66.3769383976),
        (99.4311607665, 69.9307307308), (103.959666741, 70.6234577611),
        (116.053682879, 103.529463566), (98.7208691064, 116.225806026)
    ]),
    make_polygon([
        (38.1956675047, 80.3937126391), (52.9242769177, 45.797950815),
        (78.4178185785, 40.2160024155), (93.1868796671, 37.6565553121),
        (94.9373031887, 100.329678598), (40.7750274647, 92.0877328275)
    ])
]

def test_list_1(): return is_inside(Point2D(33.80232244, 40.1306467411), obstacle_list)
def test_list_2(): return is_inside(Point2D(56.1146678827, 58.0697444317), obstacle_list)
def test_list_3(): return not is_inside(Point2D(2.5711417029, 66.5399477083), obstacle_list)
def test_list_4(): return not is_inside(Point2D(13.9783764019, 7.18968229302), obstacle_list)
def test_list_5(): return is_inside(Point2D(53.8564856441, 73.8540171214), obstacle_list)
def test_list_6(): return not is_inside(Point2D(6.73089310416, 98.2223586838), obstacle_list)
def test_list_7(): return is_inside(Point2D(39.1297616237, 26.4651184869), obstacle_list)
def test_list_8(): return is_inside(Point2D(56.7713301009, 94.4627403043), obstacle_list)
def test_list_9(): return is_inside(Point2D(45.3574516458, 83.7059860276), obstacle_list)
def test_list_10(): return not is_inside(Point2D(3.68370207946, 90.1151714186), obstacle_list)

# ----------------------------------------------------------------------
# Section 3: intersects single obstacle (10 tests)
# ----------------------------------------------------------------------
def test_intersect_single_1():
    s = Segment(Point2D(53.6485359487, 71.7573541209), Point2D(69.3874962961, 34.4658272697))
    o = make_polygon([
        (36.6290321439, 41.8994671744), (52.1251957812, 33.0951772166),
        (75.2708107577, 38.0282739993), (98.8217511127, 50.8970003448),
        (84.30376213, 85.5448503064), (55.3504039578, 98.1271932959)
    ])
    return intersects(s, o)

def test_intersect_single_2():
    s = Segment(Point2D(14.7290007744, 13.8771519447), Point2D(77.4607946959, 9.17086587972))
    o = make_polygon([
        (30.5840491753, 50.0440744071), (47.7081703861, 34.0076471631),
        (92.7221932979, 38.4970722531), (96.5458123272, 73.378710831),
        (73.8904058265, 95.5070791394), (40.5244050775, 98.1061129741)
    ])
    return not intersects(s, o)

def test_intersect_single_3():
    s = Segment(Point2D(90.8719850374, 19.9214770172), Point2D(51.5671604039, 22.7666509953))
    o = make_polygon([
        (45.6891765847, 94.2785534202), (56.1144285982, 40.9234337357),
        (91.6475153442, 33.3413007799), (95.2949662989, 71.3934236283),
        (45.6891765847, 94.2785534202), (0, 0), (95.2949662989, 71.3934236283)
    ])
    return not intersects(s, o)

def test_intersect_single_4():
    s = Segment(Point2D(95.8070292705, 76.1811158047), Point2D(46.298081168, 3.18048536815))
    o = make_polygon([
        (30.4105036719, 63.5598420041), (39.9236198234, 41.9763553484),
        (53.3124497852, 38.0092243153), (83.5256401232, 39.0128862767),
        (91.6469206227, 42.5133097878), (88.2948066618, 58.5690997787),
        (68.2670317047, 92.1525345464)
    ])
    return intersects(s, o)

def test_intersect_single_5():
    s = Segment(Point2D(19.4283899334, 26.6611203803), Point2D(77.5807481414, 67.5888638262))
    o = make_polygon([
        (30.2658729148, 36.0234145015), (78.4641210407, 34.0819715187),
        (92.2943998568, 76.3802316436), (74.143143109, 93.4844314227)
    ])
    return intersects(s, o)

def test_intersect_single_6():
    s = Segment(Point2D(78.2744134642, 92.0983933979), Point2D(57.8443842748, 72.6851016948))
    o = make_polygon([
        (33.5157502543, 36.5088501792), (88.622277622, 30.6467873426),
        (97.0063852438, 73.3300158362), (89.7144478479, 98.8075976103),
        (39.1093043755, 46.9477603642)
    ])
    return not intersects(s, o)

def test_intersect_single_7():
    s = Segment(Point2D(44.2710347319, 75.7237264322), Point2D(86.8329211775, 46.6393455266))
    o = make_polygon([
        (32.919446024, 81.0494156919), (45.8017937962, 32.7410748981),
        (89.2868455708, 77.420606861), (77.4026958629, 90.5481090231),
        (34.0224751618, 93.784241695)
    ])
    return intersects(s, o)

def test_intersect_single_8():
    s = Segment(Point2D(72.0736208491, 18.5321891265), Point2D(84.0726874249, 4.14657613595))
    o = make_polygon([
        (34.7040159874, 40.7946075512), (56.953941311, 42.5665013412),
        (76.1882451541, 47.1173076413), (86.5116968755, 74.3095508968),
        (90.8185937482, 90.086599891), (48.2732255292, 89.8481516159)
    ])
    return not intersects(s, o)

def test_intersect_single_9():
    s = Segment(Point2D(41.7514960248, 14.2325999655), Point2D(98.1958273684, 69.7768985977))
    o = make_polygon([
        (33.5640836635, 71.3332652102), (44.2197283087, 35.7944012205),
        (97.1841588569, 70.9808181846), (78.7890248416, 96.4706719416),
        (67.2675819425, 95.3690831601)
    ])
    return not intersects(s, o)

def test_intersect_single_10():
    s = Segment(Point2D(12.0078472478, 69.0700167732), Point2D(85.7316416915, 73.1534024889))
    o = make_polygon([
        (30.0738772232, 89.5853525411), (39.8839994033, 37.0839274931),
        (59.127764624, 31.3454027674), (92.1825697196, 33.7695834768),
        (55.0032370446, 95.3164702011)
    ])
    return intersects(s, o)

# ----------------------------------------------------------------------
# Section 4: intersects list of obstacles (10 tests)
# ----------------------------------------------------------------------
def test_intersect_list_1():
    s = Segment(Point2D(44.0448039328, 58.2615040828), Point2D(64.5143634949, 30.4070153914))
    return intersects(s, obstacle_list)

def test_intersect_list_2():
    s = Segment(Point2D(81.5792346562, 25.5179312254), Point2D(12.1239300937, 55.7397054033))
    return intersects(s, obstacle_list)

def test_intersect_list_3():
    s = Segment(Point2D(74.4710327959, 66.3115341105), Point2D(85.6237566124, 78.3638045378))
    return intersects(s, obstacle_list)

def test_intersect_list_4():
    s = Segment(Point2D(52.3555237663, 81.3182913464), Point2D(4.89671212604, 43.3535135246))
    return intersects(s, obstacle_list)

def test_intersect_list_5():
    s = Segment(Point2D(68.7682610295, 58.3419372188), Point2D(7.11381521567, 80.2092637235))
    return intersects(s, obstacle_list)

def test_intersect_list_6():
    s = Segment(Point2D(87.4112269582, 13.0885878663), Point2D(66.9773959799, 64.5278356649))
    return intersects(s, obstacle_list)

def test_intersect_list_7():
    s = Segment(Point2D(51.3166601182, 98.4440405158), Point2D(58.6355094438, 81.5287496785))
    return intersects(s, obstacle_list)

def test_intersect_list_8():
    s = Segment(Point2D(94.1236993023, 14.8740279712), Point2D(8.40431745859, 14.3314116815))
    return intersects(s, obstacle_list)

def test_intersect_list_9():
    s = Segment(Point2D(32.6116611098, 20.767878695), Point2D(18.1795497616, 39.1229578354))
    return not intersects(s, obstacle_list)

def test_intersect_list_10():
    s = Segment(Point2D(50.2936226658, 68.3890549204), Point2D(31.7795450083, 55.5458235569))
    return intersects(s, obstacle_list)

# ----------------------------------------------------------------------
# Section 5: translate (10 tests)
# ----------------------------------------------------------------------
def test_translate_1():
    v = Vector2D(453.515052536, 18.6670583411)
    res = translate(v, -10.074323, 32.499154)
    return res == Vector2D(443.440729357, 51.1662126034)

def test_translate_2():
    v = Vector2D(828.124425327, 663.14138786)
    res = translate(v, -29.261235, 24.788839)
    return res == Vector2D(798.863190543, 687.930226883)

def test_translate_3():
    v = Vector2D(721.287077183, 146.30619265)
    res = translate(v, -94.747760, -6.834375)
    return res == Vector2D(626.53931712, 139.471817735)

def test_translate_4():
    v = Vector2D(44.0497243914, 254.859435366)
    res = translate(v, -40.918947, 22.227622)
    return res == Vector2D(3.1307775312, 277.087057626)

def test_translate_5():
    v = Vector2D(486.034442122, 564.540298958)
    res = translate(v, 78.426786, -53.094031)
    return res == Vector2D(564.461228096, 511.446268319)

def test_translate_6():
    v = Vector2D(451.633606516, 27.3802689371)
    res = translate(v, 9.218274, -82.618266)
    return res == Vector2D(460.851880034, -55.2379968852)

def test_translate_7():
    v = Vector2D(148.846007408, 622.798284687)
    res = translate(v, 32.280522, 97.864247)
    return res == Vector2D(181.126529901, 720.662531234)

def test_translate_8():
    v = Vector2D(747.360660266, 825.570415626)
    res = translate(v, -72.987158, -70.620180)
    return res == Vector2D(674.373502493, 754.950235217)

def test_translate_9():
    v = Vector2D(635.464554956, 806.320754966)
    res = translate(v, 2.663724, 42.577535)
    return res == Vector2D(638.128278458, 848.89829015)

def test_translate_10():
    v = Vector2D(460.821735785, 407.290775849)
    res = translate(v, -95.947423, -31.290299)
    return res == Vector2D(364.874312837, 376.000476829)

# ----------------------------------------------------------------------
# Section 6: rotate (10 tests)
# ----------------------------------------------------------------------
def test_rotate_1():
    v = Vector2D(873.72009961, 570.873954725)
    r = rotate(v, 60.0)
    exp = Vector2D(-57.5312974684, 1042.10077961)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_2():
    v = Vector2D(892.418643366, 895.881952292)
    r = rotate(v, 0.0)
    exp = Vector2D(892.418643366, 895.881952292)
    return r == exp

def test_rotate_3():
    v = Vector2D(820.194136527, 672.527897496)
    r = rotate(v, 60.0)
    exp = Vector2D(-172.329175867, 1046.57290719)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_4():
    v = Vector2D(470.824134156, 547.782360837)
    r = rotate(v, 90.0)
    exp = Vector2D(-547.782360837, 470.824134156)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_5():
    v = Vector2D(707.743597929, 640.830838764)
    r = rotate(v, 0.0)
    exp = Vector2D(707.743597929, 640.830838764)
    return r == exp

def test_rotate_6():
    v = Vector2D(613.69501244, 96.6093967948)
    r = rotate(v, 45.0)
    exp = Vector2D(365.63474518, 502.261064344)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_7():
    v = Vector2D(720.373907957, 64.5843921859)
    r = rotate(v, 0.0)
    exp = Vector2D(720.373907957, 64.5843921859)
    return r == exp

def test_rotate_8():
    v = Vector2D(434.083994461, 601.267592913)
    r = rotate(v, 45.0)
    exp = Vector2D(-118.216656137, 732.104128152)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_9():
    v = Vector2D(680.586435178, 717.249675658)
    r = rotate(v, 60.0)
    exp = Vector2D(-280.863222542, 948.029980312)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_10():
    v = Vector2D(203.225324632, 365.951057989)
    r = rotate(v, 45.0)
    exp = Vector2D(-115.0644695, 402.468479736)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

# ----------------------------------------------------------------------
# Section 7: rotate_around (10 tests)
# ----------------------------------------------------------------------
def test_rotate_around_1():
    v = Vector2D(35.9066073499, 289.550942665)
    u = Vector2D(952.638012326, 75.6400840674)
    r = rotate_around(v, 0.0, u)
    exp = Vector2D(35.9066073499, 289.550942665)
    return r == exp

def test_rotate_around_2():
    v = Vector2D(882.957945821, 108.140670972)
    u = Vector2D(810.965274666, 951.763888493)
    r = rotate_around(v, 30.0, u)
    exp = Vector2D(1295.12436555, 257.161086293)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_3():
    v = Vector2D(313.576767926, 366.360442893)
    u = Vector2D(923.333721241, 806.668533859)
    r = rotate_around(v, 60.0, u)
    exp = Vector2D(999.773236946, 58.4494765394)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_4():
    v = Vector2D(128.932781509, 796.633066938)
    u = Vector2D(236.664260085, 886.864877369)
    r = rotate_around(v, 90.0, u)
    exp = Vector2D(326.896070516, 779.133398793)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_5():
    v = Vector2D(507.35705823, 351.372789345)
    u = Vector2D(911.280360951, 50.5950195544)
    r = rotate_around(v, 45.0, u)
    exp = Vector2D(412.981453999, -22.3398862103)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_6():
    v = Vector2D(589.245136089, 886.750693524)
    u = Vector2D(120.849251864, 592.13276356)
    r = rotate_around(v, 60.0, u)
    exp = Vector2D(99.9005821538, 1145.08446341)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_7():
    v = Vector2D(834.603700892, 638.038436658)
    u = Vector2D(416.176086716, 560.078263695)
    r = rotate_around(v, 60.0, u)
    exp = Vector2D(557.874403517, 961.427293788)
    return abs(r.get_x() - exp.get_x()) < 1e-6 and abs(r.get_y() - exp.get_y()) < 1e-6

def test_rotate_around_8():
    v = Vector2D(692.083638326, 240.031799907)
    u = Vector2D(543.170259369, 604.182484962)
    r = rotate_around(v, 0.0, u)
    exp = Vector2D(692.083638326, 240.031799907)
    return r == exp

def test_rotate_around_9():
    v = Vector2D(496.610131609, 58.4613661339)
    u = Vector2D(838.380494932, 526.14121529)
    r = rotate_around(v, 0.0, u)
    exp = Vector2D(496.610131609, 58.4613661339)
    return r == exp

def test_rotate_around_10():
    v = Vector2D(49.3702092369, 529.609921364)
    u = Vector2D(929.866779387, 202.029172059)
    r = rotate_around(v, 0.0, u)
    exp = Vector2D(49.3702092369, 529.609921364)
    return r == exp

# ----------------------------------------------------------------------
# Section 8: build_vision_area (10 tests)
# ----------------------------------------------------------------------
def test_build_va_0():
    p = Vector2D(711.873589966, 337.462690675)
    f = Vector2D(0.105767425526, 0.994390894818)
    va = build_vision_area(p, f, 13.282032, 60.0)
    pos, right, left, radius = va
    return (pos == Vector2D(711.873589966, 337.462690675) and
            right == Vector2D(724.014051285, 342.849858634) and
            left == Vector2D(701.137934997, 345.283054612) and
            real_equal(radius, 13.282032))

def test_build_va_1():
    p = Vector2D(412.287777988, 560.185205837)
    f = Vector2D(0.994950277681, 0.100369043751)
    va = build_vision_area(p, f, 42.051654, 90.0)
    pos, right, left, radius = va
    return (pos == Vector2D(412.287777988, 560.185205837) and
            right == Vector2D(416.508462247, 518.345901419) and
            left == Vector2D(408.067093729, 602.024510254) and
            real_equal(radius, 42.051654))

def test_build_va_2():
    p = Vector2D(14.018628577, 695.721836162)
    f = Vector2D(0.733648411268, 0.679529255179)
    va = build_vision_area(p, f, 28.290768, 45.0)
    pos, right, left, radius = va
    return (pos == Vector2D(14.018628577, 695.721836162) and
            right == Vector2D(42.2886742256, 694.63920441) and
            left == Vector2D(15.1012603289, 723.991881803) and
            real_equal(radius, 28.290768))

def test_build_va_3():
    p = Vector2D(326.427706268, 245.848718262)
    f = Vector2D(0.0609005260461, 0.998143840299)
    va = build_vision_area(p, f, 18.905249, 90.0)
    pos, right, left, radius = va
    return (pos == Vector2D(326.427706268, 245.848718262) and
            right == Vector2D(345.297864191, 244.697378648) and
            left == Vector2D(307.557548345, 247.000057877) and
            real_equal(radius, 18.905249))

def test_build_va_4():
    p = Vector2D(30.3985522021, 340.081682879)
    f = Vector2D(0.701890289109, 0.712285070779)
    va = build_vision_area(p, f, 17.908701, 45.0)
    pos, right, left, radius = va
    return (pos == Vector2D(30.3985522021, 340.081682879) and
            right == Vector2D(48.3067693421, 340.213315781) and
            left == Vector2D(30.2669192997, 357.989900014) and
            real_equal(radius, 17.908701))

def test_build_va_5():
    p = Vector2D(659.089222348, 779.539535852)
    f = Vector2D(0.970575383932, 0.240797475288)
    va = build_vision_area(p, f, 26.898808, 90.0)
    pos, right, left, radius = va
    return (pos == Vector2D(659.089222348, 779.539535852) and
            right == Vector2D(665.56638732, 753.432215286) and
            left == Vector2D(652.612057377, 805.646856418) and
            real_equal(radius, 26.898808))

def test_build_va_6():
    p = Vector2D(496.493545463, 569.62628293)
    f = Vector2D(0.885889193404, 0.463896903427)
    va = build_vision_area(p, f, 35.852754, 30.0)
    pos, right, left, radius = va
    return (pos == Vector2D(496.493545463, 569.62628293) and
            right == Vector2D(532.315860801, 568.149217798) and
            left == Vector2D(515.683879066, 599.910785473) and
            real_equal(radius, 35.852754))

def test_build_va_7():
    p = Vector2D(279.15728205, 546.12900229)
    f = Vector2D(0.362274231973, 0.932071553503)
    va = build_vision_area(p, f, 30.368795, 60.0)
    pos, right, left, radius = va
    return (pos == Vector2D(279.15728205, 546.12900229) and
            right == Vector2D(309.171817427, 550.754081307) and
            left == Vector2D(260.144578432, 569.809812904) and
            real_equal(radius, 30.368795))

def test_build_va_8():
    p = Vector2D(973.424568998, 166.55713959)
    f = Vector2D(0.939674269615, 0.342070558546)
    va = build_vision_area(p, f, 32.550815, 90.0)
    pos, right, left, radius = va
    return (pos == Vector2D(973.424568998, 166.55713959) and
            right == Vector2D(984.559244408, 135.969976439) and
            left == Vector2D(962.289893588, 197.144302741) and
            real_equal(radius, 32.550815))

def test_build_va_9():
    p = Vector2D(117.090646855, 852.15122798)
    f = Vector2D(0.870689242901, 0.491833551415)
    va = build_vision_area(p, f, 13.641254, 30.0)
    pos, right, left, radius = va
    return (pos == Vector2D(117.090646855, 852.15122798) and
            right == Vector2D(130.731298087, 852.022941919) and
            left == Vector2D(124.022071461, 863.900235442) and
            real_equal(radius, 13.641254))

# ----------------------------------------------------------------------
# Section 9: is_inside VisionArea (10 tests)
# ----------------------------------------------------------------------
def test_vision_inside_1():
    va = (Vector2D(0,50), Vector2D(2.12020107231e-15,15.3744888112), Vector2D(0,84.6255111888), 34.625511)
    return not is_inside(Point2D(88.9188196268, 21.7302019945), va)

def test_vision_inside_2():
    va = (Vector2D(0,50), Vector2D(12.1494688006,37.8505311994), Vector2D(12.1494687974,62.1494687974), 17.181944)
    return is_inside(Point2D(10.5531104268, 42.3841627006), va)

def test_vision_inside_3():
    va = (Vector2D(0,50), Vector2D(21.1247566574,28.8752433426), Vector2D(21.1247566518,71.1247566518), 29.874917)
    return not is_inside(Point2D(56.3426777906, 29.6113015898), va)

def test_vision_inside_4():
    va = (Vector2D(0,50), Vector2D(48.5718160881,50), Vector2D(48.5718160881,50), 48.571816)
    return not is_inside(Point2D(55.3145606558, 40.3743281818), va)

def test_vision_inside_5():
    va = (Vector2D(0,50), Vector2D(17.9129379696,50), Vector2D(17.9129379696,50), 17.912938)
    return not is_inside(Point2D(56.3899050601, 41.1761539624), va)

def test_vision_inside_6():
    va = (Vector2D(0,50), Vector2D(16.8472629476,40.2732282021), Vector2D(16.8472629518,59.7267717979), 19.453544)
    return not is_inside(Point2D(93.0879410992, 14.039535151), va)

def test_vision_inside_7():
    va = (Vector2D(0,50), Vector2D(23.4980775563,9.30013579236), Vector2D(23.4980775563,90.6998642178), 46.996155)
    return not is_inside(Point2D(46.5345535194, 7.78456596931), va)

def test_vision_inside_8():
    va = (Vector2D(0,50), Vector2D(15.8600220198,34.1399779802), Vector2D(15.8600220156,65.8600220156), 22.429458)
    return not is_inside(Point2D(38.905242278, 0.326704109972), va)

def test_vision_inside_9():
    va = (Vector2D(0,50), Vector2D(25.8794140438,50), Vector2D(25.8794140438,50), 25.879414)
    return not is_inside(Point2D(81.3705995645, 28.7693975702), va)

def test_vision_inside_10():
    va = (Vector2D(0,50), Vector2D(16.3554936968,50), Vector2D(16.3554936968,50), 16.355494)
    return not is_inside(Point2D(69.1832770474, 45.7693992134), va)

# Test Definitions
def test_hull_basic():
    pts = [Point2D(0, 3), Point2D(2, 2), Point2D(1, 1), Point2D(2, 1),
           Point2D(3, 0), Point2D(0, 0), Point2D(3, 3)]
    g_hull = convex_hull_graham(pts)
    j_hull = convex_hull_jarvis(pts)
    return len(g_hull) == 4 and len(j_hull) == 4

def test_hull_triangle():
    pts = [Point2D(0, 0), Point2D(5, 0), Point2D(0, 5), Point2D(1, 1)]
    return len(convex_hull_graham(pts)) == 3

# ----------------------------------------------------------------------
# Main runner
def main():
    total = 0
    total += run_section("is_inside (single obstacle)", 3,
        [test_single_1, test_single_2, test_single_3, test_single_4,
         test_single_5, test_single_6, test_single_7, test_single_8,
         test_single_9, test_single_10])
    total += run_section("is_inside (list of obstacles)", 2,
        [test_list_1, test_list_2, test_list_3, test_list_4,
         test_list_5, test_list_6, test_list_7, test_list_8,
         test_list_9, test_list_10])
    total += run_section("intersects (single obstacle)", 2,
        [test_intersect_single_1, test_intersect_single_2, test_intersect_single_3,
         test_intersect_single_4, test_intersect_single_5, test_intersect_single_6,
         test_intersect_single_7, test_intersect_single_8, test_intersect_single_9,
         test_intersect_single_10])
    total += run_section("intersects (list of obstacles)", 2,
        [test_intersect_list_1, test_intersect_list_2, test_intersect_list_3,
         test_intersect_list_4, test_intersect_list_5, test_intersect_list_6,
         test_intersect_list_7, test_intersect_list_8, test_intersect_list_9,
         test_intersect_list_10])
    total += run_section("translate", 2,
        [test_translate_1, test_translate_2, test_translate_3, test_translate_4,
         test_translate_5, test_translate_6, test_translate_7, test_translate_8,
         test_translate_9, test_translate_10])
    total += run_section("rotate", 2,
        [test_rotate_1, test_rotate_2, test_rotate_3, test_rotate_4,
         test_rotate_5, test_rotate_6, test_rotate_7, test_rotate_8,
         test_rotate_9, test_rotate_10])
    total += run_section("rotate_around", 2,
        [test_rotate_around_1, test_rotate_around_2, test_rotate_around_3,
         test_rotate_around_4, test_rotate_around_5, test_rotate_around_6,
         test_rotate_around_7, test_rotate_around_8, test_rotate_around_9,
         test_rotate_around_10])
    total += run_section("build_vision_area", 2,
        [test_build_va_0, test_build_va_1, test_build_va_2, test_build_va_3,
         test_build_va_4, test_build_va_5, test_build_va_6, test_build_va_7,
         test_build_va_8, test_build_va_9])
    total += run_section("is_inside (VisionArea)", 3,
        [test_vision_inside_1, test_vision_inside_2, test_vision_inside_3,
         test_vision_inside_4, test_vision_inside_5, test_vision_inside_6,
         test_vision_inside_7, test_vision_inside_8, test_vision_inside_9,
         test_vision_inside_10])
    total += run_section("Python Convex Hull Verification", 4, [test_hull_basic, test_hull_triangle])
    print("\n" + "="*60)
    print(f"FINAL SCORE: {total} / 20")
    print("="*60)
    sys.exit(0 if total == 20 else 1)

if __name__ == "__main__":
    main()