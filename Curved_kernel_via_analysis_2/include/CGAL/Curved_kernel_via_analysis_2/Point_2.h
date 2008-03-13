// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Eric Berberich <eric@mpi-inf.mpg.de>
//                 Pavel Emeliyanenko <asm@mpi-sb.mpg.de>
//
// ============================================================================

#ifndef CGAL_CURVED_KERNEL_POINT_2_H
#define CGAL_CURVED_KERNEL_POINT_2_H

/*! \file Curved_kernel_via_analysis_2/Point_2.h
 * \brief Defines class \c Point_2 that represents a point on a curve that can
 * be analyzed.
 */

#include <CGAL/basic.h>
#include <CGAL/Handle_with_policy.h>
#include <boost/optional.hpp>

#include <CGAL/Arr_enums.h>

#include <CGAL/Curved_kernel_via_analysis_2/Curved_kernel_via_analysis_2_functors.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

// forward class declaration
template < class CurvedKernelViaAnalysis_2, class Rep_ > 
class Point_2;

// forward class declaration
template < class CurvedKernelViaAnalysis_2 >
class Arc_2_rep;

// forward class declaration for befriending
template < class CurvedKernelViaAnalysis_2,
      class Rep_ = Arc_2_rep<CurvedKernelViaAnalysis_2> >
class Arc_2;

/*\!brief
 * representation class for Point_2
 */
template < class CurvedKernelViaAnalysis_2 >
class Point_2_rep 
{
public:
    //! this instance's template parameter
    typedef CurvedKernelViaAnalysis_2 Curved_kernel_via_analysis_2;
    
    //! the class itself
    typedef Point_2_rep< Curved_kernel_via_analysis_2 > Self;

    //! type of curve kernel
    typedef typename Curved_kernel_via_analysis_2::Curve_kernel_2 
    Curve_kernel_2;

    //! type of x-coordinate
    typedef typename Curve_kernel_2::X_coordinate_1 X_coordinate_1;
    
    //! type of a finite point on curve
    typedef typename Curve_kernel_2::Xy_coordinate_2 Xy_coordinate_2;

    //! type of curve analysis
    typedef typename Curve_kernel_2::Curve_analysis_2 Curve_analysis_2;
        
    //! default constructor
    Point_2_rep() {
    }
    
    //! constructs a "finite" point on curve,
    //! implies CGAL::NO_BOUNDARY in x/y
    Point_2_rep(const Xy_coordinate_2& xy) : 
        _m_xy(xy), _m_location(CGAL::ARR_INTERIOR) {
    }

    //! constructs a point at +/-oo in x
    Point_2_rep(CGAL::Arr_curve_end inf_end, const Curve_analysis_2& c,
                int arcno) :
        _m_curve(c), 
        _m_arcno(arcno) {
        _m_location = (inf_end == CGAL::ARR_MIN_END ?
                       CGAL::ARR_LEFT_BOUNDARY : CGAL::ARR_RIGHT_BOUNDARY);
    }
    
    //! constructs a point on curve with y-coordinate at infinity
    Point_2_rep(const X_coordinate_1& x, const Curve_analysis_2& c, 
                CGAL::Arr_curve_end inf_end) :
        _m_x(x),
        _m_curve(c) {
        _m_location = (inf_end == CGAL::ARR_MIN_END ?
                       CGAL::ARR_BOTTOM_BOUNDARY : CGAL::ARR_TOP_BOUNDARY);
        
    }

    //! curve point finite coordinates. They are valid only if boundary in y 
    //! is not set (CGAL::NO_BOUNDARY), otherwise only x-coordinate is
    //! accessible, i.e., point is in interior
    boost::optional< Xy_coordinate_2 > _m_xy;
        
    //! x-coordinate of a curve point
    boost::optional< X_coordinate_1 > _m_x;

    //! curve of point at boundary
    boost::optional< Curve_analysis_2 > _m_curve;

    //! arc of point at boundary
    boost::optional< int > _m_arcno;

    //! location of a point in parameter space
    mutable CGAL::Arr_parameter_space _m_location;
};

/*!\brief 
 * Class defines a point on a curve that can be analyzed
 * 
 * Only points with interior can be constructed explicitly 
 * (by the user). Points on the boundary use special private constructors and
 * required to represent ends of curve arcs on the boundary.
 */
template <class CurvedKernelViaAnalysis_2, 
          class Rep_ = CGALi::Point_2_rep<CurvedKernelViaAnalysis_2> >
class Point_2 : 
        public CGAL::Handle_with_policy< Rep_ > {
public:
    //!\name Public types
    //!@{
    
    //! this instance's first template parameter
    typedef CurvedKernelViaAnalysis_2 Curved_kernel_via_analysis_2;

    //! this instance's second template parameter
    typedef Rep_ Rep;

    //! this instance itself
    typedef Point_2< Curved_kernel_via_analysis_2, Rep > Self;
    
    //! type of underlying curve kernel
    typedef typename Curved_kernel_via_analysis_2::Curve_kernel_2
        Curve_kernel_2;
    
    //! type of x-coordinate
    typedef typename Curve_kernel_2::X_coordinate_1 X_coordinate_1;
    
    //! type of a finite point on curve
    typedef typename Curve_kernel_2::Xy_coordinate_2 Xy_coordinate_2;
    
    //! type that analyzes a curve
    typedef typename Curve_kernel_2::Curve_analysis_2 Curve_analysis_2;
    
    //! the handle superclass
    typedef ::CGAL::Handle_with_policy< Rep > Base;
    
    //! type of kernel point
    typedef typename Curved_kernel_via_analysis_2::Point_2 Kernel_point_2;
    
    //!@}
    
public:
    //!\name Rebind
    //!@{
    
    /*!\brief
     * An auxiliary structure for rebinding the point with a new rep
     */
    template < typename NewCKvA_2, typename NewRep >
    class rebind
    {
    public:
        //! this instance's first template parameter
        typedef NewCKvA_2 New_curved_kernel_via_analysis_2;

        //! this instance's second template parameter
        typedef NewRep New_rep;

        //! the rebound type
        typedef Point_2< New_curved_kernel_via_analysis_2, NewRep > Other;
        
        //! the rebound point
        typedef typename New_curved_kernel_via_analysis_2::Point_2 
        Rebound_point_2;
        
        /*!\brief
         * constructs a point of type \c Rebound_point_2 from the point \c pt 
         * of type \c Self.
         *
         * All known items of the base class rep will be copied.
         */
        Rebound_point_2 operator()(const Self& pt) {
            New_rep newrep;
            newrep._m_xy = pt.ptr()->_m_xy;
            newrep._m_x = pt.ptr()->_m_x;
            newrep._m_curve = pt.ptr()->_m_curve;
            newrep._m_arcno = pt.ptr()->_m_arcno;
            newrep._m_location = pt.ptr()->_m_location;
            return Rebound_point_2(newrep);
        }

        // TODO move to SfP_2l
        /*!\brief
         * reverse rebind, i.e., extracts original point type from a 
         * rebound instance
         */
        Self operator()(const Rebound_point_2& pt) {
            Rep rep;
            rep._m_xy = pt.ptr()->_m_xy;
            rep._m_x = pt.ptr()->_m_x;
            rep._m_curve = pt.ptr()->_m_curve;
            rep._m_arcno = pt.ptr()->_m_arcno;
            if (pt.is_finite()) {
                rep._m_location = CGAL::ARR_INTERIOR;
            } else {
                rep._m_location = pt.ptr()->_m_location;
            }
            return Self(rep);
        }
    };
    
public:
    //!\name Standard constructors
    //!@{

    /*!\brief
     * Default constructor
     */
    Point_2() : 
        Base(Rep()) {   
    }

    /*!\brief
     * copy constructor
     */
    Point_2(const Self& p) : 
            Base(static_cast<const Base&>(p)) {  
    }

    //!@}

public:
    //!\name Usual constructors
    //!@{
    
    /*\brief 
     * Constructs an interior point
     *
     * \param x The x-coordinate 
     * \param c The supporting curve
     * \param arcno Arcno of point on \c c
     * \return The constructed point
     */
    Point_2(const X_coordinate_1& x, const Curve_analysis_2& c, int arcno) :
        Base(Rep(Xy_coordinate_2(x, c, arcno))) {
    }

    // FUTURE TODO allow to construct without curve, 
    // i.e, isolated points on toric identifications -> do it also for arcs
    // FUTURE TODO parameter space in x/y (full set of tasks)
    
    //!@}

protected:
    //!\name Special constructors for points on the boundary
    //!@{
    
    /*!\brief 
     * Constructs a point with x-coordinate at the left/right boundary
     *
     * \param inf_end Determines whether point is on left or right boundary
     * \param c The supporting curve
     * \param arcno Arcno of point on \c on left/right boundary
     * \return The constructed point
     */
    Point_2(CGAL::Arr_curve_end inf_end, 
            const Curve_analysis_2& c, int arcno) :
        Base(Rep(inf_end, c, arcno)) {
    }
    
    /*!\brief 
     * Constructs a point on bottom or top boundary
     * 
     * \param x The x-coordinate of point
     * \param c The supporting curve
     * \param inf_end Defines whether point is on bottom or top boundary
     * \return The constructed point
     */
    Point_2(const X_coordinate_1& x, const Curve_analysis_2& c, 
            CGAL::Arr_curve_end inf_end) :
        Base(Rep(x, c, inf_end)) {
    }
    
    //!@}

protected:
    //!\name Constructors for rebind
    //!@{
    
    /*!\brief
     * constructs from a given represenation
     */
    /*!\brief
     * Constructor for for rebind
     *
     * \param rep The representation
     * \return The constructed point
     */
    Point_2(Rep rep) : 
        Base(rep) {  
    }
    
    //!@}
       
public:
    //!\name Destructors
    //!@{

    /*!\brief
     * Virtual destructor
     */
    virtual ~Point_2() {
    }
    
    //!@}

public:
    //!\name Access members
    //!@{

    /*!\brief 
     * Access to the point's x-coordinate (y-coordinate can be undefined)
     * 
     * \return The stored x-coordinate
     * \pre the point's x must be finite (set by construction)
     */
    inline 
    const X_coordinate_1& x() const {
        CGAL_precondition_msg(
                this->ptr()->_m_xy || this->ptr()->_m_x,
                "Denied access to x-coordinate of the curve end \
            lying at x-infinity");
        return (is_finite() ?
                (*(this->ptr()->_m_xy)).x() : *(this->ptr()->_m_x));
    }
    
    /*!\brief 
     * Access to underlying \c Xy_coordinate_2 object
     *
     * \return The stored xy-coordinate
     * \pre finite x/y coordinates must be set by construction, i.e., interior
     * point
     */
    inline 
    const Xy_coordinate_2& xy() const {
        CGAL_precondition_msg(this->ptr()->_m_xy,
            "Denied access to the curve end lying at x/y-infinity");
        return *(this->ptr()->_m_xy);
    }

    /*!\brief
     * supporting curve of point
     *
     * \return supporting curve of point
     */
    inline 
    Curve_analysis_2 curve() const {
        CGAL_precondition_msg(
                this->ptr()->_m_xy || this->ptr()->_m_curve,
                "Denied access to the curve end lying at y-infinity");
        return (is_finite() ? 
                (*(this->ptr()->_m_xy)).curve() : *(this->ptr()->_m_curve));
    }
    
    /*!\brief
     * Arc number of point
     *
     * \return arcno of point
     * \pre Is not endpoint of a vertical ray or branch 
     */ 
    inline int arcno() const {
        CGAL_precondition_msg(this->ptr()->_m_xy ||
                              this->ptr()->_m_arcno,
            "Denied access to the curve end lying at y-infinity");
        return (is_finite() ? 
                (*(this->ptr()->_m_xy)).arcno() : *(this->ptr()->_m_arcno));
    }
    
    //!@}

public: 
    //!\name Methods for location
    //!@{
    
    /*!\brief
     * sets location of a point in parameter space to \c loc
     */
    inline
    void set_location(CGAL::Arr_parameter_space loc) const {
        this->ptr()->_m_location = loc;
    }
    
    /*!\brief
     * location of a point in parameter space
     *
     * \return location in parameter space
     */
    inline CGAL::Arr_parameter_space location() const { 
        return this->ptr()->_m_location; 
    } 
    
    /*!\brief
     * checks if the point lies on left/right boundary
     *
     * \return \c true if it lies on left/right boundary, \c false otherwise
     */
    inline bool is_on_left_right() const {
        return (location() == CGAL::ARR_LEFT_BOUNDARY ||
                location() == CGAL::ARR_RIGHT_BOUNDARY);
    }
    
    /*!\brief
     * checks if the point lies on bottom/top boundary
     *
     * \return \c true if it lies on bottom/top boundary, \c false otherwise
     */
    inline bool is_on_bottom_top() const {
        return (location() == CGAL::ARR_BOTTOM_BOUNDARY ||
                location() == CGAL::ARR_TOP_BOUNDARY);
    }

    /*!\brief
     * checks whether the point is finite
     *
     * \return \c true, if point is finite, \c false otherwise
     */
    inline 
    bool is_finite() const {
        return this->ptr()->_m_xy;
    }
    
    //!@}      
    
    //!\name Predicates
    //!@{

#define CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(X, Y, Z) \
    typename Curved_kernel_via_analysis_2::X Y = \
         Curved_kernel_via_analysis_2::instance().Z(); \

    /*!\brief
     * Compares x-coordinates of this point with another
     * 
     * \param q The other point
     * \return CGAL::LARGER if x(*this) > x(q);
     *         CGAL::SMALLER if x(*this) \< x(q);
     *         CGAL::EQUAL if x(*this) = x(q).
     * \pre compared points are in the interior of parameter space
     */
    inline
    CGAL::Comparison_result compare_x(const Kernel_point_2& q) const {
        CGAL_precondition(this->ptr()->_m_xy);
        CGAL_precondition(q.ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Compare_x_2, 
                                              compare_x_2, 
                                              compare_x_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return compare_x_2(*dynamic_cast< const Kernel_point_2* >(this), q);
    }

    /*!\brief 
     * Compares this point with another point  lexicographically
     * 
     * \param q The other point
     * \return CGAL::LARGER if x(*this) > x(q), 
     *                      or if x(8this) = x(q) and y(*this) > y(q);
     *         CGAL::SMALLER if x(*this) \< x(q), 
     *                       or if x(*this) = x(q) and y(*this) \< y(q);
     *         CGAL::EQUAL if the two points are equal.
     * \pre Compared points are in the interior of parameter space
     */
    inline
    CGAL::Comparison_result compare_xy(const Kernel_point_2& q, 
                                       bool equal_x = false) const {
        CGAL_precondition(this->ptr()->_m_xy);
        CGAL_precondition(q.ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Compare_xy_2, 
                                              compare_xy_2, 
                                              compare_xy_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return compare_xy_2(
                *dynamic_cast< const Kernel_point_2* >(this), q, equal_x
        );
    }

    /*!\brief 
     * Checks if a point lies on on a curve
     *
     * \param curve The curve to check
     * \return \c true, if *this lies on \c curve, \c false otherwise
     */
    inline 
    bool is_on(
         const typename Curved_kernel_via_analysis_2::Curve_2& curve
    ) const {
        CGAL_precondition(this->ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Is_on_2, 
                                              is_on_2, 
                                              is_on_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return is_on_2(*dynamic_cast< const Kernel_point_2* >(this), curve);
    }

#undef CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT
    
    //!@}

    //!\name Comparison operators for points in the interior of parameter space
    //!{

    //! equality
    inline
    bool operator == (const Kernel_point_2& q) const { 
        return this->compare_xy(q) == CGAL::EQUAL;
    }
    
    //! inequality
    inline
    bool operator != (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::EQUAL;
    }
    
    //! less than in (x,y) lexicographic order
    inline
    bool operator <  (const Kernel_point_2& q) const {
        return this->compare_xy(q) == CGAL::SMALLER;
    }
    
    //! less-equal in (x,y) lexicographic order
    inline
    bool operator <= (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::LARGER;
    }

    //! greater than in (x,y) lexicographic order
    inline
    bool operator >  (const Kernel_point_2& q) const {
        return this->compare_xy(q) == CGAL::LARGER;
    }

    //! greater-equal in (x,y) lexicographic order
    inline
    bool operator >= (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::SMALLER;
    }
    
    //!@}
    
public:
    
    //!\name IO
    //!@{
    
    /*!\brief 
     * writes point to \c os
     */
    void write(std::ostream& os) const {
        
        switch(::CGAL::get_mode(os)) {
        case ::CGAL::IO::PRETTY:
            os << "point@" << this->id() << "(";
            os << "sup@" << this->curve().id() << ", ";
            os << "loc=" << this->location() << "; ";
            if (this->location() != CGAL::ARR_LEFT_BOUNDARY &&
                this->location() != CGAL::ARR_RIGHT_BOUNDARY) {
                os << "x=" << NiX::to_double(this->x()); // TODO CGAL::
            } else {
                if (this->location() == CGAL::ARR_LEFT_BOUNDARY) {
                    os << "x=-oo";
                } else {
                    os << "x=+oo";
                }
            }
            os << ", ";
            os << std::flush;
            if (is_finite()) {
                
                typedef typename Curve_kernel_2::Boundary Rational;
                typename Curve_kernel_2::Lower_boundary_y_2 lower_boundary_y;
                typename Curve_kernel_2::Upper_boundary_y_2 upper_boundary_y;
                typename Curve_kernel_2::Refine_y_2 refine_y;
                
                Rational bound(10e-10);
                
                while (upper_boundary_y(xy()) - 
                       lower_boundary_y(xy()) > bound) {
                    refine_y(xy());
                }
                
                double yd = 
                    NiX::to_double(lower_boundary_y(xy())); // TODO CGAL::
                os << "y=" << yd;
            } else {
                switch (this->location()) {
                case CGAL::ARR_BOTTOM_BOUNDARY:
                    os << "y=-oo";
                    break;
                case CGAL::ARR_TOP_BOUNDARY:
                    os << "y=+oo";
                    break;
                case CGAL::ARR_LEFT_BOUNDARY:
                case CGAL::ARR_RIGHT_BOUNDARY: {
                    // TODO concept!
                    CGAL::Object obj = 
                        this->curve().asymptotic_value_of_arc(
                                this->location(), this->arcno()
                        );
                    CGAL::Arr_parameter_space ps;
                    if (CGAL::assign(ps, obj)) {
                        if (ps == CGAL::ARR_BOTTOM_BOUNDARY) {
                            os << "y=-oo(asym)";
                        } else {
                            os << "y=+oo(asym)";
                        }
                    } else {
                        X_coordinate_1 y;
                        CGAL_assertion_code(bool check =)
                            CGAL::assign(y, obj);
                        CGAL_assertion(check);
                        os << "y=" << NiX::to_double(y) << "(asym)";
                        // TODO CGAL::
                    }
                    break;
                }
                default:
                    os << "y=?";
                    break;
                }
            }
            os << ", ";
            if (this->ptr()->_m_xy || this->ptr()->_m_arcno) {
                os << "ARCNO=" << this->arcno();
            } else {
                os << "ARCNO=n/a";
            }
            os << ")";
            break;
        case ::CGAL::IO::BINARY:
            std::cerr << "BINARY format not yet implemented" << std::endl;
            break;
        default:
            // ASCII 
            std::cerr << "ASCII format not yet implemented" << std::endl;
        }
    }
    
    //!@}
  
    // friends ////////////////////////////////////////////////////////////////

    //! befriending the arc base
    friend class Arc_2< Curved_kernel_via_analysis_2 >;

    // befriending the functors
    
#define CGAL_BEFRIEND_CKvA_2_FUNCTOR(Z) \
    friend class Curved_kernel_via_analysis_2::Z; \
    friend class Curved_kernel_via_analysis_2_Functors:: \
    Z< Curved_kernel_via_analysis_2 >; \
    
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Construct_point_2);
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Compare_x_2);
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Compare_xy_2);

#undef CGAL_BEFRIEND_CKvA_2_FUNCTOR

}; // class Point_2


/*!\relates Point_2
 * \brief 
 * writes \c pt to \c os 
 */
template < class CurvedKernelViaAnalysis_2, class Rep_ >
std::ostream& operator <<(std::ostream& os,
    const Point_2< CurvedKernelViaAnalysis_2, Rep_ >& pt) {
    
    pt.write(os);
    return os;
}

} // namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_CURVED_KERNEL_POINT_2_H
// EOF
