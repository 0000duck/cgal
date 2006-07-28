// Copyright (c) 2006 Fernando Luis Cacciola Carballal. All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#ifndef CGAL_STRAIGHT_SKELETON_BUILDER_TRAITS_2_AUX_H
#define CGAL_STRAIGHT_SKELETON_BUILDER_TRAITS_2_AUX_H 1

#include <CGAL/tags.h>
#include <CGAL/Uncertain.h>
#include <CGAL/certified_numeric_predicates.h>
#include <CGAL/Quotient.h>
#include <CGAL/certified_quotient_predicates.h>
#include <CGAL/Unfiltered_predicate_adaptor.h>
#include <CGAL/Filtered_predicate.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <boost/tuple/tuple.hpp>
#include <boost/optional/optional.hpp>
#include <boost/none.hpp>

#ifdef CGAL_STRAIGHT_SKELETON_TRAITS_ENABLE_TRACE
#  include<string>
#  include<iostream>
#  include<sstream>
#  include<iomanip>
bool sEnableTraitsTrace = false ;
#  define CGAL_STSKEL_TRAITS_ENABLE_TRACE_IF(cond) if ((cond)) sEnableTraitsTrace = true ;
#  define CGAL_STSKEL_TRAITS_DISABLE_TRACE sEnableTraitsTrace = false;
#  define CGAL_STSKEL_TRAITS_TRACE(m) \
     if ( sEnableTraitsTrace ) \
     { \
       std::ostringstream ss ; \
       ss << std::setprecision(19) << m << std::ends ; \
       std::string s = ss.str(); \
       Straight_skeleton_traits_external_trace(s); \
     }
#else
#  define CGAL_STSKEL_TRAITS_ENABLE_TRACE_IF(cond)
#  define CGAL_STSKEL_TRAITS_DISALBE_TRACE
#  define CGAL_STSKEL_TRAITS_TRACE(m)
#endif

CGAL_BEGIN_NAMESPACE

namespace CGAL_SS_i {

using boost::optional ;

// boost::make_optional is provided in Boost >= 1.34, but not before, so we define our own versions here.
template<class T> optional<T> cgal_make_optional( T const& v ) { return optional<T>(v) ; }
template<class T> optional<T> cgal_make_optional( bool cond, T const& v ) { return cond ? optional<T>(v) : optional<T>() ; }

template<class K>
struct Is_filtering_kernel
{
  typedef Tag_false type ;
} ;

template<>
struct Is_filtering_kernel< Exact_predicates_inexact_constructions_kernel >
{
  typedef Tag_true type ;
} ;

//
// This is the same as Filtered_construction but uses optional<result> instead of exceptions.
//
template <class AC
         ,class EC
         ,class FC
         ,class C2E
         ,class C2F
         ,class E2C
         ,class F2C
         ,bool Protection = true
>
class Exceptionless_filtered_construction
{
private:
  EC Exact_construction;
  FC Filter_construction;
  C2E To_Exact;
  C2F To_Filtered;
  E2C From_Exact;
  F2C From_Filtered;

  typedef typename AC::result_type  AC_result_type;
  typedef typename FC::result_type  FC_result_type;
  typedef typename EC::result_type  EC_result_type;

public:
  typedef AC_result_type           result_type;
  typedef typename AC::Arity       Arity;

public:

  Exceptionless_filtered_construction() {}

  template <class A1>
  result_type
  operator()(const A1 &a1) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      FC_result_type fr = Filter_construction(To_Filtered(a1));
      if ( fr )
        return From_Filtered(fr);
    }
    catch (Interval_nt_advanced::unsafe_comparison) {}

    Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
    EC_result_type er = Exact_construction(To_Exact(a1)) ;
    return From_Exact(er);
  }
  
  template <class A1, class A2>
  result_type
  operator()(const A1 &a1, const A2 &a2) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      FC_result_type fr = Filter_construction(To_Filtered(a1),To_Filtered(a2));
      if ( fr )
        return From_Filtered(fr);
    }
    catch (Interval_nt_advanced::unsafe_comparison) {}
    
    Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
    EC_result_type er = Exact_construction(To_Exact(a1), To_Exact(a2)) ;
    return From_Exact(er);
  }
  
  template <class A1, class A2, class A3>
  result_type
  operator()(const A1 &a1, const A2 &a2, const A3 &a3) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      FC_result_type fr = Filter_construction(To_Filtered(a1),To_Filtered(a2),To_Filtered(a3));
      if ( fr )
        return From_Filtered(fr);
    }
    catch (Interval_nt_advanced::unsafe_comparison) {}
    
    Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
    EC_result_type er = Exact_construction(To_Exact(a1), To_Exact(a2), To_Exact(a3)) ;
    return From_Exact(er);

  }
  
  template <class A1, class A2, class A3, class A4>
  result_type
  operator()(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      FC_result_type fr = Filter_construction(To_Filtered(a1),To_Filtered(a2),To_Filtered(a3),To_Filtered(a4));
      if ( fr )
        return From_Filtered(fr);
    }
    catch (Interval_nt_advanced::unsafe_comparison) {}
    
    Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
    EC_result_type er = Exact_construction(To_Exact(a1), To_Exact(a2), To_Exact(a3), To_Exact(a4)) ;
    return From_Exact(er);

  }
  
  template <class A1, class A2, class A3, class A4, class A5>
  result_type
  operator()(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5) const
  {
    try
    {
      Protect_FPU_rounding<Protection> P;
      FC_result_type fr = Filter_construction(To_Filtered(a1),To_Filtered(a2),To_Filtered(a3),To_Filtered(a4),To_Filtered(a5));
      if ( fr )
        return From_Filtered(fr);
    }
    catch (Interval_nt_advanced::unsafe_comparison) {}
    
    Protect_FPU_rounding<!Protection> P(CGAL_FE_TONEAREST);
    EC_result_type er = Exact_construction(To_Exact(a1), To_Exact(a2), To_Exact(a3), To_Exact(a4), To_Exact(a5)) ;
    return From_Exact(er);

  }
};


//
// This number type is provided because unlike Quotient<> is allows you to create it
// with a zero denominator.
//
template<class NT>
class Rational
{
  public:

    Rational( NT aN, NT aD ) : mN(aN), mD(aD) {}

    NT n() const { return mN ; }
    NT d() const { return mD ; }

    CGAL::Quotient<NT> to_quotient() const { return CGAL::Quotient<NT>(mN,mD) ; }

  private:

    NT mN, mD ;
} ;


//
// A straight skeleton event is the simultaneous coallision of 3 offseted oriented straight line segments
// e0*,e1*,e2* [e* denotes an _offseted_ edge].
//
// If the event is a an edge-event, the e0*->e1*->e2* must be consecutive right before the event so that
// after the event e0* and e2* become consecutive. Thus, there are _offset_ vertices (e0*,e1*) and (e1*,e2*) 
// in the offset polygon which not neccesarily exist in the original polygon.
//
// If the event is a split-event, e0*->e1* must be consecutive right before the event so that after the event
// e0->right(e2) and left(e2)->e1 become consecutive. Thus, there is an offset vertex (e0*,e1*) in the
// offset polygon which not neccesarily exist in the original polygon.
// 
// This record stores the segments corresponding to the INPUT edges (e0,e1,e2) whose offsets intersect
// at the event.
//
// The actual calculations for most computations depend on the collinearity of the input edges.
// Such collineariy is stored in this record to avoid determining it all the time in each computation.

// The offset vertices (e0*,e1*) and (e1*,e2*) are called the left and right seeds.
// A seed is a contour node if the vertex is already present in the input polygon, otherwise is a skeleton node.
// If a seed is a skeleton node,can be an input, is produced by an event so it is itself defined as a trisegment.
// The seed of a seed is neccesarily a contour node so the structure IS NOT recursive.
//
// If the seed is a skeleton node, an additional edge and collinearity is (optionally) stored in the nested record "Seed"
//
// 
template<class K>
class Trisegment_2
{
  public:

    typedef typename K::Segment_2 Segment_2 ;

    struct Seed
    {
      Seed ( Segment_2 const& aE, Trisegment_collinearity aCollinearity )
        :
        mE(aE), mCollinearity(aCollinearity)
      {}
        
      Segment_2               mE ;
      Trisegment_collinearity mCollinearity ;
    } ;
    typedef boost::optional<Seed> Optional_seed ;
    
  public:

    Trisegment_2() {}
      
    Trisegment_2( Segment_2 const&        aE0
                , Segment_2 const&        aE1
                , Segment_2 const&        aE2
                , Trisegment_collinearity aCollinearity 
                , Optional_seed const&    aLSeed
                , Optional_seed const&    aRSeed
                ) 
      :
      mRep( new Rep(aE0,aE1,aE2,aCollinearity,aLSeed,aRSeed) )
    {}

    Trisegment_collinearity collinearity() const { return mRep->mCollinearity ; }

    Segment_2 const& e( unsigned idx ) const { CGAL_assertion(idx<3) ; return mRep->mE[idx] ; }
    
    Segment_2 const& e0() const { return e(0) ; }
    Segment_2 const& e1() const { return e(1) ; }
    Segment_2 const& e2() const { return e(2) ; }

    Segment_2 const& collinear_edge_a  () const { return e(mRep->mCSIdx[0]) ; }
    Segment_2 const& collinear_edge_b  () const { return e(mRep->mCSIdx[1]) ; }
    Segment_2 const& non_collinear_edge() const { return e(mRep->mCSIdx[2]) ; }
    
    Optional_seed const& seed ( unsigned idx ) const { CGAL_assertion(idx<2) ; return mRep->mSeed[idx] ; }
    
    bool is_seed_a_skeleton_node( unsigned idx ) const { return !!seed(idx) ; }
    
    Trisegment_2 construct_seed_trisegment( unsigned idx ) const
    {
      CGAL_assertion( is_seed_a_skeleton_node(idx) ) ;
      
      Optional_seed null ;
      return Trisegment_2(e(idx),seed(idx)->mE,e(idx+1),seed(idx)->mCollinearity,null,null);
    }
    
    friend std::ostream& operator << ( std::ostream& os, Optional_seed const& aOSeed )
    {
      if ( aOSeed )
      {
        return os << " |[(" 
                  << aOSeed->mE.source().x() << "," << aOSeed->mE.source().y() 
                  << ")->(" 
                  << aOSeed->mE.target().x() << "," << aOSeed->mE.target().y()
                  << ")] "
                  << trisegment_collinearity_to_string(aOSeed->mCollinearity) 
                  << "|";
      }
      else return os << " |*|";
    }
    
    friend std::ostream& operator << ( std::ostream& os, Trisegment_2<K> const& aTrisegment )
    {
      return os << "{[(" 
                << aTrisegment.e0().source().x() << "," << aTrisegment.e0().source().y() 
                << ")->(" 
                << aTrisegment.e0().target().x() << "," << aTrisegment.e0().target().y()
                << ")] [(" 
                << aTrisegment.e1().source().x() << "," << aTrisegment.e1().source().y() 
                << ")->(" 
                << aTrisegment.e1().target().x() << "," << aTrisegment.e1().target().y()
                << ")] [(" 
                << aTrisegment.e2().source().x() << "," << aTrisegment.e2().source().y() 
                << ")->(" 
                << aTrisegment.e2().target().x() << "," << aTrisegment.e2().target().y()
                << ")]} " 
                << trisegment_collinearity_to_string(aTrisegment.collinearity()) 
                << aTrisegment.seed(0)
                << aTrisegment.seed(1);
    }

  private:

    struct Rep : public Ref_counted_base
    {
      Rep ( Segment_2 const&        aE0
          , Segment_2 const&        aE1
          , Segment_2 const&        aE2
          , Trisegment_collinearity aCollinearity 
          , Optional_seed const&    aLSeed
          , Optional_seed const&    aRSeed
          ) 
      {
        mCollinearity = aCollinearity ;
        mE[0]         = aE0 ;
        mE[1]         = aE1 ;
        mE[2]         = aE2 ;
        mSeed[0]      = aLSeed ;
        mSeed[1]      = aRSeed ;
        
        switch ( mCollinearity )
        {
          case TRISEGMENT_COLLINEARITY_01:
            mCSIdx[0]=0; mCSIdx[1]=1; mCSIdx[2]=2; break ;
            
          case TRISEGMENT_COLLINEARITY_12:
            mCSIdx[0]=1; mCSIdx[1]=2; mCSIdx[2]=0; break ;
            
          case TRISEGMENT_COLLINEARITY_02:
            mCSIdx[0]=0; mCSIdx[1]=2; mCSIdx[2]=1; break ;
            
          default:
            mCSIdx[0]=0; mCSIdx[1]=1; mCSIdx[2]=2; break ;
        }
      }
          
      Segment_2               mE[3];
      Trisegment_collinearity mCollinearity ;
      unsigned                mCSIdx[3] ;
      Optional_seed           mSeed[2] ;
    } ;
    
    boost::intrusive_ptr<Rep> mRep ;
} ;

template<class K>
struct Functor_base_2
{
  typedef typename K::FT        FT ;
  typedef typename K::Point_2   Point_2 ;
  typedef typename K::Segment_2 Segment_2 ;
  
  typedef Trisegment_2<K> Trisegment_2 ;
};

template<class Converter>
struct SS_converter : Converter
{
  typedef typename Converter::Source_kernel Source_kernel;
  typedef typename Converter::Target_kernel Target_kernel;

  typedef typename Source_kernel::FT Source_FT ;
  typedef typename Target_kernel::FT Target_FT ;

  typedef typename Source_kernel::Point_2 Source_point_2 ;
  typedef typename Target_kernel::Point_2 Target_point_2 ;

  typedef typename Source_kernel::Segment_2 Source_segment_2 ;
  typedef typename Target_kernel::Segment_2 Target_segment_2 ;

  typedef Trisegment_2<Source_kernel> Source_trisegment_2 ;
  typedef Trisegment_2<Target_kernel> Target_trisegment_2 ;

  typedef typename Source_trisegment_2::Seed  Source_trisegment_2_seed ;
  typedef typename Target_trisegment_2::Seed  Target_trisegment_2_seed ;
  
  typedef boost::tuple<Source_FT,Source_point_2> Source_time_and_point_2 ;
  typedef boost::tuple<Target_FT,Target_point_2> Target_time_and_point_2 ;
  
  typedef boost::optional<Source_point_2> Source_opt_point_2 ;
  typedef boost::optional<Target_point_2> Target_opt_point_2 ;
  
  typedef boost::optional<Source_time_and_point_2> Source_opt_time_and_point_2 ;
  typedef boost::optional<Target_time_and_point_2> Target_opt_time_and_point_2 ;
  
  typedef boost::optional<Source_segment_2> Source_opt_segment_2 ;
  typedef boost::optional<Target_segment_2> Target_opt_segment_2 ;
  
  typedef boost::optional<Source_trisegment_2> Source_opt_trisegment_2 ;
  typedef boost::optional<Target_trisegment_2> Target_opt_trisegment_2 ;
  
  typedef boost::optional<Source_trisegment_2_seed>  Source_trisegment_opt_seed_2 ;
  typedef boost::optional<Target_trisegment_2_seed>  Target_trisegment_opt_seed_2 ;
  
  Target_FT        cvtn(Source_FT const& n) const  { return this->Converter::operator()(n); }

  Target_point_2   cvtp(Source_point_2 const& p) const  { return this->Converter::operator()(p); }

  Target_segment_2 cvts( Source_segment_2 const& e) const { return Target_segment_2(cvtp(e.source()), cvtp(e.target()) ) ; }
  
  Target_time_and_point_2 cvttp( Source_time_and_point_2 const& v ) const
  {
    Source_FT      t ;
    Source_point_2 p ;
    boost::tie(t,p) = v ;
    return Target_time_and_point_2(cvtn(t),cvtp(p));
  }
  
  Target_trisegment_2_seed cvtseed( Source_trisegment_2_seed const& seed ) const
  {
    return  ;
  }  
  
  Target_trisegment_opt_seed_2 cvtseed( Source_trisegment_opt_seed_2 const& oseed ) const
  {
    if ( oseed )
         return Target_trisegment_opt_seed_2( Target_trisegment_2_seed( cvts(oseed->mE), oseed->mCollinearity ) ) ;
    else return Target_trisegment_opt_seed_2();
  }
  
  Target_trisegment_2 cvttri( Source_trisegment_2 const& t) const
  {
    return Target_trisegment_2(cvts(t.e0())
                              ,cvts(t.e1())
                              ,cvts(t.e2())
                              ,t.collinearity()
                              ,cvtseed(t.seed(0))
                              ,cvtseed(t.seed(1))
                              ) ;
    
  }
  
  Trisegment_collinearity  operator()(Trisegment_collinearity c) const { return c ; }
 
  Target_FT        operator()(Source_FT const& n) const { return cvtn(n) ; }

  Target_point_2   operator()( Source_point_2 const& p) const { return cvtp(p) ; }

  Target_segment_2 operator()( Source_segment_2 const& s) const { return cvts(s); }

  
  Target_trisegment_2 operator()( Source_trisegment_2 const& t) const
  {
    return cvttri(t);
  }
  
  Target_opt_trisegment_2 operator()( Source_opt_trisegment_2 const& t) const
  {
    if ( t )
         return Target_opt_trisegment_2(cvttri(*t)) ;
    else return Target_opt_trisegment_2();
    
  }
  Target_time_and_point_2 operator() ( Source_time_and_point_2 const& v ) const
  {
    return cvttp(v);
  }
  
  Target_opt_point_2 operator()( Source_opt_point_2 const& p) const 
  {
    if ( p ) 
         return Target_opt_point_2(cvtp(*p));
    else return Target_opt_point_2();
  }

  Target_opt_segment_2 operator()( Source_opt_segment_2 const& s) const 
  {
    if ( s ) 
         return Target_opt_segment_2(cvts(*s));
    else return Target_opt_segment_2();
  }
  
  Target_opt_time_and_point_2 operator()( Source_opt_time_and_point_2 const& v) const 
  { 
    if ( v ) 
         return Target_opt_time_and_point_2(cvttp(*v));
    else return Target_opt_time_and_point_2();
  }
  
  //boost::none_t operator() ( boost::none_t none ) const { return none ; } 
   
};

} // namespace CGAL_SS_i


//
// This macro defines a global functor adapter which allows users to use it in the followig ways:
//
// Given a 'Functor' provided by a given 'Traits' (or Kernel):
//
//   typedef typename CGAL::Functor<Traits>::type Functor ;
//   result r = CGAL::Functor<Traits>(traits)(a,b,c);
//
#define CGAL_STRAIGHT_SKELETON_CREATE_FUNCTOR_ADAPTER(functor) \
        template<class K> \
        typename K :: functor functor ( K const& aK ) \
        { \
          return aK.get((typename K :: functor const*)0);  \
        }


CGAL_END_NAMESPACE


#endif // CGAL_STRAIGHT_SKELETON_BUILDER_TRAITS_2_AUX_H //

// EOF //
