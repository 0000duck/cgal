#ifndef CGAL_KDS_EVENT_BASE_H
#define CGAL_KDS_EVENT_BASE_H
#include <CGAL/Kinetic/basic.h>
CGAL_KINETIC_BEGIN_NAMESPACE

template <class KDS_ptr>
class Event_base {
public:
	typedef KDS_ptr KDS_handle;
	Event_base(KDS_ptr pt): kds_(pt){}
	
	KDS_handle kds() const {
		return kds_;
	}
	KDS_handle kds() {
		return kds_;
	}
	template <class Key>
	CGAL::Comparison_result perturb_concurrent(Key a,
						   Key b) const {
		return CGAL::compare(a,b);
	}	
	template <class Key>
	bool merge_concurrent(Key a,
			      Key b) {
		return false;
	}
protected:
	KDS_handle kds_;
};

CGAL_KINETIC_END_NAMESPACE
#endif
