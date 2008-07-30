#ifndef CGAL_INTERPOLATE_H
#define CGAL_INTERPOLATE_H

CGAL_BEGIN_NAMESPACE
namespace CGALi {

// Class for interpolation of univariate or multivariate polynomials. 
// The template argument must be a model of concept Polynomial_d
// 
//
template <class Polynomial_d_>
class Interpolator{
  typedef CGAL::Polynomial_traits_d<Polynomial_d_> PT;
    
public:
  typedef typename PT::Polynomial_d Polynomial_d; 
  typedef typename PT::Coefficient Coeff; 
  typedef typename PT::Innermost_coefficient IC;

private: 
  typedef typename CGAL::Coercion_traits<Coeff,IC>::Cast IC2Coeff;
  typedef typename PT::Construct_polynomial Construct;

  std::vector<IC> xvals; 
  std::vector<Coeff> yvals; 
  std::vector<Coeff> b; 
    
  bool valid; 
  Polynomial_d interpolant; 


  Coeff eval_newton(int n, IC z)
  {
    Coeff p(b[n]);
    for (int i = n-1; i >=0; i--){
      Coeff tmp(IC2Coeff()((z - xvals[i])));
      p = p * tmp + b[i];
    }
    return p;
  }


  Polynomial_d eval_newton_poly(int n)
  {    
    CGAL_precondition(n >=0);
    Polynomial_d p(Construct()(b[n]));
    for (int i = n-1; i >=0; i--) {
      Polynomial_d tmp = Construct()(IC2Coeff()(-xvals[i]),Coeff(1));
      p = (p * tmp) + b[i];
    }
    return p;
  }
    
public:
  Interpolator(){};
  
  // constructor from an InputIterator range with value type std::pair<IC,Coeff> 
  template<class InputIterator>
  Interpolator(const InputIterator& begin, const InputIterator& end){
    for(InputIterator it = begin; it != end; it++){
      add_interpolation_point(*it);
    }
  }
    
  /*
    Interpolator(std::vector<IC> xvals_, std::vector<Coeff> yvals_)
    : valid(false) {
    CGAL_precondition(xvals_.size() != 0);
    CGAL_precondition(xvals_.size() == yvals_.size());
    for(unsigned i = 0; i < xvals_.size();  i++){
    add_interpolation_point(xvals_[i],yvals_[i]);
    }
    }
  */
    
  // void add_interpolation_point(std::pair<IC,Coeff> point){
  //    add_interpolation_point(point.first, point.second);
  // }
    
  // void add_interpolation_point(IC xval, Coeff yval){
  void add_interpolation_point(std::pair<IC,Coeff> point){
    valid = false;
//        CGAL_precondition(0 == std::count(xval, xvals.begin(), yvals.end()));
    xvals.push_back(point.first);
    yvals.push_back(point.second);
        
    Coeff num, den;
    int k = xvals.size() - 1; 
    if(k == 0){
      b.push_back(yvals[0]);
    }else{
      num = yvals[k] - eval_newton(k-1,xvals[k]);    
      den = Coeff(1);            
      for (int j = 0; j < k; j++) {
        // (k-j) if xvals's are sequential
        den *= (xvals[k] - xvals[j]);
      }
      b.push_back(num / den);
    }
  }
    
  Polynomial_d get_interpolant(){ 
    if (xvals.size() == 0) return Polynomial_d(0);
    // TODO: compute new interpolant from old interpolant ?
    if(!valid)
      interpolant = eval_newton_poly(xvals.size()-1);
    return interpolant; 
  }
    
};

} // namespace CGALi
CGAL_END_NAMESPACE

#endif // CGAL_INTERPOLATE_H 
