#ifndef CGAL_ARRANGEMENT_SPHERES_3_FEATURE_H
#define CGAL_ARRANGEMENT_SPHERES_3_FEATURE_H

#include <CGAL/Tools/Coordinate_index.h>
#include <CGAL/Arrangement_of_spheres_3/Sphere_key.h>
#include <CGAL/basic.h>

struct Combinatorial_curve{
  typedef ::Coordinate_index Coordinate_index;

  /* for rules
     Inside means below a horizontal arc and to the left of a vertical one
  */
  enum PART_BITS {L_BIT=2, R_BIT=4, T_BIT=8, B_BIT=16, ARC_BIT=32, /*INF_BIT=32,*/ IN_BIT=1};
  enum LOCATION_BITS {lL_BIT=2, lR_BIT=4, lT_BIT=8, lB_BIT=16, lIN_BIT=1, lOUT_BIT=32};
  enum Part {INVALID=0, L_RULE=L_BIT, R_RULE=R_BIT, T_RULE=T_BIT, B_RULE=B_BIT,
	     LB_ARC=L_BIT|B_BIT|ARC_BIT, 
	     LT_ARC=L_BIT|T_BIT|ARC_BIT,
	     RT_ARC=R_BIT|T_BIT|ARC_BIT, 
	     RB_ARC=R_BIT|B_BIT|ARC_BIT,
	     SPECIAL= R_BIT | L_BIT | B_BIT | T_BIT
	     /*, 
	     L_INF=L_BIT|INF_BIT,
	     R_INF=R_BIT|INF_BIT,
	     T_INF=T_BIT|INF_BIT, 
	     B_INF=B_BIT|INF_BIT*/};

  typedef Sphere_key Key;

  static Combinatorial_curve make_special(Key i) {
    return Combinatorial_curve(i, SPECIAL);
  }

  Combinatorial_curve(int i, Part pt): index_(i), pt_(pt){
    CGAL_precondition(i>=0);
    CGAL_precondition(is_finite());
  }
  Combinatorial_curve(Key i, Part pt): index_(i), pt_(pt){
    //CGAL_precondition(is_finite());
  }
  /*explicit Combinatorial_curve(Part pt): pt_(pt){
    if (pt_== T_BIT || pt_ == R_BIT) {
      index_= Key(Key::TR);
    } else {
      CGAL_assertion(pt_ == B_BIT || pt_ == L_BIT);
      index_= Key(Key::BL);
    }
    CGAL_precondition(!is_finite());
    }*/
  Combinatorial_curve():pt_(INVALID){}
  bool is_valid() const ;

  const Combinatorial_curve other_side() const;
  /*bool is_special() const {
    return pt_== SPECIAL;
    }*/

  bool is_inside() const {
    return pt_&IN_BIT;
  }
  void set_is_inside(bool tf) {
    if (tf) pt_= pt_| IN_BIT;
    else pt_= pt_ & (~IN_BIT);
  }

  const Combinatorial_curve strip_inside() const ;

  bool is_top() const {
    return pt_ & T_BIT;
  }
  bool is_right() const {
    return pt_ & R_BIT;
  }
  bool is_bottom() const {
    return pt_ & B_BIT;
  }
  bool is_left() const {
    return pt_ & L_BIT;
  }
  /*int index() const {
    return index_.to_index();
    }*/
  const Key& key() const {
    return index_;
  }
  void set_key(Key k);


  Coordinate_index
  constant_coordinate() const ;

  bool is_same_part(const Combinatorial_curve &o) const;

  bool can_intersect(int t) const;

  int quadrant() const;

  const Part part() const {
    //bool dont_use_part;
    return static_cast<Part>(pt_);
  }

  void flip_rule(Key k);

  bool is_rule() const {
    return ! is_arc();
  }
  bool is_negative() const {
    return pt_&L_BIT || pt_&B_BIT;
  }
  bool is_finite() const {
    return index_.is_input(); //! (pt_ & INF_BIT);
  }
  bool is_arc() const ;
  bool is_vertical() const ;

  bool is_same_side(Combinatorial_curve o) const ;

  static int rule_direction(const Combinatorial_curve &a,
			    const Combinatorial_curve &b) ;
  
  bool operator==(const Combinatorial_curve &o) const {
    return index_== o.index_ && pt_== o.pt_;
  }
  bool operator!=(const Combinatorial_curve &o) const {
    return index_!= o.index_ || pt_!= o.pt_;
  }
  bool operator<(const Combinatorial_curve &o) const {
    CGAL_precondition(is_valid() && o.is_valid());
    if (index_ < o.index_) return true;
    else if (index_ > o.index_) return false;
    else return pt_ < o.pt_;
  }
  bool operator>(const Combinatorial_curve &o) const {
    CGAL_precondition(is_valid() && o.is_valid());
    if (index_ > o.index_) return true;
    else if (index_ < o.index_) return false;
    else return pt_ > o.pt_;
  }
  std::ostream &write(std::ostream&out) const;

  bool is_compatible_location(int i) const;

  bool can_intersect(const Combinatorial_curve &o) const;

  Coordinate_index is_weakly_incompatible(int i) const ;

  int rule_index() const;
  int arc_index() const;

  static Combinatorial_curve make_rule(Key k, int ruleindex);

  static const char *to_string(int pt);

  const char *to_string() const {
    return to_string(pt_);
  }

  bool is_outward_rule() const {
    CGAL_precondition(is_rule());
    switch (rule_index()) {
    case 0:
    case 3:
      return !is_inside();
    case 1:
    case 2:
      return is_inside();
    default:
      CGAL_assertion(0);
      return false;
    }
  }

private:

  Combinatorial_curve(Key i, int pt, bool): index_(i), pt_(pt){
  }

  Key index_;
  int pt_;
};

inline std::ostream &operator<<(std::ostream &out, Combinatorial_curve f) {
  return f.write(out);
}

#endif
