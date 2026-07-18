#include <bits/stdc++.h>
using namespace std;

#include "Definitions.hpp"
#include <Geom.hpp>

// Helper to run a section of tests.
// Returns true if all tests in the section pass.
template<typename... Tests>
bool run_section(const string& name, int points, Tests... tests) {
    cout << "\n============================================================" << endl;
    cout << "SECTION: " << name << " (max " << points << " points)" << endl;
    cout << "============================================================" << endl;

    vector<bool> results = {tests()...};
    int failed = count(results.begin(), results.end(), false);
    int idx = 1;
    for (bool res : results) {
        cout << "  Test " << setw(2) << idx++ << ": " << (res ? "OK" : "FAILED") << endl;
    }

    if (failed == 0) {
        cout << "\nSection passed: +" << points << " points" << endl;
        return true;
    } else {
        cout << "\nSection failed (" << failed << " tests failed): 0 points" << endl;
        return false;
    }
}

// ----------------------------------------------------------------------
// Test functions for each individual assertion
// ----------------------------------------------------------------------

bool test_is_inside_single_1() {
    Point2D p(71.3438813055, 49.5812133738);
    Obstacle o({Point2D(37.0346789935, 68.4884713872), Point2D(55.873962507, 39.6978010401),
                Point2D(86.7959967319, 74.5318691289), Point2D(86.1625060235, 76.8333157796),
                Point2D(77.0670288083, 97.3336730771), Point2D(51.294188447, 96.2119083782),
                Point2D(49.7210861882, 96.114980918)});
    return !is_inside(p, o);
}
bool test_is_inside_single_2() {
    Point2D p(45.5973525997, 13.6834730447);
    Obstacle o({Point2D(41.4045994406, 59.0020003575), Point2D(65.4984002843, 40.5047956808),
                Point2D(95.8365671661, 33.716476375), Point2D(90.4738174686, 91.1803418724),
                Point2D(63.9923456735, 97.6394444277), Point2D(57.6306682469, 91.9694255359)});
    return !is_inside(p, o);
}
bool test_is_inside_single_3() {
    Point2D p(39.2502031271, 10.0902434755);
    Obstacle o({Point2D(36.3846300646, 78.060516146), Point2D(36.8680700524, 62.9920942315),
                Point2D(45.8983717274, 30.4496232875), Point2D(80.8157835684, 30.7769126565),
                Point2D(92.722722575, 32.5505837072), Point2D(95.159313751, 81.7189480464),
                Point2D(46.6597312286, 89.7569742511)});
    return !is_inside(p, o);
}
bool test_is_inside_single_4() {
    Point2D p(67.5463109126, 43.0647374057);
    Obstacle o({Point2D(41.3271938508, 62.7634189474), Point2D(68.5703293505, 37.7207163723),
                Point2D(83.5019652442, 44.574124318), Point2D(92.8664414454, 52.1903704879),
                Point2D(88.6765940776, 85.5280786127), Point2D(59.9928117007, 93.8663100749),
                Point2D(54.4258079366, 94.2588834781)});
    return is_inside(p, o);
}
bool test_is_inside_single_5() {
    Point2D p(11.5147833448, 0.941188574721);
    Obstacle o({Point2D(30.0625074048, 98.1514876689), Point2D(31.3016429836, 49.7051746779),
                Point2D(38.7823986771, 41.9260614688), Point2D(53.3652512286, 36.0938067887),
                Point2D(86.4891734179, 33.5669909796), Point2D(79.6205468479, 67.0909238632),
                Point2D(68.8801212982, 86.0610370959), Point2D(43.0119047452, 97.3971653919)});
    return !is_inside(p, o);
}
bool test_is_inside_single_6() {
    Point2D p(85.9775175951, 5.27911704075);
    Obstacle o({Point2D(31.8226876445, 40.4592527456), Point2D(94.0052332291, 30.6302795026),
                Point2D(80.2966756799, 69.723445679), Point2D(46.0001245486, 85.698333226),
                Point2D(36.5847554167, 72.1347749795)});
    return !is_inside(p, o);
}
bool test_is_inside_single_7() {
    Point2D p(20.4255191851, 30.0861453225);
    Obstacle o({Point2D(33.4938963127, 91.2616210158), Point2D(41.3486924547, 33.718629775),
                Point2D(86.4066035305, 45.4034839), Point2D(88.1429407524, 62.9462009027),
                Point2D(75.799153429, 84.8607925537), Point2D(33.8530162761, 93.7957256341)});
    return !is_inside(p, o);
}
bool test_is_inside_single_8() {
    Point2D p(27.9195314788, 60.4869270415);
    Obstacle o({Point2D(31.1715751007, 44.8775056834), Point2D(48.446063929, 43.8078138518),
                Point2D(84.4755717762, 48.3489547183), Point2D(93.180568963, 77.3866096522),
                Point2D(56.9315671464, 96.1542835219)});
    return !is_inside(p, o);
}
bool test_is_inside_single_9() {
    Point2D p(79.9894845577, 18.8460331163);
    Obstacle o({Point2D(39.8875476884, 31.0032573573), Point2D(87.2034367153, 45.8279956123),
                Point2D(94.5397078305, 67.5786937823), Point2D(85.2840433198, 94.7050362252),
                Point2D(50.7825598701, 91.3571000169), Point2D(41.0703122186, 77.2537537973)});
    return !is_inside(p, o);
}
bool test_is_inside_single_10() {
    Point2D p(63.0056442991, 67.6514046751);
    Obstacle o({Point2D(37.8730543126, 61.7974583465), Point2D(85.9264225633, 37.5537283897),
                Point2D(86.3804983078, 96.4399295958), Point2D(57.1816671715, 98.3413839533),
                Point2D(42.6867116149, 90.1060720336)});
    return is_inside(p, o);
}

// List of 10 obstacles for list tests
list<Obstacle> get_obstacle_list() {
    return {
        Obstacle({Point2D(100.842452804, 164.654021519), Point2D(116.353736977, 137.850967413),
                  Point2D(136.243813839, 105.333065969), Point2D(158.838656479, 109.779414439),
                  Point2D(155.319158361, 132.683691538), Point2D(147.972251592, 149.653486997)}),
        Obstacle({Point2D(64.6470621904, 114.143147688), Point2D(110.729760719, 77.3406591468),
                  Point2D(127.735959376, 81.9899207248), Point2D(124.500389214, 100.201511431),
                  Point2D(122.27610335, 109.479219155), Point2D(113.429296361, 112.83524066),
                  Point2D(102.08262914, 115.577154738)}),
        Obstacle({Point2D(60.0698166548, 71.8465847019), Point2D(81.6035997025, 61.9937071744),
                  Point2D(100.762217359, 77.2914798238), Point2D(101.075871077, 111.187650542),
                  Point2D(74.1181422044, 109.067690419)}),
        Obstacle({Point2D(51.4068015547, 108.89266422), Point2D(57.1468013361, 55.837114315),
                  Point2D(102.8853305, 55.6548184783), Point2D(113.688825623, 69.844104241),
                  Point2D(118.295984428, 103.755681571), Point2D(99.1219678136, 114.658887093)}),
        Obstacle({Point2D(41.988276959, 74.9580864918), Point2D(80.7393283803, 43.4310895817),
                  Point2D(89.992433446, 46.8805716474), Point2D(95.8098068002, 72.1490774272),
                  Point2D(85.0995711639, 95.0253137299), Point2D(51.2866068223, 87.9758957995)}),
        Obstacle({Point2D(4.33327083323, 10.8970926268), Point2D(28.3656943355, 9.83741480443),
                  Point2D(66.6389299547, 29.6805076731), Point2D(18.7389207429, 62.6207686039),
                  Point2D(12.8318499517, 41.7302369378)}),
        Obstacle({Point2D(91.5721796612, 116.612430678), Point2D(107.476358329, 98.3075852781),
                  Point2D(156.200527189, 93.4163326961), Point2D(151.517101486, 126.585203854),
                  Point2D(132.857109845, 142.407327114)}),
        Obstacle({Point2D(117.364482831, 146.529976444), Point2D(127.753656508, 107.039017171),
                  Point2D(157.751900107, 108.824601927), Point2D(154.248077463, 125.966756488),
                  Point2D(143.827991514, 145.266818745), Point2D(122.897760833, 161.581894083)}),
        Obstacle({Point2D(54.7121770588, 108.686303018), Point2D(60.9323249875, 66.3769383976),
                  Point2D(99.4311607665, 69.9307307308), Point2D(103.959666741, 70.6234577611),
                  Point2D(116.053682879, 103.529463566), Point2D(98.7208691064, 116.225806026)}),
        Obstacle({Point2D(38.1956675047, 80.3937126391), Point2D(52.9242769177, 45.797950815),
                  Point2D(78.4178185785, 40.2160024155), Point2D(93.1868796671, 37.6565553121),
                  Point2D(94.9373031887, 100.329678598), Point2D(40.7750274647, 92.0877328275)})
    };
}

bool test_is_inside_list_1() { return is_inside(Point2D(33.80232244, 40.1306467411), get_obstacle_list()); }
bool test_is_inside_list_2() { return is_inside(Point2D(56.1146678827, 58.0697444317), get_obstacle_list()); }
bool test_is_inside_list_3() { return !is_inside(Point2D(2.5711417029, 66.5399477083), get_obstacle_list()); }
bool test_is_inside_list_4() { return !is_inside(Point2D(13.9783764019, 7.18968229302), get_obstacle_list()); }
bool test_is_inside_list_5() { return is_inside(Point2D(53.8564856441, 73.8540171214), get_obstacle_list()); }
bool test_is_inside_list_6() { return !is_inside(Point2D(6.73089310416, 98.2223586838), get_obstacle_list()); }
bool test_is_inside_list_7() { return is_inside(Point2D(39.1297616237, 26.4651184869), get_obstacle_list()); }
bool test_is_inside_list_8() { return is_inside(Point2D(56.7713301009, 94.4627403043), get_obstacle_list()); }
bool test_is_inside_list_9() { return is_inside(Point2D(45.3574516458, 83.7059860276), get_obstacle_list()); }
bool test_is_inside_list_10() { return !is_inside(Point2D(3.68370207946, 90.1151714186), get_obstacle_list()); }

// Intersects single obstacle
bool test_intersects_single_1() {
    Segment s(Point2D(53.6485359487, 71.7573541209), Point2D(69.3874962961, 34.4658272697));
    Obstacle o({Point2D(36.6290321439, 41.8994671744), Point2D(52.1251957812, 33.0951772166),
                Point2D(75.2708107577, 38.0282739993), Point2D(98.8217511127, 50.8970003448),
                Point2D(84.30376213, 85.5448503064), Point2D(55.3504039578, 98.1271932959)});
    return intersects(s, o);
}
bool test_intersects_single_2() {
    Segment s(Point2D(14.7290007744, 13.8771519447), Point2D(77.4607946959, 9.17086587972));
    Obstacle o({Point2D(30.5840491753, 50.0440744071), Point2D(47.7081703861, 34.0076471631),
                Point2D(92.7221932979, 38.4970722531), Point2D(96.5458123272, 73.378710831),
                Point2D(73.8904058265, 95.5070791394), Point2D(40.5244050775, 98.1061129741)});
    return !intersects(s, o);
}
bool test_intersects_single_3() {
    Segment s(Point2D(90.8719850374, 19.9214770172), Point2D(51.5671604039, 22.7666509953));
    Obstacle o({Point2D(45.6891765847, 94.2785534202), Point2D(56.1144285982, 40.9234337357),
                Point2D(91.6475153442, 33.3413007799), Point2D(95.2949662989, 71.3934236283),
                Point2D(45.6891765847, 94.2785534202), Point2D(0,0), Point2D(95.2949662989, 71.3934236283)});
    return !intersects(s, o);
}
bool test_intersects_single_4() {
    Segment s(Point2D(95.8070292705, 76.1811158047), Point2D(46.298081168, 3.18048536815));
    Obstacle o({Point2D(30.4105036719, 63.5598420041), Point2D(39.9236198234, 41.9763553484),
                Point2D(53.3124497852, 38.0092243153), Point2D(83.5256401232, 39.0128862767),
                Point2D(91.6469206227, 42.5133097878), Point2D(88.2948066618, 58.5690997787),
                Point2D(68.2670317047, 92.1525345464)});
    return intersects(s, o);
}
bool test_intersects_single_5() {
    Segment s(Point2D(19.4283899334, 26.6611203803), Point2D(77.5807481414, 67.5888638262));
    Obstacle o({Point2D(30.2658729148, 36.0234145015), Point2D(78.4641210407, 34.0819715187),
                Point2D(92.2943998568, 76.3802316436), Point2D(74.143143109, 93.4844314227)});
    return intersects(s, o);
}
bool test_intersects_single_6() {
    Segment s(Point2D(78.2744134642, 92.0983933979), Point2D(57.8443842748, 72.6851016948));
    Obstacle o({Point2D(33.5157502543, 36.5088501792), Point2D(88.622277622, 30.6467873426),
                Point2D(97.0063852438, 73.3300158362), Point2D(89.7144478479, 98.8075976103),
                Point2D(39.1093043755, 46.9477603642)});
    return !intersects(s, o);
}
bool test_intersects_single_7() {
    Segment s(Point2D(44.2710347319, 75.7237264322), Point2D(86.8329211775, 46.6393455266));
    Obstacle o({Point2D(32.919446024, 81.0494156919), Point2D(45.8017937962, 32.7410748981),
                Point2D(89.2868455708, 77.420606861), Point2D(77.4026958629, 90.5481090231),
                Point2D(34.0224751618, 93.784241695)});
    return intersects(s, o);
}
bool test_intersects_single_8() {
    Segment s(Point2D(72.0736208491, 18.5321891265), Point2D(84.0726874249, 4.14657613595));
    Obstacle o({Point2D(34.7040159874, 40.7946075512), Point2D(56.953941311, 42.5665013412),
                Point2D(76.1882451541, 47.1173076413), Point2D(86.5116968755, 74.3095508968),
                Point2D(90.8185937482, 90.086599891), Point2D(48.2732255292, 89.8481516159)});
    return !intersects(s, o);
}
bool test_intersects_single_9() {
    Segment s(Point2D(41.7514960248, 14.2325999655), Point2D(98.1958273684, 69.7768985977));
    Obstacle o({Point2D(33.5640836635, 71.3332652102), Point2D(44.2197283087, 35.7944012205),
                Point2D(97.1841588569, 70.9808181846), Point2D(78.7890248416, 96.4706719416),
                Point2D(67.2675819425, 95.3690831601)});
    return !intersects(s, o);
}
bool test_intersects_single_10() {
    Segment s(Point2D(12.0078472478, 69.0700167732), Point2D(85.7316416915, 73.1534024889));
    Obstacle o({Point2D(30.0738772232, 89.5853525411), Point2D(39.8839994033, 37.0839274931),
                Point2D(59.127764624, 31.3454027674), Point2D(92.1825697196, 33.7695834768),
                Point2D(55.0032370446, 95.3164702011)});
    return intersects(s, o);
}

// Intersects list of obstacles (using same list)
bool test_intersects_list_1() { return intersects(Segment(Point2D(44.0448039328, 58.2615040828), Point2D(64.5143634949, 30.4070153914)), get_obstacle_list()); }
bool test_intersects_list_2() { return intersects(Segment(Point2D(81.5792346562, 25.5179312254), Point2D(12.1239300937, 55.7397054033)), get_obstacle_list()); }
bool test_intersects_list_3() { return intersects(Segment(Point2D(74.4710327959, 66.3115341105), Point2D(85.6237566124, 78.3638045378)), get_obstacle_list()); }
bool test_intersects_list_4() { return intersects(Segment(Point2D(52.3555237663, 81.3182913464), Point2D(4.89671212604, 43.3535135246)), get_obstacle_list()); }
bool test_intersects_list_5() { return intersects(Segment(Point2D(68.7682610295, 58.3419372188), Point2D(7.11381521567, 80.2092637235)), get_obstacle_list()); }
bool test_intersects_list_6() { return intersects(Segment(Point2D(87.4112269582, 13.0885878663), Point2D(66.9773959799, 64.5278356649)), get_obstacle_list()); }
bool test_intersects_list_7() { return intersects(Segment(Point2D(51.3166601182, 98.4440405158), Point2D(58.6355094438, 81.5287496785)), get_obstacle_list()); }
bool test_intersects_list_8() { return intersects(Segment(Point2D(94.1236993023, 14.8740279712), Point2D(8.40431745859, 14.3314116815)), get_obstacle_list()); }
bool test_intersects_list_9() { return !intersects(Segment(Point2D(32.6116611098, 20.767878695), Point2D(18.1795497616, 39.1229578354)), get_obstacle_list()); }
bool test_intersects_list_10() { return intersects(Segment(Point2D(50.2936226658, 68.3890549204), Point2D(31.7795450083, 55.5458235569)), get_obstacle_list()); }

// Translate tests
bool test_translate_1() { return translate(Vector2D(453.515052536, 18.6670583411), -10.074323, 32.499154) == Vector2D(443.440729357, 51.1662126034); }
bool test_translate_2() { return translate(Vector2D(828.124425327, 663.14138786), -29.261235, 24.788839) == Vector2D(798.863190543, 687.930226883); }
bool test_translate_3() { return translate(Vector2D(721.287077183, 146.30619265), -94.747760, -6.834375) == Vector2D(626.53931712, 139.471817735); }
bool test_translate_4() { return translate(Vector2D(44.0497243914, 254.859435366), -40.918947, 22.227622) == Vector2D(3.1307775312, 277.087057626); }
bool test_translate_5() { return translate(Vector2D(486.034442122, 564.540298958), 78.426786, -53.094031) == Vector2D(564.461228096, 511.446268319); }
bool test_translate_6() { return translate(Vector2D(451.633606516, 27.3802689371), 9.218274, -82.618266) == Vector2D(460.851880034, -55.2379968852); }
bool test_translate_7() { return translate(Vector2D(148.846007408, 622.798284687), 32.280522, 97.864247) == Vector2D(181.126529901, 720.662531234); }
bool test_translate_8() { return translate(Vector2D(747.360660266, 825.570415626), -72.987158, -70.620180) == Vector2D(674.373502493, 754.950235217); }
bool test_translate_9() { return translate(Vector2D(635.464554956, 806.320754966), 2.663724, 42.577535) == Vector2D(638.128278458, 848.89829015); }
bool test_translate_10() { return translate(Vector2D(460.821735785, 407.290775849), -95.947423, -31.290299) == Vector2D(364.874312837, 376.000476829); }

// Rotate tests
bool test_rotate_1() { return rotate(Vector2D(873.72009961, 570.873954725), 60.0) == Vector2D(-57.5312974684, 1042.10077961); }
bool test_rotate_2() { return rotate(Vector2D(892.418643366, 895.881952292), 0.0) == Vector2D(892.418643366, 895.881952292); }
bool test_rotate_3() { return rotate(Vector2D(820.194136527, 672.527897496), 60.0) == Vector2D(-172.329175867, 1046.57290719); }
bool test_rotate_4() { return rotate(Vector2D(470.824134156, 547.782360837), 90.0) == Vector2D(-547.782360837, 470.824134156); }
bool test_rotate_5() { return rotate(Vector2D(707.743597929, 640.830838764), 0.0) == Vector2D(707.743597929, 640.830838764); }
bool test_rotate_6() { return rotate(Vector2D(613.69501244, 96.6093967948), 45.0) == Vector2D(365.63474518, 502.261064344); }
bool test_rotate_7() { return rotate(Vector2D(720.373907957, 64.5843921859), 0.0) == Vector2D(720.373907957, 64.5843921859); }
bool test_rotate_8() { return rotate(Vector2D(434.083994461, 601.267592913), 45.0) == Vector2D(-118.216656137, 732.104128152); }
bool test_rotate_9() { return rotate(Vector2D(680.586435178, 717.249675658), 60.0) == Vector2D(-280.863222542, 948.029980312); }
bool test_rotate_10() { return rotate(Vector2D(203.225324632, 365.951057989), 45.0) == Vector2D(-115.0644695, 402.468479736); }

// Rotate around tests
bool test_rotate_around_1() { return rotate_around(Vector2D(35.9066073499, 289.550942665), 0.0, Vector2D(952.638012326, 75.6400840674)) == Vector2D(35.9066073499, 289.550942665); }
bool test_rotate_around_2() { return rotate_around(Vector2D(882.957945821, 108.140670972), 30.0, Vector2D(810.965274666, 951.763888493)) == Vector2D(1295.12436555, 257.161086293); }
bool test_rotate_around_3() { return rotate_around(Vector2D(313.576767926, 366.360442893), 60.0, Vector2D(923.333721241, 806.668533859)) == Vector2D(999.773236946, 58.4494765394); }
bool test_rotate_around_4() { return rotate_around(Vector2D(128.932781509, 796.633066938), 90.0, Vector2D(236.664260085, 886.864877369)) == Vector2D(326.896070516, 779.133398793); }
bool test_rotate_around_5() { return rotate_around(Vector2D(507.35705823, 351.372789345), 45.0, Vector2D(911.280360951, 50.5950195544)) == Vector2D(412.981453999, -22.3398862103); }
bool test_rotate_around_6() { return rotate_around(Vector2D(589.245136089, 886.750693524), 60.0, Vector2D(120.849251864, 592.13276356)) == Vector2D(99.9005821538, 1145.08446341); }
bool test_rotate_around_7() { return rotate_around(Vector2D(834.603700892, 638.038436658), 60.0, Vector2D(416.176086716, 560.078263695)) == Vector2D(557.874403517, 961.427293788); }
bool test_rotate_around_8() { return rotate_around(Vector2D(692.083638326, 240.031799907), 0.0, Vector2D(543.170259369, 604.182484962)) == Vector2D(692.083638326, 240.031799907); }
bool test_rotate_around_9() { return rotate_around(Vector2D(496.610131609, 58.4613661339), 0.0, Vector2D(838.380494932, 526.14121529)) == Vector2D(496.610131609, 58.4613661339); }
bool test_rotate_around_10() { return rotate_around(Vector2D(49.3702092369, 529.609921364), 0.0, Vector2D(929.866779387, 202.029172059)) == Vector2D(49.3702092369, 529.609921364); }

// Build vision area tests
bool test_build_va_0() {
    auto va = build_vision_area(Vector2D(711.873589966, 337.462690675), Vector2D(0.105767425526, 0.994390894818), 13.282032, 60.0);
    return get<0>(va) == Vector2D(711.873589966, 337.462690675) &&
           get<1>(va) == Vector2D(724.014051285, 342.849858634) &&
           get<2>(va) == Vector2D(701.137934997, 345.283054612) &&
           real_equal(get<3>(va), 13.282032);
}
bool test_build_va_1() {
    auto va = build_vision_area(Vector2D(412.287777988, 560.185205837), Vector2D(0.994950277681, 0.100369043751), 42.051654, 90.0);
    return get<0>(va) == Vector2D(412.287777988, 560.185205837) &&
           get<1>(va) == Vector2D(416.508462247, 518.345901419) &&
           get<2>(va) == Vector2D(408.067093729, 602.024510254) &&
           real_equal(get<3>(va), 42.051654);
}
bool test_build_va_2() {
    auto va = build_vision_area(Vector2D(14.018628577, 695.721836162), Vector2D(0.733648411268, 0.679529255179), 28.290768, 45.0);
    return get<0>(va) == Vector2D(14.018628577, 695.721836162) &&
           get<1>(va) == Vector2D(42.2886742256, 694.63920441) &&
           get<2>(va) == Vector2D(15.1012603289, 723.991881803) &&
           real_equal(get<3>(va), 28.290768);
}
bool test_build_va_3() {
    auto va = build_vision_area(Vector2D(326.427706268, 245.848718262), Vector2D(0.0609005260461, 0.998143840299), 18.905249, 90.0);
    return get<0>(va) == Vector2D(326.427706268, 245.848718262) &&
           get<1>(va) == Vector2D(345.297864191, 244.697378648) &&
           get<2>(va) == Vector2D(307.557548345, 247.000057877) &&
           real_equal(get<3>(va), 18.905249);
}
bool test_build_va_4() {
    auto va = build_vision_area(Vector2D(30.3985522021, 340.081682879), Vector2D(0.701890289109, 0.712285070779), 17.908701, 45.0);
    return get<0>(va) == Vector2D(30.3985522021, 340.081682879) &&
           get<1>(va) == Vector2D(48.3067693421, 340.213315781) &&
           get<2>(va) == Vector2D(30.2669192997, 357.989900014) &&
           real_equal(get<3>(va), 17.908701);
}
bool test_build_va_5() {
    auto va = build_vision_area(Vector2D(659.089222348, 779.539535852), Vector2D(0.970575383932, 0.240797475288), 26.898808, 90.0);
    return get<0>(va) == Vector2D(659.089222348, 779.539535852) &&
           get<1>(va) == Vector2D(665.56638732, 753.432215286) &&
           get<2>(va) == Vector2D(652.612057377, 805.646856418) &&
           real_equal(get<3>(va), 26.898808);
}
bool test_build_va_6() {
    auto va = build_vision_area(Vector2D(496.493545463, 569.62628293), Vector2D(0.885889193404, 0.463896903427), 35.852754, 30.0);
    return get<0>(va) == Vector2D(496.493545463, 569.62628293) &&
           get<1>(va) == Vector2D(532.315860801, 568.149217798) &&
           get<2>(va) == Vector2D(515.683879066, 599.910785473) &&
           real_equal(get<3>(va), 35.852754);
}
bool test_build_va_7() {
    auto va = build_vision_area(Vector2D(279.15728205, 546.12900229), Vector2D(0.362274231973, 0.932071553503), 30.368795, 60.0);
    return get<0>(va) == Vector2D(279.15728205, 546.12900229) &&
           get<1>(va) == Vector2D(309.171817427, 550.754081307) &&
           get<2>(va) == Vector2D(260.144578432, 569.809812904) &&
           real_equal(get<3>(va), 30.368795);
}
bool test_build_va_8() {
    auto va = build_vision_area(Vector2D(973.424568998, 166.55713959), Vector2D(0.939674269615, 0.342070558546), 32.550815, 90.0);
    return get<0>(va) == Vector2D(973.424568998, 166.55713959) &&
           get<1>(va) == Vector2D(984.559244408, 135.969976439) &&
           get<2>(va) == Vector2D(962.289893588, 197.144302741) &&
           real_equal(get<3>(va), 32.550815);
}
bool test_build_va_9() {
    auto va = build_vision_area(Vector2D(117.090646855, 852.15122798), Vector2D(0.870689242901, 0.491833551415), 13.641254, 30.0);
    return get<0>(va) == Vector2D(117.090646855, 852.15122798) &&
           get<1>(va) == Vector2D(130.731298087, 852.022941919) &&
           get<2>(va) == Vector2D(124.022071461, 863.900235442) &&
           real_equal(get<3>(va), 13.641254);
}

// is_inside VisionArea tests
bool test_vision_inside_1() {
    VisionArea va{Vector2D(0,50), Vector2D(2.12020107231e-15,15.3744888112), Vector2D(0,84.6255111888), 34.625511};
    return !is_inside(Point2D(88.9188196268,21.7302019945), va);
}
bool test_vision_inside_2() {
    VisionArea va{Vector2D(0,50), Vector2D(12.1494688006,37.8505311994), Vector2D(12.1494687974,62.1494687974), 17.181944};
    return is_inside(Point2D(10.5531104268,42.3841627006), va);
}
bool test_vision_inside_3() {
    VisionArea va{Vector2D(0,50), Vector2D(21.1247566574,28.8752433426), Vector2D(21.1247566518,71.1247566518), 29.874917};
    return !is_inside(Point2D(56.3426777906,29.6113015898), va);
}
bool test_vision_inside_4() {
    VisionArea va{Vector2D(0,50), Vector2D(48.5718160881,50), Vector2D(48.5718160881,50), 48.571816};
    return !is_inside(Point2D(55.3145606558,40.3743281818), va);
}
bool test_vision_inside_5() {
    VisionArea va{Vector2D(0,50), Vector2D(17.9129379696,50), Vector2D(17.9129379696,50), 17.912938};
    return !is_inside(Point2D(56.3899050601,41.1761539624), va);
}
bool test_vision_inside_6() {
    VisionArea va{Vector2D(0,50), Vector2D(16.8472629476,40.2732282021), Vector2D(16.8472629518,59.7267717979), 19.453544};
    return !is_inside(Point2D(93.0879410992,14.039535151), va);
}
bool test_vision_inside_7() {
    VisionArea va{Vector2D(0,50), Vector2D(23.4980775563,9.30013579236), Vector2D(23.4980775563,90.6998642178), 46.996155};
    return !is_inside(Point2D(46.5345535194,7.78456596931), va);
}
bool test_vision_inside_8() {
    VisionArea va{Vector2D(0,50), Vector2D(15.8600220198,34.1399779802), Vector2D(15.8600220156,65.8600220156), 22.429458};
    return !is_inside(Point2D(38.905242278,0.326704109972), va);
}
bool test_vision_inside_9() {
    VisionArea va{Vector2D(0,50), Vector2D(25.8794140438,50), Vector2D(25.8794140438,50), 25.879414};
    return !is_inside(Point2D(81.3705995645,28.7693975702), va);
}
bool test_vision_inside_10() {
    VisionArea va{Vector2D(0,50), Vector2D(16.3554936968,50), Vector2D(16.3554936968,50), 16.355494};
    return !is_inside(Point2D(69.1832770474,45.7693992134), va);
}

// --- Convex Hull Tests ---
bool test_convex_hull_basic() {
    vector<Point2D> pts = {Point2D(0, 3), Point2D(2, 2), Point2D(1, 1), Point2D(2, 1),
                           Point2D(3, 0), Point2D(0, 0), Point2D(3, 3)};
    vector<Point2D> graham = convex_hull_graham(pts);
    vector<Point2D> jarvis = convex_hull_jarvis(pts);
    return graham.size() == 4 && jarvis.size() == 4;
}

bool test_convex_hull_triangle() {
    vector<Point2D> pts = {Point2D(0, 0), Point2D(5, 0), Point2D(0, 5), Point2D(1, 1)};
    vector<Point2D> graham = convex_hull_graham(pts);
    return graham.size() == 3;
}

// ----------------------------------------------------------------------
int main() {
    int total = 0;

    // Section 1: is_inside single obstacle (3 points)
    if (run_section("is_inside (single obstacle)", 3,
        test_is_inside_single_1, test_is_inside_single_2, test_is_inside_single_3,
        test_is_inside_single_4, test_is_inside_single_5, test_is_inside_single_6,
        test_is_inside_single_7, test_is_inside_single_8, test_is_inside_single_9,
        test_is_inside_single_10)) total += 3;

    // Section 2: is_inside list of obstacles (2 points)
    if (run_section("is_inside (list of obstacles)", 2,
        test_is_inside_list_1, test_is_inside_list_2, test_is_inside_list_3,
        test_is_inside_list_4, test_is_inside_list_5, test_is_inside_list_6,
        test_is_inside_list_7, test_is_inside_list_8, test_is_inside_list_9,
        test_is_inside_list_10)) total += 2;

    // Section 3: intersects single obstacle (2 points)
    if (run_section("intersects (single obstacle)", 2,
        test_intersects_single_1, test_intersects_single_2, test_intersects_single_3,
        test_intersects_single_4, test_intersects_single_5, test_intersects_single_6,
        test_intersects_single_7, test_intersects_single_8, test_intersects_single_9,
        test_intersects_single_10)) total += 2;

    // Section 4: intersects list of obstacles (2 points)
    if (run_section("intersects (list of obstacles)", 2,
        test_intersects_list_1, test_intersects_list_2, test_intersects_list_3,
        test_intersects_list_4, test_intersects_list_5, test_intersects_list_6,
        test_intersects_list_7, test_intersects_list_8, test_intersects_list_9,
        test_intersects_list_10)) total += 2;

    // Section 5: translate (2 points)
    if (run_section("translate", 2,
        test_translate_1, test_translate_2, test_translate_3, test_translate_4,
        test_translate_5, test_translate_6, test_translate_7, test_translate_8,
        test_translate_9, test_translate_10)) total += 2;

    // Section 6: rotate (2 points)
    if (run_section("rotate", 2,
        test_rotate_1, test_rotate_2, test_rotate_3, test_rotate_4,
        test_rotate_5, test_rotate_6, test_rotate_7, test_rotate_8,
        test_rotate_9, test_rotate_10)) total += 2;

    // Section 7: rotate_around (2 points)
    if (run_section("rotate_around", 2,
        test_rotate_around_1, test_rotate_around_2, test_rotate_around_3,
        test_rotate_around_4, test_rotate_around_5, test_rotate_around_6,
        test_rotate_around_7, test_rotate_around_8, test_rotate_around_9,
        test_rotate_around_10)) total += 2;

    // Section 8: build_vision_area (2 points)
    if (run_section("build_vision_area", 2,
        test_build_va_0, test_build_va_1, test_build_va_2, test_build_va_3,
        test_build_va_4, test_build_va_5, test_build_va_6, test_build_va_7,
        test_build_va_8, test_build_va_9)) total += 2;

    // Section 9: is_inside VisionArea (3 points)
    if (run_section("is_inside (VisionArea)", 3,
        test_vision_inside_1, test_vision_inside_2, test_vision_inside_3,
        test_vision_inside_4, test_vision_inside_5, test_vision_inside_6,
        test_vision_inside_7, test_vision_inside_8, test_vision_inside_9,
        test_vision_inside_10)) total += 3;

    if (run_section("Convex Hull Algorithms", 4, 
        test_convex_hull_basic, 
        test_convex_hull_triangle)) {
        total += 4;
    }

    cout << "\n============================================================" << endl;
    cout << "FINAL SCORE: " << total << " / 20" << endl;
    cout << "============================================================" << endl;

    return (total == 20) ? EXIT_SUCCESS : EXIT_FAILURE;
}