#include <CGAL/basic.h>
#ifndef CGAL_USE_QT
  #include <iostream>
  int main(int, char**)
  {
    std::cout << "This platform does not have QT installed.";
    std::cout << std::endl;
    return 0;
  }
#else
  #include <iostream>
  #include <qglobal.h>
  int main(char*, char**)
  {
    std::cout << "QT_VERSION= " << QT_VERSION << std::endl
	      << "QT_VERSION_STR= " << QT_VERSION_STR << std::endl;
    return 0;
  }
#endif
