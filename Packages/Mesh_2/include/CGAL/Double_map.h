#ifndef CGAL_DOUBLE_MAP_H
#define CGAL_DOUBLE_MAP_H

CGAL_BEGIN_NAMESPACE

#include <map>
#include <utility>
#include <algorithm>
#include <CGAL/assertions.h>

template <class _Key, class _Data, class _Direct_order = std::less<_Key>, 
	  class _Reverse_order = std::less<_Data> >
class Double_map
{
public:
  typedef _Key Key;
  typedef _Data Data;
  typedef _Direct_order Direct_order;
  typedef _Reverse_order Reverse_order;
  
  typedef std::map <Key, Data, Direct_order> Direct_func;
  typedef std::multimap <Data, Key, Reverse_order> Reverse_func;

  typedef std::pair<Key, Data> Direct_entry;
  typedef std::pair<Data, Key> Reverse_entry;

  typedef typename Direct_func::size_type size_type;

  typedef typename Reverse_func::iterator reverse_iterator;
  typedef typename Direct_func::iterator direct_iterator;
  typedef reverse_iterator iterator;

private:
  // Private members
  Direct_func direct_func;
  Reverse_func reverse_func;

public :
  // The default constructor
  Double_map () {}

  // Queries
  bool empty() const
  {
    CGAL_assertion(is_valid());
    return(direct_func.empty());
  }

  size_type size() const
  {
    CGAL_assertion(is_valid());
    return direct_func.size();
  }

  bool is_valid() const
  {
    return(direct_func.size()==reverse_func.size());
  }
  
  void clear()
    {
      direct_func.clear();
      reverse_func.clear();
    }

  // Assignation
  bool insert(const Key& k, const Data& d)
    {
      std::pair<direct_iterator, bool> 
	direct_result = direct_func.insert(Direct_entry(k, d));

      if (direct_result.second != true)
	return false;

      reverse_func.insert(Reverse_entry(d, k));

      CGAL_assertion(is_valid());
      return(true);
    }

  void erase(Key& k);

  // Access
  reverse_iterator front()
    {
      return(reverse_func.begin());
    }

  void pop_front()
    {
      reverse_iterator rit = reverse_func.begin();
      direct_iterator pos = direct_func.find(rit->second);
      assert(pos != direct_func.end());
      direct_func.erase(pos);
      reverse_func.erase(rit);
    }

  class Second_is {
    Key k;
  public:
    Second_is(const Key& _k): k(_k) {};
    bool operator()(const Reverse_entry& p) const 
      {
	return p.second == k;
      }
  };

  void dump_direct_func(std::ostream& out)
    {
      for(typename Direct_func::iterator it = direct_func.begin();
	  it != direct_func.end();
	  ++it)
	std::cerr << it->second << " " 
		  << "("
		  << it->first->vertex(0)->point() << ", "
		  << it->first->vertex(1)->point() << ", "
		  << it->first->vertex(2)->point()
		  << ")" << std::endl;
    }

  void dump_reverse_func(std::ostream& out)
    {
      for(typename Reverse_func::iterator it = reverse_func.begin();
      it != reverse_func.end();
	  ++it)
	std::cerr << it->first << " " 
		  << "("
		  << it->second->vertex(0)->point() << ", "
		  << it->second->vertex(1)->point() << ", "
		  << it->second->vertex(2)->point()
		  << ")" << std::endl;
    }
};

template <class _Key, class _Data, class _Direct_order, 
  class _Reverse_order>
void
Double_map<_Key, _Data, _Direct_order, _Reverse_order>::
erase(Key& k)
{
  direct_iterator pos = direct_func.find(k);
  if (pos == direct_func.end())
    return;
  else
    {
      const Data& d = pos->second;

      reverse_iterator lb = reverse_func.lower_bound(d);
      reverse_iterator ub = reverse_func.upper_bound(d);

      direct_func.erase(pos);
      reverse_func.erase(std::find_if(lb, ub, Second_is(k)));
    }
}

CGAL_END_NAMESPACE

#endif
