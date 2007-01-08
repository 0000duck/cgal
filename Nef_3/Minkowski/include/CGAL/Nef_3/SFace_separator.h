#ifndef CGAL_NEF3_SFACE_SEPARATOR_H
#define CGAL_NEF3_SFACE_SEPARATOR_H

#include <CGAL/Nef_3/SNC_decorator.h>
#include <CGAL/Nef_3/SNC_intersection.h>
#include <CGAL/Nef_S2/SM_walls.h>

CGAL_BEGIN_NAMESPACE

template<typename Nef_>
class SFace_separator : public Modifier_base<typename Nef_::SNC_structure> {
  
  typedef Nef_                                   Nef_polyhedron;
  typedef typename Nef_polyhedron::SNC_structure SNC_structure;
  typedef CGAL::SNC_decorator<SNC_structure>     Base;
  typedef CGAL::SNC_point_locator<Base>          SNC_point_locator;
  typedef CGAL::SNC_intersection<SNC_structure>  SNC_intersection;
  typedef CGAL::SNC_constructor<SNC_structure>   SNC_constructor;

  typedef typename SNC_structure::Sphere_map     Sphere_map;
  typedef CGAL::SM_decorator<Sphere_map>         SM_decorator;  
  typedef CGAL::SM_point_locator<SM_decorator>   SM_point_locator; 
  typedef CGAL::SM_walls<Sphere_map>             SM_walls;

  typedef typename Base::SHalfedge_handle        SHalfedge_handle;
  typedef typename Base::SFace_handle            SFace_handle;

  typedef typename Base::SFace_iterator          SFace_iterator;
  typedef typename Base::SFace_cycle_iterator    SFace_cycle_iterator;

 public:
  SFace_separator() {}

  void operator()(SNC_structure& snc) {

    //    SNC_structure* sncp(sncpl.sncp);
    //    SNC_point_locator* pl(sncpl.pl);

    SFace_iterator sf;
    CGAL_forall_sfaces(sf, snc) {
      if(!sf->mark() ||
	 sf->sface_cycles_begin() == 
	 sf->sface_cycles_end()) continue;

      SM_decorator SD(&*sf->center_vertex());
      SFace_cycle_iterator sfci = ++sf->sface_cycles_begin();
      while(sfci != sf->sface_cycles_end()) {
	SFace_handle sf_new = SD.new_sface();
	sf_new->mark() = sf->mark();
	sf_new->volume() = sf->volume();
	if(!sfci.is_shalfedge())
	  CGAL_assertion("not implemented, yet");
	SHalfedge_handle se = sfci;
	SD.unlink_as_face_cycle(se);
	SD.link_as_face_cycle(se,sf_new);
	sfci = ++sf->sface_cycles_begin();
      }
    }

  }
};

CGAL_END_NAMESPACE
#endif //CGAL_NEF3_SFACE_SEPARATOR_H
