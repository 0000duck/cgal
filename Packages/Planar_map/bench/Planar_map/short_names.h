#include <CGAL/config.h> // needed for the LONGNAME flag

#if defined(CGAL_CFG_LONGNAME_BUG)
#define Quotient                        Qt
#define Homogeneous                     Hs
#define Cartesian                       Cn
#define Simple_cartesian                SC
#define Filtered_kernel                 FKl
#define Segment_2                       St
#define Point_2                         Pt2
#define Topological_map                 TM
#define Planar_map_2                    PMp
#define Arrangement_2                   Ar
#define I_HalfedgeDS_iterator           IPI
#define I_HalfedgeDS_const_iterator     IPCI
#define In_place_list_iterator          IPLI
#define In_place_list_const_iterator    IPLCI
#define allocator                       All
#define Pm_traits_wrap_2                PmTW
#define Td_X_trapezoid                  TXT
#define PL_X_curve_plus                 PXCP

// #define bidirectional_iterator_tag      BIT

#if defined(_MSC_VER)
// Has no effect, probably bug in MSVC
#pragma warning(disable:4503)
#endif

#endif
