// Copyright (c) 2005, 2006 Fernando Luis Cacciola Carballal. All rights reserved.
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
// $URL: svn+ssh://fcacciola@scm.gforge.inria.fr/svn/cgal/trunk/Straight_skeleton_2/test/Straight_skeleton_2/test_offset_builder.cpp $
// $Id: test_offset_builder.cpp 33476 2006-08-22 08:24:07Z afabri $
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>

#include<boost/tokenizer.hpp>


bool sTestInner            = true  ;
bool sTestOuter            = true  ;
bool sTestOffsets          = true  ;
bool sVerbose              = false ;
bool sNoOp                 = false ;
bool sClassifyCases        = false ;
bool sDumpEPS              = false ;
bool sDumpDXF              = false ;
bool sLogFailures          = false ;
bool sAbortOnError         = false ;
bool sAcceptNonSimpleInput = false ;
bool sValidateGeometry     = false ; 
bool sDumpOffsetPolygons   = false ;
 
int sMaxShift       = 1 ;
int sMaxVertexCount = 0 ;

double           sOffset        = 0.0 ;
bool             sOffsetAtNodes = true ; 
size_t           sOffsetCount   = 2 ;
std::vector<int> sOffsetAtEntry ;

double sDX    = 0.0 ;
double sDY    = 0.0 ;
double sScale = 1.0 ;

//#define CGAL_STRAIGHT_SKELETON_ENABLE_INTRINSIC_TESTING

//#define CGAL_STRAIGHT_SKELETON_ENABLE_TRACE 4
//#define CGAL_STRAIGHT_SKELETON_TRAITS_ENABLE_TRACE    
//#define CGAL_STRAIGHT_SKELETON_ENABLE_VALIDITY_TRACE 
//#define CGAL_POLYGON_OFFSET_ENABLE_TRACE 3

//#define CGAL_STRAIGHT_SKELETON_PROFILING_ENABLED

bool lAppToLog = false ;
void Straight_skeleton_external_trace ( std::string m )
{
  std::ofstream out("sls_log.txt", ( lAppToLog ? std::ios::app | std::ios::ate : std::ios::trunc | std::ios::ate ) );
  out << std::setprecision(19) << m << std::endl << std::flush ; 
  lAppToLog = true ;
}
void Straight_skeleton_traits_external_trace ( std::string m )
{
  std::ofstream out("sls_log.txt", ( lAppToLog ? std::ios::app | std::ios::ate : std::ios::trunc | std::ios::ate ) ) ;
  out << std::setprecision(19) << m << std::endl << std::flush ; 
  lAppToLog = true ;
}

void error_handler ( char const* what, char const* expr, char const* file, int line, char const* msg )
{
  std::cerr << "CGAL error: " << what << " violation!" << std::endl
       << "Expr: " << expr << std::endl
       << "File: " << file << std::endl
       << "Line: " << line << std::endl;
  if ( msg != 0)
      std::cerr << "Explanation:" << msg << std::endl;
      
  if ( sAbortOnError )    
    std::exit(1);     
}

typedef std::vector<std::string>                     FP_filter_failure_list ;
typedef std::map<std::string,FP_filter_failure_list> FP_filter_failure_map ;
typedef std::map<std::string,int>                    FP_filter_success_map ;

FP_filter_failure_map sPredFailureMap ;
FP_filter_failure_map sConsFailureMap ;
FP_filter_success_map sPredSuccessMap ;
FP_filter_success_map sConsSuccessMap ;

int sTotalPredFailures = 0 ;
int sTotalConsFailures = 0 ;
int sTotalPredSuccess  = 0 ;
int sTotalConsSuccess  = 0 ;

void register_predicate_failure    ( std::string pred, std::string failure ) { sPredFailureMap[pred].push_back(failure); ++ sTotalPredFailures ; }
void register_construction_failure ( std::string cons, std::string failure ) { sConsFailureMap[cons].push_back(failure); ++ sTotalConsFailures ; }
void register_predicate_success    ( std::string pred) { ++ sPredSuccessMap[pred] ; ++ sTotalPredSuccess ; }
void register_construction_success ( std::string cons) { ++ sConsSuccessMap[cons] ; ++ sTotalConsSuccess ; }

#include <CGAL/test_sls_types.h>

#include <CGAL/Real_timer.h>

#include <CGAL/IO/Dxf_stream.h>

typedef CGAL::Dxf_stream<IK> DxfStream ;

using namespace std ;
using namespace CGAL ;

inline string to_string( double n ) { ostringstream ss ; ss << n ; return ss.str(); }
inline bool   is_even ( int n ) { return n % 2 == 0 ; }

struct Zone
{
  Zone()
    :
     SkeletonTime(0.0)
    ,ContouringTime(0.0)
  {}
    
  IRegionPtr Input ;
  
  ISlsPtr Skeleton ;
  ORegion Contours ;
  double  SkeletonTime ;
  double  ContouringTime ;
} ;

struct TestCase
{
  TestCase ( string aFilename )
    : 
    Filename(aFilename)
  {
  }
  
  string Filename ;
  
  Zone Inner ;
  Zone Outer ;
} ;

const int cOK = 0 ;
const int cNonSimpleInput = 1 ;
const int cDegenerateInput = 2 ;
const int cLargeInputIgnored = 3 ;
const int cFileNotFound = 4 ;
const int cExceptionCaught = 5 ;

const char* LoadExitCodeStr[] = { "OK"
                                , "Non-simple polygon"
                                , "Degenerate polygon"
                                , "Large sample (ignored on request)"
                                , "File not found"
                                , "Exception"
                                } ;
                                
const char* LoadExitCodeNM[] = {  "??         "
                                , "Non-simple "
                                , "Degenerate "
                                , "Large      "
                                , "Not found  "
                                , "Cannot load"
                                } ;
                               
const char* load_exit_code_to_str( int aCode ) { return LoadExitCodeStr[aCode] ; }

IRegionPtr load_region( string file, int aShift, int& rExitCode )
{
  IRegionPtr rRegion ;

  try
  {
    ifstream in(file.c_str()); 
    if ( in )
    {
      CGAL::set_ascii_mode(in);
  
      rRegion = IRegionPtr( new IRegion() ) ; 
  
      int ccb_count = 0 ;
      in >> ccb_count ;
      for ( int i = 0 ; i < ccb_count && in ; ++ i )
      {
        IPolygonPtr lPoly( new IPolygon() );
  
        int v_count = 0 ;
        in >> v_count ;
        if ( sMaxVertexCount == 0 || ( v_count <= sMaxVertexCount ) )
        {
          for ( int j = 0 ; j < v_count && in ; ++ j )
          {
            double x = 0.0, y = 0.0  ;
            in >> x >> y ;
            if ( in )
            {
              x += sDX ;
              y += sDY ;
              x *= sScale ;
              y *= sScale ;
              
              lPoly->push_back( IPoint(x,y) ) ;
            }  
          }
          
          if ( lPoly->size() >= 3 )
          {
            bool lIsSimple = is_simple_2(lPoly->begin(),lPoly->end(),IK());
              
            if ( sAcceptNonSimpleInput || ( !sAcceptNonSimpleInput && lIsSimple ) )
            {
              Orientation expected = ( i == 0 ? COUNTERCLOCKWISE : CLOCKWISE ) ;
      
              double area = to_double(polygon_area_2(lPoly->begin(),lPoly->end(),IK()));
      
              Orientation orientation = area > 0 ? CGAL::COUNTERCLOCKWISE : area < 0 ? CGAL::CLOCKWISE : CGAL::COLLINEAR ;
      
              if ( aShift > 0 )
                std::rotate(lPoly->begin(),lPoly->begin()+aShift,lPoly->end());
                
              if ( orientation == expected )
                   rRegion->push_back(lPoly);
              else rRegion->push_back( IPolygonPtr( new IPolygon(lPoly->rbegin(),lPoly->rend()) ) ) ;
            }
            else
            {
              rExitCode = cNonSimpleInput ;
            }
          }
          else 
          {
            rExitCode = cDegenerateInput ;
            rRegion = IRegionPtr();
            break;
          }  
        }
        else 
        {
          rExitCode = cLargeInputIgnored ;
        }  
      }
    }
    else
    {
      rExitCode = cFileNotFound ;
    }
  }
  catch ( exception const& x ) 
  { 
    rExitCode = cExceptionCaught ;
    cerr << "std::exception caught loading file: " << x.what() << endl ; 
  }
  catch ( ... ) 
  { 
    rExitCode = cExceptionCaught ;
    cerr << "Unhandled exception loading file." << endl ;
  }
  
  if ( rRegion && rRegion->size() == 0 )
  {
    rRegion = IRegionPtr();
  }
  
  return rRegion ;
}

void update_bbox ( IRegionPtr const& aRegion, boost::optional<Bbox_2>& rBBox )
{
  if ( aRegion )
  {
    for ( IRegion::const_iterator bit = aRegion->begin() ; bit != aRegion->end() ; ++ bit )
    {
      for( IPolygon::const_iterator vit = (*bit)->begin(); vit != (*bit)->end(); ++vit)
      {
        Bbox_2 lVBBox = vit->bbox() ;
        
        if( !rBBox )
             rBBox = lVBBox ;
        else rBBox = lVBBox + *rBBox ;
      }
    }
  }
}
 
template<class Region>
void dump_region_to_eps( Region const& aRegion, const char* aType, double aScale, ostream& rOut )
{
  typedef typename Region::value_type PolygonPtr ;
  typedef typename PolygonPtr::element_type Polygon ;
  typedef typename Region::const_iterator boundary_const_iterator ;
  typedef typename Polygon::const_iterator vertex_const_iterator ;
    
  for ( boundary_const_iterator bit = aRegion.begin() ; bit != aRegion.end() ; ++ bit )
  {
    vertex_const_iterator beg  = (*bit)->begin(); 
    vertex_const_iterator end  = (*bit)->end  (); 
    vertex_const_iterator last = end - 1 ;
    
    for( vertex_const_iterator curr = beg ; curr != end ; ++ curr )
    {
      vertex_const_iterator next = curr == last ? beg : curr + 1 ;
      
      rOut << aType << endl
           << aScale * curr->x() 
           << " " 
           << aScale * curr->y()
           << " "
           << aScale * next->x()
           << " "
           << aScale * next->y() 
           << " E\n";
      
    }
  }
}

void dump_skeleton_to_eps( ISls const& aSkeleton, double aScale, ostream& rOut )
{
  for(Halfedge_const_iterator hit = aSkeleton.halfedges_begin(); hit != aSkeleton.halfedges_end(); ++hit)
  {
    Halfedge_const_handle h = hit ;

    if( h->is_bisector() && is_even(h->id()) )
    { 
      rOut << "skel" << endl 
           << aScale * h->vertex()->point().x() 
           << " " 
           << aScale * h->vertex()->point().y()
           << " "
           << aScale * h->opposite()->vertex()->point().x()
           << " "
           << aScale * h->opposite()->vertex()->point().y() 
           << " E\n";
    }
  }
}

string change_extension ( string aFilename, string aNewExt )
{
  string::size_type pos = aFilename.find_last_of('.');
  if ( pos != string::npos )
       return aFilename.replace(pos,aFilename.length()-pos,aNewExt);
  else return aFilename + aNewExt ;  
}

void dump_to_eps ( TestCase const& aCase )
{
  boost::optional<Bbox_2> lBBox ;

  update_bbox(aCase.Inner.Input, lBBox ) ;
  update_bbox(aCase.Outer.Input, lBBox ) ;

  if ( lBBox )
  {
    double lScale = 1000 / (lBBox->xmax() - lBBox->xmin()) ;
  
    if ( lScale < 1 )
      lScale = 1 ;
      
    string lEpsName = change_extension(aCase.Filename,".eps") ;
    std::ofstream lOut( lEpsName.c_str() );
    if ( lOut )
    {
      if ( sVerbose )
        cout << "  Dumping results to " << lEpsName << endl ;
        
      lOut << "%!PS-Adobe-2.0 EPSF-2.0\n%%BoundingBox:" 
          << static_cast<int>(std::floor(lScale* lBBox->xmin()-1)) 
          << " " 
          << static_cast<int>(std::floor(lScale* lBBox->ymin()-1)) 
          << " "
          << static_cast<int>(std::ceil(lScale*lBBox->xmax()+1)) 
          << " " 
          << static_cast<int>(std::ceil(lScale*lBBox->ymax()+1))
          << std::endl;
    
      lOut << "%%EndComments\n"
              "gsave\n"
              "1.0 setlinewidth\n"
              "/border { 0 1 0 setrgbcolor } bind def\n"
              "/border_w { 0.1 setlinewidth } bind def\n"
              "/cont { 0 0 0 setrgbcolor } bind def\n"
              "/cont_w { 0.1 setlinewidth } bind def\n"
              "/skel { 1 0 0 setrgbcolor } bind def\n"
              "/skel_w { 1.0 setlinewidth } bind def\n"
              "% stroke - x1 y1 x2 y2 E\n"
              "/E {newpath moveto lineto stroke} bind def\n" 
          << std::endl;
    
      if ( aCase.Inner.Input )
        dump_region_to_eps(*aCase.Inner.Input,"border",lScale,lOut);
        
      if ( aCase.Outer.Input )
        dump_region_to_eps(*aCase.Inner.Input,"border",lScale,lOut);
        
      if ( aCase.Inner.Skeleton )
        dump_skeleton_to_eps(*aCase.Inner.Skeleton,lScale,lOut);
        
      if ( aCase.Outer.Skeleton )
        dump_skeleton_to_eps(*aCase.Outer.Skeleton,lScale,lOut);
            
      dump_region_to_eps(aCase.Inner.Contours,"cont",lScale,lOut);
      dump_region_to_eps(aCase.Outer.Contours,"cont",lScale,lOut);
            
      lOut << "grestore\nshowpage" << std::endl;
    }
  }
  
}
template<class Polygon>
void dump_polygon_to_dxf( Polygon const& aPolygon, Color aColor, string aLayer, DxfStream& rDXF )
{
  rDXF << aColor << Dxf_layer(aLayer) ;
  
  rDXF.write_closed_segment_chain_2(aPolygon.begin(),aPolygon.end()) ;
}


template<class Region>
void dump_region_to_dxf( Region const& aRegion, Color aColor, string aBaseLayer, DxfStream& rDXF )
{
  int lN = 0 ;
  for ( typename Region::const_iterator bit = aRegion.begin() ; bit != aRegion.end() ; ++ bit )
  {
    ostringstream ss ; ss << aBaseLayer << "_" << lN ;
    string lLayer = ss.str();
    dump_polygon_to_dxf(**bit,aColor,lLayer,rDXF);
    ++ lN ;
  }  
}

void dump_skeleton_to_dxf( ISls const& aSkeleton
                         , Color      aContourBisectorColor
                         , Color      aSkeletonBisectorColor
                         , Color      aPeakBisectorColor
                         , string     aLayer
                         , DxfStream& rDXF 
                         )
{
  rDXF << Dxf_layer(aLayer) ;
  
  for(Halfedge_const_iterator hit = aSkeleton.halfedges_begin(); hit != aSkeleton.halfedges_end(); ++hit)
  {
    Halfedge_const_handle h = hit ;

    if (  handle_assigned(h->opposite())
       && handle_assigned(h->vertex()) 
       && handle_assigned(h->opposite()->vertex())
       )
    {
      if( h->is_bisector() && is_even(h->id()) )
      {
        if ( h->is_inner_bisector() )
        {
          if ( h->slope() == ZERO )
               rDXF << aPeakBisectorColor ; 
          else rDXF << aSkeletonBisectorColor ;
        }
        else rDXF << aContourBisectorColor ;
        rDXF << ISegment( h->vertex()->point(), h->opposite()->vertex()->point() ) ;
      }  
    }   
  }
}

void dump_to_dxf ( TestCase const& aCase )
{
  string lDxfName = change_extension(aCase.Filename,".dxf") ;
  std::ofstream lDxfFile( lDxfName.c_str() );
  if ( lDxfFile )
  {
    if ( sVerbose )
      cout << "  Dumping results to " << lDxfName << endl ;
      
    DxfStream lDxf(lDxfFile);
    
    if ( aCase.Inner.Input )
    {
      if ( sVerbose )
        cout << "    Dumping input region. " << endl ;
      dump_region_to_dxf(*aCase.Inner.Input,BLUE,"Input",lDxf);
    }
    
    if ( aCase.Inner.Skeleton )
    {
      if ( sVerbose )
        cout << "    Dumping inner skeleton." << endl ;
      dump_skeleton_to_dxf(*aCase.Inner.Skeleton,YELLOW,GREEN,PURPLE,"InnerSkeleton",lDxf);
    }
      
    if ( aCase.Outer.Skeleton )
    {
      if ( sVerbose )
        cout << "    Dumping outer skeleton." << endl ;
      dump_skeleton_to_dxf(*aCase.Outer.Skeleton,YELLOW,GREEN,PURPLE,"OuterSkeleton",lDxf);
    }
          
    dump_region_to_dxf(aCase.Inner.Contours,GRAY,"InnerOffset",lDxf);
    dump_region_to_dxf(aCase.Outer.Contours,GRAY,"OuterOffset",lDxf);
    
  }
  
}

IPolygonPtr create_outer_frame ( IPolygon const& aOuter )
{
  IPolygonPtr rFrame  ;
  
  try
  {
    Bbox_2 lBbox = bbox_2(aOuter.begin(),aOuter.end());
    
    double w = lBbox.xmax() - lBbox.xmin();
    double h = lBbox.ymax() - lBbox.ymin();
    double s = std::sqrt(w*w+h*h);
    
    IFT lOffset = s * 0.3 ;
    
    boost::optional<IFT> lOptMargin = compute_outer_frame_margin(aOuter.begin(),aOuter.end(),lOffset) ;
    
    if ( lOptMargin )
    {
      double lMargin = to_double(*lOptMargin);
      
      double flx = lBbox.xmin() - lMargin ;
      double fhx = lBbox.xmax() + lMargin ;
      double fly = lBbox.ymin() - lMargin ;
      double fhy = lBbox.ymax() + lMargin ;
      
      rFrame = IPolygonPtr( new IPolygon() ) ;
      
      rFrame->push_back( IPoint(IFT(flx),IFT(fly)) );
      rFrame->push_back( IPoint(IFT(fhx),IFT(fly)) );
      rFrame->push_back( IPoint(IFT(fhx),IFT(fhy)) );
      rFrame->push_back( IPoint(IFT(flx),IFT(fhy)) );
    }
    else 
    {
      if ( sVerbose )
        cout << "  Unable to calculate outer margin for offset " << lOffset << endl ;
    }
  
  }
  catch ( exception const& x ) 
  { 
    if ( sVerbose )
      cout << "    Failed calculating outer frame margin: std::exception caught: " << x.what() << endl ; 
  }
  catch ( ... ) 
  { 
    if ( sVerbose )
      cout << "    Failed calculating outer frame margin: Unhandled exception." << endl ;
  }
  
  return rFrame ;
}

template<class Region, class Point>
bool is_point_inside_region( Region const& aRegion, Point const& aP )
{
  bool rR = true ;
  
  typedef typename Region::value_type PolygonPtr ;
  typedef typename PolygonPtr::element_type Polygon ;
  
  for ( typename Region::const_iterator bit = aRegion.begin() ; bit != aRegion.end() && rR ; ++ bit )
  {
    Polygon const& lPoly = **bit ;
    if ( oriented_side_2(lPoly.begin(),lPoly.end(),aP) == ON_NEGATIVE_SIDE )
      rR = false ;
  }  
  
  return rR ;
}

bool is_skeleton_valid( IRegion const& aRegion, ISls const& aSkeleton )
{
  bool rValid = aSkeleton.is_valid() ;
  if ( !rValid )
  {
    if ( sVerbose )
      cout << "    Failed: Skeleton invalid.\n" ; 
  }
  else
  {
    if( sValidateGeometry )
    {
      for(Vertex_const_iterator vit = aSkeleton.vertices_begin(); vit != aSkeleton.vertices_end() && rValid ; ++vit)
      {
        Vertex_const_handle v = vit ;
        if ( v->is_skeleton() )
        {
          if ( !is_point_inside_region(aRegion,v->point()) )
          {
            rValid = false ;
            if ( sVerbose )
              cout << "    Failed: skeleton node misplaced.\n" ; 
          }
        }
      }  
    }
  }
  
  return rValid ;
}

bool test_zone ( Zone& rZone )
{
  bool rOK = false ;
  
  Real_timer t ;
  t.start();
  
  try
  {  
    if ( sVerbose )
      cout << "    Building straight skeleton." << endl ; 
    
    ISlsBuilder builder ;
    for( IRegion::const_iterator bit = rZone.Input->begin(), ebit = rZone.Input->end() ; bit != ebit ; ++ bit )
      builder.enter_contour((*bit)->begin(),(*bit)->end());
    rZone.Skeleton = builder.construct_skeleton(false) ;
  }
  catch ( exception const& x ) 
  { 
    if ( sVerbose )
      cout << "    Failed: std::exception caught: " << x.what() << endl ; 
  }
  catch ( ... ) 
  { 
    if ( sVerbose )
      cout << "    Failed: Unhandled exception." << endl ;
  }
  
  t.stop();
  
  rZone.SkeletonTime = t.time();
  
  if ( rZone.Skeleton )
  {
    rOK = is_skeleton_valid(*rZone.Input,*rZone.Skeleton) ;
    
    if ( rOK && sTestOffsets )
    {
      set<double> lTimes ;
      
      double lMaxTime = 0.0 ;
      
      for ( Vertex_const_iterator  vit  = rZone.Skeleton->vertices_begin()
                                  ,evit = rZone.Skeleton->vertices_end  ()
            ; vit != evit
            ; ++ vit
          )
      {
        Vertex_const_handle v = vit ;
        if ( v->is_skeleton() )
        {
          double lTime = to_double(v->time());
          lTimes.insert(lTime);
          if ( lTime > lMaxTime )
            lMaxTime = lTime ;
        }
      }      
      
      vector<double> lOffsets ;
      
      if ( sOffsetAtNodes )
      {
        if ( sOffsetAtEntry.size() > 0 )
        {
          int lSize = std::distance(lTimes.begin(),lTimes.end());
          
          for ( std::vector<int>::const_iterator oi = sOffsetAtEntry.begin() ; oi != sOffsetAtEntry.end() ; ++ oi )
          {
            int lEntry = *oi ;
            if ( lEntry < lSize )
            {
              set<double>::const_iterator it = lTimes.begin();
              std::advance(it,lEntry);
              lOffsets.push_back(*it);
            }
          }
        }
        else
        {
          copy(lTimes.begin(),lTimes.end(), back_inserter(lOffsets) ) ;
        } 
      }
      else if ( sOffset > 0.0 )
      {
        double lOffset = sOffset ;
        for ( size_t i = 0 ; i < sOffsetCount && lOffset < lMaxTime ; ++ i )
        {
          lOffsets.push_back(lOffset);
          lOffset += sOffset ;
        }  
      }
      
      if ( lOffsets.size() > 0 )
      {
        double lAccTime = 0.0 ;
        int    lNumContours = 0 ;
        
        SlsConverter CvtSls ;
        
        OSlsPtr lOSkeleton = CvtSls(*rZone.Skeleton) ;
        
        CGAL_assertion( lOSkeleton->is_valid() ) ;
             
        OffsetBuilder lOffsetBuilder(*lOSkeleton);
        
        for ( vector<double>::const_iterator oit = lOffsets.begin() ; oit != lOffsets.end() ; ++ oit )
        {
          double lOffset = *oit ;
          if ( lOffset > 0.01 )
          {
            if ( sVerbose )
              cout << "    Building offset contours at " << lOffset << endl ; 
              
            ORegion lContours ;
            
            t.start();
            
            try
            {  
              lOffsetBuilder.construct_offset_contours(OFT(lOffset), std::back_inserter(lContours) );
            }
            catch ( exception const& x ) 
            { 
              rOK = false ;
              if ( sVerbose )
                cout << "      Failed: std::exception caught: " << x.what() << endl ; 
            }
            catch ( ... )
            { 
              rOK = false ;
              if ( sVerbose )
                cout << "      Failed: Unhandled exception." << endl ;
            }
            
            t.stop();
            
            if ( sVerbose )
            {
              cout << "      " << lContours.size() << " contours built." << endl ;
              for ( ORegion::const_iterator bit = lContours.begin() ; bit != lContours.end() ; ++ bit )
              {
                OPolygonPtr lBdry = *bit ;
                
                cout << "        " << lBdry->size() << " vertices." << endl ;
                
                if ( sDumpOffsetPolygons )
                {
                  cout << "          " ;
                  for ( OPolygon::const_iterator vit = lBdry->begin() ; vit != lBdry->end() ; ++ vit )
                    cout << "(" << vit->x() << "," << vit->y() << ") " ;
                  cout << endl ;
                }
              }  
            } 
            
            if ( sDumpOffsetPolygons)
            {
              for ( ORegion::const_iterator bit = lContours.begin() ; bit != lContours.end() ; ++ bit )
              {
                OPolygonPtr lBdry = *bit ;
                cout << "          " ;
                for ( OPolygon::const_iterator vit = lBdry->begin() ; vit != lBdry->end() ; ++ vit )
                  cout << "(" << vit->x() << "," << vit->y() << ") " ;
                cout << endl ;
              }  
            } 
            
            for ( ORegion::const_iterator bit = lContours.begin() ; bit != lContours.end() ; ++ bit )
            {
              if ( !is_simple_2((*bit)->begin(),(*bit)->end(),OK()) )
              {
                cout << "      Failed: Non-simple offset polygon # "
                      << ( bit - lContours.begin()) 
                      << " generated at offset: " 
                      << lOffset 
                      << " (#" << ( oit - lOffsets.begin() ) << ")"
                      << endl ;
              }
            }
            
            
            copy(lContours.begin(),lContours.end(),std::back_inserter(rZone.Contours));
            lAccTime += t.time();
            ++ lNumContours ;
          }
        }
        
        if ( lNumContours > 0 )
          rZone.ContouringTime = lAccTime / double(lNumContours) ;
      }
    }
  }  
  else
  {
    if ( sVerbose )
      cout << "    Failed." << endl ;
  }
  
  return rOK ;
}


int test( TestCase& rCase, int aShift )
{
  int rR = 0 ;
  
  if ( sVerbose )
    cout << "Testing " << rCase.Filename << "(" << aShift << ")" << endl ;
  
  int lLoadExitCode = 0 ;
  
  IRegionPtr lInnerRegion = load_region(rCase.Filename,aShift,lLoadExitCode);
  
  if ( lLoadExitCode != cOK && sVerbose )
  {
    cout << "Load exit code: " << load_exit_code_to_str(lLoadExitCode) << endl ;
  }
  
  if ( lInnerRegion )
  {
    rCase.Inner.Input = lInnerRegion ;
    
    if ( !sNoOp )
    {
      if ( sTestInner )
      {
        if ( sVerbose )
          cout << "  Testing inner zone" << endl ;
          
        if (test_zone(rCase.Inner) )
             rR = rR == 0 ? 1 : rR ;
        else rR = -1 ;  
      }
      
      if ( sTestOuter )
      {
        if ( sVerbose )
          cout << "  Testing outer zone" << endl ;
          
        IPolygonPtr lOuterPoly = lInnerRegion->front();
        IPolygonPtr lFrame = create_outer_frame(*lOuterPoly);
        if ( lFrame )
        {
          rCase.Outer.Input = IRegionPtr ( new IRegion ) ;
          rCase.Outer.Input->push_back(lFrame);
          rCase.Outer.Input->push_back( IPolygonPtr( new IPolygon(lOuterPoly->rbegin(),lOuterPoly->rend()) ) ) ;
          
          if ( test_zone( rCase.Outer ) )
               rR = rR == 0 ? 1 : rR ;
          else rR = -1 ;  
        }
      }
    }
    
    if ( sVerbose && rR != 0 )
    {
      cout << ( ( rR > 0 ) ? "  OK " : "  !!!!!!!!!!!FAILED!!!!!!!!!!!!! " ) 
            << " InnerSlsTime=" << rCase.Inner.SkeletonTime 
            << " InnerOffTime=" << rCase.Inner.ContouringTime
            << " OuterSlsTime=" << rCase.Outer.SkeletonTime 
            << " OuterOffTime=" << rCase.Outer.ContouringTime 
            << endl ;
    }
      
          
    if ( sClassifyCases )
    {
      if ( rR > 0 )
      {
        ofstream ok_cases("./test_sls_ok_cases.txt", ios::app | ios::ate );
        ok_cases << rCase.Filename << endl ;
      }
      else if ( rR < 0 )
      {
        ofstream failed_cases("./test_sls_failed_cases.txt", ios::app | ios::ate );
        failed_cases << rCase.Filename << endl ;
      }
      else
      {
        ofstream failed_cases("./test_sls_invalid_cases.txt", ios::app | ios::ate );
        failed_cases << rCase.Filename << endl ;
      }
    }
    
    if ( sDumpEPS )
      dump_to_eps(rCase);
      
    if ( sDumpDXF)
      dump_to_dxf(rCase);
  }
  
  cerr << ( rR > 0 ? "OK         " : ( rR < 0 ? "!!FAILED!! "  : LoadExitCodeNM[lLoadExitCode] )  ) 
       << rCase.Filename 
       << "(" << aShift << ")" ;
  
  if ( rR > 0 )
  {
    cerr << " ( "
          << (rCase.Inner.SkeletonTime + rCase.Inner.ContouringTime + rCase.Outer.SkeletonTime  + rCase.Outer.ContouringTime)
          << " secs) " ; 
  }
  
  cerr << endl ;
  
  return rR ;
}

int test( TestCase& rCase )
{
  int rR = 0 ;
  
  for ( int s = 0  ; s < sMaxShift ; ++ s )
    rR += test(rCase,s) ;
      
  return rR ;    
}

int main( int argc, char const* argv[] )
{
  cout << "Straight skeleton test program" << endl ;
  cout << setprecision(19);
  cerr << setprecision(19);
  
  CGAL::set_error_handler  (error_handler);
  CGAL::set_warning_handler(error_handler);
  
  int lExitCode = 0 ;
  
  bool lPrintUsage                     = true ;
  bool lContinueOnErrorAssertionFailed = false ;

  string lFolder ;
  
  vector<string> args ;
  
  string rspline ;
  
  for ( int aidx = 1 ; aidx < argc ; ++ aidx )
  {
    string arg(argv[aidx]);
    if ( arg[0] == '@' )
    {
      ifstream response(&arg[1]);
      while ( getline(response,rspline) )
        if ( !rspline.empty() )
          args.push_back(rspline);
    }
    else
    {
      args.push_back(arg);
    }
  }  
    
  for( vector<string>::const_iterator ait = args.begin() ; ait != args.end() ; ++ ait ) 
  {
    string arg = *ait ;
    
    switch ( arg[0] )
    {
      case '-' :
      {
        switch(arg[1])
        {
          case 'e' : sDumpEPS     = true ; break ;
          case 'd' : sDumpDXF     = true ; break ;
          case 'v' : sVerbose     = true ; break ;
          case 'l' : sDumpOffsetPolygons = true ; break ;
          case 'a' : sAbortOnError = true ; break ;
          case 'p' : sAcceptNonSimpleInput = true ; break ;
          case 'c' : lContinueOnErrorAssertionFailed = true; break ;
          case 'n' : sNoOp         = true ; break ;
          case 't' : sValidateGeometry = true ; break ;
          
          case 'r' : 
            if ( arg.length() > 2 ) 
              sMaxShift = atoi(&arg[2]) ; 
            break ;
            
          case 'm' : 
            if ( arg.length() > 2 ) 
              sMaxVertexCount = atoi(&arg[2]) ; 
            break ;
            
          case 'x' : 
            if ( arg.length() > 2 ) 
              sDX = atof(&arg[2]) ; 
            break ;
            
          case 'y' : 
            if ( arg.length() > 2 ) 
              sDY = atof(&arg[2]) ; 
            break ;
            
          case 's' : 
            if ( arg.length() > 2 ) 
              sScale = atof(&arg[2]) ; 
            break ;
            
          
          case 'f' : 
            if ( arg.length() > 2 ) 
              lFolder = string(&arg[2]) ; 
            break ;
            
          case 'o' : 
            if ( arg.length() > 2 ) 
            {
              string sopt(&arg[2]);
              if ( sopt == "!" )
              {
                sTestOffsets = false ;
                cout << "Offsetting disabled." << endl ;
              }
              else
              {
                if ( sopt[0] == '#' )
                {
                  sOffsetAtNodes = true ;
                  if ( sopt.length() > 1 )
                  {
                    cout << "Offsseting at nodes set. Using entries " ;
                    boost::tokenizer<> tok(sopt);
                    for( boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
                    {
                      int lEntry = atoi(beg->c_str());
                      sOffsetAtEntry.push_back(lEntry)  ; 
                      cout << lEntry << " " ;
                    }
                    cout << endl ;
                  }
                  else
                  {
                    sOffsetAtEntry.clear() ;
                    cout << "Offsseting at nodes set until full depth" << endl ;
                  }
                }
                else
                {
                  sOffsetAtNodes = false ;
                  string::size_type pos = sopt.find_first_of('x');
                  string lOffVal ;
                  string lOffCount ;
                  if ( pos != string::npos )
                  {
                    string soptl = sopt.substr(0,pos) ;
                    string soptr = sopt.substr(pos+1) ;
                    
                    sOffset      = atof(soptl.c_str()) ; 
                    
                    if ( soptr != "*" )
                    {
                      sOffsetCount = atoi(soptr.c_str()) ; 
                      cout << "Repeared Offset set at " << sOffset << " " << sOffsetCount << " times." << endl ;
                    }
                    else
                    {
                      sOffsetCount = -1 ;  
                      cout << "Repeared Offset set at " << sOffset << " until full depth" << endl ;
                    } 
                  }
                  else
                  {
                    sOffset = atof(sopt.c_str()) ; 
                    sOffsetCount = 1 ;
                    cout << "Single Offset set at " << sOffset << endl ;
                  }
                }
              }
            }
            break ;
            
          default: cerr << "Invalid option: " << arg << endl ; break ;
        }
      }  
      break ;
        
      case '~' :
      {
        if ( arg.length() > 1 ) 
        {
          switch(arg[1])
          {
            case 'i' : 
              cout << "Not testing inside the polygon" << endl ;
              sTestInner = false ; 
              break ;
            case 'o' : 
              cout << "Not testing outside the polygon" << endl ;
              sTestOuter = false ; 
              break ;
          }
        }
      }
      break ;
    }
  }
  
  if ( lContinueOnErrorAssertionFailed )
    set_error_behaviour(CONTINUE);
    
  if ( sVerbose ) 
    cout << "Verbose mode is ON" << endl ;
    
  if ( sDumpDXF ) 
    cout << "Dump to DXF is ON" << endl ;
    
  if ( sDumpEPS ) 
    cout << "Dump to EPS is ON" << endl ;
    
  if ( sAbortOnError ) 
    cout << "Abort on first error is ON" << endl ;
    
  if ( sMaxShift > 1 )
    cout << "Each polygon vertex sequence will be rotated up to " << sMaxShift << " times" << endl ;
    
  if ( sMaxVertexCount > 0 )
    cout << "Polygons with more than " << sMaxVertexCount << " will be ignored" << endl ;
    
  if ( sDX != 0.0 )
    cout << "DeltaX=" << sDX << endl ;
    
  if ( sDY != 0.0 )
    cout << "DeltaY=" << sDY << endl ;
      
  if ( sScale != 1.0 )
    cout << "Scale=" << sScale << endl ;
    
  if ( lFolder != "" ) 
    cout << "Input folder is " << lFolder << endl ;
    
  vector<TestCase> lCases ;
  for( vector<string>::const_iterator ait = args.begin() ; ait != args.end() ; ++ ait ) 
  {
    string arg = *ait ;
    
    if ( arg.length() > 1 )
      if ( arg.find(".poly") != string::npos )  
        lCases.push_back( TestCase( lFolder + arg ) );
  }
  
   
  if ( lCases.size() > 0  ) 
  {
    int lOK_cases     = 0 ;
    int lFailed_cases = 0 ;
    
    lPrintUsage = false ;
    for ( vector<TestCase>::iterator it = lCases.begin() ; it != lCases.end(); ++ it )
    {
      int lR = test(*it);
      if ( lR > 0 )
      {
        ++ lOK_cases ;
      }
      else if ( lR < 0 )
      {
        ++ lFailed_cases ;
        lExitCode = 1 ;
        if ( sAbortOnError )
          break ;
      }  
    }
    
    cout << "OK     cases: " << lOK_cases     << endl ;
    cout << "Failed cases: " << lFailed_cases << endl ;
    cout << endl ;

    if ( Uncertain<bool>::number_of_failures() > 0 )
    {    
      cout << "!! " << Uncertain<bool>::number_of_failures() << " uncertain conversion failures. " << endl ;
      cout << endl ;
    }

    int lTotalPred = sTotalPredFailures + sTotalPredSuccess ;  
    int lTotalCons = sTotalConsFailures + sTotalConsSuccess ;  

    if ( lTotalPred > 0 && lTotalCons > 0 )
    {
      cout << "Floating-point filter statistics:" << endl << endl ;
      
      cout << "  " << sTotalPredFailures << " predicate    failures  (" << ( sTotalPredFailures / lTotalPred * 100.0 ) << "%)" << endl ;
      cout << "  " << sTotalPredSuccess  << " predicate    successes " << endl ;
      cout << "  " << sTotalConsFailures << " construction failures  (" << ( sTotalConsFailures / lTotalCons * 100.0 ) << "%)" << endl ;
      cout << "  " << sTotalConsSuccess  << " construction successes " << endl ;
      
      cout << "  " << (sTotalPredFailures + sTotalConsFailures ) << " total failures  ("
           << ( (sTotalPredFailures + sTotalConsFailures ) / (lTotalPred + lTotalCons) * 100.0 ) << "%)" << endl ;
      
      cout << "  Details..." << endl ;
      
      for ( FP_filter_failure_map::iterator it = sPredFailureMap.begin(); it != sPredFailureMap.end() ; ++ it )
        cout << "    " << it->first << " : " << it->second.size() << " failures." << endl ; 
        
      for ( FP_filter_success_map::iterator it = sPredSuccessMap.begin(); it != sPredSuccessMap.end() ; ++ it )
        cout << "    " << it->first << " : " << it->second << " successes." << endl ; 
        
      for ( FP_filter_failure_map::iterator it = sConsFailureMap.begin(); it != sConsFailureMap.end() ; ++ it )
        cout << "    " << it->first << " : " << it->second.size() << " failures." << endl ; 
        
      for ( FP_filter_success_map::iterator it = sConsSuccessMap.begin(); it != sConsSuccessMap.end() ; ++ it )
        cout << "    " << it->first << " : " << it->second << " successes." << endl ; 
        
    }    
  }
  
  if ( lPrintUsage )
  {
    cout << "USAGE: test_sls <options> file0 file1 ... fileN" << endl
         << endl 
         << "  <options>: " << endl
         << endl 
         << "     -o<offset-choice>" << endl
         << "         !   Disabled" << endl 
         << "         #   At every node." << endl 
         << "         #N  At the offset number N in a set of all node offsets." << endl 
         << "         D   Just one offset at distance 'D'" << endl 
         << "         DxS 'S' evenly space offsets separated distance 'D'" << endl 
         << "         Dx* Evenly spaced offsets separated distance 'D'" << endl 
         << endl 
         << "     -a      Abort on first error." << endl
         << "     -p      Permissive mode. Accept non-simple input polygons." << endl
         << "     -c      Ignore errors." << endl
         << "     -e      Dumps result into an .eps file." << endl
         << "     -d      Dumps result into an .dxf file." << endl
         << "     -v      Verbose log." << endl 
         << "     -l      Dump offset polygons." << endl 
         << "     -n      No-op mode. Doesn't create skeletons." << endl 
         << "     -g      Validate skeleton edges are disjoint." << endl
         << "     -fPATH  Append PATH to each filename" << endl 
         << "     -rMAX   Rotate input vertex sequence by one vertex up to MAX times" << endl
         << "     -mMAX   Ignore polygons with a vertex count greater than MAX" << endl
         << endl 
         << "     ~i    Do not test the polygon inside." << endl 
         << "     ~o    Do not test the polygon outside." << endl 
         << endl 
         << "     -xDX       Translates the polygons by DX (horiontally)." << endl 
         << "     -yDY       Translates the polygons by DY (vertically)." << endl 
         << "     -sSCALE    Scales the polygon by SCALE." << endl 
         << endl ;
         
  }

  
  return lExitCode ;
}


