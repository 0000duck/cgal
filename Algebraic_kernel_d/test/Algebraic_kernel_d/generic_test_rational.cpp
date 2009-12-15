// Copyright (c) 2009 Inria Lorraine (France). All rights reserved.
// 
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author: Luis Pe�aranda <luis.penaranda@loria.fr>

#include <CGAL/Algebraic_kernel_d_1_RS_Gmpq.h>
#include "include/CGAL/_test_algebraic_kernel_1.h"

typedef CGAL::Algebraic_kernel_d_1_RS_Gmpq              AK;

int main(){
        AK rational_ak;
        CGAL::test_algebraic_kernel_1<AK>(rational_ak);
        return 0;
}
